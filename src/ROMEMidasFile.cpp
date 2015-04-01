// Author: Matthias Schneebeli
//////////////////////////////////////////////////////////////////////////
//
//  ROMEMidasFile
//
//  Interface to the Midas file.
//
//////////////////////////////////////////////////////////////////////////

#define _FILE_OFFSET_BITS 64

#include <RConfig.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#if defined( R__VISUAL_CPLUSPLUS )
#   include <io.h>
#   include <SYS\STAT.H>
#endif
#if defined( R__UNIX )
#   include <unistd.h>
#endif
#include <fcntl.h>

#include "ROMEPrint.h"
#include "ROMEMidasFile.h"

#if defined( R__VISUAL_CPLUSPLUS )
#   define O_RDONLY_BINARY O_RDONLY | O_BINARY
#else
#   define O_RDONLY_BINARY O_RDONLY
#endif

//______________________________________________________________________________
ROMEMidasFile::ROMEMidasFile()
:fType(ROMEMidasFile::NONE)
,fFileName("")
,fPlainFileHandle(0)
,fGZFileHandle()
#ifdef HAVE_XZ
,fXZFileHandle(0)
,fXZStream(0)
,fXZInputBuffer(0)
#endif
#ifdef HAVE_BZ2
,fBZ2FileHandle(0)
,fBZ2File(0)
#endif
{
}

//______________________________________________________________________________
Bool_t ROMEMidasFile::Open(const char* const dir, const char* const runStr)
{
   ROMEString filename;
   ROMEString tmpname;

   Int_t retplain = -1;
   Int_t retgz    = -1;
#ifdef HAVE_XZ
   Int_t retxz    = -1;
#endif
#ifdef HAVE_BZ2
   Int_t retbz2   = -1;
#endif

   Close();

   filename.SetFormatted("%srun%s.mid", dir, runStr);

   // Plain
   tmpname.SetFormatted("%s", filename.Data());

   fPlainFileHandle = open(tmpname.Data(), O_RDONLY_BINARY);
   retplain = errno;
   if (fPlainFileHandle != -1) {
      fType = ROMEMidasFile::PLAIN;
      fFileName = tmpname;
   }

   // GZ
   if (fType == ROMEMidasFile::NONE) {
      tmpname.SetFormatted("%s.gz", filename.Data());
      fGZFileHandle = gzopen(tmpname.Data(), "rb");
      retgz = errno;
      if (fGZFileHandle) {
         fType = ROMEMidasFile::GZ;
         fFileName = tmpname;
      }
   }

   // XZ
#ifdef HAVE_XZ
   if (fType == ROMEMidasFile::NONE) {
      tmpname.SetFormatted("%s.xz", filename.Data());
      fXZFileHandle = fopen(tmpname.Data(), "rb");
      retxz = errno;
      if (fXZFileHandle) {
         fXZStream = new lzma_stream;
         lzma_stream lzma_init = LZMA_STREAM_INIT;
         *fXZStream = lzma_init;
#if 0 /* check usage */
         UInt_t level = 9;
         ULong64_t memlimit = lzma_easy_decoder_memusage(level);
#else /* set by hand */
         ULong64_t memlimit = (128 << 20);
#endif
#if 0
         lzma_ret ret = lzma_stream_decoder(fXZStream, memlimit, LZMA_TELL_UNSUPPORTED_CHECK | LZMA_CONCATENATED);
#else
         lzma_ret ret = lzma_stream_decoder(fXZStream, memlimit, 0);
#endif
         if (ret == LZMA_OK) {
            fType = ROMEMidasFile::XZ;
            fFileName = tmpname;
            fXZInputBuffer = new uint8_t[kXZBufferSize];
         } else {
            switch (ret) {
            case LZMA_MEM_ERROR:
               ROMEPrint::Error("XZ: Memory allocation failed\n");
               break;
            case LZMA_OPTIONS_ERROR:
               ROMEPrint::Error("XZ: Unsupported decompressor flags\n");
               break;
            default:
               ROMEPrint::Error("XZ: Unknown error, possibly a bug\n");
               break;
            }
            fclose(fXZFileHandle);
         }
      }
   }
#endif

   // BZ2
#ifdef HAVE_BZ2
   if (fType == ROMEMidasFile::NONE) {
      tmpname.SetFormatted("%s.bz2", filename.Data());
      fBZ2FileHandle = fopen(tmpname.Data(), "rb");
      retbz2 = errno;
      Int_t bzerror;
      if (fBZ2FileHandle) {
         fBZ2File = BZ2_bzReadOpen(&bzerror, fBZ2FileHandle, 0/* verbose 0-4 */, 0, 0, 0);
         if (bzerror == BZ_OK) {
            fType = ROMEMidasFile::BZ2;
            fFileName = tmpname;
         } else {
            BZ2_bzReadClose(&bzerror, fBZ2File);
            fBZ2File = 0;
            ROMEPrint::Error("Failed to read a bzip2 midas file.\n");
            fclose(fBZ2FileHandle);
         }
      }
   }
#endif

   if (fType == ROMEMidasFile::NONE) {
      ROMEPrint::Error("Failed to open input file\n");
      if (retplain != -1) ROMEPrint::Error("   '%s'     : %s \n", filename.Data(), strerror(retplain));
      if (retgz    != -1) ROMEPrint::Error("   '%s.gz'  : %s \n", filename.Data(), strerror(retgz));
#ifdef HAVE_XZ
      if (retxz    != -1) ROMEPrint::Error("   '%s.xz'  : %s \n", filename.Data(), strerror(retxz));
#endif
#ifdef HAVE_BZ2
      if (retbz2   != -1) ROMEPrint::Error("   '%s.bz2' : %s \n", filename.Data(), strerror(retbz2));
#endif
      return kFALSE;
   }

   return kTRUE;
}

//______________________________________________________________________________
void ROMEMidasFile::Close()
{
   switch (fType) {
   case ROMEMidasFile::PLAIN:
      close(fPlainFileHandle);
      break;
   case ROMEMidasFile::GZ:
      gzclose(fGZFileHandle);
      break;
#ifdef HAVE_XZ
   case ROMEMidasFile::XZ:
      lzma_end(fXZStream);
      fclose(fXZFileHandle);
      fXZFileHandle = 0;
      SafeDelete(fXZStream);
      SafeDeleteArray(fXZInputBuffer);
      break;
#endif
#ifdef HAVE_BZ2
   case ROMEMidasFile::BZ2:
      {
         Int_t bzerror;
         BZ2_bzReadClose(&bzerror, fBZ2File);
         fBZ2File = 0;
         fclose(fBZ2FileHandle);
         fBZ2FileHandle = 0;
      }
      break;
#endif
   default:
      break;
   }

   fType = ROMEMidasFile::NONE;
   fFileName = "";
}

//______________________________________________________________________________
ROMEMidasFile::~ROMEMidasFile()
{
   Close();
}

//______________________________________________________________________________
off_t ROMEMidasFile::CurrentPosition() const
{
   off_t pos = -1;
   switch (fType) {
   case ROMEMidasFile::PLAIN:
      pos = lseek(fPlainFileHandle, 0L, SEEK_CUR);
      break;
   case ROMEMidasFile::GZ:
      pos = gzseek(fGZFileHandle, 0L, SEEK_CUR);
      break;
#ifdef HAVE_XZ
   case ROMEMidasFile::XZ:
      // not implemented
      break;
#endif
#ifdef HAVE_BZ2
   case ROMEMidasFile::BZ2:
      // not implemented
      break;
#endif
   default:
      break;
   }

   return pos;
}

//______________________________________________________________________________
ssize_t ROMEMidasFile::Read(void *buf, size_t size)
{
   ssize_t n = 0;

   switch (fType) {
   case ROMEMidasFile::PLAIN:
      n = read(fPlainFileHandle, buf, size);
      break;
   case ROMEMidasFile::GZ:
      n = gzread(fGZFileHandle, buf, size);
      break;
#ifdef HAVE_XZ
   case ROMEMidasFile::XZ:
      {
         lzma_action action = LZMA_RUN;
         lzma_ret ret = LZMA_OK;

         union {
            void    *unpnt;
            uint8_t *ucpnt;
         };
         unpnt = buf;
         fXZStream->next_out = ucpnt;
         fXZStream->avail_out = size;

         while (1) {
            if (ferror(fXZFileHandle)) {
               return 0;
            }

            if (!fXZStream->avail_in && !feof(fXZFileHandle)) {
               fXZStream->avail_in = fread(fXZInputBuffer, 1, kXZBufferSize, fXZFileHandle);
               fXZStream->next_in = fXZInputBuffer;
               if (ferror(fXZFileHandle)) {
                  ROMEPrint::Error("Read error\n");
                  return 0;
               } else if (feof(fXZFileHandle)) {
                  action = LZMA_FINISH;
               }
            }

            ret = lzma_code(fXZStream, action);
            if (ret != LZMA_OK) {
               if (ret != LZMA_STREAM_END) {
                  switch (ret) {
                  case LZMA_MEM_ERROR:
                     ROMEPrint::Error("Memory allocation failed\n");
                     break;
                  case LZMA_FORMAT_ERROR:
                     // .xz magic bytes weren't found.
                     ROMEPrint::Error("The input is not in the .xz format\n");
                     break;
                  case LZMA_OPTIONS_ERROR:
                     ROMEPrint::Error("Unsupported compression options\n");
                     break;
                  case LZMA_DATA_ERROR:
                     ROMEPrint::Error("Compressed file is corrupt\n");
                     break;
                  case LZMA_BUF_ERROR:
                     ROMEPrint::Error("Compressed file is truncated or otherwise corrupt\n");
                     break;
                  default:
                     ROMEPrint::Error("Unknown error, possibly a bug\n");
                     break;
                  }
                  return 0;
               } else {
                  // read short than requested
                  n = size - fXZStream->avail_out;
               }
            } else {
               if (feof(fXZFileHandle) &&
                   fXZStream->avail_in == 0 && fXZStream->avail_out > 0) {
                  ROMEPrint::Error("Unexpected end of file\n");
                  return 0;
               } else if (fXZStream->avail_out == 0) {
                  // ok
                  n = size;
               }
            }
            if (n == size) {
               break;
            }
         }
      }
      break;
#endif
#ifdef HAVE_BZ2
   case ROMEMidasFile::BZ2:
      {
         Int_t bzerror;
         n = BZ2_bzRead(&bzerror, fBZ2File, buf, size);
         if (bzerror != BZ_OK) {
            switch (bzerror) {
            case BZ_MEM_ERROR:
               ROMEPrint::Error("bzip2: insufficient memory is available\n");
               break;
            case BZ_OUTBUFF_FULL:
               ROMEPrint::Error("bzip2: the size of the compressed data exceeds size of the out buffer\n");
               break;
            case BZ_DATA_ERROR:
               ROMEPrint::Error("bzip2: a data integrity error was detected in the compressed data\n");
               break;
            case BZ_DATA_ERROR_MAGIC:
               ROMEPrint::Error("bzip2: the compressed data doesn't begin with the right magic bytes\n");
               break;
            case BZ_UNEXPECTED_EOF:
               ROMEPrint::Error("bzip2: the compressed data ends unexpectedly\n");
               break;
            case BZ_STREAM_END:
               {
                  // end of stream, but not necessarily end of
                  // file: get unused bits, close stream, and
                  // open again with the saved unused bits
                  void *punused;
                  int   nunused;
                  char  unused[BZ_MAX_UNUSED];
                  BZ2_bzReadGetUnused(&bzerror, fBZ2File, &punused, &nunused);
                  if (bzerror == BZ_OK && (nunused > 0 || !feof(fBZ2FileHandle))) {
                     if (nunused > 0) {
                        memcpy(unused, punused, nunused);
                     }
                     BZ2_bzReadClose(&bzerror, fBZ2File);
                     fBZ2File = BZ2_bzReadOpen(&bzerror, fBZ2FileHandle, 0, 0, unused, nunused);
                     union {
                        void *vbuf;
                        char *cbuf;
                     };
                     vbuf = buf;
                     n += Read(cbuf + n, size - n);
                  }
               }
               break;
            default:
               ROMEPrint::Error("bzip2: unknown error\n");
               break;
            }
         }
      }
      break;
#endif
   default:
      break;
   }

   return n;
}

//______________________________________________________________________________
off_t ROMEMidasFile::Seek(Long64_t pos) const
{
   off_t ret = -1;
   switch (fType) {
   case ROMEMidasFile::PLAIN:
      ret = lseek(fPlainFileHandle, static_cast<Long_t>(pos), SEEK_SET);
      break;
   case ROMEMidasFile::GZ:
      ret = gzseek(fGZFileHandle, static_cast<Long_t>(pos), SEEK_SET);
      break;
#ifdef HAVE_XZ
   case ROMEMidasFile::XZ:
      ROMEPrint::Error("Seek for xz midas files is not implemented. Please decompress the file.\n");
      gROME->GetApplication()->Terminate(1);
      break;
#endif
#ifdef HAVE_BZ2
   case ROMEMidasFile::BZ2:
      ROMEPrint::Error("Seek for bzip2 midas files is not implemented. Please decompress the file.\n");
      gROME->GetApplication()->Terminate(1);
      break;
#endif
   default:
      break;
   }

   return ret;
}

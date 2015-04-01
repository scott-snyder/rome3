/********************************************************************
  ROMEMidasFile.h, M. Schneebeli PSI

********************************************************************/
#ifndef ROMEMidasFile_H
#define ROMEMidasFile_H

#include <RConfig.h>
#include <Rtypes.h>
#include "ROMEString.h"

#include <zlib.h>
#ifdef HAVE_XZ
#  include <lzma.h>
#endif
#ifdef HAVE_BZ2
#  include <bzlib.h>
#endif

#ifdef HAVE_XZ
//   const Int_t kXZBufferSize = 65536;
#if BUFSIZ <= 1024
const Int_t kXZBufferSize = 8192;
#else
const Int_t kXZBufferSize = (BUFSIZ & ~7U);
#endif
#endif

class ROMEMidasFile {
enum MidasFileType {
   NONE,
   PLAIN,
   GZ,
   XZ,
   BZ2
};

protected:
   Int_t         fType;            //! Type of the file
   TString       fFileName;        //! Reading file name
   Seek_t        fPlainFileHandle; //! Handle to a un-gzipped Midas Inputfile
   gzFile        fGZFileHandle;    //! Handle to Midas gzipped Inputfile
#ifdef HAVE_XZ
   FILE*         fXZFileHandle;    //! Handle to Midas XZ Inputfile
   lzma_stream*  fXZStream;        //! Handle to Midas XZ stream
   uint8_t*      fXZInputBuffer;   //! Input buffer
#endif
#ifdef HAVE_BZ2
   FILE*         fBZ2FileHandle;    //! Handle to Midas bzip2 Inputfile
   BZFILE*       fBZ2File;          //! Handle to Midas bzip2 stream
#endif

private:
   ROMEMidasFile(const ROMEMidasFile &daq); // not implemented
   ROMEMidasFile &operator=(const ROMEMidasFile &rhs); // not implemented

public:
   ROMEMidasFile();
   virtual ~ROMEMidasFile();

   const TString &GetFileName() const { return fFileName; }
   Bool_t   Open(const char* const dir, const char* const runStr);
   void     Close();
   off_t    CurrentPosition() const;
   ssize_t  Read(void *buf, size_t size);
   off_t    Seek(Long64_t pos) const;
};

#endif   // ROMEMidasFile_H

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

#if defined ( HAVE_MIDAS ) && !defined ( __MAKECINT__ )
#   include "midas.h"
#else
#if defined( R__VISUAL_CPLUSPLUS )
typedef unsigned long   DWORD;
typedef int    BOOL;
#else
typedef UInt_t   DWORD;
typedef DWORD    BOOL;
#ifndef TRUE
   const BOOL TRUE = 1;
#endif
#ifndef FALSE
   const BOOL FALSE = 0;
#endif
#endif
typedef Int_t    HNDLE;
typedef Int_t    INT;
typedef UShort_t WORD;
const Int_t CM_SUCCESS = 1;
#   define BANK_FORMAT_32BIT   (1<<4)
#   define EVENTID_BOR      ((short) 0x8000)  /**< Begin-of-run      */
#   define EVENTID_EOR      ((short) 0x8001)  /**< End-of-run        */
#   define EVENTID_MESSAGE  ((short) 0x8002)  /**< Message events    */

typedef struct {
   Short_t event_id;
   Short_t trigger_mask;
   DWORD   serial_number;
   DWORD   time_stamp;
   DWORD   data_size;
} EVENT_HEADER;

typedef struct {
   DWORD   data_size;
   DWORD   flags;
} BANK_HEADER;

typedef struct {
   char    name[4];
   WORD    type;
   WORD    data_size;
} BANK;

typedef struct {
   char    name[4];
   DWORD   type;
   DWORD   data_size;
} BANK32;

#   define ALIGN8(x)  (((x)+7) & ~7)
//Data types Definition                         min      max
const UShort_t TID_BYTE      = 1;       //< unsigned byte         0       255
const UShort_t TID_SBYTE     = 2;       //< signed byte         -128      127
const UShort_t TID_CHAR      = 3;       //< single character      0       255
const UShort_t TID_WORD      = 4;       //< two bytes             0      65535
const UShort_t TID_SHORT     = 5;       //< signed word        -32768    32767I
const UShort_t TID_DWORD     = 6;       //< four bytes            0      2^32-1
const UShort_t TID_INT       = 7;       //< signed dword        -2^31    2^31-1
const UShort_t TID_BOOL      = 8;       //< four bytes bool       0        1
const UShort_t TID_FLOAT     = 9;       //< 4 Byte float format
const UShort_t TID_DOUBLE    = 10;      //< 8 Byte float format
const UShort_t TID_BITFIELD  = 11;      //< 32 Bits Bitfield      0  111... (32)
const UShort_t TID_STRING    = 12;      //< zero terminated string
const UShort_t TID_ARRAY     = 13;      //< array with unknown contents
const UShort_t TID_STRUCT    = 14;      //< structure with fixed length
const UShort_t TID_KEY       = 15;      //< key in online database
const UShort_t TID_LINK      = 16;      //< link in online database
const UShort_t TID_LAST      = 17;      //< end of TID list indicator

#ifndef MAX_EVENT_SIZE                      /* value can be set via Makefile      */
#  define MAX_EVENT_SIZE         0x400000   /**< maximum event size 4MB           */
#endif
#endif

#if defined( R__VISUAL_CPLUSPLUS )
#  ifdef MAX_EVENT_SIZE
#     undef  MAX_EVENT_SIZE
#  endif
#  define MAX_EVENT_SIZE         0xA00000   /**< maximum event size if fixed 10MB */
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

#if !defined HAVE_MIDAS && !defined ( __MAKECINT__ )
   BOOL bk_is32(void *event);
   INT  bk_find(BANK_HEADER* pbkh, const char *name, DWORD* bklen, DWORD* bktype,void *pdata);
#endif

#endif   // ROMEMidasFile_H

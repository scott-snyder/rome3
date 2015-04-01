#include <TRint.h>
#include <Riostream.h>
#include "ROMEMidasFile.h"

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

int main(int argc, char *argv[])
{
   ROMEMidasFile *mfile = new ROMEMidasFile();
   union {
      char         *rawData;
      EVENT_HEADER *pevent;
   };
   rawData = new char[MAX_EVENT_SIZE];

   // Open file
   if (!mfile->Open("../", "00001")) {
      cerr<<"Faile to open a MIDAS file"<<endl;
      return 1;
   }
   cout<<"Reading Midas-File ";
   cout<<mfile->GetFileName()<<endl;

   ssize_t n;
   bool readError;
  
   while (1) { 
      readError = false;
      n = mfile->Read(pevent, sizeof(EVENT_HEADER));

      if (n < static_cast<long>(sizeof(EVENT_HEADER))) {
         readError = true;
      } else {

         // byteswap pevent if necessary

         cout<<"SERIAL="<<pevent->serial_number<<", ID="<<pevent->event_id<<endl;

         n = 0;
         if (pevent->data_size <= 0) {
            readError = true;
         } else {
            n = mfile->Read(pevent + 1, pevent->data_size);
            if (n != static_cast<long>(pevent->data_size)) {
               readError = true;
            }
         }
      }

      if (pevent->event_id == EVENTID_EOR) {
         break;
      }

      // check input
      if (readError) {
         if (readError && n > 0) {
            cerr<<"Unexpected end of file"<<endl;
            break;
         } else {
            cerr<<"Read error"<<endl;
            break;
         }
      }
      UInt_t dsize = reinterpret_cast<BANK_HEADER*>(pevent + 1)->data_size;

      // byteswap dsize if necessary

      if (pevent->data_size < dsize) {
         continue;
      }

      // Add code to decode your MIDAS banks
   }

   delete mfile;
   delete [] rawData;

   return 0;
}

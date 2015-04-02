#include <TRint.h>
#include <Riostream.h>
#include "ROMEMidasFile.h"

using namespace std;

int main(int argc, char *argv[])
{
   ROMEMidasFile *mfile = new ROMEMidasFile();
   union {
      char         *rawData;
      EVENT_HEADER *pevent;
      BANK_HEADER  *pbank;
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

      // Decode MIDAS banks
      DWORD bktype;
      DWORD ADC0Length  = 0;
      union {
         WORD* ADC0Pointer;
         void* ADC0VoidPointer;
      };
      ADC0Pointer = 0;
      if (pevent->event_id == 1) {
         pevent++;
         if (bk_find(pbank, "ADC0", &ADC0Length, &bktype, &ADC0VoidPointer)) {

            // byteswap ADC0 data if necessary
            // you may use bk_swap implemented in $MIDASSYS/src/midas.c

            unsigned int iCh;
            for (iCh = 0; iCh < ADC0Length; iCh++) {
               cout<<"ADC"<<iCh<<"="<<ADC0Pointer[iCh]<<endl;
            }
         }
      }

   }

   delete mfile;

   return 0;
}

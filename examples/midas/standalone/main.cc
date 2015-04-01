#include <TRint.h>
#include <Riostream.h>
#include "generated/MIDAnalyzer.h"
#include "generated/MIDMidasDAQ.h"

using namespace std;

int main(int argc, char *argv[])
{
   // Initialization of ROME objects
   new TRint("App", &argc, argv, 0, 0, kTRUE);
   new MIDAnalyzer(0, true, true, true, 0);
   gAnalyzer->SetOffline();
   gAnalyzer->SetIOType(ROMEAnalyzer::kRunNumberBased);
   gAnalyzer->SetInputDir("../");
   MIDMidasDAQ *midas = new MIDMidasDAQ();
   midas->InitDAQ();

   // Start run #1
   gAnalyzer->SetCurrentRunNumber(1);
   if (!midas->BeginOfRunDAQ()) {
      // error
      midas->TerminateDAQ();
      return 1;
   }

   // Event loop
   int ev;
   gAnalyzer->SetCurrentEventNumber(0);
   for (ev = 0; !midas->isTerminate(); ev++) {
      if (midas->isEndOfRun()) {
         break;
      }

      // Read an event
      if (!midas->EventDAQ(ev)) {
         break;
      }

      // Example code to print values,
      int i;
      cout << "\nEvent : " << gAnalyzer->GetCurrentEventNumber() << endl;
      cout << "ADC0" << endl;
      for (i = 0; i < midas->GetADC0BankEntries(); i++) {
         cout << midas->GetADC0BankAt(i) << endl;
      }
      cout << "TDC0" << endl;
      for (i = 0; i < midas->GetTDC0BankEntries(); i++) {
         cout << midas->GetTDC0BankAt(i) << endl;
      }
      cout << "SCLR" << endl;
      for (i = 0; i < midas->GetSCLRBankEntries(); i++) {
         cout << midas->GetSCLRBankAt(i) << endl;
      }
      cout << endl;
   }

   midas->EndOfRunDAQ();
   midas->TerminateDAQ();

   return 0;
}

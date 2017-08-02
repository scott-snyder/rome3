////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// MIDTPrintValues                                                            //
//                                                                            //
// Begin_Html <!--
/*-->

<!--*/
// --> End_Html
//                                                                            //
//                                                                            //
// Please note: The following information is only correct after executing     //
// the ROMEBuilder.                                                           //
//                                                                            //
// This task accesses the following folders :                                 //
//     Data                                                                   //
//     ODB                                                                    //
//                                                                            //
//                                                                            //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

/* Generated header file containing necessary includes                        */
#include "generated/MIDTPrintValuesGeneratedIncludes.h"

////////////////////////////////////////////////////////////////////////////////
/*  This header was generated by ROMEBuilder. Manual changes above the        *
 * following line will be lost next time ROMEBuilder is executed.             */
/////////////////////////////////////----///////////////////////////////////////

#include "TSystem.h"
#include "generated/MIDAnalyzer.h"
#include "generated/MIDGlobalSteering.h"
#include "tasks/MIDTPrintValues.h"
#include "MIDODB.h"
#include "ROMEiostream.h"
#include "ROMEODBOfflineDataBase.h"

// uncomment if you want to include headers of all folders
//#include "MIDAllFolders.h"


ClassImp(MIDTPrintValues)

using namespace std;

void MIDTPrintValues::Init()
{
}

void MIDTPrintValues::BeginOfRun()
{
   cout << "Run Number : " << gAnalyzer->GetODB()->GetRunNumber() << endl;
   cout << "Start Time : " << gAnalyzer->GetODB()->GetStartTime().Data() << endl;

   // copy ODB to ROOT files.
   // this can be read in "rome" DAQ mode.
   if (gAnalyzer->isOffline()) {
      Int_t nTree = gAnalyzer->GetTreeObjectEntries();
      Int_t iTree;
      ROMETree *tree;
      ROMEODBOfflineDataBase *db = static_cast<ROMEODBOfflineDataBase*>(gAnalyzer->GetDataBase("odb"));
      if (db && gAnalyzer->IsActiveDAQ("midas")) {
         for (iTree = 0; iTree < nTree; iTree++) {
            tree = gAnalyzer->GetTreeObjectAt(iTree);
            if (tree->isWrite()) {
               db->WriteToTFile(tree->GetFile());
            }
         }
      }
   }
}

void MIDTPrintValues::Event()
{

   gAnalyzer->GetData()->SetADCSize(gAnalyzer->GetGSP()->GetNChannels());
   gAnalyzer->GetData()->SetTDCSize(gAnalyzer->GetGSP()->GetNChannels());

   int i;
   cout << "\nEvent : " << gAnalyzer->GetCurrentEventNumber() << endl;
   cout << "TaskTestValue : " << gAnalyzer->GetPrintValuesTask()->GetSP()->GetTaskTestValue() << endl;
   cout << "TestValue : " << gAnalyzer->GetGSP()->GetTestValue() << endl;
   cout << "ADC0" << endl;
   for (i=0;i<gAnalyzer->GetMidasDAQ()->GetADC0BankEntries();i++) {
      if (i < gAnalyzer->GetData()->GetADCSize()) {
         gAnalyzer->GetData()->SetADCAt(i, gAnalyzer->GetMidasDAQ()->GetADC0BankAt(i));
      }
      cout << gAnalyzer->GetMidasDAQ()->GetADC0BankAt(i) << endl;
   }
   cout << "TDC0" << endl;
   for (i=0;i<gAnalyzer->GetMidasDAQ()->GetTDC0BankEntries();i++) {
      if (i < gAnalyzer->GetData()->GetTDCSize()) {
         gAnalyzer->GetData()->SetTDCAt(i, gAnalyzer->GetMidasDAQ()->GetTDC0BankAt(i));
      }
      cout << gAnalyzer->GetMidasDAQ()->GetTDC0BankAt(i) << endl;
   }
   cout << endl;

   if (gROME->isOffline()) {
      gSystem->Sleep(10);
   }
}

void MIDTPrintValues::EndOfRun()
{
   /* */

   // Example of alarm
   Alarm("TestAlarm", "Alarm from ROME analyzer", "Alarm", "", kATProgram);

   // Example of ODB access.
   TString path;
   TString msgOrg;
   Int_t iDB;
   ROMEStr2DArray* value = new ROMEStr2DArray(1, 1);
   for(iDB = 0; iDB < gAnalyzer->GetNumberOfDataBases(); iDB++){
      if(!strcmp(gAnalyzer->GetDataBase(iDB)->GetName(), "odb")){
         path = "/Alarms/Alarms/TestAlarm/Alarm Message";
         // Read ODB
         gAnalyzer->GetDataBase(iDB)->Read(value, path.Data());
         msgOrg = value->At(0, 0);
         cout<<msgOrg<<endl;

         // Write ODB
         value->SetAt("Modified message",0,0);
         gAnalyzer->GetDataBase(iDB)->Write(value, path.Data());

         // Read ODB
         gAnalyzer->GetDataBase(iDB)->Read(value, path.Data());
         cout<<value->At(0, 0)<<endl;

         // Write ODB
         value->SetAt(msgOrg.Data(),0,0);
         gAnalyzer->GetDataBase(iDB)->Write(value, path.Data());
      }
   }
   delete value;
}

void MIDTPrintValues::Terminate()
{
}


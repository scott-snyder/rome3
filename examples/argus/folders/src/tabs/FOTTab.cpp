// Author: 

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// FOTTab                                                                     //
//                                                                            //
// Begin_Html <!--
/*-->

<!--*/
// --> End_Html
//                                                                            //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

/* Generated header file containing necessary includes                        */
#include "generated/FOTTabGeneratedIncludes.h"

////////////////////////////////////////////////////////////////////////////////
/*  This header was generated by ROMEBuilder. Manual changes above the        *
 * following line will be lost next time ROMEBuilder is executed.             */
/////////////////////////////////////----///////////////////////////////////////
#include "generated/FOAnalyzer.h"
#include "generated/FODBAccess.h"
#include "include/generated/FOWindow.h"
#include "include/tabs/FOTTab.h"

// uncomment if you want to include headers of all folders
//#include "FOAllFolders.h"


ClassImp(FOTTab)

void FOTTab::Init()
{
   //ODB
   if ((gAnalyzer->IsActiveDAQ("midas") && gAnalyzer->isOnline() /* we have ODB connection */) &&
       !gAnalyzer->IsROMEMonitor() /* current running mode is not 'monitor connection to analyzer */) {
      gAnalyzer->GetDBAccess()->ReadODB();
   }
   cout<<endl<<endl<<"Run number is "<<gAnalyzer->GetODB()->GetRunNumber()<<endl<<endl;

   //ROMEDataBase
   gAnalyzer->GetDBAccess()->ReadROMEDataBase();
   cout<<"comment "<<gAnalyzer->GetROMEDataBase()->Getcomment()<<endl;

   //ROMEFolder
   if (gAnalyzer->GetNetFolder("mynetfolder")) {
      fRunInfo = (FORunInfo *) ((TFolder *) gAnalyzer->GetNetFolder("mynetfolder")->FindObjectAny("RunInfo"))->FindObjectAny("FORunInfo");
      cout<<"run mode "<<fRunInfo->GetName()<<endl;
   }

   if (gAnalyzer->GetNetFolder("mynetfolder")) {
      fPMTDatas = (TClonesArray *) ((TFolder *) gAnalyzer->GetNetFolder("mynetfolder")->FindObjectAny("PMTData"))->FindObjectAny("FOPMTData");
      for (int i = 0; i < fPMTDatas->GetEntries(); i++) {
         cout<<"PMT data-ADC "<<i<<" :"<<((FOPMTData *) (fPMTDatas->At(i)))->GetADC()<<endl;
      }
   }
}

void FOTTab::EndInit()
{
}

void FOTTab::EventHandler()
{
}

void FOTTab::MenuClicked(TGPopupMenu * /*menu*/,Long_t /*param*/)
{
}

void FOTTab::TabSelected()
{
}
void FOTTab::TabUnSelected()
{
}


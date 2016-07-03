// Author: Ryu Sawada <http://www.icepp.s.u-tokyo.ac.jp/%7Esawada>

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// PMTThread                                                                  //
//                                                                            //
// Begin_Html <!--
/*-->

<!--*/
// --> End_Html
//                                                                            //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

/* Generated header file containing necessary includes                        */
#include "generated/PMTThreadGeneratedIncludes.h"

////////////////////////////////////////////////////////////////////////////////
/*  This header was generated by ROMEBuilder. Manual changes above the        *
 * following line will be lost next time ROMEBuilder is executed.             */
/////////////////////////////////////----///////////////////////////////////////
#include "generated/PMAnalyzer.h"
#include "include/generated/PMWindow.h"
#include "include/tabs/PMTThread.h"

// uncomment if you want to include headers of all folders
//#include "PMAllFolders.h"


ClassImp(PMTThread)

using namespace std;

void PMTThread::Init()
{
   fHorz = new TGHorizontalFrame(this, (UInt_t) (700 * gAnalyzer->GetWindow()->GetWindowScale()), (UInt_t) (700 * gAnalyzer->GetWindow()->GetWindowScale()));

   fButton = new TGTextButton(fHorz, "w/ Thread", 0);
   fButton->Associate(this);
   fHorz->AddFrame(fButton, new TGLayoutHints(kLHintsCenterX, 10, 10, 4, 4));

   AddFrame(fHorz, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 10, 10, 4, 4));
}

void PMTThread::EndInit()
{
}

void PMTThread::EventHandler()
{
}

void PMTThread::MenuClicked(TGPopupMenu * /*menu*/,Long_t /*param*/)
{
}

void PMTThread::TabSelected()
{
}
void PMTThread::TabUnSelected()
{
}

Bool_t PMTThread::ProcessMessageThread(Long_t msg, Long_t param1, Long_t /*param2*/)
{
   switch (GET_MSG(msg)) {
   case kC_COMMAND:
      switch (GET_SUBMSG(msg)) {
      case kCM_BUTTON:
         switch (param1) {
         case 0:
            withMakingThread();
            break;
         }
         break;
      }
      break;
   }
   return true;
}

void PMTThread::withMakingThread()
{
   cout << "This function is executed with making new thread" << endl;
   cout << " When you don't have ProcessMessage," << endl;
   cout << "ProcessMessageThread will be executed instead of ProcessMessage." << endl;
   int i;
   for (i = 10; i > 0; i--) {
      if (IsActive())
         cout << i << endl;
      gSystem->Sleep(1000);
   }
   cout << "PMTThread::withMakingThread is finished" << endl;
   cout << endl;
}

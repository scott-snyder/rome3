////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// MIDTADCTab                                                                 //
//                                                                            //
// Begin_Html <!--
/*-->

<!--*/
// --> End_Html
//                                                                            //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

/* Generated header file containing necessary includes                        */
#include "generated/MIDTADCTabGeneratedIncludes.h"

////////////////////////////////////////////////////////////////////////////////
/*  This header was generated by ROMEBuilder. Manual changes above the        *
 * following line will be lost next time ROMEBuilder is executed.             */
/////////////////////////////////////----///////////////////////////////////////

#include "generated/MIDWindow.h"
#include "generated/MIDAnalyzer.h"
#include "tabs/MIDTADCTab.h"
#include "tasks/MIDTFillHistogram.h"

// uncomment if you want to include headers of all folders
//#include "MIDAllFolders.h"


ClassImp(MIDTADCTab)

//______________________________________________________________________________
void MIDTADCTab::Init()
{
   // Create a vertical frame containing buttons and canvas
   fVert = new TGVerticalFrame(this, (UInt_t) (700 * gAnalyzer->GetWindow()->GetWindowScale()), (UInt_t) (700 * gAnalyzer->GetWindow()->GetWindowScale()));

   // Create an embedded canvas and add to the main frame, centered in x and y
   fCanvas = new TRootEmbeddedCanvas("Sample Canvas", fVert, (UInt_t) (600 * gAnalyzer->GetWindow()->GetWindowScale()), (UInt_t) (600 * gAnalyzer->GetWindow()->GetWindowScale()));
   fCanvas->GetCanvas()->Divide(4, 2);

   // Create "Update" button
   fBUpdate = new TGTextButton(fVert, "Update");
   fBUpdate->Associate(this);

   fVert->AddFrame(fCanvas, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 4, 4, 4, 4));
   fVert->AddFrame(fBUpdate, new TGLayoutHints(kLHintsCenterX, 10, 10, 4, 4));
   AddFrame(fVert, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 10, 10, 4, 4));

   ConnectAll();
}

//______________________________________________________________________________
void MIDTADCTab::EndInit()
{
}

//______________________________________________________________________________
void MIDTADCTab::EventHandler()
{
   // enable here to automatically update histograms
//   Update();
}

//______________________________________________________________________________
void MIDTADCTab::MenuClicked(TGPopupMenu * /* menu */, Long_t /* param */)
{
}

//______________________________________________________________________________
void MIDTADCTab::TabSelected()
{
}

//______________________________________________________________________________
void MIDTADCTab::TabUnSelected()
{
}

//______________________________________________________________________________
void MIDTADCTab::Update()
{
   TH1 *histo;

   for (Int_t i = 0; i < 8; i++) {
      fCanvas->GetCanvas()->cd(i + 1);
      histo = (TH1F *) gAnalyzer->GetFillHistogramTask()->GetADCAt(i);
      if (!histo) {
         cout << "Histo ADC not available." << endl;
      } else {
         histo->Draw();
      }
   }

   fCanvas->GetCanvas()->cd(0);
   fCanvas->GetCanvas()->Modified();
   fCanvas->GetCanvas()->Update();

   return;
}

//______________________________________________________________________________
void MIDTADCTab::ConnectAll()
{
   fBUpdate->Connect("Pressed()", "MIDTADCTab", this, "Update()");
}

//______________________________________________________________________________
void MIDTADCTab::DisconnectAll()
{
   if (fBUpdate) { fBUpdate->Disconnect(this); }
}
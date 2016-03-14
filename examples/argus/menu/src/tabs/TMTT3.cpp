// Author: Ryu Sawada <http://www.icepp.s.u-tokyo.ac.jp/%7Esawada>

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// TMTT3                                                                      //
//                                                                            //
// Begin_Html <!--
/*-->

<!--*/
// --> End_Html
//                                                                            //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

/* Generated header file containing necessary includes                        */
#include "generated/TMTT3GeneratedIncludes.h"

////////////////////////////////////////////////////////////////////////////////
/*  This header was generated by ROMEBuilder. Manual changes above the        *
 * following line will be lost next time ROMEBuilder is executed.             */
/////////////////////////////////////----///////////////////////////////////////

#include <TCanvas.h>
#include <TPad.h>
#include <TRandom.h>
#include "generated/TMWindow.h"
#include "generated/TMAnalyzer.h"
#include "tabs/TMTT3.h"

// uncomment if you want to include headers of all folders
//#include "TMAllFolders.h"


ClassImp(TMTT3)

//______________________________________________________________________________
void TMTT3::Init()
{
   // Create a vertical frame containing buttons and canvas
   fVert = new TGVerticalFrame(fParent, (UInt_t) (700 * gAnalyzer->GetWindow()->GetWindowScale()), (UInt_t) (700 * gAnalyzer->GetWindow()->GetWindowScale()));

   // Create an embedded canvas and add to the main frame, centered in x and y
   fCanvas = new TRootEmbeddedCanvas("Tab1", fVert, (UInt_t) (600 * gAnalyzer->GetWindow()->GetWindowScale()), (UInt_t) (600 * gAnalyzer->GetWindow()->GetWindowScale()));

   fVert->AddFrame(fCanvas, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 4, 4, 4, 4));
   AddFrame(fVert, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 10, 10, 4, 4));

   fCanvas->GetCanvas()->Divide(2, 2);

   // histogram shown for this demo
   TH1F *h1 = new TH1F("hist1", "hist1", 100, 0, 100);
   h1->SetLineColor(1);
   TH1F *h2 = new TH1F("hist2", "hist2", 100, 0, 100);
   h2->SetLineColor(2);
   TH1F *h3 = new TH1F("hist3", "hist3", 100, 0, 100);
   h3->SetLineColor(3);
   TH1F *h4 = new TH1F("hist4", "hist4", 100, 0, 100);
   h4->SetLineColor(5);
   const Double_t mean[] = {50,20,80,40};
   const Double_t rms[]  = { 5,10,20,40};
   for (Int_t i = 0; i < 10000; i++) {
      h1->Fill(gRandom->Gaus(mean[0], rms[0]));
      h2->Fill(gRandom->Gaus(mean[1], rms[1]));
      h3->Fill(gRandom->Gaus(mean[2], rms[2]));
      h4->Fill(gRandom->Gaus(mean[3], rms[3]));
   }
   fCanvas->GetCanvas()->cd(1); h1->Draw();
   fCanvas->GetCanvas()->cd(2); h2->Draw();
   fCanvas->GetCanvas()->cd(3); h3->Draw();
   fCanvas->GetCanvas()->cd(4); h4->Draw();

   fCanvas->GetCanvas()->GetPad(1)->AddExec("ex1", ".x OpenCanvas.C");
   fCanvas->GetCanvas()->GetPad(2)->AddExec("ex2", ".x OpenCanvas.C");
   fCanvas->GetCanvas()->GetPad(3)->AddExec("ex3", ".x OpenCanvas.C");
   fCanvas->GetCanvas()->GetPad(4)->AddExec("ex4", ".x OpenCanvas.C");

   fCanvas->GetCanvas()->cd();
   fCanvas->GetCanvas()->Modified();
   fCanvas->GetCanvas()->Update();
}

//______________________________________________________________________________
void TMTT3::EndInit()
{
}

//______________________________________________________________________________
void TMTT3::EventHandler()
{
}

//______________________________________________________________________________
void TMTT3::MenuClicked(TGPopupMenu * /* menu */, Long_t param)
{
   switch (param) {
   case TMWindow::M_T3_Open_Pad1:
      OpenPad(1);
      break;
   case TMWindow::M_T3_Open_Pad2:
      OpenPad(2);
      break;
   case TMWindow::M_T3_Open_Pad3:
      OpenPad(3);
      break;
   case TMWindow::M_T3_Open_Pad4:
      OpenPad(4);
      break;
   }
}

//______________________________________________________________________________
void TMTT3::TabSelected()
{
}

//______________________________________________________________________________
void TMTT3::TabUnSelected()
{
}

//______________________________________________________________________________
void TMTT3::OpenPad(Int_t id)
{
   TPad *pnew = static_cast<TPad*>(fCanvas->GetCanvas()->GetPad(id)->Clone());
   static Int_t count = 1;
   TCanvas *c = new TCanvas(Form("cpad%d", count++));
   pnew->Draw();
   pnew->SetPad(0, 0, 1, 1);
   c->Update();
   c->SaveAs(Form("t3window_%d.pdf", id));
}
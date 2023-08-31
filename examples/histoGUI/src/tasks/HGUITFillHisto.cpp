////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// HGUITFillHisto                                                             //
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
//                                                                            //
// This task contains the following histgrams :                               //
//    MyHisto                                                                 //
//    MyOtherHisto                                                            //
//    My2DHisto                                                               //
// This task contains the following graphs :                                  //
//    MyGraph                                                                 //
//    MyGraphError                                                            //
//                                                                            //
// The histograms/graph are created and saved automaticaly by the task.       //
//                                                                            //
// The following method can be used to get a handle of the histogram/graph :  //
//                                                                            //
// Get<Histogram/Graph Name>()                                                //
//                                                                            //
// For histogram/graph arrays use :                                           //
//                                                                            //
// Get<Histogram/Graph Name>At(Int_t index)                                   //
//                                                                            //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

/* Generated header file containing necessary includes                        */
#include "generated/HGUITFillHistoGeneratedIncludes.h"

////////////////////////////////////////////////////////////////////////////////
/*  This header was generated by ROMEBuilder. Manual changes above the        *
 * following line will be lost next time ROMEBuilder is executed.             */
/////////////////////////////////////----///////////////////////////////////////
#include "generated/HGUIAnalyzer.h"

#include "ArgusHistoDisplay.h"
#include "include/tasks/HGUITFillHisto.h"
#include <TRandom.h>
#include "Riostream.h"

ClassImp(HGUITFillHisto)

void HGUITFillHisto::Init()
{
   GetMyGraph()->SetMarkerStyle(31);
   GetMyGraphError()->SetMarkerStyle(8);
   GetMyGraphError()->SetMarkerColor(4);
}

void HGUITFillHisto::BeginOfRun()
{
}

void HGUITFillHisto::Event()
{
   GetMyHistoAt(0)->Fill(gRandom->Gaus(0,10));
   GetMyHistoAt(1)->Fill(gRandom->Gaus(0,20));
   GetMyHistoAt(2)->Fill(gRandom->Gaus(0,30));
   GetMyHistoAt(3)->Fill(gRandom->Gaus(0,10));
   GetMyHistoAt(2)->SetLineColor(2);
   GetMyHistoAt(3)->SetLineColor(4);
   GetMyOtherHisto()->Fill(gRandom->Gaus(0,40));
   GetMyGraph()->SetPoint(0,0,gRandom->Rndm());
   GetMyGraph()->SetPoint(1,1,gRandom->Rndm());
   GetMyGraph()->SetPoint(2,2,gRandom->Rndm());
   GetMyGraph()->SetPoint(3,3,gRandom->Rndm());
   GetMyGraph()->SetPoint(4,4,gRandom->Rndm());
   GetMyGraphError()->SetPoint(0,0,gRandom->Rndm());
   GetMyGraphError()->SetPoint(1,1,gRandom->Rndm());
   GetMyGraphError()->SetPoint(2,2,gRandom->Rndm());
   GetMyGraphError()->SetPoint(3,3,gRandom->Rndm());
   GetMyGraphError()->SetPoint(4,4,gRandom->Rndm());
   ArgusHistoDisplay::SetLimits(GetMyGraph());
   ArgusHistoDisplay::SetLimits(GetMyGraphError());
   GetMy2DHistoAt(0)->Fill(gRandom->Gaus(0,10), gRandom->Gaus(0,10));
   GetMy2DHistoAt(1)->Fill(gRandom->Gaus(0,20), gRandom->Gaus(0,20));
}

void HGUITFillHisto::EndOfRun()
{
}

void HGUITFillHisto::Terminate()
{
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// SBSTPrintADCValues                                                         //
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
//     PMTData                                                                //
//                                                                            //
//                                                                            //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

/* Generated header file containing necessary includes                        */
#include "generated/SBSTPrintADCValuesGeneratedIncludes.h"

////////////////////////////////////////////////////////////////////////////////
/*  This header was generated by ROMEBuilder. Manual changes above the        *
 * following line will be lost next time ROMEBuilder is executed.             */
/////////////////////////////////////----///////////////////////////////////////
#include "generated/SBSAnalyzer.h"

#include <include/tasks/SBSTPrintADCValues.h>
#include <Riostream.h>

ClassImp(SBSTPrintADCValues)

void SBSTPrintADCValues::Init()
{
}

void SBSTPrintADCValues::BeginOfRun()
{
}

void SBSTPrintADCValues::Event()
{
   for (int i=0;i<10;i++) {
      gAnalyzer->GetPMTDataAt(i)->SetADC(i);
      gAnalyzer->GetPMTDataAt(i)->SetPulseHeightSize(1);
      gAnalyzer->GetPMTDataAt(i)->SetPulseHeightAt(0, i);
   }

   if (gAnalyzer->GetGSP()->GetOutputOnOff()) {
      for (int i=0;i<10;i++)
         ROMEPrint::Print("%d\n",gAnalyzer->GetPMTDataAt(i)->GetADC());
      ROMEPrint::Print("\n");
   }
}

void SBSTPrintADCValues::EndOfRun()
{
}

void SBSTPrintADCValues::Terminate()
{
}


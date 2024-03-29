////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// SDAQTPrintADCValues                                                        //
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
#include "generated/SDAQTPrintADCValuesGeneratedIncludes.h"

////////////////////////////////////////////////////////////////////////////////
/*  This header was generated by ROMEBuilder. Manual changes above the        *
 * following line will be lost next time ROMEBuilder is executed.             */
/////////////////////////////////////----///////////////////////////////////////
#include "generated/SDAQAnalyzer.h"

#include <include/tasks/SDAQTPrintADCValues.h>
#include <Riostream.h>

ClassImp(SDAQTPrintADCValues)

using namespace std;

void SDAQTPrintADCValues::Init()
{
}

void SDAQTPrintADCValues::BeginOfRun()
{
}

void SDAQTPrintADCValues::Event()
{
   if (gAnalyzer->GetGSP()->GetOutputOnOff()) {
      for (int i=0;i<10;i++)
         cout << gAnalyzer->GetPMTDataAt(i)->GetADC() << endl;
      cout << endl;
   }
}

void SDAQTPrintADCValues::EndOfRun()
{
}

void SDAQTPrintADCValues::Terminate()
{
}


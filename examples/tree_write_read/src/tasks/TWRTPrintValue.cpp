////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// TWRTPrintValue                                                             //
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
//     Data1                                                                  //
//     Data2                                                                  //
//                                                                            //
//                                                                            //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

/* Generated header file containing necessary includes                        */
#include "generated/TWRTPrintValueGeneratedIncludes.h"

////////////////////////////////////////////////////////////////////////////////
/*  This header was generated by ROMEBuilder. Manual changes above the        *
 * following line will be lost next time ROMEBuilder is executed.             */
/////////////////////////////////////----///////////////////////////////////////
#include "generated/TWRAnalyzer.h"

#include "include/tasks/TWRTPrintValue.h"
#include "Riostream.h"

ClassImp(TWRTPrintValue)

void TWRTPrintValue::Init()
{
}

void TWRTPrintValue::BeginOfRun()
{
}

void TWRTPrintValue::Event()
{
  cout << gAnalyzer->GetData1()->GetValue1() << "\t"
       << gAnalyzer->GetData2()->GetValue2() << endl;
}

void TWRTPrintValue::EndOfRun()
{
}

void TWRTPrintValue::Terminate()
{
}


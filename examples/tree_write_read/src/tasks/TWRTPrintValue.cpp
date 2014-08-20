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

  Int_t i;
  Int_t n;

  n = gAnalyzer->GetData1()->GetNArr1();
  for (i = 0; i < n; i++) {
    cout << "array " << gAnalyzer->GetData1()->GetArr1At(i) << endl;
  }

  vector<Double_t> vec = gAnalyzer->GetData1()->GetVector1();
  vector<Double_t>::iterator it;
  for (it = vec.begin(); it != vec.end(); ++it) {
    cout << "vector " << *it << endl;
  }

  cout << endl;
}

void TWRTPrintValue::EndOfRun()
{
}

void TWRTPrintValue::Terminate()
{
}


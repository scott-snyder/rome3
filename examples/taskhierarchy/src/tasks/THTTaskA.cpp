////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// THTTaskA                                                                   //
//                                                                            //
// Begin_Html <!--
/*-->

<!--*/
// --> End_Html
//                                                                            //
//                                                                            //
//                                                                            //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

/* Generated header file containing necessary includes                        */
#include "generated/THTTaskAGeneratedIncludes.h"

////////////////////////////////////////////////////////////////////////////////
/*  This header was generated by ROMEBuilder. Manual changes above the        *
 * following line will be lost next time ROMEBuilder is executed.             */
/////////////////////////////////////----///////////////////////////////////////
#include "generated/THAnalyzer.h"

#include "include/tasks/THTTaskA.h"
#include "Riostream.h"

ClassImp(THTTaskA)

using namespace std;

void THTTaskA::Init()
{
   cout << "A:" << GetSP()->Getoutput().Data() << endl;

}
void THTTaskA::BeginOfRun()
{
//   this->StartRootInterpreter();
//   gAnalyzer->SetTerminationFlag();
}

void THTTaskA::Event()
{
}

void THTTaskA::EndOfRun()
{
}

void THTTaskA::Terminate()
{
}


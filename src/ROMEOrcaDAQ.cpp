// Author: Jan Wouters
//////////////////////////////////////////////////////////////////////////
//
//  ROMEOrcaDAQ
//
//  Interface to the Orca System.
//
//  $Id: ROMEOrcaDAQ.cpp 1850 2007-04-29 23:15:00Z sawada $
//
//////////////////////////////////////////////////////////////////////////

#include "ROMEAnalyzer.h"
#include "ROMEOrcaDAQ.h"

ClassImp(ROMEOrcaDAQ)

//______________________________________________________________________________
ROMEOrcaDAQ::ROMEOrcaDAQ() {
}

//______________________________________________________________________________
Bool_t ROMEOrcaDAQ::Init() {
   ROMEPrint::Print("This is Orca\n");
   return true;
}

//______________________________________________________________________________
Bool_t ROMEOrcaDAQ::BeginOfRun() {
   return true;
}

//______________________________________________________________________________
Bool_t ROMEOrcaDAQ::Event(Long64_t /*event*/) {
   return true;
}

//______________________________________________________________________________
Bool_t ROMEOrcaDAQ::EndOfRun() {
   return true;
}

//______________________________________________________________________________
Bool_t ROMEOrcaDAQ::Terminate() {
   return true;
}

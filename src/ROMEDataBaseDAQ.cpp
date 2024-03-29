// Author: Matthias Schneebeli
//////////////////////////////////////////////////////////////////////////
//
//  ROMEDataBaseDAQ
//
//  Interface to a database used as the main input.
//
//  $Id: ROMEDataBaseDAQ.cpp 1850 2007-04-29 23:15:00Z sawada $
//
//////////////////////////////////////////////////////////////////////////

#include "ROMEDataBaseDAQ.h"
#include "ROMEAnalyzer.h"

ClassImp(ROMEDataBaseDAQ)

//______________________________________________________________________________
ROMEDataBaseDAQ::ROMEDataBaseDAQ()
:ROMEDAQSystem()
,fTimeStamp(0)
{
}

//______________________________________________________________________________
Bool_t ROMEDataBaseDAQ::Init() {
   if (gROME->isOnline()) {
      ROMEPrint::Warning("Database mode is not supported for online analysis.\n");
      return false;
   }
   if (gROME->isOffline()) {
      gROME->SetEventBasedDataBase();
   }
   return true;
}

//______________________________________________________________________________
Bool_t ROMEDataBaseDAQ::BeginOfRun() {
   if (gROME->isOffline()) {
      gROME->SetCurrentEventNumber(1);
      this->SetAnalyze();
      this->SetRunning();
   }
   return true;
}

//______________________________________________________________________________
Bool_t ROMEDataBaseDAQ::Event(Long64_t event) {
   if (gROME->isOffline()) {
      gROME->SetCurrentEventNumber(event);
   }

   return true;
}

//______________________________________________________________________________
Bool_t ROMEDataBaseDAQ::EndOfRun() {
   if (gROME->isOffline()) {
   }
   return true;
}

//______________________________________________________________________________
Bool_t ROMEDataBaseDAQ::Terminate() {
   if (gROME->isOffline()) {
   }
   return true;
}




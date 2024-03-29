/********************************************************************
  ROMEDAQSystem.h, M. Schneebeli PSI

  $Id: ROMENoDAQSystem.h 1547 2006-11-20 17:03:12Z schneebeli_m@PSI.CH $

********************************************************************/
#ifndef ROMENoDAQSystem_H
#define ROMENoDAQSystem_H

#include "ROMEAnalyzer.h"
#include "ROMEDAQSystem.h"

class ROMENoDAQSystem : public ROMEDAQSystem {
public:
   ROMENoDAQSystem() {}
   virtual ~ROMENoDAQSystem() {}

   Int_t       GetTimeStamp() { return -1; }
   const char *GetName() const { return "none"; }

   Bool_t      Init() { return true; }
   Bool_t      BeginOfRun() {
                  this->SetRunning();
                  return true;
               }
   Bool_t      Event(Long64_t event) {
                  gROME->SetCurrentEventNumber(event);
                  if (gROME->IsStandAloneARGUS() || gROME->IsROMEMonitor()) SetContinue();
                  return true;
               }
   Long64_t    Seek(Long64_t event) { return event; }
   Bool_t      EndOfRun() { return true; }
   Bool_t      Terminate() { return true; }

   ClassDef(ROMENoDAQSystem, 0) // Empty DAQSystem class
};

#endif   // ROMENoDAQSystem_H

/********************************************************************
  ROMEDataBaseDAQ.h, M. Schneebeli PSI

  $Id: ROMEDataBaseDAQ.h 1273 2006-07-16 00:01:22Z sawada $

********************************************************************/
#ifndef ROMEDataBaseDAQ_H
#define ROMEDataBaseDAQ_H

#include "ROMEDAQSystem.h"

class ROMEDataBaseDAQ : public ROMEDAQSystem
{
protected:
   Int_t      fTimeStamp;    //! Current time stamp

public:
   ROMEDataBaseDAQ();
   virtual ~ROMEDataBaseDAQ() {}

   Int_t       GetTimeStamp() { return fTimeStamp; }
   const char *GetName() const { return "database"; }

   Bool_t      Init();
   Bool_t      BeginOfRun();
   Bool_t      Event(Long64_t event);
   Bool_t      EndOfRun();
   Bool_t      Terminate();

   ClassDef(ROMEDataBaseDAQ, 0) // Database DAQ class
};

#endif   // ROMEDataBaseDAQ_H

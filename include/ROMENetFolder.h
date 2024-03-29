/********************************************************************
  ROMENetFolder.h, M. Schneebeli PSI

  $Id: ROMENetFolder.h 2178 2007-10-22 12:06:20Z schneebeli_m $

********************************************************************/

#ifndef ROMENetFolder_H
#define ROMENetFolder_H

#include "TNetFolder.h"

class ROMENetFolder : public TNetFolder
{
public:
   ROMENetFolder():TNetFolder() {}
   ROMENetFolder( const char *name, const char *title, TSocket *socket,Bool_t reconnect=true):TNetFolder(name,title,socket,reconnect) {}
   virtual ~ROMENetFolder(){}

   Long64_t GetCurrentRunNumber();
   Long64_t GetCurrentEventNumber();
   Bool_t RegisterObject(const char* name);
   Bool_t UnRegisterObject(const char* name);
   Bool_t RequestNewEvent(Long64_t oldRunNumber,Long64_t oldEventNumber);
   Bool_t RequestEvent();
   Bool_t IsProgramTerminated();
   Bool_t IsTaskActive(Int_t i);
   Bool_t IsHistoActive(Int_t iTask,Int_t iHisto);
   Bool_t IsGraphActive(Int_t iTask,Int_t iGraph);

   ClassDef(ROMENetFolder, 0) // Customized TNetFolder for ROME
};

#endif   // ROMENetFolder_H

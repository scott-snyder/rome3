/********************************************************************
  ROMEDataBase.h, M. Schneebeli PSI

  $Id: ROMEDataBase.h 2413 2008-03-25 00:28:30Z sawada $

********************************************************************/
#ifndef ROMEDataBase_H
#define ROMEDataBase_H

#include "ROMEStr2DArray.h"
#include "ROMEString.h"

class ROMEDataBase
{
protected:
   ROMEString fName;
   Bool_t fTreatWarningAsError;

public:
   ROMEDataBase():fName(""),fTreatWarningAsError(kFALSE) {}
   virtual ~ROMEDataBase() {}

   const char     *GetName() const { return fName.Data(); }
   void SetTreatWarningAsError(Bool_t flag) { fTreatWarningAsError = flag; }

   virtual Bool_t  Init(const char* name,const char* path,const char* connection) = 0;
   virtual Bool_t  Read(ROMEStr2DArray *values,const char *path,Long64_t runNumber = 0,Long64_t eventNumber = 0) = 0;
   virtual Bool_t  Write(ROMEStr2DArray* values,const char *path,Long64_t runNumber = 0,Long64_t eventNumber = 0) = 0;
   virtual const char *GetType() const = 0;
   virtual const char *GetDescription() const = 0;

   ClassDef(ROMEDataBase, 0) // Base class for database access
};

#endif   // ROMEDataBase_H

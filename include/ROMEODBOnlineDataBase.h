/********************************************************************
  ROMEODBOnlineDataBase.h, M. Schneebeli PSI

  $Id: ROMEODBOnlineDataBase.h 2161 2007-10-18 21:45:17Z sawada $

********************************************************************/
#ifndef ROMEODBOnlineDataBase_H
#define ROMEODBOnlineDataBase_H

#include "ROMEDataBase.h"

class ROMEStr2DArray;

class ROMEODBOnlineDataBase : public ROMEDataBase
{
protected:

private:
   ROMEODBOnlineDataBase(const ROMEODBOnlineDataBase &db); // not implemented
   ROMEODBOnlineDataBase &operator=(const ROMEODBOnlineDataBase &rhs); // not implemented

public:
   ROMEODBOnlineDataBase();
   virtual ~ROMEODBOnlineDataBase();

   Bool_t      Init(const char* name,const char* path,const char* connection);
   Bool_t      Read(ROMEStr2DArray *values,const char *path,Long64_t runNumber,Long64_t eventNumber);
   Bool_t      Write(ROMEStr2DArray* values,const char *path,Long64_t runNumber,Long64_t eventNumber);
   const char* GetType() const { return "odb"; }
   const char* GetDescription() const { return "Midas ODB database for online use"; }

   ClassDef(ROMEODBOnlineDataBase, 0) // Interface to ODB in online
};

#endif   // ROMEXMLDataBase_H

// $Id: ROMETextDataBase.h 2033 2007-08-05 22:58:11Z sawada $
// Author: Ryu Sawada

#ifndef ROMETextDataBase_H
#define ROMETextDataBase_H

//////////////////////////////////////////////////////////////////////////
//
//  Text format non-relational database.
//
//////////////////////////////////////////////////////////////////////////

#include "ROMEDataBase.h"

class TTimeStamp;

class ROMETextDataBase : public ROMEDataBase
{
protected:
   ROMEString  fDirectoryPath;
   TTimeStamp *fTime;

private:
   ROMETextDataBase(const ROMETextDataBase &db); // not implemented
   ROMETextDataBase &operator=(const ROMETextDataBase &rhs); // not implemented

public:
   ROMETextDataBase();
   virtual ~ROMETextDataBase();

   Bool_t      Init(const char* name,const char* path,const char* connection);
   Bool_t      Read(ROMEStr2DArray *values,const char *dataBasePath,Long64_t runNumber,Long64_t eventNumber);
   Bool_t      Write(ROMEStr2DArray* values,const char *dataBasePath,Long64_t runNumber,Long64_t eventNumber);
   const char *GetType() const { return "text"; }
   const char *GetDescription() const { return "Text data base using the file system to store tables"; }

protected:
   void        RemoveComment(ROMEString &buffer,Bool_t initialize = false) const;
   Bool_t      ContainsData(const char* buffer) const;
   const char *StartMark(const char* valueName) const;
   const char *EndMark(const char* valueName) const;
   void        AddHeader(ROMEString &buffer,const char* fileName) const;
   void        RemoveHeader(ROMEString &buffer) const;

   ClassDef(ROMETextDataBase, 0) // Simple database class
};

#endif   // ROMETexDataBase_H

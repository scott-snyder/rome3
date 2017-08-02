#ifndef MIDTReadScaler_H
#define MIDTReadScaler_H

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// MIDTReadScaler                                                             //
//                                                                            //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////



#include "generated/MIDTReadScaler_Base.h"

class MIDTReadScaler : public MIDTReadScaler_Base
{

private:
   MIDTReadScaler(const MIDTReadScaler &c); // not implemented
   MIDTReadScaler &operator=(const MIDTReadScaler &c); // not implemented

public:
   MIDTReadScaler(const char *name = 0, const char *title = 0, int level = 0, const char *taskSuffix = 0, TFolder *histoFolder = 0)
   :MIDTReadScaler_Base(name,title,level,taskSuffix,histoFolder) {}
   virtual ~MIDTReadScaler() {}

protected:
   // Event Methods
   void Init();
   void BeginOfRun();
   void Event();
   void EndOfRun();
   void Terminate();


   ClassDef(MIDTReadScaler,0)
};

#endif   // MIDTReadScaler_H

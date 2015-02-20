#ifndef MIDTFillHistogram_H
#define MIDTFillHistogram_H

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// MIDTFillHistogram                                                          //
//                                                                            //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////



#include "generated/MIDTFillHistogram_Base.h"

class MIDTFillHistogram : public MIDTFillHistogram_Base
{

private:
   MIDTFillHistogram(const MIDTFillHistogram &c); // not implemented
   MIDTFillHistogram &operator=(const MIDTFillHistogram &c); // not implemented

public:
   MIDTFillHistogram(const char *name = 0, const char *title = 0, int level = 0, const char *taskSuffix = 0, TFolder *histoFolder = 0)
   :MIDTFillHistogram_Base(name,title,level,taskSuffix,histoFolder) {}
   virtual ~MIDTFillHistogram() {}

protected:
   // Event Methods
   void Init();
   void BeginOfRun();
   void Event();
   void EndOfRun();
   void Terminate();


   ClassDef(MIDTFillHistogram,0)
};

#endif   // MIDTFillHistogram_H

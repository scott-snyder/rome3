/********************************************************************
  ROMETreeInfo.h, M. Schneebeli PSI

  $Id: ROMETreeInfo.h 2413 2008-03-25 00:28:30Z sawada $

********************************************************************/
#ifndef ROMETreeInfo_H
#define ROMETreeInfo_H

#include <TObject.h>
#include <TClass.h>

class ROMETreeInfo : public TObject
{
protected:
   Long64_t run;            // Run Number
   Long64_t event;          // Event Number
   Int_t    time;           // Time Stamp

public:
   ROMETreeInfo():TObject(),run(0),event(0),time(0) {
#if (ROOT_VERSION_CODE < ROOT_VERSION(6,0,0))
      ROMETreeInfo::Class()->IgnoreTObjectStreamer();
#endif
   }
   virtual ~ROMETreeInfo() {}

   Long64_t GetRunNumber() const   { return run; }
   Long64_t GetEventNumber() const { return event; }
   Int_t    GetTimeStamp() const   { return time; }

   void     SetRunNumber       (Long64_t RunNumber       ) { run            = RunNumber; }
   void     SetEventNumber     (Long64_t EventNumber     ) { event          = EventNumber; }
   void     SetTimeStamp       (Int_t    TimeStamp       ) { time           = TimeStamp; }

   ClassDef(ROMETreeInfo ,4) // Event header of trees
};

#endif   // ROMETreeInfo_H

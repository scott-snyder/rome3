/********************************************************************
  ROMEConfigGraph.h, M. Schneebeli PSI

  $Id: ROMEConfigGraph.h 2355 2008-01-13 08:14:07Z sawada $

********************************************************************/
#ifndef ROMEConfigGraph_H
#define ROMEConfigGraph_H

#include <ROMEString.h>
#include <TObject.h>

class ROMEConfigGraph : public TObject
{
public:
   ROMEString   fGraphActive;
   Bool_t       fGraphActiveModified;
   ROMEString   fGraphWrite;
   Bool_t       fGraphWriteModified;
   ROMEString   fGraphTitle;
   Bool_t       fGraphTitleModified;
   ROMEString   fGraphFolderTitle;
   Bool_t       fGraphFolderTitleModified;
   ROMEString   fGraphArraySize;
   Bool_t       fGraphArraySizeModified;
   ROMEString   fGraphArrayStartIndex;
   Bool_t       fGraphArrayStartIndexModified;
   ROMEString   fGraphXLabel;
   Bool_t       fGraphXLabelModified;
   ROMEString   fGraphYLabel;
   Bool_t       fGraphYLabelModified;
   ROMEString   fGraphZLabel;
   Bool_t       fGraphZLabelModified;
   ROMEString   fGraphXmin;
   Bool_t       fGraphXminModified;
   ROMEString   fGraphXmax;
   Bool_t       fGraphXmaxModified;
   ROMEString   fGraphYmin;
   Bool_t       fGraphYminModified;
   ROMEString   fGraphYmax;
   Bool_t       fGraphYmaxModified;
   ROMEString   fGraphZmin;
   Bool_t       fGraphZminModified;
   ROMEString   fGraphZmax;
   Bool_t       fGraphZmaxModified;
   ROMEString   fGraphAccumulate;
   Bool_t       fGraphAccumulateModified;

   ROMEConfigGraph()
   :TObject()
   ,fGraphActive("false")
   ,fGraphActiveModified(kFALSE)
   ,fGraphWrite("false")
   ,fGraphWriteModified(kFALSE)
   ,fGraphTitle("")
   ,fGraphTitleModified(kFALSE)
   ,fGraphFolderTitle("")
   ,fGraphFolderTitleModified(kFALSE)
   ,fGraphArraySize("1")
   ,fGraphArraySizeModified(kFALSE)
   ,fGraphArrayStartIndex("0")
   ,fGraphArrayStartIndexModified(kFALSE)
   ,fGraphXLabel("X")
   ,fGraphXLabelModified(kFALSE)
   ,fGraphYLabel("Y")
   ,fGraphYLabelModified(kFALSE)
   ,fGraphZLabel("Z")
   ,fGraphZLabelModified(kFALSE)
   ,fGraphXmin("0")
   ,fGraphXminModified(kFALSE)
   ,fGraphXmax("1")
   ,fGraphXmaxModified(kFALSE)
   ,fGraphYmin("0")
   ,fGraphYminModified(kFALSE)
   ,fGraphYmax("1")
   ,fGraphYmaxModified(kFALSE)
   ,fGraphZmin("0")
   ,fGraphZminModified(kFALSE)
   ,fGraphZmax("1")
   ,fGraphZmaxModified(kFALSE)
   ,fGraphAccumulate("false")
   ,fGraphAccumulateModified(kFALSE)
   {
   }
   virtual ~ROMEConfigGraph() {}

   ClassDef(ROMEConfigGraph, 0) // Graph configuration class
};

#endif   // ROMEConfigGraph_H

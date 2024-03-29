/********************************************************************
  ROMEConfigParameter.h, M.Schneebeli

  $Id: ROMEConfigParameter.h 2574 2011-04-27 19:05:11Z sawada $

********************************************************************/
#ifndef ROMEConfigParameter_H
#define ROMEConfigParameter_H

#include <TNamed.h>
#include "ROMEString.h"
#include "ROMEStrArray.h"

class ROMEConfigParameter : public TNamed
{
private:
   ROMEString            fArraySize;
   ROMEString            fWidgetType;
   ROMEString            fComment;
   Int_t                 fCommentLevel;
   ROMEStrArray*         fReadModifiedTrueLines;
   ROMEString            fDeclaration;
   ROMEStrArray*         fSetLines;
   ROMEStrArray*         fWriteLines;
   ROMEStrArray*         fAdditionalWriteLines;
   ROMEStrArray*         fComboBoxEntries;
   Bool_t                fWriteLinesAlways;

private:
   ROMEConfigParameter(const ROMEConfigParameter &param); // not implemented
   ROMEConfigParameter &operator=(const ROMEConfigParameter &rhs); // not implemented

public:
   ROMEConfigParameter(ROMEString name, ROMEString arraySize = "1", ROMEString widgetType = "EditBox");
   virtual ~ROMEConfigParameter();

   ROMEString&    GetComment() { return fComment; }
   Int_t          GetCommentLevel() { return fCommentLevel; }
   ROMEString&    GetArraySize() { return fArraySize; }
   ROMEString&    GetWidgetType() { return fWidgetType; }
   inline void    SetComment(Int_t level, const char* comment);

   void           ReadComment(Int_t level, const char* parentName, const char* path = 0);

   Int_t          GetNumberOfReadModifiedTrueLines() { return fReadModifiedTrueLines->GetEntriesFast(); }
   const char*    GetReadModifiedTrueLineAt(Int_t i) { return fReadModifiedTrueLines->At2(i); }
   void           AddReadModifiedTrueLine(const char* line) { fReadModifiedTrueLines->AddLast(line); }

   void           SetDeclaration(const char* line = NULL,...) G_GNUC_PRINTF(2, 3);
   const char*    GetDeclaration() const { return fDeclaration.Data(); }

   Int_t          GetNumberOfSetLines() { return fSetLines->GetEntriesFast(); }
   const char*    GetSetLineAt(Int_t i) { return fSetLines->At2(i).Data(); }
   void           AddSetLine(const char* line = NULL,...) G_GNUC_PRINTF(2, 3);

   Int_t          GetNumberOfWriteLines() { return fWriteLines->GetEntriesFast(); }
   const char*    GetWriteLineAt(Int_t i) { return fWriteLines->At2(i).Data(); }
   void           AddWriteLine(const char* line = NULL,...) G_GNUC_PRINTF(2, 3);

   Int_t          GetNumberOfAdditionalWriteLines() { return fAdditionalWriteLines->GetEntriesFast(); }
   const char*    GetAdditionalWriteLineAt(Int_t i) { return fAdditionalWriteLines->At2(i).Data(); }
   void           AddAdditionalWriteLine(const char* line = NULL,...) G_GNUC_PRINTF(2, 3);

   Int_t          GetNumberOfComboBoxEntries() { return fComboBoxEntries->GetEntriesFast(); }
   const char*    GetComboBoxEntryAt(Int_t i) { return fComboBoxEntries->At2(i).Data(); }
   void           AddComboBoxEntry(const char* line = NULL,...) G_GNUC_PRINTF(2, 3);

   Bool_t         IsWriteLinesAlways() { return fWriteLinesAlways; }
   void           DontWriteLinesAlways() { fWriteLinesAlways = false; }
};


void ROMEConfigParameter::SetComment(Int_t level, const char* comment)
{
   fCommentLevel = level;
   fComment = comment;
}

class ROMEConfigParameterGroup : public TObject {
private:
   ROMEString         fGroupName;
   ROMEString         fArraySize;
   ROMEString         fGroupIdentifier;
   ROMEString         fNameIdentifier;
   ROMEString         fArrayIdentifier;
   ROMEString         fTagName;
   ROMEString         fComment;
   ROMEString         fInfo;
   Int_t              fCommentLevel;
   Int_t              fMultiplicity;
   TObjArray*         fParameters;
   TObjArray*         fSubGroups;
   ROMEStrArray*      fReadGroupArrayInitLines;
   ROMEStrArray*      fWriteStartLines;
   ROMEStrArray*      fWriteEndLines;
   Int_t              fHierarchyLevel;
   Bool_t             fWriteAlways;
   Bool_t             fWriteEmptyLine;

public:
   ROMEConfigParameterGroup(ROMEString groupName, ROMEString arraySize = "1",
                            ROMEString groupIdentifier = "", ROMEString nameIdentifier = "",
                            ROMEString arrayIdentifier = "", ROMEString tagName = "",
                            Int_t multiplicity = 1, Bool_t emptyline=kTRUE,
                            ROMEString info = "");
   ROMEConfigParameterGroup(const ROMEConfigParameterGroup &group);
   ROMEConfigParameterGroup &operator=(const ROMEConfigParameterGroup &rhs);
   virtual ~ROMEConfigParameterGroup();

   ROMEString&                GetGroupName() { return fGroupName; }
   ROMEString&                GetArraySize() { return fArraySize; }
   ROMEString&                GetGroupIdentifier() { return fGroupIdentifier; }
   ROMEString&                GetNameIdentifier() { return fNameIdentifier; }
   ROMEString&                GetArrayIdentifier() { return fArrayIdentifier; }
   ROMEString&                GetTagName() { return fTagName; }
   ROMEString&                GetComment() { return fComment; }
   ROMEString&                GetInfo() { return fInfo; }
   Int_t                      GetCommentLevel() { return fCommentLevel; }
   Int_t                      GetMultiplicity() { return fMultiplicity; }
   Int_t                      GetHierarchyLevel() { return fHierarchyLevel; }
   Bool_t                     GetWriteEmptyLine() { return fWriteEmptyLine; }
   Bool_t                     IsWriteAlways() { return fWriteAlways; }

   void                       SetWriteAlways() { fWriteAlways = true; }
   void                       SetHierarchyLevel(Int_t level) { fHierarchyLevel = level; }
   void                       SetWriteEmptyLine(Bool_t flag) { fWriteEmptyLine = flag; }
   inline void                SetComment(Int_t level, const char* comment);

   void                       ReadComment(Int_t level, const char* tab = 0, const char* path = 0);

   Int_t                      GetNumberOfParameters() { return fParameters->GetEntriesFast(); }
   ROMEConfigParameter*       GetParameterAt(Int_t i) { return static_cast<ROMEConfigParameter*>(fParameters->At(i)); }
   ROMEConfigParameter*       GetLastParameter() { return static_cast<ROMEConfigParameter*>(fParameters->At(fParameters->GetEntriesFast()-1)); }
   void                       AddParameter(ROMEConfigParameter* parameter) { fParameters->AddLast(parameter); }

   Int_t                      GetNumberOfSubGroups() { return fSubGroups->GetEntriesFast(); }
   ROMEConfigParameterGroup*  GetSubGroupAt(Int_t i) { return static_cast<ROMEConfigParameterGroup*>(fSubGroups->At(i)); }
   ROMEConfigParameterGroup*  GetLastSubGroup() { return static_cast<ROMEConfigParameterGroup*>(fSubGroups->At(fSubGroups->GetEntriesFast()-1)); }
   void                       AddSubGroup(ROMEConfigParameterGroup* subGroup) { fSubGroups->AddLast(subGroup); subGroup->SetHierarchyLevel(fHierarchyLevel+1); }

   Int_t                      GetNumberOfReadGroupArrayInitLines() { return fReadGroupArrayInitLines->GetEntriesFast(); }
   const char*                GetReadGroupArrayInitLineAt(Int_t i) { return fReadGroupArrayInitLines->At2(i).Data(); }
   void                       AddReadGroupArrayInitLine(const char* line) { fReadGroupArrayInitLines->AddLast(line); }

   Int_t                      GetNumberOfWriteStartLines() { return fWriteStartLines->GetEntriesFast(); }
   const char*                GetWriteStartLineAt(Int_t i) { return fWriteStartLines->At2(i).Data(); }
   void                       AddWriteStartLine(const char* line) { fWriteStartLines->AddLast(line); }

   Int_t                      GetNumberOfWriteEndLines() { return fWriteEndLines->GetEntriesFast(); }
   const char*                GetWriteEndLineAt(Int_t i) { return fWriteEndLines->At2(i).Data(); }
   void                       AddWriteEndLine(const char* line) { fWriteEndLines->AddLast(line); }
};

void ROMEConfigParameterGroup::SetComment(Int_t level, const char* comment)
{
   fCommentLevel = level;
   fComment = comment;
}

#endif   // ROMEConfigParameter_H

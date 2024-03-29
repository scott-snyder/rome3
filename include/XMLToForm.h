// $Id: XMLToForm.h 2305 2007-11-22 18:36:57Z yamada $
#ifndef XMLToForm_H
#define XMLToForm_H

#include <RConfig.h>
#if defined( R__VISUAL_CPLUSPLUS )
#pragma warning( push )
#pragma warning( disable : 4800 )
#endif // R__VISUAL_CPLUSPLUS
#include <TObject.h>
#if defined( R__VISUAL_CPLUSPLUS )
#pragma warning( pop )
#endif // R__VISUAL_CPLUSPLUS
#include "ROMEString.h"

class TGWindow;
class XMLToFormWindow;
class ROMEXML;
class XMLToFormFrame;
class ROMEStrArray;

class XMLToForm : public TObject
{
   friend class XMLToFormWindow;

protected:
   XMLToFormWindow  *fWindow;
   Int_t             fMaximalWindowWidth;
   ROMEXML          *fXML;
   ROMEString        fXMLFileName;
   XMLToFormFrame   *fMainFrame;
   ROMEStrArray     *fSubstitutes;
   ROMEStrArray     *fPlaceHolders;

private:
   XMLToForm(const XMLToForm &form); // not implemented
   XMLToForm &operator=(const XMLToForm &rhs); // not implemented

public:
   XMLToForm();
   XMLToForm(const char* xmlFileName,ROMEStrArray* substitutes=NULL);
   XMLToForm(const TGWindow* p,const TGWindow* main,const char* xmlFileName,
             Int_t *exitButtonID,ROMEStrArray* substitutes=NULL);
   virtual ~XMLToForm();

   void        SaveForm();
   const char* GetValue(const char* label) const;
   Bool_t      GetBoolValue(const char* label) const { return (!strcmp(GetValue(label),"true")); }
   Int_t       GetIntValue(const char* label) const;
   double      GetDoubleValue(const char* label) const;
   Int_t       GetSelectedIndex(const char* label) const { return GetIndex(label); }
   Bool_t      IsChecked(const char* label) const { return (!strcmp(GetValue(label),"true")); }

protected:
   Bool_t      Init(const char* xmlFileName,ROMEStrArray* substitutes=NULL);
   void        InitSubstitutes(ROMEStrArray* substitutes);
   Bool_t      Substitute(ROMEString& placeHolder,ROMEString& substitute);
   Bool_t      XMLToRootClass();
   void        XMLToClass(XMLToFormFrame *frame);
   void        FillClass(XMLToFormFrame *frame);
   void        InitSubFrames(XMLToFormFrame *frame);
   Bool_t      ReadTitle(ROMEString &path,ROMEString &value);
   Int_t       GetIndex(const char* label) const;
   void        DeleteFrame(XMLToFormFrame *frame);
   void        PrintFrame(XMLToFormFrame *frame,Int_t tab=0) const;

   ClassDef(XMLToForm, 0) // Create GUI form from XML file
};

#endif   // XMLToForm_H

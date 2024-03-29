// $Id: ROMEConfigToForm.h 1850 2007-04-29 23:15:00Z sawada $
#ifndef ROMEConfigToForm_H
#define ROMEConfigToForm_H

#include "XMLToForm.h"

class ROMEConfigToForm : public XMLToForm{
protected:
   Int_t fCommentLevel; // Comment Level

public:
   ROMEConfigToForm();
   virtual ~ROMEConfigToForm();
   Int_t  Show(const TGWindow * p,const TGWindow * main);

protected:
   Bool_t Init();
   Bool_t XMLToRootClass();
   void   XMLToClass(XMLToFormFrame *frame);
   void   FillButtonFrame(XMLToFormFrame *frame);
   void   FillRunModeFrame(XMLToFormFrame *frame);

   virtual void AddConfig(XMLToFormFrame *frame) = 0;

   ClassDef(ROMEConfigToForm, 0) // Translate ROME configuration to form
};

#endif   // ROMEConfigToForm_H


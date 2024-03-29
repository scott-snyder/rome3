/********************************************************************
  ROMELabel.h, M. Schneebeli PSI

  $Id: ROMELabel.h 1850 2007-04-29 23:15:00Z sawada $

********************************************************************/
#ifndef ROMELabel_H
#define ROMELabel_H

#include <RConfig.h>
#if defined( R__VISUAL_CPLUSPLUS )
#   pragma warning( push )
#   pragma warning( disable : 4800 )
#endif
#include <TGLabel.h>
#include <TGToolTip.h>
#if defined( R__VISUAL_CPLUSPLUS )
#   pragma warning( pop )
#endif

class ROMELabel : public TGLabel {
protected:
   TGToolTip     *fTip;         // tool tip associated with label

private:
   ROMELabel(const ROMELabel &label); // not implemented
   ROMELabel &operator=(const ROMELabel &rhs); // not implemented

public:
   ROMELabel(const TGWindow *p = 0, const char *text = 0,
           GContext_t norm = GetDefaultGC()(),
           FontStruct_t font = GetDefaultFontStruct(),
           UInt_t options = kChildFrame,
           Pixel_t back = GetDefaultFrameBackground())
   : TGLabel(p, text, norm, font, options, back),fTip(0) {}
   virtual ~ROMELabel() {SafeDelete(fTip);}

   void SetToolTipText(const char *text, Long_t delayms);
   Bool_t HandleCrossing(Event_t *event);

   ClassDef(ROMELabel, 0) // Customized TGLabel for ROME
};

#endif   // ROMELabel_H

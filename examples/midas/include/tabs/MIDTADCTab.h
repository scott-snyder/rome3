#ifndef MIDTADCTab_H
#define MIDTADCTab_H

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// MIDTADCTab                                                                 //
//                                                                            //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/*  This header was generated by ROMEBuilder. Manual changes above the        *
 * following line will be lost next time ROMEBuilder is executed.             */
/////////////////////////////////////----///////////////////////////////////////

#include "generated/MIDTADCTab_Base.h"
#include "TGButton.h"
#include "TRootEmbeddedCanvas.h"
#include "TGLayout.h"
#include "TH1.h"
#include "TCanvas.h"
#include "TGFrame.h"

class MIDWindow;

class MIDTADCTab : public MIDTADCTab_Base
{
protected:
   TGVerticalFrame     *fVert;
   TRootEmbeddedCanvas *fCanvas;
   TGTextButton        *fBUpdate;

private:
   MIDTADCTab(const MIDTADCTab &c); // not implemented
   MIDTADCTab &operator=(const MIDTADCTab &c); // not implemented

public:
   MIDTADCTab(MIDWindow* window = 0):MIDTADCTab_Base(window)
   ,fVert(0)
   ,fCanvas(0)
   ,fBUpdate(0)
   {
   }

   virtual ~MIDTADCTab()
   {
      DisconnectAll();
      delete fVert;
      delete fCanvas;
      delete fBUpdate;
   }

   void Init();
   void EndInit();
   void MenuClicked(TGPopupMenu *menu,Long_t param);
   void TabSelected();
   void TabUnSelected();
   void EventHandler();
   void Update();
   void ConnectAll();
   void DisconnectAll();


   ClassDef(MIDTADCTab,0)
};

#endif   // MIDTADCTab_H
/********************************************************************
  ArgusTab.h, M. Schneebeli PSI

  $Id: ArgusTab.h 2621 2012-10-31 19:52:51Z sawada $

********************************************************************/
#ifndef ArgusTab_H
#define ArgusTab_H

#include <RConfig.h>
#if defined( R__VISUAL_CPLUSPLUS )
#   pragma warning( push )
#   pragma warning( disable : 4800 )
#endif
#include <TArrayI.h>
#include <TGFrame.h>
#include <TGMenu.h>
#if defined( R__VISUAL_CPLUSPLUS )
#   pragma warning( pop )
#endif
#include "ROME.h"
#include "ROMEStrArray.h"
#include "ROMEStopwatch.h"

class ArgusWindow;
class TGTab;

class ArgusTab : public TGCompositeFrame {

protected:
   ArgusWindow   *fWindow;                //! The window holding the tab
   ROMEString     fTitle;                 //! Tab Title
   Bool_t         fTabActive;             //! is Active 
   Bool_t         fBusy;                  //! Busy flag
   Bool_t         fForeground;            //! Flags if selected
   Long64_t       fCurrentRun;            //! Records current run# displayed in the tab 
   Long64_t       fCurrentEvent;          //! Records current event# displayed in the tab 
   ROMEStopwatch  fWatchAll;              //! Records time used by tab
   ROMEStopwatch  fWatchUserEvent;        //! Records time used by user events
   ROMEString     fCpuTimeAllString;      //! Elapsed CPU  Time of all in a readable format
   ROMEString     fRealTimeAllString;     //! Elapsed Real Time of all in a readable format
   ROMEString     fTimeUserEventString;   //! Elapsed CPU Time of user events in a readable format
   Bool_t         fRegisteringActive;     //! Registering active flag
   Bool_t         fSwitch;                //!
   Int_t          fID;                    //!
   TObjArray     *fUserPopupMenus;        //!

   // General Histo/Graph parameters (object displays, single objects)
   ROMEStrArray  *fDrawOption;            //!
   Bool_t         fDrawStat;              //!
   Float_t        fStatW;                 //!
   Float_t        fStatFontSize;          //!
   TArrayI       *fLogScaleX;             //!
   TArrayI       *fLogScaleY;             //!
   TArrayI       *fLogScaleZ;             //!

   TString        fScreenShotName;        //!

   Bool_t         fNewWindow;             //! Open new window at start up.

private:
   ArgusTab(const ArgusTab &tab); // not implemented
   ArgusTab &operator=(const ArgusTab &rhs); // not implemented

public:
   ArgusTab(ArgusWindow* window, const char* title, ROMEStrArray *drawOpt,
            Bool_t drawStat, Float_t statW, Float_t statFontSize,
            TArrayI *logX, TArrayI *logY, TArrayI *logZ, Int_t nUserMenus);
   virtual ~ArgusTab();

   void           ArgusInit();
   void           ArgusEventHandler();
   Bool_t         RequestNewEvent(Long64_t oldRunNumber,Long64_t oldEventNumber);
   Bool_t         RequestEvent();

   const char    *GetTimeStatisticsString(ROMEString& string);
   const char    *GetCpuTimeOfAll();
   const char    *GetRealTimeOfAll();
   const char    *GetTimeOfUserEvents();

   // General Histo/Graph parameter methods (object displays, single objects)
   void           SetLogScaleXAt(Int_t displayTypeIndex,Int_t flag=1) { fLogScaleX->AddAt(flag,displayTypeIndex); }
   Int_t          IsLogScaleXAt(Int_t displayTypeIndex) { return fLogScaleX->At(displayTypeIndex); }
   void           SetLogScaleYAt(Int_t displayTypeIndex,Int_t flag=1) { fLogScaleY->AddAt(flag,displayTypeIndex); }
   Int_t          IsLogScaleYAt(Int_t displayTypeIndex) { return fLogScaleY->At(displayTypeIndex); }
   void           SetLogScaleZAt(Int_t displayTypeIndex,Int_t flag=1) { fLogScaleZ->AddAt(flag,displayTypeIndex); }
   Int_t          IsLogScaleZAt(Int_t displayTypeIndex) { return fLogScaleZ->At(displayTypeIndex); }

   void           SetDrawOptionAt(Int_t displayTypeIndex,const char* option) { fDrawOption->AddAt(option,displayTypeIndex); }
   const char    *GetDrawOptionAt(Int_t displayTypeIndex) { return fDrawOption->At2(displayTypeIndex).Data(); }

   void           SetDrawStat(Bool_t f=kTRUE)  { fDrawStat = f; }
   Bool_t         IsDrawStat() const           { return fDrawStat; }
   void           SetStatW(Float_t v=0.1)      { fStatW = v; }
   Float_t        GetStatW() const             { return fStatW; }
   void           SetStatFontSize(Float_t v=0) { fStatFontSize = v; }
   Float_t        GetStatFontSize() const      { return fStatFontSize; }

   // User Methods
   void           SetTitle(const char* title) { fTitle = title; }
   const char    *GetTitle() const { return fTitle.Data(); }
   void           SetTabActive(Bool_t active) { fTabActive = active; }
   Bool_t         IsTabActive() const { return fTabActive; }
   Bool_t         IsBusy() const { return fBusy; }
   void           SetForeground(Bool_t foreground) { fForeground = foreground; }
   Bool_t         IsForeground() const { return fForeground; }

   void           SetRegisteringActive(Bool_t registeringActive) { fRegisteringActive = registeringActive; }
   Bool_t         IsRegisteringActive() const { return fRegisteringActive; }

   void           SetSwitch(Bool_t val) { fSwitch = val; }
   Bool_t         IsSwitch() const { return fSwitch; }

   void           SetID(const int id) { fID = id; }
   Int_t          GetID() const { return fID; }

   void           SetScreenShotName(const char *fname) { fScreenShotName = fname; }
   const char    *GetScreenShotName() const { return fScreenShotName; }
   Int_t          GetScreenShotNameLength() const { return fScreenShotName.Length(); }
   void           ScreenShot(const char *fname = 0, Bool_t saveframe = kTRUE, Bool_t savetab = kFALSE);
   void           SetNewWindow(Bool_t flag) { fNewWindow = flag; }
   Bool_t         IsNewWindow() const { return fNewWindow; }

   TGPopupMenu   *GetUserPopupMenuAt(int i) const { return static_cast<TGPopupMenu*>(fUserPopupMenus->At(i)); }

   virtual void   BaseInit() = 0;
   virtual void   Init() = 0;
   virtual void   EndInit() = 0;
   virtual void   BaseEventHandler() = 0;
   virtual void   EventHandler() = 0;

   virtual void   RegisterObjects() = 0;
   virtual void   UnRegisterObjects() = 0;

   virtual void   BaseMenuClicked(TGPopupMenu *menu,Long_t param) = 0;
   virtual void   BaseTabSelected() = 0;
   virtual void   BaseTabUnSelected() = 0;

   ClassDef(ArgusTab,0) // Base class of ARGUS tabs
};

#endif   // ArgusTab_H

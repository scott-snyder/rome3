/********************************************************************
  ROMEEventLoop.h, M. Schneebeli PSI

  $Id: ROMEEventLoop.h 2622 2012-11-04 19:57:42Z sawada $

********************************************************************/
#ifndef ROMEEventLoop_H
#define ROMEEventLoop_H

#include <RConfig.h>
#if defined( R__VISUAL_CPLUSPLUS )
#   pragma warning( push )
#   pragma warning( disable : 4800 4244)
#endif // R__VISUAL_CPLUSPLUS
#include <TTimeStamp.h>
#if defined( R__VISUAL_CPLUSPLUS )
#   pragma warning( pop )
#endif // R__VISUAL_CPLUSPLUS
#include "ROME.h"
#include "ROMETask.h"
#include "ROMEAnalyzer.h"
#include "TFile.h"
#if (ROOT_VERSION_CODE >= ROOT_VERSION(5,30,0))
#   include "Compression.h"
#endif

class ROMETreeInfo;
class TFile;
class TTime;
class TThread;

class ROMEEventLoop : public ROMETask {
protected:
   enum {
      kReturn,
      kBreak,
      kContinue
   };
protected:
   Long64_t      fCurrentEvent;                    //! Current Event Number inside Eventloop
   Bool_t        fFirstUserInput;                  //! Flags the first user input
   Bool_t        fStop;                            //! Stop execution

   // Stop at
   Long64_t      fStopAtRun;                       //! Stop execution at this run
   Long64_t      fStopAtEvent;                     //! Stop execution at this event

   // Statistics update
#if (ROOT_VERSION_CODE < ROOT_VERSION(5,27,6))
   ULong_t      *fStatisticsTimeOfLastEvent;       //! Time of last Event
#else
   ULong64_t    *fStatisticsTimeOfLastEvent;       //! Time of last Event
#endif
   Double_t     *fStatisticsLastEvent;             //! Last Event

   // Progress Display
   Int_t         fProgressDelta;                   //! Maximal time difference
#if (ROOT_VERSION_CODE < ROOT_VERSION(5,27,6))
   ULong_t       fProgressTimeOfLastEvent;         //! Last time for display
#else
   ULong64_t     fProgressTimeOfLastEvent;         //! Last time for display
#endif
   Long64_t      fProgressLastEvent;               //! Last Event
   Bool_t        fProgressWrite;                   //! Write flag

   // user input
#if (ROOT_VERSION_CODE < ROOT_VERSION(5,27,6))
   ULong_t       fUserInputLastTime;               //! Last time for user input
#else
   ULong64_t     fUserInputLastTime;               //! Last time for user input
#endif
   Bool_t        fContinuous;                      //! Continuous Mode

   // Output Tree Files
   ROMETreeInfo *fTreeInfo;                        //! Tree Info Object
   Int_t         fTreeUpdateIndex;                 //! Index of the tree (for trees in the same file)
   Bool_t        fAlwaysFillTrees;                 //! Flag if filling trees in every events

   // Histo File
   TFile        *fHistoFile;                       //! Histo file

   //
   ROMEStopwatch fWatchWriteEvent;                 //! Records Time used by Write Event

   // Monitor
   Bool_t        fUpdateWindow;                    //! Window update activation flag
#if (ROOT_VERSION_CODE < ROOT_VERSION(5,27,6))
   ULong_t       fLastUpdateTime;                  //! Time of the last window update
#else
   ULong64_t     fLastUpdateTime;                  //! Time of the last window update
#endif
//   Bool_t        fWindowFirstDraw;                 //! Window first time draw flag
   Long64_t      fUpdateWindowLastEvent;           //! Last event event updated on the window

   // Macro names
   TString       fBeginOfRunMacro;                 //! name of macro to execute at the begin of run
   TString       fBeginOfEventMacro;               //! name of macro to execute at the begin of run
   TString       fEndOfEventMacro;                 //! name of macro to execute at the end of event
   TString       fEndOfRunMacro;                   //! name of macro to execute at the end of run
   Bool_t        fHaveBeginOfRunMacro;             //! flag if it has macro to execute at the begin of run
   Bool_t        fHaveBeginOfEventMacro;           //! flag if it has macro to execute at the begin of run
   Bool_t        fHaveEndOfEventMacro;             //! flag if it has macro to execute at the end of event
   Bool_t        fHaveEndOfRunMacro;               //! flag if it has macro to execute at the end of run

   // NetFolderServer update
#if (ROOT_VERSION_CODE < ROOT_VERSION(5,27,6))
   ULong_t       fLastNetFolderServerUpdateTime;   //! Time of the last NetFolderServers update
#else
   ULong64_t     fLastNetFolderServerUpdateTime;   //! Time of the last NetFolderServers update
#endif
   TThread      *fNetFolderServerUpdateThread;     //! Thread to update NetFolderServers

public:
   // Static Hot Links
#if defined( HAVE_MIDAS )
   static Bool_t fHotLinksChanged;                  //! Flags Changing in Hot Links
#endif // HAVE_MIDAS

private:
   ROMEEventLoop(const ROMEEventLoop &loop); // not implemented
   ROMEEventLoop &operator=(const ROMEEventLoop &rhs); // not implemented

public:
   ROMEEventLoop();
   ROMEEventLoop(const char *name,const char *title);
   virtual ~ROMEEventLoop();

   void         ExecuteTask(Option_t *option="");
   Int_t        GetObjectInterpreterCode       (const char* /*objectPath*/) const { return 0; }
   Int_t        GetObjectInterpreterIntValue   (Int_t /*code*/, Int_t defaultValue) const { return defaultValue; }
   Double_t     GetObjectInterpreterDoubleValue(Int_t /*code*/, Double_t defaultValue) const { return defaultValue; }
   ROMEString&  GetObjectInterpreterCharValue  (Int_t /*code*/, ROMEString& defaultValue, ROMEString& /*buffer*/) const { return defaultValue; }
   void         SetOriginalHistoParameters() {}
   void         SetOriginalGraphParameters() {}
   void         Init() {}
   void         BeginOfRun() {}
   void         Event() {}
   void         EndOfRun() {}
   void         Terminate() {}

   void         SetContinuousMode(Bool_t mode) { fContinuous = mode; }
   Bool_t       isContinuousMode() const       { return fContinuous; }

   Int_t        RunEvent();
   Bool_t       StoreEvent(Bool_t useThread);
   void         NextEvent();
   void         GotoEvent(Long64_t eventNumber);

   // macros
   void         SetBeginOfRunMacro(const char* name) { fBeginOfRunMacro = name; }
   void         SetBeginOfEventMacro(const char* name) { fBeginOfEventMacro = name; }
   void         SetEndOfEventMacro(const char* name) { fEndOfEventMacro = name; }
   void         SetEndOfRunMacro(const char* name) { fEndOfRunMacro = name; }
   const char*  GetBeginOfRunMacro() const { return fBeginOfRunMacro.Data(); }
   const char*  GetBeginOfEventMacro() const { return fBeginOfEventMacro.Data(); }
   const char*  GetEndOfEventMacro() const { return fEndOfEventMacro.Data(); }
   const char*  GetEndOfRunMacro() const { return fEndOfRunMacro.Data(); }

   virtual void AddTreeBranches() = 0;

   // Histos
   void   ReadHistograms();
   Bool_t WriteHistograms(Bool_t snapShot = kFALSE);

protected:
   // Run Status
   Bool_t        isRunning() const { return gROME->GetActiveDAQ()->isRunning(); }
   Bool_t        isStopped() const { return gROME->GetActiveDAQ()->isStopped(); }

   void          SetRunning() { gROME->GetActiveDAQ()->SetRunning(); }
   void          SetStopped() { gROME->GetActiveDAQ()->SetStopped(); }

   // Event Status
   Bool_t        isAnalyze() const { return gROME->GetActiveDAQ()->isAnalyze(); }
   Bool_t        isContinue() const { return gROME->GetActiveDAQ()->isContinue(); }
   Bool_t        isBeginOfRun() const { return gROME->GetActiveDAQ()->isBeginOfRun(); }
   Bool_t        isEndOfRun() const { return gROME->GetActiveDAQ()->isEndOfRun(); }
   Bool_t        isTerminate() const { return gROME->GetActiveDAQ()->isTerminate(); }

   inline void   SetAnalyze() { gROME->GetActiveDAQ()->SetAnalyze(); }
   inline void   SetContinue() { gROME->GetActiveDAQ()->SetContinue(); }
   inline void   SetBeginOfRun() { gROME->GetActiveDAQ()->SetBeginOfRun(); }
   inline void   SetEndOfRun() { gROME->GetActiveDAQ()->SetEndOfRun(); }
   inline void   SetTerminate() { gROME->GetActiveDAQ()->SetTerminate(); }

   // event methods
   Bool_t       DAQInit();
   Bool_t       DAQBeginOfRun(Long64_t runNumber);
   Bool_t       DAQEvent();
   Bool_t       UserInput();
   Bool_t       WriteEvent();
   Bool_t       Update();
   Bool_t       DAQEndOfRun();
   Bool_t       DAQTerminate();
   static Bool_t IsTerminal() { return ROMEAnalyzer::STDOutIsTerminal() && ROMEAnalyzer::STDErrIsTerminal();}
   TFile       *CreateTFile(const char *fname, Option_t *option = "",
                            const char *ftitle = "", Int_t compressionLevel = 1, Int_t compressAlgorithm
#if (ROOT_VERSION_CODE >= ROOT_VERSION(5,30,0))
                            = ROOT::kZLIB
#else
                            = 0
#endif
                           );
   void         AutoSave();
   void         OptimizeTreeMaxMemory() const;

   // stopwatch
   Bool_t TimeEventLoop();

   // virtual methods
   virtual void InitArrayFolders() = 0;
   virtual void CleanUpFolders() = 0;
   virtual void ResetFolders() = 0;

#if defined( HAVE_MIDAS )
   virtual void InitHotLinks() = 0;
   virtual void UpdateHotLinks() = 0;
#endif // HAVE_MIDAS

   virtual void InitTrees() = 0;
   virtual void FillTrees() = 0;
   virtual void GetTreeFileName(ROMEString& buffer,Int_t treeIndex, Bool_t inputFile) const = 0;

   virtual void ReadRunHeaders() = 0;
   virtual void WriteRunHeaders() = 0;

   virtual void ResetStatistics() = 0;

   ClassDef(ROMEEventLoop, 0) // Base event loop class
};

#endif   // ROMEEventLoop_H

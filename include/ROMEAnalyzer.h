/********************************************************************
  ROMEAnalyzer.h, M. Schneebeli PSI

  $Id: ROMEAnalyzer.h 2571 2011-03-28 03:40:17Z sawada $

********************************************************************/
#ifndef ROMEAnalyzer_H
#define ROMEAnalyzer_H

#include <stdlib.h>
#include <RConfig.h>
#if defined( R__VISUAL_CPLUSPLUS )
#   pragma warning( push )
#   pragma warning( disable : 4800 )
#endif // R__VISUAL_CPLUSPLUS
#include <TTask.h>
#include <TFolder.h>
#include <TFile.h>
#include <TSystem.h>
#if (ROOT_VERSION_CODE >= ROOT_VERSION(5,15,2))
#   include <TDirectoryFile.h>
#endif
#if defined( R__VISUAL_CPLUSPLUS )
#   pragma warning( pop )
#endif // R__VISUAL_CPLUSPLUS
#include "ROME.h"
#include "Riostream.h"
#include "ROMEPrint.h"
#include "ROMEString.h"
#include "ROMEStrArray.h"
#include "ROMEDAQSystem.h"
#include "ROMETree.h"
#include "ROMERint.h"
#include "ROMEDataBase.h"
#include "ROMETask.h"
#include "ROMENetFolder.h"
#include "ROMEConfig.h"
#include "TArrayL64.h"
#if defined ( HAVE_MIDAS ) && !defined ( __MAKECINT__ )
#   include "midas.h"
#else
typedef Int_t HNDLE;
#endif

class TObjArray;
class TSocket;
class TTree;
class ROMEDAQSystem;
class ROMEAnalyzer;
class ArgusWindow;
class ROMENetFolderServer;

typedef struct {
   Double_t processedEvents;   //! Processed Events
   Double_t eventsPerSecond;   //! Events per Second
   Double_t writtenEvents;     //! Written Events
} Statistics;

extern ROMEAnalyzer *gROME;  // global ROMEAnalyzer Handle

const Int_t kMaxSocketClients = 100;

const Long64_t kEventNumberInit       = -2;
const Long64_t kEventNumberBeginOfRun = -1;
const Long64_t kEventNumberEndOfRun   = kMaxLong64 - 1;
const Long64_t kEventNumberTerminate  = kMaxLong64;

const Int_t kROMEECodeAnyError                 = 1<<0;
const Int_t kROMEECodeUnexpectedEndOfInputFile = 1<<1;

class ROMEAnalyzer : public TObject
{
friend class ArgusWindow;

public:
   // Analysis Mode
   enum {
      kAnalyzeNotSpecified,
      kAnalyzeOffline,
      kAnalyzeOnline
   };

   // IO type
   enum {
      kNotBased,
      kRunNumberBased,
      kRunNumberAndFileNameBased,
      kFileNameBased
   };

   // Program Mode
   enum {
      kStandAloneROME,
      kStandAloneARGUS,
      kROMEAndARGUS,
      kROMEMonitor
   };

   // Folder Storage Status
   enum {
      kStorageFree,
      kStorageWriting,
      kStorageReading
   };

protected:

   // Program Mode
   Int_t          fProgramMode;

   // Window Closed
   Bool_t         fWindowClosed;                 //! Window closed flag

   // Application
   ROMERint      *fApplication;                  //! Application Handle

   // Cint
   ROMEString     fCintInitialisation;           //! Initialization String for the Cint

   // Active DAQ System
   ROMEDAQSystem *fActiveDAQ;                    //! Handle to the active DAQ system

   // Modes
   Int_t          fAnalysisMode;                 //! Analysis mode flag
   Int_t          fAnalysisModeConfig;           //! Analysis mode flag in config XML file
   Bool_t         fBatchMode;                    //! Batch mode flag
   Bool_t         fDaemonMode;                   //! Daemon mode flag
   Bool_t         fQuitMode;                     //! Quit mode flag
   Bool_t         fGraphicalConfigEdit;          //! Configuration edit flag
   Bool_t         fPreserveConfig;               //! Flag if analyzer overwrites existing config file
   Bool_t         fNoGraphics;                   //! No graphics flag
   Bool_t         fSplashScreen;                 //! Splash screen flag

   Bool_t         fDontReadNextEvent;            //! Don't read the next event from file/buffer
   Bool_t         fSkipEvent;                    //! Prevents the analyzer from analyzing the subsequent tasks of an event
   Int_t          fEventStep;                    //! Analyzer every N events
   Int_t          fEventStepCounter;             //! Counter for event step

   // Directories
   ROMEString     fInputDir;                     //! General Input Directory
   ROMEString     fInputDirConstructed;          //! General Input Directory (parsed)
   ROMEString     fOutputDir;                    //! General Output Directory
   ROMEString     fOutputDirConstructed;         //! General Output Directory (parsed)
   ROMEString     fOutputFileOption;             //! General Output File Option
   Int_t          fOutputObjOption;              //! General Write Option
   ROMEString    *fDataBaseDir;                  //! Data Base File Directories
   ROMEString    *fDataBaseDirConstructed;       //! Data Base File Directories (parsed)
   ROMEString     fConfigDir;                    //! Configuration File Directory
   ROMEString     fConfigDirConstructed;         //! Configuration File Directory (parsed)
   Bool_t         fMakeOutputDirectory;          //! Flag if create directory for output file

   // Run Numbers
   Long64_t       fCurrentRunNumber;             //! Currently Analyzed Run Number
   TArrayL64      fRunNumber;                    //! Run Numbers to Analyze
   ROMEString     fRunNumberString;              //! Run Numbers in Input String Format

   // Event Numbers
   Long64_t       fCurrentEventNumber;           //! Currently Analyzed Event Number
   TArrayL64      fEventNumber;                  //! Event Numbers to Analyze
   ROMEString     fEventNumberString;            //! Event Numbers in Input String Format

   // Input File Names
   ROMEString     fCurrentInputFileName;         //! Name of Currently Analyzed Input File
   ROMEStrArray   fInputFileNames;               //! Input File Names to Analyze
   ROMEString     fInputFileNamesString;         //! Original Input File Names in Input String Format

   // IO type
   Int_t          fIOType;                       //! IO type

   // User Events
   Bool_t         fUserEvent;                    //! General User Event Flag
   Bool_t         fUserEventQ;                   //! Flag for User Event of Type Q
   Bool_t         fUserEventE;                   //! Flag for User Event of Type E
   Bool_t         fUserEventS;                   //! Flag for User Event of Type S
   Bool_t         fUserEventR;                   //! Flag for User Event of Type R
   Bool_t         fUserEventO;                   //! Flag for User Event of Type O
   Bool_t         fUserEventC;                   //! Flag for User Event of Type C
   Bool_t         fUserEventJ;                   //! Flag for User Event of Type J
   Long64_t       fUserEventJEventNumber;        //! Event Number for User Event of Type J
   Bool_t         fUserEventG;                   //! Flag for User Event of Type G
   Long64_t       fUserEventGRunNumber;          //! Run Number for User Event of Type G
   Long64_t       fUserEventGEventNumber;        //! Event Number for User Event of Type G
   Bool_t         fUserEventI;                   //! Flag for User Event of Type I

   // Event ID
   int            fEventID;                      //! Event ID of current Event

   // Flags
   Bool_t         fTerminate;                    //! Termination flag
   Bool_t         fProgramTerminated;            //! Program termination flag
   Bool_t         fFillEvent;                    //! Fill Event Flag

   // Object Handles
   TObjArray     *fTaskObjects;                  //! Handle to Task Objects
   ROMETask      *fMainTask;                     //! Handle to Main Task
   TFolder       *fMainFolder;                   //! Handle to Main Folder
   TFile         *fHistoFiles;                   //! Handle to Histogram Files
   ROMENetFolderServer *fNetFolderServer;        //! Handle to NetFolder server

   // Trees
   TObjArray     *fTreeObjects;                  //! Handle to Tree Objects
   Bool_t         fTreeAccumulation;             //! Accumulation of runs in Trees

   // Histogram Folders
   TFolder       *fMainHistoFolder;              //! Handle to Main Histogram Folder (histos)
   TObjArray     *fHistoFolders;                 //! Handle to Histogram Folder Objects

   Bool_t         fHistoRead;                    //! Histogram read flag
   ROMEString     fHistoRun;                     //! Read Histograms of the specified Run Numbers
   ROMEString     fHistoInputPath;               //! Input path to the Histograms of this Run
   ROMEString     fHistoInputPathConstructed;    //! Input path to the Histograms of this Run after replacing #,##...
   ROMEString     fHistoInputFileName;           //! Input filename to the Histograms of this Run
   ROMEString     fHistoInputFileNameConstructed; //! Input filename to the Histograms of this Run after replacing #,##...

   Bool_t         fHistoWrite;                   //! Histogram write flag
   ROMEString     fHistoOutputPath;              //! Output path to the Histograms of this Run
   ROMEString     fHistoOutputPathConstructed;   //! Output path to the Histograms of this Run after replacing #,##...
   ROMEString     fHistoOutputFileName;          //! Output filename to the Histograms of this Run
   ROMEString     fHistoOutputFileNameConstructed; //! Output filename to the Histograms of this Run after replacing #,##...
   Bool_t         fHistoAccumulateAll;           //! Accmulate all histograms and graphs
   Bool_t         fHistoDeactivateAll;           //! Deactivate all histograms and graphs
#if (ROOT_VERSION_CODE < ROOT_VERSION(5,27,6))
   ULong_t        fHistoAutoSavePeriod;          //! Period for auto save
#else
   ULong64_t      fHistoAutoSavePeriod;          //! Period for auto save
#endif
   ROMEString     fHistoSnapShotFileName;        //! Output filename to the Histograms of snap shot. "##" is replaced with event number
   ROMEString     fHistoSnapShotFileNameConstructed; //! Output filename to the Histograms of snap shot after replacing #,##...
   ULong64_t      fHistoSnapShotEvents;          //! Snap shot is written every n-th events.

   // Program name
   ROMEString     fProgramName;                  //! Name of this Program

   // Online
   ROMEString     fOnlineHost;                   //! Name of the Online Host
   ROMEString     fOnlineExperiment;             //! Name of the Online Experiment
   ROMEString     fOnlineAnalyzerName;           //! The name of the analyzer in the midas environment
   ROMEString     fOnlineMemoryBuffer;           //! The name of the midas memory buffer
   Bool_t         fReadConfigFromODB;            //! When this is true, ODB settings overwrite XML settings (XML file not overwritten)

   // Socket Server
   Bool_t         fSocketServerActive;           //! Socket active
   Int_t          fSocketServerPortNumber;       //! Port Number for TSocket
   Bool_t         fObjectStorageUpdated;         //! Object storage update flag

   // Socket Client
   ROMENetFolder *fSocketClientNetFolder;        //! Handle to the ROMENetFolder of the socket connection to ROME
   ROMEString     fSocketClientHost;             //! Socket connection to ROME host
   Int_t          fSocketClientPort;             //! Socket connection to ROME port

   // Statistics
   Statistics    *fStatistics;                   //! Event statistics for each event types
   Int_t          fMaxEventID;                   //! Max EventID

   // Data base
   ROMEDataBase **fDataBaseHandle;               //! DataBase Handles
   ROMEString    *fDataBaseName;                 //! DataBase name
   ROMEString    *fDataBaseConnection;           //! DataBase connection strings
   Int_t          fNumberOfDataBases;            //! Number of DataBases available

   // Configuration
   ROMEConfig    *fConfiguration;                //! Configuration Handle

   // Run Stat
   Bool_t         fShowRunStat;                  //! Show Run Statistics

   // Event Based Data Base
   Bool_t         fEventBasedDataBase;           //! Flag for Event Based Data Base

   // NetFolder
   Int_t          fNumberOfNetFolders;           //! Number of net folders
   ROMENetFolder **fNetFolder;                   //! netfolder handle
   Bool_t        *fNetFolderActive;              //! active flag
   Bool_t        *fNetFolderReconnect;           //! reconnect flag
   TSocket      **fNetFolderSocket;              //! socket connection handle
   Int_t         *fNetFolderPort;                //! port number
   ROMEString    *fNetFolderName;                //! name
   ROMEString    *fNetFolderHost;                //! server host name
   ROMEString    *fNetFolderRoot;                //! root directory name

   // Monitor
   ArgusWindow   *fWindow;                       //! Handle to Argus Window
   Int_t          fWindowUpdatePeriod;           //! Update period of Argus window, in ms

   // Midas
   HNDLE          fMidasOnlineDataBase;          //! Handle to the Midas Online Data Base

   Int_t          fErrorCode;                    //! Exit code in case ::Start returns false.

   // Max Tree Memory
   Long64_t      fMaxTreeMemory;                 //! maximum memory size used for baskets

   // Report summary output
   TString       fReportSummaryFileName;         //! File name of report summary output
   Int_t         fReportSummaryFileLevel;        //! Verbose level of report summary output
   Int_t         fReportSummaryFileLineLength;   //! Line legth limit for report summary output

private:
   ROMEAnalyzer(const ROMEAnalyzer &analyzer); // not implemented
   ROMEAnalyzer &operator=(const ROMEAnalyzer &rhs); // not implemented

public:
   ROMEAnalyzer();
   ROMEAnalyzer(ROMERint *app, Bool_t batch, Bool_t daemon, Bool_t nographics, Int_t analysisMode,
                const char* programName, const char* onlineName, ROMEConfig* config, int numNetFolder);
   virtual ~ROMEAnalyzer();

   // Program Mode
   Bool_t          IsStandAloneROME() const { return fProgramMode == kStandAloneROME; }
   Bool_t          IsStandAloneARGUS() const { return fProgramMode == kStandAloneARGUS; }
   Bool_t          IsROMEAndARGUS() const { return fProgramMode == kROMEAndARGUS; }
   Bool_t          IsROMEMonitor() const { return fProgramMode == kROMEMonitor; }
   void            SetStandAloneROME() { fProgramMode = kStandAloneROME; }
   void            SetStandAloneARGUS() { fProgramMode = kStandAloneARGUS; }
   void            SetROMEAndARGUS() { fProgramMode = kROMEAndARGUS; }
   void            SetROMEMonitor() { fProgramMode = kROMEMonitor; }

   // Window Closed
   Bool_t          IsWindowClosed() const { return fWindowClosed; }
   void            WindowClosed() { fWindowClosed = true; }

   // Application Handle
   ROMERint       *GetApplication() const { return fApplication; }

   // Cint
   const char     *GetCintInitialisation() const { return fCintInitialisation.Data(); }
   void            SetCintInitialisation(const char *str) { fCintInitialisation = str; }

   // Active DAQ System
   const char     *GetNameOfActiveDAQ() const { if (fActiveDAQ == 0) { return "none"; } return fActiveDAQ->GetName(); }
   ROMEDAQSystem  *GetActiveDAQ() const;
   Bool_t          IsActiveDAQ(const char* daqName) const {
      if (!fActiveDAQ) {
         return false;
      }
      return strcmp(fActiveDAQ->GetName(),daqName) == 0;
   }
   Bool_t          isActiveDAQSet() const { return fActiveDAQ != 0; }
   void            SetActiveDAQ(ROMEDAQSystem *handle) { fActiveDAQ = handle; }

   // Data Base Handle
   const char     *GetDataBaseName(Int_t i) const { return fDataBaseName[i].Data(); }
   void            SetDataBaseName(Int_t i,const char *name) { fDataBaseName[i] = name; }
   const char     *GetDataBaseConnection(Int_t i) const { return fDataBaseConnection[i].Data(); }
   void            SetDataBaseConnection(Int_t i,const char *connection) { fDataBaseConnection[i] = connection; }
   ROMEDataBase   *GetDataBase(Int_t i) const;
   ROMEDataBase   *GetDataBase(const char *name) const;
   Bool_t          isDataBaseActive(const char *name) const;
   void            SetDataBase(Int_t i,ROMEDataBase *dataBase);
   Int_t           GetNumberOfDataBases() const { return fNumberOfDataBases; }
   void            InitDataBases(Int_t number);

   // NetFolderServer
   ROMENetFolderServer *GetNetFolderServer() const { return fNetFolderServer; }

   // modes
   Bool_t          isSplashScreen() const { return fSplashScreen; }
   Bool_t          isBatchMode() const { return fBatchMode; }
   Bool_t          isDaemonMode() const { return fDaemonMode; }
   Bool_t          isQuitMode() const { return fQuitMode; }
   Bool_t          isGraphicalConfigEdit() const { return fGraphicalConfigEdit; }
   Bool_t          isPreserveConfig() const { return fPreserveConfig; }
   Bool_t          isNoGraphics() const { return fNoGraphics; }

   void            SetSplashScreen(Bool_t flag = true)  { fSplashScreen = flag; }
   void            SetBatchMode(Bool_t flag = true) { fBatchMode = flag; }
   void            SetDaemonMode(Bool_t flag = true) { fDaemonMode = flag; }
   void            SetQuitMode(Bool_t flag = true) { fQuitMode = flag; }
   void            SetGraphicalConfigEdit(Bool_t flag = true) { fGraphicalConfigEdit = flag; }
   void            SetPreserveConfig(Bool_t flag = true) { fPreserveConfig = flag; }
   void            SetNoGraphics(Bool_t flag = true) { fNoGraphics = flag; }

   // Analysis Mode
   Bool_t          isOnline() const { return fAnalysisMode == kAnalyzeOnline; }
   Bool_t          isOffline() const { return fAnalysisMode == kAnalyzeOffline; }
   Bool_t          ConfigIsOnline() const { return fAnalysisModeConfig == kAnalyzeOnline; }
   Bool_t          ConfigIsOffline() const { return fAnalysisModeConfig == kAnalyzeOffline; }

   void            SetOnline()  { fAnalysisMode = kAnalyzeOnline; }
   void            SetOffline() { fAnalysisMode = kAnalyzeOffline; }
   void            SetConfigOnline()  { fAnalysisModeConfig = kAnalyzeOnline; }
   void            SetConfigOffline() { fAnalysisModeConfig = kAnalyzeOffline; }

   // Directories
   const char     *GetInputDir()                     { return GetInputDirString().Data(); }
   const char     *GetOutputDir()                    { return GetOutputDirString().Data(); }
   const char     *GetOutputFileOption() const       { return fOutputFileOption.Data(); }
   Int_t           GetOutputObjOption() const        { return fOutputObjOption; }
   const char     *GetDataBaseDir(Int_t i)           { return GetDataBaseDirString(i).Data(); }
   const char     *GetConfigDir()                    { return GetConfigDirString().Data(); }
   const char     *GetRawInputDir() const            { return fInputDir.Data(); }
   const char     *GetRawOutputDir() const           { return fOutputDir.Data(); }
   const char     *GetRawDataBaseDir(Int_t i) const  { return fDataBaseDir[i].Data(); }
   const char     *GetRawConfigDir() const           { return fConfigDir.Data(); }

   const ROMEString &GetInputDirString();
   const ROMEString &GetOutputDirString();
   const ROMEString &GetDataBaseDirString(Int_t i);
   const ROMEString &GetConfigDirString();
   const ROMEString &GetRawInputDirString() const           { return fInputDir; }
   const ROMEString &GetRawOutputDirString() const          { return fOutputDir; }
   const ROMEString &GetRawDataBaseDirString(Int_t i) const { return fDataBaseDir[i]; }
   const ROMEString &GetRawConfigDirString() const          { return fConfigDir; }

   void            SetInputDir(const char *dir) { fInputDir = dir; }
   void            SetInputDir(ROMEString &dir) { fInputDir = dir; }
   void            SetOutputDir(const char *dir) { fOutputDir = dir; }
   void            SetOutputDir(ROMEString &dir) { fOutputDir = dir; }
   void            SetOutputFileOption(const char *opt) { SetOutputFileOption(ROMEString(opt)); }
   void            SetOutputFileOption(ROMEString &opt)
   {
      opt.ToUpper();
      if (opt == "UPDATE") {
         fOutputObjOption = 0;
      } else {
         fOutputObjOption = TObject::kOverwrite;
      }
      fOutputFileOption = opt;
   }
   void            SetDataBaseDir(Int_t i,const char *dir) { fDataBaseDir[i] = dir; }
   void            SetDataBaseDir(Int_t i,ROMEString &dir) { fDataBaseDir[i] = dir; }
   void            SetConfigDir(const char *dir) { fConfigDir = dir; }
   void            SetConfigDir(ROMEString &dir) { fConfigDir = dir; }
   void            SetMakeOutputDirectory(Bool_t flag) { fMakeOutputDirectory = flag; }
   Bool_t          isMakeOutputDirectory() const { return fMakeOutputDirectory; }

   // Fill Event Flag
   Bool_t          isFillEvent() const { return fFillEvent; }
   void            SetFillEvent(Bool_t fillEvent = true) { fFillEvent = fillEvent; }

   // Termination Flag
   Bool_t          isTerminationFlag() const { return fTerminate; }
   void            SetTerminationFlag() { fTerminate = true; }
   Bool_t          IsProgramTerminated() const;
   void            SetProgramTerminated() { fProgramTerminated = true; }

   // Event Read Flag
   Bool_t          IsDontReadNextEvent() const { return fDontReadNextEvent; }
   void            SetDontReadNextEvent(Bool_t flag = true) { fDontReadNextEvent = flag; }

   // Skip Event
   Bool_t          IsSkipEvent() const { return fSkipEvent; }
   void            SetSkipEvent(Bool_t skip = true) { fSkipEvent = skip; }

   // Event Step
   Int_t           GetEventStep() const { return fEventStep; }
   void            SetEventStep(Int_t step) { fEventStep = step; }
   Bool_t          CheckEventStep() { return (fEventStepCounter % fEventStep == 0); }
   void            IncrementEventStepCounter() { fEventStepCounter = (fEventStepCounter + 1) % fEventStep; }
   void            DecrementEventStepCounter() { fEventStepCounter = (fEventStepCounter + fEventStep - 1) % fEventStep; }

   // Tree IO
   Bool_t          isTreeAccumulation() const { return fTreeAccumulation;  }
   void            SetTreeAccumulation(Bool_t flag = true) { fTreeAccumulation = flag;  }

   // Tasks
   void            AddTask(TTask *task) { fTaskObjects->AddLast(task); }
   ROMETask       *GetTaskObjectAt(Int_t index) const { return static_cast<ROMETask*>(fTaskObjects->At(index)); }
   Int_t           GetTaskObjectEntries() const { return fTaskObjects->GetEntries(); }
   Bool_t          IsTaskActive(Int_t taskIndex);
   void            ResetHistos();
   void            ResetGraphs();

   // Trees
   void            AddTree(TTree *tree) { fTreeObjects->Add(new ROMETree(tree)); }
   ROMETree       *GetTreeObjectAt(Int_t index) const { return static_cast<ROMETree*>(fTreeObjects->At(index)); }
   Int_t           GetTreeObjectEntries() const { return fTreeObjects->GetEntries(); }

   // Histogram Folders
   TFolder        *GetMainHistoFolder() const { return fMainHistoFolder; }
   TFolder        *GetHistoFolderAt(int index) const { return static_cast<TFolder*>(fHistoFolders->At(index)); }
   Bool_t          IsHistosRead() const { return fHistoRead; }
   const char*     GetHistosRun() const { return fHistoRun.Data(); }
   const char*     GetHistosPath() const { return fHistoInputPath.Data(); } // for backward compatibility
   const char*     GetHistosInputRawPath() { return fHistoInputPath; }
   const char*     GetHistosInputPath();
   const char*     GetHistosInputRawFileName() { return fHistoInputFileName; }
   const char*     GetHistosInputFileName(Long64_t run);
   Bool_t          IsHistosWrite() const { return fHistoWrite; }
   const char*     GetHistosOutputRawPath() { return fHistoOutputPath; }
   const char*     GetHistosOutputPath();
   const char*     GetHistosOutputRawFileName() { return fHistoOutputFileName; }
   const char*     GetHistosOutputFileName();
   Bool_t          IsHistosAccumulateAll() const { return fHistoAccumulateAll; }
   Bool_t          IsHistosDeactivateAll() const { return fHistoDeactivateAll; }
#if (ROOT_VERSION_CODE < ROOT_VERSION(5,27,6))
   ULong_t         GetHistosAutoSavePeriod() const { return fHistoAutoSavePeriod; }
#else
   ULong64_t       GetHistosAutoSavePeriod() const { return fHistoAutoSavePeriod; }
#endif
   const char*     GetHistosSnapShotRawFileName() { return fHistoSnapShotFileName; }
   const char*     GetHistosSnapShotFileName();
   ULong64_t       GetHistosSnapShotEvents() const { return fHistoSnapShotEvents; }

   void            SetHistosRead(Bool_t flag) { fHistoRead = flag; }
   void            SetHistosRun(const char* runNumber) { fHistoRun = runNumber; }
   void            SetHistosPath(const char* path) { fHistoInputPath = path; } // for backward compatibility
   void            SetHistosInputPath(const char* path) { fHistoInputPath = path; }
   void            SetHistosInputFileName(const char* file) { fHistoInputFileName = file; }
   void            SetHistosWrite(Bool_t flag) { fHistoWrite = flag; }
   void            SetHistosOutputPath(const char* path) { fHistoOutputPath = path; }
   void            SetHistosOutputFileName(const char* file) { fHistoOutputFileName = file; }
   void            SetHistosAccumulateAll(Bool_t flag) { fHistoAccumulateAll = flag; }
   void            SetHistosDeactivateAll(Bool_t flag) { fHistoDeactivateAll = flag; }
#if (ROOT_VERSION_CODE < ROOT_VERSION(5,27,6))
   void            SetHistosAutoSavePeriod(ULong_t period) { fHistoAutoSavePeriod = period; }
#else
   void            SetHistosAutoSavePeriod(ULong64_t period) { fHistoAutoSavePeriod = period; }
#endif
   void            SetHistosSnapShotFileName(const char* file) { fHistoSnapShotFileName = file; }
   void            SetHistosSnapShotEvents(ULong64_t n) { fHistoSnapShotEvents = n; }

   void            ReplaceWithRunAndEventNumber(ROMEString &buffer);
   ROMEString&     ConstructFilePath(const ROMEString &dir, const ROMEString &base, ROMEString& filename);
   Bool_t          IsHistoActive(Int_t taskIndex,Int_t histoIndex);
   Bool_t          IsGraphActive(Int_t taskIndex,Int_t graphIndex);

   // Run Number
   void            GetRunNumberStringAt(ROMEString &buffer,Int_t i, const char* format = 0) const;
   Long64_t        GetRunNumberAt(Int_t i) const { if (i >= fRunNumber.GetSize()) { return 0; } return fRunNumber.At(i); }
   void            GetCurrentRunNumberString(ROMEString &buffer, const char* format = 0);
   Long64_t        GetCurrentRunNumber();
   Int_t           GetNumberOfRunNumbers() const { return fRunNumber.GetSize(); }
   const char     *GetRunNumberStringOriginal() const { return fRunNumberString.Data(); }
   Long64_t        GetFirstRunNumber();
   Long64_t        GetLastRunNumber();

   void            SetCurrentRunNumber(Long64_t runNumber);
   void            SetRunNumbers(ROMEString &numbers) {
                      fRunNumberString = numbers;
                      DecodeNumbers(numbers,fRunNumber);
                   }
   void            SetRunNumbers(const char *numbers) {
                      fRunNumberString = numbers;
                      DecodeNumbers(fRunNumberString,fRunNumber);
                   }
   Long64_t        GetNextRunNumber(const Long64_t runNumber) const;

   // Event Number
   Long64_t        GetCurrentEventNumber();
   const char     *GetEventNumberStringOriginal() const { return fEventNumberString.Data(); }

   void            SetCurrentEventNumber(Long64_t eventNumber);
   void            SetEventNumbers(ROMEString &numbers) {
                      fEventNumberString = numbers;
                      DecodeNumbers(numbers,fEventNumber);
                   }
   void            SetEventNumbers(const char *numbers) {
                      fEventNumberString = numbers;
                      DecodeNumbers(fEventNumberString,fEventNumber);
                   }

   // Input File Names
   ROMEString      GetInputFileNameAt(Int_t i) const { return fInputFileNames.At(i); }
   ROMEString      GetCurrentInputFileName() const { return fCurrentInputFileName; }
   Int_t           GetNumberOfInputFileNames() const { return fInputFileNames.GetEntriesFast(); }
   const char     *GetInputFileNamesStringOriginal() const { return fInputFileNamesString.Data(); }

   void            SetCurrentInputFileName(ROMEString &inputFileName) { fCurrentInputFileName = inputFileName; }
   void            SetCurrentInputFileName(const char *inputFileName) { fCurrentInputFileName = inputFileName; }
   void            SetInputFileNames(ROMEString &names) {
                      fInputFileNamesString = names;
                      DecodeInputFileNames(names,fInputFileNames);
                   }
   void            SetInputFileNames(const char *numbers) {
                      fInputFileNamesString = numbers;
                      DecodeInputFileNames(fInputFileNamesString,fInputFileNames);
                   }

   // IO type
   Bool_t          IsRunNumberBasedIO() const { return (fIOType == kRunNumberBased); }
   Bool_t          IsRunNumberAndFileNameBasedIO() const { return (fIOType == kRunNumberAndFileNameBased); }
   Bool_t          IsFileNameBasedIO() const { return (fIOType == kFileNameBased); }

   void            SetIOType(Int_t type) { fIOType = type; }

   // User Events
   void            SetUserEventQ() { fUserEvent = true; fUserEventQ = true; }
   void            SetUserEventE() { fUserEvent = true; fUserEventE = true; }
   void            SetUserEventS() { fUserEvent = true; fUserEventS = true; }
   void            SetUserEventR() { fUserEvent = true; fUserEventR = true; }
   void            SetUserEventO() { fUserEvent = true; fUserEventO = true; }
   void            SetUserEventC() { fUserEvent = true; fUserEventC = true; }
   void            SetUserEventJ(Long64_t eventNumber) {
                      fUserEvent = true;
                      fUserEventJ = true;
                      fUserEventJEventNumber = eventNumber;
                   }
   void            SetUserEventG(Long64_t runNumber,Long64_t eventNumber) {
                      fUserEvent = true;
                      fUserEventG = true;
                      fUserEventGRunNumber = runNumber;
                      fUserEventGEventNumber = eventNumber;
                   }
   void            SetUserEventI() { fUserEvent = true; fUserEventI = true; }

   Bool_t          HasUserEvent() const { return fUserEvent; }
   void            DeleteUserEvent() {
                      fUserEvent = false;
                      fUserEventQ = false;
                      fUserEventE = false;
                      fUserEventS = false;
                      fUserEventR = false;
                      fUserEventO = false;
                      fUserEventC = false;
                      fUserEventJ = false;
                      fUserEventG = false;
                      fUserEventI = false;
                   }
   Bool_t          IsUserEventQ() const { return fUserEventQ; }
   Bool_t          IsUserEventE() const { return fUserEventE; }
   Bool_t          IsUserEventS() const { return fUserEventS; }
   Bool_t          IsUserEventR() const { return fUserEventR; }
   Bool_t          IsUserEventO() const { return fUserEventO; }
   Bool_t          IsUserEventC() const { return fUserEventC; }
   Bool_t          IsUserEventJ() const { return fUserEventJ; }
   Long64_t        GetUserEventJEventNumber() const { return fUserEventJEventNumber; }
   Bool_t          IsUserEventG() const { return fUserEventG; }
   Long64_t        GetUserEventGRunNumber() const { return fUserEventGRunNumber; }
   Long64_t        GetUserEventGEventNumber() const { return fUserEventGEventNumber; }
   Bool_t          IsUserEventI() const { return fUserEventI; }

   // Event ID
   Int_t           GetEventID() const { return fEventID; }
   void            SetEventID(Int_t eventID) { fEventID = eventID; }

  // Processed Events
   Double_t        GetProcessedEvents() const           { return fStatistics[0].processedEvents; }
   Double_t        GetProcessedEventsAt(Int_t id) const { return id < fMaxEventID && id >= 0 ? fStatistics[id].processedEvents : 0; }

   // main objects
   TFolder        *GetMainFolder() const { return fMainFolder; }
   TTask          *GetMainTask() const { return fMainTask; }
   TFile          *GetHistoFileHandle() const { return fHistoFiles; }

   void            SetHistoFileHandle(TFile *file) { fHistoFiles = file; }

   const char     *GetProgramName() const { return fProgramName.Data(); }

   // Online
   const char     *GetOnlineHost() const { return fOnlineHost.Data(); }
   const char     *GetOnlineExperiment() const { return fOnlineExperiment.Data(); }
   const char     *GetOnlineAnalyzerName() const { return fOnlineAnalyzerName.Data(); }
   const char     *GetOnlineMemoryBuffer() const { return fOnlineMemoryBuffer.Data(); }
   Bool_t          GetReadConfigFromODB() const { return fReadConfigFromODB; }

   void            SetOnlineHost(const char *host) { fOnlineHost = host; }
   void            SetOnlineExperiment(const char *experiment) { fOnlineExperiment = experiment; }
   void            SetOnlineAnalyzerName(const char *analyzerName) { fOnlineAnalyzerName = analyzerName; }
   void            SetOnlineMemoryBuffer(const char *memoryBufferName) { fOnlineMemoryBuffer = memoryBufferName; }
   void            SetReadConfigFromODB(Bool_t flg) { fReadConfigFromODB = flg; }

   // Socket Server
   Int_t           GetSocketServerPortNumber() const { return fSocketServerPortNumber; }
   Bool_t          isSocketServerActive() const { return fSocketServerActive; }

   void            SetSocketServerPortNumber(Int_t portNumber) { fSocketServerPortNumber = portNumber; }
   void            SetSocketServerPortNumber(const char *portNumber) { char *cstop; fSocketServerPortNumber = strtol(portNumber,&cstop,10); }
   void            SetSocketServerActive(Bool_t flag = true) { fSocketServerActive = flag; }
   static THREADTYPE FillObjectsInNetFolderServer(ROMEAnalyzer *localThis);
   virtual void    FillObjectStorage() = 0;
   void            UpdateObjectStorage();
   Bool_t          IsObjectStorageUpdated() const;
   void            SetObjectStorageUpdated();

   virtual void    FillConfigParametersFolder() = 0;
   virtual void    SaveConfigParametersFolder() const = 0;

   // Socket Client
   const char     *GetSocketClientHost() const { return fSocketClientHost.Data(); }
   Int_t           GetSocketClientPort() const { return fSocketClientPort; }
   ROMENetFolder  *GetSocketClientNetFolder() { ConnectSocketClient(); return fSocketClientNetFolder; }

   void            SetSocketClientHost(const char *host) { fSocketClientHost = host; }
   void            SetSocketClientPort(Int_t portNumber) { fSocketClientPort = portNumber; }
   void            SetSocketClientPort(const char *portNumber) { char *cstop; fSocketClientPort = strtol(portNumber,&cstop,10); }

   // Midas ODB
   HNDLE           GetMidasOnlineDataBase() const { return fMidasOnlineDataBase; }
   HNDLE*          GetMidasOnlineDataBasePointer() { return &fMidasOnlineDataBase; }

   // Configuration
   ROMEConfig     *GetConfiguration() const { return fConfiguration; }
   Bool_t          WriteConfigurationFile(ROMEString &configFile) const;
   virtual Bool_t  ShowConfigurationFile() = 0;

   // Statistics
   Statistics     *GetStatisticsAt(Int_t id) const { return id < fMaxEventID && id >= 0 ? &fStatistics[id] : 0; }
   Statistics     *GetTriggerStatistics() { return &fStatistics[0]; }
   Statistics     *GetScalerStatistics() { return &fStatistics[1]; }
   Int_t           GetMaxEventID() const { return fMaxEventID; }

   // Start Method
   Bool_t  Start(int argc = 0, char **argv = 0);

   // Run/Event Number Handling
   static void     DecodeNumbers(ROMEString &str,TArrayL64 &arr);
   static void     DecodeInputFileNames(ROMEString &str,ROMEStrArray &arr);
   Int_t           CheckEventNumber(Long64_t eventNumber) const;
   Int_t           CheckRunNumber(Long64_t runNumber) const;
   static Int_t    CheckNumber(Long64_t eventNumber, const TArrayL64 &numbers);


   // Run Stat
   void            ShowRunStat(Bool_t flag = true) { fShowRunStat = flag; }
   Bool_t          IsShowRunStat() const { return fShowRunStat; }

   // Event Based Data Base
   void            SetEventBasedDataBase(Bool_t flag = true) { fEventBasedDataBase = flag; }
   Bool_t          IsEventBasedDataBase() const { return fEventBasedDataBase; }

   // NetFolder
   Bool_t          IsNetFolderActive(const char *name) const;
   ROMENetFolder  *GetNetFolder(const char *name) const;
   const char     *GetNetFolderName(Int_t i) const { return fNetFolderName[i].Data(); }
   const char     *GetNetFolderHost(Int_t i) const { return fNetFolderHost[i].Data(); }
   Int_t           GetNetFolderPort(Int_t i) const { return fNetFolderPort[i]; }
   const char     *GetNetFolderRoot(Int_t i) const { return fNetFolderRoot[i].Data(); }
   Bool_t          GetNetFolderActive(Int_t i) const { return fNetFolderActive[i]; }
   Bool_t          GetNetFolderReconnect(Int_t i) const { return fNetFolderReconnect[i]; }
   void            SetNetFolderName(Int_t i,const char *name) { fNetFolderName[i] = name; }
   void            SetNetFolderName(Int_t i,ROMEString &name) { fNetFolderName[i] = name; }
   void            SetNetFolderHost(Int_t i,const char *host) { fNetFolderHost[i] = host; }
   void            SetNetFolderHost(Int_t i,ROMEString &host) { fNetFolderHost[i] = host; }
   void            SetNetFolderPort(Int_t i,Int_t port) { fNetFolderPort[i] = port; }
   void            SetNetFolderPort(Int_t i,const char *port) { char *cstop; fNetFolderPort[i] = strtol(port,&cstop,10); }
   void            SetNetFolderPort(Int_t i,ROMEString &port) { SetNetFolderPort(i, port.Data()); }
   void            SetNetFolderRoot(Int_t i,const char *root) { fNetFolderRoot[i] = root; }
   void            SetNetFolderRoot(Int_t i,ROMEString &root) { fNetFolderRoot[i] = root; }
   void            SetNetFolderActive(Int_t i,Bool_t active) { fNetFolderActive[i] = active; }
   void            SetNetFolderReconnect(Int_t i,Bool_t reconnect) { fNetFolderReconnect[i] = reconnect; }
   Int_t           GetNumberOfNetFolders() const { return fNumberOfNetFolders; }

   // NetFolder connection
   Bool_t          ConnectNetFolder(const char *name) const;
   Bool_t          ConnectNetFolder(Int_t i) const;
   Bool_t          DisconnectNetFolder(const char *name) const;
   Bool_t          DisconnectNetFolder(Int_t i) const;
   Bool_t          ConnectNetFolders() const;
   Bool_t          DisconnectNetFolders() const;
   void            InitNetFolders(Int_t number);

   // Folder dump and load
   virtual Bool_t  DumpFolders(const char* filename, Bool_t only_database) const = 0;
   virtual Bool_t  LoadFolders(const char* filename, Bool_t only_database) = 0;

   // Monitor
   ArgusWindow    *GetWindow() const { return fWindow; }
   Bool_t          StartWindow();
   Int_t           GetWindowUpdatePeriod() const { return fWindowUpdatePeriod; }
   void            SetWindowUpdatePeriod(Int_t period) { fWindowUpdatePeriod = period; }
   Bool_t          IsWindowBusy() const;

   // Error code
   Int_t           GetErrorCode() const { return fErrorCode; }
   void            SetErrorCode(Int_t ecode) { fErrorCode = ecode; }
   void            AddErrorCode(Int_t ecode) { fErrorCode |= ecode; }

   // Program Steering
   void            NextEvent();
   void            GotoEvent(Long64_t eventNumber);

   virtual Bool_t  ReadSingleDataBaseFolders() = 0;
   virtual Bool_t  ReadArrayDataBaseFolders() = 0;

   static char     GetChar();
   static Int_t    ss_getchar(UInt_t reset);
   static UInt_t   ss_kbhit();
   Int_t           ss_daemon_init(Bool_t keep_stdout);
   static Int_t    ss_batch_init();
   static Bool_t   strtobool(const char *str);

   static Int_t    stricmp(const char*,const char*);
   static Bool_t   toBool(Int_t value);
   static void     redirectOutput(Bool_t redirect = kTRUE);
   static void     restoreOutput() { redirectOutput(kFALSE); }
   static void     Cleaning();
   static Bool_t   STDOutIsTerminal();
   static Bool_t   STDErrIsTerminal();

   static void     CopyTObjectWithStreamer(TBuffer *buffer,TObject* source,TObject* destination);
   virtual Bool_t  CheckDependences() const { return kTRUE; }
   virtual void    UpdateConfigParameters() {}
   Bool_t          ConnectSocketClient(const char* hostname = 0, Int_t port = 0);
   virtual void    ConstructHistoFolders(TFolder *f, Bool_t addToArray) = 0;
   virtual void    ConstructHistoDirectories(TDirectory *d, TObjArray *cratedDir) = 0;

   // Max Tree Memory
   void         SetMaxTreeMemory(Long64_t size) { fMaxTreeMemory = size; }
   Long64_t     GetMaxTreeMemory() const { return fMaxTreeMemory; }

   // Report Summary File
   void         SetReportSummaryFileName(const char *name) { fReportSummaryFileName = name; }
   const char  *GetReportSummaryFileName() const { return fReportSummaryFileName; }
   void         SetReportSummaryFileLevel(Int_t level) { fReportSummaryFileLevel = level; }
   Int_t        GetReportSummaryFileLevel() const { return fReportSummaryFileLevel; }
   void         SetReportSummaryFileLineLength(Int_t l) { fReportSummaryFileLineLength = l; }
   Int_t        GetReportSummaryFileLineLength() const { return fReportSummaryFileLineLength; }

#if (ROOT_VERSION_CODE < ROOT_VERSION(5,27,6))
   ULong_t   Now() { return static_cast<ULong_t>(gSystem->Now()); }
#else
   ULong64_t Now() { return static_cast<ULong64_t>(gSystem->Now()); }
#endif

protected:
   Bool_t          CreateHistoFolders(TList *,TFolder *) const;

   virtual Bool_t  ReadParameters(int argc, char *argv[]);
   virtual Bool_t  ReadUserParameter(const char* /*opt*/, const char* /*value*/, Int_t &/*i*/) { return false; }
   virtual void    ParameterUsage() const;
   virtual void    UserParameterUsage() const {}

   virtual void    startSplashScreen() = 0;
   virtual void    consoleStartScreen() = 0;
   virtual Bool_t  ConnectSocketClientNetFolder(TSocket *sock) = 0;
   virtual void    StartNetFolderServer() = 0;
   virtual void    StopNetFolderServer();
   virtual Bool_t  CheckTreeFileNames() const = 0;

   ClassDef(ROMEAnalyzer,0) // Base analyzer class
};

#endif   // ROMEAnalyzer_H

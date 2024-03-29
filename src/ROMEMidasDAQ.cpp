// Author: Matthias Schneebeli
//////////////////////////////////////////////////////////////////////////
//
//  ROMEMidasDAQ
//
//  Interface to the Midas System.
//
//  $Id: ROMEMidasDAQ.cpp 2616 2012-09-07 04:16:35Z sawada $
//
//////////////////////////////////////////////////////////////////////////

#include <RConfig.h>

#if defined( R__VISUAL_CPLUSPLUS )
#   pragma warning( push )
#   pragma warning( disable : 4800 )
#endif // R__VISUAL_CPLUSPLUS
#include <TSystem.h>
#include <TThread.h>
#include <TTimer.h>
#if defined( R__VISUAL_CPLUSPLUS )
#   pragma warning( pop )
#endif // R__VISUAL_CPLUSPLUS
#include "ROMEUtilities.h"
#include "ROMEAnalyzer.h"
#include "ROMEODBOfflineDataBase.h"
#include "ROMEMidasDAQ.h"
#include "ROMEMidasFile.h"
#include "ROMEPath.h"

const Int_t kFilePositionsResizeIncrement = 100000;

namespace {
   enum EOnlineThreadRequest {
      kNoRequest = 0,
      kReadEvent,
      kReadEventDummy,
      kReadDB,
      kWriteDB,
      kCheckTransition,
      kFlushBuffer
   };

   // inter thread communication
#if defined( HAVE_MIDAS )
   Int_t           fgOnlineLoopPeriod         = kMidasInitialOnlineLoopPeriod;
#if (ROOT_VERSION_CODE < ROOT_VERSION(5,27,6))
   ULong_t         fgLastLoopTime             = 0;
#else
   ULong64_t       fgLastLoopTime             = 0;
#endif
   Int_t           fgOnlineThreadRequest      = 0;
   Bool_t          fgReadEventReturn          = kFALSE;
   Bool_t          fgDBReadReturn             = kFALSE;
   Bool_t          fgDBWriteReturn            = kFALSE;
   ROMEStr2DArray *fgDBValues                 = 0;
   const char     *fgDBPath                   = 0;
   Long64_t        fgDBRunNumber              = 0;
   Long64_t        fgDBEventNumber            = 0;
   TVirtualMutex  *fgMutex                    = 0;
   Long64_t        fgCurrentRunNumber         = -1;
   Int_t           fgEventStatus              = -1;
   Int_t           fgRunStatus                = -1;
#endif
}

#if defined( HAVE_MIDAS )
#   undef MIDAS_DEBUG // define if you want to run the analyzer in the debugger
void ProcessMessage(Int_t /*hBuf*/, Int_t /*id*/, EVENT_HEADER * /*pheader*/, void * /*message*/)
{
// This method is called, when a system message from the online system occurs
}
#endif

ClassImp(ROMEMidasDAQ)

//______________________________________________________________________________
ROMEMidasDAQ::ROMEMidasDAQ()
:ROMEDAQSystem()
,fMaxEventSize(MAX_EVENT_SIZE)
,fFileName("")
#if defined( R__BYTESWAP )
,fByteSwap(kFALSE)
#else
,fByteSwap(kTRUE)
#endif
,fRawDataDummy(0)
,fNumberOfRawDataEvent(0)
,fCurrentRawDataEvent(-1)
,fValidRawDataEvent(0)
,fReadExistingRawData(kFALSE)
,fNumberOfEventRequests(0)
,fEventRequestID(0)
,fEventRequestMask(0)
,fEventRequestRate(0)
,fMidasOnlineBuffer(0)
,fMidasFile(0)
,fRequestAll(kFALSE)
,fOdbOffline(0)
,fTimeStamp(0)
,fSeqNumToFilePos(new TArrayL64(kFilePositionsResizeIncrement))
,fSeqNumToEventNum(new TArrayL64(kFilePositionsResizeIncrement))
,fEventNumToSeqNum(new TArrayL64(kFilePositionsResizeIncrement))
,fValidSeqNumber(0)
,fValidEventNumber(0)
,fCurrentSeqNumber(0)
,fMaxDataSeqNumber(0)
,fByteSwapFlagMightBeWrong(kFALSE)
,fOnlineHandlerThread(0)
,fOnlineHandlerTimer(0)
,fOnlineConnection(kFALSE)
,fOnlineThread(kFALSE)
,fODBBufferSize(5000)
,fODBBuffer(new char[fODBBufferSize])
{
   Int_t i;

   if (gROME->isOffline()){
      fNumberOfRawDataEvent = 2;
   } else {
      fNumberOfRawDataEvent = kRawDataEvents;
      fRawDataDummy = new char[fMaxEventSize];
   }
   for (i = 0; i < kRawDataEvents; i++) {
      if (i < fNumberOfRawDataEvent) {
         fRawDataEvent[i] = new char[fMaxEventSize];
      } else {
         fRawDataEvent[i] = 0;
      }
   }
}

//______________________________________________________________________________
void ROMEMidasDAQ::SetRawDataEventSize(Long_t size)
{
   if (fMaxEventSize == size) {
      return;
   }

   fMaxEventSize = size;
   Int_t i;
   for (i = 0; i < kRawDataEvents; i++) {
      SafeDeleteArray(fRawDataEvent[i]);
      if (i < fNumberOfRawDataEvent) {
         fRawDataEvent[i] = new char[fMaxEventSize];
      }
   }
}

//______________________________________________________________________________
ROMEMidasDAQ::~ROMEMidasDAQ()
{
   Int_t i;
   for (i = 0; i< kRawDataEvents; i++) {
      SafeDeleteArray(fRawDataEvent[i]);
   }
   SafeDeleteArray(fRawDataDummy);

   SafeDeleteArray(fEventRequestID);
   SafeDeleteArray(fEventRequestMask);
   SafeDeleteArray(fEventRequestRate);

   SafeDelete(fOdbOffline);
   SafeDelete(fSeqNumToFilePos);
   SafeDelete(fSeqNumToEventNum);
   SafeDelete(fEventNumToSeqNum);
   StopOnlineCommunication(this);
   SafeDeleteArray(fODBBuffer);
}

//______________________________________________________________________________
Bool_t ROMEMidasDAQ::Init()
{
#if defined( HAVE_MIDAS )
   if (gROME->isOnline()) {
      ROMEPrint::Print("Program is running online.\n");

      // Connect to the Frontend
      StartOnlineCommunication(this);
      while (fgCurrentRunNumber < 0) {
         gSystem->Sleep(1);
      }
      if (fgCurrentRunNumber >= 0) {
         gROME->SetCurrentRunNumber(fgCurrentRunNumber);
         fgCurrentRunNumber = -1;
      }
      if (fgEventStatus >= 0) {
         fEventStatus = fgEventStatus;
         fgEventStatus = -1;
      }
      if (fgRunStatus >= 0) {
         fRunStatus = fgRunStatus;
         fgRunStatus = -1;
      }
   } else if (gROME->isOffline()) {
      if (!gROME->IsRunNumberBasedIO()) {
         ROMEPrint::Print("The midas DAQ does not yet support InputFileNames.\n");
         return kFALSE;
      }
      ROMEPrint::Print("Program is running offline.\n");
   }
#endif
   return kTRUE;
}

//______________________________________________________________________________
Bool_t ROMEMidasDAQ::BeginOfRun()
{
   if (gROME->isOffline()) {
      ROMEString runNumberString;

      this->SetAnalyze();
      this->SetRunning();
      // Open Midas File
      if (gROME->IsRunNumberBasedIO()) {
         fMidasFile = new ROMEMidasFile();
         ROMEString runNumberString;
         gROME->GetCurrentRunNumberString(runNumberString);
         if (!fMidasFile->Open(gROME->GetInputDir(), runNumberString.Data(), fFileName.Data())) {
            return kFALSE;
         }
         ROMEPrint::Print("Reading Midas-File ");
         ROMEPrint::Print("%s\n", fMidasFile->GetFileName().Data());
      }

      fMaxDataSeqNumber = kMaxLong64;

      fCurrentSeqNumber = 0;
      fValidSeqNumber = 0;
      fValidEventNumber = 0;
      fSeqNumToFilePos->Set(0);
      fSeqNumToEventNum->Set(0);
      fEventNumToSeqNum->Set(0);

      while (!isBeginOfRun() && !isEndOfRun() && !isTerminate()) {
         if (!ReadODBOffline()) {
            return kFALSE;
         }
      }
      if (!isEndOfRun()) {
         SetAnalyze();
      }
   }
   return kTRUE;
}

//______________________________________________________________________________
Bool_t ROMEMidasDAQ::Event(Long64_t event)
{
   // Switch Raw Data Buffer
   if (!fReadExistingRawData) {
      this->SwitchRawDataBuffer();
   }

   if (gROME->isOnline()) {
#if defined( HAVE_MIDAS )
      void* mEvent;

      // check loop period
      if (fgOnlineLoopPeriod > 0) {
         if(gROME->Now() - fgLastLoopTime <
#if (ROOT_VERSION_CODE < ROOT_VERSION(5,27,6))
            static_cast<ULong_t>(fgOnlineLoopPeriod)
#else
            static_cast<ULong64_t>(fgOnlineLoopPeriod)
#endif
            ) {
            this->SetContinue();
            return kTRUE;
         }
         fgLastLoopTime = gROME->Now();
      }

      // check transition
      if (fOnlineThread) {
         if (!fOnlineHandlerThread) {
            return kFALSE;
         }
         {
            ROME_LOCKGUARD(fgMutex);
            fgOnlineThreadRequest = kCheckTransition;
         }
         while (fgOnlineThreadRequest) {
            // wait until thread finish process
            gSystem->Sleep(1);
         }
      } else {
         if (fOnlineHandlerTimer) {
            // call CheckTransition
            fOnlineHandlerTimer->Emit("Timeout()");
         }
      }
      if (fgCurrentRunNumber >= 0) {
         gROME->SetCurrentRunNumber(fgCurrentRunNumber);
         fgCurrentRunNumber = -1;
      }
      if (fgEventStatus >= 0) {
         fEventStatus = fgEventStatus;
         fgEventStatus = -1;
      }
      if (fgRunStatus >= 0) {
         fRunStatus = fgRunStatus;
         fgRunStatus = -1;
      }

      if (isBeginOfRun() || isEndOfRun() || isContinue()) {
         return kTRUE;
      }

      if (!fReadExistingRawData) {
         // read an event
         if (fValidRawDataEvent != kRawDataEvents) {
            fValidRawDataEvent = fCurrentRawDataEvent + 1;
         }

         if (fOnlineThread) {
            if (!fOnlineHandlerThread) {
               return kFALSE;
            }
            {
               ROME_LOCKGUARD(fgMutex);
               fgOnlineThreadRequest = kReadEvent;
            }
            while (fgOnlineThreadRequest) {
               // wait until thread finish process
               gSystem->Sleep(1);
            }
            if (!fgReadEventReturn) {
               this->SetContinue();
               return kTRUE;
            }
         } else {
            if (!ReadOnlineEvent(this)) {
               this->SetContinue();
               return kTRUE;
            }
         }

         mEvent = this->GetRawDataEvent();
         if (reinterpret_cast<EVENT_HEADER*>(mEvent)->event_id != kTriggerEventID) {
            gROME->SetFillEvent(kFALSE);
         } else {
            gROME->SetCurrentEventNumber(reinterpret_cast<EVENT_HEADER*>(mEvent)->serial_number);
         }
         gROME->SetEventID(reinterpret_cast<EVENT_HEADER*>(mEvent)->event_id);
         fTimeStamp = reinterpret_cast<EVENT_HEADER*>(mEvent)->time_stamp;
         if (fByteSwap) {
            //byte swapping
            if(reinterpret_cast<EVENT_HEADER*>(mEvent)->event_id != EVENTID_BOR &&
               reinterpret_cast<EVENT_HEADER*>(mEvent)->event_id != EVENTID_EOR &&
               reinterpret_cast<EVENT_HEADER*>(mEvent)->event_id != EVENTID_MESSAGE)
               if(IsActiveEventID(reinterpret_cast<EVENT_HEADER*>(mEvent)->event_id)) {
                  bk_swap(reinterpret_cast<EVENT_HEADER*>(mEvent) + 1, 0);
               }
         }
      } else {
         mEvent = this->GetRawDataEvent();
         if (reinterpret_cast<EVENT_HEADER*>(mEvent)->event_id != kTriggerEventID) {
            gROME->SetFillEvent(kFALSE);
         } else {
            gROME->SetCurrentEventNumber(reinterpret_cast<EVENT_HEADER*>(mEvent)->serial_number);
         }
         gROME->SetEventID(reinterpret_cast<EVENT_HEADER*>(mEvent)->event_id);
         fTimeStamp = reinterpret_cast<EVENT_HEADER*>(mEvent)->time_stamp;
         fReadExistingRawData = kFALSE;
      }
      this->InitMidasBanks();
#endif
   } else if (gROME->isOffline()) {
      if (!fReadExistingRawData) {
         if (StepEvent() < 0) {
            return kFALSE;
         }
      } else {
         fReadExistingRawData = kFALSE;
      }

      EVENT_HEADER *pevent = reinterpret_cast<EVENT_HEADER*>(this->GetRawDataEvent());

      // Get Handle to ODB header
      if (pevent->event_id == EVENTID_BOR) {
         if (gROME->isDataBaseActive("ODB")) {
            static_cast<ROMEODBOfflineDataBase*>(gROME->GetDataBase("ODB"))->
                  SetBuffer(reinterpret_cast<char*>(pevent + 1));
         }
         this->SetBeginOfRun();
         return kTRUE;
      }
      if (pevent->event_id == EVENTID_EOR) {
         this->SetEndOfRun();
         return kTRUE;
      }
      if (pevent->event_id < 0) {
         this->SetContinue();
         return kTRUE;
      }
      if (fByteSwap) {
         //byte swapping
         if(pevent->event_id != EVENTID_BOR &&
            pevent->event_id != EVENTID_EOR &&
            pevent->event_id != EVENTID_MESSAGE) {
            if(IsActiveEventID(pevent->event_id)) {
               bk_swap(pevent + 1, 0);
            }
         }
      }
      if (pevent->data_size < reinterpret_cast<BANK_HEADER*>(pevent + 1)->data_size) {
         this->SetContinue();
         fByteSwapFlagMightBeWrong = kTRUE;
         return kTRUE;
      }

      if (pevent->event_id == kTriggerEventID && pevent->serial_number < event ) {
         this->SetContinue();
         return kTRUE;
      }

      // initalize event
      gROME->SetEventID(pevent->event_id);
      if (pevent->event_id != kTriggerEventID) {
         gROME->SetFillEvent(kFALSE);
      } else {
         gROME->SetCurrentEventNumber(pevent->serial_number);
      }
      fTimeStamp = pevent->time_stamp;

      this->InitMidasBanks();
   }
   this->InitHeader();
   return kTRUE;
}

//______________________________________________________________________________
Long64_t ROMEMidasDAQ::StepEvent(Bool_t forward)
{
   // In online mode,
   // Move file pointer to the next or previous event
   // returns trigger event number.
   //
   // In offline mode, 'forwar' does not make sense.
   // This function reads one event from current position, and increment current
   // position.
   if (gROME->isOnline()) {
      Int_t i;
      Bool_t found = kFALSE;
      Int_t rawDataEventOld = fCurrentRawDataEvent;
      Long64_t currentEventNumber = gROME->GetCurrentEventNumber();
      Long64_t newEventNumber = currentEventNumber;

      for (i = 0; i < fValidRawDataEvent; i++) {
         if (!forward) {
            fCurrentRawDataEvent = (fCurrentRawDataEvent - 1) % fValidRawDataEvent;
            if (fCurrentRawDataEvent < 0) {
               fCurrentRawDataEvent += fValidRawDataEvent;
            }
            if (reinterpret_cast<EVENT_HEADER*>(this->GetRawDataEvent())->event_id == kTriggerEventID) {
               newEventNumber = reinterpret_cast<EVENT_HEADER*>(this->GetRawDataEvent())->serial_number;
               if (newEventNumber < currentEventNumber) {
                  found = kTRUE;
                  break;
               }
            }
         } else {
            fCurrentRawDataEvent = (fCurrentRawDataEvent + 1) % fValidRawDataEvent;
            if (reinterpret_cast<EVENT_HEADER*>(this->GetRawDataEvent())->event_id == kTriggerEventID) {
               newEventNumber = reinterpret_cast<EVENT_HEADER*>(this->GetRawDataEvent())->serial_number;
               if (newEventNumber > currentEventNumber) {
                  found = kTRUE;
                  break;
               }
            }
         }
      }
      if (!found) {
         fCurrentRawDataEvent = rawDataEventOld;
         newEventNumber = currentEventNumber;
         if (forward) {
            return currentEventNumber;
         }
      }
      fReadExistingRawData = kTRUE;
      return newEventNumber;
   } else if (gROME->isOffline()) {
      EVENT_HEADER *pevent = reinterpret_cast<EVENT_HEADER*>(this->GetRawDataEvent());
      bool readError = kFALSE;
      Long_t n;

      while (1) {
         if (fSeqNumToFilePos->GetSize() <= fCurrentSeqNumber) {
            fSeqNumToFilePos->Set(fSeqNumToFilePos->GetSize() + kFilePositionsResizeIncrement);
            fSeqNumToEventNum->Set(fSeqNumToEventNum->GetSize() + kFilePositionsResizeIncrement);
            fEventNumToSeqNum->Set(fEventNumToSeqNum->GetSize() + kFilePositionsResizeIncrement);
         }
         fSeqNumToFilePos->AddAt(fMidasFile->CurrentPosition(), static_cast<Int_t>(fCurrentSeqNumber));
         // default event number, this will be overwritten later, if this event is trigger event.
         fSeqNumToEventNum->AddAt(fCurrentSeqNumber == 0 ? 0 :
                                  static_cast<Int_t>(fSeqNumToEventNum->At(static_cast<Int_t>(fCurrentSeqNumber - 1))),
                                  static_cast<Int_t>(fCurrentSeqNumber));
         if (fValidSeqNumber < fCurrentSeqNumber + 1) {
            fValidSeqNumber = fCurrentSeqNumber + 1;
         }
         n = fMidasFile->Read(pevent, sizeof(EVENT_HEADER));

         if (n < static_cast<Long_t>(sizeof(EVENT_HEADER))) {
            readError = kTRUE;
         } else {
#if !defined( R__BYTESWAP )
            ROMEUtilities::ByteSwap(&pevent->event_id);
            ROMEUtilities::ByteSwap(&pevent->trigger_mask);
            ROMEUtilities::ByteSwap(&pevent->serial_number);
            ROMEUtilities::ByteSwap(&pevent->time_stamp);
            ROMEUtilities::ByteSwap(&pevent->data_size);
#endif
            if (pevent->event_id == kTriggerEventID) {
               fSeqNumToEventNum->AddAt(pevent->serial_number, static_cast<Int_t>(fCurrentSeqNumber));
               fEventNumToSeqNum->AddAt(static_cast<Long_t>(fCurrentSeqNumber), pevent->serial_number);
               if (fValidEventNumber < pevent->serial_number + 1) {
                  fValidEventNumber = pevent->serial_number + 1;
               }
               gROME->SetCurrentEventNumber(pevent->serial_number);
            }
            n = 0;
            if (pevent->data_size <= 0) {
               readError = kTRUE;
            } else {
               if (fMaxEventSize < static_cast<Long_t>(sizeof(EVENT_HEADER) + pevent->data_size)) {
                  ROMEPrint::Error("The MIDAS event buffer size (%lu) is smaller than actual data size (%lu).\n"
                                   "Please increase it by setting your environment variable MIDAS_MAX_EVENT_SIZE.\n"
                                   "Then recompile after removing obj/ROMEMidasDAQ.o\n",
                                   fMaxEventSize, sizeof(EVENT_HEADER) + pevent->data_size);
                  SetRawDataEventSize(2 * static_cast<Long_t>(sizeof(EVENT_HEADER) + pevent->data_size));
               }
               n = fMidasFile->Read(pevent + 1, pevent->data_size);
               if (n != static_cast<Long_t>(pevent->data_size)) {
                  readError = kTRUE;
               }
#if 0
               if (static_cast<Int_t>(reinterpret_cast<BANK_HEADER*>(pevent + 1)->data_size) <= 0) {
                  readError = kTRUE;
               }
#endif
            }
            fCurrentSeqNumber++;
         }
         if (pevent->event_id == EVENTID_EOR) {
            this->SetEndOfRun();
            return fSeqNumToEventNum->At(static_cast<Int_t>(fCurrentSeqNumber - 1));
         }
         // check input
         if (readError) {
            if (readError && n > 0) {
               ROMEPrint::Warning("Unexpected end of file\n");
               gROME->AddErrorCode(kROMEECodeUnexpectedEndOfInputFile);
               fMaxDataSeqNumber = fCurrentSeqNumber - 2;
               SetEndOfRun();
            } else {
               Warning("StepEvent", "Read error");
               SetContinue();
            }
            fSeqNumToFilePos->AddAt(-1, static_cast<Int_t>(fCurrentSeqNumber - 1));
            fSeqNumToEventNum->AddAt(-1, static_cast<Int_t>(fCurrentSeqNumber - 1));
            gROME->SetDontReadNextEvent();
            return fSeqNumToEventNum->At(static_cast<Int_t>(fCurrentSeqNumber - 1));
         }
         UInt_t dsize = reinterpret_cast<BANK_HEADER*>(pevent + 1)->data_size;
         if (fByteSwap) {
            ROMEUtilities::ByteSwap(&dsize);
         }
         if (pevent->data_size < dsize) {
            continue;
         }
         break;
      }

      return fSeqNumToEventNum->At(static_cast<Int_t>(fCurrentSeqNumber - 1));
   }
   return -1;
}

//______________________________________________________________________________
Long64_t ROMEMidasDAQ::Seek(Long64_t event)
{
   // Move file pointer to specified event
   //
   // In offline mode, it jumps to the trigger event spcified by argument
   // For example,
   //
   // Sequencial number : 1000 1001 1002 1003 1004 1005 1006 1007 1008 1009
   // Serial number     :  621  622   23    5  623  624   24  625  626  627
   // Event ID          :    1    1    2   10    1    1    2    1    1    1
   //
   // If 'event' is 622, it jumps to serial number 1001, return 622
   // If 'event' is 623, it jumps to serial number 1004, return 623
   //
   // In online mode, Seek moves in ring buffer forward or backward,
   // instead of jump to specified event number

   if (event < 0) {
      Warning("Seek", "Event number " R_LLD " was not found.", event);
      event = gROME->GetCurrentEventNumber();
      gROME->SetDontReadNextEvent();
      SetContinue();
      return -1;
   }

   if (gROME->isOnline()) {
      Long64_t currentEventNumber = gROME->GetCurrentEventNumber();

      if (event == currentEventNumber) {
         fReadExistingRawData = kTRUE;
         return event;
      } else if (event < currentEventNumber) {
         return StepEvent(kFALSE);
      } else {
         return StepEvent(kTRUE);
      }
      return -1;
   } else if (gROME->isOffline()) {
      Int_t readSeqNumber;
      Long64_t readPosition;
      Long64_t oldEventNumber = gROME->GetCurrentEventNumber();

      if (event < fValidEventNumber) {
         readSeqNumber = static_cast<Int_t>(fEventNumToSeqNum->At(static_cast<Int_t>(event)));
         readPosition = fSeqNumToFilePos->At(readSeqNumber);
         // use stored position
         if(readPosition != -1) {
            fMidasFile->Seek(static_cast<Long_t>(readPosition));
            fCurrentSeqNumber = readSeqNumber;
            gROME->SetCurrentEventNumber(fSeqNumToEventNum->At(readSeqNumber));
            return fSeqNumToEventNum->At(readSeqNumber);
         } else {
            return -1;
         }
      }

      // seek event
      while (event > gROME->GetCurrentEventNumber()) {
         if (StepEvent() < 0) {
            Seek(oldEventNumber);
            return -1;
         }
         if (isEndOfRun()) {
            Seek(oldEventNumber);
            return -1;
         }
         if (gROME->GetCurrentEventNumber() % 100 == 0) {
            ROMEPrint::Print("Stepping to " R_LLD " (" R_LLD
                             ")                                                  \r",
                             event, gROME->GetCurrentEventNumber());
         }
      }
      ROMEPrint::Print("                                                                      \r");
#if 0 // seeking gzipped file is slow
      readSeqNumber = fEventNumToSeqNum->At(static_cast<Int_t>(event));
      readPosition = fSeqNumToFilePos->At(readSeqNumber);
      // use stored position
      if(readPosition != -1) {
         fMidasFile->Seek(readPosition);
         fCurrentSeqNumber = readSeqNumber;
         gROME->SetCurrentEventNumber(fSeqNumToEventNum->At(readSeqNumber));
         return fSeqNumToEventNum->At(readSeqNumber);
      } else {
         return -1;
      }
#else
      fReadExistingRawData = kTRUE;
      return gROME->GetCurrentEventNumber();
#endif
   }
   return -1;
}

//______________________________________________________________________________
Bool_t ROMEMidasDAQ::EndOfRun()
{
   if (gROME->isOffline()) {
      delete fMidasFile;

      if (fByteSwapFlagMightBeWrong && gROME->GetProcessedEvents() < 0.5) {
         ROMEPrint::Warning("\nWarning : A flag <MidasByteSwap> in your config XML file might be wrong.\n\n");
      }
   } else { // online
      if (!isTerminate()) {
         SetContinue();
      }
      FlushBuffer();
   }
   return kTRUE;
}

//______________________________________________________________________________
Bool_t ROMEMidasDAQ::Terminate()
{
   return kTRUE;
}

//______________________________________________________________________________
Bool_t ROMEMidasDAQ::ActualReadODBOnline(ROMEStr2DArray *values, const char *dataBasePath,
                                         Long64_t runNumber, Long64_t eventNumber)
{
#if defined ( HAVE_MIDAS )
   if (gROME->isOnline()) {
      int i, iv;
      HNDLE hKey;
      char name[500];
      int type, num_values, item_size;
      int buffersize = fODBBufferSize;
      char value[500];
      ROMEString odbPath;
      ROMEString errMsg;

      ROMEPath *path = new ROMEPath();

      try {
         // decode path
         if (!path->Decode(dataBasePath, runNumber, eventNumber)) {
            errMsg.SetFormatted("Path decode error : %s", dataBasePath);
            throw errMsg.Data();
         }
         for (i = 0; i < path->GetNumberOfTables(); i++) {
            odbPath += "/";
            odbPath += path->GetTableNameAt(i);
         }
         odbPath += "/";
         odbPath += path->GetFieldName();
         Int_t nTrial(0);
         while (1) {
            if (db_find_key(gROME->GetMidasOnlineDataBase(), 0, const_cast<char*>(odbPath.Data()), &hKey) != CM_SUCCESS) {
               nTrial++;
               if (nTrial < 10) {
                  gSystem->Sleep(3000);
               } else {
                  errMsg.SetFormatted("Cannot read online database 1 %s\n", odbPath.Data());
                  throw errMsg.Data();
                  break;
               }
            } else {
               break;
            }
         }
         if (db_get_key_info(gROME->GetMidasOnlineDataBase(), hKey, name, sizeof(name), &type, &num_values, &item_size) !=
             CM_SUCCESS) {
            errMsg.SetFormatted("Cannot read online database 2 %s",  odbPath.Data());
            throw errMsg.Data();
         }
         if (num_values * item_size > fODBBufferSize) {
            SetODBBufferSize(num_values * item_size);
         }
         if (db_get_data(gROME->GetMidasOnlineDataBase(), hKey, fODBBuffer, &buffersize, type) != CM_SUCCESS) {
            errMsg.SetFormatted("Cannot read online database 3 %s",  odbPath.Data());
            throw errMsg.Data();
         }
         if (path->GetFieldIndexAt(1) == -1) {
            // single field
            if (db_sprintf(value, fODBBuffer, buffersize, 0, type) != CM_SUCCESS) {
               errMsg.SetFormatted("Cannot read online database 4 %s",  odbPath.Data());
               throw errMsg.Data();
            }
            values->SetAt(value, 0, 0);
         } else {
            // array field
            iv = 0;
            for (i = path->GetFieldIndexAt(0); i <= path->GetFieldIndexAt(1); i += path->GetFieldIndexAt(2)) {
               if (db_sprintf(value, fODBBuffer, buffersize, i, type) != CM_SUCCESS) {
                  errMsg.SetFormatted("Cannot read online database 5 %s",  odbPath.Data());
                  throw errMsg.Data();
               }
               values->SetAt(value, iv, 0);
               iv++;
            }
         }
      }
      catch(const char* err) {
         if (err) {
            ROMEPrint::Error("\n%s\n", err);
         }
         delete path;
         return kFALSE;
      }
      delete path;
      return kTRUE;
   }
#else
   WarningSuppression(values);
   WarningSuppression(dataBasePath);
   WarningSuppression(runNumber);
   WarningSuppression(eventNumber);
#endif // HAVE_MIDAS
   return kTRUE;
}

//______________________________________________________________________________
Bool_t ROMEMidasDAQ::WriteODBOnline(ROMEStr2DArray* values, const char * dataBasePath,
                                    Long64_t runNumber, Long64_t eventNumber)
{
#if defined( HAVE_MIDAS )
   if (fOnlineThread) {
      if (!fOnlineHandlerThread) {
         return kFALSE;
      }
      {
         ROME_LOCKGUARD(fgMutex);
         fgDBValues = values;
         fgDBPath = dataBasePath;
         fgDBRunNumber = runNumber;
         fgDBEventNumber = eventNumber;
         fgOnlineThreadRequest = kWriteDB;
      }
      while (fgOnlineThreadRequest) {
         // wait until thread finish process
         gSystem->Sleep(1);
      }
      return fgDBWriteReturn;
   } else {
      return ActualWriteODBOnline(values, dataBasePath, runNumber, eventNumber);
   }
#else
   WarningSuppression(values);
   WarningSuppression(dataBasePath);
   WarningSuppression(runNumber);
   WarningSuppression(eventNumber);
   return kTRUE;
#endif
}

//______________________________________________________________________________
Bool_t ROMEMidasDAQ::ReadODBOnline(ROMEStr2DArray* values, const char * dataBasePath,
                                   Long64_t runNumber, Long64_t eventNumber)
{
#if defined( HAVE_MIDAS )
   if (fOnlineThread) {
      if (!fOnlineHandlerThread) {
         return kFALSE;
      }
      {
         ROME_LOCKGUARD(fgMutex);
         fgDBValues = values;
         fgDBPath = dataBasePath;
         fgDBRunNumber = runNumber;
         fgDBEventNumber = eventNumber;
         fgOnlineThreadRequest = kReadDB;
      }
      while (fgOnlineThreadRequest) {
         // wait until thread finish process
         gSystem->Sleep(1);
      }
      return fgDBReadReturn;
   } else {
      return ActualReadODBOnline(values, dataBasePath, runNumber, eventNumber);
   }
#else
   WarningSuppression(values);
   WarningSuppression(dataBasePath);
   WarningSuppression(runNumber);
   WarningSuppression(eventNumber);
   return kTRUE;
#endif
}

//______________________________________________________________________________
Bool_t ROMEMidasDAQ::ActualWriteODBOnline(ROMEStr2DArray* values, const char * dataBasePath,
                                          Long64_t runNumber, Long64_t eventNumber)
{
#if defined ( HAVE_MIDAS )
   if (gROME->isOnline()) {
      int i, iv;
      HNDLE hKey;
      char name[500];
      int type, num_values, item_size;
      int buffersize = fODBBufferSize;
      ROMEString odbPath;
      ROMEString errMsg;

      ROMEPath *path = new ROMEPath();
      try {
         // decode path
         if (!path->Decode(dataBasePath, runNumber, eventNumber)) {
            errMsg.SetFormatted("\nPath decode error : %s\n", dataBasePath);
            throw errMsg.Data();
         }
         for (i = 0;i < path->GetNumberOfTables(); i++) {
            odbPath += "/";
            odbPath += path->GetTableNameAt(i);
         }
         odbPath += "/";
         odbPath += path->GetFieldName();

         if (db_find_key(gROME->GetMidasOnlineDataBase(), 0, const_cast<char*>(odbPath.Data()), &hKey) != CM_SUCCESS) {
            errMsg.SetFormatted("\nCannot write to online database\n");
            throw errMsg.Data();
         }
         if (db_get_key_info(gROME->GetMidasOnlineDataBase(), hKey, name, sizeof(name), &type, &num_values, &item_size) != CM_SUCCESS) {
            errMsg.SetFormatted("\nCannot write to online database\n");
            throw errMsg.Data();
         }
         if (num_values * item_size > fODBBufferSize) {
            SetODBBufferSize(num_values * item_size);
            buffersize = fODBBufferSize;
         }
         if (path->GetFieldIndexAt(1) == -1) {
            // single field
            if (db_sscanf(const_cast<char*>(values->At(0, 0).Data()), fODBBuffer, &buffersize, 0, type) != CM_SUCCESS) {
               errMsg.SetFormatted("\nCannot write to online database\n");
               throw errMsg.Data();
            }
            if (db_set_data(gROME->GetMidasOnlineDataBase(), hKey, fODBBuffer, item_size, 1, type) != CM_SUCCESS) {
               errMsg.SetFormatted("\nCannot write to online database\n");
               throw errMsg.Data();
            }
         } else {
            // array field
            iv = 0;
            for (i = path->GetFieldIndexAt(0); i <= path->GetFieldIndexAt(1); i += path->GetFieldIndexAt(2)) {
               if (db_sscanf(const_cast<char*>(values->At(iv, 0).Data()), fODBBuffer, &buffersize, 0, type) != CM_SUCCESS) {
                  errMsg.SetFormatted("\nCannot write to online database\n");
                  throw errMsg.Data();
               }
               if (db_set_data_index(gROME->GetMidasOnlineDataBase(), hKey, fODBBuffer, item_size, i, type) != CM_SUCCESS) {
                  errMsg.SetFormatted("\nCannot write to online database\n");
                  throw errMsg.Data();
               }
               iv++;
            }
         }
      }
      catch(const char* err) {
         if (err) {
            ROMEPrint::Error("\n%s\n", err);
         }
         delete path;
         return kFALSE;
      }
      delete path;
   }
#else
   WarningSuppression(values);
   WarningSuppression(dataBasePath);
   WarningSuppression(runNumber);
   WarningSuppression(eventNumber);
#endif // HAVE_MIDAS
   return kTRUE;
}

//______________________________________________________________________________
Bool_t ROMEMidasDAQ::ReadODBOffline()
{
   if (gROME->isOffline()) {
      EVENT_HEADER *pevent = reinterpret_cast<EVENT_HEADER*>(this->GetRawDataEvent());
      bool readError = kFALSE;
      Long64_t posOld = -1;

      // store current position
      posOld = fMidasFile->CurrentPosition();

      Long_t n;
      n = fMidasFile->Read(pevent, sizeof(EVENT_HEADER));
      if (n < static_cast<Long_t>(sizeof(EVENT_HEADER))) {
         readError = kTRUE;
      } else {
#if !defined( R__BYTESWAP )
         ROMEUtilities::ByteSwap(&pevent->event_id);
         ROMEUtilities::ByteSwap(&pevent->trigger_mask);
         ROMEUtilities::ByteSwap(&pevent->serial_number);
         ROMEUtilities::ByteSwap(&pevent->time_stamp);
         ROMEUtilities::ByteSwap(&pevent->data_size);
#endif
         n = 0;
         if (pevent->data_size <= 0) {
            readError = kTRUE;
         } else {
            if (fMaxEventSize < static_cast<Long_t>(sizeof(EVENT_HEADER) + pevent->data_size)) {
               ROMEPrint::Error("The MIDAS event buffer size (%lu) is smaller than the size of ODB (%lu).\n"
                                "Please increase it by setting your environment variable MIDAS_MAX_EVENT_SIZE.\n"
                                "Then recompile after removing obj/ROMEMidasDAQ.o\n",
                                fMaxEventSize, sizeof(EVENT_HEADER) + pevent->data_size);
               SetRawDataEventSize(2 * static_cast<Long_t>(sizeof(EVENT_HEADER) + pevent->data_size));
            }
            n = fMidasFile->Read(pevent + 1, pevent->data_size);
            if (n != static_cast<Long_t>(pevent->data_size)) {
               readError = kTRUE;
            }
            //            if (static_cast<int>(reinterpret_cast<BANK_HEADER*>(pevent + 1)->data_size) <= 0) readError = kTRUE;
         }
      }
      // check input
      if (readError) {
         if (n > 0) {
            ROMEPrint::Warning("Unexpected end of file\n");
         }
         this->SetEndOfRun();
         return kTRUE;
      }

      // Get Handle to ODB header
      fCurrentSeqNumber = 0;
      if (fSeqNumToFilePos->GetSize() < 1) {
         fSeqNumToFilePos->Set(kFilePositionsResizeIncrement);
         fSeqNumToEventNum->Set(kFilePositionsResizeIncrement);
         fEventNumToSeqNum->Set(kFilePositionsResizeIncrement);
      }
      if (pevent->event_id == EVENTID_BOR) {
         if (gROME->isDataBaseActive("ODB")) {
            static_cast<ROMEODBOfflineDataBase*>(gROME->GetDataBase("ODB"))->
                  SetBuffer(reinterpret_cast<char*>(pevent + 1));
         }
         fSeqNumToFilePos->AddAt(fMidasFile->CurrentPosition(), static_cast<Int_t>(fCurrentSeqNumber));
         fValidSeqNumber = 1;
      } else {
         if(posOld != -1) {
            fMidasFile->Seek(static_cast<Long_t>(posOld));
         }
         fSeqNumToFilePos->AddAt(posOld, static_cast<Int_t>(fCurrentSeqNumber));
         fValidSeqNumber = 1;
      }
      this->SetBeginOfRun();
   }
   return kTRUE;
}

#if !defined( HAVE_MIDAS )
/**
Swaps bytes from little endian to big endian or vice versa for a whole event.

An event contains a flag which is set by bk_init() to identify
the endian format of an event. If force is FALSE, this flag is evaluated and
the event is only swapped if it is in the "wrong" format for this system.
An event can be swapped to the "wrong" format on purpose for example by a
front-end which wants to produce events in a "right" format for a back-end
analyzer which has different byte ordering.
@param event pointer to data area of event
@param force If TRUE, the event is always swapped, if FALSE, the event
is only swapped if it is in the wrong format.
@return 1==event has been swap, 0==event has not been swapped.
*/
//______________________________________________________________________________
INT ROMEMidasDAQ::bk_swap(void *event, BOOL force) const
{
   BANK_HEADER  *pbh;
   BANK         *pbk;
   BANK32       *pbk32;
   char         name[ 4 ];
   void         *pdata;
   UShort_t     type;
   bool         b32;

   pbh = static_cast<BANK_HEADER*>(event);

   // only swap if flags in high 16-bit
   if (pbh->flags < 0x10000 && !force) {
      return 0;
   }

   // swap bank header
   ROMEUtilities::ByteSwap((UInt_t*)&pbh->data_size);
   ROMEUtilities::ByteSwap((UInt_t*)&pbh->flags);

   // check for 32bit banks
   b32 = ((pbh->flags & (1<<4)) > 0);

   pbk = reinterpret_cast<BANK*>(pbh + 1);
   pbk32 = reinterpret_cast<BANK32*>(pbk);

   // scan event
   const size_t pend = pbh->data_size + sizeof(BANK_HEADER);
   while (reinterpret_cast<size_t>(pbk) - reinterpret_cast<size_t>(pbh) < pend) {
      // swap bank header
      if (b32) {
         ROMEUtilities::ByteSwap(&pbk32->type);
         ROMEUtilities::ByteSwap(&pbk32->data_size);
         pdata = pbk32 + 1;
         type = static_cast<UShort_t>(pbk32->type);
         for (Int_t i = 0; i < 4; i++) {
            name[i] = pbk32->name[i];
         }
      } else {
         ROMEUtilities::ByteSwap(&pbk->type);
         ROMEUtilities::ByteSwap(&pbk->data_size);
         pdata = pbk + 1;
         type = pbk->type;
         for (Int_t i = 0; i < 4; i++) {
            name[i] = pbk->name[i];
         }
      }

      // pbk points to next bank
      if (b32) {
         pbk32 = reinterpret_cast<BANK32*>(reinterpret_cast<char*>(pbk32 + 1) + ALIGN8(pbk32->data_size));
         pbk = reinterpret_cast<BANK*>(pbk32);
      } else {
         pbk = reinterpret_cast<BANK*>(reinterpret_cast<char*>(pbk + 1) + ALIGN8(pbk->data_size));
         pbk32 = reinterpret_cast<BANK32*>(pbk);
      }

      switch (type) {
      case TID_WORD:
      case TID_SHORT:
         while (reinterpret_cast<size_t>(pdata) < reinterpret_cast<size_t>(pbk)) {
            ROMEUtilities::ByteSwap(static_cast<UShort_t*>(pdata));
            pdata = static_cast<void*>(static_cast<UShort_t*>(pdata) + 1);
         }
         break;
      case TID_DWORD:
      case TID_INT:
      case TID_BOOL:
      case TID_FLOAT:
         while (reinterpret_cast<size_t>(pdata) < reinterpret_cast<size_t>(pbk)) {
            ROMEUtilities::ByteSwap(static_cast<UInt_t*>(pdata));
            pdata = static_cast<void*>(static_cast<UInt_t*>(pdata) + 1);
         }
         break;
      case TID_DOUBLE:
         while (reinterpret_cast<size_t>(pdata) < reinterpret_cast<size_t>(pbk)) {
            ROMEUtilities::ByteSwap(static_cast<ULong64_t*>(pdata));
            pdata = static_cast<void*>(static_cast<ULong64_t*>(pdata) + 1);
         }
         break;
      case TID_STRUCT:
         while (reinterpret_cast<size_t>(pdata) < reinterpret_cast<size_t>(pbk)) {
            pdata = ByteSwapStruct( &name[0], pdata );
         }
         break;
      }
   }
   return CM_SUCCESS;
}

#endif

//______________________________________________________________________________
void ROMEMidasDAQ::StartOnlineCommunication(ROMEMidasDAQ* localThis)
{
   if (localThis->fOnlineThread) {
      if (!localThis->fOnlineHandlerThread) {
         localThis->fOnlineConnection = kTRUE;
         localThis->fOnlineHandlerThread = new TThread("ODBHandle", ROMEMidasDAQ::OnlineConnectionLoop, localThis);
         localThis->fOnlineHandlerThread->Run();
      }
   } else {
      if (!localThis->fOnlineHandlerTimer) {
         ConnectExperiment(localThis);
         InitOnlineCommunication(localThis);
         localThis->fOnlineHandlerTimer = new TTimer(100);
         localThis->fOnlineHandlerTimer->Connect("Timeout()", "ROMEMidasDAQ", localThis, "CheckTransition()");
         localThis->fOnlineHandlerTimer->TurnOn();
      }
   }
}

//______________________________________________________________________________
void ROMEMidasDAQ::StopOnlineCommunication(ROMEMidasDAQ* localThis)
{
#if defined( HAVE_MIDAS )
   if (localThis->fOnlineHandlerThread) {
      localThis->fOnlineConnection = kFALSE;
      localThis->fOnlineHandlerThread->Join();
      SafeDelete(localThis->fOnlineHandlerThread);
   }
   if (localThis->fOnlineHandlerTimer) {
      SafeDelete(localThis->fOnlineHandlerTimer);
      cm_disconnect_experiment();
   }
#else
   WarningSuppression(localThis);
#endif
}

//______________________________________________________________________________
THREADTYPE ROMEMidasDAQ::OnlineConnectionLoop(void *arg)
{
#if defined( HAVE_MIDAS )
   ROMEMidasDAQ *localThis = static_cast<ROMEMidasDAQ*>(arg);

   ConnectExperiment(localThis);
   InitOnlineCommunication(localThis);
   Bool_t ret;

   while (localThis->fOnlineConnection) {
      CheckTransition();
      switch (fgOnlineThreadRequest) {
      case kCheckTransition:
         CheckTransition();
         break;
      case kReadEvent:
         ret = ReadOnlineEvent(localThis);
         {
            ROME_LOCKGUARD(fgMutex);
            fgReadEventReturn = ret;
         }
         break;
      case kReadEventDummy:
         ReadOnlineEventDummy(localThis);
         break;
      case kReadDB:
         ret = localThis->ActualReadODBOnline(fgDBValues, fgDBPath, fgDBRunNumber, fgDBEventNumber);
         {
            ROME_LOCKGUARD(fgMutex);
            fgDBReadReturn = ret;
         }
         break;
      case kWriteDB:
         ret = localThis->ActualWriteODBOnline(fgDBValues, fgDBPath, fgDBRunNumber, fgDBEventNumber);
         {
            ROME_LOCKGUARD(fgMutex);
            fgDBWriteReturn = ret;
         }
         break;
      case kFlushBuffer:
         localThis->FlushOnlineBuffer(localThis);
         break;
      default:
         break;
      };
      if (fgOnlineThreadRequest) {
         ROME_LOCKGUARD(fgMutex);
         fgOnlineThreadRequest = kNoRequest;
      }
   }

   cm_disconnect_experiment();
#else
   WarningSuppression(arg)
#endif

   return THREADRETURN;
}

//______________________________________________________________________________
Bool_t ROMEMidasDAQ::ConnectExperiment(ROMEMidasDAQ *localThis)
{
   // Connect to the experiment
#if defined( HAVE_MIDAS )
   if (cm_connect_experiment(const_cast<char*>(gROME->GetOnlineHost()),
                             const_cast<char*>(gROME->GetOnlineExperiment()),
                             const_cast<char*>(gROME->GetOnlineAnalyzerName()), 0) != SUCCESS) {
      ROMEPrint::Error("\nCan not connect to experiment\n");
      return kFALSE;
   }

   cm_start_watchdog_thread();
   ss_ctrlc_handler(ROMERint::InterruptHandlerInt);

   INT requestId;
   Int_t i;

   // open the "system" buffer, 1M size
   bm_open_buffer(const_cast<char*>(gROME->GetOnlineMemoryBuffer()), 2 * localThis->fMaxEventSize, &localThis->fMidasOnlineBuffer);

   // set the buffer cache size
   bm_set_cache_size(localThis->fMidasOnlineBuffer, 100000, 0);

   // place a request for a specific event id
   if (localThis->GetNumberOfEventRequests() <= 0) {
#if 0 // Is no event request invalid ?
      ROMEPrint::Error("\nNo Events Requests for online mode!\n");
      ROMEPrint::Error("\nPlace Events Requests into the ROME configuration file.\n");
      return kFALSE;
#endif
   }
   const int nRequest = localThis->GetNumberOfEventRequests();
   for (i = 0; i < nRequest; i++) {
      if (localThis->GetEventRequestRate(i) == 1) {
         localThis->fRequestAll = kTRUE;
      }
      bm_request_event(localThis->fMidasOnlineBuffer, localThis->GetEventRequestID(i),
                       localThis->GetEventRequestMask(i), localThis->GetEventRequestRate(i), &requestId, 0);
   }

   // place a request for system messages
   cm_msg_register(ProcessMessage);

   // turn off watchdog if in debug mode
#if defined( MIDAS_DEBUG )
   cm_set_watchdog_params(TRUE, 0);
#endif

   // Registers a callback function for run transitions.
   if (!gROME->IsStandAloneARGUS() && !gROME->IsROMEMonitor()) {
      if(cm_register_transition(TR_START, 0 ,500) != CM_SUCCESS ||
         cm_register_transition(TR_STOP, 0, 500) != CM_SUCCESS) {
         ROMEPrint::Error("\nCan not connect to experiment\n");
         return kFALSE;
      }
   }

   // Connect to the online database
   if (cm_get_experiment_database(gROME->GetMidasOnlineDataBasePointer(), 0)!= CM_SUCCESS) {
      ROMEPrint::Error("\nCan not connect to the online database\n");
      return kFALSE;
   }
#else
   WarningSuppression(localThis);
#endif

   return kTRUE;
}

//______________________________________________________________________________
Bool_t ROMEMidasDAQ::InitOnlineCommunication(ROMEMidasDAQ *localThis)
{
#if defined( HAVE_MIDAS )
      // Check Run Status
      int state = 0;
      int statesize = sizeof(state);

      if (db_get_value(gROME->GetMidasOnlineDataBase(), 0, const_cast<char*>("/Runinfo/State"), &state,
                       &statesize, TID_INT, kFALSE) != CM_SUCCESS) {
         ROMEPrint::Error("\nCan not read run status from the online database\n");
         return kFALSE;
      }
      if (state != 3) {
         ROME_LOCKGUARD(fgMutex);
         fgEventStatus = kBeginOfRun;
         fgRunStatus = kStopped;
      }

      // Get Runnumber
      Int_t runNumber = 0;
      Int_t size = sizeof(runNumber);
      if (db_get_value(gROME->GetMidasOnlineDataBase(), 0, const_cast<char*>("/Runinfo/Run number"),
                       &runNumber, &size, TID_INT, kFALSE) != CM_SUCCESS) {
         ROMEPrint::Error("\nCan not read runnumber from the online database\n");
         return kFALSE;
      }
      {
         ROME_LOCKGUARD(fgMutex);
         fgCurrentRunNumber = runNumber;
      }

      // Initialize the online database
      HNDLE hKey;
      ROMEString str;
      // Tree Switches
      const Int_t nTree = gROME->GetTreeObjectEntries();
      Int_t i;
      for (i = 0; i <nTree; i++) {
         str = "//Tree switches/";
         str.Insert(1, gROME->GetOnlineAnalyzerName());
         str.Append(gROME->GetTreeObjectAt(i)->GetTree()->GetName());
         if (gROME->GetReadConfigFromODB()) {
            db_create_record(gROME->GetMidasOnlineDataBase(), 0, const_cast<char*>(str.Data()),
                  const_cast<char*>(gROME->GetTreeObjectAt(i)->GetSwitchesString()));
         } else {
            db_check_record(gROME->GetMidasOnlineDataBase(), 0, const_cast<char*>(str.Data()),
                  const_cast<char*>(gROME->GetTreeObjectAt(i)->GetSwitchesString()), TRUE);
         }
         db_find_key(gROME->GetMidasOnlineDataBase(), 0, const_cast<char*>(str.Data()), &hKey);
         if (!gROME->GetReadConfigFromODB()) {
            if (db_set_record(gROME->GetMidasOnlineDataBase(), hKey, gROME->GetTreeObjectAt(i)->GetSwitches(),
                     gROME->GetTreeObjectAt(i)->GetSwitchesSize(), 0) != DB_SUCCESS) {
               ROMEPrint::Warning("\nCan not write to tree switches record.\n");
            }
         }
         if (db_open_record(gROME->GetMidasOnlineDataBase(), hKey, gROME->GetTreeObjectAt(i)->GetSwitches(),
                            gROME->GetTreeObjectAt(i)->GetSwitchesSize(), MODE_READ, 0, 0) != DB_SUCCESS) {
            ROMEPrint::Warning("\nCan not open tree switches record, probably other analyzer is using it\n");
         }
      }

      // Experiment dependent ODB initializations
      localThis->InitODB();
      return kTRUE;
#else
      ROMEPrint::Error("Need Midas support for Online Mode !!\n");
      ROMEPrint::Error("Please link the midas library into this project.\n");
      ROMEPrint::Error("--> Run the ROMEBuilder with the '-midas' option.\n");
      WarningSuppression(localThis)
      return kFALSE;
#endif
}

//______________________________________________________________________________
Bool_t ROMEMidasDAQ::ReadOnlineEvent(ROMEMidasDAQ *localThis)
{
#if defined( HAVE_MIDAS )
   INT size;
   void* mEvent;
   int status;

   size = localThis->GetRawDataEventSize();
   mEvent = localThis->GetRawDataEvent();
#ifdef BM_NO_WAIT
   status = bm_receive_event(localThis->fMidasOnlineBuffer, mEvent, &size, BM_NO_WAIT);
#else
   status = bm_receive_event(localThis->fMidasOnlineBuffer, mEvent, &size, ASYNC);
#endif
   if (status != BM_SUCCESS) {
      return kFALSE;
   }
#else
   WarningSuppression(localThis)
#endif
   return kTRUE;
}

//______________________________________________________________________________
void ROMEMidasDAQ::DummyRead()
{
#if defined( HAVE_MIDAS )
   if (fOnlineThread) {
      if (!fOnlineHandlerThread) {
         return;
      }
      {
         ROME_LOCKGUARD(fgMutex);
         fgOnlineThreadRequest = kReadEventDummy;
      }
      while (fgOnlineThreadRequest) {
         // wait until thread finish process
         gSystem->Sleep(1);
      }
   } else {
      ReadOnlineEventDummy(this);
   }
#else
#endif
}

//______________________________________________________________________________
void ROMEMidasDAQ::ReadOnlineEventDummy(ROMEMidasDAQ *localThis)
{
#if defined( HAVE_MIDAS )
   INT size = localThis->GetRawDataEventSize();
#ifdef BM_NO_WAIT
   bm_receive_event(localThis->fMidasOnlineBuffer, localThis->fRawDataDummy, &size, BM_NO_WAIT);
#else
   bm_receive_event(localThis->fMidasOnlineBuffer, localThis->fRawDataDummy, &size, ASYNC);
#endif
#else
   WarningSuppression(localThis)
#endif
}

//______________________________________________________________________________
void ROMEMidasDAQ::FlushBuffer()
{
#if defined( HAVE_MIDAS )
   if (fOnlineThread) {
      if (!fOnlineHandlerThread) {
         return;
      }
      {
         ROME_LOCKGUARD(fgMutex);
         fgOnlineThreadRequest = kFlushBuffer;
      }
      while (fgOnlineThreadRequest) {
         // wait until thread finish process
         gSystem->Sleep(1);
      }
   } else {
      FlushOnlineBuffer(this);
   }
#else
#endif
}

//______________________________________________________________________________
void ROMEMidasDAQ::FlushOnlineBuffer(ROMEMidasDAQ *localThis)
{
#if defined( HAVE_MIDAS )
   INT size;
   INT status;
   do {
      size = localThis->GetRawDataEventSize();
#ifdef BM_NO_WAIT
      status = bm_receive_event(localThis->fMidasOnlineBuffer, localThis->fRawDataDummy, &size, BM_NO_WAIT);
#else
      status = bm_receive_event(localThis->fMidasOnlineBuffer, localThis->fRawDataDummy, &size, ASYNC);
#endif
   } while(status == BM_SUCCESS);
#else
   WarningSuppression(localThis)
#endif
}

//______________________________________________________________________________
Bool_t ROMEMidasDAQ::CheckTransition()
{
#if defined( HAVE_MIDAS )
   ROMEMidasDAQ *localThis = static_cast<ROMEMidasDAQ*>(gROME->GetActiveDAQ());

   INT size;
   void* mEvent;
   int status;
   int runNumber,trans; // use int instead of INT or Int_t

   static Long_t selfId = 0;
   if (!selfId) {
      // this is the first time to be called
      selfId = TThread::SelfId();
   }
   if (selfId != TThread::SelfId()) {
      // ODB communication must be done in a thread
      return kFALSE;
   }

   // ODB update
   db_send_changed_records();

   if (cm_query_transition(&trans, &runNumber, 0)) {
      if (trans == TR_START) {
         ROME_LOCKGUARD(fgMutex);
         fgCurrentRunNumber = runNumber;
         fgEventStatus = kBeginOfRun;
         fgRunStatus = kRunning;
         return kTRUE;
      }
      if (trans == TR_STOP) {
         if (localThis->fRequestAll) {
            INT numberOfBytes;
            bm_get_buffer_level(localThis->fMidasOnlineBuffer, &numberOfBytes);
            while (numberOfBytes <= 0) {
               size = localThis->GetRawDataEventSize();
               mEvent = localThis->GetRawDataEvent();
#ifdef BM_NO_WAIT
               status = bm_receive_event(localThis->fMidasOnlineBuffer, mEvent, &size, BM_NO_WAIT);
#else
               status = bm_receive_event(localThis->fMidasOnlineBuffer, mEvent, &size, ASYNC);
#endif
               if (status != BM_SUCCESS) {
                  break;
               }
               bm_get_buffer_level(localThis->fMidasOnlineBuffer, &numberOfBytes);
            }
         }
         ROME_LOCKGUARD(fgMutex);
         fgEventStatus = kEndOfRun;
         fgRunStatus = kStopped;
         return kTRUE;
      }
   }

   status = cm_yield(fgOnlineLoopPeriod > 0 ? fgOnlineLoopPeriod : 1000);

   if (status == RPC_SHUTDOWN || status == SS_ABORT) {
      cm_disconnect_experiment(); // this is needed because calling thead can not cancel itself in ROMERint::Terminate => ROMEAnalyzer::Cleaning
      gROME->GetApplication()->Terminate(1);
   }
   if (localThis->isStopped() && !localThis->isEndOfRun()) {
      localThis->SetContinue();
      return kTRUE;
   }
#endif
   return kTRUE;
}

//______________________________________________________________________________
void ROMEMidasDAQ::SetODBBufferSize(Int_t size)
{
   if (size > 0) {
      fODBBufferSize = size;
      SafeDeleteArray(fODBBuffer);
      fODBBuffer = new char[fODBBufferSize];
   }
}

//______________________________________________________________________________
void ROMEMidasDAQ::SetOnlineLoopPeriod(Int_t period)
{
#if defined( HAVE_MIDAS )
   fgOnlineLoopPeriod = period;
#else
   WarningSuppression(period);
#endif
}

//______________________________________________________________________________
Int_t ROMEMidasDAQ::GetOnlineLoopPeriod() const
{
#if defined( HAVE_MIDAS )
   return fgOnlineLoopPeriod;
#else
   return kMidasInitialOnlineLoopPeriod;
#endif
}

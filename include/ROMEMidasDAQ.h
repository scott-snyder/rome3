/********************************************************************
  ROMEMidasDAQ.h, M. Schneebeli PSI

  $Id: ROMEMidasDAQ.h 2615 2012-09-04 19:09:39Z sawada $

********************************************************************/
#ifndef ROMEMidasDAQ_H
#define ROMEMidasDAQ_H

#include <RConfig.h>
#include <TArrayL64.h>
#include "ROMEDAQSystem.h"
#include "ROMEMidasFile.h"

const Int_t kRawDataEvents                = 8;
const Int_t kMidasInitialOnlineLoopPeriod = 10;

class TThread;
class TTimer;
class ROMEStr2DArray;

class ROMEMidasDAQ : public ROMEDAQSystem {
protected:
   Long_t        fMaxEventSize;                            //! Max midas event size
   TString       fFileName;
   Bool_t        fByteSwap;
   char         *fRawDataEvent[kRawDataEvents];            //! Midas Inputdata Stack
   char         *fRawDataDummy;                            //! Buffer for dummy read
   Int_t         fNumberOfRawDataEvent;                    //! Number of Midas Inputdata Stack
   Int_t         fCurrentRawDataEvent;                     //! Index of the current event buffer
   Int_t         fValidRawDataEvent;                       //! Buffer which is filled already
   Bool_t        fReadExistingRawData;                     //! Flag to use event buffer instead of reading new evnet

   Int_t         fNumberOfEventRequests;                   //! Number of Event Requests
   Short_t      *fEventRequestID;                          //! IDs of Event Requests
   Short_t      *fEventRequestMask;                        //! Trigger Masks of Event Requests
   Int_t        *fEventRequestRate;                        //! Sampling Rates of Event Requests

//   HNDLE         fMidasOnlineDataBase;                     //! Handle to the Midas Online Data Base (Online)
   INT            fMidasOnlineBuffer;                      //! Midas Online Buffer
   ROMEMidasFile *fMidasFile;                              //! Handle to a midas file
   Bool_t         fRequestAll;                             //! True if one request type requests all events

   EVENT_HEADER *fOdbOffline;                              //! Handle to the Midas Online Data Base (Offline)

   Int_t         fTimeStamp;                               //! Current time stamp

   TArrayL64*    fSeqNumToFilePos;                         //! Sequential number => file position
   TArrayL64*    fSeqNumToEventNum;                        //! Sequential number => trigger event number
   TArrayL64*    fEventNumToSeqNum;                        //! Trigger event number => sequential number
   Long64_t      fValidSeqNumber;                          //! Max number of valid index of fSeqNumToXXX
   Long64_t      fValidEventNumber;                        //! Max number of valid index of fEventNumToSeqNum
   Long64_t      fCurrentSeqNumber;                        //! Current position in file
   Long64_t      fMaxDataSeqNumber;                        //! Maximum number of sequential number of data events in the run.

   Bool_t        fByteSwapFlagMightBeWrong;                //! Flag if <MidasByteSwap> might be wrongly specified.
   TThread      *fOnlineHandlerThread;                     //! Thread to handle online connection
   TTimer       *fOnlineHandlerTimer;                      //! Timer to handle online connection
   Bool_t        fOnlineConnection;                        //! Flag if onine connection should be alive in thread
   Bool_t        fOnlineThread;                            //! Flag if using thread for online communication
   Int_t         fODBBufferSize;                           //! Buffer size for ODB online database
   char         *fODBBuffer;                               //! Buffer for ODB online database

private:
   ROMEMidasDAQ(const ROMEMidasDAQ &daq); // not implemented
   ROMEMidasDAQ &operator=(const ROMEMidasDAQ &rhs); // not implemented

public:
   ROMEMidasDAQ();
   virtual ~ROMEMidasDAQ();

   // Online Database
//   HNDLE          GetMidasOnlineDataBase() const { return fMidasOnlineDataBase; }
//   HNDLE         *GetMidasOnlineDataBasePointer() { return &fMidasOnlineDataBase; }

   // Raw Data
   void          *GetRawDataEvent() {
      if (fCurrentRawDataEvent < 0) {
         fCurrentRawDataEvent = 0;
      }
      return fRawDataEvent[fCurrentRawDataEvent];
   }
   void          *GetLastRawDataEvent() {
      Int_t ev = (fCurrentRawDataEvent - 1) % fNumberOfRawDataEvent;
      if (ev < 0) { ev += fNumberOfRawDataEvent; }
      return fRawDataEvent[ev];
   }
   size_t         GetRawDataEventSize() const { return fMaxEventSize; }
   void           SwitchRawDataBuffer() { fCurrentRawDataEvent = (fCurrentRawDataEvent + 1) % fNumberOfRawDataEvent; }
   void           SetRawDataEventSize(Long_t size);

   // Event Requests
   Int_t          GetNumberOfEventRequests() const { return fNumberOfEventRequests; }
   Short_t        GetEventRequestID(Int_t i) const { return fEventRequestID[i]; }
   Short_t        GetEventRequestMask(Int_t i) const { return fEventRequestMask[i]; }
   Int_t          GetEventRequestRate(Int_t i) const { return fEventRequestRate[i]; }

   void           SetNumberOfEventRequests(Int_t value) { fNumberOfEventRequests = value; }
   void           SetEventRequestID(Int_t i,Short_t value)    { fEventRequestID[i] = value; }
   void           SetEventRequestMask(Int_t i,Short_t value)  { fEventRequestMask[i] = value; }
   void           SetEventRequestRate(Int_t i,Int_t value)  { fEventRequestRate[i] = value; }

   // File name
   void           SetFileName(const char* filename) { fFileName = filename; }
   const char*    GetFileName() const { return fFileName.Data(); }

   // Byte swap flag
   void           SetByteSwap(Bool_t flag = kTRUE) { fByteSwap = flag; }
   Bool_t         GetByteSwap() const { return fByteSwap; }

   // Additional Getters
   Int_t          GetTimeStamp() { return fTimeStamp; }
   const char    *GetName() const { return "midas"; }

   ROMEMidasFile* GetMidasFile() const { return fMidasFile; }

   Bool_t         Init();
   Bool_t         BeginOfRun();
   Long64_t       Seek(Long64_t event);
   Long64_t       StepEvent(Bool_t forward = kTRUE);
   Bool_t         Event(Long64_t event);
   Bool_t         EndOfRun();
   Bool_t         Terminate();
   void           DummyRead();
   void           FlushBuffer();
   void           FlushOnlineBuffer(ROMEMidasDAQ *localThis);

   Bool_t         ReadODBOffline();
   Bool_t         ReadODBOnline(ROMEStr2DArray *values, const char *dataBasePath,
                                Long64_t runNumber, Long64_t eventNumber);
   Bool_t         WriteODBOnline(ROMEStr2DArray *values, const char *dataBasePath,
                                 Long64_t runNumber, Long64_t eventNumber);
   Bool_t         ActualReadODBOnline(ROMEStr2DArray *values, const char *dataBasePath,
                                      Long64_t runNumber, Long64_t eventNumber);
   Bool_t         ActualWriteODBOnline(ROMEStr2DArray *values, const char *dataBasePath
                                       , Long64_t runNumber, Long64_t eventNumber);


   virtual Bool_t IsActiveEventID(Int_t /*id*/) const { return true; }
#if defined( HAVE_MIDAS )
   virtual Bool_t InitODB() = 0;
#endif // HAVE_MIDAS
   virtual Bool_t InitHeader() = 0;
   virtual void   InitMidasBanks() = 0;

   //byte swapping
#if !defined HAVE_MIDAS && !defined ( __MAKECINT__ )
   INT            bk_swap(void *event, BOOL force) const;
#endif
   virtual void  *ByteSwapStruct( char* /*aName*/, void* aData ) const { return aData; } // Must be overwritten by analyzer midas code.

   // online connection
   void              SetOnlineThread(Bool_t flag) { fOnlineThread = flag; }
   Bool_t            IsOnlineThread() const { return fOnlineThread; }

   void              SetOnlineLoopPeriod(Int_t period);
   Int_t             GetOnlineLoopPeriod() const;

   static THREADTYPE OnlineConnectionLoop(void *arg);
   static Bool_t     ConnectExperiment(ROMEMidasDAQ *localThis);
   static Bool_t     CheckTransition();
   static Bool_t     ReadOnlineEvent(ROMEMidasDAQ *localThis);
   static void       ReadOnlineEventDummy(ROMEMidasDAQ *localThis);
   static void       StartOnlineCommunication(ROMEMidasDAQ *localThis);
   static void       StopOnlineCommunication(ROMEMidasDAQ *localThis);
   static Bool_t     InitOnlineCommunication(ROMEMidasDAQ *localThis);

   Int_t             GetODBBufferSize() const { return fODBBufferSize; }
   void              SetODBBufferSize(Int_t size);

   ClassDef(ROMEMidasDAQ, 0) // Base DAQ class for Midas system
};

#endif   // ROMEMidasDAQ_H

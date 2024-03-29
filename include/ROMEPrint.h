/********************************************************************
  ROMEPrint.h, M. Schneebeli PSI

  $Id: ROMEPrint.h 2562 2010-08-02 19:02:18Z sawada $

********************************************************************/
#ifndef ROMEPrint_H
#define ROMEPrint_H

#include <RConfig.h>
#include <Rtypes.h>
#include <Varargs.h>
#include "ROMEAnalyzer.h"
#include "ROME.h"

// Alarm types.
// This must same with definition in midas.h
const Int_t kATInternal  = 1;
const Int_t kATProgram   = 2;
const Int_t kATEvaluated = 3;
const Int_t kATPeriodic  = 4;

namespace ROMEPrint
{
   // Verbose level
   enum {
      kMute    = 0,
      kError   = 1,
      kWarning = 2,
      kNormal  = 3,
      kVerbose = 4,
      kDebug   = 5
   };
   const char* const kReportLevelLetter = "MEWIVD";

   extern Int_t gVerboseLevel;   // verbose level
   extern Int_t gReportMaxCount; // max number of printing for a report

   // Output
#ifndef __MAKECINT__
   void  PrintAlways(const char *msgfmt, ...) G_GNUC_PRINTF(1, 2);
   void  Debug(const char *msgfmt, ...)       G_GNUC_PRINTF(1, 2);
   void  Print(const char *msgfmt, ...)       G_GNUC_PRINTF(1, 2);
   void  Info(const char *msgfmt, ...)        G_GNUC_PRINTF(1, 2);
   void  Warning(const char *msgfmt, ...)     G_GNUC_PRINTF(1, 2);
   void  Error(const char *msgfmt, ...)       G_GNUC_PRINTF(1, 2);

   Int_t  Alarm(const char *name, const char *message, const char *alarmClass, const char *cond, Int_t type);
   void  Report(const Int_t verboseLevel, const char* fileName, const char *funcName, Int_t lineNumber,
                const Long64_t run, const Long64_t event, const Bool_t printHeader, const char* va_(fmt),...) G_GNUC_PRINTF(8, 9);
#endif
   void  ReportSummary(Int_t level = -1, std::ostream* out = 0, Int_t width = 76);

   inline Int_t GetVerboseLevel() { return gVerboseLevel; }
   inline void  SetVerboseLevel(Int_t level = kNormal) { gVerboseLevel = level; }

   inline Int_t GetReportMaxCount() { return gReportMaxCount; }
   inline void  SetReportMaxCount(Int_t max = 20) { gReportMaxCount = max; }
}

// Report styles
//  R_ERROR   : Printed when gVerboseLevel >= kError
//  R_WARNING : Printed when gVerboseLevel >= kWarning
//  R_PLAIN   : Printed when gVerboseLevel >= kNormal, without line header (run number, event number, filename and line number)
//  R_INFO    : Printed when gVerboseLevel >= kNormal
//  R_VERBOSE : Printed when gVerboseLevel >= kVerbose
//  R_DEBUG   : Printed when gVerboseLevel >= kDebug

#if defined(R__UNIX)
#define R_ERROR   ROMEPrint::kError,   __FILE__, __func__, __LINE__, (gROME ? gROME->GetCurrentRunNumber() : 0), (gROME ? gROME->GetCurrentEventNumber() : 0),kTRUE
#define R_WARNING ROMEPrint::kWarning, __FILE__, __func__, __LINE__, (gROME ? gROME->GetCurrentRunNumber() : 0), (gROME ? gROME->GetCurrentEventNumber() : 0),kTRUE
#define R_PLAIN   ROMEPrint::kNormal,  __FILE__, __func__, __LINE__, (gROME ? gROME->GetCurrentRunNumber() : 0), (gROME ? gROME->GetCurrentEventNumber() : 0),kFALSE
#define R_INFO    ROMEPrint::kNormal,  __FILE__, __func__, __LINE__, (gROME ? gROME->GetCurrentRunNumber() : 0), (gROME ? gROME->GetCurrentEventNumber() : 0),kTRUE
#define R_VERBOSE ROMEPrint::kVerbose, __FILE__, __func__, __LINE__, (gROME ? gROME->GetCurrentRunNumber() : 0), (gROME ? gROME->GetCurrentEventNumber() : 0),kTRUE
#define R_DEBUG   ROMEPrint::kDebug,   __FILE__, __func__, __LINE__, (gROME ? gROME->GetCurrentRunNumber() : 0), (gROME ? gROME->GetCurrentEventNumber() : 0),kTRUE
#else
#define R_ERROR   ROMEPrint::kError,   __FILE__, __FUNCTION__, __LINE__, (gROME ? gROME->GetCurrentRunNumber() : 0), (gROME ? gROME->GetCurrentEventNumber() : 0),kTRUE
#define R_WARNING ROMEPrint::kWarning, __FILE__, __FUNCTION__, __LINE__, (gROME ? gROME->GetCurrentRunNumber() : 0), (gROME ? gROME->GetCurrentEventNumber() : 0),kTRUE
#define R_PLAIN   ROMEPrint::kNormal,  __FILE__, __FUNCTION__, __LINE__, (gROME ? gROME->GetCurrentRunNumber() : 0), (gROME ? gROME->GetCurrentEventNumber() : 0),kFALSE
#define R_INFO    ROMEPrint::kNormal,  __FILE__, __FUNCTION__, __LINE__, (gROME ? gROME->GetCurrentRunNumber() : 0), (gROME ? gROME->GetCurrentEventNumber() : 0),kTRUE
#define R_VERBOSE ROMEPrint::kVerbose, __FILE__, __FUNCTION__, __LINE__, (gROME ? gROME->GetCurrentRunNumber() : 0), (gROME ? gROME->GetCurrentEventNumber() : 0),kTRUE
#define R_DEBUG   ROMEPrint::kDebug,   __FILE__, __FUNCTION__, __LINE__, (gROME ? gROME->GetCurrentRunNumber() : 0), (gROME ? gROME->GetCurrentEventNumber() : 0),kTRUE

#endif

//using namespace ROMEPrint;

#endif   // ROMEPrint_H

// Author: Matthias Schneebeli
//////////////////////////////////////////////////////////////////////////
//
//  ROMEDAQSystem
//
//  Base class for DAQ systems.
//
//  $Id: ROMEDAQSystem.cpp 2410 2008-03-14 19:32:49Z yamada $
//
//////////////////////////////////////////////////////////////////////////

#include "ROMEDAQSystem.h"
#include "ROMEAnalyzer.h"

ClassImp(ROMEDAQSystem)

//______________________________________________________________________________
Bool_t ROMEDAQSystem::InitDAQ()
{
   fWatchEvent.Reset();
   fWatchAll.Reset();
   return Init();
}

//______________________________________________________________________________
Bool_t ROMEDAQSystem::BeginOfRunDAQ()
{
   Bool_t ret;
   fWatchAll.Start(false);
   ret = BeginOfRun();
   fWatchAll.Stop();
   return ret;
}

//______________________________________________________________________________
Bool_t ROMEDAQSystem::EventDAQ(Long64_t event)
{
   Bool_t ret;
   gROME->SetEventID(-1);
   fWatchAll.Start(false);
   fWatchEvent.Start(false);
   ret = Event(event);
   fWatchEvent.Stop();
   fWatchAll.Stop();
   return ret;
}

//______________________________________________________________________________
Bool_t ROMEDAQSystem::EndOfRunDAQ()
{
   Bool_t ret;
   fWatchAll.Start(false);
   ret = EndOfRun();
   fWatchAll.Stop();
   return ret;
}

//______________________________________________________________________________
Bool_t ROMEDAQSystem::TerminateDAQ()
{
   return Terminate();
}

//______________________________________________________________________________
Bool_t ROMEDAQSystem::TimeDAQ()
{
   int i;
   ROMEString str = GetName();
   ROMEPrint::Print(" DAQ %s", str.Data());
   for (i=0; i < 35 - 4 - 1 - str.Length(); i++) {
      ROMEPrint::Print(".");
   }
   ROMEString str1, str2;
   ROMEPrint::Print(" : %s  %s",
                    fWatchAll.GetRealTimeString(str1),
                    fWatchAll.GetCpuTimeString(str2));
   ROMEPrint::Print("  %s\n", fWatchEvent.GetCpuTimeString(str1));
   return true;
}

/********************************************************************

  main.cpp, M. Schneebeli PSI

  $Id: main.cpp 2401 2008-03-12 18:41:28Z sawada $

********************************************************************/
#include <stdlib.h>
#include <RConfig.h>
#if defined( R__VISUAL_CPLUSPLUS )
#pragma warning( push )
#pragma warning( disable : 4800 )
#endif // R__VISUAL_CPLUSPLUS
#include <TROOT.h>
#include <TSystem.h>
#if defined( R__VISUAL_CPLUSPLUS )
#pragma warning( pop )
#endif // R__VISUAL_CPLUSPLUS

#if defined( R__UNIX )
#   include <unistd.h>
#endif
#ifndef R__MACOSX
#   if defined( R__VISUAL_CPLUSPLUS )
#      include <io.h>
#   endif
#   if defined( R__UNIX )
#      include <sys/io.h>
#   endif
#endif
#include <Riostream.h>
#include "ROMEBuilder.h"
#include "ROMEVersion.h"

using namespace std;

//______________________________________________________________________________
int main(int argc, const char *argv[])
{
   char* romesys = getenv("ROMESYS");
   if (romesys==NULL) {
      cout << "Please set the environment variable ROMESYS to the ROME root-directory." << endl;
      return 1;
   }

   gROOT->SetBatch();
   ROMEBuilder* romeb = new ROMEBuilder();

   romeb->romeVersion = "Version ";
   romeb->romeVersion += ROME_RELEASE;
   romeb->romeDescription = ROME_DESCRIPTION;
   romeb->romeStable = (ROME_STABLE == 1);

   ROMEString xmlFile = "";

   if (!romeb->ReadCommandLineParameters(argc, argv)) {
      return 1;
   }

   if (!romeb->CheckFileAndPath()) {
      return 1;
   }

   if (!romeb->StartBuilder()) {
      return 1;
   }

   bool noLink = romeb->GetNoLink();
   bool makeOutput = romeb->GetMakeOutput();
   ROMEString *makeFlag = romeb->GetMakeFlag();
   ROMEString makeCommand;
#if defined( R__UNIX )
   makeCommand = "make";
#else
   makeCommand = "nmake -f Makefile.win";
#endif
   if (makeFlag->Length()) {
      makeCommand += " ";
      makeCommand += makeFlag->Data();
   }

   delete romeb;

   if (!noLink) {
      if(makeOutput) {
         cout<<"Linking the executable binary."<<endl;
      }
#if defined( R__UNIX )
      return (gSystem->Exec(makeCommand.Data())) ? EXIT_FAILURE : EXIT_SUCCESS;
#endif
#if defined( R__VISUAL_CPLUSPLUS )
      return gSystem->Exec(makeCommand.Data());
#endif
   }

   return 0;
}

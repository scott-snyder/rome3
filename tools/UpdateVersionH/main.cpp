/********************************************************************

  main.cpp, Ryu Sawada

********************************************************************/
#include <vector>
#include <memory>
#include <Riostream.h>
#include <RConfig.h>
#if defined( R__VISUAL_CPLUSPLUS )
#pragma warning( push )
#pragma warning( disable : 4800 4244)
#endif // R__VISUAL_CPLUSPLUS
#include <TSystem.h>
#include <TTimeStamp.h>
#if defined( R__VISUAL_CPLUSPLUS )
#pragma warning( pop )
#endif // R__VISUAL_CPLUSPLUS
#include "ROMEString.h"
#include "ROMEXML.h"

using namespace std;

//------
// change following values when release
const Int_t romeMajor       =  3; // format must be changed when it is larger than 9
const Int_t romeMinor       =  2; // must be less than 100
const Int_t romePatch       =  15; // must be less than 100
const Int_t isStableVersion =  0; // 0: development version, 1: stable version
//------

/*
const char* const monthName[] = {
   "Jan"
   , "Feb"
   , "Mar"
   , "Apr"
   , "May"
   , "Jun"
   , "Jul"
   , "Aug"
   , "Sep"
   , "Oct"
   , "Nov"
   , "Dec"
};
*/

Int_t GetROMEVersion     (Int_t a, Int_t b)            { return (a <<  8) + b; }
Int_t GetROMEVersionPatch(Int_t a, Int_t b, Int_t c=0) { return (a << 16) + (b << 8) +c; }

int main()
{
   TTimeStamp timestamp;
   timestamp.Set();
   Int_t revisionNumber = 0; // This is SVN revision number used in rome2, always 0 in rome3

   // Read Git description
   ROMEString desc;

   if (!gSystem->AccessPathName(".git", kFileExists)) {
      desc.ReadCommandOutput("git describe --dirty --tags", false, true);
   }
   if (desc.Length()) {
   } else {
      desc = "";
   }

   ROMEString hfile = "$(ROMESYS)/include/";
   gSystem->ExpandPathName(hfile);
   hfile.AppendFormatted("ROMEVersion.h");

   //
   // Reading ROMEVersion.h
   //
   ROMEString fileBuffer;
   ifstream originalFile(hfile.Data());
   if (originalFile.good()) {
      fileBuffer.ReadFile(originalFile);
   }
   originalFile.close();

   //
   // Writing ROMEVersion.h
   //
   ROMEString buffer;

   // current time
   UInt_t year;
   UInt_t month;
   UInt_t day;
   UInt_t hour;
   UInt_t min;
   UInt_t sec;
   timestamp.Set();
   timestamp.GetDate(kTRUE, 0, &year, &month, &day);
   timestamp.GetTime(kTRUE, 0, &hour, &min, &sec);

   // prepare new file.
   buffer.Resize(0);
   buffer.AppendFormatted("#ifndef ROMEVersion\n");
   buffer.AppendFormatted("#define ROMEVersion\n");
   buffer.AppendFormatted("\n");
   buffer.AppendFormatted("/* Version information automatically generated by installer. */\n");
   buffer.AppendFormatted("\n");
   buffer.AppendFormatted("/*\n");
   buffer.AppendFormatted(" * These macros can be used in the following way:\n");
   buffer.AppendFormatted(" *\n");
   buffer.AppendFormatted(" *    #if ROME_VERSION_CODE >= ROME_VERSION(2,5)\n");
   buffer.AppendFormatted(" *    #   include <newheader.h>\n");
   buffer.AppendFormatted(" *    #else\n");
   buffer.AppendFormatted(" *    #   include <oldheader.h>\n");
   buffer.AppendFormatted(" *    #endif\n");
   buffer.AppendFormatted(" *\n");
   buffer.AppendFormatted("*/\n");
   buffer.AppendFormatted("\n");
   buffer.AppendFormatted("#define ROME_RELEASE \"%d.%d.%d\"\n", romeMajor, romeMinor, romePatch);
   buffer.AppendFormatted("#define ROME_REVISION_CODE %d\n", revisionNumber);
   buffer.AppendFormatted("#define ROME_DESCRIPTION \"%s\"\n", desc.Data());
   buffer.AppendFormatted("#define ROME_STABLE %d\n", isStableVersion);
/*
   buffer.AppendFormatted("#define ROME_RELEASE_DATE \"%s %2d %d\"\n", monthName[month], day, year);
   buffer.AppendFormatted("#define ROME_RELEASE_TIME \"%02d:%02d:%02d\"\n", hour, min, sec);
*/
   buffer.AppendFormatted("#define ROME_VERSION_CODE %d\n", GetROMEVersion(romeMajor, romeMinor));
   buffer.AppendFormatted("#define ROME_VERSION(a,b) (((a) << 8) + (b))\n");
   buffer.AppendFormatted("#define ROME_PATCH_VERSION_CODE %d\n", GetROMEVersionPatch(romeMajor, romeMinor, romePatch));
   buffer.AppendFormatted("#define ROME_PATCH_VERSION(a,b,c) (((a) << 16) + (b << 8) + (c))\n");
   buffer.AppendFormatted("\n");
   buffer.AppendFormatted("#endif\n");

   // write file
   if (fileBuffer != buffer) {
      ofstream versionH(hfile.Data());
      if (!versionH.good()) {
         cerr<<"failed to open "<<hfile<<" for write."<<endl;
         return 1;
      }
      versionH<<buffer.Data();
      versionH.close();
   }

   return 0;
}

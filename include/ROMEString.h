/********************************************************************
  ROMEString.h, M. Schneebeli PSI

  $Id: ROMEString.h 2613 2012-09-02 07:22:32Z sawada $

********************************************************************/
#ifndef ROMEString_H
#define ROMEString_H

#include <Riostream.h>
#include <TString.h>
#include <Varargs.h>
#include "ROME.h"

class ROMEString : public TString
{
public:
   ROMEString():TString() {}
   ROMEString(Ssiz_t s):TString(s) {}
   ROMEString(char c):TString(c) {}
   ROMEString(const char* s):TString(s) {}
   ROMEString(const TString& s):TString(s) {}
   ROMEString(const TSubString& s):TString(s) {}
   virtual ~ROMEString() {}

#ifndef __MAKECINT__
   ROMEString&  AppendFormatted(const char* format=NULL,...) G_GNUC_PRINTF(2, 3);
   ROMEString&  InsertFormatted(Ssiz_t position,const char* format=NULL,...) G_GNUC_PRINTF(3, 4);
   ROMEString&  SetFormatted(const char* format=NULL,...) G_GNUC_PRINTF(2, 3);
   static char* Format(const char *format, va_list ap) G_GNUC_PRINTF(1, 0);
   static char* SlowFormat(const char *format, va_list ap, int hint) G_GNUC_PRINTF(1, 0);
#endif
   Int_t         NumberOfOccurrence(ROMEString& subString) const { return NumberOfOccurrence(subString.Data()); }
   Int_t         NumberOfOccurrence(const char* subString) const;
   void          Write() const { std::cout << Data(); }
   void          WriteLine() const { std::cout << Data() << std::endl; }
   Int_t         ToInteger() const;
   UInt_t        ToUnsignedInteger() const;
   Long_t        ToLong() const;
   ULong_t       ToUnsignedLong() const;
   Long64_t      ToLong64() const;
   ULong64_t     ToUnsignedLong64() const;
   Double_t      ToDouble() const;
   Float_t       ToFloat() const;
   Bool_t        ToBool() const;
   void          ToLower() { static_cast<TString*>(this)->ToLower(); }
   const char*   ToLower(ROMEString& destination) const;
   void          ToUpper() { static_cast<TString*>(this)->ToUpper(); }
   const char*   ToUpper(ROMEString& destination) const;
   Int_t         ReadFile(const char *filename, Bool_t useCache = kFALSE);
   std::istream& ReadFile(std::istream& str);
   std::istream& ReadLine(std::istream& str);
   const char*   ReadCommandOutput(const char* command, Bool_t readError = kTRUE, Bool_t suppressError = kFALSE);
   ROMEString&   StripSpaces();
   ROMEString&   ChopSpaces();
   Bool_t        ContainsFast(const char* s) const { return strstr(Data(), s) != 0; }
   Bool_t        ContainsInLongString(const char* str);

   ClassDef(ROMEString, 1) // Functional TString for ROME
};

#endif   // ROMEString_H

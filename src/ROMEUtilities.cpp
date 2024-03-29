// Author: Jan Wouters
//////////////////////////////////////////////////////////////////////////
//
//  ROMEUtilities
//
//  General purpose utilities.
//
//  $Id: ROMEUtilities.cpp 2370 2008-01-22 17:01:25Z sawada $
//
//////////////////////////////////////////////////////////////////////////
#include <RConfig.h>
#include <ROMEUtilities.h>
#include <ROMEXML.h>
#include <Riostream.h>
#include "ROMEiostream.h"
#if defined( R__VISUAL_CPLUSPLUS )
#  pragma warning( push )
#  pragma warning( disable : 4800 )
#endif // R__VISUAL_CPLUSPLUS
#include <TSystem.h>
#if defined( R__VISUAL_CPLUSPLUS )
#  pragma warning( pop )
#endif // R__VISUAL_CPLUSPLUS
#include "ROMEString.h"
#include "ROMEStrArray.h"
#if defined(R__MACOSX)
#   include <fenv.h>
#   if defined(__i386__) || defined(__x86_64__)
#      include <string.h>
#      define _FPU_MASK_IM  0x01
#      define _FPU_MASK_DM  0x02
#      define _FPU_MASK_ZM  0x04
#      define _FPU_MASK_OM  0x08
#      define _FPU_MASK_UM  0x10
#      define _FPU_MASK_PM  0x20
       typedef unsigned int fpu_control_t __attribute__ ((__mode__(__HI__)));
#      define _FPU_GETCW(cw) __asm__ __volatile__ ("fnstcw %0" : "=m" (*&cw))
#      define _FPU_SETCW(cw) __asm__ __volatile__ ("fldcw %0" : : "m" (*&cw))
       extern fpu_control_t __fpu_control;
#      define _FPU_GETMXCSR(cw_sse) asm volatile ("stmxcsr %0" : "=m" (cw_sse))
#      define _FPU_SETMXCSR(cw_sse) asm volatile ("ldmxcsr %0" : : "m" (cw_sse))
#   endif
#endif

using namespace std;

//______________________________________________________________________________
void ROMEUtilities::GetMidasTID(ROMEString *buf, Char_t *type)
{
   buf->Resize(0);
   if (!strcmp(type, "Byte_t")
       || !strcmp(type, "BYTE"))
      buf->Append("TID_BYTE");  // < unsigned byte         0       255
   else if (!strcmp(type, "Char_t")
            || !strcmp(type, "Text_t")
            || !strcmp(type, "char"))
      buf->Append("TID_SBYTE"); //< signed byte         -128      127
   else if (!strcmp(type, "UChar_t")
            || !strcmp(type, "unsigned char"))
      buf->Append("TID_CHAR");  //< single character      0       255
   else if (!strcmp(type, "UShort_t")
            || !strcmp(type, "WORD")
            || !strcmp(type, "unsigned short")
            || !strcmp(type, "unsigned short int"))
      buf->Append("TID_WORD");  //< two bytes             0      65535
   else if (!strcmp(type, "Short_t")
            || !strcmp(type, "Version_t")
            || !strcmp(type, "short")
            || !strcmp(type, "short int"))
      buf->Append("TID_SHORT"); //< signed word        -32768    32767
   else if (!strcmp(type, "UInt_t")
            || !strcmp(type, "DWORD")
            || !strcmp(type, "unsigned int")
            || !strcmp(type, "unsigned long")
            || !strcmp(type, "unsigned long int"))
      buf->Append("TID_DWORD"); //< four bytes            0      2^32-1
   else if (!strcmp(type, "Int_t")
            || !strcmp(type, "INT")
            || !strcmp(type, "Ssize_t")
            || !strcmp(type, "int")
            || !strcmp(type, "long")
            || !strcmp(type, "long int"))
      buf->Append("TID_INT");   //< signed dword        -2^31    2^31-1
   else if (!strcmp(type, "Bool_t")
            || !strcmp(type, "BOOL")
            || !strcmp(type, "bool"))
      buf->Append("TID_BOOL");  //< four bytes bool       0        1
   else if (!strcmp(type, "Float_t")
            || !strcmp(type, "Real_t")
            || !strcmp(type, "float"))
      buf->Append("TID_FLOAT"); //< 4 Byte float format
   else if (!strcmp(type, "Double_t")
            || !strcmp(type, "Double32_t")
            || !strcmp(type, "double"))
      buf->Append("TID_DOUBLE");        //< 8 Byte float format
#if defined( R__B64 )          // Note: Long_t and ULong_t are currently not portable types
   else if (!strcmp(type, "Long_t")
            || !strcmp(type, "ULong_t")) {
      ROMEPrint::Warning("WARNING: Long_t and ULong_t is not supported on this architecture.\n");
      buf->Append("TID_DOUBLE");        //< 8 Byte float format
   }
#else
   else if (!strcmp(type, "Long_t"))
      buf->Append("TID_INT");
   else if (!strcmp(type, "ULong_t"))
      buf->Append("TID_DWORD");
#endif
   else
      buf->Append("TID_STRING");        //< zero terminated string
}

//______________________________________________________________________________
void ROMEUtilities::SearchXMLFiles(ROMEStrArray& files, const char* filepath, const char* xmlpath)
{
   // search XML files which has 'xmlpath' from 'filepath' directory
   const char *direntry;
   TString afile;
   void *dirp = gSystem->OpenDirectory(filepath);
   if (!dirp)
      return;
   ROMEXML *tmp;

   //loop on all entries of this directory
   while ((direntry = gSystem->GetDirEntry(dirp))) {
      afile = Form("%s/%s",filepath,direntry);

      if(!afile.EndsWith(".xml") && !afile.EndsWith(".XML"))
         continue;

      tmp = new ROMEXML();
      if (!tmp->OpenFileForRead(afile.Data()))
         continue;
      if(tmp->NumberOfOccurrenceOfPath(xmlpath))
         files.AddLast(afile);
      delete tmp;
   }

   return;
}

//______________________________________________________________________________
const char* ROMEUtilities::FastCrypt(const char *str)
{
// Easy cription. Please do not use for important passwords.
   static char result[129];
   Int_t i;
   Int_t n = strlen(str);
   if (n >= 64)
      return 0;
   for (i = 0; i < n; i++) {
      result[2 * i] = (str[i] / 16) * 5 + 2 * 16 + 1;
      result[2 * i + 1] = str[i] % 16 + (i % 5) * 16 + 2 * 16 + 1;
   }
   result[2 * i] = '\0';
   return result;
}

//______________________________________________________________________________
const char* ROMEUtilities::FastDecrypt(const char *str)
{
// Easy cription. Please do not use for important passwords.
   static char result[65];
   Int_t i;
   Int_t n = strlen(str);
   if (n >= 128)
      return 0;
   for (i = 0; i < n; i += 2) {
      result[i / 2] = ((str[i] - 2 * 16 - 1) / 5) * 16 +
            (str[i + 1] - 2 * 16 - ((i / 2) % 5) * 16) - 1;
   }
   result[i / 2] = '\0';
   return result;
}

//______________________________________________________________________________
Int_t ROMEUtilities::GetFPEMask()
{
#if defined( R__VISUAL_CPLUSPLUS )
   UInt_t oldmask;
   Int_t mask = 0;
   oldmask = _control87(0, 0) & MCW_EM;

   if (oldmask & _EM_INVALID  )   mask |= kInvalid;
   if (oldmask & _EM_ZERODIVIDE)  mask |= kDivByZero;
   if (oldmask & _EM_OVERFLOW )   mask |= kOverflow;
   if (oldmask & _EM_UNDERFLOW)   mask |= kUnderflow;
   if (oldmask & _EM_INEXACT  )   mask |= kInexact;

   return static_cast<Int_t>(mask);
#else
#   if defined(R__MACOSX) && (defined(__i386__) || defined(__x86_64__))
   Int_t mask = 0;
   fpu_control_t mode, mode_sse;

   _FPU_GETCW(mode);

   if ((~mode) & _FPU_MASK_IM) mask |= kInvalid;
   if ((~mode) & _FPU_MASK_ZM) mask |= kDivByZero;
   if ((~mode) & _FPU_MASK_OM) mask |= kOverflow;
   if (((~mode) & _FPU_MASK_UM) || ((~mode) & _FPU_MASK_DM)) {
      mask |= kUnderflow;
   }
   if ((~mode) & _FPU_MASK_PM) mask |= kInexact;

   _FPU_GETMXCSR(mode_sse);
   if ((~mode_sse) & (_FPU_MASK_IM << 7)) mask |= kInvalid;
   if ((~mode_sse) & (_FPU_MASK_ZM << 7)) mask |= kDivByZero;
   if ((~mode_sse) & (_FPU_MASK_OM << 7)) mask |= kOverflow;
   if (((~mode_sse) & (_FPU_MASK_UM << 7)) ||
       ((~mode_sse) & (_FPU_MASK_DM << 7))) {
      mask |= kUnderflow;
   }
   if ((~mode_sse) & (_FPU_MASK_PM << 7)) mask |= kInexact;

   return mask;
#   else
 return gSystem->GetFPEMask();
#   endif
#endif
}

//______________________________________________________________________________
Int_t ROMEUtilities::SetFPEMask(const Int_t mask)
{
#if defined( R__VISUAL_CPLUSPLUS )
   Int_t old = GetFPEMask();

   UInt_t newm = 0;
   if (mask & kInvalid  )   newm |= _EM_INVALID;
   if (mask & kDivByZero)   newm |= _EM_ZERODIVIDE;
   if (mask & kOverflow )   newm |= _EM_OVERFLOW;
   if (mask & kUnderflow)   newm |= _EM_UNDERFLOW;
   if (mask & kInexact  )   newm |= _EM_INEXACT;

   // clear the status
   _clear87();

   Int_t cm;
   /* could use _controlfp */
   cm = _control87(0, 0);
   cm &= ~newm;
   _control87(cm, MCW_EM);
   return old;
#else // UNIX
#   if defined(R__MACOSX)
   feclearexcept(FE_ALL_EXCEPT);
#      if defined(__i386__) || defined(__x86_64__)
   fpu_control_t mode, mode_sse;
   Int_t old = GetFPEMask();

   _FPU_GETCW(mode);
   mode |= _FPU_MASK_IM | _FPU_MASK_DM | _FPU_MASK_ZM | _FPU_MASK_OM | _FPU_MASK_UM | _FPU_MASK_PM;

   if (mask & kInvalid  ) mode &= ~_FPU_MASK_IM;
   if (mask & kDivByZero) mode &= ~_FPU_MASK_ZM;
   if (mask & kOverflow ) mode &= ~_FPU_MASK_OM;
   if (mask & kUnderflow) {
      mode &= ~_FPU_MASK_UM;
      mode &= ~_FPU_MASK_DM;
   }
   if (mask & kInexact  ) mode &= ~_FPU_MASK_PM;
   _FPU_SETCW(mode);

   _FPU_GETMXCSR(mode_sse);
   mode_sse &= 0xFFFF0000;
   mode_sse |= (_FPU_MASK_IM | _FPU_MASK_DM | _FPU_MASK_ZM | _FPU_MASK_OM | _FPU_MASK_UM | _FPU_MASK_PM) << 7;

   if (mask & kInvalid  ) mode_sse &= ~(_FPU_MASK_IM << 7);
   if (mask & kDivByZero) mode_sse &= ~(_FPU_MASK_ZM << 7);
   if (mask & kOverflow ) mode_sse &= ~(_FPU_MASK_OM << 7);
   if (mask & kUnderflow) {
      mode_sse &= ~(_FPU_MASK_UM << 7);
      mode_sse &= ~(_FPU_MASK_DM << 7);
   }
   if (mask & kInexact  ) mode_sse &= ~(_FPU_MASK_PM << 7);
   _FPU_SETMXCSR(mode_sse);

   return old;
#      else
   return gSystem->SetFPEMask(mask);
#      endif
#   else
   return gSystem->SetFPEMask(mask);
#   endif
#endif
}

#if !defined(__CINT__)
//______________________________________________________________________________
const char* ROMEUtilities::GetFormat(const type_info &t)
{
   // get the format specifier (like '%s') of a declaration type

   if (t == typeid(char) ||
       t == typeid(unsigned char)) {
      return "%c";
   } else if (t == typeid(short) ||
              t == typeid(int)) {
      return "%d";
   } else if (t == typeid(unsigned short) ||
              t == typeid(unsigned int)) {
      return "%u";
   } else if (t == typeid(long)) {
      return "%ld";
   } else if (t == typeid(unsigned long)) {
      return "%lu";
   } else if (t == typeid(long long)) {
      return R_LLD;
   } else if (t == typeid(unsigned long long)) {
#if defined( R__VISUAL_CPLUSPLUS )
      return "%I64u";
#else
      return "%llu";
#endif
   } else if (t == typeid(bool)) {
      return "%d";
   } else if (t == typeid(float)) {
//      strcpy(p, "%#.6g");
      return "%g";
   } else if (t == typeid(double)) {
//      strcpy(p, "%#.14g");
      return "%g";
   } else if (t == typeid(char*) ||
              t == typeid(unsigned char*)) {
      return "%s";
   } else if (t == typeid(TString) ||
              t == typeid(ROMEString) ||
              t == typeid(std::string)) {
      return "%s";
   } else {
      cerr<<"Error in ROMEBuilder::GetFormat: Unknown type '"<<t.name()<<"'"<<endl;
      return "%s";
   }

   return 0;
}
#endif

/********************************************************************
  ROME.h, M. Schneebeli PSI

  $Id: ROME.h 2467 2008-07-17 22:56:47Z sawada $

********************************************************************/
#ifndef ROME_H
#define ROME_H
#include <Rtypes.h>
#include <Riostream.h>
#include <RConfig.h>
#if defined( R__VISUAL_CPLUSPLUS )
#pragma warning( push )
#pragma warning( disable : 4800 )
#endif // R__VISUAL_CPLUSPLUS
#include <TSystem.h>
#if defined( R__VISUAL_CPLUSPLUS )
#pragma warning( pop )
#endif // R__VISUAL_CPLUSPLUS

#define THREADRETURN NULL
#define THREADTYPE void*

#if defined( R__VISUAL_CPLUSPLUS )
#   define OS_WINNT
#endif

#if defined( OS_WINNT ) && defined( ROMEDLL_EXPORTS )
#   define DLLEXPORT __declspec(dllexport)
#else
#   define DLLEXPORT
#endif

#ifdef R__GNU
#   define SafeDeleteArray(p) { if (p) { delete [] p; p = 0; } }
#else
#   define SafeDeleteArray(p) { delete [] p; p = 0; }
#endif

#if defined(R__GNU) && !defined(__MAKECINT__)
#   define G_GNUC_PRINTF( format_idx, arg_idx )                 \
   __attribute__((format (printf, format_idx, arg_idx)))
#   define G_GNUC_SCANF( format_idx, arg_idx )          \
   __attribute__((format (scanf, format_idx, arg_idx)))
#   define G_GNUC_FORMAT( arg_idx )             \
   __attribute__((format_arg (arg_idx)))
#else
#   define G_GNUC_PRINTF( format_idx, arg_idx )
#   define G_GNUC_SCANF( format_idx, arg_idx )
#   define G_GNUC_FORMAT( arg_idx )
#endif

#define WarningSuppression(x) { if (x) { int warning_suppression = 0; if (warning_suppression) {} } }

// This macro can be used for debugging like,
//  void test(int i)
//  {
//     if(i != 3) dbgprintf("i=%d", i);
//     if(i != 3) dbgcout<<"i="<<i<<endl;
//  }
#if defined( R__VISUAL_CPLUSPLUS )
// The next line does not compile
//#   define dbgprintf(...) (printf("=== %s:%u in %s:",gSystem->BaseName(__FILE__),__LINE__,__FUNCTION__),printf(" "__VA_ARGS__))
#   define dbgcout cout<<"=== "<<gSystem->BaseName(__FILE__)<<":"<<__LINE__<<" in "<<__FUNCTION__<<": "
#else
#   define dbgprintf(...) (printf("=== %s:%u in %s:",gSystem->BaseName(__FILE__),__LINE__,__func__),printf(" "__VA_ARGS__))
#   define dbgcout std::cout<<"==== "<<gSystem->BaseName(__FILE__)<<":"<<__LINE__<<" in "<<__func__<<": "
#endif

#if (ROOT_VERSION_CODE < ROOT_VERSION(4,3,2))
const Int_t     kMaxUChar    = 256;
const Int_t     kMaxChar     = kMaxUChar >> 1;
const Int_t     kMinChar     = -kMaxChar - 1;
const Int_t     kMinShort    = -kMaxShort - 1;
const Int_t     kMinInt      = -kMaxInt - 1;
const ULong_t   kMaxULong    = ULong_t(~0);
const Long_t    kMaxLong     = Long_t(kMaxULong >> 1);
const Long_t    kMinLong     = -kMaxLong - 1;
const Long64_t  kMinLong64   = -kMaxLong64 - 1;
#endif

#if defined(R__UNIX)
#   define R_LLD     "%lld"
#   define R_ULLD    "%llu"
#   define R_05LLD   "%05lld"
#   define R_05ULLD  "%05llu"
#else
#   define R_LLD     "%I64d"
#   define R_ULLD    "%I64u"
#   define R_05LLD   "%05I64d"
#   define R_05ULLD "%05I64u"
#endif

const Int_t kTriggerEventID = 1; // event ID of trigger in midas

#endif   // ROME_H


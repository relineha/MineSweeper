// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME ParameterSetDict

/*******************************************************************/
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#define G__DICTIONARY
#include "RConfig.h"
#include "TClass.h"
#include "TDictAttributeMap.h"
#include "TInterpreter.h"
#include "TROOT.h"
#include "TBuffer.h"
#include "TMemberInspector.h"
#include "TInterpreter.h"
#include "TVirtualMutex.h"
#include "TError.h"

#ifndef G__ROOT
#define G__ROOT
#endif

#include "RtypesImp.h"
#include "TIsAProxy.h"
#include "TFileMergeInfo.h"
#include <algorithm>
#include "TCollectionProxyInfo.h"
/*******************************************************************/

#include "TDataMember.h"

// Since CINT ignores the std namespace, we need to do so in this file.
namespace std {} using namespace std;

// Header files passed as explicit arguments
#include "../../DataProducts/include/ParameterSet.hh"

// Header files passed via #pragma extra_include

namespace ROOT {
   static void *new_ParameterSet(void *p = 0);
   static void *newArray_ParameterSet(Long_t size, void *p);
   static void delete_ParameterSet(void *p);
   static void deleteArray_ParameterSet(void *p);
   static void destruct_ParameterSet(void *p);
   static void streamer_ParameterSet(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ParameterSet*)
   {
      ::ParameterSet *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::ParameterSet >(0);
      static ::ROOT::TGenericClassInfo 
         instance("ParameterSet", ::ParameterSet::Class_Version(), "DataProducts/include/ParameterSet.hh", 23,
                  typeid(::ParameterSet), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::ParameterSet::Dictionary, isa_proxy, 16,
                  sizeof(::ParameterSet) );
      instance.SetNew(&new_ParameterSet);
      instance.SetNewArray(&newArray_ParameterSet);
      instance.SetDelete(&delete_ParameterSet);
      instance.SetDeleteArray(&deleteArray_ParameterSet);
      instance.SetDestructor(&destruct_ParameterSet);
      instance.SetStreamerFunc(&streamer_ParameterSet);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ParameterSet*)
   {
      return GenerateInitInstanceLocal((::ParameterSet*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::ParameterSet*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr ParameterSet::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *ParameterSet::Class_Name()
{
   return "ParameterSet";
}

//______________________________________________________________________________
const char *ParameterSet::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ParameterSet*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int ParameterSet::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ParameterSet*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *ParameterSet::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ParameterSet*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *ParameterSet::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ParameterSet*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void ParameterSet::Streamer(TBuffer &R__b)
{
   // Stream an object of class ParameterSet.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      TObject::Streamer(R__b);
      R__b >> f_Verbosity;
      R__b >> f_PixelsPerMm;
      { TString R__str; R__str.Streamer(R__b); f_GridName = R__str.Data(); }
      R__b.ReadStaticArray((double*)f_RGBWeights);
      R__b >> f_FftNegThresh;
      R__b >> f_FftPosThresh;
      R__b >> f_SHFBrightThresh;
      R__b >> f_SHFFiducialFromWall;
      R__b >> f_DBMinPts;
      R__b >> f_DBEpsilon;
      R__b >> f_DuplicateClustDist;
      { TString R__str; R__str.Streamer(R__b); f_DebugPlotsPicName = R__str.Data(); }
      R__b >> f_HoughPointCutoff;
      R__b >> f_BaselineDifference;
      R__b >> f_IsPicInvertedXY;
      { TString R__str; R__str.Streamer(R__b); f_RunMode = R__str.Data(); }
      R__b >> f_GridSymmetryStatus;
      R__b >> f_SupportRingReflectionStatus;
      R__b >> f_DuplicateClustFiducialFraction;
      R__b >> f_ClockPhi;
      R__b >> f_GridScanFinishStatus;
      R__b >> f_MaxClustersPerProdFile;
      { TString R__str; R__str.Streamer(R__b); f_WriteMode = R__str.Data(); }
      R__b >> f_ReadNumber;
      R__b.CheckByteCount(R__s, R__c, ParameterSet::IsA());
   } else {
      R__c = R__b.WriteVersion(ParameterSet::IsA(), kTRUE);
      TObject::Streamer(R__b);
      R__b << f_Verbosity;
      R__b << f_PixelsPerMm;
      { TString R__str = f_GridName.c_str(); R__str.Streamer(R__b);}
      R__b.WriteArray(f_RGBWeights, 3);
      R__b << f_FftNegThresh;
      R__b << f_FftPosThresh;
      R__b << f_SHFBrightThresh;
      R__b << f_SHFFiducialFromWall;
      R__b << f_DBMinPts;
      R__b << f_DBEpsilon;
      R__b << f_DuplicateClustDist;
      { TString R__str = f_DebugPlotsPicName.c_str(); R__str.Streamer(R__b);}
      R__b << f_HoughPointCutoff;
      R__b << f_BaselineDifference;
      R__b << f_IsPicInvertedXY;
      { TString R__str = f_RunMode.c_str(); R__str.Streamer(R__b);}
      R__b << f_GridSymmetryStatus;
      R__b << f_SupportRingReflectionStatus;
      R__b << f_DuplicateClustFiducialFraction;
      R__b << f_ClockPhi;
      R__b << f_GridScanFinishStatus;
      R__b << f_MaxClustersPerProdFile;
      { TString R__str = f_WriteMode.c_str(); R__str.Streamer(R__b);}
      R__b << f_ReadNumber;
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_ParameterSet(void *p) {
      return  p ? new(p) ::ParameterSet : new ::ParameterSet;
   }
   static void *newArray_ParameterSet(Long_t nElements, void *p) {
      return p ? new(p) ::ParameterSet[nElements] : new ::ParameterSet[nElements];
   }
   // Wrapper around operator delete
   static void delete_ParameterSet(void *p) {
      delete ((::ParameterSet*)p);
   }
   static void deleteArray_ParameterSet(void *p) {
      delete [] ((::ParameterSet*)p);
   }
   static void destruct_ParameterSet(void *p) {
      typedef ::ParameterSet current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_ParameterSet(TBuffer &buf, void *obj) {
      ((::ParameterSet*)obj)->::ParameterSet::Streamer(buf);
   }
} // end of namespace ROOT for class ::ParameterSet

namespace {
  void TriggerDictionaryInitialization_ParameterSetDict_Impl() {
    static const char* headers[] = {
"DataProducts/include/ParameterSet.hh",
0
    };
    static const char* includePaths[] = {
"/opt/local/libexec/root6/include/root",
"/Users/ryanlinehan/LZ_Local_Work/Grids/GitlabRepo/GridsMacros/AutomatedGridScanning/MineSweeper/",
0
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "ParameterSetDict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
class __attribute__((annotate("$clingAutoload$DataProducts/include/ParameterSet.hh")))  ParameterSet;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "ParameterSetDict dictionary payload"

#ifndef G__VECTOR_HAS_CLASS_ITERATOR
  #define G__VECTOR_HAS_CLASS_ITERATOR 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
#include "DataProducts/include/ParameterSet.hh"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"ParameterSet", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("ParameterSetDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_ParameterSetDict_Impl, {}, classesHeaders);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_ParameterSetDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_ParameterSetDict() {
  TriggerDictionaryInitialization_ParameterSetDict_Impl();
}

// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME DateAndTimeDict

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
#include "../../DataProducts/include/DateAndTime.hh"

// Header files passed via #pragma extra_include

namespace ROOT {
   static void *new_DateAndTime(void *p = 0);
   static void *newArray_DateAndTime(Long_t size, void *p);
   static void delete_DateAndTime(void *p);
   static void deleteArray_DateAndTime(void *p);
   static void destruct_DateAndTime(void *p);
   static void streamer_DateAndTime(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::DateAndTime*)
   {
      ::DateAndTime *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::DateAndTime >(0);
      static ::ROOT::TGenericClassInfo 
         instance("DateAndTime", ::DateAndTime::Class_Version(), "DataProducts/include/DateAndTime.hh", 26,
                  typeid(::DateAndTime), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::DateAndTime::Dictionary, isa_proxy, 16,
                  sizeof(::DateAndTime) );
      instance.SetNew(&new_DateAndTime);
      instance.SetNewArray(&newArray_DateAndTime);
      instance.SetDelete(&delete_DateAndTime);
      instance.SetDeleteArray(&deleteArray_DateAndTime);
      instance.SetDestructor(&destruct_DateAndTime);
      instance.SetStreamerFunc(&streamer_DateAndTime);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::DateAndTime*)
   {
      return GenerateInitInstanceLocal((::DateAndTime*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::DateAndTime*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr DateAndTime::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *DateAndTime::Class_Name()
{
   return "DateAndTime";
}

//______________________________________________________________________________
const char *DateAndTime::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::DateAndTime*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int DateAndTime::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::DateAndTime*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *DateAndTime::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::DateAndTime*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *DateAndTime::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::DateAndTime*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void DateAndTime::Streamer(TBuffer &R__b)
{
   // Stream an object of class DateAndTime.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      TObject::Streamer(R__b);
      R__b >> f_Day;
      R__b >> f_Month;
      R__b >> f_Year;
      R__b >> f_Second;
      R__b >> f_Minute;
      R__b >> f_Hour;
      R__b.CheckByteCount(R__s, R__c, DateAndTime::IsA());
   } else {
      R__c = R__b.WriteVersion(DateAndTime::IsA(), kTRUE);
      TObject::Streamer(R__b);
      R__b << f_Day;
      R__b << f_Month;
      R__b << f_Year;
      R__b << f_Second;
      R__b << f_Minute;
      R__b << f_Hour;
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_DateAndTime(void *p) {
      return  p ? new(p) ::DateAndTime : new ::DateAndTime;
   }
   static void *newArray_DateAndTime(Long_t nElements, void *p) {
      return p ? new(p) ::DateAndTime[nElements] : new ::DateAndTime[nElements];
   }
   // Wrapper around operator delete
   static void delete_DateAndTime(void *p) {
      delete ((::DateAndTime*)p);
   }
   static void deleteArray_DateAndTime(void *p) {
      delete [] ((::DateAndTime*)p);
   }
   static void destruct_DateAndTime(void *p) {
      typedef ::DateAndTime current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_DateAndTime(TBuffer &buf, void *obj) {
      ((::DateAndTime*)obj)->::DateAndTime::Streamer(buf);
   }
} // end of namespace ROOT for class ::DateAndTime

namespace {
  void TriggerDictionaryInitialization_DateAndTimeDict_Impl() {
    static const char* headers[] = {
"DataProducts/include/DateAndTime.hh",
0
    };
    static const char* includePaths[] = {
"/opt/local/libexec/root6/include/root",
"/Users/ryanlinehan/LZ_Local_Work/Grids/GitlabRepo/GridsMacros/AutomatedGridScanning/MineSweeper/",
0
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "DateAndTimeDict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
class __attribute__((annotate("$clingAutoload$DataProducts/include/DateAndTime.hh")))  DateAndTime;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "DateAndTimeDict dictionary payload"

#ifndef G__VECTOR_HAS_CLASS_ITERATOR
  #define G__VECTOR_HAS_CLASS_ITERATOR 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
#include "DataProducts/include/DateAndTime.hh"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"DateAndTime", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("DateAndTimeDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_DateAndTimeDict_Impl, {}, classesHeaders);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_DateAndTimeDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_DateAndTimeDict() {
  TriggerDictionaryInitialization_DateAndTimeDict_Impl();
}

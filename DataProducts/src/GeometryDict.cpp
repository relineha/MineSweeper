// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME GeometryDict

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
#include "../../DataProducts/include/Geometry.hh"

// Header files passed via #pragma extra_include

namespace ROOT {
   static void *new_Geometry(void *p = 0);
   static void *newArray_Geometry(Long_t size, void *p);
   static void delete_Geometry(void *p);
   static void deleteArray_Geometry(void *p);
   static void destruct_Geometry(void *p);
   static void streamer_Geometry(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::Geometry*)
   {
      ::Geometry *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::Geometry >(0);
      static ::ROOT::TGenericClassInfo 
         instance("Geometry", ::Geometry::Class_Version(), "DataProducts/include/Geometry.hh", 29,
                  typeid(::Geometry), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::Geometry::Dictionary, isa_proxy, 16,
                  sizeof(::Geometry) );
      instance.SetNew(&new_Geometry);
      instance.SetNewArray(&newArray_Geometry);
      instance.SetDelete(&delete_Geometry);
      instance.SetDeleteArray(&deleteArray_Geometry);
      instance.SetDestructor(&destruct_Geometry);
      instance.SetStreamerFunc(&streamer_Geometry);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::Geometry*)
   {
      return GenerateInitInstanceLocal((::Geometry*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::Geometry*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr Geometry::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *Geometry::Class_Name()
{
   return "Geometry";
}

//______________________________________________________________________________
const char *Geometry::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Geometry*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int Geometry::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Geometry*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *Geometry::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Geometry*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *Geometry::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Geometry*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void Geometry::Streamer(TBuffer &R__b)
{
   // Stream an object of class Geometry.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      TObject::Streamer(R__b);
      R__b.StreamObject(&(f_CenterXY),typeid(f_CenterXY));
      R__b >> f_GridDiameter;
      R__b >> f_ClockPhi;
      R__b.CheckByteCount(R__s, R__c, Geometry::IsA());
   } else {
      R__c = R__b.WriteVersion(Geometry::IsA(), kTRUE);
      TObject::Streamer(R__b);
      R__b.StreamObject(&(f_CenterXY),typeid(f_CenterXY));
      R__b << f_GridDiameter;
      R__b << f_ClockPhi;
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_Geometry(void *p) {
      return  p ? new(p) ::Geometry : new ::Geometry;
   }
   static void *newArray_Geometry(Long_t nElements, void *p) {
      return p ? new(p) ::Geometry[nElements] : new ::Geometry[nElements];
   }
   // Wrapper around operator delete
   static void delete_Geometry(void *p) {
      delete ((::Geometry*)p);
   }
   static void deleteArray_Geometry(void *p) {
      delete [] ((::Geometry*)p);
   }
   static void destruct_Geometry(void *p) {
      typedef ::Geometry current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_Geometry(TBuffer &buf, void *obj) {
      ((::Geometry*)obj)->::Geometry::Streamer(buf);
   }
} // end of namespace ROOT for class ::Geometry

namespace {
  void TriggerDictionaryInitialization_GeometryDict_Impl() {
    static const char* headers[] = {
"DataProducts/include/Geometry.hh",
0
    };
    static const char* includePaths[] = {
"/opt/local/libexec/root6/include/root",
"/Users/ryanlinehan/LZ_Local_Work/Grids/GitlabRepo/GridsMacros/AutomatedGridScanning/MineSweeper/",
0
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "GeometryDict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
class __attribute__((annotate("$clingAutoload$DataProducts/include/Geometry.hh")))  Geometry;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "GeometryDict dictionary payload"

#ifndef G__VECTOR_HAS_CLASS_ITERATOR
  #define G__VECTOR_HAS_CLASS_ITERATOR 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
#include "DataProducts/include/Geometry.hh"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"Geometry", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("GeometryDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_GeometryDict_Impl, {}, classesHeaders);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_GeometryDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_GeometryDict() {
  TriggerDictionaryInitialization_GeometryDict_Impl();
}

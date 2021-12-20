// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME PictureDict

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
#include "../../DataProducts/include/Picture.hh"

// Header files passed via #pragma extra_include

namespace ROOT {
   static void *new_Picture(void *p = 0);
   static void *newArray_Picture(Long_t size, void *p);
   static void delete_Picture(void *p);
   static void deleteArray_Picture(void *p);
   static void destruct_Picture(void *p);
   static void streamer_Picture(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::Picture*)
   {
      ::Picture *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::Picture >(0);
      static ::ROOT::TGenericClassInfo 
         instance("Picture", ::Picture::Class_Version(), "DataProducts/include/Picture.hh", 30,
                  typeid(::Picture), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::Picture::Dictionary, isa_proxy, 16,
                  sizeof(::Picture) );
      instance.SetNew(&new_Picture);
      instance.SetNewArray(&newArray_Picture);
      instance.SetDelete(&delete_Picture);
      instance.SetDeleteArray(&deleteArray_Picture);
      instance.SetDestructor(&destruct_Picture);
      instance.SetStreamerFunc(&streamer_Picture);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::Picture*)
   {
      return GenerateInitInstanceLocal((::Picture*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::Picture*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr Picture::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *Picture::Class_Name()
{
   return "Picture";
}

//______________________________________________________________________________
const char *Picture::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Picture*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int Picture::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Picture*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *Picture::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Picture*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *Picture::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Picture*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void Picture::Streamer(TBuffer &R__b)
{
   // Stream an object of class Picture.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      TObject::Streamer(R__b);
      { TString R__str; R__str.Streamer(R__b); f_Name = R__str.Data(); }
      R__b.StreamObject(&(f_CenterXY),typeid(f_CenterXY));
      {
         vector<Particle> &R__stl =  f_Particles;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            Particle R__t;
            R__t.Streamer(R__b);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<Cluster> &R__stl =  f_Clusters;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            Cluster R__t;
            R__t.Streamer(R__b);
            R__stl.push_back(R__t);
         }
      }
      R__b.CheckByteCount(R__s, R__c, Picture::IsA());
   } else {
      R__c = R__b.WriteVersion(Picture::IsA(), kTRUE);
      TObject::Streamer(R__b);
      { TString R__str = f_Name.c_str(); R__str.Streamer(R__b);}
      R__b.StreamObject(&(f_CenterXY),typeid(f_CenterXY));
      {
         vector<Particle> &R__stl =  f_Particles;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<Particle>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            ((Particle&)(*R__k)).Streamer(R__b);
            }
         }
      }
      {
         vector<Cluster> &R__stl =  f_Clusters;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<Cluster>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            ((Cluster&)(*R__k)).Streamer(R__b);
            }
         }
      }
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_Picture(void *p) {
      return  p ? new(p) ::Picture : new ::Picture;
   }
   static void *newArray_Picture(Long_t nElements, void *p) {
      return p ? new(p) ::Picture[nElements] : new ::Picture[nElements];
   }
   // Wrapper around operator delete
   static void delete_Picture(void *p) {
      delete ((::Picture*)p);
   }
   static void deleteArray_Picture(void *p) {
      delete [] ((::Picture*)p);
   }
   static void destruct_Picture(void *p) {
      typedef ::Picture current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_Picture(TBuffer &buf, void *obj) {
      ((::Picture*)obj)->::Picture::Streamer(buf);
   }
} // end of namespace ROOT for class ::Picture

namespace ROOT {
   static TClass *vectorlEParticlegR_Dictionary();
   static void vectorlEParticlegR_TClassManip(TClass*);
   static void *new_vectorlEParticlegR(void *p = 0);
   static void *newArray_vectorlEParticlegR(Long_t size, void *p);
   static void delete_vectorlEParticlegR(void *p);
   static void deleteArray_vectorlEParticlegR(void *p);
   static void destruct_vectorlEParticlegR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<Particle>*)
   {
      vector<Particle> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<Particle>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<Particle>", -2, "vector", 450,
                  typeid(vector<Particle>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEParticlegR_Dictionary, isa_proxy, 0,
                  sizeof(vector<Particle>) );
      instance.SetNew(&new_vectorlEParticlegR);
      instance.SetNewArray(&newArray_vectorlEParticlegR);
      instance.SetDelete(&delete_vectorlEParticlegR);
      instance.SetDeleteArray(&deleteArray_vectorlEParticlegR);
      instance.SetDestructor(&destruct_vectorlEParticlegR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<Particle> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<Particle>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEParticlegR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<Particle>*)0x0)->GetClass();
      vectorlEParticlegR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEParticlegR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEParticlegR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<Particle> : new vector<Particle>;
   }
   static void *newArray_vectorlEParticlegR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<Particle>[nElements] : new vector<Particle>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEParticlegR(void *p) {
      delete ((vector<Particle>*)p);
   }
   static void deleteArray_vectorlEParticlegR(void *p) {
      delete [] ((vector<Particle>*)p);
   }
   static void destruct_vectorlEParticlegR(void *p) {
      typedef vector<Particle> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<Particle>

namespace ROOT {
   static TClass *vectorlEClustergR_Dictionary();
   static void vectorlEClustergR_TClassManip(TClass*);
   static void *new_vectorlEClustergR(void *p = 0);
   static void *newArray_vectorlEClustergR(Long_t size, void *p);
   static void delete_vectorlEClustergR(void *p);
   static void deleteArray_vectorlEClustergR(void *p);
   static void destruct_vectorlEClustergR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<Cluster>*)
   {
      vector<Cluster> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<Cluster>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<Cluster>", -2, "vector", 450,
                  typeid(vector<Cluster>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEClustergR_Dictionary, isa_proxy, 0,
                  sizeof(vector<Cluster>) );
      instance.SetNew(&new_vectorlEClustergR);
      instance.SetNewArray(&newArray_vectorlEClustergR);
      instance.SetDelete(&delete_vectorlEClustergR);
      instance.SetDeleteArray(&deleteArray_vectorlEClustergR);
      instance.SetDestructor(&destruct_vectorlEClustergR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<Cluster> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<Cluster>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEClustergR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<Cluster>*)0x0)->GetClass();
      vectorlEClustergR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEClustergR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEClustergR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<Cluster> : new vector<Cluster>;
   }
   static void *newArray_vectorlEClustergR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<Cluster>[nElements] : new vector<Cluster>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEClustergR(void *p) {
      delete ((vector<Cluster>*)p);
   }
   static void deleteArray_vectorlEClustergR(void *p) {
      delete [] ((vector<Cluster>*)p);
   }
   static void destruct_vectorlEClustergR(void *p) {
      typedef vector<Cluster> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<Cluster>

namespace {
  void TriggerDictionaryInitialization_PictureDict_Impl() {
    static const char* headers[] = {
"DataProducts/include/Picture.hh",
0
    };
    static const char* includePaths[] = {
"/opt/local/libexec/root6/include/root",
"/Users/ryanlinehan/LZ_Local_Work/Grids/GitlabRepo/GridsMacros/AutomatedGridScanning/MineSweeper/",
0
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "PictureDict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
class __attribute__((annotate("$clingAutoload$DataProducts/include/Picture.hh")))  Picture;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "PictureDict dictionary payload"

#ifndef G__VECTOR_HAS_CLASS_ITERATOR
  #define G__VECTOR_HAS_CLASS_ITERATOR 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
#include "DataProducts/include/Picture.hh"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"Picture", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("PictureDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_PictureDict_Impl, {}, classesHeaders);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_PictureDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_PictureDict() {
  TriggerDictionaryInitialization_PictureDict_Impl();
}

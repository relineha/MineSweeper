// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME GridDict

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
#include "../../DataProducts/include/Grid.hh"

// Header files passed via #pragma extra_include

namespace ROOT {
   static void *new_Grid(void *p = 0);
   static void *newArray_Grid(Long_t size, void *p);
   static void delete_Grid(void *p);
   static void deleteArray_Grid(void *p);
   static void destruct_Grid(void *p);
   static void streamer_Grid(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::Grid*)
   {
      ::Grid *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::Grid >(0);
      static ::ROOT::TGenericClassInfo 
         instance("Grid", ::Grid::Class_Version(), "DataProducts/include/Grid.hh", 33,
                  typeid(::Grid), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::Grid::Dictionary, isa_proxy, 16,
                  sizeof(::Grid) );
      instance.SetNew(&new_Grid);
      instance.SetNewArray(&newArray_Grid);
      instance.SetDelete(&delete_Grid);
      instance.SetDeleteArray(&deleteArray_Grid);
      instance.SetDestructor(&destruct_Grid);
      instance.SetStreamerFunc(&streamer_Grid);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::Grid*)
   {
      return GenerateInitInstanceLocal((::Grid*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::Grid*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr Grid::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *Grid::Class_Name()
{
   return "Grid";
}

//______________________________________________________________________________
const char *Grid::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Grid*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int Grid::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Grid*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *Grid::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Grid*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *Grid::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Grid*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void Grid::Streamer(TBuffer &R__b)
{
   // Stream an object of class Grid.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      TObject::Streamer(R__b);
      {
         vector<Picture> &R__stl =  f_Pictures;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            Picture R__t;
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
      f_ParameterSet.Streamer(R__b);
      f_Geometry.Streamer(R__b);
      R__b.CheckByteCount(R__s, R__c, Grid::IsA());
   } else {
      R__c = R__b.WriteVersion(Grid::IsA(), kTRUE);
      TObject::Streamer(R__b);
      {
         vector<Picture> &R__stl =  f_Pictures;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<Picture>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            ((Picture&)(*R__k)).Streamer(R__b);
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
      f_ParameterSet.Streamer(R__b);
      f_Geometry.Streamer(R__b);
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_Grid(void *p) {
      return  p ? new(p) ::Grid : new ::Grid;
   }
   static void *newArray_Grid(Long_t nElements, void *p) {
      return p ? new(p) ::Grid[nElements] : new ::Grid[nElements];
   }
   // Wrapper around operator delete
   static void delete_Grid(void *p) {
      delete ((::Grid*)p);
   }
   static void deleteArray_Grid(void *p) {
      delete [] ((::Grid*)p);
   }
   static void destruct_Grid(void *p) {
      typedef ::Grid current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_Grid(TBuffer &buf, void *obj) {
      ((::Grid*)obj)->::Grid::Streamer(buf);
   }
} // end of namespace ROOT for class ::Grid

namespace ROOT {
   static TClass *vectorlEPicturegR_Dictionary();
   static void vectorlEPicturegR_TClassManip(TClass*);
   static void *new_vectorlEPicturegR(void *p = 0);
   static void *newArray_vectorlEPicturegR(Long_t size, void *p);
   static void delete_vectorlEPicturegR(void *p);
   static void deleteArray_vectorlEPicturegR(void *p);
   static void destruct_vectorlEPicturegR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<Picture>*)
   {
      vector<Picture> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<Picture>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<Picture>", -2, "vector", 450,
                  typeid(vector<Picture>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEPicturegR_Dictionary, isa_proxy, 0,
                  sizeof(vector<Picture>) );
      instance.SetNew(&new_vectorlEPicturegR);
      instance.SetNewArray(&newArray_vectorlEPicturegR);
      instance.SetDelete(&delete_vectorlEPicturegR);
      instance.SetDeleteArray(&deleteArray_vectorlEPicturegR);
      instance.SetDestructor(&destruct_vectorlEPicturegR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<Picture> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<Picture>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEPicturegR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<Picture>*)0x0)->GetClass();
      vectorlEPicturegR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEPicturegR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEPicturegR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<Picture> : new vector<Picture>;
   }
   static void *newArray_vectorlEPicturegR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<Picture>[nElements] : new vector<Picture>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEPicturegR(void *p) {
      delete ((vector<Picture>*)p);
   }
   static void deleteArray_vectorlEPicturegR(void *p) {
      delete [] ((vector<Picture>*)p);
   }
   static void destruct_vectorlEPicturegR(void *p) {
      typedef vector<Picture> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<Picture>

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
  void TriggerDictionaryInitialization_GridDict_Impl() {
    static const char* headers[] = {
"DataProducts/include/Grid.hh",
0
    };
    static const char* includePaths[] = {
"/opt/local/libexec/root6/include/root",
"/Users/ryanlinehan/LZ_Local_Work/Grids/GitlabRepo/GridsMacros/AutomatedGridScanning/MineSweeper/",
0
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "GridDict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
class __attribute__((annotate("$clingAutoload$DataProducts/include/Grid.hh")))  Grid;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "GridDict dictionary payload"

#ifndef G__VECTOR_HAS_CLASS_ITERATOR
  #define G__VECTOR_HAS_CLASS_ITERATOR 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
#include "DataProducts/include/Grid.hh"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"Grid", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("GridDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_GridDict_Impl, {}, classesHeaders);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_GridDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_GridDict() {
  TriggerDictionaryInitialization_GridDict_Impl();
}

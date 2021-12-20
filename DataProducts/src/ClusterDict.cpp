// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME ClusterDict

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
#include "../../DataProducts/include/Cluster.hh"

// Header files passed via #pragma extra_include

namespace ROOT {
   static void *new_Cluster(void *p = 0);
   static void *newArray_Cluster(Long_t size, void *p);
   static void delete_Cluster(void *p);
   static void deleteArray_Cluster(void *p);
   static void destruct_Cluster(void *p);
   static void streamer_Cluster(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::Cluster*)
   {
      ::Cluster *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::Cluster >(0);
      static ::ROOT::TGenericClassInfo 
         instance("Cluster", ::Cluster::Class_Version(), "DataProducts/include/Cluster.hh", 34,
                  typeid(::Cluster), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::Cluster::Dictionary, isa_proxy, 16,
                  sizeof(::Cluster) );
      instance.SetNew(&new_Cluster);
      instance.SetNewArray(&newArray_Cluster);
      instance.SetDelete(&delete_Cluster);
      instance.SetDeleteArray(&deleteArray_Cluster);
      instance.SetDestructor(&destruct_Cluster);
      instance.SetStreamerFunc(&streamer_Cluster);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::Cluster*)
   {
      return GenerateInitInstanceLocal((::Cluster*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::Cluster*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr Cluster::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *Cluster::Class_Name()
{
   return "Cluster";
}

//______________________________________________________________________________
const char *Cluster::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Cluster*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int Cluster::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Cluster*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *Cluster::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Cluster*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *Cluster::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Cluster*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void Cluster::Streamer(TBuffer &R__b)
{
   // Stream an object of class Cluster.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      TObject::Streamer(R__b);
      R__b.StreamObject(&(f_LocalCenterXY),typeid(f_LocalCenterXY));
      R__b.StreamObject(&(f_GlobalCenterXY),typeid(f_GlobalCenterXY));
      R__b.StreamObject(&(f_HalfWidthsXY),typeid(f_HalfWidthsXY));
      { TString R__str; R__str.Streamer(R__b); f_Type = R__str.Data(); }
      R__b >> f_Id;
      { TString R__str; R__str.Streamer(R__b); f_OwnerPicture = R__str.Data(); }
      {
         vector<pair<double,pair<int,int> > > &R__stl =  f_HitPixels;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(struct std::__1::pair<double, struct std::__1::pair<int, int> >));
         if (R__tcl1==0) {
            Error("f_HitPixels streamer","Missing the TClass object for struct std::__1::pair<double, struct std::__1::pair<int, int> >!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            pair<double,pair<int,int> > R__t;
            R__b.StreamObject(&R__t,R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<vector<int> > &R__stl =  f_HitPixelsRGB;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class std::__1::vector<int, class std::__1::allocator<int> >));
         if (R__tcl1==0) {
            Error("f_HitPixelsRGB streamer","Missing the TClass object for class std::__1::vector<int, class std::__1::allocator<int> >!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            vector<int> R__t;
            R__b.StreamObject(&R__t,R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      R__b >> f_Integral;
      R__b >> f_Area;
      R__b >> f_AvgBrightness;
      R__b.StreamObject(&(f_OffsetFromWire),typeid(f_OffsetFromWire));
      {
         vector<pair<double,pair<int,int> > > &R__stl =  f_OgHitPixels;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(struct std::__1::pair<double, struct std::__1::pair<int, int> >));
         if (R__tcl1==0) {
            Error("f_OgHitPixels streamer","Missing the TClass object for struct std::__1::pair<double, struct std::__1::pair<int, int> >!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            pair<double,pair<int,int> > R__t;
            R__b.StreamObject(&R__t,R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      R__b >> f_OgIntegral;
      R__b >> f_OgAvgBrightness;
      R__b.CheckByteCount(R__s, R__c, Cluster::IsA());
   } else {
      R__c = R__b.WriteVersion(Cluster::IsA(), kTRUE);
      TObject::Streamer(R__b);
      R__b.StreamObject(&(f_LocalCenterXY),typeid(f_LocalCenterXY));
      R__b.StreamObject(&(f_GlobalCenterXY),typeid(f_GlobalCenterXY));
      R__b.StreamObject(&(f_HalfWidthsXY),typeid(f_HalfWidthsXY));
      { TString R__str = f_Type.c_str(); R__str.Streamer(R__b);}
      R__b << f_Id;
      { TString R__str = f_OwnerPicture.c_str(); R__str.Streamer(R__b);}
      {
         vector<pair<double,pair<int,int> > > &R__stl =  f_HitPixels;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
         TClass *R__tcl1 = TBuffer::GetClass(typeid(struct std::__1::pair<double, struct std::__1::pair<int, int> >));
         if (R__tcl1==0) {
            Error("f_HitPixels streamer","Missing the TClass object for struct std::__1::pair<double, struct std::__1::pair<int, int> >!");
            return;
         }
            vector<pair<double,pair<int,int> > >::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b.StreamObject((pair<double,pair<int,int> >*)&(*R__k),R__tcl1);
            }
         }
      }
      {
         vector<vector<int> > &R__stl =  f_HitPixelsRGB;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class std::__1::vector<int, class std::__1::allocator<int> >));
         if (R__tcl1==0) {
            Error("f_HitPixelsRGB streamer","Missing the TClass object for class std::__1::vector<int, class std::__1::allocator<int> >!");
            return;
         }
            vector<vector<int> >::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b.StreamObject((vector<int>*)&(*R__k),R__tcl1);
            }
         }
      }
      R__b << f_Integral;
      R__b << f_Area;
      R__b << f_AvgBrightness;
      R__b.StreamObject(&(f_OffsetFromWire),typeid(f_OffsetFromWire));
      {
         vector<pair<double,pair<int,int> > > &R__stl =  f_OgHitPixels;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
         TClass *R__tcl1 = TBuffer::GetClass(typeid(struct std::__1::pair<double, struct std::__1::pair<int, int> >));
         if (R__tcl1==0) {
            Error("f_OgHitPixels streamer","Missing the TClass object for struct std::__1::pair<double, struct std::__1::pair<int, int> >!");
            return;
         }
            vector<pair<double,pair<int,int> > >::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b.StreamObject((pair<double,pair<int,int> >*)&(*R__k),R__tcl1);
            }
         }
      }
      R__b << f_OgIntegral;
      R__b << f_OgAvgBrightness;
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_Cluster(void *p) {
      return  p ? new(p) ::Cluster : new ::Cluster;
   }
   static void *newArray_Cluster(Long_t nElements, void *p) {
      return p ? new(p) ::Cluster[nElements] : new ::Cluster[nElements];
   }
   // Wrapper around operator delete
   static void delete_Cluster(void *p) {
      delete ((::Cluster*)p);
   }
   static void deleteArray_Cluster(void *p) {
      delete [] ((::Cluster*)p);
   }
   static void destruct_Cluster(void *p) {
      typedef ::Cluster current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_Cluster(TBuffer &buf, void *obj) {
      ((::Cluster*)obj)->::Cluster::Streamer(buf);
   }
} // end of namespace ROOT for class ::Cluster

namespace ROOT {
   static TClass *vectorlEvectorlEintgRsPgR_Dictionary();
   static void vectorlEvectorlEintgRsPgR_TClassManip(TClass*);
   static void *new_vectorlEvectorlEintgRsPgR(void *p = 0);
   static void *newArray_vectorlEvectorlEintgRsPgR(Long_t size, void *p);
   static void delete_vectorlEvectorlEintgRsPgR(void *p);
   static void deleteArray_vectorlEvectorlEintgRsPgR(void *p);
   static void destruct_vectorlEvectorlEintgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<vector<int> >*)
   {
      vector<vector<int> > *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<vector<int> >));
      static ::ROOT::TGenericClassInfo 
         instance("vector<vector<int> >", -2, "vector", 450,
                  typeid(vector<vector<int> >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEvectorlEintgRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<vector<int> >) );
      instance.SetNew(&new_vectorlEvectorlEintgRsPgR);
      instance.SetNewArray(&newArray_vectorlEvectorlEintgRsPgR);
      instance.SetDelete(&delete_vectorlEvectorlEintgRsPgR);
      instance.SetDeleteArray(&deleteArray_vectorlEvectorlEintgRsPgR);
      instance.SetDestructor(&destruct_vectorlEvectorlEintgRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<vector<int> > >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<vector<int> >*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEvectorlEintgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<vector<int> >*)0x0)->GetClass();
      vectorlEvectorlEintgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEvectorlEintgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEvectorlEintgRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<vector<int> > : new vector<vector<int> >;
   }
   static void *newArray_vectorlEvectorlEintgRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<vector<int> >[nElements] : new vector<vector<int> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEvectorlEintgRsPgR(void *p) {
      delete ((vector<vector<int> >*)p);
   }
   static void deleteArray_vectorlEvectorlEintgRsPgR(void *p) {
      delete [] ((vector<vector<int> >*)p);
   }
   static void destruct_vectorlEvectorlEintgRsPgR(void *p) {
      typedef vector<vector<int> > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<vector<int> >

namespace ROOT {
   static TClass *vectorlEpairlEdoublecOpairlEintcOintgRsPgRsPgR_Dictionary();
   static void vectorlEpairlEdoublecOpairlEintcOintgRsPgRsPgR_TClassManip(TClass*);
   static void *new_vectorlEpairlEdoublecOpairlEintcOintgRsPgRsPgR(void *p = 0);
   static void *newArray_vectorlEpairlEdoublecOpairlEintcOintgRsPgRsPgR(Long_t size, void *p);
   static void delete_vectorlEpairlEdoublecOpairlEintcOintgRsPgRsPgR(void *p);
   static void deleteArray_vectorlEpairlEdoublecOpairlEintcOintgRsPgRsPgR(void *p);
   static void destruct_vectorlEpairlEdoublecOpairlEintcOintgRsPgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<pair<double,pair<int,int> > >*)
   {
      vector<pair<double,pair<int,int> > > *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<pair<double,pair<int,int> > >));
      static ::ROOT::TGenericClassInfo 
         instance("vector<pair<double,pair<int,int> > >", -2, "vector", 450,
                  typeid(vector<pair<double,pair<int,int> > >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEpairlEdoublecOpairlEintcOintgRsPgRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<pair<double,pair<int,int> > >) );
      instance.SetNew(&new_vectorlEpairlEdoublecOpairlEintcOintgRsPgRsPgR);
      instance.SetNewArray(&newArray_vectorlEpairlEdoublecOpairlEintcOintgRsPgRsPgR);
      instance.SetDelete(&delete_vectorlEpairlEdoublecOpairlEintcOintgRsPgRsPgR);
      instance.SetDeleteArray(&deleteArray_vectorlEpairlEdoublecOpairlEintcOintgRsPgRsPgR);
      instance.SetDestructor(&destruct_vectorlEpairlEdoublecOpairlEintcOintgRsPgRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<pair<double,pair<int,int> > > >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<pair<double,pair<int,int> > >*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEpairlEdoublecOpairlEintcOintgRsPgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<pair<double,pair<int,int> > >*)0x0)->GetClass();
      vectorlEpairlEdoublecOpairlEintcOintgRsPgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEpairlEdoublecOpairlEintcOintgRsPgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEpairlEdoublecOpairlEintcOintgRsPgRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<pair<double,pair<int,int> > > : new vector<pair<double,pair<int,int> > >;
   }
   static void *newArray_vectorlEpairlEdoublecOpairlEintcOintgRsPgRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<pair<double,pair<int,int> > >[nElements] : new vector<pair<double,pair<int,int> > >[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEpairlEdoublecOpairlEintcOintgRsPgRsPgR(void *p) {
      delete ((vector<pair<double,pair<int,int> > >*)p);
   }
   static void deleteArray_vectorlEpairlEdoublecOpairlEintcOintgRsPgRsPgR(void *p) {
      delete [] ((vector<pair<double,pair<int,int> > >*)p);
   }
   static void destruct_vectorlEpairlEdoublecOpairlEintcOintgRsPgRsPgR(void *p) {
      typedef vector<pair<double,pair<int,int> > > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<pair<double,pair<int,int> > >

namespace ROOT {
   static TClass *vectorlEintgR_Dictionary();
   static void vectorlEintgR_TClassManip(TClass*);
   static void *new_vectorlEintgR(void *p = 0);
   static void *newArray_vectorlEintgR(Long_t size, void *p);
   static void delete_vectorlEintgR(void *p);
   static void deleteArray_vectorlEintgR(void *p);
   static void destruct_vectorlEintgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<int>*)
   {
      vector<int> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<int>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<int>", -2, "vector", 450,
                  typeid(vector<int>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEintgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<int>) );
      instance.SetNew(&new_vectorlEintgR);
      instance.SetNewArray(&newArray_vectorlEintgR);
      instance.SetDelete(&delete_vectorlEintgR);
      instance.SetDeleteArray(&deleteArray_vectorlEintgR);
      instance.SetDestructor(&destruct_vectorlEintgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<int> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<int>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEintgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<int>*)0x0)->GetClass();
      vectorlEintgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEintgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEintgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<int> : new vector<int>;
   }
   static void *newArray_vectorlEintgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<int>[nElements] : new vector<int>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEintgR(void *p) {
      delete ((vector<int>*)p);
   }
   static void deleteArray_vectorlEintgR(void *p) {
      delete [] ((vector<int>*)p);
   }
   static void destruct_vectorlEintgR(void *p) {
      typedef vector<int> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<int>

namespace {
  void TriggerDictionaryInitialization_ClusterDict_Impl() {
    static const char* headers[] = {
"DataProducts/include/Cluster.hh",
0
    };
    static const char* includePaths[] = {
"/opt/local/libexec/root6/include/root",
"/Users/ryanlinehan/LZ_Local_Work/Grids/GitlabRepo/GridsMacros/AutomatedGridScanning/MineSweeper/",
0
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "ClusterDict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
class __attribute__((annotate("$clingAutoload$DataProducts/include/Cluster.hh")))  Cluster;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "ClusterDict dictionary payload"

#ifndef G__VECTOR_HAS_CLASS_ITERATOR
  #define G__VECTOR_HAS_CLASS_ITERATOR 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
#include "DataProducts/include/Cluster.hh"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"Cluster", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("ClusterDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_ClusterDict_Impl, {}, classesHeaders);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_ClusterDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_ClusterDict() {
  TriggerDictionaryInitialization_ClusterDict_Impl();
}

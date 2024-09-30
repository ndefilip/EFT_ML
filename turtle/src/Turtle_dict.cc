// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME srcdITurtle_dict

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
#include "include/Turtle.h"

// Header files passed via #pragma extra_include

namespace ROOT {
   static void *new_Turtle(void *p = 0);
   static void *newArray_Turtle(Long_t size, void *p);
   static void delete_Turtle(void *p);
   static void deleteArray_Turtle(void *p);
   static void destruct_Turtle(void *p);
   static void streamer_Turtle(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::Turtle*)
   {
      ::Turtle *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::Turtle >(0);
      static ::ROOT::TGenericClassInfo 
         instance("Turtle", ::Turtle::Class_Version(), "Turtle.h", 14,
                  typeid(::Turtle), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::Turtle::Dictionary, isa_proxy, 16,
                  sizeof(::Turtle) );
      instance.SetNew(&new_Turtle);
      instance.SetNewArray(&newArray_Turtle);
      instance.SetDelete(&delete_Turtle);
      instance.SetDeleteArray(&deleteArray_Turtle);
      instance.SetDestructor(&destruct_Turtle);
      instance.SetStreamerFunc(&streamer_Turtle);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::Turtle*)
   {
      return GenerateInitInstanceLocal((::Turtle*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::Turtle*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr Turtle::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *Turtle::Class_Name()
{
   return "Turtle";
}

//______________________________________________________________________________
const char *Turtle::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Turtle*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int Turtle::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Turtle*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *Turtle::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Turtle*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *Turtle::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Turtle*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void Turtle::Streamer(TBuffer &R__b)
{
   // Stream an object of class Turtle.

   ::Error("Turtle::Streamer", "version id <=0 in ClassDef, dummy Streamer() called"); if (R__b.IsReading()) { }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_Turtle(void *p) {
      return  p ? new(p) ::Turtle : new ::Turtle;
   }
   static void *newArray_Turtle(Long_t nElements, void *p) {
      return p ? new(p) ::Turtle[nElements] : new ::Turtle[nElements];
   }
   // Wrapper around operator delete
   static void delete_Turtle(void *p) {
      delete ((::Turtle*)p);
   }
   static void deleteArray_Turtle(void *p) {
      delete [] ((::Turtle*)p);
   }
   static void destruct_Turtle(void *p) {
      typedef ::Turtle current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_Turtle(TBuffer &buf, void *obj) {
      ((::Turtle*)obj)->::Turtle::Streamer(buf);
   }
} // end of namespace ROOT for class ::Turtle

namespace {
  void TriggerDictionaryInitialization_Turtle_dict_Impl() {
    static const char* headers[] = {
"include/Turtle.h",
0
    };
    static const char* includePaths[] = {
"/cvmfs/cms.cern.ch/slc7_amd64_gcc700/lcg/root/6.14.09-pafccj6/include",
"/lustre/home/nicola/CentOS7/turtle/",
0
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "Turtle_dict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
class __attribute__((annotate("$clingAutoload$include/Turtle.h")))  Turtle;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "Turtle_dict dictionary payload"

#ifndef G__VECTOR_HAS_CLASS_ITERATOR
  #define G__VECTOR_HAS_CLASS_ITERATOR 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
#include "include/Turtle.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"Turtle", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("Turtle_dict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_Turtle_dict_Impl, {}, classesHeaders, /*has no C++ module*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_Turtle_dict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_Turtle_dict() {
  TriggerDictionaryInitialization_Turtle_dict_Impl();
}

/**************************************************************************
 * 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * Author: The International Lepton Collider Off-line Project.            *
 *                                                                        *
 * Part of the code has been developed by ILC Off-line Project.         *
 * Contributors are mentioned in the code where appropriate.              *
*                                                                        *
* Permission to use, copy, modify and distribute this software and its   *
* documentation strictly for non-commercial purposes is hereby granted   *
* without fee, provided that the above copyright notice appears in all   *
* copies and that both the copyright notice and this permission notice   *
* appear in the supporting documentation. The authors make no claims     *
* about the suitability of this software for any purpose. It is          *
* provided "as is" without express or implied warranty.                  *
**************************************************************************/
//  $Id: IlcCodeTimer.cxx 50615 2011-07-16 23:19:19Z hristov $

//_________________________________________________________________________
// Class to get organized with the way we're timing our methods...
//
// Typical usage is based on macros (like for IlcLog related ones IlcDebug...)
//
// The idea is to instrument the code with a few macro calls, and then,
// at the end of the execution, get a printout of *all* the timers, by using
// IlcCodeTimer::Instance()->Print()
// instead of getting scattered outputs all over the place.
//
// To time a given method, use :
//
// void ClassA::MethodA(....)
// {
//    IlcCodeTimerAuto("")
// }
//
// To get several timers within a same method, use : 
//
// void ClassA::MethodB(...)
// {
//   IlcCodeTimerStart("doing something")
//   ....
//   IlcCodeTimerStop("doing something")
//
//   IlcCodeTimerStart("doing something else")
//   ....
//   IlcCodeTimerStop("doing something else")
// }

#include "IlcCodeTimer.h"

#include <TMap.h>
#include <TObjString.h>
#include <TStopwatch.h>
#include <Riostream.h>

/// \cond CLASSIMP
ClassImp(IlcCodeTimer)
ClassImp(IlcCodeTimer::IlcPair)
/// \endcond

IlcCodeTimer* IlcCodeTimer::fgInstance(0x0);

//_____________________________________________________________________________
void
IlcCodeTimer::IlcPair::Print(Option_t* opt) const
{
  // Print timer information
  cout << opt << Form("%s R:%.4fs C:%.4fs (%d slices)",
                      Name().Data(),Timer()->RealTime(),
                      Timer()->CpuTime(),Timer()->Counter()-1) << endl;
}


//_____________________________________________________________________________
IlcCodeTimer::IlcCodeTimer() : TObject(), fTimers(new TMap)
{
  /// Ctor
  fTimers->SetOwner(kTRUE);
}

//_____________________________________________________________________________
IlcCodeTimer::~IlcCodeTimer()
{
  /// Dtor
  Reset();
  delete fTimers;
}

//_____________________________________________________________________________
IlcCodeTimer*
IlcCodeTimer::Instance()
{
  // single instance of this class
  if (!fgInstance) fgInstance = new IlcCodeTimer;
  return fgInstance;
}

//_____________________________________________________________________________
void IlcCodeTimer::Continue(const char* classname, const char* methodname, 
                            const char* message)
{
  /// Resume a previously stop timer
  TStopwatch* t = Stopwatch(classname,methodname,message);
  if (t)
  {
    t->Continue();
  }
  else
  {
    IlcError(Form("No timer for %s/%s/%s",classname,methodname,message));
  }
}

//_____________________________________________________________________________
Double_t IlcCodeTimer::CpuTime(const char* classname, 
                               const char* methodname,
                               const char* message) const
{
  /// Return cpu time for a given timer
  TStopwatch* t = Stopwatch(classname,methodname,message);
  if (t)
  {
    return t->CpuTime();
  }
  else
  {
    return 0;
  }
}

//_____________________________________________________________________________
TMap*
IlcCodeTimer::MethodMap(const char* classname) const
{
  /// Return the map for a given "classname"
  return static_cast<TMap*>(fTimers->GetValue(classname));
}

//_____________________________________________________________________________
TObjArray*
IlcCodeTimer::MessageArray(const char* classname, const char* methodname) const
{
  /// Return the array for a given IlcPair (classname,methodname)
  TMap* m = MethodMap(classname);
  if ( m ) 
  {
    return static_cast<TObjArray*>(m->GetValue(methodname));
  }
  return 0;
}

//_____________________________________________________________________________
void IlcCodeTimer::PrintMethod(const char* classname, const char* methodname) const
{
  /// Print all the timers for a given method
  TObjArray* messages = MessageArray(classname,methodname);
  messages->Sort();
  
  cout << "   " << methodname << " ";
  
  if ( messages->GetLast() == 0 ) 
  {
    IlcPair* p = static_cast<IlcPair*>(messages->First());
    p->Print();
  }
  else
  {
    cout << endl;
    
    TIter next(messages);
    IlcPair* p;
  
    while ( ( p = static_cast<IlcPair*>(next()) ) ) 
    {
      p->Print("        ");
    }   
  }
}

//_____________________________________________________________________________
void IlcCodeTimer::PrintClass(const char* classname) const
{
  /// Print all the timers for a given class
  TMap* methods = MethodMap(classname);
  TIter next(methods);
  TObjString* methodname;
  TObjArray methodNameArray;
  
  while ( ( methodname = static_cast<TObjString*>(next()) ) ) 
  {
    methodNameArray.Add(methodname);
  }
  
  cout << classname << endl;
  
  methodNameArray.Sort();
  
  TIter mnext(&methodNameArray);
  
  while ( ( methodname = static_cast<TObjString*>(mnext()) ) ) 
  {
    PrintMethod(classname,methodname->String().Data());
  }
}
  
//_____________________________________________________________________________
void IlcCodeTimer::Print(Option_t* /*opt*/) const
{
  /// Print all the timers we hold
  TIter next(fTimers);
  TObjString* classname;
  TObjArray classNameArray;
  
  while ( ( classname = static_cast<TObjString*>(next()) ) )
  {
    classNameArray.Add(classname);
  }
  
  classNameArray.Sort();
  
  TIter cnext(&classNameArray);
  while ( ( classname = static_cast<TObjString*>(cnext()) ) )
  {
    PrintClass(classname->String().Data());
  }
}

//_____________________________________________________________________________
Double_t 
IlcCodeTimer::RealTime(const char* classname, const char* methodname,
                       const char* message) const
{
  /// Return real time of a given time
  TStopwatch* t = Stopwatch(classname,methodname,message);
  if (t)
  {
    return t->RealTime();
  }
  else
  {
    return 0;
  }
}

//_____________________________________________________________________________
void
IlcCodeTimer::Reset()
{
  /// Reset
  TIter next(fTimers);
  TObjString* classname;
  
  while ( ( classname = static_cast<TObjString*>(next()) ) ) 
  {
    TMap* m = static_cast<TMap*>(fTimers->GetValue(classname->String().Data()));
    m->DeleteAll();
  }
  
  fTimers->DeleteAll();
}

//_____________________________________________________________________________
void 
IlcCodeTimer::Start(const char* classname, const char* methodname,
                    const char* message)
{
  /// Start a given time
  TStopwatch* t = Stopwatch(classname,methodname,message);
  if (!t)
  {
    TMap* m = MethodMap(classname);
    if (!m)
    {
      m = new TMap;
      m->SetOwner(kTRUE);
      fTimers->Add(new TObjString(classname),m);
    }      
    TObjArray* messages = MessageArray(classname,methodname);
    if (!messages)
    {
      messages = new TObjArray;
      messages->SetOwner(kTRUE);
      m->Add(new TObjString(methodname),messages);
    }
    t = new TStopwatch;
    t->Start(kTRUE);
    t->Stop();
    messages->Add(new IlcPair(new TObjString(message),t));
  }
  t->Start(kFALSE);
}

//_____________________________________________________________________________
void 
IlcCodeTimer::Stop(const char* classname, const char* methodname,
                   const char* message)
{
  /// Stop a given timer
  TStopwatch* t = Stopwatch(classname,methodname,message);
  if (!t)
  {
    IlcError(Form("No timer for %s/%s/%s",classname,methodname,message));
  }
  else
  {
    t->Stop();
  }
}

//_____________________________________________________________________________
TStopwatch* 
IlcCodeTimer::Stopwatch(const char* classname, const char* methodname,
                        const char* message) const
{
  /// Return the internal TStopwatch for a given timer
  TObjArray* a = MessageArray(classname,methodname);
  if ( a ) 
  {
    if (message)
    {
      TIter next(a);
      IlcPair* p;
      while ( ( p = static_cast<IlcPair*>(next()) ) ) 
      {
        TString s = p->Name();
        if ( s == TString(message) ) 
        {
          return p->Timer();
        }
      }
    }
    else
    {
      return static_cast<TStopwatch*>(a->First());
    }
  }
  return 0x0;
}

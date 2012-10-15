#ifndef ILCCODETIMER_H
#define ILCCODETIMER_H

/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
* See cxx source for full Copyright notice                               */

// $Id: IlcCodeTimer.h 50615 2011-07-16 23:19:19Z hristov $

///
/// A class to organize TStopwatch timers used to time our code
/// 
// Author Laurent Aphecetche

#ifndef ROOT_TString
#  include "TString.h"
#endif
#ifndef ROOT_TObjString
#  include "TObjString.h"
#endif
#ifndef ILCLOG_H
#  include "IlcLog.h"
#endif

class TStopwatch;
class TMap;

class IlcCodeTimer : public TObject
{
public:
  
  IlcCodeTimer();
  virtual ~IlcCodeTimer();

  /// Unique instance of this class, which is a singleton
  static IlcCodeTimer* Instance();
  
  /// Continue timer(classname,methodname,message)
  void Continue(const char* classname, const char* methodname, const char* message="");

  /// Return the cpu time spent in timer(classname,methodname,message)
  Double_t CpuTime(const char* classname, const char* methodname, const char* message="") const;
  
  /// Print the list of timers we manage
  void Print(Option_t* opt="") const;
  
  /// Return the real time spent in timer(classname,methodname,message)
  Double_t RealTime(const char* classname, const char* methodname, const char* message="") const;
  
  /// Reset all our timers
  void Reset();
  
  /// Start timer(classname,methodname,message)
  void Start(const char* classname, const char* methodname, const char* message="");

  /// Stop timer(classname,methodname,message)
  void Stop(const char* classname, const char* methodname, const char* message="");
    
public:
  
  class IlcPair : public TObject
  {
  public:
    IlcPair() : TObject(),fName(0), fTimer(0) {}
    // ctor
    IlcPair(TObjString* name, TStopwatch* timer) : TObject(), fName(name), fTimer(timer) {}
    virtual ~IlcPair() { delete fName; }
    
    /// get name
    TString Name() const { return fName->String(); }
    /// get timer
    TStopwatch* Timer() const { return fTimer; }
    
    /// we are sortable (by name)
    virtual Bool_t IsSortable() const { return kTRUE; }
    /// compare the names
    virtual Int_t Compare(const TObject* object) const
    { return fName->Compare(((const IlcPair*)(object))->fName); }

    virtual void Print(Option_t* opt="") const;

private:
    IlcPair(const IlcPair&);
    IlcPair& operator=(const IlcPair&);
    
    TObjString* fName; // name of the timer
    TStopwatch* fTimer; // actual timer
    
    ClassDef(IlcPair,1) // internal class to hold (string,TStopwatch*) IlcPair
  };
    
  class IlcAutoPtr
  {
    public:
      
    /// ctor
      IlcAutoPtr(const char* classname, const char* methodname, const char* message="") 
      : fA(classname), fB(methodname), fC(message)
      { IlcCodeTimer::Instance()->Start(classname,methodname,message); } 

    /// dtor
      ~IlcAutoPtr() { IlcCodeTimer::Instance()->Stop(fA.Data(),fB.Data(),fC.Data()); }
    
    private:
      TString fA; // first id
      TString fB; // second id
      TString fC; // third id
  };
  
private:  
  
  TMap* MethodMap(const char* classname) const;
  TObjArray* MessageArray(const char* classname, const char* methodname) const;
  TStopwatch* Stopwatch(const char* classname, const char* methodname, const char* message="") const;
  void PrintClass(const char* classname) const;
  void PrintMethod(const char* classname, const char* methodname) const;
  
private:

  IlcCodeTimer(const IlcCodeTimer& rhs);
  IlcCodeTimer& operator=(const IlcCodeTimer& rhs);
  
  static IlcCodeTimer* fgInstance; //< unique instance
  
  TMap* fTimers; //< internal timers
  
  ClassDef(IlcCodeTimer,1) // A timer holder
};

#ifndef LOG_NO_DEBUG

#define IlcCodeTimerStartClass(message) IlcCodeTimer::Instance()->Start(Class()->GetName(),FUNCTIONNAME(),message);
#define IlcCodeTimerStopClass(message) IlcCodeTimer::Instance()->Stop(Class()->GetName(),FUNCTIONNAME(),message);
#define IlcCodeTimerAutoClass(message,counter) IlcCodeTimer::IlcAutoPtr ilcCodeTimerIlcAutoPtrVariable##counter(Class()->GetName(),FUNCTIONNAME(),message);

#define IlcCodeTimerStart(message) IlcCodeTimer::Instance()->Start(ClassName(),FUNCTIONNAME(),message);
#define IlcCodeTimerStop(message) IlcCodeTimer::Instance()->Stop(ClassName(),FUNCTIONNAME(),message);
#define IlcCodeTimerAuto(message,counter) IlcCodeTimer::IlcAutoPtr ilcCodeTimerIlcAutoPtrVariable##counter(ClassName(),FUNCTIONNAME(),message);

#define IlcCodeTimerStartGeneral(message) IlcCodeTimer::Instance()->Start("General",FUNCTIONNAME(),message);
#define IlcCodeTimerStopGeneral(message) IlcCodeTimer::Instance()->Stop("General",FUNCTIONNAME(),message);
#define IlcCodeTimerAutoGeneral(message,counter) IlcCodeTimer::IlcAutoPtr ilcCodeTimerIlcAutoPtrVariable##counter("General",FUNCTIONNAME(),message);

#else

#define IlcCodeTimerStartClass(message)
#define IlcCodeTimerStopClass(message) 
#define IlcCodeTimerAutoClass(message,counter) 

#define IlcCodeTimerStart(message) 
#define IlcCodeTimerStop(message) 
#define IlcCodeTimerAuto(message,counter) 

#define IlcCodeTimerStartGeneral(message) 
#define IlcCodeTimerStopGeneral(message) 
#define IlcCodeTimerAutoGeneral(message,counter) 

#endif

#endif

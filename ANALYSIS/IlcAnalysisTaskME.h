#ifndef ILCANALYSISTASKME_H
#define ILCANALYSISTASKME_H
 
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id$ */

#include "IlcAnalysisTask.h"
#include "IlcVEvent.h"
class IlcAODEvent;
class IlcInputEventHandler;
class TTree;
class IlcMultiEventInputHandler;



class IlcAnalysisTaskME : public IlcAnalysisTask
{
 public:
    IlcAnalysisTaskME();
    IlcAnalysisTaskME(const char* name);
    IlcAnalysisTaskME(const IlcAnalysisTaskME& obj);
    IlcAnalysisTaskME& operator=(const IlcAnalysisTaskME& other);
    virtual ~IlcAnalysisTaskME() {;}
    // Implementation of interface methods
    virtual void ConnectInputData(Option_t *option = "");
    virtual void CreateOutputObjects();
    virtual void Exec(Option_t* option);
    virtual void SetDebugLevel(Int_t level) {fDebug = level;}
    virtual void Init() {;}
    virtual void RequireFreshBuffer() {fFreshBufferOnly = kTRUE;}
    // To be implemented by user
    virtual void UserCreateOutputObjects()  {;}
    virtual void UserExec(Option_t* /*option*/) {;}
    // Helpers for adding branches to the AOD
    virtual void   AddAODBranch(const char* cname, void* addobj, const char *fname="");
    virtual void   SelectCollisionCandidates(UInt_t offlineTriggerMask = IlcVEvent::kMB) {fOfflineTriggerMask = offlineTriggerMask;}
    // Getters
    virtual Int_t          DebugLevel()              {return fDebug;     }
    virtual IlcVEvent*     GetEvent(Int_t iev);
    virtual IlcAODEvent*   AODEvent()                {return fOutputAOD; }
    virtual TTree*         OutputTree()              {return fTreeA;     }
    virtual Long64_t       Entry()                   {return fEntry;     }
    virtual const char*    CurrentFileName();
    
  protected:
    Int_t                      fDebug;           //  Debug flag
    Int_t                      fEntry;           //  Current entry in the chain
    Bool_t                     fFreshBufferOnly; //  Flag for Exec call for fresh buffer only
    IlcMultiEventInputHandler* fInputHandler;    //! Input Handler
    IlcAODEvent*               fOutputAOD;       //! AOD out 
    TTree*                     fTreeA;           //  AOD output Tree
    // Event Selection
    UInt_t fOfflineTriggerMask;   //  Task processes collision candidates only

    ClassDef(IlcAnalysisTaskME, 1); // Analysis task for standard jet analysis
};
 
#endif

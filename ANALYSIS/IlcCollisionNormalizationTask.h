/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */


/* $Id$ */

#ifndef ILCCOLLISIONNORMILCZATIONTASK_H
#define ILCCOLLISIONNORMILCZATIONTASK_H

#include "IlcAnalysisTaskSE.h"

class IlcCollisionNormalization;

class IlcCollisionNormalizationTask : public IlcAnalysisTaskSE {
  public:
    IlcCollisionNormalizationTask();
    IlcCollisionNormalizationTask(const char* name);

    virtual ~IlcCollisionNormalizationTask();

    virtual void   UserCreateOutputObjects();
    virtual void   UserExec(Option_t*);
    virtual void   Terminate(Option_t*);

  Bool_t IsEventInBinZero(); // returns true if the event has to be put in the bin0.
  void SetMC(Bool_t flag = kTRUE) { fIsMC = flag; }
  
  //    void SetOption(const char* opt) { fOption = opt; }
    
  //    void SetCollisionNormalization(IlcCollisionNormalization* physicsSelection) { fCollisionNormalization = physicsSelection; }
  IlcCollisionNormalization* GetCollisionNormalization() const { return fCollisionNormalization; }

 protected:
  TList* fOutput;                  //! list send on output slot 1
  //    TString fOption;                 // option string  
  Bool_t fIsMC;
  
  IlcCollisionNormalization* fCollisionNormalization; // collision normalization class

 private:
    IlcCollisionNormalizationTask(const IlcCollisionNormalizationTask&);
    IlcCollisionNormalizationTask& operator=(const IlcCollisionNormalizationTask&);

  ClassDef(IlcCollisionNormalizationTask, 1);
};

#endif

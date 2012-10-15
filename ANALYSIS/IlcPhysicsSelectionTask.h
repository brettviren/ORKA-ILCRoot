/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */


/* $Id$ */

#ifndef ILCPHYSICSSELECTIONTASK_H
#define ILCPHYSICSSELECTIONTASK_H

#include "IlcAnalysisTaskSE.h"

class IlcPhysicsSelection;

class IlcPhysicsSelectionTask : public IlcAnalysisTaskSE {
  public:
    IlcPhysicsSelectionTask();
    IlcPhysicsSelectionTask(const char* opt);

    virtual ~IlcPhysicsSelectionTask();

    virtual void   UserCreateOutputObjects();
    virtual void   UserExec(Option_t*);
    virtual void   FinishTaskOutput();
    virtual void   Terminate(Option_t*);

    void SetOption(const char* opt) { fOption = opt; }
    
    void SetPhysicsSelection(IlcPhysicsSelection* physicsSelection) { fPhysicsSelection = physicsSelection; }
    IlcPhysicsSelection* GetPhysicsSelection() const { return fPhysicsSelection; }

 protected:
    TList* fOutput;                  //! list send on output slot 1
    TString fOption;                 // option string  
    
    IlcPhysicsSelection* fPhysicsSelection; // event selection class

 private:
    IlcPhysicsSelectionTask(const IlcPhysicsSelectionTask&);
    IlcPhysicsSelectionTask& operator=(const IlcPhysicsSelectionTask&);

  ClassDef(IlcPhysicsSelectionTask, 1);
};

#endif

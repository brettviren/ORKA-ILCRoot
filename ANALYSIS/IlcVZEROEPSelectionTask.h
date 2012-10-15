#ifndef ILCVZEROEPSELECTIONTASK_H
#define ILCVZEROEPSELECTIONTASK_H

/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

//*****************************************************
//   Class IlcVZEROEPSelectionTask
//   author: Cvetan Cheshkov
//   30/01/2012
//   This analysis task reads the OADB and
//   provides the parameters needed to flatten
//   the VZERO event plane in IlcEventplane
//*****************************************************

#include "IlcAnalysisTaskSE.h"

class TProfile;

class IlcOADBContainer;
class IlcEventplane;

class IlcVZEROEPSelectionTask : public IlcAnalysisTaskSE {

 public:
  IlcVZEROEPSelectionTask();
  IlcVZEROEPSelectionTask(const char *name);
  virtual ~IlcVZEROEPSelectionTask();

  // Implementation of interface methods
  virtual void UserCreateOutputObjects();
  virtual void UserExec(Option_t *option);
  virtual void Terminate(Option_t *option);
  
  void SetUserParams(const char* inFileName, const char* listName);
  void UseVZEROCentrality() {fUseVZEROCentrality = kTRUE;}
  
 private:
  void SetEventplaneParams(IlcEventplane *esdEP,Float_t percentile);
  void SetHistograms(TList *list);
  void SetParamsFromOADB();
   
  IlcVZEROEPSelectionTask(const IlcVZEROEPSelectionTask& ep);
  IlcVZEROEPSelectionTask& operator= (const IlcVZEROEPSelectionTask& ep); 

  Int_t    fRunNumber;			// runnumber
  Bool_t   fUserParams;		        // in case one wants to use custom flatenning params
  Bool_t   fUseVZEROCentrality;         // use VZERO centrality estimator instead of SPD
  IlcOADBContainer* fVZEROEPContainer;	// VZERO event-plane OADB Container

  TProfile *fX2In[8];                   // Profile histogram for Q^2_x (read from input file)
  TProfile *fY2In[8];                   // Profile histogram for Q^2_y (read from input file)
  TProfile *fX2Y2In[8];                 // Profile histogram for Q^2_x*Q^2_y (read from input file)
  TProfile *fCos8PsiIn[8];              // Profile histogram for Cos(8*Psi) (read from input file)

  ClassDef(IlcVZEROEPSelectionTask,1) 
};

#endif


#ifndef ILCANALYSISTASKPIDRESPONSE_H
#define ILCANALYSISTASKPIDRESPONSE_H

/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcAnalysisTaskPIDResponse.h 43642 2010-09-17 15:50:04Z wiechula $ */
// Author: Jens Wiechula, 24/02/2011

//==============================================================================
//
//
//
//
//==============================================================================

#include <TVectorDfwd.h>
#include <TString.h>

#ifndef ILCANALYSISTASKSE_H
#include "IlcAnalysisTaskSE.h"
#endif

class IlcPIDResponse;
class IlcVEvent;

class IlcAnalysisTaskPIDResponse : public IlcAnalysisTaskSE {
  
  
public:
  IlcAnalysisTaskPIDResponse();
  IlcAnalysisTaskPIDResponse(const char *name);
  virtual ~IlcAnalysisTaskPIDResponse();

  void SetIsMC(Bool_t isMC=kTRUE) { fIsMC=isMC; }
  
  virtual void UserCreateOutputObjects();
  
  virtual void UserExec(Option_t */*option*/);

  void SetOADBPath(const char* path) {fOADBPath=path;}
  const char* GetOADBPath() const { return fOADBPath.Data(); }

private:
  Bool_t fIsMC;                        //  If we run on MC data
  TString fOADBPath;                   // OADB path to use
  
  IlcPIDResponse *fPIDResponse;        //! PID response Handler
  Int_t   fRun;                        //! current run number
  Int_t   fOldRun;                     //! current run number
  Int_t   fRecoPass;                   //! reconstruction pass
  
  //
  void SetRecoInfo();
    
  IlcAnalysisTaskPIDResponse(const IlcAnalysisTaskPIDResponse &other);
  IlcAnalysisTaskPIDResponse& operator=(const IlcAnalysisTaskPIDResponse &other);
  
  ClassDef(IlcAnalysisTaskPIDResponse,2)  // Task to properly set the PID response functions of all detectors
};
#endif

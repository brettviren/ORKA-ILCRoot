#ifndef ILCANALYSISTASKPIDQA_H
#define ILCANALYSISTASKPIDQA_H

/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcAnalysisTaskPIDqa.h 43642 2010-09-17 15:50:04Z wiechula $ */
// Author: Jens Wiechula, 24/02/2011

//==============================================================================
//
//
//
//
//==============================================================================

#include <TVectorDfwd.h>

#ifndef ILCANALYSISTASKSE_H
#include "IlcAnalysisTaskSE.h"
#endif

class IlcPIDResponse;
class TList;
class IlcVEvent;
class IlcESDv0KineCuts;

class IlcAnalysisTaskPIDqa : public IlcAnalysisTaskSE {
  
  
public:
  IlcAnalysisTaskPIDqa();
  IlcAnalysisTaskPIDqa(const char *name);
  virtual ~IlcAnalysisTaskPIDqa();

  virtual void UserCreateOutputObjects();
  
  virtual void UserExec(Option_t */*option*/);

  
private: 
  IlcPIDResponse *fPIDResponse;             //! PID response Handler
  IlcESDv0KineCuts *fV0cuts;                //! ESD V0 cuts

  TObjArray *fV0electrons;                  //! array with pointer to identified particles from V0 decays (electrons)
  TObjArray *fV0pions;                      //! array with pointer to identified particles from V0 decays (pions)
  TObjArray *fV0kaons;                      //! array with pointer to identified particles from V0 decays (kaons)
  TObjArray *fV0protons;                    //! array with pointer to identified particles from V0 decays (ptotons)

  TList                 *fListQA;           //! list with all QA histograms
  TList                 *fListQAits;        //! List with ITS QA histograms
  TList                 *fListQAitsSA;      //! List with ITS SA QA histograms
  TList                 *fListQAitsPureSA;  //! List with ITS pure SA QA histograms
  TList                 *fListQAtpc;        //! List with TPC QA histograms
  TList                 *fListQAtrd;        //! List with TRD QA histograms
  TList                 *fListQAtof;        //! List with TOF QA histograms
  TList                 *fListQAemcal;      //! List with EMCAL QA histograms
  TList                 *fListQAhmpid;      //! List with EMCAL QA histograms
  TList                 *fListQAtofhmpid;   //! List with EMCAL QA histograms
  TList                 *fListQAtpctof;     //! List with combined PID from TPC + TOF
  TList                 *fListQAV0;         //! List with V0 kine cuts QA histograms

  
  void ExecNewRun();

  //qa object initialisation
  void SetupITSqa();
  void SetupTPCqa();
  void SetupTRDqa();
  void SetupTOFqa();
  void SetupEMCALqa();
  void SetupHMPIDqa();
  void SetupTOFHMPIDqa();
  void SetupTPCTOFqa();
  void SetupV0qa();

  //
  void FillV0PIDlist();
  void ClearV0PIDlist();
  //
  void FillITSqa();
  void FillTPCqa();
  void FillTRDqa();
  void FillTOFqa();
  void FillEMCALqa();
  void FillHMPIDqa();
  void FillTOFHMPIDqa();
  void FillTPCTOFqa();
  
  //
  void SetRecoInfo();
  
  //helper functions
  TVectorD* MakeLogBinning(Int_t nbinsX, Double_t xmin, Double_t xmax);
  TVectorD* MakeLinBinning(Int_t nbinsX, Double_t xmin, Double_t xmax);
  TVectorD* MakeArbitraryBinning(const char* bins);
  
  
  IlcAnalysisTaskPIDqa(const IlcAnalysisTaskPIDqa &other);
  IlcAnalysisTaskPIDqa& operator=(const IlcAnalysisTaskPIDqa &other);
  
  ClassDef(IlcAnalysisTaskPIDqa,1)  // Task to properly set the PID response functions of all detectors
};
#endif

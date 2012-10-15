#ifndef ILCPIDCOMBINEDTASK_H
#define ILCPIDCOMBINEDTASK_H
/* 2012-2013 Infrastructure for Large Collider Experiment, All rights reserved. *
 * See cxx source for full Copyright notice                               */

//#########################################################
//#                                                       # 
//#        Task for testing the combined PID              #
//#                                                       #
//#  Pietro Antonioli, INFN / Pietro.Antonioli@bo.infn.it #
//#  Jens Wiechula, Uni Tübingen / Jens.Wiechula@cern.ch  #
//#                                                       #
//#########################################################

#include <IlcPID.h>
#include <IlcPIDResponse.h>
#include <IlcPIDCombined.h>

#include <IlcESDtrackCuts.h>
#include <IlcAnalysisFilter.h>

#include "IlcAnalysisTaskSE.h"

class TH1F;
class TH2D;
class fPIDResponse;
class fPIDCombined;

class IlcAnalysisTaskPIDCombined : public IlcAnalysisTaskSE {
  
public:
  static const Int_t kPtBins = 6;


  IlcAnalysisTaskPIDCombined();
  IlcAnalysisTaskPIDCombined(const char *name);
  virtual ~IlcAnalysisTaskPIDCombined(){;}
  
  virtual void  UserExec(Option_t *option);
  virtual void  UserCreateOutputObjects();
    

private:

  TList fHistList;                   //! list of histograms
  TH2D *fProbTPCnSigma[IlcPID::kSPECIES];    //! probabilities vs nSigma in the TPC
  TH2D *fProbTOFnSigma[IlcPID::kSPECIES];    //! probabilities vs nSigma  the TOF
  TH2D *fProbTPCTOFnSigmaTPC[IlcPID::kSPECIES];    //! comb. probabilities vs nSigma TPC
  TH2D *fProbTPC[IlcPID::kSPECIES];          //! probabilities vs mom in the TPC
  TH2D *fProbTOF[IlcPID::kSPECIES];          //! probabilities vs mom in the TOF
  TH2D *fProbTPCTOF[IlcPID::kSPECIES];       //! combined probabilities vs mom TPC-TOF
  TH1F *fPriors[IlcPID::kSPECIES];           //! priors

  TH2D *fProbTPCTOFnSigTPCMom[kPtBins][IlcPID::kSPECIES];  // prob. x mom. bins
  TH2D *fProbTPCnSigTPCMom[kPtBins][IlcPID::kSPECIES];     // prob. x mom. bins
  TH2D *fProbTOFnSigTOFMom[kPtBins][IlcPID::kSPECIES];     // prob. x mom. bins

  TH2D *fPriorsUsed[IlcPID::kSPECIES];       //! priors used

  const IlcPIDResponse *fPIDResponse;     //! PID response object
  IlcPIDCombined       *fPIDCombined;     //! combined PID object
  IlcESDtrackCuts      *fTrackCuts;            //! track selection
  IlcAnalysisFilter    *fTrackFilter;         //! track filter

  IlcAnalysisTaskPIDCombined(const IlcAnalysisTaskPIDCombined &c);
  IlcAnalysisTaskPIDCombined& operator= (const IlcAnalysisTaskPIDCombined &c);

  void FillHistogram(const char* name, Double_t x, Double_t weight=1.);
  void FillHistogram(const char* name, Double_t x, Double_t y, Double_t weight=1.);
  Int_t GetMomBin(Float_t mom);
  static const char* fgkBinMomDesc[kPtBins];
  
  ClassDef(IlcAnalysisTaskPIDCombined, 2);
};
#endif

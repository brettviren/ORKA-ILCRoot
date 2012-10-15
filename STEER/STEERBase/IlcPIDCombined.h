#ifndef ILCPIDCOMBINED_H
#define ILCPIDCOMBINED_H
/* 2012-2013 Infrastructure for Large Collider Experiment, All rights reserved. *
 * See cxx source for full Copyright notice                               */

//---------------------------------------------------------------//
//        Base class for combining PID response of               //
//        of different detectors                                 //
//        and compute Bayesian probabilities                     //
//                                                               //
//   Origin: Pietro Antonioli, INFN-BO, pietro.antonioli@cern.ch //
//                                                               //
//---------------------------------------------------------------//



#include <TNamed.h>
#include <IlcPID.h>
#include <IlcPIDResponse.h>
#include <TH1F.h>
#include <TH2F.h>

//class TH1;
class IlcPIDResponse;

class IlcPIDCombined : public TNamed {
public:
  IlcPIDCombined();
  IlcPIDCombined(const TString& name, const TString& title);
  virtual ~IlcPIDCombined();

  void SetDetectorMask(Int_t mask) {fDetectorMask=mask;}
  Int_t GetDetectorMask() const {return fDetectorMask;}
  void SetRejectMismatchMask(Int_t mask) {fRejectMismatchMask=mask;}
  Int_t GetRejectMismatchMask() const {return fRejectMismatchMask;}
  void SetEnablePriors(Bool_t flag) {fEnablePriors=flag;}
  Bool_t GetEnablePriors() const {return fEnablePriors;}
  void SetPriorDistribution(IlcPID::EParticleType type,TH1F *prior);
  //  const TH1* GetPriorDistribution(IlcPID::EParticleType type) const {return (TH1*)fPriorsDistributions[type];}
  TH1* GetPriorDistribution(IlcPID::EParticleType type)  const {return (TH1*)fPriorsDistributions[type];}
  
  void GetPriors(const IlcVTrack *track,Double_t* p,const IlcPIDResponse *response,UInt_t detUsed) const;
  
  void SetDefaultTPCPriors();
	
  UInt_t ComputeProbabilities(const IlcVTrack *track, const IlcPIDResponse *response, Double_t* bayesProbabilities) const;
  void SetSelectedSpecies(Int_t selectedSpecies) {fSelectedSpecies = selectedSpecies;}
  Int_t GetSelectedSpecies() const {return fSelectedSpecies;}

protected:
  void GetPriors(const IlcVTrack *track,Double_t* priors,Float_t centrality=-1) const;
  void ComputeBayesProbabilities(Double_t* bayesProbabilities,const Double_t* probDensity, const Double_t* priors) const;
  void SetCombinedStatus(const IlcPIDResponse::EDetPidStatus status,UInt_t *mask, const IlcPIDResponse::EDetCode bit, Double_t* p) const;

private:
  IlcPIDCombined(const IlcPIDCombined&);
  IlcPIDCombined &operator=(const IlcPIDCombined&);

  Int_t fDetectorMask;       // Detectors included in combined pid
  Int_t fRejectMismatchMask; // Detectors set return flat prob. if mismatch detected 
  Bool_t fEnablePriors;      // Enable bayesian PID (if kFALSE priors set flat)
  Int_t fSelectedSpecies;    // Number of selected species to study
  TH1F *fPriorsDistributions[IlcPID::kSPECIES+IlcPID::kSPECIESLN]; // priors
  Bool_t fUseDefaultTPCPriors; // switch to use Defaul TPC Priors
  static TH2F *fDefaultPriorsTPC[5]; // Default priors for TPC tracks

  ClassDef(IlcPIDCombined,2);
};

#endif

#ifndef ILCJETHADRONCORRECTIONV1_H
#define ILCJETHADRONCORRECTIONV1_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice     */

//                  
//*-- Author: Mark Horner (LBL/UCT)
//
#include "IlcJetHadronCorrection.h"

#define HCPARAMETERS    8 
#define HCPARAMETERSETS 3 

class IlcJetDummyGeo;

class IlcJetHadronCorrectionv1 : public IlcJetHadronCorrection 
{
 public:
  static IlcJetHadronCorrectionv1* Instance();
  virtual ~IlcJetHadronCorrectionv1() {}

  virtual Double_t GetEnergy(Double_t pmom, Double_t eta, Int_t gid); 
  Double_t GetEnergy(Double_t pmom, Double_t eta){return GetEnergy(pmom,eta,7);}
  
  void SetGeometry(TString name, Double_t fs = 1.); 
  void SetGeometry2(const IlcJetDummyGeo *geometry);
// Temporarily commented - will be updated removing the IlcAODpid dependence
//  void TrackPositionEMCal(const IlcAODTrack* track,Double_t &eta, Double_t &phi);

 protected:
  IlcJetHadronCorrectionv1():fSamplingFraction(0) {for (Int_t i = 0; i < 8; i++) fPar[i] = 0.;}
  IlcJetHadronCorrectionv1(const char *name, const char *title);

 private:
  void SetParameters(TString name = "") {Warning("SetParameter","Dummy method with argument %s",name.Data());}

  static IlcJetHadronCorrectionv1* fgHadrCorr;  // Pointer to global instance (singleton)
  static Double_t fgParLookup[HCPARAMETERS][HCPARAMETERSETS]; // Global array with parameters for hadronic response
  Double_t fPar[8];            // Parameters
  Float_t  fSamplingFraction;  // Sampling fraction
    
  ClassDef(IlcJetHadronCorrectionv1,2) // Hadron correction for EMC (version for MDC)
};
	
#endif // ILCJETHADRONCORRECTIONV1_H
	

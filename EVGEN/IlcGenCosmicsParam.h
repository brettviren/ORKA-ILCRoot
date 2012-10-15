#ifndef ILCGENCOSMICSPARAM_H
#define ILCGENCOSMICSPARAM_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

// Generator for muons according to kinematic parametrizations at ILC
// (not at the surface).
// Origin: andrea.dainese@lnl.infn.it


#include "IlcLog.h"
#include "IlcGenerator.h"

class IlcGenCosmicsParam : public IlcGenerator
{
public:

  IlcGenCosmicsParam();
  virtual ~IlcGenCosmicsParam() {}
  virtual void Generate();
  virtual void Init();
  void SetParamMI() { fParamMI=kTRUE; fParamACORDE=kFALSE; fParamDataTPC=kFALSE; return; }
  void SetParamACORDE() { fParamMI=kFALSE; fParamACORDE=kTRUE; fParamDataTPC=kFALSE; return; }
  void SetParamDataTPC() { fParamMI=kFALSE; fParamACORDE=kFALSE; fParamDataTPC=kTRUE; return; }
  void SetYOrigin(Float_t y=600.) { fYOrigin=y; return; }
  void SetMaxAngleWRTVertical(Float_t max=45.) { 
      if(max<0. || max>90.) IlcFatal("angle must be in [0,pi/2]");
      fMaxAngleWRTVertical=max; return; }
  void SetBkG(Float_t b) { fBkG=b; return; }
  void SetInACORDE(Bool_t onlyACORDE4ITS=kFALSE) 
    { fACORDE=kTRUE; fACORDE4ITS=onlyACORDE4ITS; return; }
  void SetInBottomScintillator() { fBottomScintillator=kTRUE; return; }
  void SetInTPC() { fTPC=kTRUE; return; }
  void SetInITS() { fITS=kTRUE; return; }
  void SetInSPDinner() { fSPDinner=kTRUE; return; }
  void SetInSPDouter() { fSPDouter=kTRUE; return; }
  void SetInSDDinner() { fSDDinner=kTRUE; return; }
  void SetInSDDouter() { fSDDouter=kTRUE; return; }
  void SetInSSDinner() { fSSDinner=kTRUE; return; }
  void SetInSSDouter() { fSSDouter=kTRUE; return; }

private:

  Bool_t IntersectCylinder(Float_t r,Float_t z,Int_t pdg,
			   Float_t o[3],Float_t p[3]) const;  
  Bool_t IntersectACORDE(Int_t pdg,
			 Float_t o[3],Float_t p[3]) const;
  Bool_t IntersectBottomScintillator(Int_t pdg,
				     Float_t o[3],Float_t p[3]) const; 
  Bool_t fParamMI;              // parametrization from M.Ivanov
  Bool_t fParamACORDE;          // parametrization from IlcGenACORDE 
  Bool_t fParamDataTPC;         // parametrization from TPC Summer08 cosmics 
                                // (parametrized at ILC y)
  Float_t fYOrigin;             // y of muon origin
  Float_t fMaxAngleWRTVertical; // maximum angle between momentum and y axis
  Float_t fBkG;                 // field in kGauss
  Bool_t fTPC;                  // acceptance cuts
  Bool_t fITS;                  // acceptance cuts
  Bool_t fSPDinner;             // acceptance cuts
  Bool_t fSPDouter;             // acceptance cuts
  Bool_t fSDDinner;             // acceptance cuts
  Bool_t fSDDouter;             // acceptance cuts
  Bool_t fSSDinner;             // acceptance cuts
  Bool_t fSSDouter;             // acceptance cuts
  Bool_t fACORDE;               // acceptance cuts
  Bool_t fACORDE4ITS;           // acceptance cuts
  Bool_t fBottomScintillator;   // acceptance cuts

  ClassDef(IlcGenCosmicsParam,5) // parametrized cosmics generator
};

#endif

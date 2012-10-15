#ifndef ILCGENFIXED_H
#define ILCGENFIXED_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcGenFixed.h 18739 2007-05-25 09:58:35Z morsch $ */

// Simple particle gun. 
// Momentum, phi and theta of the particle as well as the particle type can be set.
// andreas.morsch@cern.ch


#include "IlcGenerator.h"

class IlcGenFixed : public IlcGenerator
{
 public:
  IlcGenFixed();
  IlcGenFixed(Int_t npart);
  virtual ~IlcGenFixed() {}
  virtual void Generate();
  virtual void Init() {}
  virtual void SetSigma(Float_t sx, Float_t sy, Float_t sz);
  virtual void SetMomentum(Float_t pmom) {fPMin=pmom; fPMax=pmom; SetBit(kMomentumRange);}
  virtual void SetPhi(Float_t phi) {fPhiMin=phi*TMath::Pi()/180; fPhiMax=phi*TMath::Pi()/180; SetBit(kPhiRange);}
  virtual void SetTheta(Float_t theta) {fThetaMin=theta*TMath::Pi()/180; fThetaMax=theta*TMath::Pi()/180; SetBit(kThetaRange);}
  virtual void SetPart(Int_t part) {fIpart=part;}
  virtual void SetGun(Double_t px, Double_t py, Double_t pz, Double_t x, 
  Double_t y, Double_t z) {fP[0]=px;fP[1]=py;fP[2]=pz;fOrigin[0]=x;fOrigin[1]=y;
  fOrigin[2]=z;fExplicit=kTRUE;}
 
protected:

  Int_t fIpart;    // Particle type
  Int_t fExplicit; // True if momentum vector has been set by user. 
  Float_t fP[3];   // Momentum vector

  ClassDef(IlcGenFixed,1) // Single particle generator
};
#endif

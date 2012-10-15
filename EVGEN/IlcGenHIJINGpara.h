#ifndef ILCGENHIJINGPARA_H
#define ILCGENHIJINGPARA_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcGenHIJINGpara.h 51126 2011-08-17 13:37:49Z cvetan $ */

// Parameterisation of pi and K, eta and pt distributions
// used for the ILC TDRs.
// Author: andreas.morsch@cern.ch

class TF1;

#include "IlcGenerator.h"
#include "IlcDecayer.h"

class IlcGenHIJINGpara : public IlcGenerator
{
 public:

  IlcGenHIJINGpara();
  IlcGenHIJINGpara(Int_t npart);
  virtual ~IlcGenHIJINGpara();
  virtual void SetCutVertexZ(Float_t cut=999999.) {fCutVertexZ = cut;}
  virtual void Generate();
  virtual void Init();
  virtual void SetPtRange(Float_t ptmin = 0., Float_t ptmax=15.);
  virtual void SetPi0Decays(Bool_t flag = kFALSE) {fPi0Decays = flag;}
  virtual void SetWeighting(Int_t flag = 0) {fAnalog = flag;}
  virtual void Draw( const char *);
  
 protected:
  Int_t   fNt;          // CurrentTrack;
  Int_t   fNpartProd;   // Total number of particles produced  
  Bool_t  fPi0Decays;   // Flag for pi0 decays
  Float_t fPtWgtPi;     // pt weight for pions
  Float_t fPtWgtKa;     // pt weight for kaons
  TF1* fPtpi;           // Parametrised pt distribution for pi
  TF1* fPtka;           // Parametrised pt distribution for ka
  TF1* fETApic;         // Parametrised eta distribution for pi
  TF1* fETAkac;         // Parametrised eta distribution fro ka
  IlcDecayer* fDecayer; // ! Pointer to pythia object for decays

  void DecayPi0(Float_t* orig, Float_t * p, Float_t time);

 private:
  IlcGenHIJINGpara(const IlcGenHIJINGpara &HIJINGpara);
  IlcGenHIJINGpara& operator = (const IlcGenHIJINGpara &para) ;


  ClassDef(IlcGenHIJINGpara,3) // Hijing parametrisation generator
};
#endif











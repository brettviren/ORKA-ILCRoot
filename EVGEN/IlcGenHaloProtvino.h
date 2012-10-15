#ifndef ILCGENHALOPROTVINO_H
#define ILCGENHALOPROTVINO_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id $ */


#include "IlcGenerator.h"
#include <TString.h>

// Read background particles from a boundary source
// Very specialized generator to simulate background from beam halo.
// Author: andreas.morsch@cern.ch

class IlcGenHaloProtvino : public IlcGenerator
{
public:
    enum constants{kY1Day0, kY1Day70, kY2D0, kY2D10, kY3D90, kLHCPR674Startup, kLHCPR674Conditioned};
    IlcGenHaloProtvino();
    IlcGenHaloProtvino(Int_t npart);
    virtual ~IlcGenHaloProtvino();
    virtual void Init();
    virtual void SetFileName(TString filename) {fFileName=TString(filename);}
    virtual void Generate();
    virtual Float_t GasPressureWeight(Float_t zPrimary);
    virtual void SetSide(Int_t flag = 1) {fSide = flag;}
    virtual void SetNskip(Int_t nskip) {fNskip = nskip;}
    virtual void SetRunPeriod(Int_t t = kY3D90) {fRunPeriod = t;}
    virtual void SetTimePerEvent(Float_t t = 1.e-4) {fTimePerEvent = t;}
    virtual void Draw(Option_t * opt="");

protected:
  FILE*    fFile;                       // ! Pointer to file
  TString  fFileName;                   //   Choose the file
  Int_t    fSide;                       //   Muon arm side (1) / Castor side (-1)
  Int_t    fRunPeriod;                  //   LHC Running Period
  Float_t  fTimePerEvent;               //   Time corresponding to one event [s]
  Int_t    fNskip;                      //   Number of entries to skip
  Float_t* fZ1;                         // ! z-positions for gas pressure tables
  Float_t* fZ2;                         // ! z-positions for gas pressure tables 
  Float_t* fG1;                         // ! gas pressures
  Float_t* fG2;                         // ! gas pressures
  Int_t    fGPASize;                    // ! Size of arrays
 private:
  IlcGenHaloProtvino(const IlcGenHaloProtvino &HaloProtvino);
  IlcGenHaloProtvino & operator=(const IlcGenHaloProtvino & rhs);

  ClassDef(IlcGenHaloProtvino,1)        //   LHC background boundary source (Protvino Group results)
      

};
#endif







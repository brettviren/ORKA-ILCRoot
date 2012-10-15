#ifndef ILCDECAYERPYTHIA8_H
#define ILCDECAYERPYTHIA8_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id$*/

// Implementation of TVirtualMCDecayer using Pythia8
// Author: andreas.morsch@cern.ch

#include <ParticleData.h>
#include <TVirtualMCDecayer.h>
#include "IlcDecayer.h"

class IlcDecayerPythia8 : public TVirtualMCDecayer {
 public:
  IlcDecayerPythia8();
  virtual ~IlcDecayerPythia8(){;}
  virtual void    Init();
  virtual void    Decay(Int_t pdg, TLorentzVector* p);
  virtual Int_t   ImportParticles(TClonesArray *particles);
  virtual void    SetForceDecay(Decay_t decay) {fDecay=decay;}
  virtual void    SetForceDecay(Int_t decay)   {SetForceDecay((Decay_t) decay);}
  virtual void    ForceDecay();
  virtual Float_t GetPartialBranchingRatio(Int_t ipart);
  virtual void    HeavyFlavourOff() {fHeavyFlavour = kFALSE;}
  virtual Float_t GetLifetime(Int_t kf);
  virtual void    ReadDecayTable();

  virtual void    SetDebugLevel(Int_t debug) {fDebug = debug;}

 protected:
   void AppendParticle(Int_t pdg, TLorentzVector* p);
   void ClearEvent(); 
 private:
  IlcDecayerPythia8(const IlcDecayerPythia8&);
  IlcDecayerPythia8 operator=(const IlcDecayerPythia8&);
  void     SwitchOffHeavyFlavour();
  void     ForceHadronicD(Int_t optUser4Bodies = 1);

  IlcTPythia8*  fPythia8;          // Pointer to pythia8
  Int_t         fDebug;            // Debug level
 
  Decay_t       fDecay;            //  Forced decay mode
  Bool_t        fHeavyFlavour;     //! Flag for heavy flavors
  static Bool_t fgInit;            //! initialization flag 
  ClassDef(IlcDecayerPythia8, 1) // Particle Decayer using Pythia8
};
#endif








#ifndef ILCGENBOX_H
#define ILCGENBOX_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcGenBox.h 50426 2011-07-05 12:27:46Z morsch $ */


// Generator for particles in a preset
// kinematic range (flat distribution)
// Comments and suggestions: andreas.morsch@cern.ch


#include "IlcGenerator.h"
class IlcGenBox : public IlcGenerator
{
 public:

  IlcGenBox();
  IlcGenBox(Int_t npart);
  virtual ~IlcGenBox() {}
  virtual void Generate();
  virtual void Init();
  virtual void SetEtaRange(Float_t etamin, Float_t etamax)
      {SetBit(kEtaRange);fEtaMin = etamin; fEtaMax = etamax;}
  virtual void SetPart(Int_t part) {fIpart=part;}
  virtual void SetParticleType(Int_t part) {SetPart(part);}
protected:

  Int_t fIpart; // Particle type
  Float_t fEtaMin;  // Minimum eta 
  Float_t fEtaMax;  // Maximum eta
  ClassDef(IlcGenBox,2) // Square box random generator
};

#endif

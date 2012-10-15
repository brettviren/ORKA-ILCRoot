#ifndef ILCGENTHETASLICE_H
#define ILCGENTHETASLICE_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcGenThetaSlice.h 8644 2003-11-14 11:01:41Z morsch $ */


// Generates n particles with in the same phi angle, varies theta
// In equidistant intervals
// Comments and suggestions: Jiri.Chudoba@cern.ch


#include "IlcGenerator.h"
class IlcGenThetaSlice : public IlcGenerator
{
 public:

  IlcGenThetaSlice();
  IlcGenThetaSlice(Int_t npart);
  virtual ~IlcGenThetaSlice() {}
  virtual void Generate();
  virtual void Init();
  virtual void SetPart(Int_t part) {fIpart=part;}
protected:

  Int_t fIpart; // Particle type

  ClassDef(IlcGenThetaSlice,1) // theta slices phi constant random generator
};

#endif

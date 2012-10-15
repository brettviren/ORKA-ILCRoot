#ifndef ILCGENBEAMGASNEW_H
#define ILCGENBEAMGASNEW_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcGenBeamGasNew.h 23866 2008-02-12 07:36:44Z hristov $ */

#include "IlcGenCocktail.h"

//
// Class for the simulation of beam gas events with correct timing.
// By default HIJING is used as a generator for pO collisions.
//
// Author: Jochen Klein
//

class IlcGenBeamGasNew : public IlcGenCocktail
{
 public:
  IlcGenBeamGasNew();
  virtual ~IlcGenBeamGasNew();

  virtual void Generate();
  void VertexInternal();
  virtual void Init();

  void SetTimeWindow(Float_t twindow);
  bool SetRate(Float_t rate);
  void SetZWindow(Float_t zwindow);

 protected:
  Float_t fItime;   // time of bg-interaction
  Float_t fTwindow; // time-window in which tpc-gate is open
  Float_t fZwindow; // extension of simulation in z-direction in cm
  Float_t fRate;    // rate for bg-interaction in Hz/m

 private:
  IlcGenBeamGasNew& operator=(const IlcGenBeamGasNew &rhs);
  IlcGenBeamGasNew(const IlcGenBeamGasNew& rhs);
  ClassDef(IlcGenBeamGasNew,1);

};
#endif

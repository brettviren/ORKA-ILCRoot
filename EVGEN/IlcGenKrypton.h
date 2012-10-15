#ifndef ILCGENKR_H
#define ILCGENKR_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcGenKrypton.h 19277 2007-06-24 20:53:11Z hristov $ */

//
// Generator of Krypton decay
//
#include "IlcGenerator.h"
class IlcGenKrypton : public IlcGenerator
{
public:
  IlcGenKrypton();
  virtual void Generate();
  virtual ~IlcGenKrypton(){}
  private:
  void KrDecay(Int_t &nelectron, Int_t &ngamma, Double_t *eelectron, Double_t *egamma);

  ClassDef(IlcGenKrypton,1)
};
#endif

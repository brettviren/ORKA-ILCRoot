#ifndef ILCAMPTRNDM_H
#define ILCAMPTRNDM_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcAmptRndm.h 44990 2010-11-04 15:16:40Z hristov $ */

#include <Rtypes.h>
#include <TError.h>

class TRandom;

class IlcAmptRndm {
 public:
  IlcAmptRndm() {}
  virtual ~IlcAmptRndm() {
    fgAmptRandom=0;
  }
  
  static void SetAmptRandom(TRandom *ran=0);
  static TRandom * GetAmptRandom();

private:
  IlcAmptRndm(const IlcAmptRndm &Ampt);
  IlcAmptRndm &operator=(const IlcAmptRndm &rhs);

  static TRandom * fgAmptRandom; //! pointer to the random number generator

  ClassDef(IlcAmptRndm,0)  //Random Number generator wrapper (non persistent)
};

#endif 


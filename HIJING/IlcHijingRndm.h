#ifndef ILCHIJINGRNDM_H
#define ILCHIJINGRNDM_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcHijingRndm.h 8921 2004-01-13 11:45:46Z hristov $ */

#include <Rtypes.h>
#include <TError.h>

class TRandom;

class IlcHijingRndm {
 public:
  IlcHijingRndm() {
    // Default constructor. The static data member is initialized
    // in the implementation file
  }
  IlcHijingRndm(const IlcHijingRndm &/*rn*/) {
    // Copy constructor: no copy allowed for the object
    ::Fatal("Copy constructor","Not allowed\n");
  }
  virtual ~IlcHijingRndm() {
    // Destructor
    fgHijingRandom=0;
  }
  IlcHijingRndm & operator=(const IlcHijingRndm& /*rn*/) {
    // Assignment operator: no assignment allowed
    ::Fatal("Assignment operator","Not allowed\n");
    return (*this);
  }
  
  static void SetHijingRandom(TRandom *ran=0);
  static TRandom * GetHijingRandom();

private:

  static TRandom * fgHijingRandom; //! pointer to the random number generator

  ClassDef(IlcHijingRndm,0)  //Random Number generator wrapper (non persistent)
};

#endif 


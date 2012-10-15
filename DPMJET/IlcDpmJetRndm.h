#ifndef ILCDPMJETRNDM_H
#define ILCDPMJETRNDM_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcDpmJetRndm.h 8922 2004-01-13 13:31:23Z hristov $ */

#include <Rtypes.h>
#include <TError.h>

class TRandom;

class IlcDpmJetRndm {
 public:
  IlcDpmJetRndm() {
    // Default constructor. The static data member is initialized 
    // in the implementation file
  }
  IlcDpmJetRndm(const IlcDpmJetRndm & /*rn*/) {
    // Copy constructor: no copy allowed for the object
    ::Fatal("Copy constructor","Not allowed\n");
  }
  virtual ~IlcDpmJetRndm() {
  // Destructor
    fgDpmJetRandom=0;
  }

  IlcDpmJetRndm & operator=(const IlcDpmJetRndm& /*rn*/) {
    // Assignment operator: no assignment allowed
    ::Fatal("Assignment operator","Not allowed\n");
    return (*this);
  }
  
  static void SetDpmJetRandom(TRandom *ran=0);
  static TRandom * GetDpmJetRandom();

private:

  static TRandom * fgDpmJetRandom; //! pointer to the random number generator

  ClassDef(IlcDpmJetRndm,0)  //Random Number generator wrapper (non persistent)
};

#endif 


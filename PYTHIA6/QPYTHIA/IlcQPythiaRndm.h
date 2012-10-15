#ifndef ILCPYTHIARNDM_H
#define ILCPYTHIARNDM_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcPythiaRndm.h 8920 2004-01-13 11:29:51Z hristov $ */

#include <Rtypes.h>
#include <TError.h>

class TRandom;

class IlcPythiaRndm {
 public:
  IlcPythiaRndm() {
    // Default constructor. The static data member is initialized 
    // in the implementation file
  }
  IlcPythiaRndm(const IlcPythiaRndm & /*rn*/) {
    // Copy constructor: no copy allowed for the object
    ::Fatal("Copy constructor","Not allowed\n");
  }
  virtual ~IlcPythiaRndm() {
    // Destructor
    fgPythiaRandom=0;
  }
  IlcPythiaRndm & operator=(const IlcPythiaRndm& /*rn*/) {
    // Assignment operator: no assignment allowed
    ::Fatal("Assignment operator","Not allowed\n");
    return (*this);
  }
  
  static void SetPythiaRandom(TRandom *ran=0);
  static TRandom * GetPythiaRandom();

private:

  static TRandom * fgPythiaRandom; //! pointer to the random number generator

  ClassDef(IlcPythiaRndm,0)  //Random Number generator wrapper (non persistent)
};

#endif 


#ifndef ILCHERWIGRNDM_H
#define ILCHERWIGRNDM_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcHerwigRndm.h 24631 2008-03-20 10:51:05Z fca $ */

//-----------------------------------------------------------------------------
//   Class: IlcHerwigRndm
//   Responsibilities: Interface to Root random number generator 
//                     from Fortran (re-implements FINCTION RLU_HERWIG 
//                     from HERWIG)
//-----------------------------------------------------------------------------

#include <Rtypes.h>
#include <TError.h>

class TRandom;

class IlcHerwigRndm {
 public:
  IlcHerwigRndm() {
    // Default constructor. The static data member is initialized
    // in the implementation file
  }
  IlcHerwigRndm(const IlcHerwigRndm &/*rn*/) {
    // Copy constructor: no copy allowed for the object
    ::Fatal("Copy constructor","Not allowed\n");
  }
  virtual ~IlcHerwigRndm() {
    // Destructor
    fgHerwigRandom=0;
  }
  IlcHerwigRndm & operator=(const IlcHerwigRndm& /*rn*/) {
    // Assignment operator: no assignment allowed
    ::Fatal("Assignment operator","Not allowed\n");
    return (*this);
  }
  
  static void SetHerwigRandom(TRandom *ran=0);
  static TRandom * GetHerwigRandom();

private:

  static TRandom * fgHerwigRandom; //! pointer to the random number generator

  ClassDef(IlcHerwigRndm,0)  //Random Number generator wrapper (non persistent)
};

#endif 


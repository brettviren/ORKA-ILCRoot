#ifndef ILCSLOWNUCLEONMODELEXP_H
#define ILCSLOWNUCLEONMODELEXP_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcSlowNucleonModelExp.h 9863 2004-05-04 15:09:55Z alibrary $ */

//
// Experimental data inspired Gray Particle Model for p-Pb collisions
// Fluctuations are calculated from a binomial distribution.
// Author: A.Morsch
//

#include "IlcSlowNucleonModel.h"

class IlcCollisionGeometry;

class IlcSlowNucleonModelExp : public IlcSlowNucleonModel
{
 public:
    IlcSlowNucleonModelExp();
    virtual ~IlcSlowNucleonModelExp(){;}
    virtual void GetNumberOfSlowNucleons(IlcCollisionGeometry* geo,
					 Int_t& ngp, Int_t& ngn, Int_t& nbp, Int_t& nbn) const;
    virtual void SetParameters(Float_t alpha1, Float_t alpha2);
    
 protected:
    Float_t  fP;          // Number of protons  in the target 
    Float_t  fN;          // Number of neutrons in the target
    Float_t  fAlphaGray;  // Proportionality between gray   particles and number of collisions
    Float_t  fAlphaBlack; // Proportionality between black  particles and number of collisions
    
  ClassDef(IlcSlowNucleonModelExp,1) // Gray Particle Model (Experiment inspired)
};
#endif







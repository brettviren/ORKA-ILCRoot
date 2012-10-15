/**************************************************************************
 * 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * Author: The International Lepton Collider Off-line Project.            *
 *                                                                        *
 * Part of the code has been developed by ILC Off-line Project.         *
 * Contributors are mentioned in the code where appropriate.              *
 *                                                                        *
 * Permission to use, copy, modify and distribute this software and its   *
 * documentation strictly for non-commercial purposes is hereby granted   *
 * without fee, provided that the above copyright notice appears in all   *
 * copies and that both the copyright notice and this permission notice   *
 * appear in the supporting documentation. The authors make no claims     *
 * about the suitability of this software for any purpose. It is          *
 * provided "as is" without express or implied warranty.                  *
 **************************************************************************/

/* $Id: IlcPythiaRndm.cxx 31620 2009-03-20 15:31:17Z morsch $ */

//-----------------------------------------------------------------------------
//   Class: IlcPythiaRndm
//   Responsibilities: Interface to Root random number generator 
//                     from Fortran (re-implements FINCTION PYR from PYTHIA)
//                     Very similar to IlcHijingRndm
//   Collaborators: IlcPythia and IlcGenPythia classes
//   Example:
//
//   root> IlcPythia::Instance();
//   root> IlcPythiaRndm::SetPythiaRandom(new TRandom3()); 
//   root> IlcPythiaRndm::GetPythiaRandom()->SetSeed(0);
//   root> cout<<"Seed "<< IlcPythiaRndm::GetPythiaRandom()->GetSeed() <<endl;
//
//-----------------------------------------------------------------------------

#include <TMath.h>
#include <TRandom.h>

#include "IlcPythiaRndm.h"

TRandom * IlcPythiaRndm::fgPythiaRandom=0;

ClassImp(IlcPythiaRndm)


//_______________________________________________________________________
void IlcPythiaRndm::SetPythiaRandom(TRandom *ran) {
  //
  // Sets the pointer to an existing random numbers generator
  //
  if(ran) fgPythiaRandom=ran;
  else fgPythiaRandom=gRandom;
}

//_______________________________________________________________________
TRandom * IlcPythiaRndm::GetPythiaRandom() {
  //
  // Retrieves the pointer to the random numbers generator
  //
  return fgPythiaRandom;
}

//_______________________________________________________________________
#define pyr        pyr_
#define pygauss    pygauss_
#define pyrset     pyrset_
#define pyrget     pyrget_

extern "C" {
    Double_t pyr(Int_t*) 
    {
	// Wrapper to FUNCTION PYR from PYTHIA
	// Uses static method to retrieve the pointer to the (C++) generator
	Double_t r;
	do r=IlcPythiaRndm::GetPythiaRandom()->Rndm();
	while(0 >= r || r >= 1);
	return r;
    }
    
    Double_t pygauss(Double_t x0, Double_t sig)
    {
	Double_t s = 2.;
	Double_t v1 = 0.;
	Double_t v2 = 0.;
	
	while (s > 1.) {
	    v1 = 2. * pyr(0) - 1.;
	    v2 = 2. * pyr(0) - 1.;
	    s = v1 * v1 + v2 * v2;
	}
	return v1 * TMath::Sqrt(-2. * TMath::Log(s) / s) * sig + x0;
    }

    void pyrset(Int_t*,Int_t*) {}
    void pyrget(Int_t*,Int_t*) {}
}

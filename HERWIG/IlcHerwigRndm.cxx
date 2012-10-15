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

/* $Id: IlcHerwigRndm.cxx 30561 2009-01-09 14:20:18Z morsch $ */

//-----------------------------------------------------------------------------
//   Class: IlcHerwigRndm
//   Responsibilities: Interface to Root random number generator 
//                     from Fortran (re-implements FINCTION RLU_HERWIG 
//                     from HERWIG)
//   Note: Since IlcGenHerwig belongs to another module (THerwig) one cannot
//         pass the ponter to the generator via static variable
//   Collaborators: IlcGenHerwig class
//   Example:
//
//   root> IlcGenHerwig *gener = new IlcGenHerwig(-1);
//   root> IlcHerwigRndm::SetHerwigRandom(new TRandom3());
//   root> IlcHerwigRndm::GetHerwigRandom()->SetSeed(0);
//   root> cout<<"Seed "<< IlcHerwigRndm::GetHerwigRandom()->GetSeed() <<endl;
//-----------------------------------------------------------------------------

#include <TRandom.h>

#include "IlcHerwigRndm.h"

TRandom * IlcHerwigRndm::fgHerwigRandom=0;

ClassImp(IlcHerwigRndm)

//_______________________________________________________________________
void IlcHerwigRndm::SetHerwigRandom(TRandom *ran) {
  //
  // Sets the pointer to an existing random numbers generator
  //
  if(ran) fgHerwigRandom=ran;
  else fgHerwigRandom=gRandom;
}

//_______________________________________________________________________
TRandom * IlcHerwigRndm::GetHerwigRandom() {
  //
  // Retrieves the pointer to the random numbers generator
  //
  return fgHerwigRandom;
}

//_______________________________________________________________________
# define hwrgen    hwrgen_

extern "C" {
Double_t hwrgen(Int_t /*dummy*/) 
  {
    // Wrapper to FUNCTION HWR from HERWIG
    // Uses static method to retrieve the pointer to the (C++) generator
      Double_t r;
      do r=IlcHerwigRndm::GetHerwigRandom()->Rndm(); 
      while(0 >= r || r >= 1);
      return r;
  }
}

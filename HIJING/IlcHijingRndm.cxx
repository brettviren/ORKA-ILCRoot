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

/* $Id: IlcHijingRndm.cxx 20176 2007-08-15 17:14:00Z hristov $ */

//-----------------------------------------------------------------------------
//   Class: IlcHijingRndm
//   Responsibilities: Interface to Root random number generator 
//                     from Fortran (re-implements FINCTION RLU_HIJING 
//                     from HIJING)
//   Note: Since IlcGenHijing belongs to another module (THijing) one cannot
//         pass the ponter to the generator via static variable
//   Collaborators: IlcGenHijing class
//   Example:
//
//   root> IlcGenHijing *gener = new IlcGenHijing(-1);
//   root> IlcHijingRndm::SetHijingRandom(new TRandom3());
//   root> IlcHijingRndm::GetHijingRandom()->SetSeed(0);
//   root> cout<<"Seed "<< IlcHijingRndm::GetHijingRandom()->GetSeed() <<endl;
//-----------------------------------------------------------------------------

#include <TRandom.h>

#include "IlcHijingRndm.h"

TRandom * IlcHijingRndm::fgHijingRandom=0;

ClassImp(IlcHijingRndm)

//_______________________________________________________________________
void IlcHijingRndm::SetHijingRandom(TRandom *ran) {
  //
  // Sets the pointer to an existing random numbers generator
  //
  if(ran) fgHijingRandom=ran;
  else fgHijingRandom=gRandom;
}

//_______________________________________________________________________
TRandom * IlcHijingRndm::GetHijingRandom() {
  //
  // Retrieves the pointer to the random numbers generator
  //
  return fgHijingRandom;
}

//_______________________________________________________________________
# define rluget_hijing rluget_hijing_
# define rluset_hijing rluset_hijing_
# define rlu_hijing    rlu_hijing_

extern "C" {
  void rluget_hijing(Int_t & /*lfn*/, Int_t & /*move*/)
  {printf("Dummy version of rluget_hijing reached\n");}

  void rluset_hijing(Int_t & /*lfn*/, Int_t & /*move*/)
  {printf("Dummy version of rluset_hijing reached\n");}

  Float_t rlu_hijing(Int_t & /*idum*/) 
  {
    // Wrapper to FINCTION RLU_HIJING from HIJING
    // Uses static method to retrieve the pointer to the (C++) generator
      Double_t r;
      do r=IlcHijingRndm::GetHijingRandom()->Rndm(); 
      while(0 >= r || r >= 1);
      return (Float_t)r;
  }
}

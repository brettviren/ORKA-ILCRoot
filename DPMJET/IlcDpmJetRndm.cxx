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

/* $Id: IlcDpmJetRndm.cxx 25485 2008-04-28 15:57:44Z hristov $ */

//-----------------------------------------------------------------------------
//   Class: IlcDpmJetRndm
//   Responsibilities: Interface to Root random number generator 
//                     from Fortran (re-implements FINCTION dt_rndm_dpmjet)
//                     Very similar to IlcHijingRndm
//   Note: Since IlcGenDpmJet belongs to another module (TDPMjet) one cannot
//         pass the ponter to the generator via static variable
//   Collaborators: IlcGenDPMjet class
//-----------------------------------------------------------------------------

#include <TRandom.h>

#include "IlcDpmJetRndm.h"

TRandom * IlcDpmJetRndm::fgDpmJetRandom=0;

ClassImp(IlcDpmJetRndm)


//_______________________________________________________________________
void IlcDpmJetRndm::SetDpmJetRandom(TRandom *ran) {
  //
  // Sets the pointer to an existing random numbers generator
  //
  if(ran) fgDpmJetRandom=ran;
  else fgDpmJetRandom=gRandom;
}

//_______________________________________________________________________
TRandom * IlcDpmJetRndm::GetDpmJetRandom() {
  //
  // Retrieves the pointer to the random numbers generator
  //
  return fgDpmJetRandom;
}

#ifndef WIN32
# define dt_rndm_dpmjet   dt_rndm_dpmjet_
# define dt_rndmst_dpmjet dt_rndmst_dpmjet_
# define dt_rndmin_dpmjet dt_rndmin_dpmjet_
# define dt_rndmou_dpmjet dt_rndmou_dpmjet_
# define type_of_call
#else
# define dt_rndm_dpmjet   DT_RNDM_DPMJET_
# define dt_rndmst_dpmjet DT_RNDMST_DPMJET
# define dt_rndmin_dpmjet DT_RNDMIN_DPMJET
# define dt_rndmou_dpmjet DT_RNDMOU_DPMJET
# define type_of_call _stdcall
#endif


extern "C" {
  void type_of_call dt_rndmst_(Int_t &, Int_t &, Int_t &, Int_t &)
  {printf("Dummy version of dt_rndmst reached\n");}

  void type_of_call dt_rndmin_(Int_t &, Int_t &, Int_t &, Int_t &, Int_t &, Int_t &)
  {printf("Dummy version of dt_rndmin reached\n");}

  void type_of_call dt_rndmou_(Int_t &, Int_t &, Int_t &, Int_t &, Int_t &, Int_t &)
  {printf("Dummy version of dt_rndmou reached\n");}

  void type_of_call dt_rndmte_(Int_t &, Int_t &, Int_t &, Int_t &, Int_t &, Int_t &)
  {printf("Dummy version of dt_rndmou reached\n");}

  Double_t type_of_call dt_rndm_(Int_t &) 
  {
    // Wrapper to static method which retrieves the 
    // pointer to the Root (C++) generator
      Float_t r;
      do r = IlcDpmJetRndm::GetDpmJetRandom()->Rndm();
      while(0 >= r || r >= 1);
      return r;
  }
}



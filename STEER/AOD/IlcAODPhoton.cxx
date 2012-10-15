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

/* $Id: IlcAODPhoton.cxx 50615 2011-07-16 23:19:19Z hristov $ */

//-------------------------------------------------------------------------
//     AOD class for photons
//     Author: Yves Schutz, CERN
//-------------------------------------------------------------------------

#include <TLorentzVector.h>
#include "IlcAODPhoton.h"

ClassImp(IlcAODPhoton)


//______________________________________________________________________________
IlcAODPhoton::IlcAODPhoton() :
    IlcVParticle(),
    fMomentum(0)
{
  // constructor
}

IlcAODPhoton::IlcAODPhoton(Double_t px, Double_t py, Double_t pz, Double_t e):
    IlcVParticle(),
    fMomentum(0)
{
  // constructor
    fMomentum = new TLorentzVector(px, py, pz, e);
}

IlcAODPhoton::IlcAODPhoton(TLorentzVector & p):
    IlcVParticle(),
    fMomentum(0)
{
  // constructor
    fMomentum = new TLorentzVector(p);
}


//______________________________________________________________________________
IlcAODPhoton::~IlcAODPhoton() 
{
  // destructor
    delete fMomentum;
}

//______________________________________________________________________________
IlcAODPhoton::IlcAODPhoton(const IlcAODPhoton& photon) :
    IlcVParticle(photon),
    fMomentum(0)
{
  // Copy constructor
    fMomentum = new TLorentzVector(*photon.fMomentum);
    
}

//______________________________________________________________________________
IlcAODPhoton& IlcAODPhoton::operator=(const IlcAODPhoton& photon)
{
  // Assignment operator
  if(this!=&photon) {
  }

  return *this;
}

void IlcAODPhoton::Print(Option_t* /*option*/) const 
{
  // Print information of all data members
  printf("Photon 4-vector:\n");
  printf("     E  = %13.3f\n", E() );
  printf("     Px = %13.3f\n", Px());
  printf("     Py = %13.3f\n", Py());
  printf("     Pz = %13.3f\n", Pz());
}

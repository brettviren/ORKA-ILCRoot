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

//_________________________________________________________________________
//  Hits class for RSTACK     
//  A hit in RSTACK is the sum of all hits in a single scintillator tile
//  from one primary particle and within a time gate of 1 ps
//*--

// --- ROOT system ---
#include <TMath.h>

// --- Standard library ---

// --- IlcRoot header files ---
#include "IlcRSTACKHit.h"

ClassImp(IlcRSTACKHit)
  
  //____________________________________________________________________________
IlcRSTACKHit::IlcRSTACKHit(const IlcRSTACKHit & hit) :
IlcHit(hit),fId(hit.fId),fELOS(hit.fELOS),fTime(hit.fTime),fNPhotons(hit.fNPhotons),fDistFromLowerZ(hit.fDistFromLowerZ)
{
  // copy ctor
} 

//____________________________________________________________________________
IlcRSTACKHit::IlcRSTACKHit(Int_t shunt, Int_t track, Int_t id, Float_t *hits) : 
  IlcHit(shunt, track),fId(0),fELOS(0),fTime(0), fNPhotons(0.), fDistFromLowerZ(0)
{
  //
  // Create a hit object
  //
  
  fX           = hits[0] ;
  fY           = hits[1] ;
  fZ           = hits[2] ;
  fTime        = hits[3] ;
  fId          = id ;
  fELOS        = hits[4] ;
  fNPhotons    = hits[5];
  fDistFromLowerZ = hits[6];
}
//____________________________________________________________________________
Bool_t IlcRSTACKHit::operator==(IlcRSTACKHit const &rValue) const
{ 
  // Two hits are identical if they have the same Id, are originate from the same primary and are within a time gate of 1 ps

  Bool_t rv = kFALSE ; 

  if ( (fId == rValue.GetId()) && ( fTrack == rValue.GetPrimary() ) && ( TMath::Abs(fTime - rValue.GetTime()) < 1.e-12 ) )
    rv = kTRUE;
  
  return rv;
}

//____________________________________________________________________________
IlcRSTACKHit & IlcRSTACKHit::operator = (const IlcRSTACKHit &)
{
  Fatal("operator =", "not implemented");
  return *this;
}
//____________________________________________________________________________
IlcRSTACKHit IlcRSTACKHit::operator+(const IlcRSTACKHit &rValue)
{
  // Add the energy of the hit

  fELOS += rValue.GetEnergy() ;
  fNPhotons += rValue.GetNPhotons();

  if(rValue.GetTime() < fTime)
    fTime = rValue.GetTime() ;

   return *this;

}


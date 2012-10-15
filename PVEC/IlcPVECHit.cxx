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

/* $Id: IlcPVECHit.cxx 53186 2011-11-25 14:28:01Z kharlov $ */

//_________________________________________________________________________
//  Hits class for PVEC     
//  A hit in PVEC is the sum of all hits in a single crystal
//*--
//*-- Author: Maxime Volkov (RRC KI) & Yves Schutz (SUBATECH) & Dmitri Peressounko (RRC KI & SUBATECH)

// --- ROOT system ---
#include <TMath.h>

// --- Standard library ---

// --- IlcRoot header files ---
#include "IlcPVECHit.h"

ClassImp(IlcPVECHit)
  
  //____________________________________________________________________________
IlcPVECHit::IlcPVECHit(const IlcPVECHit & hit) :
  IlcHit(hit),fId(hit.fId),fELOS(hit.fELOS),fTime(hit.fTime)
{
  // copy ctor
} 

//____________________________________________________________________________
IlcPVECHit::IlcPVECHit(Int_t shunt, Int_t track, Int_t id, Float_t *hits) : 
  IlcHit(shunt, track),fId(0),fELOS(0),fTime(0)
{
  //
  // Create a CPV hit object
  //
  
  fX          = hits[0] ;
  fY          = hits[1] ;
  fZ          = hits[2] ;
  fTime       = hits[3] ;
  fId         = id ;
  fELOS       = hits[4] ;
}
//____________________________________________________________________________
Bool_t IlcPVECHit::operator==(IlcPVECHit const &rValue) const
{ 
  // Two hits are identical if they have the same Id and originate from the same primary 

  Bool_t rv = kFALSE ; 

  if ( (fId == rValue.GetId()) && ( fTrack == rValue.GetPrimary() ) && ( TMath::Abs(fTime - rValue.GetTime()) < 1.e-12 ) )
    rv = kTRUE;
  
  return rv;
}

//____________________________________________________________________________
IlcPVECHit & IlcPVECHit::operator = (const IlcPVECHit &)
{
  Fatal("operator =", "not implemented");
  return *this;
}
//____________________________________________________________________________
IlcPVECHit IlcPVECHit::operator+(const IlcPVECHit &rValue)
{
  // Add the energy of the hit
  
  fELOS += rValue.GetEnergy() ;

  if(rValue.GetTime() < fTime)
    fTime = rValue.GetTime() ;
    
   return *this;

}


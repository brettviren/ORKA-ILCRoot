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

/* $Id: IlcPVBARHit.cxx 53186 2011-11-25 14:28:01Z kharlov $ */

//_________________________________________________________________________
//  Hits class for PVBAR     
//  A hit in PVBAR is the sum of all hits in a single crystal
//*--
//*-- Author: Maxime Volkov (RRC KI) & Yves Schutz (SUBATECH) & Dmitri Peressounko (RRC KI & SUBATECH)

// --- ROOT system ---

// --- Standard library ---

// --- IlcRoot header files ---
#include "IlcPVBARHit.h"

ClassImp(IlcPVBARHit)
  
  //____________________________________________________________________________
IlcPVBARHit::IlcPVBARHit(const IlcPVBARHit & hit) :
  IlcHit(hit),fId(hit.fId),fELOS(hit.fELOS),fTime(hit.fTime), fTPE(hit.fTPE), fTotalNpe(0)
{
  // copy ctor

  fTotalNpe = new Float_t[fTPE]; 
  for(Int_t i=0; i<fTPE; i++)
    fTotalNpe[i] = hit.fTotalNpe[i];
} 

//____________________________________________________________________________
IlcPVBARHit::IlcPVBARHit(Int_t shunt, Int_t track, Int_t id, Float_t *hits) : 
  IlcHit(shunt, track),fId(0),fELOS(0),fTime(0), fTPE(5), fTotalNpe(0)
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

  fTotalNpe = new Float_t[fTPE];
  for(Int_t i=0; i<fTPE; i++)
    fTotalNpe[i] = 0.;

}

//____________________________________________________________________________
IlcPVBARHit::IlcPVBARHit(Int_t shunt, Int_t track, Int_t id, Float_t *hits, Float_t *TotalNpe) : 
  IlcHit(shunt, track),fId(0),fELOS(0),fTime(0), fTPE(5), fTotalNpe(0)
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

  fTotalNpe = new Float_t[fTPE];
  for(Int_t i=0; i<fTPE; i++)
    fTotalNpe[i] = TotalNpe[i];

}

//____________________________________________________________________________
Bool_t IlcPVBARHit::operator==(IlcPVBARHit const &rValue) const
{ 
  // Two hits are identical if they have the same Id and originate from the same primary 

  Bool_t rv = kFALSE ; 

  if ( (fId == rValue.GetId()) && ( fTrack == rValue.GetPrimary() ) )
    rv = kTRUE;
  
  return rv;
}

//____________________________________________________________________________
IlcPVBARHit & IlcPVBARHit::operator = (const IlcPVBARHit &)
{
  Fatal("operator =", "not implemented");
  return *this;
}
//____________________________________________________________________________
IlcPVBARHit IlcPVBARHit::operator+(const IlcPVBARHit &rValue)
{
  // Add the energy of the hit
  
  fELOS += rValue.GetEnergy() ;

  if(rValue.GetTime() < fTime)
    fTime = rValue.GetTime() ;
    
  for(Int_t i=0; i<fTPE; i++)
    fTotalNpe[i] = rValue.fTotalNpe[i];

  return *this;

}


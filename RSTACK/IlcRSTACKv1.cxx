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
// Implementation version v1 of RSTACK Manager class 
//---
//---
// Produces hits for RSTACK, hits can be cumulated
//---
//---


// --- ROOT system ---
#include <TClonesArray.h>
#include <TParticle.h>
#include <TVirtualMC.h>

// --- Standard library ---


// --- IlcRoot header files ---
#include "IlcRSTACKGeometry.h"
#include "IlcRSTACKHit.h"
#include "IlcRSTACKv1.h"
#include "IlcRun.h"
#include "IlcMC.h"
#include "IlcStack.h"
#include "IlcRSTACKSimParam.h"

ClassImp(IlcRSTACKv1)

//____________________________________________________________________________
IlcRSTACKv1::IlcRSTACKv1()
{
  //Def ctor.
}

//____________________________________________________________________________
IlcRSTACKv1::IlcRSTACKv1(const char *name, const char *title):
  IlcRSTACKv0(name,title)
{
  //
  // We store hits :
  //   - fHits (the "normal" one), which retains the hits associated with
  //     the current primary particle being tracked
  //     (this array is reset after each primary has been tracked).
  //



  // We do not want to save in TreeH the raw hits
  // But save the cumulated hits instead
  
  fHits= new TClonesArray("IlcRSTACKHit",1000) ;
  gIlc->GetMCApp()->AddHitList(fHits) ; 

  fNhits = 0 ;

  fIshunt     =  2 ; // All hits are associated with primary particles
}

//____________________________________________________________________________
IlcRSTACKv1::~IlcRSTACKv1()
{
  // dtor
 if ( fHits) {
    fHits->Delete() ; 
    delete fHits ;
    fHits = 0 ; 
 }
}

//____________________________________________________________________________
void IlcRSTACKv1::AddHit(Int_t shunt, Int_t primary, Int_t Id, Float_t * hits)
{
  // Add a hit to the hit list.
  // A RSTACK hit is the sum of all hits in a single scintillator tile from one primary and within a time gate of 1 ps

  Int_t hitCounter ;
  IlcRSTACKHit *newHit ;
  IlcRSTACKHit *curHit ;
  Bool_t deja = kFALSE ;

  newHit = new IlcRSTACKHit(shunt, primary, Id, hits) ;

  //comment out the loop if you like to store hits individually
  for ( hitCounter = fNhits-1 ; hitCounter >= 0 && !deja ; hitCounter-- ) {
    curHit = static_cast<IlcRSTACKHit*>((*fHits)[hitCounter]) ;
    if(curHit->GetPrimary() != primary) break ; 
           // We add hits with the same primary, while GEANT treats primaries succesively 
    if( *curHit == *newHit ) {
      *curHit + *newHit ;
      deja = kTRUE ;
    }
  }
         
  if ( !deja ) {
    new((*fHits)[fNhits]) IlcRSTACKHit(*newHit) ;

    fNhits++ ;
  }
  
  delete newHit;
}

//____________________________________________________________________________
void IlcRSTACKv1::FinishPrimary() 
{
  // called at the end of each track (primary) by IlcRun
  // hits are reset for each new track
  // accumulate the total hit-multiplicity

}

//____________________________________________________________________________
void IlcRSTACKv1::FinishEvent() 
{
  // called at the end of each event by IlcRun
  // accumulate the hit-multiplicity and total energy per block 
  // if the values have been updated check it
  
  IlcDetector::FinishEvent(); 
}
//____________________________________________________________________________
void IlcRSTACKv1::StepManager(void)
{

//Geometry parameters

  IlcRSTACKGeometry * geom = GetGeometry() ; 

  Double_t RSTACKRmin = geom->GetRSTACKRmin();
  Double_t RSTACKRmax = geom->GetRSTACKRmax();
  Int_t RSTACKLength = geom->GetRSTACKLength();

  Int_t          relid[4]={0,0,0,0} ;   // (sector, tile, not used, not used) indices
  Int_t          absid ;                // absolute cell ID number
  Float_t        xyztepl[7]={-1000.,-1000.,-1000.,0.,0.,0.,0.}  ; // position wrt MRS, time, energy deposited, number of photons and distance from the lower z edge
  TLorentzVector pos      ;             // Lorentz vector of the track current position
  Int_t          copy     ;

  
  static Int_t idRSSCI = gMC->VolId("RSTACK_BSCITILES");
  if(gMC->CurrentVolID(copy) == idRSSCI ) { //  We are inside a Scint tile
//     cout << gMC->CurrentVolPath() << endl;

    if(!gMC->TrackCharge()) return;  //Only charged particles make sintillation

    //Put in the TreeK particle entering RSTACK and all its parents
    if ( gMC->IsTrackEntering() || gMC->IsTrackExiting() ){
      
      
      gMC->TrackPosition(pos) ;

      //Flags particles entering or exiting from the inner/outer surface to be stored in TreeK
      if ( (pos.Perp() > RSTACKRmax-0.1) || (pos.Perp() < RSTACKRmin+0.1) ){
	//Particle close to forward or front surface (0.1 to get rid of numerical errors)
	//It could be useful to add also particles entering or exiting from from left/right surfaces
	Int_t parent = gIlc->GetMCApp()->GetCurrentTrackNumber() ;
	TParticle * part = gIlc->GetMCApp()->Particle(parent) ;
	if( (part->R() > RSTACKRmax-0.1) || (part->R() < RSTACKRmin+0.1) ){
	  part->SetBit(kKeepBit);
	  while ( parent != -1 ) {
	    part = gIlc->GetMCApp()->Particle(parent) ;
	    part->SetBit(kKeepBit);
	    parent = part->GetFirstMother() ;
	  }
	}
      }
    }


    gMC->TrackPosition(pos) ;
    xyztepl[0] = pos[0] ;
    xyztepl[1] = pos[1] ;
    xyztepl[2] = pos[2] ;

    Float_t lostenergy = gMC->Edep();
    xyztepl[4] = lostenergy ;

    if ( lostenergy != 0 ) {  // Track is inside the scint tile and deposits some energy
      xyztepl[3] = gMC->TrackTime() ;
      Int_t sector ;
      gMC->CurrentVolOffID(1,sector);
      relid[0] = sector;
      Int_t tile ;
      gMC->CurrentVolID(tile);
      relid[1] = tile;
      // Absid for scintillator tile
      geom->RelToAbsNumbering(relid, absid);
//       IlcInfo(Form("Vol: %s  sector: %d tile: %d absid: %d ", gMC->CurrentVolPath(), sector, tile, absid));


      //Birk's correction applied to EnergyLoss
      Float_t KBirks = IlcRSTACKSimParam::GetInstance()->GetBirks();
      Float_t step = gMC->TrackStep();
      Float_t dE_dx = 0.;
      Float_t ELossCorrected = 0.;
      if(step>0.){
	dE_dx = lostenergy/step;
	ELossCorrected = lostenergy/(1.+KBirks*dE_dx);
      }
      else {
	ELossCorrected = 0.;
      }

      //Calculates the light yield, the number of photons produced in the scintillator
      Float_t lightYield = IlcRSTACKSimParam::GetInstance()->GetLightYieldMean() * ELossCorrected ;
      xyztepl[5] = lightYield ;

      Float_t global[3], local[3];
      for(Int_t i=0; i<3; i++)
	global[i] = pos[i];
      
      gMC->Gmtod(global, local, 1); // transform coordinate from master to daughter system
      xyztepl[6]=RSTACKLength/2.+local[2]; // Distance of the hit production from the lower z edge of the scintillator tail


      //Pick-up the primary to associate to the hits
      Int_t primary ;
      if(fIshunt == 2){
        primary = gIlc->GetMCApp()->GetCurrentTrackNumber() ;
        TParticle * part = gIlc->GetMCApp()->Particle(primary) ;
        while ( !part->TestBit(kKeepBit) ) {
          primary = part->GetFirstMother() ;
          if(primary == -1){
            primary  =  gIlc->GetMCApp()->GetPrimary( gIlc->GetMCApp()->GetCurrentTrackNumber() );
            break ;
          }
          part = gIlc->GetMCApp()->Particle(primary) ;
        }
      }
      else{
        primary  =  gIlc->GetMCApp()->GetPrimary( gIlc->GetMCApp()->GetCurrentTrackNumber() );
      }
      
      // add current hit to the hit list
      AddHit(fIshunt, primary, absid, xyztepl);
        
    } // there is deposited energy
  } // we are inside a RSTACK Scint tile

}

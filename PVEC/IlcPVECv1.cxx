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

/* $Id: IlcPVECv1.cxx 47329 2011-02-11 19:44:15Z kharlov $ */


//_________________________________________________________________________
// Implementation version v1 of PVEC Manager class 
//---
//---
//---
//---
//*-- Author: Yves Schutz (SUBATECH)


// --- ROOT system ---
#include <TClonesArray.h>
#include <TParticle.h>
#include <TVirtualMC.h>

// --- Standard library ---


// --- IlcRoot header files ---
#include "IlcPVECGeometry.h"
#include "IlcPVECHit.h"
#include "IlcPVECv1.h"
#include "IlcRun.h"
#include "IlcMC.h"
#include "IlcStack.h"
#include "IlcPVECSimParam.h"

ClassImp(IlcPVECv1)

//____________________________________________________________________________
IlcPVECv1::IlcPVECv1()
{
  //Def ctor.
}

//____________________________________________________________________________
IlcPVECv1::IlcPVECv1(const char *name, const char *title):
  IlcPVECv0(name,title)
{
  //
  // We store hits :
  //   - fHits (the "normal" one), which retains the hits associated with
  //     the current primary particle being tracked
  //     (this array is reset after each primary has been tracked).
  //



  // We do not want to save in TreeH the raw hits
  // But save the cumulated hits instead (need to create the branch myself)
  // It is put in the Digit Tree because the TreeH is filled after each primary
  // and the TreeD at the end of the event (branch is set in FinishEvent() ). 
  
  fHits= new TClonesArray("IlcPVECHit",1000) ;
  gIlc->GetMCApp()->AddHitList(fHits) ; 

  fNhits = 0 ;

  fIshunt     =  2 ; // All hits are associated with primary particles
}

//____________________________________________________________________________
IlcPVECv1::~IlcPVECv1()
{
  // dtor
 if ( fHits) {
    fHits->Delete() ; 
    delete fHits ;
    fHits = 0 ; 
 }
}

//____________________________________________________________________________
void IlcPVECv1::AddHit(Int_t shunt, Int_t primary, Int_t Id, Float_t * hits)
{
  // Add a hit to the hit list.
  // A PVEC hit is the sum of all hits in a single crystal from one primary and within some time gate

  Int_t hitCounter ;
  IlcPVECHit *newHit ;
  IlcPVECHit *curHit ;
  Bool_t deja = kFALSE ;
  IlcPVECGeometry * geom = GetGeometry() ; 

  newHit = new IlcPVECHit(shunt, primary, Id, hits) ;

  //comment out the loop if you like to store hits individually
  for ( hitCounter = fNhits-1 ; hitCounter >= 0 && !deja ; hitCounter-- ) {
    curHit = static_cast<IlcPVECHit*>((*fHits)[hitCounter]) ;
    if(curHit->GetPrimary() != primary) break ; 
           // We add hits with the same primary, while GEANT treats primaries succesively 
    if( *curHit == *newHit ) {
      *curHit + *newHit ;
      deja = kTRUE ;
    }
  }
         
  if ( !deja ) {
    new((*fHits)[fNhits]) IlcPVECHit(*newHit) ;
    // get the block Id number
    Int_t relid[4] ;
    geom->AbsToRelNumbering(Id, relid) ;

    fNhits++ ;
  }
  
  delete newHit;
}

//____________________________________________________________________________
void IlcPVECv1::FinishPrimary() 
{
  // called at the end of each track (primary) by IlcRun
  // hits are reset for each new track
  // accumulate the total hit-multiplicity

}

//____________________________________________________________________________
void IlcPVECv1::FinishEvent() 
{
  // called at the end of each event by IlcRun
  // accumulate the hit-multiplicity and total energy per block 
  // if the values have been updated check it
  
  IlcDetector::FinishEvent(); 
}
//____________________________________________________________________________
void IlcPVECv1::StepManager(void)
{

//Geometry parameters

  IlcPVECGeometry * geom = GetGeometry() ; 

  Double_t PVECRmin = geom->GetPVECRmin();
  Double_t PVECRmax = geom->GetPVECRmax();


  Int_t          relid[4] ={0};           // (#endcap, layer, Xtal, not used) indices
  Int_t          absid    ;           // absolute cell ID number
  Float_t        xyzte[5]={-1000.,-1000.,-1000.,0.,0.}  ; // position wrt MRS, time and energy deposited
  TLorentzVector pos      ;           // Lorentz vector of the track current position
  Int_t          copy     ;

// // // //   Int_t moduleNumber ;
  
//   cout << gMC->CurrentVolPath() << endl;
  static Int_t idPVECXTL = gMC->VolId("PVEC_CsIXtal");
  if(gMC->CurrentVolID(copy) == idPVECXTL ) { //  We are inside a PBWO crystal
//     cout << gMC->CurrentVolPath() << endl;

    if(!gMC->TrackCharge()) return;  //Only charged particles make sintillation

    //Put in the TreeK particle entering PVEC and all its parents
    if ( gMC->IsTrackEntering() || gMC->IsTrackExiting() ){
      
      
      gMC->TrackPosition(pos) ;
      
      //Flags particles entering or exiting from the inner/outer surface to be stored in TreeK
      if ( (pos.Perp() > PVECRmax-0.1) || (pos.Perp() < PVECRmin+0.1) ){
	//Particle close to forward or front surface (0.1 to get rid of numerical errors) 
	//It could be useful to add also particles entering or exiting from from left/right surfaces
//         IlcInfo(Form("LeadGlass: IsEntering %d IsExiting %d TrakNum: %d Pid: %d R: %f", gMC->IsTrackEntering(), gMC->IsTrackExiting(), gIlc->GetMCApp()->GetCurrentTrackNumber(), gMC->TrackPid(), pos.Pt()));
	Int_t parent = gIlc->GetMCApp()->GetCurrentTrackNumber() ;
	TParticle * part = gIlc->GetMCApp()->Particle(parent) ;
	if( (part->R() > PVECRmax-0.1) || (part->R() < PVECRmin+0.1) ){
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
    xyzte[0] = pos[0] ;
    xyzte[1] = pos[1] ;
    xyzte[2] = pos[2] ;

    Float_t lostenergy = gMC->Edep();

    if ( lostenergy != 0 ) {  // Track is inside the crystal and deposits some energy
      xyzte[3] = gMC->TrackTime() ;
      Int_t module ;
      gMC->CurrentVolOffID(2,module);
      relid[0] = module;
      Int_t iLay ;
      gMC->CurrentVolOffID(1,iLay);
      relid[1] = iLay;
      Int_t iXtal ;
      gMC->CurrentVolID(iXtal);
      relid[2] = iXtal;
      // Absid for Xtal
      geom->RelToAbsNumbering(relid,absid);

//       //Calculates the light yield, the number of photons produced in the crystal 
//       Float_t lightYield = gRandom->Poisson(IlcPVECSimParam::GetInstance()->GetLightFactor() * lostenergy) ;
// 
//       //Calculates de energy deposited in the crystal  
//       xyzte[4] = IlcPVECSimParam::GetInstance()->GetAPDFactor() * lightYield ;

            
      //*************************************************************
      //*************************************************************
      //****
      //**** you would like to add Birks effect
      //**** and attenuation length effect
      //****
      //*************************************************************
      //*************************************************************


      Float_t ScintLightFactor = 54.*1.e6; // 54 photons/keV  //FIXME
      Float_t lightYield = gRandom->Poisson(ScintLightFactor * lostenergy) ;  //FIXME
      xyzte[4] = lightYield ;


      //Pick-up the primary to associate to the hits
      Int_t primary ;
      if(fIshunt == 2){
        primary = gIlc->GetMCApp()->GetCurrentTrackNumber() ;
        TParticle * part = gIlc->GetMCApp()->Particle(primary) ;
        while ( !part->TestBit(kKeepBit) ) {
          primary = part->GetFirstMother() ;
          if(primary == -1){
            primary  =  gIlc->GetMCApp()->GetPrimary( gIlc->GetMCApp()->GetCurrentTrackNumber() );
            break ; //there is a possibility that particle passed e.g. thermal isulator and hits a side
          //surface of the crystal. In this case it may have no primary at all. 
          //We can not easily separate this case from the case when this is part of the shower, 
          //developed in the neighboring crystal.
          }
          part = gIlc->GetMCApp()->Particle(primary) ;
        }
      }
      else{
        primary  =  gIlc->GetMCApp()->GetPrimary( gIlc->GetMCApp()->GetCurrentTrackNumber() );
      }
      
      // add current hit to the hit list
      // Info("StepManager","%d %d", primary, tracknumber) ; 
      AddHit(fIshunt, primary, absid, xyzte);
        
    } // there is deposited energy
  } // we are inside a PVEC Xtal
  


}

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

/* $Id: IlcPVBARv1.cxx 47329 2011-02-11 19:44:15Z kharlov $ */


//_________________________________________________________________________
// Implementation version v1 of PVBAR Manager class 
//---
//---
//*-- Author: Yves Schutz (SUBATECH)


// --- ROOT system ---
#include <TClonesArray.h>
#include <TParticle.h>
#include <TVirtualMC.h>
#include <TGeoManager.h>

// --- Standard library ---


// --- IlcRoot header files ---
#include "IlcPVBARCPVDigit.h"
#include "IlcPVBARGeometry.h"
#include "IlcPVBARHit.h"
#include "IlcPVBARv1.h"
#include "IlcRun.h"
#include "IlcMC.h"
#include "IlcStack.h"
#include "IlcPVBARSimParam.h"

ClassImp(IlcPVBARv1)

//____________________________________________________________________________
IlcPVBARv1::IlcPVBARv1()
{
  //Def ctor.
}

//____________________________________________________________________________
IlcPVBARv1::IlcPVBARv1(const char *name, const char *title):
  IlcPVBARv0(name,title)
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
  
  fHits= new TClonesArray("IlcPVBARHit",1000) ;
  gIlc->GetMCApp()->AddHitList(fHits) ; 

  fNhits = 0 ;

  fIshunt     =  2 ; // All hits are associated with primary particles
}

//____________________________________________________________________________
IlcPVBARv1::~IlcPVBARv1()
{
  // dtor
 if ( fHits) {
    fHits->Delete() ; 
    delete fHits ;
    fHits = 0 ; 
 }
}

//____________________________________________________________________________
void IlcPVBARv1::AddHit(Int_t shunt, Int_t primary, Int_t Id, Float_t * hits)
{
  // Add a hit to the hit list.
  // A PVBAR hit is the sum of all hits in a single crystal from one primary and within some time gate

  Int_t hitCounter ;
  IlcPVBARHit *newHit ;
  IlcPVBARHit *curHit ;
  Bool_t deja = kFALSE ;
  IlcPVBARGeometry * geom = GetGeometry() ; 

  newHit = new IlcPVBARHit(shunt, primary, Id, hits) ;

  for ( hitCounter = fNhits-1 ; hitCounter >= 0 && !deja ; hitCounter-- ) {
    curHit = static_cast<IlcPVBARHit*>((*fHits)[hitCounter]) ;
    if(curHit->GetPrimary() != primary) break ;
           // We add hits with the same primary, while GEANT treats primaries succesively
    if( *curHit == *newHit ) {
      *curHit + *newHit ;
      deja = kTRUE ;
    }
  }
         
  if ( !deja ) {
    new((*fHits)[fNhits]) IlcPVBARHit(*newHit) ;
    // get the block Id number
    Int_t relid[4] ;
    geom->AbsToRelNumbering(Id, relid) ;

    fNhits++ ;
  }
  
  delete newHit;
}

//____________________________________________________________________________
void IlcPVBARv1::FinishPrimary() 
{
  // called at the end of each track (primary) by IlcRun
  // hits are reset for each new track
  // accumulate the total hit-multiplicity

}

//____________________________________________________________________________
void IlcPVBARv1::FinishEvent() 
{
  // called at the end of each event by IlcRun
  // accumulate the hit-multiplicity and total energy per block 
  // if the values have been updated check it
  
  IlcDetector::FinishEvent(); 
}
//____________________________________________________________________________
void IlcPVBARv1::StepManager(void)
{
  
  //Geometry parameters
  IlcPVBARGeometry * geom = GetGeometry() ; 
  
  Double_t PVBARRmin = geom->GetPVBARRmin();
  Double_t PVBARRmax = geom->GetPVBARRmax();
  Double_t PVBARLength = geom->GetPVBARLength();
  Double_t PVBARTileSF57Thickness = geom->GetPVBARTileSF57Thickness();
  
  
  Int_t          relid[4] ;           // (sector, subsector, tile, type) indices
  Int_t          absid    ;           // absolute cell ID number
  Float_t        xyztepl[7]={-1000.,-1000.,-1000.,0.,0.,0.,0.}  ; // position wrt MRS, time and energy deposited, number of photons and distance from the lower z edge
  TLorentzVector pos      ;           // Lorentz vector of the track current position
  Int_t          copy     ;
  
  static Int_t idPVBLG = gMC->VolId("PVBAR_BCERTILE");
  static Int_t idPVBSCI = gMC->VolId("PVBAR_BSCITILE");
  
  if(gMC->CurrentVolID(copy) == idPVBLG ) { //  We are inside a leadglass tile
  
    //Put in the TreeK particle entering PVBAR and all its parents
    if ( gMC->IsTrackEntering() || gMC->IsTrackExiting() ){

      gMC->TrackPosition(pos) ;

      if ( (pos.Perp() > PVBARRmax-0.1) || (pos.Perp() < PVBARRmin+0.1) ){
	//Particle close to forward or front surface (0.1 to get rid of numerical errors) 
	//         IlcInfo(Form("LeadGlass: IsEntering %d IsExiting %d TrakNum: %d Pid: %d R: %f", gMC->IsTrackEntering(), gMC->IsTrackExiting(), gIlc->GetMCApp()->GetCurrentTrackNumber(), gMC->TrackPid(), pos.Pt()));
	Int_t parent = gIlc->GetMCApp()->GetCurrentTrackNumber() ;
	TParticle * part = gIlc->GetMCApp()->Particle(parent) ;
	if( (part->R() > PVBARRmax-0.1) || (part->R() < PVBARRmin+0.1) ){
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

    if ( lostenergy != 0 ) {  // Track is inside the crystal and deposits some energy
      xyztepl[3] = gMC->TrackTime() ;

      relid[3] = 0;
      Int_t tile ;
      gMC->CurrentVolID(tile);
      relid[2] = tile;
      Int_t subsect ;
      gMC->CurrentVolOffID(1,subsect);
      relid[1] = subsect;
      Int_t sect ;
      gMC->CurrentVolOffID(2,sect);
      relid[0] = sect;

      // Absid for leadglass tile
      //       absid = slice;
//       absid = tile+subsect*100+sect*10000;
//       absid = subsect+sect*100;
      geom->RelToAbsNumbering(relid, absid);
      
      //       absid = tile+(slice-1)*PVBARNTiles;
//       cout << gMC->CurrentVolPath() << " " << tile << " " << subsect << " " << sect << " " << absid << endl;
      
      
      TLorentzVector TrackMom;
      gMC->TrackMomentum(TrackMom); //Particle Momentum
      
      Float_t step = gMC->TrackStep();
      Float_t Charge = gMC->TrackCharge();
      
      Bool_t cStep = kFALSE; // charged particle step
      if(step > 0. &&  Charge != 0. && TrackMom.Rho() > 0. && lostenergy > 0. )
	cStep = kTRUE;
      
      Float_t beta = 0.;
      
      if(cStep){ // this can be a Cerenkov step
	TrackMom.SetRho(1.); //Set Particle Momentum as versor
	
	beta = TrackMom.Beta();  //Particle beta  
	
	TVector3 vCell(0.,0.,1.);  //Tile axis
	TVector3 vP = TrackMom.Vect();  //particle direction
	Double_t ThetaAngle = vP.Angle(vCell); //Angle between tile axis and particle momentum
	vP.SetTheta(ThetaAngle*TMath::DegToRad()); //convert vP from global to local coordinate
	Float_t uP[3];
	vP.GetXYZ(uP);  //Put vP components in uP array
	
	Float_t zDistanceFromWLS[2]={PVBARLength/2.-pos.Z(),PVBARLength/2.+pos.Z()};
	Float_t xyDistanceFromWLS=PVBARTileSF57Thickness; //FIXME
	
	const Int_t lambdabins = 64;  // Number of bins
	Float_t lambdawidth = 5.;     // Wavelength step
	Float_t lambdazero = 300.;    // Lowest limit of wavelength interval
// // // // 	Float_t Npe_Spectrum[lambdabins][2] ; // Contains the Npe generated the lead glass in fbins steps of the range
// // // // 	Float_t Npe_LG[lambdabins][2] ;       // Contains the Npe readout in the lead glass in fbins steps of the range
// // // // 	Float_t Npe_WLS[lambdabins][2] ;      // Contains the Npe readout in the WLS in fbins steps of the range
// // // // 	Float_t TotalNpe[5];                  // Sum of above Npe
// // // // 	
// // // // 	for(Int_t ii=0; ii<lambdabins; ii++){
// // // // 	  Npe_Spectrum[ii][0] = 0.;
// // // // 	  Npe_LG[ii][0] = 0.;
// // // // 	  Npe_WLS[ii][0] = 0.;
// // // // 	  Npe_Spectrum[ii][1] = 0.;
// // // // 	  Npe_LG[ii][1] = 0.;
// // // // 	  Npe_WLS[ii][1] = 0.;
// // // // 	}
// // // // 	
// // // // 	for(Int_t ii=0; ii<5; ii++)
// // // // 	  TotalNpe[ii] = 0.;
	
	//Calculates the light yield, the number of photons produced in the crystal 
	  //There is no dependence of reponce on distance from energy deposition to APD
	  //       Float_t lightYield = gRandom->Poisson(IlcPVBARSimParam::GetInstance()->GetLightFactor() * lostenergy) ;
	  
	  // Float_t lightYield = Npe_Cerenkov_in_Glass_Ex(beta, uP, step, zDistanceFromWLS, xyDistanceFromWLS, lambdabins, lambdazero, lambdawidth, Npe_Spectrum, Npe_LG, Npe_WLS, TotalNpe);

	  xyztepl[6]=zDistanceFromWLS[1]; // Distance of the hit production from the lower z edge of the scintillator tail

	  Float_t lightYield = Npe_Cerenkov(beta, uP, step, zDistanceFromWLS, xyDistanceFromWLS, lambdabins, lambdazero, lambdawidth);

	  xyztepl[5] = lightYield;

	  // 	if(lightYield>0.)
	  // 	  IlcInfo(Form("lightYield: %e beta: %e uP: %f %f %f step: %f zDistanceFromWLS: %f %f xyDistanceFromWLS: %f lambdabins: %d lambdazero: %f lambdawidth: %f TotalNpe %f %f %f %f %f",lightYield, beta, uP[0], uP[1], uP[2], step, zDistanceFromWLS[0], zDistanceFromWLS[1], xyDistanceFromWLS, lambdabins, lambdazero, lambdawidth, TotalNpe[0], TotalNpe[1], TotalNpe[2], TotalNpe[3], TotalNpe[4]));
	  //Calculates de energy deposited in the crystal  
	  
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
	  AddHit(fIshunt, primary, absid, xyztepl);
	  // AddHit(fIshunt, primary, absid, xyztepl, TotalNpe);
      }  //Cerenkov step
    } // there is deposited energy
  } // we are inside a leadglass tile
  
  
  if(gMC->CurrentVolID(copy) == idPVBSCI ) { //  We are inside a Scintillating tile
    
    if(!gMC->TrackCharge()) return;  //Only charged particles make sintillation

    
    //Put in the TreeK particle entering PVBAR and all its parents
    if ( gMC->IsTrackEntering() || gMC->IsTrackExiting() ){
      
      
      gMC->TrackPosition(pos) ;
      
      //Flags particles entering or exiting from the inner/outer surface to be stored in TreeK
      if ( (pos.Perp() > PVBARRmax-0.1) || (pos.Perp() < PVBARRmin+0.1) ){
	//Particle close to forward or front surface (0.1 to get rid of numerical errors) 
	//         IlcInfo(Form("Sci: IsEntering %d IsExiting %d TrakNum: %d Pid: %d R: %f", gMC->IsTrackEntering(), gMC->IsTrackExiting(), gIlc->GetMCApp()->GetCurrentTrackNumber(), gMC->TrackPid(), pos.Pt()));
	Int_t parent = gIlc->GetMCApp()->GetCurrentTrackNumber() ;
	TParticle * part = gIlc->GetMCApp()->Particle(parent) ;
	if( (part->R() > PVBARRmax-0.1) || (part->R() < PVBARRmin+0.1) ){
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

    if ( lostenergy != 0 ) {  // Track is inside the crystal and deposits some energy
      xyztepl[3] = gMC->TrackTime() ;

      relid[3] = 1;
      Int_t tile ;
      gMC->CurrentVolID(tile);
      relid[2] = tile;
      Int_t subsect ;
      gMC->CurrentVolOffID(1,subsect);
      relid[1] = subsect;
      Int_t sect ;
      gMC->CurrentVolOffID(2,sect);
      relid[0] = sect;
      
      // Absid for leadglass tile
      //       absid = slice;
//       absid = (tile+subsect*100+sect*10000)+1000000;
//       absid = (subsect+sect*100)+10000;
      geom->RelToAbsNumbering(relid, absid);
//       cout << gMC->CurrentVolPath() << " " << tile << " " << subsect << " " << sect << " " << absid << endl;

      //       absid = tile+(slice-1)*PVBARNTiles+PVBARNSlicePhi*PVBARNTiles;
      //       cout << gMC->CurrentVolPath() << " " << slice << " " << tile << " " << absid << endl;
      

      //Birk's correction applied to EnergyLoss
      Float_t KBirks = IlcPVBARSimParam::GetInstance()->GetBirks();
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
      Float_t lightYield = IlcPVBARSimParam::GetInstance()->GetLightYieldMean() * ELossCorrected ;
      xyztepl[5] = lightYield ;
	
	
      xyztepl[6]=PVBARLength/2.+pos.Z(); // Distance of the hit production from the lower z edge of the scintillator tail

	
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
      // Info("StepManager","%d %d", primary, tracknumber) ; 
      AddHit(fIshunt, primary, absid, xyztepl);
      // AddHit(fIshunt, primary, absid, xyztepl,TotalNpe);
    } // there is deposited energy
  } // We are inside a Scintillating tile
 
}

//____________________________________________________________________________
Float_t IlcPVBARv1::Npe_Cerenkov_in_Glass_Ex(Float_t beta, Float_t * uP, Float_t step, Float_t *zDistanceFromWLS,Float_t /*xyDistanceFromWLS*/, Int_t lambdabins,   Float_t lambdazero, Float_t lambdawidth, Float_t (*Npe_Spectrum)[2], Float_t (*Npe_LG)[2], Float_t (*Npe_WLS)[2], Float_t *TotalNpe)
{
// Extended version keeping into account the spectrum of the radiation and the dispersion equation of the medium
// Returns the total number of  photons in range of the spectrum in Npe_LG[32] fom the lead glass
// Returns the total number of  photons in range of the spectrum in Npe_WLS[32] from the WLS
	//inputs (distances in cm)
	//outputs:
// TotalNpe[0] = total pe generated
// TotalNpe[1] = total pe lead glass front
// TotalNpe[2] = total pe lead glass back
// TotalNpe[3] = total pe WLS front
// TotalNpe[4] = total pe WLS back

  Bool_t fIsDollanCalibrationRun = kFALSE;
  Float_t fDollanMCFactor_LG=1.0;
  Float_t fDollanMCFactor_WLS=1.0;

  Float_t Npe = 0.;
//   Float_t Npe2Fiber = 0.;
  Float_t NpeWLS = 0.;
  Float_t NpeLG = 0.;

  Float_t ug, vg, wg, Theta_Cerenkov, Phi_Cerenkov, sT, cT, sP, cP, uv;
  Float_t r_lambda;

  Float_t alpha = 1./137.;
  Float_t lambda1 ;
  Float_t lambda2 ;

  // Initialize the number of gammas
  for ( Int_t n = 0; n < 5; n++ )
    TotalNpe[n] = 0.;

// Initialize the number of gammas
  for ( Int_t nwl = 0; nwl < lambdabins; nwl++ ){
    Npe_Spectrum[nwl][0] =0.;
    Npe_LG[nwl][0] =0.;
    Npe_WLS[nwl][0] =0.;
    Npe_Spectrum[nwl][1] =0.;
    Npe_LG[nwl][1] =0.;
    Npe_WLS[nwl][1] =0.;
  }
// return (Float_t)((Int_t) (gRandom->Rndm(1)*1.02));
// Compute the number of gammas per wavelength range
  for ( Int_t nwl = 0; nwl < lambdabins; nwl++ )  // <-----  Start loop over the wavelength intervals
  {
    lambda1 = lambdazero + nwl*lambdawidth;
    lambda2 = lambda1 + lambdawidth;

// random wavelenght in the interval
    r_lambda = gRandom->Rndm();
    Float_t lambda_rndm = lambda1 * lambda2 / ( lambda2 - r_lambda * ( lambda2 - lambda1 ) );
       
// refraction index via dispersion relation
	Float_t en_Sellmeier = nIndex(lambda_rndm);
     //en_Sellmeier = 1.71;   //DEBUG
     //en_Sellmeier = 1.85;   //DEBUG

// Protect against particles below Cerenkov threshold
    if (beta*en_Sellmeier < 1.0) 
		continue;
    
// Use for restricting the range of generation of Cerenkov photons within a defined interval
    Bool_t bRestrictGenerationRange = kTRUE;
    if (bRestrictGenerationRange)
      if ((lambda_rndm <= lambdazero)|| (lambda_rndm >650.)) continue;

    Theta_Cerenkov = TMath::ACos ( 1./ ( beta*en_Sellmeier ) );

// Calculate the photons generated by the particle across the  range of the spectrum considered
    Float_t gammas_per_cm = 2. * TMath::Pi() * alpha * ( 1./lambda1-1./lambda2 ) *1.e7;  // (1.e7 nm/cm)
    Float_t f_Ngamma = gammas_per_cm * TMath::Power ( TMath::Sin ( Theta_Cerenkov ), 2. ) * step;
    /*    Float_t Ngamma = gRandom->Poisson ( f_Ngamma );*/
    Float_t Ngamma = f_Ngamma;
//     Float_t w_aper = 1./en_Sellmeier;
    if ( step < 0.000001 ) Ngamma = 0.;

    Phi_Cerenkov = 2. * TMath::Pi() * gRandom->Rndm();
    //geometry coverage of the WLS
    //    if(xyDistanceFromWLS*TMath::Tan(Phi_Cerenkov)>0.8) continue;  //FIXME
    //    if(xyDistanceFromWLS*TMath::Abs(TMath::Tan(Phi_Cerenkov))>0.14) Ngamma = 0.;  //FIXME
    //if(gRandom->Rndm()>46./845.) Ngamma = 0.;  //FIXME
	
	/********************************************************************************
	*pe-WLS capture calibration constants from July 2011 test beam			*
	*		46./845.	initial factor by Vito				*
	*		0.7		shower containment factor			*
	*		46.		pe/GeV measured at test beam			*
	*		228.5		results of this uncalibrated simulation		*
	*********************************************************************************/
	//if(CLHEP::RandFlat::shoot(0.,1.)>46./845.*46./228.5/0.7) Ngamma = 0.;  //FIXME

    Npe = Npe + Ngamma;
    TotalNpe[0] += Ngamma;
    Npe_Spectrum[nwl][0] = Npe_Spectrum[nwl][0] + Ngamma;
//     Npe_Spectrum[nwl][1] = Npe_Spectrum[nwl][1] + Ngamma;



// if (lambda_rndm > 350. && lambda_rndm < 550)
//        IlcDebug(2,Form("lambda_rndm: %f ;  en_Sellmeier: %f ;   gammas_per_cm: %f ;  step: %f ; Ngamma: %f ;", lambda_rndm, en_Sellmeier, gammas_per_cm, step, Ngamma));

// Next, it calculates the number of photons that reach the photodetector
    if ( Ngamma > 0. )
    {
     // (uP[3]) is the versor of the track in the local frame
      uv = TMath::Sqrt ( uP[0]*uP[0] + uP[1]*uP[1] );

     // (ug,vg,wg) is the versor of the photon in the local frame
      sT = TMath::Sin ( Theta_Cerenkov );
      cT = TMath::Cos ( Theta_Cerenkov );
      sP = TMath::Sin ( Phi_Cerenkov );
      cP = TMath::Cos ( Phi_Cerenkov );
      if ( uv < 0.001 )               // e- aligned with z axis in local frame
      {
        ug = sT * cP;
        vg = sT * sP;
        wg = cT;
      }
      else                                    // general case.
      {
        ug = uP[0] * cT + sT * ( uP[1]*sP + uP[0]*uP[2]*cP ) / uv;
        vg = uP[1] * cT + sT * ( -uP[0]*sP + uP[1]*uP[2]*cP ) / uv;
        wg = uP[2] * cT - sT * cP * uv;
      }
      Npe_Spectrum[0][1] = ug;
      Npe_Spectrum[1][1] = vg;
      Npe_Spectrum[2][1] = wg;
      Npe_Spectrum[3][1] = cT;
      Npe_Spectrum[4][1] = Theta_Cerenkov;


//       Final part consider the photon survival physics:
//       numerical aperture, attenuation, HEM, QE, ...
      //
/**********************************************************************************************************
      Lead Glass Light Collection Block
**********************************************************************************************************/
      Bool_t bDollanCalibration = fIsDollanCalibrationRun;  // Set to TRUE when performing the Dollan calibration
//       Bool_t bDollanCalibration = kTRUE;  // Set to TRUE when performing the Dollan calibration

// 1) Calculate probability of surviving for the pe along the leadglass (both sides). Takes into account the photon emission angle.
      Float_t z_metersLG[2] = {zDistanceFromWLS[0] / 100., zDistanceFromWLS[1] / 100.};
      if (abs(wg) > 0.0001)		// protect against divergencies
      {
	z_metersLG[0] = z_metersLG[0]/abs(wg);
	z_metersLG[1] = z_metersLG[1]/abs(wg);
      }
      else
      {
	z_metersLG[0] = 1000.;
	z_metersLG[1] = 1000.;
      }
      Float_t dBLG[2];
      Float_t Al_LG = AttenuationLength_m_LG( lambda_rndm );

      //Al_LG = 0.005;		// DEBUG
      dBLG[0]= z_metersLG[0]/Al_LG ;
      dBLG[1]= z_metersLG[1]/Al_LG ;
      Float_t probLG[2];
      probLG[0] = TMath::Exp ( - dBLG[0]  );
      probLG[1] = TMath::Exp ( - dBLG[1]  );
       
// 2) Correction factor from Dollan work
//       Float_t DollanFactor_LG = 18.2/65.; 
      Float_t dQE_LG=0.;
       
       if (bDollanCalibration)
       {
	 dQE_LG =QE_PMT ( lambda_rndm );
       }
       else
	 //       Float_t dQE_LG =QE_Fast_LG ( lambda_rndm );  //DEBUG
	 //       dQE_LG =QE_Ex_LG ( lambda_rndm );
       dQE_LG =QE_SiPM ( lambda_rndm );
       //        dQE_LG = 1.0;		// just for ORKA preliminary studies
      
       Npe_LG[nwl][0] = Npe_LG[nwl][0] + 0.5*Ngamma*probLG[0]*dQE_LG*fDollanMCFactor_LG;
       Npe_LG[nwl][1] = Npe_LG[nwl][1] + 0.5*Ngamma*probLG[1]*dQE_LG*fDollanMCFactor_LG;
       //     Npe_LG[nwl][0] = Npe_LG[nwl][0] + 0.5*Ngamma*probLG[0];  // Dollan calibration
       //     Npe_LG[nwl][1] *= Npe_LG[nwl][1] + 0.5*Ngamma*probLG[1];  // Dollan calibration
       TotalNpe[1] += 0.5*Ngamma*probLG[0]*dQE_LG*fDollanMCFactor_LG;
       TotalNpe[2] += 0.5*Ngamma*probLG[1]*dQE_LG*fDollanMCFactor_LG;
       NpeLG = NpeLG + Ngamma*probLG[0]*dQE_LG*fDollanMCFactor_LG;
      
       //IlcDebug(4,Form("lambda_rndm: %f ; zDistanceFromWLS[0]: %f ;   AttenuationLength_m_LG: %f ; probLG[0]: %f ; QE_Ex_LG ( ): %f ;", lambda_rndm, zDistanceFromWLS[0], AttenuationLength_m_LG( lambda_rndm), probLG[0], dQE_LG));
      //IlcDebug(4,Form("lambda_rndm: %f ; zDistanceFromWLS[1]: %f ;   AttenuationLength_m_LG: %f ; probLG[1]: %f ; QE_Ex_LG ( ): %f ;", lambda_rndm, zDistanceFromWLS[1], AttenuationLength_m_LG( lambda_rndm), probLG[1], dQE_LG));
          
/**********************************************************************************************************
      WLS Light Collection Block
      Fast calculation uses efficiencies by Ralph Dollan (Thesis 2004)
      Full calculation uses paramtrized absorption length and QE
**********************************************************************************************************/
      Bool_t bFullCalculation = kTRUE;
      
      // 1) Probability of surviving for the pe along the leadglass until it reaches a WLS fiber
//       Float_t Cos_Theta_Cerenkov_Recip = beta*en_Sellmeier ;
//       Float_t xy_metersLG = xyDistanceFromWLS *Cos_Theta_Cerenkov_Recip/ 100.;
      Float_t dBWLSxy[2], probWLSxy[2];
	  // ORKA has transversal WLS fibers/plates. Already calculated above
	  dBWLSxy[0] = dBLG[0];
	  dBWLSxy[1] = dBLG[1];

      probWLSxy[0] = TMath::Exp ( - dBWLSxy[0]  );
      probWLSxy[1] = TMath::Exp ( - dBWLSxy[1]  );
      //IlcDebug(4,Form("xy_metersLG: %f ;   dBWLSxy: %f ; probWLSxy: %f ;", xy_metersLG, dBWLSxy, probWLSxy));

      // 2) Probability of the photon entering the WLS: its angle must be larger than theta_max
      Float_t dProbPenetration = 0;
      Float_t en_OpticalGel = 1.45;
      Float_t en_Cladding = 1.49;
      Float_t en_Intermediate = TMath::Power(en_OpticalGel*en_Cladding, 0.5);
      Float_t en_Core = 1.59;
      Float_t sintheta2_max = (en_Sellmeier*en_Sellmeier-en_Intermediate*en_Intermediate)/en_Core/en_Core;
//       Float_t sintheta2_pe = 1. - wg*wg;
      //IlcDebug(4,Form("sintheta_max: %f ;   sintheta_pe: %f ;", TMath::Power(sintheta2_max, 0.5), TMath::Power(sintheta2_pe,0.5)));
      // for ORKA, assume all cerenkov photons enter the WLS plates
	  dProbPenetration = 1.0;

      Npe_Spectrum[5][1] = TMath::ASin(TMath::Sqrt(sintheta2_max));

      // 3) Probability of the photon being captured by the WLS: it decreases while the photon travels longitudinally along crystal and it depends on photon wavelength
      Float_t AbsorptionEff_WLSz[2];
      // this is really 100% for ORKA
      AbsorptionEff_WLSz[0] = 1.0 ;
      AbsorptionEff_WLSz[1] = 1.0 ;
      //IlcDebug(4,Form("lambda_rndm: %f ; AbsorptionEff_WLSz[0]: %f ;AbsorptionLength_WLS( lambda_rndm, 1. ): %f ;", lambda_rndm, AbsorptionEff_WLSz[0], AbsorptionLength_WLS( lambda_rndm, 1. )));


      // 4) Conversion efficiency the WLS
      Float_t dconversionWLS = 1. ; //Everythingh already in AbsorptionLength_WLS and QE
      // 4.1) Correction factor from Dollan work
//       Float_t DollanFactor_WLS = 1.299; //12 WLS fibers phi=1.5 mm per 4x4 cm^2 cell
//       Float_t DollanFactor_WLS = 0.7; //4 WLS fibers phi=1.4 mm per 4x4 cm^2 cell
//       Float_t DollanFactor_WLS = 0.5; //4 WLS fibers phi=1.0 mm per 4x4 cm^2 cell
//       Float_t WLSxCellFactor = 0.5; //4 WLS fibers phi=1.0 mm per 4x4 cm^2 cell
      Float_t WLSxCellFactor = 0.949; //10 WLS fibers phi=1.2 mm per 4x4 cm^2 cell

      // 5) Probability of surviving for the pe along the WLS because of self absortion
      Float_t z_metersWLS[2] = {zDistanceFromWLS[0] / 100. , zDistanceFromWLS[1] / 100. };  // ligth travel both sides of WLS
      if (bDollanCalibration) // fiber is 2 m long
      {
	z_metersWLS[0] +=2.;
	z_metersWLS[1] +=2.;
      }
      Float_t Al_WLS = AttenuationLength_m_WLS( lambda_rndm );
      Float_t dBWLSz[2] = {z_metersWLS[0]/Al_WLS, z_metersWLS[1]/Al_WLS };
      Float_t probWLSz[2] = {TMath::Exp ( - dBWLSz[0]  ) , TMath::Exp ( - dBWLSz[1]  ) };
      //IlcDebug(4,Form("z_metersWLS: %f ;   dBWLSz: %f %f ; probWLSz: %f ;", z_metersWLS, dBWLSz[0], dBWLSz[1], probWLSz));

      // 6) Shift the frequency in the WLS and compute the new bin
      Float_t r_lambda_shift = Shifter_WLS(lambda_rndm);
      Int_t nwl_shifted = (Int_t)( lambdabins*(r_lambda_shift-lambdazero)/((lambdabins+1)*lambdawidth) );
      //IlcDebug(4,Form("nwl_shifted: %i ; r_lambda_shift: %f ;", nwl_shifted, r_lambda_shift));

      // 7) Probability that the reemitted photon remains in the WLS fiber
	  // This is the solif angle of the fibers and it is about 4.8%
      Float_t acceptance_WLS = 0.048;

      // 8) The QE for the shifted photon
      Float_t dQE_WLS;
      if (bFullCalculation)
        {
	  //  if (bDollanCalibration)
	  //    dQE_WLS =QE_PMT ( r_lambda_shift );  // DEBUG
	  //  else
	  dQE_WLS =QE_SiPM ( r_lambda_shift );
        }  
      else
        dQE_WLS =QE_Fast_WLS ( r_lambda_shift );
        
      //IlcDebug(4,Form("QE_WLS: %f ;", dQE_WLS));

      if (nwl_shifted < lambdabins){
	Npe_WLS[nwl_shifted][0] = Npe_WLS[nwl_shifted][0] + 0.5*Ngamma*(probWLSxy[0]+probWLSxy[1])*dProbPenetration*AbsorptionEff_WLSz[0]*dconversionWLS*WLSxCellFactor*probWLSz[0]*dQE_WLS*fDollanMCFactor_WLS;

	Npe_WLS[nwl_shifted][1] = Npe_WLS[nwl_shifted][1] + 0.5*Ngamma*(probWLSxy[0]+probWLSxy[1])*dProbPenetration*AbsorptionEff_WLSz[1]*dconversionWLS*WLSxCellFactor*probWLSz[1]*dQE_WLS*fDollanMCFactor_WLS;
      }

      NpeWLS = NpeWLS + Ngamma*(2.0*acceptance_WLS)*(probWLSxy[0]+probWLSxy[1])*dProbPenetration*AbsorptionEff_WLSz[0]*dconversionWLS*WLSxCellFactor*probWLSz[0]*dQE_WLS*fDollanMCFactor_WLS;

      TotalNpe[3] += acceptance_WLS*Ngamma*(probWLSxy[0]+probWLSxy[1])*dProbPenetration*AbsorptionEff_WLSz[0]*dconversionWLS*WLSxCellFactor*probWLSz[0]*dQE_WLS*fDollanMCFactor_WLS;

      TotalNpe[4] += acceptance_WLS*Ngamma*(probWLSxy[0]+probWLSxy[1])*dProbPenetration*AbsorptionEff_WLSz[1]*dconversionWLS*WLSxCellFactor*probWLSz[1]*dQE_WLS*fDollanMCFactor_WLS;
//       Npe2Fiber =+ Npe * (probWLSxy[0]+probWLSxy[1]);

    }

  }  // End loop over  the wavelength intervals


  return Npe;
//   return NpeWLS;
//   return NpeLG;

}

//____________________________________________________________________________
Float_t IlcPVBARv1::Npe_Cerenkov(Float_t beta, Float_t * uP, Float_t step, Float_t *zDistanceFromWLS, Float_t /*xyDistanceFromWLS*/, Int_t lambdabins,   Float_t lambdazero, Float_t lambdawidth)
{
// Extended version keeping into account the spectrum of the radiation and the dispersion equation of the medium
// Returns the total number of  photons in range of the spectrum in Npe_LG[32] fom the lead glass
// Returns the total number of  photons in range of the spectrum in Npe_WLS[32] from the WLS
	//inputs (distances in cm)
	//outputs:
	// Nphotons = total Cerenkov photons generated

  Float_t Nphotons = 0.;

  Float_t /*ug, vg,*/ wg, Theta_Cerenkov, Phi_Cerenkov, sT, cT, /*sP,*/ cP, uv;
  Float_t r_lambda;

  Float_t alpha = 1./137.;
  Float_t lambda1 ;
  Float_t lambda2 ;

  // Compute the number of gammas per wavelength range
  for ( Int_t nwl = 0; nwl < lambdabins; nwl++ )  // <-----  Start loop over the wavelength intervals
  {
    lambda1 = lambdazero + nwl*lambdawidth;
    lambda2 = lambda1 + lambdawidth;

// random wavelenght in the interval
    r_lambda = gRandom->Rndm();
    Float_t lambda_rndm = lambda1 * lambda2 / ( lambda2 - r_lambda * ( lambda2 - lambda1 ) );
       
// refraction index via dispersion relation
	Float_t en_Sellmeier = nIndex(lambda_rndm);
     //en_Sellmeier = 1.71;   //DEBUG
     //en_Sellmeier = 1.85;   //DEBUG

// Protect against particles below Cerenkov threshold
    if (beta*en_Sellmeier < 1.0) 
		continue;
    
// Use for restricting the range of generation of Cerenkov photons within a defined interval
    Bool_t bRestrictGenerationRange = kTRUE;
    if (bRestrictGenerationRange)
      if ((lambda_rndm <= lambdazero)|| (lambda_rndm >650.)) continue;

    Theta_Cerenkov = TMath::ACos ( 1./ ( beta*en_Sellmeier ) );

// Calculate the photons generated by the particle across the  range of the spectrum considered
    Float_t gammas_per_cm = 2. * TMath::Pi() * alpha * ( 1./lambda1-1./lambda2 ) *1.e7;  // (1.e7 nm/cm)
    Float_t f_Ngamma = gammas_per_cm * TMath::Power ( TMath::Sin ( Theta_Cerenkov ), 2. ) * step;
    //    Float_t Ngamma = gRandom->Poisson ( f_Ngamma );
    Float_t Ngamma = f_Ngamma;
//     Float_t w_aper = 1./en_Sellmeier;
    if ( step < 0.000001 ) Ngamma = 0.;

    Phi_Cerenkov = 2. * TMath::Pi() * gRandom->Rndm();

    Nphotons += Ngamma;

// Next, it calculates the number of photons that reach the photodetector
    if ( Ngamma > 0. )
    {
     // (uP[3]) is the versor of the track in the local frame
      uv = TMath::Sqrt ( uP[0]*uP[0] + uP[1]*uP[1] );

     // (ug,vg,wg) is the versor of the photon in the local frame
      sT = TMath::Sin ( Theta_Cerenkov );
      cT = TMath::Cos ( Theta_Cerenkov );
//       sP = TMath::Sin ( Phi_Cerenkov );
      cP = TMath::Cos ( Phi_Cerenkov );
      if ( uv < 0.001 )               // e- aligned with z axis in local frame
      {
//         ug = sT * cP;
//         vg = sT * sP;
        wg = cT;
      }
      else                                    // general case.
      {
//         ug = uP[0] * cT + sT * ( uP[1]*sP + uP[0]*uP[2]*cP ) / uv;
//         vg = uP[1] * cT + sT * ( -uP[0]*sP + uP[1]*uP[2]*cP ) / uv;
        wg = uP[2] * cT - sT * cP * uv;
      }

//       Final part consider the photon survival physics:
//       numerical aperture, attenuation, HEM, QE, ...
//
/**********************************************************************************************************
      Lead Glass Light Collection Block
**********************************************************************************************************/
// // // //       Bool_t bDollanCalibration = fIsDollanCalibrationRun;  // Set to TRUE when performing the Dollan calibration
//       Bool_t bDollanCalibration = kTRUE;  // Set to TRUE when performing the Dollan calibration

// 1) Calculate probability of surviving for the pe along the leadglass (both sides). Takes into account the photon emission angle.
      Float_t z_metersLG[2] = {zDistanceFromWLS[0] / 100., zDistanceFromWLS[1] / 100.};
      if (abs(wg) > 0.0001)		// protect against divergencies
      {
	z_metersLG[0] = z_metersLG[0]/abs(wg);
	z_metersLG[1] = z_metersLG[1]/abs(wg);
      }
      else
      {
	z_metersLG[0] = 1000.;
	z_metersLG[1] = 1000.;
      }

    }

  }  // End loop over  the wavelength intervals


  return Nphotons;

}

//____________________________________________________________________________
Float_t IlcPVBARv1::nIndex(Float_t lambda){
//  Sellmeier constants for SF57HHT
  Float_t B1 = 1.81651371;
  Float_t B2 = 0.428893641;
  Float_t B3 = 1.07186278;
  Float_t C1 = 0.0143704198;
  Float_t C2 = 0.0592801172;
  Float_t C3 = 121.419942;

  Float_t lambda_um;
  if (lambda >= 260.)
    lambda_um =lambda/1000.; // in microns for Sellmeier dispersion relation
  else
    lambda_um =0.26;
 
// Sellmeier dispersion relation
  Float_t en_Sellmeier =   1 + ( B1 * lambda_um * lambda_um/ ( lambda_um * lambda_um - C1 ) ) + ( B2 * lambda_um * lambda_um/ ( lambda_um * lambda_um - C2 ) ) + ( B3 * lambda_um * lambda_um/ ( lambda_um * lambda_um - C3 ) ) ;
  en_Sellmeier = (en_Sellmeier>0.) ? en_Sellmeier : 0.; //FIXME
  en_Sellmeier = TMath::Power ( en_Sellmeier, 0.5 );

  return en_Sellmeier;
}



//____________________________________________________________________________
Float_t IlcPVBARv1::AttenuationLength_m_LG(Float_t lambda){
  // This function returns the attenuation length in meters
  // as a function of wavelength for SF57HHT lead glass.
  
  Float_t wl[30] ={2500.,2325., 1970.,1530.,1060.,700.,660.,620.,580.,546.,500.,460.,436.,420.,405.,400.,390.,380.,370.,365.,350.,334.,320.,310.,300.,290.,280.,270.,260.,250.};
  Float_t al[30] ={0.255,0.319,1.146,9.565,57.536,28.754,19.159,19.159,19.159,19.159,5.728,2.588,1.486,0.793,0.382,0.290,0.125,0.048,0.016,0.0089,0.0024,1.E-6,1.E-6,1.E-6,1.E-6,1.E-6,1.E-6,1.E-6,1.E-6,1.E-6};
  
  Float_t Alength = 1.E-6;
  for ( Int_t n = 0; n <29; n++ )
  {
    if ( lambda <= wl[n])
    {
      Alength = al[n]+(al[n+1]-al[n])*(lambda-wl[n])/(wl[n+1]-wl[n]);
    }
  }
  
  return Alength;

}

//____________________________________________________________________________
Float_t IlcPVBARv1::AttenuationLength_m_WLS(Float_t /*lambda*/){
  // This function returns the attenuation length in meters
  // as a function of wavelength for xxx WLS.
  
//   Float_t wl[30] ={2500.,2325., 1970.,1530.,1060.,700.,660.,620.,580.,546.,500.,460.,436.,420.,405.,400.,390.,380.,370.,365.,350.,334.,320.,310.,300.,290.,280.,270.,260.,250.};
//   Float_t al[30] ={0.255,0.319,1.146,9.565,57.536,28.754,19.159,19.159,19.159,19.159,5.728,2.588,1.486,0.793,0.382,0.290,0.125,0.048,0.016,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0};
  
  Float_t Alength = 12.0;

  
  return Alength;

}

//____________________________________________________________________________
Float_t IlcPVBARv1::QE_Fast_WLS(Float_t /*lambda*/){

// returns the QE for the WLS
// From Ralf Dollan 2004 thesis and
// www-zeuthen.desy.de/lcdet/Feb_05_WS/talks/rd_lcdet_sim.pdf
// Bicron BCF 91A

  return 2.0*12789./75588.*8.3/7.64;

}

//____________________________________________________________________________
Float_t IlcPVBARv1::QE_PMT(Float_t lambda){
/* QE for PMT Photonis XP1911
  lambda in nanometer*/
     
  Float_t wl[38] = {260., 265., 268., 271., 272., 274., 276., 278., 280., 282., 299., 312., 324., 337., 354., 377.0, 408.0, 420.0, 442.0, 469., 486., 506., 523.0, 535., 558., 583., 587., 590., 594., 598., 603., 608., 615., 624., 635., 636., 639., 641.};
  Float_t stvy[38] = {1., 2., 3., 4., 5., 6., 7., 8., 9., 10.,  20., 30., 40., 50., 60., 70., 80., 85.0, 80., 70., 60., 50., 40., 30., 20., 10., 9., 8., 7., 6., 5., 4., 3., 2., 1., 0.9, 0.8, 0.7};
  Float_t Sensitivity =0.;
  
  Float_t qe = 0.0;
      
      if (lambda >=  260. && lambda <= 641.) {      
      
    for ( Int_t n = 0; n <37; n++ )
    {
      if ( lambda >= wl[n])
      {
	Sensitivity = stvy[n]+(stvy[n+1]-stvy[n])*(lambda-wl[n])/(wl[n+1]-wl[n]);
      }
    }

    qe =Sensitivity*1.24/lambda; 
      }
      return qe;

}

//____________________________________________________________________________
Float_t IlcPVBARv1::QE_SiPM(Float_t lambda){
/* QE for PMT Photonis XP1911
  lambda in nanometer*/
     
  Float_t wl[13] = {360., 380., 400., 420., 440., 460., 480., 500., 520., 540., 560., 580., 600.};
  Float_t stvy[13] = {0.09, 0.14, 0.175, 0.18, 0.215, 0.21, 0.22, 0.22, 0.25, 0.23, 0.225, 0.20, 0.185};
  
  Float_t qe = 0.0;
      
  if (lambda >=  360. && lambda <= 600.) {
      
    for ( Int_t n = 0; n <13; n++ )
    {
      if ( lambda >= wl[n])
        qe = stvy[n]+(stvy[n+1]-stvy[n])*(lambda-wl[n])/(wl[n+1]-wl[n]);
    }
  }
  return qe;

}

//____________________________________________________________________________
Float_t IlcPVBARv1::Shifter_WLS(Float_t /*lambda*/){
  // This function returns the shifted wavelength of a WLS.
// c   Picks a wavelength for the blue light emitted by the scintillator 
// c  .....this spectrum was measured at Minn
// c      (data are integral probability from 460.91 to 596 nm in 5 nnm steps) 

// From Ralf Dollan 2004 thesis and
// www-zeuthen.desy.de/lcdet/Feb_05_WS/talks/rd_lcdet_sim.pdf
// Bicron BCF 91A
//   const Float_t eV= 1. ;

  const Int_t NUMENTRIES2 = 24;
//   Float_t PhotonEnergy_WLS_EM_Bcf91a_core_energy[NUMENTRIES2] =
//   { 2.69*eV, 2.67*eV, 2.66*eV, 2.64*eV, 2.63*eV,
//   2.61*eV, 2.58*eV, 2.56*eV, 2.55*eV, 2.53*eV,
//   2.50*eV, 2.48*eV, 2.46*eV, 2.45*eV, 2.44*eV,
//   2.43*eV, 2.41*eV, 2.37*eV, 2.33*eV, 2.25*eV,
//   2.24*eV, 2.19*eV, 2.15*eV, 2.08*eV
//   };

  Float_t PhotonWL_WLS_EM_Bcf91a_core[NUMENTRIES2] =
  { 460.91,464.36,466.11, 469.64, 471.42,
  475.04,	480.56,	484.31,	486.21,	490.06,
  495.94,	499.94,	504.,	506.06,	508.13,
  510.22,	514.46,	523.14,	532.12,	551.04,
  553.5,	566.14,	576.67,	596.08

  };

  Float_t WLS_EMISSION_Bcf91a_core[NUMENTRIES2] =
  {0., 0.02, 0.09, 0.20, 0.29,
  0.40, 0.59, 0.70, 0.80, 0.89,
  1.00, 0.96, 0.88, 0.79, 0.69,
  0.59, 0.50, 0.40, 0.31, 0.22,
  0.19, 0.10, 0.06, 0.
  };

retry:
    Float_t WLVal = 0.;
Int_t EmissIdx = ( Int_t ) ( gRandom->Rndm() *(NUMENTRIES2-1) );
// Int_t EmissIdx = ( Int_t ) ( CLHEP::RandFlat::shoot(0.,1.) *(NUMENTRIES2-1) );
Float_t EmissVal = gRandom->Rndm();
// Float_t EmissVal = CLHEP::RandFlat::shoot(0.,1.);


if ( EmissVal < WLS_EMISSION_Bcf91a_core[EmissIdx] )  // success
{

  if ( EmissIdx>0 && EmissIdx<(NUMENTRIES2-1) )
  {
    Float_t WLValMin = ( PhotonWL_WLS_EM_Bcf91a_core[EmissIdx]+PhotonWL_WLS_EM_Bcf91a_core[EmissIdx-1] ) /2.;
    Float_t WLValMax = ( PhotonWL_WLS_EM_Bcf91a_core[EmissIdx+1]+PhotonWL_WLS_EM_Bcf91a_core[EmissIdx] )/2.;

    WLVal = ( WLValMax - WLValMin ) * gRandom->Rndm() + WLValMin;
//     WLVal = ( WLValMax - WLValMin ) * CLHEP::RandFlat::shoot(0.,1.) + WLValMin;
  }

  if ( EmissIdx == 0 )  // first bin
  {
    Float_t WLValMax = ( PhotonWL_WLS_EM_Bcf91a_core[EmissIdx+1]+PhotonWL_WLS_EM_Bcf91a_core[EmissIdx] ) /2.;


    WLVal = ( WLValMax - PhotonWL_WLS_EM_Bcf91a_core[EmissIdx] ) *gRandom->Rndm() + PhotonWL_WLS_EM_Bcf91a_core[EmissIdx];
//     WLVal = ( WLValMax - PhotonWL_WLS_EM_Bcf91a_core[EmissIdx] ) * CLHEP::RandFlat::shoot(0.,1.) + PhotonWL_WLS_EM_Bcf91a_core[EmissIdx];

  }

  if ( EmissIdx == (NUMENTRIES2-1) )
  {
    Float_t WLValMin = ( PhotonWL_WLS_EM_Bcf91a_core[EmissIdx]+PhotonWL_WLS_EM_Bcf91a_core[EmissIdx-1] ) /2.;

    WLVal = ( PhotonWL_WLS_EM_Bcf91a_core[EmissIdx] - WLValMin ) * gRandom->Rndm() + WLValMin;
//     WLVal = ( PhotonWL_WLS_EM_Bcf91a_core[EmissIdx] - WLValMin ) * CLHEP::RandFlat::shoot(0.,1.) + WLValMin;

  }
}
else
  goto retry;

return WLVal;
}

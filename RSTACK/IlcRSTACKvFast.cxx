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

/* $Id: IlcRSTACKvFast.cxx 32955 2009-06-16 12:54:07Z fca $ */

/* History of cvs commits:
 *
 * $Log$
 * Revision 1.30  2006/09/13 07:31:01  kharlov
 * Effective C++ corrections (T.Pocheptsov)
 *
 * Revision 1.29  2005/05/28 14:19:05  schutz
 * Compilation warnings fixed by T.P.
 *
 */

//_________________________________________________________________________
// Implementation of the RSTACK manager class for fast simulations     
// Tracks particles until the reach a grossly designed RSTACK module
// Modify the particles property (momentum, energy, type) according to
//  the RSTACK response function. The result is called a virtual reconstructed
//  particle.                
//
//*-- Author: Yves Schutz (SUBATECH)

// --- ROOT system ---
 
#include <TGeometry.h>
#include <TParticle.h>
#include <TDatabasePDG.h>
#include "TClonesArray.h" 
#include <TVirtualMC.h>

// --- Standard library ---

// --- IlcRoot header files ---
#include "IlcRSTACKFastRecParticle.h"
#include "IlcRSTACKGeometry.h"
#include "IlcRSTACKLoader.h"
#include "IlcRSTACKvFast.h"
#include "IlcRun.h"

ClassImp(IlcRSTACKvFast)

IlcRSTACKvFast::IlcRSTACKvFast() :
  fBigBoxX(0.),
  fBigBoxY(0.),
  fBigBoxZ(0.),
  fFastRecParticles(0),
  fNRecParticles(0),
  fRan(0),
  fResPara1(0.),
  fResPara2(0.),
  fResPara3(0.),
  fPosParaA0(0.),
  fPosParaA1(0.),
  fPosParaB0(0.),
  fPosParaB1(0.),
  fPosParaB2(0.)    
{
  // default ctor : initialize data member
}

//____________________________________________________________________________
IlcRSTACKvFast::IlcRSTACKvFast(const char *name, const char *title):
  IlcRSTACK(name,title),
  fBigBoxX(0.),
  fBigBoxY(0.),
  fBigBoxZ(0.),
  fFastRecParticles(new IlcRSTACKFastRecParticle::FastRecParticlesList("IlcRSTACKFastRecParticle", 100)),
  fNRecParticles(0),
  fRan(0),
  fResPara1(0.030), // GeV
  fResPara2(0.00003),
  fResPara3(0.00001),
  fPosParaA0(2.87),    // mm
  fPosParaA1(-0.0975),
  fPosParaB0(0.257),
  fPosParaB1(0.137),
  fPosParaB2(0.00619)
{
  // ctor
  // create the Loader 
  SetBigBox(0, GetGeometry()->GetOuterBoxSize(0) ) ;
  SetBigBox(1, GetGeometry()->GetOuterBoxSize(3) + GetGeometry()->GetCPVBoxSize(1) ) ; 
  SetBigBox(2, GetGeometry()->GetOuterBoxSize(2) ); 
}

//____________________________________________________________________________
IlcRSTACKvFast::~IlcRSTACKvFast()
{
  // dtor
 
  fFastRecParticles->Delete() ; 
  delete fFastRecParticles ;
  fFastRecParticles = 0 ; 

}

//____________________________________________________________________________
void IlcRSTACKvFast::AddRecParticle(const IlcRSTACKFastRecParticle & rp)
{  
  // Add a virtually reconstructed particle to the list 

  new( (*fFastRecParticles)[fNRecParticles] ) IlcRSTACKFastRecParticle(rp) ;
  fNRecParticles++ ; 
}

//____________________________________________________________________________
void IlcRSTACKvFast::CreateGeometry()
{
  // Create the geometry for GEANT
  
  IlcRSTACKvFast *RSTACKtmp = (IlcRSTACKvFast*)gIlc->GetModule("RSTACK") ;
  
  if ( RSTACKtmp == NULL ) {
    
    fprintf(stderr, "RSTACK detector not found!\n") ;
    return ;
    
  }

  // Get pointer to the array containing media indeces
  Int_t *idtmed = fIdtmed->GetArray() - 699 ;
  
  Float_t bigbox[3] ; 
  bigbox[0] =   GetBigBox(0) / 2.0 ;
  bigbox[1] =   GetBigBox(1) / 2.0 ;
  bigbox[2] =   GetBigBox(2) / 2.0 ;
  
  gMC->Gsvolu("RSTACK", "BOX ", idtmed[798], bigbox, 3) ;
  
  // --- Position  RSTACK mdules in ILC setup ---
  
  Int_t idrotm[99] ;
  Double_t const kRADDEG = 180.0 / TMath::Pi() ;
  
  for( Int_t i = 1; i <= GetGeometry()->GetNModules(); i++ ) {
    
    Float_t angle = GetGeometry()->GetRSTACKAngle(i) ;
    IlcMatrix(idrotm[i-1], 90.0, angle, 90.0, 90.0+angle, 0.0, 0.0) ;
 
    Float_t r = GetGeometry()->GetIPtoCrystalSurface() + GetBigBox(1) / 2.0 ;

    Float_t xP1 = r * TMath::Sin( angle / kRADDEG ) ;
    Float_t yP1 = -r * TMath::Cos( angle / kRADDEG ) ;
    gMC->Gspos("RSTACK", i, "ILCM", xP1, yP1, 0.0, idrotm[i-1], "ONLY") ;
 
  } // for GetNModules

}


//____________________________________________________________________________
void IlcRSTACKvFast::Init(void)
{
  // Prints out an information message
  
  Int_t i;

  printf("\n");
  for(i=0;i<35;i++) printf("*");
  printf(" FAST RSTACK_INIT ");
  for(i=0;i<35;i++) printf("*");
  printf("\n");

  // Here the RSTACK initialisation code (if any!)

  for(i=0;i<80;i++) printf("*");
  printf("\n");
  
}

//___________________________________________________________________________
Float_t IlcRSTACKvFast::GetBigBox(Int_t index) const
{
  // Get the X, Y or Z dimension of the box describing a RSTACK module
  
  Float_t rv = 0 ; 

  switch (index) {
  case 0:
    rv = fBigBoxX ; 
    break ; 
  case 1:
     rv = fBigBoxY ; 
    break ; 
  case 2:
     rv = fBigBoxZ ; 
    break ; 
 }
  return rv ; 
}
//___________________________________________________________________________

void IlcRSTACKvFast::MakeBranch(Option_t* opt)
{  
  // Create new branch in the current reconstructed Root Tree
  IlcDetector::MakeBranch(opt);
  const char *cd = strstr(opt,"R");
  
  if (fFastRecParticles && fLoader->TreeR() && cd) {
    MakeBranchInTree(fLoader->TreeR(), GetName(), &fFastRecParticles, fBufferSize, 0);
  }
}
//____________________________________________________________________________

Double_t IlcRSTACKvFast::MakeEnergy(Double_t energy)
{  
  // Smears the energy according to the energy dependent energy resolution.
  // A gaussian distribution is assumed

  Double_t sigma  = SigmaE(energy) ; 
  return  fRan.Gaus(energy, sigma) ;   
}
//____________________________________________________________________________

TVector3 IlcRSTACKvFast::MakePosition(Double_t energy, TVector3 pos, Double_t theta, Double_t phi)
{
  // Smears the impact position according to the energy dependent position resolution
  // A gaussian position distribution is assumed

  TVector3 newpos ;
  Double_t sigma = SigmaP( energy, theta*180./TMath::Pi() ) ;
  Double_t x = fRan.Gaus( pos.X(), sigma ) ;
  sigma = SigmaP( energy, phi*180./TMath::Pi() ) ;
  Double_t z = fRan.Gaus( pos.Z(), sigma ) ;
  Double_t y = pos.Y() ; 
  
  newpos.SetX(x) ; 
  newpos.SetY(y) ; 
  newpos.SetZ(z) ; 
	      
  return newpos ; 
}

//____________________________________________________________________________
void IlcRSTACKvFast::MakeRecParticle(Int_t modid, TVector3 pos, IlcRSTACKFastRecParticle & rp)
{
  // Modify the primary particle properties according
  //  1. the response function of RSTACK
  //  2. the performance of the EMC+PPSD setup
  
  Int_t type = MakeType( rp ) ;
  rp.SetType(type) ;

  
  // get the detected energy

  TLorentzVector momentum ;  
  rp.Momentum(momentum) ; 
  Double_t kineticenergy = TMath::Sqrt( TMath::Power(momentum.E(), 2) - TMath::Power(rp.GetMass(), 2) ) ; 
  Double_t modifiedkineticenergy = MakeEnergy(kineticenergy ) ;
  Double_t modifiedenergy = TMath::Sqrt( TMath::Power(modifiedkineticenergy, 2)  
					 + TMath::Power( rp.GetMass(), 2) ) ;
 
  // get the angle of incidence 
  
  Double_t incidencetheta = 90. * TMath::Pi() /180 - rp.Theta() ; 
  Double_t incidencephi   = ( 270 + GetGeometry()->GetRSTACKAngle(modid) ) * TMath::Pi() / 180. - rp.Phi() ;   

  // get the detected direction
  
  TVector3 modifiedposition = MakePosition(kineticenergy, pos, incidencetheta, incidencephi) ; 
  modifiedposition *= modifiedkineticenergy / modifiedposition.Mag() ; 

  // Set the modified 4-momentum of the reconstructed particle

  rp.SetMomentum(modifiedposition.X(), modifiedposition.Y(), modifiedposition.Z(), modifiedenergy) ; 

 }

//____________________________________________________________________________
Int_t IlcRSTACKvFast::MakeType(IlcRSTACKFastRecParticle & rp )
{
  // Generate a particle type using the performance of the EMC+PPSD setup

  Int_t rv =   IlcRSTACKFastRecParticle::kUNDEFINED ;
  Int_t charge = (Int_t)rp.GetPDG()->Charge() ;
  Int_t test ; 
  Float_t ran ; 
  if ( charge != 0 && ( TMath::Abs(rp.GetPdgCode()) != 11 ) ) 
    test = - 1 ;
  else
    test = rp.GetPdgCode() ; 

  Fatal("MakeType", "SHOULD NOT BE USED until values of probabilities are properly set ") ;
   // NB: ALL VALUES SHOULD BE CHECKED !!!!
  switch (test) { 

  case 22:    // it's a photon              // NB: ALL VALUES SHOLD BE CHECKED !!!!
    ran = fRan.Rndm() ; 
    if( ran <= 0.9498 )
      rv =  IlcRSTACKFastRecParticle::kNEUTRALHAFAST ; 
    else
      rv =  IlcRSTACKFastRecParticle::kNEUTRALEMFAST ;     
    break ; 

  case 2112:  // it's a neutron
    ran = fRan.Rndm() ; 
    if ( ran <= 0.9998 )
      rv =  IlcRSTACKFastRecParticle::kNEUTRALHASLOW ; 
    else 
      rv = IlcRSTACKFastRecParticle::kNEUTRALEMSLOW ; 
    break ; 
    
  case -2112: // it's a anti-neutron
    ran = fRan.Rndm() ; 
    if ( ran <= 0.9984 )
      rv =  IlcRSTACKFastRecParticle::kNEUTRALHASLOW ; 
    else 
      rv =  IlcRSTACKFastRecParticle::kNEUTRALEMSLOW ; 
    break ; 
    
  case 11:    // it's a electron
    ran = fRan.Rndm() ; 
    if ( ran <= 0.9996 )
      rv =  IlcRSTACKFastRecParticle::kCHARGEDEMFAST ; 
    else 
      rv =  IlcRSTACKFastRecParticle::kCHARGEDHAFAST ; 
    break; 

  case -11:   // it's a positon
    ran = fRan.Rndm() ; 
    if ( ran <= 0.9996 )
      rv =  IlcRSTACKFastRecParticle::kCHARGEDEMFAST ; 
    else 
      rv =  IlcRSTACKFastRecParticle::kCHARGEDHAFAST ; 
    break; 

  case -1:    // it's a charged
    ran = fRan.Rndm() ; 
    if ( ran <= 0.9996 )
      rv =  IlcRSTACKFastRecParticle::kCHARGEDHAFAST ; 
    else 
      rv =  IlcRSTACKFastRecParticle::kNEUTRALHAFAST ; 

    break ; 
  }
    
  
  return rv ;
}

//___________________________________________________________________________
void IlcRSTACKvFast::ResetPoints()
{
  // This overloads the method in IlcDetector
  
  ResetFastRecParticles() ; 
}

//___________________________________________________________________________
void IlcRSTACKvFast::ResetFastRecParticles()
{
  // Resets the list of virtual reconstructed particles
 
  if (fFastRecParticles) 
    fFastRecParticles->Clear() ;
  fNRecParticles = 0 ; 
}

//___________________________________________________________________________
void IlcRSTACKvFast::SetBigBox(Int_t index, Float_t value)
{
  // Set the size of the Box describing a RSTACK module
  
  switch (index) {
  case 0:
    fBigBoxX = value ; 
    break ; 
  case 1:
    fBigBoxY = value ; 
    break ; 
  case 2:
    fBigBoxZ = value ; 
    break ; 
 }

}

//____________________________________________________________________________
Double_t IlcRSTACKvFast::SigmaE(Double_t energy)
{
  // Calculates the energy dependent energy resolution
  
  Double_t rv = -1 ; 
  
  rv = TMath::Sqrt( TMath::Power(fResPara1/energy, 2) 
	       + TMath::Power(fResPara2/TMath::Sqrt(energy), 2) 
	       + TMath::Power(fResPara3, 2) ) ;  

  return rv * energy ; 
}

//____________________________________________________________________________
Double_t IlcRSTACKvFast::SigmaP(Double_t energy, Double_t incidence)
{
  // Calculates the energy dependent position resolution 

  Double_t paraA = fPosParaA0 + fPosParaA1 * incidence ; 
  Double_t paraB = fPosParaB0 + fPosParaB1 * incidence + fPosParaB2 * incidence * incidence ; 

  return ( paraA / TMath::Sqrt(energy) + paraB ) * 0.1   ; // in cm  
}

//____________________________________________________________________________
void IlcRSTACKvFast::StepManager(void)
{
  // Only verifies if the particle reaches RSTACK and stops the tracking 

  TLorentzVector lv ; 
  gMC->TrackPosition(lv) ;
  TVector3 pos = lv.Vect() ; 
  Int_t modid  ; 
  gMC->CurrentVolID(modid);
  
  Float_t energy = gMC->Etot() ; //Total energy of current track

  //Calculating mass of current particle
  TDatabasePDG * pdg = TDatabasePDG::Instance() ;
  TParticlePDG * partPDG = pdg->GetParticle(gMC->TrackPid()) ;
  Float_t mass = partPDG->Mass() ;

  if(energy > mass){
    pos.SetMag(TMath::Sqrt(energy*energy-mass*mass)) ;
    TLorentzVector pTrack(pos, energy) ;  

    TParticle * part = new TParticle(gMC->TrackPid(), 0,-1,-1,-1,-1, pTrack, lv)  ;
        
    IlcRSTACKFastRecParticle rp(*part) ;

    // Adds the response of RSTACK to the particle
    MakeRecParticle(modid, pos, rp) ;
    
    // add the `track' particle to the FastRecParticles list
  
    AddRecParticle(rp) ;

    part->Delete() ;
  }
  // stop the track as soon RSTACK is reached
  
  gMC->StopTrack() ; 

}


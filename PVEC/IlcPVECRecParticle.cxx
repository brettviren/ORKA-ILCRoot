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
/* $Id: IlcPVECRecParticle.cxx 53186 2011-11-25 14:28:01Z kharlov $ */
//_________________________________________________________________________
//  A Reconstructed Particle in PVEC    
//  To become a general class of IlcRoot ?       
//  Why should I put meaningless comments
//  just to satisfy
//  the code checker                 
//       
//*-- Author: Yves Schutz (SUBATECH)


// --- ROOT system ---

// --- Standard library ---


// --- IlcRoot header files ---
#include "IlcStack.h"
#include "IlcPVECHit.h" 
#include "IlcPVECDigit.h" 
#include "IlcPVECTrackSegment.h" 
#include "IlcPVECEmcRecPoint.h" 
#include "IlcPVECRecParticle.h"
#include "IlcPVECLoader.h" 
#include "IlcPVECGeometry.h" 
#include "IlcLog.h"

//____________________________________________________________________________
IlcPVECRecParticle::IlcPVECRecParticle(): 
  fPVECTrackSegment(0),
  fDebug(kFALSE),
  fPos()
{
  // ctor
  const Int_t nSPECIES = IlcPID::kSPECIESN;
  for(Int_t i = 0; i<nSPECIES ; i++)
    fPID[i]=0.;
}


//____________________________________________________________________________
IlcPVECRecParticle::IlcPVECRecParticle(const IlcPVECRecParticle & rp):
  IlcPVECFastRecParticle(rp),
  fPVECTrackSegment(rp.fPVECTrackSegment),
  fDebug(kFALSE),
  fPos()
{
  // copy ctor
  fType             = rp.fType ; 
  fIndexInList      = rp.fIndexInList ;

  fPdgCode     = rp.fPdgCode;
  fStatusCode  = rp.fStatusCode;
  fMother[0]   = rp.fMother[0];
  fMother[1]   = rp.fMother[1];
  fDaughter[0] = rp.fDaughter[0];
  fDaughter[1] = rp.fDaughter[1];
  fWeight      = rp.fWeight;
  fCalcMass    = rp.fCalcMass;
  fPx          = rp.fPx;
  fPy          = rp.fPy;
  fPz          = rp.fPz;
  fE           = rp.fE;
  fVx          = rp.fVx;
  fVy          = rp.fVy;
  fVz          = rp.fVz;
  fVt          = rp.fVt;
  fPolarTheta  = rp.fPolarTheta;
  fPolarPhi    = rp.fPolarPhi;
  fParticlePDG = rp.fParticlePDG; 
  const Int_t nSPECIES = IlcPID::kSPECIESN;
  for(Int_t i = 0; i<nSPECIES ; i++)
    fPID[i]=rp.fPID[i];
}

//____________________________________________________________________________
IlcPVECRecParticle & IlcPVECRecParticle::operator = (const IlcPVECRecParticle &)
{
  Fatal("operator =", "not implemented");
  return *this;
}

//____________________________________________________________________________
Int_t IlcPVECRecParticle::GetNPrimaries() const  
{ 
  return -1;
}

//____________________________________________________________________________
Int_t IlcPVECRecParticle::GetNPrimariesToRecParticles() const  
{ 
  // Get the number of primaries at the origine of the RecParticle
  Int_t rv = 0 ;
  IlcRunLoader* rl = IlcRunLoader::Instance() ;
  IlcPVECLoader * PVECLoader = static_cast<IlcPVECLoader*>(rl->GetLoader("PVECLoader"));
  Int_t emcRPindex = static_cast<IlcPVECTrackSegment*>(PVECLoader->TrackSegments()->At(GetPVECTSIndex()))->GetEmcIndex();
  static_cast<IlcPVECEmcRecPoint*>(PVECLoader->EmcRecPoints()->At(emcRPindex))->GetPrimaries(rv) ; 
  return rv ; 
}

//____________________________________________________________________________
const TParticle * IlcPVECRecParticle::GetPrimary() const  
{
  // Get the primary particle at the origine of the RecParticle and 
  // which has deposited the largest energy in SDigits
  IlcRunLoader* rl = IlcRunLoader::Instance() ;
  IlcPVECLoader * PVECLoader = static_cast<IlcPVECLoader*>(rl->GetLoader("PVECLoader"));
  rl->GetEvent(rl->GetEventNumber()) ;
  rl->LoadKinematics("READ");
  rl->LoadSDigits("READ");
  
  if(GetNPrimaries() == 0)
    return 0 ;
  if(GetNPrimaries() == 1)
    return GetPrimary(0) ;
  Int_t AbsId = 0;
  Int_t module ;

  // Get PVEC Geometry object
  IlcPVECGeometry *geom;
  if (!(geom = IlcPVECGeometry::GetInstance())) 
        geom = IlcPVECGeometry::GetInstance("ORKA","");
  Double_t x,z ;
//DP to be fixed: Why do we use this method here??? M.B. use RecPoint???
  Double_t vtx[3]={0.,0.,0.} ;
  geom->ImpactOnEmc(vtx,static_cast<double>(Theta()),static_cast<double>(Phi()), module,z,x);
  Int_t amp = 0 ;
  Int_t iPrim=-1 ;
  if(module != 0){
    geom->RelPosToAbsId(module,x,z,AbsId) ;
   TClonesArray * sdigits = PVECLoader->SDigits() ;
   IlcPVECDigit * sdig ;
    
   for(Int_t i = 0 ; i < sdigits->GetEntriesFast() ; i++){
     sdig = static_cast<IlcPVECDigit *>(sdigits->At(i)) ;
     if((sdig->GetId() == AbsId)){
       if((sdig->GetAmp() > amp) && (sdig->GetNprimary())){
	 amp = sdig->GetAmp() ;
	 iPrim = sdig->GetPrimary(1) ;
       } 
       // do not scan rest of list
       if(sdig->GetId() > AbsId)
	 continue ; 
     }
   }
  }
  if(iPrim >= 0)
    return rl->Stack()->Particle(iPrim) ;
  else
    return 0 ;
} 
  
//____________________________________________________________________________
Int_t IlcPVECRecParticle::GetPrimaryIndex() const  
{
  // Get the primary track index in TreeK which deposits the most energy
  // in Digits which forms EmcRecPoint, which produces TrackSegment,
  // which the RecParticle is created from


  IlcRunLoader* rl = IlcRunLoader::Instance() ;
  IlcPVECLoader * PVECLoader = static_cast<IlcPVECLoader*>(rl->GetLoader("PVECLoader"));
  rl->GetEvent(rl->GetEventNumber()) ;
  rl->LoadHits("READ");
  rl->LoadDigits("READ");
  rl->LoadRecPoints("READ");
  rl->LoadTracks("READ");
  
  // Get TrackSegment corresponding to this RecParticle
  const IlcPVECTrackSegment *ts          = PVECLoader->TrackSegment(fPVECTrackSegment);

  // Get EmcRecPoint corresponding to this TrackSegment
  Int_t emcRecPointIndex = ts->GetEmcIndex();

  const IlcPVECEmcRecPoint  *emcRecPoint = PVECLoader->EmcRecPoint(emcRecPointIndex);

  // Get the list of digits forming this EmcRecParticle
  Int_t  nDigits   = emcRecPoint->GetDigitsMultiplicity();
  Int_t *digitList = emcRecPoint->GetDigitsList();

  // Find the digit with maximum amplitude
  Int_t maxAmp = 0;
  Int_t bestDigitIndex = -1;
  for (Int_t iDigit=0; iDigit<nDigits; iDigit++) {
    const IlcPVECDigit * digit = PVECLoader->Digit(digitList[iDigit]);
    if (digit->GetAmp() > maxAmp) {
      maxAmp = digit->GetAmp();
      bestDigitIndex = iDigit;
    }
  }
  if (bestDigitIndex>-1) {
    const IlcPVECDigit * digit = PVECLoader->Digit(digitList[bestDigitIndex]);
    if (digit==0) return -12345;
  }
  
  // Get the list of primary tracks producing this digit
  // and find which track has more track energy.
  //Int_t nPrimary = digit->GetNprimary();
  //TParticle *track = 0;
  //Double_t energyEM     = 0;
  //Double_t energyHadron = 0;
  //Int_t    trackEM      = -1;
  //Int_t    trackHadron  = -1;
  //for (Int_t iPrim=0; iPrim<nPrimary; iPrim++) {
  //  Int_t iPrimary = digit->GetPrimary(iPrim+1);
  //  if (iPrimary == -1 || TMath::Abs(iPrimary)>10000000)
  //    continue ;  //PH 10000000 is the shift of the track 
  //                //PH indexes in the underlying event
  //  track = gime->Primary(iPrimary);
  //  Int_t pdgCode   = track->GetPdgCode();
  //  Double_t energy = track->Energy();
  //  if (pdgCode==22 || TMath::Abs(pdgCode)==11) {
  //    if (energy > energyEM) {
  //	energyEM = energy;
  //	trackEM = iPrimary;
  //      }
  //   }
  //  else {
  //     if (energy > energyHadron) {
  //	energyHadron = energy;
  //	trackHadron = iPrimary;
	//    }
  //  }
  //}
  // Preferences are given to electromagnetic tracks
  //if (trackEM     != -1) return trackEM;     // track is gamma or e+-
  //if (trackHadron != -1) return trackHadron; // track is hadron
  //return -12345;                             // no track found :(


  // Get the list of hits producing this digit,
  // find which hit has deposited more energy 
  // and find the primary track.

  TClonesArray *hits = PVECLoader->Hits();
  if (hits==0) return -12345;

  Double_t maxedep  =  0;
  Int_t    maxtrack = -1;
  Int_t    nHits    = hits ->GetEntries();
  Int_t    id       = (PVECLoader->Digit(digitList[bestDigitIndex]))->GetId();

  for (Int_t iHit=0; iHit<nHits; iHit++) {
    const IlcPVECHit * hit = PVECLoader->Hit(iHit);
    if(hit->GetId() == id){
      Double_t edep  = hit->GetEnergy();
      Int_t    track = hit->GetPrimary();
      if(edep > maxedep){
	maxedep  = edep;
	maxtrack = track;
      }
    }
  }

  if (maxtrack != -1) return maxtrack; // track is hadron
  return -12345;                       // no track found :(
}

//____________________________________________________________________________
const TParticle * IlcPVECRecParticle::GetPrimary(Int_t index) const  
{
  // Get one of the primary particles at the origine of the RecParticle
  if ( index > GetNPrimariesToRecParticles() ) { 
    if (fDebug) 
      Warning("GetPrimary", "IlcPVECRecParticle::GetPrimary -> %d is larger that the number of primaries %d", 
	      index, GetNPrimaries()) ;
    return 0 ; 
  } 
  else { 
    Int_t dummy ; 
    IlcRunLoader* rl = IlcRunLoader::Instance() ;
    IlcPVECLoader * PVECLoader = static_cast<IlcPVECLoader*>(rl->GetLoader("PVECLoader"));

    Int_t emcRPindex = static_cast<IlcPVECTrackSegment*>(PVECLoader->TrackSegments()->At(GetPVECTSIndex()))->GetEmcIndex();
    Int_t primaryindex = static_cast<IlcPVECEmcRecPoint*>(PVECLoader->EmcRecPoints()->At(emcRPindex))->GetPrimaries(dummy)[index] ; 
    return rl->Stack()->Particle(primaryindex) ;
   } 
  //  return 0 ; 
}

//____________________________________________________________________________
void IlcPVECRecParticle::SetPID(Int_t type, Float_t weight)
{
  // Set the probability densities that this reconstructed particle
  // has a type of i:
  // i       particle types
  // ----------------------
  // 0       electron
  // 1       muon
  // 2       pi+-
  // 3       K+-
  // 4       p/pbar
  // 5       photon
  // 6       pi0 at high pt
  // 7       neutron
  // 8       K0L
  // 9       Conversion electron
  
  fPID[type] = weight ; 
}

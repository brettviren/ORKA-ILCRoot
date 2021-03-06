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

/* $Id: IlcAODJet.cxx 54128 2012-01-25 07:20:33Z kleinb $ */

//-------------------------------------------------------------------------
//     AOD class for jets
//     Author: Andreas Morsch, CERN
//-------------------------------------------------------------------------

#include <TLorentzVector.h>
#include "IlcAODJet.h"

ClassImp(IlcAODJet)


//______________________________________________________________________________
IlcAODJet::IlcAODJet() :
    IlcVParticle(),
    fNeutralFraction(0),
    fTrigger(0),
    fMomentum(0),
    fVectorAreaCharged(0),
    fRefTracks(new TRefArray())
{
  // constructor
    fBackgEnergy[0]   = 0.;     
    fBackgEnergy[1]   = 0.;
    fEffectiveArea[0] = 0.;   
    fEffectiveArea[1] = 0.;   
    fEffectiveAreaError[0] = fEffectiveAreaError[1] = 0;
    fPtSubtracted[0] =   fPtSubtracted[1] = 0;
}

IlcAODJet::IlcAODJet(Double_t px, Double_t py, Double_t pz, Double_t e):
    IlcVParticle(),
    fNeutralFraction(0),
    fTrigger(0),
    fMomentum(0),
    fVectorAreaCharged(0),
    fRefTracks(new TRefArray())
{
  // constructor
    fBackgEnergy[0]   = 0.;     
    fBackgEnergy[1]   = 0.;
    fEffectiveArea[0] = 0.;   
    fEffectiveArea[1] = 0.;
    fEffectiveAreaError[0] = fEffectiveAreaError[1] = 0;   
    fPtSubtracted[0] =   fPtSubtracted[1] = 0;
    fMomentum = new TLorentzVector(px, py, pz, e);
}

IlcAODJet::IlcAODJet(TLorentzVector & p):
    IlcVParticle(),
    fNeutralFraction(0),
    fTrigger(0),
    fMomentum(0),
    fVectorAreaCharged(0),
    fRefTracks(new TRefArray())
{
  // constructor
    fBackgEnergy[0]   = 0.;     
    fBackgEnergy[1]   = 0.;
    fEffectiveArea[0] = 0.;   
    fEffectiveArea[1] = 0.;
    fEffectiveAreaError[0] = fEffectiveAreaError[1] = 0;    
    fPtSubtracted[0] =   fPtSubtracted[1] = 0;
    fMomentum = new TLorentzVector(p);
}


//______________________________________________________________________________
IlcAODJet::~IlcAODJet() 
{
  // destructor
    delete fMomentum;
    delete fVectorAreaCharged;
    delete fRefTracks;
}

//______________________________________________________________________________
IlcAODJet::IlcAODJet(const IlcAODJet& jet) :
    IlcVParticle(jet),
    fNeutralFraction(jet.fNeutralFraction),
    fTrigger(jet.fTrigger),
    fMomentum(0),
    fVectorAreaCharged(0),
    fRefTracks(0)
{
  // Copy constructor
    fBackgEnergy[0]   = jet.fBackgEnergy[0];
    fBackgEnergy[1]   = jet.fBackgEnergy[1];
    fEffectiveArea[0] = jet.fEffectiveArea[0];
    fEffectiveArea[1] = jet.fEffectiveArea[1];
    fEffectiveAreaError[0] = jet.fEffectiveAreaError[0];
    fEffectiveAreaError[1] = jet.fEffectiveAreaError[1];
    fPtSubtracted[0] =   jet.fPtSubtracted[0];
    fPtSubtracted[1] =   jet.fPtSubtracted[1];
    if(jet.fMomentum)fMomentum  = new TLorentzVector(*jet.fMomentum);
    if(jet.fVectorAreaCharged)fVectorAreaCharged = new TLorentzVector(*jet.fVectorAreaCharged);
    fRefTracks = new TRefArray(*jet.fRefTracks);
}

//______________________________________________________________________________
IlcAODJet& IlcAODJet::operator=(const IlcAODJet& jet)
{
  // Assignment operator
  if(this!=&jet) {

    fBackgEnergy[0]   = jet.fBackgEnergy[0];
    fBackgEnergy[1]   = jet.fBackgEnergy[1];
    fEffectiveArea[0] = jet.fEffectiveArea[0];
    fEffectiveArea[1] = jet.fEffectiveArea[1];
    fEffectiveAreaError[0] = jet.fEffectiveAreaError[0];
    fEffectiveAreaError[1] = jet.fEffectiveAreaError[1];
    fPtSubtracted[0] =   jet.fPtSubtracted[0];
    fPtSubtracted[1] =   jet.fPtSubtracted[1];
    fNeutralFraction = jet.fNeutralFraction;
    fTrigger = jet.fTrigger;
    
    
    if(jet.fMomentum){
      if(fMomentum)*fMomentum  = *jet.fMomentum;
      else fMomentum  = new TLorentzVector(*jet.fMomentum);
    }

    if(jet.fVectorAreaCharged){
	if(fVectorAreaCharged)*fVectorAreaCharged = *jet.fVectorAreaCharged;
	else fVectorAreaCharged = new TLorentzVector(*jet.fVectorAreaCharged);
      }
    delete fRefTracks;
    fRefTracks = new TRefArray(*jet.fRefTracks);    
  }

  return *this;
}

void IlcAODJet::Print(Option_t* option) const
{

 if (!option) {
  // Print information of all data members
  printf("Jet 4-vector:\n");
  printf("     E  = %13.3f\n", E() );
  printf("     Px = %13.3f\n", Px());
  printf("     Py = %13.3f\n", Py());
  printf("     Pz = %13.3f\n", Pz());
  printf("Background Energy:\n");
  printf("Charged:  %13.3f\n", ChargedBgEnergy());
  printf("Neutral:  %13.3f\n", NeutralBgEnergy());
  printf("Total:    %13.3f\n", TotalBgEnergy());
  printf("Effective Area: \n");
  printf("Charged:  %13.3f\n", EffectiveAreaCharged());
  printf("Neutral:  %13.3f\n", EffectiveAreaNeutral());
 }
 else {
  printf("Jet %s, Eta: %13.3f, Phi: %13.3f, Pt: %13.3f\n",option,Eta(),Phi(),Pt());
 }

}

void  IlcAODJet::SetPxPyPzE(Double_t px, Double_t py, Double_t pz, Double_t e){
  // 
  // Set the four Momentum from outside
  // MomentumVector()->SetPxPyPzE() cannot be used since pointer can be 0x0
  //

  if(!fMomentum){
    fMomentum = new TLorentzVector(px,py,pz,e);
  }
  else{
    fMomentum->SetPxPyPzE(px,py,pz,e);
  }
}

void  IlcAODJet::SetPtEtaPhiM(Double_t pt, Double_t eta, Double_t phi, Double_t m){
  // 
  // Set the four Momentum from outside with pt eta phi and M
  // MomentumVector()->SetPtPhiEtaM() cannot be used since pointer can be 0x0
  //

  if(!fMomentum){
    fMomentum = new TLorentzVector();
  }
  fMomentum->SetPtEtaPhiM(pt,eta,phi,m);
}



Double_t IlcAODJet::DeltaR(const IlcVParticle* part) const {

  // Helper function to calculate the distance between two jets
  // or a jet and particle

  Double_t dPhi = Phi() - part->Phi(); 
  if(dPhi>TMath::Pi())dPhi = dPhi - 2.*TMath::Pi();
  if(dPhi<(-1.*TMath::Pi()))dPhi = dPhi + 2.*TMath::Pi();
  Double_t dEta = Eta() - part->Eta(); 
  Double_t dR = TMath::Sqrt(dPhi*dPhi+dEta*dEta);
  return dR;
}


void  IlcAODJet::AddTrack(TObject *tr) {
    // Add a track to the list of referenced tracks
    if (fRefTracks->GetEntries() == 0) {
      fRefTracks->Delete();
      new(fRefTracks) TRefArray(TProcessID::GetProcessWithUID(tr));
    }
    
    fRefTracks->Add(tr);
}


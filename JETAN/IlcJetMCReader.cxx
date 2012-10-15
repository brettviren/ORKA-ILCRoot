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
 
// Jet MC Reader 
// MC reader for jet analysis
// Author: Mercedes Lopez Noriega (mercedes.lopez.noriega@cern.ch)

// From root ...
#include <TClonesArray.h>
#include <TPDGCode.h>
#include <TParticle.h>
#include <TParticlePDG.h>
#include <TVector3.h>
#include <TLorentzVector.h>
#include <TSystem.h>
#include <TChain.h>
// From IlcRoot ...
#include "IlcJetMCReader.h"
#include "IlcJetMCReaderHeader.h"
#include "IlcESDEvent.h"
#include "IlcESDtrack.h"

ClassImp(IlcJetMCReader)


IlcJetMCReader::IlcJetMCReader():
    IlcJetESDReader(),
    fChainMC(0x0)
{
  // Constructor
}

//____________________________________________________________________________

IlcJetMCReader::~IlcJetMCReader()
{
  // Destructor
    delete fChainMC;
}

//____________________________________________________________________________


Bool_t IlcJetMCReader::FillMomentumArray(Int_t event)
{
// Fill momentum array
  TClonesArray &arrayMC = *fArrayMC;
  Int_t goodTrack = 0;
  Int_t nt = 0;
  Float_t pt, e;
  TVector3 p;

  // clear array
  ClearArray();
  // get event from chains
  fChain->GetEntry(event);
  fChainMC->GetEntry(event);
  // get number of tracks in event (for the loop)
  nt = fESD->GetNumberOfTracks();

  // get cuts set by user
  Double_t ptMin = ((IlcJetMCReaderHeader*) fReaderHeader)->GetPtCut();

  //loop over particles
  for (Int_t it = 0; it < nt; it++) {
    IlcESDtrack *track = fESD->GetTrack(it); //track
    UInt_t status = track->GetStatus();
    if ((status & IlcESDtrack::kITSrefit) == 0) continue; // quality check
    //    track->GetImpactParameters(dca,z);
    // if (dca > dcaMax) continue; // check track is reasonable 
    Int_t label = TMath::Abs(track->GetLabel());
    TParticle *part = (TParticle*)arrayMC[label]; //particle
    pt = part->Pt(); // pt of the particle
    if (pt < ptMin) continue; //check  cuts 
    p = part->P();
    e = part->Energy();
   // fill momentum array
    new ((*fMomentumArray)[goodTrack]) TLorentzVector(p.X(), p.Y(), p.Z(), e);
    goodTrack++;
  }
  printf("\nNumber of good tracks %d \n", goodTrack);
  return kTRUE;
}



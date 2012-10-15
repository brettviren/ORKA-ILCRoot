#if !defined(__CINT__) || defined(__MAKECINT__)

#include <Riostream.h>
#include <TFile.h>
#include <TTree.h>
#include <TMath.h>

#include "IlcRun.h"
#include "IlcRunLoader.h"
#include "IlcHeader.h"
#include "IlcStack.h"
#include "TParticle.h"

#include "IlcAODEvent.h"
#include "IlcAODHeader.h"
#include "IlcAODVertex.h"
#include "IlcAODTrack.h"
#include "IlcAODCluster.h"

#endif

// function declarations
void SetVertexType(TParticle *part, IlcAODVertex *vertex);
void SetChargeAndPID(Int_t pdgCode, IlcAODTrack *track);
Int_t LoopOverSecondaries(TParticle *mother);
void CreateAODfromKineTree(const char *inFileName = "gilc.root",
			   const char *outFileName = "IlcAOD.root");

// global variables
IlcAODEvent *aod = NULL;
IlcStack *stack = NULL;

Int_t jVertices = 0;
Int_t jTracks = 0; 
Int_t jKinks = 0;
Int_t jV0s = 0;
Int_t jCascades = 0;
Int_t jMultis = 0;
Int_t nPos = 0;
Int_t nNeg = 0;

Int_t nGamma = 0;
Int_t nElectron = 0;
Int_t nPositron = 0;
Int_t nMuon = 0;
Int_t nAntiMuon = 0;
Int_t nProton = 0;
Int_t nAntiProton = 0;
Int_t nNeutron = 0;
Int_t nAntiNeutron = 0;
Int_t nPi0 = 0;
Int_t nPiMinus = 0;
Int_t nPiPlus = 0;
Int_t nK0 = 0;
Int_t nKPlus = 0;
Int_t nKMinus = 0;

// global arrays and pointers
Float_t p[3];
Float_t x[3];
Float_t *cov = NULL; // set to NULL because not provided
Float_t *pid = NULL; // set to NULL because not provided
IlcAODVertex *primary = NULL; 
IlcAODVertex *secondary = NULL;
IlcAODTrack *currTrack = NULL;

void CreateAODfromKineTree(const char *inFileName,
			   const char *outFileName) {
  printf("This macro works only correctly in comiled mode!\n");
  /* This probem is due to CINT which gets confused if arrays are reused */

  // create an IlcAOD object 
  aod = new IlcAODEvent();
  aod->CreateStdContent();

  // open the file
  TFile *outFile = TFile::Open(outFileName, "RECREATE");

  // create the tree
  TTree *aodTree = new TTree("aodTree", "IlcAOD tree");
  aodTree->Branch(aod->GetList());

  IlcRunLoader *runLoader;
  delete gIlc;
  gIlc = NULL;
  runLoader = IlcRunLoader::Open(inFileName);
  if (!runLoader) return;

  runLoader->LoadHeader();
  runLoader->LoadKinematics();

  Int_t nEvents = runLoader->GetNumberOfEvents();

  // loop over events and fill them
  for (Int_t iEvent = 0; iEvent < nEvents; ++iEvent) {
 
    cout << "Event " << iEvent+1 << "/" << nEvents;
    runLoader->GetEvent(iEvent);

    IlcHeader *IlcHeader = runLoader->GetHeader();
    stack = runLoader->Stack();
    Int_t nTracks = stack->GetNtrack();
    Int_t nPrims = stack->GetNprimary();

    nPos = 0;
    nNeg = 0;
 
    nGamma = 0;
    nElectron = 0;
    nPositron = 0;
    nMuon = 0;
    nAntiMuon = 0;
    nProton = 0;
    nAntiProton = 0;
    nNeutron = 0;
    nAntiNeutron = 0;
    nPi0 = 0;
    nPiMinus = 0;
    nPiPlus = 0;
    nK0 = 0;
    nKPlus = 0;
    nKMinus = 0;

    // Access to the header
    IlcAODHeader *header = aod->GetHeader();

    Double_t emEnergy[2] = {-999., -999.};

    // fill the header
    *header = IlcAODHeader(IlcHeader->GetRun(),
			   0, // bunchX number
			   0, // orbit number
			   0, // period number
			   nTracks,
			   nPos,
			   nNeg,
			   -999, // mag. field
			   -999., // muon mag. field
			   -999., // centrality
			   -999., // ZDCN1Energy
			   -999., // ZDCP1Energy
			   -999., // ZDCN2Energy
			   -999., // ZDCP2Energy
			   emEnergy, // emEnergy
			   0, // TriggerMask
			   0, // TriggerCluster
			   0, // EventType
			   ""); // title
  
    // Access to the AOD container of vertices
    TClonesArray &vertices = *(aod->GetVertices());
    jVertices=0;
    jKinks=0;
    jV0s=0;
    jCascades=0;
    jMultis=0;

    // Access to the AOD container of tracks
    TClonesArray &tracks = *(aod->GetTracks());
    jTracks=0; 
 
    aod->ResetStd(nTracks, 1);

    // track loop
    for (Int_t iTrack = 0; iTrack < nPrims; ++iTrack) {
								
      TParticle *part = stack->Particle(iTrack);

      //if (part->IsPrimary()) { // this will exclude 'funny' primaries, too
      if (kTRUE) { // this won't
	p[0] = part->Px(); p[1] = part->Py(); p[2] = part->Pz();
	x[0] = part->Vx(); x[1] = part->Vy(); x[2] = part->Vz();
	
	if (iTrack == 0) {
	  // add primary vertex
	  primary = new(vertices[jVertices++])
	    IlcAODVertex(x, NULL, -999., NULL, IlcAODVertex::kPrimary);
	}
	
	// add primary tracks
	primary->AddDaughter(new(tracks[jTracks++]) IlcAODTrack(0, // ID,
								0, // Label
								p,
								kTRUE,
								x,
								kFALSE,
								cov, 
								(Short_t)-99,
								0, // no ITSClusterMap
								pid,
								primary,
					                        kFALSE,  // no fit performed
								kFALSE, // no fit preformed
								IlcAODTrack::kPrimary));
	currTrack = (IlcAODTrack*)tracks.Last();
	SetChargeAndPID(part->GetPdgCode(), currTrack);
	if (currTrack->Charge() != -99) {
	  if (currTrack->Charge() > 0) {
	    nPos++;
	  } else if (currTrack->Charge() < 0) {
	    nNeg++;
	  }	    
	}
	
	LoopOverSecondaries(part);
      } 
    } // end of track loop
    
    header->SetRefMultiplicityPos(nPos);
    header->SetRefMultiplicityNeg(nNeg);
    
    cout << ":: primaries: " << nPrims << " secondaries: " << tracks.GetEntriesFast()-nPrims << 
      " (pos: " << nPos << ", neg: " << nNeg << "), vertices: " << vertices.GetEntriesFast() << 
      " (kinks: " << jKinks << ", V0: " << jV0s << ", cascades: " << jCascades << 
      ", multi: " << jMultis << ")" << endl;
    /*
    cout << "  gamma: " << nGamma << " e-: " << nElectron << " e+: " << nPositron << " p: " << nProton << 
      " pBar: " << nAntiProton << " n: " << nNeutron << " nBar: " << nAntiNeutron << " pi0: " << nPi0 <<
      " pi+: " << nPiPlus << " pi-: " << nPiMinus << " K0: " << nK0 << " K+: " << nKPlus << 
      " K-: " << nKMinus << endl;
    */
    // fill the tree for this event
    aodTree->Fill();
  } // end of event loop

  aodTree->GetUserInfo()->Add(aod);
  
  // write the tree to the specified file
  outFile = aodTree->GetCurrentFile();
  outFile->cd();
  aodTree->Write();
  outFile->Close();

}


Int_t LoopOverSecondaries(TParticle *mother) {
  
  if (mother->GetNDaughters() > 0) {

    TClonesArray &vertices = *(aod->GetVertices());
    TClonesArray &tracks = *(aod->GetTracks());

    for (Int_t iDaughter = mother->GetFirstDaughter(); iDaughter <= mother->GetLastDaughter(); iDaughter++) {
      TParticle *part = stack->Particle(iDaughter);
      p[0] = part->Px(); 
      p[1] = part->Py(); 
      p[2] = part->Pz();
      x[0] = part->Vx(); 
      x[1] = part->Vy(); 
      x[2] = part->Vz();

      if (iDaughter == mother->GetFirstDaughter()) {
	// add secondary vertex
	secondary = new(vertices[jVertices++])
	  IlcAODVertex(x, NULL, -999., tracks.Last(), IlcAODVertex::kUndef);
	
	SetVertexType(part, secondary);
      }
	
      // add secondary tracks
      secondary->AddDaughter(new(tracks[jTracks++]) IlcAODTrack(0, // ID
								0, // label
								p,
								kTRUE,
								x,
								kFALSE,
								cov, 
								(Short_t)-99,
								0, // no cluster map available
								pid,
								secondary,
								kFALSE, // no fit performed
					                        kFALSE, // no fit performed
								IlcAODTrack::kSecondary));

      currTrack = (IlcAODTrack*)tracks.Last();
      SetChargeAndPID(part->GetPdgCode(), currTrack);
      if (currTrack->Charge() != -99) {
	if (currTrack->Charge() > 0) {
	  nPos++;
	} else if (currTrack->Charge() < 0) {
	  nNeg++;
	}	    
      }

      LoopOverSecondaries(part);
    }
    return 1;
  } else {
    return 0;
  }
}


void SetChargeAndPID(Int_t pdgCode, IlcAODTrack *track) {

  Float_t PID[10] = {0., 0., 0., 0., 0., 0., 0., 0., 0., 0.};

  switch (pdgCode) {

  case 22: // gamma
    track->SetCharge(0);
    PID[IlcAODTrack::kUnknown] = 1.;
    track->SetPID(PID);
    nGamma++;
    break;

  case 11: // e- 
    track->SetCharge(-1);
    PID[IlcAODTrack::kElectron] = 1.;
    track->SetPID(PID);
    nElectron++;
    break;
    
  case -11: // e+
    track->SetCharge(+1);
    PID[IlcAODTrack::kElectron] = 1.;
    track->SetPID(PID);
    nPositron++;
    break;
    
  case 13: // mu- 
    track->SetCharge(-1);
    PID[IlcAODTrack::kMuon] = 1.;
    track->SetPID(PID);
    nMuon++;
    break;
    
  case -13: // mu+
    track->SetCharge(+1);
    PID[IlcAODTrack::kMuon] = 1.;
    track->SetPID(PID);
    nAntiMuon++;
    break;
    
  case 111: // pi0
    track->SetCharge(0);
    PID[IlcAODTrack::kUnknown] = 1.;
    track->SetPID(PID);
    nPi0++;
    break;
    
  case 211: // pi+
    track->SetCharge(+1);
    PID[IlcAODTrack::kPion] = 1.;
    track->SetPID(PID);
    nPiPlus++;
    break;
    
  case -211: // pi-
    track->SetCharge(-1);
    PID[IlcAODTrack::kPion] = 1.;
    track->SetPID(PID);
    nPiMinus++;
    break;
    
  case 130: // K0L
    track->SetCharge(0);
    PID[IlcAODTrack::kUnknown] = 1.;
    track->SetPID(PID);
    nK0++;
    break;
    
  case 321: // K+
    track->SetCharge(+1);
    PID[IlcAODTrack::kKaon] = 1.;
    track->SetPID(PID);
    nKPlus++;
    break;
    
  case -321: // K- 
    track->SetCharge(-1);
    PID[IlcAODTrack::kKaon] = 1.;
    track->SetPID(PID);
    nKMinus++;
    break;
    
  case 2112: // n
    track->SetCharge(0);
    PID[IlcAODTrack::kUnknown] = 1.;
    track->SetPID(PID);
    nNeutron++;
    break;
    
  case 2212: // p
    track->SetCharge(+1);
    PID[IlcAODTrack::kProton] = 1.;
    track->SetPID(PID);
    nProton++;
    break;
    
  case -2212: // anti-p
    track->SetCharge(-1);
    PID[IlcAODTrack::kProton] = 1.;
    track->SetPID(PID);
    nAntiProton++;
    break;

  case 310: // K0S
    track->SetCharge(0);
    PID[IlcAODTrack::kUnknown] = 1.;
    track->SetPID(PID);
    nK0++;
    break;
    
  case 311: // K0
    track->SetCharge(0);
    PID[IlcAODTrack::kUnknown] = 1.;
    track->SetPID(PID);
    nK0++;
    break;
    
  case -311: // anti-K0
    track->SetCharge(0);
    PID[IlcAODTrack::kUnknown] = 1.;
    track->SetPID(PID);
    nK0++;
    break;
    
  case 221: // eta
    track->SetCharge(0);
    PID[IlcAODTrack::kUnknown] = 1.;
    track->SetPID(PID);
    break;

  case 3122: // lambda
    track->SetCharge(0);
    PID[IlcAODTrack::kUnknown] = 1.;
    track->SetPID(PID);
    break;

  case 3222: // Sigma+
    track->SetCharge(+1);
    PID[IlcAODTrack::kUnknown] = 1.;
    track->SetPID(PID);
    break;

  case 3212: // Sigma0
    track->SetCharge(-1);
    PID[IlcAODTrack::kUnknown] = 1.;
    track->SetPID(PID);
    break;

  case 3112: // Sigma-
    track->SetCharge(-1);
    PID[IlcAODTrack::kUnknown] = 1.;
    track->SetPID(PID);
    break;

  case 3322: // Xi0
    track->SetCharge(0);
    PID[IlcAODTrack::kUnknown] = 1.;
    track->SetPID(PID);
    break;

  case 3312: // Xi-
    track->SetCharge(-1);
    PID[IlcAODTrack::kUnknown] = 1.;
    track->SetPID(PID);
    break;

  case 3334: // Omega-
    track->SetCharge(-1);
    PID[IlcAODTrack::kUnknown] = 1.;
    track->SetPID(PID);
    break;

  case -2112: // n-bar
    track->SetCharge(0);
    PID[IlcAODTrack::kUnknown] = 1.;
    track->SetPID(PID);
    break;

  case -3122: // anti-Lambda
    track->SetCharge(0);
    PID[IlcAODTrack::kUnknown] = 1.;
    track->SetPID(PID);
    break;

  case -3222: // anti-Sigma-
    track->SetCharge(-1);
    PID[IlcAODTrack::kUnknown] = 1.;
    track->SetPID(PID);
    break;

  case -3212: // anti-Sigma0
    track->SetCharge(0);
    PID[IlcAODTrack::kUnknown] = 1.;
    track->SetPID(PID);
    break;

  case -3112: // anti-Sigma+
    track->SetCharge(+1);
    PID[IlcAODTrack::kUnknown] = 1.;
    track->SetPID(PID);
    break;

  case -3322: // anti-Xi0
    track->SetCharge(0);
    PID[IlcAODTrack::kUnknown] = 1.;
    track->SetPID(PID);
    break;

  case -3312: // anti-Xi+
    track->SetCharge(+1);
    break;

  case -3334: // anti-Omega+
    track->SetCharge(+1);
    PID[IlcAODTrack::kUnknown] = 1.;
    track->SetPID(PID);
    break;

  case 411: // D+
    track->SetCharge(+1);
    PID[IlcAODTrack::kUnknown] = 1.;
    track->SetPID(PID);
    break;

  case -411: // D- 
    track->SetCharge(-1);
    PID[IlcAODTrack::kUnknown] = 1.;
    track->SetPID(PID);
    break;

  case 421: // D0
    track->SetCharge(0);
    PID[IlcAODTrack::kUnknown] = 1.;
    track->SetPID(PID);
    break;

  case -421: // anti-D0
    track->SetCharge(0);
    PID[IlcAODTrack::kUnknown] = 1.;
    track->SetPID(PID);
    break;

  default : // unknown
    track->SetCharge(-99);
    PID[IlcAODTrack::kUnknown] = 1.;
    track->SetPID(PID);
 }

  return;
}


void SetVertexType(TParticle *part, IlcAODVertex *vertex) {
  // this whole thing doesn't make much sense. but anyhow...

  TParticle *mother = stack->Particle(part->GetFirstMother());
  Int_t pdgMother = mother->GetPdgCode();
  Int_t pdgPart = part->GetPdgCode();
  
  // kinks
  if (mother->GetNDaughters() == 2) {
    Int_t firstPdgCode = stack->Particle(mother->GetFirstDaughter())->GetPdgCode();
    Int_t lastPdgCode = stack->Particle(mother->GetLastDaughter())->GetPdgCode();

    if (!(pdgMother == 22 || pdgMother == 111 || pdgMother == 130 || 
	  TMath::Abs(pdgMother) == 2112 || pdgMother == 310 || pdgMother == 221 || 
	  TMath::Abs(pdgMother) == 3122 || TMath::Abs(pdgMother) == 3322 || 
	  pdgMother == -3212 || TMath::Abs(pdgMother) == 421 || 
	  TMath::Abs(pdgMother) == 311) // not neutral
	&& (((firstPdgCode == 22 || firstPdgCode == 111 || firstPdgCode == 130 || 
	      TMath::Abs(firstPdgCode) == 2112 || firstPdgCode == 310 || 
	      firstPdgCode == 221 || TMath::Abs(firstPdgCode) == 3122 || 
	      TMath::Abs(firstPdgCode) == 3322 || firstPdgCode == -3212 || 
	      TMath::Abs(firstPdgCode) == 421 || TMath::Abs(pdgMother) == 311) // neutral
	     && !(lastPdgCode == 22 || lastPdgCode == 111 || lastPdgCode == 130 || 
		  TMath::Abs(lastPdgCode) == 2112 || lastPdgCode == 310 || 
		  lastPdgCode == 221 || TMath::Abs(lastPdgCode) == 3122 || 
		  TMath::Abs(lastPdgCode) == 3322 || lastPdgCode == -3212 || 
		  TMath::Abs(lastPdgCode) == 421 || TMath::Abs(pdgMother) == 311)) // not neutral
	    || !((firstPdgCode == 22 || firstPdgCode == 111 || firstPdgCode == 130 || 
		  TMath::Abs(firstPdgCode) == 2112 || firstPdgCode == 310 || 
		  firstPdgCode == 221 || TMath::Abs(firstPdgCode) == 3122 || 
		  TMath::Abs(firstPdgCode) == 3322 || firstPdgCode == -3212 || 
		  TMath::Abs(firstPdgCode) == 421 || TMath::Abs(pdgMother) == 311) // not neutral
		 && (lastPdgCode == 22 || lastPdgCode == 111 || lastPdgCode == 130 || 
		     TMath::Abs(lastPdgCode) == 2112 || lastPdgCode == 310 || 
		     lastPdgCode == 221 || TMath::Abs(lastPdgCode) == 3122 || 
		     TMath::Abs(lastPdgCode) == 3322 || lastPdgCode == -3212 || 
		     TMath::Abs(lastPdgCode) == 421 || TMath::Abs(pdgMother) == 311)))) { // neutral
      
      vertex->SetType(IlcAODVertex::kKink);
      jKinks++;
    }
  }

  // V0
  else if (mother->GetNDaughters() == 2) {
    Int_t firstPdgCode = stack->Particle(mother->GetFirstDaughter())->GetPdgCode();
    Int_t lastPdgCode = stack->Particle(mother->GetLastDaughter())->GetPdgCode();

    if ((pdgMother == 22 || pdgMother == 111 || pdgMother == 130 || 
	 TMath::Abs(pdgMother) == 2112 || pdgMother == 310 || 
	 pdgMother == 221 || TMath::Abs(pdgMother) == 3122 || 
	 TMath::Abs(pdgMother) == 3322 || pdgMother == -3212 || 
	 TMath::Abs(pdgMother) == 421 || TMath::Abs(pdgMother) == 311) // neutral
	&& !(lastPdgCode == 22 || lastPdgCode == 111 || lastPdgCode == 130 || 
	     TMath::Abs(lastPdgCode) == 2112 || lastPdgCode == 310 || 
	     lastPdgCode == 221 || TMath::Abs(lastPdgCode) == 3122 || 
	     TMath::Abs(lastPdgCode) == 3322 || lastPdgCode == -3212 || 
	     TMath::Abs(lastPdgCode) == 421 || TMath::Abs(pdgMother) == 311) // not neutral
	&& !(firstPdgCode == 22 || firstPdgCode == 111 || firstPdgCode == 130 || 
	     TMath::Abs(firstPdgCode) == 2112 || firstPdgCode == 310 || 
	     firstPdgCode == 221 || TMath::Abs(firstPdgCode) == 3122 || 
	     TMath::Abs(firstPdgCode) == 3322 || firstPdgCode == -3212 || 
	     TMath::Abs(firstPdgCode) == 421 || TMath::Abs(pdgMother) == 311)) { // not neutral
      
      vertex->SetType(IlcAODVertex::kV0);
      jV0s++;
    }
  }

  // Cascade
  else if (mother->GetNDaughters() == 2) {
    Int_t firstPdgCode = stack->Particle(mother->GetFirstDaughter())->GetPdgCode();
    Int_t lastPdgCode = stack->Particle(mother->GetLastDaughter())->GetPdgCode();
    
    if ((TMath::Abs(pdgMother) == 3334 || TMath::Abs(pdgMother) == 3312 || TMath::Abs(pdgMother) == 3322) &&
	(TMath::Abs(pdgPart) == 3122 || TMath::Abs(pdgPart) == 211 || TMath::Abs(pdgPart) == 321)
	&& ((!(firstPdgCode == 22 || firstPdgCode == 111 || firstPdgCode == 130 || 
	       TMath::Abs(firstPdgCode) == 2112 || firstPdgCode == 310 || 
	       firstPdgCode == 221 || TMath::Abs(firstPdgCode) == 3122 || 
	       TMath::Abs(firstPdgCode) == 3322 || firstPdgCode == -3212 || 
	       TMath::Abs(firstPdgCode) == 421 || TMath::Abs(pdgMother) == 311) // not neutral   
	     && TMath::Abs(lastPdgCode) == 3122) // labmda or anti-lambda
	    || ((!(lastPdgCode == 22 || lastPdgCode == 111 || lastPdgCode == 130 || 
		   TMath::Abs(lastPdgCode) == 2112 || lastPdgCode == 310 || 
		   lastPdgCode == 221 || TMath::Abs(lastPdgCode) == 3122 || 
		   TMath::Abs(lastPdgCode) == 3322 || lastPdgCode == -3212 || 
		   TMath::Abs(lastPdgCode) == 421 || TMath::Abs(pdgMother) == 311) // not neutral
		 && TMath::Abs(firstPdgCode) == 3122)))) { // lambda or anti-lambda
      vertex->SetType(IlcAODVertex::kCascade);
      jCascades++;
    }
  }

  // Multi
  else if (mother->GetNDaughters() > 2) {

    vertex->SetType(IlcAODVertex::kMulti);
    jMultis++;
  }

  else {
    vertex->SetType(IlcAODVertex::kUndef);
  }
}
//  LocalWords:  SetCharge

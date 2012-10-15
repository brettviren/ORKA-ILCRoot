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

/* $Id: IlcESDMuonTest.C 19648 2007-07-12 07:36:20Z kleinb $ */

//
// Macro for checking ilcroot output and associated files contents
// Gines Martinez, Subatech June 2003
//

// ROOT includes
#include "TBranch.h"
#include "TClonesArray.h"
#include "TFile.h"
#include "TH1.h"
#include "TParticle.h"
#include "TTree.h"

// STEER includes
#include "IlcRun.h"
#include "IlcRunLoader.h"
#include "IlcHeader.h"
#include "IlcLoader.h"
#include "IlcStack.h"
#include "IlcESDEvent.h"

// MUON includes
#include "IlcMUON.h"
#include "IlcMUONData.h"
#include "IlcMUONConstants.h"
#include "IlcMUONRawCluster.h"
#include "IlcMUONTrack.h"
#include "IlcMUONTriggerTrack.h"
#include "IlcESDMuonTrack.h"


void IlcESDMuonTest(char * filename="gilc.root", Int_t run=0){

  TClonesArray* recTracksArray;
  TClonesArray* recTrigTracksArray;
  TTree* treeE;
  // Creating Run Loader and openning file containing Hits
  IlcRunLoader * RunLoader = IlcRunLoader::Open(filename,"MUONFolder","READ");
  if (RunLoader ==0x0) {
    printf(">>> Error : Error Opening %s file \n",filename);
    return;
  }
  
  // creating ESD file
  TFile *ef=TFile::Open("IlcESD.Muon.root","RECREATE");
  if (!ef->IsOpen()) cerr<<"Can't open IlcESD.root file!\n";

  treeE = new TTree("ESD", "ESD");

  IlcLoader* MUONLoader = RunLoader->GetLoader("MUONLoader");
  MUONLoader->LoadTracks("READ");

  // Creating MUON data container
  IlcMUONData muondata(MUONLoader,"MUON","MUON");

  // declaration  
  Int_t ievent, nevents;
  Int_t ntrackhits;
  Double_t fitfmin;
  Int_t nrectracks;

  Double_t bendingSlope, nonBendingSlope, fInverseBendingMomentum;
  Double_t fXRec, fYRec, fZRec, chi2MatchTrigger;
  Bool_t matchTrigger;

  nevents = RunLoader->GetNumberOfEvents();

  // setting pointer for tracks, triggertracks& trackparam at vertex
  IlcMUONTrack* rectrack;
  IlcMUONTriggerTrack* rectriggertrack;
  IlcMUONTrackParam* trackParam;

  for (ievent = 0; ievent < nevents; ievent++) {

    RunLoader->GetEvent(ievent);

    // setting ESD class pointer
    IlcESDEvent* event = new IlcESDEvent();
    event->CreateStdContent();
    char name[255];
    event->WriteToTree(treeE);

    event->SetRunNumber(run);
    event->SetEventNumber(ievent);

    // setting ESD MUON class
    IlcESDMuonTrack* ESDTrack = new  IlcESDMuonTrack() ;

 // ---------------- tracks ----------------
    muondata.SetTreeAddress("RT");
    muondata.GetRecTracks();
    recTracksArray = muondata.RecTracks();
    nrectracks = (Int_t) recTracksArray->GetEntriesFast();

// --------------- trigger tracks ----------
    Long_t trigPat = 0;

    muondata.SetTreeAddress("RL");
    muondata.GetRecTriggerTracks();
    recTrigTracksArray = muondata.RecTriggerTracks();
    rectriggertrack = (IlcMUONTriggerTrack*) recTrigTracksArray->First();
    trigPat = rectriggertrack->GetGTPattern();

    printf(">>> Event %d Number of Recconstructed tracks %d \n",ievent, nrectracks);
 
    for (Int_t irectracks = 0; irectracks <  nrectracks;  irectracks++) {

      // ---------------- tracks ----------------
	rectrack = (IlcMUONTrack*) recTracksArray->At(irectracks);    
	trackParam = rectrack->GetTrackParamAtVertex();

	bendingSlope            = trackParam->GetBendingSlope();
	nonBendingSlope         = trackParam->GetNonBendingSlope();
	//	printf(" SlopeX %f SlopeY %f\n",bendingSlope ,nonBendingSlope);

	fInverseBendingMomentum = trackParam->GetInverseBendingMomentum();
	fXRec  = trackParam->GetNonBendingCoor();
	fYRec  = trackParam->GetBendingCoor();
	//      printf(" X %f Y %f\n", fXRec, fYRec);

	fZRec  = trackParam->GetZ();
	//      printf(" Z %f\n", fZRec);

	ntrackhits = rectrack->GetNTrackHits();
	fitfmin = rectrack->GetFitFMin();
	matchTrigger     = rectrack->GetMatchTrigger();
	chi2MatchTrigger = rectrack->GetChi2MatchTrigger();

	// setting data member of ESD MUON
	ESDTrack->SetInverseBendingMomentum(fInverseBendingMomentum);
	ESDTrack->SetThetaX(TMath::ATan(nonBendingSlope));
	ESDTrack->SetThetaY(TMath::ATan(bendingSlope));
	ESDTrack->SetZ(fZRec);
	ESDTrack->SetBendingCoor(fYRec);
	ESDTrack->SetNonBendingCoor(fXRec);
	ESDTrack->SetChi2(fitfmin);
	ESDTrack->SetNHit(ntrackhits);
	ESDTrack->SetMatchTrigger(matchTrigger);
	ESDTrack->SetChi2MatchTrigger(chi2MatchTrigger);

      // storing ESD MUON Track into ESD Event & reset muondata
      event->AddMuonTrack(ESDTrack);
    }// track loop
 
    event->SetTrigger(trigPat);

    for (Int_t iTrack = 0; iTrack < event->GetNumberOfMuonTracks(); iTrack++) {
      IlcESDMuonTrack* muonTrack = event->GetMuonTrack(iTrack);
      Double_t ptInv = TMath::Abs(muonTrack->GetInverseBendingMomentum());
      cout << "  ptInv: "<<ptInv <<"  nb track: "<< event->GetNumberOfMuonTracks() << endl;
    }
    treeE->Fill();
    event->Reset();
        
    muondata.ResetRecTracks();
    muondata.ResetRecTriggerTracks();

  } // end loop on event
  treeE->GetUserInfo()->Add(event);
  ef->Write();
  ef->Close();
  MUONLoader->UnloadTracks();
  delete event;

}











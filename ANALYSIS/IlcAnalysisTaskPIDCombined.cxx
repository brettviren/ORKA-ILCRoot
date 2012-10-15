/*************************************************************************
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

///////////////////////////////////////////////////////////////////////////
//                                                                       //
//                        Basic Analysis Task                            //
//                      for PID        Analysis                          //
//                                                                       //
///////////////////////////////////////////////////////////////////////////

#include <TChain.h>
#include <TH1D.h>
#include <TH2D.h>

#include <IlcVEvent.h>
#include <IlcInputEventHandler.h>
#include <IlcAnalysisManager.h>

#include <IlcLog.h>
#include <IlcPID.h>
#include <IlcPIDCombined.h>
#include <IlcPIDResponse.h>

#include "IlcAnalysisTaskPIDCombined.h"

const char *IlcAnalysisTaskPIDCombined::fgkBinMomDesc[IlcAnalysisTaskPIDCombined::kPtBins] = {
  " 0 <= p < 0.5 GeV/c",
  " 0.5 <= p < 0.7 GeV/c",
  " 0.7 <= p < 1.0 GeV/c",
  " 1.0 <= p < 1.5 GeV/c",
  " 1.5 <= p < 2.0 GeV/c",
  " p >= 2.0 GeV/c"
};

ClassImp(IlcAnalysisTaskPIDCombined)

//_________________________________________________________________________________
IlcAnalysisTaskPIDCombined::IlcAnalysisTaskPIDCombined() :
  IlcAnalysisTaskSE(),
  fHistList(),
  fProbTPCnSigma(),
  fProbTOFnSigma(),
  fProbTPCTOFnSigmaTPC(),
  fProbTPC(),
  fProbTOF(),
  fProbTPCTOF(),
  fPriors(),
  fProbTPCTOFnSigTPCMom(),
  fProbTPCnSigTPCMom(),
  fProbTOFnSigTOFMom(),
  fPriorsUsed(),
  fPIDResponse(0x0),
  fPIDCombined(0x0),
  fTrackCuts(0x0),
  fTrackFilter(0x0)
{
  //
  // Constructor
  //
}

//_________________________________________________________________________________
IlcAnalysisTaskPIDCombined::IlcAnalysisTaskPIDCombined(const char *name) :
  IlcAnalysisTaskSE(name),
  fHistList(),
  fProbTPCnSigma(),
  fProbTOFnSigma(),
  fProbTPCTOFnSigmaTPC(),
  fProbTPC(),
  fProbTOF(),
  fProbTPCTOF(),
  fPriors(),
  fProbTPCTOFnSigTPCMom(),
  fProbTPCnSigTPCMom(),
  fProbTOFnSigTOFMom(),
  fPriorsUsed(),
  fPIDResponse(0x0),
  fPIDCombined(0x0),
  fTrackCuts(0x0),
  fTrackFilter(0x0)
{
  //
  // Constructor
  //
  DefineInput(0,TChain::Class());
  DefineOutput(1, TList::Class());
}

//_________________________________________________________________________________
void IlcAnalysisTaskPIDCombined::UserCreateOutputObjects()
{
  //
  // Initialise the framework objects
  //


  // ------- track cuts
  fTrackCuts = new IlcESDtrackCuts("fTrackCuts", "Standard");
  fTrackCuts->SetAcceptKinkDaughters(kFALSE);
  fTrackCuts->SetMinNClustersTPC(80);
  fTrackCuts->SetMaxChi2PerClusterTPC(4);
  fTrackCuts->SetMaxDCAToVertexXY(3);
  fTrackCuts->SetMaxDCAToVertexZ(3);
  fTrackCuts->SetRequireTPCRefit(kTRUE);
  fTrackCuts->SetRequireITSRefit(kTRUE);
  fTrackCuts->SetClusterRequirementITS(IlcESDtrackCuts::kSPD,IlcESDtrackCuts::kAny);
  fTrackFilter = new IlcAnalysisFilter("trackFilter");
  fTrackFilter->AddCuts(fTrackCuts);



  // ------- setup PIDCombined
  fPIDCombined=new IlcPIDCombined;
  fPIDCombined->SetDefaultTPCPriors();
  fPIDCombined->SetDetectorMask(IlcPIDResponse::kDetTPC+IlcPIDResponse::kDetTOF);

  // no light nuclei
  fPIDCombined->SetSelectedSpecies(IlcPID::kSPECIES);


  fHistList.Add(new TH1D("nEvents","Number of Evnets;Selection",2,0,2));

  for (Int_t ispec=0; ispec<IlcPID::kSPECIES; ++ispec){

 
    fProbTPC[ispec]=new TH2D(Form("prob%s_mom_TPC",IlcPID::ParticleName(ispec)),
                                   Form("%s probability vs. momentum;momentum;probability",IlcPID::ParticleName(ispec)),
                                   100,0.,20.,50,0.,1.);
    fHistList.Add(fProbTPC[ispec]);
    fProbTPCnSigma[ispec]=new TH2D(Form("prob%s_nSigma_TPC",IlcPID::ParticleName(ispec)),
                                   Form("%s probability vs. n#sigma;n#sigma;probability",IlcPID::ParticleName(ispec)),
                                   20,-5.,5.,50,0.,1.);
    fHistList.Add(fProbTPCnSigma[ispec]);

    for (Int_t ibin=0;ibin<kPtBins;ibin++) {
      fProbTPCnSigTPCMom[ibin][ispec]=new TH2D(Form("prob%s_nSigma_TPC (%s)",IlcPID::ParticleName(ispec),fgkBinMomDesc[ibin]),
					       Form("%s probability vs. n#sigma;n#sigma;probability",IlcPID::ParticleName(ispec)),
					       20,-5.,5.,50,0.,1.);
      fHistList.Add(fProbTPCnSigTPCMom[ibin][ispec]);
    }



    fProbTOF[ispec]=new TH2D(Form("prob%s_mom_TOF",IlcPID::ParticleName(ispec)),
                                   Form("%s probability vs. momentum;momentum;probability",IlcPID::ParticleName(ispec)),
                                   100,0.,20.,50,0.,1.);
    fHistList.Add(fProbTOF[ispec]);
    fProbTOFnSigma[ispec]=new TH2D(Form("prob%s_nSigma_TOF",IlcPID::ParticleName(ispec)),
                                   Form("%s probability vs. n#sigma;n#sigma;probability",IlcPID::ParticleName(ispec)),
                                   20,-5.,5.,50,0.,1.);
    fHistList.Add(fProbTOFnSigma[ispec]);
    for (Int_t ibin=0;ibin<kPtBins;ibin++) {
      fProbTOFnSigTOFMom[ibin][ispec]=new TH2D(Form("prob%s_nSigma_TOF (%s)",IlcPID::ParticleName(ispec),fgkBinMomDesc[ibin]),
					       Form("%s probability vs. n#sigma;n#sigma;probability",IlcPID::ParticleName(ispec)),
					       20,-5.,5.,50,0.,1.);
      fHistList.Add(fProbTOFnSigTOFMom[ibin][ispec]);
    }



    fProbTPCTOF[ispec]=new TH2D(Form("prob%s_mom_TPCTOF",IlcPID::ParticleName(ispec)),
                                   Form("%s probability vs. momentum;momentum;probability",IlcPID::ParticleName(ispec)),
                                   100,0.,20.,50,0.,1.);
    fHistList.Add(fProbTPCTOF[ispec]);
    fProbTPCTOFnSigmaTPC[ispec]=new TH2D(Form("probTPCTOF%s_nSigma_TPC",IlcPID::ParticleName(ispec)),
                                   Form("%s TPCTOF probability vs. n#sigmaTPC;n#sigma;probability",IlcPID::ParticleName(ispec)),
                                   20,-5.,5.,50,0.,1.);
    fHistList.Add(fProbTPCTOFnSigmaTPC[ispec]);
    for (Int_t ibin=0;ibin<kPtBins;ibin++) {
      fProbTPCTOFnSigTPCMom[ibin][ispec]=new TH2D(Form("probTPCTOF%s_nSigma_TPC (%s)",IlcPID::ParticleName(ispec),fgkBinMomDesc[ibin]),
					       Form("%s probability vs. n#sigma;n#sigma;probability",IlcPID::ParticleName(ispec)),
					       20,-5.,5.,50,0.,1.);
      fHistList.Add(fProbTPCTOFnSigTPCMom[ibin][ispec]);
    }



    // basic priors
    fPriors[ispec]=new TH1F(Form("%s_priors",IlcPID::ParticleName(ispec)),
			    Form("%s priors vs momentum",IlcPID::ParticleName(ispec)),
			    100,0.,20.);
    fHistList.Add(fPriors[ispec]);
    switch (ispec) {
    case IlcPID::kElectron:
      for (Int_t ich=1;ich<=100;ich++) fPriors[ispec]->SetBinContent(ich,0.02);
      break;
    case IlcPID::kMuon:
      for (Int_t ich=1;ich<=100;ich++) fPriors[ispec]->SetBinContent(ich,0.02);
      break;
    case IlcPID::kPion:
      for (Int_t ich=1;ich<=100;ich++) fPriors[ispec]->SetBinContent(ich,0.56);
      break;
    case IlcPID::kKaon:
      for (Int_t ich=1;ich<=100;ich++) fPriors[ispec]->SetBinContent(ich,0.20);
      break;
    case IlcPID::kProton:
      for (Int_t ich=1;ich<=100;ich++) fPriors[ispec]->SetBinContent(ich,0.20);
      break;
    default:
      break;
    }
    fPIDCombined->SetPriorDistribution((IlcPID::EParticleType)ispec,fPriors[ispec]);

    // priors used
    fPriorsUsed[ispec] = new TH2D(Form("%s_priorsUsed",IlcPID::ParticleName(ispec)),
			    Form("%s priors vs transverse momentum;p_{t} (GeV/c);priors",IlcPID::ParticleName(ispec)),
				  100,0.,20.,101,0,1.01);      
    fHistList.Add(fPriorsUsed[ispec]);
  }



  fHistList.SetOwner();
  PostData(1,&fHistList);


}

//_________________________________________________________________________________
void IlcAnalysisTaskPIDCombined::UserExec(Option_t *)
{
  //
  // Main loop. Called for every event
  //
  IlcAnalysisManager *man=IlcAnalysisManager::GetAnalysisManager();
  IlcInputEventHandler* inputHandler = (IlcInputEventHandler*) (man->GetInputEventHandler());
  fPIDResponse=inputHandler->GetPIDResponse();
  if (!fPIDResponse) IlcFatal("This Task needs the PID response attached to the inputHandler");

  //  Printf(" ---------------------- UserExec PID task ---------------------");
  
  FillHistogram("nEvents",0.);

  IlcVEvent *event=InputEvent();
  IlcVTrack *track=0x0;
  Int_t ntracks=event->GetNumberOfTracks();

  Double_t probTPC[IlcPID::kSPECIES]={0.};
  Double_t probTOF[IlcPID::kSPECIES]={0.};
  Double_t probTPCTOF[IlcPID::kSPECIES]={0.};
  
  //loop over all tracks
  for (Int_t itrack=0; itrack<ntracks; ++itrack){

    track=(IlcVTrack*)event->GetTrack(itrack);

    if ( fTrackFilter->IsSelected(track) ) {

      Double_t mom=track->GetTPCmomentum();
      Int_t ibin=GetMomBin(mom);

      fPIDCombined->SetDetectorMask(IlcPIDResponse::kDetTPC);
      UInt_t detUsed = fPIDCombined->ComputeProbabilities(track, fPIDResponse, probTPC);
     
      if (detUsed != 0) {  // TPC is available
	for (Int_t ispec=0; ispec<IlcPID::kSPECIES; ++ispec) {
	  Double_t nSigmaTPC = fPIDResponse->NumberOfSigmasTPC(track,(IlcPID::EParticleType)ispec);
	  fProbTPC[ispec]->Fill(mom,probTPC[ispec]);
	  fProbTPCnSigma[ispec]->Fill(nSigmaTPC,probTPC[ispec]);
	  fProbTPCnSigTPCMom[ibin][ispec]->Fill(nSigmaTPC,probTPC[ispec]);
	}

	// compute priors for TPC+TOF, even if we ask just TOF for PID
	fPIDCombined->SetDetectorMask(IlcPIDResponse::kDetTOF);
	detUsed = fPIDCombined->ComputeProbabilities(track, fPIDResponse, probTOF);
	Double_t priors[5]; 	// check priors used for TOF
	fPIDCombined->GetPriors(track,priors,fPIDResponse,detUsed);
	for(Int_t ispec=0;ispec<5;ispec++) fPriorsUsed[ispec]->Fill(TMath::Abs(track->Pt()),priors[ispec]);

	if (detUsed != 0) {  // TOF is available
	  for (Int_t ispec=0; ispec<IlcPID::kSPECIES; ++ispec) {
	    Double_t nSigmaTOF = fPIDResponse->NumberOfSigmasTOF(track,(IlcPID::EParticleType)ispec);
	    fProbTOF[ispec]->Fill(mom,probTOF[ispec]);
	    fProbTOFnSigma[ispec]->Fill(nSigmaTOF,probTOF[ispec]);
	    fProbTOFnSigTOFMom[ibin][ispec]->Fill(nSigmaTOF,probTOF[ispec]);
	  }
	}

	fPIDCombined->SetDetectorMask(IlcPIDResponse::kDetTOF|IlcPIDResponse::kDetTPC);
	detUsed = fPIDCombined->ComputeProbabilities(track, fPIDResponse, probTPCTOF);
	if (detUsed == (UInt_t)fPIDCombined->GetDetectorMask() ) {
	  for (Int_t ispec=0; ispec<IlcPID::kSPECIES; ++ispec) {
	    Double_t nSigmaTPC = fPIDResponse->NumberOfSigmasTPC(track,(IlcPID::EParticleType)ispec);
	    fProbTPCTOF[ispec]->Fill(mom,probTPCTOF[ispec]);
	    fProbTPCTOFnSigmaTPC[ispec]->Fill(nSigmaTPC,probTPCTOF[ispec]);
	    fProbTPCTOFnSigTPCMom[ibin][ispec]->Fill(nSigmaTPC,probTPCTOF[ispec]);
	  }
	}

      }


    }
  }

  PostData(1, &fHistList);
}

//_________________________________________________________________________________
void IlcAnalysisTaskPIDCombined::FillHistogram(const char* name, Double_t x, Double_t weight)
{
  //
  // Fill 1D histogram by name
  //
  ((TH1*)fHistList.FindObject(name))->Fill(x,weight);
}

//_________________________________________________________________________________
void IlcAnalysisTaskPIDCombined::FillHistogram(const char* name, Double_t x, Double_t y, Double_t weight)
{
  //
  // Fill 2D histogram by name
  //
  ((TH2*)fHistList.FindObject(name))->Fill(x,y,weight);
}


//_________________________________________________________________________________
Int_t IlcAnalysisTaskPIDCombined::GetMomBin(Float_t mom)
{
  //
  // Given momentum return histogram to be filled
  //
  if (mom>0. && mom < 0.5) return 0;
  if (mom>=0.5 && mom < 0.7) return 1;
  if (mom>=0.7 && mom < 1.0) return 2;
  if (mom>=1.0 && mom < 1.5) return 3;
  if (mom>=1.5 && mom < 2.0) return 4;
  return kPtBins-1;
}


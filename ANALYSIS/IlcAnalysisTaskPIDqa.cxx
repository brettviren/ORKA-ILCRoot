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

/* $Id: IlcAnalysisTaskPIDqa.cxx 43811 2010-09-23 14:13:31Z wiechula $ */
#include <TList.h>
#include <TVectorD.h>
#include <TObjArray.h>
#include <TH2.h>
#include <TFile.h>
#include <TPRegexp.h>
#include <TChain.h>
#include <TF1.h>
#include <TSpline.h>

#include <IlcAnalysisManager.h>
#include <IlcInputEventHandler.h>
#include <IlcVEventHandler.h>
#include <IlcVEvent.h>
#include <IlcVParticle.h>
#include <IlcVTrack.h>
#include <IlcLog.h>
#include <IlcPID.h>
#include <IlcPIDResponse.h>
#include <IlcITSPIDResponse.h>
#include <IlcTPCPIDResponse.h>
#include <IlcTRDPIDResponse.h>
#include <IlcTOFPIDResponse.h>

#include <IlcESDEvent.h>
#include <IlcAODEvent.h>
#include <IlcESDv0.h>
#include <IlcAODv0.h>
#include <IlcESDv0KineCuts.h>

#include "IlcAnalysisTaskPIDqa.h"


ClassImp(IlcAnalysisTaskPIDqa)

//______________________________________________________________________________
IlcAnalysisTaskPIDqa::IlcAnalysisTaskPIDqa():
IlcAnalysisTaskSE(),
fPIDResponse(0x0),
fV0cuts(0x0),
fV0electrons(0x0),
fV0pions(0x0),
fV0kaons(0x0),
fV0protons(0x0),
fListQA(0x0),
fListQAits(0x0),
fListQAitsSA(0x0),
fListQAitsPureSA(0x0),
fListQAtpc(0x0),
fListQAtrd(0x0),
fListQAtof(0x0),
fListQAemcal(0x0),
fListQAhmpid(0x0),
fListQAtofhmpid(0x0),
fListQAtpctof(0x0),
fListQAV0(0x0)
{
  //
  // Dummy constructor
  //
}

//______________________________________________________________________________
IlcAnalysisTaskPIDqa::IlcAnalysisTaskPIDqa(const char* name):
IlcAnalysisTaskSE(name),
fPIDResponse(0x0),
fV0cuts(0x0),
fV0electrons(0x0),
fV0pions(0x0),
fV0kaons(0x0),
fV0protons(0x0),
fListQA(0x0),
fListQAits(0x0),
fListQAitsSA(0x0),
fListQAitsPureSA(0x0),
fListQAtpc(0x0),
fListQAtrd(0x0),
fListQAtof(0x0),
fListQAemcal(0x0),
fListQAhmpid(0x0),
fListQAtofhmpid(0x0),
fListQAtpctof(0x0),
fListQAV0(0x0)
{
  //
  // Default constructor
  //
  DefineInput(0,TChain::Class());
  DefineOutput(1,TList::Class());
}

//______________________________________________________________________________
IlcAnalysisTaskPIDqa::~IlcAnalysisTaskPIDqa()
{
  //
  // Destructor
  //

  delete fV0cuts;
  delete fV0electrons;
  delete fV0pions;
  delete fV0kaons;
  delete fV0protons;

  if (!IlcAnalysisManager::GetAnalysisManager()->IsProofMode()) delete fListQA;
}

//______________________________________________________________________________
void IlcAnalysisTaskPIDqa::UserCreateOutputObjects()
{
  //
  // Create the output QA objects
  //

  IlcLog::SetClassDebugLevel("IlcAnalysisTaskPIDqa",10);

  //input hander
  IlcAnalysisManager *man=IlcAnalysisManager::GetAnalysisManager();
  IlcInputEventHandler *inputHandler=dynamic_cast<IlcInputEventHandler*>(man->GetInputEventHandler());
  if (!inputHandler) IlcFatal("Input handler needed");

  //pid response object
  fPIDResponse=inputHandler->GetPIDResponse();
  if (!fPIDResponse) IlcError("PIDResponse object was not created");
  
  // V0 Kine cuts 
  fV0cuts = new IlcESDv0KineCuts;
 
  // V0 PID Obj arrays
  fV0electrons = new TObjArray;
  fV0pions     = new TObjArray;
  fV0kaons     = new TObjArray;
  fV0protons   = new TObjArray;

  //
  fListQA=new TList;
  fListQA->SetOwner();
  
  fListQAits=new TList;
  fListQAits->SetOwner();
  fListQAits->SetName("ITS");

  fListQAitsSA=new TList;
  fListQAitsSA->SetOwner();
  fListQAitsSA->SetName("ITS_SA");

  fListQAitsPureSA=new TList;
  fListQAitsPureSA->SetOwner();
  fListQAitsPureSA->SetName("ITS_PureSA");

  fListQAtpc=new TList;
  fListQAtpc->SetOwner();
  fListQAtpc->SetName("TPC");
  
  fListQAtrd=new TList;
  fListQAtrd->SetOwner();
  fListQAtrd->SetName("TRD");
  
  fListQAtof=new TList;
  fListQAtof->SetOwner();
  fListQAtof->SetName("TOF");
  
  fListQAemcal=new TList;
  fListQAemcal->SetOwner();
  fListQAemcal->SetName("EMCAL");
  
  fListQAhmpid=new TList;
  fListQAhmpid->SetOwner();
  fListQAhmpid->SetName("HMPID");
  
  fListQAtpctof=new TList;
  fListQAtpctof->SetOwner();
  fListQAtpctof->SetName("TPC_TOF");

  fListQAtofhmpid=new TList;
  fListQAtofhmpid->SetOwner();
  fListQAtofhmpid->SetName("TOF_HMPID");
  
  fListQAV0=new TList;
  fListQAV0->SetOwner();
  fListQAV0->SetName("V0decay");
  
  fListQA->Add(fListQAits);
  fListQA->Add(fListQAitsSA);
  fListQA->Add(fListQAitsPureSA);
  fListQA->Add(fListQAtpc);
  fListQA->Add(fListQAtrd);
  fListQA->Add(fListQAtof);
  fListQA->Add(fListQAemcal);
  fListQA->Add(fListQAhmpid);
  fListQA->Add(fListQAtpctof);
  fListQA->Add(fListQAtofhmpid);
  fListQA->Add(fListQAV0);

  SetupITSqa();
  SetupTPCqa();
  SetupTRDqa();
  SetupTOFqa();
  SetupEMCALqa();
  SetupHMPIDqa();
  SetupTPCTOFqa();
  SetupTOFHMPIDqa();
  SetupV0qa();
  
  PostData(1,fListQA);
}


//______________________________________________________________________________
void IlcAnalysisTaskPIDqa::UserExec(Option_t */*option*/)
{
  //
  // Setup the PID response functions and fill the QA histograms
  //

  IlcVEvent *event=InputEvent();
  if (!event||!fPIDResponse) return;

  // Start with the V0 task (only possible for ESDs?)
  FillV0PIDlist();
  
  FillITSqa();
  FillTPCqa();
  FillTRDqa();
  FillTOFqa();
  FillEMCALqa();
  FillHMPIDqa();
  
  //combined detector QA
  FillTPCTOFqa();
  FillTOFHMPIDqa();
  
  // Clear the V0 PID arrays
  ClearV0PIDlist();


  
  PostData(1,fListQA);
}

//______________________________________________________________________________
void  IlcAnalysisTaskPIDqa::FillV0PIDlist(){

  //
  // Fill the PID object arrays holding the pointers to identified particle tracks
  //

  // Dynamic cast to ESD events (DO NOTHING for AOD events)
  IlcESDEvent *event = dynamic_cast<IlcESDEvent *>(InputEvent());
  if ( !event )  return;
  
  if(TString(event->GetBeamType())=="Pb-Pb" || TString(event->GetBeamType())=="A-A"){
    fV0cuts->SetMode(IlcESDv0KineCuts::kPurity,IlcESDv0KineCuts::kPbPb); 
  }
  else{
    fV0cuts->SetMode(IlcESDv0KineCuts::kPurity,IlcESDv0KineCuts::kPP); 
  }

  // V0 selection
  // set event
  fV0cuts->SetEvent(event);

  // loop over V0 particles
  for(Int_t iv0=0; iv0<event->GetNumberOfV0s();iv0++){

    IlcESDv0 *v0 = (IlcESDv0 *) event->GetV0(iv0);
 
    if(!v0) continue;
    if(v0->GetOnFlyStatus()) continue; 
  
    // Get the particle selection 
    Bool_t foundV0 = kFALSE;
    Int_t pdgV0, pdgP, pdgN;

    foundV0 = fV0cuts->ProcessV0(v0, pdgV0, pdgP, pdgN);
    if(!foundV0) continue;
    
    Int_t iTrackP = v0->GetPindex();  // positive track
    Int_t iTrackN = v0->GetNindex();  // negative track

    // v0 Armenteros plot (QA)
    Float_t armVar[2] = {0.0,0.0};
    fV0cuts->Armenteros(v0, armVar);

    TH2 *h=(TH2*)fListQAV0->At(0);
    if (!h) continue;
    h->Fill(armVar[0],armVar[1]);

    // fill the Object arrays
    // positive particles
    if( pdgP == -11){
      fV0electrons->Add((IlcVTrack*)event->GetTrack(iTrackP));
    }
    else if( pdgP == 211){
      fV0pions->Add((IlcVTrack*)event->GetTrack(iTrackP));
    }
    else if( pdgP == 321){
      fV0kaons->Add((IlcVTrack*)event->GetTrack(iTrackP));
    }
    else if( pdgP == 2212){
      fV0protons->Add((IlcVTrack*)event->GetTrack(iTrackP));
    }

    // negative particles
    if( pdgN == 11){
      fV0electrons->Add((IlcVTrack*)event->GetTrack(iTrackN));
    }
    else if( pdgN == -211){
      fV0pions->Add((IlcVTrack*)event->GetTrack(iTrackN));
    }
    else if( pdgN == -321){
      fV0kaons->Add((IlcVTrack*)event->GetTrack(iTrackN));
    }
    else if( pdgN == -2212){
      fV0protons->Add((IlcVTrack*)event->GetTrack(iTrackN));
    }
  

  }
}
//______________________________________________________________________________
void  IlcAnalysisTaskPIDqa::ClearV0PIDlist(){

  //
  // Clear the PID object arrays
  //

  fV0electrons->Clear();
  fV0pions->Clear();
  fV0kaons->Clear();
  fV0protons->Clear();

}
//______________________________________________________________________________
void IlcAnalysisTaskPIDqa::FillITSqa()
{
  //
  // Fill PID qa histograms for the ITS
  //

  IlcVEvent *event=InputEvent();
  
  Int_t ntracks=event->GetNumberOfTracks();
  for(Int_t itrack = 0; itrack < ntracks; itrack++){
    IlcVTrack *track=(IlcVTrack*)event->GetTrack(itrack);
    ULong_t status=track->GetStatus();
    // not that nice. status bits not in virtual interface
    // ITS refit + ITS pid selection
    if (!( ( (status & IlcVTrack::kITSrefit)==IlcVTrack::kITSrefit ) ||
	   ! ( (status & IlcVTrack::kITSpid  )==IlcVTrack::kITSpid   ) )) continue;
    Double_t mom=track->P();
    
    TList *theList = 0x0;
    if(( (status & IlcVTrack::kTPCin)==IlcVTrack::kTPCin )){
      //ITS+TPC tracks
      theList=fListQAits;
    }else{
      if(!( (status & IlcVTrack::kITSpureSA)==IlcVTrack::kITSpureSA )){ 
	//ITS Standalone tracks
    	theList=fListQAitsSA;
      }else{
	//ITS Pure Standalone tracks
	theList=fListQAitsPureSA;
      }
    }
    
    
    for (Int_t ispecie=0; ispecie<IlcPID::kSPECIES; ++ispecie){
      TH2 *h=(TH2*)theList->At(ispecie);
      if (!h) continue;
      Double_t nSigma=fPIDResponse->NumberOfSigmasITS(track, (IlcPID::EParticleType)ispecie);
      h->Fill(mom,nSigma);
    }
    TH2 *h=(TH2*)theList->At(IlcPID::kSPECIES);
    if (h) {
      Double_t sig=track->GetITSsignal();
      h->Fill(mom,sig);
    }
  }
}

//______________________________________________________________________________
void IlcAnalysisTaskPIDqa::FillTPCqa()
{
  //
  // Fill PID qa histograms for the TPC
  //
  
  IlcVEvent *event=InputEvent();
  
  Int_t ntracks=event->GetNumberOfTracks();
  for(Int_t itrack = 0; itrack < ntracks; itrack++){
    IlcVTrack *track=(IlcVTrack*)event->GetTrack(itrack);
    
    //
    //basic track cuts
    //
    ULong_t status=track->GetStatus();
    // not that nice. status bits not in virtual interface
    // TPC refit + ITS refit + TPC pid
    if (!( (status & IlcVTrack::kTPCrefit) == IlcVTrack::kTPCrefit) ||
        !( (status & IlcVTrack::kITSrefit) == IlcVTrack::kITSrefit) ) continue;

    // The TPC pid cut removes the light nuclei (>5 sigma from proton line)
    //||        !( (status & IlcVTrack::kTPCpid  ) == IlcVTrack::kTPCpid  )
    Float_t nCrossedRowsTPC = track->GetTPCClusterInfo(2,1);
    Float_t  ratioCrossedRowsOverFindableClustersTPC = 1.0;
    if (track->GetTPCNclsF()>0) {
      ratioCrossedRowsOverFindableClustersTPC = nCrossedRowsTPC/track->GetTPCNclsF();
    }
    
    if ( nCrossedRowsTPC<70 || ratioCrossedRowsOverFindableClustersTPC<.8 ) continue;
    
    Double_t mom=track->GetTPCmomentum();
    
    for (Int_t ispecie=0; ispecie<IlcPID::kSPECIES; ++ispecie){
      TH2 *h=(TH2*)fListQAtpc->At(ispecie);
      if (!h) continue;
      Double_t nSigma=fPIDResponse->NumberOfSigmasTPC(track, (IlcPID::EParticleType)ispecie);
      h->Fill(mom,nSigma);
    }
    
    TH2 *h=(TH2*)fListQAtpc->At(IlcPID::kSPECIES);
    if (h) {
      Double_t sig=track->GetTPCsignal();
      h->Fill(mom,sig);
    }
  }
}

//______________________________________________________________________________
void IlcAnalysisTaskPIDqa::FillTRDqa()
{
  //
  // Fill PID qa histograms for the TRD
  //
  IlcVEvent *event=InputEvent();
  Int_t ntracks = event->GetNumberOfTracks();
  for(Int_t itrack = 0; itrack <  ntracks; itrack++){
    IlcVTrack *track = (IlcVTrack *)event->GetTrack(itrack);

    //
    //basic track cuts
    //
    ULong_t status=track->GetStatus();
    // not that nice. status bits not in virtual interface
    // TPC refit + ITS refit + TPC pid + TRD out
    if (!( (status & IlcVTrack::kTPCrefit) == IlcVTrack::kTPCrefit) ||
        !( (status & IlcVTrack::kITSrefit) == IlcVTrack::kITSrefit) ||
//         !( (status & IlcVTrack::kTPCpid  ) == IlcVTrack::kTPCpid  ) || //removes light nuclei. So it is out for the moment
        !( (status & IlcVTrack::kTRDout  ) == IlcVTrack::kTRDout  )) continue;
    
    Float_t nCrossedRowsTPC = track->GetTPCClusterInfo(2,1);
    Float_t  ratioCrossedRowsOverFindableClustersTPC = 1.0;
    if (track->GetTPCNclsF()>0) {
      ratioCrossedRowsOverFindableClustersTPC = nCrossedRowsTPC/track->GetTPCNclsF();
    }
    
    if ( nCrossedRowsTPC<70 || ratioCrossedRowsOverFindableClustersTPC<.8 ) continue;

    Double_t likelihoods[IlcPID::kSPECIES];
    if(fPIDResponse->ComputeTRDProbability(track, IlcPID::kSPECIES, likelihoods) != IlcPIDResponse::kDetPidOk) continue;
    Int_t ntracklets = 0;
    Double_t momentum = -1.;
    for(Int_t itl = 0; itl < 6; itl++)
      if(track->GetTRDmomentum(itl) > 0.){
        ntracklets++;
        if(momentum < 0) momentum = track->GetTRDmomentum(itl);
    } 
    for(Int_t ispecie = 0; ispecie < IlcPID::kSPECIES; ispecie++){
      TH2F *hLike = (TH2F *)fListQAtrd->At(ntracklets*IlcPID::kSPECIES+ispecie);
      if (hLike) hLike->Fill(momentum,likelihoods[ispecie]);
    }
  }
}

//______________________________________________________________________________
void IlcAnalysisTaskPIDqa::FillTOFqa()
{
  //
  // Fill TOF information
  //
  IlcVEvent *event=InputEvent();

  Int_t ntracks=event->GetNumberOfTracks();
  Int_t tracksAtTof = 0;
  for(Int_t itrack = 0; itrack < ntracks; itrack++){
    IlcVTrack *track=(IlcVTrack*)event->GetTrack(itrack);

    //
    //basic track cuts
    //
    ULong_t status=track->GetStatus();
    // TPC refit + ITS refit +
    // TOF out + TOFpid +
    // kTIME
    // (we don't use kTOFmismatch because it depends on TPC....)
    if (!((status & IlcVTrack::kTPCrefit) == IlcVTrack::kTPCrefit) ||
        !((status & IlcVTrack::kITSrefit) == IlcVTrack::kITSrefit) ||
        !((status & IlcVTrack::kTOFout  ) == IlcVTrack::kTOFout  ) ||
        !((status & IlcVTrack::kTOFpid  ) == IlcVTrack::kTOFpid  ) ||
        !((status & IlcVTrack::kTIME    ) == IlcVTrack::kTIME    ) ) continue;

    Float_t nCrossedRowsTPC = track->GetTPCClusterInfo(2,1);
    Float_t  ratioCrossedRowsOverFindableClustersTPC = 1.0;
    if (track->GetTPCNclsF()>0) {
      ratioCrossedRowsOverFindableClustersTPC = nCrossedRowsTPC/track->GetTPCNclsF();
    }

    if ( nCrossedRowsTPC<70 || ratioCrossedRowsOverFindableClustersTPC<.8 ) continue;

    tracksAtTof++;

    Double_t mom=track->P();

    for (Int_t ispecie=0; ispecie<IlcPID::kSPECIES; ++ispecie){
      TH2 *h=(TH2*)fListQAtof->At(ispecie);
      if (!h) continue;
      Double_t nSigma=fPIDResponse->NumberOfSigmasTOF(track, (IlcPID::EParticleType)ispecie);
      h->Fill(mom,nSigma);
    }

    TH2 *h=(TH2*)fListQAtof->FindObject("hSigP_TOF");
    if (h) {
      Double_t sig=track->GetTOFsignal()/1000.;
      h->Fill(mom,sig);
    }

    Int_t mask = fPIDResponse->GetTOFResponse().GetStartTimeMask(mom);
    ((TH1F*)fListQAtof->FindObject("hStartTimeMask_TOF"))->Fill((Double_t)(mask+0.5));

    if (mom >= 0.75 && mom <= 1.25 ) {
      Double_t nsigma= fPIDResponse->NumberOfSigmasTOF(track, (IlcPID::EParticleType)IlcPID::kPion);
      if (mask == 0) {
	((TH1F*)fListQAtof->FindObject("hNsigma_TOF_Pion_T0-Fill"))->Fill(nsigma);
      } else if (mask == 1) {
	((TH1F*)fListQAtof->FindObject("hNsigma_TOF_Pion_T0-TOF"))->Fill(nsigma);
      } else if ( (mask == 2) || (mask == 4) || (mask == 6) ) {
	((TH1F*)fListQAtof->FindObject("hNsigma_TOF_Pion_T0-T0"))->Fill(nsigma);
      } else {
	((TH1F*)fListQAtof->FindObject("hNsigma_TOF_Pion_T0-Best"))->Fill(nsigma);
      }
    }

    Double_t res = (Double_t)fPIDResponse->GetTOFResponse().GetStartTimeRes(mom);
    ((TH1F*)fListQAtof->FindObject("hStartTimeRes_TOF"))->Fill(res);

    IlcESDEvent *esd = dynamic_cast<IlcESDEvent *>(event);
    if (esd) {
      Double_t startTime = esd->GetT0TOF(0);
      if (startTime < 90000) ((TH1F*)fListQAtof->FindObject("hStartTimeAC_T0"))->Fill(startTime);
      else {
        startTime = esd->GetT0TOF(1);
        if (startTime < 90000) ((TH1F*)fListQAtof->FindObject("hStartTimeA_T0"))->Fill(startTime);
        startTime = esd->GetT0TOF(2);
        if (startTime < 90000) ((TH1F*)fListQAtof->FindObject("hStartTimeC_T0"))->Fill(startTime);
      }
    }
  }
  if (tracksAtTof > 0) {
    ((TH1F* )fListQAtof->FindObject("hnTracksAt_TOF"))->Fill(tracksAtTof);
    Int_t mask = fPIDResponse->GetTOFResponse().GetStartTimeMask(5.);
    if (mask & 0x1) ((TH1F*)fListQAtof->FindObject("hT0MakerEff"))->Fill(tracksAtTof);
  }
}


//______________________________________________________________________________
void IlcAnalysisTaskPIDqa::FillEMCALqa()
{
  //
  // Fill PID qa histograms for the EMCAL
  //

  IlcVEvent *event=InputEvent();
  
  Int_t ntracks=event->GetNumberOfTracks();
  for(Int_t itrack = 0; itrack < ntracks; itrack++){
    IlcVTrack *track=(IlcVTrack*)event->GetTrack(itrack);
    
    //
    //basic track cuts
    //
    ULong_t status=track->GetStatus();
    // not that nice. status bits not in virtual interface
    if (!( (status & IlcVTrack::kEMCALmatch) == IlcVTrack::kEMCALmatch) ) continue;

    Double_t pt=track->Pt();
   
    //EMCAL nSigma (only for electrons at the moment)
    TH2 *h=(TH2*)fListQAemcal->At(0);
    if (!h) continue;
    Double_t nSigma=fPIDResponse->NumberOfSigmasEMCAL(track, (IlcPID::EParticleType)0);
    h->Fill(pt,nSigma);
    
  }

   //EMCAL signal (E/p vs. pT) for electrons from V0
  for(Int_t itrack = 0; itrack < fV0electrons->GetEntries(); itrack++){
    IlcVTrack *track=(IlcVTrack*)fV0electrons->At(itrack);

    //
    //basic track cuts
    //
    ULong_t status=track->GetStatus();
    // not that nice. status bits not in virtual interface
    if (!( (status & IlcVTrack::kEMCALmatch) == IlcVTrack::kEMCALmatch) ) continue;

    Double_t pt=track->Pt();

    TH2 *h=(TH2*)fListQAemcal->At(1);
    if (h) {

      Int_t nMatchClus = track->GetEMCALcluster();
      Double_t mom     = track->P();
      Double_t eop     = -1.;

      if(nMatchClus > -1){
    
        IlcVCluster *matchedClus = (IlcVCluster*)event->GetCaloCluster(nMatchClus);

        if(matchedClus){

          // matched cluster is EMCAL
          if(matchedClus->IsEMCAL()){

            Double_t fClsE       = matchedClus->E();
            eop                  = fClsE/mom;

            h->Fill(pt,eop);

          }
        }
      }
    }
  }

   //EMCAL signal (E/p vs. pT) for pions from V0
  for(Int_t itrack = 0; itrack < fV0pions->GetEntries(); itrack++){
    IlcVTrack *track=(IlcVTrack*)fV0pions->At(itrack);

    //
    //basic track cuts
    //
    ULong_t status=track->GetStatus();
    // not that nice. status bits not in virtual interface
    if (!( (status & IlcVTrack::kEMCALmatch) == IlcVTrack::kEMCALmatch) ) continue;

    Double_t pt=track->Pt();

    TH2 *h=(TH2*)fListQAemcal->At(2);
    if (h) {

      Int_t nMatchClus = track->GetEMCALcluster();
      Double_t mom     = track->P();
      Double_t eop     = -1.;

      if(nMatchClus > -1){
    
        IlcVCluster *matchedClus = (IlcVCluster*)event->GetCaloCluster(nMatchClus);

        if(matchedClus){

          // matched cluster is EMCAL
          if(matchedClus->IsEMCAL()){

            Double_t fClsE       = matchedClus->E();
            eop                  = fClsE/mom;

            h->Fill(pt,eop);

          }
        }
      }
    }
  }

   //EMCAL signal (E/p vs. pT) for protons from V0
  for(Int_t itrack = 0; itrack < fV0protons->GetEntries(); itrack++){
    IlcVTrack *track=(IlcVTrack*)fV0protons->At(itrack);

    //
    //basic track cuts
    //
    ULong_t status=track->GetStatus();
    // not that nice. status bits not in virtual interface
    if (!( (status & IlcVTrack::kEMCALmatch) == IlcVTrack::kEMCALmatch) ) continue;

    Double_t pt=track->Pt();

    TH2 *hP=(TH2*)fListQAemcal->At(3);
    TH2 *hAP=(TH2*)fListQAemcal->At(4);
    if (hP && hAP) {

      Int_t nMatchClus = track->GetEMCALcluster();
      Double_t mom     = track->P();
      Int_t charge     = track->Charge();	      
      Double_t eop     = -1.;

      if(nMatchClus > -1){
    
        IlcVCluster *matchedClus = (IlcVCluster*)event->GetCaloCluster(nMatchClus);

        if(matchedClus){

          // matched cluster is EMCAL
          if(matchedClus->IsEMCAL()){

            Double_t fClsE       = matchedClus->E();
            eop                  = fClsE/mom;

            if(charge > 0)      hP->Fill(pt,eop);
            else if(charge < 0) hAP->Fill(pt,eop);

          }
        }
      }
    }
  }

}


//______________________________________________________________________________
void IlcAnalysisTaskPIDqa::FillHMPIDqa()
{
  //
  // Fill PID qa histograms for the HMPID
  //
  
  IlcVEvent *event=InputEvent();
  
  Int_t ntracks=event->GetNumberOfTracks();
  for(Int_t itrack = 0; itrack < ntracks; itrack++){
    IlcVTrack *track=(IlcVTrack*)event->GetTrack(itrack);
    
    //
    //basic track cuts
    //
    ULong_t status=track->GetStatus();
    // not that nice. status bits not in virtual interface
    // TPC refit + ITS refit +
    // TOF out + TOFpid +
    // kTIME
    if (!((status & IlcVTrack::kTPCrefit) == IlcVTrack::kTPCrefit) ||
        !((status & IlcVTrack::kITSrefit) == IlcVTrack::kITSrefit) ) continue;

    Float_t nCrossedRowsTPC = track->GetTPCClusterInfo(2,1);
    Float_t  ratioCrossedRowsOverFindableClustersTPC = 1.0;
    if (track->GetTPCNclsF()>0) {
      ratioCrossedRowsOverFindableClustersTPC = nCrossedRowsTPC/track->GetTPCNclsF();
    }

    if ( nCrossedRowsTPC<70 || ratioCrossedRowsOverFindableClustersTPC<.8 ) continue;
    
    Double_t mom = track->P();
    Double_t ckovAngle = track->GetHMPIDsignal();
    
    TH1F *hThetavsMom = (TH1F*)fListQAhmpid->At(0);;
    
    hThetavsMom->Fill(mom,ckovAngle);    
  
  }
}
//______________________________________________________________________________
void IlcAnalysisTaskPIDqa::FillTOFHMPIDqa()
{
  //
  // Fill PID qa histograms for the HMPID
  //
  
  IlcVEvent *event=InputEvent();
  
  Int_t ntracks=event->GetNumberOfTracks();
  for(Int_t itrack = 0; itrack < ntracks; itrack++){
    IlcVTrack *track=(IlcVTrack*)event->GetTrack(itrack);
    
    //
    //basic track cuts
    //
    ULong_t status=track->GetStatus();
    // not that nice. status bits not in virtual interface
    // TPC refit + ITS refit +
    // TOF out + TOFpid +
    // kTIME
    if (!((status & IlcVTrack::kTPCrefit) == IlcVTrack::kTPCrefit) ||
        !((status & IlcVTrack::kITSrefit) == IlcVTrack::kITSrefit) ||
        !((status & IlcVTrack::kTOFout  ) == IlcVTrack::kTOFout  ) ||
        !((status & IlcVTrack::kTOFpid  ) == IlcVTrack::kTOFpid  ) ||
        !((status & IlcVTrack::kTIME    ) == IlcVTrack::kTIME    ) ) continue;

    Float_t nCrossedRowsTPC = track->GetTPCClusterInfo(2,1);
    Float_t  ratioCrossedRowsOverFindableClustersTPC = 1.0;
    if (track->GetTPCNclsF()>0) {
      ratioCrossedRowsOverFindableClustersTPC = nCrossedRowsTPC/track->GetTPCNclsF();
    }

    if ( nCrossedRowsTPC<70 || ratioCrossedRowsOverFindableClustersTPC<.8 ) continue;
    
    Double_t mom = track->P();
    Double_t ckovAngle = track->GetHMPIDsignal();
    
    Double_t nSigmaTOF[3]; 
    TH1F *h[3];
    
    for (Int_t ispecie=2; ispecie<5; ++ispecie){
      //TOF nSigma
      nSigmaTOF[ispecie-2]=fPIDResponse->NumberOfSigmasTOF(track, (IlcPID::EParticleType)ispecie);
      h[ispecie-2] = (TH1F*)fListQAtofhmpid->At(ispecie-2);}
      
    if(TMath::Abs(nSigmaTOF[0])<2)                                                              h[0]->Fill(mom,ckovAngle);
    
    if(TMath::Abs(nSigmaTOF[1])<2 && TMath::Abs(nSigmaTOF[0])>3)                                h[1]->Fill(mom,ckovAngle);

    if(TMath::Abs(nSigmaTOF[2])<2 && TMath::Abs(nSigmaTOF[1])>3 && TMath::Abs(nSigmaTOF[0])>3)  h[2]->Fill(mom,ckovAngle);
      
  }
  
}

//______________________________________________________________________________
void IlcAnalysisTaskPIDqa::FillTPCTOFqa()
{
  //
  // Fill PID qa histograms for the TOF
  //   Here also the TPC histograms after TOF selection are filled
  //

  IlcVEvent *event=InputEvent();

  Int_t ntracks=event->GetNumberOfTracks();
  for(Int_t itrack = 0; itrack < ntracks; itrack++){
    IlcVTrack *track=(IlcVTrack*)event->GetTrack(itrack);

    //
    //basic track cuts
    //
    ULong_t status=track->GetStatus();
    // not that nice. status bits not in virtual interface
    // TPC refit + ITS refit +
    // TOF out + TOFpid +
    // kTIME
    if (!((status & IlcVTrack::kTPCrefit) == IlcVTrack::kTPCrefit) ||
        !((status & IlcVTrack::kITSrefit) == IlcVTrack::kITSrefit) ||
//         !( (status & IlcVTrack::kTPCpid  ) == IlcVTrack::kTPCpid ) || //removes light nuclei, so it is out for the moment
        !((status & IlcVTrack::kTOFout  ) == IlcVTrack::kTOFout  ) ||
        !((status & IlcVTrack::kTOFpid  ) == IlcVTrack::kTOFpid  ) ||
        !((status & IlcVTrack::kTIME    ) == IlcVTrack::kTIME    ) ) continue;

    Float_t nCrossedRowsTPC = track->GetTPCClusterInfo(2,1);
    Float_t  ratioCrossedRowsOverFindableClustersTPC = 1.0;
    if (track->GetTPCNclsF()>0) {
      ratioCrossedRowsOverFindableClustersTPC = nCrossedRowsTPC/track->GetTPCNclsF();
    }

    if ( nCrossedRowsTPC<70 || ratioCrossedRowsOverFindableClustersTPC<.8 ) continue;


    Double_t mom=track->P();
    Double_t momTPC=track->GetTPCmomentum();

    for (Int_t ispecie=0; ispecie<IlcPID::kSPECIES; ++ispecie){
      //TOF nSigma
      Double_t nSigmaTOF=fPIDResponse->NumberOfSigmasTOF(track, (IlcPID::EParticleType)ispecie);
      Double_t nSigmaTPC=fPIDResponse->NumberOfSigmasTPC(track, (IlcPID::EParticleType)ispecie);

      //TPC after TOF cut
      TH2 *h=(TH2*)fListQAtpctof->At(ispecie);
      if (h && TMath::Abs(nSigmaTOF)<3.) h->Fill(momTPC,nSigmaTPC);

      //TOF after TPC cut
      h=(TH2*)fListQAtpctof->At(ispecie+IlcPID::kSPECIES);
      if (h && TMath::Abs(nSigmaTPC)<3.) h->Fill(mom,nSigmaTOF);

      //EMCAL after TOF and TPC cut
      h=(TH2*)fListQAtpctof->At(ispecie+2*IlcPID::kSPECIES);
      if (h && TMath::Abs(nSigmaTOF)<3. && TMath::Abs(nSigmaTPC)<3. ){

	Int_t nMatchClus = track->GetEMCALcluster();
	Double_t pt      = track->Pt();
	Double_t eop     = -1.;
	
	if(nMatchClus > -1){
	  
	  IlcVCluster *matchedClus = (IlcVCluster*)event->GetCaloCluster(nMatchClus);
	  
	  if(matchedClus){
	    
	    // matched cluster is EMCAL
	    if(matchedClus->IsEMCAL()){
	      
	      Double_t fClsE       = matchedClus->E();
	      eop                  = fClsE/mom;

	      h->Fill(pt,eop);
 
	      
	    }
	  }
	}
      }
    }
  }
}

//______________________________________________________________________________
void IlcAnalysisTaskPIDqa::SetupITSqa()
{
  //
  // Create the ITS qa objects
  //
  
  TVectorD *vX=MakeLogBinning(200,.1,30);
  
  //ITS+TPC tracks
  for (Int_t ispecie=0; ispecie<IlcPID::kSPECIES; ++ispecie){
    TH2F *hNsigmaP = new TH2F(Form("hNsigmaP_ITS_%s",IlcPID::ParticleName(ispecie)),
                              Form("ITS n#sigma %s vs. p;p [GeV]; n#sigma",IlcPID::ParticleName(ispecie)),
                              vX->GetNrows()-1,vX->GetMatrixArray(),
                              200,-10,10);
    fListQAits->Add(hNsigmaP);
  }
  TH2F *hSig = new TH2F("hSigP_ITS",
                        "ITS signal vs. p;p [GeV]; ITS signal [arb. units]",
                        vX->GetNrows()-1,vX->GetMatrixArray(),
                        300,0,300);
  fListQAits->Add(hSig);

  //ITS Standalone tracks
  for (Int_t ispecie=0; ispecie<IlcPID::kSPECIES; ++ispecie){
    TH2F *hNsigmaPSA = new TH2F(Form("hNsigmaP_ITSSA_%s",IlcPID::ParticleName(ispecie)),
				Form("ITS n#sigma %s vs. p;p [GeV]; n#sigma",IlcPID::ParticleName(ispecie)),
				vX->GetNrows()-1,vX->GetMatrixArray(),
				200,-10,10);
    fListQAitsSA->Add(hNsigmaPSA);
  }
  TH2F *hSigSA = new TH2F("hSigP_ITSSA",
			  "ITS signal vs. p;p [GeV]; ITS signal [arb. units]",
			  vX->GetNrows()-1,vX->GetMatrixArray(),
			  300,0,300);
  fListQAitsSA->Add(hSigSA);
  
  //ITS Pure Standalone tracks
  for (Int_t ispecie=0; ispecie<IlcPID::kSPECIES; ++ispecie){
    TH2F *hNsigmaPPureSA = new TH2F(Form("hNsigmaP_ITSPureSA_%s",IlcPID::ParticleName(ispecie)),
				    Form("ITS n#sigma %s vs. p;p [GeV]; n#sigma",IlcPID::ParticleName(ispecie)),
				    vX->GetNrows()-1,vX->GetMatrixArray(),
				    200,-10,10);
    fListQAitsPureSA->Add(hNsigmaPPureSA);
  }
  TH2F *hSigPureSA = new TH2F("hSigP_ITSPureSA",
			      "ITS signal vs. p;p [GeV]; ITS signal [arb. units]",
			      vX->GetNrows()-1,vX->GetMatrixArray(),
			      300,0,300);
  fListQAitsPureSA->Add(hSigPureSA);
  
  delete vX;  
}

//______________________________________________________________________________
void IlcAnalysisTaskPIDqa::SetupTPCqa()
{
  //
  // Create the TPC qa objects
  //
  
  TVectorD *vX=MakeLogBinning(200,.1,30);
  
  for (Int_t ispecie=0; ispecie<IlcPID::kSPECIES; ++ispecie){
    TH2F *hNsigmaP = new TH2F(Form("hNsigmaP_TPC_%s",IlcPID::ParticleName(ispecie)),
                              Form("TPC n#sigma %s vs. p;p [GeV]; n#sigma",IlcPID::ParticleName(ispecie)),
                              vX->GetNrows()-1,vX->GetMatrixArray(),
                              200,-10,10);
    fListQAtpc->Add(hNsigmaP);
  }
  
  
  TH2F *hSig = new TH2F("hSigP_TPC",
                        "TPC signal vs. p;p [GeV]; TPC signal [arb. units]",
                        vX->GetNrows()-1,vX->GetMatrixArray(),
                        300,0,300);
  fListQAtpc->Add(hSig);

  delete vX;  
}

//______________________________________________________________________________
void IlcAnalysisTaskPIDqa::SetupTRDqa()
{
  //
  // Create the TRD qa objects
  //
  TVectorD *vX=MakeLogBinning(200,.1,30);
  for(Int_t itl = 0; itl < 6; ++itl){
    for(Int_t ispecie = 0; ispecie < IlcPID::kSPECIES; ispecie++){
      TH2F *hLikeP = new TH2F(Form("hLikeP_TRD_%dtls_%s", itl, IlcPID::ParticleName(ispecie)),
                              Form("TRD Likelihood to be %s %s for tracks having %d %s; p (GeV/c); TRD %s Likelihood", ispecie == 0 ? "an" : "a", IlcPID::ParticleName(ispecie), itl+1, itl == 0 ? "tracklet" : "tracklets", IlcPID::ParticleName(ispecie)),
                              vX->GetNrows()-1, vX->GetMatrixArray(),
                              100, 0., 1.);
      fListQAtrd->Add(hLikeP);
    }
  }
  delete vX;
}

//______________________________________________________________________________
void IlcAnalysisTaskPIDqa::SetupTOFqa()
{
  //
  // Create the TOF qa objects
  //
  
  TVectorD *vX=MakeLogBinning(200,.1,30);

  for (Int_t ispecie=0; ispecie<IlcPID::kSPECIES; ++ispecie){
    TH2F *hNsigmaP = new TH2F(Form("hNsigmaP_TOF_%s",IlcPID::ParticleName(ispecie)),
                              Form("TOF n#sigma %s vs. p;p [GeV]; n#sigma",IlcPID::ParticleName(ispecie)),
                              vX->GetNrows()-1,vX->GetMatrixArray(),
                              200,-10,10);
    fListQAtof->Add(hNsigmaP);
  }

  // for Kaons PID we differentiate on Time Zero
  TH1F *hnSigT0Fill = new TH1F("hNsigma_TOF_Pion_T0-Fill","TOF n#sigma (Pion) T0-FILL [0.75-1.25. GeV/c]",200,-10,10);
  fListQAtof->Add(hnSigT0Fill);
  TH1F *hnSigT0T0 = new TH1F("hNsigma_TOF_Pion_T0-T0","TOF n#sigma (Pion) T0-T0 [0.75-1.25 GeV/c]",200,-10,10);
  fListQAtof->Add(hnSigT0T0);
  TH1F *hnSigT0TOF = new TH1F("hNsigma_TOF_Pion_T0-TOF","TOF n#sigma (Pion) T0-TOF [0.75-1.25 GeV/c]",200,-10,10);
  fListQAtof->Add(hnSigT0TOF);
  TH1F *hnSigT0Best = new TH1F("hNsigma_TOF_Pion_T0-Best","TOF n#sigma (Pion) T0-Best [0.75-1.25 GeV/c]",200,-10,10);
  fListQAtof->Add(hnSigT0Best);

  TH2F *hSig = new TH2F("hSigP_TOF",
                        "TOF signal vs. p;p [GeV]; TOF signal [ns]",
                        vX->GetNrows()-1,vX->GetMatrixArray(),
                        300,0,30);

  delete vX;
  
  fListQAtof->Add(hSig);

  TH1F *hStartTimeMaskTOF = new TH1F("hStartTimeMask_TOF","StartTime mask",8,0,8);
  fListQAtof->Add(hStartTimeMaskTOF);
  TH1F *hStartTimeResTOF = new TH1F("hStartTimeRes_TOF","StartTime resolution [ps]",100,0,500);
  fListQAtof->Add(hStartTimeResTOF);

  TH1F *hnTracksAtTOF = new TH1F("hnTracksAt_TOF","Matched tracks at TOF",20,0,20);
  fListQAtof->Add(hnTracksAtTOF);
  TH1F *hT0MakerEff = new TH1F("hT0MakerEff","Events with T0-TOF vs nTracks",20,0,20);
  fListQAtof->Add(hT0MakerEff);

  // this in principle should stay on a T0 PID QA, but are just the data prepared for TOF use
  TH1F *hStartTimeAT0 = new TH1F("hStartTimeA_T0","StartTime from T0A [ps]",1000,-1000,1000);
  fListQAtof->Add(hStartTimeAT0);
  TH1F *hStartTimeCT0 = new TH1F("hStartTimeC_T0","StartTime from T0C [ps]",1000,-1000,1000);
  fListQAtof->Add(hStartTimeCT0);
  TH1F *hStartTimeACT0 = new TH1F("hStartTimeAC_T0","StartTime from T0AC [ps]",1000,-1000,1000);;
  fListQAtof->Add(hStartTimeACT0);
}

//______________________________________________________________________________
void IlcAnalysisTaskPIDqa::SetupEMCALqa()
{
  //
  // Create the EMCAL qa objects
  //

  TVectorD *vX=MakeLogBinning(200,.1,30);
  
  TH2F *hNsigmaPt = new TH2F(Form("hNsigmaPt_EMCAL_%s",IlcPID::ParticleName(0)),
			     Form("EMCAL n#sigma %s vs. p_{T};p_{T} [GeV]; n#sigma",IlcPID::ParticleName(0)),
			     vX->GetNrows()-1,vX->GetMatrixArray(),
			     200,-10,10);
  fListQAemcal->Add(hNsigmaPt);  
  

  TH2F *hSigPtEle = new TH2F("hSigPt_EMCAL_Ele",
                        "EMCAL signal (E/p) vs. p_{T} for electrons;p_{T} [GeV]; EMCAL signal (E/p) [arb. units]",
                        vX->GetNrows()-1,vX->GetMatrixArray(),
                        200,0,2);
  fListQAemcal->Add(hSigPtEle);

  TH2F *hSigPtPions = new TH2F("hSigPt_EMCAL_Pions",
                        "EMCAL signal (E/p) vs. p_{T} for pions;p_{T} [GeV]; EMCAL signal (E/p) [arb. units]",
                        vX->GetNrows()-1,vX->GetMatrixArray(),
                        200,0,2);
  fListQAemcal->Add(hSigPtPions);

  TH2F *hSigPtProtons = new TH2F("hSigPt_EMCAL_Protons",
                        "EMCAL signal (E/p) vs. p_{T} for protons;p_{T} [GeV]; EMCAL signal (E/p) [arb. units]",
                        vX->GetNrows()-1,vX->GetMatrixArray(),
                        200,0,2);
  fListQAemcal->Add(hSigPtProtons);

  TH2F *hSigPtAntiProtons = new TH2F("hSigPt_EMCAL_Antiprotons",
                        "EMCAL signal (E/p) vs. p_{T} for antiprotons;p_{T} [GeV]; EMCAL signal (E/p) [arb. units]",
                        vX->GetNrows()-1,vX->GetMatrixArray(),
                        200,0,2);
  fListQAemcal->Add(hSigPtAntiProtons);

  delete vX;  
}

//______________________________________________________________________________
void IlcAnalysisTaskPIDqa::SetupHMPIDqa()
{
  //
  // Create the HMPID qa objects
  //
  
  TH2F *hCkovAnglevsMom   = new TH2F("hCkovAnglevsMom",  "Cherenkov angle vs momnetum",500,0,5.,500,0,1);
  fListQAhmpid->Add(hCkovAnglevsMom);
  
}

//______________________________________________________________________________
void IlcAnalysisTaskPIDqa::SetupTOFHMPIDqa()
{
  //
  // Create the HMPID qa objects
  //
  
  TH2F *hCkovAnglevsMomPion   = new TH2F("hCkovAnglevsMom_pion",  "Cherenkov angle vs momnetum for pions",500,0,5.,500,0,1);
  fListQAtofhmpid->Add(hCkovAnglevsMomPion);
  
  TH2F *hCkovAnglevsMomKaon   = new TH2F("hCkovAnglevsMom_kaon",  "Cherenkov angle vs momnetum for kaons",500,0,5.,500,0,1);
  fListQAtofhmpid->Add(hCkovAnglevsMomKaon);
  
  TH2F *hCkovAnglevsMomProton = new TH2F("hCkovAnglevsMom_proton","Cherenkov angle vs momnetum for protons",500,0,5.,500,0,1);
  fListQAtofhmpid->Add(hCkovAnglevsMomProton);
  
  
}  

//______________________________________________________________________________
void IlcAnalysisTaskPIDqa::SetupTPCTOFqa()
{
  //
  // Create the qa objects for TPC + TOF combination
  //
  
  TVectorD *vX=MakeLogBinning(200,.1,30);

  //TPC signals after TOF cut
  for (Int_t ispecie=0; ispecie<IlcPID::kSPECIES; ++ispecie){
    TH2F *hNsigmaP = new TH2F(Form("hNsigmaP_TPC_TOF_%s",IlcPID::ParticleName(ispecie)),
                              Form("TPC n#sigma %s vs. p (after TOF 3#sigma cut);p_{TPC} [GeV]; n#sigma",IlcPID::ParticleName(ispecie)),
                              vX->GetNrows()-1,vX->GetMatrixArray(),
                              200,-10,10);
    fListQAtpctof->Add(hNsigmaP);
  }

  //TOF signals after TPC cut
  for (Int_t ispecie=0; ispecie<IlcPID::kSPECIES; ++ispecie){
    TH2F *hNsigmaP = new TH2F(Form("hNsigmaP_TOF_TPC_%s",IlcPID::ParticleName(ispecie)),
                              Form("TOF n#sigma %s vs. p (after TPC n#sigma cut);p [GeV]; n#sigma",IlcPID::ParticleName(ispecie)),
                              vX->GetNrows()-1,vX->GetMatrixArray(),
                              200,-10,10);
    fListQAtpctof->Add(hNsigmaP);
  }

  //EMCAL signal after TOF and TPC cut
  for (Int_t ispecie=0; ispecie<IlcPID::kSPECIES; ++ispecie){
    TH2F *heopPt = new TH2F(Form("heopPt_TOF_TPC_%s",IlcPID::ParticleName(ispecie)),
			    Form("EMCAL signal (E/p) %s vs. p_{T};p_{T} [GeV]; EMCAL signal (E/p) [arb. units]",IlcPID::ParticleName(ispecie)),
			    vX->GetNrows()-1,vX->GetMatrixArray(),
			    200,0,2);
    fListQAtpctof->Add(heopPt);
  }

  delete vX;
}
//______________________________________________________________________________
void IlcAnalysisTaskPIDqa::SetupV0qa()
{
  //
  // Create the qa objects for V0 Kine cuts
  //
  
  TH2F *hArmenteros  = new TH2F("hArmenteros",  "Armenteros plot",200,-1.,1.,200,0.,0.4);
  fListQAV0->Add(hArmenteros);
 
}

//______________________________________________________________________________
TVectorD* IlcAnalysisTaskPIDqa::MakeLogBinning(Int_t nbinsX, Double_t xmin, Double_t xmax)
{
  //
  // Make logarithmic binning
  // the user has to delete the array afterwards!!!
  //
  
  //check limits
  if (xmin<1e-20 || xmax<1e-20){
    IlcError("For Log binning xmin and xmax must be > 1e-20. Using linear binning instead!");
    return MakeLinBinning(nbinsX, xmin, xmax);
  }
  if (xmax<xmin){
    Double_t tmp=xmin;
    xmin=xmax;
    xmax=tmp;
  }
  TVectorD *binLim=new TVectorD(nbinsX+1);
  Double_t first=xmin;
  Double_t last=xmax;
  Double_t expMax=TMath::Log(last/first);
  for (Int_t i=0; i<nbinsX+1; ++i){
    (*binLim)[i]=first*TMath::Exp(expMax/nbinsX*(Double_t)i);
  }
  return binLim;
}

//______________________________________________________________________________
TVectorD* IlcAnalysisTaskPIDqa::MakeLinBinning(Int_t nbinsX, Double_t xmin, Double_t xmax)
{
  //
  // Make linear binning
  // the user has to delete the array afterwards!!!
  //
  if (xmax<xmin){
    Double_t tmp=xmin;
    xmin=xmax;
    xmax=tmp;
  }
  TVectorD *binLim=new TVectorD(nbinsX+1);
  Double_t first=xmin;
  Double_t last=xmax;
  Double_t binWidth=(last-first)/nbinsX;
  for (Int_t i=0; i<nbinsX+1; ++i){
    (*binLim)[i]=first+binWidth*(Double_t)i;
  }
  return binLim;
}

//_____________________________________________________________________________
TVectorD* IlcAnalysisTaskPIDqa::MakeArbitraryBinning(const char* bins)
{
  //
  // Make arbitrary binning, bins separated by a ','
  //
  TString limits(bins);
  if (limits.IsNull()){
    IlcError("Bin Limit string is empty, cannot add the variable");
    return 0x0;
  }
  
  TObjArray *arr=limits.Tokenize(",");
  Int_t nLimits=arr->GetEntries();
  if (nLimits<2){
    IlcError("Need at leas 2 bin limits, cannot add the variable");
    delete arr;
    return 0x0;
  }
  
  TVectorD *binLimits=new TVectorD(nLimits);
  for (Int_t iLim=0; iLim<nLimits; ++iLim){
    (*binLimits)[iLim]=(static_cast<TObjString*>(arr->At(iLim)))->GetString().Atof();
  }
  
  delete arr;
  return binLimits;
}


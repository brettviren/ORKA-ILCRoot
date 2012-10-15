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

//-----------------------------------------------------------------------
// Example of task (running locally, on AliEn and CAF),
// which provides standard way of calculating acceptance and efficiency
// between different steps of the procedure.
// The ouptut of the task is a IlcCFContainer from which the efficiencies
// can be calculated
//-----------------------------------------------------------------------
// Author : R. Vernet, Consorzio Cometa - Catania (it)
//-----------------------------------------------------------------------


#ifndef ILCCFSINGLETRACKTASK_CXX
#define ILCCFSINGLETRACKTASK_CXX

#include "IlcCFSingleTrackTask.h"
#include "TCanvas.h"
#include "IlcStack.h"
#include "TParticle.h"
#include "TH1I.h"
#include "IlcMCEvent.h"
#include "IlcAnalysisManager.h"
#include "IlcESDEvent.h"
#include "IlcAODEvent.h"
#include "IlcCFManager.h"
#include "IlcCFCutBase.h"
#include "IlcCFContainer.h"
#include "TChain.h"
#include "IlcESDtrack.h"
#include "IlcLog.h"

ClassImp(IlcCFSingleTrackTask)

//__________________________________________________________________________
IlcCFSingleTrackTask::IlcCFSingleTrackTask() :
  fReadTPCTracks(0),
  fReadAODData(0),
  fCFManager(0x0),
  fQAHistList(0x0),
  fHistEventsProcessed(0x0)
{
  //
  //Default ctor
  //
}
//___________________________________________________________________________
IlcCFSingleTrackTask::IlcCFSingleTrackTask(const Char_t* name) :
  IlcAnalysisTaskSE(name),
  fReadTPCTracks(0),
  fReadAODData(0),
  fCFManager(0x0),
  fQAHistList(0x0),
  fHistEventsProcessed(0x0)
{
  //
  // Constructor. Initialization of Inputs and Outputs
  //
  Info("IlcCFSingleTrackTask","Calling Constructor");

  /*
    DefineInput(0) and DefineOutput(0)
    are taken care of by IlcAnalysisTaskSE constructor
  */
  DefineOutput(1,TH1I::Class());
  DefineOutput(2,IlcCFContainer::Class());
  DefineOutput(3,TList::Class());
}

//___________________________________________________________________________
IlcCFSingleTrackTask& IlcCFSingleTrackTask::operator=(const IlcCFSingleTrackTask& c) 
{
  //
  // Assignment operator
  //
  if (this!=&c) {
    IlcAnalysisTaskSE::operator=(c) ;
    fReadTPCTracks = c.fReadTPCTracks ;
    fReadAODData = c.fReadAODData ;
    fCFManager  = c.fCFManager;
    fQAHistList = c.fQAHistList ;
    fHistEventsProcessed = c.fHistEventsProcessed;
  }
  return *this;
}

//___________________________________________________________________________
IlcCFSingleTrackTask::IlcCFSingleTrackTask(const IlcCFSingleTrackTask& c) :
  IlcAnalysisTaskSE(c),
  fReadTPCTracks(c.fReadTPCTracks),
  fReadAODData(c.fReadAODData),
  fCFManager(c.fCFManager),
  fQAHistList(c.fQAHistList),
  fHistEventsProcessed(c.fHistEventsProcessed)
{
  //
  // Copy Constructor
  //
}

//___________________________________________________________________________
IlcCFSingleTrackTask::~IlcCFSingleTrackTask() {
  //
  //destructor
  //
  Info("~IlcCFSingleTrackTask","Calling Destructor");
  if (fCFManager)           delete fCFManager ;
  if (fHistEventsProcessed) delete fHistEventsProcessed ;
  if (fQAHistList) {fQAHistList->Clear(); delete fQAHistList;}
}

//_________________________________________________
void IlcCFSingleTrackTask::UserExec(Option_t *)
{
  //
  // Main loop function
  //
  Info("UserExec","") ;

  IlcVEvent*    fEvent = fInputEvent ;
  IlcVParticle* track ;
  
  if (!fEvent) {
    Error("UserExec","NO EVENT FOUND!");
    return;
  }

  if (!fMCEvent) {
    Error("UserExec","NO MC INFO FOUND");
    return ;
  }
  
  //pass the evt info to the cuts that need it 
  fCFManager->SetMCEventInfo (fMCEvent);
  fCFManager->SetRecEventInfo(fEvent);

  // MC-event selection
  Double_t containerInput[2] ;
        
  //loop on the MC event
  for (Int_t ipart=0; ipart<fMCEvent->GetNumberOfTracks(); ipart++) { 
    IlcMCParticle *mcPart  = (IlcMCParticle*)fMCEvent->GetTrack(ipart);

    //check the MC-level cuts
    if (!fCFManager->CheckParticleCuts(IlcCFManager::kPartGenCuts,mcPart)) continue;

    containerInput[0] = (Float_t)mcPart->Pt();
    containerInput[1] = mcPart->Eta() ;
    //fill the container for Gen-level selection
    fCFManager->GetParticleContainer()->Fill(containerInput,kStepGenerated);

    //check the Acceptance-level cuts
    if (!fCFManager->CheckParticleCuts(IlcCFManager::kPartAccCuts,mcPart)) continue;
    //fill the container for Acceptance-level selection
    fCFManager->GetParticleContainer()->Fill(containerInput,kStepReconstructible);
  }    

  //Now go to rec level
  for (Int_t iTrack = 0; iTrack<fEvent->GetNumberOfTracks(); iTrack++) {
    
    track = fEvent->GetTrack(iTrack);
    
    if (fReadTPCTracks) {
      if (fReadAODData) {
	Error("UserExec","TPC-only tracks are not supported with AOD");
	return ;
      }
      IlcESDtrack* esdTrack    = (IlcESDtrack*) track;
      IlcESDtrack* esdTrackTPC = new IlcESDtrack();
      if (!esdTrack->FillTPCOnlyTrack(*esdTrackTPC)) {
	Error("UserExec","Could not retrieve TPC info");
	continue;
      }
      track = esdTrackTPC ;
    }

    if (!fCFManager->CheckParticleCuts(IlcCFManager::kPartRecCuts,track)) continue;
    
    // is track associated to particle ?

    Int_t label = track->GetLabel();

    if (label<0) continue;
    IlcMCParticle *mcPart  = (IlcMCParticle*)fMCEvent->GetTrack(label);
    
    // check if this track was part of the signal
    if (!fCFManager->CheckParticleCuts(IlcCFManager::kPartGenCuts,mcPart)) continue; 
    
    //fill the container
    Double_t mom[3];
    track->PxPyPz(mom);
    Double_t pt=TMath::Sqrt(mom[0]*mom[0]+mom[1]*mom[1]);
    containerInput[0] = pt ;
    containerInput[1] = track->Eta();
    fCFManager->GetParticleContainer()->Fill(containerInput,kStepReconstructed) ;   

    if (!fCFManager->CheckParticleCuts(IlcCFManager::kPartSelCuts,track)) continue ;
    fCFManager->GetParticleContainer()->Fill(containerInput,kStepSelected);

    if (fReadTPCTracks) delete track;
  }
  
  fHistEventsProcessed->Fill(0);

  /* PostData(0) is taken care of by IlcAnalysisTaskSE */
  PostData(1,fHistEventsProcessed) ;
  PostData(2,fCFManager->GetParticleContainer()) ;
  PostData(3,fQAHistList) ;
}


//___________________________________________________________________________
void IlcCFSingleTrackTask::Terminate(Option_t*)
{
  // The Terminate() function is the last function to be called during
  // a query. It always runs on the client, it can be used to present
  // the results graphically or save the results to file.

  Info("Terminate","");
  IlcAnalysisTaskSE::Terminate();

  //draw some example plots....

  IlcCFContainer *cont= dynamic_cast<IlcCFContainer*> (GetOutputData(2));

  TH1D* h00 =   cont->ShowProjection(0,0) ;
  TH1D* h01 =   cont->ShowProjection(0,1) ;
  TH1D* h02 =   cont->ShowProjection(0,2) ;
  TH1D* h03 =   cont->ShowProjection(0,3) ;

  TH1D* h10 =   cont->ShowProjection(1,0) ;
  TH1D* h11 =   cont->ShowProjection(1,1) ;
  TH1D* h12 =   cont->ShowProjection(1,2) ;
  TH1D* h13 =   cont->ShowProjection(1,3) ;

  Double_t max1 = h00->GetMaximum();
  Double_t max2 = h10->GetMaximum();

  h00->GetYaxis()->SetRangeUser(0,max1*1.2);
  h01->GetYaxis()->SetRangeUser(0,max1*1.2);
  h02->GetYaxis()->SetRangeUser(0,max1*1.2);
  h03->GetYaxis()->SetRangeUser(0,max1*1.2);

  h10->GetYaxis()->SetRangeUser(0,max2*1.2);
  h11->GetYaxis()->SetRangeUser(0,max2*1.2);
  h12->GetYaxis()->SetRangeUser(0,max2*1.2);
  h13->GetYaxis()->SetRangeUser(0,max2*1.2);

  h00->SetMarkerStyle(23) ;
  h01->SetMarkerStyle(24) ;
  h02->SetMarkerStyle(25) ;
  h03->SetMarkerStyle(26) ;

  h10->SetMarkerStyle(23) ;
  h11->SetMarkerStyle(24) ;
  h12->SetMarkerStyle(25) ;
  h13->SetMarkerStyle(26) ;

  TCanvas * c =new TCanvas("c","",1400,800);
  c->Divide(4,2);

  c->cd(1);
  h00->Draw("p");
  c->cd(2);
  h01->Draw("p");
  c->cd(3);
  h02->Draw("p");
  c->cd(4);
  h03->Draw("p");
  c->cd(5);
  h10->Draw("p");
  c->cd(6);
  h11->Draw("p");
  c->cd(7);
  h12->Draw("p");
  c->cd(8);
  h13->Draw("p");

  c->SaveAs("plots.eps");
}


//___________________________________________________________________________
void IlcCFSingleTrackTask::UserCreateOutputObjects() {
  //HERE ONE CAN CREATE OUTPUT OBJECTS, IN PARTICULAR IF THE OBJECT PARAMETERS DON'T NEED
  //TO BE SET BEFORE THE EXECUTION OF THE TASK
  //
  Info("CreateOutputObjects","CreateOutputObjects of task %s", GetName());

  //slot #1
  OpenFile(1);
  fHistEventsProcessed = new TH1I("fHistEventsProcessed","",1,0,1) ;

//   OpenFile(2);
//   OpenFile(3);
}

#endif

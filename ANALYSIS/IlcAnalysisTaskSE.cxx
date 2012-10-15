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

/* $Id$ */
 
#include <TROOT.h>
#include <TSystem.h>
#include <TInterpreter.h>
#include <TChain.h>
#include <TFile.h>
#include <TList.h>

#include "IlcAnalysisTaskSE.h"
#include "IlcAnalysisManager.h"
#include "IlcAnalysisCuts.h"
#include "IlcAnalysisDataSlot.h"
#include "IlcAnalysisDataContainer.h"

#include "IlcESDEvent.h"
#include "IlcESDfriend.h"
#include "IlcESD.h"
#include "IlcAODEvent.h"
#include "IlcAODHeader.h"
#include "IlcAODTracklets.h"
#include "IlcAODCaloCells.h"
#include "IlcAODCaloTrigger.h"
#include "IlcAODMCParticle.h"
#include "IlcVEvent.h"
#include "IlcAODHandler.h"
#include "IlcAODInputHandler.h"
#include "IlcMCEventHandler.h"
#include "IlcInputEventHandler.h"
#include "IlcMultiInputEventHandler.h"
#include "IlcESDInputHandler.h"
#include "IlcMCEvent.h"
#include "IlcStack.h"
#include "IlcLog.h"
#include "IlcAODDimuon.h"


ClassImp(IlcAnalysisTaskSE)

////////////////////////////////////////////////////////////////////////
IlcAODHeader*    IlcAnalysisTaskSE::fgAODHeader         = NULL;
TClonesArray*    IlcAnalysisTaskSE::fgAODTracks         = NULL;
TClonesArray*    IlcAnalysisTaskSE::fgAODVertices       = NULL;
TClonesArray*    IlcAnalysisTaskSE::fgAODV0s            = NULL;
TClonesArray*    IlcAnalysisTaskSE::fgAODPMDClusters    = NULL;
TClonesArray*    IlcAnalysisTaskSE::fgAODJets           = NULL;
TClonesArray*    IlcAnalysisTaskSE::fgAODFMDClusters    = NULL;
TClonesArray*    IlcAnalysisTaskSE::fgAODCaloClusters   = NULL;
TClonesArray*    IlcAnalysisTaskSE::fgAODMCParticles    = NULL;
IlcAODTracklets* IlcAnalysisTaskSE::fgAODTracklets      = NULL;
IlcAODCaloCells* IlcAnalysisTaskSE::fgAODEmcalCells     = NULL;
IlcAODCaloCells* IlcAnalysisTaskSE::fgAODPhosCells      = NULL;
IlcAODCaloTrigger* IlcAnalysisTaskSE::fgAODEMCALTrigger = NULL;
IlcAODCaloTrigger* IlcAnalysisTaskSE::fgAODPHOSTrigger  = NULL;
TClonesArray*    IlcAnalysisTaskSE::fgAODDimuons        = NULL;
TClonesArray*    IlcAnalysisTaskSE::fgAODHmpidRings     = NULL;

IlcAnalysisTaskSE::IlcAnalysisTaskSE():
    IlcAnalysisTask(),
    fDebug(0),
    fEntry(0),
    fInputEvent(0x0),
    fESDfriend(0x0),
    fInputHandler(0x0),
    fOutputAOD(0x0),
    fMCEvent(0x0),
    fTreeA(0x0),
    fCurrentRunNumber(-1),
    fHistosQA(0x0),
    fOfflineTriggerMask(0),
    fMultiInputHandler(0),
    fMCEventHandler(0)
{
  // Default constructor
}

IlcAnalysisTaskSE::IlcAnalysisTaskSE(const char* name):
    IlcAnalysisTask(name, "AnalysisTaskSE"),
    fDebug(0),
    fEntry(0),
    fInputEvent(0x0),
    fESDfriend(0x0),
    fInputHandler(0x0),
    fOutputAOD(0x0),
    fMCEvent(0x0),
    fTreeA(0x0),
    fCurrentRunNumber(-1),
    fHistosQA(0x0),
    fOfflineTriggerMask(0),
    fMultiInputHandler(0),
    fMCEventHandler(0)
{
  // Default constructor
    DefineInput (0, TChain::Class());
    DefineOutput(0,  TTree::Class());
}

IlcAnalysisTaskSE::IlcAnalysisTaskSE(const IlcAnalysisTaskSE& obj):
    IlcAnalysisTask(obj),
    fDebug(0),
    fEntry(0),
    fInputEvent(0x0),
    fESDfriend(0x0),
    fInputHandler(0x0),
    fOutputAOD(0x0),
    fMCEvent(0x0),
    fTreeA(0x0),
    fCurrentRunNumber(-1),
    fHistosQA(0x0),
    fOfflineTriggerMask(0),
    fMultiInputHandler(obj.fMultiInputHandler),
    fMCEventHandler(obj.fMCEventHandler)
{
// Copy constructor
    fDebug            = obj.fDebug;
    fEntry            = obj.fEntry;
    fInputEvent       = obj.fInputEvent;
    fESDfriend        = obj.fESDfriend;
    fInputHandler     = obj.fInputHandler;
    fOutputAOD        = obj.fOutputAOD;
    fMCEvent          = obj.fMCEvent;
    fTreeA            = obj.fTreeA;    
    fCurrentRunNumber = obj.fCurrentRunNumber;
    fHistosQA         = obj.fHistosQA;

}


IlcAnalysisTaskSE& IlcAnalysisTaskSE::operator=(const IlcAnalysisTaskSE& other)
{
// Assignment
  if(&other == this) return *this;
  IlcAnalysisTask::operator=(other);

    IlcAnalysisTask::operator=(other);
    fDebug            = other.fDebug;
    fEntry            = other.fEntry;
    fInputEvent       = other.fInputEvent;
    fESDfriend        = other.fESDfriend;
    fInputHandler     = other.fInputHandler;
    fOutputAOD        = other.fOutputAOD;
    fMCEvent          = other.fMCEvent;
    fTreeA            = other.fTreeA;    
    fCurrentRunNumber = other.fCurrentRunNumber;
    fHistosQA         = other.fHistosQA;
    fOfflineTriggerMask = other.fOfflineTriggerMask;
    fMultiInputHandler  = other.fMultiInputHandler;
    fMCEventHandler     = other.fMCEventHandler;
    return *this;
}

//______________________________________________________________________________
void IlcAnalysisTaskSE::ConnectInputData(Option_t* /*option*/)
{
// Connect the input data
    if (fDebug > 1) printf("AnalysisTaskSE::ConnectInputData() \n");

   // Connect input handlers (multi input handler is handled)
    ConnectMultiHandler();
    
    if (fInputHandler) {
	if ((fInputHandler->GetTree())->GetBranch("ESDfriend."))
	    fESDfriend = ((IlcESDInputHandler*)fInputHandler)->GetESDfriend();

	fInputEvent = fInputHandler->GetEvent();
    } else if( fMCEvent ) {
         IlcWarning("No Input Event Handler connected, only MC Truth Event Handler") ; 
    } else {
         IlcError("No Input Event Handler connected") ; 
         return ; 
    }
    // Disconnect multi handler
    DisconnectMultiHandler();
}

void IlcAnalysisTaskSE::CreateOutputObjects()
{
// Create the output container
//
//  Default AOD
    if (fDebug > 1) printf("AnalysisTaskSE::CreateOutPutData() \n");

    IlcAODHandler* handler = dynamic_cast<IlcAODHandler*> 
         ((IlcAnalysisManager::GetAnalysisManager())->GetOutputEventHandler());
    
    Bool_t merging = kFALSE;
    IlcAODInputHandler* aodIH = static_cast<IlcAODInputHandler*>((IlcAnalysisManager::GetAnalysisManager())->GetInputEventHandler());
    if (aodIH) {
	if (aodIH->GetMergeEvents()) merging = kTRUE;
    }

    // Check if AOD replication has been required
    if (handler) {
	fOutputAOD   = handler->GetAOD();
	fTreeA = handler->GetTree();
	if (fOutputAOD && !(handler->IsStandard())) {
	    if ((handler->NeedsHeaderReplication() || merging) && !(fgAODHeader)) 
		{
		 if (fDebug > 1) IlcInfo("Replicating header");
		 fgAODHeader = new IlcAODHeader;
		 handler->AddBranch("IlcAODHeader", &fgAODHeader);
		}
	    if ((handler->NeedsTracksBranchReplication() || merging) && !(fgAODTracks))      
	    {   
		if (fDebug > 1) IlcInfo("Replicating track branch\n");
		fgAODTracks = new TClonesArray("IlcAODTrack",500);
		fgAODTracks->SetName("tracks");
		handler->AddBranch("TClonesArray", &fgAODTracks);
	    }    
	    if ((handler->NeedsVerticesBranchReplication() || merging) && !(fgAODVertices))
	    {
		if (fDebug > 1) IlcInfo("Replicating vertices branch\n");
		fgAODVertices = new TClonesArray("IlcAODVertex",500);
		fgAODVertices->SetName("vertices");
		handler->AddBranch("TClonesArray", &fgAODVertices);
	    }	
	    if ((handler->NeedsV0sBranchReplication()) && !(fgAODV0s))	  
	    {   
		if (fDebug > 1) IlcInfo("Replicating V0s branch\n");
		fgAODV0s = new TClonesArray("IlcAODv0",500);
		fgAODV0s->SetName("v0s");
		handler->AddBranch("TClonesArray", &fgAODV0s);
	    }
	    if ((handler->NeedsTrackletsBranchReplication()) && !(fgAODTracklets))	  
	    {   
		if (fDebug > 1) IlcInfo("Replicating Tracklets branch\n");
		fgAODTracklets = new IlcAODTracklets("tracklets","tracklets");
		handler->AddBranch("IlcAODTracklets", &fgAODTracklets);
	    }
	    if ((handler->NeedsPMDClustersBranchReplication()) && !(fgAODPMDClusters))	  
	    {   
		if (fDebug > 1) IlcInfo("Replicating PMDClusters branch\n");
		fgAODPMDClusters = new TClonesArray("IlcAODPmdCluster",500);
		fgAODPMDClusters->SetName("pmdClusters");
		handler->AddBranch("TClonesArray", &fgAODPMDClusters);
	    }
	    if ((handler->NeedsJetsBranchReplication() || merging) && !(fgAODJets))	  
	    {   
		if (fDebug > 1) IlcInfo("Replicating Jets branch\n");
		fgAODJets = new TClonesArray("IlcAODJet",500);
		fgAODJets->SetName("jets");
		handler->AddBranch("TClonesArray", &fgAODJets);
	    }
	    if ((handler->NeedsFMDClustersBranchReplication()) && !(fgAODFMDClusters))	  
	    {   
		IlcInfo("Replicating FMDClusters branch\n");
		fgAODFMDClusters = new TClonesArray("IlcAODFmdCluster",500);
		fgAODFMDClusters->SetName("fmdClusters");
		handler->AddBranch("TClonesArray", &fgAODFMDClusters);
	    }
	    if ((handler->NeedsCaloClustersBranchReplication() || merging) && !(fgAODCaloClusters))	  
	    {   
		if (fDebug > 1) IlcInfo("Replicating CaloClusters branch\n");
		fgAODCaloClusters = new TClonesArray("IlcAODCaloCluster",500);
		fgAODCaloClusters->SetName("caloClusters");
		handler->AddBranch("TClonesArray", &fgAODCaloClusters);

		fgAODEmcalCells = new IlcAODCaloCells("emcalCells","emcalCells",IlcVCaloCells::kEMCALCell);
		handler->AddBranch("IlcAODCaloCells", &fgAODEmcalCells);

		fgAODPhosCells = new IlcAODCaloCells("phosCells","phosCells",IlcVCaloCells::kPHOSCell);
		handler->AddBranch("IlcAODCaloCells", &fgAODPhosCells);
		}
	    if ((handler->NeedsCaloTriggerBranchReplication() || merging) && !(fgAODEMCALTrigger))	  
	    {   
		if (fDebug > 1) IlcInfo("Replicating EMCAL Calo Trigger branches\n");
		fgAODEMCALTrigger = new IlcAODCaloTrigger("emcalTrigger","emcalTrigger");
		handler->AddBranch("IlcAODCaloTrigger", &fgAODEMCALTrigger);
		}
		if ((handler->NeedsCaloTriggerBranchReplication() || merging) && !(fgAODPHOSTrigger))	  
		{   
		if (fDebug > 1) IlcInfo("Replicating PHOS Calo Trigger branches\n");
		fgAODPHOSTrigger = new IlcAODCaloTrigger("phosTrigger","phosTrigger");
		handler->AddBranch("IlcAODCaloTrigger", &fgAODPHOSTrigger);
	    }
	    if ((handler->NeedsMCParticlesBranchReplication() || merging) && !(fgAODMCParticles))	  
	    {   
		if (fDebug > 1) IlcInfo("Replicating MCParticles branch\n");
		fgAODMCParticles = new TClonesArray("IlcAODMCParticle",500);
		fgAODMCParticles->SetName("mcparticles");
		handler->AddBranch("TClonesArray", &fgAODMCParticles);
	    }
            if ((handler->NeedsDimuonsBranchReplication() || merging) && !(fgAODDimuons))      
	    {   
		if (fDebug > 1) IlcInfo("Replicating dimuon branch\n");
		fgAODDimuons = new TClonesArray("IlcAODDimuon",0);
		fgAODDimuons->SetName("dimuons");
		handler->AddBranch("TClonesArray", &fgAODDimuons);
	    }    
	    if ((handler->NeedsHMPIDBranchReplication() || merging) && !(fgAODHmpidRings))      
	    {   
		if (fDebug > 1) IlcInfo("Replicating HMPID branch\n");
		fgAODHmpidRings = new TClonesArray("IlcAODHMPIDrings",0);
		fgAODHmpidRings->SetName("hmpidRings");
		handler->AddBranch("TClonesArray", &fgAODHmpidRings);
	    }
                

	    // cache the pointerd in the AODEvent
	    fOutputAOD->GetStdContent();
	}
    }
    ConnectMultiHandler();
    UserCreateOutputObjects();
    DisconnectMultiHandler();
}

void IlcAnalysisTaskSE::Exec(Option_t* option)
{
//
// Exec analysis of one event

    ConnectMultiHandler();

    if ( fDebug >= 10)
      printf("Task is active %5d\n", IsActive());
    
    if (fDebug > 1) IlcInfo("IlcAnalysisTaskSE::Exec() \n");
//
    IlcAODHandler* handler = dynamic_cast<IlcAODHandler*> 
	((IlcAnalysisManager::GetAnalysisManager())->GetOutputEventHandler());

    IlcAODInputHandler* aodH = dynamic_cast<IlcAODInputHandler*>(fInputHandler);
//
// Was event selected ? If no event selection mechanism, the event SHOULD be selected (AG)
    UInt_t isSelected = IlcVEvent::kAny;
    if( fInputHandler && (fInputHandler->GetEventSelection() || aodH)) {
      // Get the actual offline trigger mask for the event and AND it with the
      // requested mask. If no mask requested select by default the event.
      if (fOfflineTriggerMask)
         isSelected = fOfflineTriggerMask & fInputHandler->IsEventSelected();
    }
//  Functionality below moved in the filter tasks (AG)
//    if (handler) handler->SetFillAOD(isSelected);

    if( fInputHandler ) {
	fEntry = fInputHandler->GetReadEntry();
	fESDfriend = ((IlcESDInputHandler*)fInputHandler)->GetESDfriend();
    }
    

// Notify the change of run number
    if (InputEvent() && (InputEvent()->GetRunNumber() != fCurrentRunNumber)) {
	fCurrentRunNumber = InputEvent()->GetRunNumber();
	NotifyRun();
    }    
	   
    else if( fMCEvent )
       fEntry = fMCEvent->Header()->GetEvent(); 
    if ( !((Entry()-1)%100) && fDebug > 0) 
         IlcInfo(Form("%s ----> Processing event # %lld", CurrentFileName(), Entry()));

    
    

    if (handler && aodH) {
	fMCEvent = aodH->MCEvent();
	Bool_t merging = aodH->GetMergeEvents();
      
  // Do not analyze merged events if last embedded file has less events than normal event, 
  // skip analysis after last embeded event 
  if(merging){
    if(aodH->GetReadEntry() + aodH->GetMergeOffset() >= aodH->GetTreeToMerge()->GetEntriesFast()){
      //printf("Skip Entry %lld, Offset %d, Tree Entries %d\n",aodH->GetReadEntry(),aodH->GetMergeOffset(), aodH->GetTreeToMerge()->GetEntries());
          
      // Do I need to add the lines before the return?
      // Added protection in case the derived task is not an AOD producer.
      IlcAnalysisDataSlot *out0 = GetOutputSlot(0);
      if (out0 && out0->IsConnected()) PostData(0, fTreeA);    
          
      DisconnectMultiHandler();
          
      return;
    }
    //else   printf("MERGE Entry %lld, Offset %d, Tree Entries %d\n",aodH->GetReadEntry(),aodH->GetMergeOffset(), aodH->GetTreeToMerge()->GetEntries());
  }
      
	IlcAODEvent* aod = dynamic_cast<IlcAODEvent*>(InputEvent());

	if (aod && !(handler->IsStandard()) && !(handler->AODIsReplicated())) {
	    if ((handler->NeedsHeaderReplication() || merging) && (fgAODHeader))
	    {
	      // copy the contents by assigment
	      *fgAODHeader =  *(aod->GetHeader());
	    }
	    if ((handler->NeedsTracksBranchReplication() || (merging &&  aodH->GetMergeTracks())) && (fgAODTracks))
	    {
		TClonesArray* tracks = aod->GetTracks();
		new (fgAODTracks) TClonesArray(*tracks);
	    }
	    if ((handler->NeedsVerticesBranchReplication() || merging) && (fgAODVertices))
	    {
		TClonesArray* vertices = aod->GetVertices();
		new (fgAODVertices) TClonesArray(*vertices);
	    }
	    if ((handler->NeedsV0sBranchReplication()) && (fgAODV0s))
	    {
		TClonesArray* v0s = aod->GetV0s();
		new (fgAODV0s) TClonesArray(*v0s);
	    }
	    if ((handler->NeedsTrackletsBranchReplication()) && (fgAODTracklets))
	    {
	      *fgAODTracklets = *aod->GetTracklets();
	    }
	    if ((handler->NeedsPMDClustersBranchReplication()) && (fgAODPMDClusters))
	    {
		TClonesArray* pmdClusters = aod->GetPmdClusters();
		new (fgAODPMDClusters) TClonesArray(*pmdClusters);
	    }
	    if ((handler->NeedsJetsBranchReplication() || (merging &&aodH->GetMergeTracks())) && (fgAODJets))
	    {
		TClonesArray* jets = aod->GetJets();
		new (fgAODJets) TClonesArray(*jets);
	    }
	    if ((handler->NeedsFMDClustersBranchReplication()) && (fgAODFMDClusters))
	    {
		TClonesArray* fmdClusters = aod->GetFmdClusters();
		new (fgAODFMDClusters) TClonesArray(*fmdClusters);
	    }
	    if ((handler->NeedsCaloClustersBranchReplication() || 
		 (merging && (aodH->GetMergeEMCALClusters() || aodH->GetMergePHOSClusters()))) 
		&& (fgAODCaloClusters))
	    {
		TClonesArray* caloClusters = aod->GetCaloClusters();
		new (fgAODCaloClusters) TClonesArray(*caloClusters);
	    }

	    if ((handler->NeedsMCParticlesBranchReplication() || merging) && (fgAODMCParticles))
	    {
		TClonesArray* mcParticles = (TClonesArray*) (aod->FindListObject("mcparticles"));
		if( mcParticles )
		  new (fgAODMCParticles) TClonesArray(*mcParticles);
	    }
	    
	    if ((handler->NeedsDimuonsBranchReplication() || (merging && aodH->GetMergeTracks())) && (fgAODDimuons))
	    {
	        fgAODDimuons->Clear();
		TClonesArray& dimuons = *fgAODDimuons;
		TClonesArray& tracksnew = *fgAODTracks;
		
                Int_t nMuonTrack[100]; 
                for(Int_t imuon = 0; imuon < 100; imuon++) nMuonTrack[imuon] = 0;
                Int_t nMuons=0;
		for(Int_t ii=0; ii < fgAODTracks->GetEntries(); ii++){
		    IlcAODTrack *track = (IlcAODTrack*) fgAODTracks->At(ii);
		    if(track->IsMuonTrack()) {
			nMuonTrack[nMuons]= ii;
			nMuons++;
		    }  
		}
                Int_t jDimuons=0;
		if(nMuons >= 2){
		    for(Int_t i = 0; i < nMuons; i++){
			Int_t index0 = nMuonTrack[i];
			for(Int_t j = i+1; j < nMuons; j++){
			    Int_t index1 = nMuonTrack[j];
			    tracksnew.At(index0)->ResetBit(kIsReferenced);
			    tracksnew.At(index0)->SetUniqueID(0); 
			    tracksnew.At(index1)->ResetBit(kIsReferenced);
			    tracksnew.At(index1)->SetUniqueID(0);
			    new(dimuons[jDimuons++]) IlcAODDimuon(tracksnew.At(index0),tracksnew.At(index1));
			}
		    }    
		}
	    }
            if ((handler->NeedsHMPIDBranchReplication()) && (fgAODHmpidRings))
	    {
		TClonesArray* hmpidRings = aod->GetHMPIDrings();
		new (fgAODHmpidRings) TClonesArray(*hmpidRings);
	    }
            
            
            
	    // Additional merging if needed
	    if (merging) {
	      Int_t nc;

		// mcParticles
		TClonesArray* mcparticles = (TClonesArray*) ((aodH->GetEventToMerge())->FindListObject("mcparticles"));
		if( mcparticles ){
		  Int_t npart = mcparticles->GetEntries();
		  nc = fgAODMCParticles->GetEntries();
		  
		  for (Int_t i = 0; i < npart; i++) {
		    IlcAODMCParticle* particle = (IlcAODMCParticle*) mcparticles->At(i);
		    new((*fgAODMCParticles)[nc++]) IlcAODMCParticle(*particle);
		  }
		}

		// tracks
		TClonesArray* tracks = aodH->GetEventToMerge()->GetTracks();
		if(tracks && aodH->GetMergeTracks()){
		  Int_t ntr = tracks->GetEntries();
		  nc  = fgAODTracks->GetEntries();	
		  for (Int_t i = 0; i < ntr; i++) {
		    IlcAODTrack*    track = (IlcAODTrack*) tracks->At(i);
		    IlcAODTrack* newtrack = new((*fgAODTracks)[nc++]) IlcAODTrack(*track);
		    newtrack->SetLabel(newtrack->GetLabel() + fgAODMCParticles->GetEntries());
		  }
		  
		  for (Int_t i = 0; i < nc; i++) 
		    {
		      IlcAODTrack* track = (IlcAODTrack*) fgAODTracks->At(i);
		      track->ResetBit(kIsReferenced);
		      track->SetUniqueID(0);
		    }
		}
		
		// clusters
		TClonesArray* clusters = aodH->GetEventToMerge()->GetCaloClusters();
		if( clusters  && (aodH->GetMergeEMCALClusters() || aodH->GetMergePHOSClusters())) {
		  Int_t ncl  = clusters->GetEntries();
		  nc         =  fgAODCaloClusters->GetEntries();
		  for (Int_t i = 0; i < ncl; i++) {
		    IlcAODCaloCluster*    cluster = (IlcAODCaloCluster*) clusters->At(i);
		    if(cluster->IsEMCAL() && !aodH->GetMergeEMCALClusters() ) continue;
		    if(cluster->IsPHOS()  && !aodH->GetMergePHOSClusters()  ) continue;   
		    new((*fgAODCaloClusters)[nc++]) IlcAODCaloCluster(*cluster);
		  }
		}

		// EMCAL cells
		//*fgAODEmcalCells =  *(aod->GetEMCALCells()); // This will be valid after 10.Mar.2011.
		if(aodH->GetMergeEMCALCells()) 
		  {
		    IlcAODCaloCells* copycells = aod->GetEMCALCells();
		    fgAODEmcalCells->CreateContainer(copycells->GetNumberOfCells());
		    nc  = copycells->GetNumberOfCells();
		    
		    while( nc-- ){ fgAODEmcalCells->SetCell(nc,copycells->GetCellNumber(nc),copycells->GetAmplitude(nc),
							    copycells->GetTime(nc),copycells->GetMCLabel(nc),copycells->GetEFraction(nc)); }
		    
		    IlcAODCaloCells* cellsA = aodH->GetEventToMerge()->GetEMCALCells();
		    if( cellsA )
		      {
			Int_t ncells  = cellsA->GetNumberOfCells();
			nc = fgAODEmcalCells->GetNumberOfCells();
			
			for (Int_t i  = 0; i < ncells; i++) 
			  {
			    Int_t cn  = cellsA->GetCellNumber(i);
			    Int_t pos = fgAODEmcalCells->GetCellPosition(cn);
			    
			    if (pos >= 0) 
			      {
				Double_t amp = cellsA->GetAmplitude(i) + fgAODEmcalCells->GetAmplitude(pos);
				
				//Check if it is MC, depending on that assing the mc lable, time and e fraction
				Double_t time    = 0;
				Int_t    mclabel =-1;
				Int_t    efrac   = 0;
				if(cellsA->GetMCLabel(i) >= 0 && fgAODEmcalCells->GetMCLabel(i) < 0)
				  {
				    mclabel = cellsA->GetMCLabel(i) ;
				    time    = fgAODEmcalCells->GetTime(i) ; // Time from data
				    if(amp > 0) efrac = cellsA->GetAmplitude(i) / amp;
				  }
				else if(fgAODEmcalCells->GetMCLabel(i) >= 0 &&  cellsA->GetMCLabel(i) < 0)
				  {
				    mclabel = fgAODEmcalCells->GetMCLabel(i) ;
				    time    = cellsA->GetTime(i) ; // Time from data
				    if(amp > 0) efrac = fgAODEmcalCells->GetAmplitude(i) / amp;              
				  }
				else 
				  { // take all from input
				    mclabel = cellsA->GetMCLabel(i) ;
				    time    = cellsA->GetTime(i) ; 
				    if(amp > 0) efrac = cellsA->GetAmplitude(i) / amp;  
				  }
				
				fgAODEmcalCells->SetCell(pos, cn, amp,cellsA->GetTime(i),mclabel,efrac);
				
			      } else 
			      {
				IlcAODCaloCells* copycells1 = new IlcAODCaloCells(*fgAODEmcalCells);
				fgAODEmcalCells->CreateContainer(nc+1);
				Int_t nn = copycells1->GetNumberOfCells();
				
				while( nn-- ){ fgAODEmcalCells->SetCell(nn,copycells1->GetCellNumber(nn),copycells1->GetAmplitude(nn),
									copycells1->GetTime(nn),copycells1->GetMCLabel(nn),0.); }
				
				fgAODEmcalCells->SetCell(nc++,cn,cellsA->GetAmplitude(i),cellsA->GetTime(i), cellsA->GetMCLabel(i),0.);
				
				delete copycells1;
			      }
			  }
			fgAODEmcalCells->Sort();
		      }
		  } // merge emcal cells
		
		
		// PHOS cells
		//*fgAODPhosCells =  *(aod->GetPHOSCells()); // This will be valid after 10.Mar.2011.
		if(aodH->GetMergePHOSCells()) 
		  {
		    IlcAODCaloCells* copycells = aod->GetPHOSCells();
		    fgAODPhosCells->CreateContainer(copycells->GetNumberOfCells());
		    nc  = copycells->GetNumberOfCells();
		    
		    while( nc-- ){ fgAODPhosCells->SetCell(nc,copycells->GetCellNumber(nc),copycells->GetAmplitude(nc),
							   copycells->GetTime(nc),copycells->GetMCLabel(nc),copycells->GetEFraction(nc)); }
		    
		    IlcAODCaloCells* cellsP = aodH->GetEventToMerge()->GetPHOSCells();
		    if( cellsP )
		      {
			Int_t ncellsP  = cellsP->GetNumberOfCells();
			nc = fgAODPhosCells->GetNumberOfCells();
			
			for (Int_t i  = 0; i < ncellsP; i++) 
			  {
			    Int_t cn  = cellsP->GetCellNumber(i);
			    Int_t pos = fgAODPhosCells->GetCellPosition(cn);
			    
			    if (pos >= 0) 
			      {
				Double_t amp = cellsP->GetAmplitude(i) + fgAODPhosCells->GetAmplitude(pos);
				
				//Check if it is MC, depending on that assing the mc lable, time and e fraction
				Double_t time    = 0;
				Int_t    mclabel =-1;
				Int_t    efrac   = 0;
				if(cellsP->GetMCLabel(i) >= 0 && fgAODPhosCells->GetMCLabel(i) < 0)
				  {
				    mclabel = cellsP->GetMCLabel(i) ;
				    time    = fgAODPhosCells->GetTime(i) ; // Time from data
				    if(amp > 0) efrac = cellsP->GetAmplitude(i) / amp;
				  }
				else if(fgAODPhosCells->GetMCLabel(i) >= 0 &&  cellsP->GetMCLabel(i) < 0)
				  {
				    mclabel = fgAODPhosCells->GetMCLabel(i) ;
				    time    = cellsP->GetTime(i) ; // Time from data
				    if(amp > 0) efrac = fgAODPhosCells->GetAmplitude(i) / amp;              
				  }
				else 
				  { // take all from input
				    mclabel = cellsP->GetMCLabel(i) ;
				    time    = cellsP->GetTime(i) ; 
				    if(amp > 0) efrac = cellsP->GetAmplitude(i) / amp;  
				  }
				
				fgAODPhosCells->SetCell(pos, cn, amp,cellsP->GetTime(i),mclabel,efrac);                
				
			      } else 
			      {
				IlcAODCaloCells* copycells1 = new IlcAODCaloCells(*fgAODPhosCells);
				fgAODPhosCells->CreateContainer(nc+1);
				Int_t nn = copycells1->GetNumberOfCells();
				
				while( nn-- ){ fgAODPhosCells->SetCell(nn,copycells1->GetCellNumber(nn),copycells1->GetAmplitude(nn), 
								       copycells1->GetTime(nn),copycells1->GetMCLabel(nn),0.); }
				
				fgAODPhosCells->SetCell(nc++,cn,cellsP->GetAmplitude(i),cellsP->GetTime(i), cellsP->GetMCLabel(i),0.);
				
				delete copycells1;
			      }
			  }
			fgAODPhosCells->Sort();
		      }
		  } // Merge PHOS Cells
		
		if (aodH->GetMergeEMCALTrigger() && aod->GetCaloTrigger("EMCAL")) 
		{
			Int_t   tsEMCAL[48][64], px, py, ts;
			Float_t foEMCAL[48][64], am;
			
			for (Int_t i = 0; i < 48; i++) for (Int_t j = 0; j < 64; j++) 
			{
				tsEMCAL[i][j] = 0;
				foEMCAL[i][j] = 0.;
			}
      
      IlcAODCaloTrigger& trg0 = *(aod->GetCaloTrigger("EMCAL"));
      trg0.Reset();
      while (trg0.Next())
      {
        trg0.GetPosition(px, py);
        
        if (px > -1 && py > -1) 
        {
          trg0.GetL1TimeSum(ts);
          if (ts > -1) tsEMCAL[px][py] += ts;
          
          trg0.GetAmplitude(am);
          if (am > -1) foEMCAL[px][py] += am;
        }
      }
      
      IlcAODCaloTrigger& trg1 = *((aodH->GetEventToMerge())->GetCaloTrigger("EMCAL"));
      
      trg1.Reset();
      while (trg1.Next())
      {
        trg1.GetPosition(px, py);
        
        if (px > -1 && py > -1) 
        {
          trg1.GetL1TimeSum(ts);
          if (ts > -1) tsEMCAL[px][py] += ts;
          
          trg1.GetAmplitude(am);
          if (am > -1) foEMCAL[px][py] += am;
        }
      }
      
      int nEntries = 0;
      for (Int_t i = 0; i < 48; i++) 
        for (Int_t j = 0; j < 64; j++) 
          if (tsEMCAL[i][j] || foEMCAL[i][j]) nEntries++;
      
      fgAODEMCALTrigger->Allocate(nEntries);
      Int_t timesL0[10]; for (int i = 0; i < 10; i++) timesL0[i] = -1;
      
      for (Int_t i = 0; i < 48; i++) 
        for (Int_t j = 0; j < 64; j++) 
          if (tsEMCAL[i][j] || foEMCAL[i][j]) 
            fgAODEMCALTrigger->Add(i, j, foEMCAL[i][j], -1., timesL0, 0, tsEMCAL[i][j], 0);
    }
		
		if (aodH->GetMergePHOSTrigger()) 
		{
			// To be implemented by PHOS
		}
	    } // merging
	    
	    handler->SetAODIsReplicated();
	}
    }


// Call the user analysis    
    if (!fMCEventHandler) {
	if (isSelected) 
	    UserExec(option);
    } else {
	if (isSelected && (fMCEventHandler->InitOk())) 
	    UserExec(option);
    }
    
// Added protection in case the derived task is not an AOD producer.
    IlcAnalysisDataSlot *out0 = GetOutputSlot(0);
    if (out0 && out0->IsConnected()) PostData(0, fTreeA);    

    DisconnectMultiHandler();
}

const char* IlcAnalysisTaskSE::CurrentFileName()
{
// Returns the current file name    
    if( fInputHandler )
      return fInputHandler->GetTree()->GetCurrentFile()->GetName();
    else if( fMCEvent )
      return ((IlcMCEventHandler*) ((IlcAnalysisManager::GetAnalysisManager())->GetMCtruthEventHandler()))->TreeK()->GetCurrentFile()->GetName();
    else return "";
}

void IlcAnalysisTaskSE::AddAODBranch(const char* cname, void* addobj, const char *fname)
{
    // Add a new branch to the aod tree
    IlcAODHandler* handler = dynamic_cast<IlcAODHandler*> 
	((IlcAnalysisManager::GetAnalysisManager())->GetOutputEventHandler());
    if (handler) {
	handler->AddBranch(cname, addobj, fname);
    }
}

Bool_t IlcAnalysisTaskSE::IsStandardAOD() const
{
// Check if the output AOD handler is configured for standard or delta AOD.
// Users should first check that AODEvent() returns non-null.
    IlcAODHandler* handler = dynamic_cast<IlcAODHandler*> 
         ((IlcAnalysisManager::GetAnalysisManager())->GetOutputEventHandler());
    if (!handler) {
       Error("IsStandardAOD", "No AOD handler. Please use AODEvent() to check this first");
       return kTRUE;
    }
    return handler->IsStandard();   
}

Bool_t IlcAnalysisTaskSE::Notify()
{
    return (UserNotify());
}

const IlcEventTag *IlcAnalysisTaskSE::EventTag() const
{
// Returns tag for the current event, if any. The return value should always be checked by the user.
   if (!fInputHandler) {
      Error("EventTag", "Input handler not yet available. Call this in UserExec");
      return NULL;
   }
   return fInputHandler->GetEventTag();
}

void IlcAnalysisTaskSE::LoadBranches() const
{
// Load all branches declared in fBranchNames data member of the parent class.
// Should be called in UserExec.
  if (!fInputHandler) {
     Error("LoadBranches", "Input handler not available yet. Call this in UserExec");
     return;
  }
  IlcAnalysisManager *mgr = IlcAnalysisManager::GetAnalysisManager();
  if (mgr->GetAutoBranchLoading()) return;
  TString taskbranches;
  GetBranches(fInputHandler->GetDataType(), taskbranches);
  if (taskbranches.IsNull()) return;
  TObjArray *arr = taskbranches.Tokenize(",");
  TIter next(arr);
  TObject *obj;
  while ((obj=next())) mgr->LoadBranch(obj->GetName());
}


//_________________________________________________________________________________________________
void IlcAnalysisTaskSE::ConnectMultiHandler()
{
   //
   // Connect MultiHandler
   //
   fInputHandler = (IlcInputEventHandler *)((IlcAnalysisManager::GetAnalysisManager())->GetInputEventHandler());
   fMultiInputHandler = dynamic_cast<IlcMultiInputEventHandler *>(fInputHandler);
   if (fMultiInputHandler) {
      fInputHandler = dynamic_cast<IlcInputEventHandler *>(fMultiInputHandler->GetFirstInputEventHandler());
      fMCEventHandler = dynamic_cast<IlcMCEventHandler *>(fMultiInputHandler->GetFirstMCEventHandler());
   } else { 
      fMCEventHandler = dynamic_cast<IlcMCEventHandler *>((IlcAnalysisManager::GetAnalysisManager())->GetMCtruthEventHandler());
   }
   if (fMCEventHandler) fMCEvent = fMCEventHandler->MCEvent();
}

//_________________________________________________________________________________________________
void IlcAnalysisTaskSE::DisconnectMultiHandler()
{
   //
   // Disconnect MultiHandler
   //
   if (fMultiInputHandler) fInputHandler = fMultiInputHandler;
}

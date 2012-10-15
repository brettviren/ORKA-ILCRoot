#if !defined(__CINT__) || defined(__MAKECINT__)
#include <TChain.h>
#include <TSystem.h>
#include "IlcAnalysisManager.h"
#include "IlcESDInputHandler.h"
#include "IlcAODHandler.h"
#include "IlcAnalysisTaskESDfilter.h"
#include "IlcAnalysisDataContainer.h"
#endif

void CreateAODfromESD(const char *inFileName = "IlcESDs.root",
		      const char *outFileName = "IlcAOD.root",
		      Bool_t bKineFilter = kTRUE) 
{
  
    gSystem->Load("libTree");
    gSystem->Load("libGeom");
    gSystem->Load("libPhysics");
    gSystem->Load("libVMC");
    gSystem->Load("libSTEERBase");
    gSystem->Load("libESD");
    gSystem->Load("libAOD");
    
    gSystem->Load("libANALYSIS");
    gSystem->Load("libANALYSISilc");
    gSystem->Load("libCORRFW");
    gSystem->Load("libPWGHFbase");
    gSystem->Load("libPWGmuon");

    TChain *chain = new TChain("esdTree");
    // Steering input chain
    chain->Add(inFileName);
    IlcAnalysisManager *mgr  = new IlcAnalysisManager("ESD to AOD", "Analysis Manager");

    // Input
    IlcESDInputHandler* inpHandler = new IlcESDInputHandler();
    inpHandler->SetReadFriends(kFALSE);
    inpHandler->SetReadTags();
    mgr->SetInputEventHandler  (inpHandler);
    // Output
    IlcAODHandler* aodHandler   = new IlcAODHandler();
    aodHandler->SetOutputFileName(outFileName);
    mgr->SetOutputEventHandler(aodHandler);

    // MC Truth
    if(bKineFilter){
	IlcMCEventHandler* mcHandler = new IlcMCEventHandler();
	mgr->SetMCtruthEventHandler(mcHandler);
    }


    // Tasks
    // Filtering of MC particles (decays conversions etc)
    // this task is also needed to set the MCEventHandler
    // to the AODHandler, this will not be needed when
    // AODHandler goes to ANALYSISilc
    IlcAnalysisTaskMCParticleFilter *kinefilter = new IlcAnalysisTaskMCParticleFilter("Particle Filter");
    if (bKineFilter) mgr->AddTask(kinefilter);
    
    // Barrel Tracks
    IlcAnalysisTaskESDfilter *filter = new IlcAnalysisTaskESDfilter("Filter");
    mgr->AddTask(filter);
    // Muons
    IlcAnalysisTaskESDMuonFilter *esdmuonfilter = new IlcAnalysisTaskESDMuonFilter("ESD Muon Filter");
    mgr->AddTask(esdmuonfilter);

    // Cuts on primary tracks
    IlcESDtrackCuts* esdTrackCutsL = new IlcESDtrackCuts("IlcESDtrackCuts", "Standard");
    esdTrackCutsL->SetMinNClustersTPC(50);
    esdTrackCutsL->SetMaxChi2PerClusterTPC(3.5);
    esdTrackCutsL->SetMaxCovDiagonalElements(2, 2, 0.5, 0.5, 2);
    esdTrackCutsL->SetRequireTPCRefit(kTRUE);
    esdTrackCutsL->SetMaxDCAToVertexXY(3.0);
    esdTrackCutsL->SetMaxDCAToVertexZ(3.0);
    esdTrackCutsL->SetDCAToVertex2D(kTRUE);
    esdTrackCutsL->SetRequireSigmaToVertex(kFALSE);
    esdTrackCutsL->SetAcceptKinkDaughters(kFALSE);
    // ITS stand-alone tracks
    IlcESDtrackCuts* esdTrackCutsITSsa = new IlcESDtrackCuts("IlcESDtrackCuts", "ITS stand-alone");
    esdTrackCutsITSsa->SetRequireITSStandAlone(kTRUE);

    IlcAnalysisFilter* trackFilter = new IlcAnalysisFilter("trackFilter");
    trackFilter->AddCuts(esdTrackCutsL);
    trackFilter->AddCuts(esdTrackCutsITSsa);

    // Cuts on V0s
    IlcESDv0Cuts*   esdV0Cuts = new IlcESDv0Cuts("IlcESDv0Cuts", "Standard pp");
    esdV0Cuts->SetMinRadius(0.2);
    esdV0Cuts->SetMaxRadius(200);
    esdV0Cuts->SetMinDcaPosToVertex(0.05);
    esdV0Cuts->SetMinDcaNegToVertex(0.05);
    esdV0Cuts->SetMaxDcaV0Daughters(1.0);
    esdV0Cuts->SetMinCosinePointingAngle(0.99);
    IlcAnalysisFilter* v0Filter = new IlcAnalysisFilter("v0Filter");
    v0Filter->AddCuts(esdV0Cuts);


//
    filter->SetTrackFilter(trackFilter);
    filter->SetV0Filter(v0Filter);


//  Create AOD Tags
    IlcAnalysisTaskTagCreator* tagTask = new IlcAnalysisTaskTagCreator("AOD Tag Creator");
    mgr->AddTask(tagTask);

    // Pipelining
    IlcAnalysisDataContainer *cinput1  = mgr->GetCommonInputContainer();    
    IlcAnalysisDataContainer *coutput1 = mgr->GetCommonOutputContainer();
    
    
    IlcAnalysisDataContainer *coutputT
	= mgr->CreateContainer("cTag",  TTree::Class(), IlcAnalysisManager::kOutputContainer, "AOD.tag.root");

    coutput1->SetSpecialOutput();
    coutputT->SetSpecialOutput();
    
    if(bKineFilter) {
	mgr->ConnectInput  (kinefilter,     0, cinput1  );
	mgr->ConnectOutput (kinefilter,     0, coutput1 );
    }

    mgr->ConnectInput (filter, 0, cinput1 );
    mgr->ConnectOutput(filter, 0, coutput1);

    mgr->ConnectInput (esdmuonfilter, 0, cinput1 );
//    mgr->ConnectOutput(esdmuonfilter, 0, coutput1);

    mgr->ConnectInput (tagTask, 0, cinput1);
    mgr->ConnectOutput(tagTask, 1, coutputT);

    //
    // Run the analysis
    //
    mgr->InitAnalysis();
    mgr->PrintStatus();
    mgr->StartAnalysis("local", chain);
}


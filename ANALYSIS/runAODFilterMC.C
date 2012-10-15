void runAODFilterMC()
{
  // PROOF example

      gSystem->Load("libTree.so");
      gSystem->Load("libPhysics.so");
      gSystem->Load("libGeom.so");
      gSystem->Load("libVMC.so");

      bool bKineFilter = true;
      //      TProof::Mgr("ilccaf")->SetROOTVersion("v5-21-01-ilc_dbg");
      TProof::Open("ilccaf");
      //      gProof->SetParallel(1);

      char *dataset = "/COMMON/COMMON/LHC08c11_10TeV_0.5T";
      //      char *dataset = "/PWG4/kleinb/LHC08q_jetjet100";
      // gProof->ClearPackages();
      gProof->UploadPackage("${ILC_ROOT}/STEERBase.par");
      gProof->EnablePackage("STEERBase");
      gProof->UploadPackage("${ILC_ROOT}/ESD.par");
      gProof->EnablePackage("ESD");
      gProof->UploadPackage("${ILC_ROOT}/AOD.par");
      gProof->EnablePackage("AOD");
      gProof->UploadPackage("${ILC_ROOT}/ANALYSIS.par");
      gProof->EnablePackage("ANALYSIS");
      gProof->UploadPackage("${ILC_ROOT}/ANALYSISilc.par");
      gProof->EnablePackage("ANALYSISilc");
      
      //
      if (gApplication) gApplication->InitializeGraphics();
      // Create the chain
      //


	/////////////////////////////////////////////////////////////////////////////////// 
	// Create the analysis manager
	//
	// Input 
      IlcESDInputHandler* inpHandler = new IlcESDInputHandler();
      // Output
      IlcAODHandler* aodHandler = new IlcAODHandler();
      aodHandler->SetOutputFileName("aod_ckb2.root");
      // MC Truth
      IlcMCEventHandler* mcHandler = new IlcMCEventHandler();
      IlcAnalysisManager *mgr  = new IlcAnalysisManager("Filter Manager", "Filter Manager");
     if(bKineFilter){
       mgr->SetMCtruthEventHandler(mcHandler);
     }

      mgr->SetInputEventHandler  (inpHandler);
      mgr->SetOutputEventHandler (aodHandler);
      aodHandler->Dump();

      mgr->SetDebugLevel(10);

      // Filtering of MC particles (decays conversions etc)
      // this task is also needed to set the MCEventHandler
      // to the AODHandler, this will not be needed when
      // AODHandler goes to ANALYSISilc
      IlcAnalysisTaskMCParticleFilter *kinefilter = new IlcAnalysisTaskMCParticleFilter("Particle Filter");
      if(bKineFilter)mgr->AddTask(kinefilter);
      
      
      // 
      IlcESDtrackCuts* esdTrackCutsL = new IlcESDtrackCuts("IlcESDtrackCuts", "Loose");
      esdTrackCutsL->SetMinNClustersTPC(50);
      esdTrackCutsL->SetMaxChi2PerClusterTPC(3.5);
      esdTrackCutsL->SetMaxCovDiagonalElements(2,2,0.5,0.5,2);
      esdTrackCutsL->SetRequireTPCRefit(kTRUE);
      esdTrackCutsL->SetDCAToVertexZ(3.0);
      esdTrackCutsL->SetDCAToVertexXY(3.0);
      esdTrackCutsL->SetDCAToVertex2D(kTRUE);
      esdTrackCutsL->SetRequireSigmaToVertex(kFALSE);
      esdTrackCutsL->SetAcceptKinkDaughters(kFALSE);

      IlcESDtrackCuts* esdTrackCutsITSsa = new IlcESDtrackCuts("IlcESDtrackCuts", "ITS stand-alone");
      esdTrackCutsITSsa->SetRequireITSStandAlone(kTRUE);
      
      IlcAnalysisFilter* trackFilter = new IlcAnalysisFilter("trackFilter");
      trackFilter->AddCuts(esdTrackCutsL);
      trackFilter->AddCuts(esdTrackCutsITSsa);
      
      IlcAnalysisTaskESDfilter *esdfilter = new IlcAnalysisTaskESDfilter("ESD Filter");
      esdfilter->SetTrackFilter(trackFilter);
      
      mgr->AddTask(esdfilter);
    
    
      //
      // Create containers for input/output
      IlcAnalysisDataContainer *cinput1 = mgr->GetCommonInputContainer();
      IlcAnalysisDataContainer *coutput1 = mgr->GetCommonOutputContainer();
      
      coutput1->SetSpecialOutput();

      if(bKineFilter){
	mgr->ConnectInput  (kinefilter,     0, cinput1  );
	mgr->ConnectOutput (kinefilter,     0, coutput1 );
	IlcAnalysisDataContainer *coutputEx = mgr->CreateContainer("cFilterList", TList::Class(),
								   IlcAnalysisManager::kOutputContainer,"pyxsec_hists.root");
	mgr->ConnectOutput (kinefilter,     1, coutputEx );
      }

	mgr->ConnectInput  (esdfilter,     0, cinput1  );
	mgr->ConnectOutput (esdfilter,     0, coutput1 );
      
      //
      // Run the analysis
      //    
      mgr->InitAnalysis();
      mgr->PrintStatus();
      mgr->StartAnalysis("proof",dataset,10000);

}


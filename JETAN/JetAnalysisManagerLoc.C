void JetAnalysisManagerLoc()
{
      gSystem->Load("libTree.so");
      gSystem->Load("libPhysics.so");
      gSystem->Load("libGeom.so");
      gSystem->Load("libVMC.so");
      gSystem->Load("libANALYSIS.so");
      gSystem->Load("libSTEERBase.so");
      gSystem->Load("libAOD.so");
      gSystem->Load("libESD.so");
      gSystem->Load("libANALYSISilc.so");
      gSystem->Load("libJETAN.so");

     //
    if (gApplication) gApplication->InitializeGraphics();
    // Create the chain
    //
    gROOT->LoadMacro("CreateESDChain.C");
    TChain* chain = new TChain("esdTree");
    chain->Add("~/ilc/data/highpt/kPythia6Jets125_150/030/IlcESDs.root");

    /////////////////////////////////////////////////////////////////////////////////// 
    // Create the analysis manager
    //
    // Input 
    IlcESDInputHandler* inpHandler = new IlcESDInputHandler();
    // Output
    IlcAODHandler* aodHandler = new IlcAODHandler();
    aodHandler->SetOutputFileName("aod.root");
    // MC Truth
    IlcMCEventHandler* mcHandler = new IlcMCEventHandler();
    

    IlcAnalysisManager *mgr  = new IlcAnalysisManager("Jet Manager", "Jet Manager");
    mgr->SetInputEventHandler  (inpHandler);
    mgr->SetOutputEventHandler (aodHandler);
    mgr->SetMCtruthEventHandler(mcHandler);
    mgr->SetDebugLevel(10);
    /////////////////////////////////////////////////////////////////////////////////// 
    
    
    //
    // Set of cuts
    // 
    // standard   

    IlcESDtrackCuts* esdTrackCutsL = new IlcESDtrackCuts("Standard Track Cuts", "ESD Track Cuts");
    esdTrackCutsL->SetMinNClustersTPC(50);
    esdTrackCutsL->SetMaxChi2PerClusterTPC(3.5);
    esdTrackCutsL->SetRequireTPCRefit(kTRUE);
    esdTrackCutsL->SetMaxDCAToVertexXY(2.4);
    esdTrackCutsL->SetMaxDCAToVertexZ(3.2);
    esdTrackCutsL->SetDCAToVertex2D(kTRUE);
    esdTrackCutsL->SetRequireSigmaToVertex(kFALSE);
    esdTrackCutsL->SetAcceptKinkDaughters(kFALSE);
    
    //
    // hard
    IlcESDtrackCuts* esdTrackCutsH = new IlcESDtrackCuts("IlcESDtrackCuts", "Hard");
    esdTrackCutsH->SetMinNClustersTPC(100);
    esdTrackCutsH->SetMaxChi2PerClusterTPC(2.0);
    esdTrackCutsH->SetMaxCovDiagonalElements(2,2,0.5,0.5,2);
    esdTrackCutsH->SetRequireTPCRefit(kTRUE);
    esdTrackCutsH->SetMaxNsigmaToVertex(2);
    esdTrackCutsH->SetRequireSigmaToVertex(kTRUE);
    esdTrackCutsH->SetAcceptKinkDaughters(kFALSE);
    //
    IlcAnalysisFilter* trackFilter = new IlcAnalysisFilter("trackFilter");
    trackFilter->AddCuts(esdTrackCutsL);
    trackFilter->AddCuts(esdTrackCutsH);
    //
    IlcAnalysisTaskESDfilter *esdfilter = new IlcAnalysisTaskESDfilter("ESD Filter");
    esdfilter->SetTrackFilter(trackFilter);
    esdfilter->SetDebugLevel(10);
    mgr->AddTask(esdfilter);

    IlcAnalysisTaskJets *jetana = new IlcAnalysisTaskJets("JetAnalysis");
    jetana->SetDebugLevel(10);



    IlcAnalysisTaskJets *jetanaMC = new IlcAnalysisTaskJets("JetAnalysisMC");
    jetanaMC->SetDebugLevel(10);
    jetanaMC->SetConfigFile("ConfigJetAnalysisMC.C");
    jetanaMC->SetNonStdBranch("jetsMC");
    mgr->AddTask(jetanaMC);
    mgr->AddTask(jetana);

    //
    // Create containers for input/output
    IlcAnalysisDataContainer *cinput1 = mgr->GetCommonInputContainer();
    IlcAnalysisDataContainer *coutput1 = mgr->GetCommonOutputContainer();
    IlcAnalysisDataContainer *coutput2 = mgr->CreateContainer("histos", TList::Class(),
							      IlcAnalysisManager::kOutputContainer, "histos.root");

    IlcAnalysisDataContainer *coutputMC2 = mgr->CreateContainer("histosMC", TList::Class(),
							      IlcAnalysisManager::kOutputContainer, "histosMC.root");

    mgr->ConnectInput  (esdfilter,  0, cinput1  );
    mgr->ConnectOutput (esdfilter,  0, coutput1 );

    mgr->ConnectInput  (jetana,     0, cinput1  );
    mgr->ConnectOutput (jetana,     0, coutput1 );
    mgr->ConnectOutput (jetana,     1, coutput2 );

    mgr->ConnectInput  (jetanaMC,     0, cinput1  );
    mgr->ConnectOutput (jetanaMC,     0, coutput1 );
    mgr->ConnectOutput (jetanaMC,     1, coutputMC2 );


    //
    // Run the analysis
    //    
    mgr->InitAnalysis();
    mgr->PrintStatus();
    mgr->StartAnalysis("local",chain);
}

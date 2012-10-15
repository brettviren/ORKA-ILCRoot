void JetAnalysisManagerCAF()
{
    //
    if (gApplication) gApplication->InitializeGraphics();
    //
    // Connect to proof
    
//    TProof::Reset("proof://morsch@lxb6046.cern.ch"); 
    TProof::Open("proof://morsch@lxb6046.cern.ch");
//    gProof->SetParallel(1);
//    gProof->ClearPackage("STEERBase");
//    gProof->ClearPackage("ESD");
//    gProof->ClearPackage("AOD");
//    gProof->ClearPackage("JETAN");
//    gProof->ClearPackage("ANALYSIS");
//    gProof->ClearPackage("ANALYSISilc");
    
    gProof->ShowEnabledPackages();

    // Enable the STEERBase Package
    gProof->UploadPackage("STEERBase.par");
    gProof->EnablePackage("STEERBase");
    // Enable the ESD Package
    gProof->UploadPackage("ESD.par");
    gProof->EnablePackage("ESD");
     // Enable the AOD Package
    gProof->UploadPackage("AOD.par");
    gProof->EnablePackage("AOD");
     // Enable the Analysis Package
    gProof->UploadPackage("ANALYSIS.par");
    gProof->EnablePackage("ANALYSIS");

    gProof->UploadPackage("ANALYSISilc.par");
    gProof->EnablePackage("ANALYSISilc");

    // Enable the JETAN Package
    gProof->UploadPackage("JETAN.par");
    gProof->EnablePackage("JETAN");

    //
    gProof->ShowEnabledPackages();
    
    //
    //
    // Create the chain
    //
    // TChain* chain = CreateESDChain("test.txt", 200);
 // Input 
    IlcESDInputHandler* inpHandler = new IlcESDInputHandler();
    //
    // Create the analysis manager
    //
    IlcAODHandler* aodHandler   = new IlcAODHandler();
    aodHandler->SetOutputFileName("jets.root");
    
    IlcAnalysisManager *mgr  = new IlcAnalysisManager("Jet Manager", "Jet Manager");
    mgr->SetOutputEventHandler(aodHandler);
    mgr->SetInputEventHandler(inpHandler);
    mgr-> SetDebugLevel(10);


    //
    //  ESD Filter Task
    //
    //
    // Set of cuts
    // 
    // standard
    IlcESDtrackCuts* esdTrackCutsL = new IlcESDtrackCuts("IlcESDtrackCuts", "Loose");
    esdTrackCutsL->SetMinNClustersTPC(50);
    esdTrackCutsL->SetMaxChi2PerClusterTPC(3.5);
    esdTrackCutsL->SetRequireTPCRefit(kTRUE);
    esdTrackCutsL->SetMaxDCAToVertexXY(2.4);
    esdTrackCutsL->SetMaxDCAToVertexZ(3.2);
    esdTrackCutsL->SetDCAToVertex2D(kTRUE);
    esdTrackCutsL->SetRequireSigmaToVertex(kFALSE);
    esdTrackCutsL->SetAcceptKinkDaughters(kFALSE);
    //
    //
    IlcAnalysisFilter* trackFilter = new IlcAnalysisFilter("trackFilter");
    trackFilter->AddCuts(esdTrackCutsL);
    //
    IlcAnalysisTaskESDfilter *esdfilter = new IlcAnalysisTaskESDfilter("ESD Filter");
    esdfilter->SetTrackFilter(trackFilter);
    esdfilter->SetDebugLevel(10);
    mgr->AddTask(esdfilter);

//
//   Jet Finder Task
//

    IlcAnalysisTaskJets *jetana = new IlcAnalysisTaskJets("JetAnalysis");
    jetana->SetDebugLevel(10);
    jetana->SetConfigFile("ConfigJetAnalysisAOD.C");
    mgr->AddTask(jetana);
    //
    // Create containers for input/output
    IlcAnalysisDataContainer *cinput1 = mgr->GetCommonInputContainer();

    IlcAnalysisDataContainer *coutput1 = mgr->GetCommonOutputContainer();
    coutput1->SetSpecialOutput();
    
    IlcAnalysisDataContainer *coutput2 = mgr->CreateContainer("histos", TList::Class(),
							      IlcAnalysisManager::kOutputContainer, "histos.root");

    mgr->ConnectInput  (esdfilter,  0, cinput1 );
    mgr->ConnectOutput (esdfilter,  0, coutput1);

    mgr->ConnectInput (jetana, 0, cinput1);
    mgr->ConnectOutput(jetana, 0, coutput1);
    mgr->ConnectOutput(jetana, 1, coutput2);


    //
    // Run the analysis
    //    
    mgr->InitAnalysis();
    mgr->PrintStatus();
    mgr->StartAnalysis("proof","/PWG4/arian/jetjetAbove_50_real");
}

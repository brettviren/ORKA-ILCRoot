{
    gSystem->Load("libPhysics.so");
    // Connecting to the PROOF cluster
    TProof::Open("ilccaf");

    // Clear packages if changing ROOT version on CAF or local
    //gProof->ClearPackages();
    
    // Enable the needed packages
    gProof->UploadPackage("STEERBase");
    gProof->EnablePackage("STEERBase");
    gProof->UploadPackage("ESD");
    gProof->EnablePackage("ESD");
    gProof->UploadPackage("AOD");
    gProof->EnablePackage("AOD");
    gProof->UploadPackage("ANALYSIS");
    gProof->EnablePackage("ANALYSIS");
    gProof->UploadPackage("ANALYSISilc");
    gProof->EnablePackage("ANALYSISilc");
    gProof->UploadPackage("JETAN");
    gProof->EnablePackage("JETAN");
       
    // Create the analysis manager
    mgr = new IlcAnalysisManager("Analysis UE test");

    // Create, add task
    gProof->Load("IlcKineTrackCuts.cxx+");
    IlcKineTrackCuts* trackCuts = new IlcKineTrackCuts("IlcKineTrackCuts", "Eta");
    trackCuts->SetEtaRange(-1., 1.);
    
    IlcAnalysisFilter* trackFilter = new IlcAnalysisFilter("trackFilter");
    trackFilter->AddCuts(trackCuts);
    
    gProof->Load("IlcAnalysisTaskKineFilter.cxx+");
    IlcAnalysisTaskKineFilter *kinefilter = new IlcAnalysisTaskKineFilter("Kine Filter");
    kinefilter->SetTrackFilter(trackFilter);
    mgr->AddTask(kinefilter);
    
    // Create chain of input files
    gROOT->LoadMacro("CreateESDChain.C");
    chain = CreateChain( "TE", "KINE82XX_30K.txt", 200);
 
 
 
    /////////////////////////////////////////////////////////////////////////////////// 
    // Create the analysis manager
    //
    // Input 
    // MC Truth
    IlcMCEventHandler* mcHandler = new IlcMCEventHandler();
    mcHandler->SetReadTR(kFALSE);
    
    // Output
    IlcAODHandler* aodHandler = new IlcAODHandler();
    aodHandler->SetOutputFileName( "aodKine.root" );

    IlcAnalysisManager *mgr  = new IlcAnalysisManager("Jet Manager", "Jet Manager");
//    mgr->SetInputEventHandler  (inpHandler);
    mgr->SetMCtruthEventHandler(mcHandler);
    mgr->SetOutputEventHandler (aodHandler);
    mgr->SetDebugLevel(10);
    
    /////////////////////////////////////////////////////////////////////////////////// 
    
    //
    // Set of cuts
    // 
    IlcKineTrackCuts* trackCuts = new IlcKineTrackCuts("IlcKineTrackCuts", "Eta");
    trackCuts->SetEtaRange(-1., 1.);
 //   trackCuts->SetPtRange(0.5);
              
    IlcAnalysisFilter* trackFilter = new IlcAnalysisFilter("trackFilter");
    trackFilter->AddCuts(trackCuts);
    
    
    IlcAnalysisTaskKineFilter* kinefilter = new IlcAnalysisTaskKineFilter("Kine Filter");
    kinefilter->SetTrackFilter(trackFilter);
    kinefilter->SetDebugLevel(10);
    mgr->AddTask(kinefilter);
    
    //    Analysis Task for Jet
    //  IlcAnalysisTaskJets need ConfigJetAnalysis.C macro !!!!
    IlcAnalysisTaskJets *jetana = new IlcAnalysisTaskJets("JetAnalysis");
    jetana->SetDebugLevel(10);
    mgr->AddTask(jetana);

    //
    // Create containers for input/output                  
    IlcAnalysisDataContainer *cinput1 = mgr->GetCommonInputContainer();
    if (!cinput1) cinput1 = mgr->CreateContainer("cchain",TChain::Class(), 
                                      IlcAnalysisManager::kInputContainer);

    IlcAnalysisDataContainer *coutput1 = mgr->GetCommonOutputContainer();
    IlcAnalysisDataContainer *coutput2 = mgr->CreateContainer("histos", TList::Class(),
                                                              IlcAnalysisManager::kOutputContainer, "histos.root");

    mgr->ConnectInput  (kinefilter,  0, cinput1  );
    mgr->ConnectOutput (kinefilter,  0, coutput1 );

    mgr->ConnectInput  (jetana,     0, cinput1  );
    mgr->ConnectOutput (jetana,     0, coutput1 );
    mgr->ConnectOutput (jetana,     1, coutput2 );


    //
    // Run the analysis
    //    
    if( mgr->InitAnalysis() ) {
      mgr->PrintStatus();
      mgr->StartAnalysis("proof", chain );
    }
}

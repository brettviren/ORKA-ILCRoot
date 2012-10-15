void JetAnalysisManagerKine()
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
    TChain* chain = new TChain("TE");
    chain->Add("/Users/kleinb/ilc/sim/PDC_08/LHC08v/280039/999/gilc.root");
    /////////////////////////////////////////////////////////////////////////////////// 
    // Create the analysis manager
    //
    // Output
    IlcAODHandler* aodHandler = new IlcAODHandler();
    aodHandler->SetOutputFileName("aod.root");
    // MC Truth
    IlcMCEventHandler* mcHandler = new IlcMCEventHandler();
    IlcAnalysisManager *mgr  = new IlcAnalysisManager("Jet Manager", "Jet Manager");
    mgr->SetOutputEventHandler (aodHandler);
    mgr->SetMCtruthEventHandler(mcHandler);
    mgr->SetDebugLevel(10);

    IlcAnalysisTaskKineFilter *kinefilter = new IlcAnalysisTaskKineFilter("Kine Filter");
    mgr->AddTask(kinefilter);
    
    
    IlcAnalysisTaskJets *jetana = new IlcAnalysisTaskJets("JetAnalysis");
    jetana->SetConfigFile("ConfigJetAnalysisMC.C");
    jetana->SetDebugLevel(10);
    mgr->AddTask(jetana);

    //
    // Create containers for input/output
    IlcAnalysisDataContainer *cinput1 = mgr->CreateContainer("cchain",TChain::Class(), 
							     IlcAnalysisManager::kInputContainer);

    IlcAnalysisDataContainer *coutput1 = mgr->CreateContainer("tree", TTree::Class(),
							      IlcAnalysisManager::kOutputContainer, "default");
    IlcAnalysisDataContainer *coutput2 = mgr->CreateContainer("histos", TList::Class(),
							      IlcAnalysisManager::kOutputContainer, "histos.root");

    mgr->ConnectInput  (kinefilter,     0, cinput1  );
    mgr->ConnectOutput (kinefilter,     0, coutput1 );

    mgr->ConnectInput  (jetana,     0, cinput1  );
    mgr->ConnectOutput (jetana,     0, coutput1 );
    mgr->ConnectOutput (jetana,     1, coutput2 );


    //
    // Run the analysis
    //    
    mgr->InitAnalysis();
    mgr->PrintStatus();
    mgr->StartAnalysis("local",chain);
}

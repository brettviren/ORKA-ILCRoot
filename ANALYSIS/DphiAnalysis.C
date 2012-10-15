void DphiAnalysis()
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

     //
    if (gApplication) gApplication->InitializeGraphics();
    // Create the chain
    //

    //TString path("/afs/cern.ch/user/m/morsch/public/");
    TString path("./");
    TChain* chain = new TChain("aodTree");
    chain->Add(Form("%s/%s",path.Data(),"IlcAOD.root"));

    /////////////////////////////////////////////////////////////////////////////////// 
    // Create the analysis manager
    //
    // Input 
    IlcMultiEventInputHandler* inpHandler = new IlcMultiEventInputHandler(2, 1);
    // Pool
    IlcEventPoolOTF* pool = new IlcEventPoolOTF("event pool", "AOD");

    pool->SetTagDirectory(path.Data());
    pool->SetMultiplicityBin(0, 100, 100);
    pool->Init();
    
    IlcAnalysisManager *mgr  = new IlcAnalysisManager("Jet Manager", "Jet Manager");
    mgr->SetInputEventHandler  (inpHandler);
    mgr->SetEventPool(pool);
    inpHandler->SetEventPool(pool);
    

    mgr->SetDebugLevel(10);
    /////////////////////////////////////////////////////////////////////////////////// 
    gROOT->LoadMacro("IlcAnalysisTaskPhiCorr.cxx++g");
    IlcAnalysisTaskPhiCorr *dphiana = new IlcAnalysisTaskPhiCorr("Phi Correlation Analysis");
    dphiana->SetDebugLevel(10);
    mgr->AddTask(dphiana);
    
    //
    // Create containers for input/output
    IlcAnalysisDataContainer *cinput1 = mgr->CreateContainer("cchain",TChain::Class(), 
							     IlcAnalysisManager::kInputContainer);

    IlcAnalysisDataContainer *coutput1 = mgr->CreateContainer("tree", TTree::Class(),
							      IlcAnalysisManager::kExchangeContainer, "default");
    IlcAnalysisDataContainer *coutput2 = mgr->CreateContainer("histos", TList::Class(),
							      IlcAnalysisManager::kOutputContainer, "histos.root");


    mgr->ConnectInput  (dphiana,  0,  mgr->GetCommonInputContainer());
    mgr->ConnectOutput (dphiana,  0, coutput1 );
    mgr->ConnectOutput (dphiana,  1, coutput2 );

    // 
    // Run the analysis
    //    
    mgr->InitAnalysis();
    mgr->PrintStatus();
    mgr->StartAnalysis("mix",chain, 1000);
}

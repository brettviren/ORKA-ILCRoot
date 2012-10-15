void JetAnalysisManager()
{
    //
    // Load relevant libraries
    //
    gSystem->Load("libTree.so");
    gSystem->Load("libNetx.so");
    gSystem->Load("libProof.so");
    gSystem->Load("libProofPlayer.so");
    gSystem->Load("libGeom.so");
    gSystem->Load("libEG");

    gSystem->Load("libANALYSIS.so");
    gSystem->Load("libESD.so"); 
    gSystem->Load("libJETAN.so");
    //
    // Connect to alien
    //
    TGrid::Connect("alien://"); 
    
    IlcTagAnalysis *TagAna = new IlcTagAnalysis(); 
    IlcEventTagCuts *EvCuts = new IlcEventTagCuts();
    IlcRunTagCuts   *RuCuts = new IlcRunTagCuts();
    //EvCuts->SetNChargedAbove1GeVRange(1, 1000);
    //EvCuts->SetMultiplicityRange(11,120);
    //EvCuts->SetNPionRange(2,10000);
     TAlienCollection* coll = TAlienCollection::Open("tag100.xml");
     TGridResult* TagResult = coll->GetGridResult("", 0, 0);
     TagResult->Print();
     TagAna->ChainGridTags(TagResult);

  //////////////////////////////////////////////////////////////////
  //Get the chain
     printf("*******************************\n");
     printf("*** Getting the Chain       ***\n");
     printf("*******************************\n");
     TChain* chain1 = 0x0;
     chain1 = TagAna->QueryTags(RuCuts, EvCuts);
     chain1->ls();
     
    //
    // Make the analysis manager
    //
    IlcAnalysisManager *mgr = new IlcAnalysisManager("Manager", "Manager");
    mgr-> SetDebugLevel(10);
    
    IlcAnalysisTaskJets *jetana = new IlcAnalysisTaskJets("JetAnalysis");
    jetana->SetDebugLevel(10);
    
    mgr->AddTask(jetana);
    // Create containers for input/output
    IlcAnalysisDataContainer *cinput1 = mgr->CreateContainer("cchain1",TChain::Class(), 
							     IlcAnalysisManager::kInputContainer);

    IlcAnalysisDataContainer *coutput1 = mgr->CreateContainer("chist1", TH1::Class(),
							      IlcAnalysisManager::kOutputContainer);

    mgr->ConnectInput (jetana,0,cinput1);
    mgr->ConnectOutput(jetana,0,coutput1);
    cinput1->SetData(chain1);

//
// Run the analysis
//    

    if (mgr->InitAnalysis()) {
	mgr->PrintStatus();
	mgr->StartAnalysis("local", chain1);
    }
}

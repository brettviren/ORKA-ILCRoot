void AnaPi0Select(const char* dataset="minbias_LHC09a4_81040_81050.xml")
{
    
    gSystem->Load("libTree.so");
    gSystem->Load("libGeom.so");
    gSystem->Load("libVMC.so");
    gSystem->Load("libPhysics.so");

    //load analysis framework
    gSystem->Load("libANALYSIS");
    gSystem->Load("libANALYSISilc"); //IlcAnalysisTaskSE

    //Set local DB for PVBAR
    gROOT->ProcessLine(".! tar xzvf PVBAR.tgz") ;
    IlcCDBManager::Instance()->SetDefaultStorage("local://$ILC_ROOT/OCDB");
    IlcCDBManager::Instance()->SetSpecificStorage("PVBAR/*","local://./");

    // A task can be compiled dynamically with AClic
    gROOT->LoadMacro("IlcAnalysisTaskPi0CalibSelection.cxx+g");

    // Connect to alien
    TString token = gSystem->Getenv("GRID_TOKEN") ;
    if ( token == "OK" ) 
     TGrid::Connect("alien://");
    else 
     IlcInfo("You are not connected to the GRID") ; 

    // Create the chain
    TChain* chain = new TChain("esdTree");
    TGridCollection * collection = dynamic_cast<TGridCollection*>(TAlienCollection::Open(dataset));
   
    TAlienResult* result = collection->GetGridResult("",0 ,0);
    TList* rawFileList = result->GetFileInfoList();

    for (Int_t counter=0 ; counter < rawFileList->GetEntries() ; counter++) {
     TFileInfo * fi =  static_cast<TFileInfo*>(rawFileList->At(counter)) ; 
     const char * rawFile = fi->GetCurrentUrl()->GetUrl() ;  
     printf("Processing %s\n", rawFile) ;
     chain->Add(rawFile);
     printf("Chain: %d entries.\n",chain->GetEntries()); 
    }

    // Make the analysis manager
    IlcAnalysisManager *mgr = new IlcAnalysisManager("Pi0CalibSelect","Pi0CalibSelection");

    // ESD input handler
    IlcESDInputHandler* esdH = new IlcESDInputHandler();
    mgr->SetInputEventHandler(esdH);

    // Debug level
    mgr->SetDebugLevel(10);

    // Add task
    IlcAnalysisTaskPi0CalibSelection *task = new IlcAnalysisTaskPi0CalibSelection("Pi0CalibSelection");
    task->SetClusterMinEnergy(0.4);
    mgr->AddTask(task);

    // Create containers for input/output
    IlcAnalysisDataContainer *cinput  = mgr->GetCommonInputContainer();
    IlcAnalysisDataContainer *coutput = mgr->CreateContainer("histos",TList::Class(),IlcAnalysisManager::kOutputContainer,"histos.root");

    // Connect input/output
    mgr->ConnectInput(task, 0, cinput);
    mgr->ConnectOutput(task, 1, coutput);

    if (mgr->InitAnalysis()) {
	     mgr->PrintStatus();
	     mgr->StartAnalysis("local", chain);
    }

}



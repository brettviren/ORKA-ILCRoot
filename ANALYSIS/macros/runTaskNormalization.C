
void runTaskNormalization(const char * incollection,const char * filename = "LHC09b12_7TeV_0.5T_norm.root", Bool_t isMC = 1,Int_t nev =123456789) {

  
  // Load libraries
  gSystem->Load("libANALYSIS") ;
  gSystem->Load("libANALYSISilc") ;
  gSystem->Load("libCORRFW") ;
  //  gSystem->Load("libITSbase") ;
  gSystem->Load("libPWG0base") ;


  // chain
  TChain* analysisChain = 0;
  analysisChain = new TChain("esdTree");
  if (TString(incollection).Contains(".root")){
    analysisChain->Add(incollection);
  }
  else if (TString(incollection).Contains("xml")){
    TGrid::Connect("alien://");
    TAlienCollection * coll = TAlienCollection::Open (incollection);
    while(coll->Next()){
      analysisChain->Add(TString("alien://")+coll->GetLFN());
    }
  } else {
    ifstream file_collect(incollection);
    TString line;
    while (line.ReadLine(file_collect) ) {
      analysisChain->Add(line.Data());
    }
  }
  analysisChain->GetListOfFiles()->Print();


  // Make the analysis manager
  IlcAnalysisManager *mgr = new IlcAnalysisManager("TestManager");
  //  mgr->SetDebugLevel(3);
  // Add ESD handler
  IlcESDInputHandler* esdH = new IlcESDInputHandler; 

  mgr->SetInputEventHandler(esdH);
	
  if(isMC) {
    IlcMCEventHandler *mc = new IlcMCEventHandler();
    mc->SetReadTR(kFALSE);
    mgr->SetMCtruthEventHandler(mc);
  }
  // assign simple task
//   gROOT->LoadMacro("IlcCollisionNormalization.cxx++g");   
//   gROOT->LoadMacro("IlcCollisionNormalizationTask.cxx++g");   
  //____________________________________________//
  // Physics selection
  gROOT->LoadMacro("$(ILC_ROOT)/ANALYSIS/macros/AddTaskPhysicsSelection.C");
  IlcPhysicsSelectionTask* physSelTask = AddTaskPhysicsSelection(isMC,1,!isMC); // Use Physics Selection. Enable computation of BG if is not MC
  //  task->SelectCollisionCandidates(); /// This should be disabled, at least for MC: we need all the events
  physSelTask->GetPhysicsSelection()->SetBin0Callback("TaskNormalization");

  // assign simple task
  // IlcCollisionNormalizationTask * task = new IlcCollisionNormalizationTask("TaskNormalization");
  // //  task->SetMC();
  // task->SetMC(isMC);
  // mgr->AddTask(task);



  // IlcAnalysisDataContainer *cinput1 = mgr->GetCommonInputContainer();	
  // mgr->ConnectInput(task,0,cinput1);


  
  // // Attach output
  // cOutput = mgr->CreateContainer("Norm", TList::Class(), IlcAnalysisManager::kOutputContainer,filename);
  // mgr->ConnectOutput(task, 1, cOutput);      

  gROOT->LoadMacro("$(ILC_ROOT)/ANALYSIS/macros/AddTaskNormalization.C");
  IlcCollisionNormalizationTask * task = AddTaskNormalization(isMC);
	
  if (!mgr->InitAnalysis()) return;
	
  mgr->PrintStatus();
  mgr->StartAnalysis("local",analysisChain,nev);

}

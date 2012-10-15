void runlocal() {
  TStopwatch timer;
  timer.Start();


  gSystem->SetIncludePath("-I$ROOTSYS/include -I$ILC_ROOT/include"); 
  gSystem->Load("libTree.so");
  gSystem->Load("libGeom.so");
  gSystem->Load("libVMC.so");
  gSystem->Load("libPhysics.so");
  
  //Enable the needed package
  gSystem->Load("libSTEERBase");
  gSystem->Load("libESD");
  gSystem->Load("libAOD");
  gSystem->Load("libANALYSIS");
  gSystem->Load("libANALYSISilc");

 // Create chain of input files
  gROOT->LoadMacro("$ILC_ROOT/PWG0/CreateESDChain.C");
  TChain* chain = CreateESDChain("file.txt", 3);
 
  //ANALYSIS PART
  gROOT->LoadMacro("IlcAnalysisTaskCluster.cxx++g");

    
  //____________________________________________//
  // Make the analysis manager
  IlcAnalysisManager *mgr = new IlcAnalysisManager("TestManager");
  
  //IlcVEventHandler* esdH = new IlcESDInputHandler;
  IlcESDInputHandler* esdH = new IlcESDInputHandler;
  esdH->SetReadFriends(0);
  mgr->SetInputEventHandler(esdH);  

  IlcMCEventHandler* mcH = new IlcMCEventHandler();
  mcH->SetReadTR(kFALSE);
  mgr->SetMCtruthEventHandler(mcH);


 //____________________________________________//
  // event selection 

  gROOT->LoadMacro("$ILC_ROOT/ANALYSIS/macros/AddTaskPhysicsSelection.C");
  IlcPhysicsSelectionTask* physSelTask = AddTaskPhysicsSelection();
  physSelTask->GetPhysicsSelection()->SetAnalyzeMC(); 

  IlcPhysicsSelection* physSel = physSelTask->GetPhysicsSelection();
  physSel->AddBackgroundIdentification(new IlcBackgroundSelection());

  
  //____________________________________________//
  // analysis task and esd track cuts
  IlcAnalysisTaskCluster *task1 = new IlcAnalysisTaskCluster("IlcAnalysisTaskCluster");

  IlcESDtrackCuts* esdTrackCutsL1 = new IlcESDtrackCuts("IlcESDtrackCuts","test");
  esdTrackCutsL1->SetMaxDCAToVertexXY(3.);
  esdTrackCutsL1->SetMaxDCAToVertexZ(3.);
  esdTrackCutsL1->SetAcceptKinkDaughters(kFALSE);
  

  task1->SetCuts(esdTrackCutsL1);
  task1->SelectCollisionCandidates();


  mgr->AddTask(task1);
  // Create containers for input/output
  IlcAnalysisDataContainer *cinput1 = 
    mgr->CreateContainer("cchain",TChain::Class(),IlcAnalysisManager::kInputContainer);
  IlcAnalysisDataContainer *coutput1 = 
    mgr->CreateContainer("chist1",TList::Class(),IlcAnalysisManager::kOutputContainer,
			 "Cluster.local.root");
  
  //____________________________________________//
  mgr->ConnectInput(task1, 0, mgr->GetCommonInputContainer());
  mgr->ConnectOutput(task1,1,coutput1);
  if (!mgr->InitAnalysis()) return;
  mgr->PrintStatus();
  mgr->StartAnalysis("local",chain);
  //mgr->StartAnalysis("local",chain, 100,200);//startevent, nevents

  timer.Stop();
  timer.Print();
}


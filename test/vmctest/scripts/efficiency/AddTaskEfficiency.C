IlcAnalysisTaskEfficiency * AddTaskEfficiency(Int_t runNumber)

{
  // Creates a QA task exploiting simple symmetries phi, eta +/-, charge ...
  
  // Get the pointer to the existing analysis manager via the static access method.
  //==============================================================================
  IlcAnalysisManager *mgr = IlcAnalysisManager::GetAnalysisManager();
  if (!mgr) {
    ::Error("AddTaskEfficiency", "No analysis manager to connect to.");
    return NULL;
  }  
  
  // Check the analysis type using the event handlers connected to the analysis manager.
  //==============================================================================
  if (!mgr->GetInputEventHandler()) {
    ::Error("AddTaskEfficiency", "This task requires an input event handler");
    return NULL;
  }
   
  TString inputDataType = mgr->GetInputEventHandler()->GetDataType(); 
  // can be "ESD" or "AOD"
  
  // Configure analysis
  //===========================================================================
  
  //TASKS
  //Task for global tracks
  IlcAnalysisTaskEfficiency *task0 = 
    new IlcAnalysisTaskEfficiency("IlcAnalysisTaskEfficiency_Global");
  task0->SelectCollisionCandidates(); // default setting: kMB = min bias trigger
  task0->SetTrackType(0);// global tracks

  // Task for ITS tracks
  IlcAnalysisTaskEfficiency *task1 = 
    new IlcAnalysisTaskEfficiency("IlcAnalysisTaskEfficiency_ITS");
  task1->SelectCollisionCandidates();
  task1->SetTrackType(1);// ITS tracks

  //Task for ITS_SA tracks
  IlcAnalysisTaskEfficiency *task1sa = 
    new IlcAnalysisTaskEfficiency("IlcAnalysisTaskEfficiency_ITS_SA");
  task1sa->SelectCollisionCandidates();
  task1sa->SetTrackType(1);// ITS tracks
   
  //Task for TPC tracks 
  IlcAnalysisTaskEfficiency *task2 = 
    new IlcAnalysisTaskEfficiency("IlcAnalysisTaskEfficiency_TPC");
  task2->SelectCollisionCandidates();
  task2->SetTrackType(2);// TPC only tracks
 
  //CUTS
  //cuts for global tracks
  IlcESDtrackCuts* esdTrackCutsL0 = new IlcESDtrackCuts("IlcESDtrackCuts0","Global");
  esdTrackCutsL0->SetMinNClustersTPC(70);
  esdTrackCutsL0->SetRequireTPCRefit(kTRUE);
  esdTrackCutsL0->SetRequireITSRefit(kTRUE);
  esdTrackCutsL0->SetMaxDCAToVertexXY(3.);
  esdTrackCutsL0->SetMaxDCAToVertexZ(3.);
  esdTrackCutsL0->SetAcceptKinkDaughters(kFALSE);
   
  //cuts for ITS tracks
  IlcESDtrackCuts* esdTrackCutsL1 = new IlcESDtrackCuts("IlcESDtrackCuts1","ITS");
  esdTrackCutsL1->SetMaxDCAToVertexXY(3.);
  esdTrackCutsL1->SetMaxDCAToVertexZ(3.);
  esdTrackCutsL1->SetAcceptKinkDaughters(kFALSE);
  esdTrackCutsL1->SetRequireITSRefit(kTRUE);
  esdTrackCutsL1->SetRequireITSStandAlone(kTRUE); 

  //cuts for ITS tracks SA
  IlcESDtrackCuts* esdTrackCutsL1sa = new IlcESDtrackCuts("IlcESDtrackCuts1sa","ITS_SA");
  esdTrackCutsL1sa->SetMaxDCAToVertexXY(3.);
  esdTrackCutsL1sa->SetMaxDCAToVertexZ(3.);
  esdTrackCutsL1sa->SetAcceptKinkDaughters(kFALSE);
  esdTrackCutsL1sa->SetRequireITSRefit(kTRUE);
  esdTrackCutsL1sa->SetRequireITSPureStandAlone(kTRUE);
   
  //cuts for TPC tracks
  IlcESDtrackCuts* esdTrackCutsL2 = new IlcESDtrackCuts("IlcESDtrackCuts2","TPC");
  esdTrackCutsL2->SetRequireTPCRefit(kFALSE);
  esdTrackCutsL2->SetAcceptKinkDaughters(kFALSE);
  //jacek's cuts:
  esdTrackCutsL2->SetMinNClustersTPC(70);
  // cut on max ncl=160 in Task
  esdTrackCutsL2->SetMaxDCAToVertexXY(3.);
  esdTrackCutsL2->SetMaxDCAToVertexZ(3.);
  esdTrackCutsL2->SetMaxChi2PerClusterTPC(3.999);
  //cut minChi=0 in task
  //esdTrackCutsL2->SetPRange(0.15,16); // not needed for QA
  //esdTrackCutsL2->SetEtaRange(-0.8, 0.7999); // not needed for QA
  //cuts for ITS tracks
  
  //add cuts to tasks
  task0->SetCuts(esdTrackCutsL0);
  task1->SetCuts(esdTrackCutsL1);
  task1sa->SetCuts(esdTrackCutsL1sa);
  task2->SetCuts(esdTrackCutsL2);

  // add tasks to manager
  mgr->AddTask(task0);
  mgr->AddTask(task1);
  mgr->AddTask(task1sa);
  mgr->AddTask(task2);
     
  //output container for tasks
  IlcAnalysisDataContainer *cout0    = 0;
  IlcAnalysisDataContainer *cout1    = 0;
  IlcAnalysisDataContainer *cout1sa  = 0;
  IlcAnalysisDataContainer *cout2    = 0;

   
  if(runNumber>0){ 
    cout0   =  mgr->CreateContainer("QAHists_Global",TList::Class(),
				    IlcAnalysisManager::kOutputContainer, 
				    Form("run%d.root",runNumber));
    cout1   =  mgr->CreateContainer("QAHists_ITS",TList::Class(),
				    IlcAnalysisManager::kOutputContainer,
				    Form("run%d.root",runNumber));
    cout1sa =  mgr->CreateContainer("QAHists_ITS_SA",TList::Class(),
				    IlcAnalysisManager::kOutputContainer, 
				    Form("run%d.root",runNumber));
    cout2   =  mgr->CreateContainer("QAHists_TPC",TList::Class(),
				    IlcAnalysisManager::kOutputContainer, 
				    Form("run%d.root",runNumber));
  }
   
  else{
    cout0   = mgr->CreateContainer("QAHists_Global",TList::Class(),
				   IlcAnalysisManager::kOutputContainer, 
				   Form("%s:QAHists",IlcAnalysisManager::
					GetCommonFileName()));
    cout1   = mgr->CreateContainer("QAHists_ITS",TList::Class(),
				   IlcAnalysisManager::kOutputContainer, 
				   Form("%s:QAHists",IlcAnalysisManager::
					GetCommonFileName()));
    cout1sa = mgr->CreateContainer("QAHists_ITS_SA",TList::Class(),
				   IlcAnalysisManager::kOutputContainer, 
				   Form("%s:QAHists",IlcAnalysisManager::
					GetCommonFileName()));
    cout2   = mgr->CreateContainer("QAHists_TPC",TList::Class(),
				   IlcAnalysisManager::kOutputContainer, 
				   Form("%s:QAHists",IlcAnalysisManager::
					GetCommonFileName()));
  }
  
  //connect input to task
  mgr->ConnectInput  (task0,   0, mgr->GetCommonInputContainer());
  mgr->ConnectInput  (task1,   0, mgr->GetCommonInputContainer());
  mgr->ConnectInput  (task1sa, 0, mgr->GetCommonInputContainer());
  mgr->ConnectInput  (task2,   0, mgr->GetCommonInputContainer());

  //connect output to task
  mgr->ConnectOutput (task0,   1, cout0);
  mgr->ConnectOutput (task1,   1, cout1);
  mgr->ConnectOutput (task1sa, 1, cout1sa);
  mgr->ConnectOutput (task2,   1, cout2);

  
  return task0;

}



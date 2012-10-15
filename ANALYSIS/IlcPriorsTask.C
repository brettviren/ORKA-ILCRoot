
Bool_t IlcPriorsTask()
{
  
  TBenchmark benchmark;
  benchmark.Start("IlcPriorsTask");

  IlcLog::SetGlobalDebugLevel(0);

  Load() ; //load the required libraries

  TChain * analysisChain = new TChain("esdTree");
  analysisChain->Add("IlcESDs.root");

  Int_t nIter = 5;

  // create the task
  IlcPriorsTask *task = new IlcPriorsTask("Task for Priors");
  Double_t priors[5]={0.2,0.2,0.2,0.2,0.2};
  task->SetPriors(priors);
  task->SetNiterations(nIter);

  // Make the analysis manager
  IlcAnalysisManager *mgr = new IlcAnalysisManager("TestIteration");

  IlcMCEventHandler*  mcHandler = new IlcMCEventHandler();
  mgr->SetMCtruthEventHandler(mcHandler);


   IlcESDInputHandler* esdHandler = new IlcESDInputHandler();
   mgr->SetInputEventHandler(esdHandler);


  // Create and connect containers for input/output
  IlcAnalysisDataContainer *cinput0  = mgr->GetCommonInputContainer();
  //slot 0 : default output tree (by default handled by IlcAnalysisTaskSE)
  IlcAnalysisDataContainer *coutput0 = mgr->CreateContainer("ctree0", TTree::Class(),IlcAnalysisManager::kOutputContainer,"output.root");
  // output TH1I for event counting
  IlcAnalysisDataContainer *coutput1 = mgr->CreateContainer("chist0", TH1I::Class(),IlcAnalysisManager::kOutputContainer,"output.root");
  IlcAnalysisDataContainer *coutput2 = mgr->CreateContainer("cgraph0", TH2D::Class(),IlcAnalysisManager::kOutputContainer,"output.root");


  cinput0->SetData(analysisChain);

  mgr->AddTask(task);
  mgr->ConnectInput(task,0,cinput0);
  mgr->ConnectOutput(task,0,coutput0);
  mgr->ConnectOutput(task,1,coutput1);
  mgr->ConnectOutput(task,2,coutput2);



  IlcEventPoolLoop* loop = new IlcEventPoolLoop(nIter);
  loop->SetChain(analysisChain);
  mgr->SetEventPool(loop);

  //mgr->SetDebugLevel(1); 
  printf("READY TO RUN\n");
  //RUN !!!
  if (mgr->InitAnalysis()) {
    mgr->PrintStatus();
    mgr->StartAnalysis("mixing",analysisChain);
    //mgr->StartAnalysis("local",analysisChain);
    
  }

  benchmark.Stop("IlcPriorsTask");
  benchmark.Show("IlcPriorsTask");

  return kTRUE ;
}

void Load() {

  //load the required ilcroot libraries
  gSystem->Load("libANALYSIS") ;
  gSystem->Load("libANALYSISilc") ;

  //compile online the task class
  gSystem->SetIncludePath("-I. -I$ILC_ROOT/include -I$ROOTSYS/include");
  gROOT->LoadMacro("./IlcPriorsTask.cxx++g");
}

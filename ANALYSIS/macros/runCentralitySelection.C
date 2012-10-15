void runCentralitySelection(const char *mode="local")
{
  // Load common libraries
  gSystem->Load("libCore.so");  
  gSystem->Load("libTree.so");
  gSystem->Load("libGeom.so");
  gSystem->Load("libVMC.so");
  gSystem->Load("libPhysics.so");
  gSystem->Load("libSTEERBase");
  gSystem->Load("libESD");
  gSystem->Load("libAOD");
  gSystem->Load("libANALYSIS");
  gSystem->Load("libANALYSISilc");   
  gSystem->Load("libCORRFW");   
  gSystem->Load("libPWGHFbase");   
  gSystem->Load("libPWGmuon");   
  // Use IlcRoot includes to compile our task
  gROOT->ProcessLine(".include $ILC_ROOT/include");

  // filename 
  TChain* chain = new TChain("esdTree");
  chain->AddFile("/home/alberica/analysis/centrality/data/ilc/sim/LHC10a12/104157/998/root_archive.zip#IlcESDs.root");
 
 // Create the analysis manager
  IlcAnalysisManager *mgr = new IlcAnalysisManager("AnalysisManager");
  mgr->SetDebugLevel(10);
  
  // My task
  gROOT->LoadMacro("IlcCentralitySelectionTask.cxx++g");   
  IlcCentralitySelectionTask *task = new IlcCentralitySelectionTask("CentralitySelection"); 
  task->SetPercentileFile("test_IlcCentralityBy1D.root");
  task->SetCentralityMethod("V0");
  mgr->AddTask(task);

  // My dummy task
  gROOT->LoadMacro("IlcDummy.cxx++g");   
  IlcDummy *dummytask = new IlcDummy("Dummy"); 
  mgr->AddTask(dummytask);



  IlcMCEventHandler*  mcHandler = new IlcMCEventHandler();
  mgr->SetMCtruthEventHandler(mcHandler);

  IlcESDInputHandler* esdH = new IlcESDInputHandler();
  mgr->SetInputEventHandler(esdH);

  // Create containers for input/output
  mgr->ConnectInput (task,0, mgr->GetCommonInputContainer());
  mgr->ConnectInput (dummytask,0, mgr->GetCommonInputContainer());
  //  mgr->ConnectOutput(task,0, mgr->GetCommonOutputContainer());  

  // Enable debug printouts
  mgr->SetDebugLevel(2);

  if(!mgr->InitAnalysis()) return;

  mgr->PrintStatus();
  // Start analysis in grid.
  mgr->StartAnalysis(mode, chain);

};

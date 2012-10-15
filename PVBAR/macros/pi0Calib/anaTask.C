void anaTask()
{
  //Macro to run pi0 calibration on local ESD files.
  //Author: Boris Polishchuk.
  

  //Uncomment the line below if your ESD files are from old productions
  //so there are no PVBAR geometry matrices written in the ESD.
  //You can find an ideal geometry here: $ILC_ROOT/test/QA/geometry.root
  //and copy to your working directory.

  //IlcGeomManager::LoadGeometry("geometry.root");

  //You can apply misalignment by following
  //(remember that your local OCDB in ./PVBAR should contain a copy of the
  //$ILC_ROOT/OCDB/PVBAR/Align directory while ./PVBAR/Calib/EmcGainPedestals
  //should contain the corrections to the calibration coefficients (~1) 
  //instead of real CC (~0.005)! ):

//   IlcCDBManager::Instance()->SetDefaultStorage("local://$ILC_ROOT/OCDB");
//   IlcCDBManager::Instance()->SetSpecificStorage("PVBAR/*","local://./");

//   IlcCDBManager::Instance()->SetRun(0);
//   IlcCDBEntry* e = IlcCDBManager::Instance()->Get("PVBAR/Align/Data");
//   TClonesArray *array = (TClonesArray*) e->GetObject();
//   IlcGeomManager::ApplyAlignObjsToGeom(*array);


  // load analysis framework
  gSystem->Load("libANALYSIS");
  gSystem->Load("libANALYSISilc"); //IlcAnalysisTaskSE
  gSystem->Load("libPVBARpi0Calib");

  gROOT->LoadMacro("$ILC_ROOT/PWG0/CreateESDChain.C");
  TChain* chain = CreateESDChain("list.txt", 300);

  // for includes use either global setting in $HOME/.rootrc
  // ACLiC.IncludePaths: -I$ILC_ROOT/include
  // or in each macro
  // gSystem->AddIncludePath("$ILC_ROOT/include");

  // Create the analysis manager
  IlcAnalysisManager *mgr = new IlcAnalysisManager("Pi0Calib");



  IlcVEventHandler* esdH = new IlcESDInputHandler();
  mgr->SetInputEventHandler(esdH);

  // Create task

  IlcAnalysisTaskPi0CalibSelection *task = new IlcAnalysisTaskPi0CalibSelection("Pi0CalibSelection");
  //task->SetClusterMinEnergy(0.4); 

  // Add task
  mgr->AddTask(task);

  // Create containers for input/output
  IlcAnalysisDataContainer *cinput  = mgr->GetCommonInputContainer();
  IlcAnalysisDataContainer *coutput = mgr->CreateContainer("histos", TList::Class(),  IlcAnalysisManager::kOutputContainer, "histos.root");

  // Connect input/output
  mgr->ConnectInput(task, 0, cinput);
  mgr->ConnectOutput(task, 1, coutput);

  // Enable debug printouts
  mgr->SetDebugLevel(10);

  if (!mgr->InitAnalysis())
    return;

  mgr->PrintStatus();

  mgr->StartAnalysis("local", chain);
}

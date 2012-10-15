#include "Riostream.h"
#define BIT(n)       (1ULL << (n))
/*
   This macro should be called by a script TaskBuzzer.sh like:
 export ARG="TaskBuzzer.C($1)"
 ilcroot -b -q $ARG
   Call: TaskBuzzer.sh "kCar00" or: TaskBuzzer.sh "kTender|kCar01"
*/   

TString config_name;
const char *ConfigName(UInt_t);
void LoadLibraries();
void AddAnalysisTasks();
TChain *CreateChain(const char *, const char *);

enum EBuzzerConfig {
   kBaseline         = 0x00,     // Baseline EVstat
   kCar00            = BIT(0),   // QAsym
   kCar01            = BIT(1),   // VZERO
   kCar02            = BIT(2),   // Vertex
   kCar03            = BIT(3),   // SPD
   kCar04            = BIT(4),   // SDD
   kCar05            = BIT(5),   // SSDdEdx
   kCar06            = BIT(6),   // FMD
   kCar07            = BIT(7),   // ITS
   kCar08            = BIT(8),   // TPC
   kCar09            = BIT(9),   // TRD
   kCar10            = BIT(10),  // CALO
   kCar11            = BIT(11),  // MUONTrig
   kTender           = BIT(30),  // TENDER
   kAOD              = BIT(31)   // AOD
};   
   
void TaskBuzzer(UInt_t config, const char *collection="wn.xml") 
{
  printf("Running buzzer with: %s\n", ConfigName(config));
  TGrid::Connect("alien://");
  if (!gGrid || !gGrid->IsConnected()) {
    ::Error("PilotAnalysis", "No grid connection");
    return;
  }
  TChain *chain = CreateChain(collection, "ESD");
  // Load libraries
  gSystem->SetIncludePath("-I. -I$ROOTSYS/include -I$ILC_ROOT/include -I$ILC_ROOT -I$ILC_ROOT/ITS -I$ILC_ROOT/TRD");
  LoadLibraries();
  // Create manager
  IlcAnalysisManager *mgr  = new IlcAnalysisManager("ESDfiltering", "Production train");
  mgr->SetNSysInfo(100);
  // Input handler
  IlcESDInputHandlerRP *esdHandler = new IlcESDInputHandlerRP();
//  IlcESDInputHandler *esdHandler = new IlcESDInputHandler();
  esdHandler->SetReadFriends(kTRUE);
  esdHandler->SetActiveBranches("ESDfriend");
  mgr->SetInputEventHandler(esdHandler);
  if (config & kAOD) {
     // AOD output handler
     IlcAODHandler* aodHandler   = new IlcAODHandler();
     aodHandler->SetOutputFileName("IlcAOD.root");
     if (!mergeExcludes.IsNull()) mergeExcludes += " ";
     mergeExcludes += "IlcAOD.root";
     mgr->SetOutputEventHandler(aodHandler);
  }   
  // AnalysisTasks
  AddAnalysisTasks(config);

  mgr->SetDebugLevel(1);
  if (mgr->InitAnalysis()) {                                                                                                              
    mgr->PrintStatus(); 
    mgr->StartAnalysis("local", chain);
  }  
} 

//______________________________________________________________________________
void AddAnalysisTasks(UInt_t config)
{
  IlcAnalysisManager *mgr = IlcAnalysisManager::GetAnalysisManager();
  mgr->SetCommonFileName("QAresults.root");
  //
  // Event Statistics (Jan Fiete)
  //

   gROOT->LoadMacro("$ILC_ROOT/ANALYSIS/macros/AddTaskPhysicsSelection.C");
   IlcPhysicsSelectionTask* physSelTask = AddTaskPhysicsSelection();
   mgr->RegisterExtraFile("event_stat.root");
  //
  // Tender and supplies
  //
  if (config & kTender) {
      gROOT->LoadMacro("$ILC_ROOT/ANALYSIS/TenderSupplies/AddTaskTender.C");
      IlcAnalysisTaskSE *tender = AddTaskTender();
      // SelectCollisionCandidates ?? Tender has to derive from TaskSE
      tender->SelectCollisionCandidates();
      tender->SetDebugLevel(2);
  }
  // AOD creation with collision events
  if (config & kAOD) {
    gROOT->LoadMacro("$ILC_ROOT/ANALYSIS/macros/AddTaskESDFilter.C");
    mgr->RegisterExtraFile("IlcAOD.Muons.root");
    mgr->RegisterExtraFile("IlcAOD.Dimuons.root");
    IlcAnalysisTaskESDfilter *taskesdfilter = AddTaskESDFilter(kFALSE, kTRUE, kTRUE, doEventStat);
  }   
  // TPC QA (E. Sicking)
  //
  if (config & kCar00) {
    gROOT->LoadMacro("$ILC_ROOT/PWGPP/PilotTrain/AddTaskQAsym.C");
    IlcAnalysisTaskSE * taskqasim = AddTaskQAsym(0);
    taskqasim->SelectCollisionCandidates();
  }  
  //
  // VZERO QA  (C. Cheshkov)
  //
  if (config & kCar01) {
    gROOT->LoadMacro("$ILC_ROOT/PWGPP/PilotTrain/AddTaskVZEROQA.C");
    IlcAnalysisTaskSE * taskv0qa = AddTaskVZEROQA(0);
//  taskv0qa->SelectCollisionCandidates();
  }
  // Vertexing (A. Dainese)
  // 
  if (config & kCar02) {
    gROOT->LoadMacro("$ILC_ROOT/PWGPP/macros/AddTaskVertexESD.C");
    IlcAnalysisTaskVertexESD* taskvertexesd =  AddTaskVertexESD();
    taskvertexesd->SelectCollisionCandidates();
  }  
  //
  // SPD (A. Mastroserio)
  //
  if (config & kCar03) {
    gROOT->LoadMacro("$ILC_ROOT/PWGPP/PilotTrain/AddTaskSPDQA.C");
    IlcAnalysisTaskSE* taskspdqa = AddTaskSPDQA();
    taskspdqa->SelectCollisionCandidates();
  }  
  //
  // SDD (F. Prino)
  //
  if (config & kCar04) {
    gROOT->LoadMacro("$ILC_ROOT/PWGPP/PilotTrain/AddSDDPoints.C");
    IlcAnalysisTaskSE* tasksdd = AddSDDPoints();
    tasksdd->SelectCollisionCandidates();
  }
  //
  // SSD dEdx (Marek Chojnacki)
  //
  if (config & kCar05) {
    gROOT->LoadMacro("$ILC_ROOT/PWGPP/PilotTrain/AddTaskdEdxSSDQA.C");
    IlcAnalysisTaskSE* taskssddedx = AddTaskdEdxSSDQA();
    taskssddedx->SelectCollisionCandidates();
  }

  // FMD (Hans Hjersing Dalsgaard)
  //
  if (config & kCar06) {
    gROOT->LoadMacro("$ILC_ROOT/PWGPP/PilotTrain/AddTaskFMD.C");
    IlcAnalysisTaskSE* taskfmd = AddTaskFMD();
    taskfmd->SelectCollisionCandidates();
  }  
  //
  // ITS
  //
  if (config & kCar07) {
      gROOT->LoadMacro("$ILC_ROOT/PWGPP/macros/AddTaskPerformanceITS.C");
      IlcAnalysisTaskITSTrackingCheck *itsQA = AddTaskPerformanceITS(kFALSE);
  }
  //
  // TPC (Jacek Otwinowski)
  //
  if (config & kCar08) {
    gROOT->LoadMacro("$(ILC_ROOT)/PWGPP/TPC/macros/AddTaskPerformanceTPCQA.C");
    IlcPerformanceTask *tpcQA = AddTaskPerformanceTPCQA(kFALSE, kTRUE);
  }  
  //
  // TRD (Alex Bercuci, M. Fasel) 
  //
  if(config & kCar09) {
      gROOT->LoadMacro("$ILC_ROOT/PWGPP/macros/AddTrainPerformanceTRD.C");
      AddTrainPerformanceTRD("ALL");
  }

  //
  // Calorimetry (Gustavo Conesa)
  //

  if(config & kCar10) {
      gROOT->LoadMacro("$ILC_ROOT/PWG4/macros/QA/AddTaskCalorimeterQA.C");
      IlcAnalysisTaskParticleCorrelation *taskCaloQA = AddTaskCalorimeterQA("ESD", kTRUE, kFALSE);
      taskCaloQA->SetDebugLevel(0);
  }

  //
  // Muon Trigger
  //
  
  if(config & kCar11) {
      gROOT->LoadMacro("$ILC_ROOT/PWGPP/macros/AddTaskMTRchamberEfficiency.C");
      IlcAnalysisTaskTrigChEff *taskMuonTrig = AddTaskMTRchamberEfficiency();
  }

  //
  // Muon Efficiency
  //
/*
  if(doMUONEff) {
      gROOT->LoadMacro("$ILC_ROOT/PWG3/muondep/AddTaskMUONTrackingEfficiency.C");
      IlcAnalysisTaskMuonTrackingEff *taskMuonTrackEff = AddTaskMUONTrackingEfficiency();
  }
  
  //
  // V0-Decay Reconstruction (Ana Marin)
  // 

  if (doV0) {
      gROOT->LoadMacro("$ILC_ROOT/PWGPP/macros/AddTaskV0QA.C");
      IlcAnalysisTaskV0QA *taskv0QA = AddTaskV0QA(kFALSE);
  }
*/
}

//______________________________________________________________________________
const char *ConfigName(UInt_t config) 
{
   config_name = "EVstat";
   if (config & kTender) config_name += "+TENDER";
   if (config & kAOD)    config_name   += "+AOD";
   if (config & kCar00)  config_name += "+QAsym";
   if (config & kCar01)  config_name += "+VZERO";
   if (config & kCar02)  config_name += "+Vertex";
   if (config & kCar03)  config_name += "+SPD";
   if (config & kCar04)  config_name += "+SDD";
   if (config & kCar05)  config_name += "+SSDdEdx";
   if (config & kCar06)  config_name += "+FMD";
   if (config & kCar07)  config_name += "+ITS";
   if (config & kCar08)  config_name += "+TPC";
   if (config & kCar09)  config_name += "+TRD";
   if (config & kCar10)  config_name += "+CALO";
   if (config & kCar11)  config_name += "+MUONTrig";
   return config_name.Data();
}

//______________________________________________________________________________
void LoadLibraries()
{
  gSystem->Load("libANALYSIS");
  gSystem->Load("libANALYSISilc");
  gSystem->Load("libCORRFW");
  gSystem->Load("libTENDER");
  gSystem->Load("libTENDERSupplies");
  gSystem->Load("libPWG0base.so");
  gSystem->Load("libPWG0dep.so");
  gSystem->Load("libPWG0selectors.so");
  gSystem->Load("libPWGPP.so");
  gSystem->Load("libPWG2.so");
  gSystem->Load("libPWG2forward.so");
  gSystem->Load("libEMCALUtils");
  gSystem->Load("libPWG4PartCorrBase");
  gSystem->Load("libPWG4PartCorrDep");
  gSystem->Load("libPWGHFbase");
  gSystem->Load("libPWGmuon");
  gSystem->Load("libPWGmuondep");
// fast xrootd reading enabled
   printf("!!! You requested FastRead option. Using xrootd flags to reduce timeouts. Note that this may skip some files that could be accessed !!!");
   gEnv->SetValue("XNet.ConnectTimeout",10);
   gEnv->SetValue("XNet.RequestTimeout",10);
   gEnv->SetValue("XNet.MaxRedirectCount",2);
   gEnv->SetValue("XNet.ReconnectTimeout",10);
   gEnv->SetValue("XNet.FirstConnectMaxCnt",1);
}

//________________________________________________________________________________
TChain* CreateChain(const char *xmlfile, const char *type)
{
// Create a chain using url's from xml file
   TString treename = type;
   treename.ToLower();
   treename += "Tree";
   printf("***************************************\n");
   printf("    Getting chain of trees %s\n", treename.Data());
   printf("***************************************\n");
   TAlienCollection *coll = TAlienCollection::Open(xmlfile);
   if (!coll) {
      ::Error("CreateChain", "Cannot create an AliEn collection from %s", xmlfile);
      return NULL;
   }
   TChain *chain = new TChain(treename);
   coll->Reset();
   while (coll->Next()) {
      chain->Add(coll->GetTURL(""));
   }
   if (!chain->GetNtrees()) {
      ::Error("CreateChain", "No tree found from collection %s", xmlfile);
      return NULL;
   }
   return chain;
}

#define TENDER

void runProofNormalization(const char * dataset = "LHC09b12_7TeV_0.5T", TString dataSetPath ="/PWG0/jgrosseo/",const char * filename = "LHC09b12_7TeV_0.5T_norm.root", Bool_t isMC = 1,Int_t nev =123456789) {

#ifdef TENDER
  TGrid::Connect("alien://");
#endif
  gEnv->SetValue("XSec.GSI.DelegProxy","2");
  TProof::Open("ilc-caf","workers=1x");// limit the number of workers
  //  gROOT->ProcessLine(Form(".include %s/include",gSystem->ExpandPathName("$ILC_ROOT")));
  //  gSystem->AddIncludePath("-I${ILC_ROOT}/include/ -I${ILC_ROOT}/PWG0/ -I${ILC_ROOT}/PWG0/dNdEta/");
  //  gSystem->AddIncludePath("-I${ILC_ROOT}/include/");
    // Use a precompiled tag
    TString ilcrootMode="";    // STEERBase,ESD,AOD,ANALYSIS,ANALYSISilc (default ilcroot mode)
    //ilcrootMode="ILCROOT";     // $ILC_ROOT/macros/loadlibs.C
    //  ilcrootMode="REC";     // $ILC_ROOT/macros/loadlibsrec.C
    //  ilcrootMode="SIM";     // $ILC_ROOT/macros/loadlibssim.C
    //  ilcrootMode="TRAIN";   // $ILC_ROOT/macros/loadlibstrain.C (not working yet)
    //  ilcrootMode="CUSTOM";  // nothing is loaded, but ilcroot variables are set (not working yet)
 
    TString extraLibs;
    extraLibs= ""; // not needed in default ilcroot mode
    extraLibs+="CDB:RAWDatabase:STEER:TENDER:TRDbase:STAT:TRDrec:VZERObase:VZEROsim:VZEROrec:RAWDatarec:TPCbase:TPCrec:TPCcalib:TENDERSupplies:RAWDatabase:RAWDatarec:RAWDatasim:TOFbase:TOFrec";
    TList *list = new TList();
    // sets $ILCROOT_MODE on each worker to let proof to know to run in special mode
    list->Add(new TNamed("ILCROOT_MODE", ilcrootMode.Data()));
    // sets $ILCROOT_EXTRA_LIBS on each worker to let proof to know to load extra libs
    list->Add(new TNamed("ILCROOT_EXTRA_LIBS", extraLibs.Data()));
#ifdef TENDER
    list->Add(new TNamed("ILCROOT_ENABLE_ALIEN", "1"));
#endif
    // connect to proof
    gProof->EnablePackage("VO_ILC@IlcRoot::v4-21-22-AN", list);
    //    gProof->Exec("TGrid::Connect(\"alien://\");");
  // gProof->UploadPackage("$ILC_ROOT/obj/STEERBase");
  // gProof->EnablePackage("$ILC_ROOT/obj/STEERBase");
  // gProof->UploadPackage("$ILC_ROOT/obj/ESD");
  // gProof->EnablePackage("$ILC_ROOT/obj/ESD");
  // gProof->UploadPackage("$ILC_ROOT/obj/AOD");
  // gProof->EnablePackage("$ILC_ROOT/obj/AOD");
  // gProof->UploadPackage("$ILC_ROOT/obj/ANALYSIS");
  // gProof->EnablePackage("$ILC_ROOT/obj/ANALYSIS");
  // gProof->UploadPackage("$ILC_ROOT/obj/OADB");
  // gProof->EnablePackage("$ILC_ROOT/obj/OADB");
  // gProof->UploadPackage("$ILC_ROOT/obj/ANALYSISilc");
  // gProof->EnablePackage("$ILC_ROOT/obj/ANALYSISilc");
//   gProof->UploadPackage("STEERBase.par");
//   gProof->EnablePackage("STEERBase");
//   gProof->UploadPackage("ESD.par");
//   gProof->EnablePackage("ESD");
//   gProof->UploadPackage("AOD.par");
//   gProof->EnablePackage("AOD");
//   gProof->UploadPackage("ANALYSIS.par");
//   gProof->EnablePackage("ANALYSIS");
//   gProof->UploadPackage("ANALYSISilc.par");
//   gProof->EnablePackage("ANALYSISilc");
//   gProof->UploadPackage("CORRFW.par");
//   gProof->EnablePackage("CORRFW"); 

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
  //____________________________________________//

#ifdef TENDER
  gROOT->LoadMacro("$ILC_ROOT/ANALYSIS/TenderSupplies/AddTaskTender.C");
  IlcAnalysisTask* tender=0x0;
  if(!isMC)
    {
      tender = AddTaskTender(kTRUE);
      // tender->SetDebugLevel(10);
    }
  else
    {
      tender = AddTaskTender(kFALSE);
      // tender->SetDebugLevel(10);
    }
#endif

  gROOT->LoadMacro("$(ILC_ROOT)/ANALYSIS/macros/AddTaskPhysicsSelection.C");
  IlcPhysicsSelectionTask* physSelTask = AddTaskPhysicsSelection(isMC,1,!isMC); // Use Physics Selection. Enable computation of BG if is not MC
  //  task->SelectCollisionCandidates(); /// This should be disabled, at least for MC: we need all the events
  physSelTask->GetPhysicsSelection()->SetBin0Callback("TaskNormalization");

  // assign simple task
  IlcCollisionNormalizationTask * task = new IlcCollisionNormalizationTask("TaskNormalization");
  //  task->SetMC();
  task->SetMC(isMC);
  mgr->AddTask(task);



  IlcAnalysisDataContainer *cinput1 = mgr->GetCommonInputContainer();	
  mgr->ConnectInput(task,0,cinput1);


  
  // Attach output
  cOutput = mgr->CreateContainer("Norm", TList::Class(), IlcAnalysisManager::kOutputContainer,filename);
  mgr->ConnectOutput(task, 1, cOutput);      
	
  if (!mgr->InitAnalysis()) return;
	
  mgr->PrintStatus();
  mgr->StartAnalysis("proof",dataSetPath+dataset+"#esdTree",nev);

}

//DEFINITION OF A FEW CONSTANTS
const Double_t ymin  = -1.0 ;
const Double_t ymax  =  1.0 ;
const Double_t ptmin =  0.0 ;
const Double_t ptmax =  8.0 ;
const Int_t    mintrackrefsTPC = 2 ;
const Int_t    mintrackrefsITS = 3 ;
const Int_t    charge  = 1 ;
const Int_t    PDG = 2212; 
const Int_t    minclustersTPC = 50 ;
//----------------------------------------------------

Bool_t IlcCFSingleTrackTaskCAF()
{
  
  TBenchmark benchmark;
  benchmark.Start("IlcSingleTrackTaskCAF");

  //
  // Connect to proof
  //

  TProof::Reset("proof://user@lxb6046.cern.ch");
  TProof::Open("proof://user@lxb6046.cern.ch");

  //   gProof->ClearPackage("STEERBase");
  //   gProof->ClearPackage("ESD");
  //   gProof->ClearPackage("AOD");
  //   gProof->ClearPackage("ANALYSIS");
  //   gProof->ClearPackage("ANALYSISilc");

  // Enable the STEERBase Package
  gProof->UploadPackage("STEERBase.par");
  gProof->EnablePackage("STEERBase");
  // Enable the ESD Package
  gProof->UploadPackage("ESD.par");
  gProof->EnablePackage("ESD");
  // Enable the AOD Package
  gProof->UploadPackage("AOD.par");
  gProof->EnablePackage("AOD");
  // Enable the Analysis Package
  gProof->UploadPackage("ANALYSIS.par");
  gProof->EnablePackage("ANALYSIS");
  gProof->UploadPackage("ANALYSISilc.par");
  gProof->EnablePackage("ANALYSISilc");
  // Enable the CORRFW Package
  // gProof->ClearPackage("CORRFW");
  gProof->UploadPackage("CORRFW.par");
  gProof->EnablePackage("CORRFW");

  gProof->ShowEnabledPackages();
  gProof->Load("./IlcCFSingleTrackTask.cxx+g");

  //
  // Create the chain
  //
  gROOT->LoadMacro("CreateESDChain.C");
  TChain* analysisChain = CreateESDChain("ESD1503X_v1.txt", 2);


  //CONTAINER DEFINITION
  Info("IlcCFSingleTrackTaskCAF","SETUP CONTAINER");
  //the sensitive variables (2 in this example), their indices
  UInt_t ipt = 0;
  UInt_t iy  = 1;
  //Setting up the container grid... 
  UInt_t nstep = 4 ; //number of selection steps MC 
  const Int_t nvar   = 2 ; //number of variables on the grid:pt,y
  const Int_t nbin1  = 8 ; //bins in pt
  const Int_t nbin2  = 8 ; //bins in y 

  //arrays for the number of bins in each dimension
  Int_t iBin[nvar];
  iBin[0]=nbin1;
  iBin[1]=nbin2;

  //arrays for lower bounds :
  Double_t *binLim1=new Double_t[nbin1+1];
  Double_t *binLim2=new Double_t[nbin2+1];

  //values for bin lower bounds
  for(Int_t i=0; i<=nbin1; i++) binLim1[i]=(Double_t)ptmin + (ptmax-ptmin)/nbin1*(Double_t)i ; 
  for(Int_t i=0; i<=nbin2; i++) binLim2[i]=(Double_t)ymin  + (ymax-ymin)  /nbin2*(Double_t)i ;
  //one "container" for MC
  IlcCFContainer* container = new IlcCFContainer("container","container for tracks",nstep,nvar,iBin);
  //setting the bin limits
  container -> SetBinLimits(ipt,binLim1);
  container -> SetBinLimits(iy,binLim2);


  // SET TLIST FOR QA HISTOS
  TList* qaList = new TList();

  //CREATE THE  CUTS -----------------------------------------------

  // Gen-Level kinematic cuts
  IlcCFTrackKineCuts *mcKineCuts = new IlcCFTrackKineCuts("mcKineCuts","MC-level kinematic cuts");
  mcKineCuts->SetPtRange(ptmin,ptmax);
  mcKineCuts->SetRapidityRange(ymin,ymax);
  mcKineCuts->SetChargeMC(charge);
  mcKineCuts->SetQAOn(qaList);

  //Particle-Level cuts:  
  IlcCFParticleGenCuts* mcGenCuts = new IlcCFParticleGenCuts("mcGenCuts","MC particle generation cuts");
  mcGenCuts->SetRequireIsPrimary();
  mcGenCuts->SetRequirePdgCode(PDG);
  mcGenCuts->SetQAOn(qaList);

  //Acceptance Cuts
  IlcCFAcceptanceCuts *mcAccCuts = new IlcCFAcceptanceCuts("mcAccCuts","MC acceptance cuts");
  mcAccCuts->SetMinNHitITS(mintrackrefsITS);
  mcAccCuts->SetMinNHitTPC(mintrackrefsTPC);
  mcAccCuts->SetQAOn(qaList);

  // Rec-Level kinematic cuts
  IlcCFTrackKineCuts *recKineCuts = new IlcCFTrackKineCuts("recKineCuts","rec-level kine cuts");
  recKineCuts->SetPtRange(ptmin,ptmax);
  recKineCuts->SetRapidityRange(ymin,ymax);
  recKineCuts->SetChargeRec(charge);
  recKineCuts->SetQAOn(qaList);

  IlcCFTrackQualityCuts *recQualityCuts = new IlcCFTrackQualityCuts("recQualityCuts","rec-level quality cuts");
  recQualityCuts->SetMinNClusterTPC(minclustersTPC);
  recQualityCuts->SetRequireITSRefit(kTRUE);
  recQualityCuts->SetQAOn(qaList);

  IlcCFTrackIsPrimaryCuts *recIsPrimaryCuts = new IlcCFTrackIsPrimaryCuts("recIsPrimaryCuts","rec-level isPrimary cuts");
  recIsPrimaryCuts->SetMaxNSigmaToVertex(3);
  recIsPrimaryCuts->SetQAOn(qaList);

  IlcCFTrackCutPid* cutPID = new IlcCFTrackCutPid("cutPID","ESD_PID") ;
  int n_species = IlcPID::kSPECIES ;
  Double_t* prior = new Double_t[n_species];
  
  prior[0] = 0.0244519 ;
  prior[1] = 0.0143988 ;
  prior[2] = 0.805747  ;
  prior[3] = 0.0928785 ;
  prior[4] = 0.0625243 ;
  
  cutPID->SetPriors(prior);
  cutPID->SetProbabilityCut(0.0);
  cutPID->SetDetectors("TPC TOF");
  switch(TMath::Abs(PDG)) {
  case 11   : cutPID->SetParticleType(IlcPID::kElectron, kTRUE); break;
  case 13   : cutPID->SetParticleType(IlcPID::kMuon    , kTRUE); break;
  case 211  : cutPID->SetParticleType(IlcPID::kPion    , kTRUE); break;
  case 321  : cutPID->SetParticleType(IlcPID::kKaon    , kTRUE); break;
  case 2212 : cutPID->SetParticleType(IlcPID::kProton  , kTRUE); break;
  default   : printf("UNDEFINED PID\n"); break;
  }
  cutPID->SetQAOn(qaList);

  printf("CREATE MC KINE CUTS\n");
  TObjArray* mcList = new TObjArray(0) ;
  mcList->AddLast(mcKineCuts);
  mcList->AddLast(mcGenCuts);

  printf("CREATE ACCEPTANCE CUTS\n");
  TObjArray* accList = new TObjArray(0) ;
  accList->AddLast(mcAccCuts);

  printf("CREATE RECONSTRUCTION CUTS\n");
  TObjArray* recList = new TObjArray(0) ;
  recList->AddLast(recKineCuts);
  recList->AddLast(recQualityCuts);
  recList->AddLast(recIsPrimaryCuts);

  printf("CREATE PID CUTS\n");
  TObjArray* fPIDCutList = new TObjArray(0) ;
  fPIDCutList->AddLast(cutPID);

  //CREATE THE INTERFACE TO CORRECTION FRAMEWORK USED IN THE TASK
  printf("CREATE INTERFACE AND CUTS\n");
  IlcCFManager* man = new IlcCFManager() ;
  man->SetParticleContainer     (container);
  man->SetParticleCutsList(IlcCFManager::kPartGenCuts,mcList);
  man->SetParticleCutsList(IlcCFManager::kPartAccCuts,accList);
  man->SetParticleCutsList(IlcCFManager::kPartRecCuts,recList);
  man->SetParticleCutsList(IlcCFManager::kPartSelCuts,fPIDCutList);


  //CREATE THE TASK
  printf("CREATE TASK\n");
  // create the task
  IlcCFSingleTrackTask *task = new IlcCFSingleTrackTask("IlcSingleTrackTask");
  task->SetCFManager(man); //here is set the CF manager
  task->SetQAList(qaList);

  //SETUP THE ANALYSIS MANAGER TO READ INPUT CHAIN AND WRITE DESIRED OUTPUTS
  printf("CREATE ANALYSIS MANAGER\n");
  // Make the analysis manager
  IlcAnalysisManager *mgr = new IlcAnalysisManager("TestManager");

  IlcMCEventHandler*  mcHandler = new IlcMCEventHandler();
  IlcESDInputHandler* esdHandler = new IlcESDInputHandler();
  mgr->SetMCtruthEventHandler(mcHandler);
  mgr->SetInputEventHandler(esdHandler);

  // Create and connect containers for input/output

  //------ input data ------
  IlcAnalysisDataContainer *cinput0  = mgr->CreateContainer("cchain0",TChain::Class(),IlcAnalysisManager::kInputContainer);

  // ----- output data -----
  
  //slot 0 : default output tree (by default handled by IlcAnalysisTaskSE)
  IlcAnalysisDataContainer *coutput0 = mgr->CreateContainer("ctree0", TTree::Class(),IlcAnalysisManager::kOutputContainer,"output.root");

  //now comes user's output objects :
  
  // output TH1I for event counting
  IlcAnalysisDataContainer *coutput1 = mgr->CreateContainer("chist0", TH1I::Class(),IlcAnalysisManager::kOutputContainer,"output.root");
  // output Correction Framework Container (for acceptance & efficiency calculations)
  IlcAnalysisDataContainer *coutput2 = mgr->CreateContainer("ccontainer0", IlcCFContainer::Class(),IlcAnalysisManager::kOutputContainer,"output.root");
  // output QA histograms 
  IlcAnalysisDataContainer *coutput3 = mgr->CreateContainer("clist0", TList::Class(),IlcAnalysisManager::kOutputContainer,"output.root");

  cinput0->SetData(analysisChain);

  mgr->AddTask(task);
  mgr->ConnectInput(task,0,cinput0);
  mgr->ConnectOutput(task,0,coutput0);
  mgr->ConnectOutput(task,1,coutput1);
  mgr->ConnectOutput(task,2,coutput2);
  mgr->ConnectOutput(task,3,coutput3);

  printf("READY TO RUN\n");
  //RUN !!!
  if (mgr->InitAnalysis()) {
    mgr->PrintStatus();
    mgr->StartAnalysis("proof",analysisChain);
  }

  benchmark.Stop("IlcSingleTrackTaskCAF");
  benchmark.Show("IlcSingleTrackTaskCAF");

  return kTRUE ;
}


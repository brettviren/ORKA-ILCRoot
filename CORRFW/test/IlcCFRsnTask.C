//DEFINITION OF A FEW CONSTANTS
const Double_t   ymin  = -1.0 ;
const Double_t   ymax  =  1.0 ;
const Double_t   ptmin =  0.0 ;
const Double_t   ptmax =  8.0 ;
const Int_t      mintrackrefsTPC = 2 ;
const Int_t      mintrackrefsITS = 3 ;
const Int_t      charge  = 0 ;
const Int_t      PDG = 313; 
const Int_t      minclustersTPC = 50 ;
const Double32_t nsigmavtx = 3. ;       //max track sigma to PVertex
//----------------------------------------------------

Bool_t IlcCFRsnTask(
		    const Bool_t useGrid = 1,
		    const Bool_t readAOD = 0,
		    const char * kTagXMLFile="wn.xml", // XML file containing tags
		    )
{
  
  TBenchmark benchmark;
  benchmark.Start("IlcRsnTask");

  IlcLog::SetGlobalDebugLevel(0);

  Load(useGrid) ; //load the required libraries

  TChain * analysisChain ;

  if (useGrid) { //data located on AliEn
    TGrid::Connect("alien://") ;    //  Create an IlcRunTagCuts and an IlcEventTagCuts Object and impose some selection criteria
    IlcRunTagCuts      *runCuts   = new IlcRunTagCuts(); 
    IlcEventTagCuts    *eventCuts = new IlcEventTagCuts(); 
    IlcLHCTagCuts      *lhcCuts   = new IlcLHCTagCuts(); 
    IlcDetectorTagCuts *detCuts   = new IlcDetectorTagCuts(); 
    eventCuts->SetMultiplicityRange(0,20000);
    //  Create an IlcTagAnalysis Object and chain the tags
    IlcTagAnalysis   *tagAna = new IlcTagAnalysis(); 
    tagAna->SetType("ESD");  //for ilcroot > v4-05
    TAlienCollection *coll   = TAlienCollection::Open(kTagXMLFile); 
    TGridResult      *tagResult = coll->GetGridResult("",0,0);
    tagResult->Print();
    tagAna->ChainGridTags(tagResult);
    //  Create a new esd chain and assign the chain that is returned by querying the tags
    analysisChain = tagAna->QueryTags(runCuts,lhcCuts,detCuts,eventCuts); 
  }
  else {// local data
    //here put your input data path
    if (readAOD) {
      analysisChain = new TChain("aodTree");
      analysisChain->Add("your_data_path/001/IlcAOD.root");
      analysisChain->Add("your_data_path/002/IlcAOD.root");
    }
    else {
      analysisChain = new TChain("esdTree");
      analysisChain->Add("your_data_path/001/IlcESDs.root");
      analysisChain->Add("your_data_path/002/IlcESDs.root");
    }
  }
  
  
  Info("IlcCFRsnTask",Form("CHAIN HAS %d ENTRIES",(Int_t)analysisChain->GetEntries()));

  //CONTAINER DEFINITION
  Info("IlcCFRsnTask","SETUP CONTAINER");
  //the sensitive variables (2 in this example), their indices
  Int_t ipt = 0;
  Int_t iy  = 1;
  //Setting up the container grid... 
  Int_t nstep = 4 ; //number of selection steps MC 
  const Int_t nvar   = 2 ; //number of variables on the grid:pt,y
  const Int_t nbin1  = 8 ; //bins in pt
  const Int_t nbin2  = 8 ; //bins in y 
  //arrays for the number of bins in each dimension
  const Int_t iBin[nvar] ={nbin1,nbin2};
  //arrays for lower bounds :
  Double_t binLim1[nbin1+1];
  Double_t binLim2[nbin2+1];
  //values for bin lower bounds
  for(Int_t i=0; i<=nbin1; i++) binLim1[i]=(Double_t)ptmin + (ptmax-ptmin)/nbin1*(Double_t)i ; 
  for(Int_t i=0; i<=nbin2; i++) binLim2[i]=(Double_t)ymin  + (ymax-ymin)  /nbin2*(Double_t)i ;
  //one "container" for MC
  IlcCFContainer* container = new IlcCFContainer("container","container for tracks",nstep,nvar,iBin);
  //setting the bin limits
  container -> SetBinLimits(ipt,binLim1);
  container -> SetBinLimits(iy,binLim2);


  //CREATE THE  CUTS -----------------------------------------------
  //Particle-Level cuts:  

  // Gen-Level kinematic cuts
  IlcCFTrackKineCuts *mcKineCuts = new IlcCFTrackKineCuts("mcKineCuts","MC-level kinematic cuts");
  mcKineCuts->SetPtRange(ptmin,ptmax);
  mcKineCuts->SetRapidityRange(ymin,ymax);
  mcKineCuts->SetChargeMC(charge);

  IlcCFParticleGenCuts* mcGenCuts = new IlcCFParticleGenCuts("mcGenCuts","MC particle generation cuts");
  //  mcGenCuts->SetRequireIsPrimary();
  mcGenCuts->SetRequirePdgCode(PDG);

  //Acceptance Cuts
  IlcCFPairAcceptanceCuts *mcAccCuts = new IlcCFPairAcceptanceCuts("mcAccCuts","MC acceptance cuts");
  mcAccCuts->GetNegCut()->SetMinNHitITS(mintrackrefsITS);
  mcAccCuts->GetPosCut()->SetMinNHitITS(mintrackrefsITS);
  mcAccCuts->GetNegCut()->SetMinNHitTPC(mintrackrefsTPC);
  mcAccCuts->GetPosCut()->SetMinNHitTPC(mintrackrefsTPC);

  // Rec-Level kinematic cuts
  IlcCFTrackKineCuts *recKineCuts = new IlcCFTrackKineCuts("recKineCuts","rec-level kine cuts");
  recKineCuts->SetPtRange(ptmin,ptmax);
  recKineCuts->SetRapidityRange(ymin,ymax);
  recKineCuts->SetChargeRec(charge);

  IlcCFPairQualityCuts *recQualityCuts = new IlcCFPairQualityCuts("recQualityCuts","rec-level quality cuts");
  if (!readAOD) {
    recQualityCuts->GetNegCut()->SetMinNClusterTPC(minclustersTPC);
    recQualityCuts->GetPosCut()->SetMinNClusterTPC(minclustersTPC);
  }
  recQualityCuts->GetNegCut()->SetStatus(IlcESDtrack::kTPCrefit & IlcESDtrack::kITSrefit);
  recQualityCuts->GetPosCut()->SetStatus(IlcESDtrack::kTPCrefit & IlcESDtrack::kITSrefit);


  IlcCFPairIsPrimaryCuts *recIsPrimaryCuts = new IlcCFPairIsPrimaryCuts("recIsPrimaryCuts","rec-level isPrimary cuts");
  if (readAOD) {
    recIsPrimaryCuts->GetNegCut()->SetAODType(IlcAODTrack::kPrimary);
    recIsPrimaryCuts->GetPosCut()->SetAODType(IlcAODTrack::kPrimary);
  }
  else {
    recIsPrimaryCuts->GetNegCut()->SetMaxNSigmaToVertex(nsigmavtx);
    recIsPrimaryCuts->GetPosCut()->SetMaxNSigmaToVertex(nsigmavtx);
  }

  IlcCFPairPidCut* cutPID = new IlcCFPairPidCut("cutPID","ESD_PID") ;
  Double_t prior_pp[IlcPID::kSPECIES] = {0.0244519,
					 0.0143988,
					 0.805747 ,
					 0.0928785,
					 0.0625243 };

  Double_t prior_pbpb[IlcPID::kSPECIES] = {0.0609,
					   0.1064,
					   0.7152 ,
					   0.0442,
					   0.0733 };


  cutPID->GetNegCut()->SetPriors(prior_pbpb);
  cutPID->GetPosCut()->SetPriors(prior_pbpb);
  cutPID->GetNegCut()->SetProbabilityCut(0);
  cutPID->GetPosCut()->SetProbabilityCut(0);
  cutPID->GetNegCut()->SetDetectors("ITS TPC TRD");
  cutPID->GetPosCut()->SetDetectors("ITS TPC TRD");
  if (readAOD) {
    cutPID->GetNegCut()->SetAODmode(kTRUE);
    cutPID->GetPosCut()->SetAODmode(kTRUE);
  }
  else {
    cutPID->GetNegCut()->SetAODmode(kFALSE);
    cutPID->GetPosCut()->SetAODmode(kFALSE);
  }
  
  switch(PDG) {
  case  -313  : 
    cutPID->GetNegCut()->SetParticleType(IlcPID::kKaon  ,kTRUE);
    cutPID->GetPosCut()->SetParticleType(IlcPID::kPion  ,kTRUE);
    break;
  case   313  : 
    cutPID->GetNegCut()->SetParticleType(IlcPID::kPion  ,kTRUE);
    cutPID->GetPosCut()->SetParticleType(IlcPID::kKaon  ,kTRUE); 
    break;
  case   333  : 
    cutPID->GetNegCut()->SetParticleType(IlcPID::kKaon  ,kTRUE);
    cutPID->GetPosCut()->SetParticleType(IlcPID::kKaon  ,kTRUE); 
    break;
  case  3124  : 
    cutPID->GetNegCut()->SetParticleType(IlcPID::kKaon  ,kTRUE);
    cutPID->GetPosCut()->SetParticleType(IlcPID::kProton,kTRUE); 
    break;
  case -3124  : 
    cutPID->GetNegCut()->SetParticleType(IlcPID::kProton,kTRUE);
    cutPID->GetPosCut()->SetParticleType(IlcPID::kKaon  ,kTRUE); 
    break;
  default     : printf("UNDEFINED PID\n"); break;
  }
  //cutPID->SetQAOn(kTRUE);

  Info("IlcCFRsnTask","CREATE MC KINE CUTS");
  TObjArray* mcList = new TObjArray(0) ;
  mcList->AddLast(mcKineCuts);
  mcList->AddLast(mcGenCuts);

  Info("IlcCFRsnTask","CREATE ACCEPTANCE CUTS");
  TObjArray* accList = new TObjArray(0) ;
  accList->AddLast(mcAccCuts);

  Info("IlcCFRsnTask","CREATE RECONSTRUCTION CUTS");
  TObjArray* recList = new TObjArray(0) ;
  recList->AddLast(recKineCuts);
  recList->AddLast(recQualityCuts);
  recList->AddLast(recIsPrimaryCuts);

  Info("IlcCFRsnTask","CREATE PID CUTS");
  TObjArray* fPIDCutList = new TObjArray(0) ;
  fPIDCutList->AddLast(cutPID);


  //CREATE THE INTERFACE TO CORRECTION FRAMEWORK USED IN THE TASK
  Info("IlcCFRsnTask","CREATE INTERFACE AND CUTS");
  IlcCFManager* man = new IlcCFManager() ;
  man->SetParticleContainer     (container);
  man->SetParticleCutsList(IlcCFManager::kPartGenCuts,mcList);
  man->SetParticleCutsList(IlcCFManager::kPartAccCuts,accList);
  man->SetParticleCutsList(IlcCFManager::kPartRecCuts,recList);
  man->SetParticleCutsList(IlcCFManager::kPartSelCuts,fPIDCutList);


  //CREATE THE TASK
  Info("IlcCFRsnTask","CREATE TASK");
  // create the task
  IlcCFRsnTask *task = new IlcCFRsnTask("IlcRsnTask");
  task->SetCFManager(man); //here is set the CF manager
  task->SetRsnPDG(PDG);


  //SETUP THE ANALYSIS MANAGER TO READ INPUT CHAIN AND WRITE DESIRED OUTPUTS
  Info("IlcCFRsnTask","CREATE ANALYSIS MANAGER");
  // Make the analysis manager
  IlcAnalysisManager *mgr = new IlcAnalysisManager("TestManager");

  if (useGrid) mgr->SetAnalysisType(IlcAnalysisManager::kGridAnalysis);
  else mgr->SetAnalysisType(IlcAnalysisManager::kLocalAnalysis);

  IlcMCEventHandler* mcHandler = new IlcMCEventHandler();
  mgr->SetMCtruthEventHandler(mcHandler);

  IlcInputEventHandler* dataHandler ;
  if (readAOD) dataHandler = new IlcAODInputHandler();
  else         dataHandler = new IlcESDInputHandler();
  mgr->SetInputEventHandler(dataHandler);
  


  // Create and connect containers for input/output

  //input data
  IlcAnalysisDataContainer *cinput0  = mgr->CreateContainer("cchain0",TChain::Class(),IlcAnalysisManager::kInputContainer);

  //slot 0 : default output tree (by default handled by IlcAnalysisTaskSE)
  IlcAnalysisDataContainer *coutput0 = mgr->CreateContainer("ctree0", TTree::Class(),IlcAnalysisManager::kOutputContainer,"output_rsn.root");

  // output histo (number of events processed)
  IlcAnalysisDataContainer *coutput1 = mgr->CreateContainer("chist0", TH1I::Class(),IlcAnalysisManager::kOutputContainer,"output_rsn.root");
  // output Correction Framework Container (for acceptance & efficiency calculations)
  IlcAnalysisDataContainer *coutput2 = mgr->CreateContainer("ccontainer0", IlcCFContainer::Class(),IlcAnalysisManager::kOutputContainer,"output_rsn.root");

  cinput0->SetData(analysisChain);

  mgr->AddTask(task);
  mgr->ConnectInput (task,0,mgr->GetCommonInputContainer());
  mgr->ConnectOutput(task,0,coutput0);
  mgr->ConnectOutput(task,1,coutput1);
  mgr->ConnectOutput(task,2,coutput2);
 

  Info("IlcCFRsnTask","READY TO RUN");
  //RUN !!!
  if (mgr->InitAnalysis()) {
    mgr->PrintStatus();
    mgr->StartAnalysis("local",analysisChain);
  }

  benchmark.Stop("IlcRsnTask");
  benchmark.Show("IlcRsnTask");
  
  return kTRUE ;
}

void Load(Bool_t useGrid) {
  //load the required ilcroot libraries
  gSystem->Load("libANALYSIS") ;
  gSystem->Load("libANALYSISilc") ;
  gSystem->Load("libPWG2resonances");
  gSystem->Load("libCORRFW");

  gSystem->AddIncludePath("-I$ILC_ROOT/PWG2/RESONANCES");
  gROOT->LoadMacro("./IlcCFRsnTask.cxx+");
}

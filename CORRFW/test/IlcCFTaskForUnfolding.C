//DEFINITION OF A FEW CONSTANTS
const Double_t ptmin  =   0.0 ;
const Double_t ptmax  =   1.0 ;
const Double_t etamin =  -1.5 ;
const Double_t etamax =   1.5 ;
const Int_t    PDG    =    211; 
const Int_t    minclustersTPC = 40 ;
//----------------------------------------------------

/*

Macro to prepare the necessary objects to perform spectrum unfolding 
(to be used by the class IlcCFUnfolding)

Note that the efficiency calculation (and therfore the container filling) 
has to be done using MC values (and not reconstructed values)

This macro creates the following objects :
- (IlcCFContainer) container    : used to calculate the efficiency (see IlcCFEffGrid)
- (THnSparseD)     correlation  : this is the response matrix

Once you have run this macro, you may launch unfolding using 
the example macro CORRFW/test/testUnfolding.C

*/

Bool_t IlcCFTaskForUnfolding()
{
  
  TBenchmark benchmark;
  benchmark.Start("IlcSingleTrackTask");

  IlcLog::SetGlobalDebugLevel(0);

  Load() ; //load the required libraries

  TChain * analysisChain ;
  printf("\n\nRunning on local file, please check the path\n\n");

  analysisChain = new TChain("esdTree");
  analysisChain->Add("your_data_path/001/IlcESDs.root");
  analysisChain->Add("your_data_path/002/IlcESDs.root");

  
  Info("IlcCFTaskForUnfolding",Form("CHAIN HAS %d ENTRIES",(Int_t)analysisChain->GetEntries()));

  //CONTAINER DEFINITION
  Info("IlcCFTaskForUnfolding","SETUP CONTAINER");
  //the sensitive variables (2 in this example), their indices
  UInt_t ipt = 0;
  UInt_t ieta  = 1;
  //Setting up the container grid... 
  UInt_t nstep = 3 ; //number of selection steps MC 
  const Int_t nvar   = 2 ; //number of variables on the grid:pt,eta
  const Int_t nbin[nvar] = {20,20} ;

  //arrays for the number of bins in each dimension
  Int_t iBin[nvar];
  iBin[0]=nbin[0];
  iBin[1]=nbin[1];

  //arrays for lower bounds :
  Double_t *binLim0=new Double_t[nbin[0]+1];
  Double_t *binLim1=new Double_t[nbin[1]+1];

  //values for bin lower bounds
  for(Int_t i=0; i<=nbin[0]; i++) binLim0[i]=(Double_t)ptmin  + ( ptmax- ptmin)/nbin[0]*(Double_t)i ; 
  for(Int_t i=0; i<=nbin[1]; i++) binLim1[i]=(Double_t)etamin + (etamax-etamin)/nbin[1]*(Double_t)i ; 

  //one "container" for MC
  IlcCFContainer* container = new IlcCFContainer("container","container for tracks",nstep,nvar,iBin);
  //setting the bin limits
  container -> SetBinLimits(ipt,binLim0);
  container -> SetBinLimits(ieta ,binLim1);
  container -> SetVarTitle(ipt,"pT");
  container -> SetVarTitle(ieta,"#eta");

  // Gen-Level kinematic cuts
  IlcCFTrackKineCuts *mcKineCuts = new IlcCFTrackKineCuts("mcKineCuts","MC-level kinematic cuts");
  mcKineCuts->SetPtRange (ptmin ,ptmax );
  mcKineCuts->SetEtaRange(etamin,etamax);

  //Particle-Level cuts:  
  IlcCFParticleGenCuts* mcGenCuts = new IlcCFParticleGenCuts("mcGenCuts","MC particle generation cuts");
  mcGenCuts->SetRequireIsPrimary();
  mcGenCuts->SetRequirePdgCode(PDG);

  // Rec-Level kinematic cuts
  IlcCFTrackKineCuts *recKineCuts = new IlcCFTrackKineCuts("recKineCuts","rec-level kine cuts");
  recKineCuts->SetPtRange( ptmin, ptmax);
  recKineCuts->SetPtRange(etamin,etamax);

  IlcCFTrackQualityCuts *recQualityCuts = new IlcCFTrackQualityCuts("recQualityCuts","rec-level quality cuts");
  recQualityCuts->SetMinNClusterTPC(minclustersTPC);

  IlcCFTrackIsPrimaryCuts *recIsPrimaryCuts = new IlcCFTrackIsPrimaryCuts("recIsPrimaryCuts","rec-level isPrimary cuts");
  recIsPrimaryCuts->SetMaxNSigmaToVertex(3);

  printf("CREATE MC KINE CUTS\n");
  TObjArray* mcList = new TObjArray(0) ;
  mcList->AddLast(mcKineCuts);
  mcList->AddLast(mcGenCuts);

  printf("CREATE RECONSTRUCTION CUTS\n");
  TObjArray* recList = new TObjArray(0) ;
  recList->AddLast(recKineCuts);
  recList->AddLast(recQualityCuts);
  recList->AddLast(recIsPrimaryCuts);

  TObjArray* emptyList = new TObjArray(0);

  //CREATE THE INTERFACE TO CORRECTION FRAMEWORK USED IN THE TASK
  printf("CREATE INTERFACE AND CUTS\n");
  IlcCFManager* man = new IlcCFManager() ;
  man->SetNStepEvent(0);
  man->SetParticleContainer(container);
  man->SetParticleCutsList(0,mcList);
  man->SetParticleCutsList(1,recList);
  man->SetParticleCutsList(2,emptyList); // this is special step for monte carlo spectrum

  //CREATE THE TASK
  printf("CREATE TASK\n");
  // create the task
  IlcCFTaskForUnfolding *task = new IlcCFTaskForUnfolding("IlcCFTaskForUnfolding");
  task->SetCFManager(man); //here is set the CF manager

  //create correlation matrix for unfolding
  Int_t thnDim[2*nvar];
  for (int k=0; k<nvar; k++) {
    //first half  : reconstructed 
    //second half : MC
    thnDim[k]      = nbin[k];
    thnDim[k+nvar] = nbin[k];
  }
  THnSparseD* correlation = new THnSparseD("correlation","THnSparse with correlations",2*nvar,thnDim);
  Double_t** binEdges = new Double_t[nvar];
  for (int k=0; k<nvar; k++) {
    binEdges[k]=new Double_t[nbin[k]+1];
    container->GetBinLimits(k,binEdges[k]);
    correlation->SetBinEdges(k,binEdges[k]);
    correlation->SetBinEdges(k+nvar,binEdges[k]);
  }
  correlation->Sumw2();
  task->SetCorrelationMatrix(correlation);
  //---

  //SETUP THE ANALYSIS MANAGER TO READ INPUT CHAIN AND WRITE DESIRED OUTPUTS
  printf("CREATE ANALYSIS MANAGER\n");
  // Make the analysis manager
  IlcAnalysisManager *mgr = new IlcAnalysisManager("TestManager");
  mgr->SetAnalysisType(IlcAnalysisManager::kLocalAnalysis);

  IlcMCEventHandler*  mcHandler = new IlcMCEventHandler();
  mgr->SetMCtruthEventHandler(mcHandler);
 
  IlcInputEventHandler* dataHandler = new IlcESDInputHandler();
  mgr->SetInputEventHandler(dataHandler);

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
  IlcAnalysisDataContainer *coutput3 = mgr->CreateContainer("corr0", THnSparseD::Class(),IlcAnalysisManager::kOutputContainer,"output.root");

  cinput0->SetData(analysisChain);

  mgr->AddTask(task);
  mgr->ConnectInput(task,0,mgr->GetCommonInputContainer());
  mgr->ConnectOutput(task,0,coutput0);
  mgr->ConnectOutput(task,1,coutput1);
  mgr->ConnectOutput(task,2,coutput2);
  mgr->ConnectOutput(task,3,coutput3);
 
  printf("READY TO RUN\n");
  //RUN !!!
  if (mgr->InitAnalysis()) {
    mgr->PrintStatus();
    mgr->StartAnalysis("local",analysisChain);
  }

  benchmark.Stop("IlcSingleTrackTask");
  benchmark.Show("IlcSingleTrackTask");

  return kTRUE ;
}

void Load() {

  //load the required ilcroot libraries
  gSystem->Load("libANALYSIS") ;
  gSystem->Load("libANALYSISilc") ;
  gSystem->Load("libCORRFW.so") ;

  //compile online the task class
  gSystem->SetIncludePath("-I. -I$ILC_ROOT/include -I$ROOTSYS/include");
  gROOT->LoadMacro("./IlcCFTaskForUnfolding.cxx+");
}

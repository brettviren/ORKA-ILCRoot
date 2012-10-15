/*
  Template of calibration/filtering  macro using ESD

  .L $ILC_ROOT/ANALYSIS/macros/runCalibTrain.C
  runCalibTrain(105160);

*/

#if !defined(__CINT__) || defined(__MAKECINT__)
#include <TChain.h>
#include <TSystem.h>
#include "IlcAnalysisManager.h"
#include "IlcESDInputHandler.h"
#include "IlcAnalysisTaskESDfilter.h"
#include "IlcAnalysisDataContainer.h"
#endif

void runCalibTrain(TString runNumberString, const char *inFileName = "IlcESDs.root",
                   const char *outFileName = "IlcESDs_v1.root")
{

    gSystem->Load("libTree");
    gSystem->Load("libGeom");
    gSystem->Load("libPhysics");
    gSystem->Load("libVMC");
    gSystem->Load("libSTEERBase");
    gSystem->Load("libESD");
    gSystem->Load("libAOD");

    gSystem->Load("libANALYSIS");
    gSystem->Load("libANALYSISilc");
    gSystem->Load("libANALYSIScalib");
    gSystem->Load("libCORRFW");
    gSystem->Load("libPWGmuon");
    gROOT->LoadMacro("ConfigCalibTrain.C");
    gROOT->LoadMacro("AddTaskCopyESD.C");
    gROOT->LoadMacro("AddTaskFilterFriend.C");
    gROOT->LoadMacro("AddTaskFilterFriendSecond.C");
    gROOT->LoadMacro("AddTaskAddObject.C");

    // detector tasks

    gROOT->LoadMacro("AddTaskTPCCalib.C");

    IlcLog::SetClassDebugLevel("IlcESDEvent",19);
    TChain *chain = new TChain("esdTree");

    // Steering input chain

    chain->Add(inFileName);
    Int_t runNumber = runNumberString.Atoi();
    printf("runNumber from runCalibTrain = %d\n",runNumber);
    ConfigCalibTrain(runNumber, "raw://");

    IlcAnalysisManager *mgr  = new IlcAnalysisManager("ESD to ESD", "Analysis Manager");
    // mgr->SetDebugLevel(3);

    // Input

    IlcESDInputHandler* inpHandler = new IlcESDInputHandler();
    inpHandler->SetActiveBranches("ESDfriend*");
    mgr->SetInputEventHandler  (inpHandler);

    // Output

    IlcESDHandler* esdHandler   = new IlcESDHandler();
    esdHandler->SetOutputFileName(outFileName);
    mgr->SetOutputEventHandler(esdHandler);

    // Steering Tasks

    IlcAnalysisTaskCopyESD *copy = AddTaskCopyESD();
    IlcAnalysisTaskFilterFriend* filter = AddTaskFilterFriend();
    IlcAnalysisTaskFilterFriendSecond* filter2 = AddTaskFilterFriendSecond();
    IlcAnalysisTaskAddObject* add = AddTaskAddObject();

    // Detector Tasks

   IlcAnalysisTask* tTPC = AddTaskTPCCalib(runNumber);

    // Run the analysis

    if (!mgr->InitAnalysis()) {
            printf("Analysis cannot be started, returning\n");
            return;
    }

    mgr->PrintStatus();
    mgr->StartAnalysis("local", chain);

    return;
}

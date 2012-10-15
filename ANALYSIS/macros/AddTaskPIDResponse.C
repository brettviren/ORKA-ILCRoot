IlcAnalysisTask *AddTaskPIDResponse(Bool_t isMC=kFALSE, Bool_t autoMCesd=kTRUE)
{
// Macro to connect a centrality selection task to an existing analysis manager.
  IlcAnalysisManager *mgr = IlcAnalysisManager::GetAnalysisManager();
  if (!mgr) {
    ::Error("AddTaskPIDResponse", "No analysis manager to connect to.");
    return 0x0;
  }

  IlcVEventHandler *inputHandler=mgr->GetInputEventHandler();

  //case of multi input event handler (needed for mixing)
  if (inputHandler->IsA() == IlcMultiInputEventHandler::Class()) {
    printf("========================================================================================\n");
    printf("PIDResponse: IlcMultiInputEventHandler detected, initialising IlcPIDResponseInputHandler\n");
    printf("========================================================================================\n");
    IlcMultiInputEventHandler *multiInputHandler=(IlcMultiInputEventHandler*)inputHandler;
    
    IlcPIDResponseInputHandler *pidResponseIH = new IlcPIDResponseInputHandler();
    multiInputHandler->AddInputEventHandler(pidResponseIH);

    if (autoMCesd &&
        multiInputHandler->GetFirstInputEventHandler()->IsA()==IlcESDInputHandler::Class() &&
        multiInputHandler->GetFirstMCEventHandler()
       ) isMC=kTRUE;
    pidResponseIH->SetIsMC(isMC);

    return 0x0;
  }

  // standard with task
  printf("========================================================================================\n");
  printf("PIDResponse: Initialising IlcAnalysisTaskPIDResponse\n");
  printf("========================================================================================\n");
  
  if ( autoMCesd && (inputHandler->IsA() == IlcESDInputHandler::Class()) ) {
    isMC=mgr->GetMCtruthEventHandler()!=0x0;
  }

  IlcAnalysisTaskPIDResponse *pidTask = new IlcAnalysisTaskPIDResponse("PIDResponseTask");
//   pidTask->SelectCollisionCandidates(IlcVEvent::kMB);
  pidTask->SetIsMC(isMC);
  mgr->AddTask(pidTask);
  
//   IlcAnalysisDataContainer *coutput1 = mgr->CreateContainer("PIDResponseQA",
//     TList::Class(), IlcAnalysisManager::kOutputContainer,
//     "PIDResponseQA.root");
  
  mgr->ConnectInput(pidTask, 0, mgr->GetCommonInputContainer());
//   mgr->ConnectOutput(pidTask,1,coutput1);
  
  return pidTask;
}

IlcEPSelectionTask *AddTaskEventplane()
{
  // Macro to connect an event plane selection task to an existing analysis manager.
  IlcAnalysisManager *mgr = IlcAnalysisManager::GetAnalysisManager();
  if (!mgr) {
    ::Error("AddTaskEventplane", "No analysis manager to connect to.");
    return NULL;
  }      
  // Check the analysis type using the event handlers connected to the analysis manager.
  //==============================================================================
  if (!mgr->GetInputEventHandler()) {
    ::Error("AddTaskEventplane", "This task requires an input event handler");
    return NULL;
  }
  TString inputDataType = mgr->GetInputEventHandler()->GetDataType(); // can be "ESD" or "AOD"
 
  
  IlcEPSelectionTask *eventplaneTask = new IlcEPSelectionTask("EventplaneSelection");
  eventplaneTask->SelectCollisionCandidates(IlcVEvent::kMB);
  if (inputDataType == "AOD"){
    eventplaneTask->SetInput("AOD");
  }
  eventplaneTask->SetTrackType("TPC");
  eventplaneTask->SetUsePtWeight();
  eventplaneTask->SetUsePhiWeight();
  eventplaneTask->SetSaveTrackContribution();
  
  mgr->AddTask(eventplaneTask);

  IlcAnalysisDataContainer *cinput0 = mgr->GetCommonInputContainer();
  IlcAnalysisDataContainer *coutput1 = mgr->CreateContainer("EPStat",
                TList::Class(), IlcAnalysisManager::kOutputContainer,
                "EventStat_temp.root");
  
  mgr->ConnectInput(eventplaneTask, 0, mgr->GetCommonInputContainer());
  mgr->ConnectOutput(eventplaneTask,1,coutput1);

  return eventplaneTask;
}

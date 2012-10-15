IlcAODHandler* AddAODOutputHandler()
{
  IlcAnalysisManager *mgr = IlcAnalysisManager::GetAnalysisManager();
  if (!mgr) 
  {
    ::Error("AddAODOutputHandler", "No analysis manager to connect to.");
    return NULL;
  }

  IlcAODHandler* handler = new IlcAODHandler();
  handler->SetOutputFileName("IlcAOD.root");
  mgr->SetOutputEventHandler(handler);
  IlcAnalysisDataContainer* cout_aod = mgr->GetCommonOutputContainer();
  cout_aod->SetSpecialOutput();
  
  return handler;
}

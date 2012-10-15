IlcESDInputHandler* AddESDHandler()
{
  IlcAnalysisManager *mgr = IlcAnalysisManager::GetAnalysisManager();
  if (!mgr) 
  {
    ::Error("AddESDHandler", "No analysis manager to connect to.");
    return NULL;
  }

  IlcESDInputHandler *esdHandler = new IlcESDInputHandler();
  mgr->SetInputEventHandler(esdHandler);
  
  return esdHandler;
}

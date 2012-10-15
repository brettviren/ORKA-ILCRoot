IlcAODInputHandler* AddAODHandler()
{
  IlcAnalysisManager *mgr = IlcAnalysisManager::GetAnalysisManager();
  if (!mgr) 
  {
    ::Error("AddAODHandler", "No analysis manager to connect to.");
    return NULL;
  }

  IlcAODInputHandler* handler = new IlcAODInputHandler();
  mgr->SetInputEventHandler(handler);
  
  return handler;
}

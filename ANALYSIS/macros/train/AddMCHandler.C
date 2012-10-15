IlcMCEventHandler* AddMCHandler(Bool_t readTrackRef = kFALSE)
{
  IlcAnalysisManager *mgr = IlcAnalysisManager::GetAnalysisManager();
  if (!mgr) 
  {
    ::Error("AddMCHandler", "No analysis manager to connect to.");
    return NULL;
  }

  IlcMCEventHandler* handler = new IlcMCEventHandler();
  mgr->SetMCtruthEventHandler(handler);
  handler->SetReadTR(readTrackRef);

  return handler;
}

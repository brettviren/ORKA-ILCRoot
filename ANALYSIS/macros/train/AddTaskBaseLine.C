IlcAnalysisTask* AddTaskBaseLine() 
{
  IlcAnalysisManager *mgr = IlcAnalysisManager::GetAnalysisManager();
  if (!mgr) {
    ::Error("AddTaskPhysicsSelection", "No analysis manager to connect to.");
    return NULL;
  }  
  
  IlcAnalysisTask *task = new IlcAnalysisTaskBaseLine("baseline");
  mgr->AddTask(task);
  
  return task;
}   

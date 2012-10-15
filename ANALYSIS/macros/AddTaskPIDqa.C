IlcAnalysisTask *AddTaskPIDqa(const char *useroutputfile=""){
  //get the current analysis manager
  IlcAnalysisManager *mgr = IlcAnalysisManager::GetAnalysisManager();
  if (!mgr) {
    Error("AddTaskPIDqa", "No analysis manager found.");
    return 0;
  }
  
  //========= Add task to the ANALYSIS manager =====
  IlcAnalysisTaskPIDqa *task=new IlcAnalysisTaskPIDqa("PIDqaTask");
  mgr->AddTask(task);
  
  //================================================
  //              data containers
  //================================================

  TString outputfile=useroutputfile;
  if (outputfile.IsNull()) outputfile = Form("%s:PIDqa", IlcAnalysisManager::GetCommonFileName());
  IlcAnalysisDataContainer *coutput1 =
    mgr->CreateContainer("PIDqa", TList::Class(),
                         IlcAnalysisManager::kOutputContainer,outputfile);
  
  //           connect containers
  mgr->ConnectInput  (task,  0, mgr->GetCommonInputContainer() );
  mgr->ConnectOutput (task,  1, coutput1);
  
  return task;
}

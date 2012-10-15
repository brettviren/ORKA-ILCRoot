//=============================================================================
//
// *** AddTaskCopyESD.C ***
//
// This macro initialize a complete AnalysisTask object for Copying ESD.
//
//=============================================================================

IlcAnalysisTaskCopyESD *AddTaskCopyESD()
{

	// pointer to the analysis manager
	IlcAnalysisManager *mgr = IlcAnalysisManager::GetAnalysisManager();
	if (!mgr) {
		Error("AddTaskTOFCalib", "No analysis manager to connect to.");
		return NULL;
	}  

	// check the input handler
	if (!mgr->GetInputEventHandler()) {
		::Error("AddTask", "This task requires an input event handler");
		return NULL;
	}  
	TString type = mgr->GetInputEventHandler()->GetDataType(); // can be "ESD" or "AOD"

	// create the task
	IlcAnalysisTaskCopyESD *copy = new IlcAnalysisTaskCopyESD("ESD copying task");
	mgr->AddTask(copy);        

	// connecting the input/output containers
	IlcAnalysisDataContainer *cinput0  = mgr->GetCommonInputContainer();
	IlcAnalysisDataContainer *coutput0 = mgr->GetCommonOutputContainer();

	mgr->ConnectInput (copy, 0, cinput0 );
	mgr->ConnectOutput(copy, 0, coutput0);

	return copy;
}

//=============================================================================
//
// *** AddTaskAddObject.C ***
//
// This macro initialize a complete AnalysisTask object for filtering ESD with IlcAnalysisTaskFilterFriendSecond.
//
//=============================================================================

IlcAnalysisTaskAddObject *AddTaskAddObject()
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
	IlcAnalysisTaskAddObject* add = new IlcAnalysisTaskAddObject("addObj");
	mgr->AddTask(add);

	// connecting the input/output containers
	IlcAnalysisDataContainer *cinput0  = mgr->GetCommonInputContainer();
	IlcAnalysisDataContainer *coutput0 = mgr->CreateContainer("histo",  TH1::Class(), IlcAnalysisManager::kOutputContainer, "IlcESDfriends_v1.root");

	mgr->ConnectInput (add, 0, cinput0 );
	mgr->ConnectOutput(add, 0, coutput0);

	return add;
}

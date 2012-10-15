void SetupAnalysis(TString mode,
		   TString analysisMode="full",
		   Bool_t useMC = kFALSE,
		   Int_t nEvents=1.0*1e9, 
		   Int_t nEventsSkip=0,
		   TString format="esd")
{
  
  // ILC stuff
  IlcAnalysisManager *mgr = IlcAnalysisManager::GetAnalysisManager();
  if (!mgr) mgr = new IlcAnalysisManager("CAF train");
  
  // Create and configure the alien handler plugin 
  gROOT->LoadMacro("CreateAnalysisPlugin.C"); 
  IlcAnalysisGrid *alienHandler = CreateAnalysisPlugin(analysisMode);   
  if (!alienHandler) return;
  mgr->SetGridHandler(alienHandler);
  
  // input handler for esd or AOD, real or MC data
  InputHandlerSetup(format,useMC);

  // physics selection
  if(!format.CompareTo("esd")){
    gROOT->LoadMacro("$ILC_ROOT/ANALYSIS/macros/AddTaskPhysicsSelection.C");
    IlcPhysicsSelectionTask* physSelTask = AddTaskPhysicsSelection(kFALSE);
    if(useMC) physSelTask->GetPhysicsSelection()->SetAnalyzeMC();   
    IlcPhysicsSelection* physSel = physSelTask->GetPhysicsSelection();
    physSel->AddBackgroundIdentification(new IlcBackgroundSelection());
  }
  
  gROOT->ProcessLine(Form(".include %s/include",gSystem->ExpandPathName("$ILC_ROOT")));
  
  gROOT->LoadMacro("IlcAnalysisTaskEfficiency.cxx+g");
  
  // load and run AddTask macro
  gROOT->LoadMacro("AddTaskEfficiency.C");
  

  IlcAnalysisTaskSE* task1 = AddTaskEfficiency(-1);
  if(!task1){
    Printf("AddTask could not be run.");
  }

  // Run analysis
  mgr->InitAnalysis();
  
  if ((!mode.CompareTo("proof")) ||(!mode.CompareTo("local"))) {
    mgr->StartAnalysis(mode.Data(),nEvents,nEventsSkip);
  }
  else {
    mgr->StartAnalysis(mode.Data());
    
  }
  
}


TString GetFormatFromDataSet(TString dataset) {
  
  TString dsTreeName;
  if (dataset.Contains("#")) {
    Info("runSKAF.C",Form("Detecting format from dataset name '%s' ...",dataset.Data()));
    dsTreeName=dataset(dataset.Last('#'),dataset.Length());
  } else {
    Info("runSKAF.C",Form("Detecting format from dataset '%s' (may take while, depends on network connection) ...",
			  dataset.Data()));
    TFileCollection *ds = gProof->GetDataSet(dataset.Data());
    if (!ds) {
      Error(Form("Dataset %s doesn't exist on proof cluster!!!!",dataset.Data()));
      return "";
    }
    dsTreeName = ds->GetDefaultTreeName();
  }
  
  if (dsTreeName.Contains("esdTree")) {
    Info("runSKAF.C","ESD input format detected ...");
    return "ESD";
  } else if (dsTreeName.Contains("aodTree"))  {
    Info("runSKAF.C","AOD input format detected ...");
    return "AOD";
  } else {
    Error("runSKAF.C",Form("Tree %s is not supported !!!",dsTreeName.Data()));
    Error("runSKAF.C",Form("Maybe set your DS to %s#esdTree or %s#aodTree",
			   dataset.Data(),dataset.Data()));
  }
  
  return "";
}

Bool_t InputHandlerSetup(TString format = "esd", Bool_t useKine = kTRUE)
{
  format.ToLower();

  IlcAnalysisManager *mgr = IlcAnalysisManager::GetAnalysisManager();

  IlcAnalysisDataContainer *cin = mgr->GetCommonInputContainer();

  if (cin) return;

  if (!format.CompareTo("esd"))
    {
      IlcESDInputHandler *esdInputHandler = 
	dynamic_cast<IlcESDInputHandler*>(IlcAnalysisManager::GetAnalysisManager()->
					  GetInputEventHandler());

      if (!esdInputHandler)
	{
	  Info("CustomAnalysisTaskInputSetup", "Creating esdInputHandler ...");
	  esdInputHandler = new IlcESDInputHandler();
	  mgr->SetInputEventHandler(esdInputHandler);
	}

      if (useKine)
	{
	  IlcMCEventHandler* mcInputHandler = 
	    dynamic_cast<IlcMCEventHandler*>(IlcAnalysisManager::GetAnalysisManager()->
					     GetMCtruthEventHandler());

	  if (!mcInputHandler)
	    {
	      Info("CustomAnalysisTaskInputSetup", "Creating mcInputHandler ...");
	      IlcMCEventHandler* mcInputHandler = new IlcMCEventHandler();
	      mgr->SetMCtruthEventHandler(mcInputHandler);
	    }
	}

    }
  else if (!format.CompareTo("aod"))
    {
      IlcAODInputHandler *aodInputHandler = 
	dynamic_cast<IlcAODInputHandler*>(IlcAnalysisManager::GetAnalysisManager()->
					  GetInputEventHandler());

      if (!aodInputHandler)
	{
	  Info("CustomAnalysisTaskInputSetup", "Creating aodInputHandler ...");
	  aodInputHandler = new IlcAODInputHandler();
	  mgr->SetInputEventHandler(aodInputHandler);
	}
    }
  else
    {
      IlcWarning("Wrong input format!!! Only ESD and AOD are supported. Skipping Task ...");
      return kFALSE;
    }

  return kTRUE;
}

IlcAnalysisTask *AddTaskTender(Bool_t useV0=kFALSE, 
                               Bool_t useTPC=kTRUE,
                               Bool_t useTOF=kTRUE,
                               Bool_t useTRD=kTRUE,
                               Bool_t usePID=kTRUE,
                               Bool_t useVTX=kTRUE,
                               Bool_t useT0=kTRUE,
                               Bool_t usePtFix=kFALSE)
{
  if (!(useV0 | useTPC | useTOF | useTRD | usePID | useVTX | useT0 | usePtFix)) {
     ::Error("AddTaskTender", "No supply added to tender, so tender not created");
     return 0;
  }   
  //get the current analysis manager
  Bool_t checkEvtSelection = useV0;
  IlcAnalysisManager *mgr = IlcAnalysisManager::GetAnalysisManager();
  if (!mgr) {
    ::Error("AddTaskTender", "No analysis manager found.");
    return 0;
  }
  // currently don't accept AOD input
  if (!mgr->GetInputEventHandler()->InheritsFrom(IlcESDInputHandler::Class())) {
    ::Error("AddTask_tender_Tender","The analysis tender only works with ESD input!");
    return 0;
  }

  
  //========= Add tender to the ANALYSIS manager and set default storage =====
  IlcTender *tender=new IlcTender("AnalysisTender");
  tender->SetCheckEventSelection(checkEvtSelection);
  tender->SetDefaultCDBStorage("raw://");
  mgr->AddTask(tender);
  if (checkEvtSelection) {
    if (mgr->GetTasks()->First() != (TObject*)tender ) {
      TString firstName(mgr->GetTasks()->First()->GetName());
      Bool_t isSecond=(mgr->GetTasks()->At(1) == (TObject*)tender);
      if (! (firstName=="PIDResponseTask" && isSecond )){
        Fatal("AddTaskTender","When setting the tender to check the event selection, it has to be the first wagon, or the first after the PID Response!");
        return NULL;
      }
    }
  }
  
  //========= Attach VZERO supply ======

  if (useV0) {
     IlcVZEROTenderSupply *vzeroSupply=new IlcVZEROTenderSupply("VZEROtender");
     vzeroSupply->SetDebug(kFALSE);
     tender->AddSupply(vzeroSupply);
  }   

 
  //========= Attach TPC supply ======
  if (useTPC) {
     IlcTPCTenderSupply *tpcSupply=new IlcTPCTenderSupply("TPCtender");
     tpcSupply->SetDebugLevel(2);
     //tpcSupply->SetMip(50.);
     tender->AddSupply(tpcSupply);
  }   

  //========= Attach track 1/pt correction supply ======
  if (usePtFix) {
     IlcTrackFixTenderSupply *trfixSupply=new IlcTrackFixTenderSupply("PTInvFix");
     //trfixSupply->SetDebugLevel(2);
     tender->AddSupply(trfixSupply);
  }   

  //========= Attach T0 supply ======
  if (useT0) {
    IlcT0TenderSupply *t0Tender = new IlcT0TenderSupply("T0tender");
    t0Tender ->SetPass4LHC11aCorrection(kTRUE);
    tender->AddSupply(t0Tender);
  }   

  //========= Attach TOF supply ======
  if (useTOF) {
    IlcTOFTenderSupply *tofTender = new IlcTOFTenderSupply("TOFtender");
    tender->AddSupply(tofTender);
 }   

  //========= Attach TRD supply ======
  if (useTRD) {
    IlcTRDTenderSupply *trdSupply=new IlcTRDTenderSupply("TRDtender");
    trdSupply->SetLoadReferencesFromCDB();
    // Mask Bad chambers
    trdSupply->AddBadChamber(265);      // low drift
    trdSupply->AddBadChamber(50);
    trdSupply->AddBadChamber(524);      // low drift
    trdSupply->AddBadChamber(32);       // intermediate gain
    trdSupply->AddBadChamber(15);
    trdSupply->AddBadChamber(231);      // low gain
    trdSupply->AddBadChamber(273);      // intermediate gain
    trdSupply->AddBadChamber(532);
    trdSupply->AddBadChamber(5);        // low drift
    trdSupply->AddBadChamber(227);
    trdSupply->AddBadChamber(287);      // low drift
    trdSupply->AddBadChamber(212);      // intermediate gain
    trdSupply->AddBadChamber(228);      // low gain
    trdSupply->AddBadChamber(52);       // low gain
    trdSupply->AddBadChamber(169);      // low drift
    trdSupply->AddBadChamber(236);      // low drift

    trdSupply->SetPIDmethod(IlcTRDTenderSupply::k1DLQpid);
    trdSupply->SetNormalizationFactor(1./7.603);
    tender->AddSupply(trdSupply);
  }  

  //========= Attach PID supply ======
  if (usePID) {
    tender->AddSupply(new IlcPIDTenderSupply("PIDtender"));
  }  

  //========= Attach Primary Vertex supply ======
  if (useVTX) {
    tender->AddSupply(new IlcVtxTenderSupply("PriVtxtender"));
  }  

  //================================================
  //              data containers
  //================================================

    //            define output containers, please use 'username'_'somename'
  IlcAnalysisDataContainer *coutput1 =
      mgr->CreateContainer("tender_event", IlcESDEvent::Class(),
                           IlcAnalysisManager::kExchangeContainer,"default_tender");
 
  //           connect containers
  mgr->ConnectInput  (tender,  0, mgr->GetCommonInputContainer() );
  mgr->ConnectOutput (tender,  1, coutput1);
 
  return tender;
}

void rec() {
  IlcReconstruction reco;

  reco.SetWriteESDfriend();
  reco.SetWriteAlignmentData();

  reco.SetDefaultStorage("local://$ILC_ROOT/OCDB");
  reco.SetSpecificStorage("GRP/GRP/Data",
			  Form("local://%s",gSystem->pwd()));
  reco.SetRunPlaneEff(kTRUE);

  reco.SetRunQA("ALL:ALL") ;
  
  reco.SetQARefDefaultStorage("local://$ILC_ROOT/QAref") ;
  
  for (Int_t det = 0 ; det < IlcQA::kNDET ; det++) {
    reco.SetQACycles(det, 999) ;
    reco.SetQAWriteExpert(det) ; 
  }
  
  TStopwatch timer;
  timer.Start();
  reco.Run();
  timer.Stop();
  timer.Print();
}

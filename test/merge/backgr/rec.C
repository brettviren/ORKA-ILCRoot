void rec() {

  IlcReconstruction reco;
  reco.SetWriteESDfriend();
  reco.SetWriteAlignmentData();

  reco.SetDefaultStorage("local://$ILC_ROOT/OCDB");
  reco.SetSpecificStorage("GRP/GRP/Data",
			  Form("local://%s",gSystem->pwd()));
  reco.SetSpecificStorage("VZERO/Calib/Data",
			  "local://$ILC_ROOT/OCDB/VZERO/PbPb");
  reco.SetRunPlaneEff(kTRUE);
 
  TStopwatch timer;
  timer.Start();
  reco.Run();
  timer.Stop();
  timer.Print();
}

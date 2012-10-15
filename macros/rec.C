void rec() {

  //Perform DCH RecPoints
  gROOT->Macro("$ILC_ROOT/DCH/IlcDCHFindClustersFast.C");

  IlcReconstruction reco;

  //Select detectors to make RecPoints
  reco.SetRunLocalReconstruction("TARGET");

  //Select detectors for reconstruction
  reco.SetRunTracking("DCH TARGET");
  reco.SetFillESD("DCH TARGET");

  TStopwatch timer;
  timer.Start();

  //do the work
  reco.Run();


  timer.Stop();
  timer.Print();
}

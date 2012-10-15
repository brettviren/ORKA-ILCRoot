void sim(Int_t nev=1) {
  IlcSimulation simulator;

  simulator.SetDefaultStorage("local://$ILC_ROOT/OCDB");
  simulator.SetSpecificStorage("GRP/GRP/Data",
			       Form("local://%s",gSystem->pwd()));

  TStopwatch timer;
  timer.Start();
  simulator.Run(nev);
  timer.Stop();
  timer.Print();
}

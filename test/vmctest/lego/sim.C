// $Id: sim.C 53007 2011-11-18 17:11:47Z ivana $
//
// Macro for running lego simulation in test/vmctest/lego.

void sim(Int_t nev=1, const TString& config, const TString& det)  {
  IlcSimulation simulator(config);
  TString configFunction("Config(\"");
  configFunction.Append(det.Data());
  configFunction.Append(TString("\");"));
  cout << configFunction.Data() << endl;
  gIlc->SetConfigFunction(configFunction.Data());
  TStopwatch timer;
  timer.Start();
  simulator.RunLego(config.Data());
  timer.Stop();
  timer.Print();
}

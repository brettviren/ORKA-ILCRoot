void sim(Int_t nev=5) {
  if (!strcmp(gSystem->GetBuildArch(),"win32gcc")) {
    gSystem->Load("libProof");
    gSystem->Load("libGui");
    gROOT->Macro("loadlibssim.C");
    new IlcRun("gIlc","The ILC Off-line Simulation Framework");
  }

  IlcSimulation simulator;
  simulator.SetMakeSDigits("TRD TOF PHOS HMPID EMCAL MUON FMD ZDC PMD T0 VZERO");
  simulator.SetMakeDigitsFromHits("ITS TPC");
  if (strcmp(gSystem->GetBuildArch(),"win32gcc")) {
    simulator.SetWriteRawData("ALL","raw.root",kTRUE);
  }

  simulator.SetDefaultStorage("local://$ILC_ROOT/OCDB");
  simulator.SetSpecificStorage("GRP/GRP/Data",
			       Form("local://%s",gSystem->pwd()));
 
  TStopwatch timer;
  timer.Start();
  simulator.Run(nev);
  timer.Stop();
  timer.Print();
}

void sim2(Int_t nevt=1) {

  //Simulation spets are performet all together

  //Set debug level to 1 in RSTACK module
  //IlcLog::SetModuleDebugLevel("RSTACK",1);

  IlcSimulation simulator;

  TStopwatch timer;
  timer.Start();

  //Select detectors to SDigitize
  simulator.SetMakeSDigits("TARGET RSTACK");

  //Select detectors to Digitize
  simulator.SetMakeDigits("TARGET RSTACK");

  //Perform Hits+SDigits+Digits simulation
  simulator.Run(nevt);

  //Export geometry to be used for tracks reconstruction
  gGeoManager->Export("geometry.root");

  timer.Stop();
  timer.Print();
}

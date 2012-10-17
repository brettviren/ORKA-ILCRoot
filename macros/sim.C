void sim(Int_t nevt=1) {

  //Simulation spets are performet independently

  //Set debug level to 1 in RSTACK module
  //IlcLog::SetModuleDebugLevel("RSTACK",1);

  IlcSimulation simulator;

  TStopwatch timer;
  timer.Start();

  //Perform Hits simulation
  simulator.RunSimulation(nevt);

  //Export geometry to be used for tracks reconstruction
  gGeoManager->Export("geometry.root");

  //Perform SDigits simulation
  simulator.RunSDigitization("TARGET RSTACK");

  //Perform Digits simulation
  simulator.RunDigitization("TARGET RSTACK");

  timer.Stop();
  timer.Print();
}

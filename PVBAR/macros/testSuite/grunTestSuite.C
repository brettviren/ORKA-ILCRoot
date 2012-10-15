void grunTestSuite (Int_t nevent=3, const char *config="ConfigTestSuite.C")
{
  //
  // Simple macro to run ilcroot in a batch mode
  //
  gIlc->Init(config);
  TStopwatch timer;
  timer.Start();
//
//  If nevent is negative it is assumed that in config the 
//  global variable eventsPerRun has been set.
//
  if (nevent < 0) 
      nevent = eventsPerRun;
  gIlc->Run(nevent);
  timer.Stop();
  timer.Print();
}

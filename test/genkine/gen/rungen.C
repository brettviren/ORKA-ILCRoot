void rungen(Int_t nev=1){
  // Simulation and reconstruction
  TStopwatch timer;
  timer.Start();
  gSystem->SetIncludePath("-I$ROOTSYS/include -I$ILC_ROOT/include -I$ILC_ROOT");
  gSystem->Load("liblhapdf.so");      // Parton density functions
  gSystem->Load("libEGPythia6.so");   // TGenerator interface
  gSystem->Load("libpythia6.so");     // Pythia
  gSystem->Load("libIlcPythia6.so");  // ILC specific implementations
  gROOT->LoadMacro("fastGen.C+");
  fastGen(nev);
  timer.Stop();
  timer.Print();
}

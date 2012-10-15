void check(){
  if (!strcmp(gSystem->GetBuildArch(),"win32gcc")) {
    gSystem->Load("libProof");
    gSystem->Load("libGui");
    gROOT->Macro("loadlibs.C");
    new IlcRun("gIlc","The ILC Off-line Simulation Framework");
  }

  gSystem->Load("liblhapdf");      // Parton density functions
  gSystem->Load("libEGPythia6");   // TGenerator interface
  gSystem->Load("libpythia6");     // Pythia
  gSystem->Load("libIlcPythia6");  // ILC specific implementations
  gSystem->Load("libdpmjet");
  gSystem->Load("libTDPMjet");
  gROOT->Macro("$ILC_ROOT/STEER/CheckESD.C");
}

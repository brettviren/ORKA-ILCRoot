// $Id: g3Config.C 50428 2011-07-05 12:56:44Z ivana $
//
// Configuration macro for running ilcroot with Geant3
// with primary events read from external file.
//
// By I. Hrivnacova, IPN Orsay

void Config()
{
  cout << "Running g3Config.C ... " << endl;

  // IlcRoot setup
  //
  gROOT->LoadMacro("$ILC_ROOT/test/vmctest/gun/commonConfig.C");
  commonConfig();

  // Load Geant3 + Geant3 VMC libraries
  //
#if defined(__CINT__)
    gSystem->Load("libgeant321");
#endif

  // Create TGeant3
  //  
  new  TGeant3TGeo("C++ Interface to Geant3");

  // IlcRoot event generator
  // (it has to be created after MC, as it may use decayer via VMC)
  //
  gROOT->LoadMacro("$ILC_ROOT/test/vmctest/gun/genExtFileConfig.C");
  genExtFileConfig();

  cout << "Running g3Config.C finished ... " << endl;
}  

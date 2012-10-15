// $Id: genConfig.C 39165 2010-02-25 16:12:24Z ivana $
//
// Configuration macro for primary event generation for ppbench test (in vmctest).
//
// By I. Hrivnacova, IPN Orsay

void Config()
{
  cout << "Running genConfig.C ... " << endl;

  // IlcRoot setup
  //
  gROOT->LoadMacro("$ILC_ROOT/test/vmctest/ppbench/commonConfig.C");
  commonConfig(kFALSE);

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
  gROOT->LoadMacro("$ILC_ROOT/test/vmctest/ppbench/genPPbenchConfig.C");
  genGunConfig();

  cout << "Running genConfig.C finished ... " << endl;
}  

// $Id: g3Config.C 53007 2011-11-18 17:11:47Z ivana $
//
// Configuration macro for running ilcroot with Geant3
// with primary events read from external file.
//
// By I. Hrivnacova, IPN Orsay

void Config(const TString& det)
{
  cout << "Running g3Config.C ... " << endl;

  // IlcRoot setup
  //
  gROOT->LoadMacro("$ILC_ROOT/test/vmctest/lego/commonConfig.C");
  commonConfig(det);

  // Load Geant3 + Geant3 VMC libraries
  //
#if defined(__CINT__)
    gSystem->Load("libgeant321");
#endif

  // Create TGeant3
  //  
  new  TGeant3TGeo("C++ Interface to Geant3");

  cout << "Running g3Config.C finished ... " << endl;
}  

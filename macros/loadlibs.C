Int_t loadlibs () 
{
  // Macro which loads the libraries needed for simulation and reconstruction
  // Possible usage: In a Root session (no IlcRoot) one does
  // root [0] .x loadlibs.C
  // root [1] gIlc = new IlcRun("gIlc","test")           
  // root [2] IlcSimulation sim
  // root [3] sim.Run()
  // root [4] IlcReconstruction rec
  // root [5] rec.Run()

  Int_t ret=-1;

  if ( gSystem->Load("libPhysics") < 0 ) return ret; ret--;
  if ( gSystem->Load("libMinuit") < 0 ) return ret; ret--;
  if ( gSystem->Load("libProof") < 0 ) return ret; ret--;

  if ( gSystem->Load("libmicrocern") < 0 ) return ret; ret--;
//   if ( gSystem->Load("liblhapdf") < 0 ) return ret; ret--;
  if ( gSystem->Load("libpythia6") < 0 ) return ret; ret--;

  if ( gSystem->Load("libEG") < 0 ) return ret; ret--;
  if ( gSystem->Load("libGeom") < 0 ) return ret; ret--;
  if ( gSystem->Load("libVMC") < 0 ) return ret; ret--;

  if ( gSystem->Load("libEGPythia6") < 0 ) return ret; ret--;

  if ( gSystem->Load("libSTEERBase") < 0 ) return ret; ret--;
  if ( gSystem->Load("libESD") < 0 ) return ret; ret--;
  if ( gSystem->Load("libCDB") < 0 ) return ret; ret--;
  if ( gSystem->Load("libRAWDatabase") < 0 ) return ret; ret--;
  if ( gSystem->Load("libRAWDatarec") < 0 ) return ret; ret--;
  if ( gSystem->Load("libAOD") < 0 ) return ret; ret--;
  if ( gSystem->Load("libANALYSIS") < 0 ) return ret; ret--;
  if ( gSystem->Load("libSTEER") < 0 ) return ret; ret--;
  if ( gSystem->Load("libRAWDatasim") < 0 ) return ret; ret--;
  if ( gSystem->Load("libFASTSIM") < 0 ) return ret; ret--;
  if ( gSystem->Load("libEVGEN") < 0 ) return ret; ret--;
  if ( gSystem->Load("libIlcPythia6") < 0 ) return ret; ret--;
  if ( gSystem->Load("libSTAT") < 0 ) return ret; ret--;

  if ( gSystem->Load("libhijing") < 0 ) return ret; ret--;
  if ( gSystem->Load("libTHijing") < 0 ) return ret; ret--;// IlcGenHijingEventHeader needed by libZDCsim.so

  if ( gSystem->Load("libSTRUCT") < 0 ) return ret; ret--;
  if ( gSystem->Load("libRSTACKUtils") < 0 ) return ret; ret--;
  if ( gSystem->Load("libRSTACKbase") < 0 ) return ret; ret--;
  if ( gSystem->Load("libRSTACKsim") < 0 ) return ret; ret--;
  if ( gSystem->Load("libRSTACKrec") < 0 ) return ret; ret--;
  if ( gSystem->Load("libPVBARUtils") < 0 ) return ret; ret--;
  if ( gSystem->Load("libPVBARbase") < 0 ) return ret; ret--;
  if ( gSystem->Load("libPVBARsim") < 0 ) return ret; ret--;
  if ( gSystem->Load("libPVBARrec") < 0 ) return ret; ret--;
  if ( gSystem->Load("libPVECUtils") < 0 ) return ret; ret--;
  if ( gSystem->Load("libPVECbase") < 0 ) return ret; ret--;
  if ( gSystem->Load("libPVECsim") < 0 ) return ret; ret--;
  if ( gSystem->Load("libPVECrec") < 0 ) return ret; ret--;

  if ( gSystem->Load("libDCHbase") < 0 ) return ret; ret--;
  if ( gSystem->Load("libDCHrec") < 0 ) return ret; ret--;
  if ( gSystem->Load("libDCHsim") < 0 ) return ret; ret--;
  if ( gSystem->Load("libDCHfast") < 0 ) return ret; ret--;
  if ( gSystem->Load("libTARGETbase") < 0 ) return ret; ret--;
  if ( gSystem->Load("libTARGETsim") < 0 ) return ret; ret--;
  if ( gSystem->Load("libTARGETrec") < 0 ) return ret; ret--;
	
  #ifdef MFT_UPGRADE
  if ( gSystem->Load("libMFTbase") < 0 ) return ret; ret--;
  if ( gSystem->Load("libMFTrec") < 0 ) return ret; ret--;
  if ( gSystem->Load("libMFTsim") < 0 ) return ret; ret--;
  #endif
	

  return 0;

}

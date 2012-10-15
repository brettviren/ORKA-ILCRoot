/* $Id: IlcPVBARecalibrate.C 30923 2009-02-09 15:47:08Z hristov $ */

// Digitize and decalibrate events assuming that SDigits 
// have been already produced.
// Author: Boris Polichtchouk (Boris.Polichtchouk@cern.ch)

void IlcPVBARDecalibrate(Int_t nevents=1)
{

  //Use "decalibration" database to simulate decalibrated PVBAR.

  IlcCDBManager::Instance()->SetDefaultStorage("local://$ILC_ROOT/OCDB");
  IlcCDBManager::Instance()->SetSpecificStorage("PVBAR","local://DeCalibDB");

  // Make digitization, calibration parameters will be taken from CDB

  IlcSimulation sim ; 
  sim.SetRunGeneration(kFALSE) ;
  sim.SetMakeSDigits("") ;
  sim.SetMakeDigits("PVBAR") ;
  sim.Run(nevents) ;  

}

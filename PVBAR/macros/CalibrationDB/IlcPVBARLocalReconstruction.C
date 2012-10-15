IlcPVBARLocalReconstruction()
{
  //Run PVBAR clusterization using information from calibration database.
  // Author: Boris Polishchuk (Boris.Polichtchouk at cern.ch)

  IlcCDBManager::Instance()->SetDefaultStorage("local://$ILC_ROOT/OCDB");
  IlcCDBManager::Instance()->SetSpecificStorage("PVBAR","local://CalibDB");
 
  // Make clusterization, calibration parameters will be taken from CDB

  IlcPVBARClusterizerv1 *clu = new IlcPVBARClusterizerv1("gilc.root");
  clu->SetEventRange(0,-1);
  clu->Exec("deb tim");
}

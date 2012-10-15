//
// Macro to initialize: 
// - the OCDB (run number required as input argument)
// - the geometry (expected to be in the current directory)
// to run the Calibration train.
// 

void ConfigCalibTrain(Int_t run, const char *ocdb="raw://"){

  // OCDB
 
  printf("setting run to %d\n",run);
  IlcCDBManager::Instance()->SetDefaultStorage(ocdb);
  IlcCDBManager::Instance()->SetRun(run); 

  // geometry
  IlcGeomManager::LoadGeometry();
  IlcGeomManager::ApplyAlignObjsFromCDB("GRP ITS TPC");
}

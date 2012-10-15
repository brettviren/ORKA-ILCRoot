void merge(Int_t run=117048)
{
  IlcCDBManager *cdbm = IlcCDBManager::Instance();
  cdbm->SetRun(run);
  cdbm->SetDefaultStorage("local://$ILC_ROOT/OCDB");     
  cdbm->SetSpecificStorage("GRP/GRP/Data",Form("local://%s","$ILC_ROOT/test/rawmerge"));  
  //cdbm->SetSpecificStorage("GRP/GRP/Data",Form("local://%s",gSystem->pwd()));  

  IlcGeomManager::LoadGeometry("geometry.root");
  TGeoGlobalMagField::Instance()->SetField(new IlcMagF("Maps","Maps", -1., -1., IlcMagF::k5kG));

  IlcSimulation sim;
  sim.SetSeed(12345);
  sim.SetRunGeneration(0);
  sim.SetRunSimulation(0);
  sim.SetRunHLT("");
  sim.SetNumberOfEvents(1);
  sim.SetMakeSDigits("");
  sim.SetMakeDigits("ITS TPC");
  sim.SetMakeDigitsFromHits("");
  sim.SetGIlcFile("gilc.root");
  sim.SetGeometryFile("geometry.root");
  sim.MergeWith("$ILC_ROOT/test/rawmerge/bkg/gilc.root",1);
  sim.Run();
  return;
}

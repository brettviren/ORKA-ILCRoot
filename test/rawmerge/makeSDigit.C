void makeSDigit(Int_t run=117048,
                const char *fname="/ilc/data/2010/LHC10b/000117048/raw/10000117048041.30.root")
{
  IlcSimulation sim;
  //IlcLog::SetGlobalDebugLevel(1);
  IlcLog::SetModuleDebugLevel("STEER",1);
  IlcCDBManager *cdbm = IlcCDBManager::Instance();
  cdbm->SetRun(run);
  cdbm->SetDefaultStorage("local://$ILC_ROOT/OCDB");     
  //cdbm->SetDefaultStorage("raw://");     
  cdbm->SetSpecificStorage("GRP/GRP/Data",Form("local://%s","$ILC_ROOT/test/rawmerge"));  
  sim.SetMakeSDigits("ITS TPC");
  sim.SetNumberOfEvents(10000);
  sim.ConvertRaw2SDigits(fname);
  return;
}

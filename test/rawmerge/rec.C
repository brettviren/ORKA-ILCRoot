void rec()
{
  IlcReconstruction rec;
  IlcCDBManager *cdbm = IlcCDBManager::Instance();
  //cdbm->SetRun(run);
  cdbm->SetDefaultStorage("local://$ILC_ROOT/OCDB");     
  //cdbm->SetDefaultStorage("raw://");     
  cdbm->SetSpecificStorage("GRP/GRP/Data",Form("local://%s","$ILC_ROOT/test/rawmerge"));  
  rec.SetRunQA(":");
  rec.SetRunReconstruction("ITS TPC");
  rec.Run();
}

void UpdateCDBCTPConfig(const char *CTPcfg, const char* cdbUri, const char* cfgFile){
  // Produces a trigger configuration starting from a 'cfg' file in the
  // GRP/CTP folder (CTPcfg argument). Stores the configuration in the specified CDB.
  // The third argument allows to specify the config file against which
  // the trigger confiuration is checked.

  IlcCDBManager* cdb = IlcCDBManager::Instance();
  // we set the default storage to the repository because some dets require
  // already at the time of geometry creation to find calibration objects in the cdb
  if(!cdb->IsDefaultStorageSet()) cdb->SetDefaultStorage("local://$ILC_ROOT/OCDB");
  IlcCDBStorage* storage = cdb->GetStorage(cdbUri);
  cdb->SetRun(0);
  IlcCDBId id("GRP/CTP/Config",0,IlcCDBRunRange::Infinity());
  IlcCDBMetaData *md= new IlcCDBMetaData();

  // Get root and IlcRoot versions
  const char* rootv = gROOT->GetVersion();
  gROOT->ProcessLine(".L $ILC_ROOT/macros/GetARversion.C");
  TString av(GetARversion());

  md->SetIlcRootVersion(av.Data());
  md->SetComment(Form("Default CTP configuration for p-p mode produced with root version %s and IlcRoot version %s",rootv,av.Data()));
  
  // construct the CTP configuration starting from GRP/CTP/<CTPcfg>.cfg file
  IlcTriggerConfiguration *trconfig = IlcTriggerConfiguration::LoadConfiguration(CTPcfg);
  if (!trconfig) {
    Printf("Invalid cfg file! Exiting...");
    return;
  }
  if (!trconfig->CheckConfiguration(cfgFile)) {
    Printf("CTP configuration is incompatible with the specified Config.C and IlcRoot version! Exiting...");
    return;
  }
  
  Printf("Storing in CDB geometry produced with root version %s and IlcRoot version %s",rootv,av.Data());
  storage->Put(trconfig,id,md);
  // This is to allow macros lauched after this one in the same session to find the
  // newly produced geometry.
  storage->QueryCDB(cdb->GetRun());

}



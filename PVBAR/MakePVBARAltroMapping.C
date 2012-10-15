void MakePVBARAltroMapping(){
  // Create TObjArray of PVBAR altro mapping objects.
  // Set the environment variables in order to store it in the OCDB:
  // export TOCDB=kTRUE
  // export STORAGE=local://$ILC_ROOT/OCDB
  // Then the newly created root file $ILC_ROOT/OCDB/PVBAR/Calib/Mapping
  // should be committed to SVN and submitted to AliEn production manager
  //
  // Yuri Kharlov. 10 September 2009
  // $Id: MakePVBARAltroMapping.C 34740 2009-09-10 12:07:33Z kharlov $

  const char* macroname = "MakePVBARAltroMapping.C";

  TObjArray mappingsArray(20);
  
  nModules = 5;
  nRCU     = 4;
  TString path = gSystem->Getenv("ILC_ROOT");
  path += "/PVBAR/mapping/";
  TString path1, path2;
  for(Int_t m = 0; m < nModules; m++) {
    path1 = path;
    path1 += "Mod";
    path1 += m;
    path1 += "RCU";
    for(Int_t i = 0; i < nRCU; i++) {
      path2 = path1;
      path2 += i;
      path2 += ".data";
      Info(macroname,"Mapping file: %s",path2.Data());
      IlcAltroMapping *mapping = new IlcCaloAltroMapping(path2.Data());
      mappingsArray.Add(mapping);
    }
  }

  if( TString(gSystem->Getenv("TOCDB")) != TString("kTRUE") ){
    // save on file
    const char* filename = "PVBARAltroMapping.root";
    TFile f(filename,"RECREATE");
    if(!f){
      Error(macroname,"cannot open file for output\n");
      return;
    }
    Info(macroname,"Saving altro mapping objects to the file %s", filename);
    f.cd();
    f.WriteObject(&mappingsArray,"PVBARAtroMappings","kSingleKey");
    f.Close();
  }else{
    // save in CDB storage
    TString Storage = gSystem->Getenv("STORAGE");
    if(!Storage.BeginsWith("local://") && !Storage.BeginsWith("alien://")) {
      Error(macroname,"STORAGE variable set to %s is not valid. Exiting\n",Storage.Data());
      return;
    }
    Info(macroname,"Saving altro mapping objects in CDB storage %s",
	 Storage.Data());
    IlcCDBManager* cdb = IlcCDBManager::Instance();
    IlcCDBStorage* storage = cdb->GetStorage(Storage.Data());
    if(!storage){
      Error(macroname,"Unable to open storage %s\n",Storage.Data());
      return;
    }
    IlcCDBMetaData* md = new IlcCDBMetaData();
    md->SetResponsible("Yuri Kharlov");
    md->SetComment("Default ALTRO mapping for PVBAR: 20 mapping objects, one per modules per RCU");
    md->SetIlcRootVersion(gSystem->Getenv("ARVERSION"));
    IlcCDBId id("PVBAR/Calib/Mapping",0,IlcCDBRunRange::Infinity());
    storage->Put(&mappingsArray,id,md);

    delete md;
  }

  mappingsArray.Delete();

}


void UpdateCDBGRPEntry() {
  // produce the GRP default entry in CDB
  
  IlcCDBManager* man = IlcCDBManager::Instance();
  man->SetDefaultStorage("local://$ILC_ROOT/OCDB");
  man->SetRun(0);
  IlcCDBId id("GRP/GRP/Data",0,IlcCDBRunRange::Infinity());
  IlcCDBMetaData *metadata= new IlcCDBMetaData();

  // Get root version
  const char* rootv = gROOT->GetVersion();

  /*
  // Get IlcRoot version from file to set it in the metadata of the entry
  TFile *fv= TFile::Open("$ILC_ROOT/CVS/Repository?filetype=raw","READ");
  Int_t size = fv->GetSize();
  char *buf = new Char_t[size];
  memset(buf, '\0', size);
  fv->Seek(0);
  char* ilcrootv;
  if ( fv->ReadBuffer(buf, size) ) {
    Printf("Error reading IlcRoot version from file to buffer!");
    ilcrootv = "";
  }
  if(buf=="IlcRoot"){
    ilcrootv="HEAD";
  }else{
    ilcrootv = buf;
    metadata->SetIlcRootVersion(ilcrootv);
  }
  */
    metadata->SetResponsible("Chiara.Zampolli@cern.ch");
    metadata->SetComment("Default values for the GRP monitored parameters");

  TMap *mappp = GetGRPList("pp");
  //TMap *mappbpb = GetGRPList("PbPb");   // not used as default!!!

  //  Printf("Storing in CDB the default values for the GRP %d parameters produced with root %s and IlcRoot version %s",list->GetEntries(),rootv,ilcrootv);

  man->Put(mappp,id,metadata);
}

//_______________________________________//
TMap *GetGRPList(const char* system) {
  TString fSystem = system;
  TMap *map = new TMap();
  map->SetName(system);

  //DAQ
  map->Add(new TObjString("fIlcStartTime"),new TObjString("0"));
  map->Add(new TObjString("fIlcStopTime"),new TObjString("9999"));
  if(fSystem == "pp")
    map->Add(new TObjString("fIlcBeamEnergy"),new TObjString("14000"));
  else map->Add(new TObjString("fIlcBeamEnergy"),new TObjString("5500"));
  map->Add(new TObjString("fIlcBeamType"),new TObjString(system));
  map->Add(new TObjString("fNumberOfDetectors"),new TObjString("21"));
  map->Add(new TObjString("fDetectorMask"),new TObjString("1074790399"));
  map->Add(new TObjString("fLHCPeriod"),new TObjString("LHC08c"));

  //DCS
  map->Add(new TObjString("fLHCState"),new TObjString("test"));
  map->Add(new TObjString("fLHCCondition"),new TObjString("test"));
  map->Add(new TObjString("fLHCLuminosity"),new TObjString("0"));
  map->Add(new TObjString("fBeamIntensity"),new TObjString("0"));
  map->Add(new TObjString("fL3Current"),new TObjString("30000"));
  map->Add(new TObjString("fL3Polarity"),new TObjString("0"));
  map->Add(new TObjString("fDipoleCurrent"),new TObjString("6000"));
  map->Add(new TObjString("fDipolePolarity"),new TObjString("0"));
  map->Add(new TObjString("fCavernTemperature"),new TObjString("0"));
  map->Add(new TObjString("fCavernPressure"),new TObjString("0"));

  return map;
}

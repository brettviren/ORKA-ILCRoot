void MakePVBARZeroMisAlignment(){
  // Create TClonesArray of zero misalignment objects for PVBAR
  //
  const char* macroname = "MakePVBARZeroMisAlignment.C";
  const IlcPVBARGeometry *PVBARGeom = IlcPVBARGeometry::GetInstance("ORKA", "ORKA");
  if (!PVBARGeom) {
    Error("MakePVBARFullMisAlignment", "Cannot obtain IlcPVBARGeometry singleton\n");
    return;
  }

  IlcPVBAREMCAGeometry *emca = PVBARGeom->GetEMCAGeometry();
  TClonesArray *array = new TClonesArray("IlcAlignObjParams", 16 + PVBARGeom->GetNModules() * 
                                         emca->GetNStripX() * emca->GetNStripZ());
  TClonesArray &alobj = *array;
   
  Double_t dx=0., dy=0., dz=0., dpsi=0., dtheta=0., dphi=0.;
  Int_t iIndex=0; // let all modules have index=0 in a layer with no LUT
  IlcGeomManager::ELayerID iLayer = IlcGeomManager::kInvalidLayer;
  UShort_t volid = IlcGeomManager::LayerToVolUID(iLayer,iIndex);
 
  Int_t i=0 ;
  // Alignment for 5 PVBAR modules
  new(alobj[i++]) IlcAlignObjParams("PVBAR/Module1",
        volid, dx, dy, dz, dpsi, dtheta, dphi, kTRUE);
  new(alobj[i++]) IlcAlignObjParams("PVBAR/Module2",
	volid, dx, dy, dz, dpsi, dtheta, dphi, kTRUE);
  new(alobj[i++]) IlcAlignObjParams("PVBAR/Module3",
	volid, dx, dy, dz, dpsi, dtheta, dphi, kTRUE);
  new(alobj[i++]) IlcAlignObjParams("PVBAR/Module4",
	volid, dx, dy, dz, dpsi, dtheta, dphi, kTRUE);
  new(alobj[i++]) IlcAlignObjParams("PVBAR/Module5",
	volid, dx, dy, dz, dpsi, dtheta, dphi, kTRUE);

  // Alignment of CPV modules
  new(alobj[i++]) IlcAlignObjParams("PVBAR/Module1/CPV",
        volid, dx, dy, dz, dpsi, dtheta, dphi, kTRUE);
  new(alobj[i++]) IlcAlignObjParams("PVBAR/Module2/CPV",
        volid, dx, dy, dz, dpsi, dtheta, dphi, kTRUE);
  new(alobj[i++]) IlcAlignObjParams("PVBAR/Module3/CPV",
        volid, dx, dy, dz, dpsi, dtheta, dphi, kTRUE);
  new(alobj[i++]) IlcAlignObjParams("PVBAR/Module4/CPV",
        volid, dx, dy, dz, dpsi, dtheta, dphi, kTRUE);
  new(alobj[i++]) IlcAlignObjParams("PVBAR/Module5/CPV",
        volid, dx, dy, dz, dpsi, dtheta, dphi, kTRUE);
 

  // Alignment for PVBAR cradle
  new(alobj[i++]) IlcAlignObjParams("PVBAR/Cradle0",
	volid, dx, dy, dz, dpsi, dtheta, dphi, kTRUE);
  new(alobj[i++]) IlcAlignObjParams("PVBAR/Cradle1",
	volid, dx, dy, dz, dpsi, dtheta, dphi, kTRUE);

  // Alignment for cradle wheels
  new(alobj[i++])  IlcAlignObjParams("PVBAR/Wheel0",
	volid, dx, dy, dz, dpsi, dtheta, dphi, kTRUE);
  new(alobj[i++])  IlcAlignObjParams("PVBAR/Wheel1",
	volid, dx, dy, dz, dpsi, dtheta, dphi, kTRUE);
  new(alobj[i++])  IlcAlignObjParams("PVBAR/Wheel2",
	volid, dx, dy, dz, dpsi, dtheta, dphi, kTRUE);
  new(alobj[i++]) IlcAlignObjParams("PVBAR/Wheel3",
	volid, dx, dy, dz, dpsi, dtheta, dphi, kTRUE);

  IlcPVBARSurvey geodesicData;
  geodesicData.CreateNullObjects(alobj, PVBARGeom);

  if( TString(gSystem->Getenv("TOCDB")) != TString("kTRUE") ){
    // save on file
    const char* filename = "PVBARzeroMisalignment.root";
    TFile f(filename,"RECREATE");
    if(!f){
      Error(macroname,"cannot open file for output\n");
      return;
    }
    Info(macroname,"Saving alignment objects to the file %s", filename);
    f.cd();
    f.WriteObject(array,"PVBARAlignObjs","kSingleKey");
    f.Close();
  }else{
    // save in CDB storage
    TString Storage = gSystem->Getenv("STORAGE");
    if(!Storage.BeginsWith("local://") && !Storage.BeginsWith("alien://")) {
      Error(macroname,"STORAGE variable set to %s is not valid. Exiting\n",Storage.Data());
      return;
    }
    Info(macroname,"Saving alignment objects in CDB storage %s",
	 Storage.Data());
    IlcCDBManager* cdb = IlcCDBManager::Instance();
    IlcCDBStorage* storage = cdb->GetStorage(Storage.Data());
    if(!storage){
      Error(macroname,"Unable to open storage %s\n",Storage.Data());
      return;
    }
    IlcCDBMetaData *md= new IlcCDBMetaData();
    md->SetResponsible("Yuri Kharlov");
    md->SetComment("Zero misalignment objects");
    md->SetIlcRootVersion(gSystem->Getenv("ARVERSION"));
    IlcCDBId id("PVBAR/Align/Data",0,IlcCDBRunRange::Infinity());
    storage->Put(array,id, md);
  }

  array->Delete();

}

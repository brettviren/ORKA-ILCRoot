void MakePVBARAlignmentModule2()
{
  // Create misalignment object for PVBAR module 2 
  // from the survey measurements on P2 in May 2008.
  // To store alignment in OCDB, define the evnironment variables:
  // TOCDB=kTRUE
  // STORAGE="local://$ILC_ROOT/OCDB"
  // Author: Timur Pocheptsov, 19.06.2008

  const char * macroName = "MakePVBAR2Alignment";
  
  const IlcPVBARGeometry *PVBARGeom = IlcPVBARGeometry::GetInstance("ORKA", "ORKA");
  if (!PVBARGeom) {
    Error(macroName, "Cannot obtain IlcPVBARGeometry singleton.\n");
    return;
  }

  //Activate CDB storage and load geometry from CDB
  //I need valid gGeoManager to create local transformations.
  
  //[Part of code, taken from ITS version of MakexxxFullMisalignment
  IlcCDBManager * cdb = IlcCDBManager::Instance();
  if (!cdb->IsDefaultStorageSet())
    cdb->SetDefaultStorage("local://$ILC_ROOT/OCDB");
  cdb->SetRun(0);

  if (TString(gSystem->Getenv("TOCDB")) == TString("kTRUE")) {
    const TString storageENV(gSystem->Getenv("STORAGE"));
    if (!storageENV.BeginsWith("local://") && !storageENV.BeginsWith("alien://")) {
      Error(macroName, "STORAGE variable set to %s is not valid.\n", storageENV.Data());
      return;
    }
    
    IlcCDBStorage * storage = cdb->GetStorage(storageENV.Data());
    if (!storage) {
      Error(macroName, "Unable to open storage %s.\n", storageENV.Data());
      return;
    }
    
    IlcCDBPath path("GRP","Geometry","Data");
    IlcCDBEntry * entry = storage->Get(path.GetPath(), cdb->GetRun());
    if (!entry) {
      Error(macroName,"Could not get the specified CDB entry!");
      return;  
    }
    
    entry->SetOwner(0);
    IlcGeomManager::SetGeometry((TGeoManager*) entry->GetObject());
  }else{
    IlcGeomManager::LoadGeometry("geometry.root"); //load geom from default CDB storage
  }    
  //end of code taken from ITS version of MakexxxFullMisalignment]

  IlcPVBAREMCAGeometry * emca = PVBARGeom->GetEMCAGeometry();
  TClonesArray alobj("IlcAlignObjParams", 16);// + PVBARGeom->GetNModules() * emca->GetNStripX() * emca->GetNStripZ());
   
  const Double_t dpsi = 0., dtheta = 0., dphi = 0.;
  const Double_t displacement = 10.;
  Int_t iIndex = 0; //let all modules have index=0 in a layer with no LUT
  const IlcGeomManager::ELayerID iLayer = IlcGeomManager::kInvalidLayer;
  UShort_t volid = IlcGeomManager::LayerToVolUID(iLayer, iIndex);
  Int_t i = 0;

  // Alignment for 5 PVBAR modules

  new(alobj[i++]) IlcAlignObjParams("PVBAR/Module1",
	  volid, 0., 0., 0., 0., 0., 0., kTRUE);
  new(alobj[i++]) IlcAlignObjParams("PVBAR/Module2",
	  volid, 0., 0., 0., 0., 0., 0., kTRUE);

  Double_t rotMatrix[9] = {0.999992695, -0.00295322, -0.0024267, 
			   0.002955489, 0.999995199, 0.00093165, 
			   0.002423942, -0.000938811, 0.99999662};
  TGeoRotation rotation;
  rotation.SetMatrix(rotMatrix);
  Double_t dX=1.25474126, dY=-1.4088643, dZ=-12.856;
  IlcAlignObjParams * mod3 = 
    new(alobj[i++]) IlcAlignObjParams("PVBAR/Module3", volid, dX, dY, dZ, 0., 0., 0., kFALSE);
  mod3->SetRotation(rotation);
  
  new(alobj[i++]) IlcAlignObjParams("PVBAR/Module4",
	  volid, 0.,  0., 0., 0., 0., 0., kTRUE);
  new(alobj[i++]) IlcAlignObjParams("PVBAR/Module5",
	  volid, 0., 0., 0., 0., 0., 0., kTRUE);

  const Double_t dx = 0., dy = 0., dz = 0. ;
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
	  volid, 0., 0., -displacement, dpsi, dtheta, dphi, kTRUE);
  new(alobj[i++]) IlcAlignObjParams("PVBAR/Cradle1",
	  volid, 0., 0., +displacement, dpsi, dtheta, dphi, kTRUE);

  // Alignment for cradle wheels
  new(alobj[i++]) IlcAlignObjParams("PVBAR/Wheel0",
	  volid, 0., 0., -displacement, dpsi, dtheta, dphi, kTRUE);
  new(alobj[i++]) IlcAlignObjParams("PVBAR/Wheel1",
	  volid, 0., 0., -displacement, dpsi, dtheta, dphi, kTRUE);
  new(alobj[i++]) IlcAlignObjParams("PVBAR/Wheel2",
	  volid, 0., 0., +displacement, dpsi, dtheta, dphi, kTRUE);
  new(alobj[i++]) IlcAlignObjParams("PVBAR/Wheel3",
	  volid, 0., 0., +displacement, dpsi, dtheta, dphi, kTRUE);

  // *************************    2nd step    ***************
  if ( TString(gSystem->Getenv("TOCDB")) != TString("kTRUE") ) {
    // save on file
    const char * fileName = "PVBARfullMisalignment.root";
    TFile f(fileName,"RECREATE");
    if (!f) {
      Error(macroName, "cannot open file for output\n");
      return;
    }
    
    Info(macroName,"Saving alignment objects to the file %s", fileName);
    f.cd();
    f.WriteObject(&alobj,"PVBARAlignObjs","kSingleKey");
    f.Close();
  }else{
    // save in CDB storage
    TString storageENV = gSystem->Getenv("STORAGE");
    if(!storageENV.BeginsWith("local://") && !storageENV.BeginsWith("alien://")) {
      Error(macroName,"STORAGE variable set to %s is not valid. Exiting\n", storageENV.Data());
      return;
    }
    
    Info(macroName,"Saving alignment objects in CDB storage %s", storageENV.Data());
    IlcCDBManager * cdb = IlcCDBManager::Instance();
    IlcCDBStorage * storage = cdb->GetStorage(storageENV.Data());
    if (!storage) {
      Error(macroName, "Unable to open storage %s\n", storageENV.Data());
      return;
    }
    
    IlcCDBMetaData md;
    md.SetResponsible("Timur Pocheptsov");
    md.SetComment("Alignment objects for PVBAR module 2; survey in May 2008");
    md.SetIlcRootVersion(gSystem->Getenv("ARVERSION"));
    IlcCDBId id("PVBAR/Align/Data",0,IlcCDBRunRange::Infinity());
    storage->Put(&alobj, id, &md);
  }

  alobj.Delete();
}

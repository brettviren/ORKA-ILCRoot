void MakePVBARFullMisAlignment()
{
  // Create misalignment object for PVBAR module 2,3,3
  // from the survey measurements on P2 in August 2009.
  // To store alignment in OCDB, define the evnironment variables:
  // TOCDB=kTRUE
  // STORAGE="local://$ILC_ROOT/OCDB"
  // Author: Timur Pocheptsov, 19.06.2008
  // Modified: Yuri Kharlov, 11.03.2010

  const char * macroName = "MakePVBAR2Alignment";
  
  const IlcPVBARGeometry *PVBARGeom = IlcPVBARGeometry::GetInstance("ORKA", "ORKA");
  if (!PVBARGeom) {
    Error(macroName, "Cannot obtain IlcPVBARGeometry singleton.\n");
    return;
  }

  IlcPVBAREMCAGeometry * emca = PVBARGeom->GetEMCAGeometry();
  TClonesArray alobj("IlcAlignObjParams", 16);// + PVBARGeom->GetNModules() * emca->GetNStripX() *
                                              //   emca->GetNStripZ());
  
  const Double_t dpsi = 0., dtheta = 0., dphi = 0.;
  const Double_t displacement = 10.;
  Int_t iIndex = 0; //let all modules have index=0 in a layer with no LUT
  const IlcGeomManager::ELayerID iLayer = IlcGeomManager::kInvalidLayer;
  UShort_t volid = IlcGeomManager::LayerToVolUID(iLayer, iIndex);
  Int_t i = 0;

  // Alignment for 5 PVBAR modules

  TString surveyFileName;
  
  const Char_t * szEnv = gSystem->Getenv("ILC_ROOT");
  if (szEnv && szEnv[0]) {
    surveyFileName += szEnv;
    if (surveyFileName[surveyFileName.Length() - 1] != '/')
      surveyFileName += '/';
  } else {
    Warning(macroName, "cannot find ILC_ROOT environment variable\n"
		       "probably, I wan't be able to find survey file");
  }
    
  surveyFileName += "PVBAR/data/Survey_1053236_PVBAR.txt";

  IlcSurveyObj survey;
  survey.FillFromLocalFile(surveyFileName.Data());
  TGeoHMatrix module3Delta, module2Delta, module1Delta;
  IlcPVBARModuleMisalignment delta(*PVBARGeom);

  delta.DeltaTransformation(0, survey.GetData(), "410000", "410027", "424000", 
        		    &module1Delta);
  new(alobj[i++]) IlcAlignObjParams("PVBAR/Module1", volid, module1Delta, kTRUE);

  delta.DeltaTransformation(1, survey.GetData(), "310000", "310027", "324000", 
        		    &module2Delta);
  new(alobj[i++]) IlcAlignObjParams("PVBAR/Module2", volid, module2Delta, kTRUE);

  delta.DeltaTransformation(2, survey.GetData(), "210000", "210027", "224000", 
        		    &module3Delta);
  new(alobj[i++]) IlcAlignObjParams("PVBAR/Module3", volid, module3Delta, kTRUE);

  new(alobj[i++]) IlcAlignObjParams("PVBAR/Module4", volid, 0., 0., 0., 0., 0., 0., kTRUE);

  new(alobj[i++]) IlcAlignObjParams("PVBAR/Module5", volid, 0., 0., 0., 0., 0., 0., kTRUE);

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
    md.SetResponsible("Yuri Kharlov");
    md.SetComment("Alignment objects for PVBAR modules 2,3,4; survey in August 2009");
    md.SetIlcRootVersion(gSystem->Getenv("ARVERSION"));
    IlcCDBId id("PVBAR/Align/Data",0,IlcCDBRunRange::Infinity());
    storage->Put(&alobj, id, &md);
  }

  alobj.Delete();
}

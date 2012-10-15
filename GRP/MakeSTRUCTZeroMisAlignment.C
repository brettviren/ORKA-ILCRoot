void MakeSTRUCTZeroMisAlignment(){
  // Create TClonesArray of zero misalignment objects for all STRUCTures
  // (presently this includes only FRAME)
  // 
  const char* macroname = "MakeSTRUCTZeroMisAlignment.C";

  TClonesArray *array = new TClonesArray("IlcAlignObjParams",20);

  Int_t iIndex=0; //let all modules have index=0 in a layer with no LUT
  IlcGeomManager::ELayerID iLayer = IlcGeomManager::kInvalidLayer;
  UShort_t dvoluid = IlcGeomManager::LayerToVolUID(iLayer,iIndex); //dummy vol id

  const char* basepath ="ILCM_1/B077_1/BSEGMO";
  TString segmpath;

  for(Int_t sm=0; sm<18; sm++){
    segmpath=basepath;
    segmpath+=sm;
    segmpath+="_1";
    cout<<segmpath.Data()<<endl;
    new((*array)[sm]) IlcAlignObjParams(segmpath.Data(),dvoluid,0.,0.,0.,0.,0.,0.,kTRUE);
  }

  if( TString(gSystem->Getenv("TOCDB")) != TString("kTRUE") ){
    // save on file
    const char* filename = "STRUCTzeroMisalignment.root";
    TFile f(filename,"RECREATE");
    if(!f){
      Error(macroname,"cannot open file for output\n");
      return;
    }
    Info(macroname,"Saving alignment objects in %s", filename);
    f.cd();
    f.WriteObject(array,"STRUCTAlignObjs","kSingleKey");
    f.Close();
  }else{
    // save in CDB storage
    TString Storage = gSystem->Getenv("STORAGE");
    if(!Storage.BeginsWith("local://") && !Storage.BeginsWith("alien://")) {
      Error(macroname,"STORAGE variable set to %s is not valid. Exiting\n",Storage.Data());
      return;
    }
    Info(macroname,"Saving alignment objects in CDB storage %s",Storage.Data());
    IlcCDBManager* cdb = IlcCDBManager::Instance();
    IlcCDBStorage* storage = cdb->GetStorage(Storage.Data());
    if(!storage){
      Error(macroname,"Unable to open storage %s\n",Storage.Data());
      return;
    }
    IlcCDBMetaData* md = new IlcCDBMetaData();
    md->SetResponsible("Grosso Raffaele");
    md->SetComment("Zero misalignment for STRUCT: presently includes objects for FRAME");
    md->SetIlcRootVersion(gSystem->Getenv("ARVERSION"));
    IlcCDBId id("GRP/Align/Data",0,IlcCDBRunRange::Infinity());
    storage->Put(array,id,md);
  }

  array->Delete();

}


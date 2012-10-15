void MakePVBARCalibration(){

   //Fills PVBAR re-calibration parameters into OADB
   //Each run-dependent object contains list of 3 objects:
   //calibration parameters for pass1, pass2 and pass3 reconstruction.
   //"$ILC_ROOT/OADB/PVBAR/PVBARRecalibration.root"
   
  IlcOADBContainer calibContainer("PVBARRecalibration");

  IlcCDBManager * man = IlcCDBManager::Instance();
  man->SetRun(140000) ;
  man->SetDefaultStorage("local://OCDB");
  IlcPVBARCalibData* PVBARCalibData = new IlcPVBARCalibData(-1);

  // -- LHC10h --
  TObjArray * lhc10aAll = new TObjArray(3); 
  lhc10aAll->SetName("PVBARRecalibration_LHC10b");
  lhc10aAll->AddAt(PVBARCalibData,2) ; //pass 3 reconstruction
  calibContainer.AppendObject(lhc10aAll,114737,117223) ;

  calibContainer.WriteToFile("PVBARCalibrations.root");


}
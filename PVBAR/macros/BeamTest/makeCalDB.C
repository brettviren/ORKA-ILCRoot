void makeCalDB(void){
  //Read Connection Table DB
  TFile f("ConTableDB.root") ;
  IlcPVBARConTableDB * ctdb = (IlcPVBARConTableDB *)f.Get("IlcPVBARConTableDB") ;
  f.Close() ;

  IlcPVBARCalibrationDB * c = new IlcPVBARCalibrationDB("beamtest.root") ;
  c->SetConTableDB(ctdb) ;
  c->ReadCalibrationParameters("gains.dat","gains") ;
  c->ReadCalibrationParameters("pedestals.dat","pedestals") ;
  TFile g("beamtest.root","recreate") ;
  g.cd() ;
  c->Write(0) ;
  g.Close() ;
}

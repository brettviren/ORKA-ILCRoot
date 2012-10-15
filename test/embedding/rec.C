void rec(Int_t embrun=0) {
//  new IlcRun("gIlc","The ILC Off-line Simulation Framework");
  IlcReconstruction reco;
  reco.SetWriteESDfriend(kTRUE);
  reco.SetWriteAlignmentData(kFALSE);
//    reco.SetRecoParam("ITS",IlcITSRecoParam::GetHighFluxParam());
//    reco.SetRecoParam("TPC",IlcTPCRecoParam::GetHighFluxParam());
//    reco.SetRecoParam("TRD",IlcTRDrecoParam::GetHighFluxParam());
//    reco.SetRecoParam("PHOS",IlcPHOSRecoParam::GetDefaultParameters());
//    reco.SetRecoParam("MUON",IlcMUONRecoParam::GetHighFluxParam());
   
//   IlcTPCRecoParam * tpcRecoParam = IlcTPCRecoParam::GetLowFluxParam();
//   IlcTPCReconstructor::SetRecoParam(tpcRecoParam);
//   IlcTPCReconstructor::SetStreamLevel(0);
  reco.SetRunReconstruction("ITS TPC TRD TOF VZERO");
//   reco.SetDefaultStorage("alien://Folder=/ilc/simulation/2008/v4-15-Release/Ideal/");
//   reco.SetRunQA(kFALSE);
//   reco.SetRunGlobalQA(kFALSE);

  reco.SetDefaultStorage("local://$ILC_ROOT/OCDB");
  if (embrun == 1) {
    reco.SetSpecificStorage("GRP/GRP/Data",
			    Form("local://%s/../BackgroundFull",gSystem->pwd()));
  }
  else {
    reco.SetSpecificStorage("GRP/GRP/Data",
			    Form("local://%s",gSystem->pwd()));
  }
  reco.SetRunPlaneEff(kTRUE);
  reco.SetRunQA("ALL:ALL") ;
  
  IlcQA::SetQARefStorage("local://$ILC_ROOT/OCDB") ;
  
  for (Int_t det = 0 ; det < IlcQA::kNDET ; det++) {
    reco.SetQACycles(det, 999) ;
    reco.SetQAWriteExpert(det) ; 
  }
  
  TStopwatch timer;
  timer.Start();
  reco.Run();
  timer.Stop();
  timer.Print();
}

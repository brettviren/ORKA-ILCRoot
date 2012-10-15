void Rec(TString file="raw.root")
{
  // Reconstruction of RAW data from the input file raw.root
  // Boris Polichtchouk, 13 Mar 2008

  //IlcCDBManager::Instance()->SetDefaultStorage("local://$ILC_ROOT/OCDB");
  //IlcCDBManager::Instance()->SetSpecificStorage("PVBAR/*","local://BadMap");

  IlcReconstruction rec ;
  rec.SetRunTracking("") ;
  rec.SetRunVertexFinder(kFALSE) ; 
  rec.SetRunLocalReconstruction("PVBAR") ;
  rec.SetFillESD("PVBAR") ;

  //Set rec. parameters different from the default ones.
  IlcPVBARRecoParam* recEmc = new IlcPVBARRecoParamEmc();
  recEmc->SetSubtractPedestals(kTRUE);
  recEmc->SetMinE(0.01);                     //Minimal Digit energy
  recEmc->SetClusteringThreshold(0.02);      //Minimal cluster seed energy
  recEmc->SetDecoderVersion("v1");           //Comment out to choose Max-Ped version
  recEmc->SetOldRCUFormat(kTRUE);

  IlcPVBARReconstructor::SetRecoParamEmc(recEmc);

  rec.SetInput(file.Data());  // read RAW data
  rec.SetNumberOfEventsPerFile(5000);	
  rec.Run();


}

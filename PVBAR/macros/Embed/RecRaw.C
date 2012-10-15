void RecRaw(char * file)
{
  // Reconstruction of RAW data from the input (raw) root file 
  // D.Peressounko after Boris Polichtchouk, 31 Aug 2007

   IlcCDBManager::Instance()->SetDefaultStorage("local://$ILC_ROOT/OCDB");
  // IlcCDBManager::Instance()->SetDefaultStorage("local://./");
  // Provide here address of misalignment parametrs, Calibration or bad modules maps
  // IlcCDBManager::Instance()->SetSpecificStorage("PVBAR/*","local:///data/prsnko/");


  IlcReconstruction rec ;
  rec.SetOption("PVBAR","OldRCUFormat");
  rec.SetRunTracking("") ;
  rec.SetRunVertexFinder(kFALSE) ; 
  //Digits are produced as a by-product of local reconstruction...
  rec.SetRunLocalReconstruction("PVBAR") ;
  //Here we do not want to produce ESD
  rec.SetFillESD("") ;

  //Uncomment following lines if you want to set rec. 
  //parameters other than default ones.
  // IlcPVBARRecoParam* recEmc = new IlcPVBARRecoParamEmc();
  // recEmc->SetSubtractPedestals(kTRUE);
  // //Minimal energy of digits used in clusteriztion
  // recEmc->SetMinE(0.01);
  // //Minimal energy of cluster seed
  // recEmc->SetClusteringThreshold(0.02);
  // //Choose here method of energy/time extraction:
  // //fitting of samples - "v1"
  // //maximal value extraction - ""
  // recEmc->SetDecoderVersion("v1");
  // IlcPVBARReconstructor::SetRecoParamEmc(recEmc);

  rec.SetInput(file);  // read RAW data
  //If necessary, set first event and number of events to reconstruct
  //  rec.SetEventRange(0,20);
  rec.Run();

}

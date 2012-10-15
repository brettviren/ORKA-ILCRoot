void IlcPVBARRawReconstruction(TString file="raw.root")
{
  // Reconstruction of RAW data from the input file raw.root
  // Boris Polichtchouk, 31 Aug 2007


  //IlcLog::SetGlobalDebugLevel(1);

  IlcReconstruction rec ;
//   rec.SetOption("PVBAR","OldRCUFormat");
  rec.SetRunTracking("") ;
  rec.SetRunVertexFinder(kFALSE) ; 
  rec.SetRunLocalReconstruction("PVBAR") ;
  rec.SetFillESD("PVBAR") ;

  //Set rec. parameters different from the default ones.
  IlcPVBARRecoParam* recEmc = new IlcPVBARRecoParamEmc();
  recEmc->SetSubtractPedestals(kFALSE); // do not sibtract pedestals!

  IlcPVBARReconstructor::SetRecoParamEmc(recEmc);

  rec.SetInput(file.Data());  // read RAW data
  rec.Run();


}

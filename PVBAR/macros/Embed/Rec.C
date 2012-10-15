void Rec(const Int_t debLevel=0)
{

  IlcLog::SetGlobalDebugLevel(debLevel);
  IlcReconstruction rec ;
  rec.SetRunReconstruction("PVBAR") ;
  rec.SetRunTracking("PVBAR") ;
  rec.SetRunVertexFinder(kFALSE) ; 
  rec.SetFillESD("PVBAR") ; 

  //Uncomment the following lines to use rec. parameters 
  //other than default ones.
  // IlcPVBARRecoParam* recEmc = new IlcPVBARRecoParamEmc();
  //  recEmc->SetSubtractPedestals(kTRUE);
  //  recEmc->SetMinE(0.01);
  //  recEmc->SetClusteringThreshold(0.01);
  //  IlcPVBARReconstructor::SetRecoParamEmc(recEmc);

  rec.Run();

}

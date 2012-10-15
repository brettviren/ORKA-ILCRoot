UpdateRecoParam(const Int_t runNumber)
{
  // Read the array of PVBAR recoparam objects from OCDB and update
  // EMC fitter version to "v4".
  // Write the updated object to OCDB.
  // Yuri Kharlov. 9.12.2011
  //
  /* $Id: UpdateRecoParam.C 53497 2011-12-09 12:22:47Z kharlov $ */

  IlcCDBManager::Instance()->SetDefaultStorage("raw://");
  IlcCDBManager::Instance()->SetRun(runNumber);

  IlcCDBEntry* cdbEntry = IlcCDBManager::Instance()->Get("PVBAR/Calib/RecoParam");
  IlcCDBMetaData *md = cdbEntry->GetMetaData();
  cout << "Responsible: " << md->GetResponsible() << endl;
  cout << "MD Comment : " << md->GetComment() << endl;
  TObjArray* arrayRecoParam = (TObjArray*)cdbEntry->GetObject();

  cout << "N recoparam = " << arrayRecoParam->GetEntries() << endl;

  IlcPVBARRecoParam *rp = 0;
  for (Int_t i=0; i<arrayRecoParam->GetEntries(); i++) {
    rp = (IlcPVBARRecoParam*)arrayRecoParam->At(i);
    printf("RP %d: event specie = %d, fitter version = %s\n",
	   i,rp->GetEventSpecie(),rp->EMCFitterVersion());
    rp->SetEMCFitterVersion("v4");
  }

  // Writing new recoparam to OCDB

  IlcCDBManager* cdb = IlcCDBManager::Instance();
  cdb->SetDefaultStorage("local://OCDB");

  IlcCDBMetaData *md= new IlcCDBMetaData();
  md->SetResponsible("Yuri Kharlov");
  md->SetComment("PVBAR recoparameters: EMC fitter version is updated to v4");
  IlcCDBId id("PVBAR/Calib/RecoParam",167690,IlcCDBRunRange::Infinity());
  cdb->Put(arrayRecoParam,id, md);

}
//-----------------------------------------------------------------------------

ReadRecoParam(const Int_t runNumber)
{
  // Read the array of PVBAR recoparam objects from OCDB and
  // print its content to stdout

  IlcCDBManager::Instance()->SetDefaultStorage("raw://");
  IlcCDBManager::Instance()->SetRun(runNumber);

  IlcCDBEntry* cdbEntry = IlcCDBManager::Instance()->Get("PVBAR/Calib/RecoParam");
  IlcCDBMetaData *md = cdbEntry->GetMetaData();
  printf("Responsible: %s\n",md->GetResponsible());
  printf("MD Comment : %s\n",md->GetComment());
  TObjArray* arrayRecoParam = (TObjArray*)cdbEntry->GetObject();

  IlcPVBARRecoParam *rp = 0;
  for (Int_t i=0; i<arrayRecoParam->GetEntries(); i++) {
    rp = (IlcPVBARRecoParam*)arrayRecoParam->At(i);
    printf("Recoparam %d: event specie = %d\n",
	   i,rp->GetEventSpecie());

    printf("\tEMCClusteringThreshold = %g\n",rp->GetEMCClusteringThreshold());
    printf("\tEMCLocalMaxCut         = %g\n",rp->GetEMCLocalMaxCut());
    printf("\tEMCRawDigitThreshold   = %g\n",rp->GetEMCRawDigitThreshold());
    printf("\tEMCMinE                = %g\n",rp->GetEMCMinE());
    printf("\tEMCLogWeight           = %g\n",rp->GetEMCLogWeight());
    printf("\tEMCSampleQualityCut    = %g\n",rp->GetEMCSampleQualityCut());
    printf("\tEMCEcoreRadius         = %g\n",rp->GetEMCEcoreRadius());
    printf("\tEMCEcore2ESD           = %d\n",rp->EMCEcore2ESD());
    printf("\tEMCSubtractPedestals   = %d\n",rp->EMCSubtractPedestals());
    printf("\tEMCToUnfold            = %d\n",rp->EMCToUnfold());
    printf("\tEMCfitter version      = %s\n",rp->EMCFitterVersion());
    printf("\tEMCEnergyCorrectionOn  = %d\n",rp->GetEMCEnergyCorrectionOn());
    printf("\tGlobalAltroOffset      = %f\n",rp->GetGlobalAltroOffset());
    printf("\tGlobalAltroThreshold   = %d\n",rp->GetGlobalAltroThreshold());
    printf("\tTimeGateAmpThresh      = %g\n",rp->GetTimeGateAmpThresh());
    printf("\tTimeGateLow            = %g\n",rp->GetTimeGateLow());
    printf("\tTimeGateHigh           = %g\n",rp->GetTimeGateHigh());
    printf("\tNonlinearityCorrectionVersion = %s\n",rp->GetNonlinearityCorrectionVersion());
  }
}

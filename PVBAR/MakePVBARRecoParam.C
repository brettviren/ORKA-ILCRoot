void MakePVBARRecoParam(IlcRecoParam::EventSpecie_t default=IlcRecoParam::kLowMult)
{
  //========================================================================
  //
  // Steering macro for PVBAR reconstruction parameters
  //
  // Author: Yuri Kharlov
  // 01.11.2009
  //========================================================================
  // $Id: MakePVBARRecoParam.C 36642 2009-11-11 08:59:00Z kharlov $ */

  const char* macroname = "MakePVBARRecoParam.C";
  const Int_t firstRun = 0;

  // Activate CDB storage and load geometry from CDB
  IlcCDBManager* cdb = IlcCDBManager::Instance();
  if(!cdb->IsDefaultStorageSet()) cdb->SetDefaultStorage("local://OCDB");
  
  TObjArray *recoParamArray = new TObjArray();
  
  {
    // Reconstruction parameters for cosmic run 2009
    IlcPVBARRecoParam * PVBARRecoParam = IlcPVBARRecoParam::GetDefaultParameters();
    PVBARRecoParam->SetEventSpecie(IlcRecoParam::kCosmic);
    PVBARRecoParam->SetEMCSubtractPedestals(kFALSE);
    PVBARRecoParam->SetEMCMinE(0.025);
    PVBARRecoParam->SetEMCClusteringThreshold(0.05);
    PVBARRecoParam->SetEMCFitterVersion("v0");
    PVBARRecoParam->SetEMCUnfolding(kFALSE);
    PVBARRecoParam->SetEMCEnergyCorrectionOn(kFALSE);
    PVBARRecoParam->SetName("Cosmic2009");
    recoParamArray->AddLast(PVBARRecoParam);
  }
  {
    // Reconstruction parameters for the first pp run 2009
    IlcPVBARRecoParam * PVBARRecoParam = IlcPVBARRecoParam::GetDefaultParameters();
    PVBARRecoParam->SetEventSpecie(IlcRecoParam::kLowMult);
    PVBARRecoParam->SetEMCSubtractPedestals(kFALSE);
    PVBARRecoParam->SetEMCRawDigitThreshold(2);
    PVBARRecoParam->SetEMCMinE(0.012);
    PVBARRecoParam->SetEMCClusteringThreshold(0.22);
    PVBARRecoParam->SetEMCFitterVersion("v0");
    PVBARRecoParam->SetEMCUnfolding(kTRUE);
    PVBARRecoParam->SetEMCSampleQualityCut(10.);
    PVBARRecoParam->SetName("LowMult2009");
    recoParamArray->AddLast(PVBARRecoParam);
  }
  {
    // Reconstruction parameters for the first PbPb run 2010
    IlcPVBARRecoParam * PVBARRecoParam = IlcPVBARRecoParam::GetDefaultParameters();
    PVBARRecoParam->SetEventSpecie(IlcRecoParam::kHighMult);
    PVBARRecoParam->SetEMCSubtractPedestals(kFALSE);
    PVBARRecoParam->SetEMCRawDigitThreshold(2);
    PVBARRecoParam->SetEMCMinE(0.015);
    PVBARRecoParam->SetEMCClusteringThreshold(0.40);
    PVBARRecoParam->SetEMCFitterVersion("v0");
    PVBARRecoParam->SetEMCUnfolding(kTRUE);
    PVBARRecoParam->SetName("HighMult2009");
    recoParamArray->AddLast(PVBARRecoParam);
  }
  { // Reconstruction parameters for "calibration" events
    IlcPVBARRecoParam * PVBARRecoParam = IlcPVBARRecoParam::GetDefaultParameters();
    PVBARRecoParam->SetEventSpecie(IlcRecoParam::kCalib);
    PVBARRecoParam->SetEMCSubtractPedestals(kFALSE);
    PVBARRecoParam->SetEMCMinE(0.025);
    PVBARRecoParam->SetEMCClusteringThreshold(0.05);
    PVBARRecoParam->SetEMCFitterVersion("v0");
    PVBARRecoParam->SetEMCUnfolding(kFALSE);
    PVBARRecoParam->SetEMCEnergyCorrectionOn(kFALSE);
    PVBARRecoParam->SetName("Calib2009");
    recoParamArray->AddLast(PVBARRecoParam);
  }

  // Set the default
  Bool_t defaultIsSet = kFALSE;
  for(Int_t i =0; i < recoParamArray->GetEntriesFast(); i++) {
    IlcDetectorRecoParam *param = (IlcDetectorRecoParam *)recoParamArray->UncheckedAt(i);
    if (!param) continue;
    if (default & param->GetEventSpecie()) {
      param->SetAsDefault();
      defaultIsSet = kTRUE;
    }
  }

  if (!defaultIsSet) {
    Error(macroname,"The default reconstruction parameters are not set! Exiting...");
    return;
  }

  // save in CDB storage
  IlcCDBMetaData *md= new IlcCDBMetaData();
  md->SetResponsible("Yuri Kharlov");
  md->SetComment("Reconstruction parameters PVBAR for cosmic, lowMult and highMult");
  md->SetIlcRootVersion(gSystem->Getenv("ARVERSION"));
  md->SetBeamPeriod(0);
  IlcCDBId id("PVBAR/Calib/RecoParam",firstRun,IlcCDBRunRange::Infinity());
  cdb->GetDefaultStorage()->Put(recoParamArray,id, md);

  return;
}

void MakeGRPRecoParam(IlcRecoParam::EventSpecie_t default=IlcRecoParam::kLowMult) {
//========================================================================
//
// Steering macro for GRP reconstruction parameters
//
//
//========================================================================


  const char* macroname = "MakeGRPRecoParam.C";

  // Activate CDB storage and load geometry from CDB
  IlcCDBManager* cdb = IlcCDBManager::Instance();
  if(!cdb->IsDefaultStorageSet()) cdb->SetDefaultStorage("local://OCDB");
  
  TObjArray *recoParamArray = new TObjArray();

  {
    IlcGRPRecoParam * param = IlcGRPRecoParam::GetCosmicTestParam();
    param->SetEventSpecie(IlcRecoParam::kCosmic);
    param->SetVertexerTracksConstraintITS(kFALSE);
    param->SetVertexerTracksConstraintTPC(kFALSE);
    recoParamArray->AddLast(param);
  }
  {
    // new settings for pass 2reco of Dec09 pp data
    IlcGRPRecoParam * param = IlcGRPRecoParam::GetLowFluxParam();
    param->SetEventSpecie(IlcRecoParam::kLowMult);
    param->SetVertexerTracksConstraintITS(kTRUE);
    Double_t cutsITS[21]={0.1,
                          0.1,
			  0.5,
			  //4, // minimum 4 clusters (default was 5)
			  3, // minimum 3 clusters (was 4)
                          1,
                          3.,
                          100.,
                          1000.,
                          3.,
                          30.,
                          6, // 6: MultiVertexer (was 1)
                          4,
			  // multivertexer settings
			  7., 
			  1e3.,
			  5.0,
			  0.05,
			  10e-4,
			  2.,
			  10.,
			  1.,
			  50.
    };
    param->SetVertexerTracksCutsITS(21,cutsITS);
    param->SetVertexerTracksConstraintTPC(kTRUE);
    recoParamArray->AddLast(param);
  }
  {
    IlcGRPRecoParam * param = IlcGRPRecoParam::GetHighFluxParam();
    param->SetEventSpecie(IlcRecoParam::kHighMult);
    param->SetVertexerTracksConstraintITS(kTRUE);
    Double_t cutsITS[21]={0.1,
                          0.1,
			  0.5,
			  4, // minimum 4 clusters (default was 5)
                          1,
                          3.,
                          100.,
                          1000.,
                          3.,
                          30.,
                          1,
                          1,
			  // multivertexer settings
			  7., 
			  1e3.,
			  5.0,
			  0.05,
			  10e-4,
			  2.,
			  10.,
			  1.,
			  50.
    }; // faster finder algo for Iteration 0
    param->SetVertexerTracksCutsITS(21,cutsITS);
    param->SetVertexerTracksConstraintTPC(kTRUE);
    recoParamArray->AddLast(param);
  }

  // Set the default
  Bool_t defaultIsSet = kFALSE;
  for(Int_t i =0; i < recoParamArray->GetEntriesFast(); i++) {
    IlcDetectorRecoParam *par = (IlcDetectorRecoParam *)recoParamArray->UncheckedAt(i);
    if (!par) continue;
    if (default & par->GetEventSpecie()) {
      par->SetAsDefault();
      defaultIsSet = kTRUE;
    }
  }

  if (!defaultIsSet) {
    Error(macroname,"The default reconstruction parameters are not set! Exiting...");
    return;
  }

  // save in CDB storage
  IlcCDBMetaData *md= new IlcCDBMetaData();
  md->SetResponsible("Cvetan Cheshkov");
  md->SetComment("GRP reconstruction parameters");
  md->SetIlcRootVersion(gSystem->Getenv("ARVERSION"));
  md->SetBeamPeriod(0);
  IlcCDBId id("GRP/Calib/RecoParam",0,IlcCDBRunRange::Infinity());
  cdb->GetDefaultStorage()->Put(recoParamArray,id, md);

  return;
}

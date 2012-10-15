// Macro to add 1/pt correction for specific run range to OADB object
// If both runMin and runMax are negative, the object is added as default (will override old one if any)
// Contact: ruben.shahoyan@cern.ch

void AddPtInvCorr(
		  int runMin,int runMax,
		  TGraph* corrPtInvGloA,  // A side corr. for globals 
		  TGraph* corrPtInvGloC,  // C side corr. for globals
		  TGraph* corrPtInvTPCA,  // A side corr. for TPC tracks 
		  TGraph* corrPtInvTPCC,  // C side corr. for TPC tracks
		  double corrXiniGlo=-1,  // if >0, globals will be re-propagate to vtxTrc starting from this X
		  double corrXiniTPC=-1,  // if >0, TPC tracks will be re-propagate to vtxTPC starting from this X
		  const char* fileOADB = "$OADB/PWGPP/data/CorrPTInv.root",
		  const char* objName  = "CorrPTInv"
)
{
  //
  gSystem->Load("libANALYSIS.so");
  gSystem->Load("libANALYSISilc.so");
  gSystem->Load("libTENDER.so");
  gSystem->Load("libOADB.so");
  //
  //
  TString fileName = fileOADB;
  if (fileName.BeginsWith("$OADB")) fileName.ReplaceAll("$OADB",Form("%s/",IlcAnalysisManager::GetOADBPath()));
  gSystem->ExpandPathName(fileName);
  //
  Bool_t isDefault = kFALSE;
  if (runMin<0 && runMax<0) {
    printf("Run ranges is negative, will add as default object\n");
    isDefault = kTRUE;
  }
  IlcOADBTrackFix* corObj = new IlcOADBTrackFix(isDefault ? "default" : Form("corrPTInv_%d_%d",runMin,runMax));
  corObj->SetPtInvCorr(IlcOADBTrackFix::kCorModeGlob,0,corrPtInvGloA);
  corObj->SetPtInvCorr(IlcOADBTrackFix::kCorModeGlob,1,corrPtInvGloC);
  corObj->SetXIniPtInvCorr(IlcOADBTrackFix::kCorModeGlob, corrXiniGlo);
  //
  corObj->SetPtInvCorr(IlcOADBTrackFix::kCorModeTPCInner,0,corrPtInvTPCA);
  corObj->SetPtInvCorr(IlcOADBTrackFix::kCorModeTPCInner,1,corrPtInvTPCC);
  corObj->SetXIniPtInvCorr(IlcOADBTrackFix::kCorModeTPCInner, corrXiniTPC);
  //
  IlcOADBContainer *oadbCont = new IlcOADBContainer(objName);
  //
  if (oadbCont->InitFromFile(fileName.Data(),objName)) {
    printf("New object will be created\n");
    oadbCont->SetNameTitle(objName, "object for 1/pt correction");
  }
  if (isDefault) {
    oadbCont->CleanDefaultList();
    oadbCont->AddDefaultObject(corObj);
  }
  else oadbCont->AppendObject(corObj, runMin,runMax);
  //
  oadbCont->WriteToFile(fileName.Data());
  //
}

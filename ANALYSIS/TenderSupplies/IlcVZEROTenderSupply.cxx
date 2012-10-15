/**************************************************************************
 * 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * Author: The International Lepton Collider Off-line Project.            *
 *                                                                        *
 * Part of the code has been developed by ILC Off-line Project.         *
 * Contributors are mentioned in the code where appropriate.              *
 *                                                                        *
 * Permission to use, copy, modify and distribute this software and its   *
 * documentation strictly for non-commercial purposes is hereby granted   *
 * without fee, provided that the above copyright notice appears in all   *
 * copies and that both the copyright notice and this permission notice   *
 * appear in the supporting documentation. The authors make no claims     *
 * about the suitability of this software for any purpose. It is          *
 * provided "as is" without express or implied warranty.                  *
 **************************************************************************/


///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//  Recalculate VZERO timing and decision using the tender                   //
//  (in case the LHC phase drift is updated in OCDB)                         //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#include <TList.h>
#include <TObjString.h>
#include <TChain.h>
#include <TF1.h>

#include <IlcLog.h>
#include <IlcESDEvent.h>
#include <IlcESDVZERO.h>
#include <IlcCDBId.h>
#include <IlcCDBManager.h>
#include <IlcCDBEntry.h>
#include <IlcTender.h>
#include <IlcLHCClockPhase.h>
#include <IlcVZEROCalibData.h>
#include <IlcVZEROTriggerMask.h>
#include <IlcVZEROReconstructor.h>

#include "IlcVZEROTenderSupply.h"

ClassImp(IlcVZEROTenderSupply)

IlcVZEROTenderSupply::IlcVZEROTenderSupply() :
  IlcTenderSupply(),
  fCalibData(NULL),
  fTimeSlewing(NULL),
  fRecoParam(NULL),
  fLHCClockPhase(0),
  fDebug(kFALSE)
{
  //
  // default ctor
  //
}

//_____________________________________________________
IlcVZEROTenderSupply::IlcVZEROTenderSupply(const char *name, const IlcTender *tender) :
  IlcTenderSupply(name,tender),
  fCalibData(NULL),
  fTimeSlewing(NULL),
  fRecoParam(NULL),
  fLHCClockPhase(0),
  fDebug(kFALSE)
{
  //
  // named ctor
  //
}

//_____________________________________________________
void IlcVZEROTenderSupply::Init()
{
  //
  // Initialise VZERO tender
  //
}

//_____________________________________________________
void IlcVZEROTenderSupply::ProcessEvent()
{
  //
  // Reapply the LHC-clock phase drift
  //

  IlcESDEvent *event=fTender->GetEvent();
  if (!event) return;
  
  //load gain correction if run has changed
  if (fTender->RunChanged()){
    if (fDebug) printf("IlcVZEROTenderSupply::ProcessEvent - Run Changed (%d)\n",fTender->GetRun());
    GetPhaseCorrection();

    IlcCDBEntry *entryGeom = fTender->GetCDBManager()->Get("GRP/Geometry/Data",fTender->GetRun());
    if (!entryGeom) {
      IlcError("No geometry entry is found");
      return;
    } else {
      if (fDebug) printf("IlcVZEROTenderSupply::Used geometry entry: %s\n",entryGeom->GetId().ToString().Data());
    }

    IlcCDBEntry *entryCal = fTender->GetCDBManager()->Get("VZERO/Calib/Data",fTender->GetRun());
    if (!entryCal) {
      IlcError("No VZERO calibration entry is found");
      fCalibData = NULL;
      return;
    } else {
      fCalibData = (IlcVZEROCalibData*)entryCal->GetObject();
      if (fDebug) printf("IlcVZEROTenderSupply::Used VZERO calibration entry: %s\n",entryCal->GetId().ToString().Data());
    }

    IlcCDBEntry *entrySlew = fTender->GetCDBManager()->Get("VZERO/Calib/TimeSlewing",fTender->GetRun());
    if (!entrySlew) {
      IlcError("VZERO time slewing function is not found in OCDB !");
      fTimeSlewing = NULL;
      return;
    } else {
      fTimeSlewing = (TF1*)entrySlew->GetObject();
      if (fDebug) printf("IlcVZEROTenderSupply::Used VZERO time slewing entry: %s\n",entrySlew->GetId().ToString().Data());
    }

    IlcCDBEntry *entryRecoParam = fTender->GetCDBManager()->Get("VZERO/Calib/RecoParam",fTender->GetRun());
    if (!entryRecoParam) {
      IlcError("VZERO reco-param object is not found in OCDB !");
      fRecoParam = NULL;
      return;
    } else {
      TObjArray *recoParamArr = (TObjArray*)entryRecoParam->GetObject();
      if (fDebug) printf("IlcVZEROTenderSupply::Used VZERO reco-param entry: %s\n",entryRecoParam->GetId().ToString().Data());
      fRecoParam = NULL;
      for(Int_t i = 0; i < recoParamArr->GetEntriesFast(); i++) {
	IlcVZERORecoParam *par = (IlcVZERORecoParam*)recoParamArr->At(i);
	if (!par) continue;
	if (par->IsDefault()) fRecoParam = par;
      }
      if (!fRecoParam) IlcError("No default VZERO reco-param object is found in OCDB !");
    }
  }

  if (!fCalibData || !fTimeSlewing || !fRecoParam) {
    IlcWarning("VZERO calibration objects not found!");
    return;
  }
  //
  // correct VZERO time signals and decision
  //
  IlcESDVZERO *esdVZERO = event->GetVZEROData();
  if (!esdVZERO) {
    IlcError("No VZERO object is found inside ESD!");
    return;
  }
  if (!esdVZERO->TestBit(IlcESDVZERO::kDecisionFilled)) {
    IlcWarning("VZERO offline trigger decisions were not filled in ESD, the tender supply is disabled");
    return;
  }

  if (fDebug) printf("LHC-clock phase correction: %f\n",fLHCClockPhase);

  if (fDebug) printf("Original VZERO decision %d (%f ns) and %d (%f ns)\n",
		     esdVZERO->GetV0ADecision(),esdVZERO->GetV0ATime(),
		     esdVZERO->GetV0CDecision(),esdVZERO->GetV0CTime());
  Float_t time[64];
  for(Int_t i = 0; i < 64; ++i) {
    time[i] = esdVZERO->GetTime(i);
    if (time[i] > (IlcVZEROReconstructor::kInvalidTime + 1e-6))
      time[i] += fLHCClockPhase;
  }
  esdVZERO->SetTime(time);

  {
    IlcVZEROTriggerMask triggerMask;
    triggerMask.SetRecoParam(fRecoParam);
    triggerMask.FillMasks(esdVZERO, fCalibData, fTimeSlewing);
  }
  if (fDebug) printf("Modified VZERO decision %d (%f ns) and %d (%f ns)\n",
		     esdVZERO->GetV0ADecision(),esdVZERO->GetV0ATime(),
		     esdVZERO->GetV0CDecision(),esdVZERO->GetV0CTime());

}

//_____________________________________________________
void IlcVZEROTenderSupply::GetPhaseCorrection()
{
  //
  // Get Gain splines from OCDB
  //

  IlcInfo("Get LHC-clock phase correction");

  //
  //find previous entry from the UserInfo
  //
  TTree *tree=((TChain*)fTender->GetInputData(0))->GetTree();
  if (!tree) {
    IlcError("Tree not found in ESDhandler");
    return;
  }
  
  TList *userInfo=(TList*)tree->GetUserInfo();
  if (!userInfo) {
    IlcError("No UserInfo found in tree");
    return;
  }

  TList *cdbList=(TList*)userInfo->FindObject("cdbList");
  if (!cdbList) {
    IlcError("No cdbList found in UserInfo");
    if (IlcLog::GetGlobalLogLevel()>=IlcLog::kError) userInfo->Print();
    return;
  }

  Float_t oldPhase = 0;

  TIter nextCDB(cdbList);
  TObjString *os=0x0;
  while ( (os=(TObjString*)nextCDB()) ){
    if (!(os->GetString().Contains("GRP/Calib/LHCClockPhase"))) continue;
    IlcCDBId *id=IlcCDBId::MakeFromString(os->GetString());
    
    IlcCDBEntry *entry=fTender->GetCDBManager()->Get(*id);
    if (!entry) {
      IlcError("The previous LHC-clock phase entry is not found");
      delete id;
      return;
    }
    
    if (fDebug) printf("IlcVZEROTenderSupply::Used old LHC-clock phase entry: %s\n",entry->GetId().ToString().Data());
    
    IlcLHCClockPhase *phase = (IlcLHCClockPhase*)entry->GetObject();
    if (!phase) {
      IlcError("Phase object is not found in the calibration entry");
      delete id;
      return;
    }

    oldPhase = phase->GetMeanPhase();

    delete id;
    break;
  }

  //
  //new LHC-clock phase entry
  //
  Float_t newPhase = 0;
  IlcCDBEntry *entryNew=fTender->GetCDBManager()->Get("GRP/Calib/LHCClockPhase",fTender->GetRun());
  if (!entryNew) {
    IlcError("No new LHC-clock phase calibration entry is found");
    return;
  }
  if (fDebug) printf("IlcVZEROTenderSupply::Used new LHC-clock phase entry: %s\n",entryNew->GetId().ToString().Data());
  
  IlcLHCClockPhase *phase2 = (IlcLHCClockPhase*)entryNew->GetObject();
  if (!phase2) {
    IlcError("Phase object is not found in the calibration entry");
    return;
  }

  newPhase = phase2->GetMeanPhase();

  fLHCClockPhase = newPhase - oldPhase;
}

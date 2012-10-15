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

////////////////////////////////////////////////////////////////////////////
//                                                                        //
// IlcGRPManager class                                                    //
// The class can be used in order to access and read the Global Run       //
// Parameters entry from OCDB.                                            //
// It has a methods to set the magnetic field instanton and return        //
// the run and event info objects.                                        //
//                                                                        //
// cvetan.cheshkov@cern.ch 15/06/2009                                     //
//                                                                        //
// Usage:                                                                 //
// IlcGRPManager grpMan;                                                  //
// Bool_t status = kTRUE;                                                 //
// status = grpMan.ReadGRPEntry(); // Read the corresponding OCDB entry   //
// status = grpMan.SetMagField();  // Set global field instanton          //
// IlcRunInfo *runInfo = grpMan.GetRunInfo();// Get instance of run info  //
//                                                                        //
// Note: CDB manager should be initialized beforehand                     //
////////////////////////////////////////////////////////////////////////////

#include <TGeoGlobalMagField.h>

#include "IlcGRPManager.h"
#include "IlcLog.h"
#include "IlcRunInfo.h"
#include "IlcRunLoader.h"
#include "IlcGRPObject.h"
#include "IlcCDBManager.h"
#include "IlcCDBEntry.h"
#include "IlcMagF.h"

ClassImp(IlcGRPManager)

//_____________________________________________________________________________
IlcGRPManager::IlcGRPManager() :
  TObject(),
  fGRPData(NULL)
{
  // Default constructor
}

//_____________________________________________________________________________
IlcGRPManager::~IlcGRPManager()
{
  // Destructor
  if (fGRPData) delete fGRPData;
}

//_____________________________________________________________________________
Bool_t IlcGRPManager::ReadGRPEntry()
{
  //------------------------------------
  // Initialization of the GRP entry. 
  // Returns kTRUE in case of success.
  //------------------------------------
  IlcWarning("ReadGRPEntry method disabled");
  return 1;

  IlcCDBEntry* entry = IlcCDBManager::Instance()->Get("GRP/GRP/Data");

  if (entry) {

    TMap* m = dynamic_cast<TMap*>(entry->GetObject());  // old GRP entry

    if (m) {
       IlcInfo("Found a TMap in GRP/GRP/Data, converting it into an IlcGRPObject");
       m->Print();
       fGRPData = new IlcGRPObject();
       fGRPData->ReadValuesFromMap(m);
    }

    else {
       IlcInfo("Found an IlcGRPObject in GRP/GRP/Data, reading it");
       fGRPData = dynamic_cast<IlcGRPObject*>(entry->GetObject());  // new GRP entry
       entry->SetOwner(0);
    }

    IlcCDBManager::Instance()->UnloadFromCache("GRP/GRP/Data");
  }

  if (!fGRPData) {
     IlcError("No GRP entry found in OCDB!");
     return kFALSE;
  }

  return kTRUE;
}

//_____________________________________________________________________________
Bool_t IlcGRPManager::SetMagField()
{
  // Dealing with the magnetic field map
  // Construct the mag field map from the data in GRP
  // Set the global mag field instance

  //IlcMagF* fld = fGRPData->GetMagField();
  Bool_t check = kFALSE;
  TDirectory * olddir = gDirectory;
  TFile *tmpfile = TFile::Open("gilc.root");

  if(!tmpfile ){
    IlcError("Couldn't open gilc.root for this run");
    return check;
  }
  
  IlcMagF* MagField = (IlcMagF*)gDirectory->Get("MagField");
  if (MagField) {
    TGeoGlobalMagField::Instance()->SetField(MagField);
    TGeoGlobalMagField::Instance()->Lock();
    IlcInfo("Running with the B field stored in gIlc");
    check = kTRUE;
  }
  tmpfile->Close();
  olddir->cd();
  return check;

  if ( TGeoGlobalMagField::Instance()->IsLocked() ) {
    if (TGeoGlobalMagField::Instance()->GetField()->TestBit(IlcMagF::kOverrideGRP)) {
      IlcInfo("ExpertMode!!! GRP information will be ignored !");
      IlcInfo("ExpertMode!!! Running with the externally locked B field !");
      return kTRUE;
    }
    else {
      IlcInfo("Destroying existing B field instance!");
      delete TGeoGlobalMagField::Instance();
    }
  }
  //
  if (!fGRPData) {
    IlcError("GRP Data is not loaded");
    return kFALSE;
  }
  //
  // Construct the field map out of the information retrieved from GRP.
  Bool_t ok = kTRUE;
  // L3
  Float_t l3Current = fGRPData->GetL3Current((IlcGRPObject::Stats)0);
  if (l3Current == IlcGRPObject::GetInvalidFloat()) {
    IlcError("GRP/GRP/Data entry:  missing value for the L3 current !");
    ok = kFALSE;
  }
  
  Char_t l3Polarity = fGRPData->GetL3Polarity();
  if (l3Polarity == IlcGRPObject::GetInvalidChar()) {
    IlcError("GRP/GRP/Data entry:  missing value for the L3 polarity !");
    ok = kFALSE;
  }
  
  // Dipole
  Float_t diCurrent = fGRPData->GetDipoleCurrent((IlcGRPObject::Stats)0);
  if (diCurrent == IlcGRPObject::GetInvalidFloat()) {
    IlcError("GRP/GRP/Data entry:  missing value for the dipole current !");
    ok = kFALSE;
  }
  
  Char_t diPolarity = fGRPData->GetDipolePolarity();
  if (diPolarity == IlcGRPObject::GetInvalidChar()) {
    IlcError("GRP/GRP/Data entry:  missing value for the dipole polarity !");
    ok = kFALSE;
  }
  
  TString beamType = fGRPData->GetBeamType();
  if (beamType==IlcGRPObject::GetInvalidString()) {
    IlcError("GRP/GRP/Data entry:  missing value for the beam type ! Using UNKNOWN");
    beamType = "UNKNOWN";
    //ok = kFALSE;  // temprorary suppressed to make read cosmics data
  }
  
  Float_t beamEnergy = fGRPData->GetBeamEnergy();
  if (beamEnergy==IlcGRPObject::GetInvalidFloat()) {
    IlcError("GRP/GRP/Data entry:  missing value for the beam energy ! Using 0");
    beamEnergy = 0;
    //ok = kFALSE;  // temprorary suppressed to make read cosmics data
  }
  
  // read special bits for the polarity convention and map type
  Int_t  polConvention = fGRPData->IsPolarityConventionLHC() ? IlcMagF::kConvLHC : IlcMagF::kConvDCS2008;
  Bool_t uniformB = fGRPData->IsUniformBMap();
  
  if (ok) {
    
    IlcMagF* fld = fGRPData->GetMagField();
//     IlcMagF* fld = new IlcMagF("Maps","Maps", 1.5, 1., IlcMagF::kConst, IlcMagF::kNoBeamField);
//     IlcMagF* fld = IlcMagF::CreateFieldMap(TMath::Abs(l3Current) * (l3Polarity ? -1:1), 
// 					   TMath::Abs(diCurrent) * (diPolarity ? -1:1), 
// 					   polConvention,uniformB,beamEnergy, beamType.Data());
    if (fld) {
      TGeoGlobalMagField::Instance()->SetField( fld );
      TGeoGlobalMagField::Instance()->Lock();
      IlcInfo("Running with the B field constructed out of GRP !");
    }
    else {
      IlcError("Failed to create a B field map !");
      ok = kFALSE;
    }
  }
  else {
    IlcError("B field is neither set nor constructed from GRP ! Exitig...");
  }
  
  return ok;
}

//_____________________________________________________________________________
IlcRunInfo* IlcGRPManager::GetRunInfo()
{
  // Constructs and returns an object
  // containing the run information
  // The user code is the owner of the object

  TString lhcState = fGRPData->GetLHCState();
  if (lhcState==IlcGRPObject::GetInvalidString()) {
    IlcError("GRP/GRP/Data entry:  missing value for the LHC state ! Using UNKNOWN");
    lhcState = "UNKNOWN";
  }

  TString beamType = fGRPData->GetBeamType();
  if (beamType==IlcGRPObject::GetInvalidString()) {
    IlcError("GRP/GRP/Data entry:  missing value for the beam type ! Using UNKNOWN");
    beamType = "UNKNOWN";
  }

  Float_t beamEnergy = fGRPData->GetBeamEnergy();
  if (beamEnergy==IlcGRPObject::GetInvalidFloat()) {
    IlcError("GRP/GRP/Data entry:  missing value for the beam energy ! Using 0");
    beamEnergy = 0;
  }
  // energy is provided in MeV*120
  beamEnergy /= 120E3;

  TString runType = fGRPData->GetRunType();
  if (runType==IlcGRPObject::GetInvalidString()) {
    IlcError("GRP/GRP/Data entry:  missing value for the run type ! Using UNKNOWN");
    runType = "UNKNOWN";
  }

  Int_t activeDetectors = fGRPData->GetDetectorMask();
  if (activeDetectors==IlcGRPObject::GetInvalidUInt()) {
    IlcError("GRP/GRP/Data entry:  missing value for the detector mask ! Using 1074790399");
    activeDetectors = 1074790399;
  }

  return new IlcRunInfo(lhcState, beamType, beamEnergy, runType, activeDetectors);
}

//_____________________________________________________________________________
void IlcGRPManager::SetGRPEntry(const IlcGRPObject* source)
{
  // Create a GRP entry from the extrnaly provide GRP object
  // To be used by HLT to create an online GRP instance
  if (!source) return;
  if (fGRPData) delete fGRPData;
  fGRPData = new IlcGRPObject(*source);
  IlcInfo("Created GRP Data from external object");
  //
}
 

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

/* $Id: IlcPVECRecoParam.cxx 53524 2011-12-11 21:34:35Z policheh $ */

// Base class for the PVEC reconstruction parameters.
// Do not use in the reconstruction; use derivative classes instead.
// Author: Boris Polichtchouk.

// --- IlcRoot header files ---
#include "IlcCDBManager.h"
#include "IlcCDBEntry.h"
#include "IlcLog.h"
#include "IlcPVECRecoParam.h"

ClassImp(IlcPVECRecoParam)

TObjArray* IlcPVECRecoParam::fgkMaps =0; //ALTRO mappings
TObject*   IlcPVECRecoParam::fgkTrigParams =0; // Trigger parameters

//-----------------------------------------------------------------------------
IlcPVECRecoParam::IlcPVECRecoParam() :
  IlcDetectorRecoParam(),
  fNonLinearityParams(3),
  fEMCClusteringThreshold(0.2),
  fEMCLocMaxCut(0.03),
  fEMCRawDigitThreshold(2),
  fEMCMinE(0.012),
  fEMCW0(4.5),
  fEMCSampleQualityCut(1.),
  fTimeGateAmpThresh(10.),
  fTimeGateLow(1.e-6),
  fTimeGateHigh(1.e-6),
  fEMCEcoreRadius(3.),
  fEMCEcore2ESD(kFALSE),
  fEMCSubtractPedestals(kTRUE),
  fEMCUnfold(kTRUE),
  fEMCEnergyCorrectionOn(kTRUE),
  fEMCFitterVersion(""),
  fNonlinearityCorrVersion(""),
  fGlobalAltroOffset(0),
  fGlobalAltroThreshold(0),
  fCPVClusteringThreshold(0.0),
  fCPVLocMaxCut(0.03),
  fCPVMinE(0.0),
  fCPVW0(4.0),
  fCPVUnfold(kTRUE)
{
  //Default constructor.
  SetNonlinearityCorrectionVersion("Gustavo2005") ;
}

//-----------------------------------------------------------------------------
IlcPVECRecoParam::IlcPVECRecoParam(const IlcPVECRecoParam& ):
  IlcDetectorRecoParam(),
  fNonLinearityParams(3),
  fEMCClusteringThreshold(0.2),
  fEMCLocMaxCut(0.03),
  fEMCRawDigitThreshold(2),
  fEMCMinE(0.012),
  fEMCW0(4.5),
  fEMCSampleQualityCut(1.),
  fTimeGateAmpThresh(10.),
  fTimeGateLow(1.e-6),
  fTimeGateHigh(1.e-6),
  fEMCEcoreRadius(3.),
  fEMCEcore2ESD(kFALSE),
  fEMCSubtractPedestals(kTRUE),
  fEMCUnfold(kTRUE),
  fEMCEnergyCorrectionOn(kTRUE),
  fEMCFitterVersion(""),
  fNonlinearityCorrVersion(""),
  fGlobalAltroOffset(0),
  fGlobalAltroThreshold(0),
  fCPVClusteringThreshold(0.0),
  fCPVLocMaxCut(0.03),
  fCPVMinE(0.0),
  fCPVW0(4.0),
  fCPVUnfold(kTRUE)
{
  //Copy constructor.
}

//-----------------------------------------------------------------------------
IlcPVECRecoParam& IlcPVECRecoParam::operator = (const IlcPVECRecoParam& recoParam)
{
  //Assignment operator.

  if(this != &recoParam) {
    fNonLinearityParams     = recoParam.fNonLinearityParams;
    fEMCClusteringThreshold = recoParam.fEMCClusteringThreshold;
    fEMCLocMaxCut           = recoParam.fEMCLocMaxCut;
    fEMCRawDigitThreshold   = recoParam.fEMCRawDigitThreshold;
    fEMCMinE                = recoParam.fEMCMinE;
    fEMCW0                  = recoParam.fEMCW0;
    fEMCSampleQualityCut    = recoParam.fEMCSampleQualityCut;
    fTimeGateAmpThresh      = recoParam.fTimeGateAmpThresh ;
    fTimeGateLow            = recoParam.fTimeGateLow ;
    fTimeGateHigh           = recoParam.fTimeGateHigh ;
    fEMCEcoreRadius         = recoParam.fEMCEcoreRadius;
    fEMCEcore2ESD           = recoParam.fEMCEcore2ESD;
    fEMCSubtractPedestals   = recoParam.fEMCSubtractPedestals;
    fEMCUnfold              = recoParam.fEMCUnfold;
    fEMCEnergyCorrectionOn  = recoParam.fEMCEnergyCorrectionOn;
    fEMCFitterVersion       = recoParam.fEMCFitterVersion;
    fNonlinearityCorrVersion= recoParam.fNonlinearityCorrVersion;
    fGlobalAltroOffset      = recoParam.fGlobalAltroOffset;
    fGlobalAltroThreshold   = recoParam.fGlobalAltroThreshold;
    fCPVClusteringThreshold = recoParam.fCPVClusteringThreshold;
    fCPVLocMaxCut           = recoParam.fCPVLocMaxCut;
    fCPVMinE                = recoParam.fCPVMinE;
    fCPVW0                  = recoParam.fCPVW0;
    fCPVUnfold              = recoParam.fCPVUnfold;
    fNonLinearityParams     = recoParam.fNonLinearityParams ;
  }

  return *this;
}

//-----------------------------------------------------------------------------
void IlcPVECRecoParam::Print(Option_t * /*option*/) const
{
  IlcDebug(2,Form("PVEC reconstruction parameters:\n"
		  "\tEMCClusteringThreshold = %f\n"
		  "\tEMCLocMaxCut           = %f\n"
		  "\tEMCRawDigitThreshold   = %f\n"
		  "\tEMCMinE                = %f\n"
		  "\tEMCW0                  = %f\n"
		  "\tEMCSampleQualityCut    = %f\n"
		  "\tEMCEcoreRadius         = %f\n"
		  "\tEMCEcore2ESD           = %d\n"
		  "\tEMCSubtractPedestals   = %d\n"
		  "\tEMCUnfold              = %d\n"
		  "\tEMCEnergyCorrectionOn  = %d\n"
		  "\tEMCFitterVersion       = \"%s\"\n"
		  "\tGlobalAltroOffset      = %d\n"
		  "\tGlobalAltroThreshold   = %d",
		  fEMCClusteringThreshold,
		  fEMCLocMaxCut,
		  fEMCRawDigitThreshold,
		  fEMCMinE,
		  fEMCW0,
		  fEMCSampleQualityCut,
		  fEMCEcoreRadius,
		  fEMCEcore2ESD,
		  fEMCSubtractPedestals,
		  fEMCUnfold,
		  fEMCEnergyCorrectionOn,
		  fEMCFitterVersion.Data(),
		  fGlobalAltroOffset,
		  fGlobalAltroThreshold));

}

//-----------------------------------------------------------------------------
IlcPVECRecoParam* IlcPVECRecoParam::GetDefaultParameters()
{
  //Default parameters for the reconstruction

  IlcPVECRecoParam* params = new IlcPVECRecoParam();
  return params;
}

//-----------------------------------------------------------------------------
const TObjArray* IlcPVECRecoParam::GetMappings()
{
  //Returns array of IlcAltroMappings for RCU0..RCU3.
  //If not found, read it from OCDB.

  //Quick check as follows:
  //  root [0] IlcCDBManager::Instance()->SetDefaultStorage("local://$ILC_ROOT/OCDB");
  //  root [1] IlcCDBManager::Instance()->SetRun(1);
  //  root [2] TObjArray* maps = IlcPVECRecoParam::GetMappings();
  //  root [3] maps->Print();
  
  if(fgkMaps) return fgkMaps;
  
  IlcCDBEntry* entry = IlcCDBManager::Instance()->Get("PVEC/Calib/Mapping");
  if(entry)
    fgkMaps = (TObjArray*)entry->GetObject();
  
  return fgkMaps;
  
}
//-----------------------------------------------------------------------------
void IlcPVECRecoParam::SetNonlinearityCorrectionVersion(const char * ver){
  //Set non-linearity correction version and adjust parameters

  if(strcmp(ver,"NoCorrection")==0){
    fNonLinearityParams.Set(1) ;
    fNonLinearityParams.AddAt(0.,0) ;
    fNonlinearityCorrVersion=ver ;
    return ;
  }
  if(strcmp(ver,"Henrik2010")==0){
    Float_t pars[7]={1.051,2.54e-003,-1.737e-002,5.69e-002,3.3e-001,1.42e-001,1.50e-002} ;
    fNonLinearityParams.Set(7,pars) ;
    fNonlinearityCorrVersion=ver ;
    return ;
  }
  if(strcmp(ver,"Gustavo2005")==0){
    Float_t pars[3]={0.0241, 1.0504, 0.000249} ;
    fNonLinearityParams.Set(3,pars) ;
    fNonlinearityCorrVersion=ver ;
    return ;
  }
  IlcError(Form("Non known correction version: %s, still using default \n",ver)) ;


}

//-----------------------------------------------------------------------------
const TObject* IlcPVECRecoParam::GetTriggerParameters()
{
  //Returns trigger parameters.

  //Quick check as follows:
  //  root [0] IlcCDBManager::Instance()->SetDefaultStorage("local://$ILC_ROOT/OCDB");
  //  root [1] IlcCDBManager::Instance()->SetRun(158171);
  //  root [2] TObject* parameters = IlcPVECRecoParam::GetTriggerParameters();
  //  root [3] parameters->Print();
  
  if(fgkTrigParams) return fgkTrigParams;
  
  IlcCDBEntry* entry = IlcCDBManager::Instance()->Get("PVEC/Trigger/Parameters");
  if(entry)
    fgkTrigParams = entry->GetObject();
  
  return fgkTrigParams;
  
}

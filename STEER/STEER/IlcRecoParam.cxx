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
// ILC Reconstruction parameterization:                                    //
//                                                                           //
//                                                                           //
// Base Class for Detector reconstruction parameters                         //
// Revision: cvetan.cheshkov@cern.ch 12/06/2008                              //
// Its structure has been revised and it is interfaced to IlcEventInfo.      //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#include "TClass.h"
#include "TObjArray.h"
#include "TMath.h"
#include "THashTable.h"
#include "TString.h"
#include "TRegexp.h"
#include "IlcDetectorRecoParam.h"

#include "IlcLog.h"
#include "IlcRecoParam.h"
#include "IlcRunInfo.h"
#include "IlcEventInfo.h"
#include "IlcLog.h"

ClassImp(IlcRecoParam)

TString IlcRecoParam::fkgEventSpecieName[] = {"Default", "LowMultiplicity", "HighMultiplicity", "Cosmic", "Calib", "Unknown"} ; 

IlcRecoParam::IlcRecoParam(): 
  TObject(),
  fEventSpecie(kDefault)
{
  // Default constructor
  // ...
  for(Int_t iDet = 0; iDet < kNDetectors; iDet++)
    fDetRecoParams[iDet] = NULL;
  for(Int_t iSpecie = 0; iSpecie < kNSpecies; iSpecie++) {
    for(Int_t iDet = 0; iDet < kNDetectors; iDet++) {
      fDetRecoParamsIndex[iSpecie][iDet] = -1;
    }
  }
}

IlcRecoParam::IlcRecoParam(const IlcRecoParam& par) :
  TObject(),
  fEventSpecie(par.fEventSpecie)
{
  // copy constructor
  for(Int_t iDet = 0; iDet < kNDetectors; iDet++) {
    if (par.fDetRecoParams[iDet])
      fDetRecoParams[iDet] = (TObjArray*)(par.fDetRecoParams[iDet]->Clone());
    else
      fDetRecoParams[iDet] = NULL;
  }
  for(Int_t iSpecie = 0; iSpecie < kNSpecies; iSpecie++) {
    for(Int_t iDet = 0; iDet < kNDetectors; iDet++) {
      fDetRecoParamsIndex[iSpecie][iDet] = par.fDetRecoParamsIndex[iSpecie][iDet];
    }
  }
}

//_____________________________________________________________________________
IlcRecoParam& IlcRecoParam::operator = (const IlcRecoParam& par)
{
  // assignment operator

  if(&par == this) return *this;

  this->~IlcRecoParam();
  new(this) IlcRecoParam(par);
  return *this;
}

IlcRecoParam::~IlcRecoParam(){
  // Destructor
  // ...
  // Delete the array with the reco-param objects
  for(Int_t iDet = 0; iDet < kNDetectors; iDet++) {
    if (fDetRecoParams[iDet]){
      fDetRecoParams[iDet]->Delete();
      delete fDetRecoParams[iDet];
    }
  }
}

Int_t IlcRecoParam::AConvert(EventSpecie_t es)
{
  //Converts EventSpecie_t  into int
  Int_t rv = -1 ; 
  switch (es) {
    case kDefault:
      rv = 0 ; 
      break;
    case kLowMult:
      rv = 1 ; 
      break;
    case kHighMult:
      rv = 2 ; 
      break;
    case kCosmic:
      rv = 3 ; 
      break;
    case kCalib:
      rv = 4 ; 
      break;
    default:
      break;
  }

  if (rv < 0) 
    IlcFatalClass(Form("Wrong event specie conversion %d", es)) ; 

  return rv ;
}

IlcRecoParam::EventSpecie_t IlcRecoParam::Convert(Int_t ies)
{
  //Converts int into EventSpecie_t
  IlcRecoParam::EventSpecie_t es = kDefault ; 
  if ( ies >> 1) 
    es = kLowMult ; 
  if ( ies >> 2) 
    es = kHighMult ;   
  if ( ies >> 3) 
    es = kCosmic ;   
  if ( ies >> 4) 
    es = kCalib ;

  return es ;   
}

IlcRecoParam::EventSpecie_t IlcRecoParam::ConvertIndex(Int_t index)
{
  //Converts index of lists into eventspecie
  EventSpecie_t es = kDefault ; 
  switch (index) {
    case 0:
      es = kDefault ; 
      break;
    case 1:
      es = kLowMult ; 
      break;
    case 2:
      es = kHighMult ;   
      break;
    case 3:
      es = kCosmic ;   
      break;
    case 4:
      es = kCalib ;
      break;
    default:
      break;
  }
  return es ;
}

void  IlcRecoParam::Print(Option_t *option) const {
  //
  // Print reconstruction setup
  //
  for(Int_t iDet = 0; iDet < kNDetectors; iDet++) {
    if (fDetRecoParams[iDet]){
      printf("IlcDetectorRecoParam objects for detector %d:\n",iDet); 
      Int_t nparam = fDetRecoParams[iDet]->GetEntriesFast();
      for (Int_t iparam=0; iparam<nparam; iparam++){
	IlcDetectorRecoParam * param = (IlcDetectorRecoParam *)fDetRecoParams[iDet]->At(iparam);
	if (!param) continue;
	param->Print(option);
      }
    }
    else {
      printf("No IlcDetectorRecoParam objects specified for detector %d\n",iDet); 
    }
  }
}

void IlcRecoParam::SetEventSpecie(const IlcRunInfo *runInfo, const IlcEventInfo &evInfo,
	const THashTable *cosmicTriggersList)
{
    // Implemented according to the discussions
    // and meetings with physics and trigger coordination

  IlcWarning("SetEventSpecie method disabled");
    return;


    fEventSpecie = kDefault;

    if (strcmp(runInfo->GetRunType(),"PHYSICS")) {
	// Not a physics run, the event specie is set to kCalib
	fEventSpecie = kCalib;
	return;
    }

    // Special DAQ events considered as calibration events
    if (evInfo.GetEventType() != 7) {
	// START_OF_*, END_OF_*, CALIBRATION etc events
	fEventSpecie = kCalib;
	return;
    }

    TString lhcState(runInfo->GetLHCState());
    TString beamType(runInfo->GetBeamType());
    TRegexp reStable("^STABLE[_ ]BEAMS$");
    TRegexp reASthg("^A-");
    TRegexp reSthgA(".*-A$");
    TRegexp repSthg("^[pP]-.*");
    TRegexp reSthgp(".*-[pP]$");

    if(lhcState.Index(reStable)==0){
        if(beamType.Index(repSthg)==0 || beamType.Index(reSthgp)==0){
            // Proton run, the event specie is set to kLowMult
            fEventSpecie = kLowMult;
        }else if(beamType.Index(reASthg)==0 || beamType.Index(reSthgA)==0){
            // Heavy ion run (any beam that is not pp, the event specie is set to kHighMult
            fEventSpecie = kHighMult;
	}
    }
    if(beamType==TString("-")){
	// No beams, we assume cosmic data
	fEventSpecie = kCosmic;
    }

    // Now we look into the trigger type in order to decide
    // on the remaining cases (cosmic event within LHC run,
    // calibration, for example TPC laser, triggers within physics run
    TString triggerClasses = evInfo.GetTriggerClasses();
    TObjArray* trClassArray = triggerClasses.Tokenize(" ");
    Int_t nTrClasses = trClassArray->GetEntriesFast();
    Bool_t cosmicTrigger = kFALSE,
	   calibTrigger = kFALSE,
	   otherTrigger = kFALSE;
    for( Int_t i=0; i<nTrClasses; ++i ) {
	TString trClass = ((TObjString*)trClassArray->At(i))->String();

	if (trClass.BeginsWith("C0L")) {
	    // Calibration triggers always start with C0L
	    calibTrigger = kTRUE;
	    continue;
	}

	if (cosmicTriggersList) {
	    if (cosmicTriggersList->FindObject(trClass.Data())) {
		// Cosmic trigger accorind to the table
		// provided in OCDB
		cosmicTrigger = kTRUE;
		IlcDebug(1,Form("Trigger %s identified as cosmic according to the list defined in OCDB.",
			    trClass.Data()));
		continue;
	    }
	}
	else {
	    IlcDebug(1,"Cosmic trigger list is not provided, cosmic event specie is effectively disabled!");
	}

	otherTrigger = kTRUE;
    }
    delete trClassArray;

    if (calibTrigger) {
	fEventSpecie = kCalib;
	return;
    }
    if (cosmicTrigger && !otherTrigger) {
	fEventSpecie = kCosmic;
	return;
    }

    // Here we have to add if we have other cases
    // and also HLT info if any...
}

const IlcDetectorRecoParam *IlcRecoParam::GetDetRecoParam(Int_t iDet) const
{
  // Return IlcDetectorRecoParam object for a given detector
  // according to the event specie provided as an argument
  if ( iDet >= kNDetectors) return NULL;
  if (!fDetRecoParams[iDet]) return NULL;
  if (fDetRecoParams[iDet]->GetEntries() == 0) return NULL;

  for(Int_t iBit = 0; iBit < kNSpecies; iBit++) {
    if (fEventSpecie & (1 << iBit)) {
      if (fDetRecoParamsIndex[iBit][iDet] >= 0)
	return (IlcDetectorRecoParam *)fDetRecoParams[iDet]->At(fDetRecoParamsIndex[iBit][iDet]);
      else if (fDetRecoParamsIndex[0][iDet] >= 0)
	return (IlcDetectorRecoParam *)fDetRecoParams[iDet]->At(fDetRecoParamsIndex[0][iDet]);
      else {
	IlcError(Form("no RecoParam set for detector %d", iDet));
	return NULL;
      }
    }
  }

  // Default one
  IlcError(Form("Invalid event specie: %d!",fEventSpecie));
  if (fDetRecoParamsIndex[0][iDet] >= 0)
    return (IlcDetectorRecoParam *)fDetRecoParams[iDet]->At(fDetRecoParamsIndex[0][iDet]);

  IlcError(Form("no RecoParam set for detector %d", iDet));
  return NULL;
}

void  IlcRecoParam::AddDetRecoParam(Int_t iDet, IlcDetectorRecoParam* param)
{
  // Add an instance of reco params object into
  // the fDetRecoParams for detector iDet
  // Updates the fDetRecoParams index
  if (!fDetRecoParams[iDet]) fDetRecoParams[iDet] = new TObjArray;
  fDetRecoParams[iDet]->AddLast(param);
  Int_t index = fDetRecoParams[iDet]->GetLast();

  // Index
  Int_t specie = param->GetEventSpecie();
  for(Int_t iBit = 0; iBit < kNSpecies; iBit++) {
    if (specie & (1 << iBit)) {
      fDetRecoParamsIndex[iBit][iDet] = index;
    }
  }
}

Bool_t IlcRecoParam::AddDetRecoParamArray(Int_t iDet, TObjArray* parArray)
{
  // Add an array of reconstruction parameter objects
  // for a given detector
  // Basic check on the consistency of the array
  Bool_t defaultFound = kFALSE;
  if (!parArray) return defaultFound;
  for(Int_t i = 0; i < parArray->GetEntriesFast(); i++) {
    IlcDetectorRecoParam *par = (IlcDetectorRecoParam*)parArray->At(i);
    if (!par) continue;
    if (par->IsDefault()) defaultFound = kTRUE;

    Int_t specie = par->GetEventSpecie();
    for(Int_t iBit = 0; iBit < kNSpecies; iBit++) {
      if (specie & (1 << iBit)) {
	fDetRecoParamsIndex[iBit][iDet] = i;
      }
    }
 }
   
  fDetRecoParams[iDet] = parArray;

  return defaultFound;
}

const char*  IlcRecoParam::PrintEventSpecie() const
{
  // Print the current
  // event specie
  switch (fEventSpecie) {
  case kDefault:
    return fkgEventSpecieName[0].Data() ;
    break;
  case kLowMult:
    return fkgEventSpecieName[1].Data() ;
    break;
  case kHighMult:
    return fkgEventSpecieName[2].Data() ;
    break;
  case kCosmic:
    return fkgEventSpecieName[3].Data() ;
    break;
  case kCalib:
    return fkgEventSpecieName[4].Data() ;
    break;
  default:
    return fkgEventSpecieName[5].Data() ;
    break;
  }
}

const char * IlcRecoParam::GetEventSpecieName(EventSpecie_t es)
{
  switch (es) {
    case kDefault:
      return fkgEventSpecieName[0].Data() ;
      break;
    case kLowMult:
      return fkgEventSpecieName[1].Data() ;
      break;
    case kHighMult:
      return fkgEventSpecieName[2].Data() ;
      break;
    case kCosmic:
      return fkgEventSpecieName[3].Data() ;
      break;
    case kCalib:
      return fkgEventSpecieName[4].Data() ;
      break;
    default:
      return fkgEventSpecieName[5].Data() ;
      break;
  }
}

const char * IlcRecoParam::GetEventSpecieName(Int_t esIndex)
{
  if ( esIndex >= 0 && esIndex < kNSpecies) 
    return fkgEventSpecieName[esIndex].Data() ;
  else 
    return fkgEventSpecieName[kNSpecies].Data() ;
}

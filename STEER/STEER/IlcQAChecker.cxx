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

/* $Id: */

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// class for running the Quality Assurance Checker
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#include "IlcCDBEntry.h"
#include "IlcQAManager.h"
#include "IlcCDBStorage.h"
#include "IlcRunInfo.h" 
#include "IlcLog.h"
#include "IlcModule.h" 
#include "IlcQAv1.h"
#include "IlcQAChecker.h"
#include "IlcQACheckerBase.h"
#include "IlcCorrQAChecker.h"
#include "IlcGlobalQAChecker.h"
#include "IlcGRPObject.h"

#include <TKey.h>
#include <TObjArray.h>
#include <TObjString.h>
#include <TPluginManager.h> 
#include <TROOT.h>
#include <TStopwatch.h> 
#include <TString.h> 
#include <TSystem.h> 
#include <TList.h>
#include <TNtupleD.h>

ClassImp(IlcQAChecker)
  IlcQAChecker * IlcQAChecker::fgQAChecker = 0x0 ;

//_____________________________________________________________________________
IlcQAChecker::IlcQAChecker(const char* name, const char* title) :
  TNamed(name, title),
  fDataFile(0x0), 
  fRunInfo(0x0), 
  fRunInfoOwner(kFALSE), 
  fRefFile(0x0), 
  fFoundDetectors("."), 
  fEventSpecie(IlcRecoParam::kDefault), 
  fRun(0)
{
  // ctor: initialise checkers and open the data file   
  for (Int_t det = 0 ; det < IlcQAv1::kNDET ; det++) 
    fCheckers[det] = NULL ; 
}

//_____________________________________________________________________________
IlcQAChecker::IlcQAChecker(const IlcQAChecker& qac) :
  TNamed(qac),
  fDataFile(qac.fDataFile), 
  fRunInfo(qac.fRunInfo), 
  fRunInfoOwner(kFALSE),   
  fRefFile(qac.fRefFile), 
  fFoundDetectors(qac.fFoundDetectors),
  fEventSpecie(qac.fEventSpecie),
  fRun(qac.fRun)
{
  // copy constructor
  
  for (Int_t det = 0 ; det < IlcQAv1::kNDET ; det++) 
    fCheckers[det] = NULL ; 
}

//_____________________________________________________________________________
IlcQAChecker& IlcQAChecker::operator = (const IlcQAChecker& qac)
{
// assignment operator

  this->~IlcQAChecker();
  new(this) IlcQAChecker(qac);
  return *this;
}

//_____________________________________________________________________________
IlcQAChecker::~IlcQAChecker()
{
// clean up
  if (fRunInfo)
    delete fRunInfo ; 
  for (Int_t det=0; det<IlcQAv1::kNDET; det++)
    delete fCheckers[det] ; 
  IlcQAv1::Close() ; 
}

//_____________________________________________________________________________
  IlcQACheckerBase * IlcQAChecker::GetDetQAChecker(Int_t det)
{
	// Gets the Quality Assurance checker for the detector specified by its name

  if (det<0 || det>=IlcQAv1::kNDET) return NULL;

	if (fCheckers[det]) 
    return fCheckers[det];

	IlcQACheckerBase * qac = NULL ;

	TString detName(IlcQAv1::GetDetName(det)) ; 
	
	if (det == IlcQAv1::kGLOBAL) {
		qac = new IlcGlobalQAChecker() ; 
	} else if (det == IlcQAv1::kCORR) {
		qac = new IlcCorrQAChecker() ; 
	} else {
		IlcDebugClass(IlcQAv1::GetQADebugLevel(), Form("Retrieving QA checker for %s", detName.Data())) ; 
		TPluginManager* pluginManager = gROOT->GetPluginManager() ;
		TString qacName = "Ilc" + detName + "QAChecker" ;

		// first check if a plugin is defined for the quality assurance checker
		TPluginHandler* pluginHandler = pluginManager->FindHandler("IlcQAChecker", detName.Data());
		// if not, add a plugin for it
		if (!pluginHandler) {
			//IlcDebug(IlcQAv1::GetQADebugLevel(), Form("defining plugin for %s", qacName.Data()));
			TString libs = gSystem->GetLibraries();
		
			if (libs.Contains("lib" + detName + "base.so") || (gSystem->Load("lib" + detName + "base.so") >= 0))
				pluginManager->AddHandler("IlcQAChecker", detName, qacName, detName + "qac", qacName + "()");
			else 
				pluginManager->AddHandler("IlcQAChecker", detName, qacName, detName, qacName + "()");

			pluginHandler = pluginManager->FindHandler("IlcQAChecker", detName);	

			if (pluginHandler && (pluginHandler->LoadPlugin() == 0)) 
				qac = (IlcQACheckerBase *) pluginHandler->ExecPlugin(0);
  
		}
	}
	if (qac) 
		fCheckers[det] = qac ;
	
	return qac ; 
}

//_____________________________________________________________________________
IlcQAChecker * IlcQAChecker::Instance()
{
	// returns unique instance of the checker
  if ( ! fgQAChecker ) 
   fgQAChecker = new IlcQAChecker() ; 
  return fgQAChecker ;  
}

//_____________________________________________________________________________
void IlcQAChecker::LoadRunInfoFromGRP() 
{
  IlcCDBManager* man = IlcCDBManager::Instance() ;
  IlcCDBEntry* entry = man->Get(IlcQAv1::GetGRPPath().Data());
  IlcGRPObject* grpObject = 0x0;
  if (entry) {

	  TMap* m = static_cast<TMap*>(entry->GetObject());  // old GRP entry

	  if (m) {
	    IlcDebug(IlcQAv1::GetQADebugLevel(), "It is a map");
	    //m->Print();
	    grpObject = new IlcGRPObject();
	         grpObject->ReadValuesFromMap(m);
    }

    else {
	    IlcDebug(IlcQAv1::GetQADebugLevel(), "It is a new GRP object");
        grpObject = static_cast<IlcGRPObject*>(entry->GetObject());  // new GRP entry
    }

    entry->SetOwner(0);
    IlcCDBManager::Instance()->UnloadFromCache("GRP/GRP/Data");
  }

  if (!grpObject) {
     IlcFatal("No GRP entry found in OCDB!");
  }

  TString lhcState = grpObject->GetLHCState();
  if (lhcState==IlcGRPObject::GetInvalidString()) {
    IlcError("GRP/GRP/Data entry:  missing value for the LHC state ! Using UNKNOWN");
    lhcState = "UNKNOWN";
  }

  TString beamType = grpObject->GetBeamType();
  if (beamType==IlcGRPObject::GetInvalidString()) {
    IlcError("GRP/GRP/Data entry:  missing value for the beam type ! Using UNKNOWN");
    beamType = "UNKNOWN";
  }

  Float_t beamEnergy = grpObject->GetBeamEnergy();
  if (beamEnergy==IlcGRPObject::GetInvalidFloat()) {
    IlcError("GRP/GRP/Data entry:  missing value for the beam energy ! Using 0");
    beamEnergy = 0;
  }

  TString runType = grpObject->GetRunType();
  if (runType==IlcGRPObject::GetInvalidString()) {
    IlcError("GRP/GRP/Data entry:  missing value for the run type ! Using UNKNOWN");
    runType = "UNKNOWN";
  }

  Int_t activeDetectors = grpObject->GetDetectorMask();
  if (activeDetectors==IlcGRPObject::GetInvalidInt()) {
    IlcError("GRP/GRP/Data entry:  missing value for the detector mask ! Using 1074790399");
    activeDetectors = 1074790399;
  }

  fRunInfo = new IlcRunInfo(lhcState, beamType, beamEnergy, runType, activeDetectors);

  fRunInfoOwner = kTRUE ; 

  // set the event specie
  fEventSpecie = IlcRecoParam::kDefault ;
  if (strcmp(runType,"PHYSICS")) {
    // Not a physics run, the event specie is set to kCalib
    fEventSpecie = IlcRecoParam::kCalib ;
    return;
  }
  if (strcmp(lhcState,"STABLE_BEAMS") == 0) {
    // Heavy ion run (any beam tha is not pp, the event specie is set to kHighMult
    fEventSpecie = IlcRecoParam::kHighMult ;
    if ((strcmp(beamType,"p-p") == 0) ||
        (strcmp(beamType,"p-")  == 0) ||
        (strcmp(beamType,"-p")  == 0) ||
        (strcmp(beamType,"P-P") == 0) ||
        (strcmp(beamType,"P-")  == 0) ||
        (strcmp(beamType,"-P")  == 0)) {
      // Proton run, the event specie is set to kLowMult
      fEventSpecie = IlcRecoParam::kLowMult ;
    }
    else if (strcmp(beamType,"-") == 0) {
      // No beams, we assume cosmic data
      fEventSpecie = IlcRecoParam::kCosmic ;
    }
    else if (strcmp(beamType,"UNKNOWN") == 0) {
      // No LHC beam information is available, we use the default event specie
      fEventSpecie = IlcRecoParam::kDefault ;
    }
  }
}

//_____________________________________________________________________________
Bool_t IlcQAChecker::Run(const char * fileName, IlcDetectorRecoParam * recoParam)
{
  // run the Quality Assurance Checker for all tasks Hits, SDigits, Digits, DigitsR, RecPoints, TrackSegments, RecParticles and ESDs
  // starting from data in file  
  TStopwatch stopwatch;
  stopwatch.Start();
  
  //search for all detectors QA directories
  TList * detKeyList = IlcQAv1::GetQADataFile(fileName)->GetListOfKeys() ; 
  TIter nextd(detKeyList) ; 
  TKey * detKey ; 
  while ( (detKey = static_cast<TKey *>(nextd()) ) ) {
    IlcDebug(IlcQAv1::GetQADebugLevel(), Form("Found %s", detKey->GetName())) ;
    //Check which detector
    TString detName ; 
    TString detNameQA(detKey->GetName()) ; 
    Int_t det ; 
    for ( det = 0; det < IlcQAv1::kNDET ; det++) {
      detName = IlcQAv1::GetDetName(det) ; 
      if (detNameQA.Contains(detName)) {
        fFoundDetectors+=detName ; 
        fFoundDetectors+="." ;		
        break ; 
      }
    } 
    TDirectory * detDir = IlcQAv1::GetQADataFile(fileName)->GetDirectory(detKey->GetName()) ; 
    TList * taskKeyList = detDir->GetListOfKeys() ;
    TIter nextt(taskKeyList) ; 
    TKey * taskKey ; 
    // now search for the tasks dir
    while ( (taskKey = static_cast<TKey *>(nextt()) ) ) {
      TString taskName( taskKey->GetName() ) ; 
      IlcDebug(IlcQAv1::GetQADebugLevel(), Form("Found %s", taskName.Data())) ;
      TDirectory * taskDir = detDir->GetDirectory(taskName.Data()) ; 
      taskDir->cd() ; 
      IlcQACheckerBase * qac = GetDetQAChecker(det) ; 
      if (qac)
        IlcDebug(IlcQAv1::GetQADebugLevel(), Form("QA checker found for %s", detName.Data())) ; 
      if (!qac)
        IlcFatal(Form("QA checker not found for %s", detName.Data())) ; 
      IlcQAv1::ILCTASK_t index = IlcQAv1::kNULLTASK ; 
      if ( taskName == IlcQAv1::GetTaskName(IlcQAv1::kHITS) ) 
        index = IlcQAv1::kSIM ; 
      if ( taskName == IlcQAv1::GetTaskName(IlcQAv1::kSDIGITS) ) 
        index = IlcQAv1::kSIM ; 
      if ( taskName == IlcQAv1::GetTaskName(IlcQAv1::kDIGITS) ) 
        index = IlcQAv1::kSIM ; 
      if ( taskName == IlcQAv1::GetTaskName(IlcQAv1::kRAWS) ) 
        index = IlcQAv1::kRAW ;       
      if ( taskName == IlcQAv1::GetTaskName(IlcQAv1::kDIGITSR) ) 
        index = IlcQAv1::kREC ; 
      if ( taskName == IlcQAv1::GetTaskName(IlcQAv1::kRECPOINTS) ) 
        index = IlcQAv1::kREC ; 
      if ( taskName == IlcQAv1::GetTaskName(IlcQAv1::kTRACKSEGMENTS) ) 
        index = IlcQAv1::kREC ; 
      if ( taskName == IlcQAv1::GetTaskName(IlcQAv1::kRECPARTICLES) ) 
        index = IlcQAv1::kREC ; 
      if ( taskName == IlcQAv1::GetTaskName(IlcQAv1::kESDS) ) 
        index = IlcQAv1::kESD ; 
      qac->Init(IlcQAv1::DETECTORINDEX_t(det)) ; 
      qac->Run(index, recoParam) ; 
    }
  }
  TString detList ; 
  for ( Int_t det = 0; det < IlcQAv1::kNDET; det++) {
    if (fFoundDetectors.Contains(IlcQAv1::GetDetName(det))) {
      detList += IlcQAv1::GetDetName(det) ; 
      detList += " " ; 
      fFoundDetectors.ReplaceAll(IlcQAv1::GetDetName(det), "") ; 
      IlcQAv1::Instance()->Show(IlcQAv1::GetDetIndex(IlcQAv1::GetDetName(det))) ; 
    }	
  }
  IlcInfo(Form("QA performed for following detectors: %s", detList.Data())) ; 
  return kTRUE ; 
}

//_____________________________________________________________________________
Bool_t IlcQAChecker::Run(IlcQAv1::DETECTORINDEX_t det, IlcQAv1::TASKINDEX_t task, TObjArray ** list, IlcDetectorRecoParam * recoParam)
{
	// run the Quality Assurance Checker for detector det, for task task starting from data in list

  if (det >= IlcQAv1::kNDET) {
    IlcError(Form("det = %i is larger than IlcQAv1::kNDET ... should never happen", det)); 
    return kFALSE ; 
  }
  IlcQACheckerBase * qac = GetDetQAChecker(det) ; 
  if (qac)
    IlcDebug(IlcQAv1::GetQADebugLevel(), Form("QA checker found for %s", IlcQAv1::GetDetName(det).Data())) ;
  if (!qac) {
    IlcError(Form("QA checker not found for %s", IlcQAv1::GetDetName(det).Data())) ; 
    return kFALSE;
  }
  
  IlcQAv1::ILCTASK_t index = IlcQAv1::kNULLTASK ; 
  if ( task == IlcQAv1::kRAWS ) 
    index = IlcQAv1::kRAW ; 
  else if ( task == IlcQAv1::kHITS ) 
    index = IlcQAv1::kSIM ; 
  else if ( task == IlcQAv1::kSDIGITS ) 
    index = IlcQAv1::kSIM ; 
  else if ( task == IlcQAv1::kDIGITS ) 
    index = IlcQAv1::kSIM ; 
  else if ( task == IlcQAv1::kDIGITSR ) 
    index = IlcQAv1::kREC ; 
  else if ( task == IlcQAv1::kRECPOINTS ) 
    index = IlcQAv1::kREC ; 
  else if ( task == IlcQAv1::kTRACKSEGMENTS ) 
    index = IlcQAv1::kREC ; 
  else if ( task == IlcQAv1::kRECPARTICLES ) 
    index = IlcQAv1::kREC ; 
  else if ( task == IlcQAv1::kESDS ) 
    index = IlcQAv1::kESD ; 
  
  qac->Init(det) ; 
  qac->Run(index, list, recoParam) ; 
  
  // make the image 
  qac->MakeImage(list, task, IlcQAv1::Mode(task)) ; 
  
	return kTRUE ; 
}

//_____________________________________________________________________________
Bool_t IlcQAChecker::Run(IlcQAv1::DETECTORINDEX_t det, IlcQAv1::TASKINDEX_t task, TNtupleD ** list, IlcDetectorRecoParam * recoParam)
{
  // run the Quality Assurance Checker for detector det, for task task starting from data in list
  
  IlcQACheckerBase * qac = GetDetQAChecker(det) ; 
  if (qac)
    IlcDebug(IlcQAv1::GetQADebugLevel(), Form("QA checker found for %s", IlcQAv1::GetDetName(det).Data())) ;
  if (!qac) {
    IlcError(Form("QA checker not found for %s", IlcQAv1::GetDetName(det).Data())) ; 
    return kFALSE;
  }
  
  IlcQAv1::ILCTASK_t index = IlcQAv1::kNULLTASK ; 
  if ( task == IlcQAv1::kRAWS ) 
    index = IlcQAv1::kRAW ; 
  else if ( task == IlcQAv1::kHITS ) 
    index = IlcQAv1::kSIM ; 
  else if ( task == IlcQAv1::kSDIGITS ) 
    index = IlcQAv1::kSIM ; 
  else if ( task == IlcQAv1::kDIGITS ) 
    index = IlcQAv1::kSIM ; 
  else if ( task == IlcQAv1::kDIGITSR ) 
    index = IlcQAv1::kREC ; 
  else if ( task == IlcQAv1::kRECPOINTS ) 
    index = IlcQAv1::kREC ; 
  else if ( task == IlcQAv1::kTRACKSEGMENTS ) 
    index = IlcQAv1::kREC ; 
  else if ( task == IlcQAv1::kRECPARTICLES ) 
    index = IlcQAv1::kREC ; 
  else if ( task == IlcQAv1::kESDS ) 
    index = IlcQAv1::kESD ; 
  
  qac->Init(det) ; 
  qac->Run(index, list, recoParam) ; 

  return kTRUE ; 
}

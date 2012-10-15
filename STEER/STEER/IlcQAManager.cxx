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

/* $Id: IlcQAManager.cxx 30894 2009-02-05 13:46:48Z schutz $ */
///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// class for running the QA makers                                           //
//                                                                           //
//   IlcQAManager qas;                                                       //
//   qas.Run(IlcQAv1::kRAWS, rawROOTFileName);                               //
//   qas.Run(IlcQAv1::kHITS);                                                //
//   qas.Run(IlcQAv1::kSDIGITS);                                             //
//   qas.Run(IlcQAv1::kDIGITS);                                              //
//   qas.Run(IlcQAv1::kRECPOINTS);                                           //
//   qas.Run(IlcQAv1::kESDS);                                                //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#include <TCanvas.h>
#include <TKey.h>
#include <TFile.h>
#include <TFileMerger.h>
#include <TGrid.h>
#include <TGridCollection.h>
#include <TGridResult.h>
#include <TPluginManager.h>
#include <TROOT.h>
#include <TString.h>
#include <TSystem.h>
#include <TStopwatch.h>

#include "IlcCDBManager.h"
#include "IlcCDBEntry.h"
#include "IlcCDBId.h"
#include "IlcCDBMetaData.h"
#include "IlcCodeTimer.h"
#include "IlcCorrQADataMakerRec.h"
#include "IlcDetectorRecoParam.h"
#include "IlcESDEvent.h"
#include "IlcGeomManager.h"
#include "IlcGlobalQADataMaker.h"
#include "IlcHeader.h"
#include "IlcLog.h"
#include "IlcModule.h"
#include "IlcQAv1.h"
#include "IlcQAChecker.h"
#include "IlcQACheckerBase.h"
#include "IlcQADataMakerRec.h"
#include "IlcQADataMakerSim.h"
#include "IlcQAManager.h" 
#include "IlcRawReaderDate.h"
#include "IlcRawReaderFile.h"
#include "IlcRawReaderRoot.h"
#include "IlcRun.h"
#include "IlcRunLoader.h"
#include "IlcRunTag.h"

ClassImp(IlcQAManager) 
IlcQAManager* IlcQAManager::fgQAInstance = 0x0;

//_____________________________________________________________________________
IlcQAManager::IlcQAManager() :
  IlcCDBManager(), 
  fCurrentEvent(0),   
  fCycleSame(kFALSE),
  fDetectors("ALL"), 
  fDetectorsW("ALL"), 
  fESD(NULL), 
  fESDTree(NULL),
  fEventInfo(NULL), 
  fGIlcFileName(""), 
  fFirstEvent(0),        
  fMaxEvents(0),   
  fMode(""), 
  fNumberOfEvents(999999), 
  fRecoParam(),
  fRunNumber(0), 
  fRawReader(NULL), 
  fRawReaderDelete(kTRUE), 
  fRunLoader(NULL), 
  fTasks(""),  
  fEventSpecie(IlcRecoParam::kDefault), 
  fPrintImage(kTRUE), 
  fSaveData(kTRUE) 
{
  // default ctor
  fMaxEvents = fNumberOfEvents ; 
  for (UInt_t iDet = 0; iDet < fgkNDetectors; iDet++) {
    if (IsSelected(IlcQAv1::GetDetName(iDet))) {
      fLoader[iDet]      = NULL ;
      fQADataMaker[iDet] = NULL ;
      fQACycles[iDet]    = 999999 ;
    }
  }	
  SetWriteExpert() ; 
}

//_____________________________________________________________________________
IlcQAManager::IlcQAManager(IlcQAv1::MODE_t mode, const Char_t* gIlcFilename) :
  IlcCDBManager(), 
  fCurrentEvent(0),  
  fCycleSame(kFALSE),
  fDetectors("ALL"), 
  fDetectorsW("ALL"), 
  fESD(NULL), 
  fESDTree(NULL),
  fEventInfo(NULL),  
  fGIlcFileName(gIlcFilename), 
  fFirstEvent(0),        
  fMaxEvents(0),   
  fMode(IlcQAv1::GetModeName(mode)), 
  fNumberOfEvents(999999), 
  fRecoParam(),
  fRunNumber(0), 
  fRawReader(NULL), 
  fRawReaderDelete(kTRUE), 
  fRunLoader(NULL), 
  fTasks(""), 
  fEventSpecie(IlcRecoParam::kDefault), 
  fPrintImage(kTRUE), 
  fSaveData(kTRUE) 
{
  // default ctor
  fMaxEvents = fNumberOfEvents ; 
  for (UInt_t iDet = 0; iDet < fgkNDetectors; iDet++) {
    if (IsSelected(IlcQAv1::GetDetName(iDet))) {
      fLoader[iDet]      = NULL ;
      fQADataMaker[iDet] = NULL ;
      fQACycles[iDet]    = 999999 ;
    }
  }
  SetWriteExpert() ; 
}

//_____________________________________________________________________________
IlcQAManager::IlcQAManager(const IlcQAManager & qas) : 
  IlcCDBManager(), 
  fCurrentEvent(qas.fCurrentEvent),  
  fCycleSame(kFALSE),
  fDetectors(qas.fDetectors), 
  fDetectorsW(qas.fDetectorsW), 
  fESD(NULL), 
  fESDTree(NULL), 
  fEventInfo(NULL), 
  fGIlcFileName(qas.fGIlcFileName), 
  fFirstEvent(qas.fFirstEvent),        
  fMaxEvents(qas.fMaxEvents),    
  fMode(qas.fMode), 
  fNumberOfEvents(qas.fNumberOfEvents), 
  fRecoParam(),		
  fRunNumber(qas.fRunNumber), 
  fRawReader(NULL), 
  fRawReaderDelete(kTRUE), 
  fRunLoader(NULL), 
  fTasks(qas.fTasks), 
  fEventSpecie(qas.fEventSpecie), 
  fPrintImage(qas.fPrintImage), 
  fSaveData(qas.fSaveData) 

{
  // cpy ctor
  for (UInt_t iDet = 0; iDet < fgkNDetectors; iDet++) {
    fLoader[iDet]         = qas.fLoader[iDet] ;
    fQADataMaker[iDet]    = qas.fQADataMaker[iDet] ;
    fQACycles[iDet]       = qas.fQACycles[iDet] ;	
    fQAWriteExpert[iDet] = qas.fQAWriteExpert[iDet] ;
  }
}

//_____________________________________________________________________________
IlcQAManager & IlcQAManager::operator = (const IlcQAManager & qas) 
{
  // assignment operator
  this->~IlcQAManager() ;
  new(this) IlcQAManager(qas) ;
  return *this ;
}

//_____________________________________________________________________________
IlcQAManager::~IlcQAManager() 
{
  // dtor
  for (UInt_t iDet = 0; iDet < fgkNDetectors; iDet++) {
    if (IsSelected(IlcQAv1::GetDetName(iDet))) {
      fLoader[iDet] = NULL;
      if (fQADataMaker[iDet]) {
	(fQADataMaker[iDet])->Finish() ; 
	delete fQADataMaker[iDet] ;
      }
    }
  }
  if (fRawReaderDelete) { 
    fRunLoader = NULL ;
    delete fRawReader ;
    fRawReader = NULL ;
  }
}
//_____________________________________________________________________________
Bool_t IlcQAManager::DoIt(const IlcQAv1::TASKINDEX_t taskIndex)
{
    // Runs all the QA data Maker for every detector
  
  Bool_t rv = kFALSE ;
    // Fill QA data in event loop 
  for (UInt_t iEvent = fFirstEvent ; iEvent < (UInt_t)fMaxEvents ; iEvent++) {
    fCurrentEvent++ ; 
      // Get the event
    if ( iEvent%10 == 0  ) 
      IlcDebug(IlcQAv1::GetQADebugLevel(), Form("processing event %d", iEvent));
    if ( taskIndex == IlcQAv1::kRAWS ) {
      if ( !fRawReader->NextEvent() )
        break ;
    } else if ( taskIndex == IlcQAv1::kESDS ) {
      if ( fESDTree->GetEntry(iEvent) == 0 )
        break ;
    } else {
      if ( fRunLoader->GetEvent(iEvent) != 0 )
        break ;
    }
      // loop  over active loaders
    TString detList ; 
    if ( GetEventInfo()) 
      detList = GetEventInfo()->GetTriggerCluster() ; 
    for (UInt_t iDet = 0; iDet < fgkNDetectors ; iDet++) {
      if (!detList.IsNull() && !detList.Contains(IlcQAv1::GetDetName(iDet)))
        continue ;
      if (IsSelected(IlcQAv1::GetDetName(iDet)) ){
        IlcQADataMaker * qadm = GetQADataMaker(iDet) ;
        if (!qadm) continue; // This detector doesn't have any QA (for example, HLT)
        if ( qadm->IsCycleDone() ) {
          qadm->EndOfCycle(taskIndex) ;
        }
        TTree * data = NULL ; 
        IlcLoader* loader = GetLoader(qadm->GetUniqueID());
        switch (taskIndex) {
          case IlcQAv1::kNULLTASKINDEX : 
            break ; 
          case IlcQAv1::kRAWS :
            qadm->Exec(taskIndex, fRawReader) ; 
            break ; 
          case IlcQAv1::kHITS :
            if( loader ) {
              loader->LoadHits() ; 
              data = loader->TreeH() ; 
              if ( ! data ) {
                IlcWarning(Form(" Hit Tree not found for  %s", IlcQAv1::GetDetName(iDet))) ; 
                break ; 
              } 
              qadm->Exec(taskIndex, data) ;
            } 
            break ;
          case IlcQAv1::kSDIGITS :
          {
            
            TString fileName(Form("%s.SDigits.root", IlcQAv1::GetDetName(iDet))) ; 
            if (gSystem->FindFile("./", fileName)) {
              if( loader ) {      
                loader->LoadSDigits() ; 
                data = loader->TreeS() ; 
                if ( ! data ) {
                  IlcWarning(Form(" SDigit Tree not found for  %s", IlcQAv1::GetDetName(iDet))) ; 
                  break ; 
                } 
                qadm->Exec(taskIndex, data) ; 
              }
            }
          }
            break; 
          case IlcQAv1::kDIGITS :
            if( loader ) {      
              loader->LoadDigits() ; 
              data = loader->TreeD() ; 
              if ( ! data ) {
                IlcWarning(Form(" Digit Tree not found for  %s", IlcQAv1::GetDetName(iDet))) ; 
                break ; 
              } 
              qadm->Exec(taskIndex, data) ;
            }
            break;
          case IlcQAv1::kDIGITSR :
            if( loader ) {      
              loader->LoadDigits() ; 
              data = loader->TreeD() ; 
              if ( ! data ) {
                IlcWarning(Form(" Digit Tree not found for  %s", IlcQAv1::GetDetName(iDet))) ; 
                break ; 
              } 
              qadm->Exec(taskIndex, data) ;
            }
            break; 
          case IlcQAv1::kRECPOINTS :
            if( loader ) {      
              loader->LoadRecPoints() ; 
              data = loader->TreeR() ; 
              if (!data) {
                IlcWarning(Form("RecPoints not found for %s", IlcQAv1::GetDetName(iDet))) ; 
                break ; 
              } 
              qadm->Exec(taskIndex, data) ; 
            }
            break; 
          case IlcQAv1::kTRACKSEGMENTS :
            break; 
          case IlcQAv1::kRECPARTICLES :
            break; 
          case IlcQAv1::kESDS :
            qadm->Exec(taskIndex, fESD) ;
            break; 
          case IlcQAv1::kNTASKINDEX :
            break; 
        } //task switch
      }
    } // detector loop
    Increment(taskIndex) ; 
  } // event loop	
    // Save QA data for all detectors
  
  EndOfCycle() ; 
  
  if ( taskIndex == IlcQAv1::kRAWS ) 
    fRawReader->RewindEvents() ;
  
  return rv ; 
}

//_____________________________________________________________________________
Bool_t IlcQAManager::Finish(const IlcQAv1::TASKINDEX_t taskIndex) 
{
  // write output to file for all detectors
  
  IlcQAChecker::Instance()->SetRunNumber(fRunNumber) ; 

  for (UInt_t iDet = 0; iDet < fgkNDetectors ; iDet++) {
    if (IsSelected(IlcQAv1::GetDetName(iDet))) {
      IlcQADataMaker * qadm = GetQADataMaker(iDet) ;
      if (qadm) 
        qadm->EndOfCycle(taskIndex) ;
    }
  }
  return kTRUE ; 
}

//_____________________________________________________________________________
TObjArray * IlcQAManager::GetFromOCDB(IlcQAv1::DETECTORINDEX_t det, IlcQAv1::TASKINDEX_t task, const Char_t * year) const 
{
  // Retrieve the list of QA data for a given detector and a given task 
  TObjArray * rv = NULL ;
  if ( !strlen(IlcQAv1::GetQARefStorage()) ) { 
    IlcError("No storage defined, use IlcQAv1::SetQARefStorage") ; 
    return NULL ; 
  }	
  if ( ! IsDefaultStorageSet() ) {
    TString tmp(IlcQAv1::GetQARefDefaultStorage()) ; 
    tmp.Append(year) ; 
    tmp.Append("/") ; 
    Instance()->SetDefaultStorage(tmp.Data()) ; 		
    Instance()->SetSpecificStorage(Form("%s/*", IlcQAv1::GetQAName()), IlcQAv1::GetQARefStorage()) ;
  }
  TString detOCDBDir(Form("%s/%s/%s", IlcQAv1::GetQAName(), IlcQAv1::GetDetName((Int_t)det), IlcQAv1::GetRefOCDBDirName())) ; 
  IlcDebug(IlcQAv1::GetQADebugLevel(), Form("Retrieving reference data from %s/%s for %s", IlcQAv1::GetQARefStorage(), detOCDBDir.Data(), IlcQAv1::GetTaskName(task).Data())) ; 
  IlcCDBEntry* entry = QAManager()->Get(detOCDBDir.Data(), 0) ; //FIXME 0 --> Run Number
  TList * listDetQAD = static_cast<TList *>(entry->GetObject()) ;
  if ( listDetQAD ) 
    rv = static_cast<TObjArray *>(listDetQAD->FindObject(IlcQAv1::GetTaskName(task))) ; 
  return rv ; 
}

//_____________________________________________________________________________
TCanvas ** IlcQAManager::GetImage(Char_t * detName)
{
  // retrieves QA Image for the given detector
  TCanvas ** rv = NULL ; 
  Int_t detIndex = IlcQAv1::GetDetIndex(detName) ; 
  if ( detIndex != IlcQAv1::kNULLDET) {
    IlcQACheckerBase * qac = IlcQAChecker::Instance()->GetDetQAChecker(detIndex) ; 
    rv = qac->GetImage() ;
  }
  return rv ; 
}

//_____________________________________________________________________________
IlcLoader * IlcQAManager::GetLoader(Int_t iDet)
{
  // get the loader for a detector

  if ( !fRunLoader || iDet == IlcQAv1::kCORR || iDet == IlcQAv1::kGLOBAL ) 
    return NULL ; 
	
  TString detName = IlcQAv1::GetDetName(iDet) ;
  fLoader[iDet] = fRunLoader->GetLoader(detName + "Loader");
  if (fLoader[iDet]) 
    return fLoader[iDet] ;
	
  // load the QA data maker object
  TPluginManager* pluginManager = gROOT->GetPluginManager() ;
  TString loaderName = "Ilc" + detName + "Loader" ;

  IlcLoader * loader = NULL ;
  // first check if a plugin is defined for the quality assurance data maker
  TPluginHandler* pluginHandler = pluginManager->FindHandler("IlcLoader", detName) ;
  // if not, add a plugin for it
  if (!pluginHandler) {
    IlcDebug(IlcQAv1::GetQADebugLevel(), Form("defining plugin for %s", loaderName.Data())) ;
    TString libs = gSystem->GetLibraries() ;
    if (libs.Contains("lib" + detName + "base.so") || (gSystem->Load("lib" + detName + "base.so") >= 0)) {
      pluginManager->AddHandler("IlcQADataMaker", detName, loaderName, detName + "loader", loaderName + "()") ;
    } else {
      pluginManager->AddHandler("IlcLoader", detName, loaderName, detName, loaderName + "()") ;
    }
    pluginHandler = pluginManager->FindHandler("IlcLoader", detName) ;
  }
  if (pluginHandler && (pluginHandler->LoadPlugin() == 0)) {
    loader = (IlcLoader *) pluginHandler->ExecPlugin(0) ;
  }
  if (loader) 
    fLoader[iDet] = loader ;
  return loader ;
}

//_____________________________________________________________________________
IlcQAv1 * IlcQAManager::GetQA(UInt_t run, UInt_t evt) 
{
  // retrieves the QA object stored in a file named "Run{run}.Event{evt}_1.ESD.tag.root"  
  Char_t * fileName = Form("Run%d.Event%d_1.ESD.tag.root", run, evt) ; 
  TFile * tagFile = TFile::Open(fileName) ;
  if ( !tagFile ) {
    IlcError(Form("File %s not found", fileName)) ;
    return NULL ; 
  }
  TTree * tagTree = static_cast<TTree *>(tagFile->Get("T")) ; 
  if ( !tagTree ) {
    IlcError(Form("Tree T not found in %s", fileName)) ; 
    tagFile->Close() ; 
    return NULL ; 
  }
  IlcRunTag * tag = new IlcRunTag ; 
  tagTree->SetBranchAddress("IlcTAG", &tag) ; 
  tagTree->GetEntry(evt) ; 
  IlcQAv1 * qa = IlcQAv1::Instance(tag->GetQALength(), tag->GetQAArray(), tag->GetESLength(), tag->GetEventSpecies()) ; 
  tagFile->Close() ; 
  return qa ; 
}

//_____________________________________________________________________________
IlcQADataMaker * IlcQAManager::GetQADataMaker(const Int_t iDet) 
{
  // get the quality assurance data maker for a detector
	
  IlcQADataMaker * qadm =  fQADataMaker[iDet] ; 
  
  if (qadm) {
 
    qadm->SetEventSpecie(fEventSpecie) ;  
    if ( qadm->GetRecoParam() ) 
      if ( IlcRecoParam::Convert(qadm->GetRecoParam()->GetEventSpecie()) != IlcRecoParam::kDefault)
        qadm->SetEventSpecie(qadm->GetRecoParam()->GetEventSpecie()) ; 

  } else if (iDet == IlcQAv1::kGLOBAL && strcmp(GetMode(), IlcQAv1::GetModeName(IlcQAv1::kRECMODE)) == 0) { //Global QA

    qadm = new IlcGlobalQADataMaker();
    qadm->SetName(IlcQAv1::GetDetName(iDet));
    qadm->SetUniqueID(iDet);
    fQADataMaker[iDet] = qadm;
    qadm->SetEventSpecie(fEventSpecie) ;  
    if ( qadm->GetRecoParam() ) 
      if ( IlcRecoParam::Convert(qadm->GetRecoParam()->GetEventSpecie()) != IlcRecoParam::kDefault)  
        qadm->SetEventSpecie(qadm->GetRecoParam()->GetEventSpecie()) ; 

  }	else if (iDet == IlcQAv1::kCORR && strcmp(GetMode(), IlcQAv1::GetModeName(IlcQAv1::kRECMODE)) == 0 ) { //the data maker for correlations among detectors
    qadm = new IlcCorrQADataMakerRec(fQADataMaker) ; 
    qadm->SetName(IlcQAv1::GetDetName(iDet));
    qadm->SetUniqueID(iDet);
    fQADataMaker[iDet] = qadm;
    qadm->SetEventSpecie(fEventSpecie) ;  
    if ( qadm->GetRecoParam() ) 
      if ( IlcRecoParam::Convert(qadm->GetRecoParam()->GetEventSpecie()) != IlcRecoParam::kDefault)  
        qadm->SetEventSpecie(qadm->GetRecoParam()->GetEventSpecie()) ; 

  } else if ( iDet < IlcQAv1::kGLOBAL ) {
    TString  smode(GetMode()) ; 
    if (smode.Contains(IlcQAv1::GetModeName(IlcQAv1::kQAMODE)))
      smode = IlcQAv1::GetModeName(IlcQAv1::kRECMODE) ; 
    // load the QA data maker object
    TPluginManager* pluginManager = gROOT->GetPluginManager() ;
    TString detName = IlcQAv1::GetDetName(iDet) ;
    TString qadmName = "Ilc" + detName + "QADataMaker" + smode ;
    
    // first check if a plugin is defined for the quality assurance data maker
    TPluginHandler* pluginHandler = pluginManager->FindHandler("IlcQADataMaker", detName) ;
    // if not, add a plugin for it
    if (!pluginHandler) {
      IlcDebug(IlcQAv1::GetQADebugLevel(), Form("defining plugin for %s", qadmName.Data())) ;
      TString libs = gSystem->GetLibraries() ;
      TString temp(smode) ;
      temp.ToLower() ; 
      if (libs.Contains("lib" + detName + smode + ".so") || (gSystem->Load("lib" + detName + temp.Data() + ".so") >= 0)) {
        pluginManager->AddHandler("IlcQADataMaker", detName, qadmName, detName + "qadm", qadmName + "()") ;
      } else {
        pluginManager->AddHandler("IlcQADataMaker", detName, qadmName, detName, qadmName + "()") ;
      }
      pluginHandler = pluginManager->FindHandler("IlcQADataMaker", detName) ;
    }
    if (pluginHandler && (pluginHandler->LoadPlugin() == 0)) {
      qadm = (IlcQADataMaker *) pluginHandler->ExecPlugin(0) ;
    }
    if (qadm) {
      qadm->SetName(IlcQAv1::GetDetName(iDet));
      qadm->SetUniqueID(iDet);
      fQADataMaker[iDet] = qadm ;
      qadm->SetEventSpecie(fEventSpecie) ;  
      if ( qadm->GetRecoParam() ) 
        if ( IlcRecoParam::Convert(qadm->GetRecoParam()->GetEventSpecie()) != IlcRecoParam::kDefault)  
          qadm->SetEventSpecie(qadm->GetRecoParam()->GetEventSpecie()) ; 
    }
  }
  return qadm ;
}

//_____________________________________________________________________________
void  IlcQAManager::EndOfCycle(TObjArray * detArray) 
{
  // End of cycle QADataMakers 
	
  IlcQAChecker::Instance()->SetRunNumber(fRunNumber) ; 
  TCanvas fakeCanvas ; 

  fakeCanvas.Print(Form("%s%s%d.%s[", IlcQAv1::GetImageFileName(), GetMode(), fRunNumber, IlcQAv1::GetImageFileFormat()), "ps") ; 
  for (UInt_t iDet = 0; iDet < fgkNDetectors ; iDet++) {
    if (IsSelected(IlcQAv1::GetDetName(iDet))) {
      IlcQADataMaker * qadm = GetQADataMaker(iDet) ;
      if (!qadm) 
	continue ;	
      // skip non active detectors
      if (detArray) {
	IlcModule* det = static_cast<IlcModule*>(detArray->FindObject(IlcQAv1::GetDetName(iDet))) ;
	if (!det || !det->IsActive())  
	  continue ;
      }
      IlcQACheckerBase * qac = IlcQAChecker::Instance()->GetDetQAChecker(iDet) ;
      if (qac) 
        qac->SetPrintImage(fPrintImage) ;
      for (UInt_t taskIndex = 0; taskIndex < IlcQAv1::kNTASKINDEX; taskIndex++) {
        if ( fTasks.Contains(Form("%d", taskIndex)) ) 
          qadm->EndOfCycle(IlcQAv1::GetTaskIndex(IlcQAv1::GetTaskName(taskIndex))) ;
      }
      qadm->Finish();
    }
  }
  if (fPrintImage) 
    fakeCanvas.Print(Form("%s%s%d.%s]", IlcQAv1::GetImageFileName(), GetMode(), fRunNumber, IlcQAv1::GetImageFileFormat()), "ps"); 
}

//_____________________________________________________________________________
void  IlcQAManager::EndOfCycle(TString detectors) 
{
  // End of cycle QADataMakers 
  
  IlcQAChecker::Instance()->SetRunNumber(fRunNumber) ; 
  TCanvas fakeCanvas ; 
  if (fPrintImage) 
    fakeCanvas.Print(Form("%s%s%d.%s[", IlcQAv1::GetImageFileName(), GetMode(), fRunNumber, IlcQAv1::GetImageFileFormat()), "ps") ; 
  for (UInt_t iDet = 0; iDet < fgkNDetectors ; iDet++) {
    if (IsSelected(IlcQAv1::GetDetName(iDet))) {
      IlcQADataMaker * qadm = GetQADataMaker(iDet) ;
      if (!qadm) 
	continue ;	
      // skip non active detectors
      if (!detectors.Contains(IlcQAv1::GetDetName(iDet))) 
        continue ;
      IlcQACheckerBase * qac = IlcQAChecker::Instance()->GetDetQAChecker(iDet) ;
      if (qac) 
        qac->SetPrintImage(fPrintImage) ;
      for (UInt_t taskIndex = 0; taskIndex < IlcQAv1::kNTASKINDEX; taskIndex++) {
        if ( fTasks.Contains(Form("%d", taskIndex)) ) 
          qadm->EndOfCycle(IlcQAv1::GetTaskIndex(IlcQAv1::GetTaskName(taskIndex))) ;
      }
      qadm->Finish();
    }
  }
  if (fPrintImage) 
    fakeCanvas.Print(Form("%s%s%d.%s]", IlcQAv1::GetImageFileName(), GetMode(), fRunNumber, IlcQAv1::GetImageFileFormat()), "ps"); 
}

//_____________________________________________________________________________
IlcRecoParam::EventSpecie_t IlcQAManager::GetEventSpecieFromESD() 
{
  IlcRecoParam::EventSpecie_t runtype = IlcRecoParam::kDefault ; 
  if (!gSystem->AccessPathName("IlcESDs.root")) { // IlcESDs.root exists
    TFile * esdFile = TFile::Open("IlcESDs.root") ;
    TTree * esdTree = static_cast<TTree *> (esdFile->Get("esdTree")) ; 
    if ( !esdTree ) {
      IlcError("esdTree not found") ; 
    } else {
      IlcESDEvent * esd    = new IlcESDEvent() ;
      esd->ReadFromTree(esdTree) ;
      esdTree->GetEntry(0) ; 
      runtype = IlcRecoParam::Convert(esd->GetEventType()) ; 
    }
  } else {
    IlcError("IlcESDs.root not found") ; 
  }
  return runtype ;
}

//_____________________________________________________________________________
void IlcQAManager::Increment(const IlcQAv1::TASKINDEX_t taskIndex)
{
  // Increments the cycle counter for all QA Data Makers
  static IlcQAv1::TASKINDEX_t currentTask = IlcQAv1::kNTASKINDEX ; 
  if ( (currentTask == taskIndex) && taskIndex != IlcQAv1::kNULLTASKINDEX )
    return ; 
  else 
    currentTask = taskIndex ; 
  for (UInt_t iDet = 0; iDet < fgkNDetectors ; iDet++) {
    if (IsSelected(IlcQAv1::GetDetName(iDet))) {
      IlcQADataMaker * qadm = GetQADataMaker(iDet) ;
      if (qadm) 
        qadm->Increment() ;
    }
  }
}
  
//_____________________________________________________________________________
Bool_t IlcQAManager::InitQA(const IlcQAv1::TASKINDEX_t taskIndex, const  Char_t * input )
{
  // Initialize the event source and QA data makers
	
  fTasks += Form("%d", taskIndex) ; 

  if (taskIndex == IlcQAv1::kRAWS) { 
    if (!fRawReader) {
      fRawReader = IlcRawReader::Create(input);
    }
    if ( ! fRawReader ) 
      return kFALSE ; 
    fRawReaderDelete = kTRUE ; 
    fRawReader->NextEvent() ; 
    fRunNumber = fRawReader->GetRunNumber() ; 
    SetRun(fRunNumber) ; 
    fRawReader->RewindEvents();
    fNumberOfEvents = 999999 ;
    if ( fMaxEvents < 0 ) 
      fMaxEvents = fNumberOfEvents ; 
  } else if (taskIndex == IlcQAv1::kESDS) {
    fTasks = IlcQAv1::GetTaskName(IlcQAv1::kESDS) ; 
    if (!gSystem->AccessPathName("IlcESDs.root")) { // IlcESDs.root exists
      TFile * esdFile = TFile::Open("IlcESDs.root") ;
      fESDTree = static_cast<TTree *> (esdFile->Get("esdTree")) ; 
      if ( !fESDTree ) {
	IlcError("esdTree not found") ; 
	return kFALSE ; 
      } else {
	fESD     = new IlcESDEvent() ;
	fESD->ReadFromTree(fESDTree) ;
	fESDTree->GetEntry(0) ; 
	fRunNumber = fESD->GetRunNumber() ; 
	fNumberOfEvents = fESDTree->GetEntries() ;
	if ( fMaxEvents < 0 ) 
	  fMaxEvents = fNumberOfEvents ; 
      }
    } else {
      IlcError("IlcESDs.root not found") ; 
      return kFALSE ; 
    }			
  } else {
    if ( !InitRunLoader() ) { 
      IlcWarning("No Run Loader not found") ; 
    } else {
      fNumberOfEvents = fRunLoader->GetNumberOfEvents() ;
      if ( fMaxEvents < 0 ) 
	fMaxEvents = fNumberOfEvents ; 
    }
  }

  // Get Detectors 
  TObjArray* detArray = NULL ; 
  if (fRunLoader) // check if RunLoader exists 
    if ( fRunLoader->GetIlcRun() ) { // check if IlcRun exists in gIlc.root
      detArray = fRunLoader->GetIlcRun()->Detectors() ;
      fRunNumber = fRunLoader->GetHeader()->GetRun() ; 
    }

  // Initialize all QA data makers for all detectors
  fRunNumber = IlcCDBManager::Instance()->GetRun() ; 
  if ( !  IlcGeomManager::GetGeometry() ) 
    IlcGeomManager::LoadGeometry() ; 
	
  InitQADataMaker(fRunNumber, detArray) ; //, fCycleSame, kTRUE, detArray) ; 
  if (fPrintImage) {
    TCanvas fakeCanvas ; 
    TStopwatch timer ; 
    timer.Start() ; 
    while (timer.CpuTime()<5) {
      timer.Continue();
      gSystem->ProcessEvents();
    }
    fakeCanvas.Print(Form("%s%s%d.%s[", IlcQAv1::GetImageFileName(), GetMode(), fRunNumber, IlcQAv1::GetImageFileFormat()), "ps") ;    
  }    
  return kTRUE ; 
}

//_____________________________________________________________________________
void  IlcQAManager::InitQADataMaker(UInt_t run, TObjArray * detArray) 
{
  // Initializes The QADataMaker for all active detectors and for all active tasks 
  fRunNumber = run ; 
  for (UInt_t iDet = 0; iDet < fgkNDetectors ; iDet++) {
    if (IsSelected(IlcQAv1::GetDetName(iDet))) {
      IlcQADataMaker * qadm = GetQADataMaker(iDet) ;
      if (!qadm) {
	IlcError(Form("IlcQADataMaker not found for %s", IlcQAv1::GetDetName(iDet))) ; 
	fDetectorsW.ReplaceAll(IlcQAv1::GetDetName(iDet), "") ; 
      } else {
        if (fQAWriteExpert[iDet])
          qadm->SetWriteExpert() ; 
	IlcDebug(IlcQAv1::GetQADebugLevel(), Form("Data Maker found for %s %d", qadm->GetName(), qadm->WriteExpert())) ; 
	// skip non active detectors
	if (detArray) {
	  IlcModule* det = static_cast<IlcModule*>(detArray->FindObject(IlcQAv1::GetDetName(iDet))) ;
	  if (!det || !det->IsActive())  
	    continue ;
	}
	// Set default reco params
        Bool_t sameCycle = kFALSE ; 
	for (UInt_t taskIndex = 0; taskIndex < IlcQAv1::kNTASKINDEX; taskIndex++) {
	  if ( fTasks.Contains(Form("%d", taskIndex)) ) {
	    qadm->Init(IlcQAv1::GetTaskIndex(IlcQAv1::GetTaskName(taskIndex)), GetQACycles(qadm->GetUniqueID())) ;
            qadm->StartOfCycle(IlcQAv1::GetTaskIndex(IlcQAv1::GetTaskName(taskIndex)), run,  sameCycle) ;
            sameCycle = kTRUE ;
	  }
	}
      }
    }
  }
}


//_____________________________________________________________________________
Bool_t IlcQAManager::InitRunLoader()
{
  // get or create the run loader
  if (fRunLoader) {
    fCycleSame = kTRUE ; 
  } else {
    if (!gSystem->AccessPathName(fGIlcFileName.Data())) { // gilc.root exists
      // load all base libraries to get the loader classes
      TString libs = gSystem->GetLibraries() ;
      for (UInt_t iDet = 0; iDet < fgkNDetectors; iDet++) {
	if (!IsSelected(IlcQAv1::GetDetName(iDet))) 
	  continue ; 
	TString detName = IlcQAv1::GetDetName(iDet) ;
	if (detName == "HLT") 
	  continue;
	if (libs.Contains("lib" + detName + "base.so")) 
	  continue;
	gSystem->Load("lib" + detName + "base.so");
      }
      fRunLoader = IlcRunLoader::Open(fGIlcFileName.Data());
      if (!fRunLoader) {
	IlcError(Form("no run loader found in file %s", fGIlcFileName.Data()));
	return kFALSE;
      }
      fRunLoader->CdGAFile();
      if (fRunLoader->LoadgIlc() == 0) {
	gIlc = fRunLoader->GetIlcRun();
      }

      if (!gIlc) {
	IlcError(Form("no gIlc object found in file %s", fGIlcFileName.Data()));
	return kFALSE;
      }

    } else {               // gilc.root does not exist
      IlcError(Form("the file %s does not exist", fGIlcFileName.Data()));
      return kFALSE;
    }
  }

  if (!fRunNumber) { 
    fRunLoader->LoadHeader();
    fRunNumber = fRunLoader->GetHeader()->GetRun() ; 
  }
  return kTRUE;
}

//_____________________________________________________________________________
Bool_t IlcQAManager::IsSelected(const Char_t * det) 
{
  // check whether detName is contained in detectors
  // if yes, it is removed from detectors
	
  Bool_t rv = kFALSE;
  const TString detName(det) ;
  // always activates Correlation
//  if ( detName.Contains(IlcQAv1::GetDetName(IlcQAv1::kCORR)) || detName.Contains(IlcQAv1::GetDetName(IlcQAv1::kGLOBAL))) {
//    rv = kTRUE ; 
//  } else {
    // check if all detectors are selected
    if (fDetectors.Contains("ALL")) {
      fDetectors = "ALL";
      rv = kTRUE;
    } else if ((fDetectors.CompareTo(detName) == 0) ||
               fDetectors.BeginsWith(detName+" ") ||
               fDetectors.EndsWith(" "+detName) ||
               fDetectors.Contains(" "+detName+" ")) {
      rv = kTRUE;
    }
//  }
  return rv ;
}

//_____________________________________________________________________________
Bool_t IlcQAManager::Merge(Int_t runNumber, const char *fileName) const
{
  // Merge data from all detectors from a given run in one single file 
  // Merge the QA results from all the data chunks in one run
  // The 'fileName' is name of the output file with merged QA data  
  if ( runNumber == -1)
    runNumber = fRunNumber ; 
  Bool_t rv = MergeData(runNumber,fileName) ; 
  //rv *= MergeResults(runNumber) ; // not needed for the time being
  return rv ; 
}
	
//______________________________________________________________________
Bool_t IlcQAManager::MergeXML(const Char_t * collectionFile, const Char_t * subFile, const Char_t * outFile) 
{
  // merges files listed in a xml collection 
  // usage Merge(collection, outputFile))
  //              collection: is a xml collection  
  
  Bool_t rv = kFALSE ; 
  
  if ( strstr(collectionFile, ".xml") == 0 ) {
    IlcError("Input collection file must be an \".xml\" file\n") ; 
    return kFALSE ; 
  }
    
  if ( !gGrid ) 
    TGrid::Connect("alien://"); 
  if ( !gGrid ) 
    return kFALSE ; 
 
  // Open the file collection 
  IlcInfoClass(Form("*** Create Collection       ***\n***  Wk-Dir = |%s|             \n***  Coll   = |%s|             \n",gSystem->WorkingDirectory(), collectionFile));              	
  
  TGridCollection * collection = (TGridCollection*)gROOT->ProcessLine(Form("TAlienCollection::Open(\"%s\")",collectionFile));
  TGridResult* result = collection->GetGridResult("", 0, 0);
  
  Int_t index = 0  ;
  const Char_t * turl ;
  TFileMerger merger(kFALSE) ; 
  if (!outFile) {
    TString tempo(collectionFile) ; 
    if ( subFile) 
      tempo.ReplaceAll(".xml", subFile) ; 
    else 
      tempo.ReplaceAll(".xml", "_Merged.root") ; 
    outFile = tempo.Data() ; 
  }
  merger.OutputFile(outFile) ; 
  
  while ( (turl = result->GetKey(index, "turl")) ) {
    Char_t * file ;
    if ( subFile )
      file = Form("%s#%s", turl, subFile) ; 
    else 
      file = Form("%s", turl) ; 
    
    IlcDebug(IlcQAv1::GetQADebugLevel(), Form("%s\n", file)) ; 
    merger.AddFile(file) ; 
    index++ ;  
  }
  
  if (index) 
    merger.Merge() ; 
  
  IlcDebug(IlcQAv1::GetQADebugLevel(), Form("Files merged into %s\n", outFile)) ;
  
  rv = kFALSE;
  return rv ;
}

//_____________________________________________________________________________
void IlcQAManager::MergeCustom() const
{
  // Custom Merge of QA data from all detectors for all runs in one single file 
  // search all the run numbers
  // search all the run numbers
  gROOT->ProcessLine(".! ls *QA*.root > QAtempo.txt") ;
  TString theQAfile ; 
  FILE * theQAfiles = fopen("QAtempo.txt", "r") ; 
  Int_t index = 0 ; 
  TList srunList ; 
  TIter nextRun(&srunList) ; 
  TObjString * srun = NULL ; 
  Int_t loRun = 999999999 ; 
  Int_t hiRun = 0 ; 
  while ( theQAfile.Gets(theQAfiles) ) {
    Bool_t runExist = kFALSE ; 
    TString srunNew(theQAfile(theQAfile.Index("QA.")+3, theQAfile.Index(".root")-(theQAfile.Index("QA.")+3))) ; 
    Int_t cuRun = srunNew.Atoi() ;
    if (cuRun < loRun) 
      loRun = cuRun ; 
    if (cuRun > hiRun)
      hiRun = cuRun ; 
    while ( (srun = static_cast<TObjString *> (nextRun())) ) {
      if ( cuRun == (srun->String()).Atoi() ) {
        runExist = kTRUE ; 
        break ; 
      } 
    }
    nextRun.Reset() ; 
    if ( ! runExist ) 
      srunList.Add(new TObjString(srunNew.Data()));
  }
  nextRun.Reset() ;    
  Int_t runNumber = 0 ; 
  TFile mergedFile(Form("Merged.%s.Data.root", IlcQAv1::GetQADataFileName()), "RECREATE") ; 
  TH1I * hisRun = new TH1I("hLMR", "List of merged runs", hiRun-loRun+10, loRun, hiRun+10) ; 
  // create the structure into the merged file
  for (Int_t iDet = 0; iDet < IlcQAv1::kNDET ; iDet++) {
    TDirectory * detDir = mergedFile.mkdir(IlcQAv1::GetDetName(iDet)) ; 
    for (Int_t taskIndex = 0; taskIndex < IlcQAv1::kNTASKINDEX; taskIndex++) {
      detDir->cd() ; 
      TDirectory * taskDir = gDirectory->mkdir(IlcQAv1::GetTaskName(taskIndex)) ; 
      for (Int_t es = 0 ; es < IlcRecoParam::kNSpecies ; es++) {
        taskDir->cd() ; 
        TDirectory * esDir = gDirectory->mkdir(IlcRecoParam::GetEventSpecieName(es)) ;
        esDir->cd() ; 
        gDirectory->mkdir(IlcQAv1::GetExpert()) ; 
      }
    }
  }
  while ( (srun = static_cast<TObjString *> (nextRun())) ) {
    runNumber = (srun->String()).Atoi() ; 
    hisRun->Fill(runNumber) ; 
    IlcDebug(IlcQAv1::GetQADebugLevel(), Form("Merging run number %d", runNumber)) ; 
    // search all QA files for runNumber in the current directory
    Char_t * fileList[IlcQAv1::kNDET] ;
    index = 0 ; 
    for (Int_t iDet = 0; iDet < IlcQAv1::kNDET ; iDet++) {
      Char_t * file = gSystem->Which(gSystem->WorkingDirectory(), Form("%s.%s.%d.root", IlcQAv1::GetDetName(iDet), IlcQAv1::GetQADataFileName(), runNumber)); 
      if (file) 
        fileList[index++] = file ;
    }
    if ( index == 0 ) {
      IlcError("No QA data file found\n") ; 
      return ; 
    }
    for ( Int_t i = 0 ; i < index ; i++) {
      TFile * inFile = TFile::Open(fileList[i]) ;  
      TList * listOfKeys =inFile->GetListOfKeys() ; 
      TIter nextkey(listOfKeys) ; 
      TObject * obj1 ; 
      TString dirName("") ; 
      while ( (obj1 = nextkey()) ) {
        TDirectory * directoryDet = inFile->GetDirectory(obj1->GetName()) ; 
        if ( directoryDet ) {
          IlcDebug(IlcQAv1::GetQADebugLevel(), Form("%s dir = %s", inFile->GetName(), directoryDet->GetName())) ; 
          dirName += Form("%s/", directoryDet->GetName() ) ; 
          directoryDet->cd() ;
          TList * listOfTasks = directoryDet->GetListOfKeys() ; 
          TIter nextTask(listOfTasks) ; 
          TObject * obj2 ; 
          while ( (obj2 = nextTask()) ) {
            TDirectory * directoryTask = directoryDet->GetDirectory(obj2->GetName()) ; 
            if ( directoryTask ) {
              dirName += Form("%s", obj2->GetName()) ; 
              IlcDebug(IlcQAv1::GetQADebugLevel(), Form("%s", dirName.Data())) ; 
              directoryTask->cd() ; 
              TList * listOfEventSpecie = directoryTask->GetListOfKeys() ; 
              TIter nextEventSpecie(listOfEventSpecie) ; 
              TObject * obj3 ; 
              while ( (obj3 = nextEventSpecie()) ) {
                TDirectory * directoryEventSpecie = directoryTask->GetDirectory(obj3->GetName()) ; 
                if ( directoryEventSpecie ) {
                  dirName += Form("/%s/", obj3->GetName()) ; 
                  IlcDebug(IlcQAv1::GetQADebugLevel(), Form("%s\n", dirName.Data())) ; 
                  directoryEventSpecie->cd() ; 
                  // histograms are here
                  TDirectory * mergedDirectory = mergedFile.GetDirectory(dirName.Data()) ;
                  TList * listOfData = directoryEventSpecie->GetListOfKeys() ; 
                  TIter nextData(listOfData) ; 
                  TKey * key ; 
                  while ( (key = static_cast<TKey *>(nextData())) ) {
                    TString className(key->GetClassName()) ; 
                    if (  className.Contains("TH") || className.Contains("TProfile") ) {
                      TH1 * histIn = static_cast<TH1*> (key->ReadObj()) ; 
                      TH1 * histOu = static_cast<TH1*> (mergedDirectory->FindObjectAny(histIn->GetName())) ; 
                      IlcDebug(IlcQAv1::GetQADebugLevel(), Form("%s %p %p\n", key->GetName(), histIn, histOu)) ; 
                      mergedDirectory->cd() ; 
                      if ( ! histOu ) {
                        histIn->Write() ; 
                      } else {
                        histOu->Add(histIn) ; 
                        histOu->Write(histOu->GetName(), kOverwrite) ; 
                      }
                    }
                    else if ( className.Contains("TDirectoryFile") ) {
                      TDirectory * dirExpert = directoryEventSpecie->GetDirectory(key->GetName()) ; 
                      dirExpert->cd() ; 
                      TDirectory * mergedDirectoryExpert = mergedDirectory->GetDirectory(dirExpert->GetName()) ; 
                      TList * listOfExpertData = dirExpert->GetListOfKeys() ; 
                      TIter nextExpertData(listOfExpertData) ; 
                      TKey * keykey ; 
                      while ( (keykey = static_cast<TKey *>(nextExpertData())) ) {
                        TString classNameExpert(keykey->GetClassName()) ; 
                        if (classNameExpert.Contains("TH")) {
                          TH1 * histInExpert = static_cast<TH1*> (keykey->ReadObj()) ; 
                          TH1 * histOuExpert = static_cast<TH1*> (mergedDirectory->FindObjectAny(histInExpert->GetName())) ; 
                          mergedDirectoryExpert->cd() ; 
                          if ( ! histOuExpert ) {
                            histInExpert->Write() ; 
                          } else {
                            histOuExpert->Add(histInExpert) ; 
                            histOuExpert->Write(histOuExpert->GetName(), kOverwrite) ; 
                          }
                        }
                      }
                    } else {
                      IlcError(Form("No merge done for this object %s in %s", key->GetName(), dirName.Data())) ; 
                    }
                  }
                  dirName.ReplaceAll(Form("/%s/",obj3->GetName()), "") ; 
                }
              }
              dirName.ReplaceAll(obj2->GetName(), "") ; 
            }
          }
        }
      }
      inFile->Close() ; 
    }
  }
  mergedFile.cd() ;
  hisRun->Write() ; 
  mergedFile.Close() ; 
  srunList.Delete() ;   
}

//_____________________________________________________________________________
Bool_t IlcQAManager::MergeData(const Int_t runNumber, const char *fileName) const
{
  // Merge QA data from all detectors for a given run in one single file 
  
  TFileMerger merger(kFALSE) ; 
  TString outFileName = fileName;
  if (outFileName.IsNull()) outFileName.Form("Merged.%s.Data.root",IlcQAv1::GetQADataFileName());
  merger.OutputFile(outFileName.Data()) ; 
  for (UInt_t iDet = 0; iDet < fgkNDetectors ; iDet++) {
    Char_t * file = gSystem->Which(gSystem->WorkingDirectory(), Form("%s.%s.%d.root", IlcQAv1::GetDetName(iDet), IlcQAv1::GetQADataFileName(), runNumber)); 
    if (file) 
      merger.AddFile(file);
    delete[] file;
  }
  merger.Merge() ; 
  return kTRUE ; 
}

//_____________________________________________________________________________
Bool_t IlcQAManager::MergeResults(const Int_t runNumber) const
{
  // Merge the QA result from all the data chunks in a run 
  // to be revised whwn it will be used (see MergeData)
  TString cmd ;
  cmd = Form(".! ls %s*.root > tempo.txt", IlcQAv1::GetQADataFileName()) ; 
  gROOT->ProcessLine(cmd.Data()) ;
  ifstream in("tempo.txt") ; 
  const Int_t chunkMax = 100 ;  
  TString fileList[chunkMax] ;
	
  Int_t index = 0 ; 
  while ( 1 ) {
    TString file ; 
    in >> fileList[index] ; 
    if ( !in.good() ) 
      break ; 
    IlcDebug(IlcQAv1::GetQADebugLevel(), Form("index = %d file = %s", index, (fileList[index].Data()))) ; 
    index++ ;
  }
	
  if ( index == 0 ) { 
    IlcError("No QA Result File found") ; 
    return kFALSE ; 
  }
	
  TFileMerger merger ; 
  TString outFileName ;
  if (runNumber != -1) 
    outFileName = Form("Merged.%s.Result.%d.root",IlcQAv1::GetQADataFileName(),runNumber); 
  else 
    outFileName = Form("Merged.%s.Result.root",IlcQAv1::GetQADataFileName()); 
  merger.OutputFile(outFileName.Data()) ; 
  for (Int_t ifile = 0 ; ifile < index ; ifile++) {
    TString file = fileList[ifile] ; 
    merger.AddFile(file) ; 
  }
  merger.Merge() ; 
	
  return kTRUE ; 
}

//_____________________________________________________________________________
void IlcQAManager::Reset(const Bool_t sameCycle)
{
  // Reset the default data members

  for (UInt_t iDet = 0; iDet < fgkNDetectors ; iDet++) {
    if (IsSelected(IlcQAv1::GetDetName(iDet))) {
      IlcQADataMaker * qadm = GetQADataMaker(iDet);
      if (qadm) 
        qadm->Reset();
    }
  } 
  if (fRawReaderDelete) { 
    delete fRawReader ;
    fRawReader      = NULL ;
  }

  fCycleSame      = sameCycle ; 
  fESD            = NULL ; 
  fESDTree        = NULL ; 
  //fFirst          = kTRUE ;   
  fNumberOfEvents = 999999 ;  
}

//_____________________________________________________________________________
void IlcQAManager::ResetDetectors(IlcQAv1::TASKINDEX_t task, IlcQAv1::DETECTORINDEX_t det)
{
  //calls ResetDetector of specified or all detectors
  UInt_t iDet    = 0 ;
  UInt_t iDetMax = fgkNDetectors ;    
  if ( det != IlcQAv1::kNULLDET ) {
    iDet    = det ;
    iDetMax = det+1 ;    
  }
  
  for (iDet = 0; iDet < iDetMax ; iDet++) {
    if (IsSelected(IlcQAv1::GetDetName(iDet))) {
      IlcQADataMaker * qadm = GetQADataMaker(iDet);
      qadm->ResetDetector(task);
    }
  }   
}

//_____________________________________________________________________________
IlcQAManager * IlcQAManager::QAManager(IlcQAv1::MODE_t mode, TMap *entryCache, Int_t run) 
{
  // returns IlcQAManager instance (singleton)
  
  if (!fgQAInstance) {
    if ( (mode != IlcQAv1::kSIMMODE) && (mode != IlcQAv1::kRECMODE) && (mode != IlcQAv1::kQAMODE) ) {
      IlcWarningClass("You must specify kSIMMODE or kRECMODE or kQAMODE") ; 
      return NULL ; 
    }
    fgQAInstance = new IlcQAManager(mode) ;  
    if (!entryCache)
      fgQAInstance->Init();
    else
      fgQAInstance->InitFromCache(entryCache,run);
  }
  return fgQAInstance;
}

//_____________________________________________________________________________
IlcQAManager * IlcQAManager::QAManager(IlcQAv1::TASKINDEX_t task) 
{
  // returns IlcQAManager instance (singleton)
  return QAManager(IlcQAv1::Mode(task)) ; 
}

//_____________________________________________________________________________
TString IlcQAManager::Run(const Char_t * detectors, IlcRawReader * rawReader, const Bool_t sameCycle) 
{
  //Runs all the QA data Maker for Raws only
	
  fCycleSame       = sameCycle ;
  fRawReader       = rawReader ;
  fDetectors       = detectors ; 
  fDetectorsW      = detectors ; 	
	
  IlcCDBManager* man = IlcCDBManager::Instance() ; 

  if ( man->GetRun() == -1 ) {// check if run number not set previously and set it from raw data
    rawReader->NextEvent() ; 
    man->SetRun(fRawReader->GetRunNumber()) ;
    rawReader->RewindEvents() ;
  }	
	
  if (!fCycleSame) 
    if ( !InitQA(IlcQAv1::kRAWS) ) 
      return "" ; 
  fRawReaderDelete = kFALSE ; 

  DoIt(IlcQAv1::kRAWS) ; 
  return 	fDetectorsW ;
}

//_____________________________________________________________________________
TString IlcQAManager::Run(const Char_t * detectors, const Char_t * fileName, const Bool_t sameCycle) 
{
  //Runs all the QA data Maker for Raws only

  fCycleSame       = sameCycle ;
  fDetectors       = detectors ; 
  fDetectorsW      = detectors ; 	
	
  IlcCDBManager* man = IlcCDBManager::Instance() ; 
  if ( man->GetRun() == -1 ) { // check if run number not set previously and set it from IlcRun
    IlcRunLoader * rl = IlcRunLoader::Open("gilc.root") ;
    if ( ! rl ) {
      IlcFatal("gilc.root file not found in current directory") ; 
    } else {
      rl->CdGAFile() ; 
      rl->LoadgIlc() ;
      if ( ! rl->GetIlcRun() ) {
	IlcFatal("IlcRun not found in gilc.root") ;
      } else {
	rl->LoadHeader() ;
	man->SetRun(rl->GetHeader()->GetRun());
      }
    }
  }
	
  if (!fCycleSame) 
    if ( !InitQA(IlcQAv1::kRAWS, fileName) ) 
      return "" ; 
	
  DoIt(IlcQAv1::kRAWS) ; 
  return 	fDetectorsW ;
}

//_____________________________________________________________________________
TString IlcQAManager::Run(const Char_t * detectors, const IlcQAv1::TASKINDEX_t taskIndex, Bool_t const sameCycle, const  Char_t * fileName ) 
{
  // Runs all the QA data Maker for every detector
	
  fCycleSame       = sameCycle ;
  fDetectors       = detectors ; 
  fDetectorsW      = detectors ; 		
	
  IlcCDBManager* man = IlcCDBManager::Instance() ; 	
  if ( man->GetRun() == -1 ) { // check if run number not set previously and set it from IlcRun
    IlcRunLoader * rl = IlcRunLoader::Open("gilc.root") ;
    if ( ! rl ) {
      IlcFatal("gilc.root file not found in current directory") ; 
    } else {
      rl->CdGAFile() ; 
      rl->LoadgIlc() ;
      if ( ! rl->GetIlcRun() ) {
	IlcDebug(IlcQAv1::GetQADebugLevel(), "IlcRun not found in gilc.root") ;
      } else {
	rl->LoadHeader() ;
	man->SetRun(rl->GetHeader()->GetRun()) ;
      }
    }
  }
  if ( taskIndex == IlcQAv1::kNULLTASKINDEX) { 
    for (UInt_t task = 0; task < IlcQAv1::kNTASKINDEX; task++) {
      if ( fTasks.Contains(Form("%d", task)) ) {
        if (!fCycleSame)
          if ( !InitQA(IlcQAv1::GetTaskIndex(IlcQAv1::GetTaskName(task)), fileName) ) 
            return "" ;
        DoIt(IlcQAv1::GetTaskIndex(IlcQAv1::GetTaskName(task))) ;
      }
    }
  } else {
    if (! fCycleSame )
      if ( !InitQA(taskIndex, fileName) ) 
        return "" ; 
    DoIt(taskIndex) ; 
  } 		
  return fDetectorsW ;
}

//_____________________________________________________________________________
void IlcQAManager::RunOneEvent(IlcRawReader * rawReader) 
{
    //Runs all the QA data Maker for Raws only and on one event only (event loop done by calling method)
  
  if ( ! rawReader ) 
    return ; 
  if (fTasks.Contains(Form("%d", IlcQAv1::kRAWS))){
    TString detList ; 
    if ( GetEventInfo()) 
      detList = GetEventInfo()->GetTriggerCluster() ; 
    for (UInt_t iDet = 0; iDet < fgkNDetectors; iDet++) {
      if (!IsSelected(IlcQAv1::GetDetName(iDet)) || (!detList.IsNull() && !detList.Contains(IlcQAv1::GetDetName(iDet)))) 
        continue;
      IlcQADataMaker *qadm = GetQADataMaker(iDet);  
      if (!qadm) 
        continue;
      if ( qadm->IsCycleDone() ) {
        qadm->EndOfCycle() ;
      }
      qadm->SetEventSpecie(fEventSpecie) ;  
      if ( qadm->GetRecoParam() ) 
        if ( IlcRecoParam::Convert(qadm->GetRecoParam()->GetEventSpecie()) != IlcRecoParam::kDefault) 
          qadm->SetEventSpecie(qadm->GetRecoParam()->GetEventSpecie()) ; 
      qadm->Exec(IlcQAv1::kRAWS, rawReader) ;
    }
  }
}

//_____________________________________________________________________________
void IlcQAManager::RunOneEvent(IlcESDEvent *& esd, IlcESDEvent *& hltesd) 
{
    //Runs all the QA data Maker for ESDs only and on one event only (event loop done by calling method)
	
  if (fTasks.Contains(Form("%d", IlcQAv1::kESDS))) {
    TString detList ; 
    if ( GetEventInfo()) 
      detList = GetEventInfo()->GetTriggerCluster() ; 
    for (UInt_t iDet = 0; iDet < fgkNDetectors; iDet++) {
      if (!IsSelected(IlcQAv1::GetDetName(iDet)) || (!detList.IsNull() && !detList.Contains(IlcQAv1::GetDetName(iDet)))) 
        continue;
      IlcQADataMaker *qadm = GetQADataMaker(iDet);  
      if (!qadm) 
        continue;
      qadm->SetEventSpecie(fEventSpecie) ;  
      if ( qadm->GetRecoParam() ) 
        if ( IlcRecoParam::Convert(qadm->GetRecoParam()->GetEventSpecie()) != IlcRecoParam::kDefault)  
          qadm->SetEventSpecie(qadm->GetRecoParam()->GetEventSpecie()) ; 
      if ( qadm->IsCycleDone() ) {
        qadm->EndOfCycle() ;
      }
      if (iDet == IlcQAv1::kHLT) {
        TObjArray esdarray;
        esdarray.Add(esd); 
        esdarray.Add(hltesd); 
        qadm->Exec(IlcQAv1::kESDS, &esdarray);
      } else {
        qadm->Exec(IlcQAv1::kESDS, esd) ;        
      }
    }
  }
}

//_____________________________________________________________________________
void IlcQAManager::RunOneEventInOneDetector(Int_t det, TTree * tree) 
{
    // Runs all the QA data Maker for ESDs only and on one event only (event loop done by calling method)
  
  TString detList ; 
  if ( GetEventInfo()) 
    detList = GetEventInfo()->GetTriggerCluster() ; 
  if (!detList.IsNull() && !detList.Contains(IlcQAv1::GetDetName(det)))
    return ;

  TString test(tree->GetName()) ; 
  if (fTasks.Contains(Form("%d", IlcQAv1::kRECPOINTS))) {
    if (IsSelected(IlcQAv1::GetDetName(det))) {      
      IlcQADataMaker *qadm = GetQADataMaker(det);  
      if (qadm) { 
        qadm->SetEventSpecie(fEventSpecie) ;  
        if ( qadm->GetRecoParam() ) {
          if ( IlcRecoParam::Convert(qadm->GetRecoParam()->GetEventSpecie()) != IlcRecoParam::kDefault)  
            qadm->SetEventSpecie(qadm->GetRecoParam()->GetEventSpecie()) ; 
          else
            IlcError(Form("%d defined by %s is not an event specie", qadm->GetRecoParam()->GetEventSpecie(), qadm->GetName())) ; 
        }                    
        if ( qadm->IsCycleDone() ) {
          qadm->EndOfCycle() ;
        }
        if (test.Contains("TreeD")) {
          qadm->Exec(IlcQAv1::kDIGITSR, tree) ;
        } else  if (test.Contains("TreeR")) {
          qadm->Exec(IlcQAv1::kRECPOINTS, tree) ;
        }
      }
    }
  }
}

//_____________________________________________________________________________
Bool_t IlcQAManager::Save2OCDB(const Int_t runNumber, IlcRecoParam::EventSpecie_t es, const Char_t * year, const Char_t * detectors) const
{
  // take the locasl QA data merge into a single file and save in OCDB 
  Bool_t rv = kTRUE ; 
  TString tmp(IlcQAv1::GetQARefStorage()) ; 
  if ( tmp.IsNull() ) { 
    IlcError("No storage defined, use IlcQAv1::SetQARefStorage") ; 
    return kFALSE ; 
  }
  if ( !(tmp.Contains(IlcQAv1::GetLabLocalOCDB()) || tmp.Contains(IlcQAv1::GetLabAliEnOCDB())) ) {
    IlcError(Form("%s is a wrong storage, use %s or %s", IlcQAv1::GetQARefStorage(), IlcQAv1::GetLabLocalOCDB().Data(), IlcQAv1::GetLabAliEnOCDB().Data())) ; 
    return kFALSE ; 
  }
  TString sdet(detectors) ; 
  sdet.ToUpper() ;
  TFile * inputFile ; 
  if ( sdet.Contains("ALL") ) {
    rv = Merge(runNumber) ; 
    if ( ! rv )
      return kFALSE ; 
    TString inputFileName(Form("Merged.%s.Data.%d.root", IlcQAv1::GetQADataFileName(), runNumber)) ; 
    inputFile = TFile::Open(inputFileName.Data()) ; 
    rv = SaveIt2OCDB(runNumber, inputFile, year, es) ; 
  } else {
    for (Int_t index = 0; index < IlcQAv1::kNDET; index++) {
      if (sdet.Contains(IlcQAv1::GetDetName(index))) {
	TString inputFileName(Form("%s.%s.%d.root", IlcQAv1::GetDetName(index), IlcQAv1::GetQADataFileName(), runNumber)) ; 
	inputFile = TFile::Open(inputFileName.Data()) ; 			
	rv *= SaveIt2OCDB(runNumber, inputFile, year, es) ; 
      }
    }
  }
  return rv ; 
}

//_____________________________________________________________________________
Bool_t IlcQAManager::SaveIt2OCDB(const Int_t runNumber, TFile * inputFile, const Char_t * year, IlcRecoParam::EventSpecie_t es) const
{
  // reads the TH1 from file and adds it to appropriate list before saving to OCDB
  Bool_t rv = kTRUE ;
  IlcDebug(IlcQAv1::GetQADebugLevel(), Form("Saving TH1s in %s to %s", inputFile->GetName(), IlcQAv1::GetQARefStorage())) ; 
  if ( ! IsDefaultStorageSet() ) {
    TString tmp( IlcQAv1::GetQARefStorage() ) ; 
    if ( tmp.Contains(IlcQAv1::GetLabLocalOCDB()) ) 
      Instance()->SetDefaultStorage(IlcQAv1::GetQARefStorage()) ;
    else {
      TString tmp1(IlcQAv1::GetQARefDefaultStorage()) ; 
      tmp1.Append(year) ; 
      tmp1.Append("?user=ilcdaq") ; 
      Instance()->SetDefaultStorage(tmp1.Data()) ; 
    }
  }
  Instance()->SetSpecificStorage("*", IlcQAv1::GetQARefStorage()) ; 
  if(GetRun() < 0) 
    Instance()->SetRun(runNumber);

  IlcCDBMetaData mdr ;
  mdr.SetResponsible("yves schutz");

  for ( Int_t detIndex = 0 ; detIndex < IlcQAv1::kNDET ; detIndex++) {
    TDirectory * detDir = inputFile->GetDirectory(IlcQAv1::GetDetName(detIndex)) ; 
    if ( detDir ) {
      IlcDebug(IlcQAv1::GetQADebugLevel(), Form("Entering %s", detDir->GetName())) ;
      IlcQAv1::SetQARefDataDirName(es) ;
      TString detOCDBDir(Form("%s/%s/%s", IlcQAv1::GetDetName(detIndex), IlcQAv1::GetRefOCDBDirName(), IlcQAv1::GetRefDataDirName())) ; 
      IlcCDBId idr(detOCDBDir.Data(), runNumber, IlcCDBRunRange::Infinity())  ;
      TList * listDetQAD = new TList() ;
      TString listName(Form("%s QA data Reference", IlcQAv1::GetDetName(detIndex))) ; 
      mdr.SetComment(Form("%s QA stuff", IlcQAv1::GetDetName(detIndex)));
      listDetQAD->SetName(listName) ; 
      TList * taskList = detDir->GetListOfKeys() ; 
      TIter nextTask(taskList) ; 
      TKey * taskKey ; 
      while ( (taskKey = static_cast<TKey*>(nextTask())) ) {
	TDirectory * taskDir = detDir->GetDirectory(taskKey->GetName()) ; 
        TDirectory * esDir   = taskDir->GetDirectory(IlcRecoParam::GetEventSpecieName(es)) ; 
	IlcDebug(IlcQAv1::GetQADebugLevel(), Form("Saving %s", esDir->GetName())) ; 
	TObjArray * listTaskQAD = new TObjArray(100) ; 
	listTaskQAD->SetName(Form("%s/%s", taskKey->GetName(), IlcRecoParam::GetEventSpecieName(es))) ;
	listDetQAD->Add(listTaskQAD) ; 
	TList * histList = esDir->GetListOfKeys() ; 
	TIter nextHist(histList) ; 
	TKey * histKey ; 
	while ( (histKey = static_cast<TKey*>(nextHist())) ) {
	  TObject * odata = esDir->Get(histKey->GetName()) ; 
	  if ( !odata ) {
	    IlcError(Form("%s in %s/%s returns a NULL pointer !!", histKey->GetName(), detDir->GetName(), taskDir->GetName())) ;
	  } else {
            if ( IlcQAv1::GetExpert() == histKey->GetName() ) {
              TDirectory * expertDir   = esDir->GetDirectory(histKey->GetName()) ; 
              TList * expertHistList = expertDir->GetListOfKeys() ; 
              TIter nextExpertHist(expertHistList) ; 
              TKey * expertHistKey ; 
              while ( (expertHistKey = static_cast<TKey*>(nextExpertHist())) ) {
                TObject * expertOdata = expertDir->Get(expertHistKey->GetName()) ; 
                if ( !expertOdata ) {
                  IlcError(Form("%s in %s/%s/Expert returns a NULL pointer !!", expertHistKey->GetName(), detDir->GetName(), taskDir->GetName())) ;
                } else {
                  IlcDebug(IlcQAv1::GetQADebugLevel(), Form("Adding %s", expertHistKey->GetName())) ;
                  if ( expertOdata->IsA()->InheritsFrom("TH1") ) {
                    IlcDebug(IlcQAv1::GetQADebugLevel(), Form("Adding %s", expertHistKey->GetName())) ;
                    TH1 * hExpertdata = static_cast<TH1*>(expertOdata) ; 
                    listTaskQAD->Add(hExpertdata) ; 
                  }                  
                }                
              }
            }
	    IlcDebug(IlcQAv1::GetQADebugLevel(), Form("Adding %s", histKey->GetName())) ;
	    if ( odata->IsA()->InheritsFrom("TH1") ) {
	      IlcDebug(IlcQAv1::GetQADebugLevel(), Form("Adding %s", histKey->GetName())) ;
	      TH1 * hdata = static_cast<TH1*>(odata) ; 
	      listTaskQAD->Add(hdata) ; 
	    }
	  }
	}
      }
      Instance()->Put(listDetQAD, idr, &mdr) ;
    }
  }
  return rv ; 
}	

//_____________________________________________________________________________

void IlcQAManager::SetCheckerExternParam(IlcQAv1::DETECTORINDEX_t detIndex, TList * parameterList) 
{
  // set the external parameters list for the detector checkers 
  IlcQACheckerBase * qac = IlcQAChecker::Instance()->GetDetQAChecker(detIndex) ; 
  qac->SetExternParamlist(parameterList) ; 
  qac->PrintExternParam() ;  
}

//_____________________________________________________________________________
void IlcQAManager::SetEventSpecie(IlcRecoParam::EventSpecie_t es) 
{
  // set the current event specie and inform IlcQAv1 that this event specie has been encountered
  fEventSpecie = es ; 
  IlcQAv1::Instance()->SetEventSpecie(es) ; 
}

//_____________________________________________________________________________
void IlcQAManager::SetRecoParam(const Int_t det, const IlcDetectorRecoParam *par) 
{
  // Set custom reconstruction parameters for a given detector
  // Single set of parameters for all the events
  GetQADataMaker(det)->SetRecoParam(par) ; 
}

//_____________________________________________________________________________
void IlcQAManager::SetWriteExpert()
{
  // enable the writing of QA expert data
  for (UInt_t iDet = 0; iDet < fgkNDetectors; iDet++) {
    if (IsSelected(IlcQAv1::GetDetName(iDet))) 
      fQAWriteExpert[iDet] = kTRUE ;
  }
}  

//_____________________________________________________________________________
void IlcQAManager::Destroy() {
  // delete IlcQAManager instance and
  // all associated objects

  if (fgQAInstance) {
    delete fgQAInstance ;
    fgQAInstance = NULL ;
  }
}

//_____________________________________________________________________________
void IlcQAManager::ShowQA() {
  // Show the result of the QA checking
  // for all detectors 
  for ( Int_t detIndex = 0 ; detIndex < IlcQAv1::kNDET ; detIndex++) 
    if ( IsSelected(IlcQAv1::GetDetName(detIndex)) ) 
      IlcQAv1::Instance(IlcQAv1::GetDetIndex(IlcQAv1::GetDetName(detIndex)))->Show() ; 
}

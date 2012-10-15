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
 * copies and that both the copyright notice and this permission notifce   *
 * appear in the supporting documentation. The authors make no claims     *
 * about the suitability of this software for any purpose. It is          *
 * provided "as is" without express or implied warranty.                  *
 **************************************************************************/


/* $Id: IlcQADataMakerRec.cxx 54863 2012-02-28 12:10:29Z hristov $ */

//
//  Base Class
//  Produces the data needed to calculate the quality assurance for Reconstruction
//  All data must be mergeable objects.
//  Y. Schutz CERN July 2007
//

// --- ROOT system ---
#include <TFile.h>
#include <TTree.h>
#include <TNtupleD.h>
#include <TObjArray.h>

// --- Standard library ---

// --- IlcRoot header files ---
#include "IlcCDBPath.h"
#include "IlcCDBEntry.h"
#include "IlcDetectorRecoParam.h"
#include "IlcCDBManager.h"

#include "IlcLog.h"
#include "IlcQADataMakerRec.h"
#include "IlcQAManager.h"
#include "IlcESDEvent.h"
#include "IlcRawReader.h"

ClassImp(IlcQADataMakerRec)
             
//____________________________________________________________________________ 
IlcQADataMakerRec::IlcQADataMakerRec(const char * name, const char * title) : 
IlcQADataMaker(name, title), 
  fDigitsQAList(NULL),
  fESDsQAList(NULL), 
  fRawsQAList(NULL), 
  fRecPointsQAList(NULL),
  fCorrNt(NULL), 
  fRecoParam(NULL),
  fRecPointsArray(NULL)
{
  // ctor
  fDetectorDirName = GetName() ; 
}

//____________________________________________________________________________ 
IlcQADataMakerRec::IlcQADataMakerRec(const IlcQADataMakerRec& qadm) :
  IlcQADataMaker(qadm.GetName(), qadm.GetTitle()), 
  fDigitsQAList(qadm.fDigitsQAList),
  fESDsQAList(qadm.fESDsQAList),
  fRawsQAList(qadm.fRawsQAList),
  fRecPointsQAList(qadm.fRecPointsQAList),
  fCorrNt(qadm.fCorrNt),  
  fRecoParam(qadm.fRecoParam),
  fRecPointsArray(NULL)
{
  //copy ctor
  fDetectorDirName = GetName() ; 
}

//____________________________________________________________________________ 
IlcQADataMakerRec::~IlcQADataMakerRec()
{
  //dtor: delete the TObjArray and thei content
  if ( fESDsQAList ) {
    for (Int_t specie = 0 ; specie < IlcRecoParam::kNSpecies ; specie++) {
      if ( fESDsQAList[specie] ) {
	fESDsQAList[specie]->Delete() ;     
      }
    }
    delete[] fESDsQAList ;
  }
  if ( fRawsQAList ) {
    for (Int_t specie = 0 ; specie < IlcRecoParam::kNSpecies ; specie++) {
      if ( fRawsQAList[specie] ) {
        fRawsQAList[specie]->Delete() ;
      }
    }
    delete[] fRawsQAList ;
  }
  if ( fDigitsQAList ) {
    for (Int_t specie = 0 ; specie < IlcRecoParam::kNSpecies ; specie++) {
      if ( fDigitsQAList[specie] ) {
        fDigitsQAList[specie]->Delete() ;
      }
    }
    delete[] fDigitsQAList ; 
  }
  if ( fRecPointsQAList ) {
    for (Int_t specie = 0 ; specie < IlcRecoParam::kNSpecies ; specie++) {
      if ( fRecPointsQAList[specie] ) {
        fRecPointsQAList[specie]->Delete() ;
      }
    }
    delete[] fRecPointsQAList ; 
  }
  if (fRecPointsArray) {
    fRecPointsArray->Clear() ; 
    delete fRecPointsArray ; 
  }
}

//__________________________________________________________________
IlcQADataMakerRec& IlcQADataMakerRec::operator = (const IlcQADataMakerRec& qadm )
{
  // Assignment operator.
  this->~IlcQADataMakerRec();
  new(this) IlcQADataMakerRec(qadm);
  return *this;
}

//____________________________________________________________________________
void IlcQADataMakerRec::EndOfCycle() 
{
  // Finishes a cycle of QA for all the tasks
  EndOfCycle(IlcQAv1::kRAWS) ; 
  EndOfCycle(IlcQAv1::kDIGITSR) ; 
  EndOfCycle(IlcQAv1::kRECPOINTS) ; 
  EndOfCycle(IlcQAv1::kESDS) ; 
  ResetCycle() ; 
}

//____________________________________________________________________________
void IlcQADataMakerRec::EndOfCycle(IlcQAv1::TASKINDEX_t task) 
{
  // Finishes a cycle of QA 
	
    
  TObjArray ** list = NULL ; 
	
  if ( task == IlcQAv1::kRAWS )     
    list = fRawsQAList ; 
  else if ( task == IlcQAv1::kDIGITSR ) 
    list = fDigitsQAList ; 
  else if ( task == IlcQAv1::kRECPOINTS ) 
    list = fRecPointsQAList ; 
  else if ( task == IlcQAv1::kESDS )
    list = fESDsQAList ; 

 
  if ( ! list && ! fCorrNt ) 
    return ; 
  //DefaultEndOfDetectorCycle(task) ;
  EndOfDetectorCycle(task, list) ;
  
  if (! IlcQAManager::QAManager(IlcQAv1::kRECMODE)->IsSaveData()) return; 

  fDetectorDir = fOutput->GetDirectory(GetDetectorDirName()) ; 
  if (!fDetectorDir) fDetectorDir = fOutput->mkdir(GetDetectorDirName()) ; 
  TDirectory * subDir = fDetectorDir->GetDirectory(IlcQAv1::GetTaskName(task)) ; 
  if (!subDir) subDir = fDetectorDir->mkdir(IlcQAv1::GetTaskName(task)) ;  
  subDir->cd();
  // 
  for (Int_t specie = 0 ; specie < IlcRecoParam::kNSpecies ; specie++) { // skip Default
    if (! IlcQAv1::Instance(IlcQAv1::GetDetIndex(GetName()))->IsEventSpecieSet(IlcRecoParam::ConvertIndex(specie)) || IlcRecoParam::ConvertIndex(specie) == IlcRecoParam::kDefault) continue ; 
    TDirectory * eventSpecieDir = subDir->GetDirectory(IlcRecoParam::GetEventSpecieName(specie)) ;
    if (!eventSpecieDir) eventSpecieDir = subDir->mkdir(IlcRecoParam::GetEventSpecieName(specie)) ; 
    eventSpecieDir->cd();    
    if (list) {
      if (list[specie]) {
        TIter next(list[specie]) ; 
        TObject * obj ; 
        while( (obj = next()) ) {
          if (!obj->TestBit(IlcQAv1::GetExpertBit())) obj->Write(); // RS: Note, this can be also TObjArray of clones
        }
        if (WriteExpert()) {
          TDirectory * expertDir = eventSpecieDir->GetDirectory(IlcQAv1::GetExpert()) ; 
          if (!expertDir)
            expertDir = eventSpecieDir->mkdir(IlcQAv1::GetExpert()) ; 
          expertDir->cd() ;
          next.Reset() ; 
          while( (obj = next()) ) {
            if (!obj->TestBit(IlcQAv1::GetExpertBit())) continue; 
            obj->Write();  // RS: Note, this can be also TObjArray of clones
          }      
        }
      }
    } 
    else if ( fCorrNt ) {
      if (fCorrNt[specie] && IlcQAv1::GetDetIndex(GetName()) == IlcQAv1::kCORR) {
        if (fCorrNt[specie]->GetNvar() != 0) {
          eventSpecieDir->cd() ; 
          fCorrNt[specie]->Write() ; 
        }
      }
      fOutput->Save() ; 
    }
  }
}

//____________________________________________________________________________
void IlcQADataMakerRec::Exec(IlcQAv1::TASKINDEX_t task, TObject * data) 
{ 
  // creates the quality assurance data for the various tasks (Hits, SDigits, Digits, ESDs)
	
  if ( task == IlcQAv1::kRAWS ) {
    IlcDebug(IlcQAv1::GetQADebugLevel(), "Processing Raws QA") ; 
    IlcRawReader * rawReader = static_cast<IlcRawReader *>(data) ; 
    if (rawReader) 
      MakeRaws(rawReader) ;
    else
      IlcDebug(IlcQAv1::GetQADebugLevel(), "Raw data are not processed") ;     
  } else if ( task == IlcQAv1::kDIGITSR ) {
    IlcDebug(IlcQAv1::GetQADebugLevel(), "Processing Digits QA") ; 
    TTree * tree = static_cast<TTree *>(data) ; 
    if (strcmp(tree->ClassName(), "TTree") == 0) {
      MakeDigits(tree) ; 
    } else {
      IlcWarning("data are not a TTree") ; 
    }
  } else if ( task == IlcQAv1::kRECPOINTS ) {
    IlcDebug(IlcQAv1::GetQADebugLevel(), "Processing RecPoints QA") ; 
    TTree * tree = static_cast<TTree *>(data) ; 
    if (strcmp(tree->ClassName(), "TTree") == 0) {
      MakeRecPoints(tree) ; 
    } else {
      IlcWarning("data are not a TTree") ; 
    }
  } else if ( task == IlcQAv1::kESDS ) {
    IlcDebug(IlcQAv1::GetQADebugLevel(), "Processing ESDs QA") ; 
    IlcESDEvent * esd = static_cast<IlcESDEvent *>(data) ; 
    if (strcmp(esd->ClassName(), "IlcESDEvent") == 0) 
      MakeESDs(esd) ;
    else 
      IlcError("Wrong type of esd container") ; 
  }  
}

//____________________________________________________________________________ 
TObjArray **  IlcQADataMakerRec::Init(IlcQAv1::TASKINDEX_t task, Int_t cycles)
{
  // general intialisation
  InitRecoParams() ;
  TObjArray ** rv = NULL ; 
  
  if (cycles > 0)
    SetCycle(cycles) ;  
	
  if ( task == IlcQAv1::kRAWS ) {
    if (! fRawsQAList ) { 
      fRawsQAList = new TObjArray *[IlcRecoParam::kNSpecies] ; 
      for (Int_t specie = 0 ; specie < IlcRecoParam::kNSpecies ; specie++) {
        fRawsQAList[specie] = new TObjArray(IlcQAv1::GetMaxQAObj()) ;	 
        fRawsQAList[specie]->SetName(Form("%s_%s_%s", GetName(), IlcQAv1::GetTaskName(task).Data(), IlcRecoParam::GetEventSpecieName(specie))) ;
      }
    }
    rv = fRawsQAList ;
  } else if ( task == IlcQAv1::kDIGITSR ) {
    if ( ! fDigitsQAList ) {
      fDigitsQAList = new TObjArray *[IlcRecoParam::kNSpecies] ; 
      for (Int_t specie = 0 ; specie < IlcRecoParam::kNSpecies ; specie++) {
        fDigitsQAList[specie] = new TObjArray(IlcQAv1::GetMaxQAObj()) ; 
        fDigitsQAList[specie]->SetName(Form("%s_%s_%s", GetName(), IlcQAv1::GetTaskName(task).Data(), IlcRecoParam::GetEventSpecieName(specie))) ; 
      }
    }
    rv = fDigitsQAList ;
  } else if ( task == IlcQAv1::kRECPOINTS ) {
    if ( ! fRecPointsQAList ) {
      fRecPointsQAList = new TObjArray *[IlcRecoParam::kNSpecies] ; 
      for (Int_t specie = 0 ; specie < IlcRecoParam::kNSpecies ; specie++) {
        fRecPointsQAList[specie] = new TObjArray(IlcQAv1::GetMaxQAObj()) ; 
        fRecPointsQAList[specie]->SetName(Form("%s_%s_%s", GetName(), IlcQAv1::GetTaskName(task).Data(), IlcRecoParam::GetEventSpecieName(specie))) ; 
      }
    }
    rv = fRecPointsQAList ;
  } else if ( task == IlcQAv1::kESDS ) {
    if ( ! fESDsQAList ) {
      fESDsQAList = new TObjArray *[IlcRecoParam::kNSpecies] ; 
      for (Int_t specie = 0 ; specie < IlcRecoParam::kNSpecies ; specie++) {
        fESDsQAList[specie] = new TObjArray(IlcQAv1::GetMaxQAObj()) ;
        fESDsQAList[specie]->SetName(Form("%s_%s", GetName(), IlcQAv1::GetTaskName(task).Data())); //, IlcRecoParam::GetEventSpecieName(specie))) ; 
      }
    }
    rv = fESDsQAList ;
  }
  return rv ; 
}

//____________________________________________________________________________ 
void IlcQADataMakerRec::Init(IlcQAv1::TASKINDEX_t task, TObjArray ** list, Int_t run, Int_t cycles)
{
  // Intialisation by passing the list of QA data booked elsewhere
  
  InitRecoParams() ;
  fRun = run ;
  if (cycles > 0)
    SetCycle(cycles) ;  
	
  if ( task == IlcQAv1::kRAWS ) {
    fRawsQAList = list ;	 
  } else if ( task == IlcQAv1::kDIGITSR ) {
    fDigitsQAList = list ; 
  } else if ( task == IlcQAv1::kRECPOINTS ) {
    fRecPointsQAList = list ; 
  } else if ( task == IlcQAv1::kESDS ) {
    fESDsQAList = list ; 
  }
}

//____________________________________________________________________________
void IlcQADataMakerRec::InitRecoParams() 
{
  // Get the recoparam form the OCDB 
  if (!fRecoParam) {
    IlcDebug(IlcQAv1::GetQADebugLevel(), Form("Loading reconstruction parameter objects for detector %s", GetName()));
    IlcCDBPath path(GetName(),"Calib","RecoParam");
    IlcCDBEntry *entry=IlcCDBManager::Instance()->Get(path.GetPath());    
    if(!entry) {
      fRecoParam = NULL ; 
      IlcDebug(IlcQAv1::GetQADebugLevel(), Form("Couldn't find RecoParam entry in OCDB for detector %s",GetName()));
    }
    else {
      //      entry->SetOwner(kTRUE);
      TObject * recoParamObj = entry->GetObject() ; 
      if ( strcmp(recoParamObj->ClassName(), "TObjArray") == 0 ) {
        // The detector has only one set of reco parameters
        IlcDebug(IlcQAv1::GetQADebugLevel(), Form("Array of reconstruction parameters found for detector %s",GetName()));
        TObjArray *recoParamArray = static_cast<TObjArray*>(recoParamObj) ;
	//        recoParamArray->SetOwner(kTRUE);
        for (Int_t iRP=0; iRP<recoParamArray->GetEntriesFast(); iRP++) {
          fRecoParam = static_cast<IlcDetectorRecoParam*>(recoParamArray->At(iRP)) ;
          if (!fRecoParam) 
            break ; 
          else if (fRecoParam->IsDefault()) 
            break ; 
        }
      }
      else if (recoParamObj->InheritsFrom("IlcDetectorRecoParam")) {
        // The detector has only one set of reco parameters
        // Registering it in IlcRecoParam
        IlcDebug(IlcQAv1::GetQADebugLevel(), Form("Single set of reconstruction parameters found for detector %s",GetName()));
        fRecoParam = static_cast<IlcDetectorRecoParam*>(recoParamObj) ;
        static_cast<IlcDetectorRecoParam*>(recoParamObj)->SetAsDefault();
      } else { 
        IlcError(Form("No valid RecoParam object found in the OCDB for detector %s",GetName()));
      }
    }
  }
}

//____________________________________________________________________________ 
void IlcQADataMakerRec::ResetDetector(IlcQAv1::TASKINDEX_t task)
{
  // default reset that resets all the QA objects.
  // to be overloaded by detectors, if necessary

  TObjArray ** list = NULL ; 
  if ( task == IlcQAv1::kRAWS ) {
    list = fRawsQAList ;	 
  } else if ( task == IlcQAv1::kDIGITSR ) {
    list = fDigitsQAList ; 
  } else if ( task == IlcQAv1::kRECPOINTS ) {
    list = fRecPointsQAList ; 
  } else if ( task == IlcQAv1::kESDS ) {
    list = fESDsQAList ; 
  }
  //list was not initialized, skip
  if (!list) return; 
  
  for (int spec = 0; spec < IlcRecoParam::kNSpecies; spec++) {
    if (!IlcQAv1::Instance()->IsEventSpecieSet(IlcRecoParam::ConvertIndex(spec))) continue;
    TIter next(list[spec]) ; 
    TObject *obj = NULL; 
    while ( (obj = next()) ) {
      if (obj->TestBit(IlcQAv1::GetClonedBit())) { // this is array of cloned histos
	TObjArray* arr = (TObjArray*)obj;
	for (int ih=arr->GetEntriesFast();ih--;) {
	  TH1* histo = (TH1*)arr->At(ih); 
	  if (!histo) continue;
	  histo->Reset("ICE");
	  histo->ResetStats();
	}
      }
      else {
	((TH1*)obj)->Reset("ICE");
	((TH1*)obj)->ResetStats();
      }
    }
    ResetEvCountCycle(IlcRecoParam::ConvertIndex(spec));
    ResetEvCountTotal(IlcRecoParam::ConvertIndex(spec));
  }
}

//____________________________________________________________________________
void IlcQADataMakerRec::StartOfCycle(Int_t run) 
{
  // Finishes a cycle of QA for all the tasks
  Bool_t samecycle = kFALSE ; 
  StartOfCycle(IlcQAv1::kRAWS,      run, samecycle) ;
  samecycle = kTRUE ; 
  StartOfCycle(IlcQAv1::kDIGITSR,   run, samecycle) ; 
  StartOfCycle(IlcQAv1::kRECPOINTS, run, samecycle) ; 
  StartOfCycle(IlcQAv1::kESDS,      run, samecycle) ;
}

//____________________________________________________________________________
void IlcQADataMakerRec::StartOfCycle(IlcQAv1::TASKINDEX_t task, Int_t run, const Bool_t sameCycle) 
{ 
  // Finishes a cycle of QA data acquistion
  if ( run > 0 ) fRun = run ; 
  if ( !sameCycle || fCurrentCycle == -1) {
    ResetCycle() ;
    if (fOutput) 
      fOutput->Close() ; 
    if (IlcQAManager::QAManager(IlcQAv1::kRECMODE)->IsSaveData())
      fOutput = IlcQAv1::GetQADataFile(GetName(), fRun) ; 	
  }	
  IlcDebug(IlcQAv1::GetQADebugLevel(), Form(" Run %d Cycle %d task %s file %s", 
					    fRun, fCurrentCycle, IlcQAv1::GetTaskName(task).Data(), fOutput->GetName() )) ;

  //	fDetectorDir = fOutput->GetDirectory(GetDetectorDirName()) ; 
  //	if (!fDetectorDir)
  //		fDetectorDir = fOutput->mkdir(GetDetectorDirName()) ; 
  //  
  //	TDirectory * subDir = fDetectorDir->GetDirectory(IlcQAv1::GetTaskName(task)) ; 
  //	if (!subDir)
  //		subDir = fDetectorDir->mkdir(IlcQAv1::GetTaskName(task)) ;  
  //  
  //  for ( Int_t specie = IlcRecoParam::kDefault ; specie < IlcRecoParam::kNSpecies ; specie++ ) {
  //    TDirectory * eventSpecieDir = subDir->GetDirectory(IlcRecoParam::GetEventSpecieName(specie)) ; 
  //    if (!eventSpecieDir) 
  //      eventSpecieDir = subDir->mkdir(IlcRecoParam::GetEventSpecieName(specie)) ; 
  //    TDirectory * expertDir = eventSpecieDir->GetDirectory(IlcQAv1::GetExpert()) ; 
  //    if (!expertDir)
  //      expertDir = eventSpecieDir->mkdir(IlcQAv1::GetExpert()) ; 
  //  } 
  StartOfDetectorCycle();
  ResetEvCountCycle();
}

//____________________________________________________________________________
void IlcQADataMakerRec::ClonePerTrigClass(IlcQAv1::TASKINDEX_t task)
{
  // clone the histos of the array corresponding to task
  switch (task) 
    {
    case IlcQAv1::kRAWS      : ClonePerTrigClassL(fRawsQAList, task);      break;
    case IlcQAv1::kDIGITS    : ClonePerTrigClassL(fDigitsQAList, task);    break;
    case IlcQAv1::kRECPOINTS : ClonePerTrigClassL(fRecPointsQAList, task); break;
    case IlcQAv1::kESDS      : ClonePerTrigClassL(fESDsQAList, task);      break;
    default : IlcError(Form("Task %s is invalid in this context", IlcQAv1::GetTaskName(task).Data() )); break;
    }
  //
}

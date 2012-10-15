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


/* $Id: IlcQADataMakerSim.cxx 50615 2011-07-16 23:19:19Z hristov $ */

//
//  Base Class
//  Produces the data needed to calculate the quality assurance. 
//  All data must be mergeable objects.
//  Y. Schutz CERN July 2007
//

// --- ROOT system ---
#include <TFile.h>
#include <TTree.h>
#include <TClonesArray.h>

// --- Standard library ---

// --- IlcRoot header files ---
#include "IlcLog.h"
#include "IlcQADataMakerSim.h"

ClassImp(IlcQADataMakerSim)
             
//____________________________________________________________________________ 
IlcQADataMakerSim::IlcQADataMakerSim(const char * name, const char * title) : 
  IlcQADataMaker(name, title), 
  fDigitsQAList(NULL), 
  fHitsQAList(NULL),
  fSDigitsQAList(NULL),  
  fHitsArray(NULL),
  fSDigitsArray(NULL)
{
	// ctor
	fDetectorDirName = GetName() ; 
}

//____________________________________________________________________________ 
IlcQADataMakerSim::IlcQADataMakerSim(const IlcQADataMakerSim& qadm) :
  IlcQADataMaker(qadm.GetName(), qadm.GetTitle()), 
  fDigitsQAList(qadm.fDigitsQAList),
  fHitsQAList(qadm.fHitsQAList),
  fSDigitsQAList(qadm.fSDigitsQAList),  
  fHitsArray(NULL),
  fSDigitsArray(NULL)
{
  //copy ctor
  fDetectorDirName = GetName() ; 
}

//____________________________________________________________________________ 
IlcQADataMakerSim::~IlcQADataMakerSim()
{
	//dtor: delete the TObjArray and thei content
	if ( fDigitsQAList ) { 
    for (Int_t specie = 0 ; specie < IlcRecoParam::kNSpecies ; specie++) {
			fDigitsQAList[specie]->Delete() ;
    }
		delete[] fDigitsQAList ;
  }
	if ( fHitsQAList ) {
    for (Int_t specie = 0 ; specie < IlcRecoParam::kNSpecies ; specie++) {
			fHitsQAList[specie]->Delete() ;
    }
   	delete[] fHitsQAList ;
  }
	if ( fSDigitsQAList ) { 
    for (Int_t specie = 0 ; specie < IlcRecoParam::kNSpecies ; specie++) {
			fSDigitsQAList[specie]->Delete() ; 
    }
 		delete[] fSDigitsQAList ;
  }
  if (fHitsArray) {
    fHitsArray->Clear() ; 
    delete fHitsArray ;
  }
  if (fSDigitsArray) {
    fSDigitsArray->Clear() ; 
    delete fSDigitsArray ;
  }  
}

//__________________________________________________________________
IlcQADataMakerSim& IlcQADataMakerSim::operator = (const IlcQADataMakerSim& qadm )
{
  // Assignment operator.
  this->~IlcQADataMakerSim();
  new(this) IlcQADataMakerSim(qadm);
  return *this;
}

//____________________________________________________________________________
void IlcQADataMakerSim::EndOfCycle() 
{ 
  // Finishes a cycle of QA for all tasks
  EndOfCycle(IlcQAv1::kHITS) ; 
  EndOfCycle(IlcQAv1::kSDIGITS) ; 
  EndOfCycle(IlcQAv1::kDIGITS) ;
  ResetCycle() ; 
}

//____________________________________________________________________________
void IlcQADataMakerSim::EndOfCycle(IlcQAv1::TASKINDEX_t task) 
{ 
  // Finishes a cycle of QA data acquistion
	TObjArray ** list = NULL ; 
	
	if ( task == IlcQAv1::kHITS ) 
		list = fHitsQAList ; 
	else if ( task == IlcQAv1::kSDIGITS )
		list = fSDigitsQAList ; 
	else if ( task == IlcQAv1::kDIGITS ) 
		list = fDigitsQAList ; 
  
  if ( ! list ) 
    return ; 
	EndOfDetectorCycle(task, list) ; 
  fDetectorDir = fOutput->GetDirectory(GetDetectorDirName()) ;
	if (!fDetectorDir) 
    fDetectorDir = fOutput->mkdir(GetDetectorDirName()) ; 
  TDirectory * subDir = fDetectorDir->GetDirectory(IlcQAv1::GetTaskName(task)) ; 
  if (!subDir)
    subDir = fDetectorDir->mkdir(IlcQAv1::GetTaskName(task)) ;  
  subDir->cd() ; 
  for (Int_t specie = 0 ; specie < IlcRecoParam::kNSpecies ; specie++) {
    if (! IlcQAv1::Instance(IlcQAv1::GetDetIndex(GetName()))->IsEventSpecieSet(IlcRecoParam::ConvertIndex(specie)) ) 
      continue ;
    if (list[specie]->GetEntries() != 0 ) {
      TDirectory * eventSpecieDir = subDir->GetDirectory(IlcRecoParam::GetEventSpecieName(specie)) ;
      if (!eventSpecieDir) 
        eventSpecieDir = subDir->mkdir(IlcRecoParam::GetEventSpecieName(specie)) ; 
      eventSpecieDir->cd() ; 
      TIter next(list[specie]) ; 
      TObject * obj ; 
      while ( (obj = next()) )  {
        if (!obj->TestBit(IlcQAv1::GetExpertBit()))
          obj->Write() ;
      }
      if (WriteExpert()) {
        TDirectory * expertDir = eventSpecieDir->GetDirectory(IlcQAv1::GetExpert()) ; 
        if (!expertDir) 
          expertDir = eventSpecieDir->mkdir(IlcQAv1::GetExpert()) ; 
        expertDir->cd() ;
        next.Reset() ; 
        while ( (obj = next()) ) {
          if (!obj->TestBit(IlcQAv1::GetExpertBit()))
            continue ; 
          obj->Write() ;
        }      
      }
    }
    fOutput->Save() ; 
  }
}

//____________________________________________________________________________
void IlcQADataMakerSim::Exec(IlcQAv1::TASKINDEX_t task, TObject * data) 
{ 
  // creates the quality assurance data for the various tasks (Hits, SDigits, Digits, ESDs)
  
	if ( task == IlcQAv1::kHITS ) {  
		IlcDebug(IlcQAv1::GetQADebugLevel(), "Processing Hits QA") ; 
 		if (strcmp(data->ClassName(), "TClonesArray") == 0) { 
      fHitsArray = static_cast<TClonesArray *>(data) ; 
			MakeHits() ;
		} else if (strcmp(data->ClassName(), "TTree") == 0) {
			TTree * tree = static_cast<TTree *>(data) ; 
      MakeHits(tree) ; 
    } else {
      IlcWarning("data are neither a TClonesArray nor a TTree") ; 
    }
	} else if ( task == IlcQAv1::kSDIGITS ) {
		IlcDebug(IlcQAv1::GetQADebugLevel(), "Processing SDigits QA") ; 
		if (strcmp(data->ClassName(), "TClonesArray") == 0) { 
      fSDigitsArray = static_cast<TClonesArray *>(data) ; 
			MakeSDigits() ;
		} else if (strcmp(data->ClassName(), "TTree") == 0) {
			TTree * tree = static_cast<TTree *>(data) ; 
      MakeSDigits(tree) ; 
    } else {
      IlcWarning("data are neither a TClonesArray nor a TTree") ; 
    }
 	} else if ( task == IlcQAv1::kDIGITS ) {
		IlcDebug(IlcQAv1::GetQADebugLevel(), "Processing Digits QA") ; 
		if (strcmp(data->ClassName(), "TClonesArray") == 0) { 
      fDigitsArray = static_cast<TClonesArray *>(data) ; 
			MakeDigits() ;
		} else if (strcmp(data->ClassName(), "TTree") == 0)  {
			TTree * tree = static_cast<TTree *>(data) ; 
      MakeDigits(tree) ; 
    } else {
      IlcWarning("data are neither a TClonesArray nor a TTree") ; 
    }
  }
}

//____________________________________________________________________________ 
TObjArray **  IlcQADataMakerSim::Init(IlcQAv1::TASKINDEX_t task, Int_t cycles)
{
  // general intialisation
	
	if (cycles > 0)
		SetCycle(cycles) ;  
	TObjArray ** rv = NULL ; 
	if ( task == IlcQAv1::kHITS ) {
		if ( ! fHitsQAList ) {
      fHitsQAList = new TObjArray *[IlcRecoParam::kNSpecies] ; 
      for (Int_t specie = 0 ; specie < IlcRecoParam::kNSpecies ; specie++) {
        fHitsQAList[specie] = new TObjArray(IlcQAv1::GetMaxQAObj()) ;	 
        fHitsQAList[specie]->SetName(Form("%s_%s_%s", GetName(), IlcQAv1::GetTaskName(task).Data(), IlcRecoParam::GetEventSpecieName(specie))) ;
      }
		}
		rv = fHitsQAList ;
	} else if ( task == IlcQAv1::kSDIGITS ) {
		if ( ! fSDigitsQAList ) {
      fSDigitsQAList = new TObjArray *[IlcRecoParam::kNSpecies] ; 
      for (Int_t specie = 0 ; specie < IlcRecoParam::kNSpecies ; specie++) {
        fSDigitsQAList[specie] = new TObjArray(IlcQAv1::GetMaxQAObj()) ; 
        fSDigitsQAList[specie]->SetName(Form("%s_%s_%s", GetName(), IlcQAv1::GetTaskName(task).Data(), IlcRecoParam::GetEventSpecieName(specie))) ; 
      }
		}
		rv = fSDigitsQAList ;
   } else if ( task == IlcQAv1::kDIGITS ) {
	   if ( ! fDigitsQAList ) {
       fDigitsQAList = new TObjArray *[IlcRecoParam::kNSpecies] ; 
       for (Int_t specie = 0 ; specie < IlcRecoParam::kNSpecies ; specie++) {    
         fDigitsQAList[specie] = new TObjArray(IlcQAv1::GetMaxQAObj()) ;
         fDigitsQAList[specie]->SetName(Form("%s_%s_%s", GetName(), IlcQAv1::GetTaskName(task).Data(), IlcRecoParam::GetEventSpecieName(specie))) ;
       }
	   }
	   rv =  fDigitsQAList ;
   }
  
	return rv ; 
} 

//____________________________________________________________________________ 
void IlcQADataMakerSim::Init(IlcQAv1::TASKINDEX_t task, TObjArray ** list, Int_t run, Int_t cycles)
{
  // Intialisation by passing the list of QA data booked elsewhere
  
	fRun = run ;
	if (cycles > 0)
		SetCycle(cycles) ;  
	
	if ( task == IlcQAv1::kHITS ) {
		fHitsQAList = list ;	 
	} else if ( task == IlcQAv1::kSDIGITS) {
		fSDigitsQAList = list ; 
	} else if ( task == IlcQAv1::kDIGITS ) {
		fDigitsQAList = list ; 
	} 
}

//____________________________________________________________________________ 
void IlcQADataMakerSim::ResetDetector(IlcQAv1::TASKINDEX_t task)
{
    // default reset that resets all the QA objects.
    // to be overloaded by detectors, if necessary
  
  TObjArray ** list = NULL ; 
  if ( task == IlcQAv1::kHITS ) {
		list = fHitsQAList ;	 
	} else if ( task == IlcQAv1::kSDIGITS ) {
		list = fSDigitsQAList ; 
	} else if ( task == IlcQAv1::kDIGITS ) {
		list = fDigitsQAList ; 
	}
    //list was not initialized, skip
  if (!list) 
    return ; 
  
  for (int spec = 0; spec < IlcRecoParam::kNSpecies; spec++) {
    if (!IlcQAv1::Instance()->IsEventSpecieSet(IlcRecoParam::ConvertIndex(spec)))
      continue;
    TIter next(list[spec]) ; 
    TH1 * histo = NULL ; 
    while ( (histo = dynamic_cast<TH1*> (next())) ) {
      histo->Reset() ;
    }
  }
}
  
//____________________________________________________________________________
void IlcQADataMakerSim::StartOfCycle(Int_t run) 
{ 
  // Finishes a cycle of QA for all tasks
  Bool_t samecycle = kFALSE ; 
  StartOfCycle(IlcQAv1::kHITS,    run, samecycle) ;
  samecycle = kTRUE ; 
  StartOfCycle(IlcQAv1::kSDIGITS, run, samecycle) ;
  StartOfCycle(IlcQAv1::kDIGITS,  run, samecycle) ;
}

//____________________________________________________________________________
void IlcQADataMakerSim::StartOfCycle(IlcQAv1::TASKINDEX_t task, Int_t run, const Bool_t sameCycle) 
{ 
  // Finishes a cycle of QA data acquistion
  if ( run > 0 ) 
    fRun = run ; 
	if ( !sameCycle || fCurrentCycle == -1) {
		ResetCycle() ;
	if (fOutput) 
		fOutput->Close() ; 
	fOutput = IlcQAv1::GetQADataFile(GetName(), fRun) ; 	
	}	

	IlcDebug(IlcQAv1::GetQADebugLevel(), Form(" Run %d Cycle %d task %s file %s", 
				 fRun, fCurrentCycle, IlcQAv1::GetTaskName(task).Data(), fOutput->GetName() )) ;

	//fDetectorDir = fOutput->GetDirectory(GetDetectorDirName()) ; 
//	if (!fDetectorDir)
//		fDetectorDir = fOutput->mkdir(GetDetectorDirName()) ; 
//
//	TDirectory * subDir = fDetectorDir->GetDirectory(IlcQAv1::GetTaskName(task)) ; 
//	if (!subDir)
//		subDir = fDetectorDir->mkdir(IlcQAv1::GetTaskName(task)) ;  
//  
//  for ( Int_t index = IlcRecoParam::kDefault ; index < IlcRecoParam::kNSpecies ; index++ ) {
//    TDirectory * eventSpecieDir = subDir->GetDirectory(IlcRecoParam::GetEventSpecieName(index)) ; 
//    if (!eventSpecieDir) 
//      eventSpecieDir = subDir->mkdir(IlcRecoParam::GetEventSpecieName(index)) ; 
//    TDirectory * expertDir = eventSpecieDir->GetDirectory(IlcQAv1::GetExpert()) ; 
//    if (!expertDir) 
//      expertDir = eventSpecieDir->mkdir(IlcQAv1::GetExpert()) ; 
//   }   
	StartOfDetectorCycle() ; 
}


//____________________________________________________________________________
void IlcQADataMakerSim::ClonePerTrigClass(IlcQAv1::TASKINDEX_t task)
{
  // clone the histos of the array corresponding to task
  switch (task) 
    {
    case IlcQAv1::kHITS      : ClonePerTrigClassL(fHitsQAList, task);      break;
    case IlcQAv1::kSDIGITS   : ClonePerTrigClassL(fSDigitsQAList, task);   break;
    case IlcQAv1::kDIGITS    : ClonePerTrigClassL(fDigitsQAList, task);    break;
    default : IlcError(Form("Task %s is invalid in this context", IlcQAv1::GetTaskName(task).Data() )); break;
    }
  //
}

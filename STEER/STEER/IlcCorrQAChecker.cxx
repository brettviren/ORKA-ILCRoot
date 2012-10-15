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


/* $Id: $ */

/*
  Checks the quality assurance. 
  By comparing with reference data
  Y. Schutz CERN July 2007
*/

// --- ROOT system ---
#include <TClass.h>
#include <TH1F.h> 
#include <TH1I.h> 
#include <TIterator.h> 
#include <TKey.h> 
#include <TFile.h> 
#include <TNtupleD.h>

// --- Standard library ---

// --- IlcRoot header files ---
#include "IlcLog.h"
#include "IlcQAv1.h"
#include "IlcQAChecker.h"
#include "IlcCorrQAChecker.h"

ClassImp(IlcCorrQAChecker)

//__________________________________________________________________
Double_t * IlcCorrQAChecker::CheckN(IlcQAv1::ILCTASK_t index, TNtupleD ** nData, IlcDetectorRecoParam * /*recoParam*/) 
{
 // check the QA of correlated data stored in a ntuple
  
  Double_t * test = new Double_t[IlcRecoParam::kNSpecies] ; 
  for (Int_t specie = 0 ; specie < IlcRecoParam::kNSpecies ; specie++) 
    test[specie] = 0. ; 
    
  if ( index != IlcQAv1::kRAW ) {
    IlcWarning("Checker not implemented") ; 
    for (Int_t specie = 0 ; specie < IlcRecoParam::kNSpecies ; specie++) 
      test[specie] = 1. ; 
    return test ; 
  }
//	if (!fRefSubDir) {
//		test = 1. ; // no reference data
//	} else {
    if ( ! nData ) {
      IlcError(Form("nRawCorr not found in %s", fDataSubDir->GetName())) ; 
    } else {
      for (Int_t specie = 0 ; specie < IlcRecoParam::kNSpecies ; specie++) {
        if ( !IlcQAv1::Instance()->IsEventSpecieSet(specie) ) 
          continue ; 
        TObjArray * bList = nData[specie]->GetListOfBranches() ; 
        for (Int_t b = 0 ; b < bList->GetEntries() ; b++) {
          IlcInfo(Form("Ntuple %s parameter name %d : %s", nData[specie]->GetName(), b, bList->At(b)->GetName())) ;  
        }
      }
    }
 // }
  return test ; 
}
//__________________________________________________________________
void   IlcCorrQAChecker::Run(IlcQAv1::ILCTASK_t tsk, TNtupleD ** nt, IlcDetectorRecoParam * recoParam) 
{
    // special run for TNtupleD
	IlcDebug(IlcQAv1::GetQADebugLevel(), Form("Processing %s", IlcQAv1::GetIlcTaskName(tsk))) ; 
  
	Double_t * rv = NULL ;
  rv = CheckN(tsk, nt, recoParam) ;
	SetQA(tsk, rv) ; 	
	
  IlcDebug(IlcQAv1::GetQADebugLevel(), Form("Test result of %s", IlcQAv1::GetIlcTaskName(tsk))) ;
	
  if (rv) 
    delete [] rv ; 
  Finish() ; 
} 


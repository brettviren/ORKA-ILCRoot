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


/* $Id: IlcPVECQAChecker.cxx 56633 2012-05-22 22:14:15Z hristov $ */

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

// --- Standard library ---

// --- IlcRoot header files ---
#include "IlcLog.h"
#include "IlcQAv1.h"
#include "IlcQAChecker.h"
#include "IlcPVECQAChecker.h"

ClassImp(IlcPVECQAChecker)

//__________________________________________________________________

IlcPVECQAChecker & IlcPVECQAChecker::operator = (const IlcPVECQAChecker &)
{
  Fatal("operator =", "not implemented");
  return *this;
}

//____________________________________________________________________________
void IlcPVECQAChecker::Check(Double_t * test, IlcQAv1::ILCTASK_t task, TObjArray ** list, const IlcDetectorRecoParam * /* recoParam */) 
{
  // Performs a basic checking
  // Compares all the histograms in the list

  for (Int_t specie = 0 ; specie < IlcRecoParam::kNSpecies ; specie++) {
    test[specie] = 1.0;
    if ( !IlcQAv1::Instance()->IsEventSpecieSet(specie) ) 
      continue ; 
    // checking for empty histograms
    // if (CheckEntries(list[specie]) == 0)  {
    //   IlcWarning("histograms are empty");
    //   test[specie] = 0.4;//-> Corresponds to kWARNING see IlcQACheckerBase::Run
    // }
  
    // checking raw data
    if(task == IlcQAv1::kRAW){
      if(IlcRecoParam::ConvertIndex(specie) == IlcRecoParam::kCalib    ||
         IlcRecoParam::ConvertIndex(specie) == IlcRecoParam::kHighMult ||
         IlcRecoParam::ConvertIndex(specie) == IlcRecoParam::kLowMult  ||
	 IlcRecoParam::ConvertIndex(specie) == IlcRecoParam::kDefault) {
	// list[specie]->Print();
	TH1F *hHighNtot = (TH1F*)list[specie]->At(13);
	if (hHighNtot!=0) {
	  if (hHighNtot->GetMean() < 1000) test[specie]=1;
	}
	else test[specie]=0.1;
      }
    }

    //default check response. It will be changed when reasonable checks will be considered
    else test[specie] = 0.7 ; // /-> Corresponds to kINFO see IlcQACheckerBase::Run 
  } // species loop
}

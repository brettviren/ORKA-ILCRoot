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

/* $Id: IlcAnalysisFilter.cxx 53615 2011-12-15 15:52:21Z agheata $ */

//
// Manager class for filter decisions based on cuts
// The filter contains a list of sets of cuts.
// A bit field is filled in order to store the decision of each cut-set. 
// Author: Andreas Morsch
// andreas.morsch@cern.ch

#include <TObject.h>
#include <TList.h>
#include "IlcAnalysisFilter.h"
#include "IlcAnalysisCuts.h"


ClassImp(IlcAnalysisFilter)


////////////////////////////////////////////////////////////////////////

IlcAnalysisFilter::IlcAnalysisFilter():
    TNamed(),
    fCuts(0)
{
  // Default constructor
}

IlcAnalysisFilter::IlcAnalysisFilter(const char* name, const char* title):
    TNamed(name, title),
    fCuts(new TList())
{
  // Constructor
}

IlcAnalysisFilter::IlcAnalysisFilter(const IlcAnalysisFilter& obj):
    TNamed(obj),
    fCuts(0)
{
// Copy constructor
    fCuts = obj.fCuts;
}

IlcAnalysisFilter::~IlcAnalysisFilter()
{
// Destructor
   if (fCuts) fCuts->Delete("slow");
   delete fCuts;
}   

IlcAnalysisFilter& IlcAnalysisFilter::operator=(const IlcAnalysisFilter& other)
{
// Assignment
   if (&other != this) {
	   TNamed::operator=(other);
	   fCuts = other.fCuts;
   }
   return *this;
   }
   
UInt_t IlcAnalysisFilter::IsSelected(TObject* obj)
{
    //
    // Loop over all set of cuts
    // and store the decision
    UInt_t result = 0;
    UInt_t filterMask;
    
    TIter next(fCuts);
    IlcAnalysisCuts *cuts;
    Int_t iCutB = 1;
	
    while((cuts = (IlcAnalysisCuts*)next())) {
	Bool_t acc = cuts->IsSelected(obj);
	if ((filterMask = cuts->GetFilterMask()) > 0) {
	    acc = (acc && (filterMask == result));
	}
	cuts->SetSelected(acc);
	if (acc) {result |= iCutB & 0x00ffffff;}
	iCutB *= 2;
    }  

    return result;
}

UInt_t IlcAnalysisFilter::IsSelected(TList* list)
{
    //
    // Loop over all set of cuts
    // and store the decision
    UInt_t result = 0;
    UInt_t filterMask;

    TIter next(fCuts);
    IlcAnalysisCuts *cuts;
    Int_t iCutB = 1;
	
    while((cuts = (IlcAnalysisCuts*)next())) {
	Bool_t acc = cuts->IsSelected(list);
	if ((filterMask = cuts->GetFilterMask()) > 0) {
	    acc = (acc && (filterMask & result));
	}
	cuts->SetSelected(acc);
	if (acc) {result |= iCutB & 0x00ffffff;}
	iCutB *= 2;
    }  

    return result;
}

void IlcAnalysisFilter::Init()
{
    //
    // Loop over all set of cuts and call Init
    TIter next(fCuts);
    IlcAnalysisCuts *cuts;
    while((cuts = (IlcAnalysisCuts*)next())) cuts->Init();
}

void IlcAnalysisFilter::AddCuts(IlcAnalysisCuts* cuts)
{
    // Add a set of cuts
    fCuts->Add(cuts);
}

Bool_t IlcAnalysisFilter::IsSelected(char* name)
{
    //
    // Returns current result for cut with name
    IlcAnalysisCuts* cut = (IlcAnalysisCuts*) (fCuts->FindObject(name));
    if (cut) {
      return (cut->Selected());
    } else  {
      return 0;
    }
}

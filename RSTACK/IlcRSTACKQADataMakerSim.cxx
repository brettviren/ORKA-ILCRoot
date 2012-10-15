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


/* $Id: IlcRSTACKQADataMakerSim.cxx 50396 2011-07-04 07:46:08Z hristov $ */

/*
  Produces the data needed to calculate the quality assurance. 
  All data must be mergeable objects.
  Y. Schutz CERN July 2007
*/

// --- ROOT system ---
#include <TClonesArray.h>
#include <TFile.h> 
#include <TH1F.h> 
#include <TH1I.h> 
#include <TH2F.h> 
#include <TTree.h>

// --- Standard library ---

// --- IlcRoot header files ---
#include "IlcESDCaloCluster.h"
#include "IlcLog.h"
#include "IlcRSTACKDigit.h"
#include "IlcRSTACKHit.h"
#include "IlcRSTACKQADataMakerSim.h"
#include "IlcQAChecker.h"

ClassImp(IlcRSTACKQADataMakerSim)
           
//____________________________________________________________________________ 
IlcRSTACKQADataMakerSim::IlcRSTACKQADataMakerSim() : 
  IlcQADataMakerSim(IlcQAv1::GetDetName(IlcQAv1::kRSTACK), "RSTACK Quality Assurance Data Maker")
{
  // ctor
}

//____________________________________________________________________________ 
IlcRSTACKQADataMakerSim::IlcRSTACKQADataMakerSim(const IlcRSTACKQADataMakerSim& qadm) :
  IlcQADataMakerSim()
{
  //copy ctor 
  SetName((const char*)qadm.GetName()) ; 
  SetTitle((const char*)qadm.GetTitle()); 
}

//__________________________________________________________________
IlcRSTACKQADataMakerSim& IlcRSTACKQADataMakerSim::operator = (const IlcRSTACKQADataMakerSim& qadm )
{
  // Assign operator.
  this->~IlcRSTACKQADataMakerSim();
  new(this) IlcRSTACKQADataMakerSim(qadm);
  return *this;
}
 
//____________________________________________________________________________ 
void IlcRSTACKQADataMakerSim::EndOfDetectorCycle(IlcQAv1::TASKINDEX_t task, TObjArray ** list)
{
  //Detector specific actions at end of cycle
  // do the QA checking
  IlcQAChecker::Instance()->Run(IlcQAv1::kRSTACK, task, list) ;  
}

//____________________________________________________________________________ 
void IlcRSTACKQADataMakerSim::InitHits()
{
  // create Hits histograms in Hits subdir
  const Bool_t expert   = kTRUE ; 
  const Bool_t image    = kTRUE ; 
  TH1F * h0 = new TH1F("hRSTACKHits",    "Hits energy distribution in RSTACK;Energy [MeV];Counts",       100, 0., 100.) ; 
  h0->Sumw2() ;
  Add2HitsList(h0, kHits, !expert, image) ;
  TH1I * h1 = new TH1I("hRSTACKHitsMul", "Hits multiplicity distribution in RSTACK;# of Hits;Entries", 500, 0., 10000) ; 
  h1->Sumw2() ;
  Add2HitsList(h1, kHitsMul, !expert, image) ;
  //
  ClonePerTrigClass(IlcQAv1::kHITS); // this should be the last line  
}

//____________________________________________________________________________ 
void IlcRSTACKQADataMakerSim::InitDigits()
{
  // create Digits histograms in Digits subdir
  const Bool_t expert   = kTRUE ; 
  const Bool_t image    = kTRUE ; 
  TH1I * h0 = new TH1I("hRSTACKDigits",    "Digits amplitude distribution in RSTACK;Amplitude [ADC counts];Counts",    500, 0, 1000) ; 
  h0->Sumw2() ;
  Add2DigitsList(h0, kDigits, !expert, image) ;
  TH1I * h1 = new TH1I("hRSTACKDigitsMul", "Digits multiplicity distribution in RSTACK;# of Digits;Entries", 2000, 0, 10000) ; 
  h1->Sumw2() ;
  Add2DigitsList(h1, kDigitsMul, !expert, image) ;
  //
  ClonePerTrigClass(IlcQAv1::kDIGITS); // this should be the last line
}

//____________________________________________________________________________ 
void IlcRSTACKQADataMakerSim::InitSDigits()
{
  // create SDigits histograms in SDigits subdir
  const Bool_t expert   = kTRUE ; 
  const Bool_t image    = kTRUE ; 
  TH1F * h0 = new TH1F("hRSTACKSDigits",    "SDigits energy distribution in RSTACK; Energy [MeV];Counts",       500, 0., 1000.) ; 
  h0->Sumw2() ;
  Add2SDigitsList(h0, kSDigits, !expert, image) ;
  TH1I * h1 = new TH1I("hRSTACKSDigitsMul", "SDigits multiplicity distribution in RSTACK;# of SDigits;Entries", 500, 0,  1000) ; 
  h1->Sumw2() ;
  Add2SDigitsList(h1, kSDigitsMul, !expert, image) ;
  //
  ClonePerTrigClass(IlcQAv1::kSDIGITS); // this should be the last line
}

//____________________________________________________________________________
void IlcRSTACKQADataMakerSim::MakeHits()
{
  //make QA data from Hits
  
  TIter next(fHitsArray) ; 
  IlcRSTACKHit * hit ; 
  while ( (hit = dynamic_cast<IlcRSTACKHit *>(next())) ) {
    FillHitsData(kHits, hit->GetEnergy()) ;
  }
}

//____________________________________________________________________________
void IlcRSTACKQADataMakerSim::MakeHits(TTree * hitTree)
{
  // make QA data from Hit Tree
  
  if (fHitsArray)
    fHitsArray->Clear() ; 
  else
    fHitsArray = new TClonesArray("IlcRSTACKHit", 1000);

  TBranch * branch = hitTree->GetBranch("RSTACK") ;
  if ( ! branch ) { IlcWarning("RSTACK branch in Hit Tree not found"); return;}
  //
  Int_t nHits = 0;
  branch->SetAddress(&fHitsArray) ;
  for (Int_t ientry = 0 ; ientry < branch->GetEntries() ; ientry++) {
    branch->GetEntry(ientry) ;
    nHits += fHitsArray->GetEntriesFast();
    MakeHits() ; 
    fHitsArray->Clear();
  } 	
  FillHitsData(1,nHits) ;
  //  
  IncEvCountCycleHits();
  IncEvCountTotalHits();
  //
}

//____________________________________________________________________________
void IlcRSTACKQADataMakerSim::MakeDigits()
{
  // makes data from Digits
 
  FillDigitsData(1,fDigitsArray->GetEntriesFast()) ; 
  TIter next(fDigitsArray) ; 
  IlcRSTACKDigit * digit ; 
  while ( (digit = dynamic_cast<IlcRSTACKDigit *>(next())) ) {
    FillDigitsData(kDigits, digit->GetEnergy()) ;
  }  
}

//____________________________________________________________________________
void IlcRSTACKQADataMakerSim::MakeDigits(TTree * digitTree)
{
  // makes data from Digit Tree
  if (fDigitsArray) 
    fDigitsArray->Clear() ; 
  else
    fDigitsArray = new TClonesArray("IlcRSTACKDigit", 1000) ; 
  
  TBranch * branch = digitTree->GetBranch("RSTACK") ;
  if ( ! branch ) {IlcWarning("RSTACK branch in Digit Tree not found"); return;}
  branch->SetAddress(&fDigitsArray) ;
  branch->GetEntry(0) ; 
  MakeDigits() ; 
  //
  IncEvCountCycleDigits();
  IncEvCountTotalDigits();
  //
}

//____________________________________________________________________________
void IlcRSTACKQADataMakerSim::MakeSDigits()
{
  // makes data from SDigits

  FillSDigitsData(1,fSDigitsArray->GetEntriesFast()) ; 
  TIter next(fSDigitsArray) ; 
  IlcRSTACKDigit * sdigit ; 
  while ( (sdigit = dynamic_cast<IlcRSTACKDigit *>(next())) ) {
    FillSDigitsData(kSDigits, sdigit->GetEnergy()) ;
  } 
}

//____________________________________________________________________________
void IlcRSTACKQADataMakerSim::MakeSDigits(TTree * sdigitTree)
{
	// makes data from SDigit Tree
  if (fSDigitsArray) 
    fSDigitsArray->Clear() ; 
  else
    fSDigitsArray = new TClonesArray("IlcRSTACKDigit", 1000) ; 
  
  TBranch * branch = sdigitTree->GetBranch("RSTACK") ;
  if ( ! branch ) {IlcWarning("RSTACK branch in SDigit Tree not found"); return;}
  branch->SetAddress(&fSDigitsArray) ;
  branch->GetEntry(0) ;
  MakeSDigits() ; 
  //
  IncEvCountCycleSDigits();
  IncEvCountTotalSDigits();
  //
}

//____________________________________________________________________________ 
void IlcRSTACKQADataMakerSim::StartOfDetectorCycle()
{
  //Detector specific actions at start of cycle
  
}

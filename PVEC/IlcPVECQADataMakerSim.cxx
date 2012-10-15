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


/* $Id: IlcPVECQADataMakerSim.cxx 50396 2011-07-04 07:46:08Z hristov $ */

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
#include "IlcPVECDigit.h"
#include "IlcPVECHit.h"
#include "IlcPVECQADataMakerSim.h"
#include "IlcQAChecker.h"

ClassImp(IlcPVECQADataMakerSim)
           
//____________________________________________________________________________ 
IlcPVECQADataMakerSim::IlcPVECQADataMakerSim() : 
  IlcQADataMakerSim(IlcQAv1::GetDetName(IlcQAv1::kPVEC), "PVEC Quality Assurance Data Maker")
{
  // ctor
}

//____________________________________________________________________________ 
IlcPVECQADataMakerSim::IlcPVECQADataMakerSim(const IlcPVECQADataMakerSim& qadm) :
  IlcQADataMakerSim()
{
  //copy ctor 
  SetName((const char*)qadm.GetName()) ; 
  SetTitle((const char*)qadm.GetTitle()); 
}

//__________________________________________________________________
IlcPVECQADataMakerSim& IlcPVECQADataMakerSim::operator = (const IlcPVECQADataMakerSim& qadm )
{
  // Assign operator.
  this->~IlcPVECQADataMakerSim();
  new(this) IlcPVECQADataMakerSim(qadm);
  return *this;
}
 
//____________________________________________________________________________ 
void IlcPVECQADataMakerSim::EndOfDetectorCycle(IlcQAv1::TASKINDEX_t task, TObjArray ** list)
{
  //Detector specific actions at end of cycle
  // do the QA checking
  IlcQAChecker::Instance()->Run(IlcQAv1::kPVEC, task, list) ;  
}

//____________________________________________________________________________ 
void IlcPVECQADataMakerSim::InitHits()
{
  // create Hits histograms in Hits subdir
  const Bool_t expert   = kTRUE ; 
  const Bool_t image    = kTRUE ; 
  TH1F * h0 = new TH1F("hPVECHits",    "Hits energy distribution in PVEC;Energy [MeV];Counts",       100, 0., 100.) ; 
  h0->Sumw2() ;
  Add2HitsList(h0, kHits, !expert, image) ;
  TH1I * h1 = new TH1I("hPVECHitsMul", "Hits multiplicity distribution in PVEC;# of Hits;Entries", 500, 0., 10000) ; 
  h1->Sumw2() ;
  Add2HitsList(h1, kHitsMul, !expert, image) ;
  //
  ClonePerTrigClass(IlcQAv1::kHITS); // this should be the last line  
}

//____________________________________________________________________________ 
void IlcPVECQADataMakerSim::InitDigits()
{
  // create Digits histograms in Digits subdir
  const Bool_t expert   = kTRUE ; 
  const Bool_t image    = kTRUE ; 
  TH1I * h0 = new TH1I("hPVECDigits",    "Digits amplitude distribution in PVEC;Amplitude [ADC counts];Counts",    500, 0, 1000) ; 
  h0->Sumw2() ;
  Add2DigitsList(h0, kDigits, !expert, image) ;
  TH1I * h1 = new TH1I("hPVECDigitsMul", "Digits multiplicity distribution in PVEC;# of Digits;Entries", 2000, 0, 10000) ; 
  h1->Sumw2() ;
  Add2DigitsList(h1, kDigitsMul, !expert, image) ;
  //
  ClonePerTrigClass(IlcQAv1::kDIGITS); // this should be the last line
}

//____________________________________________________________________________ 
void IlcPVECQADataMakerSim::InitSDigits()
{
  // create SDigits histograms in SDigits subdir
  const Bool_t expert   = kTRUE ; 
  const Bool_t image    = kTRUE ; 
  TH1F * h0 = new TH1F("hPVECSDigits",    "SDigits energy distribution in PVEC; Energy [MeV];Counts",       500, 0., 1000.) ; 
  h0->Sumw2() ;
  Add2SDigitsList(h0, kSDigits, !expert, image) ;
  TH1I * h1 = new TH1I("hPVECSDigitsMul", "SDigits multiplicity distribution in PVEC;# of SDigits;Entries", 500, 0,  1000) ; 
  h1->Sumw2() ;
  Add2SDigitsList(h1, kSDigitsMul, !expert, image) ;
  //
  ClonePerTrigClass(IlcQAv1::kSDIGITS); // this should be the last line
}

//____________________________________________________________________________
void IlcPVECQADataMakerSim::MakeHits()
{
  //make QA data from Hits
  
  TIter next(fHitsArray) ; 
  IlcPVECHit * hit ; 
  while ( (hit = dynamic_cast<IlcPVECHit *>(next())) ) {
    FillHitsData(kHits, hit->GetEnergy()) ;
  }
}

//____________________________________________________________________________
void IlcPVECQADataMakerSim::MakeHits(TTree * hitTree)
{
  // make QA data from Hit Tree
  
  if (fHitsArray)
    fHitsArray->Clear() ; 
  else
    fHitsArray = new TClonesArray("IlcPVECHit", 1000);

  TBranch * branch = hitTree->GetBranch("PVEC") ;
  if ( ! branch ) { IlcWarning("PVEC branch in Hit Tree not found"); return;}
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
void IlcPVECQADataMakerSim::MakeDigits()
{
  // makes data from Digits
 
  FillDigitsData(1,fDigitsArray->GetEntriesFast()) ; 
  TIter next(fDigitsArray) ; 
  IlcPVECDigit * digit ; 
  while ( (digit = dynamic_cast<IlcPVECDigit *>(next())) ) {
    FillDigitsData(kDigits, digit->GetEnergy()) ;
  }  
}

//____________________________________________________________________________
void IlcPVECQADataMakerSim::MakeDigits(TTree * digitTree)
{
  // makes data from Digit Tree
  if (fDigitsArray) 
    fDigitsArray->Clear() ; 
  else
    fDigitsArray = new TClonesArray("IlcPVECDigit", 1000) ; 
  
  TBranch * branch = digitTree->GetBranch("PVEC") ;
  if ( ! branch ) {IlcWarning("PVEC branch in Digit Tree not found"); return;}
  branch->SetAddress(&fDigitsArray) ;
  branch->GetEntry(0) ; 
  MakeDigits() ; 
  //
  IncEvCountCycleDigits();
  IncEvCountTotalDigits();
  //
}

//____________________________________________________________________________
void IlcPVECQADataMakerSim::MakeSDigits()
{
  // makes data from SDigits

  FillSDigitsData(1,fSDigitsArray->GetEntriesFast()) ; 
  TIter next(fSDigitsArray) ; 
  IlcPVECDigit * sdigit ; 
  while ( (sdigit = dynamic_cast<IlcPVECDigit *>(next())) ) {
    FillSDigitsData(kSDigits, sdigit->GetEnergy()) ;
  } 
}

//____________________________________________________________________________
void IlcPVECQADataMakerSim::MakeSDigits(TTree * sdigitTree)
{
	// makes data from SDigit Tree
  if (fSDigitsArray) 
    fSDigitsArray->Clear() ; 
  else
    fSDigitsArray = new TClonesArray("IlcPVECDigit", 1000) ; 
  
  TBranch * branch = sdigitTree->GetBranch("PVEC") ;
  if ( ! branch ) {IlcWarning("PVEC branch in SDigit Tree not found"); return;}
  branch->SetAddress(&fSDigitsArray) ;
  branch->GetEntry(0) ;
  MakeSDigits() ; 
  //
  IncEvCountCycleSDigits();
  IncEvCountTotalSDigits();
  //
}

//____________________________________________________________________________ 
void IlcPVECQADataMakerSim::StartOfDetectorCycle()
{
  //Detector specific actions at start of cycle
  
}

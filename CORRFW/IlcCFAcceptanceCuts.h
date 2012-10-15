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


///////////////////////////////////////////////////////////////////////////
//          ----   CORRECTION FRAMEWORK   ----
// IlcCFAcceptanceCuts implementation
// Class to cut on the number of IlcTrackReference's 
// for each detector
///////////////////////////////////////////////////////////////////////////
// author : R. Vernet (renaud.vernet@cern.ch)
///////////////////////////////////////////////////////////////////////////


#ifndef ILCCFACCEPTANCECUTS_H
#define ILCCFACCEPTANCECUTS_H

#include "IlcCFCutBase.h"

class IlcMCEvent;
class TH1F ;
class TH2F ;
class TBits;

class IlcCFAcceptanceCuts : public IlcCFCutBase
{
 public :
  IlcCFAcceptanceCuts() ;
  IlcCFAcceptanceCuts(const Char_t* name, const Char_t* title) ;
  IlcCFAcceptanceCuts(const IlcCFAcceptanceCuts& c) ;
  IlcCFAcceptanceCuts& operator=(const IlcCFAcceptanceCuts& c) ;
  virtual ~IlcCFAcceptanceCuts() { };
  virtual Bool_t IsSelected(TObject* obj) ;
  virtual Bool_t IsSelected(TList*  /*list*/) {return kTRUE;}
  virtual void   SetMCEventInfo(const TObject* mcInfo) ;
  void SetMinNHitITS (Int_t nhits) {fMinNHitITS=nhits;} 
  void SetMinNHitTPC (Int_t nhits) {fMinNHitTPC=nhits;} 
  void SetMinNHitTRD (Int_t nhits) {fMinNHitTRD=nhits;} 
  void SetMinNHitTOF (Int_t nhits) {fMinNHitTOF=nhits;} 
  void SetMinNHitMUON(Int_t nhits) {fMinNHitMUON=nhits;}

  enum { 
    kCutHitsITS ,
    kCutHitsTPC ,
    kCutHitsTRD ,
    kCutHitsTOF ,
    kCutHitsMUON,
    kNCuts,           // number of single selections
    kNStepQA=2        // number of QA steps (before/after the cuts)
  };

 protected:
  IlcMCEvent *fMCInfo;        // pointer to MC Information
  Int_t       fMinNHitITS ;   // min number of track references in ITS 
  Int_t       fMinNHitTPC ;   // min number of track references in TPC 
  Int_t       fMinNHitTRD ;   // min number of track references in TRD 
  Int_t       fMinNHitTOF ;   // min number of track references in TOF 
  Int_t       fMinNHitMUON ;  // min number of track references in MUON
  
  //QA histos
  TH1F*  fhCutStatistics;		// Histogram: statistics of what cuts the tracks did not survive
  TH2F*  fhCutCorrelation;		// Histogram: 2d statistics plot
  TH1F*  fhQA[kNCuts][kNStepQA];        // QA Histograms
  TBits* fBitmap ; 			// stores single selection decisions
  void SelectionBitMap(TObject* obj);
  void FillHistograms(TObject* obj, Bool_t afterCuts);
  void AddQAHistograms(TList *qaList) ;
  void DefineHistograms();

  ClassDef(IlcCFAcceptanceCuts,1);
};

#endif

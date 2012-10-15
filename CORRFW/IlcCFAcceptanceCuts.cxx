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

#include "IlcLog.h"
#include "IlcMCParticle.h"
#include "IlcCFAcceptanceCuts.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TList.h"
#include "TBits.h"

ClassImp(IlcCFAcceptanceCuts)

//______________________________
IlcCFAcceptanceCuts::IlcCFAcceptanceCuts() : 
  IlcCFCutBase(),
  fMCInfo(0x0),
  fMinNHitITS(0),
  fMinNHitTPC(0),
  fMinNHitTRD(0),
  fMinNHitTOF(0),
  fMinNHitMUON(0),
  fhCutStatistics(0x0),
  fhCutCorrelation(0x0),
  fBitmap(new TBits(0))
{
  //
  //ctor
  //
  for (int i=0; i<kNCuts; i++) for (int j=0; j<kNStepQA; j++) fhQA[i][j]=0x0;
}

//______________________________
IlcCFAcceptanceCuts::IlcCFAcceptanceCuts(const Char_t* name, const Char_t* title) : 
  IlcCFCutBase(name,title),
  fMCInfo(0x0),
  fMinNHitITS(0),
  fMinNHitTPC(0),
  fMinNHitTRD(0),
  fMinNHitTOF(0),
  fMinNHitMUON(0),
  fhCutStatistics(0x0),
  fhCutCorrelation(0x0),
  fBitmap(new TBits(0))
{
  //
  //ctor
  //
  for (int i=0; i<kNCuts; i++) for (int j=0; j<kNStepQA; j++) fhQA[i][j]=0x0;
}

//______________________________
IlcCFAcceptanceCuts::IlcCFAcceptanceCuts(const IlcCFAcceptanceCuts& c) : 
  IlcCFCutBase(c),
  fMCInfo(c.fMCInfo),
  fMinNHitITS(c.fMinNHitITS),
  fMinNHitTPC(c.fMinNHitTPC),
  fMinNHitTRD(c.fMinNHitTRD),
  fMinNHitTOF(c.fMinNHitTOF),
  fMinNHitMUON(c.fMinNHitMUON),
  fhCutStatistics(c.fhCutStatistics),
  fhCutCorrelation(c.fhCutCorrelation),
  fBitmap(c.fBitmap)
{
  //
  //copy ctor
  //
  for (int i=0; i<kNCuts; i++) for (int j=0; j<kNStepQA; j++) fhQA[i][j]=c.fhQA[i][j];
}

//______________________________
IlcCFAcceptanceCuts& IlcCFAcceptanceCuts::operator=(const IlcCFAcceptanceCuts& c)
{
  //
  // Assignment operator
  //
  if (this != &c) {
    IlcCFCutBase::operator=(c) ;
    fMCInfo=c.fMCInfo;
    fMinNHitITS=c.fMinNHitITS;
    fMinNHitTPC=c.fMinNHitTPC;
    fMinNHitTRD=c.fMinNHitTRD;
    fMinNHitTOF=c.fMinNHitTOF;
    fMinNHitMUON=c.fMinNHitMUON;
    fhCutStatistics  = c.fhCutStatistics ;
    fhCutCorrelation = c.fhCutCorrelation ;
    fBitmap          = c.fBitmap ;
    for (int i=0; i<kNCuts; i++) for (int j=0; j<kNStepQA; j++) fhQA[i][j]=c.fhQA[i][j];
  }
  return *this ;
}

//______________________________________________________________
Bool_t IlcCFAcceptanceCuts::IsSelected(TObject *obj) {
  //
  // check if selections on 'obj' are passed
  // 'obj' must be an IlcMCParticle
  //
  
  SelectionBitMap(obj);

  if (fIsQAOn) FillHistograms(obj,kFALSE);

  for (UInt_t icut=0; icut<fBitmap->GetNbits(); icut++)
    if (!fBitmap->TestBitNumber(icut)) return kFALSE ;
  
  if (fIsQAOn) FillHistograms(obj,kTRUE);
  return kTRUE;
}

//______________________________
void IlcCFAcceptanceCuts::SelectionBitMap(TObject* obj) {
  //
  // checks the number of track references associated to 'obj'
  // 'obj' must be an IlcMCParticle
  //

  for (UInt_t i=0; i<kNCuts; i++) fBitmap->SetBitNumber(i,kFALSE);

  if (!obj) return;
  TString className(obj->ClassName());
  if (className.CompareTo("IlcMCParticle") != 0) {
    IlcError("obj must point to an IlcMCParticle !");
    return ;
  }

  IlcMCParticle * part = dynamic_cast<IlcMCParticle*>(obj);
  if(!part) return ;

  Int_t nHitsITS=0, nHitsTPC=0, nHitsTRD=0, nHitsTOF=0, nHitsMUON=0 ;
  for (Int_t iTrackRef=0; iTrackRef<part->GetNumberOfTrackReferences(); iTrackRef++) {
    IlcTrackReference * trackRef = part->GetTrackReference(iTrackRef);
    if(trackRef){
      Int_t detectorId = trackRef->DetectorId();
      switch(detectorId) {
      case IlcTrackReference::kITS  : nHitsITS++  ; break ;
      case IlcTrackReference::kTPC  : nHitsTPC++  ; break ;
      case IlcTrackReference::kTRD  : nHitsTRD++  ; break ;
      case IlcTrackReference::kTOF  : nHitsTOF++  ; break ;
      case IlcTrackReference::kMUON : nHitsMUON++ ; break ;
      default : break ;
      }
    }
  }
  
  Int_t iCutBit = 0;

  if (nHitsITS  >= fMinNHitITS  ) fBitmap->SetBitNumber(iCutBit,kTRUE);
  iCutBit++;
  if (nHitsTPC  >= fMinNHitTPC  ) fBitmap->SetBitNumber(iCutBit,kTRUE);
  iCutBit++;
  if (nHitsTRD  >= fMinNHitTRD  ) fBitmap->SetBitNumber(iCutBit,kTRUE);
  iCutBit++;
  if (nHitsTOF  >= fMinNHitTOF  ) fBitmap->SetBitNumber(iCutBit,kTRUE);
  iCutBit++;
  if (nHitsMUON  >= fMinNHitMUON) fBitmap->SetBitNumber(iCutBit,kTRUE);
}


void IlcCFAcceptanceCuts::SetMCEventInfo(const TObject* mcInfo) {
  //
  // Sets pointer to MC event information (IlcMCEvent)
  //

  if (!mcInfo) {
    IlcError("Pointer to IlcMCEvent !");
    return;
  }
  
  TString className(mcInfo->ClassName());
  if (className.CompareTo("IlcMCEvent") != 0) {
    IlcError("argument must point to an IlcMCEvent !");
    return ;
  }
  
  fMCInfo = (IlcMCEvent*) mcInfo ;
}


//__________________________________________________________________________________
void IlcCFAcceptanceCuts::AddQAHistograms(TList *qaList) {
  //
  // saves the histograms in a TList
  //

  DefineHistograms();

  qaList->Add(fhCutStatistics);
  qaList->Add(fhCutCorrelation);

  for (Int_t j=0; j<kNStepQA; j++) {
    for(Int_t i=0; i<kNCuts; i++)
      qaList->Add(fhQA[i][j]);
  }
}

//__________________________________________________________________________________
void IlcCFAcceptanceCuts::DefineHistograms() {
  //
  // histograms for cut variables, cut statistics and cut correlations
  //
  Int_t color = 2;

  // book cut statistics and cut correlation histograms
  fhCutStatistics = new TH1F(Form("%s_cut_statistics",GetName()),"",kNCuts,0.5,kNCuts+0.5);
  fhCutStatistics->SetLineWidth(2);
  int k = 1;
  fhCutStatistics->GetXaxis()->SetBinLabel(k,"hits ITS") ; k++;
  fhCutStatistics->GetXaxis()->SetBinLabel(k,"hits TPC") ; k++;
  fhCutStatistics->GetXaxis()->SetBinLabel(k,"hits TRD") ; k++;
  fhCutStatistics->GetXaxis()->SetBinLabel(k,"hits TOF") ; k++;
  fhCutStatistics->GetXaxis()->SetBinLabel(k,"hits MUON"); k++;


  fhCutCorrelation = new TH2F(Form("%s_cut_correlation",GetName()),"",kNCuts,0.5,kNCuts+0.5,kNCuts,0.5,kNCuts+0.5);
  fhCutCorrelation->SetLineWidth(2);
  for (k=1; k<=kNCuts; k++) {
    fhCutCorrelation->GetXaxis()->SetBinLabel(k,fhCutStatistics->GetXaxis()->GetBinLabel(k));
    fhCutCorrelation->GetYaxis()->SetBinLabel(k,fhCutStatistics->GetXaxis()->GetBinLabel(k));
  }

  Char_t str[5];
  for (int i=0; i<kNStepQA; i++) {
    if (i==0) snprintf(str,5," ");
    else snprintf(str,5,"_cut");
    fhQA[kCutHitsITS] [i] = new TH1F(Form("%s_HitsITS%s"  ,GetName(),str),"",10,0,10);
    fhQA[kCutHitsTPC] [i] = new TH1F(Form("%s_HitsTPC%s"  ,GetName(),str),"",5,0,5);
    fhQA[kCutHitsTRD] [i] = new TH1F(Form("%s_HitsTRD%s"  ,GetName(),str),"",20,0,20);
    fhQA[kCutHitsTOF] [i] = new TH1F(Form("%s_HitsTOF%s"  ,GetName(),str),"",5,0,5);
    fhQA[kCutHitsMUON][i] = new TH1F(Form("%s_HitsMUON%s" ,GetName(),str),"",5,0,5);
  }
  for(Int_t i=0; i<kNCuts; i++) fhQA[i][1]->SetLineColor(color);
}

//__________________________________________________________________________________
void IlcCFAcceptanceCuts::FillHistograms(TObject* obj, Bool_t afterCuts)
{
  //
  // fill the QA histograms
  //
  IlcMCParticle* part = dynamic_cast<IlcMCParticle *>(obj);
  if (!part) {
    IlcError("casting failed");
    return;
  }

  Int_t nHitsITS=0, nHitsTPC=0, nHitsTRD=0, nHitsTOF=0, nHitsMUON=0 ;
  for (Int_t iTrackRef=0; iTrackRef<part->GetNumberOfTrackReferences(); iTrackRef++) {
    IlcTrackReference * trackRef = part->GetTrackReference(iTrackRef);
    if(trackRef){
      Int_t detectorId = trackRef->DetectorId();
      switch(detectorId) {
      case IlcTrackReference::kITS  : nHitsITS++  ; break ;
      case IlcTrackReference::kTPC  : nHitsTPC++  ; break ;
      case IlcTrackReference::kTRD  : nHitsTRD++  ; break ;
      case IlcTrackReference::kTOF  : nHitsTOF++  ; break ;
      case IlcTrackReference::kMUON : nHitsMUON++ ; break ;
      default : break ;
      }
    }
  }
  
  fhQA[kCutHitsITS ][afterCuts]->Fill(nHitsITS );
  fhQA[kCutHitsTPC ][afterCuts]->Fill(nHitsTPC );
  fhQA[kCutHitsTRD ][afterCuts]->Fill(nHitsTRD );
  fhQA[kCutHitsTOF ][afterCuts]->Fill(nHitsTOF );
  fhQA[kCutHitsMUON][afterCuts]->Fill(nHitsMUON);

  // fill cut statistics and cut correlation histograms with information from the bitmap
  if (afterCuts) return;

  // Number of single cuts in this class
  UInt_t ncuts = fBitmap->GetNbits();
  for(UInt_t bit=0; bit<ncuts;bit++) {
    if (!fBitmap->TestBitNumber(bit)) {
      fhCutStatistics->Fill(bit+1);
      for (UInt_t bit2=bit; bit2<ncuts;bit2++) {
        if (!fBitmap->TestBitNumber(bit2)) 
          fhCutCorrelation->Fill(bit+1,bit2+1);
      }
    }
  }
}

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

/* $Id$ */

//--------------------------------------------------
// Filling of CalTrkTrack objects in the reader task
//
// Author: magali.estienne@subatech.in2p3.fr
//         alexandre.shabetai@cern.ch
//-------------------------------------------------

// --- IlcRoot header files ---
#include "IlcJetFillCalTrkTrack.h"
#include "IlcJetReaderHeader.h"
#include "IlcVEvent.h"
#include "IlcESDtrack.h"
#include "IlcAODTrack.h"

ClassImp(IlcJetFillCalTrkTrack)

/////////////////////////////////////////////////////////////////////

IlcJetFillCalTrkTrack::IlcJetFillCalTrkTrack(): 
  IlcJetFillCalTrkEvent(),
  fHadCorr(0),
  fApplyMIPCorrection(kTRUE),
  fVEvt(0x0)
{
  // constructor
}

//-----------------------------------------------------------------------
IlcJetFillCalTrkTrack::IlcJetFillCalTrkTrack(IlcVEvent* evt): 
  IlcJetFillCalTrkEvent(),
  fHadCorr(0),
  fApplyMIPCorrection(kTRUE),
  fVEvt(evt)
{
  // constructor
}

//-----------------------------------------------------------------------
IlcJetFillCalTrkTrack::IlcJetFillCalTrkTrack(const IlcJetFillCalTrkTrack &det): 
  IlcJetFillCalTrkEvent(det),
  fHadCorr(det.fHadCorr),
  fApplyMIPCorrection(det.fApplyMIPCorrection),
  fVEvt(det.fVEvt)
{
  // Copy constructor
}

//-----------------------------------------------------------------------
IlcJetFillCalTrkTrack& IlcJetFillCalTrkTrack::operator=(const IlcJetFillCalTrkTrack& other)
{
  // Assignment
  if (this != &other) {
   fHadCorr = other.fHadCorr;
   fApplyMIPCorrection = other.fApplyMIPCorrection;
   fVEvt = other.fVEvt;
 }

  return (*this);

}

//-----------------------------------------------------------------------
IlcJetFillCalTrkTrack::~IlcJetFillCalTrkTrack()
{
  // destructor
}

//-----------------------------------------------------------------------
void IlcJetFillCalTrkTrack::Exec(Option_t* const /*option*/)
{
  // Main method.
  // Fill IlcJetFillCalTrkTrack the with the charged particle information

  fDebug = fReaderHeader->GetDebug();
  fOpt = fReaderHeader->GetDetector();
  TString datatype = fReaderHeader->GetDataType();
  datatype.ToUpper();

  Int_t type=0; 

  if      (datatype.Contains("AODEXTRAONLY")) { type=kTrackAODextraonly;} 
  else if (datatype.Contains("AODEXTRA")) { type=kTrackAODextra; }
  else if (datatype.Contains("AOD")) { type=kTrackAOD; }
  else if (datatype.Contains("ESD")) { type=kTrackESD; }
  else { cout<< "Unknown Data type !" << endl; }

  // temporary storage of signal and pt cut flag
  Bool_t sflag = 0;
  Bool_t cflag = 0;

  // get cuts set by user
  Float_t ptMin  = fReaderHeader->GetPtCut();
  Float_t etaMin = fReaderHeader->GetFiducialEtaMin();
  Float_t etaMax = fReaderHeader->GetFiducialEtaMax();
  Float_t phiMin = fReaderHeader->GetFiducialPhiMin();
  Float_t phiMax = fReaderHeader->GetFiducialPhiMax();
  UInt_t filterMask = ((IlcJetReaderHeader*)fReaderHeader)->GetTestFilterMask();
  UInt_t filterType = ((IlcJetReaderHeader*)fReaderHeader)->GetFilterType();

  if(fDebug>2)Printf("%s:%d Selecting tracks with %d",(char*)__FILE__,__LINE__,type);

  Int_t goodTrackStd = 0;
  Int_t goodTrackNonStd = 0;
  if(type==kTrackESD || type==kTrackAOD || type==kTrackAODextra || type==kTrackAODextraonly){
    if(type!=kTrackAODextraonly) {
      if(!fVEvt){
        if(fDebug>2)Printf("%s:%d No AOD or ESD input event",(char*)__FILE__,__LINE__);
        return;
      }
      for(int it = 0;it < fVEvt->GetNumberOfTracks();++it){
        IlcVTrack *tr = (IlcVTrack*)fVEvt->GetTrack(it);
        cflag=sflag=0;
        if(fVEvt->InheritsFrom("IlcESDEvent")){ 
          // ESD case ....
          if(tr->GetStatus()==0){ 
            if(fDebug>10)Printf("%s:%d Not matching status %d/%d",(char*)__FILE__,__LINE__,it,fVEvt->GetNumberOfTracks());
            continue;
          }
          if ((tr->GetStatus() & IlcESDtrack::kTPCrefit) == 0)    continue;      // quality check
          if ((tr->GetStatus() & IlcESDtrack::kITSrefit) == 0)    continue;      // quality check
          if (((IlcJetReaderHeader*) fReaderHeader)->ReadSignalOnly() 
	      && TMath::Abs(tr->GetLabel()) > 10000)  continue;      // quality check
          if (((IlcJetReaderHeader*) fReaderHeader)->ReadBkgdOnly() 
	      && TMath::Abs(tr->GetLabel()) < 10000)  continue;      // quality check
        }
        if((fVEvt->InheritsFrom("IlcAODEvent"))){
          // AOD case ....
          IlcAODTrack *trAOD = dynamic_cast<IlcAODTrack*> (tr);
          if(!trAOD) continue;
       	  Bool_t bGood = false;
	  if(filterType == 0)bGood = true;
	  else if(filterType == 1)bGood = trAOD->IsHybridTPCConstrainedGlobal();
	  else if(filterType == 2)bGood = trAOD->IsHybridGlobalConstrainedGlobal();
          if((filterMask>0)&&((!trAOD->TestFilterBit(filterMask)||(!bGood)))){
	    if(fDebug>10)Printf("%s:%d Not matching filter %d/%d %d/%d",(char*)__FILE__,__LINE__,it,fVEvt->GetNumberOfTracks(),filterMask,trAOD->GetFilterMap());
	    continue;
	  }
          // Should we also check the GetStatus ?
        }
        if((tr->Eta()> etaMax) || (tr->Eta() < etaMin)){
          if(fDebug>10)Printf("%s:%d Not matching eta %d/%d",(char*)__FILE__,__LINE__,it,fVEvt->GetNumberOfTracks());
          continue;
        }
        Float_t phi = ( (tr->Phi()) < 0) ? (tr->Phi()) + 2. * TMath::Pi() : (tr->Phi());
        if((phi > phiMax) || (phi < phiMin)){
          if(fDebug>10)Printf("%s:%d Not matching eta %d/%d",(char*)__FILE__,__LINE__,it,fVEvt->GetNumberOfTracks());
          continue;
        }
        if(tr->Pt() <= ptMin){
          if(fDebug>10)Printf("%s:%d Not matching pt %d/%d",(char*)__FILE__,__LINE__,it,fVEvt->GetNumberOfTracks());
        }
        else {cflag=1;}
        sflag=(TMath::Abs(tr->GetLabel()) < 10000) ? 1 : 0;

        if(fDebug>10)Printf("%s:%d MATCHED %d/%d",(char*)__FILE__,__LINE__,it,fVEvt->GetNumberOfTracks());

        fCalTrkEvent->AddCalTrkTrack(tr,cflag,sflag); 
        goodTrackStd++;
      }
    }
    if(type==kTrackAODextra || type==kTrackAODextraonly) {
      if(!fVEvt){
        return;   
      }
      TClonesArray *aodExtraTracks = dynamic_cast<TClonesArray*>(fVEvt->FindListObject("aodExtraTracks"));
      if(!aodExtraTracks) return; 
      for(int it =0; it<aodExtraTracks->GetEntries(); it++) {
        cflag=sflag=0;
        IlcVParticle *track = dynamic_cast<IlcVParticle*> ((*aodExtraTracks)[it]);
        if (!track) continue; 
        IlcAODTrack *trackAOD = dynamic_cast<IlcAODTrack*> (track);
        if(!trackAOD) continue;
        if (trackAOD->GetStatus() == 0) continue; 
	Bool_t bGood = false;
	if(filterType == 0)bGood = true;
	else if(filterType == 1)bGood = trackAOD->IsHybridTPCConstrainedGlobal();
	else if(filterType == 2)bGood = trackAOD->IsHybridGlobalConstrainedGlobal();
        if((filterMask>0)&&((!trackAOD->TestFilterBit(filterMask)||(!bGood))))continue;
        if((trackAOD->Eta()> etaMax) || (trackAOD->Eta() < etaMin)){
          if(fDebug>10)Printf("%s:%d Not matching eta %d/%d",(char*)__FILE__,__LINE__,it,fVEvt->GetNumberOfTracks());
          continue;
        }
        Float_t phi = ( (trackAOD->Phi()) < 0) ? (trackAOD->Phi()) + 2. * TMath::Pi() : (trackAOD->Phi());
        if((phi > phiMax) || (phi < phiMin)){
          if(fDebug>10)Printf("%s:%d Not matching eta %d/%d",(char*)__FILE__,__LINE__,it,fVEvt->GetNumberOfTracks());
          continue;
        }
        if(trackAOD->Pt() <= ptMin){
          if(fDebug>10)Printf("%s:%d Not matching pt %d/%d",(char*)__FILE__,__LINE__,it,fVEvt->GetNumberOfTracks());
        }
        else {cflag=1;}
        sflag=(TMath::Abs(trackAOD->GetLabel()) < 10000) ? 1 : 0; 
        fCalTrkEvent->AddCalTrkTrack(trackAOD,cflag,sflag);
        goodTrackNonStd++;
      }
    }
  }

  if(fDebug>0)printf("Number of good tracks selected: %5d \n", goodTrackStd+goodTrackNonStd);

}














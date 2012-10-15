#ifndef ILCJETFILLCALTRKTRACK_H
#define ILCJETFILLCALTRKTRACK_H
 
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */
 
/* $Id$ */

//--------------------------------------------------
// Filling of CalTrkTrack objects in the reader task
//
// Author: magali.estienne@subatech.in2p3.fr
//         alexandre.shabetai@cern.ch
//-------------------------------------------------

#include "IlcJetFillCalTrkEvent.h"

class IlcJetFillCalTrkTrack : public IlcJetFillCalTrkEvent
{
 public: 
  IlcJetFillCalTrkTrack();
  IlcJetFillCalTrkTrack(IlcVEvent *fVEvt);
  virtual ~IlcJetFillCalTrkTrack();
  
  // Setter
  void  SetHadCorrector(IlcJetHadronCorrection* const corr) {fHadCorr = corr;}
  void  SetApplyMIPCorrection(Bool_t const val)             {fApplyMIPCorrection = val;}
  void  SetVEvent(IlcVEvent* const evt)                     {fVEvt = evt;} 

  // Getter
  Int_t GetHadCorrection()  const {return fApplyMIPCorrection;}

  // Other
  void  Exec(Option_t* const option);

  // we have different cases
  // AOD reading -> MC from AOD
  // ESD reading -> MC from Kinematics
  // this has to match with our selection of input events
  enum {kTrackUndef = 0, kTrackESD, kTrackAOD, kTrackAODextra, kTrackAODextraonly};

 protected:
  IlcJetHadronCorrection* fHadCorr;            // Pointer to Hadron Correction Object
  Bool_t                  fApplyMIPCorrection; // Apply MIP or not ? Exclusive with fApplyFractionHadronicCorrection
  IlcVEvent*              fVEvt;               // Pointer to IlcVEvent object

 private:
  IlcJetFillCalTrkTrack(const IlcJetFillCalTrkTrack &det);
  IlcJetFillCalTrkTrack &operator=(const IlcJetFillCalTrkTrack &det);

  ClassDef(IlcJetFillCalTrkTrack,1)            // Fill IlcJetCalTrkTrack with track information

};

#endif

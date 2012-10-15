#ifndef ILCJETFILLCALTRKTRACKMC_H
#define ILCJETFILLCALTRKTRACKMC_H
 
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */
 
/* $Id$ */

//--------------------------------------------------
// Filling of CalTrkTrack objects in the MC reader task
//
// Author: magali.estienne@subatech.in2p3.fr
//         alexandre.shabetai@cern.ch
//-------------------------------------------------

#include "IlcJetFillCalTrkEvent.h"

class IlcVEvent;
class IlcMCEvent;

class IlcJetFillCalTrkTrackMC : public IlcJetFillCalTrkEvent
{
 public: 
  IlcJetFillCalTrkTrackMC();
  IlcJetFillCalTrkTrackMC(IlcVEvent *fVEvt);
  virtual ~IlcJetFillCalTrkTrackMC();
  
  // Setter
  void    SetHadCorrector(IlcJetHadronCorrection* const corr) {fHadCorr = corr;}
  void    SetApplyMIPCorrection(Bool_t const val)             {fApplyMIPCorrection = val;}
  void    SetVEvent(IlcVEvent* const evt)                     {fVEvt = evt;} 
  void    SetMCEvent(IlcMCEvent* const mc)                    {fMCEvent = mc ;}

  // Getter
  Int_t   GetHadCorrection() const                            {return fApplyMIPCorrection;}

  // Other
  void    Exec(Option_t* const option);
  Bool_t  FillKine();
  // Fast Simulation
  Float_t SmearMomentum(Int_t ind, Float_t p);
  Bool_t  Efficiency(Float_t pt, Float_t eta, Float_t phi);

  // we have different cases
  // AOD reading -> MC from AOD
  // ESD reading -> MC from Kinematics
  // this has to match with our selection of input events
  enum {kTrackUndef = 0, kTrackKineAll, kTrackKineCharged, kTrackAODMCAll, kTrackAODMCCharged, kTrackAODMCChargedAcceptance};

 protected:
  IlcJetHadronCorrection* fHadCorr;            // Pointer to Hadron Correction Object
  Bool_t                  fApplyMIPCorrection; // Apply MIP or not ? Exclusive with fApplyFractionHadronicCorrection
  IlcVEvent*              fVEvt;               // Pointer to IlcVEvent object
  IlcMCEvent*             fMCEvent;            // Pointer to IlcMCEvent object
          
 private:
  IlcJetFillCalTrkTrackMC(const IlcJetFillCalTrkTrackMC &det);
  IlcJetFillCalTrkTrackMC &operator=(const IlcJetFillCalTrkTrackMC &det);

  ClassDef(IlcJetFillCalTrkTrackMC,1)          // Fill IlcJetCalTrkTrack/TrackKine with MC track information
};

#endif

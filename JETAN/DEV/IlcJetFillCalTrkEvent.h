#ifndef ILCJETFILLCALTRKEVENT_H
#define ILCJETFILLCALTRKEVENT_H
 
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */
 
/* $Id$ */

//--------------------------------------------------
// Filling of CalTrkEvent objects in the reader task
//
// Author: magali.estienne@subatech.in2p3.fr
//         alexandre.shabetai@cern.ch
//-------------------------------------------------

class IlcJetReaderHeader;
class IlcEMCALGeometry;
class IlcEMCALRecoUtils;
class IlcVEvent;
class IlcMCEvent;
class IlcJetHadronCorrection;

#include <TMath.h>
#include "IlcJetCalTrk.h"

class IlcJetFillCalTrkEvent 
{
 public: 
  IlcJetFillCalTrkEvent();
  virtual ~IlcJetFillCalTrkEvent();
  IlcJetFillCalTrkEvent(const IlcJetFillCalTrkEvent &det);
  IlcJetFillCalTrkEvent &operator=(const IlcJetFillCalTrkEvent &det);
  
  // Setter
  virtual void SetReaderHeader(IlcJetReaderHeader* const readerHeader) {fReaderHeader = readerHeader;}
  virtual void SetGeom(IlcEMCALGeometry* const geom)                   {fGeom = geom;}
  virtual void SetCalTrkEvent(IlcJetCalTrkEvent* caltrkevt) {fCalTrkEvent = caltrkevt;}
  virtual void SetHadCorrector(IlcJetHadronCorrection* /*corr*/)  {;}
  virtual void SetApplyMIPCorrection(Bool_t /*val*/)              {;}
  virtual void SetVEvent(IlcVEvent */*aod*/)                       {;}
  virtual void SetMCEvent(IlcMCEvent */*MC*/)                      {;}
  virtual void SetEMCALRecoUtils(IlcEMCALRecoUtils */*ru*/)       {;}
  virtual void SetApplyElectronCorrection(Int_t /*flag*/)         {;}
  virtual void SetApplyFractionHadronicCorrection(Bool_t /*val*/) {;}
  virtual void SetFractionHadronicCorrection(Double_t /*val*/)    {;}

  // Getter
  virtual IlcJetCalTrkEvent* GetCalTrkEvent() const {return fCalTrkEvent;}

  // Other
  virtual void          Exec(Option_t* const /*option*/) {;}
  virtual Float_t       EtaToTheta(Float_t arg);

 protected:
  Int_t                 fOpt;             // Detector to be used for jet reconstruction
  Int_t                 fDebug;           // Debug option
  IlcJetReaderHeader   *fReaderHeader;    // ReaderHeader
  IlcJetCalTrkEvent    *fCalTrkEvent;     // CalTrk event

  IlcEMCALGeometry     *fGeom;            // Define EMCal geometry

 private:

  ClassDef(IlcJetFillCalTrkEvent,1) // Fill IlcJetFillCalTrkEvent with tpc and/or emcal information
};

#endif

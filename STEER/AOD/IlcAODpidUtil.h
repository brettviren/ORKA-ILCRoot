#ifndef ILCAODPIDUTIL_H
#define ILCAODPIDUTIL_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcAODpidUtil.h 38493 2010-01-26 16:33:03Z hristov $ */

//-------------------------------------------------------
//                    Combined PID class
//                    for the AOD class
//   Origin: Rosa Romita, GSI, r.romita@gsi.de 
//   Modified: Jens Wiechula, Uni Tuebingen, jens.wiechula@cern.ch
//   Modified: Pietro Antonioli, INFN BO, pietro.antonioli@bo.infn.it
//-------------------------------------------------------
#include <Rtypes.h>
#include <TMatrixD.h>
#include "IlcAODEvent.h" // Needed for inline functions
#include "IlcAODTrack.h" // Needed for inline functions
#include "IlcAODPid.h" // Needed for inline functions
#include "IlcTOFHeader.h" //Needed for inline functions
//#include "HMPID/IlcHMPID.h"

#include "IlcPIDResponse.h"

class IlcAODEvent;
class IlcVParticle;

class IlcAODpidUtil : public IlcPIDResponse  {
public:
  //TODO: isMC???
  IlcAODpidUtil(Bool_t isMC = kFALSE): IlcPIDResponse(isMC) {;}
  virtual ~IlcAODpidUtil() {;}

  Int_t MakePID(const IlcAODTrack *track,Double_t *p) const;
  void MakeTPCPID(const IlcAODTrack *track,Double_t *p) const;
  void MakeITSPID(const IlcAODTrack *track,Double_t *p) const;
  void MakeTOFPID(const IlcAODTrack *track,Double_t *p) const;
  //  void MakeHMPIDPID(IlcESDtrack *track);
  void MakeTRDPID(const IlcAODTrack *track,Double_t *p) const;

  virtual Float_t NumberOfSigmasTOF(const IlcVParticle *vtrack, IlcPID::EParticleType type) const;
  
private:
  
  ClassDef(IlcAODpidUtil,3)  // PID calculation class
};

inline Float_t IlcAODpidUtil::NumberOfSigmasTOF(const IlcVParticle *vtrack, IlcPID::EParticleType type) const {
  IlcAODTrack *track=(IlcAODTrack*)vtrack;
  Double_t times[IlcPID::kSPECIES];
  Double_t sigmaTOFPid[IlcPID::kSPECIES];
  IlcAODPid *pidObj = track->GetDetPid();
  if (!pidObj) return -999.;
  Double_t tofTime=pidObj->GetTOFsignal();
  pidObj->GetIntegratedTimes(times);
  pidObj->GetTOFpidResolution(sigmaTOFPid);
  IlcAODEvent *event=(IlcAODEvent*)track->GetAODEvent();
  if (event) {
    IlcTOFHeader* tofH=(IlcTOFHeader*)event->GetTOFHeader();
    if (tofH) { 
      sigmaTOFPid[type]=fTOFResponse.GetExpectedSigma(track->P(),times[type],IlcPID::ParticleMass(type)); //fTOFResponse is set in InitialiseEvent
      tofTime -= fTOFResponse.GetStartTime(vtrack->P());
    } 
  }
  if (sigmaTOFPid[type]>0) return (tofTime - times[type])/sigmaTOFPid[type];
  else return (tofTime - times[type])/fTOFResponse.GetExpectedSigma(track->P(),times[type],IlcPID::ParticleMass(type));
}

#endif



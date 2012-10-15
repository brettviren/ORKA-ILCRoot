#ifndef ILCESDPID_H
#define ILCESDPID_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcESDpid.h 50615 2011-07-16 23:19:19Z hristov $ */

//-------------------------------------------------------
//                    Combined PID class
//           for the Event Summary Data class
//   Origin: Iouri Belikov, CERN, Jouri.Belikov@cern.ch 
//   Modified: Jens Wiechula, Uni Tuebingen, jens.wiechula@cern.ch
//-------------------------------------------------------
#include <Rtypes.h>
#include "IlcESDtrack.h" // Needed for inline functions

//#include "HMPID/IlcHMPID.h"
//#include "TRD/IlcTRDpidESD.h"

#include "IlcPIDResponse.h"

class IlcESDEvent;
class IlcVEvent;
class IlcVParticle;

class IlcESDpid : public IlcPIDResponse  {
public:
  IlcESDpid(Bool_t forMC=kFALSE): IlcPIDResponse(forMC), fRangeTOFMismatch(5.) {;}
  virtual ~IlcESDpid() {}
  
  Int_t MakePID(IlcESDEvent *event, Bool_t TPCOnly = kFALSE, Float_t timeZeroTOF=9999) const;
  void MakeTPCPID(IlcESDtrack *track) const;
  void MakeITSPID(IlcESDtrack *track) const;
  void MakeTOFPID(IlcESDtrack *track, Float_t /*timeZeroTOF*/) const;
  Bool_t CheckTOFMatching(IlcESDtrack *track) const;
  //  void MakeHMPIDPID(IlcESDtrack *track);
  void MakeTRDPID(IlcESDtrack *track) const;
  void CombinePID(IlcESDtrack *track) const;
  
  virtual Float_t NumberOfSigmasTOF(const IlcVParticle *vtrack, IlcPID::EParticleType type, const Float_t timeZeroTOF) const;
  virtual Float_t NumberOfSigmasTOF(const IlcVParticle *vtrack, IlcPID::EParticleType type) const {return NumberOfSigmasTOF(vtrack,type,0); }
  
  void SetTOFResponse(IlcVEvent *vevent,EStartTimeType_t option);

  void SetNMaxSigmaTOFTPCMismatch(Float_t range) {fRangeTOFMismatch=range;}
  Float_t GetNMaxSigmaTOFTPCMismatch() const {return fRangeTOFMismatch;}

private:
  Float_t           fRangeTOFMismatch; // nSigma max for TOF matching with TPC

  ClassDef(IlcESDpid,6)  // PID calculation class
};


inline Float_t IlcESDpid::NumberOfSigmasTOF(const IlcVParticle *vtrack, IlcPID::EParticleType type, const Float_t /*timeZeroTOF*/) const {
  IlcESDtrack *track=(IlcESDtrack*)vtrack;
  Double_t times[IlcPID::kSPECIES];
  track->GetIntegratedTimes(times);
  return (track->GetTOFsignal() - fTOFResponse.GetStartTime(track->GetP()) - times[type])/fTOFResponse.GetExpectedSigma(track->GetP(),times[type],IlcPID::ParticleMass(type));
}

#endif



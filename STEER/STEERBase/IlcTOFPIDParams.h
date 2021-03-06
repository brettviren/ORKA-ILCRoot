#ifndef ILCTOFPIDPARAMS_H
#define ILCTOFPIDPARAMS_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

//***********************************************************
// Class AliTODPIDparams
// class to store PID parameters for TOF in OADB
// Author: P. Antonioli, pietro.antonioli@to.infn.it
//***********************************************************

#include <TObject.h>
#include <TNamed.h>
#include "IlcPIDResponse.h"

class IlcTOFPIDParams : public TNamed {

 public:
  IlcTOFPIDParams();
  IlcTOFPIDParams(Char_t * name);
  virtual ~IlcTOFPIDParams();

  enum {kSigPparams = 4};

  Float_t GetTOFresolution(void) const {return fTOFresolution;}
  IlcPIDResponse::EStartTimeType_t GetStartTimeMethod(void) const {return fStartTime;}
  Float_t GetSigParams(Int_t i) const {
    return ((i >= 0)  && (i<kSigPparams)) ? fSigPparams[i] : 0;}    
  void SetTOFresolution(Float_t res){fTOFresolution = res;}
  void SetStartTimeMethod(IlcPIDResponse::EStartTimeType_t method){fStartTime=method;}
  void SetSigPparams(Float_t *params);

 private:
  IlcPIDResponse::EStartTimeType_t fStartTime;      // startTime method
  Float_t fTOFresolution;                           // TOF MRPC intrinsic resolution
  Float_t fSigPparams[kSigPparams];                // parameterisation of sigma(p) dependency 

  ClassDef(IlcTOFPIDParams,1);

};

#endif


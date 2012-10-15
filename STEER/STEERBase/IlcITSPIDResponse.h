#ifndef ILCITSPIDRESPONSE_H
#define ILCITSPIDRESPONSE_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcITSPIDResponse.h 50615 2011-07-16 23:19:19Z hristov $ */

//-------------------------------------------------------
//                    ITS PID response class
//
//
//-------------------------------------------------------
//#include <Rtypes.h>
#include <TObject.h>
#include "IlcPID.h"

class IlcITSPIDResponse : public TObject {

public:
  IlcITSPIDResponse(Bool_t isMC=kFALSE);
  //IlcITSPIDResponse(Double_t *param);
 ~IlcITSPIDResponse() {}

 void SetBetheBlochParamsITSTPC(Double_t* param){
   for(Int_t iPar=0; iPar<5; iPar++) fBBtpcits[iPar]=param[iPar];
 }
 void SetBetheBlochParamsITSsa(Double_t* param){
   for(Int_t iPar=0; iPar<5; iPar++) fBBsa[iPar]=param[iPar];
 }
 void SetElectronBetheBlochParamsITSsa(Double_t* param){
   for(Int_t iPar=0; iPar<5; iPar++) fBBsaElectron[iPar]=param[iPar];
 }

 Double_t BetheAleph(Double_t p,Double_t mass) const;
 Double_t Bethe(Double_t p, Double_t mass, Bool_t iSA=kFALSE) const;
 Double_t GetResolution(Double_t bethe, Int_t nPtsForPid=4, Bool_t isSA=kFALSE) const;
 void GetITSProbabilities(Float_t mom, Double_t qclu[4], Double_t condprobfun[IlcPID::kSPECIES],Bool_t isMC=kFALSE) const;
 Float_t GetNumberOfSigmas(Float_t mom, Float_t signal, IlcPID::EParticleType type, Int_t nPtsForPid=4, Bool_t isSA=kFALSE) const {
   Float_t bethe = Bethe(mom,IlcPID::ParticleMass(type),isSA);
   return (signal - bethe)/GetResolution(bethe,nPtsForPid,isSA);
 }
 Int_t GetParticleIdFromdEdxVsP(Float_t mom, Float_t signal, Bool_t isSA=kFALSE) const;

private:


  // Data members for truncated mean method
  Float_t  fRes;             // relative dEdx resolution
  Double_t fKp1;             // ALEPH BB param 1
  Double_t fKp2;             // ALEPH BB param 2
  Double_t fKp3;             // ALEPH BB param 3
  Double_t fKp4;             // ALEPH BB param 4
  Double_t fKp5;             // ALEPH BB param 
  Double_t  fBBsa[5];         // parameters of BB for SA tracks
  Double_t  fBBsaElectron[5];         // parameters of BB for SA tracks
  Double_t  fBBtpcits[5];     // parameters of BB for TPC+ITS tracks
  Float_t  fResolSA[5];      // resolutions vs. n. of SDD/SSD points
  Float_t  fResolTPCITS[5];  // resolutions vs. n. of SDD/SSD points

  ClassDef(IlcITSPIDResponse,3)   // ITS PID class
};

#endif



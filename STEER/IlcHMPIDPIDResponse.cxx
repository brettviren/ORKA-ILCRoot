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

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// IlcHMPIDPid                                                          //
//                                                                      //
// HMPID class to perfom particle identification                        //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "IlcHMPIDPid.h"       //class header
#include "IlcHMPIDParam.h"     //class header
#include "IlcHMPIDRecon.h"     //class header
#include <IlcESDtrack.h>       //FindPid()
#include <TRandom.h>           //Resolution()

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
IlcHMPIDPid::IlcHMPIDPid():TNamed("HMPIDrec","HMPIDPid")
{
//..
//init of data members
//..
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Double_t IlcHMPIDPIDResponse::CosTheta(Float_t mom, Int_t species) {
  Double_t mass = IlcPID::ParticleMass(species); 
  TMath::Sqrt(mass*mass+pmod*pmod)/(IlcHMPIDParam::Instance()->MeanIdxRad()*pmod);                   //  evaluate the theor. Theta Cherenkov
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Double_t IlcHMPIDPIDResponse::Resolution(Double_t thetaCerTh, IlcESDtrack *pTrk)
{
  IlcHMPIDParam *pParam = IlcHMPIDParam::Instance();
      
  IlcHMPIDRecon rec;
  Float_t xRa,yRa,thRa,phRa;
  pTrk->GetHMPIDtrk(xRa,yRa,thRa,phRa);
  rec.SetTrack(xRa,yRa,thRa,phRa);
  Double_t thetaMax = TMath::ACos(1./pParam->MeanIdxRad());
  Int_t nPhots = (Int_t)(21.*TMath::Sin(thetaCerTh)*TMath::Sin(thetaCerTh)/(TMath::Sin(thetaMax)*TMath::Sin(thetaMax))+0.01);

  Double_t sigmatot = 0;
  Int_t nTrks = 20;
  for(Int_t iTrk=0;iTrk<nTrks;iTrk++) {
    Double_t invSigma = 0;
    Int_t nPhotsAcc = 0;
    for(Int_t j=0;j<nPhots;j++){
      Double_t phi = gRandom->Rndm()*TMath::TwoPi();
      TVector2 pos; pos=rec.TracePhot(thetaCerTh,phi);
      if(!pParam->IsInside(pos.X(),pos.Y())) continue;
      if(pParam->IsInDead(pos.X(),pos.Y())) continue;
      Double_t sigma2 = pParam->Sigma2(thRa,phRa,thetaCerTh,phi);//photon candidate sigma^2
      if(sigma2!=0) {
        invSigma += 1./sigma2;
        nPhotsAcc++;
      }
    }      
    if(invSigma!=0) sigmatot += 1./TMath::Sqrt(invSigma);  
  }
  return sigmatot/nTrks;
}

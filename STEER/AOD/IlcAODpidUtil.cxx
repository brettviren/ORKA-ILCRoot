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

/* $Id: IlcAODpidUtil.cxx 38329 2010-01-17 19:17:24Z hristov $ */

//-----------------------------------------------------------------
//           Implementation of the combined PID class
//           For the AOD Class
//           containing information on the particle identification
//      Origin: Rosa Romita, GSI, r.romita@gsi.de
//-----------------------------------------------------------------

#include "IlcLog.h"
#include "IlcPID.h"
#include "IlcAODpidUtil.h"
#include "IlcAODEvent.h"
#include "IlcAODTrack.h"
#include "IlcAODPid.h"
#include "IlcTRDPIDResponse.h"
#include "IlcESDtrack.h"

ClassImp(IlcAODpidUtil)

  Int_t IlcAODpidUtil::MakePID(const IlcAODTrack *track,Double_t *p) const {
  //
  //  Calculate probabilities for all detectors, except if TPConly==kTRUE
  //  and combine PID
  //  
  //   Option TPConly==kTRUE is used during reconstruction, 
  //  because ITS tracking uses TPC pid
  //  HMPID and TRD pid are done in detector reconstructors
  //

  /*
    Float_t TimeZeroTOF = 0;
    if (subtractT0) 
    TimeZeroTOF = event->GetT0();
  */
  Int_t ns=IlcPID::kSPECIES;
  Double_t tpcPid[IlcPID::kSPECIES];
  MakeTPCPID(track,tpcPid);
  Double_t itsPid[IlcPID::kSPECIES];
  Double_t tofPid[IlcPID::kSPECIES];
  Double_t trdPid[IlcPID::kSPECIES];
  MakeITSPID(track,itsPid);
  MakeTOFPID(track,tofPid);
  //MakeHMPIDPID(track);
  MakeTRDPID(track,trdPid);
  for (Int_t j=0; j<ns; j++) {
    p[j]=tpcPid[j]*itsPid[j]*tofPid[j]*trdPid[j];
  }

  return 0;
}
//_________________________________________________________________________
void IlcAODpidUtil::MakeTPCPID(const IlcAODTrack *track,Double_t *p) const
{
  //
  //  TPC pid using bethe-bloch and gaussian response
  //

  if ((track->GetStatus()&IlcESDtrack::kTPCin )==0) return;

  IlcAODPid *pidObj = track->GetDetPid();
  Double_t mom      = track->P();
  Double_t dedx     = 0.;  
  UShort_t nTPCClus = 0;
  if (pidObj) {
      nTPCClus = pidObj->GetTPCsignalN();
      dedx     = pidObj->GetTPCsignal();
      mom      = pidObj->GetTPCmomentum();
  }
  
  Bool_t mismatch=kTRUE;

  for (Int_t j=0; j<IlcPID::kSPECIES; j++) {
    IlcPID::EParticleType type=IlcPID::EParticleType(j);
    Double_t bethe=fTPCResponse.GetExpectedSignal(mom,type); 
    Double_t sigma=fTPCResponse.GetExpectedSigma(mom,nTPCClus,type);
    if (TMath::Abs(dedx-bethe) > fRange*sigma) {
      p[j]=TMath::Exp(-0.5*fRange*fRange)/sigma;
    } else {
      p[j]=TMath::Exp(-0.5*(dedx-bethe)*(dedx-bethe)/(sigma*sigma))/sigma;
      mismatch=kFALSE;
    }

  }

  if (mismatch)
    for (Int_t j=0; j<IlcPID::kSPECIES; j++) p[j]=1./IlcPID::kSPECIES;


  return;
}
//_________________________________________________________________________
void IlcAODpidUtil::MakeITSPID(const IlcAODTrack *track,Double_t *p) const
{
  //
  // ITS PID
  //  1) Truncated mean method
  //


  if ((track->GetStatus()&IlcESDtrack::kITSin)==0) return;
  UChar_t clumap=track->GetITSClusterMap();
  Int_t nPointsForPid=0;
  for(Int_t i=2; i<6; i++){
   if(clumap&(1<<i)) ++nPointsForPid;
  }
  if(nPointsForPid<3) { // track not to be used for combined PID purposes
    for (Int_t j=0; j<IlcPID::kSPECIES; j++) 
      p[j] = 1./IlcPID::kSPECIES;
    return;
  }
  Double_t mom=track->P();  
  IlcAODPid *pidObj = track->GetDetPid();

  Double_t dedx = 0.;
  if (pidObj) {
      dedx = pidObj->GetITSsignal();
  }
  
  Bool_t mismatch = kTRUE;
  Bool_t isSA = kTRUE;
  if(track->GetStatus() & IlcESDtrack::kTPCin){
    isSA = kFALSE;
    if (pidObj)
      mom = pidObj->GetTPCmomentum();
  }
  for (Int_t j=0; j<IlcPID::kSPECIES; j++) {
    Double_t mass = IlcPID::ParticleMass(j);//GeV/c^2
    Double_t bethe = fITSResponse.Bethe(mom,mass);
    Double_t sigma = fITSResponse.GetResolution(bethe,nPointsForPid,isSA);
    if (TMath::Abs(dedx-bethe) > fRange*sigma) {
      p[j]=TMath::Exp(-0.5*fRange*fRange)/sigma;
    } else {
      p[j]=TMath::Exp(-0.5*(dedx-bethe)*(dedx-bethe)/(sigma*sigma))/sigma;
      mismatch=kFALSE;
    }

    // Check for particles heavier than (IlcPID::kSPECIES - 1)

  }

  if (mismatch)
    for (Int_t j=0; j<IlcPID::kSPECIES; j++) p[j]=1./IlcPID::kSPECIES;

  return;

}
//_________________________________________________________________________
void IlcAODpidUtil::MakeTOFPID(const IlcAODTrack *track, Double_t *p) const
{
  //
  //   TOF PID using gaussian response
  //
  if ((track->GetStatus()&IlcESDtrack::kTOFout )==0)    return;
  if ((track->GetStatus()&IlcESDtrack::kTIME )==0)     return;
  if ((track->GetStatus()&IlcESDtrack::kTOFpid )==0)   return;

  Double_t time[IlcPID::kSPECIESN];
  Double_t sigma[IlcPID::kSPECIESN];
  IlcAODPid *pidObj = track->GetDetPid();
  pidObj->GetIntegratedTimes(time);

  for (Int_t iPart = 0; iPart < IlcPID::kSPECIES; iPart++) {
    sigma[iPart] = fTOFResponse.GetExpectedSigma(track->P(),time[iPart],IlcPID::ParticleMass(iPart));
  }

  IlcDebugGeneral("IlcESDpid::MakeTOFPID",2,
		  Form("Expected TOF signals [ps]: %f %f %f %f %f",
		       time[IlcPID::kElectron],
		       time[IlcPID::kMuon],
		       time[IlcPID::kPion],
		       time[IlcPID::kKaon],
		       time[IlcPID::kProton]));

  IlcDebugGeneral("IlcESDpid::MakeTOFPID",2,
		  Form("Expected TOF std deviations [ps]: %f %f %f %f %f",
		       sigma[IlcPID::kElectron],
		       sigma[IlcPID::kMuon],
		       sigma[IlcPID::kPion],
		       sigma[IlcPID::kKaon],
		       sigma[IlcPID::kProton]
		       ));

  Double_t tof = pidObj->GetTOFsignal();

  Bool_t mismatch = kTRUE;
  for (Int_t j=0; j<IlcPID::kSPECIES; j++) {
    Double_t sig = sigma[j];
    if (TMath::Abs(tof-time[j]) > fRange*sig) {
      p[j] = TMath::Exp(-0.5*fRange*fRange)/sig;
    } else
      p[j] = TMath::Exp(-0.5*(tof-time[j])*(tof-time[j])/(sig*sig))/sig;

    // Check the mismatching
    Double_t mass = IlcPID::ParticleMass(j);
    Double_t pm = fTOFResponse.GetMismatchProbability(track->P(),mass);
    if (p[j]>pm) mismatch = kFALSE;

    // Check for particles heavier than (IlcPID::kSPECIES - 1)

  }

  if (mismatch)
    for (Int_t j=0; j<IlcPID::kSPECIES; j++) p[j]=1./IlcPID::kSPECIES;

  return;
}
//_________________________________________________________________________
void IlcAODpidUtil::MakeTRDPID(const IlcAODTrack *track,Double_t *p) const
{
  ComputeTRDProbability(track, IlcPID::kSPECIES, p); 
  return;
}


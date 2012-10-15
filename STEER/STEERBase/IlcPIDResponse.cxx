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

/* $Id: IlcPIDResponse.cxx 46193 2010-12-21 09:00:14Z wiechula $ */

//-----------------------------------------------------------------
//        Base class for handling the pid response               //
//        functions of all detectors                             //
//        and give access to the nsigmas                         //
//                                                               //
//   Origin: Jens Wiechula, Uni Tuebingen, jens.wiechula@cern.ch //
//-----------------------------------------------------------------

#include <TList.h>
#include <TObjArray.h>
#include <TPRegexp.h>
#include <TF1.h>
#include <TSpline.h>
#include <TFile.h>
#include <TArrayF.h>

#include <IlcVEvent.h>
#include <IlcVTrack.h>
#include <IlcLog.h>
#include <IlcPID.h>
#include <IlcOADBContainer.h>
#include <IlcTRDPIDResponseObject.h>
#include <IlcTOFPIDParams.h>

#include "IlcPIDResponse.h"

#include "IlcCentrality.h"

ClassImp(IlcPIDResponse);

IlcPIDResponse::IlcPIDResponse(Bool_t isMC/*=kFALSE*/) :
TNamed("PIDResponse","PIDResponse"),
fITSResponse(isMC),
fTPCResponse(),
fTRDResponse(),
fTOFResponse(),
fEMCALResponse(),
fRange(5.),
fITSPIDmethod(kITSTruncMean),
fIsMC(isMC),
fOADBPath(),
fBeamType("PP"),
fLHCperiod(),
fMCperiodTPC(),
fMCperiodUser(),
fCurrentFile(),
fRecoPass(0),
fRecoPassUser(-1),
fRun(0),
fOldRun(0),
fArrPidResponseMaster(0x0),
fResolutionCorrection(0x0),
fTRDPIDResponseObject(0x0),
fTOFtail(1.1),
fTOFPIDParams(0x0),
fEMCALPIDParams(0x0),
fCurrentEvent(0x0),
fCurrCentrality(0.0)
{
  //
  // default ctor
  //
  IlcLog::SetClassDebugLevel("IlcPIDResponse",10);
  IlcLog::SetClassDebugLevel("IlcESDpid",10);
  IlcLog::SetClassDebugLevel("IlcAODpidUtil",10);

  memset(fTRDslicesForPID,0,sizeof(UInt_t)*2);
}

//______________________________________________________________________________
IlcPIDResponse::~IlcPIDResponse()
{
  //
  // dtor
  //
    delete fArrPidResponseMaster;
    delete fTRDPIDResponseObject;
  if (fTOFPIDParams) delete fTOFPIDParams;
}

//______________________________________________________________________________
IlcPIDResponse::IlcPIDResponse(const IlcPIDResponse &other) :
TNamed(other),
fITSResponse(other.fITSResponse),
fTPCResponse(other.fTPCResponse),
fTRDResponse(other.fTRDResponse),
fTOFResponse(other.fTOFResponse),
fEMCALResponse(other.fEMCALResponse),
fRange(other.fRange),
fITSPIDmethod(other.fITSPIDmethod),
fIsMC(other.fIsMC),
fOADBPath(other.fOADBPath),
fBeamType("PP"),
fLHCperiod(),
fMCperiodTPC(),
fMCperiodUser(other.fMCperiodUser),
fCurrentFile(),
fRecoPass(0),
fRecoPassUser(other.fRecoPassUser),
fRun(0),
fOldRun(0),
fArrPidResponseMaster(0x0),
fResolutionCorrection(0x0),
fTRDPIDResponseObject(0x0),
fTOFtail(1.1),
fTOFPIDParams(0x0),
fEMCALPIDParams(0x0),
fCurrentEvent(0x0),
fCurrCentrality(0.0)
{
  //
  // copy ctor
  //
  memset(fTRDslicesForPID,0,sizeof(UInt_t)*2);
}

//______________________________________________________________________________
IlcPIDResponse& IlcPIDResponse::operator=(const IlcPIDResponse &other)
{
  //
  // copy ctor
  //
  if(this!=&other) {
    delete fArrPidResponseMaster;
    TNamed::operator=(other);
    fITSResponse=other.fITSResponse;
    fTPCResponse=other.fTPCResponse;
    fTRDResponse=other.fTRDResponse;
    fTOFResponse=other.fTOFResponse;
    fEMCALResponse=other.fEMCALResponse;
    fRange=other.fRange;
    fITSPIDmethod=other.fITSPIDmethod;
    fOADBPath=other.fOADBPath;
    fIsMC=other.fIsMC;
    fBeamType="PP";
    fLHCperiod="";
    fMCperiodTPC="";
    fMCperiodUser=other.fMCperiodUser;
    fCurrentFile="";
    fRecoPass=0;
    fRecoPassUser=other.fRecoPassUser;
    fRun=0;
    fOldRun=0;
    fArrPidResponseMaster=0x0;
    fResolutionCorrection=0x0;
    fTRDPIDResponseObject=0x0;
    fEMCALPIDParams=0x0;
    memset(fTRDslicesForPID,0,sizeof(UInt_t)*2);
    fTOFtail=1.1;
    fTOFPIDParams=0x0;
    fCurrentEvent=other.fCurrentEvent;
  }
  return *this;
}

//______________________________________________________________________________
Float_t IlcPIDResponse::NumberOfSigmas(EDetCode detCode, const IlcVParticle *track, IlcPID::EParticleType type) const
{
  //
  // NumberOfSigmas for 'detCode'
  //

  switch (detCode){
    case kDetITS: return NumberOfSigmasITS(track, type); break;
    case kDetTPC: return NumberOfSigmasTPC(track, type); break;
    case kDetTOF: return NumberOfSigmasTOF(track, type); break;
//     case kDetTRD: return ComputeTRDProbability(track, type); break;
//     case kDetPHOS: return ComputePHOSProbability(track, type); break;
//     case kDetEMCAL: return NumberOfSigmasEMCAL(track, type); break;
//     case kDetHMPID: return ComputeHMPIDProbability(track, type); break;
    default: return -999.;
  }

}

//______________________________________________________________________________
Float_t IlcPIDResponse::NumberOfSigmasEMCAL(const IlcVTrack *track, IlcPID::EParticleType type) const {

  IlcVCluster *matchedClus = NULL;

  Double_t mom     = -1.; 
  Double_t pt      = -1.; 
  Double_t EovP    = -1.;
  Double_t fClsE   = -1.;
  
  Int_t nMatchClus = -1;
  Int_t charge     = 0;
  
  // Track matching
  nMatchClus = track->GetEMCALcluster();
  if(nMatchClus > -1){

    mom    = track->P();
    pt     = track->Pt();
    charge = track->Charge();
    
    matchedClus = (IlcVCluster*)fCurrentEvent->GetCaloCluster(nMatchClus);
    
    if(matchedClus){
      
    // matched cluster is EMCAL
    if(matchedClus->IsEMCAL()){
      
      fClsE       = matchedClus->E();
      EovP        = fClsE/mom;
      
      
      // NSigma value really meaningful only for electrons!
      return fEMCALResponse.GetNumberOfSigmas(pt,EovP,type,charge); 
    }
  }
  }

  return -999;
  
}

//______________________________________________________________________________
IlcPIDResponse::EDetPidStatus IlcPIDResponse::ComputePIDProbability  (EDetCode detCode,  const IlcVTrack *track, Int_t nSpecies, Double_t p[]) const
{
  //
  // Compute PID response of 'detCode'
  //

  switch (detCode){
    case kDetITS: return ComputeITSProbability(track, nSpecies, p); break;
    case kDetTPC: return ComputeTPCProbability(track, nSpecies, p); break;
    case kDetTOF: return ComputeTOFProbability(track, nSpecies, p); break;
    case kDetTRD: return ComputeTRDProbability(track, nSpecies, p); break;
    case kDetPHOS: return ComputePHOSProbability(track, nSpecies, p); break;
    case kDetEMCAL: return ComputeEMCALProbability(track, nSpecies, p); break;
    case kDetHMPID: return ComputeHMPIDProbability(track, nSpecies, p); break;
    default: return kDetNoSignal;
  }
}

//______________________________________________________________________________
IlcPIDResponse::EDetPidStatus IlcPIDResponse::ComputeITSProbability  (const IlcVTrack *track, Int_t nSpecies, Double_t p[]) const
{
  //
  // Compute PID response for the ITS
  //

  // set flat distribution (no decision)
  for (Int_t j=0; j<nSpecies; j++) p[j]=1./nSpecies;

  if ((track->GetStatus()&IlcVTrack::kITSin)==0 &&
    (track->GetStatus()&IlcVTrack::kITSout)==0) return kDetNoSignal;
  
  Double_t mom=track->P();
  Double_t dedx=track->GetITSsignal();
  Bool_t isSA=kTRUE;
  Double_t momITS=mom;
  ULong_t trStatus=track->GetStatus();
  if(trStatus&IlcVTrack::kTPCin) isSA=kFALSE;
  UChar_t clumap=track->GetITSClusterMap();
  Int_t nPointsForPid=0;
  for(Int_t i=2; i<6; i++){
    if(clumap&(1<<i)) ++nPointsForPid;
  }
  
  if(nPointsForPid<3) { // track not to be used for combined PID purposes
    //       track->ResetStatus(IlcVTrack::kITSpid);
    return kDetNoSignal;
  }

  Bool_t mismatch=kTRUE/*, heavy=kTRUE*/;
  for (Int_t j=0; j<IlcPID::kSPECIES; j++) {
    Double_t mass=IlcPID::ParticleMass(j);//GeV/c^2
    Double_t bethe=fITSResponse.Bethe(momITS,mass);
    Double_t sigma=fITSResponse.GetResolution(bethe,nPointsForPid,isSA);
    if (TMath::Abs(dedx-bethe) > fRange*sigma) {
      p[j]=TMath::Exp(-0.5*fRange*fRange)/sigma;
    } else {
      p[j]=TMath::Exp(-0.5*(dedx-bethe)*(dedx-bethe)/(sigma*sigma))/sigma;
      mismatch=kFALSE;
    }

    // Check for particles heavier than (IlcPID::kSPECIES - 1)
    //       if (dedx < (bethe + fRange*sigma)) heavy=kFALSE;

  }

  if (mismatch){
    for (Int_t j=0; j<IlcPID::kSPECIES; j++) p[j]=1./IlcPID::kSPECIES;
    return kDetNoSignal;
  }

    
  return kDetPidOk;
}
//______________________________________________________________________________
IlcPIDResponse::EDetPidStatus IlcPIDResponse::ComputeTPCProbability  (const IlcVTrack *track, Int_t nSpecies, Double_t p[]) const
{
  //
  // Compute PID response for the TPC
  //

  // set flat distribution (no decision)
  for (Int_t j=0; j<nSpecies; j++) p[j]=1./nSpecies;

  // check quality of the track
  if ( (track->GetStatus()&IlcVTrack::kTPCin )==0 && (track->GetStatus()&IlcVTrack::kTPCout)==0 ) return kDetNoSignal;

  Double_t mom = track->GetTPCmomentum();

  Double_t dedx=track->GetTPCsignal();
  Bool_t mismatch=kTRUE/*, heavy=kTRUE*/;

  for (Int_t j=0; j<IlcPID::kSPECIES; j++) {
    IlcPID::EParticleType type=IlcPID::EParticleType(j);
    Double_t bethe=fTPCResponse.GetExpectedSignal(mom,type);
    Double_t sigma=fTPCResponse.GetExpectedSigma(mom,track->GetTPCsignalN(),type);
    if (TMath::Abs(dedx-bethe) > fRange*sigma) {
      p[j]=TMath::Exp(-0.5*fRange*fRange)/sigma;
    } else {
      p[j]=TMath::Exp(-0.5*(dedx-bethe)*(dedx-bethe)/(sigma*sigma))/sigma;
      mismatch=kFALSE;
    }

    // TODO: Light nuclei, also in TPC pid response
    
    // Check for particles heavier than (IlcPID::kSPECIES - 1)
//     if (dedx < (bethe + fRange*sigma)) heavy=kFALSE;

  }

  if (mismatch){
    for (Int_t j=0; j<nSpecies; j++) p[j]=1./nSpecies;
    return kDetNoSignal;
  }

  return kDetPidOk;
}
//______________________________________________________________________________
IlcPIDResponse::EDetPidStatus IlcPIDResponse::ComputeTOFProbability  (const IlcVTrack *track, Int_t nSpecies, Double_t p[]) const
{
  //
  // Compute PID response for the
  //

  Double_t meanCorrFactor = 0.11/fTOFtail; // Correction factor on the mean because of the tail (should be ~ 0.1 with tail = 1.1)

  // set flat distribution (no decision)
  for (Int_t j=0; j<nSpecies; j++) p[j]=1./nSpecies;
  
  if ((track->GetStatus()&IlcVTrack::kTOFout)==0) return kDetNoSignal;
  if ((track->GetStatus()&IlcVTrack::kTIME)==0) return kDetNoSignal;
  
  Double_t time[IlcPID::kSPECIESN];
  track->GetIntegratedTimes(time);
  
  Double_t sigma[IlcPID::kSPECIES];
  for (Int_t iPart = 0; iPart < IlcPID::kSPECIES; iPart++) {
    sigma[iPart] = fTOFResponse.GetExpectedSigma(track->P(),time[iPart],IlcPID::ParticleMass(iPart));
  }
  
  Bool_t mismatch = kTRUE/*, heavy = kTRUE*/;
  for (Int_t j=0; j<IlcPID::kSPECIES; j++) {
    IlcPID::EParticleType type=IlcPID::EParticleType(j);
    Double_t nsigmas=NumberOfSigmasTOF(track,type) + meanCorrFactor;

    Double_t sig = sigma[j];
    if (TMath::Abs(nsigmas) > (fRange+2)) {
      if(nsigmas < fTOFtail)
	p[j] = TMath::Exp(-0.5*(fRange+2)*(fRange+2))/sig;
      else
	p[j] = TMath::Exp(-(fRange+2 - fTOFtail*0.5)*fTOFtail)/sig;
    } else{
      if(nsigmas < fTOFtail)
	p[j] = TMath::Exp(-0.5*nsigmas*nsigmas)/sig;
      else
	p[j] = TMath::Exp(-(nsigmas - fTOFtail*0.5)*fTOFtail)/sig;
    }

    /* OLD Gaussian shape
    if (TMath::Abs(nsigmas) > (fRange+2)) {
      p[j] = TMath::Exp(-0.5*(fRange+2)*(fRange+2))/sig;
    } else
      p[j] = TMath::Exp(-0.5*nsigmas*nsigmas)/sig;
    */

    if (TMath::Abs(nsigmas)<5.){
      Double_t nsigmasTPC=NumberOfSigmasTPC(track,type);
      if (TMath::Abs(nsigmasTPC)<5.) mismatch=kFALSE;
    }
  }

  if (mismatch){
    return kDetMismatch;    
  }

    // TODO: Light nuclei
    
  return kDetPidOk;
}
//______________________________________________________________________________
IlcPIDResponse::EDetPidStatus IlcPIDResponse::ComputeTRDProbability  (const IlcVTrack *track, Int_t nSpecies, Double_t p[]) const
{
  //
  // Compute PID response for the
  //

  // set flat distribution (no decision)
  for (Int_t j=0; j<nSpecies; j++) p[j]=1./nSpecies;
  if((track->GetStatus()&IlcVTrack::kTRDout)==0) return kDetNoSignal;

  Float_t mom[6];
  Double_t dedx[48];  // Allocate space for the maximum number of TRD slices
  Int_t nslices = fTRDslicesForPID[1] - fTRDslicesForPID[0] + 1;
  IlcDebug(1, Form("First Slice: %d, Last Slice: %d, Number of slices: %d",  fTRDslicesForPID[0], fTRDslicesForPID[1], nslices));
  for(UInt_t ilayer = 0; ilayer < 6; ilayer++){
    mom[ilayer] = track->GetTRDmomentum(ilayer);
    for(UInt_t islice = fTRDslicesForPID[0]; islice <= fTRDslicesForPID[1]; islice++){
      dedx[ilayer*nslices+islice-fTRDslicesForPID[0]] = track->GetTRDslice(ilayer, islice);
    }
  }
  fTRDResponse.GetResponse(nslices, dedx, mom, p);
  return kDetPidOk;
}
//______________________________________________________________________________
IlcPIDResponse::EDetPidStatus IlcPIDResponse::ComputeEMCALProbability  (const IlcVTrack *track, Int_t nSpecies, Double_t p[]) const
{
  //
  // Compute PID response for the EMCAL
  //

  IlcVCluster *matchedClus = NULL;

  Double_t mom     = -1.; 
  Double_t pt      = -1.; 
  Double_t EovP    = -1.;
  Double_t fClsE   = -1.;
  
  Int_t nMatchClus = -1;
  Int_t charge     = 0;
  
  // Track matching
  nMatchClus = track->GetEMCALcluster();

  if(nMatchClus > -1){

    mom    = track->P();
    pt     = track->Pt();
    charge = track->Charge();
    
    matchedClus = (IlcVCluster*)fCurrentEvent->GetCaloCluster(nMatchClus);
    
    if(matchedClus){    

    // matched cluster is EMCAL
    if(matchedClus->IsEMCAL()){

      fClsE       = matchedClus->E();
      EovP        = fClsE/mom;
      
      
      // compute the probabilities 
      if( 999 != fEMCALResponse.ComputeEMCALProbability(pt,EovP,charge,p)){	

  	// in case everything is OK
  	return kDetPidOk;
	
      }
    }
  }
  }
  
  // in all other cases set flat distribution (no decision)
  for (Int_t j=0; j<nSpecies; j++) p[j]=1./nSpecies;
  return kDetNoSignal;
  
}
//______________________________________________________________________________
IlcPIDResponse::EDetPidStatus IlcPIDResponse::ComputePHOSProbability (const IlcVTrack */*track*/, Int_t nSpecies, Double_t p[]) const
{
  //
  // Compute PID response for the PHOS
  //

  // set flat distribution (no decision)
  for (Int_t j=0; j<nSpecies; j++) p[j]=1./nSpecies;
  return kDetNoSignal;
}
//______________________________________________________________________________
IlcPIDResponse::EDetPidStatus IlcPIDResponse::ComputeHMPIDProbability(const IlcVTrack *track, Int_t nSpecies, Double_t p[]) const
{
  //
  // Compute PID response for the HMPID
  //

  // set flat distribution (no decision)
  for (Int_t j=0; j<nSpecies; j++) p[j]=1./nSpecies;
  if((track->GetStatus()&IlcVTrack::kHMPIDpid)==0) return kDetNoSignal;

  track->GetHMPIDpid(p);
  
  return kDetPidOk;
}

//______________________________________________________________________________
void IlcPIDResponse::InitialiseEvent(IlcVEvent *event, Int_t pass)
{
  //
  // Apply settings for the current event
  //
  fRecoPass=pass;
  
  fCurrentEvent=0x0;
  if (!event) return;
  fCurrentEvent=event;
  fRun=event->GetRunNumber();
  
  if (fRun!=fOldRun){
    ExecNewRun();
    fOldRun=fRun;
  }
  
  //TPC resolution parametrisation PbPb
  if ( fResolutionCorrection ){
    Double_t corrSigma=fResolutionCorrection->Eval(GetTPCMultiplicityBin(event));
    fTPCResponse.SetSigma(3.79301e-03*corrSigma, 2.21280e+04);
  }
  
  //TOF resolution
  SetTOFResponse(event, (IlcPIDResponse::EStartTimeType_t)fTOFPIDParams->GetStartTimeMethod());


  // Get and set centrality
  IlcCentrality *centrality = event->GetCentrality();
  if(centrality){
    fCurrCentrality = centrality->GetCentralityPercentile("V0M");
  }
  else{
    fCurrCentrality = -1;
  }
}

//______________________________________________________________________________
void IlcPIDResponse::ExecNewRun()
{
  //
  // Things to Execute upon a new run
  //
  SetRecoInfo();
  
  SetITSParametrisation();
  
  SetTPCPidResponseMaster();
  SetTPCParametrisation();

  SetTRDPidResponseMaster(); 
  InitializeTRDResponse();

  SetEMCALPidResponseMaster(); 
  InitializeEMCALResponse();
  
  SetTOFPidResponseMaster();
  InitializeTOFResponse();
}

//_____________________________________________________
Double_t IlcPIDResponse::GetTPCMultiplicityBin(const IlcVEvent * const event)
{
  //
  // Get TPC multiplicity in bins of 150
  //
  
  const IlcVVertex* vertexTPC = event->GetPrimaryVertex();
  Double_t tpcMulti=0.;
  if(vertexTPC){
    Double_t vertexContribTPC=vertexTPC->GetNContributors();
    tpcMulti=vertexContribTPC/150.;
    if (tpcMulti>20.) tpcMulti=20.;
  }
  
  return tpcMulti;
}

//______________________________________________________________________________
void IlcPIDResponse::SetRecoInfo()
{
  //
  // Set reconstruction information
  //
  
  //reset information
  fLHCperiod="";
  fMCperiodTPC="";
  
  fBeamType="";
    
  fBeamType="PP";
  
  TPRegexp reg(".*(LHC11[a-z]+[0-9]+[a-z_]*)/.*");
  //find the period by run number (UGLY, but not stored in ESD and AOD... )
  if (fRun>=114737&&fRun<=117223)      { fLHCperiod="LHC10B"; fMCperiodTPC="LHC10D1";  }
  else if (fRun>=118503&&fRun<=121040) { fLHCperiod="LHC10C"; fMCperiodTPC="LHC10D1";  }
  else if (fRun>=122195&&fRun<=126437) { fLHCperiod="LHC10D"; fMCperiodTPC="LHC10F6A"; }
  else if (fRun>=127710&&fRun<=130850) { fLHCperiod="LHC10E"; fMCperiodTPC="LHC10F6A"; }
  else if (fRun>=133004&&fRun<=135029) { fLHCperiod="LHC10F"; fMCperiodTPC="LHC10F6A"; }
  else if (fRun>=135654&&fRun<=136377) { fLHCperiod="LHC10G"; fMCperiodTPC="LHC10F6A"; }
  else if (fRun>=136851&&fRun<=139517) {
    fLHCperiod="LHC10H";
    fMCperiodTPC="LHC10H8";
    if (reg.MatchB(fCurrentFile)) fMCperiodTPC="LHC11A10";
    fBeamType="PBPB";
  }
  else if (fRun>=139699&&fRun<=146860) { fLHCperiod="LHC11A"; fMCperiodTPC="LHC10F6A"; }
  //TODO: periods 11B, 11C are not yet treated assume 11d for the moment
  else if (fRun>=148531&&fRun<=155384) { fLHCperiod="LHC11D"; fMCperiodTPC="LHC10F6A"; }
  else if (fRun>=156477&&fRun<=159635) { fLHCperiod="LHC11D"; fMCperiodTPC="LHC10F6A"; }
  else if (fRun>=166529) {
    fLHCperiod="LHC11H";
    fMCperiodTPC="LHC11A10";
    fBeamType="PBPB";
  }


  //exception new pp MC productions from 2011
  if (fBeamType=="PP" && reg.MatchB(fCurrentFile)) fMCperiodTPC="LHC11B2";
}

//______________________________________________________________________________
void IlcPIDResponse::SetITSParametrisation()
{
  //
  // Set the ITS parametrisation
  //
}

//______________________________________________________________________________
void IlcPIDResponse::SetTPCPidResponseMaster()
{
  //
  // Load the TPC pid response functions from the OADB
  //
  //don't load twice for the moment
   if (fArrPidResponseMaster) return;
 

  //reset the PID response functions
  delete fArrPidResponseMaster;
  fArrPidResponseMaster=0x0;
  
  TString fileName(Form("%s/COMMON/PID/data/TPCPIDResponse.root", fOADBPath.Data()));
  
  TFile *f=TFile::Open(fileName.Data());
  if (f && f->IsOpen() && !f->IsZombie()){
    fArrPidResponseMaster=dynamic_cast<TObjArray*>(f->Get("TPCPIDResponse"));
  }
  delete f;
  
  if (!fArrPidResponseMaster){
    IlcFatal(Form("Could not retrieve the TPC pid response from: %s",fileName.Data()));
    return;
  }
  fArrPidResponseMaster->SetOwner();
}

//______________________________________________________________________________
void IlcPIDResponse::SetTPCParametrisation()
{
  //
  // Change BB parametrisation for current run
  //
  
  if (fLHCperiod.IsNull()) {
    IlcFatal("No period set, not changing parametrisation");
    return;
  }
  
  //
  // Set default parametrisations for data and MC
  //
  
  //data type
  TString datatype="DATA";
  //in case of mc fRecoPass is per default 1
  if (fIsMC) {
    datatype="MC";
    fRecoPass=1;
  }
  
  //
  //reset old splines
  //
  for (Int_t ispec=0; ispec<IlcPID::kSPECIES; ++ispec){
    fTPCResponse.SetResponseFunction((IlcPID::EParticleType)ispec,0x0);
  }
  
  //
  //set the new PID splines
  //
  TString period=fLHCperiod;
  if (fArrPidResponseMaster){
    TObject *grAll=0x0;
    //for MC don't use period information
//     if (fIsMC) period="[A-Z0-9]*";
    //for MC use MC period information
    if (fIsMC) period=fMCperiodTPC;
//pattern for the default entry (valid for all particles)
    TPRegexp reg(Form("TSPLINE3_%s_([A-Z]*)_%s_PASS%d_%s_MEAN",datatype.Data(),period.Data(),fRecoPass,fBeamType.Data()));
    
    //loop over entries and filter them
    for (Int_t iresp=0; iresp<fArrPidResponseMaster->GetEntriesFast();++iresp){
      TObject *responseFunction=fArrPidResponseMaster->At(iresp);
      if (responseFunction==0x0) continue;
      TString responseName=responseFunction->GetName();
      
      if (!reg.MatchB(responseName)) continue;
      
      TObjArray *arr=reg.MatchS(responseName);
      TString particleName=arr->At(1)->GetName();
      delete arr;
      if (particleName.IsNull()) continue;
      if (particleName=="ALL") grAll=responseFunction;
      else {
        //find particle id
        for (Int_t ispec=0; ispec<IlcPID::kSPECIES; ++ispec){
          TString particle=IlcPID::ParticleName(ispec);
          particle.ToUpper();
          if ( particle == particleName ){
            fTPCResponse.SetResponseFunction((IlcPID::EParticleType)ispec,responseFunction);
            fTPCResponse.SetUseDatabase(kTRUE);
            IlcInfo(Form("Adding graph: %d - %s",ispec,responseFunction->GetName()));
            break;
          }
        }
      }
    }
    
    //set default response function to all particles which don't have a specific one
    if (grAll){
      for (Int_t ispec=0; ispec<IlcPID::kSPECIES; ++ispec){
        if (!fTPCResponse.GetResponseFunction((IlcPID::EParticleType)ispec)){
          fTPCResponse.SetResponseFunction((IlcPID::EParticleType)ispec,grAll);
          IlcInfo(Form("Adding graph: %d - %s",ispec,grAll->GetName()));
        }
      }
    }
  }
  
  //
  // Setup resolution parametrisation
  //
  
  //default
  fTPCResponse.SetSigma(3.79301e-03, 2.21280e+04);
  
  if (fRun>=122195){
    fTPCResponse.SetSigma(2.30176e-02, 5.60422e+02);
  }
  if (fArrPidResponseMaster)
  fResolutionCorrection=(TF1*)fArrPidResponseMaster->FindObject(Form("TF1_%s_ALL_%s_PASS%d_%s_SIGMA",datatype.Data(),period.Data(),fRecoPass,fBeamType.Data()));
  
  if (fResolutionCorrection) IlcInfo(Form("Setting multiplicity correction function: %s",fResolutionCorrection->GetName()));
}

//______________________________________________________________________________
void IlcPIDResponse::SetTRDPidResponseMaster()
{
  //
  // Load the TRD pid params and references from the OADB
  //
  if(fTRDPIDResponseObject) return;
  IlcOADBContainer contParams("contParams"); 

  Int_t statusResponse = contParams.InitFromFile(Form("%s/COMMON/PID/data/TRDPIDResponse.root", fOADBPath.Data()), "IlcTRDPIDResponseObject");
  if(statusResponse){
    IlcError("Failed initializing PID Response Object from OADB");
  } else {
    IlcInfo(Form("Loading TRD Response from %s/COMMON/PID/data/TRDPIDResponse.root", fOADBPath.Data()));
    fTRDPIDResponseObject = dynamic_cast<IlcTRDPIDResponseObject *>(contParams.GetObject(fRun));
    if(!fTRDPIDResponseObject){
      IlcError(Form("TRD Response not found in run %d", fRun));
    }
  }
  /*
  IlcOADBContainer contRefs("contRefs");
  Int_t statusRefs = contRefs.InitFromFile(Form("%s/COMMON/PID/data/TRDPIDReferenceLQ1D.root", fOADBPath.Data()), "IlcTRDPIDReference");
  if(statusRefs){
    IlcInfo("Failed Loading References for TRD");
  } else {
    IlcInfo(Form("Loading TRD References from %s/COMMON/PID/data/TRDPIDReferenceLQ1D.root", fOADBPath.Data()));
    fTRDPIDReference = dynamic_cast<IlcTRDPIDReference *>(contRefs.GetObject(fRun));
    if(!fTRDPIDReference){
      IlcError(Form("TRD References not found in OADB Container for run %d", fRun));
    }
    }
    */
}

//______________________________________________________________________________
void IlcPIDResponse::InitializeTRDResponse(){
  //
  // Set PID Params and references to the TRD PID response
  // 
    fTRDResponse.SetPIDResponseObject(fTRDPIDResponseObject);
    SetTRDPIDmethod();
}

void IlcPIDResponse::SetTRDPIDmethod(IlcTRDPIDResponse::ETRDPIDMethod method){
  
  fTRDResponse.SetPIDmethod(method);
  if(fLHCperiod == "LHC10d" || fLHCperiod == "LHC10e"){
    // backward compatibility for setting with 8 slices
    fTRDslicesForPID[0] = 0;
    fTRDslicesForPID[1] = 7;
  }
  else{
    if(method==IlcTRDPIDResponse::kLQ1D){
      fTRDslicesForPID[0] = 0; // first Slice contains normalized dEdx
      fTRDslicesForPID[1] = 0;
    }
    if(method==IlcTRDPIDResponse::kLQ2D){
      fTRDslicesForPID[0] = 1;
      fTRDslicesForPID[1] = 7;
    }
  }
  IlcDebug(1,Form("Slice Range set to %d - %d",fTRDslicesForPID[0],fTRDslicesForPID[1]));
}

//______________________________________________________________________________
void IlcPIDResponse::SetTOFPidResponseMaster()
{
  //
  // Load the TOF pid params from the OADB
  //
  TFile *oadbf = new TFile(Form("%s/COMMON/PID/data/TOFPIDParams.root",fOADBPath.Data()));
  if (oadbf->IsOpen()) {
    IlcInfo(Form("Loading TOF Params from %s/COMMON/PID/data/TOFPIDParams.root", fOADBPath.Data()));
    IlcOADBContainer *oadbc = (IlcOADBContainer *)oadbf->Get("TOFoadb");
    if (fTOFPIDParams) delete fTOFPIDParams;
    fTOFPIDParams = dynamic_cast<IlcTOFPIDParams *>(oadbc->GetObject(fRun,"TOFparams"));
    oadbf->Close();
    delete oadbc;
  } else {
    IlcError(Form("TOFPIDParams.root not found in %s/COMMON/PID/data !!",fOADBPath.Data()));
  }
  delete oadbf;

  }

//______________________________________________________________________________
void IlcPIDResponse::InitializeTOFResponse(){
  //
  // Set PID Params to the TOF PID response
  // 
  TString stMethod[4]={"kFILL_T0","kTOF_T0","kT0_T0","kBest_T0"};
  for (Int_t i=0;i<4;i++) {
    fTOFResponse.SetTrackParameter(i,fTOFPIDParams->GetSigParams(i));
  }
  fTOFResponse.SetTimeResolution(fTOFPIDParams->GetTOFresolution());

}


//_________________________________________________________________________
Bool_t IlcPIDResponse::IdentifiedAsElectronTRD(const IlcVTrack *vtrack, Double_t efficiencyLevel) const {
  //
  // Check whether track is identified as electron under a given electron efficiency hypothesis
  //
  Double_t probs[IlcPID::kSPECIES];
  ComputeTRDProbability(vtrack, IlcPID::kSPECIES, probs);

  Int_t ntracklets = vtrack->GetTRDntrackletsPID();
  // Take mean of the TRD momenta in the given tracklets
  Float_t p = 0, trdmomenta[IlcVTrack::kTRDnPlanes];
  Int_t nmomenta = 0;
  for(Int_t iPl=0;iPl<IlcVTrack::kTRDnPlanes;iPl++){
    if(vtrack->GetTRDmomentum(iPl) > 0.){
      trdmomenta[nmomenta++] = vtrack->GetTRDmomentum(iPl); 
    }
  }
  p = TMath::Mean(nmomenta, trdmomenta);

  return fTRDResponse.IdentifiedAsElectron(ntracklets, probs, p, efficiencyLevel);
}

//______________________________________________________________________________
void IlcPIDResponse::SetEMCALPidResponseMaster()
{
  //
  // Load the EMCAL pid response functions from the OADB
  //
  TObjArray* fEMCALPIDParamsRun      = NULL;
  TObjArray* fEMCALPIDParamsPass     = NULL;

  if(fEMCALPIDParams) return;
  IlcOADBContainer contParams("contParams"); 

  Int_t statusPars = contParams.InitFromFile(Form("%s/COMMON/PID/data/EMCALPIDParams.root", fOADBPath.Data()), "IlcEMCALPIDParams");
  if(statusPars){
    IlcError("Failed initializing PID Params from OADB");
  } 
  else {
    IlcInfo(Form("Loading EMCAL Params from %s/COMMON/PID/data/EMCALPIDParams.root", fOADBPath.Data()));

    fEMCALPIDParamsRun = dynamic_cast<TObjArray *>(contParams.GetObject(fRun));
    if(fEMCALPIDParamsRun)  fEMCALPIDParamsPass = dynamic_cast<TObjArray *>(fEMCALPIDParamsRun->FindObject(Form("pass%d",fRecoPass)));
    if(fEMCALPIDParamsPass) fEMCALPIDParams     = dynamic_cast<TObjArray *>(fEMCALPIDParamsPass->FindObject(Form("EMCALPIDParams_Particles")));

    if(!fEMCALPIDParams){
      IlcError(Form("EMCAL Params not found in run %d pass %d", fRun, fRecoPass));
      IlcInfo("Will take the standard LHC11d instead ...");

      fEMCALPIDParamsRun = dynamic_cast<TObjArray *>(contParams.GetObject(156477));
      if(fEMCALPIDParamsRun)  fEMCALPIDParamsPass = dynamic_cast<TObjArray *>(fEMCALPIDParamsRun->FindObject(Form("pass%d",1)));
      if(fEMCALPIDParamsPass) fEMCALPIDParams     = dynamic_cast<TObjArray *>(fEMCALPIDParamsPass->FindObject(Form("EMCALPIDParams_Particles")));

      if(!fEMCALPIDParams){
	IlcError(Form("DEFAULT EMCAL Params (LHC11d) not found in file %s/COMMON/PID/data/EMCALPIDParams.root", fOADBPath.Data()));	
      }
    }
  }
}

//______________________________________________________________________________
void IlcPIDResponse::InitializeEMCALResponse(){
  //
  // Set PID Params to the EMCAL PID response
  // 
  fEMCALResponse.SetPIDParams(fEMCALPIDParams);

}

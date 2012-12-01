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


//-----------------------------------------------------------------
//        Base class for combining PID of different detectors    //
//        (user selected) and compute Bayesian probabilities     //
//                                                               //
//                                                               //
//   Origin: Pietro Antonioli, INFN-BO Pietro.Antonioli@cern.ch  //
//                                                               //
//-----------------------------------------------------------------
#ifdef WIN32
#define CVG_NOVECTORS
#ifndef CVG_NOVECTORS
	#include <vector>
#endif
#endif

#include <TH1.h>

#include <IlcVTrack.h>
#include <IlcLog.h>
#include <IlcPID.h>
#include <IlcPIDResponse.h>

#include "IlcPIDCombined.h"

#include "TMath.h"
#include "TFile.h"

#include "IlcOADBContainer.h"

// initialize static members
TH2F* IlcPIDCombined::fDefaultPriorsTPC[5];

ClassImp(IlcPIDCombined);

IlcPIDCombined::IlcPIDCombined():
	TNamed("CombinedPID","CombinedPID"),
	fDetectorMask(0),
	fRejectMismatchMask(0x7F),
	fEnablePriors(kTRUE),
	fSelectedSpecies(IlcPID::kSPECIES),
	fUseDefaultTPCPriors(kFALSE)
{	
  //
  // default constructor
  //	
  for (Int_t i=0;i<IlcPID::kSPECIES+IlcPID::kSPECIESLN;i++) fPriorsDistributions[i]=NULL;
  IlcLog::SetClassDebugLevel("IlcPIDCombined",10);
}

//-------------------------------------------------------------------------------------------------	
IlcPIDCombined::IlcPIDCombined(const TString& name,const TString& title):
	TNamed(name,title),
	fDetectorMask(0),
	fRejectMismatchMask(0x7F),
	fEnablePriors(kTRUE),
	fSelectedSpecies(IlcPID::kSPECIES),
	fUseDefaultTPCPriors(kFALSE)
{
  //
  // default constructor with name and title
  //
  for (Int_t i=0;i<IlcPID::kSPECIES+IlcPID::kSPECIESLN;i++) fPriorsDistributions[i]=NULL;
  IlcLog::SetClassDebugLevel("IlcPIDCombined",10);

}

//-------------------------------------------------------------------------------------------------	
IlcPIDCombined::~IlcPIDCombined() {

  for(Int_t i=0;i < IlcPID::kSPECIES+IlcPID::kSPECIESLN;i++){
    if(fPriorsDistributions[i])
      delete fPriorsDistributions[i];
  }
}

//-------------------------------------------------------------------------------------------------	
void IlcPIDCombined::SetPriorDistribution(IlcPID::EParticleType type,TH1F *prior) {
  if ( (type < 0) || ( type >= (IlcPID::kSPECIESN+IlcPID::kSPECIESLN) ) ){
    IlcError(Form("Invalid EParticleType setting prior (offending type: %d)",type));
    return;
  }
  if(prior) {
    Int_t i = (Int_t)type;
    if (i >= IlcPID::kSPECIES ) {
      if (i < (Int_t)IlcPID::kDeuteron) {
	IlcError(Form("Invalid EParticleType setting prior. Type: %d (neutral) not supported",i));
	return;
      } else i -= (IlcPID::kSPECIESN-IlcPID::kSPECIES);
    }
    if (i>(IlcPID::kSPECIES+IlcPID::kSPECIESLN)) {             // coverity fix (to put it mildly....)
      IlcError(Form("Unexpected EParticleType setting prior. Type: %d (neutral) not supported",i));
      return;
    }
    if (fPriorsDistributions[i]) {
      delete fPriorsDistributions[i]; 
    }
    fPriorsDistributions[i]=new TH1F(*prior);
  }
}

//-------------------------------------------------------------------------------------------------	
UInt_t IlcPIDCombined::ComputeProbabilities(const IlcVTrack *track, const IlcPIDResponse *response, Double_t* bayesProbabilities) const {
#ifdef WIN32
	#ifdef CVG_NOVECTORS
	Double_t *pITS = (Double_t *)malloc(fSelectedSpecies);
	Double_t *pTPC = (Double_t *)malloc(fSelectedSpecies);
	Double_t *pTRD = (Double_t *)malloc(fSelectedSpecies);
	Double_t *pTOF = (Double_t *)malloc(fSelectedSpecies);
	Double_t *pHMPID = (Double_t *)malloc(fSelectedSpecies);
	Double_t *pEMCAL = (Double_t *)malloc(fSelectedSpecies);
	Double_t *pPHOS = (Double_t *)malloc(fSelectedSpecies);
	#else
	std::vector<Double_t>  pITS[fSelectedSpecies];
	std::vector<Double_t>  pTPC[fSelectedSpecies];
	std::vector<Double_t>  pTRD[fSelectedSpecies];
	std::vector<Double_t>  pTOF[fSelectedSpecies];
	std::vector<Double_t>  pHMPID[fSelectedSpecies];
	std::vector<Double_t>  pEMCAL[fSelectedSpecies];
	std::vector<Double_t>  pPHOS[fSelectedSpecies];
	#endif
#else
	Double_t pITS[fSelectedSpecies];
	Double_t pTPC[fSelectedSpecies];
	Double_t pTRD[fSelectedSpecies];
	Double_t pTOF[fSelectedSpecies];
	Double_t pHMPID[fSelectedSpecies];
	Double_t pEMCAL[fSelectedSpecies];
	Double_t pPHOS[fSelectedSpecies];
#endif
	for (Int_t i=0;i<fSelectedSpecies;i++) {
	 pITS[i]=1.;
	 pTPC[i]=1.;
	 pTRD[i]=1.;
	 pTOF[i]=1.;
	 pHMPID[i]=1.;
	 pEMCAL[i]=1.;
	 pPHOS[i]=1.;
	}
	UInt_t usedMask=0;
	IlcPIDResponse::EDetPidStatus status=IlcPIDResponse::kDetNoSignal;
#ifdef WIN32
	#ifdef CVG_NOVECTORS
	Double_t *p = (Double_t *)malloc(fSelectedSpecies);
	#else
	std::vector<Double_t>  p[fSelectedSpecies];
	#endif
#else
	Double_t p[fSelectedSpecies];
#endif
	memset(p,0,sizeof(Double_t)*fSelectedSpecies);

	// getting probability distributions for selected detectors only
	if (fDetectorMask & IlcPIDResponse::kDetITS) {
	  status = response->ComputeITSProbability(track, fSelectedSpecies, pITS);
	  SetCombinedStatus(status,&usedMask,IlcPIDResponse::kDetITS,pITS);
	}

	if (fDetectorMask & IlcPIDResponse::kDetTPC) { 
	  status = response->ComputeTPCProbability(track, fSelectedSpecies, pTPC);
	  SetCombinedStatus(status,&usedMask,IlcPIDResponse::kDetTPC,pTPC);
	}


	if (fDetectorMask & IlcPIDResponse::kDetTRD) { 
	  status = response->ComputeTRDProbability(track, fSelectedSpecies, pTRD);
	  SetCombinedStatus(status,&usedMask,IlcPIDResponse::kDetTRD,pTRD);
	}

	if (fDetectorMask &  IlcPIDResponse::kDetTOF) { 
	  status = response->ComputeTOFProbability(track, fSelectedSpecies, pTOF);
	  SetCombinedStatus(status,&usedMask,IlcPIDResponse::kDetTOF,pTOF);
	}

	if (fDetectorMask & IlcPIDResponse::kDetHMPID) { 
	  status = response->ComputeHMPIDProbability(track, fSelectedSpecies, pHMPID);
	  SetCombinedStatus(status,&usedMask,IlcPIDResponse::kDetHMPID,pHMPID);
	}


	if (fDetectorMask & IlcPIDResponse::kDetEMCAL) { 
	  status = response->ComputeEMCALProbability(track, fSelectedSpecies, pEMCAL);
	  SetCombinedStatus(status,&usedMask,IlcPIDResponse::kDetEMCAL,pEMCAL);
	}


	if (fDetectorMask & IlcPIDResponse::kDetPHOS) { 
	  status = response->ComputePHOSProbability(track, fSelectedSpecies, pPHOS);
	  SetCombinedStatus(status,&usedMask,IlcPIDResponse::kDetPHOS,pPHOS);
	}


	for (Int_t i =0; i<fSelectedSpecies; i++){
	  p[i] = pITS[i]*pTPC[i]*pTRD[i]*pTOF[i]*pHMPID[i]*pEMCAL[i]*pPHOS[i];
	}
#ifdef WIN32
	#ifdef CVG_NOVECTORS
	Double_t *priors = (Double_t *)malloc(fSelectedSpecies);
	#else
	std::vector<Double_t>  priors[fSelectedSpecies];
	#endif
#else
	Double_t priors[fSelectedSpecies];
#endif
	memset(priors,0,fSelectedSpecies*sizeof(Double_t));
	if (fEnablePriors){
	  GetPriors(track,priors,response->GetCurrentCentrality());
	  
	  // apply tof matching efficiency to priors if TOF joined PID for this track
	  if(fUseDefaultTPCPriors && (usedMask & IlcPIDResponse::kDetTOF)){
	    Double_t pt=TMath::Abs(track->Pt());
	    Float_t kaonTOFfactor = 0.1;
	    if(pt > 0.35) kaonTOFfactor = 1 - TMath::Exp(-TMath::Power(pt,4.19618E-07)/5.68017E-01)*TMath::Power(pt,-1.50705);
	    Float_t protonTOFfactor = 0.1;
	    if(pt > 0.4) protonTOFfactor = 1 - TMath::Exp(-TMath::Power(pt,3.30978)/8.57396E-02)*TMath::Power(pt,-4.42661E-01);
	    
	    if(track->Charge() < 0){ // for negative tracks
	      kaonTOFfactor *= 1 - TMath::Exp(-TMath::Power(pt,4.87912E-07)/3.26431E-01)*TMath::Power(pt,-1.22893);
	      protonTOFfactor *= 1 - TMath::Exp(-TMath::Power(pt,2.00575E-07)/4.95605E-01)*TMath::Power(pt,-6.71305E-01);
	    }
	    
	    p[3] *= kaonTOFfactor;
	    p[4] *= protonTOFfactor;
	  }
	}
	else { for (Int_t i=0;i<fSelectedSpecies;i++) priors[i]=1.;}
	ComputeBayesProbabilities(bayesProbabilities,p,priors);

#ifdef WIN32
	#ifdef CVG_NOVECTORS
	free (pITS);
	free (pTPC);
	free (pTRD);
	free (pTOF);
	free (pHMPID);
	free (pEMCAL);
	free (pPHOS);
	free (p);
	free (priors);
	#endif
#endif

	return usedMask;
}


//-------------------------------------------------------------------------------------------------
void IlcPIDCombined::GetPriors(const IlcVTrack *track, Double_t* p,Float_t centrality) const {
	
	//
	// get priors from distributions
	//
	
	Double_t pt=TMath::Abs(track->Pt());

        if(fUseDefaultTPCPriors){ // use default priors if requested
	  Float_t usedCentr = centrality+5*(centrality>0);
	  if(usedCentr < -0.99) usedCentr = -0.99;
	  else if(usedCentr > 99) usedCentr = 99;
	  if(pt > 9.99) pt = 9.99;
	  else if(pt < 0.01)  pt = 0.01;
	  
	  for(Int_t i=0;i<5;i++) p[i] = fDefaultPriorsTPC[i]->Interpolate(usedCentr,pt);

	  return;
	}
	
	Double_t sumPriors = 0;
	for (Int_t i=0;i<fSelectedSpecies;++i){
		if (fPriorsDistributions[i]){
			p[i]=fPriorsDistributions[i]->Interpolate(pt);
		}
		else {
			p[i]=0.;
		}
		sumPriors+=p[i];		
	}

	// normalizing priors
	if (sumPriors == 0) return;
	for (Int_t i=0;i<fSelectedSpecies;++i){
	   p[i]=p[i]/sumPriors;
	}
	return;
}
//-------------------------------------------------------------------------------------------------
void IlcPIDCombined::GetPriors(const IlcVTrack *track,Double_t* p,const IlcPIDResponse *response,UInt_t detUsed) const{
	
	//
	// get priors from distributions
	//
        Double_t pt=TMath::Abs(track->Pt());
	
        if(fUseDefaultTPCPriors){ // use default priors if requested
	  Float_t usedCentr = response->GetCurrentCentrality()+5*(response->GetCurrentCentrality()>0);
	  if(usedCentr < -0.99) usedCentr = -0.99;
	  else if(usedCentr > 99) usedCentr = 99;
	  if(pt > 9.99) pt = 9.99;
	  else if(pt < 0.01)  pt = 0.01;
	  
	  for(Int_t i=0;i<5;i++) p[i] = fDefaultPriorsTPC[i]->Interpolate(usedCentr,pt);

	  // Extra factor if TOF matching was required
	  if(detUsed & IlcPIDResponse::kDetTOF){
	    Float_t kaonTOFfactor = 0.1;
	    if(pt > 0.35) kaonTOFfactor = 1 - TMath::Exp(-TMath::Power(pt,4.19618E-07)/5.68017E-01)*TMath::Power(pt,-1.50705);
	    Float_t protonTOFfactor = 0.1;
	    if(pt > 0.4) protonTOFfactor = 1 - TMath::Exp(-TMath::Power(pt,3.30978)/8.57396E-02)*TMath::Power(pt,-4.42661E-01);
	    
	    if(track->Charge() < 0){ // for negative tracks
	      kaonTOFfactor *= 1 - TMath::Exp(-TMath::Power(pt,4.87912E-07)/3.26431E-01)*TMath::Power(pt,-1.22893);
	      protonTOFfactor *= 1 - TMath::Exp(-TMath::Power(pt,2.00575E-07)/4.95605E-01)*TMath::Power(pt,-6.71305E-01);
	    }
	    
	    p[3] *= kaonTOFfactor;
	    p[4] *= protonTOFfactor;
	  }

	  return;
	}


	Double_t sumPriors = 0;
	for (Int_t i=0;i<fSelectedSpecies;++i){
		if (fPriorsDistributions[i]){
			p[i]=fPriorsDistributions[i]->Interpolate(pt);
		}
		else {
			p[i]=0.;
		}
		sumPriors+=p[i];		
	}

	// normalizing priors
	if (sumPriors == 0) return;
	for (Int_t i=0;i<fSelectedSpecies;++i){
	   p[i]=p[i]/sumPriors;
	}
	return;
}
//-------------------------------------------------------------------------------------------------	
void IlcPIDCombined::ComputeBayesProbabilities(Double_t* probabilities, const Double_t* probDensity, const Double_t* prior) const {


  //
  // calculate Bayesian probabilities
  //
  Double_t sum = 0.;
  for (Int_t i = 0; i < fSelectedSpecies; i++) {
    sum += probDensity[i] * prior[i];
  }
  if (sum <= 0) {

    IlcError("Invalid probability densities or priors");
    for (Int_t i = 0; i < fSelectedSpecies; i++) probabilities[i] = -1;
    return;
  }
  for (Int_t i = 0; i < fSelectedSpecies; i++) {
    probabilities[i] = probDensity[i] * prior[i] / sum;
  }


}

//----------------------------------------------------------------------------------------
void IlcPIDCombined::SetCombinedStatus(IlcPIDResponse::EDetPidStatus status, UInt_t *maskDetIn, IlcPIDResponse::EDetCode bit, Double_t* p) const {
  switch (status) {
  case IlcPIDResponse::kDetNoSignal:
    break;
  case IlcPIDResponse::kDetPidOk:
    *maskDetIn = *maskDetIn | bit;
    break;
  case IlcPIDResponse::kDetMismatch:
    if ( fRejectMismatchMask & bit) for (Int_t j=0;j<fSelectedSpecies;j++) p[j]=1./fSelectedSpecies;
    else *maskDetIn = *maskDetIn | bit;
    break;
  }

}

//----------------------------------------------------------------------------------------
void IlcPIDCombined::SetDefaultTPCPriors(){
  fEnablePriors=kTRUE;
  fUseDefaultTPCPriors = kTRUE;

  TString oadbfilename("$ILC_ROOT/OADB/COMMON/PID/data/");
  oadbfilename += "/PIDdefaultPriors.root";
  TFile * foadb = TFile::Open(oadbfilename.Data());
  if(!foadb->IsOpen()) IlcFatal(Form("Cannot open OADB file %s", oadbfilename.Data()));
  
  IlcOADBContainer * priorsContainer = (IlcOADBContainer*) foadb->Get("priorsTPC");
  if (!priorsContainer) IlcFatal("Cannot fetch OADB container for Priors");
  
  const char *namespecies[5] = {"El","Mu","Pi","Ka","Pr"};
  char name[100];

  for(Int_t i=0;i < 5;i++){
    snprintf(name,99,"hDefault%sPriors",namespecies[i]);
    fDefaultPriorsTPC[i] = (TH2F*) priorsContainer->GetDefaultObject(name);
    if (!fDefaultPriorsTPC[i]) IlcFatal(Form("Cannot find priors for %s", namespecies[i]));
  }
}

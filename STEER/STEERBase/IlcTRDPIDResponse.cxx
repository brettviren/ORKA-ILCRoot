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
//
// PID Response class for the TRD detector
// Based on 1D Likelihood approach
// Calculation of probabilities using Bayesian approach
// Attention: This method is only used to separate electrons from pions
//
// Authors:
//  Markus Fasel <M.Fasel@gsi.de>
//  Anton Andronic <A.Andronic@gsi.de>
//
#include <TAxis.h>
#include <TClass.h>
#include <TDirectory.h>
#include <TFile.h>
#include <TH1.h>
#include <TKey.h>
#include <TMath.h>
#include <TObjArray.h>
#include <TROOT.h> 
#include <TString.h>
#include <TSystem.h>

#include "IlcLog.h"

#include "IlcTRDPIDResponseObject.h"
#include "IlcTRDPIDResponse.h"
#include "IlcTRDTKDInterpolator.h"

ClassImp(IlcTRDPIDResponse)

//____________________________________________________________
IlcTRDPIDResponse::IlcTRDPIDResponse():
  TObject()
  ,fkPIDResponseObject(NULL)
  ,fGainNormalisationFactor(1.)
  ,fPIDmethod(kLQ1D)
{
  //
  // Default constructor
  //
}

//____________________________________________________________
IlcTRDPIDResponse::IlcTRDPIDResponse(const IlcTRDPIDResponse &ref):
  TObject(ref)
  ,fkPIDResponseObject(NULL)
  ,fGainNormalisationFactor(ref.fGainNormalisationFactor)
  ,fPIDmethod(ref.fPIDmethod)
{
  //
  // Copy constructor
  //
}

//____________________________________________________________
IlcTRDPIDResponse &IlcTRDPIDResponse::operator=(const IlcTRDPIDResponse &ref){
  //
  // Assignment operator
  //
  if(this == &ref) return *this;
  
  // Make copy
  TObject::operator=(ref);
  fGainNormalisationFactor = ref.fGainNormalisationFactor;
  fkPIDResponseObject = ref.fkPIDResponseObject;
  fPIDmethod = ref.fPIDmethod;
  
  return *this;
}

//____________________________________________________________
IlcTRDPIDResponse::~IlcTRDPIDResponse(){
  //
  // Destructor
  //
    if(IsOwner()) delete fkPIDResponseObject;
}

//____________________________________________________________
Bool_t IlcTRDPIDResponse::Load(const Char_t * filename){
  //
  // Load References into the toolkit
  //
  IlcDebug(1, "Loading reference histos from root file");
  TDirectory *owd = gDirectory;// store old working directory
  
  if(!filename)
    filename = Form("%s/STEER/LQ1dRef_v1.root",gSystem->ExpandPathName("$ILC_ROOT"));
  TFile *in = TFile::Open(filename);
  if(!in){
    IlcError("Ref file not available.");
    return kFALSE;
  }
  
  gROOT->cd();
  fkPIDResponseObject = dynamic_cast<const IlcTRDPIDResponseObject *>(in->Get("TRDPIDResponse")->Clone());
  in->Close(); delete in;
  owd->cd();
  SetBit(kIsOwner, kTRUE);
  IlcDebug(2, Form("Successfully loaded References for %d Momentum bins", fkPIDResponseObject->GetNumberOfMomentumBins()));
  return kTRUE;
}

//____________________________________________________________
Bool_t IlcTRDPIDResponse::GetResponse(Int_t n, const Double_t * const dedx, const Float_t * const p, Double_t prob[IlcPID::kSPECIES], Bool_t kNorm) const
{
  //
  // Calculate TRD likelihood values for the track based on dedx and 
  // momentum values. The likelihoods are calculated by query the 
  // reference data depending on the PID method selected
  //
  // Input parameter :
  //   n - number of dedx slices/chamber
  //   dedx - array of dedx slices organized layer wise
  //   p - array of momentum measurements organized layer wise
  // 
  // Return parameters
  //   prob - probabilities allocated by TRD for particle specis
  //   kNorm - switch to normalize probabilities to 1. By default true. If false return not normalized prob.
  // 
  // Return value
  //   true if calculation success
  // 

    if(!fkPIDResponseObject){
	IlcWarning("Missing reference data. PID calculation not possible.");
	return kFALSE;
    }

    for(Int_t is(IlcPID::kSPECIES); is--;) prob[is]=.2;
    Double_t prLayer[IlcPID::kSPECIES];
    Double_t dE[10], s(0.);
    for(Int_t il(kNlayer); il--;){
	memset(prLayer, 0, IlcPID::kSPECIES*sizeof(Double_t));
	if(!CookdEdx(n, &dedx[il*n], &dE[0])) continue;

	s=0.;
        Bool_t filled=kTRUE;
	for(Int_t is(IlcPID::kSPECIES); is--;){
	    if((fPIDmethod==kLQ2D)&&(!(is==0||is==2)))continue;
	    if((dE[0] > 0.) && (p[il] > 0.)) prLayer[is] = GetProbabilitySingleLayer(is, p[il], &dE[0]);
	    IlcDebug(3, Form("Probability for Species %d in Layer %d: %e", is, il, prLayer[is]));
	    if(prLayer[is]<1.e-30){
		IlcDebug(2, Form("Null for species %d species prob layer[%d].",is,il));
		filled=kFALSE;
		break;
	    }
	    s+=prLayer[is];
	}
	if(!filled){
	    continue;
	}
	if(s<1.e-30){
	    IlcDebug(2, Form("Null all species prob layer[%d].", il));
	    continue;
	}
	for(Int_t is(IlcPID::kSPECIES); is--;){
	    if(kNorm) prLayer[is] /= s;
	    prob[is] *= prLayer[is];
	}
    }
    if(!kNorm) return kTRUE;

    s=0.;
    for(Int_t is(IlcPID::kSPECIES); is--;) s+=prob[is];
    if(s<1.e-30){
	IlcDebug(2, "Null total prob.");
	return kFALSE;
    }
    for(Int_t is(IlcPID::kSPECIES); is--;) prob[is]/=s;
    return kTRUE;
}

//____________________________________________________________
Double_t IlcTRDPIDResponse::GetProbabilitySingleLayer(Int_t species, Double_t plocal, Double_t *dEdx) const {
  //
  // Get the non-normalized probability for a certain particle species as coming
  // from the reference histogram
  // Interpolation between momentum bins
  //
  IlcDebug(1, Form("Make Probability for Species %d with Momentum %f", species, plocal));

  Double_t probLayer = 0.;
  Float_t pLower, pUpper;
	
  switch(fPIDmethod){
  case kNN: // NN
      break;
  case kLQ2D: // 2D LQ
      {
	  if(species==0||species==2){ // references only for electrons and pions
	      Double_t error;
	      Double_t point[kNslicesLQ2D];
	      for(Int_t idim=0;idim<kNslicesLQ2D;idim++){point[idim]=dEdx[idim];}

	      IlcTRDTKDInterpolator *refLower = dynamic_cast<IlcTRDTKDInterpolator*>(fkPIDResponseObject->GetLowerReference((IlcPID::EParticleType)species, plocal, pLower,kLQ2D));

	      if(refLower){
		  refLower->Eval(point,probLayer,error);
	      }
	      else {
		  IlcError("No references available");
	      }
	      IlcDebug(2,Form("Eval 2D Q0 %f Q1 %f P %e Err %e",point[0],point[1],probLayer,error));
	  }
      }
      break;
  case kLQ1D: // 1D LQ
      {
	  TH1 *refUpper = dynamic_cast<TH1 *>(fkPIDResponseObject->GetUpperReference((IlcPID::EParticleType)species, plocal, pUpper,kLQ1D)),
	      *refLower = dynamic_cast<TH1 *>(fkPIDResponseObject->GetLowerReference((IlcPID::EParticleType)species, plocal, pLower,kLQ1D));
	  // Do Interpolation exept for underflow and overflow
	  if(refLower && refUpper){
	      Double_t probLower = refLower->GetBinContent(refLower->GetXaxis()->FindBin(dEdx[0]));
	      Double_t probUpper = refUpper->GetBinContent(refUpper->GetXaxis()->FindBin(dEdx[0]));

	      probLayer = probLower + (probUpper - probLower)/(pUpper-pLower) * (plocal - pLower);
	  } else if(refLower){
	      // underflow
	      probLayer = refLower->GetBinContent(refLower->GetXaxis()->FindBin(dEdx[0]));
	  } else if(refUpper){
	      // overflow
	      probLayer = refUpper->GetBinContent(refUpper->GetXaxis()->FindBin(dEdx[0]));
	  } else {
	      IlcError("No references available");
	  }
	  IlcDebug(1, Form("Eval 1D dEdx %f Probability %e", dEdx[0],probLayer));
      }
      break;
  default:
      break;
  }
  return probLayer;
}

//____________________________________________________________
void IlcTRDPIDResponse::SetOwner(){
  //
  // Make Deep Copy of the Reference Histograms
  //
    if(!fkPIDResponseObject || IsOwner()) return;
    const IlcTRDPIDResponseObject *tmp = fkPIDResponseObject;
    fkPIDResponseObject = dynamic_cast<const IlcTRDPIDResponseObject *>(tmp->Clone());
    SetBit(kIsOwner, kTRUE);
}

//____________________________________________________________
Bool_t IlcTRDPIDResponse::CookdEdx(Int_t nSlice, const Double_t * const in, Double_t *out) const
{
    //
    // Recalculate dE/dx
    //
  switch(fPIDmethod){
  case kNN: // NN 
      break;
  case kLQ2D: // 2D LQ
      out[0]=0;
      out[1]=0;
      for(Int_t islice = 0; islice < nSlice; islice++){
	  if(islice<fkPIDResponseObject->GetNSlicesQ0())out[0]+= in[islice];
	  else out[1]+= in[islice];
      }
      if(out[0] < 1e-6) return kFALSE;
      IlcDebug(3,Form("CookdEdx Q0 %f Q1 %f",out[0],out[1]));
      break;
  case kLQ1D: // 1D LQ
      out[0]= 0.;
      for(Int_t islice = 0; islice < nSlice; islice++)
	  if(in[islice] > 0) out[0] += in[islice] * fGainNormalisationFactor;   // Protect against negative values for slices having no dE/dx information
      if(out[0] < 1e-6) return kFALSE;
      IlcDebug(3,Form("CookdEdx dEdx %f",out[0]));
      break;

  default:
      return kFALSE;
  }
  return kTRUE;
}

//____________________________________________________________
Bool_t IlcTRDPIDResponse::IdentifiedAsElectron(Int_t nTracklets, const Double_t *like, Double_t p, Double_t level) const {
    //
    // Check whether particle is identified as electron assuming a certain electron efficiency level
    // Only electron and pion hypothesis is taken into account
    //
    // Inputs:
    //         Number of tracklets
    //         Likelihood values
    //         Momentum
    //         Electron efficiency level
    //
    // If the function fails when the params are not accessible, the function returns true
    //
  if(!fkPIDResponseObject){
    IlcError("No PID Param object available");
    return kTRUE;
  } 
  Double_t probEle = like[IlcPID::kElectron]/(like[IlcPID::kElectron] + like[IlcPID::kPion]);
  Double_t params[4];
  if(!fkPIDResponseObject->GetThresholdParameters(nTracklets, level, params,fPIDmethod)){
    IlcError("No Params found for the given configuration");
    return kTRUE;
  }
  Double_t threshold = 1. - params[0] - params[1] * p - params[2] * TMath::Exp(-params[3] * p);
  if(probEle > TMath::Max(TMath::Min(threshold, 0.99), 0.2)) return kTRUE; // truncate the threshold upperwards to 0.999 and lowerwards to 0.2 and exclude unphysical values
  return kFALSE;
}

//____________________________________________________________
Bool_t IlcTRDPIDResponse::SetPIDResponseObject(const IlcTRDPIDResponseObject * obj){

    fkPIDResponseObject = obj;
    if((IlcLog::GetDebugLevel("",IsA()->GetName()))>0)fkPIDResponseObject->Print("");
    return kTRUE;
}

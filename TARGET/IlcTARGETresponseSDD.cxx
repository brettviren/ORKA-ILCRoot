/**************************************************************************
 * Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 *                                                                        *
// Author: The ILC Off-line Project. 
 // Part of the code has been developed by Alice Off-line Project. 
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
#include <TMath.h>
#include "IlcTARGETresponseSDD.h"
//////////////////////////////////////////////////////
//  Base response class forTARGET                      //
//  It is used to set static data members           //
//  connected to parameters equal for all           //
//  the modules                                     //
//                                                  //
//                                                  //
//////////////////////////////////////////////////////


const Int_t IlcTARGETresponseSDD::fgkMaxAdcDefault = 1024;
const Double_t IlcTARGETresponseSDD::fgkDynamicRangeDefault = 132.;
const Double_t IlcTARGETresponseSDD::fgkfChargeLossDefault = 0;
const Float_t IlcTARGETresponseSDD::fgkDiffCoeffDefault = 3.23;
const Float_t IlcTARGETresponseSDD::fgkDiffCoeff1Default = 30.;
const TString IlcTARGETresponseSDD::fgkParam1Default = "same";
const TString IlcTARGETresponseSDD::fgkParam2Default = "same";
const TString IlcTARGETresponseSDD::fgkOptionDefault = "1D";
const Double_t IlcTARGETresponseSDD::fgkDriftSpeedDefault = 7.3;
const Double_t IlcTARGETresponseSDD::fgkNsigmasDefault = 3.;
const Int_t IlcTARGETresponseSDD::fgkNcompsDefault = 121;

ClassImp(IlcTARGETresponseSDD)

//_________________________________________________________________________
IlcTARGETresponseSDD::IlcTARGETresponseSDD():IlcTARGETresponse(){
  // default constructor
  fGaus = 0;
  SetMaxAdc(fgkMaxAdcDefault);
  SetDiffCoeff(fgkDiffCoeffDefault,fgkDiffCoeff1Default);
  SetDriftSpeed(fgkDriftSpeedDefault);
  SetNSigmaIntegration(fgkNsigmasDefault);
  //  SetNLookUp(fgkNcompsDefault);

  SetJitterError();
  SetElectronics();
  SetDynamicRange(fgkDynamicRangeDefault);
  SetChargeLoss(fgkfChargeLossDefault);
  SetParamOptions(fgkParam1Default.Data(),fgkParam2Default.Data());
  SetZeroSupp(fgkOptionDefault);
  SetDo10to8();
  SetOutputOption();
}


//______________________________________________________________________
IlcTARGETresponseSDD::~IlcTARGETresponseSDD() { 

  if(fGaus) delete fGaus;
}

//______________________________________________________________________
IlcTARGETresponseSDD::IlcTARGETresponseSDD(const IlcTARGETresponseSDD &ob) : IlcTARGETresponse(ob) {
  // Copy constructor
  // Copies are not allowed. The method is protected to avoid misuse.
  Error("IlcTARGETresponseSDD","Copy constructor not allowed\n");
}

//______________________________________________________________________
IlcTARGETresponseSDD& IlcTARGETresponseSDD::operator=(const IlcTARGETresponseSDD& /* ob */){
  // Assignment operator
  // Assignment is not allowed. The method is protected to avoid misuse.
  Error("= operator","Assignment operator not allowed\n");
  return *this;
}

//______________________________________________________________________
Int_t IlcTARGETresponseSDD::Convert8to10(Int_t signal) const {
  // Undo the lossive 10 to 8 bit compression.
  // code from Davide C. and Albert W.

  if(Do10to8()){  // kTRUE if the compression is active
    if (signal < 0 || signal > 255) {
      Warning("Convert8to10","out of range signal=%d",signal);
      return 0;
    } // end if signal <0 || signal >255

    if (signal < 128) return signal;
    if (signal < 192) {
      if (TMath::Odd(signal)) return (128+((signal-128)<<1));
      else  return (128+((signal-128)<<1)+1);
    } // end if signal < 192
    if (signal < 224) {
      if (TMath::Odd(signal)) return (256+((signal-192)<<3)+3);
      else  return (256+((signal-192)<<3)+4);
    } // end if signal < 224
    if (TMath::Odd(signal)) return (512+((signal-224)<<4)+7);
    return (512+((signal-224)<<4)+8);
  }
  else {  
    return signal;
  }
}

//________________________________________________________________________
void IlcTARGETresponseSDD::SetNLookUp(Int_t p1){
  // Set number of sigmas over which cluster disintegration is performed
  fNcomps=p1;
  if (fGaus) delete fGaus;
  fGaus = new TArrayF(fNcomps+1);
  for(Int_t i=0; i<=fNcomps; i++) {
    Double_t x = -fNsigmas + (2.*i*fNsigmas)/(fNcomps-1);
    (*fGaus)[i] = exp(-((x*x)/2));
  }
}

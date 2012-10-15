/**************************************************************************
 * Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 *                                                                         *
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


#include "IlcTARGETresponseSSD.h"
//////////////////////////////////////////////////
//  Base response class for TARGET                 //
//  It is used to set static data members       //
//  connected to parameters equal for all       //
//  the SSD modules                             //
//////////////////////////////////////////////////


const Float_t IlcTARGETresponseSSD::fgkDiffCoeffDefault = 0.;
const TString IlcTARGETresponseSSD::fgkOption1Default = "";
const TString IlcTARGETresponseSSD::fgkOption2Default = "";
const Double_t IlcTARGETresponseSSD::fgkfCouplingPR = 0.01;
const Double_t IlcTARGETresponseSSD::fgkfCouplingPL = 0.01;
const Double_t IlcTARGETresponseSSD::fgkfCouplingNR = 0.01;
const Double_t IlcTARGETresponseSSD::fgkfCouplingNL = 0.01;
const Int_t IlcTARGETresponseSSD::fgkZSThreshold = 3;


ClassImp(IlcTARGETresponseSSD)

//______________________________________________________________________
IlcTARGETresponseSSD::IlcTARGETresponseSSD():IlcTARGETresponse(){
    // Default Constructor

  SetDiffCoeff(fgkDiffCoeffDefault,0.);
  SetParamOptions(fgkOption1Default.Data(),fgkOption2Default.Data());
  SetADCpereV();
  SetCouplings(fgkfCouplingPR,fgkfCouplingPL,fgkfCouplingNR,fgkfCouplingNL);
  SetZSThreshold(fgkZSThreshold);
}

//______________________________________________________________________
IlcTARGETresponseSSD::IlcTARGETresponseSSD(const IlcTARGETresponseSSD &ob) : IlcTARGETresponse(ob) {
  // Copy constructor
  // Copies are not allowed. The method is protected to avoid misuse.
  Error("IlcTARGETresponseSSD","Copy constructor not allowed\n");
}

//______________________________________________________________________
IlcTARGETresponseSSD& IlcTARGETresponseSSD::operator=(const IlcTARGETresponseSSD& /* ob */){
  // Assignment operator
  // Assignment is not allowed. The method is protected to avoid misuse.
  Error("= operator","Assignment operator not allowed\n");
  return *this;
}


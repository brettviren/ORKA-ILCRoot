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
///////////////////////////////////////////////////////////////////////////
//  Base Response class forTARGET                      
//  It is used to set static data members           
//  connected to parameters equal for all           
//  the SPD modules                                 
//
//  Modified by D. Elia, G.E. Bruno
//  March-April 2006
//
///////////////////////////////////////////////////////////////////////////

#include "IlcTARGETresponseSPD.h"

const Float_t IlcTARGETresponseSPD::fgkDiffCoeffDefault = 0.; //change this
const TString IlcTARGETresponseSPD::fgkCouplingOptDefault = "old";
const Float_t IlcTARGETresponseSPD::fgkCouplingColDefault = 0;
const Float_t IlcTARGETresponseSPD::fgkCouplingRowDefault = 0.047;
const Float_t IlcTARGETresponseSPD::fgkEccentricityDiffDefault = 0.85;
//geb const Float_t IlcTARGETresponseSPD::fgkDistanceOverVoltage = 0.0010;

ClassImp(IlcTARGETresponseSPD)	
//______________________________________________________________________
IlcTARGETresponseSPD::IlcTARGETresponseSPD():
  IlcTARGETresponse(){


  // constructor
  SetCouplingParam(fgkCouplingColDefault,fgkCouplingRowDefault);
  SetCouplingOption(fgkCouplingOptDefault);
  SetDiffCoeff(fgkDiffCoeffDefault,0.);
  SetSigmaDiffusionAsymmetry(fgkEccentricityDiffDefault);

}

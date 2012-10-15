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

//***********************************************************
// Class AliTODPIDParams
// class to store PID parameters for TOF in OADB
// Author: P. Antonioli, pietro.antonioli@to.infn.it
//***********************************************************

#include <Riostream.h>
#include <TNamed.h>
#include "IlcLog.h"
#include "IlcTOFPIDParams.h"

ClassImp(IlcTOFPIDParams)

//_____________________________________________________________________________
IlcTOFPIDParams::IlcTOFPIDParams():
TNamed("default",""),
  fStartTime(IlcPIDResponse::kBest_T0),
  fTOFresolution(100)
{
  for (Int_t i=0;i<kSigPparams;i++) fSigPparams[i]=0;
}

//_____________________________________________________________________________
IlcTOFPIDParams::IlcTOFPIDParams(Char_t *name):
  TNamed(name,""),
  fStartTime(IlcPIDResponse::kBest_T0),
  fTOFresolution(100)
{
  for (Int_t i=0;i<kSigPparams;i++) fSigPparams[i]=0;
}

//_____________________________________________________________________________
IlcTOFPIDParams::~IlcTOFPIDParams(){
}


//_____________________________________________________________________________
void IlcTOFPIDParams::SetSigPparams(Float_t *d) 
{
  //
  // Setting the SigP values
  //
  if (d == 0x0){
    IlcError(Form("Null pointer passed"));
  }
  else{
    for (Int_t i=0;i<kSigPparams;i++) fSigPparams[i]=d[i];
  }
  return;
}


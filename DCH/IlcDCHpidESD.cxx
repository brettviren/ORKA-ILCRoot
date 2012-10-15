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

//-----------------------------------------------------------------
//           Implementation of the DCH PID class
// Assigns the electron and pion liklihoods for each ESD track.
// The function MakePID(IlcESD *event) calculates the probability
// of having dedx and the probability of having timbin at a given 
// momentum (mom) and particle type k (0 for e) and (2 for pi)
// from the precalculated timbin distributions. 
// Prashant Shukla <shukla@pi0.physi.uni-heidelberg.de>
//-----------------------------------------------------------------

#include "IlcDCHpidESD.h"
#include "IlcESD.h"
#include "IlcESDtrack.h"
#include "IlcDCHcalibDB.h"
#include "Cal/IlcDCHCalPIDLQ.h"

ClassImp(IlcDCHpidESD)

//_________________________________________________________________________
IlcDCHpidESD::IlcDCHpidESD(Double_t *param)
{
  //
  //  The main constructor
  //
  fMIP=param[0];   // MIP signal
  fRes=param[1];   // relative resolution
  fRange=param[2]; // PID "range" (in sigmas)
}

Double_t IlcDCHpidESD::Bethe(Double_t bg) 
{
  //
  // Parametrization of the Bethe-Bloch-curve
  // The parametrization is the same as for the TPC and is taken from Lehrhaus.
  //

  // This parameters have been adjusted to averaged values from GEANT
  const Double_t kP1 = 7.17960e-02;
  const Double_t kP2 = 8.54196;
  const Double_t kP3 = 1.38065e-06;
  const Double_t kP4 = 5.30972;
  const Double_t kP5 = 2.83798;

  // This parameters have been adjusted to Xe-data found in:
  // Allison & Cobb, Ann. Rev. Nucl. Sci. (1980), 30, 253
  //const Double_t kP1 = 0.76176E-1;
  //const Double_t kP2 = 10.632;
  //const Double_t kP3 = 3.17983E-6;
  //const Double_t kP4 = 1.8631;
  //const Double_t kP5 = 1.9479;

  // Lower cutoff of the Bethe-Bloch-curve to limit step sizes
  const Double_t kBgMin = 0.8;
  const Double_t kBBMax = 6.83298;
  //const Double_t kBgMin = 0.6;
  //const Double_t kBBMax = 17.2809;
  //const Double_t kBgMin = 0.4;
  //const Double_t kBBMax = 82.0;

  if (bg > kBgMin) {
    Double_t yy = bg / TMath::Sqrt(1. + bg*bg);
    Double_t aa = TMath::Power(yy,kP4);
    Double_t bb = TMath::Power((1./bg),kP5);
             bb = TMath::Log(kP3 + bb);
    return ((kP2 - aa - bb)*kP1 / aa);
  }
  else {
    return kBBMax;
  }

}

//_________________________________________________________________________
Int_t IlcDCHpidESD::MakePID(IlcESD *event)
{
  //
  //  This function calculates the "detector response" PID probabilities 
  //
  
  IlcDCHcalibDB* calibration = IlcDCHcalibDB::Instance();
  if (!calibration)
    return -1;
  
  // The class IlcDCHCalPIDLQ contains precalculated prob dis.
  const IlcDCHCalPIDLQ *pd = calibration->GetPIDLQObject();
  if (!pd) return -1;

  //  Example to get mean for particle 2 (pi) and momentum number 4 (2 GeV)
  //  printf("%.2f \n", pd->GetMean(2, 4));
  //  Example of use of Copy Constructor 
  //  IlcDCHCalPIDLQ *pd1 = new IlcDCHCalPIDLQ(*pd);

  Int_t ntrk=event->GetNumberOfTracks();
  for (Int_t i=0; i<ntrk; i++) {
    IlcESDtrack *t=event->GetTrack(i);
    if ((t->GetStatus()&IlcESDtrack::kMUDin)==0)
      if ((t->GetStatus()&IlcESDtrack::kMUDout)==0)
	if ((t->GetStatus()&IlcESDtrack::kMUDrefit)==0) continue;
    if(t->GetMUDsignal()==0) continue;
    //    Int_t ns=IlcESDtrack::kSPECIES;
    Int_t ns=IlcPID::kSPECIES;
    Double_t p[10];
    Double_t mom=t->GetP();
    Double_t probTotal=0.0;
    for (Int_t j=0; j<ns; j++) {
      p[j]=1.;
      for (Int_t ilayer=0; ilayer <6; ilayer++) {
        Double_t dedx=t->GetMUDsignals(ilayer,1);
        Int_t timbin=t->GetMUDTimBin(ilayer);
	p[j]*= pd->GetProbability(j,mom,dedx);
	p[j]*= pd->GetProbabilityT(j,mom,timbin);
	p[j]*= 100;
      } // loop over layers
      probTotal+=p[j];
    } //loop over particle species
    //  printf(" %f  %d  %f  %f  %f \n", mom, timbin, p[0], p[1], p[2]);
    for (Int_t j=0; j<ns; j++) {
      if(probTotal) p[j]/= probTotal;
      else p[j]=1.0;
      //      p[j]=1.;
    } //loop over particle species
    t->SetMUDpid(p);
  } //loop over tracks
  return 0;
}

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
// TARGET PID method # 1
//           Implementation of the TARGET PID class
// Very naive one... Should be made better by the detector experts...
//      Origin: Iouri Belikov, CERN, Jouri.Belikov@cern.ch
//-----------------------------------------------------------------
#include "IlcTARGETpidESD.h"
#include "IlcTARGETpidESD1.h"
#include "IlcESDEvent.h"
#include "IlcESDtrack.h"

ClassImp(IlcTARGETpidESD1)

IlcTARGETpidESD1::IlcTARGETpidESD1(): IlcTARGETpidESD() {
  //Default constructor
  fMIP=0;
  fRes=0;
  fRange=0;
}
//_________________________________________________________________________
IlcTARGETpidESD1::IlcTARGETpidESD1(Double_t *param): IlcTARGETpidESD()
{
  //
  //  The main constructor
  //
  fMIP=param[0];
  fRes=param[1];
  fRange=param[2];
}


//_________________________________________________________________________
Int_t IlcTARGETpidESD1::MakePID(IlcESDEvent *event)
{
  //
  //  This function calculates the "detector response" PID probabilities 
  //
  Int_t ntrk=event->GetNumberOfTracks();
  for (Int_t i=0; i<ntrk; i++) {
    IlcESDtrack *t=event->GetTrack(i);
    if ((t->GetStatus()&IlcESDtrack::kVXDin )==0)
      if ((t->GetStatus()&IlcESDtrack::kVXDout)==0) continue;
    Double_t mom=t->GetP();
    Double_t dedx=t->GetVXDsignal()/fMIP;
    Int_t ns=IlcPID::kSPECIES;
    Double_t p[10];
    for (Int_t j=0; j<ns; j++) {
      Double_t mass=IlcPID::ParticleMass(j);
      Double_t bethe=IlcTARGETpidESD::Bethe(mom/mass); 
      Double_t sigma=fRes*bethe;
      if (TMath::Abs(dedx-bethe) > fRange*sigma) {
	p[j]=TMath::Exp(-0.5*fRange*fRange)/sigma;
        continue;
      }
      p[j]=TMath::Exp(-0.5*(dedx-bethe)*(dedx-bethe)/(sigma*sigma))/sigma;
    }
    t->SetVXDpid(p);
  }
  return 0;
}

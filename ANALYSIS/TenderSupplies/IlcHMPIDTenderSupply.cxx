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


///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// HMPID tender: - recalculate pid bit using tighter cuts
//               - this is needed for all 2010 and 11a-c data
//             
//             
// Contacts: Giacomo.Volpe@ba.infn.it                                        //
//           Jens.Wiechula@cern.ch                                           //
///////////////////////////////////////////////////////////////////////////////
#include <TMath.h>
#include <TRandom.h>
#include <IlcLog.h>
#include <IlcESDEvent.h>
#include <IlcESDtrack.h>
#include <IlcESDInputHandler.h>
#include <IlcAnalysisManager.h>
#include <IlcESDpid.h>
#include <IlcTender.h>

#include "IlcHMPIDTenderSupply.h"

ClassImp(IlcHMPIDTenderSupply)

IlcHMPIDTenderSupply::IlcHMPIDTenderSupply() :
IlcTenderSupply()
{
  //
  // default ctor
  //
}

//_____________________________________________________
IlcHMPIDTenderSupply::IlcHMPIDTenderSupply(const char *name, const IlcTender *tender) :
IlcTenderSupply(name,tender)
{
  //
  // named ctor
  //
}

//_____________________________________________________
void IlcHMPIDTenderSupply::Init()
{
  // Initialise HMPID tender
  
}

//_____________________________________________________
void IlcHMPIDTenderSupply::ProcessEvent()
{
  //
  // recalculate HMPIDpid bit
  // 
  

  IlcESDEvent *event=fTender->GetEvent();
  if (!event) return;
  
  // re-evaluate the HMPIDpid bit for all tracks
  Int_t ntracks=event->GetNumberOfTracks();
  for(Int_t itrack = 0; itrack < ntracks; itrack++){
    IlcESDtrack *track=event->GetTrack(itrack);
    if (!itrack) continue;
    //reset pid bit first
    track->ResetStatus(IlcESDtrack::kHMPIDpid);

    Float_t xPc=0., yPc=0., xMip=0., yMip=0., thetaTrk=0., phiTrk=0.;
    Int_t nPhot=0, qMip=0;
 
    track->GetHMPIDtrk(xPc,yPc,thetaTrk,phiTrk);
    track->GetHMPIDmip(xMip,yMip,qMip,nPhot);
    //
    //make cuts, just an example, THIS NEEDS TO BE CHANGED
    //
    //if ((track->GetStatus()&IlcESDtrack::kHMPIDout)!=IlcESDtrack::kHMPIDout) continue;
   
    Float_t dist = TMath::Sqrt((xPc-xMip)*(xPc-xMip) + (yPc-yMip)*(yPc-yMip));    

    if(dist > 0.7 || nPhot> 30 || qMip < 100  ) continue;

    //set pid bit, track was accepted
    track->SetStatus(IlcESDtrack::kHMPIDpid);
    
  }
  
  
}

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
// PID tender: Do combined PID                                               //
//                                                                           //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////


#include <IlcESDpid.h>
#include <IlcESDEvent.h>
#include <IlcESDInputHandler.h>
#include "IlcTender.h"

#include "IlcPIDTenderSupply.h"

IlcPIDTenderSupply::IlcPIDTenderSupply() :
  IlcTenderSupply()
{
  //
  // default ctor
  //
}

//_____________________________________________________
IlcPIDTenderSupply::IlcPIDTenderSupply(const char *name, const IlcTender *tender) :
  IlcTenderSupply(name,tender)
{
  //
  // named ctor
  //
}

//_____________________________________________________
void IlcPIDTenderSupply::ProcessEvent()
{
  //
  // Combine PID information
  //

  IlcESDEvent *event=fTender->GetEvent();
  if (!event) return;

  IlcESDpid *pid=fTender->GetESDhandler()->GetESDpid();
  if (!pid) return;
  //
  // recalculate combined PID probabilities
  //
  Int_t ntracks=event->GetNumberOfTracks();
  for(Int_t itrack = 0; itrack < ntracks; itrack++)
    pid->CombinePID(event->GetTrack(itrack));
  
}

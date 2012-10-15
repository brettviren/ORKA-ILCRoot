// $Id: event_goto.C 55699 2012-04-11 10:01:56Z hristov $
// Main authors: Matevz Tadel & Alja Mrak-Tadel: 2006, 2007

/**************************************************************************
 * 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * Author: The International Lepton Collider Off-line Project.            *
 *                                                                        *
 * Part of the code has been developed by ILC Off-line Project.         *
 * Contributors are mentioned in the code where appropriate.              *
 * See http://ilcinfo.cern.ch/Offline/IlcRoot/License.html for          *
 * full copyright notice.                                                 *
 **************************************************************************/

#if !defined(__CINT__) || defined(__MAKECINT__)
#include <IlcEveEventManager.h>
#endif

/// \ingroup evemacros
/// \file event_goto.C

void event_goto(Int_t event=0)
{
  if (IlcEveEventManager::GetMaster() == 0) {
    printf("IlcEveEventManager is not initialized!\n");
    return;
  }
  IlcEveEventManager::GetMaster()->GotoEvent(event);
}

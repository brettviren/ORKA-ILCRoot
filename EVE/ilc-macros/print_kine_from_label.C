// $Id: print_kine_from_label.C 55699 2012-04-11 10:01:56Z hristov $
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
#include <TParticle.h>

#include <IlcRunLoader.h>
#include <IlcStack.h>
#include <IlcEveEventManager.h>
#endif

void print_kine_from_label(Int_t label)
{
  IlcRunLoader* rl = IlcEveEventManager::AssertRunLoader();
  rl->LoadKinematics();
  IlcStack* stack = rl->Stack();

  printf("Number primaries %d, all particles %d, label %d\n",
	 stack->GetNprimary(), stack->GetNtrack(), label);
  if (label < 0 || label >= stack->GetNtrack()) {
    printf("  Label exceeds available range.\n");
    return;
  }

  TParticle* part = stack->Particle(label);
  if(part != 0) {
    part->Print();
    while(part->GetMother(0) >= 0) {
      part = stack->Particle(part->GetMother(0));
      part->Print();
    }
  }
}

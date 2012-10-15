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

/* $Id: IlcHit.cxx 50615 2011-07-16 23:19:19Z hristov $ */

//-----------------------------------------------------------------------
//    Base Hit class for all detectors
//    Contains the coordinates of the hit (single energy deposition)
//    and the number of correspondent track
//    Author:
//-----------------------------------------------------------------------

#include "TParticle.h"

#include "IlcHit.h"
#include "IlcRun.h"
#include "IlcMC.h"
#include "IlcStack.h"

ClassImp(IlcHit)

//_______________________________________________________________________
IlcHit::IlcHit():
  fTrack(0),
  fX(0),
  fY(0),
  fZ(0)
{
  //
  // Default constructor
  //
}

//_______________________________________________________________________
IlcHit::IlcHit(Int_t shunt, Int_t track):
  fTrack(0),
  fX(0),
  fY(0),
  fZ(0)
{
  //
  // Standard constructor
  //
  if(shunt == 1) {
    int primary = gIlc->GetMCApp()->GetPrimary(track);
    gIlc->GetMCApp()->Particle(primary)->SetBit(kKeepBit);
    fTrack=primary;
  } 

  else if (shunt == 2) {
    // the "primary" particle associated to the hit is
    // the last track that has been flagged in the StepManager
    // used by PHOS to associate the hit with the decay gamma
    // rather than with the original pi0 
    TParticle *part;
    Int_t current;
    Int_t parent=track;
    while (1) {
      current=parent;
      part = gIlc->GetMCApp()->Particle(current);
      parent=part->GetFirstMother();    
      if(parent<0 || part->TestBit(kKeepBit))
	break;
    }
    fTrack=current;   
  } else {
    fTrack=track;
    gIlc->GetMCApp()->FlagTrack(fTrack);
  }
}

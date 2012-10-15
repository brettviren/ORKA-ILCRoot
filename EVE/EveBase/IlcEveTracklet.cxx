// $Id: IlcEveTracklet.cxx 36365 2009-11-04 11:11:01Z mtadel $
// Author: Matevz Tadel 2009

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

#include "IlcEveTracklet.h"

#include <IlcVVertex.h>
#include <TEveTrackPropagator.h>

//______________________________________________________________________________
// IlcEveTracklet is a representation of SPD tracklet.
// It inherits from IlcEveTrack to allow for common functionality
// regarding track counting.
//
// TEveTrack::fV - stores primary vertex.
// TEveTrack::fP - stores vector in direction of the tracklet with
//                 transverse component equal to 1.

ClassImp(IlcEveTracklet)

Float_t IlcEveTracklet::fgDefaultRadius = 10;

//______________________________________________________________________________
Float_t IlcEveTracklet::GetDefaultRadius()
{
  // Static - return defualt extrapolation radius.

  return fgDefaultRadius;
}

//______________________________________________________________________________
void IlcEveTracklet::SetDefaultRadius(Float_t r)
{
  // Static - set defualt extrapolation radius.

  fgDefaultRadius = r;
}

//==============================================================================

//______________________________________________________________________________
IlcEveTracklet::IlcEveTracklet(Int_t index, const IlcVVertex* pv, Float_t theta, Float_t phi,
                               TEveTrackPropagator* prop) :
  IlcEveTrack()
{
  // Constructor.

  fIndex = index;
  fV.Set(pv->GetX(), pv->GetY(), pv->GetZ());
  fP.Set(TMath::Cos(phi), TMath::Sin(phi), 1.0/TMath::Tan(theta));

  if (prop) SetPropagator(prop);
}

//==============================================================================

//______________________________________________________________________________
void IlcEveTracklet::MakeTrack(Bool_t recurse)
{
  // Make track -- just make a line to radius specified in propagator
  // or use the default if it is not set.

  Float_t r = fPropagator ? fPropagator->GetMaxR() : fgDefaultRadius;
  Reset(2);
  SetPoint(0, fV.fX, fV.fY, fV.fZ);
  SetPoint(1, fV.fX + r*fP.fX, fV.fY + r*fP.fY, fV.fZ + r*fP.fZ);

  if (recurse)
  {
    for (List_i i=fChildren.begin(); i!=fChildren.end(); ++i)
    {
      TEveTrack* t = dynamic_cast<TEveTrack*>(*i);
      if (t) t->MakeTrack(recurse);
    }
  }
}

//______________________________________________________________________________
void IlcEveTracklet::SecSelected(TEveTrack* track)
{
  // Emits "SecSelected(TEveTrack*)" signal.
  // Called from TEveTrackGL on secondary-selection.

  Emit("SecSelected(TEveTrack*)", (Long_t)track);
  SecSelectedTracklet((IlcEveTracklet*) track);
}

//______________________________________________________________________________
void IlcEveTracklet::SecSelectedTracklet(IlcEveTracklet* track)
{
  // Emits "SecSelectedTracklet(IlcEveTracklet*)" signal.

  Emit("SecSelectedTracklet(IlcEveTracklet*)", (Long_t)track);
}

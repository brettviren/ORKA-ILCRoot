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

/* $Id: IlcPoints.cxx,v 1.2 2009/11/30 18:26:34 vitomeg Exp $ */

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//  This class contains the points for the ILC event display               //
//                                                                           //
//Begin_Html
/*
<img src="picts/IlcPointsClass.gif">
*/
//End_Html
//                                                                           //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#include "TPad.h"
#include "TParticle.h"
#include "TView.h"

#include "IlcDetector.h"
#include "IlcMC.h"
#include "IlcPoints.h"
#include "IlcRun.h"
 
ClassImp(IlcPoints)

//_______________________________________________________________________
IlcPoints::IlcPoints():
  fDetector(0),
  fIndex(0)
{
  //
  // Default constructor
  //
}

//_______________________________________________________________________
IlcPoints::IlcPoints(const IlcPoints &pts):
  TPolyMarker3D(pts),
  fDetector(0),
  fIndex(0)
{
  //
  // Copy constructor
  //
  pts.Copy(*this);
}

//_______________________________________________________________________
IlcPoints::IlcPoints(Int_t nhits):
  TPolyMarker3D(nhits),
  fDetector(0),
  fIndex(0)
{
  //
  // Standard constructor
  //
  ResetBit(kCanDelete);
}
	 
//_______________________________________________________________________
IlcPoints::~IlcPoints()
{
  //
  // Default destructor
  //
}

//_______________________________________________________________________
void IlcPoints::Copy(TObject &pts) const
{
  //
  // Copy *this onto pts
  //
  if((TObject*)this != &pts) {
    ((TPolyMarker3D*)this)->Copy(dynamic_cast<TPolyMarker3D&>(pts));
    /*(dynamic_cast<IlcPoints&>(pts)).fGLList = fGLList;*/
    (dynamic_cast<IlcPoints&>(pts)).fLastPoint = fLastPoint;
    (dynamic_cast<IlcPoints&>(pts)).fDetector = fDetector;
    (dynamic_cast<IlcPoints&>(pts)).fIndex = fIndex;
  }
}

//_______________________________________________________________________
Int_t IlcPoints::DistancetoPrimitive(Int_t px, Int_t py)
{
  //
  //*-*-*-*-*-*-*Compute distance from point px,py to a 3-D polymarker*-*-*-*-*
  //*-*          =====================================================
  //*-*
  //*-*  Compute the closest distance of approach from point
  //*-*  px,py to each segment
  //*-*  of the polyline.
  //*-*  Returns when the distance found is below DistanceMaximum.
  //*-*  The distance is computed in pixels units.
  //*-*
  //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*

  //const Int_t inaxis = 7;
  //Int_t dist = 9999;
  return TPolyMarker3D::DistancetoPrimitive(px,py);
}

//_______________________________________________________________________
void IlcPoints::DumpParticle() const
{
  //
  //   Dump particle corresponding to this point
  //
  TParticle *particle = GetParticle();
  if (particle) particle->Dump();
}

//_______________________________________________________________________
void IlcPoints::ExecuteEvent(Int_t event, Int_t px, Int_t py)
{
  //
  //*-*-*-*-*-*-*-*-*-*Execute action corresponding to one event*-*-*-*-*-*-*-*
  //*-*                =========================================
  //*-*
  //*-*  This member function must be implemented to reilcze the action
  //*-*  corresponding to the mouse click on the object in the window
  //*-*
  //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*

  gPad->SetCursor(kCross);
  
  if (gPad->GetView())
    gPad->GetView()->ExecuteRotateView(event, px, py);

}

//_______________________________________________________________________
const Text_t *IlcPoints::GetName() const
{
  //
  // Return name of the Geant3 particle corresponding to this point
  //
  TParticle *particle = GetParticle();
  if (!particle) return "Particle";
  return particle->GetName();
}

//_______________________________________________________________________
Text_t *IlcPoints::GetObjectInfo(Int_t, Int_t) const
{
  //
  //   Redefines TObject::GetObjectInfo.
  //   Displays the info (particle,etc
  //   corresponding to cursor position px,py
  //
  static char info[64];
  sprintf(info,"%s %d",GetName(),fIndex);
  return info;
}

//_______________________________________________________________________
TParticle *IlcPoints::GetParticle() const
{
  //
  //   Returns pointer to particle index in IlcRun::fParticles
  //
  if (fIndex < 0 || fIndex >= gIlc->GetMCApp()->GetNtrack()) return 0;
  else return gIlc->GetMCApp()->Particle(fIndex);
}

//_______________________________________________________________________
void IlcPoints::InspectParticle() const
{
  //
  //   Inspect particle corresponding to this point
  //
  TParticle *particle = GetParticle();
  if (particle) particle->Inspect();
}

//_______________________________________________________________________
void IlcPoints::Propagate()
{
  //
  //   Set attributes of all detectors to be the attributes of this point
  //
  Int_t ntracks,track;
  TObjArray *points;
  IlcPoints *pm;
  //  
  TIter next(gIlc->Detectors());
  IlcDetector *detector;
  while((detector = (IlcDetector*)(next()))) {
    if (!detector->IsActive()) continue;
    points = detector->Points();
    if (!points) continue;
    ntracks = points->GetEntriesFast();
    for (track=0;track<ntracks;track++) {
      pm = dynamic_cast<IlcPoints*>(points->UncheckedAt(track));
      if (!pm) continue;
      if (fIndex == pm->GetIndex()) {
	pm->SetMarkerColor(GetMarkerColor());
	pm->SetMarkerSize(GetMarkerSize());
	pm->SetMarkerStyle(GetMarkerStyle());
      }
    }
  }
}

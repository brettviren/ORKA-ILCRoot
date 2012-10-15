// $Id: primary_vertex_tracks.C 55699 2012-04-11 10:01:56Z hristov $
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
#include <TMarker.h>
#include <TEveManager.h>
#include <TEveCompound.h>
#include <TEveTrack.h>
#include <TEveTrackPropagator.h>
#include <TEveUtil.h>

#include <IlcESDEvent.h>
#include <IlcESDVertex.h>
#include <IlcESDtrack.h>
#include <IlcEveEventManager.h>
#include <IlcEveTrack.h>

#include "esd_tracks.C"
#endif

TEveTrackList* primary_vertex_tracks()
{
  TEveUtil::LoadMacro("esd_tracks.C");
  IlcESDEvent   *esd = IlcEveEventManager::AssertESD();
  const IlcESDVertex *pv  = esd->GetPrimaryVertex();

  TEveTrackList* cont = new TEveTrackList("Tracks for Primary Vertex");
  cont->SetMainColor(7);
  TEveTrackPropagator* rnrStyle = cont->GetPropagator();
  rnrStyle->SetMagField( 0.1*esd->GetMagneticField() );
  rnrStyle->fRnrFV = kTRUE;
  rnrStyle->fFVAtt.SetMarkerColor(2);
  gEve->AddElement(cont);

  for (Int_t n=0; n<pv->GetNIndices(); n++)
  {
    IlcESDtrack* at = esd->GetTrack(pv->GetIndices()[n]);
    IlcEveTrack* track = esd_make_track(at, cont);
    track->SetLineWidth(4);
    track->SetLineColor(cont->GetMainColor());
    track->SetLineStyle(7);
    gEve->AddElement(track, cont);
  }

  cont->MakeTracks();
  gEve->Redraw3D();

  return cont;
}

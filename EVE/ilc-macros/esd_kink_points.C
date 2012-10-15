// $Id: esd_kink_points.C 55699 2012-04-11 10:01:56Z hristov $
// Main authors: Paraskevi Ganoti: 2009

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
#include <TMath.h>
#include <TVector3.h>
#include <TEvePointSet.h>
#include <TEveManager.h>

#include <IlcESDEvent.h>
#include <IlcESDtrack.h>
#include <IlcESDkink.h>
#include <IlcEveEventManager.h>
#endif

void esd_kink_fill_pointset(TEvePointSet* ps)
{
  IlcESDEvent* esd = IlcEveEventManager::AssertESD();

  for (Int_t n=0; n<esd->GetNumberOfTracks(); ++n)
  { 
      IlcESDtrack* track = esd->GetTrack(n);
      if(track->GetKinkIndex(0)<0){
    
          IlcESDkink *kink = esd->GetKink(TMath::Abs(track->GetKinkIndex(0))-1);
	  const TVector3 Position(kink->GetPosition());
	  ps->SetNextPoint(Position.X(), Position.Y(), Position.Z());
          ps->SetPointId(kink);
      }
  }

}

TEvePointSet* esd_kink_points()
{
  TEvePointSet* points = new TEvePointSet("Kink vertex locations");

  esd_kink_fill_pointset(points);

  points->SetTitle(Form("N=%d", points->Size()));
  points->SetMarkerStyle(4);
  points->SetMarkerSize(1.5);
  points->SetMarkerColor(kOrange+8);

  gEve->AddElement(points);
  gEve->Redraw3D();

  return points;
}

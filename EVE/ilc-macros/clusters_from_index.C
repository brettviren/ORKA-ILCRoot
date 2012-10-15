// $Id: clusters_from_index.C 55699 2012-04-11 10:01:56Z hristov $
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
#include <TEveManager.h>
#include <TEveElement.h>
#include <TEvePointSet.h>

#include <IlcESDEvent.h>
#include <IlcTrackPointArray.h>
#include <IlcEveEventManager.h>
#include <IlcEveMultiView.h>
#endif

TEvePointSet* clusters_from_index(Int_t index=0, TEveElement* cont=0)
{
  IlcESDEvent* esd = IlcEveEventManager::AssertESD();

  if (index < 0) {
    Warning("clusters_from_index", "index not set.");
    return 0;
  }

  if (index >= esd->GetNumberOfTracks()) {
    Warning("clusters_from_index", "index out of range");
    return 0;
  }

  TEvePointSet* clusters = new TEvePointSet(64);
  clusters->SetOwnIds(kTRUE);

  IlcESDtrack* at = esd->GetTrack(index);
  const IlcTrackPointArray* pArr = at->GetTrackPointArray();
  if (pArr == 0) {
    Warning("clusters_from_index", "TrackPointArray not stored with ESD track.");
  }
  
  Int_t np =  pArr->GetNPoints();
  const Float_t* x = pArr->GetX();
  const Float_t* y = pArr->GetY();
  const Float_t* z = pArr->GetZ();
  for (Int_t i=0; i<np; ++i) {
    clusters->SetNextPoint(x[i], y[i], z[i]);
    IlcTrackPoint *atp = new IlcTrackPoint;
    pArr->GetPoint(*atp, i);
    clusters->SetPointId(atp);    }


  if(clusters->Size() == 0 && gEve->GetKeepEmptyCont() == kFALSE) {
    Warning("clusters_from_index", "No clusters for index '%d'", index);
    delete clusters;
    return 0;
  }

  clusters->SetMarkerStyle(2);
  clusters->SetMarkerSize(2);
  clusters->SetMarkerColor(4);

  clusters->SetName(Form("Clusters idx=%d", index));
  clusters->SetTitle(Form("N=%d", clusters->Size()));

  gEve->AddElement(clusters);

  if (IlcEveMultiView::Instance())
  {
    IlcEveMultiView::Instance()->ImportEventRPhi(clusters);
    IlcEveMultiView::Instance()->ImportEventRhoZ(clusters);
  }

  gEve->Redraw3D();

  return clusters;
}

// $Id: PVEC_hits.C 55699 2012-04-11 10:01:56Z hristov $
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
#include <TTree.h>
#include <TString.h>
#include <TEveManager.h>
#include <TEveElement.h>
#include <TEvePointSet.h>

#include <IlcEveEventManager.h>
#include <IlcRunLoader.h>
#endif

TEvePointSet*
PVEC_hits(const char  *varexp    = "PVEC.fX:PVEC.fY:PVEC.fZ",
	    const char  *selection = "",
	    TEveElement *cont      = 0)
{
  IlcRunLoader* rl =  IlcEveEventManager::AssertRunLoader();
  rl->LoadHits("PVEC");

  TTree* ht = rl->GetTreeH("PVEC", false);

  TEvePointSet* points = new TEvePointSet(Form("PVEC Hits '%s'", selection));

  TEvePointSelector ps(ht, points, varexp, selection);
  ps.Select();

  if(points->Size() == 0 && gEve->GetKeepEmptyCont() == kFALSE)
  {
    Warning("PVEC_hits", "No hits match '%s'", selection);
    delete points;
    return 0;
  }

  points->SetName(Form("PVEC Hits"));
  const TString viz_tag("SIM Hits PVEC");
  points->ApplyVizTag(viz_tag, "Hits");

  points->SetTitle(Form("N=%d", points->Size()));
  points->SetMarkerSize(.5);
  points->SetMarkerColor(2);

  gEve->AddElement(points, cont);
  gEve->Redraw3D();

  return points;
}

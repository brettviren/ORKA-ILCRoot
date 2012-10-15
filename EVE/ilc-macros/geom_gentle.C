// $Id: geom_gentle.C 54257 2012-01-30 20:52:05Z quark $
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
#include <TFile.h>
#include <TEveManager.h>
#include <TEveGeoNode.h>
#include <TEveElement.h>
#include <TEveGeoShape.h>
#include <TEveGeoShapeExtract.h>
#endif

TEveGeoShape* geom_gentle(Bool_t register_as_global=kTRUE)
{
  TFile f("$ILC_ROOT/EVE/ilc-data/gentle_geo.root");
  TEveGeoShapeExtract* gse = (TEveGeoShapeExtract*) f.Get("Gentle");
  TEveGeoShape* gsre = TEveGeoShape::ImportShapeExtract(gse);
  f.Close();

  TEveElement* elPHOS = gsre->FindChild("PHOS");
  elPHOS->SetRnrState(kTRUE);
  elPHOS->FindChild("PHOS_4")->SetRnrState(kFALSE);
  elPHOS->FindChild("PHOS_5")->SetRnrState(kFALSE);

  if (register_as_global)
  {
    gEve->AddGlobalElement(gsre);
  }

  return gsre;
}

TEveGeoShape* geom_gentle_rphi()
{
  // The resulting geometry is NOT added into the global scene!

  TFile f("$ILC_ROOT/EVE/ilc-data/gentle_rphi_geo.root");
  TEveGeoShapeExtract* gse = (TEveGeoShapeExtract*) f.Get("Gentle");
  TEveGeoShape* gsre = TEveGeoShape::ImportShapeExtract(gse);
  f.Close();

  TEveElement* elPHOS = gsre->FindChild("PHOS");
  elPHOS->SetRnrState(kTRUE);
  elPHOS->FindChild("PHOS_4")->SetRnrState(kFALSE);
  elPHOS->FindChild("PHOS_5")->SetRnrState(kFALSE);

  return gsre;
}

TEveGeoShape* geom_gentle_rhoz()
{
  // The resulting geometry is NOT added into the global scene!

  TFile f("$ILC_ROOT/EVE/ilc-data/gentle_rhoz_geo.root");
  TEveGeoShapeExtract* gse = (TEveGeoShapeExtract*) f.Get("Gentle");
  TEveGeoShape* gsre = TEveGeoShape::ImportShapeExtract(gse);
  f.Close();

  return gsre;
}

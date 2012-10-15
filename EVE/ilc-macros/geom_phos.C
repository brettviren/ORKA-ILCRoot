// $Id: geom_phos.C 58244 2012-08-21 12:57:19Z hristov $
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
#include <TGeoManager.h>
#include <TEveManager.h>
#include <TEveGeoNode.h>

#include <IlcEveEventManager.h>
#endif

void geom_phos()
{
  IlcEveEventManager::AssertGeometry();

  TEveElementList* list = new TEveElementList("PHOS");
  gEve->AddGlobalElement(list);

  for(Int_t i=1; i<=5; ++i) {
    //PH The line below is replaced waiting for a fix in Root
    //PH which permits to use variable siza arguments in CINT
    //PH on some platforms (alphalinuxgcc, solariscc5, etc.)
    //PH TGeoNode* node = gGeoManager->GetTopVolume()->FindNode(Form("PHOS_%d", i));
    char form[1000];
    sprintf(form,"PHOS_%d", i);
    TGeoNode* node = gGeoManager->GetTopVolume()->FindNode(form);

    TEveGeoTopNode* re = new TEveGeoTopNode(gGeoManager, node);
    re->UseNodeTrans();
    gEve->AddGlobalElement(re, list);
  }

  gEve->Redraw3D();
}

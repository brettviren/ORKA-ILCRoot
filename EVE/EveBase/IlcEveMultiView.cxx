// $Id: IlcEveMultiView.cxx 58244 2012-08-21 12:57:19Z hristov $
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

#include "IlcEveMultiView.h"


//______________________________________________________________________________
// Full description of IlcEveMultiView
//

ClassImp(IlcEveMultiView)

IlcEveMultiView* IlcEveMultiView::fgInstance = 0;

IlcEveMultiView* IlcEveMultiView::Instance()
{
  // Return static instance.

  return fgInstance;
}

IlcEveMultiView::IlcEveMultiView(Bool_t setMuonView) :
  fRPhiMgr(0), fRhoZMgr(0), fMuonMgr(0),
  f3DView(0), fRPhiView(0), fRhoZView(0), fMuonView(0),
  fRPhiGeomScene(0), fRhoZGeomScene(0), fMuonGeomScene(0),
  fRPhiEventScene(0), fRhoZEventScene(0), fMuonEventScene(0),
  fGeomGentle(0), fGeomGentleRPhi(0), fGeomGentleRhoZ(0),
  fGeomGentleTrd(0), fGeomGentleMuon(0), fIsMuonView(kFALSE)
{
  // Constructor --- creates required scenes, projection managers
  // and GL viewers.

  if (fgInstance)
    throw TEveException("IlcEveMultiView::IlcEveMultiView already instantiated.");
  fgInstance = this;

  // Scenes
  //========

  fRPhiGeomScene  = gEve->SpawnNewScene("RPhi Geometry",
                                        "Scene holding projected geometry for the RPhi view.");
  fRhoZGeomScene  = gEve->SpawnNewScene("RhoZ Geometry",
                                        "Scene holding projected geometry for the RhoZ view.");
  fMuonGeomScene  = gEve->SpawnNewScene("Muon Geometry",
                                        "Scene holding projected geometry for the Muon view.");
  fRPhiEventScene = gEve->SpawnNewScene("RPhi Event Data",
                                        "Scene holding projected event-data for the RPhi view.");
  fRhoZEventScene = gEve->SpawnNewScene("RhoZ Event Data",
                                        "Scene holding projected event-data for the RhoZ view.");
  fMuonEventScene = gEve->SpawnNewScene("Muon Event Data",
                                        "Scene holding projected event-data for the Muon view.");

  fIsMuonView = setMuonView;

  // Projection managers
  //=====================

  fRPhiMgr = new TEveProjectionManager();
  fRPhiMgr->SetProjection(TEveProjection::kPT_RPhi);
  gEve->AddToListTree(fRPhiMgr, kFALSE);
  {
    TEveProjectionAxes* a = new TEveProjectionAxes(fRPhiMgr);
    a->SetMainColor(kWhite);
    a->SetTitle("R-Phi");
    a->SetTitleSize(0.05);
    a->SetTitleFont(102);
    a->SetLabelSize(0.025);
    a->SetLabelFont(102);
    fRPhiGeomScene->AddElement(a);
  }

  fRhoZMgr = new TEveProjectionManager();
  fRhoZMgr->SetProjection(TEveProjection::kPT_RhoZ);
  gEve->AddToListTree(fRhoZMgr, kFALSE);
  {
    TEveProjectionAxes* a = new TEveProjectionAxes(fRhoZMgr);
    a->SetMainColor(kWhite);
    a->SetTitle("Rho-Z");
    a->SetTitleSize(0.05);
    a->SetTitleFont(102);
    a->SetLabelSize(0.025);
    a->SetLabelFont(102);
    fRhoZGeomScene->AddElement(a);
  }

if(fIsMuonView)
{
  fMuonMgr = new TEveProjectionManager();
  fMuonMgr->SetProjection(TEveProjection::kPT_RhoZ);
  gEve->AddToListTree(fMuonMgr, kFALSE);
  {
    TEveProjectionAxes* a = new TEveProjectionAxes(fMuonMgr);
    a->SetMainColor(kWhite);
    a->SetTitle("Rho-Z Muon");
    a->SetTitleSize(0.05);
    a->SetTitleFont(102);
    a->SetLabelSize(0.025);
    a->SetLabelFont(102);
    fMuonGeomScene->AddElement(a);
  }
}

  // Viewers
  //=========

  TEveWindowSlot *slot = 0;
  TEveWindowPack *pack = 0;

  slot = TEveWindow::CreateWindowInTab(gEve->GetBrowser()->GetTabRight());
  pack = slot->MakePack();
  pack->SetElementName("Multi View");
  pack->SetHorizontal();
  pack->SetShowTitleBar(kFALSE);
  pack->NewSlot()->MakeCurrent();
  f3DView = gEve->SpawnNewViewer("3D View", "");
  f3DView->AddScene(gEve->GetGlobalScene());
  f3DView->AddScene(gEve->GetEventScene());

  pack = pack->NewSlot()->MakePack();
  pack->SetShowTitleBar(kFALSE);
  pack->NewSlot()->MakeCurrent();
  fRPhiView = gEve->SpawnNewViewer("RPhi View", "");
  fRPhiView->GetGLViewer()->SetCurrentCamera(TGLViewer::kCameraOrthoXOY);
  fRPhiView->AddScene(fRPhiGeomScene);
  fRPhiView->AddScene(fRPhiEventScene);

  pack->NewSlot()->MakeCurrent();
  fRhoZView = gEve->SpawnNewViewer("RhoZ View", "");
  fRhoZView->GetGLViewer()->SetCurrentCamera(TGLViewer::kCameraOrthoXOY);
  fRhoZView->AddScene(fRhoZGeomScene);
  fRhoZView->AddScene(fRhoZEventScene);

if(fIsMuonView)
{
  pack->NewSlot()->MakeCurrent();
  fMuonView = gEve->SpawnNewViewer("RhoZ View Muon", "");
  fMuonView->GetGLViewer()->SetCurrentCamera(TGLViewer::kCameraOrthoXOY);
  fMuonView->AddScene(fMuonGeomScene);
  fMuonView->AddScene(fMuonEventScene);
}

}

IlcEveMultiView::~IlcEveMultiView()
{
		DestroyAllGeometries();
		
		delete fGeomGentle;
		delete fGeomGentleRPhi;
		delete fGeomGentleRhoZ;
		
		delete	fRPhiMgr;
		delete fRhoZMgr;
		delete fMuonMgr;
		
}

//-------------------------------------------------------------------------

void IlcEveMultiView::InitGeomGentle(TEveGeoShape* g3d, TEveGeoShape* grphi, TEveGeoShape* grhoz, TEveGeoShape* gmuon)
{
  // Initialize gentle geometry.

  fGeomGentle     = g3d;
  fGeomGentleRPhi = grphi; fGeomGentleRPhi->IncDenyDestroy();
  fGeomGentleRhoZ = grhoz; fGeomGentleRhoZ->IncDenyDestroy();
  if(fIsMuonView) { fGeomGentleMuon = gmuon; fGeomGentleMuon->IncDenyDestroy(); }

  ImportGeomRPhi(fGeomGentleRPhi);
  ImportGeomRhoZ(fGeomGentleRhoZ);
  if(fIsMuonView) ImportGeomMuon(fGeomGentleMuon);
}

void IlcEveMultiView::InitGeomGentleTrd(TEveGeoShape* gtrd)
{
  // Initialize gentle geometry TRD.

  fGeomGentleTrd = gtrd;
  ImportGeomRPhi(fGeomGentleTrd);
  ImportGeomRhoZ(fGeomGentleTrd);
  if(fIsMuonView) ImportGeomMuon(fGeomGentleTrd);
}

void IlcEveMultiView::InitGeomGentleMuon(TEveGeoShape* gmuon, Bool_t showRPhi, Bool_t showRhoZ, Bool_t showMuon)
{
  // Initialize gentle geometry for MUON.

  fGeomGentleMuon = gmuon;
  if (showRPhi) ImportGeomRPhi(fGeomGentleMuon);
  if (showRhoZ) ImportGeomRhoZ(fGeomGentleMuon);
  if (showMuon && fIsMuonView) ImportGeomMuon(fGeomGentleMuon);

}

//-------------------------------------------------------------------------

void IlcEveMultiView::SetDepth(Float_t d)
{
  // Set current depth on all projection managers.

  fRPhiMgr->SetCurrentDepth(d);
  fRhoZMgr->SetCurrentDepth(d);
  if(fIsMuonView) fMuonMgr->SetCurrentDepth(d);

}

//-------------------------------------------------------------------------

void IlcEveMultiView::ImportGeomRPhi(TEveElement* el)
{ 
  // Import el into r-phi geometry scene.

  fRPhiMgr->ImportElements(el, fRPhiGeomScene);
}

void IlcEveMultiView::ImportGeomRhoZ(TEveElement* el)
{ 
  // Import el into rho-z geometry scene.

  fRhoZMgr->ImportElements(el, fRhoZGeomScene);
}

void IlcEveMultiView::ImportGeomMuon(TEveElement* el)
{ 
  // Import el into muon geometry scene.

    if(fIsMuonView) fMuonMgr->ImportElements(el, fMuonGeomScene);
}

void IlcEveMultiView::ImportEventRPhi(TEveElement* el)
{ 
  // Import el into r-phi event scene.

  fRPhiMgr->ImportElements(el, fRPhiEventScene);
}

void IlcEveMultiView::ImportEventRhoZ(TEveElement* el)
{ 
  // Import el into rho-z event scene.

  fRhoZMgr->ImportElements(el, fRhoZEventScene);
}

void IlcEveMultiView::ImportEventMuon(TEveElement* el)
{ 
  // Import el into muon event scene.

    if(fIsMuonView) fMuonMgr->ImportElements(el, fMuonEventScene);
}

void IlcEveMultiView::DestroyEventRPhi()
{
  // Destroy all elements in r-phi event scene.

  fRPhiEventScene->DestroyElements();
}

void IlcEveMultiView::DestroyEventRhoZ()
{
  // Destroy all elements in rho-z event scene.

  fRhoZEventScene->DestroyElements();
}

void IlcEveMultiView::DestroyEventMuon()
{
  // Destroy all elements in rho-z event scene.

    if(fIsMuonView) fMuonEventScene->DestroyElements();
}


//-------------------------------------------------------------------------

void IlcEveMultiView::SetCenterRPhi(Double_t x, Double_t y, Double_t z)
{
  // Set center of r-phi manager.

  fRPhiMgr->SetCenter(x, y, z);
}

void IlcEveMultiView::SetCenterRhoZ(Double_t x, Double_t y, Double_t z)
{
  // Set center of rho-z manager.

  fRhoZMgr->SetCenter(x, y, z);
}

void IlcEveMultiView::SetCenterMuon(Double_t x, Double_t y, Double_t z)
{
  // Set center of rho-z manager.

    if(fIsMuonView) fMuonMgr->SetCenter(x, y, z);
}

void IlcEveMultiView::DestroyAllGeometries()
{
  // Destroy 3d, r-phi and rho-z geometries.

  fGeomGentle->DestroyElements();
  fGeomGentleRPhi->DestroyElements();
  fGeomGentleRhoZ->DestroyElements();
  if(fIsMuonView) fGeomGentleMuon->DestroyElements();

}


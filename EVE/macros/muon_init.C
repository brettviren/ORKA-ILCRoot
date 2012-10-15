// $Id: muon_init.C 53875 2012-01-11 15:36:52Z vulpescu $

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

/// \ingroup evemacros
/// \file muon_init.C
///
/// \author P. Pillot, L. Aphecetche; Subatech

class IlcEveMacroExecutor;
class TEveProjectionManager;
class TEveGeoShape;
class TEveUtil;
class TSystem;
class TInterpreter;

Bool_t gShowMuonRPhi = kFALSE;
Bool_t gShowMuonRhoZ = kTRUE;
Bool_t gShowMuon = kTRUE;

Bool_t gCenterProjectionsAtPrimaryVertex = kFALSE;


void muon_init(const TString& cdburi = "",
               const TString& path   = ".",
	       Bool_t showBarrel = kTRUE)
{
  if (gSystem->Getenv("ILC_ROOT") != 0)
  {
    gInterpreter->AddIncludePath(Form("%s/MUON", gSystem->Getenv("ILC_ROOT")));
    gInterpreter->AddIncludePath(Form("%s/MUON/mapping", gSystem->Getenv("ILC_ROOT")));
  }
  
  if (cdburi.IsNull() && ! IlcCDBManager::Instance()->IsDefaultStorageSet())
  {
    gEnv->SetValue("Root.Stacktrace", "no");
    Fatal("muon_init.C", "OCDB path MUST be specified as the first argument.");
  }
  
  TEveUtil::LoadMacro("ilceve_init.C");
  path.Remove(TString::kTrailing, '/');
  if (path.BeginsWith("alien:")) IlcEveEventManager::SearchRawForCentralReconstruction();
  ilceve_init(cdburi, path, -1);
  
  TEveUtil::AssertMacro("VizDB_scan.C");
  
  IlcEveMacroExecutor *exec    = IlcEveEventManager::GetMaster()->GetExecutor();
  TEveBrowser         *browser = gEve->GetBrowser();
  browser->ShowCloseTab(kFALSE);
  
  
  //==============================================================================
  // Geometry, scenes, projections and viewers
  //==============================================================================
  
  IlcEveMultiView *mv = new IlcEveMultiView;
  
  mv->SetDepth(-10);
  
  TEveUtil::LoadMacro("geom_gentle.C");
  TEveUtil::LoadMacro("geom_gentle_muon.C");

  mv->InitGeomGentle(geom_gentle(), geom_gentle_rphi(), geom_gentle_rhoz(), geom_gentle_muon(kFALSE));
  
  mv->InitGeomGentleMuon(geom_gentle_muon(kFALSE), gShowMuonRPhi, gShowMuonRhoZ, gShowMuon);
  
  mv->SetDepth(0);
  
  //==============================================================================
  // Registration of per-event macros
  //==============================================================================
  
  exec->AddMacro(new IlcEveMacro(IlcEveMacro::kRunLoader, "SIM Track","kine_tracks.C+",   "kine_tracks",  "", kFALSE));

  exec->AddMacro(new IlcEveMacro(IlcEveMacro::kRunLoader, "SIM TrackRef","muon_trackRefs.C+","muon_trackRefs","kTRUE", kFALSE));
  
  exec->AddMacro(new IlcEveMacro(IlcEveMacro::kRawReader, "RAW MUON", "muon_raw.C+",     "muon_raw",     "", kTRUE));

  exec->AddMacro(new IlcEveMacro(IlcEveMacro::kRunLoader, "DIG MUON", "muon_digits.C+",  "muon_digits",  "", kFALSE));
  
  exec->AddMacro(new IlcEveMacro(IlcEveMacro::kRunLoader, "CLU MUON", "muon_clusters.C+","muon_clusters","", kTRUE));

  exec->AddMacro(new IlcEveMacro(IlcEveMacro::kESD, "REC Track", "esd_muon_tracks.C+", "esd_muon_tracks","kTRUE,kTRUE", kTRUE));

  if (showBarrel) {
    exec->AddMacro(new IlcEveMacro(IlcEveMacro::kESD, "REC PVTX", "primary_vertex.C+", "primary_vertex", "", kTRUE));
    exec->AddMacro(new IlcEveMacro(IlcEveMacro::kESD, "REC PVTX SPD", "primary_vertex.C+", "primary_vertex_spd", "", kTRUE));
    exec->AddMacro(new IlcEveMacro(IlcEveMacro::kESD, "REC Tracks by category", "esd_tracks.C+", "esd_tracks_by_category", "", kTRUE));
  }
  
  //==============================================================================
  // Additional GUI components
  //==============================================================================
  
  // Macro / data selection
  slot = TEveWindow::CreateWindowInTab(browser->GetTabRight());
  slot->StartEmbedding();
  IlcEveMacroExecutorWindow* exewin = new IlcEveMacroExecutorWindow(exec);
  slot->StopEmbedding("DataSelection");
  exewin->PopulateMacros();
  
  // Event selection tab
  slot = TEveWindow::CreateWindowInTab(browser->GetTabRight());
  slot->StartEmbedding();
  new IlcEveEventSelectorWindow(gClient->GetRoot(), 600, 400, IlcEveEventManager::GetMaster()->GetEventSelector());
  slot->StopEmbedding("Selections");
  
  // QA viewer
  slot = TEveWindow::CreateWindowInTab(browser->GetTabRight());
  slot->StartEmbedding();
  new IlcQAHistViewer(gClient->GetRoot(), 600, 400, kTRUE);
  slot->StopEmbedding("QA histograms");
  
  browser->GetTabRight()->SetTab(1);
  
  browser->StartEmbedding(TRootBrowser::kBottom);
  new IlcEveEventManagerWindow(IlcEveEventManager::GetMaster());
  browser->StopEmbedding("EventCtrl");
  
  slot = TEveWindow::CreateWindowInTab(browser->GetTabRight());
  TEveWindowTab *store_tab = slot->MakeTab();
  store_tab->SetElementNameTitle("WindowStore",
				 "Undocked windows whose previous container is not known\n"
				 "are placed here when the main-frame is closed.");
  gEve->GetWindowManager()->SetDefaultContainer(store_tab);
  
  
  //==============================================================================
  // IlcEve objects - global tools
  //==============================================================================
  
  IlcEveTrackCounter* g_trkcnt = new IlcEveTrackCounter("Primary Counter");
  gEve->AddToListTree(g_trkcnt, kFALSE);
  
  
  //==============================================================================
  // Final stuff
  //==============================================================================
  
  // A refresh to show proper window.
  //gEve->GetViewers()->SwitchColorSet();
  gEve->Redraw3D(kTRUE);
  gSystem->ProcessEvents();
  
  // Register command to call on each event.
  IlcEveEventManager::GetMaster()->AddNewEventCommand("on_new_event();");
  IlcEveEventManager::GetMaster()->GotoEvent(0);
  
  gEve->EditElement(g_trkcnt);
  
  gEve->Redraw3D(kTRUE);
  
  // Assure 3D view rotates around the origin.
  gSystem->ProcessEvents();
  IlcEveMultiView::Instance()->Get3DView()->GetGLViewer()->CurrentCamera().SetCenterVec(0,0,0);
  IlcEveMultiView::Instance()->Get3DView()->GetGLViewer()->RequestDraw();
}

/******************************************************************************/

void on_new_event()
{
  Double_t x[3] = { 0, 0, 0 };
  
  if (IlcEveEventManager::HasESD())
  {
    IlcESDEvent* esd = IlcEveEventManager::AssertESD();
    esd->GetPrimaryVertex()->GetXYZ(x);
    
    TTimeStamp ts(esd->GetTimeStamp());
    TString win_title("Eve Main Window -- Timestamp: ");
    win_title += ts.AsString("s");
    win_title += "; Event # in ESD file: ";
    win_title += esd->GetEventNumberInFile();
    gEve->GetBrowser()->SetWindowName(win_title);
  }
  
  TEveElement* top = gEve->GetCurrentEvent();
  
  IlcEveMultiView *mv = IlcEveMultiView::Instance();
  
  mv->DestroyEventRPhi();
  if (gCenterProjectionsAtPrimaryVertex)
    mv->SetCenterRPhi(x[0], x[1], x[2]);
  mv->ImportEventRPhi(top);
  
  mv->DestroyEventRhoZ();
  if (gCenterProjectionsAtPrimaryVertex)
    mv->SetCenterRhoZ(x[0], x[1], x[2]);
  mv->ImportEventRhoZ(top);
}

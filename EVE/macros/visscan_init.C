// $Id: visscan_init.C 58244 2012-08-21 12:57:19Z hristov $
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
#include <IlcQAHistViewer.h>
#endif

class IlcEveMacroExecutor;
class TEveProjectionManager;
class TEveGeoShape;
class TEveUtil;
class TSystem;
class TInterpreter;


Bool_t gShowMuonRPhi = kFALSE;
Bool_t gShowMuonRhoZ = kTRUE;

Bool_t gCenterProjectionsAtPrimaryVertex = kFALSE;


void visscan_init(const TString& cdburi = "",
		  const TString& path   = ".",
		  Bool_t showMuon = kTRUE,
		  Bool_t showTrd = kFALSE)
{
  
  if (showMuon)
  {
    if (gSystem->Getenv("ILC_ROOT") != 0)
    {
      gInterpreter->AddIncludePath(Form("%s/MUON", gSystem->Getenv("ILC_ROOT")));
      gInterpreter->AddIncludePath(Form("%s/MUON/mapping", gSystem->Getenv("ILC_ROOT")));
    }
  }
  else
  {
    gShowMuonRPhi = gShowMuonRhoZ = kFALSE;
  }
  
  if (cdburi.IsNull() && ! IlcCDBManager::Instance()->IsDefaultStorageSet())
  {
    gEnv->SetValue("Root.Stacktrace", "no");
    Fatal("visscan_init.C", "OCDB path MUST be specified as the first argument.");
  }

  IlcEveEventManager::AddAODfriend("IlcAOD.VertexingHF.root");

  TEveUtil::LoadMacro("ilceve_init.C");
  ilceve_init(cdburi, path, -1);

  // TEveLine::SetDefaultSmooth(1);

  TEveUtil::AssertMacro("VizDB_scan.C");

  IlcEveMacroExecutor *exec    = IlcEveEventManager::GetMaster()->GetExecutor();
  TEveBrowser         *browser = gEve->GetBrowser();
  browser->ShowCloseTab(kFALSE);


  //==============================================================================
  // Geometry, scenes, projections and viewers
  //==============================================================================

  IlcEveMultiView *mv = new IlcEveMultiView;

  mv->SetDepth(-10);

  TEveUtil::LoadMacro("geom_gentle.C+");
  mv->InitGeomGentle(geom_gentle(), geom_gentle_rphi(), geom_gentle_rhoz(), 0);

  if (showTrd) {
    TEveUtil::LoadMacro("geom_gentle_trd.C+");
    mv->InitGeomGentleTrd(geom_gentle_trd());
  }

  if (gShowMuonRPhi || gShowMuonRhoZ) {
    TEveUtil::LoadMacro("geom_gentle_muon.C+");
    mv->InitGeomGentleMuon(geom_gentle_muon(kFALSE), gShowMuonRPhi, gShowMuonRhoZ, kFALSE);
  }

  mv->SetDepth(0);

  //==============================================================================
  // Registration of per-event macros
  //==============================================================================

  exec->AddMacro(new IlcEveMacro(IlcEveMacro::kRunLoader, "SIM Track",   "kine_tracks.C+", "kine_tracks", "", kFALSE));

  exec->AddMacro(new IlcEveMacro(IlcEveMacro::kRunLoader, "SIM Hits ITS", "its_hits.C+",    "its_hits",    "", kFALSE));
  exec->AddMacro(new IlcEveMacro(IlcEveMacro::kRunLoader, "SIM Hits TPC", "tpc_hits.C+",    "tpc_hits",    "", kFALSE));
  exec->AddMacro(new IlcEveMacro(IlcEveMacro::kRunLoader, "SIM Hits T0",  "t0_hits.C+",     "t0_hits",     "", kFALSE));
  exec->AddMacro(new IlcEveMacro(IlcEveMacro::kRunLoader, "SIM Hits FMD", "fmd_hits.C+",    "fmd_hits",    "", kFALSE));
  exec->AddMacro(new IlcEveMacro(IlcEveMacro::kRunLoader, "SIM Hits ACORDE", "acorde_hits.C+",    "acorde_hits",    "", kFALSE));
  exec->AddMacro(new IlcEveMacro(IlcEveMacro::kRunLoader, "SIM Hits EMCAL", "emcal_hits.C+",    "emcal_hits",    "", kFALSE));
  exec->AddMacro(new IlcEveMacro(IlcEveMacro::kRunLoader, "SIM Hits TOF",  "tof_hits.C+",     "tof_hits",     "", kFALSE));
  exec->AddMacro(new IlcEveMacro(IlcEveMacro::kRunLoader, "SIM Hits TRD", "trd_hits.C+",    "trd_hits",    "", kFALSE));
  exec->AddMacro(new IlcEveMacro(IlcEveMacro::kRunLoader, "SIM Hits VZERO", "vzero_hits.C+",    "vzero_hits",    "", kFALSE));

  exec->AddMacro(new IlcEveMacro(IlcEveMacro::kRunLoader, "DIG ITS",     "its_digits.C+",  "its_digits",  "", kFALSE));
  exec->AddMacro(new IlcEveMacro(IlcEveMacro::kRunLoader, "DIG TPC",     "tpc_digits.C+",  "tpc_digits",  "", kFALSE));
  exec->AddMacro(new IlcEveMacro(IlcEveMacro::kRunLoader, "DIG TOF",     "tof_digits.C+",  "tof_digits",  "", kFALSE));
  exec->AddMacro(new IlcEveMacro(IlcEveMacro::kRunLoader, "DIG HMPID",   "hmpid_digits.C+","hmpid_digits","", kFALSE));
  exec->AddMacro(new IlcEveMacro(IlcEveMacro::kRunLoader, "DIG FMD",     "fmd_digits.C+",  "fmd_digits",  "", kFALSE));

  exec->AddMacro(new IlcEveMacro(IlcEveMacro::kRawReader, "RAW ITS",     "its_raw.C+",     "its_raw",     "", kFALSE));
  exec->AddMacro(new IlcEveMacro(IlcEveMacro::kRawReader, "RAW TPC",     "tpc_raw.C+",     "tpc_raw",     "", kFALSE));
  exec->AddMacro(new IlcEveMacro(IlcEveMacro::kRawReader, "RAW TOF",     "tof_raw.C+",     "tof_raw",     "", kFALSE));
  exec->AddMacro(new IlcEveMacro(IlcEveMacro::kRawReader, "RAW HMPID",   "hmpid_raw.C+",   "hmpid_raw",   "", kFALSE));
  exec->AddMacro(new IlcEveMacro(IlcEveMacro::kRawReader, "RAW T0",      "t0_raw.C+",      "t0_raw",      "", kFALSE));
  exec->AddMacro(new IlcEveMacro(IlcEveMacro::kRawReader, "RAW FMD",     "fmd_raw.C+",     "fmd_raw",     "", kFALSE));
  exec->AddMacro(new IlcEveMacro(IlcEveMacro::kRawReader, "RAW VZERO",   "vzero_raw.C+",   "vzero_raw",   "", kFALSE));
  exec->AddMacro(new IlcEveMacro(IlcEveMacro::kRawReader, "RAW ACORDE",  "acorde_raw.C+",  "acorde_raw",  "", kFALSE));

  exec->AddMacro(new IlcEveMacro(IlcEveMacro::kESD, "REC PVTX",             "primary_vertex.C+", "primary_vertex",             "",                kTRUE));
  exec->AddMacro(new IlcEveMacro(IlcEveMacro::kESD, "REC PVTX Ellipse",     "primary_vertex.C+", "primary_vertex_ellipse",     "",                kTRUE));
  exec->AddMacro(new IlcEveMacro(IlcEveMacro::kESD, "REC PVTX Box",         "primary_vertex.C+", "primary_vertex_box",         "kFALSE, 3, 3, 3", kFALSE));
  exec->AddMacro(new IlcEveMacro(IlcEveMacro::kESD, "REC PVTX SPD",         "primary_vertex.C+", "primary_vertex_spd",         "",                kTRUE));
  exec->AddMacro(new IlcEveMacro(IlcEveMacro::kESD, "REC PVTX Ellipse SPD", "primary_vertex.C+", "primary_vertex_ellipse_spd", "",                kTRUE));
  exec->AddMacro(new IlcEveMacro(IlcEveMacro::kESD, "REC PVTX Box SPD",     "primary_vertex.C+", "primary_vertex_box_spd",     "kFALSE, 3, 3, 3", kFALSE));
  exec->AddMacro(new IlcEveMacro(IlcEveMacro::kESD, "REC PVTX TPC",         "primary_vertex.C+", "primary_vertex_tpc",         "",                kFALSE));
  exec->AddMacro(new IlcEveMacro(IlcEveMacro::kESD, "REC PVTX Ellipse TPC", "primary_vertex.C+", "primary_vertex_ellipse_tpc", "",                kFALSE));
  exec->AddMacro(new IlcEveMacro(IlcEveMacro::kESD, "REC PVTX Box TPC",     "primary_vertex.C+", "primary_vertex_box_tpc",     "kFALSE, 3, 3, 3", kFALSE));

  exec->AddMacro(new IlcEveMacro(IlcEveMacro::kESD, "REC V0",   "esd_V0_points.C+",       "esd_V0_points_onfly"));
  exec->AddMacro(new IlcEveMacro(IlcEveMacro::kESD, "REC V0",   "esd_V0_points.C+",       "esd_V0_points_offline"));
  exec->AddMacro(new IlcEveMacro(IlcEveMacro::kESD, "REC V0",   "esd_V0.C+",              "esd_V0"));
  exec->AddMacro(new IlcEveMacro(IlcEveMacro::kESD, "REC CSCD", "esd_cascade_points.C+",  "esd_cascade_points"));
  exec->AddMacro(new IlcEveMacro(IlcEveMacro::kESD, "REC CSCD", "esd_cascade.C+",         "esd_cascade"));
  exec->AddMacro(new IlcEveMacro(IlcEveMacro::kESD, "REC KINK", "esd_kink_points.C+",     "esd_kink_points"));
  exec->AddMacro(new IlcEveMacro(IlcEveMacro::kESD, "REC KINK", "esd_kink.C+",            "esd_kink"));

  exec->AddMacro(new IlcEveMacro(IlcEveMacro::kESD, "REC Tracks",              "esd_tracks.C+", "esd_tracks",              "", kFALSE));
  exec->AddMacro(new IlcEveMacro(IlcEveMacro::kESD, "REC Tracks ITS standalone",          "esd_tracks.C+", "esd_tracks_ITS_standalone",              "", kFALSE));
  exec->AddMacro(new IlcEveMacro(IlcEveMacro::kESD, "REC Tracks ITS",          "esd_tracks.C+", "esd_tracks_ITS",              "", kFALSE));
  exec->AddMacro(new IlcEveMacro(IlcEveMacro::kESD, "REC Tracks TPC",           "esd_tracks.C+", "esd_tracks_TPC",              "", kFALSE));
  exec->AddMacro(new IlcEveMacro(IlcEveMacro::kESD, "REC Tracks MI",           "esd_tracks.C+", "esd_tracks_MI",           "", kFALSE));
  exec->AddMacro(new IlcEveMacro(IlcEveMacro::kESD, "REC Tracks by category",  "esd_tracks.C+", "esd_tracks_by_category",  "", kTRUE));
  exec->AddMacro(new IlcEveMacro(IlcEveMacro::kESD, "REC Tracks by anal cuts", "esd_tracks.C+", "esd_tracks_by_anal_cuts", "", kFALSE));
  exec->AddMacro(new IlcEveMacro(IlcEveMacro::kESD, "REC Tracks Lego", "lego.C+", "lego", "", kFALSE));
  exec->AddMacro(new IlcEveMacro(IlcEveMacro::kESD, "REC Tracks Beams Info", "beams_info.C+", "beams_info", "", kFALSE));

  exec->AddMacro(new IlcEveMacro(IlcEveMacro::kESD, "REC Tracklets SPD", "esd_spd_tracklets.C+", "esd_spd_tracklets", "", kTRUE));

  exec->AddMacro(new IlcEveMacro(IlcEveMacro::kESD, "REC ZDC",      "esd_zdc.C+", "esd_zdc", "", kFALSE));

  exec->AddMacro(new IlcEveMacro(IlcEveMacro::kRunLoader, "REC Clusters",     "clusters.C+",     "clusters", "", kFALSE));
  exec->AddMacro(new IlcEveMacro(IlcEveMacro::kRunLoader, "REC Clusters ITS", "its_clusters.C+", "its_clusters"));
  exec->AddMacro(new IlcEveMacro(IlcEveMacro::kRunLoader, "REC Clusters TPC", "tpc_clusters.C+", "tpc_clusters"));
  exec->AddMacro(new IlcEveMacro(IlcEveMacro::kRunLoader, "REC Clusters TRD", "trd_clusters.C+", "trd_clusters"));
  exec->AddMacro(new IlcEveMacro(IlcEveMacro::kRunLoader, "REC Clusters TOF", "tof_clusters.C+", "tof_clusters"));
  exec->AddMacro(new IlcEveMacro(IlcEveMacro::kRunLoader, "REC Clusters HMPID", "hmpid_clusters.C+", "hmpid_clusters"));
  exec->AddMacro(new IlcEveMacro(IlcEveMacro::kRunLoader, "REC Clusters PHOS", "phos_clusters.C+", "phos_clusters"));

  exec->AddMacro(new IlcEveMacro(IlcEveMacro::kRunLoader, "REC Clusters TPC", "vplot_tpc.C+",    "vplot_tpc", "", kFALSE));

  exec->AddMacro(new IlcEveMacro(IlcEveMacro::kAOD, "ANA HF",   "aod_HF.C+",   "aod_HF",   "", kFALSE));
  exec->AddMacro(new IlcEveMacro(IlcEveMacro::kAOD, "ANA Jets", "jetplane.C+", "jetplane", "", kFALSE));

  exec->AddMacro(new IlcEveMacro(IlcEveMacro::kESD, "DUMP VZERO",   "vzero_dump.C+",   "vzero_dump",   "", kFALSE));
  
  if (showMuon)
  {
    exec->AddMacro(new IlcEveMacro(IlcEveMacro::kRunLoader, "SIM TrackRef MUON", "muon_trackRefs.C+", "muon_trackRefs", "kTRUE", kFALSE));
    exec->AddMacro(new IlcEveMacro(IlcEveMacro::kRawReader, "RAW MUON", "muon_raw.C+", "muon_raw", "", kFALSE));
    exec->AddMacro(new IlcEveMacro(IlcEveMacro::kRunLoader, "DIG MUON", "muon_digits.C+", "muon_digits", "", kFALSE));
    exec->AddMacro(new IlcEveMacro(IlcEveMacro::kRunLoader, "REC Clusters MUON", "muon_clusters.C+", "muon_clusters", "", kTRUE));
    exec->AddMacro(new IlcEveMacro(IlcEveMacro::kESD, "REC Tracks MUON", "esd_muon_tracks.C+", "esd_muon_tracks", "kTRUE,kFALSE", kTRUE));
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

  IlcEveTrackFitter* fitter = new IlcEveTrackFitter();
  gEve->AddToListTree(fitter, 1);
  gEve->AddElement(fitter, gEve->GetEventScene());

  IlcEveTrackCounter* g_trkcnt = new IlcEveTrackCounter("Primary Counter");
  gEve->AddToListTree(g_trkcnt, kFALSE);


  //==============================================================================
  // Final stuff
  //==============================================================================

  // A refresh to show proper window.
  gEve->GetViewers()->SwitchColorSet();
  gEve->Redraw3D(kTRUE);
  gSystem->ProcessEvents();

  // Register command to call on each event.
  IlcEveEventManager::GetMaster()->AddNewEventCommand("on_new_event();");
  IlcEveEventManager::GetMaster()->GotoEvent(0);

  gEve->EditElement(g_trkcnt);

  gEve->Redraw3D(kTRUE);
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

  //mv->DestroyEventRPhi();
  if (gCenterProjectionsAtPrimaryVertex)
    mv->SetCenterRPhi(x[0], x[1], x[2]);
  mv->ImportEventRPhi(top);

  //mv->DestroyEventRhoZ();
  if (gCenterProjectionsAtPrimaryVertex)
    mv->SetCenterRhoZ(x[0], x[1], x[2]);
  mv->ImportEventRhoZ(top);
}

TEveViewer *gSignalView     = 0;
TEveViewer *gBackgroundView = 0;

void embed_init()
{
  TEveUtil::LoadMacro("ilceve_init.C");
  ilceve_init("Signal", -1);

  IlcEveEventManager::GetMaster()->AddNewEventCommand("main_event()");

  // ------------------------------------------------------------------------

  Info("embed_init", "Opening background event ...");
  IlcEveEventManager* bkg =
    IlcEveEventManager::AddDependentManager("Background Event", "Background");
  bkg->IncDenyDestroy();
  bkg->AddNewEventCommand("background_event()");
  gEve->AddToListTree(bkg, kTRUE);

  TEveScene* bs = gEve->SpawnNewScene("Background");
  bs->AddElement(bkg);

  gEve->GetDefaultViewer()->AddScene(bs);

  // ------------------------------------------------------------------------

  TEveBrowser* browser = gEve->GetBrowser();

  TEveWindowSlot *slot = 0;
  TEveWindowPack *pack = 0;

  slot = TEveWindow::CreateWindowInTab(browser->GetTabRight());
  pack = slot->MakePack();
  pack->SetElementName("Parallel View");
  pack->SetHorizontal();
  pack->SetShowTitleBar(kFALSE);

  pack->NewSlot()->MakeCurrent();
  gSignalView = gEve->SpawnNewViewer("Signal View", "");
  gSignalView->AddScene(gEve->GetEventScene());

  pack->NewSlot()->MakeCurrent();
  gBackgroundView = gEve->SpawnNewViewer("Background View", "");
  gBackgroundView->AddScene(bs);

  // ------------------------------------------------------------------------

  TEveUtil::LoadMacro("its_clusters.C+");
  TEveUtil::LoadMacro("tpc_clusters.C+");

  // ------------------------------------------------------------------------

  browser->StartEmbedding(TRootBrowser::kBottom);
  new IlcEveEventManagerWindow(IlcEveEventManager::GetMaster());
  browser->StopEmbedding("EventCtrl");

  // ------------------------------------------------------------------------

  IlcEveEventManager::GetMaster()->GotoEvent(0);
  gEve->Redraw3D(kTRUE);
}

void main_event()
{
  printf("Main Event - post load\n");

  its_clusters();
  tpc_clusters();
}

void background_event()
{
  printf("Background Event - post load\n");

  TEvePointSet* c;

  c = its_clusters();
  c->SetMarkerColor(kOrange);

  c = tpc_clusters();
  c->SetMarkerColor(kCyan);
}

// $Id: ilceve_init.C 55906 2012-04-24 11:50:26Z hristov $
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

/// \ingroup evemacros
/// \file ilceve_init.C

void ilceve_init(const TString& cdburi = "local://$ILC_ROOT/OCDB",
		 const TString& path   = ".", Int_t event=0,
                 const Text_t* esdfile = 0,
                 const Text_t* aodfile = 0,
                 const Text_t* rawfile = 0,
		 Bool_t assert_runloader = kFALSE,
                 Bool_t assert_esd       = kFALSE,
                 Bool_t assert_aod       = kFALSE,
                 Bool_t assert_raw       = kFALSE)
{
  if (cdburi.IsNull() && ! IlcCDBManager::Instance()->IsDefaultStorageSet())
  {
    gEnv->SetValue("Root.Stacktrace", "no");
    Fatal("ilceve_init.C", "OCDB path MUST be specified as the first argument.");
  }

  Info("ilceve_init", "Adding standard macros.");
  TString  hack = gSystem->pwd(); // Problem with TGFileBrowser cding
  ilceve_init_import_macros();
  gSystem->cd(hack);

  TEveUtil::AssertMacro("VizDB_scan.C");

  gSystem->ProcessEvents();

  IlcEveEventManager::SetESDFileName(esdfile);
  IlcEveEventManager::SetRawFileName(rawfile);
  IlcEveEventManager::SetCdbUri(cdburi);
  IlcEveEventManager::SetAssertElements(assert_runloader, assert_esd,
					assert_aod, assert_raw);

  // Open event
  if (path.BeginsWith("alien:") || ! cdburi.BeginsWith("local:"))
  {
    if (gGrid != 0)
    {
      Info("ilceve_init", "TGrid already initializied. Skiping checks and initialization.");
    }
    else
    {
      Info("ilceve_init", "AliEn requested - connecting.");
      if (gSystem->Getenv("GSHELL_ROOT") == 0)
      {
	Error("ilceve_init", "AliEn environment not initialized. Aborting.");
	gSystem->Exit(1);
      }
      if (TGrid::Connect("alien") == 0)
      {
	Error("ilceve_init", "TGrid::Connect() failed. Aborting.");
	gSystem->Exit(1);
      }
    }
  }

  Info("ilceve_init", "Opening event %d from '%s' ...", event, path.Data());
  TString name("Event"); // CINT has trouble with direct "Event".
  new IlcEveEventManager(name, path, event);
  gEve->AddEvent(IlcEveEventManager::GetMaster());
}

void ilceve_init_import_macros()
{
  // Put macros in the list of browsables, add a macro browser to
  // top-level GUI.

  TString macdir("$(ILC_ROOT)/EVE/ilc-macros");
  gSystem->ExpandPathName(macdir);

  TFolder* f = gEve->GetMacroFolder();
  void* dirhandle = gSystem->OpenDirectory(macdir.Data());
  if (dirhandle != 0)
  {
    char* filename;
    TPMERegexp re("\\.C$");
    TObjArray names;
    while ((filename = gSystem->GetDirEntry(dirhandle)) != 0)
    {
      if (re.Match(filename))
	names.AddLast(new TObjString(filename));
    }
    names.Sort();

    for (Int_t ii=0; ii<names.GetEntries(); ++ii)
    {
      TObjString * si = (TObjString*) names.At(ii);
       f->Add(new TEveMacro(Form("%s/%s", macdir.Data(), (si->GetString()).Data())));
    }
  }
  gSystem->FreeDirectory(dirhandle);

  gROOT->GetListOfBrowsables()->Add(new TSystemDirectory(macdir.Data(), macdir.Data()));

  {
    TEveBrowser   *br = gEve->GetBrowser();
    TGFileBrowser *fb = 0;
    fb = br->GetFileBrowser();
    fb->GotoDir(macdir);
    {
      br->StartEmbedding(0);
      fb = br->MakeFileBrowser();
      fb->BrowseObj(f);
      fb->Show();
      br->StopEmbedding();
      br->SetTabTitle("Macros", 0);
      br->SetTab(0, 0);
    }
  }
}

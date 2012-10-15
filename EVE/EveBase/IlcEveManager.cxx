#include <TInterpreter.h>
#include <TGeoManager.h>
#include <TEveBrowser.h>
#include <TEveGedEditor.h>
#include <TEveManager.h>
#include <TEveViewer.h>
#include <TEveSelection.h>
#include <TSystem.h>
#include <TString.h>
#include <TROOT.h>

#include <IlcLog.h>
#include <IlcEveConfigManager.h>
#include <IlcEveMultiView.h>

#include "IlcEveManager.h"

//______________________________________________________________________________
// IlcEveManager
//
// Central aplication manager for IlcEve.
// Manages environment, gEve, elements, GUI, GL scenes and GL viewers.
//
// ILC_ROOT must be defined prior creating this object
ClassImp(IlcEveManager);

IlcEveManager::IlcEveManager(UInt_t w, UInt_t h, Bool_t map_window, Option_t* opt)
	: TEveManager(w, h, map_window, opt)
{

	Init();
	
}

IlcEveManager* IlcEveManager::Create(Bool_t map_window, Option_t* opt)
{
	Int_t w = 1024;
	Int_t h =  768;
  
  if(gEve == 0){
        TEveUtil::SetupEnvironment();
      TEveUtil::SetupGUI();

     gEve = new IlcEveManager(w, h, map_window, opt);
  }
	
	return (IlcEveManager*)gEve;
}

IlcEveManager::~IlcEveManager()
{ 
      IlcEveMultiView* mv = IlcEveMultiView::Instance();
      
      delete mv;
      mv = 0;
}

void IlcEveManager::Init()
{
	GetDefaultViewer()->SetElementName("ORKA 3D View");
	GetSelection()->SetPickToSelect(TEveSelection::kPS_PableCompound);
	GetHighlight()->SetPickToSelect(TEveSelection::kPS_PableCompound);
	TString win_title("Eve Main Window for ORKA");
	gEve->GetBrowser()->SetWindowName(win_title);
	RegisterGeometryAlias("Default", Form("%s/EVE/ilc-data/default_geo.root",  gSystem->Getenv("ILC_ROOT")) );

	IlcEveConfigManager::InitializeMaster(); // initializes menus
}

void IlcEveManager::CloseEveWindow()
{
		// Close button haas been clicked on EVE main window (browser).
   // Cleanup and terminate application.

   TEveBrowser *eb = dynamic_cast<TEveBrowser*>( GetMainWindow() );
 	 eb->DontCallClose();
 		
	TEveGedEditor::DestroyEditors();

}

void IlcEveManager::Terminate()
{

}

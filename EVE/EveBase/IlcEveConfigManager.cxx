// $Id: IlcEveConfigManager.cxx 43420 2010-09-08 09:02:32Z pdebski $
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

#include "IlcEveConfigManager.h"

#include <IlcEveMultiView.h>
#include <TEveManager.h>
#include <TEveBrowser.h>
#include <TEveWindow.h>
#include <TGFileDialog.h>
#include <TGMenu.h>
#include <TGLEmbeddedViewer.h>

#include "IlcEveEventManager.h"
#include "IlcEveMacroExecutor.h"
#include "IlcEveMacroExecutorWindow.h"
#include "IlcEveMacro.h"

class IlcEveMacroExecutor;
class TEveProjectionManager;
class TEveGeoShape;
class TEveUtil;

#include <TSystem.h>
#include <TPRegexp.h>
#include <RVersion.h>

//______________________________________________________________________________
// Full description of IlcEveConfigManager
//

ClassImp(IlcEveConfigManager)

IlcEveConfigManager* IlcEveConfigManager::fgMaster = 0;

namespace
{
 enum EIlcEveMenu_e
 {
   kAEMDefault, kAEMScreen, kAEMProjector, kAEMNotransparency, kAEMTransparentDark, kAEMTransparentLight, kAEMTransparentMonoDark, kAEMTransparentMonoLight, kAEMGreen, kAEMBright, kAEMYellow, kAEMTpc, kAEMAll, kAEM3d, kAEMRphi, kAEMRhoz, kAEMAllhr, kAEM3dhr, kAEMRphihr, kAEMRhozhr, kAEMSavemacros, kAEMLoadmacros, kAEMSave, kAEMOpen, kAEMSetDefault, kAEMResiduals,  kAEMCuts, kAEMVectors, kAEMGui,
   kGLPerspYOZ, kGLPerspXOZ, kGLPerspXOY, kGLXOY, kGLXOZ, kGLZOY, kGLXnOY, kGLXnOZ, kGLZnOY, kGLOrthoRotate
 };
}
 
//______________________________________________________________________________
IlcEveConfigManager* IlcEveConfigManager::InitializeMaster()
{
  // Get main instance.

  static const TEveException kEH("IlcEveConfigManager::InitializeMaster ");

  if (fgMaster)
    throw kEH + "Master already initialized.";

  fgMaster = new IlcEveConfigManager;
  return fgMaster;
}

//______________________________________________________________________________
IlcEveConfigManager* IlcEveConfigManager::GetMaster()
{
  // Get main instance.

  static const TEveException kEH("IlcEveConfigManager::GetMaster ");

  if (!fgMaster)
    throw kEH + "Master not initialized.";

  return fgMaster;
}

//______________________________________________________________________________
IlcEveConfigManager::IlcEveConfigManager() :
  TObject(),
  fAnalysisPopup(0),
  fMenuCameraPopup(0),
  fIlcEvePopup(0),
  fIlcEveGeometries(0),
  fIlcEvePictures(0),
  fIlcEvePicturesHR(0),
  fIlcEveDataSelection(0),
  fIlcEveVizDBs(0),
  fLoadCheck(kFALSE)
{
  // Constructor.
  // Expected TEveManager is already initialized.


  fIlcEveGeometries = new TGPopupMenu(gClient->GetRoot());
  fIlcEveGeometries->AddEntry("&Default", kAEMDefault);
  fIlcEveGeometries->AddEntry("&Screen", kAEMScreen);
  fIlcEveGeometries->AddEntry("&Projector", kAEMProjector);

  fIlcEveGeometries->AddSeparator();

  fIlcEveGeometries->AddEntry("&Low transparency", kAEMNotransparency);

  fIlcEveGeometries->AddSeparator();

  fIlcEveGeometries->AddEntry("&Transparent screen", kAEMTransparentDark);
  fIlcEveGeometries->AddEntry("&Transparent projector", kAEMTransparentLight);
  fIlcEveGeometries->AddEntry("&Transparent mono dark", kAEMTransparentMonoDark);
  fIlcEveGeometries->AddEntry("&Transparent mono light", kAEMTransparentMonoLight);

  fIlcEveGeometries->AddSeparator();

  fIlcEveGeometries->AddEntry("&First collision setup", kAEMGreen);
  fIlcEveGeometries->AddEntry("&Bright", kAEMBright);

  fIlcEveGeometries->AddSeparator();

  fIlcEveGeometries->AddEntry("&TPC Yellow", kAEMYellow);
  fIlcEveGeometries->AddEntry("&TPC Blue", kAEMTpc);

  fIlcEveGeometries->AddSeparator();

  fIlcEvePictures = new TGPopupMenu(gClient->GetRoot());

  fIlcEvePictures->AddEntry("&Save all views", kAEMAll);
  fIlcEvePictures->AddEntry("&Save 3D View",   kAEM3d);
  fIlcEvePictures->AddEntry("&Save RPhi View", kAEMRphi);
  fIlcEvePictures->AddEntry("&Save RhoZ View", kAEMRhoz);

  fIlcEvePictures->AddSeparator();

  fIlcEvePicturesHR = new TGPopupMenu(gClient->GetRoot());

  fIlcEvePicturesHR->AddEntry("&Save all views HR", kAEMAllhr);
  fIlcEvePicturesHR->AddEntry("&Save 3D View HR",   kAEM3dhr);
  fIlcEvePicturesHR->AddEntry("&Save RPhi View HR", kAEMRphihr);
  fIlcEvePicturesHR->AddEntry("&Save RhoZ View HR", kAEMRhozhr);

  fIlcEvePicturesHR->AddSeparator();

  fIlcEveDataSelection = new TGPopupMenu(gClient->GetRoot());

  fIlcEveDataSelection->AddEntry("&Save Data Selection macros", kAEMSavemacros);
  fIlcEveDataSelection->AddEntry("&Load Data Selection macros",   kAEMLoadmacros);

  fIlcEveDataSelection->AddSeparator();

  fIlcEveVizDBs = new TGPopupMenu(gClient->GetRoot());

  fIlcEveVizDBs->AddEntry("&Save VizDB", kAEMSave);
  fIlcEveVizDBs->AddEntry("&Load VizDB", kAEMOpen);

  fIlcEveVizDBs->AddSeparator();

  fIlcEvePopup = new TGPopupMenu(gClient->GetRoot());
  fIlcEvePopup->AddEntry("&Set Default Settings", kAEMSetDefault);
  fIlcEvePopup->AddSeparator();
  fIlcEvePopup->AddPopup("&Geometries/VizDBs", fIlcEveGeometries);
  fIlcEvePopup->AddSeparator();
  fIlcEvePopup->AddPopup("&Pictures", fIlcEvePictures);
  fIlcEvePopup->AddSeparator();
  fIlcEvePopup->AddPopup("&PicturesHR", fIlcEvePicturesHR);
  fIlcEvePopup->AddSeparator();
  fIlcEvePopup->AddPopup("&VizDBs",  fIlcEveVizDBs);
  fIlcEvePopup->AddSeparator();
  fIlcEvePopup->AddPopup("&DataSelection", fIlcEveDataSelection);
  fIlcEvePopup->AddSeparator();

  fMenuCameraPopup = new TGPopupMenu(gClient->GetRoot());
  fMenuCameraPopup->AddEntry("Perspective (Floor XOZ)", kGLPerspXOZ);
  fMenuCameraPopup->AddEntry("Perspective (Floor YOZ)", kGLPerspYOZ);
  fMenuCameraPopup->AddEntry("Perspective (Floor XOY)", kGLPerspXOY);
  fMenuCameraPopup->AddEntry("Orthographic (XOY)", kGLXOY);
  fMenuCameraPopup->AddEntry("Orthographic (XOZ)", kGLXOZ);
  fMenuCameraPopup->AddEntry("Orthographic (ZOY)", kGLZOY);
  fMenuCameraPopup->AddEntry("Orthographic (XnOY)", kGLXnOY);
  fMenuCameraPopup->AddEntry("Orthographic (XnOZ)", kGLXnOZ);
  fMenuCameraPopup->AddEntry("Orthographic (ZnOY)", kGLZnOY);
  fMenuCameraPopup->AddSeparator();
  fMenuCameraPopup->AddEntry("Ortho allow rotate", kGLOrthoRotate);
//   fMenuCameraPopup->AddEntry("Ortho allow dolly",  kGLOrthoDolly);

  fAnalysisPopup = new TGPopupMenu(gClient->GetRoot());  
//  fAnalysisPopup->AddEntry("&Residuals", kAEMResiduals);
//  fAnalysisPopup->AddSeparator();
  fAnalysisPopup->AddEntry("&Cuts", kAEMCuts);
  fAnalysisPopup->AddSeparator();
  fAnalysisPopup->AddEntry("&Momentum vectors", kAEMVectors);
  fAnalysisPopup->AddSeparator();
//  fAnalysisPopup->AddEntry("&Gui Mode", kAEMGui);
//  fAnalysisPopup->AddSeparator();

  fIlcEvePopup->Connect("Activated(Int_t)", "IlcEveConfigManager",
                        this, "IlcEvePopupHandler(Int_t)");

  fMenuCameraPopup->Connect("Activated(Int_t)", "IlcEveConfigManager",
                        this, "IlcEvePopupHandler(Int_t)");

  fAnalysisPopup->Connect("Activated(Int_t)", "IlcEveConfigManager",
                        this, "IlcEvePopupHandler(Int_t)");

  fLoadCheck = kFALSE;

#if ROOT_VERSION_CODE >= ROOT_VERSION(5,25,4)
  TGMenuBar *mBar = gEve->GetBrowser()->GetMenuBar();
  mBar->AddPopup("&IlcEve", fIlcEvePopup, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 4, 0, 0));
  ((TGCompositeFrame*)mBar->GetParent()->GetParent())->Layout();
  mBar->AddPopup("&Camera", fMenuCameraPopup, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 4, 0, 0));
  ((TGCompositeFrame*)mBar->GetParent()->GetParent())->Layout();
  mBar->AddPopup("&Tools", fAnalysisPopup, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 4, 0, 0));
  gEve->GetBrowser()->GetTopMenuFrame()->Layout();
#else
  // Uber hack as TRootBrowser does not provede manu-bar getter.
  TGFrameElement   *xxFE = (TGFrameElement*)   gEve->GetBrowser()->GetList()->First();
  TGCompositeFrame *xxCF = (TGCompositeFrame*) xxFE->fFrame;
  xxFE = (TGFrameElement*)   xxCF->GetList()->First();
  xxCF = (TGCompositeFrame*) xxFE->fFrame;
  xxFE = (TGFrameElement*)   xxCF->GetList()->First();
  xxCF = (TGCompositeFrame*) xxFE->fFrame;
  xxFE = (TGFrameElement*)   xxCF->GetList()->First();
  TGMenuBar *mBar = (TGMenuBar*) xxFE->fFrame;
  mBar->AddPopup("&IlcEve", fIlcEvePopup, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 4, 0, 0));
  ((TGCompositeFrame*)mBar->GetParent()->GetParent())->Layout();
  mBar->AddPopup("&Camera", fMenuCameraPopup, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 4, 0, 0));
  ((TGCompositeFrame*)mBar->GetParent()->GetParent())->Layout();
  mBar->AddPopup("&Tools", fAnalysisPopup, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 4, 0, 0));
  ((TGCompositeFrame*)mBar->GetParent()->GetParent())->Layout();
#endif
}

//==============================================================================

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

namespace
{
const char *gMacroSaveAsTypes[] = {"CINT Macro", "*.C",
                                   0, 0}; //for saving/loading macros

const char *gPictureSaveAsTypes[] = {"PNG Image", "*.png",
                                   0, 0}; //for saving pictures

}

void IlcEveConfigManager::IlcEvePopupHandler(Int_t id)
{
  // Handle user selections from IlcEve popup.

  static const TEveException kEH("IlcEveConfigManager::IlcEvePopupHandler ");

  switch (id)
  {

    case kAEMDefault: //default geometry and VizDB
    {
      IlcEveMultiView *mv = IlcEveMultiView::Instance();

      mv->DestroyAllGeometries(); //destroy RPhi and Rhoz geometries before putting new

      gEve->LoadVizDB("geom_gentle_default.C", kTRUE, kTRUE); //loading geometry

      gEve->LoadVizDB("VizDB_scan.C", kTRUE, kTRUE); //loading VizDB

      if(!gEve->GetViewers()->UseLightColorSet())
        gEve->GetViewers()->SwitchColorSet(); //white background

      gEve->FullRedraw3D();   
      
      break;
    }

    case kAEMScreen: //default geometry with black background
    {
      IlcEveMultiView *mv = IlcEveMultiView::Instance();

      mv->DestroyAllGeometries();

      gEve->LoadVizDB("geom_gentle_default.C", kTRUE, kTRUE);

      gEve->LoadVizDB("VizDB_scan_screen.C", kTRUE, kTRUE);

      if(gEve->GetViewers()->UseLightColorSet())
        gEve->GetViewers()->SwitchColorSet();

      gEve->FullRedraw3D();    

      break;
    }

    case kAEMProjector: //default geometry with white background
    {
      IlcEveMultiView *mv = IlcEveMultiView::Instance();

      mv->DestroyAllGeometries();

      gEve->LoadVizDB("geom_gentle_projector.C", kTRUE, kTRUE);

      gEve->LoadVizDB("VizDB_scan_projector.C", kTRUE, kTRUE);

      if(!gEve->GetViewers()->UseLightColorSet())
        gEve->GetViewers()->SwitchColorSet();

      gEve->FullRedraw3D();    

      break;
    }

    case kAEMNotransparency: //default geometry with low transparency (5%)
    {
      IlcEveMultiView *mv = IlcEveMultiView::Instance();

      mv->DestroyAllGeometries();

      gEve->LoadVizDB("geom_gentle_notransparency.C", kTRUE, kTRUE);

      gEve->LoadVizDB("VizDB_scan.C", kTRUE, kTRUE);

      if(!gEve->GetViewers()->UseLightColorSet())
        gEve->GetViewers()->SwitchColorSet();

      gEve->FullRedraw3D();    

      break;
    }


    case kAEMTransparentDark: //default geometry with black background, high transparency (80%)
    {
      IlcEveMultiView *mv = IlcEveMultiView::Instance();

      mv->DestroyAllGeometries();

      gEve->LoadVizDB("geom_gentle_transparent.C", kTRUE, kTRUE);

      gEve->LoadVizDB("VizDB_scan_transparentdark.C", kTRUE, kTRUE);

      if(gEve->GetViewers()->UseLightColorSet())
        gEve->GetViewers()->SwitchColorSet();

      gEve->FullRedraw3D();   

      break;
    }

    case kAEMTransparentLight: //default geometry with white background, high transparency (80%)
    {
      IlcEveMultiView *mv = IlcEveMultiView::Instance();

      mv->DestroyAllGeometries();

      gEve->LoadVizDB("geom_gentle_transparent.C", kTRUE, kTRUE);

      gEve->LoadVizDB("VizDB_scan_transparentlight.C", kTRUE, kTRUE);

      if(!gEve->GetViewers()->UseLightColorSet())
        gEve->GetViewers()->SwitchColorSet();

      gEve->FullRedraw3D();    

      break;
    }

    case kAEMTransparentMonoDark:
    {
      IlcEveMultiView *mv = IlcEveMultiView::Instance();

      mv->DestroyAllGeometries();

      gEve->LoadVizDB("geom_gentle_transparentdark.C", kTRUE, kTRUE);

      gEve->LoadVizDB("VizDB_scan_transparentdark.C", kTRUE, kTRUE);

      if(gEve->GetViewers()->UseLightColorSet())
        gEve->GetViewers()->SwitchColorSet();

      gEve->FullRedraw3D();   

      break;
    }

    case kAEMTransparentMonoLight:
    {
      IlcEveMultiView *mv = IlcEveMultiView::Instance();

      mv->DestroyAllGeometries();

      gEve->LoadVizDB("geom_gentle_transparentlight.C", kTRUE, kTRUE);

      gEve->LoadVizDB("VizDB_scan_transparentlight.C", kTRUE, kTRUE);

      if(!gEve->GetViewers()->UseLightColorSet())
        gEve->GetViewers()->SwitchColorSet();

      gEve->FullRedraw3D();    

      break;
    }

    case kAEMGreen:
    {
      IlcEveMultiView *mv = IlcEveMultiView::Instance();

      mv->DestroyAllGeometries();

      gEve->LoadVizDB("geom_gentle_green.C", kTRUE, kTRUE);

      gEve->LoadVizDB("VizDB_scan.C", kTRUE, kTRUE);

      if(!gEve->GetViewers()->UseLightColorSet())
        gEve->GetViewers()->SwitchColorSet();

      gEve->FullRedraw3D();    

      break;
    }

    case kAEMBright:
    {
      IlcEveMultiView *mv = IlcEveMultiView::Instance();

      mv->DestroyAllGeometries();

      gEve->LoadVizDB("geom_gentle_bright.C", kTRUE, kTRUE);

      gEve->LoadVizDB("VizDB_scan.C", kTRUE, kTRUE);

      if(gEve->GetViewers()->UseLightColorSet())
        gEve->GetViewers()->SwitchColorSet();

      gEve->FullRedraw3D();

      break;
    }

    case kAEMYellow:
    {
      IlcEveMultiView *mv = IlcEveMultiView::Instance();

      mv->DestroyAllGeometries();

      gEve->LoadVizDB("geom_gentle_yellow.C", kTRUE, kTRUE);

      gEve->LoadVizDB("VizDB_scan_yellow.C", kTRUE, kTRUE);

      if(!gEve->GetViewers()->UseLightColorSet())
        gEve->GetViewers()->SwitchColorSet();

      gEve->FullRedraw3D();    

      break;
    }

    case kAEMTpc:
    {
      IlcEveMultiView *mv = IlcEveMultiView::Instance();

      mv->DestroyAllGeometries();

      gEve->LoadVizDB("geom_gentle_tpc.C", kTRUE, kTRUE);

      gEve->LoadVizDB("VizDB_scan_tpc.C", kTRUE, kTRUE);

      if(!gEve->GetViewers()->UseLightColorSet())
        gEve->GetViewers()->SwitchColorSet();

      gEve->FullRedraw3D();

      break;
    }

    case kAEMAll: //saving pictures from all three viewers
    {
      
     TGFileInfo fi;
     fi.fFileTypes   = gPictureSaveAsTypes;
     fi.fIniDir      = StrDup(""); // current directory
     fi.fFileTypeIdx = 0;
     fi.fOverwrite   = kTRUE;
     new TGFileDialog(gClient->GetDefaultRoot(),
     gEve->GetMainWindow(), kFDSave, &fi); // dialog 
     if (!fi.fFilename) return;

     TPMERegexp filere(".*/([^/]+$)");
     if (filere.Match(fi.fFilename) != 2)
     {
       Warning("IlcEvePopupHandler", "file '%s' bad.", fi.fFilename);
       return;
     }

     TString file1(filere[1]);
     TString file2(filere[1]);
     TString file3(filere[1]);
     
     if (!file1.EndsWith(".png"))
       file1 += "_3D.png"; // adding extensions
       
     if (!file2.EndsWith(".png"))
       file2 += "_RPhi.png"; // adding extensions
       
     if (!file3.EndsWith(".png"))
       file3 += "_RhoZ.png"; // adding extensions

     gSystem->ChangeDirectory(fi.fIniDir);
      
     printf("Saving...\n");

     TEveViewerList *viewers = gEve->GetViewers();
     TEveElement::List_i i = viewers->BeginChildren();
     i++;
     TEveViewer* view3d = ((TEveViewer*)*i);
     view3d->GetGLViewer()->SavePicture(file1); // saving pictures
     i++;
     TEveViewer* viewrphi = ((TEveViewer*)*i);
     viewrphi->GetGLViewer()->SavePicture(file2); // saving pictures
     i++;
     TEveViewer* viewrhoz = ((TEveViewer*)*i);
     viewrhoz->GetGLViewer()->SavePicture(file3); // saving pictures
     
     printf("Done.\n"); 
      
      break;
    }
    
    case kAEM3d: // saving only 3d view
    {
      
     TGFileInfo fi;
     fi.fFileTypes   = gPictureSaveAsTypes;
     fi.fIniDir      = StrDup(""); // current directory
     fi.fFileTypeIdx = 0;
     fi.fOverwrite   = kTRUE;
     new TGFileDialog(gClient->GetDefaultRoot(),
     gEve->GetMainWindow(), kFDSave, &fi);
     if (!fi.fFilename) return;

     TPMERegexp filere(".*/([^/]+$)");
     if (filere.Match(fi.fFilename) != 2)
     {
       Warning("IlcEvePopupHandler", "file '%s' bad.", fi.fFilename);
       return;
     }

     TString file1(filere[1]);
     
     if (!file1.EndsWith(".png"))
       file1 += ".png";

     gSystem->ChangeDirectory(fi.fIniDir);
      
     printf("Saving...\n");

     TEveViewerList *viewers = gEve->GetViewers();
     TEveElement::List_i i = viewers->BeginChildren();
     i++;
     TEveViewer* view3d = ((TEveViewer*)*i);
     view3d->GetGLViewer()->SavePicture(file1);
     
     printf("Done.\n"); 
      
      break;
    }
    
     case kAEMRphi: // saving only RPhi view
    {
      
     TGFileInfo fi;
     fi.fFileTypes   = gPictureSaveAsTypes;
     fi.fIniDir      = StrDup(""); // current directory
     fi.fFileTypeIdx = 0;
     fi.fOverwrite   = kTRUE;
     new TGFileDialog(gClient->GetDefaultRoot(),
     gEve->GetMainWindow(), kFDSave, &fi);
     if (!fi.fFilename) return;

     TPMERegexp filere(".*/([^/]+$)");
     if (filere.Match(fi.fFilename) != 2)
     {
       Warning("IlcEvePopupHandler", "file '%s' bad.", fi.fFilename);
       return;
     }

     TString file1(filere[1]);
     
     if (!file1.EndsWith(".png"))
       file1 += ".png";
     
     gSystem->ChangeDirectory(fi.fIniDir);
      
     printf("Saving...\n");

     TEveViewerList *viewers = gEve->GetViewers();
     TEveElement::List_i i = viewers->BeginChildren();
     i++;
     i++;
     TEveViewer* viewrphi = ((TEveViewer*)*i);
     viewrphi->GetGLViewer()->SavePicture(file1);
     
     printf("Done.\n"); 
      
      break;
    }
    
     case kAEMRhoz: // saving only RhoZ view
    {
    
     TGFileInfo fi;
     fi.fFileTypes   = gPictureSaveAsTypes;
     fi.fIniDir      = StrDup(""); // current directory
     fi.fFileTypeIdx = 0;
     fi.fOverwrite   = kTRUE;
     new TGFileDialog(gClient->GetDefaultRoot(),
     gEve->GetMainWindow(), kFDSave, &fi);
     if (!fi.fFilename) return;

     TPMERegexp filere(".*/([^/]+$)");
     if (filere.Match(fi.fFilename) != 2)
     {
       Warning("IlcEvePopupHandler", "file '%s' bad.", fi.fFilename);
       return;
     }

     TString file1(filere[1]);
       
     if (!file1.EndsWith(".png"))
       file1 += ".png";

     gSystem->ChangeDirectory(fi.fIniDir);
     
     printf("Saving...\n");

     TEveViewerList *viewers = gEve->GetViewers();
     TEveElement::List_i i = viewers->BeginChildren();
     i++;
     i++;
     i++;
     TEveViewer* viewrhoz = ((TEveViewer*)*i);
     viewrhoz->GetGLViewer()->SavePicture(file1);
     
     printf("Done.\n"); 
      
      break;
    }

    case kAEMAllhr: // saving all three views in high resolution
    {
      
     TGFileInfo fi;
     fi.fFileTypes   = gPictureSaveAsTypes;
     fi.fIniDir      = StrDup(""); // current directory
     fi.fFileTypeIdx = 0;
     fi.fOverwrite   = kTRUE;
     new TGFileDialog(gClient->GetDefaultRoot(),
     gEve->GetMainWindow(), kFDSave, &fi);
     if (!fi.fFilename) return;

     TPMERegexp filere(".*/([^/]+$)");
     if (filere.Match(fi.fFilename) != 2)
     {
       Warning("IlcEvePopupHandler", "file '%s' bad.", fi.fFilename);
       return;
     }

     TString file1(filere[1]);
     TString file2(filere[1]);
     TString file3(filere[1]);
     
     if (!file1.EndsWith(".png"))
       file1 += "_3D.png";
       
     if (!file2.EndsWith(".png"))
       file2 += "_RPhi.png";
       
     if (!file3.EndsWith(".png"))
       file3 += "_RhoZ.png";

     gSystem->ChangeDirectory(fi.fIniDir);
      
     printf("Saving...\n");

     TEveViewerList *viewers = gEve->GetViewers();
     TEveElement::List_i i = viewers->BeginChildren();
     i++;
     TEveViewer* view3d = ((TEveViewer*)*i);
     view3d->GetGLViewer()->SavePictureScale(file1,6.0); // getting high resolution
     i++;
     TEveViewer* viewrphi = ((TEveViewer*)*i);
     viewrphi->GetGLViewer()->SavePictureScale(file2,6.0);
     i++;
     TEveViewer* viewrhoz = ((TEveViewer*)*i);
     viewrhoz->GetGLViewer()->SavePictureScale(file3,6.0);
     
     printf("Done.\n"); 
      
      break;
    }
    
    case kAEM3dhr: // saving only 3d view in high resolution
    {
      
     TGFileInfo fi;
     fi.fFileTypes   = gPictureSaveAsTypes;
     fi.fIniDir      = StrDup(""); // current directory
     fi.fFileTypeIdx = 0;
     fi.fOverwrite   = kTRUE;
     new TGFileDialog(gClient->GetDefaultRoot(),
     gEve->GetMainWindow(), kFDSave, &fi);
     if (!fi.fFilename) return;

     TPMERegexp filere(".*/([^/]+$)");
     if (filere.Match(fi.fFilename) != 2)
     {
       Warning("IlcEvePopupHandler", "file '%s' bad.", fi.fFilename);
       return;
     }

     TString file1(filere[1]);
     
     if (!file1.EndsWith(".png"))
       file1 += ".png";

     gSystem->ChangeDirectory(fi.fIniDir);
      
     printf("Saving...\n");

     TEveViewerList *viewers = gEve->GetViewers();
     TEveElement::List_i i = viewers->BeginChildren();
     i++;
     TEveViewer* view3d = ((TEveViewer*)*i);
     view3d->GetGLViewer()->SavePictureScale(file1,4.0);
     
     printf("Done.\n"); 
      
      break;
    }
    
     case kAEMRphihr: // saving only RPhi view in high resolution
    {
      
     TGFileInfo fi;
     fi.fFileTypes   = gPictureSaveAsTypes;
     fi.fIniDir      = StrDup(""); // current directory
     fi.fFileTypeIdx = 0;
     fi.fOverwrite   = kTRUE;
     new TGFileDialog(gClient->GetDefaultRoot(),
     gEve->GetMainWindow(), kFDSave, &fi);
     if (!fi.fFilename) return;

     TPMERegexp filere(".*/([^/]+$)");
     if (filere.Match(fi.fFilename) != 2)
     {
       Warning("IlcEvePopupHandler", "file '%s' bad.", fi.fFilename);
       return;
     }

     TString file1(filere[1]);
     
     if (!file1.EndsWith(".png"))
       file1 += ".png";
     
     gSystem->ChangeDirectory(fi.fIniDir);
      
     printf("Saving...\n");

     TEveViewerList *viewers = gEve->GetViewers();
     TEveElement::List_i i = viewers->BeginChildren();
     i++;
     i++;
     TEveViewer* viewrphi = ((TEveViewer*)*i);
     viewrphi->GetGLViewer()->SavePictureScale(file1,4.0);
     
     printf("Done.\n"); 
      
      break;
    }
    
     case kAEMRhozhr: // saving only RhoZ view in high resolution
    {
    
     TGFileInfo fi;
     fi.fFileTypes   = gPictureSaveAsTypes;
     fi.fIniDir      = StrDup(""); // current directory
     fi.fFileTypeIdx = 0;
     fi.fOverwrite   = kTRUE;
     new TGFileDialog(gClient->GetDefaultRoot(),
     gEve->GetMainWindow(), kFDSave, &fi);
     if (!fi.fFilename) return;

     TPMERegexp filere(".*/([^/]+$)");
     if (filere.Match(fi.fFilename) != 2)
     {
       Warning("IlcEvePopupHandler", "file '%s' bad.", fi.fFilename);
       return;
     }

     TString file1(filere[1]);
       
     if (!file1.EndsWith(".png"))
       file1 += ".png";

     gSystem->ChangeDirectory(fi.fIniDir);
     
     printf("Saving...\n");

     TEveViewerList *viewers = gEve->GetViewers();
     TEveElement::List_i i = viewers->BeginChildren();
     i++;
     i++;
     i++;
     TEveViewer* viewrhoz = ((TEveViewer*)*i);
     viewrhoz->GetGLViewer()->SavePictureScale(file1,4.0);
     
     printf("Done.\n"); 
      
      break;
    }

     case kAEMSavemacros:// Saving Data Selection macros
    {

      IlcEveMacroExecutor *exec = IlcEveEventManager::GetMaster()->GetExecutor();

      exec->SaveAddedMacros();

      break;
    }
 
   case kAEMLoadmacros://Loading Data Selection macros
    {

      TEveBrowser *browser = gEve->GetBrowser();
      browser->ShowCloseTab(kFALSE);

      if(fLoadCheck)
        browser->RemoveTab(TRootBrowser::kRight, 5);//remove the tab with previous DataSelection window
      else
        browser->RemoveTab(TRootBrowser::kRight, 2);


      TGFileInfo fi;
      fi.fFileTypes   = gMacroSaveAsTypes;
      fi.fIniDir      = StrDup(""); // current directory
      fi.fFileTypeIdx = 0;
      fi.fOverwrite   = kTRUE;
      new TGFileDialog(gClient->GetDefaultRoot(), gEve->GetMainWindow(), kFDOpen, &fi);//dialog
      if (!fi.fFilename) return;

      TPMERegexp filere(".*/([^/]+$)");
      if (filere.Match(fi.fFilename) != 2)
      {
        Warning("IlcEvePopupHandler", "file '%s' bad.", fi.fFilename);
        return;
      }
      printf("Loading...\n");

      TString file(filere[1]);
      gSystem->ChangeDirectory(fi.fIniDir);

      TEveUtil::Macro(file);//run macro

      IlcEveEventManager *eman = IlcEveEventManager::GetMaster();//reload event (gEve->Refresh() crashes)
      Int_t ev = eman->GetEventId();
      eman->Close();
      eman->Open();
      eman->GotoEvent(ev);

      printf("Done.\n");

      fLoadCheck = kTRUE;

      break;

    }

    case kAEMSave://saving VizDB
    {
      TGFileInfo fi;
      fi.fFileTypes   = gMacroSaveAsTypes;
      fi.fIniDir      = StrDup(""); // current directory
      fi.fFileTypeIdx = 0;
      fi.fOverwrite   = kTRUE;
      new TGFileDialog(gClient->GetDefaultRoot(), gEve->GetMainWindow(), kFDSave, &fi);
      if (!fi.fFilename) return;

      TPMERegexp filere(".*/([^/]+$)");
      if (filere.Match(fi.fFilename) != 2)
      {
        Warning("IlcEvePopupHandler", "file '%s' bad.", fi.fFilename);
        return;
      }
      printf("Saving...\n");

      TString file(filere[1]);
      if (!file.EndsWith(".C"))
        file += ".C";
      gSystem->ChangeDirectory(fi.fIniDir);
      gEve->SaveVizDB(file);

//Last line "gEve->SaveVizDB(file);" gives macro with many unnecessary
//lines like "x038->SetMinPt(0);" tahat are not interpreted properly later

      string text;
      string all;

      ifstream myfile1(file);
      if(myfile1.is_open())
        {
        while(!myfile1.eof())
          {
            getline(myfile1,text);
            TString check(text);
            if(!(check.EndsWith("MinPt(0);")||check.EndsWith("MaxPt(0);")
               ||check.EndsWith("LimPt(0);")||check.EndsWith("MinP(0);")
               ||check.EndsWith("MaxP(0);")||check.EndsWith("LimP(0);")))
              {
              all += text; //Cut off unnecessary lines and bring everything together
              all += "\n";
              }
          }
        myfile1.close();
        }

      ofstream myfile2(file); //Replacing old file with the one without "bad" lines
      myfile2 << all;
      myfile2.close();

      printf("Done.\n");
      break;

    }

    case kAEMOpen://Opening VizDB
    {
      TGFileInfo fi;
      fi.fFileTypes   = gMacroSaveAsTypes;
      fi.fIniDir      = StrDup(""); // current directory
      fi.fFileTypeIdx = 0;
      fi.fOverwrite   = kTRUE;
      new TGFileDialog(gClient->GetDefaultRoot(), gEve->GetMainWindow(), kFDOpen, &fi);
      if (!fi.fFilename) return;

      TPMERegexp filere(".*/([^/]+$)");
      if (filere.Match(fi.fFilename) != 2)
      {
        Warning("IlcEvePopupHandler", "file '%s' bad.", fi.fFilename);
        return;
      }
      printf("Opening...\n");

      TString file(filere[1]);

      gSystem->ChangeDirectory(fi.fIniDir);

      gEve->LoadVizDB(file, kTRUE, kTRUE);

      gEve->Redraw3D(kTRUE);

      printf("Done.\n");
      break;

    }

    case kAEMSetDefault://Restore default settings
    {

      printf("Setting...\n");

      TEveBrowser *browser = gEve->GetBrowser();
      browser->ShowCloseTab(kFALSE);

      if(fLoadCheck)
        browser->RemoveTab(TRootBrowser::kRight, 5);//remove the tab with previous DataSelection window
      else
        browser->RemoveTab(TRootBrowser::kRight, 2);

//       TEveUtil::Macro("geom_gentle_default.C");
      TEveUtil::Macro("geom_simple.C");
      gEve->LoadVizDB("VizDB_scan.C", kTRUE, kTRUE);
      TEveUtil::Macro("DataSelection_init.C");

//       if(!gEve->GetViewers()->UseLightColorSet())
//         gEve->GetViewers()->SwitchColorSet(); //white background

      IlcEveEventManager *eman = IlcEveEventManager::GetMaster();//reload event (gEve->Refresh() crashes)
      Int_t ev = eman->GetEventId();
      eman->Close();
      eman->Open();
      eman->GotoEvent(ev);

      printf("Done.\n");

      fLoadCheck = kTRUE;

      gEve->Redraw3D(kTRUE);


    }

/*
    case kAEMResiduals:
    {

      TEveUtil::Macro("make_residuals.C");

      break;

    }
*/

    case kAEMCuts:
    {

      TEveUtil::Macro("ilceve_set_cuts.C");

      break;

    }

    case kAEMVectors:
    {

      TEveUtil::Macro("set_momentum_vectors.C");

      break;

    }

/*
    case kAEMGui:
    {

      TEveUtil::Macro("ilceve_gui_mode.C");

      break;

    }
*/


      case kGLPerspXOZ:
      {
	 TGLViewer *v = gEve->GetDefaultGLViewer();
	 v->SetCurrentCamera(TGLViewer::kCameraPerspXOZ);
// 	 v->UpdateScene();
 
         break;
      }

      case kGLPerspYOZ:
      {
	 TGLViewer *v = gEve->GetDefaultGLViewer();
	 v->SetCurrentCamera(TGLViewer::kCameraPerspYOZ);
// 	 v->UpdateScene();
 
         break;
      }

      case kGLPerspXOY:
      {
	 TGLViewer *v = gEve->GetDefaultGLViewer();
	 v->SetCurrentCamera(TGLViewer::kCameraPerspXOY);
// 	 v->UpdateScene();
 
         break;
      }

      case kGLXOY:
      {
	 TGLViewer *v = gEve->GetDefaultGLViewer();
	 v->SetCurrentCamera(TGLViewer::kCameraOrthoXOY);
// 	 v->UpdateScene();
 
	Bool_t state = fMenuCameraPopup->IsEntryChecked(kGLOrthoRotate);
	((TGLOrthoCamera&)v->CurrentCamera()).SetEnableRotate(state);
 
         break;
      }
      case kGLXOZ:
      {
	 TGLViewer *v = gEve->GetDefaultGLViewer();
	 v->SetCurrentCamera(TGLViewer::kCameraOrthoXOZ);
// 	 v->UpdateScene();
 
	Bool_t state = fMenuCameraPopup->IsEntryChecked(kGLOrthoRotate);
	((TGLOrthoCamera&)v->CurrentCamera()).SetEnableRotate(state);
 
         break;
      }

      case kGLZOY:
      {
	 TGLViewer *v = gEve->GetDefaultGLViewer();
	 v->SetCurrentCamera(TGLViewer::kCameraOrthoZOY);
// 	 v->UpdateScene();
 
	Bool_t state = fMenuCameraPopup->IsEntryChecked(kGLOrthoRotate);
	((TGLOrthoCamera&)v->CurrentCamera()).SetEnableRotate(state);

	break;
      }

      case kGLXnOY:
      {
	 TGLViewer *v = gEve->GetDefaultGLViewer();
	 v->SetCurrentCamera(TGLViewer::kCameraOrthoXnOY);
// 	 v->UpdateScene();
 
	Bool_t state = fMenuCameraPopup->IsEntryChecked(kGLOrthoRotate);
	((TGLOrthoCamera&)v->CurrentCamera()).SetEnableRotate(state);
 
         break;
      }
      case kGLXnOZ:
      {
	 TGLViewer *v = gEve->GetDefaultGLViewer();
	 v->SetCurrentCamera(TGLViewer::kCameraOrthoXnOZ);
// 	 v->UpdateScene();
 
	Bool_t state = fMenuCameraPopup->IsEntryChecked(kGLOrthoRotate);
	((TGLOrthoCamera&)v->CurrentCamera()).SetEnableRotate(state);
 
         break;
      }

      case kGLZnOY:
      {
	 TGLViewer *v = gEve->GetDefaultGLViewer();
	 v->SetCurrentCamera(TGLViewer::kCameraOrthoZnOY);
// 	 v->UpdateScene();
 
	Bool_t state = fMenuCameraPopup->IsEntryChecked(kGLOrthoRotate);
	((TGLOrthoCamera&)v->CurrentCamera()).SetEnableRotate(state);

	break;
      }


      case kGLOrthoRotate:
      {
	// Toggle state of the 'Ortho allow rotate' menu entry.
	
	if (fMenuCameraPopup->IsEntryChecked(kGLOrthoRotate))
	  fMenuCameraPopup->UnCheckEntry(kGLOrthoRotate);
	else
	  fMenuCameraPopup->CheckEntry(kGLOrthoRotate);

	Bool_t state = fMenuCameraPopup->IsEntryChecked(kGLOrthoRotate);
	
	TGLViewer *v = gEve->GetDefaultGLViewer();
	((TGLOrthoCamera&)v->CurrentCamera()).SetEnableRotate(state);
	//    v->UpdateScene();
	
	break;
      }

//          ToggleOrthoRotate();
//          break;
//       case kGLOrthoDolly:
//          ToggleOrthoDolly();
//          break;






    default:
    {
      Warning(kEH, "Unknown menu entry.");
      break;
    }
  }
}

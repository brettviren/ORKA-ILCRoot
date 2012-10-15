/**************************************************************************
 * 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * Author: The International Lepton Collider Off-line Project.            *
 *                                                                        *
 * Part of the code has been developed by ALICE Off-line Project.         *
 * Contributors are mentioned in the code where appropriate.              *
 *                                                                        *
 * Permission to use, copy, modify and distribute this software and its   *
 * documentation strictly for non-commercial purposes is hereby granted   *
 * without fee, provided that the above copyright notice appears in all   *
 * copies and that both the copyright notice and this permission notice   *
 * appear in the supporting documentation. The authors make no claims     *
 * about the suitability of this software for any purpose. It is          *
 * provided "as is" without express or implied warranty.                  *
 **************************************************************************/

/////////////////////////////////////////////////////////////////////////
// ILC MENU CLASS                                                    //
// Author: Mayeul   ROUSSELET                                          //
// e-mail: Mayeul.Rousselet@cern.ch                                    //
// Last update:26/08/2003                                              //
/////////////////////////////////////////////////////////////////////////

#include <TGMenu.h>
#include <TGLayout.h>
#include <TGButton.h>
#include <TGFileDialog.h>
#include <TRootHelpDialog.h>
#include <TApplication.h>

#include <IlcMenu.h>
#include <IlcSettingFrame.h>
#include <IlcDisplay2.h>


//extern filetypes;
const char *gIlcFileTypes[] = {"ROOT files","*.root","All files","*",0,0};
const char *gIlcImgTypes[] = {"GIF files","*.gif",0,0};

// Help text
const char helpTxt[] = "\tIlcDisplay v2.0\n\t\tHelp\n\n\nWelcome in the IlcDisplay help.\nHere is a list of useful subjects which discribes\nthe main functionnalities of the software\n \nEvent:Use the arrows to get the next or previous event\nView:Each button corresponds to a different view\nDetectors:Select the module you want to see\nOptions:Select the view mode\nSliders:Use the rapidity (or eta) slider to cut the set of hits\n\tAnd the momentum slider to cut with respect to the momentum\n";


ClassImp(IlcMenu)

//_____________________________________________________________
IlcMenu::IlcMenu(TGCompositeFrame *p, UInt_t w, UInt_t h, UInt_t options)
{
  // Constructor
  fMenuBar = new TGMenuBar(p,w,h,options);
  fToolBar = new TGToolBar(p,60,20,options);
  
  fMenuBarLayout = new TGLayoutHints(kLHintsTop | kLHintsExpandX | kLHintsLeft ,0,0,0,0);
  fMenuBarItemLayout = new TGLayoutHints(kLHintsTop | kLHintsLeft,0,4,0,0);
  
  fMenuFile = new TGPopupMenu(gClient->GetRoot());
  fMenuFile->AddEntry("Open",kIdmOPEN);
  fMenuFile->AddEntry("Save as",kIdmSAVEAS);
  fMenuFile->AddEntry("Close",kIdmCLOSE);
  fMenuFile->AddSeparator();
  fMenuFile->AddEntry("Print",kIdmPRINT);
  fMenuFile->AddEntry("Print setup",kIdmPRINTSETUP);
  fMenuFile->AddSeparator();
  fMenuFile->AddEntry("Exit",kIdmEXIT);
  fMenuFile->DisableEntry(kIdmSAVEAS);
  fMenuFile->Associate(p);
  fMenuBar->AddPopup("File",fMenuFile,fMenuBarItemLayout);
  fMenuFile->Connect("Activated(Int_t)","IlcMenu",this,"DoMenu(Int_t)");
  
  fMenuOptions = new TGPopupMenu(gClient->GetRoot());
  fMenuOptions->AddEntry("Settings",kIdmSETTINGS);
  fMenuOptions->AddEntry("Save settings",kIdmSAVESETTINGS);
  fMenuOptions->Associate(p);
  fMenuBar->AddPopup("Options",fMenuOptions,fMenuBarItemLayout);
  fMenuOptions->Connect("Activated(Int_t)","IlcMenu",this,"DoMenu(Int_t)");
  
  fMenuView = new TGPopupMenu(gClient->GetRoot());
  fMenuView->AddEntry("X3d ",kIdmVIEWX3D);
  fMenuView->AddEntry("OpenGL",kIdmVIEWGL);
  fMenuView->Associate(p);
  fMenuBar->AddPopup("View",fMenuView,fMenuBarItemLayout);
  fMenuView->Connect("Activated(Int_t)","IlcMenu",this,"DoMenu(Int_t)");
  
  fMenuHelp = new TGPopupMenu(gClient->GetRoot());
  fMenuHelp->AddEntry("Help",kIdmHELP);
  fMenuHelp->AddSeparator();
  fMenuHelp->AddEntry("About",kIdmABOUT);
  fMenuHelp->Associate(p);
  fMenuBar->AddPopup("Help",fMenuHelp,fMenuBarItemLayout);
  fMenuHelp->Connect("Activated(Int_t)","IlcMenu",this,"DoMenu(Int_t)");
  
  p->AddFrame(fMenuBar,fMenuBarLayout);
  fTBD = new ToolBarData_t;
  
  fToolBarLayout = new TGLayoutHints(kLHintsTop | kLHintsExpandX,0,0,0,0);
  AddPictureButton("open.xpm","Open file",kIdmOPEN,5);
  AddPictureButton("save.xpm","Save current pad as gif file",kIdmSAVEAS,0);
  AddPictureButton("settings.xpm","Settings",kIdmSETTINGS,5);
  AddPictureButton("help.xpm","Help",kIdmHELP,5);
  AddPictureButton("quit.xpm","Exit IlcDisplay",kIdmEXIT,5);
  AddPictureButton("opengl.xpm","Open GL view",kIdmVIEWGL,5);
  AddPictureButton("x3d.xpm","x3d view",kIdmVIEWX3D,0);
  AddPictureButton("zoomplus16.xpm","Zoom in",kIdbZoomIN,5);
  AddPictureButton("zoommoins16.xpm","Zoom out",kIdbZoomOUT,0);
  AddPictureButton("zoomzone.xpm","Zoom on zone",kIdbZoomZONE,0);
  p->AddFrame(fToolBar,fToolBarLayout);
}

//_____________________________________________________________
IlcMenu::~IlcMenu()
{
  // Destructor
  delete fMenuBarLayout;
  delete fMenuBarItemLayout;
  delete fMenuFile;
  delete fMenuOptions;
  delete fMenuView;
  delete fMenuHelp;
  delete fToolBarLayout;
  delete fToolBar;
  delete fMenuBar;
  delete fTBD;
}

//_____________________________________________________________
void IlcMenu::DoMenu(Int_t id)
{
  switch(id){
  case kIdmOPEN:{
    TGFileInfo fi;
    static TString dir(".");
    fi.fFileTypes = gIlcFileTypes;
    fi.fIniDir = StrDup(dir.Data());
    new TGFileDialog(gClient->GetRoot(),gIlcDisplay2->GetMainFrame(),kFDOpen,&fi);
    if(!fi.fFilename) return;
  }
    break;
  case kIdmEXIT:{
    gApplication->Terminate(0);
  }
    break;
  case kIdmSAVEAS:{
    TGFileInfo fi;
    static TString dir(".");
    fi.fFileTypes = gIlcImgTypes;
    fi.fIniDir = StrDup(dir.Data());
    new TGFileDialog(gClient->GetRoot(),gIlcDisplay2->GetMainFrame(),kFDSave,&fi);
    if(!fi.fFilename) return;
    gIlcDisplay2->SavePadGIF(fi.fFilename);
  }
    break;
  case kIdmSETTINGS:{
    new IlcSettingFrame((TGWindow *)gClient->GetRoot(),(TGWindow *)gIlcDisplay2->GetMainFrame(),200,150);
  }
    break;
  case kIdmHELP:{
    TRootHelpDialog *hd=new TRootHelpDialog((TGWindow *)gClient->GetRoot(),"Help",300,300);
    hd->SetText(helpTxt);	
    hd->Popup();
  }
    break;
    
  case kIdmSAVESETTINGS:{
    gIlcDisplay2->DoSaveSettings();
  }
    break;
  case kIdmVIEWX3D:{
    gIlcDisplay2->DrawX3d();
  }
    break;
  case kIdmVIEWGL:{
    gIlcDisplay2->DrawGL();
  }
    break;
  case kIdbZoomIN:{
    gIlcDisplay2->SetZoomFactor(gIlcDisplay2->GetZoomFactor()*gIlcDisplay2->GetZoomStep());
    gIlcDisplay2->Draw();
  }
    break;
  case kIdbZoomZONE:{
    gIlcDisplay2->SetZoomMode(kTRUE);
    gIlcDisplay2->SetEditable(kFALSE);
  }
    break;
  case kIdbZoomOUT:{		
    gIlcDisplay2->SetZoomFactor(gIlcDisplay2->GetZoomFactor()/gIlcDisplay2->GetZoomStep());
    gIlcDisplay2->Draw();
  }
    break;
  default:break;
  }
}

//_____________________________________________________________
void IlcMenu::DoToolBar(Int_t /*id*/)
{
  TGFrame *frame = (TGFrame *) gTQSender;
  TGButton *bu = (TGButton *) frame;
  DoMenu(bu->WidgetId());
}

//_____________________________________________________________
void IlcMenu::AddPictureButton( const char *fname, const char *tiptext,UInt_t id, UInt_t spacing)
{
  TString filename = StrDup(gIlcDisplay2->GetIconsPath());
  filename.Append(fname);
  
  fTBD->fPixmap=filename.Data();
  fTBD->fTipText = tiptext;
  fTBD->fId = id;
  fTBD->fStayDown = kFALSE;
  
  fToolBar->AddButton(fToolBar,fTBD,spacing);
  if(fTBD->fButton)
    fTBD->fButton->Connect("Clicked()","IlcMenu",this,"DoToolBar(Int_t)");
}

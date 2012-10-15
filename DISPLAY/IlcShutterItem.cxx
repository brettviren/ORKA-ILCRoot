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
// ILC SHUTTER ITEM CLASS                                            //
// Author: Mayeul   ROUSSELET                                          //
// e-mail: Mayeul.Rousselet@cern.ch                                    //
// Last update:26/08/2003                                              //
/////////////////////////////////////////////////////////////////////////

#include <TGShutter.h>
#include <TGFrame.h>
#include <TGButton.h>

#include "IlcShutterItem.h"
#include "IlcDisplay2.h"

ClassImp(IlcShutterItem)

//_____________________________________________________________
IlcShutterItem::IlcShutterItem(TGShutter *s, const char *text, UInt_t id)
{
  // Constructor
  fShutterItem = new TGShutterItem(s, new TGHotString(text), id);
  fMainFrame = (TGCompositeFrame *) fShutterItem->GetContainer();
  s->AddItem(fShutterItem);
}

//_____________________________________________________________
IlcShutterItem::~IlcShutterItem(void)
{
  // Destructor
  delete fButton;
  delete fShutterItem;
  delete fMainFrame;
}

//_____________________________________________________________
void IlcShutterItem::AddTextButton(const char* text, const char *tiptext, UInt_t idb)
{
  //Add a TGTextButton in the TGShutterItem. This button will execute the fonction
  fButton = new TGTextButton(fMainFrame,new TGHotString(text),idb);
  fButton->Resize(100,fButton->GetDefaultHeight());
  fButton->Connect("Clicked()","IlcShutterItem",this,"DoButton(Int_t)");
  fButton->SetToolTipText(tiptext);
  //fButton->Connect("Clicked()","IlcDisplay2",gIlcDisplay2,"DoViews(Int_t)");
  fMainFrame->AddFrame(fButton, new TGLayoutHints( kLHintsTop | kLHintsCenterX ,5,5,10,10));
}

//_____________________________________________________________
void IlcShutterItem::AddPictureButton(const char* file, const char *tiptext, UInt_t idb)
{
  //Add a TGPictureButton in the TGShutterItem. The icon file must be in DISPLAY/icons
  TString filename=StrDup(gIlcDisplay2->GetIconsPath());
  filename.Append(file);
  TGPicture *picture = (TGPicture *) gClient->GetPicture(filename);
  fButton = new TGPictureButton(fMainFrame,picture,idb);		
  fButton->SetToolTipText(tiptext);
  fButton->Connect("Clicked()","IlcShutterItem",this,"DoButton(Int_t)");
  fMainFrame->AddFrame(fButton, new TGLayoutHints( kLHintsTop | kLHintsCenterX ,5,5,10,10));
}

//_____________________________________________________________
void IlcShutterItem::AddCheckButton(const char *text,Int_t idb)
{
  // Add check button
  fButton = new TGCheckButton(fMainFrame,new TGHotString(text),idb);
  fButton->Resize(100,fButton->GetDefaultHeight());
  fButton->Connect("Clicked()","IlcShutterItem",this,"DoButton(Int_t)");
  fMainFrame->AddFrame(fButton, new TGLayoutHints( kLHintsTop | kLHintsLeft ,5,5,10,10));
}

//_____________________________________________________________
void IlcShutterItem::DoButton(Int_t /*pos*/) const
{
  // Show next/previous event if the buttom was used
  TGFrame *frame = (TGFrame *) gTQSender;
  TGButton *bu= (TGButton *) frame;
  int id = bu->WidgetId();
  switch(id){
  case kIdbNextEVENT:{
    gIlcDisplay2->ShowNextEvent(1);
  }
    break;
  case kIdbPrevEVENT:{
    gIlcDisplay2->ShowNextEvent(-1);
  }
    break;
  case kIdbCheckHITS:{
    if(gIlcDisplay2->IsEnabled(kHits)) gIlcDisplay2->Disable(kHits);
    else gIlcDisplay2->Enable(kHits);	  
  }
    break;
  case kIdbCheckCLUSTERS:{
    if(gIlcDisplay2->IsEnabled(kClusters)) gIlcDisplay2->Disable(kClusters);
    else gIlcDisplay2->Enable(kClusters);
  }
    break;
  case kIdbCheckHLT:{
    if(gIlcDisplay2->IsEnabled(kHLT)) gIlcDisplay2->Disable(kHLT);
    else gIlcDisplay2->Enable(kHLT);
  }
    break;
  case kIdbCheckTRACKS:{
    if(gIlcDisplay2->IsEnabled(kTracks)) gIlcDisplay2->Disable(kTracks);
    else gIlcDisplay2->Enable(kTracks);
  }
    break;
  case kIdbSIDEVIEW:{
    gIlcDisplay2->DoView(kIdbSIDEVIEW);
  }
    break;
  case kIdbFRONTVIEW:{
    gIlcDisplay2->DoView(kIdbFRONTVIEW);
  }
    break;
  case kIdbTOPVIEW:{
    gIlcDisplay2->DoView(kIdbTOPVIEW);
  }
    break;
  case kIdbALLVIEW:{
    gIlcDisplay2->DoView(kIdbALLVIEW);
  }
    break;
  default:break;
  }
}


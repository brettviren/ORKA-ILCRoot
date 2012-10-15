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
// ILC SETTING FRAME CLASS                                           //
// Author: Mayeul   ROUSSELET                                          //
// e-mail: Mayeul.Rousselet@cern.ch                                    //
// Last update:26/08/2003                                              //
/////////////////////////////////////////////////////////////////////////

#include <TGWindow.h>
#include <TGFrame.h>
#include <TGLayout.h>
#include <TGNumberEntry.h>
#include <TGLabel.h>
#include <TGButton.h>

#include "IlcDisplay2.h"
#include "IlcSettingFrame.h"

ClassImp(IlcSettingFrame)

//_____________________________________________________________
IlcSettingFrame::IlcSettingFrame(const TGWindow *p, const TGWindow *main, UInt_t w, UInt_t h)
  :TGTransientFrame(p,main,w,h)
{
  // Constructor
  fMainFrame = new TGCompositeFrame((TGWindow *)((TGTransientFrame *)this),w,h,kVerticalFrame);
  
  fZoomStepFrame = new TGCompositeFrame(fMainFrame,w,50,kHorizontalFrame);
  fZoomStepLayout = new TGLayoutHints(kLHintsTop | kLHintsLeft |kLHintsExpandX,5,5,5,5);
  fZoomStepEntry = new TGNumberEntryField(fZoomStepFrame,kIdtZoomSTEP,gIlcDisplay2->GetZoomStep());
  fZoomStepEntry->Connect("ReturnPressed()","IlcSettingFrame",this,"DoSettings(Int_t)");
  fZoomStepLabel = new TGLabel(fZoomStepFrame,"Zoom step");
  fZoomStepFrame->AddFrame(fZoomStepLabel,new TGLayoutHints(kLHintsTop | kLHintsLeft,0,0,0,0));
  fZoomStepFrame->AddFrame(fZoomStepEntry,new TGLayoutHints(kLHintsTop | kLHintsLeft | kLHintsExpandX ,5,5,0,0));
  fMainFrame->AddFrame(fZoomStepFrame,fZoomStepLayout);
  
  fSliderStepFrame = new TGCompositeFrame(fMainFrame,w,50,kHorizontalFrame);
  fSliderStepLayout = new TGLayoutHints(kLHintsTop | kLHintsLeft |kLHintsExpandX,5,5,5,5);
  fSliderStepEntry = new TGNumberEntryField(fSliderStepFrame,kIdtSliderSTEP,gIlcDisplay2->GetSliderStep());
  fSliderStepEntry->Connect("ReturnPressed()","IlcSettingFrame",this,"DoSettings(Int_t)");
  fSliderStepLabel = new TGLabel(fSliderStepFrame,"Slider step");
  fSliderStepFrame->AddFrame(fSliderStepLabel,new TGLayoutHints(kLHintsTop | kLHintsLeft,0,0,0,0));
  fSliderStepFrame->AddFrame(fSliderStepEntry,new TGLayoutHints(kLHintsTop | kLHintsLeft | kLHintsExpandX ,5,5,0,0));
  fMainFrame->AddFrame(fSliderStepFrame,fSliderStepLayout);
  
  fSliderUpdateFrame = new TGCompositeFrame(fMainFrame,w,50,kHorizontalFrame);
  fSliderUpdateLayout = new TGLayoutHints(kLHintsTop | kLHintsLeft |kLHintsExpandX,5,5,5,5);
  fSliderUpdateButton = new TGCheckButton(fSliderUpdateFrame,"Update display on slider move",kIdtSliderUPDATE);
  fSliderUpdateButton->Connect("Clicked()","IlcSettingFrame",this,"DoSettings(Int_t)");
  fIsLoading = kTRUE;
  if(gIlcDisplay2->GetSliderUpdate()) fSliderUpdateButton->SetState(kButtonDown);
  else fSliderUpdateButton->SetState(kButtonUp);
  fIsLoading = kFALSE;
  
  fSliderUpdateFrame->AddFrame(fSliderUpdateButton,new TGLayoutHints(kLHintsTop | kLHintsLeft,0,0,0,0));
  fMainFrame->AddFrame(fSliderUpdateFrame,fSliderUpdateLayout);
  
  AddFrame(fMainFrame,new TGLayoutHints(kLHintsTop | kLHintsLeft |kLHintsExpandX,0,0,0,0));
  fMainFrame->Layout();
  // position relative to the parent's window
  Window_t wdum;
  int ax, ay;
  gVirtualX->TranslateCoordinates(main->GetId(), GetParent()->GetId(),
				  (Int_t)(((TGFrame *) main)->GetWidth() - GetWidth()) >> 1,
				  (Int_t)(((TGFrame *) main)->GetHeight() - GetHeight()) >> 1,
				  ax, ay, wdum);
  Move(ax, ay);
  
  SetWindowName("Setting frame");
  MapSubwindows();
  MapWindow();
  Layout();
}

//_____________________________________________________________
IlcSettingFrame::~IlcSettingFrame()
{
  // Destructor
  delete fZoomStepLayout;
  delete fZoomStepEntry;
  delete fZoomStepLabel;
  delete fSliderStepLayout;
  delete fSliderStepEntry;
  delete fSliderStepLabel;	
  
  delete fSliderUpdateLayout;
  delete fSliderUpdateButton;
  
  delete fMainFrame;
  delete fZoomStepFrame;
  delete fSliderUpdateFrame;
  delete fSliderStepLayout;
}

//_____________________________________________________________
void IlcSettingFrame::DoSettings(Int_t /*pos*/) const
{
  // Updates settings
  TGNumberEntryField *ne = (TGNumberEntryField *) gTQSender;
  int id = ne->WidgetId();
  switch(id){
  case kIdtZoomSTEP:{
    gIlcDisplay2->SetZoomStep(ne->GetNumber());
  }
    break;
  case kIdtSliderSTEP:{
    gIlcDisplay2->SetSliderStep(ne->GetNumber());
  }
    break;
  case kIdtSliderUPDATE:{
    if(fIsLoading) return ;
    if(gIlcDisplay2->GetSliderUpdate()) gIlcDisplay2->SetSliderUpdate(kFALSE);
    else gIlcDisplay2->SetSliderUpdate(kTRUE);
  }
    break;
  default: break;
  }
}


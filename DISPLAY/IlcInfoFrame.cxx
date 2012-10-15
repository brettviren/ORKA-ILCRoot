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
// ILC INFO FRAME CLASS                                              //
// Author: Mayeul   ROUSSELET                                          //
// e-mail: Mayeul.Rousselet@cern.ch                                    //
// Last update:26/08/2003                                              //
/////////////////////////////////////////////////////////////////////////

#include <TGFrame.h>
#include <TGLabel.h>
#include <TGIcon.h>

#include "IlcInfoFrame.h"
#include "IlcDisplay2.h"

ClassImp(IlcInfoFrame)

//_____________________________________________________________
IlcInfoFrame::IlcInfoFrame(TGCompositeFrame *p, UInt_t w, UInt_t h)
{
  // Constructor
  fMainFrame = new TGCompositeFrame(p, w, h, kVerticalFrame);

  fTitleFrame = new TGCompositeFrame(fMainFrame,w,h,kRaisedFrame|kVerticalFrame);
  AddLabel("ORKA",kLHintsTop | kLHintsCenterX);
  AddLabel("Event Display",kLHintsTop | kLHintsCenterX);
  
  TString filename=StrDup(gIlcDisplay2->GetIconsPath());
  filename.Append("ORKA.xpm");
  TGPicture *ilclogo = (TGPicture *) gClient->GetPicture(filename);
  TGIcon *ilc = new TGIcon(fTitleFrame,ilclogo,50,50);
  fTitleFrame->AddFrame(ilc,new TGLayoutHints(kLHintsTop | kLHintsCenterX,0,0,0,0));
  
  AddLabel("Powered by",kLHintsTop | kLHintsCenterX);
  AddLabel("ILCRoot",kLHintsTop | kLHintsCenterX);
  fMainFrame->AddFrame(fTitleFrame,new TGLayoutHints(kLHintsTop | kLHintsLeft | kLHintsExpandX,0,0,0,3));
  
  //Feedback
  fFiguresFrame = new TGCompositeFrame(fMainFrame,w,h,kRaisedFrame|kVerticalFrame);
  TGCompositeFrame *frame = new TGCompositeFrame(fFiguresFrame,w,100, kHorizontalFrame);
  fNbEventLabel = new TGLabel(frame,"");
  TGLabel * label = new TGLabel(frame,"Event number");
  fNbEventLabel->SetText(gIlcDisplay2->GetEventNumber());
  frame->AddFrame(label,new TGLayoutHints(kLHintsTop | kLHintsLeft ,10,0,0,0));
  frame->AddFrame(fNbEventLabel,new TGLayoutHints(kLHintsTop | kLHintsRight,5,10,0,0));
  
  fFiguresFrame->AddFrame(frame,new TGLayoutHints(kLHintsTop | kLHintsLeft | kLHintsExpandX,0,0,0,0));
  
  frame = new TGCompositeFrame(fFiguresFrame,w,100, kHorizontalFrame);
  label = new TGLabel(frame,"Nb Particles");
  fNbParticuleLabel = new TGLabel(frame,"");
  fNbParticuleLabel->SetText(gIlcDisplay2->GetNbParticles());
  frame->AddFrame(label,new TGLayoutHints(kLHintsTop | kLHintsLeft,10,0,0,0));
  frame->AddFrame(fNbParticuleLabel,new TGLayoutHints(kLHintsTop | kLHintsRight,5,10,0,0));
  
  fFiguresFrame->AddFrame(frame,new TGLayoutHints(kLHintsTop | kLHintsLeft | kLHintsExpandX,0,0,0,0));
  
  frame = new TGCompositeFrame(fFiguresFrame,w,100, kHorizontalFrame);
  label = new TGLabel(frame,"Nb Hits");
  fNbHitsLabel = new TGLabel(frame,"");
  fNbHitsLabel->SetText("--");
  frame->AddFrame(label,new TGLayoutHints(kLHintsTop | kLHintsLeft,10,0,0,0));
  frame->AddFrame(fNbHitsLabel,new TGLayoutHints(kLHintsTop | kLHintsRight ,5,10,0,0));
  frame->Layout();
  fFiguresFrame->AddFrame(frame,new TGLayoutHints(kLHintsTop | kLHintsLeft | kLHintsExpandX,0,0,0,0));
  
  frame = new TGCompositeFrame(fFiguresFrame,w,100, kHorizontalFrame);
  label = new TGLabel(frame,"Nb Clusters");
  fNbClustersLabel = new TGLabel(frame,"");
  fNbClustersLabel->SetText("--");
  frame->AddFrame(label,new TGLayoutHints(kLHintsTop | kLHintsLeft,10,0,0,0));
  frame->AddFrame(fNbClustersLabel,new TGLayoutHints(kLHintsTop | kLHintsRight ,5,10,0,0));
  frame->Layout();
  fFiguresFrame->AddFrame(frame,new TGLayoutHints(kLHintsTop | kLHintsLeft | kLHintsExpandX,0,0,0,0));
  fMainFrame->AddFrame(fFiguresFrame,new TGLayoutHints(kLHintsTop | kLHintsLeft | kLHintsExpandX,0,0,2,0));
  
  fMainFrame->Layout();
  fMainFrame->MapSubwindows();
  fMainFrame->MapWindow();
}

//_____________________________________________________________
IlcInfoFrame::~IlcInfoFrame(void){
  // Destructor
  delete fMainFrame;
  delete fTitleFrame;
  delete fFiguresFrame;
  delete fNbParticuleLabel;
  delete fNbEventLabel;
  delete fNbHitsLabel;
}

//_____________________________________________________________
void IlcInfoFrame::AddLabel(const char *text, UInt_t options){
  // Adds new label
  TGLabel * label = new TGLabel(fTitleFrame,text);
  fTitleFrame->AddFrame(label,new TGLayoutHints(options,0,0,0,0));
}

//_____________________________________________________________
void IlcInfoFrame::Update()
{
  // Updates the layout
  fNbParticuleLabel->SetText(gIlcDisplay2->GetNbParticles());
  fNbEventLabel->SetText(gIlcDisplay2->GetEventNumber());
  if(gIlcDisplay2->IsEnabled(kHits))fNbHitsLabel->SetText(gIlcDisplay2->GetNbHits());
  else fNbHitsLabel->SetText("--");
  if(gIlcDisplay2->IsEnabled(kClusters))fNbClustersLabel->SetText(gIlcDisplay2->GetNbClusters());
  else fNbClustersLabel->SetText("--");
  fMainFrame->Layout();
}


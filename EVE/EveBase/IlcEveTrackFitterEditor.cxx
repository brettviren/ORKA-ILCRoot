// $Id: IlcEveTrackFitterEditor.cxx 47044 2011-02-03 10:46:29Z fca $
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

#include "IlcEveTrackFitterEditor.h"
#include "IlcEveTrackFitter.h"

#include <TGButton.h>

//==============================================================================
//==============================================================================
// IlcEveTrackFitterEditor
//==============================================================================

//______________________________________________________________________________
//
// GUI editor for class IlcEveTrackFitter

ClassImp(IlcEveTrackFitterEditor)

IlcEveTrackFitterEditor::IlcEveTrackFitterEditor(const TGWindow *p, Int_t width, Int_t height,
    UInt_t options, Pixel_t back) :
  TGedFrame(p, width, height, options | kVerticalFrame, back),
  fM(0),
  fFit(0),
  fReset(0),
  fStart(0),
  fStop(0),
  fGraph(0)
{
  // Constructor.

  MakeTitle("IlcEveTrackFitter");

  fStart = new TGTextButton(this, "Start");
  AddFrame(fStart, new TGLayoutHints(kLHintsLeft | kLHintsExpandX, 4, 1, 3, 1));
  fStart->Connect("Clicked()",
                  "IlcEveTrackFitterEditor", this, "DoStart()");

  fFit = new TGTextButton(this, "Fit");
  AddFrame(fFit, new TGLayoutHints(kLHintsLeft | kLHintsExpandX, 4, 1, 1, 1));
  fFit->Connect("Clicked()",
                "IlcEveTrackFitterEditor", this, "DoFit()");

  fReset = new TGTextButton(this, "Reset");
  AddFrame(fReset, new TGLayoutHints(kLHintsLeft | kLHintsExpandX, 4, 1, 1, 1));
  fReset->Connect("Clicked()",
                  "IlcEveTrackFitterEditor", this, "DoReset()");

  fStop = new TGTextButton(this, "Stop");
  AddFrame(fStop, new TGLayoutHints(kLHintsLeft | kLHintsExpandX, 4, 1, 1, 4));
  fStop->Connect("Clicked()",
                 "IlcEveTrackFitterEditor", this, "DoStop()");

  fGraph = new TGTextButton(this, "DebugGraph");
  AddFrame(fGraph, new TGLayoutHints(kLHintsLeft | kLHintsExpandX, 4, 2, 4, 1));
  fGraph->Connect("Clicked()",
                 "IlcEveTrackFitterEditor", this, "DoGraph()");
 }

/******************************************************************************/

void IlcEveTrackFitterEditor::SetModel(TObject* obj)
{
  // Set model object.

  fM = static_cast<IlcEveTrackFitter*>(obj);

  if(fM->GetConnected())
  {
    fStart->SetState(kButtonDisabled);
    fStop->SetState(kButtonUp);
  }
  else
  {
    fStop->SetState(kButtonDisabled);
    fStart->SetState(kButtonEngaged);
    fStart->SetState(kButtonUp);
  }
}

/**************************************************************************/

void IlcEveTrackFitterEditor::DoFit()
{
  // Fit slot.

  fM->FitTrack();
  Update();
}

void IlcEveTrackFitterEditor::DoReset()
{
  // Reset slot.

  fM->Reset();
  Update();
}

void IlcEveTrackFitterEditor::DoStart()
{
  // Start selection slot.

  fM->Start();
  fStart->SetState(kButtonDisabled);
  fStop->SetState(kButtonUp);
}

void IlcEveTrackFitterEditor::DoStop()
{
  // Stop selection slot.

  fM->Stop();
  fStop->SetState(kButtonDisabled);
  fStart->SetState(kButtonUp);
}

void IlcEveTrackFitterEditor::DoGraph()
{
  // Draw graph slot.

  fM->DrawDebugGraph();
  Update();
}

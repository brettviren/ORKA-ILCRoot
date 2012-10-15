// $Id: IlcEveCosmicRayFitterEditor.cxx 47102 2011-02-04 11:24:48Z fca $
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

#include "IlcEveCosmicRayFitterEditor.h"
#include "IlcEveCosmicRayFitter.h"

#include <TGButton.h>


//_____________________________________________________________________________
// IlcEveCosmicRayFitterEditor
//

ClassImp(IlcEveCosmicRayFitterEditor)

IlcEveCosmicRayFitterEditor::IlcEveCosmicRayFitterEditor(const TGWindow *p, Int_t width, Int_t height,
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

  MakeTitle("IlcEveCosmicRayFitter");

  fStart = new TGTextButton(this, "Start");
  AddFrame(fStart, new TGLayoutHints(kLHintsLeft | kLHintsExpandX, 4, 1, 3, 1));
  fStart->Connect("Clicked()",
                  "IlcEveCosmicRayFitterEditor", this, "DoStart()");

  fFit = new TGTextButton(this, "Fit");
  AddFrame(fFit, new TGLayoutHints(kLHintsLeft | kLHintsExpandX, 4, 1, 1, 1));
  fFit->Connect("Clicked()",
                "IlcEveCosmicRayFitterEditor", this, "DoFit()");

  fReset = new TGTextButton(this, "Reset");
  AddFrame(fReset, new TGLayoutHints(kLHintsLeft | kLHintsExpandX, 4, 1, 1, 1));
  fReset->Connect("Clicked()",
                  "IlcEveCosmicRayFitterEditor", this, "DoReset()");

  fStop = new TGTextButton(this, "Stop");
  AddFrame(fStop, new TGLayoutHints(kLHintsLeft | kLHintsExpandX, 4, 1, 1, 4));
  fStop->Connect("Clicked()",
                 "IlcEveCosmicRayFitterEditor", this, "DoStop()");

  fGraph = new TGTextButton(this, "DebugGraph");
  AddFrame(fGraph, new TGLayoutHints(kLHintsLeft | kLHintsExpandX, 4, 2, 4, 1));
  fGraph->Connect("Clicked()",
                 "IlcEveCosmicRayFitterEditor", this, "DoGraph()");
 }

/******************************************************************************/

void IlcEveCosmicRayFitterEditor::SetModel(TObject* obj)
{
  // Set model object.

  fM = static_cast<IlcEveCosmicRayFitter*>(obj);

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

void IlcEveCosmicRayFitterEditor::DoFit()
{
  // Fit slot.

  fM->FitTrack();
  Update();
}

void IlcEveCosmicRayFitterEditor::DoReset()
{
  // Reset slot.

  fM->Reset();
  Update();
}

void IlcEveCosmicRayFitterEditor::DoStart()
{
  // Start selection slot.

  fM->Start();
  fStart->SetState(kButtonDisabled);
  fStop->SetState(kButtonUp);
}

void IlcEveCosmicRayFitterEditor::DoStop()
{
  // Stop selection slot.

  fM->Stop();
  fStop->SetState(kButtonDisabled);
  fStart->SetState(kButtonUp);
}

void IlcEveCosmicRayFitterEditor::DoGraph()
{
  // Draw graph slot.

  fM->DrawDebugGraph();
  Update();
}

// $Id: IlcEveLegoEditor.cxx 43215 2010-08-30 12:48:52Z mtadel $
// Author: Stefano Carrazza 2010, CERN, stefano.carrazza@cern.ch

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

#include "IlcEveLego.h"
#include "IlcEveLegoEditor.h"

#include "TColor.h"
#include "TGButton.h"
#include "TGButtonGroup.h"
#include "TGColorSelect.h"
#include "TGComboBox.h"
#include "TGDoubleSlider.h"
#include "TGFrame.h"
#include "TGLabel.h"
#include "TGNumberEntry.h"
#include "TGString.h"
#include "TVirtualPad.h"

//______________________________________________________________________________
// This is the GUI editor for IlcEveLego.
//

ClassImp(IlcEveLegoEditor)

//______________________________________________________________________________
IlcEveLegoEditor::IlcEveLegoEditor(const TGWindow *p, Int_t width, Int_t height,
             UInt_t options, Pixel_t back) :
  TGedFrame(p, width, height, options | kVerticalFrame, back),
  fM(0),
  fAllEventsButton(0),
  fParticlesBG(0),
  fTrackSelection(0),
  fPosCharged(0),
  fNegCharged(0),
  fElectrons(0),
  fMuons(0),
  fPions(0),
  fKaons(0),
  fProtons(0),
  fLabel(0),
  fLabel1(0),
  fThreshold(0),
  fMaxPt(0),
  fSelect(0),
  fParticlesBGAE(0),  
  fTrackSelectionAE(0),
  fPosChargedAE(0),
  fNegChargedAE(0),
  fElectronsAE(0),
  fMuonsAE(0),
  fPionsAE(0),
  fKaonsAE(0),
  fProtonsAE(0),
  fApplyChanges(0),
  fLabelAE(0),
  fLabel1AE(0),
  fThresholdAE(0),
  fMaxPtAE(0),
  fEventControl(0),
  fIsMC(kFALSE),
  fCollisionCandidatesOnly(0)
{
  // Constructor.
  MakeTitle("IlcEveLego");

  // Create widgets

  //------ AllEventsButton ------
  fAllEventsButton = new TGTextButton(this, "Create lego of all events");
  AddFrame(fAllEventsButton, new TGLayoutHints(kLHintsExpandX));
  fAllEventsButton->Connect("Clicked()", "IlcEveLegoEditor", this, "DoAllEvents()");

  //------ Particle Selection ------
  fParticlesBG = new TGGroupFrame(this, "Particle selection:", kVerticalFrame);
  fPosCharged  = new TGCheckButton(fParticlesBG, new TGHotString("&Positive charged"));
  fNegCharged  = new TGCheckButton(fParticlesBG, new TGHotString("&Negative charged"));
  fElectrons   = new TGCheckButton(fParticlesBG, new TGHotString("&Electrons"));
  fMuons       = new TGCheckButton(fParticlesBG, new TGHotString("&Muons"));
  fPions       = new TGCheckButton(fParticlesBG, new TGHotString("&Pions"));
  fKaons       = new TGCheckButton(fParticlesBG, new TGHotString("&Kaons"));
  fProtons     = new TGCheckButton(fParticlesBG, new TGHotString("&Protons"));

  fPosCharged->SetState(kButtonDown);
  fNegCharged->SetState(kButtonDown);
  fElectrons->SetState(kButtonUp);
  fMuons->SetState(kButtonUp);
  fPions->SetState(kButtonUp);
  fKaons->SetState(kButtonUp);
  fProtons->SetState(kButtonUp);

  fPosCharged->Connect("Clicked()", "IlcEveLegoEditor", this, "ShowPosCharge()");
  fNegCharged->Connect("Clicked()", "IlcEveLegoEditor", this, "ShowNegCharge()");
  fElectrons->Connect("Clicked()", "IlcEveLegoEditor", this, "ShowElectrons()");
  fMuons->Connect("Clicked()", "IlcEveLegoEditor", this, "ShowMuons()");
  fPions->Connect("Clicked()", "IlcEveLegoEditor", this, "ShowPions()");
  fKaons->Connect("Clicked()", "IlcEveLegoEditor", this, "ShowKaons()");
  fProtons->Connect("Clicked()", "IlcEveLegoEditor", this, "ShowProtons()");

  fParticlesBG->AddFrame(fPosCharged, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
  fParticlesBG->AddFrame(fNegCharged, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
  fParticlesBG->AddFrame(fElectrons, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
  fParticlesBG->AddFrame(fMuons, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
  fParticlesBG->AddFrame(fPions, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
  fParticlesBG->AddFrame(fKaons, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
  fParticlesBG->AddFrame(fProtons, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
  fParticlesBG->SetLayoutManager(new TGVerticalLayout(fParticlesBG));

  AddFrame(fParticlesBG, new TGLayoutHints(kLHintsExpandX));

  //------ Track selection ------
  fTrackSelection = new TGButtonGroup(this, "Track selection:", kHorizontalFrame);
  fRtracks[0] = new TGRadioButton(fTrackSelection, new TGHotString("&All tracks  "));
  fRtracks[1] = new TGRadioButton(fTrackSelection, new TGHotString("&Primary tracks"));
  fRtracks[0]->SetState(kButtonDown);
  AddFrame(fTrackSelection, new TGLayoutHints(kLHintsExpandX));
  fTrackSelection->Connect("Clicked(Int_t)", "IlcEveLegoEditor", this, "ShowByTracks(Int_t)");

  //------ Track threshold ------
  TGHorizontalFrame *horz = new TGHorizontalFrame(this);
  AddFrame(horz, new TGLayoutHints(kLHintsExpandX | kLHintsCenterY));
  fLabel = new TGLabel(horz, "Tracks maximum Pt (GeV): ");
  horz->AddFrame(fLabel, new TGLayoutHints(kLHintsLeft | kLHintsCenterY));

  fMaxPt = new TGNumberEntry(horz, 10000, 7, -1,
                                 TGNumberFormat::kNESRealOne,
                                 TGNumberFormat::kNEANonNegative,
                                 TGNumberFormat::kNELLimitMinMax,
                                 0, 10000);

  fMaxPt->Connect("ValueSet(Long_t)", "IlcEveLegoEditor", this, "SetMaxPt()");

  horz->AddFrame( fMaxPt, new TGLayoutHints(kLHintsRight | kLHintsNormal | kLHintsCenterY));

  TGHorizontalFrame *horz1 = new TGHorizontalFrame(this);
  AddFrame(horz1, new TGLayoutHints(kLHintsExpandX | kLHintsCenterY));
  fLabel1 = new TGLabel(horz1, "Tracks threshold (GeV): ");
  horz1->AddFrame(fLabel1, new TGLayoutHints(kLHintsLeft | kLHintsCenterY));

  fThreshold = new TGNumberEntry(horz1, 0, 7, -1,
                                 TGNumberFormat::kNESRealOne,
                                 TGNumberFormat::kNEANonNegative,
                                 TGNumberFormat::kNELLimitMinMax,
                                 0, 10000);

  fThreshold->Connect("ValueSet(Long_t)", "IlcEveLegoEditor", this, "SetThreshold()");
  horz1->AddFrame( fThreshold, new TGLayoutHints(kLHintsRight | kLHintsNormal | kLHintsCenterY));


}

//______________________________________________________________________________
void IlcEveLegoEditor::SetModel(TObject* obj)
{
  // Calls the associated IlcEveLego object
  fM = dynamic_cast<IlcEveLego*>(obj);
}

//______________________________________________________________________________
void IlcEveLegoEditor::DoAllEvents()
{
  // Creates the all events editor
  fAllEventsButton->SetEnabled(kFALSE);
  CreateAllEventsEditor();
  fM->LoadAllEvents();
}

//______________________________________________________________________________
void IlcEveLegoEditor::ShowPosCharge()
{   
  // Send particle type to main class
  fM->SetParticleType(0, fPosCharged->IsOn());
}

//______________________________________________________________________________
void IlcEveLegoEditor::ShowNegCharge()
{
  // Send particle type to main class
  fM->SetParticleType(1, fNegCharged->IsOn());
}

//______________________________________________________________________________
void IlcEveLegoEditor::ShowElectrons()
{
  // Send particle type to main class
  fM->SetParticleType(2, fElectrons->IsOn());
}

//______________________________________________________________________________
void IlcEveLegoEditor::ShowMuons()
{
  // Send particle type to main class
  fM->SetParticleType(3, fMuons->IsOn());
}

//______________________________________________________________________________
void IlcEveLegoEditor::ShowPions()
{
  // Send particle type to main class
  fM->SetParticleType(4, fPions->IsOn());
}

//______________________________________________________________________________
void IlcEveLegoEditor::ShowKaons()
{
  // Send particle type to main class
  fM->SetParticleType(5, fKaons->IsOn());
}

//______________________________________________________________________________
void IlcEveLegoEditor::ShowProtons()
{
  // Send particle type to main class
  fM->SetParticleType(6, fProtons->IsOn());
}

//______________________________________________________________________________
void IlcEveLegoEditor::ShowPosChargeAE()
{
  // Send particle type to main class
  fM->SetParticleTypeAE(0, fPosChargedAE->IsOn());
}

//______________________________________________________________________________
void IlcEveLegoEditor::ShowNegChargeAE()
{
  // Send particle type to main class
  fM->SetParticleTypeAE(1, fNegChargedAE->IsOn());
}

//______________________________________________________________________________
void IlcEveLegoEditor::ShowElectronsAE()
{
  // Send particle type to main class
  fM->SetParticleTypeAE(2, fElectronsAE->IsOn());
}

//______________________________________________________________________________
void IlcEveLegoEditor::ShowMuonsAE()
{
  // Send particle type to main class
  fM->SetParticleTypeAE(3, fMuonsAE->IsOn());
}

//______________________________________________________________________________
void IlcEveLegoEditor::ShowPionsAE()
{
  // Send particle type to main class
  fM->SetParticleTypeAE(4, fPionsAE->IsOn());
}

//______________________________________________________________________________
void IlcEveLegoEditor::ShowKaonsAE()
{
  // Send particle type to main class
  fM->SetParticleTypeAE(5, fKaonsAE->IsOn());
}

//______________________________________________________________________________
void IlcEveLegoEditor::ShowProtonsAE()
{
  // Send particle type to main class
  fM->SetParticleTypeAE(6, fProtonsAE->IsOn());
}

//______________________________________________________________________________
void IlcEveLegoEditor::SetMaxPt()
{
  // Send particle type to main class
  fM->SetMaxPt(fMaxPt->GetNumber());
}

//______________________________________________________________________________
void IlcEveLegoEditor::SetMaxPtAE()
{
  // Send particle type to main class
  fM->SetMaxPtAE(fMaxPtAE->GetNumber());
}

//______________________________________________________________________________
void IlcEveLegoEditor::SetThreshold()
{
  // Send particle type to main class
  fM->SetThreshold(fThreshold->GetNumber());
}

//______________________________________________________________________________
void IlcEveLegoEditor::SetThresholdAE()
{
  // Send particle type to main class
  fM->SetThresholdAE(fThresholdAE->GetNumber());
}

//______________________________________________________________________________
void IlcEveLegoEditor::ShowByTracks(Int_t id)
{
  // Send particle type to main class
  fM->SetTracks(id);
}

//______________________________________________________________________________
void IlcEveLegoEditor::ShowByTracksAE(Int_t id)
{
  // Send particle type to main class
  fM->SetTracksAE(id);
}

//______________________________________________________________________________
void IlcEveLegoEditor::CreateAllEventsEditor()
{
   // Create the GUI of all events
   TGVerticalFrame *this2 = this->CreateEditorTabSubFrame("All events style");

   //------ Event control ------
   fEventControl = new TGButtonGroup(this2, "Event control:", kVerticalFrame);
   fIsMC = new TGCheckButton(fEventControl, new TGHotString("&Data is from simulation (MC)"));
   fCollisionCandidatesOnly = new TGCheckButton(fEventControl, new TGHotString("&Only collision candidates events"));

   //------ Simulation checkbox ------
   fIsMC->SetState(kButtonUp);
   fCollisionCandidatesOnly->SetState(kButtonUp);
   fIsMC->Connect("Clicked()", "IlcEveLegoEditor", this, "DataIsMC()");
   fCollisionCandidatesOnly->Connect("Clicked()", "IlcEveLegoEditor", this, "CollisionCandidatesOnly()");
   this2->AddFrame(fEventControl, new TGLayoutHints(kLHintsExpandX));

   //------ Particle selection ------
   fParticlesBGAE = new TGButtonGroup(this2, "Particle selection:", kVerticalFrame);
   fPosChargedAE  = new TGCheckButton(fParticlesBGAE, new TGHotString("&Positive charged"));
   fNegChargedAE  = new TGCheckButton(fParticlesBGAE, new TGHotString("&Negative charged"));
   fElectronsAE   = new TGCheckButton(fParticlesBGAE, new TGHotString("&Electrons"));
   fMuonsAE       = new TGCheckButton(fParticlesBGAE, new TGHotString("&Muons"));
   fPionsAE       = new TGCheckButton(fParticlesBGAE, new TGHotString("&Pions"));
   fKaonsAE       = new TGCheckButton(fParticlesBGAE, new TGHotString("&Kaons"));
   fProtonsAE     = new TGCheckButton(fParticlesBGAE, new TGHotString("&Protons"));

   fPosChargedAE->SetState(kButtonDown);
   fNegChargedAE->SetState(kButtonDown);
   fElectronsAE->SetState(kButtonUp);
   fMuonsAE->SetState(kButtonUp);
   fPionsAE->SetState(kButtonUp);
   fKaonsAE->SetState(kButtonUp);
   fProtonsAE->SetState(kButtonUp);

   fPosChargedAE->Connect("Clicked()", "IlcEveLegoEditor", this, "ShowPosChargeAE()");
   fNegChargedAE->Connect("Clicked()", "IlcEveLegoEditor", this, "ShowNegChargeAE()");
   fElectronsAE->Connect("Clicked()", "IlcEveLegoEditor", this, "ShowElectronsAE()");
   fMuonsAE->Connect("Clicked()", "IlcEveLegoEditor", this, "ShowMuonsAE()");
   fPionsAE->Connect("Clicked()", "IlcEveLegoEditor", this, "ShowPionsAE()");
   fKaonsAE->Connect("Clicked()", "IlcEveLegoEditor", this, "ShowKaonsAE()");
   fProtonsAE->Connect("Clicked()", "IlcEveLegoEditor", this, "ShowProtonsAE()");

   this2->AddFrame(fParticlesBGAE, new TGLayoutHints(kLHintsExpandX));

   //------ Apply particle selection criteria ------
   fApplyChanges = new TGTextButton(this2, "Apply particle selection");
   fApplyChanges->Connect("Clicked()", "IlcEveLegoEditor", this, "ApplyChanges()");
   this2->AddFrame(fApplyChanges, new TGLayoutHints(kLHintsExpandX));

   //------ Track selection ------
   fTrackSelectionAE = new TGButtonGroup(this2, "Track selection:", kHorizontalFrame);
   fRtracksAE[0] = new TGRadioButton(fTrackSelectionAE, new TGHotString("&All tracks  "));
   fRtracksAE[1] = new TGRadioButton(fTrackSelectionAE, new TGHotString("&Primary tracks"));
   fRtracksAE[0]->SetState(kButtonDown);   
   fTrackSelectionAE->Connect("Clicked(Int_t)", "IlcEveLegoEditor", this, "ShowByTracksAE(Int_t)");
   this2->AddFrame(fTrackSelectionAE, new TGLayoutHints(kLHintsExpandX));

   //------ Threshold setup ------
   TGHorizontalFrame *horzAE = new TGHorizontalFrame(this2);

   fLabelAE = new TGLabel(horzAE, "Tracks maximum Pt (GeV): ");
   horzAE->AddFrame(fLabelAE, new TGLayoutHints(kLHintsLeft | kLHintsCenterY));

   fMaxPtAE = new TGNumberEntry(horzAE, 10000, 7, -1,
                                  TGNumberFormat::kNESRealOne,
                                  TGNumberFormat::kNEANonNegative,
                                  TGNumberFormat::kNELLimitMinMax,
                                  0, 10000);

   fMaxPtAE->Connect("ValueSet(Long_t)", "IlcEveLegoEditor", this, "SetMaxPtAE()");

   horzAE->AddFrame( fMaxPtAE, new TGLayoutHints(kLHintsRight | kLHintsNormal | kLHintsCenterY));
   this2->AddFrame(horzAE, new TGLayoutHints(kLHintsExpandX | kLHintsCenterY));

   TGHorizontalFrame *horz1AE = new TGHorizontalFrame(this2);

   fLabel1AE = new TGLabel(horz1AE, "Tracks threshold (GeV): ");
   horz1AE->AddFrame(fLabel1AE, new TGLayoutHints(kLHintsLeft | kLHintsCenterY));

   fThresholdAE = new TGNumberEntry(horz1AE, 0, 7, -1,
                                  TGNumberFormat::kNESRealOne,
                                  TGNumberFormat::kNEANonNegative,
                                  TGNumberFormat::kNELLimitMinMax,
                                  0, 10000);

   fThresholdAE->Connect("ValueSet(Long_t)", "IlcEveLegoEditor", this, "SetThresholdAE()");
   horz1AE->AddFrame( fThresholdAE, new TGLayoutHints(kLHintsRight | kLHintsNormal | kLHintsCenterY));

   this2->AddFrame(horz1AE, new TGLayoutHints(kLHintsExpandX | kLHintsCenterY));
}

//______________________________________________________________________________
void IlcEveLegoEditor::ApplyChanges()
{
  // Apply particle selection for all events
  fM->ApplyParticleTypeSelectionAE();
}

//______________________________________________________________________________
void IlcEveLegoEditor::DataIsMC()
{
  // Set data type
  fM->SwitchDataType(fIsMC->IsOn());
}

//______________________________________________________________________________
void IlcEveLegoEditor::CollisionCandidatesOnly()
{
  // Activate collision candidates only
  fM->SetCollisionCandidatesOnly();
}

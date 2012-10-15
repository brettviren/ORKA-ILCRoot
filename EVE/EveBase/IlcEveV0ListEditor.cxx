// @(#)root/eve:$Id: IlcEveV0ListEditor.cxx 47044 2011-02-03 10:46:29Z fca $
// Author: Matevz Tadel 2007

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

#include "IlcEveV0ListEditor.h"
#include "IlcEveV0.h"

#include "TEveGValuators.h"

#include "TVirtualPad.h"
#include "TColor.h"

// Cleanup these includes:
#include "TGLabel.h"
#include "TGButton.h"
#include "TGNumberEntry.h"
#include "TGColorSelect.h"
#include "TGDoubleSlider.h"
#include "TGComboBox.h"
#include "TGLabel.h"

//______________________________________________________________________________
// GUI editor for IlcEveV0List.
//

ClassImp(IlcEveV0ListEditor)

//______________________________________________________________________________
IlcEveV0ListEditor::IlcEveV0ListEditor(const TGWindow *p, Int_t width, Int_t height,
             UInt_t options, Pixel_t back) :
  TGedFrame(p, width, height, options | kVerticalFrame, back),
  fM(0),
  fMinMaxRCut(0),
  fMinMaxDaughterDCA(0),
  fMinMaxPt(0),
  fNegativeSpecies(0),
  fPositiveSpecies(0),
  fNegativeCheckMaxPidProbability(0),
  fPositiveCheckMaxPidProbability(0),
  fNegativeLevelPidProbability(0),
  fPositiveLevelPidProbability(0),
  fMinMaxInvariantMass(0)
{
  // Constructor.

  MakeTitle("IlcEveV0List");

  // Create widgets
  // fXYZZ = new TGSomeWidget(this, ...);
  // AddFrame(fXYZZ, new TGLayoutHints(...));
  // fXYZZ->Connect("SignalName()", "Reve::IlcEveV0ListEditor", this, "DoXYZZ()");

   fMinMaxRCut = new TEveGDoubleValuator(this,"Radius:", 130, 0);
   fMinMaxRCut->SetNELength(5);
   fMinMaxRCut->SetLabelWidth(74);
   fMinMaxRCut->Build();
   fMinMaxRCut->GetSlider()->SetWidth(200);
   fMinMaxRCut->SetLimits(0, 100, TGNumberFormat::kNESRealOne);
   fMinMaxRCut->Connect("ValueSet()", "IlcEveV0ListEditor", this, "DoMinMaxRCut()");
   AddFrame(fMinMaxRCut, new TGLayoutHints(kLHintsTop, 1, 1, 1, 1));

   fMinMaxDaughterDCA = new TEveGDoubleValuator(this,"DCA:", 130, 0);
   fMinMaxDaughterDCA->SetNELength(5);
   fMinMaxDaughterDCA->SetLabelWidth(74);
   fMinMaxDaughterDCA->Build();
   fMinMaxDaughterDCA->GetSlider()->SetWidth(200);
   fMinMaxDaughterDCA->SetLimits(0, 1, TGNumberFormat::kNESRealTwo);
   fMinMaxDaughterDCA->Connect("ValueSet()", "IlcEveV0ListEditor", this, "DoMinMaxDaughterDCA()");
   AddFrame(fMinMaxDaughterDCA, new TGLayoutHints(kLHintsTop, 1, 1, 1, 1));

   fMinMaxPt = new TEveGDoubleValuator(this,"pT:", 80, 0);
   fMinMaxPt->SetNELength(5);
   fMinMaxPt->SetLabelWidth(74);
   fMinMaxPt->Build();
   fMinMaxPt->GetSlider()->SetWidth(200);
   fMinMaxPt->SetLimits(0, 20, TGNumberFormat::kNESRealOne);
   fMinMaxPt->Connect("ValueSet()", "IlcEveV0ListEditor", this, "DoMinMaxPt()");
   AddFrame(fMinMaxPt, new TGLayoutHints(kLHintsTop, 1, 1, 1, 1));

   TGHorizontalFrame* fNegFrame = new TGHorizontalFrame(this);
   TGLabel* labNeg = new TGLabel(fNegFrame, "Neg:");
   fNegFrame->AddFrame(labNeg, new TGLayoutHints(kLHintsLeft|kLHintsBottom, 1, 1, 1, 1));
   fNegativeSpecies = new TGComboBox(fNegFrame);
   fNegativeSpecies->AddEntry("e", 11);
   fNegativeSpecies->AddEntry("mu", 13);
   fNegativeSpecies->AddEntry("pi", 211);
   fNegativeSpecies->AddEntry("K", 321);
   fNegativeSpecies->AddEntry("p", 2212);
   TGListBox* lbNeg = fNegativeSpecies->GetListBox();
   lbNeg->Resize(lbNeg->GetWidth(), 2*18);
   fNegativeSpecies->Resize(45, 20);
   fNegativeSpecies->Connect("Selected(Int_t)", "IlcEveV0ListEditor", this, "DoSelectNegPid(Int_t)");
   fNegFrame->AddFrame(fNegativeSpecies, new TGLayoutHints(kLHintsTop, 1, 1, 1, 1));
   AddFrame(fNegFrame);

   fNegativeCheckMaxPidProbability = new TGCheckButton(fNegFrame, "Check");
   fNegFrame->AddFrame(fNegativeCheckMaxPidProbability, new TGLayoutHints(kLHintsLeft, 0, 2, 1, 1));
   fNegativeCheckMaxPidProbability->Connect("Toggled(Bool_t)", "IlcEveV0ListEditor", this, "DoCheckNegPid()");

   fNegativeLevelPidProbability = new TGNumberEntry(fNegFrame, 0.5, 3, -1, TGNumberFormat::kNESRealTwo, TGNumberFormat::kNEANonNegative,TGNumberFormat::kNELLimitMinMax, 0, 1);
   fNegativeLevelPidProbability->Resize(50,20);
   fNegativeLevelPidProbability->Connect("ValueSet(Long_t)", "IlcEveV0ListEditor", this, "DoSelectNegProb()");
   fNegFrame->AddFrame(fNegativeLevelPidProbability, new TGLayoutHints(kLHintsLeft|kLHintsExpandX, 1, 1, 1, 1));

   TGHorizontalFrame* fPosFrame = new TGHorizontalFrame(this);
   TGLabel* labPos = new TGLabel(fPosFrame, "Pos:");
   fPosFrame->AddFrame(labPos, new TGLayoutHints(kLHintsLeft|kLHintsBottom, 1, 1, 1, 1));
   fPositiveSpecies = new TGComboBox(fPosFrame);
   fPositiveSpecies->AddEntry("e", 11);
   fPositiveSpecies->AddEntry("mu", 13);
   fPositiveSpecies->AddEntry("pi", 211);
   fPositiveSpecies->AddEntry("K", 321);
   fPositiveSpecies->AddEntry("p", 2212);
   TGListBox* lbPos = fPositiveSpecies->GetListBox();
   lbPos->Resize(lbPos->GetWidth(), 2*18);
   fPositiveSpecies->Resize(45, 20);
   fPositiveSpecies->Connect("Selected(Int_t)", "IlcEveV0ListEditor", this, "DoSelectPosPid(Int_t)");
   fPosFrame->AddFrame(fPositiveSpecies, new TGLayoutHints(kLHintsTop, 3, 1, 1, 1));
   AddFrame(fPosFrame);

   fPositiveCheckMaxPidProbability = new TGCheckButton(fPosFrame, "Check");
   fPosFrame->AddFrame(fPositiveCheckMaxPidProbability, new TGLayoutHints(kLHintsLeft, 0, 2, 1, 1));
   fPositiveCheckMaxPidProbability->Connect("Toggled(Bool_t)", "IlcEveV0ListEditor", this, "DoCheckPosPid()");

   fPositiveLevelPidProbability = new TGNumberEntry(fPosFrame, 0.5, 3, -1, TGNumberFormat::kNESRealTwo, TGNumberFormat::kNEANonNegative,TGNumberFormat::kNELLimitMinMax, 0, 1);
   fPositiveLevelPidProbability->Resize(50,20);
   fPositiveLevelPidProbability->Connect("ValueSet(Long_t)", "IlcEveV0ListEditor", this, "DoSelectPosProb()");
   fPosFrame->AddFrame(fPositiveLevelPidProbability, new TGLayoutHints(kLHintsLeft|kLHintsExpandX, 1, 1, 1, 1));

   fMinMaxInvariantMass = new TEveGDoubleValuator(this,"Inv. Mass:", 80, 0);
   fMinMaxInvariantMass->SetNELength(5);
   fMinMaxInvariantMass->SetLabelWidth(74);
   fMinMaxInvariantMass->Build();
   fMinMaxInvariantMass->GetSlider()->SetWidth(200);
   fMinMaxInvariantMass->SetLimits(0, 1.2, TGNumberFormat::kNESRealThree);
   fMinMaxInvariantMass->Connect("ValueSet()", "IlcEveV0ListEditor", this, "DoMinMaxInvariantMass()");
   AddFrame(fMinMaxInvariantMass, new TGLayoutHints(kLHintsTop, 1, 1, 1, 1));
}

/******************************************************************************/

//______________________________________________________________________________
void IlcEveV0ListEditor::SetModel(TObject* obj)
{
  // Set model object.

  fM = static_cast<IlcEveV0List*>(obj);

  // Set values of widgets
  // fXYZZ->SetValue(fM->GetXYZZ());

  fMinMaxRCut->SetValues(fM->fMinRCut, fM->fMaxRCut);
  fMinMaxDaughterDCA->SetValues(fM->fMinDaughterDCA, fM->fMaxDaughterDCA);
  fMinMaxPt->SetValues(fM->fMinPt, fM->fMaxPt);
  fMinMaxInvariantMass->SetValues(fM->fMinInvariantMass, fM->fMaxInvariantMass);
}

/******************************************************************************/

// Implements callback/slot methods

//______________________________________________________________________________
// void IlcEveV0ListEditor::DoXYZZ()
// {
//    // Slot for XYZZ.
//
//    fM->SetXYZZ(fXYZZ->GetValue());
//    Update();
// }

void IlcEveV0ListEditor::DoMinMaxRCut()
{
  fM->FilterByRadius(fMinMaxRCut->GetMin(), fMinMaxRCut->GetMax());
}

void IlcEveV0ListEditor::DoMinMaxDaughterDCA()
{
  fM->FilterByDaughterDCA(fMinMaxDaughterDCA->GetMin(), fMinMaxDaughterDCA->GetMax());
}

void IlcEveV0ListEditor::DoMinMaxPt()
{
  fM->FilterByPt(fMinMaxPt->GetMin(), fMinMaxPt->GetMax());
}

void IlcEveV0ListEditor::DoSelectNegPid(Int_t rNegPid)
{
  fM->SetNegCheckedPid(rNegPid);
  Update();
}

void IlcEveV0ListEditor::DoCheckNegPid()
{
  Int_t   lNegPid  = fM->GetNegCheckedPid();
  Float_t lNegProb = fM->GetNegCheckedProb();
  if (lNegPid) {
    fM->FilterByCheckedPidMinProb(fNegativeCheckMaxPidProbability->IsOn(),0,lNegPid,lNegProb);
    printf("Selection for negative daughter pid %d prob %.2f \n",lNegPid,lNegProb);
    Update();
  }
}

void IlcEveV0ListEditor::DoSelectNegProb()
{
  Float_t rMinNegProb = (Float_t)fNegativeLevelPidProbability->GetNumber();
  fM->SetNegCheckedProb(rMinNegProb);
  Update();
}

void IlcEveV0ListEditor::DoSelectPosPid(Int_t rPosPid)
{
  fM->SetPosCheckedPid(rPosPid);
  Update();
}

void IlcEveV0ListEditor::DoCheckPosPid()
{
  Int_t   lPosPid  = fM->GetPosCheckedPid();
  Float_t lPosProb = fM->GetPosCheckedProb();
  if (lPosPid) {
    fM->FilterByCheckedPidMinProb(fPositiveCheckMaxPidProbability->IsOn(),1,lPosPid,lPosProb);
    printf("Selection for positive daughter pid %d prob %.2f \n",lPosPid,lPosProb);
    Update();
  }
}

void IlcEveV0ListEditor::DoSelectPosProb()
{
  Float_t rMinPosProb = (Float_t)fPositiveLevelPidProbability->GetNumber();
  fM->SetPosCheckedProb(rMinPosProb);
  Update();
}

void IlcEveV0ListEditor::DoMinMaxInvariantMass()
{
  Int_t lNegPid = fM->GetNegCheckedPid();
  Int_t lPosPid = fM->GetPosCheckedPid();
  if( lNegPid && lPosPid)
    fM->FilterByInvariantMass(fMinMaxInvariantMass->GetMin(), fMinMaxInvariantMass->GetMax(),lNegPid,lPosPid);
  else 
    fM->FilterByInvariantMass(fMinMaxInvariantMass->GetMin(), fMinMaxInvariantMass->GetMax(),211,-211);
}

// $Id: IlcEveKinkListEditor.cxx 47044 2011-02-03 10:46:29Z fca $
// Author: Paraskevi Ganoti: 2009

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

#include "IlcEveKinkListEditor.h"
#include "IlcEveKink.h"

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
// GUI editor for IlcEveKinkList.
//

ClassImp(IlcEveKinkListEditor)

//______________________________________________________________________________
IlcEveKinkListEditor::IlcEveKinkListEditor(const TGWindow *p, Int_t width, Int_t height,
             UInt_t options, Pixel_t back) :
  TGedFrame(p, width, height, options | kVerticalFrame, back),
  fM(0),
  fMinMaxRCut(0),
  fMinMaxKinkAngleCut (0),
  fMinMaxPt(0),
  fMinMaxInvariantMass(0),
  fDaughterSpecies(0),
  fDaughterCheckMaxPidProbability(0),
  fDaughterLevelPidProbability(0)
{
  // Constructor.

  MakeTitle("IlcEveKinkList");

   fMinMaxRCut = new TEveGDoubleValuator(this,"Radius:", 130, 0);
   fMinMaxRCut->SetNELength(5);
   fMinMaxRCut->SetLabelWidth(74);
   fMinMaxRCut->Build();
   fMinMaxRCut->GetSlider()->SetWidth(200);
   fMinMaxRCut->SetLimits(0, 250, TGNumberFormat::kNESRealOne);
   fMinMaxRCut->Connect("ValueSet()", "IlcEveKinkListEditor", this, "DoMinMaxRCut()");
   AddFrame(fMinMaxRCut, new TGLayoutHints(kLHintsTop, 1, 1, 1, 1));
   
   fMinMaxKinkAngleCut = new TEveGDoubleValuator(this,"Kink Angle:", 130, 0);
   fMinMaxKinkAngleCut->SetNELength(5);
   fMinMaxKinkAngleCut->SetLabelWidth(74);
   fMinMaxKinkAngleCut->Build();
   fMinMaxKinkAngleCut->GetSlider()->SetWidth(200);
   fMinMaxKinkAngleCut->SetLimits(0, 280, TGNumberFormat::kNESRealOne);
   fMinMaxKinkAngleCut->Connect("ValueSet()", "IlcEveKinkListEditor", this, "DoMinMaxKinkAngleCut()");
   AddFrame(fMinMaxKinkAngleCut, new TGLayoutHints(kLHintsTop, 1, 1, 1, 1));  

   fMinMaxPt = new TEveGDoubleValuator(this,"pT:", 80, 0);
   fMinMaxPt->SetNELength(5);
   fMinMaxPt->SetLabelWidth(74);
   fMinMaxPt->Build();
   fMinMaxPt->GetSlider()->SetWidth(200);
   fMinMaxPt->SetLimits(0, 20, TGNumberFormat::kNESRealOne);
   fMinMaxPt->Connect("ValueSet()", "IlcEveKinkListEditor", this, "DoMinMaxPt()");
   AddFrame(fMinMaxPt, new TGLayoutHints(kLHintsTop, 1, 1, 1, 1));

   fMinMaxInvariantMass = new TEveGDoubleValuator(this,"Inv. Mass:", 80, 0);
   fMinMaxInvariantMass->SetNELength(5);
   fMinMaxInvariantMass->SetLabelWidth(74);
   fMinMaxInvariantMass->Build();
   fMinMaxInvariantMass->GetSlider()->SetWidth(200);
   fMinMaxInvariantMass->SetLimits(0, 1.0, TGNumberFormat::kNESRealThree);
   fMinMaxInvariantMass->Connect("ValueSet()", "IlcEveKinkListEditor", this, "DoMinMaxInvariantMass()");
   AddFrame(fMinMaxInvariantMass, new TGLayoutHints(kLHintsTop, 1, 1, 1, 1));
   
   TGHorizontalFrame* fDaugFrame = new TGHorizontalFrame(this);
   TGLabel* labDaug = new TGLabel(fDaugFrame, "Daug:");
   fDaugFrame->AddFrame(labDaug, new TGLayoutHints(kLHintsLeft|kLHintsBottom, 1, 1, 1, 1));
   fDaughterSpecies = new TGComboBox(fDaugFrame);
   fDaughterSpecies->AddEntry("e", 11);
   fDaughterSpecies->AddEntry("mu", 13);
   fDaughterSpecies->AddEntry("pi", 211);
   TGListBox* lbaug = fDaughterSpecies->GetListBox();
   lbaug->Resize(lbaug->GetWidth(), 2*18);
   fDaughterSpecies->Resize(45, 20);
   fDaughterSpecies->Connect("Selected(Int_t)", "IlcEveKinkListEditor", this, "DoSelectDaugPid(Int_t)");
   fDaugFrame->AddFrame(fDaughterSpecies, new TGLayoutHints(kLHintsTop, 1, 1, 1, 1));
   AddFrame(fDaugFrame);

   fDaughterCheckMaxPidProbability = new TGCheckButton(fDaugFrame, "Check");
   fDaugFrame->AddFrame(fDaughterCheckMaxPidProbability, new TGLayoutHints(kLHintsLeft, 0, 2, 1, 1));
   fDaughterCheckMaxPidProbability->Connect("Toggled(Bool_t)", "IlcEveKinkListEditor", this, "DoCheckDaugPid()");

   fDaughterLevelPidProbability = new TGNumberEntry(fDaugFrame, 0.5, 3, -1, TGNumberFormat::kNESRealTwo, TGNumberFormat::kNEANonNegative,TGNumberFormat::kNELLimitMinMax, 0, 1);
   fDaughterLevelPidProbability->Resize(50,20);
   fDaughterLevelPidProbability->Connect("ValueSet(Long_t)", "IlcEveKinkListEditor", this, "DoSelectDaugProb()");
   fDaugFrame->AddFrame(fDaughterLevelPidProbability, new TGLayoutHints(kLHintsLeft|kLHintsExpandX, 1, 1, 1, 1)); 
}

/******************************************************************************/

//______________________________________________________________________________
void IlcEveKinkListEditor::SetModel(TObject* obj)
{
  // Set model object.

  fM = static_cast<IlcEveKinkList*>(obj);

  // Set values of widgets
  // fXYZZ->SetValue(fM->GetXYZZ());

  fMinMaxRCut->SetValues(fM->fMinRCut, fM->fMaxRCut);
  fMinMaxKinkAngleCut->SetValues(fM->fMinKinkAngle, fM->fMaxKinkAngle);
  fMinMaxPt->SetValues(fM->fMinPt, fM->fMaxPt);
  fMinMaxInvariantMass->SetValues(fM->fMinInvariantMass, fM->fMaxInvariantMass);
}

/******************************************************************************/

// Implements callback/slot methods

//______________________________________________________________________________

void IlcEveKinkListEditor::DoMinMaxRCut()
{
  fM->FilterByRadius(fMinMaxRCut->GetMin(), fMinMaxRCut->GetMax());
}

void IlcEveKinkListEditor::DoMinMaxKinkAngleCut()
{
  fM->FilterByKinkAngle(fMinMaxRCut->GetMin(), fMinMaxRCut->GetMax());
}

void IlcEveKinkListEditor::DoMinMaxPt()
{
  fM->FilterByPt(fMinMaxPt->GetMin(), fMinMaxPt->GetMax());
}

void IlcEveKinkListEditor::DoMinMaxInvariantMass()
{
    fM->FilterByInvariantMass(fMinMaxInvariantMass->GetMin(), fMinMaxInvariantMass->GetMax(),13);
}

void IlcEveKinkListEditor::DoSelectDaugPid(Int_t rDaugPid)
{
  fM->SetDaugCheckedPid(rDaugPid);
  Update();
}

void IlcEveKinkListEditor::DoCheckDaugPid()
{
  Int_t   lDaugPid  = fM->GetDaugCheckedPid();
  Float_t lDaugProb = fM->GetDaugCheckedProb();
  if (lDaugPid) {
    fM->FilterByCheckedPidMinProb(fDaughterCheckMaxPidProbability->IsOn(),lDaugPid,lDaugProb);
    printf("Selection for daughter pid %d prob %.2f \n",lDaugPid,lDaugProb);
    Update();
  }
}

void IlcEveKinkListEditor::DoSelectDaugProb()
{
  Float_t rMinDaugProb = (Float_t)fDaughterLevelPidProbability->GetNumber();
  fM->SetDaugCheckedProb(rMinDaugProb);
  Update();
}

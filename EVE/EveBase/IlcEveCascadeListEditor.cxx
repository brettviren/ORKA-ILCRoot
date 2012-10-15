// @(#)root/eve:$Id: IlcEveCascadeListEditor.cxx 47102 2011-02-04 11:24:48Z fca $
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



#include "TVirtualPad.h"
#include "TColor.h"

#include "TGLabel.h"
#include "TEveGValuators.h"

// Cleanup these includes:

//#include "TGButton.h"
//#include "TGNumberEntry.h"
//#include "TGColorSelect.h"
#include "TGDoubleSlider.h"
#include <TGComboBox.h>

#include "IlcEveCascadeListEditor.h"
#include "IlcEveCascade.h"



//______________________________________________________________________________
// GUI editor for IlcEveCascadeList.
//

ClassImp(IlcEveCascadeListEditor)

//______________________________________________________________________________
IlcEveCascadeListEditor::IlcEveCascadeListEditor(const TGWindow *p, Int_t width, Int_t height,
             UInt_t options, Pixel_t back) :
  TGedFrame(p, width, height, options | kVerticalFrame, back),
  fM(0),
  fCascadeSpecies(0),
  fMinMaxRCut(0),
  fMinMaxDaughterDCA(0),
  fMinMaxPt(0),
  fMinMaxInvariantMass(0)
{
  // Constructor.

  MakeTitle("IlcEveCascadeList");

  // Create widgets
  // fXYZZ = new TGSomeWidget(this, ...);
  // AddFrame(fXYZZ, new TGLayoutHints(...));
  // fXYZZ->Connect("SignalName()", "Reve::IlcEveCascadeListEditor", this, "DoXYZZ()");

   fMinMaxRCut = new TEveGDoubleValuator(this,"Radius:", 130, 0);
   fMinMaxRCut->SetNELength(5);
   fMinMaxRCut->SetLabelWidth(74);
   fMinMaxRCut->Build();
   fMinMaxRCut->GetSlider()->SetWidth(200);
   fMinMaxRCut->SetLimits(0, 100, TGNumberFormat::kNESRealOne);
   fMinMaxRCut->Connect("ValueSet()", "IlcEveCascadeListEditor", this, "DoMinMaxRCut()");
   AddFrame(fMinMaxRCut, new TGLayoutHints(kLHintsTop, 1, 1, 1, 1));

   fMinMaxDaughterDCA = new TEveGDoubleValuator(this,"DCA:", 130, 0);
   fMinMaxDaughterDCA->SetNELength(5);
   fMinMaxDaughterDCA->SetLabelWidth(74);
   fMinMaxDaughterDCA->Build();
   fMinMaxDaughterDCA->GetSlider()->SetWidth(200);
   fMinMaxDaughterDCA->SetLimits(0, 1, TGNumberFormat::kNESRealTwo);
   fMinMaxDaughterDCA->Connect("ValueSet()", "IlcEveCascadeListEditor", this, "DoMinMaxDaughterDCA()");
   AddFrame(fMinMaxDaughterDCA, new TGLayoutHints(kLHintsTop, 1, 1, 1, 1));

   fMinMaxPt = new TEveGDoubleValuator(this,"pT:", 130, 0);
   fMinMaxPt->SetNELength(5);
   fMinMaxPt->SetLabelWidth(74);
   fMinMaxPt->Build();
   fMinMaxPt->GetSlider()->SetWidth(200);
   fMinMaxPt->SetLimits(0, 20, TGNumberFormat::kNESRealOne);
   fMinMaxPt->Connect("ValueSet()", "IlcEveCascadeListEditor", this, "DoMinMaxPt()");
   AddFrame(fMinMaxPt, new TGLayoutHints(kLHintsTop, 1, 1, 1, 1));
   
   
   TGHorizontalFrame* fCascadeFrame = new TGHorizontalFrame(this);
   TGLabel* labPos = new TGLabel(fCascadeFrame, "Cascade:");
   fCascadeFrame->AddFrame(labPos, new TGLayoutHints(kLHintsLeft|kLHintsBottom, 1, 1, 1, 1));
   fCascadeSpecies = new TGComboBox(fCascadeFrame);
   fCascadeSpecies->AddEntry("Xi",     kXiMinus);
   fCascadeSpecies->AddEntry("Omega",  kOmegaMinus);
   TGListBox* lbPos = fCascadeSpecies->GetListBox();
   lbPos->Resize(lbPos->GetWidth(), 2*18);
   fCascadeSpecies->Resize(45, 20);
   fCascadeSpecies->Connect("Selected(Int_t)", "IlcEveCascadeListEditor", this, "DoSelectInvMassHyp(Int_t)");
   fCascadeFrame->AddFrame(fCascadeSpecies, new TGLayoutHints(kLHintsTop, 3, 1, 1, 1));
   AddFrame(fCascadeFrame);
   
   
   fMinMaxInvariantMass = new TEveGDoubleValuator(this,"Inv. Mass:", 130, 0);
   fMinMaxInvariantMass->SetNELength(5);
   fMinMaxInvariantMass->SetLabelWidth(74);
   fMinMaxInvariantMass->Build();
   fMinMaxInvariantMass->GetSlider()->SetWidth(200);
   fMinMaxInvariantMass->SetLimits(1, 6, TGNumberFormat::kNESRealThree);
   fMinMaxInvariantMass->Connect("ValueSet()", "IlcEveCascadeListEditor", this, "DoMinMaxInvariantMass()");
   AddFrame(fMinMaxInvariantMass, new TGLayoutHints(kLHintsBottom, 1, 1, 1, 1));
   
}

/******************************************************************************/

//______________________________________________________________________________
void IlcEveCascadeListEditor::SetModel(TObject* obj)
{
  // Set model object.

  fM = static_cast<IlcEveCascadeList*>(obj);

  // Set values of widgets
  // fXYZZ->SetValue(fM->GetXYZZ());

  fMinMaxRCut->SetValues(fM->fMinRCut, fM->fMaxRCut);
  fMinMaxDaughterDCA->SetValues(fM->fMinDaughterDCA, fM->fMaxDaughterDCA);
  fMinMaxPt->SetValues(fM->fMinPt, fM->fMaxPt);
  fMinMaxInvariantMass->SetValues(fM->fMinInvariantMass,fM->fMaxInvariantMass);
}

/******************************************************************************/

// Implements callback/slot methods

//______________________________________________________________________________
// void IlcEveCascadeListEditor::DoXYZZ()
// {
//    // Slot for XYZZ.
//
//    fM->SetXYZZ(fXYZZ->GetValue());
//    Update();
// }

void IlcEveCascadeListEditor::DoMinMaxRCut()
{
  // Filter cascade candidates by transverse radius	
	
  fM->FilterByRadius(fMinMaxRCut->GetMin(), fMinMaxRCut->GetMax());
}

void IlcEveCascadeListEditor::DoMinMaxDaughterDCA()
{
  // Filter cascade candidates by DCA to primary vertex	
	
  fM->FilterByDaughterDCA(fMinMaxDaughterDCA->GetMin(), fMinMaxDaughterDCA->GetMax());
}

void IlcEveCascadeListEditor::DoMinMaxPt()
{
   // Filter cascade candidates by transverse momentum
	
  fM->FilterByPt(fMinMaxPt->GetMin(), fMinMaxPt->GetMax());
}



void IlcEveCascadeListEditor::DoSelectInvMassHyp(Int_t rInvMassHyp)
{
   // Apply the invariant mass hypothesis according the choice made by user
	
	fM->SetInvMassHyp(rInvMassHyp);
	Update();
}

void IlcEveCascadeListEditor::DoMinMaxInvariantMass()
{
    // Filter cascade candidates by invariant mass (under mass hypothesis made by user)
	
	Int_t rInvMassHyp = fM->GetInvMassHyp();
	if(rInvMassHyp)
	fM->FilterByInvariantMass(fMinMaxInvariantMass->GetMin(), fMinMaxInvariantMass->GetMax(), rInvMassHyp);
	else 
	fM->FilterByInvariantMass(fMinMaxInvariantMass->GetMin(), fMinMaxInvariantMass->GetMax(), kXiMinus);
	
}


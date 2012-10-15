// @(#)root/eve:$Id: IlcEveHFListEditor.cxx 47102 2011-02-04 11:24:48Z fca $
// Main author: Davide Caffarri 2009

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

#include "IlcEveHFListEditor.h"
#include "IlcEveHF.h"

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
#include "TGSlider.h"

//______________________________________________________________________________
// GUI editor for IlcEveHFList.
//

ClassImp(IlcEveHFListEditor)

//______________________________________________________________________________
IlcEveHFListEditor::IlcEveHFListEditor(const TGWindow *p, Int_t width, Int_t height,
             UInt_t options, Pixel_t back) :
  TGedFrame(p, width, height, options | kVerticalFrame, back),
  fM(0),
  fMinMaxPt(0),
  fMinMaxCosPointingAngle(0),
  fValueInvMass(0)

{
  // Constructor.

  MakeTitle("IlcEveHFList");

  // Create widgets
  // fXYZZ = new TGSomeWidget(this, ...);
  // AddFrame(fXYZZ, new TGLayoutHints(...));
  // fXYZZ->Connect("SignalName()", "Reve::IlcEveHFListEditor", this, "DoXYZZ()");

   fMinMaxPt = new TEveGDoubleValuator(this,"pT:", 80, 0);
   fMinMaxPt->SetNELength(5);
   fMinMaxPt->SetLabelWidth(120);
   fMinMaxPt->Build();
   fMinMaxPt->GetSlider()->SetWidth(200);
   fMinMaxPt->SetLimits(0, 20, TGNumberFormat::kNESRealOne);
   fMinMaxPt->Connect("ValueSet()", "IlcEveHFListEditor", this, "DoMinMaxPt()");
   AddFrame(fMinMaxPt, new TGLayoutHints(kLHintsTop, 1, 1, 1, 1));

   fMinMaxCosPointingAngle = new TEveGDoubleValuator(this,"Cos Pointing Angle:", 80, 0);
   fMinMaxCosPointingAngle->SetNELength(5);
   fMinMaxCosPointingAngle->SetLabelWidth(120);
   fMinMaxCosPointingAngle->Build();
   fMinMaxCosPointingAngle->GetSlider()->SetWidth(200);
   fMinMaxCosPointingAngle->SetLimits(0, 1, TGNumberFormat::kNESRealOne);
   fMinMaxCosPointingAngle->Connect("ValueSet()", "IlcEveHFListEditor", this, "DoMinMaxCosPointingAngle()");
   AddFrame(fMinMaxCosPointingAngle, new TGLayoutHints(kLHintsTop, 1, 1, 1, 1));


   fValueInvMass = new TEveGDoubleValuator(this, "Delta Invariant Mass:", 80, 0);
   fValueInvMass->SetNELength(5);
   fValueInvMass->SetLabelWidth(120);
   fValueInvMass->Build();
   fValueInvMass->GetSlider()->SetWidth(200);
   fValueInvMass->SetLimits(0,1, TGNumberFormat::kNESRealTwo);
   fValueInvMass->Connect("ValueSet()", "IlcEveHFListEditor", this, "DoMinMaxInvMass()");
   AddFrame(fValueInvMass, new TGLayoutHints(kLHintsTop, 1, 1, 1, 1));


}



/******************************************************************************/

//______________________________________________________________________________
void IlcEveHFListEditor::SetModel(TObject* obj)
{
  // Set model object.

  fM = static_cast<IlcEveHFList*>(obj);

  // Set values of widgets
  // fXYZZ->SetValue(fM->GetXYZZ());

  fMinMaxPt->SetValues(fM->fMinPt, fM->fMaxPt);
  fMinMaxCosPointingAngle->SetValues(fM->fMinCosPointingAngle, fM->fMaxCosPointingAngle);
  fValueInvMass->SetValues(0, fM->fDeltaInvariantMass);
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



void IlcEveHFListEditor::DoMinMaxPt()
{
  fM->FilterByPt(fMinMaxPt->GetMin(), fMinMaxPt->GetMax());
}

void IlcEveHFListEditor::DoMinMaxCosPointingAngle()
{
  fM->FilterByCosPointingAngle(fMinMaxCosPointingAngle->GetMin(), fMinMaxCosPointingAngle->GetMax());
}

void IlcEveHFListEditor::DoMinMaxInvMass()
{
  fM->FilterByInvariantMass(0, fValueInvMass->GetMax());
}

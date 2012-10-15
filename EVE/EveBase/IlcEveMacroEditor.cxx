// @(#)root/eve:$Id: IlcEveMacroEditor.cxx 47044 2011-02-03 10:46:29Z fca $
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

#include "IlcEveMacroEditor.h"
#include "IlcEveMacro.h"

#include "TVirtualPad.h"

// Cleanup these includes:
#include "TGLabel.h"
#include "TGNumberEntry.h"
#include "TGColorSelect.h"
#include "TGTextEntry.h"

#include "TGComboBox.h"

//______________________________________________________________________________
// GUI editor for IlcEveMacro.
//

ClassImp(IlcEveMacroEditor)

//______________________________________________________________________________
IlcEveMacroEditor::IlcEveMacroEditor(const TGWindow *p, Int_t width, Int_t height,
				     UInt_t options, Pixel_t back) :
  TGedFrame(p, width, height, options | kVerticalFrame, back),
  fM(0),
  fSources(0),
  fTags(0),
  fMacro(0),
  fFunc(0),
  fArgs(0),
  fActive(0)
{
  // Constructor.

  MakeTitle("IlcEveMacro");

  TGHorizontalFrame *f = 0;
  //TGLabel           *l = 0;
  Int_t labelW = 48;
  {
    f = MkHFrame();
    MkLabel(f, "Active: ", labelW);

    fActive = new TGCheckButton(f);
    f->AddFrame(fActive); // new TGLayoutHints());
    fActive->Connect("Clicked()", "IlcEveMacroEditor", this,
		     "DoActive()");

    MkLabel(f, "Source: ", 56);
    fSources = new TGComboBox(f);
    f->AddFrame(fSources); // new TGLayoutHints());
    fSources->AddEntry("None",      IlcEveMacro::kNone);
    fSources->AddEntry("RunLoader", IlcEveMacro::kRunLoader);
    fSources->AddEntry("ESD",       IlcEveMacro::kESD);
    fSources->AddEntry("ESDfriend", IlcEveMacro::kESDfriend);
    fSources->AddEntry("RawReader", IlcEveMacro::kRawReader);
    {
      TGListBox* lb = fSources->GetListBox();
      lb->Resize(lb->GetWidth(), 5*16);
    }
    fSources->Resize(92, 20);
    fSources->Connect("Selected(Int_t)", "IlcEveMacroEditor", this,
		      "DoSources(Int_t)");

    MkLabel(f, "Tags: ", 40);
    fTags = new TGTextEntry(f);
    f->AddFrame(fTags, new TGLayoutHints(kLHintsNormal|kLHintsExpandX));
    fTags->Connect("TextChanged(const char *)", "IlcEveMacroEditor", this,
		    "DoTags()");
  }
  {
    f = MkHFrame();
    MkLabel(f, "Macro: ", labelW);
    fMacro = new TGTextEntry(f);
    f->AddFrame(fMacro, new TGLayoutHints(kLHintsNormal));//|kLHintsExpandX));
    fMacro->Connect("TextChanged(const char *)", "IlcEveMacroEditor", this,
		    "DoMacro()");

    MkLabel(f, "Func: ", labelW);
    fFunc = new TGTextEntry(f);
    f->AddFrame(fFunc, new TGLayoutHints(kLHintsNormal|kLHintsExpandX));
    fFunc->Connect("TextChanged(const char *)", "IlcEveMacroEditor", this,
		   "DoFunc()");
  }
  {
    f = MkHFrame();
    MkLabel(f, "Args: ", labelW);
    fArgs = new TGTextEntry(f);
    f->AddFrame(fArgs, new TGLayoutHints(kLHintsNormal|kLHintsExpandX));
    fArgs->Connect("TextChanged(const char *)", "IlcEveMacroEditor", this,
		   "DoArgs()");
  }
}

/******************************************************************************/

//______________________________________________________________________________
void IlcEveMacroEditor::SetModel(TObject* obj)
{
  // Set model object.

  fM = static_cast<IlcEveMacro*>(obj);

  fSources->Select  (fM->GetSources(), kFALSE);
  fTags   ->SetText (fM->GetTags(),  kFALSE);
  fMacro  ->SetText (fM->GetMacro(), kFALSE);
  fFunc   ->SetText (fM->GetFunc(),  kFALSE);
  fArgs   ->SetText (fM->GetArgs(),  kFALSE);
  fActive ->SetState(fM->fActive ? kButtonDown : kButtonUp);
}

//______________________________________________________________________________
void IlcEveMacroEditor::DoSources(Int_t v)
{
   // Slot for Sources.

   fM->SetSources(v);
   Update();
}

//______________________________________________________________________________
void IlcEveMacroEditor::DoTags()
{
   // Slot for Tags.

   fM->SetTags(fTags->GetText());
   Update();
}

//______________________________________________________________________________
void IlcEveMacroEditor::DoMacro()
{
   // Slot for Macro.

   fM->SetMacro(fMacro->GetText());
   Update();
}

//______________________________________________________________________________
void IlcEveMacroEditor::DoFunc()
{
   // Slot for Func.

   fM->SetFunc(fFunc->GetText());
   Update();
}
//______________________________________________________________________________
void IlcEveMacroEditor::DoArgs()
{
   // Slot for Args.

   fM->SetArgs(fArgs->GetText());
   Update();
}

//______________________________________________________________________________
void IlcEveMacroEditor::DoActive()
{
   // Slot for Active.

   fM->SetActive(fActive->IsOn());
   Update();
}

/******************************************************************************/

TGHorizontalFrame* IlcEveMacroEditor::MkHFrame(TGCompositeFrame* p)
{
  // Make standard horizontal frame.

  if (p == 0)
    p = this;
  TGHorizontalFrame* f = new TGHorizontalFrame(p);
  p->AddFrame(f, new TGLayoutHints(kLHintsNormal|kLHintsExpandX));
  return f;
}

TGLabel* IlcEveMacroEditor::MkLabel(TGCompositeFrame* p, const char* txt, Int_t width,
				    Int_t lo, Int_t ro, Int_t to, Int_t bo)
{
  // Make standard label.

  TGLabel *l = new TGLabel(p, txt);
  p->AddFrame(l, new TGLayoutHints(kLHintsNormal, lo,ro,to,bo));
  l->SetTextJustify(kTextRight);
  l->SetWidth(width);
  l->ChangeOptions(l->GetOptions() | kFixedWidth);
  return l;
}

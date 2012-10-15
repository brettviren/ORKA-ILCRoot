// @(#)root/eve:$Id: IlcEveMacroExecutorWindow.cxx 58244 2012-08-21 12:57:19Z hristov $
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

#include "IlcEveMacroExecutorWindow.h"
#include "IlcEveMacroExecutor.h"
#include "IlcEveMacro.h"
#include "IlcEveEventManager.h"

#include <TGedEditor.h>
#include <TGLabel.h>
#include <TGListBox.h>
#include <TGTextEntry.h>
#include <Buttons.h>

#include <TPRegexp.h>

class IlcEveMEWListBox : public TGListBox
{
public:
  IlcEveMEWListBox(const TGWindow* p = 0, Int_t id = -1) : TGListBox(p, id)
  {
    if (gfGC == 0)
    {
      const TGGC& old = TGTextLBEntry::GetDefaultGC();

      gfFont = gClient->GetFontPool()->GetFont("-*-lucidatypewriter-*-*-*-*-12-*-*-*-*-*-iso8859-1");
      gfGC   = gClient->GetGCPool()->GetGC((GCValues_t*) old.GetAttributes(), kTRUE);
      gfGC->SetFont(gVirtualX->GetFontHandle(gfFont->GetFontStruct()));
    }
  }

  using TGListBox::AddEntry;
  virtual void AddEntry(const char* s, Int_t id)
  {
    static const Pixel_t gkBackground[] = { 0x00ffffff, 0xf5f7f8 };

    TGTextLBEntry *lbe = new TGTextLBEntry(fLbc, new TGString(s), id,
					   gfGC->GetGC(), gfFont->GetFontStruct());
    fItemVsize = TMath::Max(fItemVsize, lbe->GetDefaultHeight());
    fLbc->AddEntry(lbe, new TGLayoutHints(kLHintsExpandX | kLHintsTop));
    // Need to set it here as the above line sets it to white (for some strange reason).
    lbe->SetBackgroundColor(gkBackground[id%2]);
  }

protected:
  static TGGC    *gfGC;
  static TGFont  *gfFont;

private:
  IlcEveMEWListBox(const IlcEveMEWListBox&);            // Not implemented
  IlcEveMEWListBox& operator=(const IlcEveMEWListBox&); // Not implemented
};

TGGC   *IlcEveMEWListBox::gfGC   = 0;
TGFont *IlcEveMEWListBox::gfFont = 0;


class IlcEveMEWEditor : public TGedEditor
{
public:
  IlcEveMEWEditor(IlcEveMacroExecutorWindow* w) : TGedEditor(0), fMEW(w) {}
  virtual ~IlcEveMEWEditor() {}
  virtual void Update(TGedFrame* gframe=0)
  {
    TGedEditor::Update(gframe);
    fMEW->PopulateMacros();
  }
  virtual void Refresh()
  {
    SetModel(fPad, fModel, kButton1Down);
  }
protected:
  IlcEveMacroExecutorWindow* fMEW;
private:
  IlcEveMEWEditor(const IlcEveMEWEditor&);            // Not implemented
  IlcEveMEWEditor& operator=(const IlcEveMEWEditor&); // Not implemented
};

//______________________________________________________________________________
// Full description of IlcEveMacroExecutorWindow
//

ClassImp(IlcEveMacroExecutorWindow)

//______________________________________________________________________________
IlcEveMacroExecutorWindow::IlcEveMacroExecutorWindow(IlcEveMacroExecutor* master) :
  TGMainFrame(gClient->GetRoot()), fM(master),
  fMainFrame(0), fCtrlFrame(0), fListBox(0), fEditor(0),
  fSelectTags(0),
  fBoxContents()
{
  // Constructor.

  fMainFrame = new TGVerticalFrame(this);
  AddFrame(fMainFrame, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY));

  // TGHorizontalFrame *f = 0;
  TGButton          *b = 0;
  {
    fCtrlFrame = MkHFrame(fMainFrame);
    fCtrlFrame->Resize(400, 22);

    b = new TGTextButton(fCtrlFrame, "Reload event");
    fCtrlFrame->AddFrame(b);
    b->Connect("Clicked()", "IlcEveMacroExecutorWindow", this,
	       "DoReloadEvent()");

    MkLabel(fCtrlFrame, "Select: ", 48);
    fSelectTags = new TGTextEntry(fCtrlFrame);
    fCtrlFrame->AddFrame(fSelectTags, new TGLayoutHints(kLHintsNormal));//|kLHintsExpandX));
    fSelectTags->Connect("ReturnPressed()", "IlcEveMacroExecutorWindow", this,
			 "DoSelectTags()");
    b = new TGTextButton(fCtrlFrame, "Select");
    fCtrlFrame->AddFrame(b);
    b->Connect("Clicked()", "IlcEveMacroExecutorWindow", this,
	       "DoSelectTags()");

    b = new TGTextButton(fCtrlFrame, "Enable all");
    fCtrlFrame->AddFrame(b);
    b->Connect("Clicked()", "IlcEveMacroExecutorWindow", this,
	       "DoEnableAll()");

    b = new TGTextButton(fCtrlFrame, "Disable all");
    fCtrlFrame->AddFrame(b);
    b->Connect("Clicked()", "IlcEveMacroExecutorWindow", this,
	       "DoDisableAll()");
  }

  fListBox = new IlcEveMEWListBox(fMainFrame);
  fMainFrame->AddFrame(fListBox, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY));
  fListBox->Resize(400, 400);
  fListBox->Connect("Selected(Int_t)", "IlcEveMacroExecutorWindow", this,
		    "DoMacroSelected(Int_t)");

  fMainFrame->SetEditDisabled(kEditEnable);
  fMainFrame->SetEditable();
  fEditor = new IlcEveMEWEditor(this);
  fEditor->SetGlobal(kFALSE);
  fMainFrame->SetEditable(kEditDisable);
  fMainFrame->SetEditable(kFALSE);
  {
    TGFrameElement *el = 0;
    TIter next(fMainFrame->GetList());
    while ((el = (TGFrameElement *) next())) {
      if (el->fFrame == fEditor)
	if (el->fLayout) {
	  el->fLayout->SetLayoutHints(kLHintsExpandX);
	  el->fLayout->SetPadLeft(1); el->fLayout->SetPadRight(1);
	  el->fLayout->SetPadTop(2);  el->fLayout->SetPadBottom(2);
	  break;
	}
    }
  }
  fEditor->Resize(400, 150);
  fEditor->ChangeOptions(fEditor->GetOptions() | kFixedHeight);

  Resize(400, 700);

  //SetCleanup(kDeepCleanup);
  Layout();
  MapSubwindows();
  MapWindow();

  IlcEveEventManager::GetMaster()->Connect("NewEventLoaded()", "IlcEveMacroExecutorWindow", this,
			"NewEventLoaded()");
}

IlcEveMacroExecutorWindow::~IlcEveMacroExecutorWindow()
{
  // Destructor.
	fBoxContents.clear();
		
  IlcEveEventManager::GetMaster()->Disconnect("NewEventLoaded()", this);
}

/******************************************************************************/

void IlcEveMacroExecutorWindow::PopulateMacros(Bool_t keep_selected)
{
  // Populate list-box (or whatever will replace it) with all macros.
  // prototype: no selection, sorting

  // printf("IlcEveMacroExecutorWindow::PopulateMacros()\n");

  IlcEveMacro* ex_sel = 0;
  if (keep_selected && fListBox->GetSelected() != -1)
    ex_sel = fBoxContents[fListBox->GetSelected()];

  Int_t sbar_pos = fListBox->GetVScrollbar()->GetPosition();

  fListBox->RemoveAll();
  fBoxContents.clear();

  TPMERegexp *regexp = 0;
  TString     select = fSelectTags->GetText();
  if ( ! select.IsNull())
  {
    regexp = new TPMERegexp(select, "io");
  }

  TIter next(fM->fMacros);
  IlcEveMacro *mac;
  Int_t        id =  0;
  Int_t    sel_id = -1;
  while ((mac = (IlcEveMacro*) next()))
  {
    if (regexp && regexp->Match(mac->GetTags()) == 0)
      continue;
    if (mac == ex_sel)
      sel_id = id;

    fListBox->AddEntry(mac->FormForDisplay(), id++);
    fBoxContents.push_back(mac);
  }

  if (sel_id != -1)
    fListBox->Select(sel_id);

  fListBox->Layout();
  fListBox->GetVScrollbar()->SetPosition(sbar_pos);
  fListBox->MapSubwindows();
}

void IlcEveMacroExecutorWindow::SetActiveStateOfShownMacros(Bool_t active)
{
  // Set active-state of all shown macros to active.
  // Automatically refreshes the list and editor.

  for (std::vector<IlcEveMacro*>::iterator m = fBoxContents.begin(); m != fBoxContents.end(); ++m)
    (*m)->SetActive(active);
  PopulateMacros();
  fEditor->Refresh();
}

/******************************************************************************/

void IlcEveMacroExecutorWindow::NewEventLoaded()
{
  // Slot called after a new event has been loaded

  // !!! Once we have exit status from the macro, can update GUI showing this.
}

void IlcEveMacroExecutorWindow::DoReloadEvent()
{
  // Slot for reload-event.

  IlcEveEventManager::GetMaster()->GotoEvent(IlcEveEventManager::GetMaster()->GetEventId());
}

void IlcEveMacroExecutorWindow::DoSelectTags()
{
  // Slot for select-tags.

  PopulateMacros();
}

void IlcEveMacroExecutorWindow::DoMacroSelected(Int_t mid)
{
  // Slot for macro-selected.

  fEditor->SetModel(0, fBoxContents[mid], kButton1Down);
}

/******************************************************************************/

TGHorizontalFrame* IlcEveMacroExecutorWindow::MkHFrame(TGCompositeFrame* p)
{
  // Make standard horizontal frame.

  if (p == 0)
    p = this;
  TGHorizontalFrame* f = new TGHorizontalFrame(p);
  p->AddFrame(f, new TGLayoutHints(kLHintsNormal|kLHintsExpandX));
  return f;
}

TGLabel* IlcEveMacroExecutorWindow::MkLabel(TGCompositeFrame* p, const char* txt, Int_t width,
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

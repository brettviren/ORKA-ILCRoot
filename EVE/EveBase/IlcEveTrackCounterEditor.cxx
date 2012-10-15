// @(#)root/eve:$Id: IlcEveTrackCounterEditor.cxx 47044 2011-02-03 10:46:29Z fca $
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

#include "IlcEveTrackCounterEditor.h"
#include "IlcEveTrackCounter.h"
#include "IlcEveEventManager.h"
#include "IlcESDEvent.h"

#include "TGedEditor.h"
#include "TVirtualPad.h"
#include "TColor.h"

// Cleanup these includes:
#include "TGLabel.h"
#include "TGNumberEntry.h"
#include "TGComboBox.h"
#include "TGMsgBox.h"

#include "TGButtonGroup.h"

#include "TTree.h"
#include "TH1F.h"

#include "TCanvas.h"
#include "TLatex.h"
#include "TEveManager.h"

#include "TROOT.h"
#include "TSystem.h" // File input/output for track-count status.
#include "TDatime.h"

//______________________________________________________________________________
// GUI editor for IlcEveTrackCounter.
//

ClassImp(IlcEveTrackCounterEditor)

//______________________________________________________________________________
IlcEveTrackCounterEditor::IlcEveTrackCounterEditor(const TGWindow *p, Int_t width, Int_t height,
                                               UInt_t options, Pixel_t back) :
   TGedFrame(p, width, height, options | kVerticalFrame, back),
   fM(0), fAF(0), fDF(0),
   fClickAction(0),
   fInfoLabelTracks   (0),
   fInfoLabelTracklets(0),
   fEventId(0),
   fEventCat(0),
   fScanSummaryFile(0)
{
   // Constructor.

   MakeTitle("IlcEveTrackCounter");

   Int_t labelW = 42;


   // Active frame

   fAF = new TGVerticalFrame(this);

   { // Deactivate button
      TGHorizontalFrame* f = new TGHorizontalFrame(fAF, 210, 20, kFixedWidth);

      TGTextButton* b = new TGTextButton(f, "Deactivate");
      f->AddFrame(b, new TGLayoutHints(kLHintsLeft|kLHintsExpandX, 1, 1, 0, 4));
      b->Connect("Clicked()", "IlcEveTrackCounterEditor", this, "DoDeactivate()");

      fAF->AddFrame(f, new TGLayoutHints(kLHintsLeft, 0, 0, 0, 0));
   }
   { // ClickAction
      TGHorizontalFrame* f = new TGHorizontalFrame(fAF);
      TGLabel* lab = new TGLabel(f, "Click:");
      f->AddFrame(lab, new TGLayoutHints(kLHintsLeft|kLHintsBottom, 1, 10, 1, 2));
      fClickAction = new TGComboBox(f);
      fClickAction->AddEntry("Print", 0);
      fClickAction->AddEntry("Toggle", 1);
      TGListBox* lb = fClickAction->GetListBox();
      lb->Resize(lb->GetWidth(), 2*16);
      fClickAction->Resize(70, 20);
      fClickAction->Connect("Selected(Int_t)", "IlcEveTrackCounterEditor", this,
                            "DoClickAction(Int_t)");
      f->AddFrame(fClickAction, new TGLayoutHints(kLHintsLeft, 1, 2, 1, 1));

      fAF->AddFrame(f);
   }
   { // fInfoLabelTracks
      TGHorizontalFrame* f = new TGHorizontalFrame(fAF);
      TGLabel* lab = new TGLabel(f, "Tracks:");
      f->AddFrame(lab, new TGLayoutHints(kLHintsLeft, 1, 5, 1, 2));

      fInfoLabelTracks = new TGLabel(f);
      f->AddFrame(fInfoLabelTracks, new TGLayoutHints(kLHintsLeft|kLHintsExpandX, 1, 9, 1, 2));

      fAF->AddFrame(f);
   }
   { // fInfoLabelTracklets
      TGHorizontalFrame* f = new TGHorizontalFrame(fAF);
      TGLabel* lab = new TGLabel(f, "Tracklets:");
      f->AddFrame(lab, new TGLayoutHints(kLHintsLeft, 1, 5, 1, 2));

      fInfoLabelTracklets = new TGLabel(f);
      f->AddFrame(fInfoLabelTracklets, new TGLayoutHints(kLHintsLeft|kLHintsExpandX, 1, 9, 1, 2));

      fAF->AddFrame(f);
   }
   {
      TGHorizontalFrame* f = new TGHorizontalFrame(fAF, 210, 20, kFixedWidth);

      TGHorizontalFrame* g = new TGHorizontalFrame(f, labelW, 0, kFixedWidth);
      TGLabel* l = new TGLabel(g, "Event:");
      g->AddFrame(l, new TGLayoutHints(kLHintsLeft, 0,0,4,0));
      f->AddFrame(g);

      TGTextButton* b;

      b = new TGTextButton(f, "Prev");
      f->AddFrame(b, new TGLayoutHints(kLHintsLeft|kLHintsExpandX, 1, 1, 0, 0));
      b->Connect("Clicked()", "IlcEveTrackCounterEditor", this, "DoPrev()");

      fEventId = new TGNumberEntry(f, 0, 3, -1,TGNumberFormat::kNESInteger, TGNumberFormat::kNEANonNegative,
                                   TGNumberFormat::kNELLimitMinMax, 0, 10000);
      f->AddFrame(fEventId, new TGLayoutHints(kLHintsLeft|kLHintsExpandX, 1, 1, 0, 0));
      fEventId->Connect("ValueSet(Long_t)", "IlcEveTrackCounterEditor", this, "DoSetEvent()");

      b = new TGTextButton(f, "Next");
      f->AddFrame(b, new TGLayoutHints(kLHintsLeft|kLHintsExpandX, 1, 1, 0, 0));
      b->Connect("Clicked()", "IlcEveTrackCounterEditor", this, "DoNext()");

      fAF->AddFrame(f);
   }
   {
      TGHorizontalFrame* f = new TGHorizontalFrame(fAF, 210, 20, kFixedWidth);

      TGHorizontalFrame* g = new TGHorizontalFrame(f, labelW, 0, kFixedWidth);
      TGLabel* l = new TGLabel(g, "Report:");
      g->AddFrame(l, new TGLayoutHints(kLHintsLeft, 0,0,4,0));
      f->AddFrame(g);

      TGTextButton* b;

      b = new TGTextButton(f, "Print");
      f->AddFrame(b, new TGLayoutHints(kLHintsLeft|kLHintsExpandX, 1, 1, 0, 0));
      b->Connect("Clicked()", "IlcEveTrackCounterEditor", this, "DoPrintReport()");

      b = new TGTextButton(f, "File");
      f->AddFrame(b, new TGLayoutHints(kLHintsLeft|kLHintsExpandX, 1, 1, 0, 0));
      b->Connect("Clicked()", "IlcEveTrackCounterEditor", this, "DoFileReport()");

      fAF->AddFrame(f, new TGLayoutHints(kLHintsLeft, 0, 0, 4, 0));
   }
   {
      TGHorizontalFrame* f = new TGHorizontalFrame(fAF, 210, 20, kFixedWidth);

      TGHorizontalFrame* g = new TGHorizontalFrame(f, labelW, 0, kFixedWidth);
      TGLabel* l = new TGLabel(g, "Histos:");
      g->AddFrame(l, new TGLayoutHints(kLHintsLeft, 0,0,4,0));
      f->AddFrame(g);

      TGTextButton* b;

      b = new TGTextButton(f, "Show");
      f->AddFrame(b, new TGLayoutHints(kLHintsLeft|kLHintsExpandX, 1, 1, 0, 0));
      b->Connect("Clicked()", "IlcEveTrackCounterEditor", this, "DoShowHistos()");

      fAF->AddFrame(f, new TGLayoutHints(kLHintsLeft, 0, 0, 0, 0));
   }
   {
      TGHorizontalFrame* f = new TGHorizontalFrame(fAF, 210, 20, kFixedWidth);

      TGButtonGroup *fTypeSelector = new TGButtonGroup(f, "Event Categorization");
      new TGRadioButton(fTypeSelector, "Good");
      new TGRadioButton(fTypeSelector, "Splash");
      new TGRadioButton(fTypeSelector, "Empty");
      new TGRadioButton(fTypeSelector, "Background");
      new TGRadioButton(fTypeSelector, "Unclear/Other");
      fTypeSelector->Connect("Clicked(Int_t)", "IlcEveTrackCounterEditor", this, "DoEventCategorization(Int_t)");

      f->AddFrame(fTypeSelector, new TGLayoutHints(kLHintsTop | kLHintsCenterX | kLHintsExpandX, 2, 3, 2, 2));
   
      fAF->AddFrame(f, new TGLayoutHints(kLHintsLeft, 0, 0, 0, 0));
   }

   AddFrame(fAF, new TGLayoutHints(kLHintsNormal|kLHintsExpandX|kLHintsExpandY));


   // Disabled frame

   fDF = new TGVerticalFrame(this);

   { // Activate button
      TGHorizontalFrame* f = new TGHorizontalFrame(fDF, 210, 20, kFixedWidth);

      TGTextButton* b = new TGTextButton(f, "Activate");
      f->AddFrame(b, new TGLayoutHints(kLHintsLeft|kLHintsExpandX, 1, 1, 0, 0));
      b->Connect("Clicked()", "IlcEveTrackCounterEditor", this, "DoActivate()");

      fDF->AddFrame(f, new TGLayoutHints(kLHintsLeft, 0, 0, 0, 0));
   }

   AddFrame(fDF, new TGLayoutHints(kLHintsNormal|kLHintsExpandX|kLHintsExpandY));

   IlcEveEventManager::GetMaster()->Connect("NewEventLoaded()", "IlcEveTrackCounterEditor", this, "UpdateModel()");
}

IlcEveTrackCounterEditor::~IlcEveTrackCounterEditor()
{
  // Destructor.

  IlcEveEventManager::GetMaster()->Disconnect("NewEventLoaded()", this);

  if (fScanSummaryFile) {
    fScanSummaryFile->close();
    delete fScanSummaryFile;
    fScanSummaryFile = 0;
  }
}

/******************************************************************************/

void IlcEveTrackCounterEditor::UpdateModel()
{
  TEveException e;
  if (fGedEditor && fM && fGedEditor->GetModel() == fM->GetEditorObject(e))
  {
    SetModel(fM->GetEditorObject(e));
  }
}

//______________________________________________________________________________
void IlcEveTrackCounterEditor::SetModel(TObject* obj)
{
   // Set model object.

   fM = static_cast<IlcEveTrackCounter*>(obj);

   if (fM->GetActive())
   {
      ShowFrame(fAF); HideFrame(fDF);
 
      fClickAction->Select(fM->fClickAction, kFALSE);
      fInfoLabelTracks   ->SetText(Form("All: %3d; Primaries: %3d", fM->fAllTracks,    fM->fGoodTracks));
      fInfoLabelTracklets->SetText(Form("All: %3d; Primaries: %3d", fM->fAllTracklets, fM->fGoodTracklets));
      fEventId->SetNumber(fM->GetEventId());
   }
   else
   {
     ShowFrame(fDF); HideFrame(fAF);
   }

   Layout();
}

/******************************************************************************/

void IlcEveTrackCounterEditor::DoActivate()
{
   // Activate track-counter

   fM->SetActive(kTRUE);
   IlcEveEventManager::GetMaster()->GotoEvent(IlcEveEventManager::GetMaster()->GetEventId());
   fGedEditor->Layout();

   if (fScanSummaryFile) {
     fScanSummaryFile->close();
     delete fScanSummaryFile;
   }

   char fname[200];
   TDatime dat;
   snprintf(fname, 200,"ScanSummary.%i.%i.txt", dat.GetDate(), dat.GetTime());
   fScanSummaryFile = new ofstream(fname);
   (*fScanSummaryFile) << "Scan summary" << std::endl;
   (*fScanSummaryFile) << "Scan started at " << dat.GetDate() << " " << dat.GetTime() << std::endl;
   IlcESDEvent *esd = IlcEveEventManager::AssertESD();
   (*fScanSummaryFile) << "Run number " << esd->GetRunNumber() << std::endl;
}

void IlcEveTrackCounterEditor::DoDeactivate()
{
   // Deactivate track-counter.

   fM->SetActive(kFALSE);
   IlcEveEventManager::GetMaster()->GotoEvent(IlcEveEventManager::GetMaster()->GetEventId());

   if (fScanSummaryFile) {
     fScanSummaryFile->close();
     delete fScanSummaryFile;
     fScanSummaryFile = 0;
   }
}

/******************************************************************************/

//______________________________________________________________________________
void IlcEveTrackCounterEditor::DoPrev()
{
   // Slot for Prev.

   IlcEveEventManager::GetMaster()->PrevEvent();
}

//______________________________________________________________________________
void IlcEveTrackCounterEditor::DoNext()
{
   // Slot for Next.

   if (fScanSummaryFile) {
     IlcESDEvent *esd = IlcEveEventManager::AssertESD();
     (*fScanSummaryFile) << std::hex << std::right ;
     fScanSummaryFile->width(5); (*fScanSummaryFile) << esd->GetPeriodNumber() << "   " ;
     fScanSummaryFile->width(6); (*fScanSummaryFile) << esd->GetOrbitNumber() << "   ";
     fScanSummaryFile->width(4); (*fScanSummaryFile) << esd->GetBunchCrossNumber() << "   ";
     switch (fEventCat) {
     case 1: (*fScanSummaryFile) << "GOOD        "; break;
     case 2: (*fScanSummaryFile) << "SPLASH      "; break;
     case 3: (*fScanSummaryFile) << "EMPTY       "; break;
     case 4: (*fScanSummaryFile) << "BACKGROUND  "; break;
     case 5: (*fScanSummaryFile) << "OTHER       "; break;
     default: break;
     }
     if (fM->GetActive())
       {
	 (*fScanSummaryFile) << std::dec;
	 fScanSummaryFile->width(5); (*fScanSummaryFile) << fM->fAllTracks << "  ";
	 fScanSummaryFile->width(5); (*fScanSummaryFile) << fM->fGoodTracks << "     ";
	 fScanSummaryFile->width(5); (*fScanSummaryFile) << fM->fAllTracklets << "  ";
	 fScanSummaryFile->width(5); (*fScanSummaryFile) << fM->fGoodTracklets << "   ";
       }
     if ((esd->GetPrimaryVertex()) && (esd->GetPrimaryVertex()->GetStatus()))
       { fScanSummaryFile->width(9); (*fScanSummaryFile) << esd->GetPrimaryVertex()->GetZ(); }
     else { (*fScanSummaryFile) << "NOVTX    "; }

     (*fScanSummaryFile) << std::endl;
   }

   IlcEveEventManager::GetMaster()->NextEvent();
}

//______________________________________________________________________________
void IlcEveTrackCounterEditor::DoSetEvent()
{
   // Slot for SetEvent.

   IlcEveEventManager::GetMaster()->GotoEvent((Int_t) fEventId->GetNumber());
}

/******************************************************************************/

//______________________________________________________________________________
void IlcEveTrackCounterEditor::DoPrintReport()
{
   // Slot for PrintReport.

   fM->PrintEventTracks();
}

//______________________________________________________________________________
void IlcEveTrackCounterEditor::DoFileReport()
{
   // Slot for FileReport.

   fM->OutputEventTracks();
}

//______________________________________________________________________________
void IlcEveTrackCounterEditor::DoShowHistos()
{
  // Slot for ShowHistos.

  TEveUtil::Macro("make_scan_results.C");
  TEveUtil::Macro("show_scan_results.C");
}

/******************************************************************************/

//______________________________________________________________________________
void IlcEveTrackCounterEditor::DoClickAction(Int_t mode)
{
   // Slot for ClickAction.

   fM->SetClickAction(mode);
}

//______________________________________________________________________________
void IlcEveTrackCounterEditor::DoEventCategorization(Int_t mode)
{
   // Slot for ClickAction.

  printf("Mode is %i\n", mode);
  fEventCat = mode;
}

// $Id: IlcEveJetPlaneEditor.h 31373 2009-03-09 11:00:26Z mtadel $
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

#ifndef IlcEveJetPlaneEditor_H
#define IlcEveJetPlaneEditor_H

#include <TGedFrame.h>

class TGButton;
class TGCheckButton;
class TGNumberEntry;
class TGColorSelect;
class TEveGValuator;

class IlcEveJetPlane;

class IlcEveJetPlaneEditor : public TGedFrame
{
public:
  IlcEveJetPlaneEditor(const TGWindow* p=0, Int_t width=170, Int_t height=30,
                       UInt_t options=kChildFrame, Pixel_t back=GetDefaultFrameBackground());
  virtual ~IlcEveJetPlaneEditor() {}

  virtual void SetModel(TObject* obj);

  // Declare callback/slot methods
  // void DoXYZZ();
  void DoRnrJets();
  void DoRnrTracks();
  void DoArrowJetScale();
  void DoArrowTrackScale();
	void DoEnergyScale();
  void DoOneSelection();
  void DoTwoSelection();
  void DoStaticDataWindow();


  // --- Internal class for common settings

  class StaticDataWindow : public TGTransientFrame
  {
  public:
    StaticDataWindow(const TGWindow *p, const TGWindow *main, UInt_t w, UInt_t h,
		     UInt_t options = kVerticalFrame);
    virtual ~StaticDataWindow();

    // slots
    void DoClose();
    void DoOK();
    void DoCancel();
    void DoTab(Int_t id);

  private:
    StaticDataWindow(const StaticDataWindow&);            // Not implemented
    StaticDataWindow& operator=(const StaticDataWindow&); // Not implemented

    TGCompositeFrame    *fFrame1, *fF2;             // Frames.
    TGButton            *fOkButton, *fCancelButton; // Ok, cancel buttons.
    TGLayoutHints       *fL1, *fL2, *fL3, *fL5;     // Layout hints.
    TGTab               *fTab;                      // Tab container.
    TGButton            *fChk1, *fChk2,*fChk3, *fChk4,*fChk5; // Check-buttons.

    ClassDef(StaticDataWindow, 0); // Common settings for all IlcEveJetPlane objects.
  };

protected:
  IlcEveJetPlane   *fM; // Model object.

  TGCheckButton    *fRnrJets;          // Widget for flag RnrJets.
  TGCheckButton    *fRnrTracks;        // Widget for flag RnrTracks.
  TEveGValuator    *fEnergyScale;      // Widget for EnergyScale.
  TEveGValuator    *fArrowJetScale;    // Widget for ArrowJetScale.
  TEveGValuator    *fArrowTrackScale;  // Widget for ArrowTrackScale.
  TGButton         *fOneSelection, *fTwoSelection;  // Widgets for one/two selection flags.
  TGButton         *fInformationSetup; // Widget for InformationSetup.

  static StaticDataWindow* fgStaticWindow; // Common window for global settings.

private:
  IlcEveJetPlaneEditor(const IlcEveJetPlaneEditor&);            // Not implemented
  IlcEveJetPlaneEditor& operator=(const IlcEveJetPlaneEditor&); // Not implemented

  ClassDef(IlcEveJetPlaneEditor, 0); // Editor for IlcEveJetPlane.
};

#endif

// $Id: IlcEveLegoEditor.h 43215 2010-08-30 12:48:52Z mtadel $
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

#ifndef ILCEVELEGOEDITOR_H
#define ILCEVELEGOEDITOR_H

#include "TGedFrame.h"

class TGButton;
class TGButtonGroup;
class TGCheckButton;
class TGColorSelect;
class TGComboBox;
class TGGroupFrame;
class TGLabel;
class TGNumberEntry;
class TGRadioButton;

class IlcEveLego;

//______________________________________________________________________________
// IlcEveLegoEditor is the class editor of IlcEveLego
//

class IlcEveLegoEditor : public TGedFrame
{
public:
   IlcEveLegoEditor(const TGWindow* p=0, Int_t width=170, Int_t height=30,
         UInt_t options=kChildFrame, Pixel_t back=GetDefaultFrameBackground());
   virtual ~IlcEveLegoEditor() {}

   virtual void SetModel(TObject* obj);

   // Slot methods
   void ApplyChanges();
   void CollisionCandidatesOnly();
   void CreateAllEventsEditor();
   void DataIsMC();
   void DoAllEvents();
   void ShowPosCharge();
   void ShowNegCharge();
   void ShowElectrons();
   void ShowMuons();
   void ShowPions();
   void ShowKaons();
   void ShowProtons();
   void ShowPosChargeAE();
   void ShowNegChargeAE();
   void ShowElectronsAE();
   void ShowMuonsAE();
   void ShowPionsAE();
   void ShowKaonsAE();
   void ShowProtonsAE();
   void SetThreshold();
   void SetThresholdAE();
   void SetMaxPt();
   void SetMaxPtAE();
   void ShowByTracks(Int_t id);
   void ShowByTracksAE(Int_t id);

protected:
   IlcEveLego    *fM;                // Model object.

private:
   // Single event GUI
   TGTextButton  *fAllEventsButton;  // text button for all events
   TGGroupFrame  *fParticlesBG;      // particle selection button
   TGButtonGroup *fTrackSelection;   // track selection button
   TGCheckButton *fPosCharged;       // check button for positive only charged particles
   TGCheckButton *fNegCharged;       // check button for negative only charged particles
   TGCheckButton *fElectrons;        // check button for electrons
   TGCheckButton *fMuons;            // check button for muons
   TGCheckButton *fPions;            // check button for pions
   TGCheckButton *fKaons;            // check button for kaons
   TGCheckButton *fProtons;          // check button for protons
   TGRadioButton *fRtracks[2];       // radio button for track selection
   TGLabel       *fLabel;            // label for track selection
   TGLabel       *fLabel1;           // label for event selection
   TGNumberEntry *fThreshold;        // number entry to setup threshold
   TGNumberEntry *fMaxPt;            // number entry to setup max pt
   TGComboBox    *fSelect;           // combo box to filter events

   // All events GUI
   TGButtonGroup *fParticlesBGAE;    // particle selection button for all events
   TGButtonGroup *fTrackSelectionAE; // track selection for all events
   TGCheckButton *fPosChargedAE;     // check button for positive only charged particles
   TGCheckButton *fNegChargedAE;     // check button for negative only charged particles
   TGCheckButton *fElectronsAE;      // check button for electrons
   TGCheckButton *fMuonsAE;          // check button for muons
   TGCheckButton *fPionsAE;          // check button for pions
   TGCheckButton *fKaonsAE;          // check button for kaons
   TGCheckButton *fProtonsAE;        // check button for protons
   TGTextButton  *fApplyChanges;     // apply selections
   TGRadioButton *fRtracksAE[2];     // radio button track
   TGLabel       *fLabelAE;          // label for track selection
   TGLabel       *fLabel1AE;         // label for event selection
   TGNumberEntry *fThresholdAE;      // number entry to setup threshold
   TGNumberEntry *fMaxPtAE;          // number entry to setup max pt
   TGButtonGroup *fEventControl;     // event control panel
   TGCheckButton *fIsMC;             // check if data is from MC
   TGCheckButton *fCollisionCandidatesOnly; // fill all collision candidates events

   IlcEveLegoEditor(const IlcEveLegoEditor&);            // Not implemented
   IlcEveLegoEditor& operator=(const IlcEveLegoEditor&); // Not implemented

   ClassDef(IlcEveLegoEditor, 0); // GUI editor for IlcEveLego.
};

#endif

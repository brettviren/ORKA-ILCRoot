// $Id: IlcEveBeamsInfo.h 43215 2010-08-30 12:48:52Z mtadel $
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

#ifndef ILCEVEBEAMSINFO_H
#define ILCEVEBEAMSINFO_H

#include "TEveElement.h"

class IlcESDEvent;
class IlcPhysicsSelection;
class IlcEveEventSelector;
class IlcEveMultiView;

class TGLOverlayButton;
class TEveViewer;

//______________________________________________________________________________
// Display beams and triggers information on IlcEve viewers, from ESD.
//

class IlcEveBeamsInfo : public TEveElementList
{
public:
  IlcEveBeamsInfo(const char* name="IlcEveBeamsInfo");
  virtual ~IlcEveBeamsInfo();
  void ShowEventSelection(Bool_t status);

  // Set Methods
  void SetAlpha(Double_t val);

  // Get Methods
  TString * SepareTriggerClasses(Int_t &fNumberOfClasses, TString fTriggerSource);

  // Functions
  void AddOverlayButton(TGLOverlayButton *button);
  void AddTriggerClasses();
  void CreateEventPanel();
  void CreateRunPanel();
  void RemoveOverlayButton(TGLOverlayButton *button);
  void RemoveTriggerClasses();
  void SelectEventSelection(Int_t id);
  void ShowBeamsInfo(Bool_t show, Bool_t updateonly = kFALSE);
  void ShowPrevEvent();
  void ShowNextEvent();
  void SwitchDataType(Bool_t status);
  void Update();
  void UpdateTriggerClasses();  

private:
  Double_t            fAlpha;                // Alpha value
  Bool_t              fIsMC;                 // Check data type
  IlcESDEvent         *fEsd;                 // Esd event
  Bool_t              fShowEventsInfo;       // Determine if show events info
  IlcPhysicsSelection *fPhysicsSelection;    // Physics selection object

  TGLOverlayButton    *fEventNumber;         // Event number
  TGLOverlayButton    *fCollisionCandidate;  // IlcPhysicsSelection button output
  TGLOverlayButton    *fCollisionBoolean;    // Collision boolean

  TGLOverlayButton    *fBeam1;               // Beam 1 information
  TGLOverlayButton    *fBeam1Boolean;        // Beam 1 boolean
  TGLOverlayButton    *fBeam2;               // Beam 2 information
  TGLOverlayButton    *fBeam2Boolean;        // Beam 2 boolean

  TGLOverlayButton    *fRunNumber;           // Show data run number
  TGLOverlayButton    *fEventType;           // Show event type
  TGLOverlayButton    *fEventTypeLabel;      // Show event type label
  TGLOverlayButton    *fPeriod;              // Show event period
  TGLOverlayButton    *fOrbit;               // Show orbit
  TGLOverlayButton    *fBC;                  // Show bc

  TGLOverlayButton    *fTimeStamp;           // Time stamp information
  TGLOverlayButton    *fMagnetField;         // Magnetic field
  TGLOverlayButton    *fTrigger;             // Trigger

  TGLOverlayButton    *fTriggerClassesPanel; // Active trigger classes panel
  Int_t               fNumberOfActiveTriggerClasses; // Number of active trigger classes
  TGLOverlayButton    **fTriggerClasses;      // Active trigger classes

  IlcEveMultiView     *fAl;                   // Multiview instance
  TEveViewer          *fHisto2dv;             // 2D lego view
  IlcEveEventSelector *fEventSelector;        // Current event selector

  IlcEveBeamsInfo(const IlcEveBeamsInfo&);            // Not implemented
  IlcEveBeamsInfo& operator=(const IlcEveBeamsInfo&); // Not implemented

  ClassDef(IlcEveBeamsInfo, 0); // Short description.
};

#endif

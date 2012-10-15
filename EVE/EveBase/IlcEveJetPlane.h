// $Id: IlcEveJetPlane.h 31373 2009-03-09 11:00:26Z mtadel $
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

#ifndef IlcEveJetPlane_H
#define IlcEveJetPlane_H

#include <TEveElement.h>

#include <TAtt3D.h>
#include <TAttBBox.h>

#include <IlcAODJet.h>
#include <IlcAODTrack.h>

#include <vector>

//==============================================================================
//
// IlcEveJetPlane
//
// Class for display of jets and tracks in eta-phi plane.

class IlcEveJetPlane : public TEveElementList,
		       public TAtt3D,
		       public TAttBBox
{
  friend class IlcEveJetPlaneGL;

public:
  IlcEveJetPlane(Int_t iev);
  virtual ~IlcEveJetPlane();

  void AddJet(IlcAODJet* jet);
  void AddTrack(IlcAODTrack* track);

  void CreateArrows();

  Int_t GetNEtaDiv() const  { return fNEtaDiv; }
  void  SetNEtaDiv(Int_t r) { fNEtaDiv = r; }

  Int_t GetNPhiDiv() const  { return fNPhiDiv; }
  void  SetNPhiDiv(Int_t r) { fNPhiDiv = r; }

  Bool_t GetRnrJets() const   { return fRnrJets; }
  void   SetRnrJets(Bool_t r) { fRnrJets = r; CreateArrows(); }

  Bool_t GetRnrTracks() const   { return fRnrTracks; }
  void   SetRnrTracks(Bool_t r) { fRnrTracks = r; CreateArrows(); }

  Bool_t GetOneSelection() const   { return fOneSelection; }
  void   SetOneSelection(Bool_t r) { fOneSelection = r; }

  Bool_t GetTwoSelection() const   { return fTwoSelection; }
  void   SetTwoSelection(Bool_t r) { fTwoSelection = r; }

  Float_t GetEnergyScale() const { return fEnergyScale; }
  void    SetEnergyScale(Float_t s) { fEnergyScale = s; CreateArrows(); }

  Float_t GetArrowJetScale() const { return fArrowJetScale; }
  void    SetArrowJetScale(Float_t s) { fArrowJetScale = s; CreateArrows(); }
	
  Float_t GetArrowTrackScale() const { return fArrowTrackScale; }
  void    SetArrowTrackScale(Float_t s) { fArrowTrackScale = s; CreateArrows(); }

  const IlcAODJet& GetJet1() const { return *fJet1; }
  const IlcAODJet& GetJet2() const { return *fJet2; }
  const IlcAODTrack& GetTrack1() const { return *fTrack1; }
  const IlcAODTrack& GetTrack2() const { return *fTrack2; }

  void    SetJet1(IlcAODJet* s) { fJet1 = s; }
  void    SetJet2(IlcAODJet* s) { fJet2 = s; }
  void    SetTrack1(IlcAODTrack* s) { fTrack1 = s; }
  void    SetTrack2(IlcAODTrack* s) { fTrack2 = s; }

  void    SetSelectionFlag(Int_t s) { fSelectionFlag = s;}
  void    SelectionAdded(TEveElement* el);

  virtual Bool_t  CanEditMainColor()const { return kTRUE; }

  virtual void ComputeBBox();
  virtual void Paint(Option_t* option = "");

protected:
  Float_t fMinEta;    // Min eta for display.
  Float_t fMaxEta;    // Max eta for display.
  Float_t fMinPhi;    // Min phi for display.
  Float_t fMaxPhi;    // Max phi for display.
  Int_t   fNEtaDiv;   // Number of eta divisions for display.
  Int_t   fNPhiDiv;   // Number of phi divisions for display.

  Float_t fEtaScale;          // Multiplier for eta.
  Float_t fPhiScale;          // Multiplier for phi.
  Float_t fEnergyScale;       // Multiplier for energy.
  Float_t fArrowJetScale;     // Multiplier for jet arrow dim.
  Float_t fArrowTrackScale;   // Multiplier for track arrow dim.
	
  Color_t fGridColor; // Color of coordinate grid.

  std::vector<IlcAODJet>   fJets;   // Jets to display.
  std::vector<IlcAODTrack> fTracks; // Tracks to display.

  Bool_t                 fRnrJets;       // Show jets.
  Bool_t                 fRnrTracks;     // Show tracks.
  Bool_t                 fOneSelection;  // One object selected.
  Bool_t                 fTwoSelection;  // Two objects selected.
  Bool_t                 fSelConnected;  // Connected to EVE selection.

  IlcAODJet             *fJet1,   *fJet2;    // Selection jets.
  IlcAODTrack           *fTrack1, *fTrack2;  // Selection tracks.

  Int_t                  fSelectionFlag; // Selection state, handled by GL renderer.

  // Common settings:
  static Bool_t fgOneMomentumXYZ;       // Display momentum as coordinates.
  static Bool_t fgOneMomentumPhiTheta;  // Display momentum as phi/theta.
  static Bool_t fgOneEta;               // Display eta.
  static Bool_t fgOneE;                 // Display energy.
  static Bool_t fgOneChgMass;           // Display charge and mass.

private:
  IlcEveJetPlane(const IlcEveJetPlane&);            // Not implemented
  IlcEveJetPlane& operator=(const IlcEveJetPlane&); // Not implemented

  ClassDef(IlcEveJetPlane, 0); // Show jets and tracks in eta-phi plane.
};

#endif

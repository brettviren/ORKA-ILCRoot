// $Id: IlcEveV0.h 43420 2010-09-08 09:02:32Z pdebski $
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

#ifndef IlcEveV0_H
#define IlcEveV0_H


//------------------------------------------------------------------------------
// IlcEveV0
//------------------------------------------------------------------------------
//
// Representation of a reconstructed V0.
//
//
//------------------------------------------------------------------------------
// IlcEveV0List
//------------------------------------------------------------------------------
//
// Container for IlcEveV0s.
//
// Allows runtime selection by pT and DCA of daughters, radius of V0
// creation and PID priobabilities

//==============================================================================

#include "IlcEveTrack.h"
#include <TEveVSDStructs.h>

#include <TPolyLine3D.h>

#include <TPDGCode.h>

class TH1F;
class TH2F;


class IlcEveV0List;

class IlcEveV0 : public TEvePointSet
{
  friend class IlcEveV0List;
  friend class IlcEveV0Editor;

public:
  IlcEveV0();
  IlcEveV0(TEveRecTrack* tNeg, TEveRecTrack* tPos, TEveRecV0* v0,
     TEveTrackPropagator* rs);
  virtual ~IlcEveV0();

  void MakeV0();

  virtual void  SetMainColor(Color_t col)
  {
    TEvePointSet::SetMainColor(col);
    fPointingLine->SetLineColor(fMarkerColor);
  }

  void SetRnrStyle(TEveTrackPropagator* rs) { fRnrStyle = rs; }

  Float_t GetDaughterDCA() const { return fDaughterDCA; }
  void SetDaughterDCA(Float_t dca) { fDaughterDCA = dca; }

  Float_t GetPhi()    const { return fRecDecayP.Phi(); }
  Float_t GetEta()    const { return fRecDecayP.Eta(); }
  Float_t GetRadius() const { return fRecDecayV.Perp(); }
  Float_t GetPt()     const { return fRecDecayP.Perp(); }
  Float_t GetMomentum() const { return fRecDecayP.Mag(); }

  Float_t GetInvMass(Int_t nPdgCode, Int_t pPdgCode) const;
  Float_t GetK0sInvMass() const { return GetInvMass(kPiMinus,kPiPlus); }
  Float_t GetLambdaInvMass() const { return GetInvMass(kPiMinus,kProton); }
  Float_t GetAntiLambdaInvMass() const { return GetInvMass(kProton,kPiPlus); }

  Bool_t GetOnFlyStatus()    const { return fOnFlyStatus; }
  void   SetOnFlyStatus(Bool_t fs) { fOnFlyStatus = fs; }

  void    SetMaxProbPdgPid(Int_t iDaughter, Int_t rPdg, Float_t rPid);
  Int_t   GetNegMaxProbPdg() const { return fNegMaxProbPdg; }
  Int_t   GetPosMaxProbPdg() const { return fPosMaxProbPdg; }
  Float_t GetNegMaxProbPid() const { return fNegMaxProbPid; }
  Float_t GetPosMaxProbPid() const { return fPosMaxProbPid; }

  Int_t GetESDIndex() const { return fESDIndex; }
  void  SetESDIndex(Int_t ind) { fESDIndex = ind;}

  virtual const Text_t* GetName()  const { return Form("ESDv0_%i",fESDIndex); }
  virtual const Text_t* GetTitle() const { return Form("ESDv0_%i",fESDIndex); }

  TEveTrackPropagator* GetPropagator() const  { return fRnrStyle; }

  IlcEveTrack* GetNegTrack() const { return fNegTrack; }
  IlcEveTrack* GetPosTrack() const { return fPosTrack; }

  TEveLine*  GetPointingLine() const { return fPointingLine; }

protected:
  TEveVector fRecBirthV;    // Reconstucted birth point of neutral particle
  TEveVector fRecDecayV;    // Point of closest approach
  TEveVector fRecDecayP;    // Reconstructed momentum of decayed particle.

  IlcEveTrack        *fNegTrack; // Representation of negative track.
  IlcEveTrack        *fPosTrack; // Representation of positive track.

  TEveTrackPropagator *fRnrStyle; // Track propagator for neg/pos track.

  TEveLine         *fPointingLine; // Representation of pointing line.

  Int_t             fESDIndex;    // Index in ESD V0 array.
  Bool_t            fOnFlyStatus; // Reconstructed during tracking.
  Float_t           fDaughterDCA; // Distance at the point of closest approach. 
  Float_t           fChi2V0;      // Some Chi-square.

  Int_t             fNegMaxProbPdg; // Maximum PDG probability for the negative daughter
  Int_t             fPosMaxProbPdg; // Maximum PDG probability for the positive daughter
  Float_t           fNegMaxProbPid; // Maximum PID probability for the negative daughter
  Float_t           fPosMaxProbPid; // Maximum PID probability for the positive daughter

private:
  IlcEveV0(const IlcEveV0&);            // Not implemented
  IlcEveV0& operator=(const IlcEveV0&); // Not implemented

  ClassDef(IlcEveV0, 0); // Visual representation of a IlcEveV0.
};


/******************************************************************************/
// IlcEveV0List
/******************************************************************************/

class IlcEveV0List : public TEveElementList
{
  friend class IlcEveV0ListEditor;

public:
  IlcEveV0List();
  IlcEveV0List(TEveTrackPropagator* rs);
  IlcEveV0List(const Text_t* name, TEveTrackPropagator* rs=0);
  virtual ~IlcEveV0List() {}

  virtual const Text_t* GetTitle() const { return fTitle; }
  virtual void SetTitle(const Text_t* t) { fTitle = t; }
  virtual void SetTracksColor(Color_t cNeg, Color_t cPos) {
    fNegColor = cNeg; fPosColor = cPos;}

  virtual Bool_t CanEditMainColor() const { return kTRUE; }

  void  SetRnrStyle(TEveTrackPropagator* rst) { fRnrStyle = rst; }
  TEveTrackPropagator* GetPropagator()        { return fRnrStyle; }

  Bool_t GetRnrV0vtx()     const { return fRnrV0vtx; }
  Bool_t GetRnrV0path()    const { return fRnrV0path; }
  Bool_t GetRnrDaughters() const { return fRnrDaughters; }

  void   MakeV0s();

  void   FilterByRadius(Float_t minR, Float_t maxR);
  void   FilterByDaughterDCA(Float_t minDaughterDCA, Float_t maxDaughterDCA);
  void   FilterByPt(Float_t minPt, Float_t maxPt);
  void   FilterByCheckedPidMinProb(Int_t rFlag, Int_t rDaughter, Int_t rPid, Float_t rProb);
  void   SetNegCheckedPid(Int_t rNegCheckedPid) {fNegCheckedPid = rNegCheckedPid;}
  void   SetPosCheckedPid(Int_t rPosCheckedPid) {fPosCheckedPid = rPosCheckedPid;}
  Int_t  GetNegCheckedPid() const { return fNegCheckedPid; }
  Int_t  GetPosCheckedPid() const { return fPosCheckedPid; }

  void   SetNegCheckedProb(Float_t rNegCheckedProb) {fNegCheckedProb = rNegCheckedProb;}
  void   SetPosCheckedProb(Float_t rPosCheckedProb) {fPosCheckedProb = rPosCheckedProb;}
  Float_t  GetNegCheckedProb() const { return fNegCheckedProb; }
  Float_t  GetPosCheckedProb() const { return fPosCheckedProb; }

  void   FilterByInvariantMass(Float_t minPt, Float_t maxPt, Int_t nPdgCode, Int_t pPdgCode);

protected:
  TString              fTitle;    // Title of the object.

  TEveTrackPropagator *fRnrStyle; // Track propagator to be passed do conteined V0s.

  Bool_t               fRnrDaughters; // Flag - display daughter tracks.
  Bool_t               fRnrV0vtx;     // Flag - display V0 vertex.
  Bool_t               fRnrV0path;    // Flag - display V0 path.

  Color_t              fNegColor;     // Color for negative tracks.
  Color_t              fPosColor;     // Color for positive tracks.

  Float_t              fMinRCut;      // Minimum radius cut.
  Float_t              fMaxRCut;      // Maximum radius cut.

  Float_t              fMinDaughterDCA; // Minimum daughter DCA cut.
  Float_t              fMaxDaughterDCA; // Maximum daughter DCA cut.

  Float_t              fMinPt;          // Minimum Pt cut.
  Float_t              fMaxPt;          // Maximum Pt cut.

  Int_t                fNegCheckedPid;  // BORIS ?
  Int_t                fPosCheckedPid;  // BORIS ?

  Float_t              fNegCheckedProb; // BORIS ?
  Float_t              fPosCheckedProb; // BORIS ?

  Float_t              fMinInvariantMass; // Minimum invariant mass cut.
  Float_t              fMaxInvariantMass; // Maximum invariant mass cut.

private:
  void Init();

  IlcEveV0List(const IlcEveV0List&);            // Not implemented
  IlcEveV0List& operator=(const IlcEveV0List&); // Not implemented

  ClassDef(IlcEveV0List, 0); // A list of IlcEveV0 objecs.
};


#endif

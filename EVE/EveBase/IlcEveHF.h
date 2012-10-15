// $Id: IlcEveHF.h 32812 2009-06-08 16:37:54Z mtadel $
// Main author: Davide Caffarri 2009
// Base header class to HF visualization

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

#ifndef IlcEveHF_H
#define IlcEveHF_H

#include "IlcAODRecoDecay.h"
#include "IlcEveTrack.h"

#include <TEveVSDStructs.h>
#include <TPolyLine3D.h>

#include <TPDGCode.h>


class TH1F;
class TH2F;


class IlcEveHFList;

class IlcEveHF : public TEvePointSet
{
  friend class IlcEveHFList; //friend class for the list of HF in the same event
  friend class IlcEveHFEditor; //friend class for the list of HF visualization

public:
  IlcEveHF();
  IlcEveHF(TEveRecTrack* tNeg, TEveRecTrack* tPos, Double_t primVtx[3], IlcAODRecoDecay* aodObj, TEveTrackPropagator* rs);
  virtual ~IlcEveHF();

  void MakeHF();

  virtual void SetMainColor(Color_t col)
  {
    TEvePointSet::SetMainColor(col);
    fPointingLine->SetLineColor(fMarkerColor);
  }

  void SetRnrStyle(TEveTrackPropagator* rs) { fRnrStyle = rs; }

  //HF Property

  Float_t GetPhi()     { return fRecDecayHF.Phi(); }
  Float_t GetEta()     { return fRecDecayMomHF.Eta(); }
  Float_t GetRadius()  { return fRecDecayHF.Perp(); }
  Float_t GetPt()      { return fRecDecayMomHF.Perp(); }

  Double_t GetInvariantMassPart()     { CalculateInvMass(fDecay); return fInvariantMassPart; }
  Double_t GetInvariantMassAntiPart() { CalculateInvMass(fDecay); return fInvariantMassAntiPart; }

  Float_t GetChi2Vtx() const { return fChi2SecondVtx; }
  Float_t GetCosPointingAngle() const {return fPointingAngleHF; }

  IlcAODRecoDecay *GetAODobj() const {return fAODobj; }

  Int_t GetAODIndex() const { return fAODIndex; }
  void  SetAODIndex(Int_t ind) { fAODIndex = ind;}

  virtual const Text_t* GetName() const    { return Form("AOD_HF_%i",fAODIndex); }
  virtual const Text_t* GetTitle() const   { return Form("AOD_HF_%i",fAODIndex); }

  //Prongs Property

  Double_t GetProngDCA(Int_t iProng)const { return fProngDCA[iProng]; }
  void SetProngDCA() const ;

  Double_t Getd0Prong(Int_t iProng)const  { return fProngd0[iProng]; }
  void Setd0Prong() const ;

  void CalculateInvMass(Int_t decay);

  Bool_t SelectInvMass(Int_t decay, Float_t decayCuts);

  void      SetMaxProbPdgPid();
  Int_t     GetPdgProngMaxProb(Int_t iProng)const { return fProngMaxProbPdg[iProng]; }
  Double_t  GetPidProngMaxProb(Int_t iProng)const { return fProngMaxProbPid[iProng]; }

  TEveTrackPropagator* GetPropagator() const  { return fRnrStyle; }

  TEveTrack* GetNegTrack()const { return fNegTrack; }
  TEveTrack* GetPosTrack()const { return fPosTrack; }

  TEveLine*  GetPointingLine()const { return fPointingLine; }

protected:

  IlcAODRecoDecay  *fAODobj;  //AOD object of the HF decay. 

  TEveVector  fRecBirthHF;    // Reconstucted birth point of neutral particle
  TEveVector  fRecDecayHF;    // Point of closest approach
  TEveVector  fRecDecayMomHF;   // Momentum of the HF 
  Double_t    fPointingAngleHF; // Track Pointing Angle

  TEveTrack        *fNegTrack;  //Negative daughter of the HF
  TEveTrack        *fPosTrack;  //Positive daughter of the HF

  TEveTrackPropagator *fRnrStyle;  //Eve propagator for the track 

  TEveLine         *fPointingLine;  //Flight Line of the HF 

  Int_t             fnProng;      // Number of Prong.
  Int_t             fAODIndex;    // Index in HF loop array.
  Double_t          fChi2SecondVtx;      //Secondary Vertex Chi-square.

  Double_t          *fProngDCA;//[fnProng] Distance at the point of closest approach.
  Double_t          *fProngd0;//[fnProng] Impact Paramter if each prong.
  Int_t             *fProngMaxProbPdg;//[fnProng] Maximum PDG probability for the negative daughter
  Double_t          *fProngMaxProbPid;//[fnProng] Maximum PID probability for the negative daughter

  Double_t           fInvariantMassPart; //Invariant Mass of the particle
  Double_t           fInvariantMassAntiPart; //Invariant Mass of the Antiparticle

  Int_t              fDecay; //Index for the type of decay

 private:
  IlcEveHF(const IlcEveHF&);            // Not implemented
  IlcEveHF& operator=(const IlcEveHF&); // Not implemented

  ClassDef(IlcEveHF,0); // Visual representation of a IlcEveHF.
};


/******************************************************************************/
// IlcEveHFList
/******************************************************************************/
class IlcEveHFList : public TEveElementList
{
  friend class IlcEveHFListEditor; //Class for the list of HF visualization 


public:
  IlcEveHFList();
  IlcEveHFList(TEveTrackPropagator* rs);
  IlcEveHFList(const Text_t* name, TEveTrackPropagator* rs=0);
  virtual  ~IlcEveHFList() {}

  virtual const Text_t* GetTitle() const { return fTitle; }
  virtual void SetTitle(const Text_t* t) { fTitle = t; }
  virtual void SetTracksColor(Color_t cNeg, Color_t cPos, Int_t ip)
  {fProngColor[ip] = cNeg; fProngColor[ip++] = cPos;}

  virtual Bool_t CanEditMainColor() const { return kTRUE; }

  void  SetRnrStyle(TEveTrackPropagator* rst) { fRnrStyle = rst; }
  TEveTrackPropagator* GetPropagator()        { return fRnrStyle; }

  Bool_t GetRnrHFvtx()     const { return fRnrHFvtx; }
  Bool_t GetRnrHFpath()    const { return fRnrHFpath; }
  Bool_t GetRnrDaughters() const { return fRnrDaughters; }

  void   MakeHFs();

  void   FilterByPt(Float_t minPt, Float_t maxPt);
  void   FilterByRadius(Float_t minR, Float_t maxR);
  void   FilterByCosPointingAngle(Float_t minCosPointingAngle, Float_t maxCosPointingAngle);
  void   FilterByDCA(Float_t minDaughterDCA, Float_t maxDaughterDCA);
  void   FilterByd0(Float_t mind0, Float_t maxd0);
  //void   FilterByCheckedPidMinProb(Int_t rFlag, Int_t rDaughter, Int_t rPid, Float_t rProb);

  // void   SetProngCheckedPid(Int_t rProngCheckedPid) const;
  Int_t  GetProngCheckedPid(Int_t iProng) {return fProngCheckedPid[iProng];}
  // void   SetProngCheckedProb(Float_t rProngCheckedProb) const;
  // Float_t  GetProngCheckedProb(Int_t iProng) const  { return fProngCheckedProb[iProng]; }

  void   FilterByInvariantMass (Int_t decay, Float_t deltaInvariantMass);

protected:
  TString              fTitle; 

  TEveTrackPropagator *fRnrStyle;

  Bool_t               fRnrDaughters; //variable for HF daughters visualization
  Bool_t               fRnrHFvtx; //variable for the HF vertex visualization
  Bool_t               fRnrHFpath; //variable for the visualization of the HF line

  Color_t*             fProngColor;//[fnProng]

  Float_t              fMinRCut; //minimum cut for the radius 
  Float_t              fMaxRCut; //maximum cut for the radius

  Float_t              fMinDaughterDCA; //minimum cut for the DCA of the daughter particles
  Float_t              fMaxDaughterDCA; //maximum cut for the DCA of the daughter particles

  Float_t              fMinPt; //minimum cut for the Pt
  Float_t              fMaxPt; //maximum cut for the Pt

  Float_t              fMinCosPointingAngle; //minimum cut for the cosine of the pointing angle 
  Float_t              fMaxCosPointingAngle; //maximum cut for the cosine of the pointing angle 

  Float_t              fMind0; //minimum cut for the impact parameter
  Float_t              fMaxd0; //maximum cut for the impact parameter

  Int_t*               fProngCheckedPid;//[fnProng]

  Float_t*             fProngCheckedProb;//[fnProng]

  Float_t              fDeltaInvariantMass; //invariant mass window to select the candidate
  Int_t                fDecay; //index for the type of decay

private:
  void Init();

  IlcEveHFList(const IlcEveHFList&);            // Not implemented
  IlcEveHFList& operator=(const IlcEveHFList&); // Not implemented

  ClassDef(IlcEveHFList,0); // A list of IlcEveHF objecs.
};


#endif

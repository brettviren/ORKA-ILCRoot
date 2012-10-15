/**************************************************************************
 * 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * Author: The International Lepton Collider Off-line Project.            *
 *                                                                        *
 * Part of the code has been developed by ILC Off-line Project.         *
 * Contributors are mentioned in the code where appropriate.              *
 *                                                                        *
 * Permission to use, copy, modify and distribute this software and its   *
 * documentation strictly for non-commercial purposes is hereby granted   *
 * without fee, provided that the above copyright notice appears in all   *
 * copies and that both the copyright notice and this permission notice   *
 * appear in the supporting documentation. The authors make no claims     *
 * about the suitability of this software for any purpose. It is          *
 * provided "as is" without express or implied warranty.                  *
 **************************************************************************/


/////////////////////////////////////////////////////
// Class to handle pairs of tracks of opposite charge
// Useful for resonance analysis
// Derives from IlcVParticle => 
// usable in Correction Framework
/////////////////////////////////////////////////////
// author : renaud.vernet@cern.ch
/////////////////////////////////////////////////////


#ifndef ILCCFPAIR_H
#define ILCCFPAIR_H

#include "IlcVParticle.h"

class IlcESDtrack ;
class IlcESDv0;
class IlcESDEvent;
class IlcAODv0;

class IlcCFPair : public IlcVParticle {

 public:
  IlcCFPair(IlcVParticle* t1, IlcVParticle* t2);
  IlcCFPair(IlcESDv0* v0, IlcESDEvent* esd);
  IlcCFPair(IlcAODv0* v0);
  IlcCFPair(const IlcCFPair& c);
  IlcCFPair& operator=(const IlcCFPair& c);
  virtual ~IlcCFPair(){};

  IlcVParticle* GetNeg() const {return fTrackNeg;}
  IlcVParticle* GetPos() const {return fTrackPos;}
  IlcESDv0*    GetESDV0()  const {return fESDV0;}
  IlcAODv0*    GetAODV0()  const {return fAODV0;}
  void         SetV0PDG(Int_t pdg) {fV0PDG=pdg;}
  virtual Bool_t       PxPyPz(Double_t p[3]) const ;
  virtual Double32_t   P()  const ;
  virtual Double32_t   Pt() const ;
  virtual Double32_t   Px() const ;
  virtual Double32_t   Py() const ;
  virtual Double32_t   Pz() const ;
  virtual Double32_t   E () const ;
  virtual Double32_t   Xv() const ;
  virtual Double32_t   Yv() const ;
  virtual Double32_t   Zv() const ;
  virtual Bool_t       XvYvZv(Double_t x[3]) const ;

  virtual Double32_t OneOverPt() const {return 1/Pt();}
  virtual Double32_t Phi()   const ;
  virtual Double32_t Theta() const ;
  virtual Double32_t M() const ;
  virtual Double32_t Eta() const ;
  virtual Double32_t Y() const ;
  virtual Short_t    Charge() const {return 0;} // returns 0 because opposite charge tracks... maybe to extend to all kinds of pairs
  virtual Int_t      GetLabel() const {return fLabel;}
  virtual void       SetLabel(Int_t label) {fLabel=label;}
  virtual void       SetMassHypothesis(Double_t massNeg, Double_t massPos) {fMassNeg=massNeg;fMassPos=massPos;}
  // PID
  virtual const Double_t *PID() const {return 0;} // return PID object (to be defined, still)
  virtual Int_t PdgCode() const {return 0;}

 private:
  Bool_t fIsV0;            // true if V0 passed to the constructor
  IlcVParticle* fTrackNeg; // pointer to the negative track 
  IlcVParticle* fTrackPos; // pointer to the positive track 
  IlcESDv0*    fESDV0;     // pointer to the ESD V0 if IlcESDv0 is passed to the constructor
  IlcAODv0*    fAODV0;     // pointer to the AOD V0 if IlcAODv0 is passed to the constructor
  Int_t        fLabel;     // associated MC label
  Int_t        fV0PDG;     // assumed V0 PDG
  Double_t     fMassNeg;
  Double_t     fMassPos;
  
  ClassDef(IlcCFPair,0);
};

#endif

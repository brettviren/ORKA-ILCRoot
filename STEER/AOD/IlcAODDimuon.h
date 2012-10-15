#ifndef IlcAODDimuon_H
#define IlcAODDimuon_H

/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * Author: The International Lepton Collider Off-line Project.            *
 *                                                                        *
 * Part of the code has been developed by ILC Off-line Project.         */
// Author: P. Cortese, Universita' del Piemonte Orientale in Alessandria and
// INFN of Torino - Italy
//
// The class defines a dimuon pair object from two IlcAODTrack objects.
// IlcAODDimuon objects are supposed to be added to the IlcAODEvent structure
// during analysis. They would then allow to calculate the dimuon-related
// kinematic variables with a minimal disk occupancy.
// The payload of the class has been reduced to two pointers to the two
// tracks. An instance of this class has also to be added to the IlcAODEvent 
// structure to provide additional information that is specific to MUON and 
// therefore has not been included into the AOD header.
// Two transient data members are not stored on file as they can be recomputed
// at runtime.
//

// 2007/07/07 v1.00 Initial version
// 2007/12/06 v1.01 Introduction of IlcAODEventInfo
// 2007/12/18 v1.02 Corrected CostCS for Like-Sign, added CostKh, CostHe and xf
// 2008/02/01 v1.03 Apply coding conventions

#include "TRef.h"
#include "IlcVParticle.h"
#include "IlcAODTrack.h"

class TLorentzVector;

class IlcAODDimuon: public IlcVParticle {
public:
  IlcAODDimuon();
  IlcAODDimuon(const IlcAODDimuon& dimu);
  IlcAODDimuon &operator=(const IlcAODDimuon& dimu);
  IlcAODDimuon(TObject *mu0, TObject *mu1);
  virtual ~IlcAODDimuon();

  // Methods to access kinematics
  virtual Double_t Px() const;
  virtual Double_t Py() const;
  virtual Double_t Pz() const;
  virtual Bool_t PxPyPz(Double_t* p) const { p[0]=Px(); p[1]=Py(); p[2]=Pz(); return 1;}
  virtual Double_t Pt() const;
  virtual Double_t P() const;

  virtual Double_t OneOverPt() const {return Pt()>0 ? 1./Pt() : -999999999;}
  virtual Double_t Phi() const;
  virtual Double_t Theta() const;

  virtual Double_t E() const;
  virtual Double_t M() const;

  virtual Double_t Mass() const { return M(); }

  virtual Double_t Eta() const;
  virtual Double_t Y() const;
  
  virtual Short_t Charge() const;

  // Dimuon vertex will be implemented when the muon track covariance matrix 
  // at vertex will be included in the ESD (and AOD)
  // It would require also the information about magnetic field when filling AOD
  virtual Double_t Xv() const {return -999999999;}
  virtual Double_t Yv() const {return -999999999;}
  virtual Double_t Zv() const {return -999999999;}
  virtual Bool_t XvYvZv(Double_t* v) const { v[0]=-999999999; v[1]=-999999999; v[2]=-999999999; return 0;}

  // Added functions
  Double_t XF();     // Feynman x
  Double_t CostCS(); // Cosinus of the Collins-Soper polar decay angle
  Double_t CostHe(); // Cosinus of the Helicity polar decay angle
  Double_t PhiCS();  // Azimuthal angle in the Collins-Soper frame
  Double_t PhiHe();  // Azimuthal angle in the Helicity frame
  Int_t AnyPt();
  Int_t LowPt();
  Int_t HighPt();
  Double_t MaxChi2Match();
  // PID
  virtual const Double_t *PID() const {return 0;} // return PID object (to be defined, still)
  
  //
  Int_t GetLabel() const {return -1;}
  // Additional getters and setters
  IlcAODTrack* GetMu(Int_t imu=0) const {return Mu(imu); } // Get a pointer to a muon
  IlcAODTrack* Mu(Int_t imu=0) const {return (imu==0||imu==1)&&(fMu[imu]!=0) ? (IlcAODTrack*)fMu[imu].GetObject() : 0; } // Get a pointer to a muon

  void SetMu(Int_t imu=0, IlcAODTrack *mu=0);
  void SetMuons(IlcAODTrack *mu0=0, IlcAODTrack *mu1=0);
  // Dummy
  virtual Int_t PdgCode() const {return 0;}

private:

  Int_t CheckPointers() const;

  TLorentzVector* TLV() const; 

  // Data members
  TRef fMu[2];	// Pointers to the reconstructed muons
  mutable TLorentzVector *fP; //! TLorentzVector of dimuon momentum (not stored into file)

  // Useful constants
  Double_t fMProton; //! Proton mass (not stored into file)

  ClassDef(IlcAODDimuon,2)  // IlcAODDimuon track
};

#endif

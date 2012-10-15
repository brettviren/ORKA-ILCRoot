#ifndef ILCRSTACKHIT_H
#define ILCRSTACKHIT_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcRSTACKebugHit.h 7865 2003-07-13 09:26:14Z hristov $ */

//_________________________________________________________________________
//  Hits class for RSTACK     
//  A hit in RSTACK is the sum of all hits in a single crystal
//               
//*-- Author: Maxime Volkov (RRC KI) & Yves Schutz (SUBATECH)

// --- ROOT system ---
#include <TLorentzVector.h>

// --- IlcRoot header files ---
#include "IlcHit.h"

// --- Standard library ---

class IlcRSTACKHit : public IlcHit {

  friend ostream& operator << (ostream&, const IlcRSTACKHit&) ;
  
 public:
  
  IlcRSTACKHit() {
    // default ctor 
  }
  IlcRSTACKHit(const IlcRSTACKHit & hit) ; 
  IlcRSTACKHit(Int_t shunt, Int_t primary, Int_t tracknumber, Int_t id, Float_t *hits, Int_t pid, TLorentzVector p, Float_t *xy);
  virtual ~IlcRSTACKHit(void) {
    // dtor 
  }  
  
  Float_t GetEnergy(void)   const { 
    // returns the energy loss for this hit 
    return fELOS ; 
  }
  Int_t   GetId(void)       const { 
    // return the identificator of this his
    return fId ; 
  }
  Int_t   GetPid(void)      const { 
    // return the particle PDG code which initiates this hit
    return fPid ; 
  }
  Int_t   GetPrimary(void)  const { 
    // returns the primary particle id at the origine of this hit 
    return fPrimary ; 
  }
  TLorentzVector GetMomentum()  { return  fMomentum; }   
    // momentum of the particle which initiated this hit

  Bool_t operator == (IlcRSTACKHit const &rValue) const ;
  IlcRSTACKHit operator + (const IlcRSTACKHit& rValue) ;


 private:

  Int_t          fId ;        // Absolute Id number of RSTACK Xtal or PPSD pad
  Float_t        fELOS ;      // Energy deposited
  Int_t          fPid ;       // type of the particle that initiates that hit 
  Int_t          fPrimary ;   // Primary particles at the origine of the hit

  TLorentzVector fMomentum;   // 4-momentum of the particle

  ClassDef(IlcRSTACKHit,1)  // Hit for RSTACK

} ;

//////////////////////////////////////////////////////////////////////////////

#endif // ILCRSTACKHIT_H

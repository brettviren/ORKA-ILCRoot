#ifndef ILCPVBARHIT_H
#define ILCPVBARHIT_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcPVBARebugHit.h 7865 2003-07-13 09:26:14Z hristov $ */

//_________________________________________________________________________
//  Hits class for PVBAR     
//  A hit in PVBAR is the sum of all hits in a single crystal
//               
//*-- Author: Maxime Volkov (RRC KI) & Yves Schutz (SUBATECH)

// --- ROOT system ---
#include <TLorentzVector.h>

// --- IlcRoot header files ---
#include "IlcHit.h"

// --- Standard library ---

class IlcPVBARHit : public IlcHit {

  friend ostream& operator << (ostream&, const IlcPVBARHit&) ;
  
 public:
  
  IlcPVBARHit() {
    // default ctor 
  }
  IlcPVBARHit(const IlcPVBARHit & hit) ; 
  IlcPVBARHit(Int_t shunt, Int_t primary, Int_t tracknumber, Int_t id, Float_t *hits, Int_t pid, TLorentzVector p, Float_t *xy);
  virtual ~IlcPVBARHit(void) {
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

  Bool_t operator == (IlcPVBARHit const &rValue) const ;
  IlcPVBARHit operator + (const IlcPVBARHit& rValue) ;


 private:

  Int_t          fId ;        // Absolute Id number of PVBAR Xtal or PPSD pad
  Float_t        fELOS ;      // Energy deposited
  Int_t          fPid ;       // type of the particle that initiates that hit 
  Int_t          fPrimary ;   // Primary particles at the origine of the hit

  TLorentzVector fMomentum;   // 4-momentum of the particle

  ClassDef(IlcPVBARHit,1)  // Hit for PVBAR

} ;

//////////////////////////////////////////////////////////////////////////////

#endif // ILCPVBARHIT_H

#ifndef ILCRSTACKHIT_H
#define ILCRSTACKHIT_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */


//_________________________________________________________________________
//  Hits class for RSTACK     
//  A hit in RSTACK is the sum of all hits in a single scintillator tile
//  from one primary particle and within a time gate of 1 ps
//               
//*-- Author: Maxime Volkov (RRC KI) & Yves Schutz (SUBATECH)

// --- ROOT system ---

// --- IlcRoot header files ---
#include "IlcHit.h"

// --- Standard library ---


class IlcRSTACKHit : public IlcHit {

//   friend ostream& operator << (ostream&, const IlcRSTACKHit&) ;
  
 public:
  
  IlcRSTACKHit() : fId(0),fELOS(0),fTime(0),fNPhotons(0.),fDistFromLowerZ(0.) {
    // default ctor 
  }
  IlcRSTACKHit(const IlcRSTACKHit & hit) ; 
  IlcRSTACKHit(Int_t shunt, Int_t tracknumber, Int_t id, Float_t *hits);
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
  Int_t   GetPrimary(void)  const { 
    // returns the primary particle id at the origine of this hit 
    return fTrack ; 
  }

  Float_t GetTime(void)     const {
    // returns the time of the first energy deposition
    return fTime ;
  }
  Float_t GetNPhotons(void) const { 
    // return the number of photons produced
    return fNPhotons ; 
  }

  Float_t GetDistFromLowerZ(void)     const {
    // returns the distance of the hit production from the lower z edge of the scintillator tile of the first energy deposition
    return fDistFromLowerZ ;
  }

  Bool_t operator == (IlcRSTACKHit const &rValue) const ;
  IlcRSTACKHit operator + (const IlcRSTACKHit& rValue) ;


private:
  IlcRSTACKHit & operator = (const IlcRSTACKHit & /*hit*/);

private:

  Int_t     fId ;        // Absolute Id number of RSTACK Xtal or PPSD pad
  Float_t   fELOS ;      // Energy deposited
  Float_t   fTime ;      // Time of the energy deposition
  Float_t   fNPhotons ;  // Number of photons produced
  Float_t   fDistFromLowerZ ; // Distance of the hit production from the lower z edge of the scintillator tile

  ClassDef(IlcRSTACKHit,1)  // Hit for RSTACK

} ;

//////////////////////////////////////////////////////////////////////////////

#endif // ILCRSTACKHIT_H

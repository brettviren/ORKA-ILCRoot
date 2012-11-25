#ifndef ILCPVBARHIT_H
#define ILCPVBARHIT_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcPVBARHit.h 53186 2011-11-25 14:28:01Z kharlov $ */

//_________________________________________________________________________
//  Hits class for PVBAR     
//  A hit in PVBAR is the sum of all hits in a single crystal
//               
//*-- Author: Maxime Volkov (RRC KI) & Yves Schutz (SUBATECH)

// --- ROOT system ---

// --- IlcRoot header files ---
#include "IlcHit.h"

// --- Standard library ---


class IlcPVBARHit : public IlcHit {

//   friend ostream& operator << (ostream&, const IlcPVBARHit&) ;
  
 public:
  
  IlcPVBARHit() : fId(0),fELOS(0),fTime(0),fNPhotons(0.),fDistFromLowerZ(0.) {
    // default ctor 
  }
  IlcPVBARHit(const IlcPVBARHit & hit) ; 
  IlcPVBARHit(Int_t shunt, Int_t tracknumber, Int_t id, Float_t *hits);
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
    // returns the distance of the hit production from the lower z edge of the PVBAR tile of the first energy deposition
    return fDistFromLowerZ ;
  }

  Bool_t operator == (IlcPVBARHit const &rValue) const ;
  IlcPVBARHit operator + (const IlcPVBARHit& rValue) ;


private:
  IlcPVBARHit & operator = (const IlcPVBARHit & /*hit*/);

private:

  Int_t     fId ;        // Absolute Id number of PVBAR module
  Float_t   fELOS ;      // Energy deposited
  Float_t   fTime ;      // Time of the energy deposition
  Float_t   fNPhotons ;  // Number of photons produced
  Float_t   fDistFromLowerZ ; // Distance of the hit production from the lower z edge of the scintillator tile


  ClassDef(IlcPVBARHit,1)  // Hit for PVBAR

} ;

//////////////////////////////////////////////////////////////////////////////

#endif // ILCPVBARHIT_H

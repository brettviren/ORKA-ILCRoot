#ifndef ILCPVECHIT_H
#define ILCPVECHIT_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcPVECHit.h 53186 2011-11-25 14:28:01Z kharlov $ */

//_________________________________________________________________________
//  Hits class for PVEC     
//  A hit in PVEC is the sum of all hits in a single crystal
//               
//*-- Author: Maxime Volkov (RRC KI) & Yves Schutz (SUBATECH)

// --- ROOT system ---

// --- IlcRoot header files ---
#include "IlcHit.h"

// --- Standard library ---


class IlcPVECHit : public IlcHit {

//   friend ostream& operator << (ostream&, const IlcPVECHit&) ;
  
 public:
  
  IlcPVECHit() : fId(0),fELOS(0),fTime(0) {
    // default ctor 
  }
  IlcPVECHit(const IlcPVECHit & hit) ; 
  IlcPVECHit(Int_t shunt, Int_t tracknumber, Int_t id, Float_t *hits);
  virtual ~IlcPVECHit(void) {
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


  Bool_t operator == (IlcPVECHit const &rValue) const ;
  IlcPVECHit operator + (const IlcPVECHit& rValue) ;


private:
  IlcPVECHit & operator = (const IlcPVECHit & /*hit*/);

private:

  Int_t          fId ;        // Absolute Id number of PVEC Xtal or PPSD pad
  Float_t        fELOS ;      // Energy deposited
  Float_t        fTime ;      // Time of the energy deposition

  ClassDef(IlcPVECHit,1)  // Hit for PVEC

} ;

//////////////////////////////////////////////////////////////////////////////

#endif // ILCPVECHIT_H

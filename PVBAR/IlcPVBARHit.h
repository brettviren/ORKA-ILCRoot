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
  
  IlcPVBARHit() : fId(0),fELOS(0),fTime(0), fTPE(0), fTotalNpe(0) {
    // default ctor 
  }
  IlcPVBARHit(const IlcPVBARHit & hit) ; 
  IlcPVBARHit(Int_t shunt, Int_t tracknumber, Int_t id, Float_t *hits);
  IlcPVBARHit(Int_t shunt, Int_t tracknumber, Int_t id, Float_t *hits, Float_t *TotalNpe);
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

  // Lead glass arrays and variables
  Int_t  GetPeArrayLength(void) const {
    return fTPE;          // Array length of total pe
  }

  Float_t *GetTotalNpe(void)  {
    return fTotalNpe ;  //Contains the sum of Npe 
  }

  Bool_t operator == (IlcPVBARHit const &rValue) const ;
  IlcPVBARHit operator + (const IlcPVBARHit& rValue) ;


private:
  IlcPVBARHit & operator = (const IlcPVBARHit & /*hit*/);

private:

  Int_t          fId ;        // Absolute Id number of PVBAR Xtal or PPSD pad
  Float_t        fELOS ;      // Energy deposited
  Float_t        fTime ;      // Time of the energy deposition

  Int_t   fTPE;
// TotalNpe[0] = total pe generated
// TotalNpe[1] = total pe lead glass front
// TotalNpe[2] = total pe lead glass back
// TotalNpe[3] = total pe WLS or scint front
// TotalNpe[4] = total pe WLS or scint back
  Float_t *fTotalNpe ;  //[fTPE] Contains the sum of photoelectrons

  ClassDef(IlcPVBARHit,1)  // Hit for PVBAR

} ;

//////////////////////////////////////////////////////////////////////////////

#endif // ILCPVBARHIT_H

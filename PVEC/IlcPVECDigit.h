#ifndef ILCPVECDIGIT_H
#define ILCPVECDIGIT_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcPVECDigit.h 53186 2011-11-25 14:28:01Z kharlov $ */

//_________________________________________________________________________
//  PVEC digit: Id
//              energy
//              3 identifiers for the primary particle(s) at the origine of the digit
//  The digits are made in FinishEvent() by summing all the hits in a single PVEC crystal
//  It would be nice to replace the 3 identifiers by an array, but, because digits are kept in a TClonesArray,
//   it is not possible to stream such an array... (beyond my understqnding!)
//
//*-- Author: Laurent Aphecetche & Yves Schutz (SUBATECH)

// --- ROOT system ---

//#include "TObject.h" 

// --- Standard library ---

// --- IlcRoot header files ---

#include "IlcDigitNew.h"

class IlcPVECDigit : public IlcDigitNew {

  friend ostream& operator << ( ostream& , const IlcPVECDigit&) ;

 public:
  
  IlcPVECDigit() ;
  IlcPVECDigit(Int_t primary, Int_t id, Int_t DigEnergy, Float_t Time, Int_t index = -1) ;
  IlcPVECDigit(Int_t primary, Int_t id, Float_t energy , Float_t Time, Int_t index = -1) ;
  IlcPVECDigit(const IlcPVECDigit & digit) ;
  virtual ~IlcPVECDigit() ;

  Bool_t operator==(const IlcPVECDigit &rValue) const;

  IlcPVECDigit& operator += (IlcPVECDigit const &rValue) ;
  IlcPVECDigit& operator *= (Float_t factor) ; 

public:

  Int_t     Compare(const TObject * obj) const ;  
  Int_t     GetNprimary()           const { return fNprimary ; }
  Int_t     GetPrimary(Int_t index) const ; 
  Float_t   GetEnergy(void)         const {return fEnergy ;}
  Float_t   GetTime(void)           const {return fTime ;}
  Float_t   GetTimeR(void)          const {return fTimeR ;}
  Int_t     GetNSamplesHG()         const {return fNSamplesHG;}
  Int_t     GetNSamplesLG()         const {return fNSamplesLG;}
  UShort_t *GetSamplesHG()          const {return fSamplesHG;}
  UShort_t *GetSamplesLG()          const {return fSamplesLG;}
  Bool_t    IsSortable()            const { return kTRUE ; }
  void      Print(const Option_t * = "") const;
  void      SetAmp(Int_t Amp)      {fAmp   = Amp  ;} 
  void      SetEnergy(Float_t E)   {fEnergy= E    ;} 
  void      SetTime(Float_t time)  {fTime  = time ;}
  void      SetTimeR(Float_t time) {fTimeR = time ;}
  void      SetALTROSamplesHG(Int_t nSamplesHG, Int_t *samplesHG);
  void      SetALTROSamplesLG(Int_t nSamplesLG, Int_t *samplesLG);
  void      ShiftPrimary(Int_t shift); // shift to separate different TreeK in merging

private:
  IlcPVECDigit & operator = (const IlcPVECDigit & /*digit*/);

private:

  Int_t       fNprimary ;  // Number of primaries
  Int_t *     fPrimary ;   //[fNprimary] Array of primaries      
  Float_t     fEnergy ;    // Deposited energy in ADC counts
  Float_t     fTime ;      // Calculcated time 
  Float_t     fTimeR ;     // Earliest time: to be used by Digits2Raw
  Int_t       fNSamplesHG; // Number of high-gain ALTRO samples
  Int_t       fNSamplesLG; // Number of low-gain  ALTRO samples
  UShort_t   *fSamplesHG;  //[fNSamplesHG] Array of high-gain ALTRO samples
  UShort_t   *fSamplesLG;  //[fNSamplesLG] Array of low-gain  ALTRO samples

  ClassDef(IlcPVECDigit,6) // Digit in PVEC 

} ;

#endif //  ILCPVECDIGIT_H

#ifndef ILCRSTACKDIGIT_H
#define ILCRSTACKDIGIT_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */


//_________________________________________________________________________
//  RSTACK digit: Id
//              energy
//              3 identifiers for the primary particle(s) at the origine of the digit
//  The digits are made by summing all the hits in a single RSTACK tile
//
//*-- Author: Laurent Aphecetche & Yves Schutz (SUBATECH)

// --- ROOT system ---

// --- Standard library ---

// --- IlcRoot header files ---

#include "IlcDigitNew.h"

class IlcRSTACKDigit : public IlcDigitNew {

  friend ostream& operator << ( ostream& , const IlcRSTACKDigit&) ;

 public:
  
  IlcRSTACKDigit() ;
  IlcRSTACKDigit(Int_t primary, Int_t id, Int_t DigEnergy, Float_t *NPE, Float_t Time, Int_t index = -1) ;
  IlcRSTACKDigit(Int_t primary, Int_t id, Float_t energy, Float_t *NPE, Float_t Time, Int_t index = -1) ;
  IlcRSTACKDigit(const IlcRSTACKDigit & digit) ;
  virtual ~IlcRSTACKDigit() ;

  Bool_t operator==(const IlcRSTACKDigit &rValue) const;

  IlcRSTACKDigit& operator += (IlcRSTACKDigit const &rValue) ;
  IlcRSTACKDigit& operator *= (Float_t factor) ; 

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
  void      SetNPE(Float_t nPE[2])   {fNPE[0]= nPE[0]; fNPE[1]= nPE[1];} 
  Float_t*  GetNPE(void)   {return fNPE;} 

private:
  IlcRSTACKDigit & operator = (const IlcRSTACKDigit & /*digit*/);

private:

  Int_t       fNprimary ;  // Number of primaries
  Int_t *     fPrimary ;   //[fNprimary] Array of primaries      
  Float_t     fEnergy ;    // Deposited energy in ADC counts
  Float_t     fNPE[2] ;    // Number of photo-electrons in ADC counts for both scintillator ends
  Float_t     fTime ;      // Calculcated time 
  Float_t     fTimeR ;     // Earliest time: to be used by Digits2Raw
  Int_t       fNSamplesHG; // Number of high-gain ALTRO samples
  Int_t       fNSamplesLG; // Number of low-gain  ALTRO samples
  UShort_t   *fSamplesHG;  //[fNSamplesHG] Array of high-gain ALTRO samples
  UShort_t   *fSamplesLG;  //[fNSamplesLG] Array of low-gain  ALTRO samples

  ClassDef(IlcRSTACKDigit,6) // Digit in RSTACK 

} ;

#endif //  ILCRSTACKDIGIT_H

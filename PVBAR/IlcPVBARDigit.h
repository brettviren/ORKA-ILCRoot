#ifndef ILCPVBARDIGIT_H
#define ILCPVBARDIGIT_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcPVBARDigit.h 53186 2011-11-25 14:28:01Z kharlov $ */

/* History of cvs commits:
 *
 * $Log$
 * Revision 1.34  2006/04/22 10:30:17  hristov
 * Add fEnergy to IlcPVBARDigit and operate with EMC amplitude in energy units (Yu.Kharlov)
 *
 * Revision 1.33  2005/05/28 14:19:04  schutz
 * Compilation warnings fixed by T.P.
 *
 */

//_________________________________________________________________________
//  PVBAR digit: Id
//              energy
//              3 identifiers for the primary particle(s) at the origine of the digit
//  The digits are made in FinishEvent() by summing all the hits in a single PVBAR crystal or PPSD gas cell
//  It would be nice to replace the 3 identifiers by an array, but, because digits are kept in a TClonesArray,
//   it is not possible to stream such an array... (beyond my understqnding!)
//
//*-- Author: Laurent Aphecetche & Yves Schutz (SUBATECH)

// --- ROOT system ---

//#include "TObject.h" 

// --- Standard library ---

// --- IlcRoot header files ---

#include "IlcDigitNew.h"

class IlcPVBARDigit : public IlcDigitNew {

  friend ostream& operator << ( ostream& , const IlcPVBARDigit&) ;

 public:
  
  IlcPVBARDigit() ;
  IlcPVBARDigit(Int_t primary, Int_t id, Int_t DigEnergy, Float_t *NPE, Float_t Time, Int_t index = -1) ;
  IlcPVBARDigit(Int_t primary, Int_t id, Float_t energy, Float_t *NPE, Float_t Time, Int_t index = -1) ;
  IlcPVBARDigit(const IlcPVBARDigit & digit) ;
  virtual ~IlcPVBARDigit() ;

  Bool_t operator==(const IlcPVBARDigit &rValue) const;

  IlcPVBARDigit& operator += (IlcPVBARDigit const &rValue) ;
  IlcPVBARDigit& operator *= (Float_t factor) ; 

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
  void      SetNPE(Float_t nPE[4])   {for(Int_t idx=0; idx<4; idx++) fNPE[idx]= nPE[idx];} 
  Float_t*  GetNPE(void)   {return fNPE;} 

private:
  IlcPVBARDigit & operator = (const IlcPVBARDigit & /*digit*/);

private:

  Int_t       fNprimary ;  // Number of primaries
  Int_t *     fPrimary ;   //[fNprimary] Array of primaries      
  Float_t     fEnergy ;    // Deposited energy in ADC counts
  Float_t     fNPE[4] ;    // Number of photo-electrons in ADC counts
// fNPE[0] = total pe lead glass front
// fNPE[1] = total pe lead glass back
// fNPE[2] = total pe WLS or scint front
// fNPE[3] = total pe WLS or scint back
  Float_t     fTime ;      // Calculcated time 
  Float_t     fTimeR ;     // Earliest time: to be used by Digits2Raw
  Int_t       fNSamplesHG; // Number of high-gain ALTRO samples
  Int_t       fNSamplesLG; // Number of low-gain  ALTRO samples
  UShort_t   *fSamplesHG;  //[fNSamplesHG] Array of high-gain ALTRO samples
  UShort_t   *fSamplesLG;  //[fNSamplesLG] Array of low-gain  ALTRO samples


  ClassDef(IlcPVBARDigit,6) // Digit in PVBAR 

} ;

#endif //  ILCPVBARDIGIT_H

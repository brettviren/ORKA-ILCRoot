#ifndef ILCDCHDIGIT_H
#define ILCDCHDIGIT_H
/* Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcDCHdigit.h,v */

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//  The DCH digit                                                            //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#include "IlcDigitNew.h"

//_____________________________________________________________________________
class IlcDCHdigit : public IlcDigitNew {

 public:

  IlcDCHdigit();
  IlcDCHdigit(Bool_t isRaw, Int_t *digits, Int_t *amp);
  virtual ~IlcDCHdigit();

  static  UInt_t RawDigit()          { return fgkRawDigit; };

          Int_t  GetAmp() const      { if (TestBit(fgkRawDigit))
                                         return DecodeAmp();
                                       else
                                         return fAmp; };
          Int_t  GetDetector() const { return fId;   };
          Int_t  GetRow() const      { return fRow;  };
          Int_t  GetCol() const      { return fCol;  };
          Int_t  GetTime() const     { return fTime; };

  virtual Int_t  DecodeAmp() const;

 protected:

  static const UInt_t fgkRawDigit; // Marks a raw digit

  UShort_t     fRow;               // Pad row number
  UShort_t     fCol;               // Pad col number
  UShort_t     fTime;              // Time bucket

  ClassDef(IlcDCHdigit,2)          // Digit for the DCH

};

#endif

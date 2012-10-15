/**************************************************************************
 * Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 *                                                                        *
// Author: The ILC Off-line Project. 
 // Part of the code has been developed by Alice Off-line Project. 
 * Contributors are mentioned in the code where appropriate.              *
 *                                                                        *
 * Permission to use, copy, modify and distribute this software and its   *
 * documentation strictly for non-commercial purposes is hereby granted   *
 * without fee, provided that the above copyright notice appears in all   *
 * copies and that both the copyright notice and this permission notice   *
 * appear in the supporting documentation. The authors make no claims     *
 * about the suitability of this software for any purpose. It is          *
 * provided "as is" without express or implied warranty.                  *
 **************************************************************************/

/* $Id: IlcDCHCalROC.cxx,v 1.1.1.1 2008/03/11 14:53:01 vitomeg Exp $ */

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//  Calibration base class for a single ROC                                  //
//  Contains one UShort_t value per pad                                      //
//  However, values are set and get as float, there are stored internally as //
//  (UShort_t) value * 10000                                                 //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#include "IlcDCHCalROC.h"

ClassImp(IlcDCHCalROC)

//_____________________________________________________________________________
IlcDCHCalROC::IlcDCHCalROC():TObject()
{
  //
  // Default constructor
  //

  fPla          = 0;
  fCha          = 0;

  fNrows        = 0;
  fNcols        = 0;

  fNchannels    = 0;
  fData         = 0;
}

//_____________________________________________________________________________
IlcDCHCalROC::IlcDCHCalROC(Int_t p, Int_t c):TObject()
{
  //
  // Constructor that initializes a given pad plane type
  //

  fPla = p;
  fCha = c;

  fNcols      = 144;

  //
  // The pad plane parameter
  //
  switch (p) {
  case 0:
    if (c == 2) {
      // L0C0 type
      fNrows        =  12;
    }
    else {
      // L0C1 type
      fNrows        =  16;
    }
    break;
  case 1:
    if (c == 2) {
      // L1C0 type
      fNrows        =  12;
    }
    else {
      // L1C1 type
      fNrows        =  16;
    }
    break;
  case 2:
    if (c == 2) {
      // L2C0 type
      fNrows        =  12;
    }
    else {
      // L2C1 type
      fNrows        =  16;
    }
    break;
  case 3:
    if (c == 2) {
      // L3C0 type
      fNrows        =  12;
    }
    else {
      // L3C1 type
      fNrows        =  16;
    }
    break;
  case 4:
    if (c == 2) {
      // L4C0 type
      fNrows        =  12;
    }
    else {
      // L4C1 type
      fNrows        =  16;
    }
    break;
  case 5:
    if (c == 2) {
      // L5C0 type
      fNrows        =  12;
    }
    else {
      // L5C1 type
      fNrows        =  16;
    }
    break;
  };

  fNchannels = fNrows * fNcols;
  if (fNchannels != 0)
    fData = new UShort_t[fNchannels];

  for (Int_t i=0; i<fNchannels; ++i)
    fData[i] = 0;
}

//_____________________________________________________________________________
IlcDCHCalROC::IlcDCHCalROC(const IlcDCHCalROC &c):TObject(c)
{
  //
  // IlcDCHCalROC copy constructor
  //

  ((IlcDCHCalROC &) c).Copy(*this);

}

//_____________________________________________________________________________
IlcDCHCalROC::~IlcDCHCalROC()
{
  //
  // IlcDCHCalROC destructor
  //

  if (fData) {
    delete [] fData;
    fData = 0;
  }
}

//_____________________________________________________________________________
IlcDCHCalROC &IlcDCHCalROC::operator=(const IlcDCHCalROC &c)
{
  //
  // Assignment operator
  //

  if (this != &c) ((IlcDCHCalROC &) c).Copy(*this);
  return *this;

}

//_____________________________________________________________________________
void IlcDCHCalROC::Copy(TObject &c) const
{
  //
  // Copy function
  //

  ((IlcDCHCalROC &) c).fPla          = fPla;
  ((IlcDCHCalROC &) c).fCha          = fCha;

  ((IlcDCHCalROC &) c).fNrows        = fNrows;
  ((IlcDCHCalROC &) c).fNcols        = fNcols;

  Int_t iBin = 0;

  ((IlcDCHCalROC &) c).fNchannels = fNchannels;

  if (((IlcDCHCalROC &) c).fData) delete [] ((IlcDCHCalROC &) c).fData;
  ((IlcDCHCalROC &) c).fData = new UShort_t[fNchannels];
  for (iBin = 0; iBin < fNchannels; iBin++) {
    ((IlcDCHCalROC &) c).fData[iBin] = fData[iBin];
  }

  TObject::Copy(c);

}

//_____________________________________________________________________________
void IlcDCHCalROC::Scale(Float_t value)
{
  //
  // Scales all values of this ROC with the provided parameter. Is used if ROC defines
  // local variations of a global (or per detector defined) parameter
  //

  for (Int_t iBin = 0; iBin < fNchannels; iBin++) {
    fData[iBin] = (UShort_t) (value * fData[iBin]);
  }
}

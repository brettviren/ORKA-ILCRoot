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

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//  Calibration base class for a single ROC                                  //
//  Contains one char value per pad                                         //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#include "IlcDCHCalSingleChamberStatus.h"

ClassImp(IlcDCHCalSingleChamberStatus)

//_____________________________________________________________________________
IlcDCHCalSingleChamberStatus::IlcDCHCalSingleChamberStatus():TObject()
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
IlcDCHCalSingleChamberStatus::IlcDCHCalSingleChamberStatus(Int_t p, Int_t c, Int_t cols):TObject()
{
  //
  // Constructor that initializes a given pad plane type
  //

  fPla = p;
  fCha = c;

  fNcols      = cols;

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
    fData = new Char_t[fNchannels];

  for (Int_t i=0; i<fNchannels; ++i)
    fData[i] = 0;
}

//_____________________________________________________________________________
IlcDCHCalSingleChamberStatus::IlcDCHCalSingleChamberStatus(const IlcDCHCalSingleChamberStatus &c):TObject(c)
{
  //
  // IlcDCHCalSingleChamberStatus copy constructor
  //

  ((IlcDCHCalSingleChamberStatus &) c).Copy(*this);

}

//_____________________________________________________________________________
IlcDCHCalSingleChamberStatus::~IlcDCHCalSingleChamberStatus()
{
  //
  // IlcDCHCalSingleChamberStatus destructor
  //

  if (fData) {
    delete [] fData;
    fData = 0;
  }
}

//_____________________________________________________________________________
IlcDCHCalSingleChamberStatus &IlcDCHCalSingleChamberStatus::operator=(const IlcDCHCalSingleChamberStatus &c)
{
  //
  // Assignment operator
  //

  if (this != &c) ((IlcDCHCalSingleChamberStatus &) c).Copy(*this);
  return *this;

}

//_____________________________________________________________________________
void IlcDCHCalSingleChamberStatus::Copy(TObject &c) const
{
  //
  // Copy function
  //

  ((IlcDCHCalSingleChamberStatus &) c).fPla          = fPla;
  ((IlcDCHCalSingleChamberStatus &) c).fCha          = fCha;

  ((IlcDCHCalSingleChamberStatus &) c).fNrows        = fNrows;
  ((IlcDCHCalSingleChamberStatus &) c).fNcols        = fNcols;

  Int_t iBin = 0;

  ((IlcDCHCalSingleChamberStatus &) c).fNchannels = fNchannels;

  if (((IlcDCHCalSingleChamberStatus &) c).fData) delete [] ((IlcDCHCalSingleChamberStatus &) c).fData;
  ((IlcDCHCalSingleChamberStatus &) c).fData = new Char_t[fNchannels];
  for (iBin = 0; iBin < fNchannels; iBin++) {
    ((IlcDCHCalSingleChamberStatus &) c).fData[iBin] = fData[iBin];
  }

  TObject::Copy(c);

}

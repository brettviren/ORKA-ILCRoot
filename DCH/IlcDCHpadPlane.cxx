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

/* $Id: IlcDCHpadPlane.cxx,v 1.1.1.1 2008/03/11 14:53:01 vitomeg Exp $ */

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//  Describes a pad plane of a DCH ROC                                       //
//                                                                           //
//  Contains the information on pad postions, pad dimensions,                //
//  tilting angle, etc.                                                      //
//  It also provides methods to identify the current pad number from         //
//  global coordinates.                                                      //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#include "IlcDCHpadPlane.h"
#include "IlcDCHgeometry.h"

ClassImp(IlcDCHpadPlane)

//_____________________________________________________________________________
IlcDCHpadPlane::IlcDCHpadPlane():TObject()
{
  //
  // Default constructor
  //

  fGeo          = 0;

  fPla          = 0;
  fCha          = 0;

  fLength       = 0.0;
  fWidth        = 0.0;
  fLengthRim    = 0.0;
  fWidthRim     = 0.0;
  fLengthOPad   = 0.0;
  fWidthOPad    = 0.0;
  fLengthIPad   = 0.0;
  fWidthIPad    = 0.0;

  fRowSpacing   = 0.0;
  fColSpacing   = 0.0;

  fNrows        = 0;
  fNcols        = 0;

  fPadRow       = 0;
  fPadCol       = 0;

  fTiltingAngle = 0.0;
  fTiltingTan   = 0.0;

}

//_____________________________________________________________________________
IlcDCHpadPlane::IlcDCHpadPlane(Int_t p, Int_t c):TObject(),fPadRow(0),fPadCol(0)
{
  //
  // Constructor that initializes a given pad plane type
  //

  fGeo = new IlcDCHgeometry();

  fPla = p;
  fCha = c;

  fRowSpacing = 0.0;
  fColSpacing = 0.0;

  fLengthRim  = 1.0;
  fWidthRim   = 0.5;

  fNcols      = 144;

  //
  // The pad plane parameter
  //
  switch (p) {
  case 0:
    if (c == 2) {
      // L0C0 type
      fNrows        =  12;
      fLength       = 108.0;
      fWidth        =  92.2;
      fLengthOPad   =   8.0;
      fWidthOPad    =   0.515;
      fLengthIPad   =   9.0;
      fWidthIPad    =   0.635;
      fTiltingAngle =  -2.0;
    }
    else {
      // L0C1 type
      fNrows        =  16;
      fLength       = 122.0;
      fWidth        =  92.2;
      fLengthOPad   =   7.5;
      fWidthOPad    =   0.515;
      fLengthIPad   =   7.5;
      fWidthIPad    =   0.635;
      fTiltingAngle =  -2.0;
    }
    break;
  case 1:
    if (c == 2) {
      // L1C0 type
      fNrows        =  12;
      fLength       = 108.0;
      fWidth        =  96.6;
      fLengthOPad   =   8.0;
      fWidthOPad    =   0.585;
      fLengthIPad   =   9.0;
      fWidthIPad    =   0.665;
      fTiltingAngle =   2.0;
    }
    else {
      // L1C1 type
      fNrows        =  16;
      fLength       = 122.0;
      fWidth        =  96.6;
      fLengthOPad   =   7.5;
      fWidthOPad    =   0.585;
      fLengthIPad   =   7.5;
      fWidthIPad    =   0.665;
      fTiltingAngle =   2.0;
    }
    break;
  case 2:
    if (c == 2) {
      // L2C0 type
      fNrows        =  12;
      fLength       = 108.0;
      fWidth        = 101.1;
      fLengthOPad   =   8.0;
      fWidthOPad    =   0.705;
      fLengthIPad   =   9.0;
      fWidthIPad    =   0.695;
      fTiltingAngle =  -2.0;
    }
    else {
      // L2C1 type
      fNrows        =  16;
      fLength       = 129.0;
      fWidth        = 101.1;
      fLengthOPad   =   7.5;
      fWidthOPad    =   0.705;
      fLengthIPad   =   8.0;
      fWidthIPad    =   0.695;
      fTiltingAngle =  -2.0;
    }
    break;
  case 3:
    if (c == 2) {
      // L3C0 type
      fNrows        =  12;
      fLength       = 108.0;
      fWidth        = 105.5;
      fLengthOPad   =   8.0;
      fWidthOPad    =   0.775;
      fLengthIPad   =   9.0;
      fWidthIPad    =   0.725;
      fTiltingAngle =   2.0;
    }
    else {
      // L3C1 type
      fNrows        =  16;
      fLength       = 136.0;
      fWidth        = 105.5;
      fLengthOPad   =   7.5;
      fWidthOPad    =   0.775;
      fLengthIPad   =   8.5;
      fWidthIPad    =   0.725;
      fTiltingAngle =   2.0;
    }
    break;
  case 4:
    if (c == 2) {
      // L4C0 type
      fNrows        =  12;
      fLength       = 108.0;
      fWidth        = 109.9;
      fLengthOPad   =   8.0;
      fWidthOPad    =   0.845;
      fLengthIPad   =   9.0;
      fWidthIPad    =   0.755;
      fTiltingAngle =  -2.0;
    }
    else {
      // L4C1 type
      fNrows        =  16;
      fLength       = 143.0;
      fWidth        = 109.9;
      fLengthOPad   =   7.5;
      fWidthOPad    =   0.845;
      fLengthIPad   =   9.0;
      fWidthIPad    =   0.755;
      fTiltingAngle =  -2.0;
    }
    break;
  case 5:
    if (c == 2) {
      // L5C0 type
      fNrows        =  12;
      fLength       = 108.0;
      fWidth        = 114.4;
      fLengthOPad   =   8.0;
      fWidthOPad    =   0.965;
      fLengthIPad   =   9.0;
      fWidthIPad    =   0.785;
      fTiltingAngle =   2.0;
    }
    else {
      // L5C1 type
      fNrows        =  16;
      fLength       = 145.0;
      fWidth        = 114.4;
      fLengthOPad   =   8.5;
      fWidthOPad    =   0.965;
      fLengthIPad   =   9.0;
      fWidthIPad    =   0.785;
      fTiltingAngle =   2.0;
    }
    break;
  };

  //
  // Store tilting angle as tangens
  //
  fTiltingTan = TMath::Tan(TMath::Pi()/180.0 * fTiltingAngle);

  //
  // The positions of the borders of the pads
  //
  // Row direction
  //
  fPadRow = new Double_t[fNrows];
  Double_t row = fGeo->GetChamberLength(p,0)
    	       + fGeo->GetChamberLength(p,1)
               + fGeo->GetChamberLength(p,2) / 2.
               - fGeo->RpadW()
               - fLengthRim;
  for (Int_t ic = 0; ic < c; ic++) {
    row -= fGeo->GetChamberLength(p,ic);
  }
  for (Int_t ir = 0; ir < fNrows; ir++) {
    fPadRow[ir] = row;
    row -= fRowSpacing;
    if (ir == 0) {
      row -= fLengthOPad;
    }
    else {
      row -= fLengthIPad;
    }
  }
  //
  // Column direction
  //
  fPadCol = new Double_t[fNcols];
  Double_t col = fGeo->GetChamberWidth(p) / 2. 
               + fGeo->CroWid()
               - fWidthRim;
  for (Int_t ic = 0; ic < fNcols; ic++) {
    fPadCol[ic] = col;
    col -= fColSpacing;
    if (ic == 0) {
      col -= fWidthOPad;
    }
    else {
      col -= fWidthIPad;
    }
  }

}

//_____________________________________________________________________________
IlcDCHpadPlane::IlcDCHpadPlane(const IlcDCHpadPlane &p):TObject(p)
{
  //
  // IlcDCHpadPlane copy constructor
  //

  ((IlcDCHpadPlane &) p).Copy(*this);

}

//_____________________________________________________________________________
IlcDCHpadPlane::~IlcDCHpadPlane()
{
  //
  // IlcDCHpadPlane destructor
  //

  if (fGeo) {
    delete fGeo;
    fGeo    = 0;
  }

  if (fPadRow) {
    delete [] fPadRow;
    fPadRow = 0;
  }

  if (fPadCol) {
    delete [] fPadCol;
    fPadCol = 0;
  }

}

//_____________________________________________________________________________
IlcDCHpadPlane &IlcDCHpadPlane::operator=(const IlcDCHpadPlane &p)
{
  //
  // Assignment operator
  //

  if (this != &p) ((IlcDCHpadPlane &) p).Copy(*this);
  return *this;

}

//_____________________________________________________________________________
void IlcDCHpadPlane::Copy(TObject &p) const
{
  //
  // Copy function
  //

  Int_t iBin = 0;

  ((IlcDCHpadPlane &) p).fGeo          = 0;

  ((IlcDCHpadPlane &) p).fPla          = fPla;
  ((IlcDCHpadPlane &) p).fCha          = fCha;

  ((IlcDCHpadPlane &) p).fLength       = fLength;
  ((IlcDCHpadPlane &) p).fWidth        = fWidth;
  ((IlcDCHpadPlane &) p).fLengthRim    = fLengthRim;
  ((IlcDCHpadPlane &) p).fWidthRim     = fWidthRim;
  ((IlcDCHpadPlane &) p).fLengthOPad   = fLengthOPad;
  ((IlcDCHpadPlane &) p).fWidthOPad    = fWidthOPad;
  ((IlcDCHpadPlane &) p).fLengthIPad   = fLengthIPad;
  ((IlcDCHpadPlane &) p).fWidthIPad    = fWidthIPad;

  ((IlcDCHpadPlane &) p).fRowSpacing   = fRowSpacing;
  ((IlcDCHpadPlane &) p).fColSpacing   = fColSpacing;

  ((IlcDCHpadPlane &) p).fNrows        = fNrows;
  ((IlcDCHpadPlane &) p).fNcols        = fNcols;

  ((IlcDCHpadPlane &) p).fTiltingAngle = fTiltingAngle;
  ((IlcDCHpadPlane &) p).fTiltingTan   = fTiltingTan;

  if (((IlcDCHpadPlane &) p).fPadRow) delete [] ((IlcDCHpadPlane &) p).fPadRow;
  ((IlcDCHpadPlane &) p).fPadRow = new Double_t[fNrows];
  for (iBin = 0; iBin < fNrows; iBin++) {
    ((IlcDCHpadPlane &) p).fPadRow[iBin] = fPadRow[iBin];
  }                                                                             

  if (((IlcDCHpadPlane &) p).fPadCol) delete [] ((IlcDCHpadPlane &) p).fPadCol;
  ((IlcDCHpadPlane &) p).fPadCol = new Double_t[fNrows];
  for (iBin = 0; iBin < fNrows; iBin++) {
    ((IlcDCHpadPlane &) p).fPadCol[iBin] = fPadCol[iBin];
  }                                                                             

  TObject::Copy(p);

}

//_____________________________________________________________________________
Int_t IlcDCHpadPlane::GetPadRowNumber(Double_t z) const
{
  //
  // Finds the pad row number for a given global z-position
  //

  Int_t row    = 0;
  Int_t nabove = 0;
  Int_t nbelow = 0;
  Int_t middle = 0;

  if ((z > GetRow0()) || (z < GetRowEnd())) {

    row = -1;

  }
  else {

    nabove = fNrows+1;
    nbelow = 0;
    while (nabove - nbelow > 1) {
      middle = (nabove + nbelow) / 2;
      if (z == fPadRow[middle-1]) row    = middle;
      if (z  > fPadRow[middle-1]) nabove = middle;
      else                        nbelow = middle;
    }
    row = nbelow - 1;

  }

  return row;

}

//_____________________________________________________________________________
Int_t IlcDCHpadPlane::GetPadColNumber(Double_t rphi
				      , Double_t /*rowOffset*/) const
{
  //
  // Finds the pad column number for a given global rphi-position
  //

  Int_t    col       = 0;
  Int_t    nabove    = 0;
  Int_t    nbelow    = 0;
  Int_t    middle    = 0;
  Double_t rphiShift = 0;

  // MI change don't apply tilting angle here - better to do it directly on hit level
  // Take the tilting angle into account by shifting the hit position
  // into the opposite direction
  // 


  rphiShift = rphi ;

  if ((rphiShift > GetCol0()) || (rphiShift < GetColEnd())) {

    col = -1;

  }
  else {

    nabove = fNcols+1;
    nbelow = 0;
    while (nabove - nbelow > 1) {
      middle = (nabove + nbelow) / 2;
      if (rphiShift == fPadCol[middle-1]) col    = middle;
      if (rphiShift  > fPadCol[middle-1]) nabove = middle;
      else                                nbelow = middle;
    }
    col = nbelow - 1;

  }

  return col;

}

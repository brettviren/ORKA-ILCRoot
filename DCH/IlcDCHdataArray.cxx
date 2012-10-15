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

/* $Id: IlcDCHdataArray.cxx,v 1.1.1.1 2008/03/11 14:53:01 vitomeg Exp $ */

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//  Base class of a general container for data of a DCH detector segment.    //
//  Adapted from IlcDigits (origin: M.Ivanov).                               //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#include <stdlib.h> 
#include <Riostream.h>
#include "TClass.h"
#include "TError.h"
#include "IlcDCHsegmentID.h"
#include "IlcDCHarrayI.h"
#include "IlcDCHdataArray.h"

ClassImp(IlcDCHdataArray)

//_____________________________________________________________________________
IlcDCHdataArray::IlcDCHdataArray()
{
  //
  // Default constructor
  //

  fIndex   =  0;

  fNdim1   = -1;
  fNdim2   = -1;
  fNelems  = -1; 

  fBufType = -1;

  fNrow    =  0;
  fNcol    =  0;
  fNtime   =  0;

}

//_____________________________________________________________________________
IlcDCHdataArray::IlcDCHdataArray(Int_t nrow, Int_t ncol, Int_t ntime)
{
  //
  // Creates a IlcDCHdataArray with the dimensions <nrow>, <ncol>, and <ntime>.
  // The row- and column dimensions are compressible.
  //

  fIndex   = 0;

  Allocate(nrow,ncol,ntime);

}

//_____________________________________________________________________________
IlcDCHdataArray::IlcDCHdataArray(const IlcDCHdataArray &d):IlcDCHsegmentID(d)
{
  //
  // IlcDCHdataArray copy constructor
  //

  ((IlcDCHdataArray &) d).Copy(*this);

}

//_____________________________________________________________________________
IlcDCHdataArray::~IlcDCHdataArray()
{
  //
  // IlcDCHdataArray destructor
  //

  if (fIndex) delete fIndex;
  
}

//_____________________________________________________________________________
IlcDCHdataArray &IlcDCHdataArray::operator=(const IlcDCHdataArray &d)
{
  //
  // Assignment operator
  //

  if (this != &d) ((IlcDCHdataArray &) d).Copy(*this);
  return *this;

}

//_____________________________________________________________________________
void IlcDCHdataArray::Copy(TObject &d) const
{
  //
  // Copy function
  //

  ((IlcDCHdataArray &) d).fNrow         = fNrow;
  ((IlcDCHdataArray &) d).fNcol         = fNcol;
  ((IlcDCHdataArray &) d).fNtime        = fNtime;

  ((IlcDCHdataArray &) d).fNdim1        = fNdim1;
  ((IlcDCHdataArray &) d).fNdim2        = fNdim2;

  ((IlcDCHdataArray &) d).fBufType      = fBufType;
  ((IlcDCHdataArray &) d).fNelems       = fNelems;

  ((IlcDCHdataArray &) d).fCurrentIdx1  = 0;
  ((IlcDCHdataArray &) d).fCurrentIdx2  = 0;
  ((IlcDCHdataArray &) d).fCurrentIndex = 0;

  fIndex->Copy(*((IlcDCHdataArray &) d).fIndex);

}

//_____________________________________________________________________________
void IlcDCHdataArray::Allocate(Int_t nrow, Int_t ncol,Int_t ntime)
{
  //
  // Allocates memory for a IlcDCHdataArray with the dimensions 
  // <nrow>, <ncol>, and <ntime>.
  // The row- and column dimensions are compressible.
  //
 
  if (nrow  <=0) {
    Error("IlcDCHdataArray::Allocate","The number of rows has to be positive");
    exit(1);
  }
  if (ncol  <=0) {
    Error("IlcDCHdataArray::Allocate","The number of columns has to be positive");
    exit(1);
  }
  if (ntime <= 0) {
    Error("IlcDCHdataArray::Allocate","The number of timebins has to be positive");
    exit(1);
  }

  // The two-dimensional array row/column gets mapped into the first 
  // dimension of the array. The second array dimension, which is not compressible,
  // corresponds to the time direction
  fNdim1  = nrow * ncol;
  fNdim2  = ntime;
  fNelems = fNdim1 * fNdim2;

  fNrow   = nrow;
  fNcol   = ncol;
  fNtime  = ntime;

  if (fIndex) delete fIndex;
  fIndex = new IlcDCHarrayI();
  fIndex->Set(fNdim2);
  for (Int_t i = 0, k = 0; i < fNdim2; i++, k += fNdim1) { 
    (*fIndex)[i] = k;
  }

  fBufType = 0;

}

//_____________________________________________________________________________
Bool_t IlcDCHdataArray::CheckBounds(const char *where, Int_t idx1, Int_t idx2) 
{
  //
  // Does the boundary checking
  //

  if ((idx2 >= fNdim2) || (idx2 < 0)) 
    return OutOfBoundsError(where,idx1,idx2);

  Int_t index = (*fIndex).At(idx2) + idx1;
  if ((index < 0) || (index > fNelems)) 
    return OutOfBoundsError(where,idx1,idx2);

  return kTRUE;  

}

//_____________________________________________________________________________
Bool_t IlcDCHdataArray::OutOfBoundsError(const char *where, Int_t idx1, Int_t idx2) 
{
  //
  // Generate an out-of-bounds error. Always returns false.
  //

  TObject::Error(where, "idx1 %d  idx2 %d out of bounds (size: %d x %d, this: 0x%08x)"
	   ,idx1,idx2,fNdim1,fNdim2,this);

  return kFALSE;

}

//_____________________________________________________________________________
void IlcDCHdataArray::Reset() 
{ 
  //
  // Reset the array (old content gets deleted)
  //

  if (fIndex) delete fIndex;
  fIndex = new IlcDCHarrayI();
  fIndex->Set(0); 

  fNdim1   = -1;
  fNdim2   = -1;
  fNelems  = -1; 

  fBufType = -1;

  fNrow    =  0;
  fNcol    =  0;
  fNtime   =  0;

}

//_____________________________________________________________________________
Int_t IlcDCHdataArray::GetIdx1(Int_t row, Int_t col) const
{
  //
  // Maps the two-dimensional row/column plane into an one-dimensional array.
  //

  if (row >= fNrow) {
    TObject::Error("GetIdx1"
                  ,"row %d out of bounds (size: %d, this: 0x%08x)"
                  ,row,fNrow,this);
    return -1;
  }  

  if (col >= fNcol) {
    TObject::Error("GetIdx1"
                  ,"col %d out of bounds (size: %d, this: 0x%08x)"
                  ,col,fNcol,this);
    return -1;
  }  

  return row + col * fNrow;

}

//_____________________________________________________________________________
Int_t IlcDCHdataArray::GetIndex(Int_t row, Int_t col, Int_t time) const
{
  //
  // Maps the row/column/time into one number
  // 

  if (time > fNtime) {
    TObject::Error("GetIdx1"
                  ,"time %d out of bounds (size: %d, this: 0x%08x)"
                  ,time,fNtime,this);
    return -1;
  }  
  
  return time * fNrow*fNcol + GetIdx1(row,col);

}
 

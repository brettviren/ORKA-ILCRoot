#ifndef ILCDCHDATAARRAY_H
#define ILCDCHDATAARRAY_H

/* Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcDCHdataArray.h,v 1.1.1.1 2008/03/11 14:53:01 vitomeg Exp $ */
 
#include   "IlcDCHsegmentID.h"

/////////////////////////////////////////////////////////////
//  General container for data from DCH detector segments  //
//  Adapted from IlcDigits, origin M.Ivanov                //
/////////////////////////////////////////////////////////////

class IlcDCHarrayI;

class IlcDCHdataArray : public IlcDCHsegmentID {

 public:

  IlcDCHdataArray();
  IlcDCHdataArray(Int_t nrow, Int_t ncol,Int_t ntime);
  IlcDCHdataArray(const IlcDCHdataArray &d);
  virtual ~IlcDCHdataArray();
  IlcDCHdataArray &operator=(const IlcDCHdataArray &d);

  virtual void   Copy(TObject &d) const;
  virtual void   Allocate(Int_t nrow, Int_t ncol,Int_t ntime);
  virtual void   Reset();

  virtual Int_t  GetNrow() const               { return fNrow;       };
  virtual Int_t  GetNcol() const               { return fNcol;       };
  virtual Int_t  GetNtime() const              { return fNtime;      };
          Int_t  GetIndex(Int_t row, Int_t col, Int_t time) const;
          Int_t  GetIndexUnchecked(Int_t row, Int_t col, Int_t time) const
	    { return time * fNrow*fNcol + GetIdx1Unchecked(row,col); };
          Int_t  GetBufType() const            { return fBufType;    };
  virtual Int_t  GetNelems() const             { return fNelems;     };

 protected:

          Int_t  GetIdx1(Int_t row, Int_t col) const;
  inline  Int_t  GetIdx1Unchecked(Int_t row, Int_t col) const
                                               { return row + col * fNrow; };
  inline  Bool_t CheckBounds(const char *where, Int_t idx1, Int_t idx2);
  inline  Bool_t OutOfBoundsError(const char *where, Int_t idx1, Int_t idx2);
 
  Int_t          fNrow;            // Number of rows of the detector segement
  Int_t          fNcol;            // Number of columns of the detector segment
  Int_t          fNtime;           // Number of timebins of the detector segment

  Int_t          fNdim1;           // First dimension of the array (row * column)
  Int_t          fNdim2;           // Second dimension of the array (time, not compressed) 

  IlcDCHarrayI  *fIndex;           // Index position of column
  Int_t          fBufType;         // Type of the buffer - defines the compression algorithm  
  Int_t          fNelems;          // Total number of elements 
  Int_t          fCurrentIdx1;     // !Current index 1
  Int_t          fCurrentIdx2;     // !Current index 2
  Int_t          fCurrentIndex;    // !Current index in field
 
  ClassDef(IlcDCHdataArray,1)      // Data container for one DCH detector segment

};
 
#endif


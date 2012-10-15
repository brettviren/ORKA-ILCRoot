#ifndef ILCDCHDATAARRAYI_H
#define ILCDCHDATAARRAYI_H

/* Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcDCHdataArrayI.h,v */
 
#include   "IlcDCHdataArray.h"

/////////////////////////////////////////////////////////////
//                                                         //
//  General container for integer data from DCH detector   //
//  segments.                                              //
//  Adapted from IlcDigits, origin M.Ivanov                //
//                                                         //
/////////////////////////////////////////////////////////////

class IlcDCHdataArrayI : public IlcDCHdataArray {

 public:

  IlcDCHdataArrayI();
  IlcDCHdataArrayI(Int_t nrow, Int_t ncol, Int_t ntime);
  IlcDCHdataArrayI(const IlcDCHdataArrayI &a);
  virtual ~IlcDCHdataArrayI();
  IlcDCHdataArrayI &operator=(const IlcDCHdataArrayI &a);

  virtual void   Allocate(Int_t nrow, Int_t ncol, Int_t ntime);
  virtual void   Copy(TObject &a) const;
  virtual void   Compress(Int_t bufferType, Int_t threshold);
  virtual void   Compress(Int_t bufferType); 
  virtual void   Expand();
  virtual Bool_t First();
  virtual Bool_t Next(); 
  virtual void   Reset();

          void   SetData(Int_t row, Int_t col, Int_t time, Int_t value);
          void   SetDataUnchecked(Int_t row, Int_t col, Int_t time, Int_t value)
                                 { SetDataFast(GetIdx1Unchecked(row,col),time,value); };

  virtual void   SetThreshold(Int_t threshold) { fThreshold = threshold; };

  virtual Int_t  GetData(Int_t row, Int_t col, Int_t time) const;
          Int_t  GetDataUnchecked(Int_t row, Int_t col, Int_t time) const
                                 { return GetDataFast(GetIdx1Unchecked(row,col),time); };

  virtual Int_t  GetThreshold() const          { return fThreshold;  };

  virtual Int_t  GetSize() const;
  virtual Int_t  GetDataSize() const; 
  virtual Int_t  GetOverThreshold(Int_t threshold);  

 protected:

  void   SetDataFast(Int_t idx1, Int_t idx2, Int_t value);
  Int_t  GetDataFast(Int_t idx1, Int_t idx2) const;

  Int_t          GetData1(Int_t idx1, Int_t idx2) const; 
  void           Expand1(); 
  void           Compress1(); 
  void           Expand2();
  void           Compress2();
  Bool_t         First0();
  Bool_t         Next0(); 
  Bool_t         First1();
  Bool_t         Next1();
 
  IlcDCHarrayI  *fElements;        // Buffer of 4 bytes integers for the array content
  Int_t          fThreshold;       // Threshold for zero suppression
 
  ClassDef(IlcDCHdataArrayI,1)     // Container for integer data of one DCH detector segment

};
 
#endif



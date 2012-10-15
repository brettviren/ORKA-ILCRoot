#ifndef ILCDCHDATAARRAYF_H
#define ILCDCHDATAARRAYF_H

/* Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcDCHdataArrayF.h,v */

#include <TMath.h>
 
#include "IlcDCHdataArray.h"

/////////////////////////////////////////////////////////////
//                                                         //
//  General container for float data from DCH detector     //
//  segments.                                              //
//  Adapted from IlcDigits, origin M.Ivanov                //
//                                                         //
/////////////////////////////////////////////////////////////

class IlcDCHarrayF;

class IlcDCHdataArrayF : public IlcDCHdataArray {

 public:

  IlcDCHdataArrayF();
  IlcDCHdataArrayF(Int_t nrow, Int_t ncol,Int_t ntime);
  IlcDCHdataArrayF(const IlcDCHdataArrayF &a);
  virtual ~IlcDCHdataArrayF();
  IlcDCHdataArrayF &operator=(const IlcDCHdataArrayF &a);

  virtual void    Allocate(Int_t nrow, Int_t ncol,Int_t ntime);
  virtual void    Copy(TObject &a) const;
  virtual void    Compress(Int_t bufferType, Float_t threshold);
  virtual void    Compress(Int_t bufferType); 
  virtual void    Expand();
  virtual Bool_t  First();
  virtual Bool_t  Next(); 
  virtual void    Reset();

          void    SetData(Int_t row, Int_t col, Int_t time, Float_t value);
          void    SetDataUnchecked(Int_t row, Int_t col, Int_t time, Float_t value)
                                  { SetDataFast(GetIdx1Unchecked(row,col),time,value); };

  virtual void    SetThreshold(Float_t threshold) { fThreshold = threshold; };

  virtual Float_t GetData(Int_t row, Int_t col, Int_t time) const;
          Float_t GetDataUnchecked(Int_t row, Int_t col, Int_t time) const
                                  { return GetDataFast(GetIdx1Unchecked(row,col),time); };
                                                  
  virtual Float_t GetThreshold() const            { return fThreshold;  };

  virtual Int_t   GetSize() const;
  virtual Int_t   GetDataSize() const; 
  virtual Int_t   GetOverThreshold(Float_t threshold);  

 protected:

  void    SetDataFast(Int_t idx1, Int_t idx2, Float_t v);  
  Float_t GetDataFast(Int_t idx1, Int_t idx2) const;

  Float_t         GetData1(Int_t idx1, Int_t idx2) const; 
  void            Expand1(); 
  void            Compress1(); 
  void            Expand2();
  void            Compress2();
  Bool_t          First0();
  Bool_t          Next0(); 
  Bool_t          First1();
  Bool_t          Next1();

  IlcDCHarrayF  *fElements;        // Buffer of 4 bytes floats for the array content
  Float_t        fThreshold;       // Threshold for zero suppression
 
  ClassDef(IlcDCHdataArrayF,1)     // Container for float data of one DCH detector segment

};
 
#endif


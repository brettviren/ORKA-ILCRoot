#ifndef ILCDCHCALROC_H
#define ILCDCHCALROC_H
/* Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 * See cxx source for full Copyright notice                               */

//////////////////////////////////////////////////
//                                              //
//  DCH calibration base class for one ROC      //
//                                              //
//////////////////////////////////////////////////

#include <TObject.h>

//_____________________________________________________________________________
class IlcDCHCalROC : public TObject {

 public:

  IlcDCHCalROC();
  IlcDCHCalROC(Int_t p, Int_t c);
  IlcDCHCalROC(const IlcDCHCalROC &c);
  virtual           ~IlcDCHCalROC();
  IlcDCHCalROC      &operator=(const IlcDCHCalROC &c);
  virtual void       Copy(TObject &c) const;

  Int_t    GetNrows() const                  { return fNrows; };
  Int_t    GetNcols() const                  { return fNcols; };

  Int_t        GetChannel(Int_t c, Int_t r)     { return r+c*fNrows; };
  Int_t        GetNchannels()       const       { return fNchannels;   };

  Float_t GetValue(Int_t ich) const       { return (Float_t) fData[ich] / 10000; };
  Float_t GetValue(Int_t col, Int_t row)  { return GetValue(GetChannel(col,row)); };

  void         SetValue(Int_t ich, Float_t value) { fData[ich] = (UShort_t) (value * 10000); };
  void         SetValue(Int_t col, Int_t row, Float_t value) { SetValue(GetChannel(col,row), value); };

  void Scale(Float_t value);

 protected:

  Int_t     fPla;             //  Plane number
  Int_t     fCha;             //  Chamber number

  Int_t     fNrows;           //  Number of rows
  Int_t     fNcols;           //  Number of columns

  Int_t     fNchannels;             //  Number of channels
  UShort_t  *fData;                //[fNchannels] Data

  ClassDef(IlcDCHCalROC, 2)    //  DCH ROC calibration class

};

#endif

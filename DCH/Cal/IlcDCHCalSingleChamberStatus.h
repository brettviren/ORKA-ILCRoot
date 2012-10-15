#ifndef IlcDCHCalSingleChamberStatus_H
#define IlcDCHCalSingleChamberStatus_H
/* Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 * See cxx source for full Copyright notice                               */

//////////////////////////////////////////////////
//                                              //
//  DCH calibration base class containing status values for one ROC      //
//                                              //
//////////////////////////////////////////////////

#include <TObject.h>

//_____________________________________________________________________________
class IlcDCHCalSingleChamberStatus : public TObject {

 public:

  IlcDCHCalSingleChamberStatus();
  IlcDCHCalSingleChamberStatus(Int_t p, Int_t c, Int_t cols);
  IlcDCHCalSingleChamberStatus(const IlcDCHCalSingleChamberStatus &c);
  virtual           ~IlcDCHCalSingleChamberStatus();
  IlcDCHCalSingleChamberStatus      &operator=(const IlcDCHCalSingleChamberStatus &c);
  virtual void       Copy(TObject &c) const;

  Int_t    GetNrows() const                  { return fNrows; };
  Int_t    GetNcols() const                  { return fNcols; };

  Int_t        GetChannel(Int_t c, Int_t r) const    { return r+c*fNrows; };
  Int_t        GetNchannels()       const       { return fNchannels;   };
  Char_t       GetStatus(Int_t ich) const       { return fData[ich]; };
  Char_t       GetStatus(Int_t col, Int_t row) const { return fData[GetChannel(col,row)]; };

  void         SetStatus(Int_t ich, Char_t vd) { fData[ich] = vd;   };
  void         SetStatus(Int_t col, Int_t row, Char_t vd)
                                                { fData[GetChannel(col,row)] = vd; };

 protected:

  Int_t     fPla;             //  Plane number
  Int_t     fCha;             //  Chamber number

  Int_t     fNrows;           //  Number of rows
  Int_t     fNcols;           //  Number of columns

  Int_t     fNchannels;             //  Number of channels
  Char_t   *fData;                //[fNchannels] Data

  ClassDef(IlcDCHCalSingleChamberStatus,1)    //  DCH ROC calibration class

};

#endif

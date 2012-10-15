#ifndef ILCDCHCALDET_H
#define ILCDCHCALDET_H
/* Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcDCHCalDet.h,v 1.1.1.1 2008/03/11 14:53:01 vitomeg Exp $ */

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//  DCH calibration class for parameters which are saved per detector        //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#include "TNamed.h"
#include "IlcDCHgeometry.h"

class IlcDCHCalDet : public TNamed {

 public:
 
  enum { kNplan = 6, kNcham = 5, kNsect = 18, kNdet = 540 };

  IlcDCHCalDet();
  IlcDCHCalDet(const Text_t* name, const Text_t* title);
  IlcDCHCalDet(const IlcDCHCalDet &c);   
  virtual ~IlcDCHCalDet();
  IlcDCHCalDet &operator=(const IlcDCHCalDet &c);

  virtual void     Copy(TObject &c) const;

  Float_t GetValue(Int_t d) const { return fData[d]; };
  Float_t GetValue(Int_t p, Int_t c, Int_t s) const { return fData[IlcDCHgeometry::GetDetector(p,c,s)]; };

  void SetValue(Int_t d, Float_t value) { fData[d] = value; };
  void SetValue(Int_t p, Int_t c, Int_t s, Float_t value) { fData[IlcDCHgeometry::GetDetector(p,c,s)] = value; };
  
  protected:

  Float_t  fData[kNdet];                          //[kNdet] Data

  ClassDef(IlcDCHCalDet,1)                      //  DCH calibration class for parameters which are saved per detector

};

#endif

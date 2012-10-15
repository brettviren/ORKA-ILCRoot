#ifndef ILCDCHCALPAD_H
#define ILCDCHCALPAD_H
/* Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcDCHCalPad.h,v 1.1.1.1 2008/03/11 14:53:01 vitomeg Exp $ */

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//  DCH calibration class for parameters which are saved per pad                 //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#include "TNamed.h"

class IlcDCHCalROC;
class IlcDCHCalDet;

class IlcDCHCalPad : public TNamed {

 public:
 
  enum { kNplan = 6, kNcham = 5, kNsect = 18, kNdet = 540 };

  IlcDCHCalPad();
  IlcDCHCalPad(const Text_t* name, const Text_t* title);
  IlcDCHCalPad(const IlcDCHCalPad &c);   
  virtual ~IlcDCHCalPad();
  IlcDCHCalPad &operator=(const IlcDCHCalPad &c);

  virtual void     Copy(TObject &c) const;

  static inline Int_t               GetDet(Int_t p, Int_t c, Int_t s) { return p+c*kNplan+s*kNplan*kNcham; };

  IlcDCHCalROC *GetCalROC(Int_t d) const { return fROC[d]; };
  IlcDCHCalROC *GetCalROC(Int_t p, Int_t c, Int_t s) const
                                               { return fROC[GetDet(p,c,s)]; };
  
  void ScaleROCs(IlcDCHCalDet* values);

 protected:

  IlcDCHCalROC *fROC[kNdet];                    //  Array of ROC objects which contain the values per pad

  ClassDef(IlcDCHCalPad,1)                      //  DCH calibration class for parameters which are saved per pad

};

#endif

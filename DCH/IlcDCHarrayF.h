#ifndef ILCDCHARRAYF_H
#define ILCDCHARRAYF_H
/* Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcDCHarrayF.h,v */

#include <TObject.h>
#include <TArrayF.h>
 
/////////////////////////////////////////////////////////////
//                                                         //
//  Array of floats                                        //
//  Origin M.Ivanov                                        //
//                                                         //
/////////////////////////////////////////////////////////////                   

class IlcDCHarrayF: public TObject, public TArrayF {

public:

  IlcDCHarrayF();
  virtual ~IlcDCHarrayF();
  void Copy(TObject &a) const;
  void Expand(Int_t n);  

  ClassDef(IlcDCHarrayF,1)  // An array of floats

};

#endif 


#ifndef ILCDCHARRAYI_H
#define ILCDCHARRAYI_H
/* Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcDCHarrayI.h,v */

#include <TObject.h>
#include <TArrayI.h>
 
/////////////////////////////////////////////////////////////
//                                                         //
//  Array of integers                                      //
//  Origin M.Ivanov                                        //
//                                                         //
/////////////////////////////////////////////////////////////                   

class IlcDCHarrayI: public TObject, public TArrayI {

public:

  IlcDCHarrayI();
  virtual ~IlcDCHarrayI();
  void Copy(TObject &a) const;
  void Expand(Int_t n);  

  ClassDef(IlcDCHarrayI,1)  // An array of integers

};

#endif 


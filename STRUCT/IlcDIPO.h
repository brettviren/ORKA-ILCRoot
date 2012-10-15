#ifndef ILCDIPO_H
#define ILCDIPO_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcDIPO.h 1279 2000-06-11 12:38:40Z morsch $ */

////////////////////////////////////////////////
//  Manager class for Module: DIPO          //
////////////////////////////////////////////////
 
#include "IlcModule.h"
 
 
class IlcDIPO : public IlcModule {
 
public:
  IlcDIPO();
  IlcDIPO(const char *name, const char *title);
  virtual      ~IlcDIPO() {}
  virtual void  Init();
  
  ClassDef(IlcDIPO,1)  //Class for the dipole magnet
};

#endif

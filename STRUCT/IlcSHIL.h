#ifndef ILCSHIL_H
#define ILCSHIL_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcSHIL.h 47909 2011-02-28 15:05:53Z fca $ */

////////////////////////////////////////////////
//  Manager class for Module: SHIL          //
////////////////////////////////////////////////
 
#include "IlcModule.h"
 
 
class IlcSHIL : public IlcModule {
  
public:
  IlcSHIL();
  IlcSHIL(const char *name, const char *title);
  virtual      ~IlcSHIL() {}
  virtual void  CreateGeometry();
  virtual void  CreateMaterials();
  virtual void  Init();
  virtual Int_t IsVersion() const {return 0;}
 
  ClassDef(IlcSHIL,1)  // Muon Shield Class
};

#endif

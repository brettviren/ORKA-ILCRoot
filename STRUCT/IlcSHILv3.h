#ifndef ILCSHILV3_H
#define ILCSHILV3_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id $ */

////////////////////////////////////////////////
//  Manager class for Module: SHIL          //
////////////////////////////////////////////////
 
#include "IlcSHIL.h"
class TGeoPcon;

 
class IlcSHILv3 : public IlcSHIL {
  
public:
  IlcSHILv3();
  IlcSHILv3(const char *name, const char *title);
  virtual      ~IlcSHILv3() {}
  virtual void  CreateGeometry();
  virtual void  Init();
 private:
  virtual void InvertPcon(TGeoPcon* pcon);
  virtual TGeoPcon* MakeShapeFromTemplate(const TGeoPcon* pcon, Float_t drIn, Float_t drOut);  
 protected:
  ClassDef(IlcSHILv3,1)  // Small angle absorber as built
};

#endif

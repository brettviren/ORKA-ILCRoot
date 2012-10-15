#ifndef ILCABSOV3_H
#define ILCABSOV3_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id $ */

////////////////////////////////////////////////
//  Manager class for Module: ABSO          //
////////////////////////////////////////////////
 
#include "IlcABSO.h"
class TGeoPcon;

 
class IlcABSOv3: public IlcABSO {
  
public:
  IlcABSOv3();
  IlcABSOv3(const char *name, const char *title);
  virtual      ~IlcABSOv3() {}
  virtual void  Init(){;}
  virtual void  CreateGeometry();
 private:
  virtual TGeoPcon* MakeShapeFromTemplate(const TGeoPcon* pcon, Float_t drIn, Float_t drOut);  
  ClassDef(IlcABSOv3,1)  // Front Absorber as built
};

#endif

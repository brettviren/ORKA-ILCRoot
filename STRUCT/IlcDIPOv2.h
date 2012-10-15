#ifndef ILCDIPOV2_H
#define ILCDIPOV2_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcDIPOv2.h 47909 2011-02-28 15:05:53Z fca $ */

/////////////////////////////////////////////////
//  Manager class for detector: DIPO version 2 //
/////////////////////////////////////////////////
 
#include "IlcDIPO.h"
  
class IlcDIPOv2 : public IlcDIPO {
  
public:
  IlcDIPOv2();
  IlcDIPOv2(const char *name, const char *title);
  virtual      ~IlcDIPOv2() {}
  virtual void  CreateGeometry();
  virtual void  CreateMaterials();
  virtual Int_t IsVersion() const {return 2;}
 private:
  virtual void  CreateSpectrometerDipole();
  virtual void  CreateCompensatorDipole();
  
  ClassDef(IlcDIPOv2,1)  //Class manager for magnetic dipole version 2
};

#endif

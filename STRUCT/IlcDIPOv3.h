#ifndef ILCDIPOV3_H
#define ILCDIPOV3_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcDIPOv3.h 18166 2007-04-26 16:36:32Z morsch $ */

/////////////////////////////////////////////////
//  Manager class for detector: DIPO version 3 //
/////////////////////////////////////////////////
 
#include "IlcDIPOv2.h"
  
class IlcDIPOv3 : public IlcDIPOv2 {
  
public:
  IlcDIPOv3();
  IlcDIPOv3(const char *name, const char *title);
  virtual      ~IlcDIPOv3() {}
  virtual Int_t IsVersion() const {return 3;}
 private:
  virtual void  CreateSpectrometerDipole();
  ClassDef(IlcDIPOv3,1)  //Class manager for magnetic dipole version 2
};

#endif

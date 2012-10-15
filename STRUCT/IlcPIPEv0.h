#ifndef ILCPIPEV0_H
#define ILCPIPEV0_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcPIPEv0.h 18237 2007-04-30 12:24:59Z morsch $ */

/////////////////////////////////////////////////////////
//  Manager and class for detector: PIPE  version 0    //
/////////////////////////////////////////////////////////
 
#include "IlcPIPE.h"

class IlcPIPEv0 : public IlcPIPE {
  
 public:
    enum constants {kC=6, kAlu=9, kInox=19, kGetter=20, kBe=5, kVac=16,
	  kAir=15, kAlBe=21, kPA = 22, kAirHigh = 35};
	
  IlcPIPEv0();
  IlcPIPEv0(const char *name, const char *title);
  virtual       ~IlcPIPEv0() {}
  virtual void   CreateGeometry();
  virtual void   CreateMaterials();
  virtual void   SetPipeMaterial(Int_t mat = kBe) {fPipeMaterial = mat;}
  virtual Int_t  IsVersion() const {return 0;}
 protected:
  Int_t   fPipeMaterial; // Pipe material (Al, Be, or Inox)
  
  ClassDef(IlcPIPEv0,2)  //Class for PIPE version 0
};
 
#endif

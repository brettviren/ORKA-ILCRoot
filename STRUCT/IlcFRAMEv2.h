#ifndef ILCFRAMEV2_H
#define ILCFRAMEV2_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcFRAMEv2.h 29876 2008-11-12 15:47:19Z fca $ */

/////////////////////////////////////////////////////////
//  Manager and class for detector: FRAME  version 2    //
/////////////////////////////////////////////////////////
 
#include "IlcFRAME.h"

class IlcFRAMEv2 : public IlcFRAME {
  
public:
  IlcFRAMEv2();
  IlcFRAMEv2(const char *name, const char *title);
  virtual       ~IlcFRAMEv2() {}
  virtual void   CreateGeometry();
  virtual void   CreateMaterials();
  virtual void   AddAlignableVolumes() const;
  virtual void   Init();
  virtual void   StepManager();
  virtual Int_t  IsVersion() const;
  virtual void   SetHoles(Int_t flag=0) {fHoles = flag;}
  virtual Int_t  Holes() const {return fHoles;}
  virtual void   MakeHeatScreen(const char* name, Float_t dyP, Int_t rot1, Int_t rot2);
  virtual void   WebFrame(const char* name, Float_t dHz, Float_t theta0, Float_t phi0);
 private:
  Int_t  fHoles; // flag fHoles=0 => no holes, with holes otherwise
  
   ClassDef(IlcFRAMEv2,2)  //Class for FRAME version 2
};
 
#endif

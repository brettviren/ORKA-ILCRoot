#ifndef ILCPIPEV3_H
#define ILCPIPEV3_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcPIPEv3.h 56417 2012-05-11 08:46:54Z hristov $*/

/////////////////////////////////////////////////////////
// ILC beam pipe geometry                            //
// This version uses TGeo.                             //
// Author:                                             //
// Andreas Morsch                                      //
// e-mail: andreas.morsch@cern.ch                      // 
/////////////////////////////////////////////////////////
 
#include "IlcPIPE.h"
class TGeoPcon;
class TGeoVolume;


class IlcPIPEv3 : public IlcPIPE {
    
 public:
    enum constants {kC=6, kAlu=9, kInox=19, kGetter=20, kBe=5, kVac=16,
	  kAir=15, kAlBe=21, kPA = 22};
	
  IlcPIPEv3();
  IlcPIPEv3(const char *name, const char *title);
  virtual       ~IlcPIPEv3() {}
  virtual void   CreateGeometry();
  virtual void   CreateMaterials();
  virtual Int_t  IsVersion() const {return 0;}
  virtual void   SetBeamBackgroundSimulation() {fBeamBackground = kTRUE;}
  virtual void   AddAlignableVolumes() const;
	  
 private:
  virtual TGeoPcon*   MakeMotherFromTemplate(const TGeoPcon* shape, Int_t imin = -1, Int_t imax = -1, Float_t r0 = 0., Int_t nz =-1);
  virtual TGeoPcon*   MakeInsulationFromTemplate(TGeoPcon* shape);
  virtual TGeoVolume* MakeBellow(const char* ext, Int_t nc, Float_t rMin, Float_t rMax, Float_t dU, Float_t rPlie, Float_t dPlie);
  Bool_t  fBeamBackground; // Flag for beam background simulations
  
  ClassDef(IlcPIPEv3, 2)  //Class for PIPE version using TGeo
};
 
#endif

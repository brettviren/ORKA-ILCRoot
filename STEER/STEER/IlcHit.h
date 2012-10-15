#ifndef ILCHIT_H
#define ILCHIT_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcHit.h 50615 2011-07-16 23:19:19Z hristov $ */

// Base class for hits
// This class is used as a base class for 
// hits in the different detectors

#include "TObject.h"

class IlcHit : public TObject {
public:
  IlcHit();
  IlcHit(Int_t shunt, Int_t track);
  virtual ~IlcHit() {}
  virtual Int_t GetTrack() const {return fTrack;}
  virtual void SetTrack(Int_t track) {fTrack=track;}
  virtual Float_t X() const {return fX;}
  virtual Float_t Y() const {return fY;}
  virtual Float_t Z() const {return fZ;}
  virtual Int_t Track() const {return fTrack;}
  
protected:
  Int_t     fTrack;  // Track number
  Float_t   fX;      // X position of the hit
  Float_t   fY;      // Y position of the hit
  Float_t   fZ;      // Z position of the hit

  ClassDef(IlcHit,1)  //Base class for all Ilc hits
};
#endif

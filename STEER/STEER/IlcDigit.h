#ifndef ILCDIGIT_H
#define ILCDIGIT_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcDigit.h 50615 2011-07-16 23:19:19Z hristov $ */

////////////////////////////////////////////////
//  Base class for Ilc Digits               //
////////////////////////////////////////////////

#include <assert.h>
#include "TObject.h"

class IlcDigit : public TObject {
public:
  IlcDigit();
  IlcDigit(Int_t *track);
  virtual ~IlcDigit() {}
  virtual Int_t *GetTracks() {return fTracks;}
  virtual Int_t GetTrack(Int_t i) const 
  {assert(0<=i&&i<=2); return fTracks[i];}
  
protected:
  Int_t     fTracks[3];   //tracks number making this digit (up to 3)

  ClassDef(IlcDigit,1)  //Base class for all Ilc digits
};
#endif

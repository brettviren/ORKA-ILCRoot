#ifndef ILCPVBARRAWFITTERV3_H
#define ILCPVBARRAWFITTERV3_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                          */

/* $Id: $ */

// This class extracts the PVBAR "digits" of current event
// (amplitude,time, position,gain) from the raw stream 
// provided by IlcRawReader. 
// Uses FastFitting procedure to evaluate time and estimate sample quality

#include "IlcPVBARRawFitterv0.h"

class IlcPVBARRawFitterv3 : public IlcPVBARRawFitterv0 {

public:

  IlcPVBARRawFitterv3();
  IlcPVBARRawFitterv3(const IlcPVBARRawFitterv3& rawFitter);
  IlcPVBARRawFitterv3& operator = (const IlcPVBARRawFitterv3& rawFitter);
  virtual ~IlcPVBARRawFitterv3();

  virtual Bool_t Eval(const UShort_t *signal, Int_t sigStart, Int_t sigLength);

private:
  
  ClassDef(IlcPVBARRawFitterv3,1)
};

#endif

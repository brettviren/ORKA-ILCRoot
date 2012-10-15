#ifndef ILCRSTACKRAWFITTERV3_H
#define ILCRSTACKRAWFITTERV3_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                          */

/* $Id: $ */

// This class extracts the RSTACK "digits" of current event
// (amplitude,time, position,gain) from the raw stream 
// provided by IlcRawReader. 
// Uses FastFitting procedure to evaluate time and estimate sample quality

#include "IlcRSTACKRawFitterv0.h"

class IlcRSTACKRawFitterv3 : public IlcRSTACKRawFitterv0 {

public:

  IlcRSTACKRawFitterv3();
  IlcRSTACKRawFitterv3(const IlcRSTACKRawFitterv3& rawFitter);
  IlcRSTACKRawFitterv3& operator = (const IlcRSTACKRawFitterv3& rawFitter);
  virtual ~IlcRSTACKRawFitterv3();

  virtual Bool_t Eval(const UShort_t *signal, Int_t sigStart, Int_t sigLength);

private:
  
  ClassDef(IlcRSTACKRawFitterv3,1)
};

#endif

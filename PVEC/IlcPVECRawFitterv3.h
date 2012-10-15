#ifndef ILCPVECRAWFITTERV3_H
#define ILCPVECRAWFITTERV3_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                          */

/* $Id: $ */

// This class extracts the PVEC "digits" of current event
// (amplitude,time, position,gain) from the raw stream 
// provided by IlcRawReader. 
// Uses FastFitting procedure to evaluate time and estimate sample quality

#include "IlcPVECRawFitterv0.h"

class IlcPVECRawFitterv3 : public IlcPVECRawFitterv0 {

public:

  IlcPVECRawFitterv3();
  IlcPVECRawFitterv3(const IlcPVECRawFitterv3& rawFitter);
  IlcPVECRawFitterv3& operator = (const IlcPVECRawFitterv3& rawFitter);
  virtual ~IlcPVECRawFitterv3();

  virtual Bool_t Eval(const UShort_t *signal, Int_t sigStart, Int_t sigLength);

private:
  
  ClassDef(IlcPVECRawFitterv3,1)
};

#endif

#ifndef ILCPVBARRAWFITTERV2_H
#define ILCPVBARRAWFITTERV2_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                          */

/* $Id: $ */

// This class extracts amplitude, t0 and quality of the PVBAR "samples" 
// ising FastFit and two-exponent parameterization

#include "IlcPVBARRawFitterv0.h"
class TArrayD ;

class IlcPVBARRawFitterv2 : public IlcPVBARRawFitterv0 {

public:

  IlcPVBARRawFitterv2();
  IlcPVBARRawFitterv2(const IlcPVBARRawFitterv2& rawFitter);
  IlcPVBARRawFitterv2& operator = (const IlcPVBARRawFitterv2& rawFitter);
  virtual ~IlcPVBARRawFitterv2();

  virtual Bool_t Eval(const UShort_t *signal, Int_t sigStart, Int_t sigLength);
  void SetRawParams(Double_t alpha, Double_t beta){fAlpha=alpha; fBeta=beta;}

private: 
  Bool_t FindAmpT(TArrayD samples, TArrayD times) ;
  void FindMax() ;

private:
  Double_t fAlpha ; //Parameter of sample shape
  Double_t fBeta ;  //Parameter of sample shape
  Double_t fMax ;   //Maximum of parameterization
  
  ClassDef(IlcPVBARRawFitterv2,2)
};

#endif

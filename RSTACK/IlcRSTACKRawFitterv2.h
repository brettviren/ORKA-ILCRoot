#ifndef ILCRSTACKRAWFITTERV2_H
#define ILCRSTACKRAWFITTERV2_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                          */

/* $Id: $ */

// This class extracts amplitude, t0 and quality of the RSTACK "samples" 
// ising FastFit and two-exponent parameterization

#include "IlcRSTACKRawFitterv0.h"
class TArrayD ;

class IlcRSTACKRawFitterv2 : public IlcRSTACKRawFitterv0 {

public:

  IlcRSTACKRawFitterv2();
  IlcRSTACKRawFitterv2(const IlcRSTACKRawFitterv2& rawFitter);
  IlcRSTACKRawFitterv2& operator = (const IlcRSTACKRawFitterv2& rawFitter);
  virtual ~IlcRSTACKRawFitterv2();

  virtual Bool_t Eval(const UShort_t *signal, Int_t sigStart, Int_t sigLength);
  void SetRawParams(Double_t alpha, Double_t beta){fAlpha=alpha; fBeta=beta;}

private: 
  Bool_t FindAmpT(TArrayD samples, TArrayD times) ;
  void FindMax() ;

private:
  Double_t fAlpha ; //Parameter of sample shape
  Double_t fBeta ;  //Parameter of sample shape
  Double_t fMax ;   //Maximum of parameterization
  
  ClassDef(IlcRSTACKRawFitterv2,2)
};

#endif

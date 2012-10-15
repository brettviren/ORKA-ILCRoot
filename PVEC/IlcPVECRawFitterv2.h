#ifndef ILCPVECRAWFITTERV2_H
#define ILCPVECRAWFITTERV2_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                          */

/* $Id: $ */

// This class extracts amplitude, t0 and quality of the PVEC "samples" 
// ising FastFit and two-exponent parameterization

#include "IlcPVECRawFitterv0.h"
class TArrayD ;

class IlcPVECRawFitterv2 : public IlcPVECRawFitterv0 {

public:

  IlcPVECRawFitterv2();
  IlcPVECRawFitterv2(const IlcPVECRawFitterv2& rawFitter);
  IlcPVECRawFitterv2& operator = (const IlcPVECRawFitterv2& rawFitter);
  virtual ~IlcPVECRawFitterv2();

  virtual Bool_t Eval(const UShort_t *signal, Int_t sigStart, Int_t sigLength);
  void SetRawParams(Double_t alpha, Double_t beta){fAlpha=alpha; fBeta=beta;}

private: 
  Bool_t FindAmpT(TArrayD samples, TArrayD times) ;
  void FindMax() ;

private:
  Double_t fAlpha ; //Parameter of sample shape
  Double_t fBeta ;  //Parameter of sample shape
  Double_t fMax ;   //Maximum of parameterization
  
  ClassDef(IlcPVECRawFitterv2,2)
};

#endif

#ifndef ILCPVECRAWFITTERV4_H
#define ILCPVECRAWFITTERV4_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                          */

/* $Id: $ */

// This class extracts the signal parameters (energy, time, quality)
// from ALTRO samples. Energy is in ADC counts, time is in time bin units.
// A coarse algorithm is applied.

class TArrayI;
class TArrayD;
class IlcPVECCalibData;
#include "IlcPVECRawFitterv1.h"

class IlcPVECRawFitterv4 : public IlcPVECRawFitterv1 
{

public:

  IlcPVECRawFitterv4();
  virtual ~IlcPVECRawFitterv4();

  virtual Bool_t Eval(const UShort_t *signal, Int_t sigStart, Int_t sigLength);
  
  //Switch on/off fitting of HighGain samples. By default off
  void FitHighGain(Bool_t on=kTRUE){fFitHighGain=on;}

protected:   
  static void UnfoldingChiSquare(Int_t & nPar, Double_t * Grad, Double_t & fret, Double_t * x, Int_t iflag)  ;
                                            // Chi^2 of the fit. Should be static to be passed to MINUIT
  Bool_t EvalWithFitting(TArrayI*samples, TArrayI * times);

  Bool_t fFitHighGain ; //Switch on/off fitting of the HG channel

 private:  
  IlcPVECRawFitterv4(const IlcPVECRawFitterv4& rawFitterv4);
  IlcPVECRawFitterv4& operator=(const IlcPVECRawFitterv4& rawFitterv4);

  ClassDef(IlcPVECRawFitterv4,1)
};

#endif

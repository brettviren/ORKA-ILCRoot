#ifndef ILCGAUSCORR_H
#define ILCGAUSCORR_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcGausCorr.h 50615 2011-07-16 23:19:19Z hristov $ */

////////////////////////////////////////////////////////////////////////
// Class used to generate correlated gaussian numbers with mean
// zero and known covariance matrix.
// M.Masera 15.03.2001 9:30 - modified on 26.02.2002 17:40
////////////////////////////////////////////////////////////////////////

#include <TMatrixD.h>
class TArrayD;


class IlcGausCorr : public TObject 
{
 public:
  //
  IlcGausCorr();
  IlcGausCorr(const TMatrixD & cov, Int_t size);
  IlcGausCorr(const IlcGausCorr & tgcorr);
  virtual ~IlcGausCorr();
  void GetGaussN(TArrayD &vec) const;
  TMatrixD GetSqrtMatrix() const { return *fCv;}
  void PrintCv() const;
  IlcGausCorr & operator=(const IlcGausCorr & tgcorr);
  //
 private:
  //
  Int_t fSize;   // number of correlated gaussian random numbers
  TMatrixD *fCv; // 'square root' of the covariance matrix

  ClassDef(IlcGausCorr,1)  
};


#endif




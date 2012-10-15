#ifndef ILCTRACKRESIDUALSFAST_H
#define ILCTRACKRESIDUALSFAST_H

//************************************************************************
// IlcTrackResidualsFast: derived class (from IlcTrackResiduals) which   *
// implements a simple linear minimization of the track residuals chi2.  *
// The minimization relies on the fact that the alignment parameters     *
// (angles and translations) are small.                                  *
//                                                                       *
//                                                                       *
//************************************************************************

#include "TMatrixDSym.h"
#include "TMatrixD.h"

#include "IlcAlignObj.h"
#include "IlcTrackResiduals.h"

class IlcTrackResidualsFast : public IlcTrackResiduals {

 public:
  IlcTrackResidualsFast();
  IlcTrackResidualsFast(Int_t ntracks);
  IlcTrackResidualsFast(const IlcTrackResidualsFast &res);
  IlcTrackResidualsFast& operator= (const IlcTrackResidualsFast& res);
  virtual ~IlcTrackResidualsFast() { }

  Bool_t Minimize();

 protected:

  void   AddPoints(IlcTrackPoint &p, IlcTrackPoint &pprime);
  Bool_t Update();

  Double_t fSum[27]; // Sums used during the chi2 minimization
  Double_t fSumR;    // Sum of r squared

  ClassDef(IlcTrackResidualsFast,1)

};

#endif

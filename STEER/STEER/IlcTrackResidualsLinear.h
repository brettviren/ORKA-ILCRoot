#ifndef ILCTRACKRESIDUALSLINEAR_H
#define ILCTRACKRESIDUALSLINEAR_H

//************************************************************************
// IlcTrackResidualsLinear: derived class (from IlcTrackResiduals) which   *
// implements a simple linear minimization of the track residuals chi2.  *
// The minimization relies on the fact that the alignment parameters     *
// (angles and translations) are small.                                  *
//                                                                       *
//                                                                       *
//************************************************************************

#include "IlcAlignObj.h"
#include "IlcTrackResiduals.h"
class TLinearFitter;

class IlcTrackResidualsLinear : public IlcTrackResiduals {

 public:
  IlcTrackResidualsLinear();
  IlcTrackResidualsLinear(Int_t ntracks);
  IlcTrackResidualsLinear(const IlcTrackResidualsLinear &res);
  IlcTrackResidualsLinear& operator= (const IlcTrackResidualsLinear& res);
  virtual ~IlcTrackResidualsLinear();
  Bool_t Minimize();
  void   SetRobust(Float_t fraction){fFraction=fraction;}
  const Double_t * GetParameters() const { return fParams;}
  const Double_t * GetCovariance() const { return fCovar;}
 protected:
  Bool_t Update();
  void   AddPoints(IlcTrackPoint &p, IlcTrackPoint &pprime);
  TLinearFitter *fFitter;           // !Linear fitter
  Float_t fFraction;                // fraction of points for robust fitter if less 0 - no robust fitter invoked
  Double_t fParams[6];               // resulting parameters 
  Double_t fCovar[36];               // covariance matrix 
  Double_t fChi2Orig;               // original chi2 
  ClassDef(IlcTrackResidualsLinear,1)

};

#endif

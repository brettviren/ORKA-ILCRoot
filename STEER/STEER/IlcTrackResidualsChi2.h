#ifndef ILCTRACKRESIDUALSCHI2_H
#define ILCTRACKRESIDUALSCHI2_H

//************************************************************************
// IlcTrackResidualsChi2: derived class (from IlcTrackResiduals) which   *
// implements a MINUIT minimization of the track residuals chi2.         *
//                                                                       *
//                                                                       *
//************************************************************************

#include "IlcAlignObj.h"
#include "IlcTrackResiduals.h"

class IlcTrackResidualsChi2 : public IlcTrackResiduals {

 public:
  IlcTrackResidualsChi2():IlcTrackResiduals() { }
  IlcTrackResidualsChi2(Int_t ntracks):IlcTrackResiduals(ntracks) { }
  IlcTrackResidualsChi2(const IlcTrackResidualsChi2 &res):IlcTrackResiduals(res) { }
  IlcTrackResidualsChi2& operator= (const IlcTrackResidualsChi2& res) { ((IlcTrackResiduals *)this)->operator=(res); return *this; }
  virtual ~IlcTrackResidualsChi2() { }

  Bool_t Minimize();

  void   Chi2(Int_t & /* npar */, Double_t * /* gin */, Double_t &f, Double_t *par, Int_t /* iflag */);

 protected:

  ClassDef(IlcTrackResidualsChi2,1)

};

#endif

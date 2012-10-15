#ifndef ILCTRACKFITTERKALMAN_H
#define ILCTRACKFITTERKALMAN_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

///////////////////////////////////////////////////////////////////////////////
//
//                        Kalman-Filter-like fit 
//   to a straight-line crossing a set of arbitrarily oriented planes.
//           (See IlcTrackFitterKalman.cxx for the details)
//
//          Origin: Iouri Belikov, CERN, Jouri.Belikov@cern.ch
//
//////////////////////////////////////////////////////////////////////////////

#include "IlcTrackFitter.h"

class IlcTrackFitterKalman : public IlcTrackFitter {
public:
  IlcTrackFitterKalman() : 
     IlcTrackFitter(), 
     fMaxChi2(fgkMaxChi2) {}

  IlcTrackFitterKalman(IlcTrackPointArray *array, Bool_t owner = kTRUE);
  virtual ~IlcTrackFitterKalman() {}

  void SetMaxChi2(Double_t chi2) {fMaxChi2=chi2;}

  void   SetSeed(const Double_t par[6], const Double_t cov[15]);
  Bool_t MakeSeed(const IlcTrackPoint *p, const IlcTrackPoint *p2);

  Bool_t GetPCA(const IlcTrackPoint &p, IlcTrackPoint &p2) const;

  Bool_t Begin(Int_t first, Int_t last);
  Bool_t AddPoint(const IlcTrackPoint *p);
  Bool_t Update() {return kTRUE;}

private:
  IlcTrackFitterKalman(const IlcTrackFitterKalman &kalman);
  IlcTrackFitterKalman &operator=(const IlcTrackFitterKalman& kalman);

  Bool_t Propagate(const IlcTrackPoint *p);
  Double_t GetPredictedChi2(const IlcTrackPoint *p) const;
  Bool_t Update(const IlcTrackPoint *p,Double_t chi2);


  static const Double_t fgkMaxChi2;  // Default maximal allowed chi2 

  Double_t fMaxChi2;                 // A point is added if chi2 < fMaxChi2 

  ClassDef(IlcTrackFitterKalman,3)   // Kalman-Filter fit to a straight line

};

#endif

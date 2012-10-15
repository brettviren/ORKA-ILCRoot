#ifndef ILCDAJETFINDER_H
#define ILCDAJETFINDER_H

/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id$ */

//---------------------------------------------------------------------
//Jet finder based on Deterministic Annealing
//Author: Davide Perrino (davide.perrino@ba.infn.it)
//        magali.estienne@subatech.in2p3.fr &
//        alexandre.shabetai@cern.ch (Modification of the input object (reader/finder splitting))
// ** 2011
// Modified accordingly to reader/finder splitting and new handling of neutral information
//---------------------------------------------------------------------

#include "IlcJetFinder.h"

#include <TMatrixD.h>
#include <TVectorD.h>

class IlcDAJetFinder : public IlcJetFinder
{
 public:
  IlcDAJetFinder();
  virtual  ~IlcDAJetFinder();

  void     FindJets      ();

 protected:
  Double_t fAlpha;	     // beta increment
  Double_t fDelta;	     // perturbation proportional to Delta
  Double_t fAvDist;	     // minimum distance to distinguish two clusters
  Double_t fEps;	     // convergence criterium below max number of loops
  Double_t fEpsMax;	     // convergence criterium above max number of loops
  Int_t    fNloopMax;	     // maximum number of loops at a fixed beta
  Double_t fBeta;	     // increasing multiplier of entropy
  Int_t    fNclustMax;	     // maximum number of clusters to find
  Int_t    fNin;	     // number of input data
  Int_t    fNeff;	     // total input data, including fakes

 private:
  void     InitDetAnn    (Double_t &dEtSum, Double_t **xData, TVectorD *px, TVectorD *py, TMatrixD *pyx, TMatrixD *y);
  void     Annealing     (Int_t nk, Double_t **xData, const TVectorD *vPx, TVectorD *vPy, TMatrixD *mPyx, TMatrixD *mY);
  void     NumCl         (Int_t &nc, Int_t &nk, TVectorD *vPy, TMatrixD *mPyx, TMatrixD *mY);
  void     ReduceClusters(Int_t **iSame, Int_t nc, Int_t &ncout, Int_t **cont, Int_t *nSameOut) const;
  void     DoubleClusters(Int_t nc, Int_t &nk, TVectorD *vPy, TMatrixD *mY) const;
  void     EndDetAnn     (Int_t &nk, Double_t **xData, Int_t *xx, Double_t etx, const TVectorD *px, TVectorD *py, TMatrixD *pyx, TMatrixD *y);
  void     StoreJets     (Int_t nk, Double_t **xData, const Int_t *xx, const TMatrixD *mY);

  ClassDef(IlcDAJetFinder,4) // DA Jet finder

};

#endif

#ifndef ILCANALYSISTASKKMEANS_H
#define ILCANALYSISTASKKMEANS_H
//* 2012-2013 Infrastructure for Large Collider Experiment.       *
//* All rights reserved.                                                   *
//  * See cxx source for full Copyright notice                               */

/* $Id$ */

//-------------------------------------------------------------------------
//     Analysis Task that uses the Soft K-Means Algorithm to find clusters in
//     the eta-phi space of Minimum Bias. No pt information is used for the clustering.
//     
//
//     Author: Andreas Morsch (CERN)
//     andreas.morsch@cern.ch
//-------------------------------------------------------------------------

class TH1F;
class TH2F;
class TList;
class TProfile;
class IlcESDEvent;
class IlcESDtrack;
class IlcESDtrackCuts;
class IlcKMeansResult;

#include "IlcAnalysisTaskSE.h"

class IlcAnalysisTaskKMeans : public IlcAnalysisTaskSE {
 public:
  IlcAnalysisTaskKMeans();
  IlcAnalysisTaskKMeans(const char *name);
  IlcAnalysisTaskKMeans(const IlcAnalysisTaskKMeans &res);
  IlcAnalysisTaskKMeans& operator=(const IlcAnalysisTaskKMeans& trclass);
  virtual ~IlcAnalysisTaskKMeans() {}
  virtual void     UserCreateOutputObjects();
  virtual void     UserExec(Option_t *option);
  virtual void     Terminate(Option_t *);
  virtual void     SetCuts(IlcESDtrackCuts* cuts) {fCuts = cuts;}
  virtual Double_t DeltaPhi(Double_t phi1, Double_t phi2);
  virtual Double_t DeltaR(Double_t phi1, Double_t eta1, Double_t phi2, Double_t eta2);
  virtual void     SetK(Int_t k) {fK = k;} 
  virtual void     SetMinimumMultiplicity(Int_t k) {fNMin = k;} 
  virtual void     SetEtaPhi(TH2F* h2) {fH2REtaPhi = h2;}
 private:
  // Others
  Int_t            fK;             // K                        
  Int_t            fNMin;          // Minimum multipicity                         
  TList*           fHists;         //! Histograms
  TH1F*            fH1CEta;        //! Eta distribution of clusters
  TH1F*            fH1CPhi;        //! Phi distribution of clusters  
  TH1F*            fH1CEtaR;       //! Eta distribution of clusters for rndm evnt
  TH2F*            fH2N1N2;        //! Cluster sizes 
  TH1F*            fH1Pt;          //! pt outside clusters
  TH1F*            fH1PtC;         //! pt outside clusters
  TH1F*            fH1PtC1;        //! pt dr > 0.4
  TH1F*            fH1PtC2;        //! pt dr > 0.2 
  TH1F*            fH1PtAS;        //! away-side peak 
  TH1F*            fH1PtR;         //! away-side peak 
  TH1F*            fH1SPt;         //! sum pt
  TH1F*            fH1SPtC;        //! sum pt
  TH1F*            fH1DPhi;        //! Dphi wr to cluster
  TH1F*            fH1DR;          //! DR   wr to cluster
  TH1F*            fH1DRR;         //! DR   wr to cluster from rndm events   
  TH2F*            fH2DPhiEta;     //! eta-phi wr to cluster
  TH2F*            fH2DPhiEtaR;    //! eta-phi wr to cluster for rndm events 
  TH2F*            fH2DPhiEtaL;    //! eta-phi of leading particle
  TH2F*            fH2DPhiEtaLR;   //! eta-phi of leading particle
  TH2F*            fH2DPhiEtaC;    //! eta-phi of Clusters
  TH2F*            fH2DPhiEtaCR;   //! eta-phi of Clusters
  TH1F*            fH1Resp;        //! responsibility
  TH1F*            fH1RespR;       //! responsibility
  TH2F*            fH2Sigma;       //! sigma
  TH2F*            fH2SigmaR;      //! sigma random
  TH1F*            fHDensity;      //! Particle density
  TH1F*            fHCSize;        //! Cluster Size
  TH1F*            fHNCluster;     //! Number of clusters
  TH2F*            fHPtDensity;    //! Pt vs density
  TH1F*            fHDPhi;         //! Phi Correlation 
  TH2F*            fH2EtaPhi;      //! eta phi 
  TH2F*            fH2REtaPhi;     //  eta phi 
  IlcKMeansResult* fA[10];         //! Array of results
  IlcKMeansResult* fB[10];         //! Array of results
  IlcESDtrackCuts* fCuts;          // List of cuts
  ClassDef(IlcAnalysisTaskKMeans, 1); // A k-means clustering analysis
};

#endif

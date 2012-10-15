#ifndef ILCTARGETTRACKSA_H
#define ILCTARGETTRACKSA_H 
/* Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 * See cxx source for full Copyright notice                               */
////////////////////////////////////////////////////
//  Stand alone track class                       //
//  Origin:  Elisabetta Crescio                   //
//  e-mail:  crescio@to.infn.it                   //
//                                                //
////////////////////////////////////////////////////

#include "IlcTARGETtrackMI.h"

class IlcTARGETtrackSA : public IlcTARGETtrackMI {


 public:

  IlcTARGETtrackSA();
  IlcTARGETtrackSA(const IlcTARGETtrackMI& t);
  IlcTARGETtrackSA(const IlcTARGETtrackSA& t);
  IlcTARGETtrackSA(IlcTARGETgeom* geom,Int_t layer, Int_t ladder, Int_t detector, 
                Double_t Ycoor, Double_t Zcoor, Double_t phi, 
                Double_t tanlambda, Double_t curv, Int_t lab);
  Int_t GetClusterIndexSA(Int_t i) const {return fSain[i];}
  Int_t GetClusterMark(Int_t layer,Int_t i) const {return fCluMark[layer][i];}
  Int_t GetNumberOfClustersSA() const {return fNSA;}
  Int_t GetNumberOfMarked(Int_t lay) const {return fNM[lay];}
  Int_t GetMaxNumberOfClusters() const {return fgkMaxNumberOfClusters;}
  Int_t GetMaxNMarkedPerLayer(int /*lay*/) const {return (fnLayers2<4)?fgkMaxNumberOfClustersL:(fgkMaxNumberOfClustersL+1)/2;}
  void  AddClusterSA(Int_t layer, Int_t clnumb);
  void  AddClusterV2(Int_t layer,Int_t clnumb);
  void  AddClusterMark(Int_t layer, Int_t clnumb);
  int GetNLayers() const {return fnLayers;}
 protected: 

  void SetNumberOfClustersSA(Int_t n){fNSA = n;}
  void SetNumberOfMarked(Int_t lay,Int_t n) {fNM[lay] = n;}
  void ResetIndexSA(){for(Int_t k=0; k<fgkMaxNumberOfClusters; k++) fSain[k]=0;}
  void ResetMarked(); 

  static const Int_t fgkMaxNumberOfClustersL = 4;// Max. n. of clusters/layer 
  static const Int_t fgkMaxNumberOfClusters = 32;// Max. number of clusters 
                                                 // from which will be looked at different combinations
  static const Int_t fgkLayers = kMaxLayer; //Number of lyers
                                            // per trackSA
  UInt_t  fSain[fgkMaxNumberOfClusters];   // cluster index (SA)
  Int_t fNSA;          // number of clusters SA 

  Int_t fCluMark[fgkLayers][fgkMaxNumberOfClustersL]; //indices for cluster used
  Int_t fNM[fgkLayers]; //number of marked clusters
  Int_t fnLayers; //number of not emty layers
  Int_t fnLayers2; //number of layer with nclusters>=2

  ClassDef(IlcTARGETtrackSA,5)
};

#endif




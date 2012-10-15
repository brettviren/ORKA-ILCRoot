#ifndef ILCTARGETCLUSTERV2_H
#define ILCTARGETCLUSTERV2_H
/* Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 * See cxx source for full Copyright notice                               */

//-------------------------------------------------------------------------
//                        TARGET Cluster Class
//
//      Origin: Iouri Belikov, CERN, Jouri.Belikov@cern.ch 
//-------------------------------------------------------------------------

#include "TMath.h"
#include "IlcCluster.h"
#include "IlcTARGETrecoV2.h"

//_____________________________________________________________________________
class IlcTARGETclusterV2 : public IlcCluster {
public:
  IlcTARGETclusterV2() : IlcCluster() {
    fQ=0; fLayer=0; fNz=fNy=1; fType=0; fDeltaProb=0;
  }
  IlcTARGETclusterV2(Int_t *lab,Float_t *hit, Int_t *info) : IlcCluster(0,hit) {
    fIndex=lab[3];
    fQ=hit[4];
    fNy    = info[0];
    fNz    = info[1];
    fLayer = info[2];
    fChargeRatio = 0;
    fType=0;
    fDeltaProb=0.;
  }

  void Use(Int_t = 0) {fQ=-fQ;}
  void UnUse() {fQ=TMath::Abs(fQ);}
  void SetQ(Float_t q) {fQ=q;}
  void SetDetectorIndex(Int_t i) { fIndex=i; }
  void SetLayer(Int_t layer) {fLayer=layer;}
  void SetNz(Int_t nz) {fNz =nz;}
  void SetNy(Int_t ny){fNy=ny;}
  void SetChargeRatio(Float_t ratio) { fChargeRatio = ratio;}
  void SetPhiR(Float_t y) { fChargeRatio=y; }
  void SetType(Int_t type){ fType=type;}
  void SetDeltaProbability(Float_t prob){fDeltaProb = prob;}

  Int_t IsUsed() const {return (fQ<0)?1:0;}
  Float_t GetQ() const {return TMath::Abs(fQ);}
  Int_t GetDetectorIndex() const { return 0x3FF&fIndex; }
  Int_t GetLayer() const {return fLayer;}
  Int_t GetNz() const {return fNz;}
  Int_t GetNy() const {return fNy;}
  Float_t GetChargeRatio() const {return fChargeRatio;}
  Float_t GetPhiR() const {return fChargeRatio;}
  Int_t GetPindex() const { return 0xFFF00000&fIndex; }  //SSD clusters only
  Int_t GetNindex() const { return 0xFFC00&fIndex; }  //SSD clusters only
  Int_t GetType() const {return fType;}  // type of the cluster
  Float_t GetDeltaProbability() const{return fDeltaProb;} //probability to belong to the delta ray
private:
  Int_t    fIndex;    // detector index
  Float_t  fQ ;       // Q of cluster (in ADC counts)
  Char_t   fLayer;    // layer number
  Short_t   fNz;       //number of digits in Z direction
  Short_t   fNy;       //number of digits in y direction 
  Float_t  fChargeRatio; //charge ratio
  Int_t    fType;         //quilcty factor of the cluster
  Float_t  fDeltaProb;    // probability to be deleta electron
  ClassDef(IlcTARGETclusterV2,2)  // TARGET clusters
};

#endif



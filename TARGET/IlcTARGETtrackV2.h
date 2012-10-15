#ifndef ILCTARGETTRACKV2_H
#define ILCTARGETTRACKV2_H
/* Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 * See cxx source for full Copyright notice                               */

//-------------------------------------------------------------------------
//                       TARGET Track Class
//
//        Origin: Iouri Belikov, CERN, Jouri.Belikov@cern.ch 
//     dEdx analysis by: Boris Batyunya, JINR, Boris.Batiounia@cern.ch
//-------------------------------------------------------------------------

#include <IlcKalmanTrack.h>
#include "IlcTARGETrecoV2.h"

class IlcESDtrack;

const int kClusterLayerMask=0xfe000000;
const int kClusterLayerShift=25;
const int kClusterCountMask=0x00ffffff;
const int kClusterDirMask  =0x01000000;


//_____________________________________________________________________________
class IlcTARGETtrackV2 : public IlcKalmanTrack {
public:
  IlcTARGETtrackV2();
  IlcTARGETtrackV2(IlcESDtrack& t,Bool_t c=kFALSE) throw (const Char_t *);
  IlcTARGETtrackV2(const IlcTARGETtrackV2& t);

  Bool_t CorrectForMaterial(Double_t d, Double_t x0=21.82) {
    return IlcExternalTrackParam::CorrectForMaterial(d,x0,GetMass());
  }
  Bool_t PropagateTo(Double_t xr, Double_t d, Double_t x0=21.82);
  Double_t GetPredictedChi2(const IlcCluster *cluster) const;
  Bool_t Update(const IlcCluster *cl, Double_t chi2, Int_t i);

  Bool_t 
     PropagateToVertex(const IlcESDVertex *v,Double_t d=0.,Double_t x0=0.);
  Bool_t Propagate(Double_t alpha, Double_t xr,bool byR=false);
  Bool_t Improve(Double_t x0,Double_t xyz[3],Double_t ers[3]);

  void SetdEdx(Double_t dedx) {fdEdx=dedx;}
  void SetSampledEdx(Float_t q, Int_t i);
  Float_t GetSampledEdx(Int_t i) const {return fdEdxSample[i];}
  void CookdEdx(Double_t low=0., Double_t up=0.51);
  void SetDetectorIndex(Int_t i) {SetLabel(i);}
  void ResetClusters() { SetChi2(0.); SetNumberOfClusters(0); }
  void UpdateESDtrack(ULong_t flags) const;
  
  IlcESDtrack *GetESDtrack() const {return fESDtrack;}

  Int_t GetDetectorIndex() const {return GetLabel();}
  Double_t GetdEdx() const {return fdEdx;}
  Double_t GetPIDsignal() const {return GetdEdx();}
  Double_t GetC() const {return IlcExternalTrackParam::GetC(GetBz());}
  Double_t GetD(Double_t x, Double_t y) const {
    return IlcExternalTrackParam::GetD(x,y,GetBz());
  }
  void GetDZ(Double_t xv, Double_t yv, Double_t zv, Float_t dz[2]) const {
    return IlcExternalTrackParam::GetDZ(xv,yv,zv,GetBz(),dz);
  }

  Bool_t GetGlobalXYZat(Double_t r,Double_t &x,Double_t &y,Double_t &z) const;

  Int_t Compare(const TObject *o) const;
  Int_t GetClusterIndex(Int_t i) const {return fIndex[i];}
  Bool_t Invariant() const;

  void  SetExtraCluster(Int_t i, Int_t idx) {fIndex[kMaxLayer+i]=idx;}
  Int_t GetExtraCluster(Int_t i) const {return fIndex[kMaxLayer+i];}

  Bool_t Rotate(Double_t alpha){return IlcExternalTrackParam::Rotate(alpha,GetBz());}
  Bool_t RotateZ(bool toendcap){return IlcExternalTrackParam::RotateZ(toendcap,GetBz());}
  Double_t GetBz() const ;

protected:
  Double_t fdEdx;            // dE/dx
  Float_t fdEdxSample[kMaxClusters];    // array of dE/dx samples b.b.

  Int_t fIndex[2*kMaxClusters]; // indices of associated clusters 

  IlcESDtrack *fESDtrack;    //! pointer to the connected ESD track

private:
  ClassDef(IlcTARGETtrackV2,6)  //TARGET reconstructed track
};

inline
void IlcTARGETtrackV2::SetSampledEdx(Float_t q, Int_t i) {
  //----------------------------------------------------------------------
  // This function stores dEdx sample corrected for the track segment length 
  // Origin: Boris Batyunya, JINR, Boris.Batiounia@cern.ch
  //----------------------------------------------------------------------
  //  if (i<0) return;
  //  if (i>3) return;
  Double_t s=GetSnp(), t=GetTgl();
  q *= TMath::Sqrt((1-s*s)/(1+t*t));
  fdEdxSample[i]=q;
}

#endif



#ifndef ILCTRACKFITTERSTRAIGHT_H
#define ILCTRACKFITTERSTRAIGHT_H

#include "IlcTrackFitter.h"

class IlcTrackFitterStraight : public IlcTrackFitter{
 public:
  IlcTrackFitterStraight();
  IlcTrackFitterStraight(IlcTrackPointArray *array, Bool_t owner = kTRUE);
  IlcTrackFitterStraight(const IlcTrackFitterStraight &fitter);
  IlcTrackFitterStraight &operator =(const IlcTrackFitterStraight& fitter);
  virtual ~IlcTrackFitterStraight();

  Bool_t Fit(const TArrayI *volIdx,const TArrayI *volIdsFit = 0x0,
	     IlcGeomManager::ELayerID layerRangeMin = IlcGeomManager::kFirstLayer,
	     IlcGeomManager::ELayerID layerRangeMax = IlcGeomManager::kLastLayer);
  Bool_t GetPCA(const IlcTrackPoint &p, IlcTrackPoint &p2) const;

  void Reset();
  void AddPoint(Float_t x, Float_t y, Float_t z, Float_t sy, Float_t sz);
  Bool_t Update();

  //  Double_t GetC(); 
  Double_t GetYat(Double_t x) const;
  Double_t GetZat(Double_t x) const;
  Double_t GetDYat(Double_t x) const;
  Double_t GetDZat(Double_t x) const;
  Bool_t   GetXYZat(Double_t r, Float_t *xyz) const;

 protected:

  Double_t      fAlpha;     //angle to transform to the fitting coordinate system
  Double_t      fSumXY[5];  //sums for XY part
  Double_t      fSumYY;     //sum for YY part
  Double_t      fSumXZ[5];  //sums for XZ part
  Double_t      fSumZZ;     //sum for ZZ part
  Int_t         fNUsed;     //actual number of space-points used in the fit
  Bool_t        fConv;      // indicates convergation

 private:
  Bool_t Begin(Int_t, Int_t) {Reset(); return kTRUE;}
  Bool_t AddPoint(const IlcTrackPoint *) {return kTRUE;}

  ClassDef(IlcTrackFitterStraight,1)  // Fast fit of straight tracks

};

#endif

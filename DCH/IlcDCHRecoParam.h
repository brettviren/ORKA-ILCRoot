#ifndef ILCDCHRECOPARAM_H
#define ILCDCHRECOPARAM_H
/* Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 * See cxx source for full Copyright notice                               */

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// Class with DCH reconstruction parameters                                  //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#include <Rtypes.h>

#include "TObject.h"

class IlcDCHRecoParam : public TObject
{
 public: 
  IlcDCHRecoParam();
  virtual ~IlcDCHRecoParam();
  Double_t GetCtgRange() const     { return fCtgRange;}
  Double_t GetMaxSnpTracker() const{ return fMaxSnpTracker;}
  Double_t GetMaxSnpTrack() const  { return fMaxSnpTrack;}
  //
  Int_t    GetFirstBin() const     { return fFirstBin;}
  Int_t    GetLastBin() const      { return fLastBin;}
  void     SetTimeBinRange(Int_t first, Int_t last){ fFirstBin = first; fLastBin = last;}
  Bool_t   GetCalcPedestal() const { return fBCalcPedestal;}
  Bool_t   GetDoUnfold() const     { return fBDoUnfold;}
  Float_t  GetDumpAmplitudeMin() const  { return fDumpAmplitudeMin;}
  Float_t  GetMaxNoise() const     { return fMaxNoise;}  
  //
  Bool_t   GetDoKinks() const      { return fBKinkFinder;}
  Float_t  GetMaxC()    const      { return fMaxC;}
  Bool_t   GetSpecialSeeding() const { return fBSpecialSeeding;}
  Bool_t   GetBYMirror() const { return fBYMirror;}
  static   IlcDCHRecoParam *GetLowFluxParam();        // make reco parameters for low  flux env.
  static   IlcDCHRecoParam *GetHighFluxParam();       // make reco parameters for high flux env. 
  static   IlcDCHRecoParam *GetLaserTestParam(Bool_t bPedestal);  // special setting for laser 
  static   IlcDCHRecoParam *GetCosmicTestParam(Bool_t bPedestal); // special setting for cosmic  
  //
 protected:
  Double_t fCtgRange;        // +-fCtgRange is the ctg(Theta) window used for clusterization and tracking (MI) 
  Double_t fMaxSnpTracker;   // max sin of local angle  - for DCH tracker
  Double_t fMaxSnpTrack;     // max sin of local angle  - for track 
  Bool_t   fBYMirror;        // mirror of the y - pad coordinate 
  //
  //   clusterer parameters
  //
  Int_t    fFirstBin;        // first time bin used by cluster finder
  Int_t    fLastBin;         // last time bin  used by cluster finder 
  Bool_t   fBCalcPedestal;   // calculate Pedestal
  Bool_t   fBDoUnfold;       // do unfolding of clusters
  Float_t  fDumpAmplitudeMin; // minimal amplitude of signal to be dumped 
  Float_t  fMaxNoise;        // maximal noise sigma on pad to be used in cluster finder
  //
  //
  Float_t  fMaxC;            // maximal curvature for tracking
  Bool_t   fBSpecialSeeding; // special seeding with big inclination angles allowed (for Cosmic and laser)
  Bool_t   fBKinkFinder;     // do kink finder reconstruction
  ClassDef(IlcDCHRecoParam, 1)
};


/*********************************************/
/*********************************************/


class IlcDCHrecoParam:public TObject{
public:
  IlcDCHrecoParam();
  virtual ~IlcDCHrecoParam() {}
  void SetRoadY(double roady)      {fRoadY=roady;}
  void SetRoadZ(double roadz)      {fRoadZ=roadz;}
  void SetSafetyZ(double safetyz)  {fSafetyZ=safetyz;}
  double GetRoadY()            const{return fRoadY;}
  double GetRoadZ()            const{return fRoadZ;}
  double GetSafetyZ()          const{return fSafetyZ;}
  void SetNearestByC       (double fnearestbyC  )  {fNearestByC       =fnearestbyC  ;}
  void SetNearestByTgl     (double fnearestbyTgl)  {fNearestByTgl     =fnearestbyTgl;}
  void SetNearestByDistance2(double fnearestbyDst2)  {fNearestByDistance2=fnearestbyDst2;}
  double GetNearestByC()       const{ return fNearestByC;  }          //
  double GetNearestByTgl()     const{return fNearestByTgl;}        //
  double GetNearestByDistance2()const{return fNearestByDistance2;}   //
  void SetMinNOfClusetrs(int minNC){fMinNOfClusetrs=minNC;}    
  int GetMinNOfClusetrs()const{return  fMinNOfClusetrs;}  
  void SetMinDensity(double den){fMinDensity=den;}
  double GetMinDensity() const{return fMinDensity;} 
  double SetSeedTrackSigma2(double seedsigma2){return  fSeedTrackSigma2=seedsigma2;}
  double GetSeedTrackSigma2()const{return  fSeedTrackSigma2;}
  double SetSeedBeamSigma2(double seedbeamsigma2){return  fSeedBeamSigma2=seedbeamsigma2;}
  double GetSeedBeamSigma2()const{return  fSeedBeamSigma2;}
  void   SetCutTrackSigma2(double cutsigma2) {fCutTrackSigma2=cutsigma2;};
  double GetCutTrackSigma2() const{return fCutTrackSigma2;};
  void SetMinDCAr(double cut) {fMinDCAr=cut;};
  void SetMinDCAz(double cut) {fMinDCAz=cut;};
  double GetMinDCAr() const {return fMinDCAr;};
  double GetMinDCAz() const {return fMinDCAz;};
  void SetScaleForQuality(double dr) {fScaleForQuality=dr;};
  double GetScaleForQuality() const {return fScaleForQuality;};
  void SetDeltaZ(double dz) {fDeltaZ=dz;};
  double GetDeltaZ() const {return fDeltaZ;};
  void SetDistFromEdge(double dist)  {fDistFromEdge=dist;};
  double GetDistFromEdge() const {return fDistFromEdge;};
  double GetMinRofV0         () const {return fMinRofV0         ;}; 
  double GetMaxRofV0         () const {return fMaxRofV0         ;}; 
  double GetCutOnDistanceInV0() const {return fCutOnDistanceInV0;}; 
  void SetMinRofV0         (double rmin) {fMinRofV0=rmin     ;}; 
  void SetMaxRofV0         (double rmax) {fMaxRofV0=rmax     ;}; 
  void SetCutOnDistanceInV0(double dr)   {fCutOnDistanceInV0=dr;}; 
protected:
  double fRoadY; // road forr searching clusters
  double fRoadZ;
  double fSafetyZ; // Aditional Z forr good region
  // if (TMath::Abs(z)<(IlcDCHReconstructor::GetCtgRange()*t.GetX()+fSafetyZ)) t.fNFoundable++;

  //  Distance to check overlapping o tracks 
  double fNearestByC;          // Delta Curvature 
  double fNearestByTgl;        // Delta Tgl 
  double fNearestByDistance2;  // dy^2+dz^2
  // number of clusters
  int fMinNOfClusetrs;    // Minimum Number of Clusters forr acceptable tracks 
  double fMinDensity;      //minimum acceptable dencity of clusters in track
  // 
  double fSeedTrackSigma2; // default resolution of track position after make seed with 3 clusters
  double fSeedBeamSigma2; // default resolution addition to beam resolution during make seed with 2 clusters and beam point
                          // 25000*Curv*Curv+fSeedBeamSigma2
  double fCutTrackSigma2; // cut on resolution of track position after make seed

  //kinks
  double fMinDCAr; // cut on r near beam point when looking forr circular track
  double fMinDCAz; // cut on z near beam point when looking forr circular track
  double fDeltaZ; // cut on z in middle of DCH when merge tracks
  double fDistFromEdge; // distance from inner and outer radius of DCH forr radius of kink
  double fScaleForQuality; // dAngle/(fScaleForQuality(3),+abs(Rkink-Rtrack))

  //v0
  double fMinRofV0          ; // Minimum radius of V0
  double fMaxRofV0          ; // Maximum radius of V0
  double fCutOnDistanceInV0 ; //Cut On Distance In V0 of tracks
  ClassDef(IlcDCHrecoParam,1);
};

/*********************************************/
/*********************************************/

//namespace IlcDCHreco {    
   const Int_t kMaxClusterPerLayer=2000;
   const Int_t kRowsToSkip=10;
   const Int_t kMaxLayer=280;

   const Double_t kMaxCHI2cluster=16.;
   const Double_t kMaxCHI2track=3.;
   const Double_t kMaxROAD=30.;

namespace ClusterIndex{
  enum Index_t{
      Sign   =0x80000000,
      Sector =0xff000000,
      Layer  =0x00fff000,
      NotAccept =0x00000800,
      Cluster=0x000007ff,
      SignOffset   =31,
      SectorOffset =24,
      LayerOffset  =12,
      NotAcceptOffset =11,
      ClusterOffset=0
  };
}
//}  //using namespace IlcDCHreco;

#endif

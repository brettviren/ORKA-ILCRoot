#ifndef ILCTARGETTRACKERV2_H
#define ILCTARGETTRACKERV2_H
/* Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 * See cxx source for full Copyright notice                               */

//-------------------------------------------------------------------------
//                          TARGET tracker
//     reads IlcTARGETRecPoint clusters and creates IlcTARGETtrackV2 tracks
//           Origin: Iouri Belikov, CERN, Jouri.Belikov@cern.ch 
//-------------------------------------------------------------------------
#include "IlcTracker.h"
#include "IlcTARGETrecoV2.h"
#include "IlcTARGETtrackV2.h"

class IlcTARGETRecPoint;
class IlcESD;
class IlcTARGETgeom;
class TTree;


//-------------------------------------------------------------------------
class IlcTARGETtrackerV2 : public IlcTracker {
public:
  IlcTARGETtrackerV2();
  IlcTARGETtrackerV2(const IlcTARGETgeom *geom);
  ~IlcTARGETtrackerV2(){}
  IlcCluster *GetCluster(Int_t index) const;
  IlcTARGETRecPoint *GetCluster(Int_t l, Int_t c) const {
    return fgLayers[l].GetCluster(c);
  }
  Int_t GetNumberOfClustersLayer(Int_t n) const {
     return fgLayers[n].GetNumberOfClusters();
  }   
  Int_t LoadClusters(TTree *cf);
  void UnloadClusters();
  Int_t Clusters2Tracks(IlcESD *event);
  Int_t PropagateBack(IlcESD *event);
  Int_t RefitInward(IlcESD *event);
  Bool_t RefitAt(Double_t x, IlcTARGETtrackV2 *seed, 
                 const IlcTARGETtrackV2 *t, Bool_t extra=kFALSE);
  void SetupFirstPass(Int_t *flags, Double_t *cuts=0);
  void SetupSecondPass(Int_t *flags, Double_t *cuts=0);

  void SetLastLayerToTrackTo(Int_t l=0) {fLastLayerToTrackTo=l;} 
  void SetLayersNotToSkip(Int_t *l);

  void UseClusters(const IlcKalmanTrack *t, Int_t from=0) const;

  class IlcTARGETdetector {
  public:
    IlcTARGETdetector():fR(0.),fPhi(0.){}
    IlcTARGETdetector(Double_t r,Double_t phi):fR(r),fPhi(phi){}
    Double_t GetR()   const {return fR;}
    Double_t GetPhi() const {return fPhi;}
  private:
    Double_t fR;    // polar coordinates 
    Double_t fPhi;  // of this detector
  };

  class IlcTARGETlayer {
  public:
    enum {kNsector=5, kMaxClusterPerSector=kMaxClusterPerLayer/kNsector};
    IlcTARGETlayer();
    IlcTARGETlayer(Double_t r, Double_t p, Double_t z, Int_t nl, Int_t nd);
   ~IlcTARGETlayer();
    Int_t InsertCluster(IlcTARGETRecPoint *c);
    void ResetClusters();
    Int_t SelectClusters(Float_t zmi, Float_t zma, Float_t ymi, Float_t yma);
    const IlcTARGETRecPoint *GetNextCluster(Int_t &ci);
    void ResetRoad();
    Double_t GetRoad() const {return fRoad;}
    Double_t GetR() const {return fR;}
    IlcTARGETRecPoint *GetCluster(Int_t i) const { return fClusters[i]; } 
    IlcTARGETdetector &GetDetector(Int_t n) const { return fDetectors[n]; }
    Int_t FindDetectorIndex(Double_t phi, Double_t z) const;
    Double_t GetThickness(Double_t y, Double_t z, Double_t &x0) const;
    Int_t GetNladders() const {return fNladders;}
    Int_t GetNdetectors() const {return fNdetectors;}
    Int_t GetNumberOfClusters() const;
  protected:
    IlcTARGETlayer(const IlcTARGETlayer&);
    IlcTARGETlayer &operator=(const IlcTARGETlayer &tr);  
    Double_t fR;                // mean radius of this layer
    Double_t fPhiOffset;        // offset of the first detector in Phi
    Int_t fNladders;            // number of ladders
    Double_t fZOffset;          // offset of the first detector in Z
    Int_t fNdetectors;          // detectors/ladder
    IlcTARGETdetector *fDetectors; // array of detectors

    IlcTARGETRecPoint *fClusters[kMaxClusterPerLayer]; // pointers to clusters
    Int_t fN[kNsector];         // numbers of clusters sector by sector 

    Int_t fIndex[kMaxClusterPerLayer]; // indexes of selected clusters 
    Int_t fNsel;                       // number of selected clusters

    Double_t fRoad;     // road defined by the cluster density
    Int_t FindClusterIndex(Float_t z, Int_t s) const;
  };

protected:
  IlcTARGETtrackerV2(const IlcTARGETtrackerV2&);
  void CookLabel(IlcKalmanTrack *t,Float_t wrong) const;
  Double_t GetEffectiveThickness(Double_t y, Double_t z) const;
  void  FollowProlongation();
  Int_t TakeNextProlongation();
  void ResetBestTrack() {
     fBestTrack.~IlcTARGETtrackV2();
     new(&fBestTrack) IlcTARGETtrackV2(fTrackToFollow);
  }
  void ResetTrackToFollow(const IlcTARGETtrackV2 &t) {
     fTrackToFollow.~IlcTARGETtrackV2();
     new(&fTrackToFollow) IlcTARGETtrackV2(t);
  }
  Int_t fI;                              // index of the current layer
  static IlcTARGETlayer fgLayers[kMaxLayer];// TARGET layers
  IlcTARGETtrackV2 fTracks[kMaxLayer];      // track estimations at the TARGET layers
  IlcTARGETtrackV2 fBestTrack;              // "best" track 
  IlcTARGETtrackV2 fTrackToFollow;          // followed track
  Int_t fPass;                           // current pass through the data 
  Int_t fConstraint[2];                  // constraint flags

  Int_t fLayersNotToSkip[kMaxLayer];     // layer masks
  Int_t fLastLayerToTrackTo;             // the innermost layer to track to

private:
  IlcTARGETtrackerV2 &operator=(const IlcTARGETtrackerV2 &tr);  
  ClassDef(IlcTARGETtrackerV2,1)   //TARGET tracker V2
};


inline void IlcTARGETtrackerV2::SetupFirstPass(Int_t *flags, Double_t *cuts) {
  // This function sets up flags and cuts for the first tracking pass   
  //
  //   flags[0] - vertex constaint flag                                
  //              negative means "skip the pass"                        
  //              0        means "no constraint"                        
  //              positive means "normal constraint"                    

   fConstraint[0]=flags[0];
   if (cuts==0) return;
}

inline void IlcTARGETtrackerV2::SetupSecondPass(Int_t *flags, Double_t *cuts) {
  // This function sets up flags and cuts for the second tracking pass   
  //
  //   flags[0] - vertex constaint flag                                
  //              negative means "skip the pass"                        
  //              0        means "no constraint"                        
  //              positive means "normal constraint"                    

   fConstraint[1]=flags[0];
   if (cuts==0) return;
}

inline void IlcTARGETtrackerV2::CookLabel(IlcKalmanTrack *t,Float_t wrong) const {
  //--------------------------------------------------------------------
  //This function "cooks" a track label. If label<0, this track is fake.
  //--------------------------------------------------------------------
   Int_t tpcLabel=t->GetLabel();
   if (tpcLabel<0) return;
   IlcTracker::CookLabel(t,wrong);
   if (tpcLabel != t->GetLabel()) t->SetLabel(-tpcLabel); 
}

#endif

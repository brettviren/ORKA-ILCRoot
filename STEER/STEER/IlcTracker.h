#ifndef ILCTRACKER_H
#define ILCTRACKER_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcTracker.h 53891 2012-01-13 10:34:05Z hristov $ */

//-------------------------------------------------------------------------
//                          class IlcTracker
//   that is the base for IlcTPCtracker, IlcITStrackerV2 and IlcTRDtracker
//       Origin: Iouri Belikov, CERN, Jouri.Belikov@cern.ch 
//-------------------------------------------------------------------------

#include "IlcTrackerBase.h"

#include "IlcRecoParam.h"
#include "IlcPlaneEff.h"

class TTree;
class IlcCluster;
class IlcESDEvent;
class IlcESDtrack;
class IlcExternalTrackParam;
class IlcTrackPoint;
class IlcKalmanTrack;
class IlcEventInfo;

class IlcTracker : public IlcTrackerBase {
public:
  IlcTracker();
  virtual ~IlcTracker(){}

  virtual Int_t Clusters2Tracks(IlcESDEvent *event)=0;
  virtual Int_t PropagateBack(IlcESDEvent *event)=0;
  virtual Int_t RefitInward(IlcESDEvent *event)=0;
  virtual Int_t LoadClusters(TTree *)=0;
  virtual void UnloadClusters()=0;
  virtual IlcCluster *GetCluster(Int_t index) const=0;

  virtual Int_t PostProcess(IlcESDEvent */*event*/) {return 0;}
  virtual void FillClusterArray(TObjArray* array) const;
  virtual IlcPlaneEff *GetPlaneEff() {return NULL;}
  virtual Bool_t GetTrackPoint(Int_t /* index */ , IlcTrackPoint& /* p */) const { return kFALSE;}
  virtual Bool_t GetTrackPointTrackingError(Int_t /* index */, 
  	   IlcTrackPoint& /* p */, const IlcESDtrack* /* t */) { return kFALSE;}
  virtual void  UseClusters(const IlcKalmanTrack *t, Int_t from=0) const;
  virtual void  CookLabel(IlcKalmanTrack *t,Float_t wrong) const; 

  static void FillResiduals(const IlcExternalTrackParam *t,
			   Double_t *p, Double_t *cov, 
                           UShort_t id, Bool_t updated=kTRUE);
  static void FillResiduals(const IlcExternalTrackParam *t,
                            const IlcCluster *c, Bool_t updated=kTRUE);
  static void SetFillResiduals(IlcRecoParam::EventSpecie_t es, Bool_t flag=kTRUE) { fFillResiduals=flag; fEventSpecie = es ;}
  static void SetResidualsArray(TObjArray **arr) { fResiduals=arr; }
  static TObjArray ** GetResidualsArray() { return fResiduals; }

  void                SetEventInfo(IlcEventInfo *evInfo) {fEventInfo = evInfo;}
  const IlcEventInfo* GetEventInfo() const {return fEventInfo;}

protected:
  IlcTracker(const IlcTracker &atr);
private:
  IlcTracker & operator=(const IlcTracker & atr);
  static Bool_t fFillResiduals;     // Fill residuals flag
  static TObjArray **fResiduals;    //! Array of histograms with residuals

  static IlcRecoParam::EventSpecie_t fEventSpecie ; //! event specie, see IlcRecoParam
  IlcEventInfo*                      fEventInfo;    //! pointer to the event info object
  
  ClassDef(IlcTracker,6) //abstract tracker
};

#endif

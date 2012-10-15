#ifndef ILCANALYSISTASKESDFILTER_H
#define ILCANALYSISTASKESDFILTER_H
 
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id$ */

#include <TList.h> 
#include <TF1.h> 
#include "IlcAnalysisTaskSE.h"
#include "IlcESDtrack.h"
#include "IlcAODTrack.h"
#include "IlcAODPid.h"
#include "IlcESDpid.h"

class IlcAnalysisFilter;
class IlcStack;
class IlcESDtrack;
class IlcMCEventHandler;
class TRefArray;
class IlcAODHeader;
class IlcESDtrackCuts;

class IlcAnalysisTaskESDfilter : public IlcAnalysisTaskSE
{
 public:
    IlcAnalysisTaskESDfilter();
    IlcAnalysisTaskESDfilter(const char* name);
    virtual ~IlcAnalysisTaskESDfilter();
    // Implementation of interface methods
    virtual void   UserCreateOutputObjects();
    virtual void   Init();
    virtual void   LocalInit() {Init();}
    virtual void   UserExec(Option_t *option);
    virtual void   Terminate(Option_t *option);

    virtual void ConvertESDtoAOD();
    // Setters
    virtual void SetTrackFilter   (IlcAnalysisFilter*   trackF) {fTrackFilter    =   trackF;}
    virtual void SetTPCOnlyFilterMask (UInt_t filterMask)       {SetTPCConstrainedFilterMask(filterMask);}

    virtual void SetTPCConstrainedFilterMask (UInt_t filterMask)       {fTPCConstrainedFilterMask    =  filterMask;}
    virtual void SetHybridFilterMaskTPCConstrainedGlobal(UInt_t filterMask)  {fHybridFilterMaskTPCCG    =  filterMask;}
    virtual void SetWriteHybridTPCConstrainedOnly(bool b){fWriteHybridTPCCOnly = b;}

    virtual void SetGlobalConstrainedFilterMask (UInt_t filterMask)    {fGlobalConstrainedFilterMask    =  filterMask;}
    virtual void SetHybridFilterMaskGlobalConstrainedGlobal(UInt_t filterMask)  {fHybridFilterMaskGCG    =  filterMask;}
    virtual void SetWriteHybridGlobalConstrainedOnly(bool b){fWriteHybridGCOnly = b;}


    virtual void SetKinkFilter    (IlcAnalysisFilter*    KinkF) {fKinkFilter     =    KinkF;}
    virtual void SetV0Filter      (IlcAnalysisFilter*      V0F) {fV0Filter       =      V0F;}
    virtual void SetCascadeFilter (IlcAnalysisFilter* CascadeF) {fCascadeFilter  = CascadeF;}
    virtual void SetPthreshold    (Double_t p)                  {fHighPthreshold =        p;}
    virtual void SetPshape        (TF1 *func)                   {fPtshape        =     func;}
    virtual void SetEnableFillAOD (Bool_t b)                    {fEnableFillAOD  =     b;}

    virtual void SetAODPID(IlcESDtrack *esdtrack, IlcAODTrack *aodtrack, IlcAODPid *detpid);
    void SetDetectorRawSignals(IlcAODPid *aodpid, IlcESDtrack *track);

    void SetV0Cuts(const Double_t cuts[7]) {for (Int_t icut = 0; icut<7; icut++) fV0Cuts[icut] = cuts[icut];}

    void SetCascadeCuts(const Double_t cuts[8]) {for (Int_t icut = 0; icut<8; icut++) fCascadeCuts[icut] = cuts[icut];} 

    void GetV0Cuts(Double_t cuts[7]) const {for (Int_t icut = 0; icut<7; icut++) cuts[icut] = fV0Cuts[icut];}

    void GetCascadeCuts(Double_t cuts[8]) const {for (Int_t icut = 0; icut<8; icut++) cuts[icut] = fCascadeCuts[icut];}


  void PrintTask(Option_t *option="all", Int_t indent=0) const;
  
  void DisableVZERO() { fIsVZEROEnabled = kFALSE; }
  void DisableTZERO() { fIsTZEROEnabled = kFALSE; }
  void DisableZDC()   { fIsZDCEnabled   = kFALSE; }
  void DisableCascades() { fAreCascadesEnabled = kFALSE; }
  void DisableV0s() { fAreV0sEnabled = kFALSE; }
  void DisableKinks() { fAreKinksEnabled = kFALSE; }
  void DisableTracks() { fAreTracksEnabled = kFALSE; }
  void DisablePmdClusters() { fArePmdClustersEnabled = kFALSE; }
  void DisableCaloClusters() { fAreCaloClustersEnabled = kFALSE; }
  void DisableCells() { fAreEMCALCellsEnabled = fArePHOSCellsEnabled = kFALSE; }
  void DisableCaloTrigger(TString calo = "PHOS") { if (calo.Contains("EMCAL")) fAreEMCALTriggerEnabled = kFALSE; else fArePHOSTriggerEnabled = kFALSE; }
  void DisableTracklets() { fAreTrackletsEnabled = kFALSE; }
  void DisableHMPID()  { fIsHMPIDEnabled = kFALSE; } 

  void EnableV0CascadeVerticesReco() { fIsV0CascadeRecoEnabled = kTRUE; }

  void SetPropagateTrackToEMCal(Bool_t propagate) {fDoPropagateTrackToEMCal = propagate;}

  virtual void SetTimeZeroType(IlcESDpid::EStartTimeType_t tofTimeZeroType) {fTimeZeroType = tofTimeZeroType;}
  
private:
    IlcAnalysisTaskESDfilter(const IlcAnalysisTaskESDfilter&);
    IlcAnalysisTaskESDfilter& operator=(const IlcAnalysisTaskESDfilter&);
    void PrintMCInfo(IlcStack *pStack,Int_t label); // for debugging
    Double_t Chi2perNDF(IlcESDtrack* track);
    
  IlcAODHeader* ConvertHeader(const IlcESDEvent& esd);
  void ConvertCascades(const IlcESDEvent& esd);
  void ConvertV0s(const IlcESDEvent& esd);
  void ConvertKinks(const IlcESDEvent& esd);
  void ConvertPrimaryVertices(const IlcESDEvent& esd);
  void ConvertTracks(const IlcESDEvent& esd);
  void ConvertPmdClusters(const IlcESDEvent& esd);
  void ConvertCaloClusters(const IlcESDEvent& esd);
  void ConvertEMCALCells(const IlcESDEvent& esd);
  void ConvertPHOSCells(const IlcESDEvent& esd);
  void ConvertCaloTrigger(TString calo, const IlcESDEvent& esd);
  void ConvertTracklets(const IlcESDEvent& esd);
  void ConvertTPCOnlyTracks(const IlcESDEvent& esd);
  void ConvertGlobalConstrainedTracks(const IlcESDEvent& esd);
  void ConvertVZERO(const IlcESDEvent& esd);
  void ConvertTZERO(const IlcESDEvent& esd);
  void ConvertZDC(const IlcESDEvent& esd);
  Int_t ConvertHMPID(const IlcESDEvent& esd);
  void PropagateTrackToEMCal(IlcESDtrack *esdTrack);

  TClonesArray& Tracks();
  TClonesArray& V0s();
  TClonesArray& Vertices();
  TClonesArray& Cascades();
  
  // Filtering
  IlcAnalysisFilter* fTrackFilter;      //  Track   Filter
  IlcAnalysisFilter* fKinkFilter;       //  Kink    Filter
  IlcAnalysisFilter* fV0Filter;         //  V0      Filter
  IlcAnalysisFilter* fCascadeFilter;    //  Cascade Filter
  // PID
  Double_t     fHighPthreshold;    //  Pt threshold for detector signal setting
  TF1 *        fPtshape;           //  Pt spectrum distribution
  Bool_t       fEnableFillAOD;     //  value that decides if this task activates AOD filling
  Bool_t* fUsedTrack; //! indices of used tracks
  Bool_t* fUsedKink; //! indices of used kinks
  Bool_t* fUsedV0; //! indices of used V0s
  TRefArray* fAODTrackRefs; // array of track references
  TRefArray* fAODV0VtxRefs; // array of v0 vertices references
  TRefArray* fAODV0Refs ; // array of v0s references
  IlcMCEventHandler* fMChandler; // pointer to MC handler (if any)
  Int_t fNumberOfTracks; // current number of tracks
  Int_t fNumberOfPositiveTracks; // current number of positive tracks
  Int_t fNumberOfV0s; // current number of v0s
  Int_t fNumberOfVertices; // current number of vertices
  Int_t fNumberOfCascades; // current number of cascades
  Int_t fNumberOfKinks; // current number of kinks
  Bool_t fOldESDformat; // is the ESD in old format ?
  IlcAODVertex* fPrimaryVertex; // pointer to primary vertex of the event
  UInt_t fTPCConstrainedFilterMask;      //  Filter Mask used to select and store refitted TPC only tracks
  UInt_t fHybridFilterMaskTPCCG; //  Filter Mask used to mark global tracks as hybrid
  Bool_t fWriteHybridTPCCOnly;// write only the complent tracks not all global constrained

  UInt_t fGlobalConstrainedFilterMask; //  Filter Mask used to select and store refitted TPC only tracks
  UInt_t fHybridFilterMaskGCG;  //  Filter Mask used to mark global tracks as hybrid
  Bool_t fWriteHybridGCOnly;// write only the complent tracks not all global constrained

  Bool_t fIsVZEROEnabled; // whether or not to fill the vzero branch (true by default)
  Bool_t fIsTZEROEnabled; // whether or not to fill the tzero branch (true by default)
  Bool_t fIsZDCEnabled; // whether or not to fill the zdc branch (true by default)
  Bool_t fIsHMPIDEnabled; // whether or not to fill the hmpid branch (true by default) 
  Bool_t fIsV0CascadeRecoEnabled; // whether or not to reconstruct again V0s and cascades (false by default)
  Bool_t fAreCascadesEnabled; // whether or not to fill the cascades branch (true by default)
  Bool_t fAreV0sEnabled; // whether or not to fill the v0 branch (true by default)
  Bool_t fAreKinksEnabled; // whether or not to fill the kinks (true by default)
  Bool_t fAreTracksEnabled; // whether or not to fill the (central) tracks branch (true by default)
  Bool_t fArePmdClustersEnabled; // whether or not to fill the pmd clusters (true by default)
  Bool_t fAreCaloClustersEnabled; // whether or not to fill the calo clusters (true by default)
  Bool_t fAreEMCALCellsEnabled; // whether or not to fill the emcal cells (true by default)
  Bool_t fArePHOSCellsEnabled; // whether or not to fill the phos cells (true by default)
  Bool_t fAreEMCALTriggerEnabled; // whether or not to fill the emcal trigger (true by default)
  Bool_t fArePHOSTriggerEnabled; // whether or not to fill the phos trigger (true by default)
  Bool_t fAreTrackletsEnabled; // whether or not to fill the tracklets (true by default)
  IlcESDpid* fESDpid; // esd pid
  Bool_t fIsPidOwner; // whether we own fESDpid
  Int_t fTimeZeroType;  //  time zero type 
  IlcESDtrackCuts* fTPCaloneTrackCuts; // TPC stand-alone track cuts
  Double_t        fV0Cuts[7];       // Array to store the values for the different reco selections V0 related
  Double_t        fCascadeCuts[8];  // Array to store the values for the different reco selections cascades related
  Bool_t fDoPropagateTrackToEMCal;  // whether or not to propagate the tracks to EMCal surface (430cm) -- true by default
  
  
  ClassDef(IlcAnalysisTaskESDfilter, 15); // Analysis task for standard ESD filtering
};

#endif

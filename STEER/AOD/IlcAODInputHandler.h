#ifndef ILCAODINPUTHANDLER_H
#define ILCAODINPUTHANDLER_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcAODInputHandler.h 53658 2011-12-16 11:37:42Z hristov $ */

//-------------------------------------------------------------------------
//     AOD Input Handler realisation of the IlcVEventHandler interface
//     Author: Andreas Morsch, CERN
//-------------------------------------------------------------------------

#include "IlcInputEventHandler.h"
#include "IlcAODEvent.h"
#include "IlcMCEvent.h"
class TList;
class IlcMCEvent;
class TH2F;
class IlcMCEvent;
class IlcAODpidUtil;
class IlcPIDResponse;


class IlcAODInputHandler : public IlcInputEventHandler {

 public:
    IlcAODInputHandler();
    IlcAODInputHandler(const char* name, const char* title);
    virtual ~IlcAODInputHandler();
    virtual Bool_t       Init(Option_t* /*opt*/) {return kTRUE;}
    virtual Bool_t       Init(TTree* tree, Option_t* opt);
    IlcAODEvent         *GetEvent() const {return fEvent;}
    IlcMCEvent          *MCEvent()  const {return fMCEvent;}
    virtual void         AddFriend(char* filename);
    virtual Bool_t       BeginEvent(Long64_t entry);
    virtual Bool_t       Notify() { return IlcVEventHandler::Notify();};
    virtual Bool_t       Notify(const char* path);
    virtual Bool_t       FinishEvent();
    Option_t            *GetDataType() const;
    // Get the statistics object (currently TH2). Option can be BIN0.
    virtual TObject     *GetStatistics(Option_t *option="") const;
    // Provisions for event merging
    void                 SetMergeEvents(Bool_t flag) {fMergeEvents = flag;}
    Bool_t               GetMergeEvents() const {return fMergeEvents;}
    IlcAODEvent*         GetEventToMerge() {return fAODEventToMerge;}
    TTree*               GetTreeToMerge()  const {return fTreeToMerge;}
    void                 SetMergeOffset(Int_t ioff) {fMergeOffset = ioff;}
    Int_t                GetMergeOffset()     const {return fMergeOffset;}
    void                 SetMergeTracks(Bool_t flag) {fMergeTracks = flag;}
    Bool_t               GetMergeTracks()      const {return fMergeTracks;}
    void                 SetMergeEMCALClusters(Bool_t flag) {fMergeEMCALClusters = flag;}
    Bool_t               GetMergeEMCALClusters()      const {return fMergeEMCALClusters;}
    void                 SetMergeEMCALCells(Bool_t flag)    {fMergeEMCALCells    = flag;}
    Bool_t               GetMergeEMCALCells()         const {return fMergeEMCALCells   ;} 
    void                 SetMergePHOSClusters(Bool_t flag) {fMergePHOSClusters   = flag;}
    Bool_t               GetMergePHOSClusters()      const {return fMergePHOSClusters  ;}
    void                 SetMergePHOSCells(Bool_t flag)    {fMergePHOSCells      = flag;}
    Bool_t               GetMergePHOSCells()         const {return fMergePHOSCells     ;}  
    void                 SetMergeEMCALTrigger(Bool_t flag)    {fMergeEMCALTrigger      = flag;}
    Bool_t               GetMergeEMCALTrigger()         const {return fMergeEMCALTrigger     ;}  
    void                 SetMergePHOSTrigger(Bool_t flag)    {fMergePHOSTrigger      = flag;}
    Bool_t               GetMergePHOSTrigger()         const {return fMergePHOSTrigger     ;}  

    //PID response
    virtual IlcPIDResponse* GetPIDResponse() {return (IlcPIDResponse*)fAODpidUtil;}
    virtual void CreatePIDResponse(Bool_t isMC=kFALSE);
    IlcAODpidUtil *GetAODpidUtil() const { return fAODpidUtil; }
  
 private:
    void ConnectFriends();
    IlcAODInputHandler(const IlcAODInputHandler& handler);             
    IlcAODInputHandler& operator=(const IlcAODInputHandler& handler);  
 private:
    IlcAODEvent    *fEvent;   //! Pointer to the event
    IlcMCEvent     *fMCEvent; //! Pointer to the MCEvent
    TList          *fFriends; //  List of friend trees
    IlcAODpidUtil  *fAODpidUtil; //! Pointer to PID information
  
// Support for merged events
    Bool_t          fMergeEvents;     // Flag for event merging
    Bool_t          fMergeTracks;        // Merge tracks
    Bool_t          fMergeEMCALClusters; // Merge PHOS  cluster
    Bool_t          fMergePHOSClusters;  // Merge EMCAL cluster
    Bool_t          fMergeEMCALCells;    // Merge PHOS  cluster
    Bool_t          fMergePHOSCells;     // Merge EMCAL cluster
    Bool_t          fMergeEMCALTrigger;    // Merge EMCAL  cluster
    Bool_t          fMergePHOSTrigger;     // Merge PHOS cluster
    Bool_t          fFriendsConnected;// Friends are connected
    TFile          *fFileToMerge;     //! File for merging
    TTree          *fTreeToMerge;     //! Tree for merging
    IlcAODEvent    *fAODEventToMerge; //! Event for merging
    Int_t           fMergeOffset;     //! Event offset for merging
    TH2F*           fHistStatistics[2]; //! how many events are cut away why {all,bin 0}
    ClassDef(IlcAODInputHandler, 3);
};

#endif

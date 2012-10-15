#ifndef ILCANALYSISTASKSE_H
#define ILCANALYSISTASKSE_H
 
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id$ */

#include "IlcAnalysisTask.h"
#include "IlcVEvent.h"

class IlcAODEvent;
class IlcAODHeader;
class IlcAODTracklets;
class IlcAODCaloCells;
class IlcAODCaloTrigger;
class IlcMCEvent;
class IlcMCEventHandler;
class IlcInputEventHandler;
class IlcMultiInputEventHandler;
class IlcAnalysisCuts;
class IlcESDfriend;
class IlcEventTag;

class TTree;
class TList;


class IlcAnalysisTaskSE : public IlcAnalysisTask
{
 public:
    IlcAnalysisTaskSE();
    IlcAnalysisTaskSE(const char* name);
    IlcAnalysisTaskSE(const IlcAnalysisTaskSE& obj);
    IlcAnalysisTaskSE& operator=(const IlcAnalysisTaskSE& other);
    virtual ~IlcAnalysisTaskSE() {;}
    // Implementation of interface methods
    virtual void   ConnectInputData(Option_t *option = "");
    virtual void   CreateOutputObjects();
    virtual void   Exec(Option_t* option);
    virtual void   SetDebugLevel(Int_t level) {fDebug = level;}
    virtual void   Init() {;}
    virtual Bool_t Notify();
    // To be implemented by user
    virtual void   UserCreateOutputObjects()  {;}
    virtual void   UserExec(Option_t* /*option*/) {;}
    virtual void   UserExecMix(Option_t */*option*/) {;}
    virtual Bool_t UserNotify() {return kTRUE;}
    virtual void   NotifyRun()  {;}
    
    // Helpers for adding branches to the AOD
    virtual void   AddAODBranch(const char* cname, void* addobj, const char *fname="");
    // Event Selection
    virtual void   SelectCollisionCandidates(UInt_t offlineTriggerMask = IlcVEvent::kMB) {fOfflineTriggerMask = offlineTriggerMask;}
    // Loading the declared input branches
    void           LoadBranches() const;
 // Getters
    virtual Int_t         DebugLevel()  {return fDebug;     }
    virtual IlcVEvent*    InputEvent()  {return fInputEvent;}
    virtual IlcESDfriend* ESDfriend()   {return fESDfriend; }
    virtual IlcAODEvent*  AODEvent()    {return fOutputAOD; }
    virtual TTree*        OutputTree()  {return fTreeA;     }
    virtual IlcMCEvent*   MCEvent()     {return fMCEvent;   }
    virtual Long64_t      Entry()       {return fEntry;     }
    virtual const IlcEventTag *EventTag() const;
    virtual const char*   CurrentFileName();
    virtual Bool_t        IsStandardAOD() const;
    virtual TList*        GetQAHistos()   const {return fHistosQA;}
    virtual Bool_t        IsEventInBinZero() { return kFALSE;}
    virtual UInt_t        GetCollisionCandidates() const { return fOfflineTriggerMask;}
 protected:
    void ConnectMultiHandler();
    void DisconnectMultiHandler();

  protected:
    Int_t                 fDebug;           //  Debug flag
    // IO
    Int_t                 fEntry;           //  Current entry in the chain
    IlcVEvent*            fInputEvent;      //! VEvent Input
    IlcESDfriend*         fESDfriend;       //! ESD friend
    IlcInputEventHandler* fInputHandler;    //! Input Handler
    IlcAODEvent*          fOutputAOD;       //! AOD out 
    IlcMCEvent*           fMCEvent;         //! MC
    TTree*                fTreeA;           //  AOD output Tree
    Int_t                 fCurrentRunNumber;//! Current run number
    // Output histos for QA
    TList*                fHistosQA;        //! Output histos for QA
    // Provisions for replication
    static IlcAODHeader*    fgAODHeader;        //! Header for replication
    static TClonesArray*    fgAODTracks;        //! Tracks for replication
    static TClonesArray*    fgAODVertices;      //! Vertices for replication
    static TClonesArray*    fgAODV0s;           //! V0s for replication
    static TClonesArray*    fgAODPMDClusters;   //! PMDClusters for replication
    static TClonesArray*    fgAODJets;          //! Jets for replication
    static TClonesArray*    fgAODFMDClusters;   //! FMDClusters for replication
    static TClonesArray*    fgAODCaloClusters;  //! CaloClusters for replication
    static IlcAODCaloTrigger* fgAODEMCALTrigger; //! Emcal Trigger for replication
    static IlcAODCaloTrigger* fgAODPHOSTrigger;  //! Phos Trigger for replication
    static TClonesArray*    fgAODMCParticles;   //! MC Particles for replicatio
    static IlcAODTracklets* fgAODTracklets;     //! Tracklets for replication
    static IlcAODCaloCells* fgAODEmcalCells;    //! Emcal Cell replication
    static IlcAODCaloCells* fgAODPhosCells;     //! Phos  Cell replication
    static TClonesArray*    fgAODDimuons;       //! Dimuons replication
    static TClonesArray*    fgAODHmpidRings;    //! HMPID replication
    // Event Selection
    UInt_t fOfflineTriggerMask;   //  Task processes collision candidates only
    // Event Mixing
    IlcMultiInputEventHandler *fMultiInputHandler;  //! pointer to multihandler
    IlcMCEventHandler         *fMCEventHandler;     //! pointer to MCEventHandler
    ClassDef(IlcAnalysisTaskSE, 4); // Analysis task for standard jet analysis
};
 
#endif

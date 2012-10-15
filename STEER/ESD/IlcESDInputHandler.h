#ifndef ILCESDINPUTHANDLER_H
#define ILCESDINPUTHANDLER_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcESDInputHandler.h 58336 2012-08-28 15:07:04Z hristov $ */

//-------------------------------------------------------------------------
//     ESD Input Handler realisation of the IlcVEventHandler interface
//     Author: Andreas Morsch, CERN
//-------------------------------------------------------------------------

#include "IlcInputEventHandler.h"
#include "IlcESDEvent.h"
class TChain;
class TTree;
class IlcRunTag;
class IlcEventTag;
class TMap;
class IlcESDfriend;
class IlcESDpid;
class IlcESDEvent;
class IlcPIDResponse;


class IlcESDInputHandler : public IlcInputEventHandler {

 public:
    IlcESDInputHandler();
    IlcESDInputHandler(const char* name, const char* title);
    virtual ~IlcESDInputHandler();
    virtual Bool_t       Init(Option_t* opt) {return IlcInputEventHandler::Init(opt);}
    virtual Bool_t       Init(TTree* tree, Option_t* opt);
    virtual Bool_t       BeginEvent(Long64_t entry);
    virtual Bool_t       Notify() { return IlcInputEventHandler::Notify(); };
    virtual Bool_t       Notify(const char* path);
    virtual Bool_t       FinishEvent();
    void                 CheckSelectionMask();
    IlcESDEvent         *GetEvent()        const {return fEvent;}
    Option_t            *GetAnalysisType() const {return fAnalysisType;}
    Option_t            *GetDataType() const;
    // Tag cut summary analysis
    Int_t                GetNEventAcceptedInFile();
    Int_t                GetNEventRejectedInFile();
    Bool_t               GetCutSummaryForChain(Int_t *aTotal, Int_t *aAccepted, Int_t *aRejected);
    Int_t                GetNFilesEmpty();
    // HLT  analysis
    IlcESDEvent         *GetHLTEvent()     const {return fHLTEvent;}
    TTree               *GetHLTTree()      const {return fHLTTree;}    
    void                 SetReadHLT()            {Changed(); fUseHLT = kTRUE;}
    Bool_t               GetReadHLT()      const {return fUseHLT;}
    // Friends&Co
    IlcESDfriend        *GetESDfriend()    const {return fFriend;}
    void                 SetReadFriends(Bool_t flag)   {Changed(); fReadFriends = flag;}
    Bool_t               GetReadFriends()  const {return fReadFriends;}
    void                 SetFriendFileName(const char *fname)  {Changed(); fFriendFileName = fname;}
    const char          *GetFriendFileName() const {return fFriendFileName;}
    // Tag analysis
    void                 SetReadTags()           {Changed(); fUseTags = kTRUE;}
    IlcRunTag           *GetRunTag() const       {return fRunTag;}
    const IlcEventTag   *GetEventTag() const     {return fEventTag;}
    // Get the statistics object (currently TH2). Option can be BIN0.
    virtual TObject     *GetStatistics(Option_t *option="") const;

    //PID response
    virtual IlcPIDResponse* GetPIDResponse() {return (IlcPIDResponse*)fESDpid;}
    virtual void CreatePIDResponse(Bool_t isMC=kFALSE);
    IlcESDpid           *GetESDpid()       const {return fESDpid;}
    void                 SetESDpid(IlcESDpid* pid)     {Changed(); fESDpid = pid;}
  
 private:
    IlcESDInputHandler(const IlcESDInputHandler& handler);             
    IlcESDInputHandler& operator=(const IlcESDInputHandler& handler);  
 protected:
    // ESD event
    IlcESDEvent    *fEvent;         //! Pointer to the event
    IlcESDfriend   *fFriend;        //! Pointer to the esd friend
    IlcESDpid      *fESDpid;        //! Pointer to PID information
    Option_t       *fAnalysisType;  //! local, proof, grid
    Int_t           fNEvents;       //! Number of events in the current tree
    // HLT event
    IlcESDEvent    *fHLTEvent;      //! Pointer to the HLT Event (if present)
    TTree          *fHLTTree;       //! Pointer to the HLT Event (if present)
    Bool_t          fUseHLT;        //  Flag to access HLT Events
    // ESD Tag Cut Summary
    TMap           *fTagCutSumm;    //! Tag cut summary map
    // ESD Tags (optional)
    Bool_t          fUseTags;       //  Flag to use tags
    TChain         *fChainT;        //! File with event tags
    TTree          *fTreeT;         //! Tree of tags
    IlcRunTag      *fRunTag;        //! Pointer to the run tag
    const IlcEventTag *fEventTag;      //! Current event tag
    // Friends
    Bool_t          fReadFriends;   //  Flag for friends reading 
    TString         fFriendFileName;//  Name of the file containing the frien tree (branch)
    ClassDef(IlcESDInputHandler, 6);
};

#endif

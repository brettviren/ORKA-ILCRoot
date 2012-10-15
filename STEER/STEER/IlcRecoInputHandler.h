#ifndef ILCRECOINPUTHANDLER_H
#define ILCRECOINPUTHANDLER_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

//-------------------------------------------------------------------------
//     Reconstruction-specific input handler
//     Author: Andrei Gheata, CERN
//-------------------------------------------------------------------------

#ifndef ILCESDINPUTHANDLER_H
#include "IlcESDInputHandler.h"
#endif

class IlcReconstruction;

class IlcRecoInputHandler : public IlcESDInputHandler {

 public:
    IlcRecoInputHandler() {}
    IlcRecoInputHandler(const char* name, const char* title);
    virtual ~IlcRecoInputHandler() {}
    virtual Bool_t       Notify() { return IlcESDInputHandler::Notify(); };
    virtual Bool_t       Notify(const char *) {return kTRUE;}
    virtual Bool_t       Init(Option_t* opt) {return IlcESDInputHandler::Init(opt);}
    virtual Bool_t       Init(TTree* tree, Option_t* opt="LOCAL");
    virtual Bool_t       BeginEvent(Long64_t entry);
    virtual Bool_t       FinishEvent() {return kTRUE;}
//    void                 CheckSelectionMask();
//    IlcESDEvent         *GetEvent()        const {return fEvent;}
//    Option_t            *GetAnalysisType() const {return fAnalysisType;}
//    Option_t            *GetDataType() const;
    // Tag cut summary analysis
//    Int_t                GetNEventAcceptedInFile();
//    Int_t                GetNEventRejectedInFile();
//    Bool_t               GetCutSummaryForChain(Int_t *aTotal, Int_t *aAccepted, Int_t *aRejected);
//    Int_t                GetNFilesEmpty();
    // HLT  analysis
//    IlcESDEvent         *GetHLTEvent()     const {return fHLTEvent;}
//    TTree               *GetHLTTree()      const {return fHLTTree;}    
//    void                 SetReadHLT()            {fUseHLT = kTRUE;}
    // Friends&Co
//    IlcESDfriend        *GetESDfriend()    const {return fFriend;}
//    void                 SetReadFriends(Bool_t flag)   {fReadFriends = flag;}
//    void                 SetFriendFileName(const char *fname)  {fFriendFileName = fname;}
    // Tag analysis
//    void                 SetReadTags()           {fUseTags = kTRUE;}
//    IlcRunTag           *GetRunTag() const       {return fRunTag;}
//    const IlcEventTag   *GetEventTag() const     {return fEventTag;}
    // Get the statistics object (currently TH2). Option can be BIN0.
//    virtual TObject     *GetStatistics(Option_t *option="") const;

    //PID response
//    virtual IlcPIDResponse* GetPIDResponse() {return (IlcPIDResponse*)fESDpid;}
//    virtual void CreatePIDResponse(Bool_t isMC=kFALSE);
//    IlcESDpid           *GetESDpid()       const {return fESDpid;}
//    void                 SetESDpid(IlcESDpid* pid)     {fESDpid = pid;}
      
 private:
    IlcRecoInputHandler(const IlcESDInputHandler& handler);             
    IlcRecoInputHandler& operator=(const IlcESDInputHandler& handler);  
    // Private setters used by IlcReconstruction
    friend class IlcReconstruction;
    void                 SetEvent(IlcESDEvent *event)          {fEvent = event;}
    void                 SetESDfriend(IlcESDfriend *esdfriend) {fFriend = esdfriend;}
    void                 SetHLTEvent(IlcESDEvent *hltevent)    {fHLTEvent = hltevent;}
    void                 SetHLTTree(TTree *hlttree)            {fHLTTree = hlttree;}
    
    ClassDef(IlcRecoInputHandler, 1);
};

#endif

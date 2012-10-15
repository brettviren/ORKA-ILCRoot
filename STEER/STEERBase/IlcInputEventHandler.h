#ifndef ILCINPUTEVENTHANDLER_H
#define ILCINPUTEVENTHANDLER_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcInputEventHandler.h 58336 2012-08-28 15:07:04Z hristov $ */

//-------------------------------------------------------------------------
//     Input Handler realisation of the IlcVEventHandler interface
//     Author: Andreas Morsch, CERN
//-------------------------------------------------------------------------

#include "IlcVEventHandler.h"
#include <TTree.h>


class IlcVEvent;
class IlcVCuts;
class IlcRunTag;
class IlcEventTag;
class IlcPIDResponse;

class IlcInputEventHandler : public IlcVEventHandler {

 public:
 enum EInputHandlerFlags {
    kUserCallSelectionMask = BIT(14), // Watch out for defining base class bits
    kCheckStatistics       = BIT(15)
 };
    IlcInputEventHandler();
    IlcInputEventHandler(const char* name, const char* title);
    virtual ~IlcInputEventHandler();
    virtual void         SetOutputFileName(const char* /*fname*/) {;}
    virtual const char  *GetOutputFileName()                          {return 0;}
    virtual Bool_t       Init(Option_t* opt) {if(fMixingHandler) fMixingHandler->Init(opt);return kTRUE;}
    virtual Bool_t       Init(TTree* tree, Option_t* opt) {if(fMixingHandler) fMixingHandler->Init(tree,opt);return kTRUE;}
    virtual Bool_t       GetEntry() {if(fMixingHandler) fMixingHandler->GetEntry(); return kTRUE;}
    virtual Bool_t       BeginEvent(Long64_t entry) {if(fMixingHandler) fMixingHandler->BeginEvent(entry);return kTRUE;}

    virtual Bool_t       Notify()      { return IlcVEventHandler::Notify();}
    virtual Bool_t       Notify(const char *path) {if(fMixingHandler) fMixingHandler->Notify(path);return kTRUE;}
    virtual Bool_t       FinishEvent() {if(fMixingHandler) fMixingHandler->FinishEvent();return kTRUE;}        
    virtual Bool_t       Terminate()   {if(fMixingHandler) fMixingHandler->Terminate();return kTRUE;}
    virtual Bool_t       TerminateIO() {if(fMixingHandler) fMixingHandler->TerminateIO();return kTRUE;}

    // Setters
    virtual void         SetInputTree(TTree* tree)                    {fTree = tree;}
    virtual void         SetEventSelection(IlcVCuts* cuts)            {if (fEventCuts) Changed(); fEventCuts = cuts;}
    virtual void         SetUserCallSelectionMask(Bool_t flag=kTRUE)  {TObject::SetBit(kUserCallSelectionMask,flag);}
    virtual void         SetCheckStatistics(Bool_t flag=kTRUE)        {Changed(); TObject::SetBit(kCheckStatistics,flag);}
    //
    void SetInactiveBranches(const char* branches) {Changed(); fBranches   = branches;}
    void SetActiveBranches  (const char* branches) {Changed(); fBranchesOn = branches;}
     // Getters
    virtual IlcVEvent   *GetEvent()        const                      {return 0;}
    virtual const IlcEventTag   *GetEventTag() const                  {return 0;}
    virtual IlcRunTag   *GetRunTag()       const                      {return 0;}
    // Get the statistics object (currently TH2). Option can be BIN0.
    virtual TObject     *GetStatistics(Option_t *option="") const;
    virtual Option_t    *GetAnalysisType() const                      {return 0;}
    virtual TTree       *GetTree( )        const                      {return fTree;}
    virtual IlcVCuts    *GetEventSelection() const                    {return fEventCuts;}
    virtual Long64_t     GetReadEntry()    const;
    virtual Bool_t       IsCheckStatistics() const                    {return TObject::TestBit(kCheckStatistics);}
    virtual Bool_t       IsUserCallSelectionMask() const              {return TObject::TestBit(kUserCallSelectionMask);}
    virtual Bool_t       NewEvent()
	{Bool_t ne = fNewEvent; fNewEvent = kFALSE; return ne;}
    virtual UInt_t       IsEventSelected() 
        {return fIsSelectedResult;}
    // Mixing
    void SetMixingHandler(IlcInputEventHandler* mixing) {Changed(); fMixingHandler = mixing;}
    IlcInputEventHandler* MixingHandler()               {return fMixingHandler;}
    // Parent Handler
    void SetParentHandler(IlcInputEventHandler* parent) {Changed(); fParentHandler = parent;}
    IlcInputEventHandler* ParentHandler()               {return fParentHandler;}

    //PID response
    virtual IlcPIDResponse* GetPIDResponse() {return 0x0;}
    virtual void CreatePIDResponse(Bool_t /*isMC*/=kFALSE) {;}
  
 protected:
    void SwitchOffBranches() const;
    void SwitchOnBranches()  const;
 private:
    IlcInputEventHandler(const IlcInputEventHandler& handler);             
    IlcInputEventHandler& operator=(const IlcInputEventHandler& handler);  
 protected:
    TTree          *fTree;         //! Pointer to the tree
    TString         fBranches;     //  List of branches to be switched off (separated by space)
    TString         fBranchesOn;   //  List of branches to be switched on  (separated by space)
    Bool_t          fNewEvent;     //  New event flag 
    IlcVCuts*       fEventCuts;    //  Cuts on the event level
    UInt_t          fIsSelectedResult; //  Selection result
    IlcInputEventHandler* fMixingHandler; // Optionla plugin for mixing
    IlcInputEventHandler* fParentHandler; // optional pointer to parent handlers (used in IlcMultiInputEventHandler)
    ClassDef(IlcInputEventHandler, 6);
};

#endif

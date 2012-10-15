#ifndef ILCMULTIEVENTINPUTHANDLER_H
#define ILCMULTIEVENTINPUTHANDLER_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id$ */

//----------------------------------------------------------------------------
//     Multi VEvent Input Handler realisation of the IlcVEventHandler interface.
//     This class handles multiple events for mixing.
//     Author: Andreas Morsch, CERN
//----------------------------------------------------------------------------

#include "IlcInputEventHandler.h"
class IlcVEventPool;
class IlcVEvent;

class IlcMultiEventInputHandler : public IlcInputEventHandler {

 public:
    IlcMultiEventInputHandler();
    IlcMultiEventInputHandler(Int_t size, Int_t format = 1);
    IlcMultiEventInputHandler(const char* name, const char* title, Int_t size, Int_t format = 1);
    virtual ~IlcMultiEventInputHandler();
    void   SetBufferSize(Int_t size) {fBufferSize = size;}
    void   SetEventPool(IlcVEventPool* pool) {fEventPool = pool;}
    Int_t  GetBufferSize()           const {return fBufferSize;}
    Int_t  GetNBuffered()            const {return fNBuffered;}
    Bool_t IsBufferReady()           const {return (fNBuffered >= (fBufferSize -1));}
    Bool_t IsFreshBuffer()           const {return (fIndex == (fBufferSize - 1));}
    IlcVEventPool           *GetEventPool()      const {return fEventPool;}
    virtual IlcVEvent       *GetEvent()          const {return GetLatestEvent();}
    virtual IlcVEvent       *GetEvent(Int_t iev) const;
    IlcVEvent               *GetLatestEvent()    const {return fEventBuffer[fIndex];}
    Int_t                    GetFormat() { return fFormat ;} 
    void                     EventSkipped() {fEventSkipped = kTRUE;}
    // From the interface
    virtual Bool_t Init(Option_t* /*opt*/)    {return kTRUE;}
    virtual Bool_t Init(TTree* tree, Option_t* /*opt*/);
    virtual Bool_t FinishEvent();
    virtual Bool_t BeginEvent(Long64_t /*entry*/);
    virtual Bool_t Notify() { return IlcInputEventHandler::Notify();}
    virtual Bool_t Notify(const char */*path*/);
 private:
    IlcMultiEventInputHandler(const IlcMultiEventInputHandler& handler);             
    IlcMultiEventInputHandler& operator=(const IlcMultiEventInputHandler& handler);  
 private:
    Int_t          fBufferSize;   // Size of the buffer
    Int_t          fFormat;       // 0: ESD 1: AOD
    Int_t          fNBuffered;    // Number of events actually buffered
    Int_t          fIndex;        // Pointer to most recent event
    Int_t          fCurrentBin;   // Current bin from the pool
    Int_t          fCurrentEvt;   // Current event
    Bool_t         fInit;         // Current event
    IlcVEventPool* fEventPool;    // Pointer to the pool
    IlcVEvent**    fEventBuffer;  //! The event buffer
    Bool_t         fEventSkipped; // User requires event to be skip
    ClassDef(IlcMultiEventInputHandler, 1);
};

#endif

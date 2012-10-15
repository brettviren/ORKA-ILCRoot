//
// Class IlcMixEventInputHandler
//
// Mixing input handler prepare N events before UserExec
// TODO example
// author:
//        Martin Vala (martin.vala@cern.ch)
//

#ifndef ILCMIXEVENTINPUTHANDLER_H
#define ILCMIXEVENTINPUTHANDLER_H

#include <TObjArray.h>

#include "IlcInputEventHandler.h"
#include <TArrayI.h>

class TChain;
class IlcMixEventPool;
class IlcMixInputHandlerInfo;
class IlcMixEventInputHandler : public IlcInputEventHandler {

public:
   IlcMixEventInputHandler(const Int_t size=1);
   virtual ~IlcMixEventInputHandler() {;};

   // From the interface
   virtual Bool_t  Init(Option_t *opt /*opt*/);
   virtual Bool_t  Init(TTree *tree, Option_t* /*opt*/);
   virtual Bool_t  BeginEvent(Long64_t entry /*entry*/);
   virtual Bool_t  GetEntry();
   virtual Bool_t  FinishEvent();
   virtual Bool_t  Notify();
   virtual Bool_t  Notify(const char *path);


   void SetInputHandlerForMixing(const IlcInputEventHandler *const inHandler);
   void SetEventPool(IlcMixEventPool *const evPool) {fEventPool = evPool;}

   IlcInputEventHandler *InputEventHandler(const Int_t index);
   IlcMixEventPool *GetEventPool() const { return fEventPool;}
   Int_t           BufferSize() const {return fBufferSize;}
   Int_t           MixedEventNumber() const {return fMixEventNumber;}
   Long64_t        EntryAll() const { return fEntryCounter;}
protected:

   Int_t             fBufferSize;          // Size of the buffer
   TObjArray         fInputHandlers;       // buffer of input handlers
   TObjArray         fMixTrees;            // buffer of input handlers
   TArrayI           fTreeMap;             // tree map
   IlcMixInputHandlerInfo *fMixIntupHandlerInfoTmp;    //! mix input handler info full chain
   Long64_t          fEntryCounter;        // entry counter
   IlcMixEventPool  *fEventPool;           // event pool

   Int_t             fMixEventNumber;      // number mix

private:

   IlcMixEventInputHandler(const IlcMixEventInputHandler &handler);
   IlcMixEventInputHandler &operator=(const IlcMixEventInputHandler &handler);

   ClassDef(IlcMixEventInputHandler, 1)
};

#endif

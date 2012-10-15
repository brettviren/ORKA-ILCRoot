//
// Class IlcMultiInputEventHandler
//
// Multi input event handler
// TODO example
// author:
//        Martin Vala (martin.vala@cern.ch)
//

#ifndef ILCMULTIINPUTEVENTHANDLER_H
#define ILCMULTIINPUTEVENTHANDLER_H

#include <TObjArray.h>

#include "IlcInputEventHandler.h"

class IlcMCEventHandler;
class IlcMultiInputEventHandler : public IlcInputEventHandler {

public:
   IlcMultiInputEventHandler(const Int_t size = 0, const char *name = "name");
   virtual ~IlcMultiInputEventHandler();

   // From the interface
   virtual Bool_t  Init(Option_t *opt);
   virtual Bool_t  Init(TTree *tree, Option_t *opt);
   virtual Bool_t  BeginEvent(Long64_t entry);
   virtual Bool_t  GetEntry();
   virtual Bool_t  FinishEvent();
   virtual Bool_t  Notify();
   virtual Bool_t  Notify(const char *path);
   virtual UInt_t  IsEventSelected();
   // add input handler
   virtual void                AddInputEventHandler(IlcVEventHandler*inHandler);
   IlcVEventHandler           *InputEventHandler(const Int_t index);
   Int_t                       BufferSize() const { return fBufferSize; }
   TObjArray                  *InputEventHandlers() { return &fInputHandlers; }

   IlcInputEventHandler       *GetFirstInputEventHandler();
   IlcMCEventHandler          *GetFirstMCEventHandler();
   IlcMultiInputEventHandler  *GetFirstMultiInputHandler();

   Option_t                   *GetDataType() const;
   
   //PID response
   virtual IlcPIDResponse* GetPIDResponse();
   virtual void CreatePIDResponse(Bool_t isMC);

protected:

   Int_t                   fBufferSize;    // Size of the buffer
   TObjArray               fInputHandlers; // buffer of input handlers
   Option_t               *fAnalysisType;  //! local, proof, grid
private:
   IlcMultiInputEventHandler(const IlcMultiInputEventHandler& handler);
   IlcMultiInputEventHandler &operator=(const IlcMultiInputEventHandler &handler);

   ClassDef(IlcMultiInputEventHandler, 1)
};

#endif

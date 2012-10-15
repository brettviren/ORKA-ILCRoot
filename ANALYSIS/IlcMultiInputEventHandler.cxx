//
// Class IlcMultiInputEventHandler
//
// Multi input event handler
// TODO example
// author:
//        Martin Vala (martin.vala@cern.ch)
//

#include "IlcLog.h"
#include "IlcMCEventHandler.h"

#include "IlcMultiInputEventHandler.h"

ClassImp(IlcMultiInputEventHandler)

static Option_t *gCurrentMultiDataType = "ESD";

//_____________________________________________________________________________
IlcMultiInputEventHandler::IlcMultiInputEventHandler(const Int_t size, const char *name) :
   IlcInputEventHandler(name, name),
   fBufferSize(size),
   fInputHandlers(),
   fAnalysisType(0)
{
//
// Default constructor.
//
   IlcDebug(IlcLog::kDebug + 10, "<-");
   fInputHandlers.SetOwner(kTRUE);
   IlcDebug(IlcLog::kDebug + 10, "->");
}

//_____________________________________________________________________________
IlcMultiInputEventHandler::~IlcMultiInputEventHandler()
{
   //
   // Destructor
   //
   IlcDebug(IlcLog::kDebug + 10, "<-");
   IlcDebug(IlcLog::kDebug + 10, "->");
}


//_____________________________________________________________________________
IlcVEventHandler *IlcMultiInputEventHandler::InputEventHandler(const Int_t index)
{
   //
   // Returns input handler
   //
   IlcDebug(IlcLog::kDebug + 5, Form("<-"));
   if ((index >= 0) && (index < fBufferSize)) {
      return (IlcVEventHandler *) fInputHandlers.At(index);
   }
   IlcDebug(IlcLog::kDebug + 5, Form("->"));
   return 0;
}

//_____________________________________________________________________________
void IlcMultiInputEventHandler::AddInputEventHandler(IlcVEventHandler*inHandler)
{
   //
   // Create N (fBufferSize) copies of input handler
   //
   if (inHandler->InheritsFrom("IlcESDInputHandler")) gCurrentMultiDataType = "ESD";
   if (inHandler->InheritsFrom("IlcAODInputHandler")) gCurrentMultiDataType = "AOD";
   IlcDebug(IlcLog::kDebug + 5, Form("<-"));
   IlcDebug(IlcLog::kDebug + 5, Form("Creating %d input event handlers ...", fBufferSize));
   IlcDebug(IlcLog::kDebug + 5, Form("Adding input handler with index %d ...", fBufferSize));
   fInputHandlers.Add(inHandler);
   fBufferSize++;
   IlcDebug(IlcLog::kDebug + 5, Form("->"));
}

//_____________________________________________________________________________
Bool_t IlcMultiInputEventHandler::Init(Option_t *opt)
{
   //
   // Init() is called for all mix input handlers.
   //
   fAnalysisType = opt;
   IlcDebug(IlcLog::kDebug + 5, Form("<- \"%s\"", opt));
   IlcInputEventHandler *eh = 0;
   TObjArrayIter next(&fInputHandlers);
   while ((eh = (IlcInputEventHandler *) next())) {
      eh->Init(fAnalysisType);
   }
   IlcDebug(IlcLog::kDebug + 5, Form("->"));
   return IlcInputEventHandler::Init(opt);
}
//_____________________________________________________________________________
Bool_t IlcMultiInputEventHandler::Init(TTree *tree, Option_t *opt)
{
   //
   // Init(const char*path) is called for all mix input handlers.
   // Create event pool if needed
   //
   fAnalysisType = opt;
   IlcDebug(IlcLog::kDebug + 5, Form("<- %p %s", (void *) tree, tree->GetName()));
   if (!tree) {
      IlcError(Form("-> tree is null"));
      return kFALSE;
   }
   IlcInputEventHandler *eh = 0;
   TObjArrayIter next(&fInputHandlers);
   while ((eh = (IlcInputEventHandler *) next())) {
      // using mixing input hadnler from Base class
      // for me fParentHandler would be better name
      eh->SetParentHandler(this);
      eh->Init(tree, fAnalysisType);
   }
   IlcDebug(IlcLog::kDebug + 5, Form("->"));
   return IlcInputEventHandler::Init(tree, opt);
}
//_____________________________________________________________________________
Bool_t IlcMultiInputEventHandler::Notify()
{
   //
   // Notify() is called for all mix input handlers
   //
   IlcDebug(IlcLog::kDebug + 5, Form("<-"));
   IlcInputEventHandler *eh = 0;
   TObjArrayIter next(&fInputHandlers);
   while ((eh = (IlcInputEventHandler *) next())) {
      eh->Notify();
   }
   IlcDebug(IlcLog::kDebug + 5, Form("->"));
   return IlcInputEventHandler::Notify();
}

//_____________________________________________________________________________
Bool_t IlcMultiInputEventHandler::Notify(const char *path)
{
   //
   // Notify(const char*path) is called for all mix input handlers
   //
   IlcDebug(IlcLog::kDebug + 5, Form("<- %s", path));
   IlcInputEventHandler *eh = 0;
   TObjArrayIter next(&fInputHandlers);
   while ((eh = (IlcInputEventHandler *) next())) {
      eh->Notify(path);
   }
   IlcDebug(IlcLog::kDebug + 5, Form("->"));
//   return IlcInputEventHandler::Notify(path);
   return IlcInputEventHandler::Notify(path);
}
//_____________________________________________________________________________
Bool_t IlcMultiInputEventHandler::BeginEvent(Long64_t entry)
{
   //
   // BeginEvent(Long64_t entry) is called for all mix input handlers
   //
   IlcDebug(IlcLog::kDebug + 5, Form("<- %lld", entry));
   IlcInputEventHandler *eh = 0;
   TObjArrayIter next(&fInputHandlers);
   while ((eh = (IlcInputEventHandler *) next())) {
      eh->BeginEvent(entry);
   }
   GetEntry();
   IlcDebug(IlcLog::kDebug + 5, Form("->"));
   return IlcInputEventHandler::BeginEvent(entry);
}


//_____________________________________________________________________________
Bool_t IlcMultiInputEventHandler::GetEntry()
{
   //
   // Sets correct events to every mix events
   //
   IlcDebug(IlcLog::kDebug + 5, Form("<-"));
   IlcInputEventHandler *eh = 0;
   TObjArrayIter next(&fInputHandlers);
   while ((eh = (IlcInputEventHandler *) next())) {
      eh->GetEntry();
   }
   IlcDebug(IlcLog::kDebug + 5, Form("->"));
   return IlcInputEventHandler::GetEntry();
}
//_____________________________________________________________________________
Bool_t IlcMultiInputEventHandler::FinishEvent()
{
   //
   // FinishEvent() is called for all mix input handlers
   //
   IlcDebug(IlcLog::kDebug + 5, Form("<-"));
   IlcInputEventHandler *eh = 0;
   TObjArrayIter next(&fInputHandlers);
   while ((eh = (IlcInputEventHandler *) next())) {
      eh->FinishEvent();
   }
   IlcDebug(IlcLog::kDebug + 5, Form("->"));
   return IlcInputEventHandler::FinishEvent();
}

IlcInputEventHandler *IlcMultiInputEventHandler::GetFirstInputEventHandler()
{
   //
   // Return first InputEventHandler
   //
   IlcDebug(IlcLog::kDebug + 5, Form("<-"));
   IlcVEventHandler *eh = 0;
   IlcInputEventHandler *handler = 0;
   TObjArrayIter next(&fInputHandlers);
   while ((eh = (IlcVEventHandler *) next())) {
      handler = dynamic_cast<IlcInputEventHandler *>(eh);
      if (handler) return handler;
   }
   IlcDebug(IlcLog::kDebug + 5, Form("->"));
   return 0;
}
IlcMCEventHandler *IlcMultiInputEventHandler::GetFirstMCEventHandler()
{
   //
   // Return first MCEventHandler
   //
   IlcDebug(IlcLog::kDebug + 5, Form("<-"));
   IlcVEventHandler *eh = 0;
   IlcMCEventHandler *handler = 0;
   TObjArrayIter next(&fInputHandlers);
   while ((eh = (IlcVEventHandler *) next())) {
      handler = dynamic_cast<IlcMCEventHandler *>(eh);
      if (handler) return handler;
   }
   IlcDebug(IlcLog::kDebug + 5, Form("->"));
   return 0;
}

IlcMultiInputEventHandler *IlcMultiInputEventHandler::GetFirstMultiInputHandler()
{
   //
   // Return first MultiInputHandler
   //
   IlcDebug(IlcLog::kDebug + 5, Form("<-"));
   IlcVEventHandler *eh = 0;
   IlcMultiInputEventHandler *handler = 0;
   TObjArrayIter next(&fInputHandlers);
   while ((eh = (IlcVEventHandler *) next())) {
      handler = dynamic_cast<IlcMultiInputEventHandler *>(eh);
      if (handler) return handler;
   }
   IlcDebug(IlcLog::kDebug + 5, Form("->"));
   return 0;
}

//______________________________________________________________________________
Option_t *IlcMultiInputEventHandler::GetDataType() const
{
   // Returns handled data type.
   return gCurrentMultiDataType;
}

//______________________________________________________________________________
UInt_t  IlcMultiInputEventHandler::IsEventSelected() 
{
  // returns if event is selected
  
  IlcInputEventHandler *firstIH = dynamic_cast<IlcInputEventHandler*> (GetFirstInputEventHandler());
  if (firstIH) {
    return firstIH->IsEventSelected();
  }
  
  return fIsSelectedResult;
}

//______________________________________________________________________________
IlcPIDResponse* IlcMultiInputEventHandler::GetPIDResponse()
{
   // retrieve PID response
   
   IlcInputEventHandler *firstIH = dynamic_cast<IlcInputEventHandler*> (GetFirstInputEventHandler());
   if (firstIH) {
      return firstIH->GetPIDResponse();
   }
   
   return 0x0;
}
   
//______________________________________________________________________________
void IlcMultiInputEventHandler::CreatePIDResponse(Bool_t isMC)
{
   // create PID response
   IlcInputEventHandler *firstIH = dynamic_cast<IlcInputEventHandler*> (GetFirstInputEventHandler());
   if (firstIH) {
      firstIH->CreatePIDResponse(isMC);
   }
}

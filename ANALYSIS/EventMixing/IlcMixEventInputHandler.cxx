//
// Class IlcMixEventInputHandler
//
// Mixing input handler prepare N events before UserExec
// TODO example
// author:
//        Martin Vala (martin.vala@cern.ch)
//

#include <TFile.h>
#include <TChain.h>
#include <TEntryList.h>
#include "IlcLog.h"
#include "IlcMixEventPool.h"
#include "IlcMixEventInputHandler.h"
#include "IlcAnalysisManager.h"

#include "IlcMixInputHandlerInfo.h"
#include <TChainElement.h>

ClassImp(IlcMixEventInputHandler)

//_____________________________________________________________________________
IlcMixEventInputHandler::IlcMixEventInputHandler(const Int_t size) :
   IlcInputEventHandler(),
   fBufferSize(size),
   fInputHandlers(),
   fMixTrees(),
   fTreeMap(size),
   fMixIntupHandlerInfoTmp(0),
   fEntryCounter(0),
   fEventPool(0),
   fMixEventNumber(0) {
//
// Default constructor.
//
   IlcDebug(IlcLog::kDebug + 10, "<-");
   IlcDebug(IlcLog::kDebug + 10, "->");
}

//_____________________________________________________________________________
IlcInputEventHandler *IlcMixEventInputHandler::InputEventHandler(const Int_t index) {
   //
   // Returns input handler
   //
   IlcDebug(IlcLog::kDebug, Form("<-"));
   if ((index >= 0) && (index < fBufferSize)) {
      IlcDebug(IlcLog::kDebug, Form("->"));
      return (IlcInputEventHandler *) fInputHandlers.At(index);
   }
   IlcDebug(IlcLog::kDebug, Form("->"));
   return 0;
}
//_____________________________________________________________________________
void IlcMixEventInputHandler::SetInputHandlerForMixing(const IlcInputEventHandler *const inHandler) {
   //
   // Create N (fBufferSize) copies of input handler
   //
   IlcDebug(IlcLog::kDebug, Form("<-"));
   IlcDebug(IlcLog::kDebug, Form("Creating %d input event handlers ...", fBufferSize));
   for (Int_t i = 0; i < fBufferSize; i++) {
      IlcDebug(IlcLog::kDebug + 5, Form("Adding %d ...", i));
      fInputHandlers.Add((IlcInputEventHandler *) inHandler->Clone());
   }
   IlcDebug(IlcLog::kDebug, Form("->"));
}
//_____________________________________________________________________________
Bool_t IlcMixEventInputHandler::Init(Option_t *opt) {
   //
   // Init() is called for all mix input handlers.
   //
   IlcDebug(IlcLog::kDebug, Form("<- \"%s\"", opt));
   IlcInputEventHandler *eh = 0;
   TObjArrayIter next(&fInputHandlers);
   while ((eh = (IlcInputEventHandler *) next())) {
      eh->Init(opt);
   }
   IlcDebug(IlcLog::kDebug, Form("->"));
   return kTRUE;
}
//_____________________________________________________________________________
Bool_t IlcMixEventInputHandler::Init(TTree *tree, Option_t *) {
   //
   // Init(const char*path) is called for all mix input handlers.
   // Create event pool if needed
   //
   IlcDebug(IlcLog::kDebug, Form("<- %p", tree));
   if (!tree) {
      IlcDebug(IlcLog::kDebug, Form("->"));
      return kFALSE;
   }
   IlcDebug(IlcLog::kDebug, Form("%s", tree->GetCurrentFile()->GetName()));

   // clears array of input handlers
   fMixTrees.SetOwner(kTRUE);
   fMixTrees.Clear();

   // create IlcMixInputHandlerInfo
   if (!fMixIntupHandlerInfoTmp) fMixIntupHandlerInfoTmp = new IlcMixInputHandlerInfo(tree->GetName());

   // adds current file
   fMixIntupHandlerInfoTmp->AddTreeToChain(tree);
   Int_t lastIndex = fMixIntupHandlerInfoTmp->GetChain()->GetListOfFiles()->GetEntries();
   TChainElement *che = (TChainElement *)fMixIntupHandlerInfoTmp->GetChain()->GetListOfFiles()->At(lastIndex - 1);
   IlcMixInputHandlerInfo *mixIHI = 0;
   for (Int_t i = 0; i < fInputHandlers.GetEntries(); i++) {

      IlcDebug(IlcLog::kDebug, Form("fInputHandlers[%d]", i));
      mixIHI = new IlcMixInputHandlerInfo(fMixIntupHandlerInfoTmp->GetName(), fMixIntupHandlerInfoTmp->GetTitle());
      mixIHI->PrepareEntry(che, -1, InputEventHandler(i));
      IlcDebug(IlcLog::kDebug, Form("chain[%d]->GetEntries() = %lld", i, mixIHI->GetChain()->GetEntries()));
      fMixTrees.Add(mixIHI);
   }
   IlcDebug(IlcLog::kDebug, Form("fEntryCounter=%lld", fEntryCounter));

   if (fEventPool->NeedInit())
      fEventPool->Init();

   IlcDebug(IlcLog::kDebug, Form("->"));
   return kTRUE;
}
//_____________________________________________________________________________
Bool_t IlcMixEventInputHandler::Notify() {
   //
   // Notify() is called for all mix input handlers
   //
   IlcDebug(IlcLog::kDebug, Form("<-"));
   IlcInputEventHandler *eh = 0;
   TObjArrayIter next(&fInputHandlers);
   while ((eh = (IlcInputEventHandler *) next())) {
      eh->Notify();
   }
   IlcDebug(IlcLog::kDebug, Form("->"));
   return kTRUE;
}

//_____________________________________________________________________________
Bool_t IlcMixEventInputHandler::Notify(const char *path) {
   //
   // Notify(const char*path) is called for all mix input handlers
   //
   IlcDebug(IlcLog::kDebug, Form("<- %s", path));
   IlcInputEventHandler *eh = 0;
   TObjArrayIter next(&fInputHandlers);
   while ((eh = (IlcInputEventHandler *) next())) {
      eh->Notify(path);
   }
   IlcDebug(IlcLog::kDebug, Form("->"));
   return kTRUE;
}
//_____________________________________________________________________________
Bool_t IlcMixEventInputHandler::BeginEvent(Long64_t entry) {
   //
   // BeginEvent(Long64_t entry) is called for all mix input handlers
   //

   IlcDebug(IlcLog::kDebug, Form("-> %lld", entry));
   IlcInputEventHandler *eh = 0;
   TObjArrayIter next(&fInputHandlers);
   while ((eh = (IlcInputEventHandler *) next())) {
      eh->BeginEvent(entry);
   }
   IlcDebug(IlcLog::kDebug, Form("->"));
   return kTRUE;
}


//_____________________________________________________________________________
Bool_t IlcMixEventInputHandler::GetEntry() {
   //
   // Sets correct events to every mix events
   //
   IlcDebug(IlcLog::kDebug, Form("<-"));

   IlcAnalysisManager *mgr = IlcAnalysisManager::GetAnalysisManager();
   IlcInputEventHandler *inEvHMain = dynamic_cast<IlcInputEventHandler *>(mgr->GetInputEventHandler());

   Long64_t zeroChainEntries = fMixIntupHandlerInfoTmp->GetChain()->GetEntries() - inEvHMain->GetTree()->GetEntries();

   // if fEntryCounter is 0 just add entry
   if (!fEntryCounter) {
      if (inEvHMain) {

         fEventPool->AddEntry(inEvHMain->GetTree()->GetReadEntry() + zeroChainEntries, inEvHMain->GetEvent());
      }
      return kTRUE;
   }

   IlcDebug(IlcLog::kDebug, Form("++++++++++++++ BEGIN SETUP EVENT %lld +++++++++++++++++++", fEntryCounter));

   fMixEventNumber = 0;
   TEntryList *el = fEventPool->FindEntryList(inEvHMain->GetEvent());
   Long64_t elNum = 0;
   if (el)
      elNum = el->GetN();
   else
      IlcDebug(IlcLog::kDebug, "el is null");

   IlcInputEventHandler *eh = 0;
   IlcMixInputHandlerInfo *mihi = 0;
   TObjArrayIter next(&fInputHandlers);
   Int_t counter = 0;
   Long64_t entryMix = 0;
   while ((eh = (IlcInputEventHandler *) next())) {
      if (fEventPool->GetListOfEventCuts()->GetEntries() > 0) {
         entryMix = -1;
         if (el && el->GetN() >= fBufferSize) {
            Long64_t entryInEntryList =  elNum - 1 - counter;
            if (entryInEntryList < 0) break;
            entryMix = el->GetEntry(entryInEntryList);
         }
      } else {
         entryMix = fEntryCounter - 1 - counter ;
      }

      IlcDebug(IlcLog::kDebug, Form("Handler[%d] entryMix %lld ", counter, entryMix));
      if (entryMix < 0) break;

      mihi = (IlcMixInputHandlerInfo *) fMixTrees.At(counter);
      TChainElement *te = fMixIntupHandlerInfoTmp->GetEntryInTree(entryMix);
      mihi->PrepareEntry(te, entryMix, InputEventHandler(counter));
      fMixEventNumber++;
      counter++;
   }

   if (inEvHMain) {
      fEventPool->AddEntry(inEvHMain->GetTree()->GetReadEntry() + zeroChainEntries, inEvHMain->GetEvent());
   }

   IlcDebug(IlcLog::kDebug, Form("fEntryCounter=%lld fMixEventNumber=%d", fEntryCounter, fMixEventNumber));
   IlcDebug(IlcLog::kDebug, Form("++++++++++++++ END SETUP EVENT %lld +++++++++++++++++++", fEntryCounter));
   IlcDebug(IlcLog::kDebug, Form("->"));
   return kTRUE;
}
//_____________________________________________________________________________
Bool_t IlcMixEventInputHandler::FinishEvent() {
   //
   // FinishEvent() is called for all mix input handlers
   //
   IlcDebug(IlcLog::kDebug, Form("<-"));
   IlcInputEventHandler *eh = 0;
   TObjArrayIter next(&fInputHandlers);
   while ((eh = (IlcInputEventHandler *) next())) {
      eh->FinishEvent();
   }
   fEntryCounter++;
   IlcDebug(IlcLog::kDebug, Form("->"));
   return kTRUE;
}

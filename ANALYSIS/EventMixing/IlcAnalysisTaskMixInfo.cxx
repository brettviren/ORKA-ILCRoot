//
// Class IlcAnalysisTaskMixInfo
//
// IlcAnalysisTaskMixInfo is task
// for mixing info
//
// Mixing info can be enabled by setting one of following lines in UserCreateOutput() in your task
//       // prints mixing info
//       IlcLog::SetClassDebugLevel("IlcAnalysisTaskMixInfo", IlcLog::kDebug);
//       // prints mixing info + event info for both (main and mixed) events
//       IlcLog::SetClassDebugLevel("IlcAnalysisTaskMixInfo", IlcLog::kDebug+1);
//
// authors:
//          Martin Vala (martin.vala@cern.ch)
//

#include <TList.h>
#include <TObjString.h>

#include "IlcAnalysisManager.h"

#include "IlcMixInputEventHandler.h"
#include "IlcAnalysisTaskMixInfo.h"
#include "IlcMixInfo.h"
#include "IlcMixEventPool.h"
#include "IlcMixEventCutObj.h"


ClassImp(IlcAnalysisTaskMixInfo)

//________________________________________________________________________
IlcAnalysisTaskMixInfo::IlcAnalysisTaskMixInfo(const char *name)
   : IlcAnalysisTaskSE(name),
     fInputEHMain(0),
     fInputEHMix(0),
     fOutputList(0),
     fMixInfo(0),
     fCurrentEntryTmp(-1),
     fLogType(IlcLog::kInfo),
     fLogClassesString()
{
   //
   // Constructor
   //
   DefineOutput(1, TList::Class());
}

//________________________________________________________________________
IlcAnalysisTaskMixInfo::~IlcAnalysisTaskMixInfo()
{
   //
   // Destructor
   //
   if (fOutputList && !IlcAnalysisManager::GetAnalysisManager()->IsProofMode()) delete fOutputList;
}

//________________________________________________________________________
void IlcAnalysisTaskMixInfo::UserCreateOutputObjects()
{
   // Create histograms
   // Called once

   SetDebugForAllClasses();
   fOutputList = new TList();
   fOutputList->SetOwner(kTRUE);

   // sets all Inuput Handler pointers
   InitInputHandlers();

   // inits mix info
   InitMixInfo();

   if (fInputEHMix) {
      IlcMixEventPool *evPool = fInputEHMix->GetEventPool();
      if (evPool) {
         evPool->SetBufferSize(fInputEHMix->BufferSize());
         evPool->SetMixNumber(fInputEHMix->MixNumber());
         fMixInfo->SetEventPool(evPool);
      }
   }
   if (fMixInfo) fOutputList->Add(fMixInfo);

   // Post output data.
   PostData(1, fOutputList);
}

//________________________________________________________________________
void IlcAnalysisTaskMixInfo::UserExec(Option_t *)
{
   // Main loop
   // Called for each event
   if (fMixInfo && fInputEHMix) {
      if (fInputEHMix->BufferSize() > 1) {
         if (fInputEHMix->NumberMixedTimes() >= fInputEHMix->BufferSize())
            fMixInfo->FillHistogram(IlcMixInfo::kMainEvents, fInputEHMix->CurrentBinIndex());
      } else {
         if ((!fInputEHMix->IsMixingIfNotEnoughEvents())) {
            if (fInputEHMix->NumberMixed() == fInputEHMix->MixNumber())
               // add main entry only when there was enough mixed events mixed
               fMixInfo->FillHistogram(IlcMixInfo::kMainEvents, fInputEHMix->CurrentBinIndex());
         } else {
            fMixInfo->FillHistogram(IlcMixInfo::kMainEvents, fInputEHMix->CurrentBinIndex());
         }
      }
      IlcDebug(IlcLog::kDebug, Form("Main %lld %d [%lld,%lld] %d", fInputEHMix->CurrentEntry(), fInputEHMix->CurrentBinIndex(), fInputEHMix->CurrentEntryMain(), fInputEHMix->CurrentEntryMix(), fInputEHMix->NumberMixed()));
   }
   // Post output data.
   PostData(1, fOutputList);
}

//________________________________________________________________________
void IlcAnalysisTaskMixInfo::UserExecMix(Option_t *)
{
   // UserExecMix

   if (!fInputEHMix) return;

   // fills bin index (even when it is -1, so we know out of range combinations)
   if (fMixInfo) fMixInfo->FillHistogram(IlcMixInfo::kMixedEvents, fInputEHMix->CurrentBinIndex());

   // just test
   if (fInputEHMix->CurrentEntryMix() < 0) {
      IlcError("Mix entry is -1 and it should not happen !!!!!");
      return ;
   }
   IlcDebug(IlcLog::kDebug, Form("Mixing %lld %d [%lld,%lld] %d", fInputEHMix->CurrentEntry(), fInputEHMix->CurrentBinIndex(), fInputEHMix->CurrentEntryMain(), fInputEHMix->CurrentEntryMix(), fInputEHMix->NumberMixed()));
   if (IlcLog::GetDebugLevel("", IsA()->GetName()) > IlcLog::kDebug) PrintEventInfo();
   // Post output data.
   PostData(1, fOutputList);
}

//________________________________________________________________________
void IlcAnalysisTaskMixInfo::FinishTaskOutput()
{
   // FinishTaskOutput
   if (fMixInfo) fMixInfo->Print();
}


//________________________________________________________________________
void IlcAnalysisTaskMixInfo::Terminate(Option_t *)
{
   // Draw result to the screen
   // Called once at the end of the query
   fOutputList = dynamic_cast<TList *>(GetOutputData(1));
   if (!fOutputList) {
      IlcError("fOutputList not available");
      return;
   }
   fOutputList->Print();
   fMixInfo = (IlcMixInfo *) fOutputList->FindObject("mixInfo");
   if (fMixInfo) {
      fMixInfo->Draw("HIST");
      IlcMixEventPool *evPool = (IlcMixEventPool *) fMixInfo->GetEventPool("mixEventPool");
      if (evPool) evPool->Print();
   }
}

//_____________________________________________________________________________
void IlcAnalysisTaskMixInfo::SetLogType(IlcLog::EType_t type, TString allClasses)
{
   //
   // Set Log level for this and other classes (list of their names)
   //
   IlcDebug(IlcLog::kDebug + 10, "<-");
   fLogType = type;
   fLogClassesString = allClasses;
   SetDebugForAllClasses();
   IlcDebug(IlcLog::kDebug + 10, "->");
}

//_____________________________________________________________________________
void IlcAnalysisTaskMixInfo::SetDebugForAllClasses()
{
   //
   // Set debug level for all classes for which it is required
   //
   IlcDebug(IlcLog::kDebug + 10, "<-");
   TObjArray *array = fLogClassesString.Tokenize(":");
   TObjString *str;
   TString strr;
   for (Int_t i = 0; i < array->GetEntriesFast(); i++) {
      str = (TObjString *) array->At(i);
      strr = str->GetString();
      IlcLog::SetClassDebugLevel(strr.Data(), fLogType);
      IlcDebug(IlcLog::kDebug + 5, Form("Setting Debug level %d to %s ...", (Int_t)fLogType - IlcLog::kDebug, strr.Data()));
   }
   IlcDebug(IlcLog::kDebug + 10, "->");
}

//_____________________________________________________________________________
void IlcAnalysisTaskMixInfo::InitMixInfo()
{
   //
   // Init mixing info
   //
   if (fInputEHMix) {
      fMixInfo = new IlcMixInfo("mixInfo", "Mix title");
      IlcMixEventPool *evPool = fInputEHMix->GetEventPool();
      if (!evPool) {
         //             TList *list = new TList;
         if (fMixInfo) fMixInfo->CreateHistogram(IlcMixInfo::kMainEvents, 1, 1, 2);
         if (fMixInfo) fMixInfo->CreateHistogram(IlcMixInfo::kMixedEvents, 1, 1, 2);
      } else {
         if (evPool->NeedInit()) evPool->Init();
         Int_t num = evPool->GetListOfEntryLists()->GetEntriesFast();
         if (fMixInfo) fMixInfo->CreateHistogram(IlcMixInfo::kMainEvents, num, 1, num + 1);
         if (fMixInfo) fMixInfo->CreateHistogram(IlcMixInfo::kMixedEvents, num, 1, num + 1);
      }
   }
}

//_____________________________________________________________________________
void IlcAnalysisTaskMixInfo::PrintEventInfo()
{
   //
   // Prints event info for all event mxing cuts
   //
   if (fInputEHMix) {
      TObjArrayIter next(fInputEHMix->GetEventPool()->GetListOfEventCuts());
      IlcMixEventCutObj *cut;
      IlcInputEventHandler *ihMain = fInputEHMain->GetFirstInputEventHandler();
      IlcMultiInputEventHandler *ihMultiMix = fInputEHMix->GetFirstMultiInputHandler();
      IlcInputEventHandler *ihMix = 0;
      if (ihMultiMix) ihMix = ihMultiMix->GetFirstInputEventHandler();
      if (!ihMix) return;
      while ((cut = (IlcMixEventCutObj *) next())) {
         cut->PrintValues(ihMain->GetEvent(), ihMix->GetEvent());
      }
   }
}

//_____________________________________________________________________________
void IlcAnalysisTaskMixInfo::InitInputHandlers()
{
   //
   // Sets needed input handlers
   //
   IlcAnalysisManager *mgr = IlcAnalysisManager::GetAnalysisManager();
   fInputEHMain = dynamic_cast<IlcMultiInputEventHandler *>(mgr->GetInputEventHandler());
   if (fInputEHMain) {
      fInputEHMix = dynamic_cast<IlcMixInputEventHandler *>(fInputEHMain->GetFirstMultiInputHandler());
   }
}

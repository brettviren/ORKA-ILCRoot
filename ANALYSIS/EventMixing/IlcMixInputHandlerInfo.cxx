//
// Class IlcMixInputHandlerInfo
//
// IlcMixInputHandlerInfo is interface with mixed
// input handlers
//
// author:
//        Martin Vala (martin.vala@cern.ch)
//
#include <TTree.h>
#include <TChain.h>
#include <TFile.h>
#include <TChainElement.h>

#include "IlcLog.h"
#include "IlcInputEventHandler.h"

#include "IlcMixInputHandlerInfo.h"

ClassImp(IlcMixInputHandlerInfo)

//_____________________________________________________________________________
IlcMixInputHandlerInfo::IlcMixInputHandlerInfo(const char *name, const char *title): TNamed(name, title),
   fChain(0),
   fChainEntriesArray(),
   fZeroEntryNumber(0),
   fNeedNotify(kFALSE)
{
   //
   // Default constructor.
   //
}
//_____________________________________________________________________________
IlcMixInputHandlerInfo::~IlcMixInputHandlerInfo()
{
   //
   // Destructor
   //
   if (fChain) delete fChain;
}

//_____________________________________________________________________________
TChain *IlcMixInputHandlerInfo::GetChain()
{
   //
   // Returns curren chain. When chain is null it will create it
   //
   if (!fChain) fChain = new TChain(GetName());
   return fChain;
}

//_____________________________________________________________________________
void IlcMixInputHandlerInfo::AddChain(TChain *chain)
{
   //
   // Add chain
   //
   IlcDebug(IlcLog::kDebug + 5, "<-");
   if (!chain) return;
   if (fChain) delete fChain;
   fChain = new TChain(GetName());
   fChain->Add(chain);
   IlcDebug(IlcLog::kDebug + 5, "->");
}

//_____________________________________________________________________________
void IlcMixInputHandlerInfo::AddTreeToChain(const char *path)
{
   //
   // Adds tree in to chain
   //
   IlcDebug(IlcLog::kDebug + 5, Form("<- %s", path));
   GetChain();
   fChain->AddFile(path);
   IlcDebug(IlcLog::kDebug + 3, Form("Num files in fChain %d", fChain->GetListOfFiles()->GetEntries()));
   Long64_t sumTree = fZeroEntryNumber;
   for (Int_t i = 0; i < fChainEntriesArray.GetSize() ; i++) sumTree += fChainEntriesArray.At(i);
   fChain->LoadTree(sumTree);
   Int_t lastIndex = fChain->GetListOfFiles()->GetEntries();
   IlcDebug(IlcLog::kDebug + 3, Form("Num files in fChain %d", lastIndex));
   fChainEntriesArray.Set(lastIndex);
   IlcDebug(IlcLog::kDebug + 3, Form("Adding %lld to id %d", fChain->GetTree()->GetEntries(), lastIndex - 1));
   fChainEntriesArray.AddAt((Int_t)fChain->GetTree()->GetEntries(), (Int_t)lastIndex - 1);
   IlcDebug(IlcLog::kDebug + 5, Form("-> %s", path));
}

//_____________________________________________________________________________
TChainElement *IlcMixInputHandlerInfo::GetEntryInTree(Long64_t &entry)
{
   //
   // Get entry in current tree
   //
   IlcDebug(IlcLog::kDebug + 5, Form("<- %lld", entry));
   fZeroEntryNumber = 0;
   if (entry < fZeroEntryNumber) {
      IlcError(Form("Num %lld is less then ZeroEntryNumber(%lld)", entry, fZeroEntryNumber));
      entry = -1;
      IlcDebug(IlcLog::kDebug + 5, "->");
      return 0;
   }
   Long64_t sumTree = fZeroEntryNumber;
   for (Int_t i = 0; i < fChainEntriesArray.GetSize() ; i++) {
      sumTree += fChainEntriesArray.At(i);
      if (sumTree > entry) {
         sumTree = entry - sumTree + fChainEntriesArray.At(i);
         IlcDebug(IlcLog::kDebug + 1, Form("Entry in current tree num is %lld with i=%d", sumTree, i));
         entry = sumTree;
         IlcDebug(IlcLog::kDebug + 5, "->");
         return (TChainElement *) fChain->GetListOfFiles()->At(i);
      }
   }
   entry = -1;
   IlcDebug(IlcLog::kDebug + 5, "->");
   return 0;
}

//_____________________________________________________________________________
void IlcMixInputHandlerInfo::PrepareEntry(TChainElement *te, Long64_t entry, IlcInputEventHandler *eh, Option_t *opt)
{
   //
   // Prepare Entry
   //
   IlcDebug(IlcLog::kDebug + 5, Form("<- %lld", entry));
   if (!te) {
      IlcDebug(IlcLog::kDebug + 5, "-> te is null");
      return;
   }
   if (entry < 0) {
      IlcDebug(IlcLog::kDebug, Form("We are creating new chain from file %s ...", te->GetTitle()));
      if (!fChain) {
         fChain = new TChain(te->GetName());
         fChain->AddFile(te->GetTitle());
         fChain->GetEntry(0);
         eh->Init(opt);
         eh->Init(fChain->GetTree(), opt);
      }
      fNeedNotify = kTRUE;
      IlcDebug(IlcLog::kDebug + 5, "->");
      return;
   }
   if (fChain) {
      IlcDebug(IlcLog::kDebug, Form("Filename is %s", fChain->GetTree()->GetCurrentFile()->GetName()));
      TString fn = fChain->GetTree()->GetCurrentFile()->GetName();
      if (fn.CompareTo(te->GetTitle())) {
         IlcDebug(IlcLog::kDebug, Form("Filename %s is NOT same ...", te->GetTitle()));
         IlcDebug(IlcLog::kDebug, Form("We are changing to file %s ...", te->GetTitle()));
         // change file
         delete fChain;
         fChain = new TChain(te->GetName());
         fChain->AddFile(te->GetTitle());
         fChain->GetEntry(0);
         eh->Init(opt);
         eh->Init(fChain->GetTree(), opt);
         eh->Notify(te->GetTitle());
         fChain->GetEntry(entry);
         eh->BeginEvent(entry);
         fNeedNotify = kFALSE;
      } else {
         IlcDebug(IlcLog::kDebug, Form("We are reusing file %s ...", te->GetTitle()));
         if (fNeedNotify) eh->Notify(te->GetTitle());
         fNeedNotify = kFALSE;
         IlcDebug(IlcLog::kDebug, Form("Entry is %lld  fChain->GetEntries %lld ...", entry, fChain->GetEntries()));
         fChain->GetEntry(entry);
         eh->BeginEvent(entry);
         // file is in tree fChain already
      }
   }
   IlcDebug(IlcLog::kDebug, Form("We are USING file %s ...", te->GetTitle()));
   IlcDebug(IlcLog::kDebug, Form("We are USING file from fChain->GetTree() %s ...", fChain->GetTree()->GetCurrentFile()->GetName()));
   // here we have correct chain with 1 tree only
   IlcDebug(IlcLog::kDebug + 5, "->");
}

//_____________________________________________________________________________
Long64_t IlcMixInputHandlerInfo::GetEntries()
{
   //
   // Returns number of entries
   //
   if (fChain) return fChain->GetEntries();
   return -1;
}

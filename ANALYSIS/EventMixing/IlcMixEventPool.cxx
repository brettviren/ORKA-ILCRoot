//
// Class IlcMixEventPool
//
// IlcMixEventPool is used to find
// similar events
//
// author:
//        Martin Vala (martin.vala@cern.ch)
//

#include <TEntryList.h>

#include "IlcLog.h"
#include "IlcMixEventCutObj.h"

#include "IlcMixEventPool.h"

ClassImp(IlcMixEventPool)

//_________________________________________________________________________________________________
IlcMixEventPool::IlcMixEventPool(const char *name, const char *title) : TNamed(name, title),
   fListOfEntryList(),
   fListOfEventCuts(),
   fBinNumber(0),
   fBufferSize(0),
   fMixNumber(0)
{
   //
   // Default constructor.
   //
   IlcDebug(IlcLog::kDebug + 5, "<-");
   IlcDebug(IlcLog::kDebug + 5, "->");
}
//_________________________________________________________________________________________________
IlcMixEventPool::IlcMixEventPool(const IlcMixEventPool &obj) : TNamed(obj),
   fListOfEntryList(obj.fListOfEntryList),
   fListOfEventCuts(obj.fListOfEventCuts),
   fBinNumber(obj.fBinNumber),
   fBufferSize(obj.fBufferSize),
   fMixNumber(obj.fMixNumber)
{
   //
   // Copy constructor
   //
   IlcDebug(IlcLog::kDebug + 5, "<-");
   IlcDebug(IlcLog::kDebug + 5, "->");
}

//_________________________________________________________________________________________________
IlcMixEventPool &IlcMixEventPool::operator=(const IlcMixEventPool &obj)
{
   //
   // Assigned operator
   //
   if (&obj != this) {
      TNamed::operator=(obj);
      fListOfEntryList = obj.fListOfEntryList;
      fListOfEventCuts = obj.fListOfEventCuts;
      fBinNumber = obj.fBinNumber;
      fBufferSize = obj.fBufferSize;
      fMixNumber = obj.fMixNumber;
   }
   return *this;
}


//_________________________________________________________________________________________________
IlcMixEventPool::~IlcMixEventPool()
{
   //
   // Destructor
   //
   IlcDebug(IlcLog::kDebug + 5, "<-");
   IlcDebug(IlcLog::kDebug + 5, "->");
}
//_________________________________________________________________________________________________
void IlcMixEventPool::AddCut(IlcMixEventCutObj *cut)
{
   //
   // Adds cut
   //
   if (cut) fListOfEventCuts.Add(new IlcMixEventCutObj(*cut));
}
//_________________________________________________________________________________________________
void IlcMixEventPool::Print(const Option_t *option) const
{
   //
   // Prints usefull information
   //
   TObjArrayIter next(&fListOfEventCuts);
   //   Int_t c=0;
   IlcMixEventCutObj *cut;
   while ((cut = (IlcMixEventCutObj *) next())) {
      cut->Print(option);
   }
   IlcDebug(IlcLog::kDebug, Form("NumOfEntryList %d", fListOfEntryList.GetEntries()));
   TEntryList *el;
   for (Int_t i = 0; i < fListOfEntryList.GetEntries(); i++) {
      el = (TEntryList *) fListOfEntryList.At(i);
      IlcDebug(IlcLog::kDebug, Form("EntryList[%d] %lld", i, el->GetN()));
   }
}
//_________________________________________________________________________________________________
Int_t IlcMixEventPool::Init()
{
   //
   // Init event pool
   //
   IlcDebug(IlcLog::kDebug + 5, "<-");
   CreateEntryListsRecursivly(fListOfEventCuts.GetEntries() - 1);
   fBinNumber++;
   IlcDebug(IlcLog::kDebug, Form("fBinnumber = %d", fBinNumber));
   AddEntryList();
   IlcDebug(IlcLog::kDebug + 5, "->");
   return 0;
}

//_________________________________________________________________________________________________
void IlcMixEventPool::CreateEntryListsRecursivly(Int_t index)
{
   //
   // Helper function which create entrylist recursivly
   //
   IlcDebug(IlcLog::kDebug + 5, "<-");
   IlcMixEventCutObj *cut;
   if (index >= 0) {
      IlcDebug(IlcLog::kDebug + 1, Form("index = %d", index));
      cut = dynamic_cast<IlcMixEventCutObj *>(fListOfEventCuts.At(index));
      if (!cut) return;
      cut->Reset();
      while (cut->HasMore()) {
         cut->AddStep();
         CreateEntryListsRecursivly(index - 1);
         if (cut->HasMore()) {
            fBinNumber++;
            IlcDebug(IlcLog::kDebug + 1, Form("fBinnumber = %d", fBinNumber));
            AddEntryList();
            //                 PrintCurrentCutIntervals();
         }
      }
   }
   IlcDebug(IlcLog::kDebug + 5, "->");
}

//_________________________________________________________________________________________________
TEntryList *IlcMixEventPool::AddEntryList()
{
   //
   // Adds endtry list
   //
   IlcDebug(IlcLog::kDebug + 5, "<-");
   TObjArrayIter next(&fListOfEventCuts);
   IlcMixEventCutObj *cut;
   while ((cut = (IlcMixEventCutObj *) next())) {
      if (cut) cut->PrintCurrentInterval();
   }
   TEntryList *el = new TEntryList;
   fListOfEntryList.Add(el);
   IlcDebug(IlcLog::kDebug + 1, Form("Number in Entry list -> %lld", el->GetN()));
   IlcDebug(IlcLog::kDebug + 5, "->");
   return el;
}

//_________________________________________________________________________________________________
Bool_t IlcMixEventPool::AddEntry(Long64_t entry, IlcVEvent *ev)
{
   //
   // Adds entry to correct entry list
   //
   IlcDebug(IlcLog::kDebug + 5, "<-");
   IlcDebug(IlcLog::kDebug + 5, Form("AddEntry(%lld,%p)", entry, (void *)ev));
   if (entry < 0) {
      IlcDebug(IlcLog::kDebug, Form("Entry %lld was NOT added !!!", entry));
      return kFALSE;
   }
   Int_t idEntryList = -1;
   TEntryList *el =  FindEntryList(ev, idEntryList);
   if (el) {
      el->Enter(entry);
      IlcDebug(IlcLog::kDebug, Form("Entry %lld was added with idEntryList %d !!!", entry, idEntryList));
      return kTRUE;
   }
   IlcDebug(IlcLog::kDebug, Form("Entry %lld was NOT added !!!", entry));
   IlcDebug(IlcLog::kDebug + 5, "->");
   return kFALSE;
}

//_________________________________________________________________________________________________
TEntryList *IlcMixEventPool::FindEntryList(IlcVEvent *ev, Int_t &idEntryList)
{
   //
   // Find entrlist in list of entrlist
   //
   IlcDebug(IlcLog::kDebug + 5, "<-");
   Int_t num = fListOfEventCuts.GetEntriesFast();
   if (num < 1) return 0;
   Int_t *indexes = new Int_t[num] ;
   Int_t *lenght = new Int_t[num];
   Int_t i = 0;
   TObjArrayIter next(&fListOfEventCuts);
   IlcMixEventCutObj *cut;
   while ((cut = (IlcMixEventCutObj *) next())) {
      indexes[i] = cut->GetIndex(ev);
      if (indexes[i] < 0) {
         IlcDebug(IlcLog::kDebug, Form("idEntryList %d", -1));
         delete [] indexes;
         delete [] lenght;
         return 0;
      }
      lenght[i] = cut->GetNumberOfBins();
      IlcDebug(IlcLog::kDebug + 1, Form("indexes[%d] %d", i, indexes[i]));
      i++;
   }
   idEntryList = 0;
   SearchIndexRecursive(fListOfEventCuts.GetEntries() - 1, &indexes[0], &lenght[0], idEntryList);
   IlcDebug(IlcLog::kDebug, Form("idEntryList %d", idEntryList - 1));
   // index which start with 0 (idEntryList-1)
   delete [] indexes;
   delete [] lenght;
   IlcDebug(IlcLog::kDebug + 5, "->");
   return (TEntryList *) fListOfEntryList.At(idEntryList - 1);
}

//_________________________________________________________________________________________________
void IlcMixEventPool::SearchIndexRecursive(Int_t num, Int_t *i, Int_t *d, Int_t &index)
{
   //
   // Search for index of entrylist
   //
   IlcDebug(IlcLog::kDebug + 5, "<-");
   if (num > 0) {
      index += (i[num] - 1) * d[num - 1];
      SearchIndexRecursive(num - 1, i, d, index);
   } else {
      index += i[num];
   }
   IlcDebug(IlcLog::kDebug + 5, "->");
}

//_________________________________________________________________________________________________
Bool_t IlcMixEventPool::SetCutValuesFromBinIndex(Int_t index)
{
   //
   // Sets cut value from bin index
   //

   Int_t numCuts = fListOfEventCuts.GetEntriesFast();
   Int_t *lenght = new Int_t[numCuts];
   Int_t *indexes = new Int_t[numCuts];
   Long64_t timesNum = 1;
   IlcMixEventCutObj *cut;
   Int_t i = 0, j = 0;
   for (i = 0; i < numCuts; i++) {
      cut = (IlcMixEventCutObj *) fListOfEventCuts.At(i);
      cut->Reset();
      lenght[i] = cut->GetNumberOfBins();
      indexes[i] = 1;
      timesNum *= lenght[i];
   }

   if (index < 0 || index >= timesNum) {
//       IlcError(Form("index=%d is out of range !!!", index));
      delete [] lenght;
      delete [] indexes;
      return kFALSE;
   }

   Long64_t indexNum = index;
   for (i = 0; i < numCuts; i++) {
      timesNum = 1;
      for (j = 0; j < numCuts - i - 1; j++) timesNum *= lenght[j];
      indexNum /= timesNum;
      indexes[numCuts - i - 1] = indexNum + 1;
      index -= indexNum * timesNum;
      indexNum = index;
   }

   for (i = 0; i < numCuts; i++) {
      cut = (IlcMixEventCutObj *) fListOfEventCuts.At(i);
      for (j = 0; j < indexes[i]; j++) cut->AddStep();
      cut->PrintCurrentInterval();

   }

   for (i = 0; i < numCuts; i++) IlcDebug(IlcLog::kDebug, Form("indexes[%d]=%d", i, indexes[i]));

   delete [] lenght;
   delete [] indexes;

   return kTRUE;
}

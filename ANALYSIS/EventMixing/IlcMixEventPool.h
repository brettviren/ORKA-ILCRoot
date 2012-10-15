//
// Class IlcMixEventPool
//
// IlcMixEventPool is used to find
// similar events
//
// author:
//        Martin Vala (martin.vala@cern.ch)
//

#ifndef ILCMIXEVENTPOOL_H
#define ILCMIXEVENTPOOL_H

#include <TObjArray.h>
#include <TNamed.h>

class TEntryList;
class IlcMixEventCutObj;
class IlcVEvent;
class IlcMixEventPool : public TNamed {
public:
   IlcMixEventPool(const char *name = "mixEventPool", const char *title = "Mix event pool");
   IlcMixEventPool(const IlcMixEventPool &obj);
   IlcMixEventPool &operator= (const IlcMixEventPool &obj);
   virtual ~IlcMixEventPool();

   // prints object info
   virtual void      Print(const Option_t *option = "") const;

   // inits correctly object
   Int_t       Init();

   void        CreateEntryListsRecursivly(Int_t index);
   void        SearchIndexRecursive(Int_t num, Int_t *i, Int_t *d, Int_t &index);
   TEntryList *AddEntryList();

   Bool_t      AddEntry(Long64_t entry, IlcVEvent *ev);
   TEntryList *FindEntryList(IlcVEvent *ev, Int_t &idEntryList);

   void        AddCut(IlcMixEventCutObj *cut);

   Bool_t      NeedInit() { return (fListOfEntryList.GetEntries() == 0); }
   TObjArray  *GetListOfEntryLists() { return &fListOfEntryList; }
   TObjArray  *GetListOfEventCuts() { return &fListOfEventCuts; }

   Bool_t      SetCutValuesFromBinIndex(Int_t index);
   void        SetBufferSize(Int_t buffer) { fBufferSize = buffer; }
   void        SetMixNumber(Int_t numMix) { fMixNumber = numMix; }
   Int_t       GetBufferSize() const { return fBufferSize; }
   Int_t       GetMixNumber() const { return fMixNumber; }

private:

   TObjArray   fListOfEntryList;       // list of entry lists
   TObjArray   fListOfEventCuts;       // list of entry lists

   Int_t       fBinNumber;             // bin number
   Int_t       fBufferSize;            // buffer size
   Int_t       fMixNumber;             // mixing number

   ClassDef(IlcMixEventPool, 1)
};

#endif

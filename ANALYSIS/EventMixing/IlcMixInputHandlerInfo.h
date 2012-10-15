//
// Class IlcMixInputHandlerInfo
//
// IlcMixInputHandlerInfo is interface with mixed
// input handlers
//
// author:
//        Martin Vala (martin.vala@cern.ch)
//
#ifndef ILCMIXINPUTHANDLERINFO_H
#define ILCMIXINPUTHANDLERINFO_H
#include <TArrayI.h>
#include <TNamed.h>

class TTree;
class TChain;
class TChainElement;
class IlcInputEventHandler;
class IlcMixInputHandlerInfo : public TNamed {

public:
   IlcMixInputHandlerInfo(const char *name = "defautlTree", const char *title = "Defautl tree");
   virtual ~IlcMixInputHandlerInfo();
   TChain *GetChain();

   void AddChain(TChain *chain);
//     void AddTreeToChain(TTree *tree);
   void AddTreeToChain(const char *path);

   void PrepareEntry(TChainElement *te, Long64_t entry, IlcInputEventHandler *eh, Option_t *opt);

   void SetZeroEntryNumber(Long64_t num) { fZeroEntryNumber = num; }
   TChainElement *GetEntryInTree(Long64_t &entry);
   Long64_t      GetEntries();

private:
   TChain    *fChain;              // current chain
   TArrayI   fChainEntriesArray;   // array of entries of every chaing
   Long64_t  fZeroEntryNumber;     // zero entry number (will be used when we will delete not needed chains)
   Bool_t    fNeedNotify;          // flag if Notify is needed for current input handler

   IlcMixInputHandlerInfo(const IlcMixInputHandlerInfo &handler);
   IlcMixInputHandlerInfo &operator=(const IlcMixInputHandlerInfo &handler);

   ClassDef(IlcMixInputHandlerInfo, 1); // Mix Input Handler info
};

#endif // ILCMIXINPUTHANDLERINFO_H

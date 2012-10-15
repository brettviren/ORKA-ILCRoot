//
// Class IlcMixInfo
//
// IlcMixInfo object contains information about one cut on for event mixing
// available for users containing mixing information
//
// authors:
//          Martin Vala (martin.vala@cern.ch)
//

#ifndef ILCMIXINFO_H
#define ILCMIXINFO_H

#include <TNamed.h>
#include <Rtypes.h>

class IlcMixEventPool;
class TH1I;
class TList;
class TCollection;
class IlcMixInfo : public TNamed {
public:
   enum EInfoHistorgramType { kMainEvents = 0, kMixedEvents = 1, kNumTypes };

   IlcMixInfo(const char *name = "mix", const char *title = "MixInfo");
   IlcMixInfo(const IlcMixInfo &obj);
   virtual ~IlcMixInfo();

   void Reset();
   virtual void Print(Option_t *option = "") const;
   virtual void Draw(Option_t *option = "");
   virtual Long64_t Merge(TCollection *list);

   void Add(IlcMixInfo *mi);

   void SetOutputList(TList *const list) { fHistogramList = list; }
   void CreateHistogram(EInfoHistorgramType type, Int_t nbins, Int_t min, Int_t max);
   void FillHistogram(IlcMixInfo::EInfoHistorgramType type, Int_t value);
   const char *GetNameHistogramByType(Int_t index) const;
   const char *GetTitleHistogramByType(Int_t index) const;
   TH1I  *GetHistogramByType(Int_t index) const;

   void    SetEventPool(IlcMixEventPool *evPool);
   IlcMixEventPool *GetEventPool(const char *name);


   static void DynamicExec(IlcMixInfo *const mixInfo);
private:

   TList     *fHistogramList;  // histogram list

   IlcMixInfo &operator=(const IlcMixInfo &) { return *this; }

   ClassDef(IlcMixInfo, 1)

};

#endif

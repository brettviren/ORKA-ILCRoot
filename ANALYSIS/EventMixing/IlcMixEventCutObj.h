//
// Class IlcMixEventCutObj
//
// IlcMixEventCutObj object contains information about one cut on for event mixing
// used by IlcMixEventPool class
//
// authors:
//          Martin Vala (martin.vala@cern.ch)
//

#ifndef ILCMIXEVENTCUTOBJ_H
#define ILCMIXEVENTCUTOBJ_H

#include <TObject.h>
#include <TString.h>

class IlcVEvent;
class IlcAODEvent;
class IlcESDEvent;
class IlcMixEventCutObj : public TObject {
public:
   enum EEPAxis_t {kMultiplicity = 0, kZVertex = 1, kNumberV0s = 2, kNumberTracklets = 3, kCentrality = 4, kEventPlane = 5, kAllEventAxis = 6};

   IlcMixEventCutObj(IlcMixEventCutObj::EEPAxis_t type = kMultiplicity, Float_t min = 0.0, Float_t max = 0.0, Float_t step = 1.0, const char *opt = "");
   IlcMixEventCutObj(const IlcMixEventCutObj &obj);
   IlcMixEventCutObj &operator=(const IlcMixEventCutObj &obj);

   virtual void Print(const Option_t *) const;
   void PrintCurrentInterval();
   void PrintValues(IlcVEvent *main, IlcVEvent *mix);
   void Reset();
   void AddStep();

   Bool_t      HasMore() const;

   Int_t       GetNumberOfBins() const;
   Float_t     GetMin() const { return fCurrentVal; }
   Float_t     GetMax() const { return fCurrentVal + fCutStep - fCutSmallVal; }
   Float_t     GetStep() const { return fCutStep; }
   Short_t     GetType() const { return fCutType; }
   Int_t       GetBinNumber(Float_t num) const;
   Int_t       GetIndex(IlcVEvent *ev);
   Double_t    GetValue(IlcVEvent *ev);
   Double_t    GetValue(IlcESDEvent *ev);
   Double_t    GetValue(IlcAODEvent *ev);

   const char *GetCutName(Int_t index = -1) const;

   void        SetCurrentValueToIndex(Int_t index);

private:
   Int_t       fCutType;       // cut type
   TString     fCutOpt;        // cut option string
   Float_t     fCutMin;        // cut min
   Float_t     fCutMax;        // cut max
   Float_t     fCutStep;       // cut step
   Float_t     fCutSmallVal;   // small value

   Float_t     fCurrentVal;    // current value

   ClassDef(IlcMixEventCutObj, 2)
};

#endif

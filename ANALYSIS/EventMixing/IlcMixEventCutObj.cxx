//
// Class IlcMixEventCutObj
//
// IlcMixEventCutObj object contains information about one cut on for event mixing
// used by IlcMixEventPool class
//
// authors:
//          Martin Vala (martin.vala@cern.ch)
//

#include "IlcLog.h"
#include "IlcESDEvent.h"
#include "IlcAODEvent.h"
#include "IlcMultiplicity.h"
#include "IlcAODVertex.h"
#include "IlcEventplane.h"

#include "IlcMixEventCutObj.h"

ClassImp(IlcMixEventCutObj)

//_________________________________________________________________________________________________
IlcMixEventCutObj::IlcMixEventCutObj(IlcMixEventCutObj::EEPAxis_t type, Float_t min, Float_t max, Float_t step, const char *opt) : TObject(),
   fCutType((Int_t)type),
   fCutOpt(opt),
   fCutMin(min),
   fCutMax(max),
   fCutStep(step),
   fCutSmallVal(0),
   fCurrentVal(min)
{
   //
   // Default constructor
   //
   IlcDebug(IlcLog::kDebug + 5, "<-");
   if (fCutStep < 1e-5) IlcError("fCutStep is too small !!! This cut will not work !!!");
   IlcDebug(IlcLog::kDebug + 5, "->");
}

//_________________________________________________________________________________________________
IlcMixEventCutObj::IlcMixEventCutObj(const IlcMixEventCutObj &obj) : TObject(obj),
   fCutType(obj.fCutType),
   fCutOpt(obj.fCutOpt),
   fCutMin(obj.fCutMin),
   fCutMax(obj.fCutMax),
   fCutStep(obj.fCutStep),
   fCutSmallVal(obj.fCutSmallVal),
   fCurrentVal(obj.fCurrentVal)
{
   //
   // Copy constructor
   //
   IlcDebug(IlcLog::kDebug + 5, "<-");
   IlcDebug(IlcLog::kDebug + 5, "->");
}

//_________________________________________________________________________________________________
IlcMixEventCutObj &IlcMixEventCutObj::operator=(const IlcMixEventCutObj &obj)
{
   //
   // Assigned operator
   //
   if (&obj != this) {
      TObject::operator=(obj);
      fCutType = obj.fCutType;
      fCutOpt = obj.fCutOpt;
      fCutMin = obj.fCutMin;
      fCutMax = obj.fCutMax;
      fCutStep = obj.fCutStep;
      fCutSmallVal = obj.fCutSmallVal;
      fCurrentVal = obj.fCurrentVal;
//       fNoMore = obj.fNoMore;
   }
   return *this;
}


//_________________________________________________________________________________________________
void IlcMixEventCutObj::Reset()
{
   //
   // Reset cut
   //
   IlcDebug(IlcLog::kDebug + 5, "<-");
   fCurrentVal = fCutMin - fCutStep;
   IlcDebug(IlcLog::kDebug + 5, "->");
}
//_________________________________________________________________________________________________
Bool_t IlcMixEventCutObj::HasMore() const
{
   //
   // Return kTRUE when fCurrentVal is in interval of cut range
   //
   return ((fCurrentVal + fCutStep) < fCutMax);
}

//_________________________________________________________________________________________________
void IlcMixEventCutObj::AddStep()
{
   //
   // Adds step
   //
   fCurrentVal += fCutStep;
}

//_________________________________________________________________________________________________
void IlcMixEventCutObj::Print(const Option_t *) const
{
   //
   // Prints cut information
   //
   IlcInfo(Form("%s %f %f %f", GetCutName(fCutType), fCutMin, fCutMax, fCutStep));
}
//_________________________________________________________________________________________________
void IlcMixEventCutObj::PrintCurrentInterval()
{
   //
   // Prints current cut interval information
   //
   IlcDebug(IlcLog::kDebug, Form("%s <%f,%f>", GetCutName(fCutType), GetMin(), GetMax()));
}

//_________________________________________________________________________________________________
Int_t IlcMixEventCutObj::GetNumberOfBins() const
{
   //
   // Returns number of bins
   //
   if (fCutStep < 1e-5) return -1;
   return (Int_t)((fCutMax - fCutMin) / fCutStep);
}

//_________________________________________________________________________________________________
Int_t IlcMixEventCutObj::GetBinNumber(Float_t num) const
{
   //
   // Returns bin (index) number in current cut.
   // Returns -1 in case of out of range
   //
   Int_t binNum = 0;
   for (Float_t iCurrent = fCutMin; iCurrent < fCutMax; iCurrent += fCutStep) {
      binNum++;
      if ((num >= iCurrent) && (num < iCurrent + fCutStep - fCutSmallVal)) {
         return binNum;
      }
   }
   return -1;
}

//_________________________________________________________________________________________________
Int_t IlcMixEventCutObj::GetIndex(IlcVEvent *ev)
{
   //
   // Finds bin (index) in current cut from event information.
   //
   return GetBinNumber(GetValue(ev));
}

//_________________________________________________________________________________________________
Double_t IlcMixEventCutObj::GetValue(IlcVEvent *ev)
{
   //
   // Returns value from event
   //

   IlcESDEvent *esd = dynamic_cast<IlcESDEvent *>(ev);
   if (esd) return GetValue(esd);
   IlcAODEvent *aod = dynamic_cast<IlcAODEvent *>(ev);
   if (aod) return GetValue(aod);

   IlcFatal("Event is not supported in Event Mixing cuts!!!!");
   return -99999;
}

//_________________________________________________________________________________________________
Double_t IlcMixEventCutObj::GetValue(IlcESDEvent *ev)
{
   //
   // Returns value from esd event
   //

   const IlcMultiplicity *multESD = 0;

   switch (fCutType) {
      case kMultiplicity:
         return (Double_t)ev->GetNumberOfTracks();
      case kZVertex:
         return ev->GetVertex()->GetZ();
      case kNumberV0s:
         return ev->GetNumberOfV0s();
      case kNumberTracklets:
         multESD = ev->GetMultiplicity();
         if (multESD) return multESD->GetNumberOfTracklets();
         else IlcFatal("esd->GetMultiplicity() is null");
         break;
      case kCentrality:
      {
         IlcCentrality *c = ev->GetCentrality();
         if (!c) IlcFatal("esd->GetCentrality() is null");
         if (fCutOpt.IsNull()) IlcFatal("fCutOpt is null");
         return c->GetCentralityPercentile(fCutOpt.Data());
      }
      case kEventPlane:
      {
         IlcEventplane *evtPlane = new IlcEventplane();
         if (fCutOpt.IsNull()) IlcFatal("fCutOpt is null");
         if (!fCutOpt.IsDigit()) IlcFatal("fCutOpt is not a digit string");
         Double_t val = evtPlane->GetEventplane("V0",ev,fCutOpt.Atoi());
         delete evtPlane;
         return val;
      }
   }

   IlcFatal("Mixing Cut TYPE is not supported for ESD");
   return -99999;

}

//_________________________________________________________________________________________________
Double_t IlcMixEventCutObj::GetValue(IlcAODEvent *ev)
{
   //
   // Returns value from aod event
   //

   IlcAODVertex *v=0;
   switch (fCutType) {
      case kMultiplicity:
         return (Double_t) ev->GetNumberOfTracks();
      case kZVertex:
         v = ev->GetVertex(0);
         if (!v)  return -99999;
         return ev->GetVertex(0)->GetZ();
         // if verttex is null return -9999
         return -99999;
      case kNumberV0s:
         return ev->GetNumberOfV0s();
      case kCentrality:
      {
         IlcCentrality *c = ev->GetCentrality();
         if (!c) IlcFatal("esd->GetCentrality() is null");
         if (fCutOpt.IsNull()) IlcFatal("fCutOpt is null");
         return c->GetCentralityPercentile(fCutOpt.Data());
      }
      case kEventPlane:
      {
         IlcEventplane *evtPlane = new IlcEventplane();
         if (fCutOpt.IsNull()) IlcFatal("fCutOpt is null");
         if (!fCutOpt.IsDigit()) IlcFatal("fCutOpt is not a digit string");
         Double_t val = evtPlane->GetEventplane("V0",ev,fCutOpt.Atoi());
         delete evtPlane;
         return val;
      }
   }

   IlcFatal("Mixing Cut TYPE is not supported for AOD");
   return -99999;
}

//_________________________________________________________________________________________________
const char *IlcMixEventCutObj::GetCutName(Int_t index) const
{
   //
   // Retruns name of cut
   //

   if (index < 0) index = fCutType;
   switch (index) {
      case kMultiplicity:
         return "Multiplicity";
      case kZVertex:
         return "ZVertex";
      case kNumberV0s:
         return "NumberV0s";
      case kNumberTracklets:
         return "NumberTracklets";
      case kCentrality:
         return Form("kCentrality[%s]", fCutOpt.Data());
      case kEventPlane:
         return Form("EventPlane[%s]", fCutOpt.Data());
   }
   return "";
}

//_________________________________________________________________________________________________
void IlcMixEventCutObj::SetCurrentValueToIndex(Int_t index)
{
   //
   // Sets current value to index
   //
   for (Int_t i = 0; i < index; i++) AddStep();
}

//_________________________________________________________________________________________________
void IlcMixEventCutObj::PrintValues(IlcVEvent *main, IlcVEvent *mix)
{
   //
   // Prints values of both events for current type
   //
   IlcInfo(Form("name=%s main=%f mix=%f", GetCutName(), GetValue(main), GetValue(mix)));
}


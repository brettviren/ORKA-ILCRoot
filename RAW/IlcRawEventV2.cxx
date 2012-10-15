// Author: Cvetan Cheshkov  11/05/2009

/**************************************************************************
 * 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * Author: The International Lepton Collider Off-line Project.            *
 *                                                                        *
 * Part of the code has been developed by ILC Off-line Project.         *
 * Contributors are mentioned in the code where appropriate.              *
 *                                                                        *
 * Permission to use, copy, modify and distribute this software and its   *
 * documentation strictly for non-commercial purposes is hereby granted   *
 * without fee, provided that the above copyright notice appears in all   *
 * copies and that both the copyright notice and this permission notice   *
 * appear in the supporting documentation. The authors make no claims     *
 * about the suitability of this software for any purpose. It is          *
 * provided "as is" without express or implied warranty.                  *
 **************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// IlcRawEventV2                                                          //
//                                                                      //
// Set of classes defining the ILC RAW event format. The IlcRawEventV2  //
// class defines a RAW event. It consists of an IlcEventHeader object   //
// an IlcEquipmentHeader object, an IlcRawData object and an array of   //
// sub-events, themselves also being IlcRawEventV2s. The number of        //
// sub-events depends on the number of DATE LDC's.                      //
// The IlcRawEventV2 objects are written to a ROOT file using different   //
// technologies, i.e. to local disk via IlcRawDB or via rfiod using     //
// IlcRawRFIODB or via rootd using IlcRawRootdDB or to CASTOR via       //
// rootd using IlcRawCastorDB (and for performance testing there is     //
// also IlcRawNullDB).                                                  //
// The IlcStats class provides statics information that is added as     //
// a single keyed object to each raw file.                              //
// The IlcTagDB provides an interface to a TAG database.                //
// The IlcMDC class is usid by the "ilcmdc" stand-alone program         //
// that reads data directly from DATE.                                  //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include <TObjArray.h>
#include <TClass.h>

#include "IlcLog.h"

#include "IlcRawEventHeaderBase.h"
#include "IlcRawEquipmentV2.h"

#include "IlcRawEventV2.h"


ClassImp(IlcRawEventV2)


//______________________________________________________________________________
IlcRawEventV2::IlcRawEventV2():
IlcRawVEvent(),
fEquipments("IlcRawEquipmentV2",1000),
fEvtHdrs(NULL),
fIndex(0),
fNAllocHdrs(0),
fNAllocEqs(0)
{
   // Create ILC event object. If ownData is kFALSE we will use a static
   // raw data object, otherwise a private copy will be made.

}

//______________________________________________________________________________
IlcRawEventHeaderBase *IlcRawEventV2::GetHeader(char*& data)
{
  // Get event header part of IlcRawEventV2.
  // First the DATE version is identified and then the
  // corresponding event header version object is created

  IlcRawEventHeaderBase *hdr = NULL;

  if (!fEvtHdrs) {
    hdr = IlcRawEventHeaderBase::Create(data);
    hdr->IsA()->IgnoreTObjectStreamer();
    fEvtHdrs = new TClonesArray(hdr->IsA()->GetName(),100);
    delete hdr;
  }

  if (fIndex < fNAllocHdrs) {
    TClonesArray &arr = *fEvtHdrs;
    return (IlcRawEventHeaderBase *)arr[fIndex];
  }
  else {
    fNAllocHdrs = fIndex + 1;
    return (IlcRawEventHeaderBase *)fEvtHdrs->New(fIndex);
  }
}

//______________________________________________________________________________
IlcRawEventHeaderBase *IlcRawEventV2::GetHeader()
{
  IlcRawEventHeaderBase *hdr = NULL;
  if (!fEvtHdrs || !(hdr = (IlcRawEventHeaderBase *)fEvtHdrs->UncheckedAt(fIndex))) {
    IlcFatal("Event header does not exist!");
    return NULL;
  }

  return hdr;
}

//______________________________________________________________________________
IlcRawEquipmentV2 *IlcRawEventV2::NextEquipment()
{
   // Returns next equipment object.

  IlcRawEventHeaderBase *hdr = (IlcRawEventHeaderBase *)fEvtHdrs->UncheckedAt(fIndex);
  Int_t nEquipments = fEquipments.GetEntriesFast();
  hdr->AddEqIndex(nEquipments);

  if (nEquipments < fNAllocEqs) {
    return (IlcRawEquipmentV2 *)fEquipments[nEquipments];
  }
  else {
    fNAllocEqs = nEquipments + 1;
    return new (fEquipments[nEquipments]) IlcRawEquipmentV2();
  }
}

//______________________________________________________________________________
IlcRawVEquipment *IlcRawEventV2::GetEquipment(Int_t index) const
{
   // Get specified equipment. Returns 0 if equipment does not exist.

  //   if (!fEquipments)
  //      return NULL;

   IlcRawEventHeaderBase *hdr = NULL;
   if (!fEvtHdrs || !(hdr = (IlcRawEventHeaderBase *)fEvtHdrs->UncheckedAt(fIndex))) {
     IlcFatal("Header is not yet initialized!");
     return NULL;
   }

   if ((index + hdr->GetFirstEqIndex()) > hdr->GetLastEqIndex()) {
     IlcFatal("Equipment index out of scope!");
     return NULL;
   }     

   return (IlcRawVEquipment *) fEquipments.UncheckedAt(index+hdr->GetFirstEqIndex());
}


//______________________________________________________________________________
Int_t IlcRawEventV2::GetNEquipments() const
{
  //   if (!fEquipments)
  //      return 0;
  
   IlcRawEventHeaderBase *hdr = NULL;
   if (!fEvtHdrs || !(hdr = (IlcRawEventHeaderBase *)fEvtHdrs->UncheckedAt(fIndex))) {
     IlcFatal("Header is not yet initialized!");
     return 0;
   }

   return (hdr->GetFirstEqIndex() < 0) ? 0 : (hdr->GetLastEqIndex() - hdr->GetFirstEqIndex() + 1);
}

//______________________________________________________________________________
IlcRawEventV2 *IlcRawEventV2::NextSubEvent()
{
   // Returns next sub-event object.

  fIndex++;

  return this;
}

//______________________________________________________________________________
IlcRawVEvent *IlcRawEventV2::GetSubEvent(Int_t index)
{
   // Get specified sub event. Returns 0 if sub event does not exist.

  if (!fEvtHdrs) {
    IlcFatal("Headers are not yet initialized!");
    return NULL;
  }

  fIndex = index + 1;

  return this;
}

//______________________________________________________________________________
void IlcRawEventV2::Reset()
{
   // Reset the event in case it needs to be re-used (avoiding costly
   // new/delete cycle). We reset the size marker for the IlcRawData
   // objects and the sub event counter.

  fEquipments.Clear();

  if (fEvtHdrs) {
    for (int i = 0; i < fEvtHdrs->GetEntriesFast(); i++) {
      IlcRawEventHeaderBase *hdr = (IlcRawEventHeaderBase *)fEvtHdrs->UncheckedAt(i);
      hdr->Reset();
    }
    fEvtHdrs->Clear();
  }
  fIndex = 0;
}

//______________________________________________________________________________
IlcRawEventV2::~IlcRawEventV2()
{
   // Clean up event object. Delete also, possible, private raw data.

  //   if (fEquipments)
  fEquipments.Delete();
  //   delete fEquipments;
   if (fEvtHdrs)
      fEvtHdrs->Delete();
   delete fEvtHdrs;
}

//______________________________________________________________________________
void IlcRawEventV2::Clear(Option_t*)
{
   // Clear the event in case it needs to be re-used (avoiding costly
   // new/delete cycle). Can be used inside the event loop.

  fEquipments.Clear("C");

  if (fEvtHdrs) {
    for (int i = 0; i < fEvtHdrs->GetEntriesFast(); i++) {
      IlcRawEventHeaderBase *hdr = (IlcRawEventHeaderBase *)fEvtHdrs->UncheckedAt(i);
      hdr->Reset();
    }
    fEvtHdrs->Clear();
  }
  fIndex = 0;
}


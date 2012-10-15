// @(#) $Id: IlcRawEvent.cxx 32403 2009-05-15 14:28:39Z hristov $
// Author: Fons Rademakers  26/11/99

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
// IlcRawEvent                                                          //
//                                                                      //
// Set of classes defining the ILC RAW event format. The IlcRawEvent  //
// class defines a RAW event. It consists of an IlcEventHeader object   //
// an IlcEquipmentHeader object, an IlcRawData object and an array of   //
// sub-events, themselves also being IlcRawEvents. The number of        //
// sub-events depends on the number of DATE LDC's.                      //
// The IlcRawEvent objects are written to a ROOT file using different   //
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

#include "IlcLog.h"

#include "IlcRawEventHeaderBase.h"
#include "IlcRawEquipment.h"

#include "IlcRawEvent.h"


ClassImp(IlcRawEvent)


//______________________________________________________________________________
IlcRawEvent::IlcRawEvent():
fNEquipments(0),
fNSubEvents(0),
fEvtHdr(NULL),
fEquipments(NULL),
fSubEvents(NULL)
{
   // Create ILC event object. If ownData is kFALSE we will use a static
   // raw data object, otherwise a private copy will be made.

}

//______________________________________________________________________________
IlcRawEventHeaderBase *IlcRawEvent::GetHeader()
{
  if (!fEvtHdr) {
    IlcFatal("Event header does not exist!");
    return 0x0;
  }

  return fEvtHdr;
}

//______________________________________________________________________________
IlcRawVEquipment *IlcRawEvent::GetEquipment(Int_t index) const
{
   // Get specified equipment. Returns 0 if equipment does not exist.

   if (!fEquipments)
      return 0;

   return (IlcRawEquipment *) fEquipments->At(index);
}

//______________________________________________________________________________
IlcRawVEvent *IlcRawEvent::GetSubEvent(Int_t index)
{
   // Get specified sub event. Returns 0 if sub event does not exist.

   if (!fSubEvents)
      return 0;

   return (IlcRawEvent *) fSubEvents->At(index);
}

//______________________________________________________________________________
IlcRawEvent::~IlcRawEvent()
{
   // Clean up event object. Delete also, possible, private raw data.

   delete fEvtHdr;
   if (fEquipments)
      fEquipments->Delete();
   delete fEquipments;
   if (fSubEvents)
      fSubEvents->Delete();
   delete fSubEvents;
}

//______________________________________________________________________________
void IlcRawEvent::Streamer(TBuffer &R__b)
{
   // Stream an object of class IlcRawEvent.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      TObject::Streamer(R__b);
      R__b >> fNEquipments;
      R__b >> fNSubEvents;
      R__b >> fEvtHdr;
      R__b >> fEquipments;
      R__b >> fSubEvents;
      R__b.CheckByteCount(R__s, R__c, IlcRawEvent::IsA());
   } else {
      R__c = R__b.WriteVersion(IlcRawEvent::IsA(), kTRUE);
      TObject::Streamer(R__b);
      R__b << fNEquipments;
      R__b << fNSubEvents;
      R__b << fEvtHdr;
      R__b << fEquipments;
      R__b << fSubEvents;
      R__b.SetByteCount(R__c, kTRUE);
   }
}

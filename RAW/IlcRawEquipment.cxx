// @(#) $Id: IlcRawEquipment.cxx 46017 2010-12-14 12:01:07Z cvetan $
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

#include <IlcRawDataArray.h>

#include "IlcRawEquipmentHeader.h"
#include "IlcRawData.h"

#include "IlcRawEquipment.h"


ClassImp(IlcRawEquipment)


//______________________________________________________________________________
IlcRawEquipment::IlcRawEquipment():
fEqpHdr(NULL),
fRawData(NULL),
fRawDataRef(NULL)
{
   // Create ILC equipment object.

}

//______________________________________________________________________________
IlcRawEquipmentHeader *IlcRawEquipment::GetEquipmentHeader()
{
   // Get equipment header part of IlcRawEquipment.

   if (!fEqpHdr)
      fEqpHdr = new IlcRawEquipmentHeader;

   return fEqpHdr;
}

//______________________________________________________________________________
IlcRawData *IlcRawEquipment::GetRawData()
{
   // Get raw data part of IlcRawEquipment.

  if (!fRawData) {
    if (!fRawDataRef.IsValid())
      fRawData = new IlcRawData;
    else {
      fRawData = (IlcRawData*)fRawDataRef.GetObject();
    }
  }
  return fRawData;
}

//______________________________________________________________________________
IlcRawEquipment::~IlcRawEquipment()
{
   // Clean up event object. Delete also, possible, private raw data.

   delete fEqpHdr;
   delete fRawData;
}

//______________________________________________________________________________
void IlcRawEquipment::CloneRawData(const IlcRawData *rawData)
{
  // Clone the input raw data and
  // flush the TRef

  fRawDataRef = NULL;
  if (rawData) fRawData = (IlcRawData*)rawData->Clone();
}

//______________________________________________________________________________
void IlcRawEquipment::Streamer(TBuffer &R__b)
{
   // Stream an object of class IlcRawEquipment.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      TObject::Streamer(R__b);
      R__b >> fEqpHdr;
      R__b >> fRawData;
      fRawDataRef.Streamer(R__b);
      R__b.CheckByteCount(R__s, R__c, IlcRawEquipment::IsA());
   } else {
      R__c = R__b.WriteVersion(IlcRawEquipment::IsA(), kTRUE);
      TObject::Streamer(R__b);
      R__b << fEqpHdr;
      R__b << fRawData;
      fRawDataRef.Streamer(R__b);
      R__b.SetByteCount(R__c, kTRUE);
   }
}

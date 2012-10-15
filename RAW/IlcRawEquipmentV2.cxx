// @(#) $Id: IlcRawEquipment.cxx 23318 2008-01-14 12:43:28Z hristov $
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
// IlcRawEquipmentV2                                                    //
//                                                                      //
// Set of classes defining the ILC RAW event format. The IlcRawEventV2//
// class defines a RAW event. It consists of an IlcEventHeader object   //
// an IlcEquipmentHeader object, an IlcRawData object and an array of   //
// sub-events, themselves also being IlcRawEventV2s. The number of      //
// sub-events depends on the number of DATE LDC's.                      //
// The IlcRawEventV2 objects are written to a ROOT file using different //
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

#include "IlcRawEquipmentV2.h"
#include "IlcRawDataArrayV2.h"
#include "IlcRawData.h"

ClassImp(IlcRawEquipmentV2)

//______________________________________________________________________________
IlcRawEquipmentV2::IlcRawEquipmentV2():
IlcRawVEquipment(),
fEqpHdr(),
fRawData(NULL),
fRawDataRef(NULL)
{
   // Create ILC equipment object.

}

//______________________________________________________________________________
IlcRawEquipmentHeader *IlcRawEquipmentV2::GetEquipmentHeader()
{
   // Get equipment header part of IlcRawEquipmentV2.

   return &fEqpHdr;
}

//______________________________________________________________________________
IlcRawData *IlcRawEquipmentV2::GetRawData()
{
   // Get raw data part of IlcRawEquipmentV2.

  if (!fRawData) {
    if (fRawDataRef.IsValid()) {
      fRawData = (IlcRawData*)fRawDataRef.GetObject();
    }
  }
  return fRawData;
}

//______________________________________________________________________________
void IlcRawEquipmentV2::Reset()
{
   // Reset the equipment in case it needs to be re-used (avoiding costly
   // new/delete cycle). We reset the size marker for the IlcRawData
   // object.

   fEqpHdr.Reset();
   fRawDataRef = NULL;
}

//______________________________________________________________________________
void IlcRawEquipmentV2::Clear(Option_t*)
{
   // Clear the equipment in case it needs to be re-used (avoiding costly
   // new/delete cycle). Called by IlcRawEventV2 Clear method inside the event loop.

   fEqpHdr.Reset();
   fRawDataRef = NULL;
   fRawData = NULL;
}

//______________________________________________________________________________
IlcRawEquipmentV2::~IlcRawEquipmentV2()
{
   // Clean up event object. Delete also, possible, private raw data.

   if (!fRawDataRef.IsValid()) delete fRawData;
}

//______________________________________________________________________________
IlcRawData *IlcRawEquipmentV2::NextRawData(IlcRawDataArrayV2 *array)
{
  // Get a pointer to the raw-data object
  // stored within an array in a separate
  // branch of the tree.
  // Set the reference to the raw-data object

  IlcRawData *raw = NULL;
  if (array) {
    raw = array->Add();
    fRawDataRef = raw;
  }
  else {
    Error("NextRawData", "Raw-data array does not exist! Can not set a reference to a raw-data object!");    
    fRawDataRef = NULL;
  }

  return raw;
}

//______________________________________________________________________________
void IlcRawEquipmentV2::CloneRawData(const IlcRawData *rawData)
{
  // Clone the input raw data and
  // flush the TRef

  fRawDataRef = NULL;
  if (rawData) fRawData = (IlcRawData*)rawData->Clone();
}

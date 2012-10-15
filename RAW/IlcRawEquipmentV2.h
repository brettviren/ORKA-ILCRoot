#ifndef ILCRAWEQUIPMENTV2_H
#define ILCRAWEQUIPMENTV2_H
// Author: Cvetan Cheshkov 11/05/2009

/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

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
// The IlcRunDB class provides the interface to the run and file        //
// catalogues (AliEn or plain MySQL).                                   //
// The IlcStats class provides statics information that is added as     //
// a single keyed object to each raw file.                              //
// The IlcTagDB provides an interface to a TAG database.                //
// The IlcMDC class is usid by the "ilcmdc" stand-alone program         //
// that reads data directly from DATE.                                  //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#ifndef ROOT_TObject
#include <TObject.h>
#endif
#include <TRef.h>

// Forward class declarations
class IlcRawDataArrayV2;

#include "IlcRawVEquipment.h"
#include "IlcRawEquipmentHeader.h"

class IlcRawEquipmentV2 : public IlcRawVEquipment {

public:
   IlcRawEquipmentV2();
   virtual ~IlcRawEquipmentV2();

   virtual IlcRawEquipmentHeader *GetEquipmentHeader();
   virtual IlcRawData            *GetRawData();
   void                           Reset();
   virtual void	                  Clear(Option_t* = "");
   IlcRawData                    *NextRawData(IlcRawDataArrayV2 *array);

   virtual void                   CloneRawData(const IlcRawData *rawData);

private:
   IlcRawEquipmentHeader  fEqpHdr;      // equipment header
   IlcRawData            *fRawData;     //! raw data container
   TRef                   fRawDataRef;  // reference to raw data container

   IlcRawEquipmentV2(const IlcRawEquipmentV2& eq);
   IlcRawEquipmentV2& operator = (const IlcRawEquipmentV2& eq);

   ClassDef(IlcRawEquipmentV2,1)  // ILC raw equipment object
};

#endif

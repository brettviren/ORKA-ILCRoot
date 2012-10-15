#ifndef ILCRAWEQUIPMENT_H
#define ILCRAWEQUIPMENT_H
// @(#) $Id: IlcRawEquipment.h 34635 2009-09-04 19:52:00Z cvetan $
// Author: Fons Rademakers  26/11/99
// Updated: Dario Favretto  15/04/2003

/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

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
class IlcRawDataArray;
class IlcRawEquipmentHeader;
class IlcRawData;

#include "IlcRawVEquipment.h"

class IlcRawEquipment : public IlcRawVEquipment {

public:
   IlcRawEquipment();
   virtual ~IlcRawEquipment();

   virtual IlcRawEquipmentHeader *GetEquipmentHeader();
   virtual IlcRawData            *GetRawData();

   virtual void                   CloneRawData(const IlcRawData *rawData);

 private:
   IlcRawEquipmentHeader *fEqpHdr;      // equipment header
   IlcRawData            *fRawData;     // raw data container
   TRef                   fRawDataRef;  // reference to raw data container

   IlcRawEquipment(const IlcRawEquipment& rawEvent);
   IlcRawEquipment& operator = (const IlcRawEquipment& rawEvent);

   ClassDef(IlcRawEquipment,3)  // ILC raw equipment object
};

#endif

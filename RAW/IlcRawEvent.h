#ifndef ILCRAWEVENT_H
#define ILCRAWEVENT_H
// @(#) $Id: IlcRawEvent.h 32403 2009-05-15 14:28:39Z hristov $
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

#ifndef ROOT_TObjArray
#include <TObjArray.h>
#endif


// Forward class declarations
class IlcRawEventHeaderBase;

#include "IlcRawVEvent.h"

class IlcRawEvent : public IlcRawVEvent {

public:
   IlcRawEvent();
   virtual ~IlcRawEvent();

   virtual IlcRawEventHeaderBase *GetHeader();
   virtual Int_t                  GetNEquipments() const { return fNEquipments; }
   virtual IlcRawVEquipment       *GetEquipment(Int_t index) const;
   virtual Int_t                  GetNSubEvents() const { return fNSubEvents; }
   virtual IlcRawVEvent           *GetSubEvent(Int_t index);

private:
   Int_t                  fNEquipments; // number of valid equipments
   Int_t                  fNSubEvents;  // number of valid sub-events
   IlcRawEventHeaderBase *fEvtHdr;      // event header object
   TObjArray             *fEquipments;  // IlcRawEquipment's
   TObjArray             *fSubEvents;   // sub IlcRawEvent's

   IlcRawEvent(const IlcRawEvent& rawEvent);
   IlcRawEvent& operator = (const IlcRawEvent& rawEvent);

   ClassDef(IlcRawEvent,3)  // ILC raw event object
};

#endif

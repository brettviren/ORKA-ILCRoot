#ifndef ILCRAWVEVENT_H
#define ILCRAWVEVENT_H
// Author: Cvetan Cheshkov  11/05/2009

/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// IlcRawVEvent                                                         //
//                                                                      //
// Set of classes defining the ILC RAW event format. The IlcRawVEvent //
// class defines a RAW event. It consists of an IlcEventHeader object   //
// an IlcEquipmentHeader object, an IlcRawData object and an array of   //
// sub-events, themselves also being IlcRawVEvents. The number of        //
// sub-events depends on the number of DATE LDC's.                      //
// The IlcRawVEvent objects are written to a ROOT file using different   //
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

#ifndef ROOT_TClonesArray
#include <TClonesArray.h>
#endif


// Forward class declarations
class IlcRawEventHeaderBase;
class IlcRawVEquipment;


class IlcRawVEvent : public TObject {

public:
   IlcRawVEvent() { }
   virtual ~IlcRawVEvent() { }

   virtual IlcRawEventHeaderBase *GetHeader() = 0;
   virtual Int_t                  GetNEquipments() const = 0;
   virtual IlcRawVEquipment      *GetEquipment(Int_t index) const = 0;
   virtual Int_t                  GetNSubEvents() const = 0;
   virtual IlcRawVEvent          *GetSubEvent(Int_t index) = 0;

private:

   IlcRawVEvent(const IlcRawVEvent& rawEvent);
   IlcRawVEvent& operator = (const IlcRawVEvent& rawEvent);

   ClassDef(IlcRawVEvent,1)  // ILC raw event object
};

#endif

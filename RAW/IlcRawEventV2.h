#ifndef ILCRAWEVENTV2_H
#define ILCRAWEVENTV2_H
// Author: Cvetan Cheshkov  11/05/2009

/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

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
class IlcRawEquipmentV2;

#include "IlcRawVEvent.h"

class IlcRawEventV2 : public IlcRawVEvent {

public:
   IlcRawEventV2();
   virtual ~IlcRawEventV2();

   IlcRawEventHeaderBase *GetHeader(char*& data);
   virtual IlcRawEventHeaderBase *GetHeader();
   virtual Int_t                  GetNEquipments() const;
   IlcRawEquipmentV2             *NextEquipment();
   virtual IlcRawVEquipment      *GetEquipment(Int_t index) const;
   virtual Int_t                  GetNSubEvents() const { return (fEvtHdrs) ? (fEvtHdrs->GetEntriesFast()-1) : 0; }
   IlcRawEventV2                 *NextSubEvent();
   virtual IlcRawVEvent          *GetSubEvent(Int_t index);
   void                           Reset();
   virtual void	                  Clear(Option_t* = "");

private:
   TClonesArray           fEquipments;  // IlcRawEquipmentV2's
   TClonesArray          *fEvtHdrs;     //-> event and subevent headers

   Int_t                  fIndex;       //!
   Int_t                  fNAllocHdrs;  //!
   Int_t                  fNAllocEqs;   //!

   IlcRawEventV2(const IlcRawEventV2& rawEvent);
   IlcRawEventV2& operator = (const IlcRawEventV2& rawEvent);

   ClassDef(IlcRawEventV2,1)  // ILC raw event object
};

#endif

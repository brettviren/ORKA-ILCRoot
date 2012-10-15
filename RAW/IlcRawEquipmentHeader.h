#ifndef ILCRAWEQUIPMENTHEADER_H
#define ILCRAWEQUIPMENTHEADER_H
// @(#) $Id: IlcRawEquipmentHeader.h 32401 2009-05-15 14:01:03Z cvetan $
// Author: Fons Rademakers  26/11/99

/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// IlcRawEquipmentHeader                                                //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#ifndef ROOT_TObject
#include <TObject.h>
#endif


class IlcRawEquipmentHeader : public TObject {

public:
   IlcRawEquipmentHeader();
   ~IlcRawEquipmentHeader() { }

   void         *HeaderBegin() { return (void *) &fSize; }
   Int_t         HeaderSize() const { return (Long_t) &fBasicElementSizeType - (Long_t) &fSize + sizeof(fBasicElementSizeType); }
   UInt_t        SwapWord(UInt_t x) const;
   void          Swap();

   UInt_t        GetEquipmentSize() const { return fSize; }
   UInt_t        GetEquipmentType() const { return fEquipmentType; }
   UInt_t        GetId() const { return fEquipmentID; }
   const UInt_t *GetTypeAttribute() const { return fTypeAttribute; }
   UInt_t        GetBasicSizeType() const { return fBasicElementSizeType; }

   void          Reset();

   void          Print( const Option_t* opt ="" ) const;

   enum {
     kAttributeWords = 3
   };

private:
   UInt_t fSize;                            // number of raw data bytes
   UInt_t fEquipmentType;                   // equipment type
   UInt_t fEquipmentID;                     // equipment ID
   UInt_t fTypeAttribute[kAttributeWords];  // system (0,1) and user (2) attributes
   UInt_t fBasicElementSizeType;            // basic element size type

   ClassDef(IlcRawEquipmentHeader,2) //Ilc equipment header
};

#endif

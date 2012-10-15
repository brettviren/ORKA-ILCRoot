#ifndef ILCRAWEVENTHEADERBASE_H
#define ILCRAWEVENTHEADERBASE_H

// Author: Cvetan Cheshkov  10/10/2005

/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// IlcRawEventHeaderBase                                                //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include <TObject.h>

class IlcRawEventHeaderBase : public TObject {

public:
   IlcRawEventHeaderBase();
   virtual ~IlcRawEventHeaderBase() { if (fExtendedData) delete [] fExtendedData; }

   void         *HeaderBaseBegin() const { return (void *) &fSize; }
   Int_t         HeaderBaseSize() const { return (Long_t) &fVersion - (Long_t) &fSize + sizeof(fVersion); }
   void         *HeaderBegin() const;
   Int_t         HeaderSize() const;
   Bool_t        DataIsSwapped() const { return fIsSwapped; }
   Bool_t        IsSwapped() const { return (fMagic == fgkEventMagicNumberSwapped) ? kTRUE : kFALSE; }
   Bool_t        IsValid() const { return IsSwapped() ? kTRUE : ((fMagic == fgkEventMagicNumber) ? kTRUE : kFALSE); }
   void          Swap();
   UInt_t        SwapWord(UInt_t x) const;
   void          SwapData(const void* data, const void* buf, UInt_t size);

   UInt_t        GetEventSize() const { return fSize; }
   UInt_t        GetMagic() const { return fMagic; }
   UInt_t        GetHeadSize() const { return fHeadSize; }
   UInt_t        GetVersion() const { return fVersion; }
   UInt_t        GetMajorVersion() const { return ((fVersion>>16)&0x0000ffff); }
   UInt_t        GetMinorVersion() const { return (fVersion&0x0000ffff); }

   UInt_t        GetExtendedDataSize() const { return fExtendedDataSize; }
   char*         GetExtendedData() const { return fExtendedData; }

   const char *  GetTypeName() const;
   static const char *  GetTypeName(UInt_t eventType);
   static IlcRawEventHeaderBase* Create(char*& data);
   void          AllocateExtendedData(Int_t extsize);
   Int_t         ReadHeader(char*& data);
   Int_t         ReadExtendedData(char*& data);
   UInt_t        Get(const char *datamember) const;
   const UInt_t* GetP(const char *datamember) const;

   void          Print( const Option_t* opt ="" ) const;

   Int_t         GetFirstEqIndex() const { return fFirstEqIndex; }
   Int_t         GetLastEqIndex() const { return fLastEqIndex; }
   void          AddEqIndex(Int_t index);
   void          Reset();

   // The following enumeration can be used once the kEventTypeMask has been
   // applied to the raw event type
   enum EIlcRawEventType {
     kStartOfRun =       1,    // START_OF_RUN
     kEndOfRun =         2,    // END_OF_RUN
     kStartOfRunFiles =  3,    // START_OF_RUN_FILES
     kEndOfRunFiles =    4,    // END_OF_RUN_FILES
     kStartOfBurst =     5,    // START_OF_BURST
     kEndOfBurst =       6,    // END_OF_BURST
     kPhysicsEvent =     7,    // PHYSICS_EVENT
     kCalibrationEvent = 8,    // CALIBRATION_EVENT
     kFormatError =      9,    // EVENT_FORMAT_ERROR
     kStartOfData =      10,   // START_OF_DATA
     kEndOfData =        11,   // END_OF_DATA
     kSystemSoftwareTriggerEvent   = 12, // SYSTEM_SOFTWARE_TRIGGER_EVENT
     kDetectorSoftwareTriggerEvent = 13, // DETECTOR_SOFTWARE_TRIGGER_EVENT
     kSyncEvent = 14  // SYNC_EVENT
   };

private:
   IlcRawEventHeaderBase(const IlcRawEventHeaderBase&);
   IlcRawEventHeaderBase& operator=(const IlcRawEventHeaderBase&);

   UInt_t fSize;          // size of event in bytes
   UInt_t fMagic;         // magic number used for consistency check
   UInt_t fHeadSize;      // size of header in bytes
   UInt_t fVersion;       // unique version identifier

   UInt_t fExtendedDataSize; // size of header extension data
   UInt_t fExtendedAllocSize;//! size of allocated memory for header extension data
   char  *fExtendedData;  //[fExtendedDataSize] pointer to header extension data 

   Bool_t fIsSwapped;     // is data swapped
   Int_t  fHeaderSize;    //! cache for the header size estimate
   void  *fHeaderBegin;   //! cache for the header begin pointer

   Int_t  fFirstEqIndex;  // index of the first equipment
   Int_t  fLastEqIndex;   // index of the last equipment

   static const UInt_t fgkEventMagicNumber        = 0xDA1E5AFE; // magic word
   static const UInt_t fgkEventMagicNumberSwapped = 0xFE5A1EDA; // swapped magic word

   ClassDef(IlcRawEventHeaderBase,4)  // Ilc raw event header base class
};

#define EVENT_HEADER_VERSION(AA,BB) IlcRawEventHeaderV##AA##_##BB
#define INIT_HEADER_VARS

#define START_EVENT_HEADER(AA,BB) \
class IlcRawEventHeaderV##AA##_##BB:public IlcRawEventHeaderBase { \
public: \
 IlcRawEventHeaderV##AA##_##BB():IlcRawEventHeaderBase(),		\
   INIT_HEADER_VARS {};	\
   virtual ~IlcRawEventHeaderV##AA##_##BB() {}; \
private:

#define END_EVENT_HEADER(AA,BB) \
ClassDef(IlcRawEventHeaderV##AA##_##BB,1) \
};

#endif

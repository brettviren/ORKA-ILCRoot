#ifndef ILCRAWREADERROOT_H
#define ILCRAWREADERROOT_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcRawReaderRoot.h 53108 2011-11-23 10:02:07Z hristov $ */

///////////////////////////////////////////////////////////////////////////////
///
/// This is a class for reading raw data from a root file.
///
///////////////////////////////////////////////////////////////////////////////

#include "IlcRawReader.h"

class IlcRawVEvent;
class IlcRawVEquipment;
class IlcRawData;
class TFile;
class TBranch;


class IlcRawReaderRoot: public IlcRawReader {
  public :
    IlcRawReaderRoot();
    IlcRawReaderRoot(const char* fileName, Int_t eventNumber = -1);
    IlcRawReaderRoot(IlcRawVEvent* event);
    IlcRawReaderRoot(const IlcRawReaderRoot& rawReader);
    IlcRawReaderRoot& operator = (const IlcRawReaderRoot& rawReader);
    virtual ~IlcRawReaderRoot();

    virtual const IlcRawEventHeaderBase* GetEventHeader() const;

    virtual UInt_t   GetType() const;
    virtual UInt_t   GetRunNumber() const;
    virtual const UInt_t* GetEventId() const;
    virtual const UInt_t* GetTriggerPattern() const;
    virtual const UInt_t* GetDetectorPattern() const;
    virtual const UInt_t* GetAttributes() const;
    virtual const UInt_t* GetSubEventAttributes() const;
    virtual UInt_t   GetLDCId() const;
    virtual UInt_t   GetGDCId() const;
    virtual UInt_t   GetTimestamp() const;

    virtual Int_t    GetEquipmentSize() const;
    virtual Int_t    GetEquipmentType() const;
    virtual Int_t    GetEquipmentId() const;
    virtual const UInt_t* GetEquipmentAttributes() const;
    virtual Int_t    GetEquipmentElementSize() const;
    virtual Int_t    GetEquipmentHeaderSize() const;

    virtual Bool_t   ReadHeader();
    virtual Bool_t   ReadNextData(UChar_t*& data);
    virtual Bool_t   ReadNext(UChar_t* data, Int_t size);

    virtual Bool_t   Reset();

    virtual Bool_t   NextEvent();
    virtual Bool_t   RewindEvents();
    virtual Bool_t   GotoEvent(Int_t event);
    virtual Int_t    GetEventIndex() const { return fEventIndex; }
    virtual Int_t    GetNumberOfEvents() const;

    virtual Int_t    CheckData() const;

    virtual const IlcRawVEvent* GetEvent() const {return fEvent;}

    virtual IlcRawReader* CloneSingleEvent() const;
    static Bool_t           GetUseOrder() {return fgUseOrder;}
    static void             UseOrder() {fgUseOrder = kTRUE;}

  protected :
    TFile*           fFile;         // raw data root file
    TBranch*         fBranch;       // branch of raw events
    Int_t            fEventIndex;   // index of the event in the tree
    IlcRawVEvent*    fEvent;        // (super) event
    IlcRawEventHeaderBase* fEventHeader; // (super) event header
    Int_t            fSubEventIndex; // index of current sub event
    IlcRawVEvent*    fSubEvent;     // current sub event
    Int_t            fEquipmentIndex; // index of current equipment
    IlcRawVEquipment*fEquipment;    // current equipment
    IlcRawData*      fRawData;      // current raw data
    UChar_t*         fPosition;     // current position in the raw data
    UChar_t*         fEnd;          // end position of the current subevent
    Long64_t*        fIndex;       // Index of the tree
    static Bool_t    fgUseOrder;       // Flag to use or not sorting in decreased size order

    void SwapData(const void* inbuf, const void* outbuf, UInt_t size);
    void MakeIndex();


    ClassDef(IlcRawReaderRoot, 0) // class for reading raw digits from a root file
};

#endif

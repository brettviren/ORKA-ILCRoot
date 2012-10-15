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

/* $Id: IlcRawReaderDate.cxx 34851 2009-09-17 12:39:10Z cvetan $ */

///////////////////////////////////////////////////////////////////////////////
///
/// This is a class for reading raw data from a date file or event.
///
/// The IlcRawReaderDate is constructed either with a pointer to a
/// date event or with a file name and an event number.
///
///////////////////////////////////////////////////////////////////////////////

#include "IlcRawReaderDate.h"
#ifdef ILC_DATE
#include "event.h"
#endif

ClassImp(IlcRawReaderDate)


IlcRawReaderDate::IlcRawReaderDate(
#ifdef ILC_DATE
				   void* event, Bool_t owner
#else
				   void* /* event */, Bool_t owner
#endif
				   ) :
  fFile(NULL),
  fEvent(NULL),
  fSubEvent(NULL),
  fEquipment(NULL),
  fPosition(NULL),
  fEnd(NULL),
  fOwner(owner)
{
// create an object to read digits from the given date event

#ifdef ILC_DATE
  fEvent = (eventHeaderStruct*) event;
#else
  Fatal("IlcRawReaderDate", "this class was compiled without DATE");
#endif
}

IlcRawReaderDate::IlcRawReaderDate(
#ifdef ILC_DATE
				   const char* fileName, Int_t eventNumber
#else
				   const char* /*fileName*/, 
				   Int_t /*eventNumber*/
#endif
				   ) :
  fFile(NULL),
  fEvent(NULL),
  fSubEvent(NULL),
  fEquipment(NULL),
  fPosition(NULL),
  fEnd(NULL),
  fOwner(kTRUE)
{
// create an object to read digits from the given date event

#ifdef ILC_DATE
  fFile = fopen(fileName, "rb");
  if (!fFile) {
    Error("IlcRawReaderDate", "could not open file %s", fileName);
    fIsValid = kFALSE;
    return;
  }
  if (eventNumber < 0) return;

  eventHeaderStruct header;
  UInt_t headerSize = sizeof(eventHeaderStruct);
  while (fread(&header, 1, headerSize, fFile) == headerSize) {
    if (eventNumber == 0) {
      UChar_t* buffer = new UChar_t[header.eventSize];
      fseek(fFile, -(long)headerSize, SEEK_CUR);
      if (fread(buffer, 1, header.eventSize, fFile) != header.eventSize) break;
      fEvent = (eventHeaderStruct*) buffer;
      break;
    }
    fseek(fFile, header.eventSize-headerSize, SEEK_CUR);
    eventNumber--;
  }

#else
  Fatal("IlcRawReaderDate", "this class was compiled without DATE");
#endif
}

IlcRawReaderDate::IlcRawReaderDate(const IlcRawReaderDate& rawReader) :
  IlcRawReader(rawReader),
  fFile(rawReader.fFile),
  fEvent(rawReader.fEvent),
  fSubEvent(rawReader.fSubEvent),
  fEquipment(rawReader.fEquipment),
  fPosition(rawReader.fPosition),
  fEnd(rawReader.fEnd),
  fOwner(rawReader.fOwner)

{
// copy constructor

  Fatal("IlcRawReaderDate", "copy constructor not implemented");
}

IlcRawReaderDate& IlcRawReaderDate::operator = (const IlcRawReaderDate& 
						/*rawReader*/)
{
// assignment operator

  Fatal("operator =", "assignment operator not implemented");
  return *this;
}

IlcRawReaderDate::~IlcRawReaderDate()
{
// destructor

#ifdef ILC_DATE
  if (fEvent && fOwner) delete[] fEvent;
  if (fFile) {
    fclose(fFile);
  }
#endif
}


UInt_t IlcRawReaderDate::GetType() const
{
// get the type from the event header

#ifdef ILC_DATE
  if (!fEvent) return 0;
  return fEvent->eventType;
#else
  return 0;
#endif
}

UInt_t IlcRawReaderDate::GetRunNumber() const
{
// get the run number from the event header

#ifdef ILC_DATE
  if (!fEvent) return 0;
  return fEvent->eventRunNb;
#else
  return 0;
#endif
}

const UInt_t* IlcRawReaderDate::GetEventId() const
{
// get the event id from the event header

#ifdef ILC_DATE
  if (!fEvent) return NULL;
  return fEvent->eventId;
#else
  return NULL;
#endif
}

const UInt_t* IlcRawReaderDate::GetTriggerPattern() const
{
// get the trigger pattern from the event header

#ifdef ILC_DATE
  if (!fEvent) return NULL;
  return fEvent->eventTriggerPattern;
#else
  return NULL;
#endif
}

const UInt_t* IlcRawReaderDate::GetDetectorPattern() const
{
// get the detector pattern from the event header

#ifdef ILC_DATE
  if (!fEvent) return NULL;
  return fEvent->eventDetectorPattern;
#else
  return NULL;
#endif
}

const UInt_t* IlcRawReaderDate::GetAttributes() const
{
// get the type attributes from the event header

#ifdef ILC_DATE
  if (!fEvent) return NULL;
  return fEvent->eventTypeAttribute;
#else
  return NULL;
#endif
}

const UInt_t* IlcRawReaderDate::GetSubEventAttributes() const
{
// get the type attributes from the sub event header

#ifdef ILC_DATE
  if (!fSubEvent) return NULL;
  return fSubEvent->eventTypeAttribute;
#else
  return NULL;
#endif
}

UInt_t IlcRawReaderDate::GetLDCId() const
{
// get the LDC Id from the event header

#ifdef ILC_DATE
  if (!fSubEvent) return 0;
  return fSubEvent->eventLdcId;
#else
  return 0;
#endif
}

UInt_t IlcRawReaderDate::GetGDCId() const
{
// get the GDC Id from the event header

#ifdef ILC_DATE
  if (!fEvent) return 0;
  return fEvent->eventGdcId;
#else
  return 0;
#endif
}

UInt_t IlcRawReaderDate::GetTimestamp() const
{
// get the timestamp from the event header

#ifdef ILC_DATE
  if (!fEvent) return 0;
  return fEvent->eventTimestamp;
#else
  return 0;
#endif
}

Int_t IlcRawReaderDate::GetEquipmentSize() const
{
// get the size of the equipment (including the header)

#ifdef ILC_DATE
  if (!fEquipment) return 0;
  if (fSubEvent->eventVersion <= 0x00030001) {
    return fEquipment->equipmentSize + sizeof(equipmentHeaderStruct);
  } else {
    return fEquipment->equipmentSize;
  }
#else
  return 0;
#endif
}

Int_t IlcRawReaderDate::GetEquipmentType() const
{
// get the type from the equipment header

#ifdef ILC_DATE
  if (!fEquipment) return -1;
  return fEquipment->equipmentType;
#else
  return 0;
#endif
}

Int_t IlcRawReaderDate::GetEquipmentId() const
{
// get the ID from the equipment header

#ifdef ILC_DATE
  if (!fEquipment) return -1;
  return fEquipment->equipmentId;
#else
  return 0;
#endif
}

const UInt_t* IlcRawReaderDate::GetEquipmentAttributes() const
{
// get the attributes from the equipment header

#ifdef ILC_DATE
  if (!fEquipment) return NULL;
  return fEquipment->equipmentTypeAttribute;
#else
  return 0;
#endif
}

Int_t IlcRawReaderDate::GetEquipmentElementSize() const
{
// get the basic element size from the equipment header

#ifdef ILC_DATE
  if (!fEquipment) return 0;
  return fEquipment->equipmentBasicElementSize;
#else
  return 0;
#endif
}

Int_t IlcRawReaderDate::GetEquipmentHeaderSize() const
{
  // Get the equipment header size
  // 28 bytes by default
#ifdef ILC_DATE
  return sizeof(equipmentHeaderStruct);
#else
  return 0;
#endif
}

Bool_t IlcRawReaderDate::ReadHeader()
{
// read a data header at the current position
// returns kFALSE if the data header could not be read

  fErrorCode = 0;

#ifdef ILC_DATE
  fHeader = NULL;
  if (!fEvent) return kFALSE;
  // check whether there are sub events
  if (fEvent->eventSize <= fEvent->eventHeadSize) return kFALSE;

  do {
    // skip payload (if event was not selected)
    if (fCount > 0) fPosition += fCount;

    // get the first or the next equipment if at the end of an equipment
    if (!fEquipment || (fPosition >= fEnd)) {
      fEquipment = NULL;

      // get the first or the next sub event if at the end of a sub event
      if (!fSubEvent || 
	  (fPosition >= ((UChar_t*)fSubEvent) + fSubEvent->eventSize)) {

	// check for end of event data
	if (fPosition >= ((UChar_t*)fEvent)+fEvent->eventSize) return kFALSE;
        if (!TEST_SYSTEM_ATTRIBUTE(fEvent->eventTypeAttribute, 
                                   ATTR_SUPER_EVENT)) {
	  fSubEvent = fEvent;   // no super event
	} else if (fSubEvent) {
	  fSubEvent = (eventHeaderStruct*) (((UChar_t*)fSubEvent) + 
					    fSubEvent->eventSize);
	} else {
	  fSubEvent = (eventHeaderStruct*) (((UChar_t*)fEvent) + 
					    fEvent->eventHeadSize);
	}

	// check the magic word of the sub event
	if (fSubEvent->eventMagic != EVENT_MAGIC_NUMBER) {
	  Error("ReadHeader", "wrong magic number in sub event!\n"
		" run: %d  event: %d %d  LDC: %d  GDC: %d\n", 
		fSubEvent->eventRunNb, 
		fSubEvent->eventId[0], fSubEvent->eventId[1],
		fSubEvent->eventLdcId, fSubEvent->eventGdcId);
	  fErrorCode = kErrMagic;
	  return kFALSE;
	}

	// continue if no data in the subevent
	if (fSubEvent->eventSize == fSubEvent->eventHeadSize) {
	  fPosition = fEnd = ((UChar_t*)fSubEvent) + fSubEvent->eventSize;
	  fCount = 0;
	  continue;
	}

	fEquipment = (equipmentHeaderStruct*)
	  (((UChar_t*)fSubEvent) + fSubEvent->eventHeadSize);

      } else {
	fEquipment = (equipmentHeaderStruct*) fEnd;
      }

      fCount = 0;
      fPosition = ((UChar_t*)fEquipment) + sizeof(equipmentHeaderStruct);
      if (fSubEvent->eventVersion <= 0x00030001) {
        fEnd = fPosition + fEquipment->equipmentSize;
      } else {
        fEnd = ((UChar_t*)fEquipment) + fEquipment->equipmentSize;
      }
    }

    // continue with the next sub event if no data left in the payload
    if (fPosition >= fEnd) continue;

    if (fRequireHeader) {
      // check that there are enough bytes left for the data header
      if (fPosition + sizeof(IlcRawDataHeader) > fEnd) {
	Error("ReadHeader", "could not read data header data!");
	Warning("ReadHeader", "skipping %d bytes\n"
		" run: %d  event: %d %d  LDC: %d  GDC: %d\n", 
		fEnd - fPosition, fSubEvent->eventRunNb, 
		fSubEvent->eventId[0], fSubEvent->eventId[1],
		fSubEvent->eventLdcId, fSubEvent->eventGdcId);
	fCount = 0;
	fPosition = fEnd;
	fErrorCode = kErrNoDataHeader;
	continue;
      }

      // "read" the data header
      fHeader = (IlcRawDataHeader*) fPosition;
      if ((fPosition + fHeader->fSize) != fEnd) {
	if ((fHeader->fSize != 0xFFFFFFFF) &&
	    (fEquipment->equipmentId != 4352))
	  Warning("ReadHeader",
		  "raw data size found in the header is wrong (%d != %d)! Using the equipment size instead !",
		  fHeader->fSize, fEnd - fPosition);
	fHeader->fSize = fEnd - fPosition;
      }
      fPosition += sizeof(IlcRawDataHeader);
    }

    if (fHeader && (fHeader->fSize != 0xFFFFFFFF)) {
      fCount = fHeader->fSize - sizeof(IlcRawDataHeader);

      // check consistency of data size in the header and in the sub event
      if (fPosition + fCount > fEnd) {
	Error("ReadHeader", "size in data header exceeds event size!");
	Warning("ReadHeader", "skipping %d bytes\n"
		" run: %d  event: %d %d  LDC: %d  GDC: %d\n", 
		fEnd - fPosition, fSubEvent->eventRunNb, 
		fSubEvent->eventId[0], fSubEvent->eventId[1],
		fSubEvent->eventLdcId, fSubEvent->eventGdcId);
	fCount = 0;
	fPosition = fEnd;
	fErrorCode = kErrSize;
	continue;
      }

    } else {
      fCount = fEnd - fPosition;
    }

  } while (!fEquipment || !IsSelected());

  return kTRUE;
#else
  return kFALSE;
#endif
}

Bool_t IlcRawReaderDate::ReadNextData(UChar_t*& data)
{
// reads the next payload at the current position
// returns kFALSE if the data could not be read

  fErrorCode = 0;
  while (fCount == 0) {
    if (!ReadHeader()) return kFALSE;
  }
  data = fPosition;
  fPosition += fCount;  
  fCount = 0;
  return kTRUE;
}

Bool_t IlcRawReaderDate::ReadNext(UChar_t* data, Int_t size)
{
// reads the next block of data at the current position
// returns kFALSE if the data could not be read

  fErrorCode = 0;
  if (fPosition + size > fEnd) {
    Error("ReadNext", "could not read data!");
    fErrorCode = kErrOutOfBounds;
    return kFALSE;
  }
  memcpy(data, fPosition, size);
  fPosition += size;
  fCount -= size;
  return kTRUE;
}


Bool_t IlcRawReaderDate::Reset()
{
// reset the current position to the beginning of the event

#ifdef ILC_DATE
  fSubEvent = NULL;
  fEquipment = NULL;
#endif
  fCount = 0;
  fPosition = fEnd = NULL;
  return kTRUE;
}


Bool_t IlcRawReaderDate::NextEvent()
{
// go to the next event in the date file

#ifdef ILC_DATE
  if (!fFile) {
    if (fEventNumber < 0 && fEvent) {
      fEventNumber++;
      return kTRUE;
    }
    else
      return kFALSE;
  }

  Reset();
  eventHeaderStruct header;
  UInt_t headerSize = sizeof(eventHeaderStruct);
  if (fEvent) delete[] fEvent;
  fEvent = &header;

  while (fread(&header, 1, headerSize, fFile) == headerSize) {
    if (!IsEventSelected()) {
      fseek(fFile, header.eventSize-headerSize, SEEK_CUR);
      continue;
    }
    UChar_t* buffer = new UChar_t[header.eventSize];
    fseek(fFile, -(long)headerSize, SEEK_CUR);
    if (fread(buffer, 1, header.eventSize, fFile) != header.eventSize) {
      Error("NextEvent", "could not read event from file");
      delete[] buffer;
      break;
    }
    fEvent = (eventHeaderStruct*) buffer;
    fEventNumber++;
    return kTRUE;
  };

  fEvent = NULL;
#endif

  return kFALSE;
}

Bool_t IlcRawReaderDate::RewindEvents()
{
// go back to the beginning of the date file

  if (fFile)
    fseek(fFile, 0, SEEK_SET);

  fEventNumber = -1;
  return Reset();
}


Int_t IlcRawReaderDate::CheckData() const
{
// check the consistency of the data

#ifdef ILC_DATE
  if (!fEvent) return 0;
  // check whether there are sub events
  if (fEvent->eventSize <= fEvent->eventHeadSize) return 0;

  eventHeaderStruct* subEvent = NULL;
  UChar_t* position = 0;
  UChar_t* end = 0;
  Int_t result = 0;

  while (kTRUE) {
    // get the first or the next sub event if at the end of a sub event
    if (!subEvent || (position >= end)) {

      // check for end of event data
      if (position >= ((UChar_t*)fEvent)+fEvent->eventSize) return result;
      if (!TEST_SYSTEM_ATTRIBUTE(fEvent->eventTypeAttribute, 
                                 ATTR_SUPER_EVENT)) {
        subEvent = fEvent;   // no super event
      } else if (subEvent) {
	subEvent = (eventHeaderStruct*) (((UChar_t*)subEvent) + 
					 subEvent->eventSize);
      } else {
	subEvent = (eventHeaderStruct*) (((UChar_t*)fEvent) + 
					 fEvent->eventHeadSize);
      }

      // check the magic word of the sub event
      if (subEvent->eventMagic != EVENT_MAGIC_NUMBER) {
	result |= kErrMagic;
	return result;
      }

      position = ((UChar_t*)subEvent) + subEvent->eventHeadSize + 
	sizeof(equipmentHeaderStruct);
      end = ((UChar_t*)subEvent) + subEvent->eventSize;
    }

    // continue with the next sub event if no data left in the payload
    if (position >= end) continue;

    if (fRequireHeader) {
    // check that there are enough bytes left for the data header
      if (position + sizeof(IlcRawDataHeader) > end) {
	result |= kErrNoDataHeader;
	position = end;
	continue;
      }

      // check consistency of data size in the data header and in the sub event
      IlcRawDataHeader* header = (IlcRawDataHeader*) position;
      if ((position + header->fSize) != end) {
	if (header->fSize != 0xFFFFFFFF)
	  Warning("ReadHeader",
		  "raw data size found in the header is wrong (%d != %d)! Using the equipment size instead !",
		  header->fSize, end - position);
	header->fSize = end - position;
	result |= kErrSize;
      }
    }
    position = end;
  };

#endif
  return 0;
}

IlcRawReader* IlcRawReaderDate::CloneSingleEvent() const
{
  // Clones the current event and
  // creates raw-reader for the cloned event
  // Can be used in order to make asynchronious
  // access to the current raw data within
  // several threads (online event display/reco)

#ifdef ILC_DATE
  if (fEvent) {
    UInt_t evSize = fEvent->eventSize;
    if (evSize) {
      UChar_t *newEvent = new UChar_t[evSize];
      memcpy(newEvent,fEvent,evSize);
      return new IlcRawReaderDate((void *)newEvent,kTRUE);
    }
  }
#else
  Fatal("IlcRawReaderDate", "this class was compiled without DATE");
#endif
  return NULL;
}

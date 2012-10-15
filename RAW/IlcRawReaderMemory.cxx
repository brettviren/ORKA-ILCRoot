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

/* $Id: IlcRawReaderMemory.cxx 53319 2011-12-01 11:04:04Z cvetan $ */

///////////////////////////////////////////////////////////////////////////////
/// 
/// This is a class for reading raw data memory buffers.
///
///////////////////////////////////////////////////////////////////////////////

#include "IlcRawReaderMemory.h"
#include <TSystem.h>


ClassImp(IlcRawReaderMemory)


IlcRawReaderMemory::IlcRawReaderMemory() :
  fPosition(0),
  fBuffers(),
  fCurrent(0)
{
// create an object to read digits from
// the given memory location
}

IlcRawReaderMemory::IlcRawReaderMemory(UChar_t* memory, UInt_t size) :
  fPosition(0),
  fBuffers(),
  fCurrent(0)
{
// create an object to read digits from the given memory
  fBuffers.push_back(IlcRRMBuffer(memory, size, -1));
}

IlcRawReaderMemory::~IlcRawReaderMemory()
{
// close the input memory
}

void IlcRawReaderMemory::RequireHeader(Bool_t required)
{
  // Reading of raw data in case of missing
  // raw data header is not implemented for
  // this class
  if (!required)
    Fatal("IlcRawReaderMemory","Reading of raw data without raw data header is not implemented !");

  IlcRawReader::RequireHeader(required);
}

Bool_t IlcRawReaderMemory::ReadHeader()
{
// read a data header at the current buffer position
// returns kFALSE if the mini header could not be read

  Bool_t result=kFALSE;
  if (fCurrent>=fBuffers.size()) return kFALSE;

  do {
  result=kFALSE;
  do {
    if (fBuffers[fCurrent].GetEquipmentId() == -1)
      {
	Warning("ReadHeader", "The equipment ID is not set for the DDL memory buffer.");
      }
    if (!fBuffers[fCurrent].GetBuffer()) break;

    // Check if we would not read past the end of the buffer.
    if ( fPosition+fCount >= fBuffers[fCurrent].GetBufferSize() ) break;
    
    fHeader = reinterpret_cast<IlcRawDataHeader*>(fBuffers[fCurrent].GetBuffer()+fPosition+fCount);
    
    // Check that the header is sane, that is the size does not go past the buffer.
    // Otherwise try again at the next word location.
    while (1) {
    if ( ( (fHeader->fSize == 0) || 
	   ((Int_t)fPosition + fCount + (Int_t)fHeader->fSize > (Int_t)fBuffers[fCurrent].GetBufferSize() ) ) 
	 && fHeader->fSize != 0xFFFFFFFF) {

      if (fPosition + sizeof(UInt_t) <= fBuffers[fCurrent].GetBufferSize()) {
        fPosition += sizeof(UInt_t);
        continue;
      } else {
        Error("ReadHeader", "Could not find a valid DDL header!");
        return kFALSE;
      }
    } else {
      fPosition += fCount + sizeof(IlcRawDataHeader);
    }
    break;
    }

    if (fHeader->fSize != 0xFFFFFFFF) {
      fCount = fHeader->fSize - sizeof(IlcRawDataHeader);
    } else {
      fCount = fBuffers[fCurrent].GetBufferSize() - sizeof(IlcRawDataHeader);
    }
  } while (!(result=IsSelected()) && OpenNextBuffer());
  } while (!result && OpenNextBuffer());

  return result;
}

Bool_t IlcRawReaderMemory::OpenNextBuffer()
{
  // increment to next buffer
  fPosition=0;
  fCount=0;
  if (fCurrent>=fBuffers.size()) return kFALSE;
  if (++fCurrent>=fBuffers.size()) return kFALSE;
  return kTRUE;
}

Bool_t IlcRawReaderMemory::ReadNextData(UChar_t*& data)
{
// reads the next payload at the current buffer position
// returns kFALSE if the data could not be read

  while (fCount == 0) {
    if (!ReadHeader()) return kFALSE;
  }

  if(fCount < 0){
    Error("ReadNextData","Cannot read data, payload is negative");
    return kFALSE;
  }

  UInt_t currentPosition = fPosition;
  fPosition += fCount;
  fCount = 0;

  if(fBuffers[fCurrent].GetBufferSize()<currentPosition){
    Error("ReadNextData","Current position exceeds buffersize.");
    return kFALSE;
  }
  data = fBuffers[fCurrent].GetBuffer()+currentPosition;
  return kTRUE;
}

Bool_t IlcRawReaderMemory::ReadNext(UChar_t* data, Int_t size)
{
// reads the next block of data at the current buffer position
// but does not shift to the next equipment. The next equipment
// must be activated by calling ReadHeader
// returns kFALSE if the data could not be read

  
  if (fCurrent>=fBuffers.size()) return kFALSE;
  if ( fBuffers[fCurrent].GetBufferSize()-fPosition < (UInt_t)size ) return kFALSE;

  memcpy( data, fBuffers[fCurrent].GetBuffer()+fPosition, size );
  fCount -= size;
  fPosition += size;
  return kTRUE;
}


Bool_t IlcRawReaderMemory::Reset()
{
// reset the current position in the buffer to the beginning of the curevent

  fHeader = NULL;
  fCount = 0;
  fPosition = 0;
  fCurrent=0;
  return kTRUE;
}

Bool_t IlcRawReaderMemory::NextEvent()
{
// each memory buffer always contains only one event
  if (fEventNumber < 0) {
    fEventNumber++;
    return kTRUE;
  }
  else
    return kFALSE; 
}

Bool_t IlcRawReaderMemory::RewindEvents()
{
// reset the event counter
  fEventNumber = -1;

  return Reset();
}

Bool_t IlcRawReaderMemory::SetMemory( UChar_t* memory, ULong_t size )
{
  // SetMemory function kept for backward compatibility, only allowed
  // if no blocks have been added so far 
  if (!memory || size<=0) return kFALSE;
  if (fBuffers.size()>1 || (fBuffers.size()==1 && fPosition==0 && fCurrent==0)) {
    Error("SetMemory","can not SetMemory for multiple buffers, use AddBuffer(...)");
    return kFALSE;
  }
  if (fBuffers.size()==1) fBuffers.pop_back();
  fBuffers.push_back(IlcRRMBuffer(memory, size, -1));
  fCurrent=0;
  fHeader = NULL;
  fCount = 0;
  fPosition = 0;
  return kTRUE;
}

void  IlcRawReaderMemory::SetEquipmentID(Int_t id)
{
  // SetMemory function kept for backward compatibility, only allowed
  // if no blocks have been added so far, set equipment id of the first
  // buffer
  if (fBuffers.size()>1) {
    Error("SetEquipmentID", "can not SetEquipmentID for multiple buffers, use AddBuffer(...)");
    return;
  }
  if (fBuffers.size()==0 || fCurrent>=fBuffers.size()) {
    Error("SetEquipmentID", "no block available to set equipment id");
    return;    
  }
  fBuffers[fCurrent].SetEquipmentId(id);
}

Int_t IlcRawReaderMemory::GetEquipmentSize() const
{
  // get the size of the equipment, that is payload + CDH
  if (fCurrent>=fBuffers.size()) return 0;
  return fBuffers[fCurrent].GetBufferSize();
}

Int_t IlcRawReaderMemory::GetEquipmentId() const
{
  // get the current equipment id
  if (fCurrent>=fBuffers.size()) return -1;
  return fBuffers[fCurrent].GetEquipmentId();
}

Bool_t IlcRawReaderMemory::AddBuffer(UChar_t* memory, ULong_t size, Int_t equipmentId )
{
  // Add a buffer to the list
  if (!memory || size<=0 || equipmentId<0 ) return kFALSE;
  fBuffers.push_back(IlcRRMBuffer(memory, size, equipmentId));
  return kTRUE;
}

void IlcRawReaderMemory::ClearBuffers()
{
  // Clear the buffer list
  fBuffers.clear();
  Reset();
}

IlcRawReaderMemory::IlcRRMBuffer::IlcRRMBuffer()
  :
  fBuffer(NULL),
  fBufferSize(0),
  fEquipmentId(-1)
{
  // ctor
}

IlcRawReaderMemory::IlcRRMBuffer::IlcRRMBuffer(UChar_t* pBuffer, UInt_t bufferSize, Int_t equipmentId)
  :
  fBuffer(pBuffer),
  fBufferSize(bufferSize),
  fEquipmentId(equipmentId)
{
  // ctor
}

IlcRawReaderMemory::IlcRRMBuffer::~IlcRRMBuffer()
{
  // dtor
}

IlcRawReaderMemory::IlcRRMBuffer::IlcRRMBuffer(const IlcRRMBuffer& src)
  :
  fBuffer(src.fBuffer),
  fBufferSize(src.fBufferSize),
  fEquipmentId(src.fEquipmentId)
{
  // copy ctor, there are no buffers allocated internally, pointers
  // are just copied
}

IlcRawReaderMemory::IlcRRMBuffer& IlcRawReaderMemory::IlcRRMBuffer::operator=(const IlcRRMBuffer& src)
{
  // assignment op
  if(&src == this) return *this;
  fBuffer=src.fBuffer;
  fBufferSize=src.fBufferSize;
  fEquipmentId=src.fEquipmentId;
  return *this;
}

/**************************************************************************
 * Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 *                                                                        *
// Author: The ILC Off-line Project. 
 // Part of the code has been developed by Alice Off-line Project. 
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

/* $Id: IlcDCHRawStream.cxx,v 1.1.1.1 2008/03/11 14:53:01 vitomeg Exp $ */

///////////////////////////////////////////////////////////////////////////////
///
/// This class provides access to DCH digits in raw data.
///
/// It loops over all DCH digits in the raw data given by the IlcRawReader.
/// The Next method goes to the next digit. If there are no digits left
/// it returns kFALSE.
/// Several getters provide information about the current digit.
///
///////////////////////////////////////////////////////////////////////////////

#include "IlcDCHRawStream.h"
#include "IlcRawReader.h"
#include "IlcDCHcalibDB.h"

ClassImp(IlcDCHRawStream)


IlcDCHRawStream::IlcDCHRawStream(IlcRawReader* rawReader) :
  fRawReader(rawReader),
  fCount(0),
  fDetector(-1),
  fPrevDetector(-1),
  fNPads(-1),
  fRow(-1),
  fPrevRow(-1),
  fColumn(-1),
  fPrevColumn(-1),
  fTime(-1),
  fSignal(-1)
{
// create an object to read DCH raw digits

  fRawReader->Select(4);
}

IlcDCHRawStream::IlcDCHRawStream(const IlcDCHRawStream& stream) :
  TObject(stream),
  fRawReader(NULL),
  fCount(0),
  fDetector(-1),
  fPrevDetector(-1),
  fNPads(-1),
  fRow(-1),
  fPrevRow(-1),
  fColumn(-1),
  fPrevColumn(-1),
  fTime(-1),
  fSignal(-1)
{
  Fatal("IlcDCHRawStream", "copy constructor not implemented");
}

IlcDCHRawStream& IlcDCHRawStream::operator = (const IlcDCHRawStream& 
					      /* stream */)
{
  Fatal("operator =", "assignment operator not implemented");
  return *this;
}

IlcDCHRawStream::~IlcDCHRawStream()
{
// clean up

}


Bool_t IlcDCHRawStream::Next()
{
// read the next raw digit
// returns kFALSE if there is no digit left

  fPrevDetector = fDetector;
  fPrevRow = fRow;
  fPrevColumn = fColumn;
  UChar_t data;

  IlcDCHcalibDB* calibration = IlcDCHcalibDB::Instance();
  if (!calibration)
    return kFALSE;
  
  Int_t timeBins = calibration->GetNumberOfTimeBins();
  
  while (fCount >= 0) {

    while (fCount == 0) {  // next detector
      // read the flag
      if (!fRawReader->ReadNextChar(data)) return kFALSE;
      if (data != 0xBB) {
	Error("Next", "wrong flag: %x", data);
	fCount = -1;
	return kFALSE;
      }

      // read the detector number
      if (!fRawReader->ReadNextChar(data)) {
	Error("Next", "could not read detector number");
	fCount = -1;
	return kFALSE;
      }
      fDetector = data;
      if (!fRawReader->ReadNextChar(data)) {
	Error("Next", "could not read detector number");
	fCount = -1;
	return kFALSE;
      }
      fDetector += (UInt_t(data) << 8);

      // read the number of byts
      if (!fRawReader->ReadNextChar(data)) {
	Error("Next", "could not read number of bytes");
	fCount = -1;
	return kFALSE;
      }
      fCount = data;
      if (!fRawReader->ReadNextChar(data)) {
	Error("Next", "could not read number of bytes");
	fCount = -1;
	return kFALSE;
      }
      fCount += (UInt_t(data) << 8);
      if (!fRawReader->ReadNextChar(data)) {
        Error("Next", "could not read number of bytes");
        fCount = -1;
        return kFALSE;
      }
      fCount += (UInt_t(data) << 16);

      // read the number of active pads
      if (!fRawReader->ReadNextChar(data)) {
	Error("Next", "could not read number of active pads");
	fCount = -1;
	return kFALSE;
      }
      fNPads = data;
      if (!fRawReader->ReadNextChar(data)) {
	Error("Next", "could not read number of active pads");
	fCount = -1;
	return kFALSE;
      }
      fNPads += (UInt_t(data) << 8);

      fTime = timeBins;

    }

    // read the pad row and column number
    if ((fTime >= timeBins) && (fCount > 2)) {
      if (!fRawReader->ReadNextChar(data)) {
	Error("Next", "could not read row number");
	fCount = -1;
	return kFALSE;
      }
      fCount--;
      fRow = data - 1;
      if (!fRawReader->ReadNextChar(data)) {
	Error("Next", "could not read column number");
	fCount = -1;
	return kFALSE;
      }
      fCount--;
      fColumn = data - 1;
      fTime = 0;
    }

    // read the next data byte
    if (!fRawReader->ReadNextChar(data)) {
      Error("Next", "could not read data");
      fCount = -1;
      return kFALSE;
    }
    fCount--;

    if (data == 0) {  // zeros
      if (!fRawReader->ReadNextChar(data)) {
	Error("Next", "could not read time value");
	fCount = -1;
	return kFALSE;
      }
      fCount--;
      fTime += data + 1;

    } else {          // signal
      fSignal = (UInt_t(data & 0x7F) << 8);
      if (!fRawReader->ReadNextChar(data)) {
	Error("Next", "could not read ADC value");
	fCount = -1;
	return kFALSE;
      }
      fCount--;
      fSignal += data;
      fTime++;
      return kTRUE;
    }
  }

  return kFALSE;
}

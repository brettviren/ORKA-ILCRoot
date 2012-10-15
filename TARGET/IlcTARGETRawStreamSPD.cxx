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

/* $Id: IlcTARGETRawStreamSPD.cxx,v 1.1.1.1 2008/03/11 14:52:51 vitomeg Exp $ */

///////////////////////////////////////////////////////////////////////////////
///
/// This class provides access to TARGET SPD digits in raw data.
///
///////////////////////////////////////////////////////////////////////////////

#include "IlcTARGETRawStreamSPD.h"
#include "IlcRawReader.h"

ClassImp(IlcTARGETRawStreamSPD)


const Int_t IlcTARGETRawStreamSPD::fgkDDLModuleMap[kDDLsNumber][kModulesPerDDL] = {
  { 0, 1, 4, 5, 80, 81, 84, 85, 88, 89, 92, 93},
  { 2, 3, 6, 7, 82, 83, 86, 87, 90, 91, 94, 95},
  { 8, 9,12,13, 96, 97,100,101,104,105,108,109},
  {10,11,14,15, 98, 99,102,103,106,107,110,111},
  {16,17,20,21,112,113,116,117,120,121,124,125},
  {18,19,22,23,114,115,118,119,122,123,126,127},
  {24,25,28,29,128,129,132,133,136,137,140,141},
  {26,27,30,31,130,131,134,135,138,139,142,143},
  {32,33,36,37,144,145,148,149,152,153,156,157},
  {34,35,38,39,146,147,150,151,154,155,158,159},
  {40,41,44,45,160,161,164,165,168,169,172,173},
  {42,43,46,47,162,163,166,167,170,171,174,175},
  {48,49,52,53,176,177,180,181,184,185,188,189},
  {50,51,54,55,178,179,182,183,186,187,190,191},
  {56,57,60,61,192,193,196,197,200,201,204,205},
  {58,59,62,63,194,195,198,199,202,203,206,207},
  {64,65,68,69,208,209,212,213,216,217,220,221},
  {66,67,70,71,210,211,214,215,218,219,222,223},
  {72,73,76,77,224,225,228,229,232,233,236,237},
  {74,75,78,79,226,227,230,231,234,235,238,239}
};


IlcTARGETRawStreamSPD::IlcTARGETRawStreamSPD(IlcRawReader* rawReader) :
  IlcTARGETRawStream(rawReader),
  fData(0),
  fDDLNumber(-1),
  fEventNumber(-1),
  fModuleNumber(-1),
  fOffset(0),
  fHitCount(0)
{
// create an object to read TARGET SPD raw digits

  fRawReader->Select(1);
}


Bool_t IlcTARGETRawStreamSPD::Next()
{
// read the next raw digit
// returns kFALSE if there is no digit left

  fPrevModuleID = fModuleID;
  while (fRawReader->ReadNextShort(fData)) {
  
    if ((fData & 0xC000) == 0x4000) {           // header
      fHitCount = 0;
      UShort_t eventNumber = (fData >> 4) & 0x007F;
      if (fEventNumber < 0) {
	fEventNumber = eventNumber;
      } else if (eventNumber != fEventNumber) {
	Warning("Next", "mismatching event numbers: %d != %d", 
		eventNumber, fEventNumber);
      }
      UShort_t chipAddr = fData & 0x000F;
      if (fRawReader->GetDDLID() != fDDLNumber) {
	fModuleNumber = -1;
	fDDLNumber = fRawReader->GetDDLID();
      }
      if ((chipAddr == 5) || (chipAddr == 0)) {
	fModuleNumber++;
	while (fRawReader->TestBlockAttribute(fModuleNumber/2)) fModuleNumber++;
      }
      if (fDDLNumber < kDDLsNumber) {
	fModuleID = fgkDDLModuleMap[fDDLNumber][fModuleNumber];
      } else {
	fModuleID = fModuleNumber;
      }
      fOffset = 32 * (chipAddr % 5);
    } else if ((fData & 0xC000) == 0x0000) {    // trailer
      UShort_t hitCount = fData & 0x1FFF;
      if (hitCount != fHitCount) Error("Next", "wrong number of hits: %d != %d", fHitCount, hitCount);
    } else if ((fData & 0xC000) == 0x8000) {    // pixel hit
      fHitCount++;
      fCoord1 = (fData & 0x001F) + fOffset;
      fCoord2 = (fData >> 5) & 0x00FF;
      return kTRUE;
    } else {                                    // fill word
      if ((fData & 0xC000) != 0xC000) Error("Next", "wrong fill word!");
    }

  }

  return kFALSE;
}

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

/* $Id: IlcBitPacking.cxx 10278 2004-07-01 09:02:49Z tkuhr $ */

///////////////////////////////////////////////////////////////////////////////
///
/// This is a service class for packing and unpacking bits in a 32 bit word.
///
///////////////////////////////////////////////////////////////////////////////

#include <TError.h>

#include "IlcBitPacking.h"


ClassImp(IlcBitPacking)


//_____________________________________________________________________________
Bool_t IlcBitPacking::PackWord(UInt_t data, UInt_t &word, 
			       Int_t startBit, Int_t stopBit)
{
// Packs data into the word buffer from startBit bit up to stopBit bit

  // check that startBit and stopBit are reasonable
  if (startBit > stopBit) {
    ::Error("IlcBitPacking::PackWord", 
	    "startBit is larger than stopBit");
    return kFALSE;
  }
  if (stopBit > 31) {
    ::Error("IlcBitPacking::PackWord", 
	    "stopBit exceeds valid range of 32 bits");
    return kFALSE;
  }

  // create a word with the bits 0 to (stopBit-startBit) set
  UInt_t bits = 0xFFFFFFFF;
  if (stopBit-startBit < 31) bits = (1 << (stopBit-startBit+1)) - 1;

  // check that the data fits into the given bit range
  if (data > bits){
    ::Error("IlcBitPacking::PackWord", 
	    "Word to be filled is not within desired length");
    return kFALSE;
  }

  // clear the bits from startBit to stopBit
  word &= (0xFFFFFFFF ^ (bits << startBit));

  // fill in the data bits
  word |= (data << startBit);

  return kTRUE;
}

//_____________________________________________________________________________
UInt_t IlcBitPacking::UnpackWord(UInt_t word, Int_t startBit, Int_t stopBit)
{
// Unpacks data of stopBit-startBit+1 bits from word buffer starting from 
// the position indicated by startBit

  // check that startBit and stopBit are reasonable
  if (startBit > stopBit) {
    ::Error("IlcBitPacking::UnpackWord", 
	    "startBit is larger than stopBit");
    return 0xFFFFFFFF;
  }
  if (stopBit > 31) {
    ::Error("IlcBitPacking::UnpackWord", 
	    "stopBit exceeds valid range of 32 bits");
    return 0xFFFFFFFF;
  }

  // create a word with the bits 0 to (stopBit-startBit) set
  UInt_t bits = 0xFFFFFFFF;
  if (stopBit-startBit < 31) bits = (1 << (stopBit-startBit+1)) - 1;

  // pick out the requested bits
  return ((word >> startBit) & bits);
}

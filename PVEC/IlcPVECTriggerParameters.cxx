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

/* $Id: IlcPVECTriggerParameters.cxx 52412 2011-10-28 19:54:58Z kharlov $ */

///////////////////////////////////////////////////////////////////////////////
//.                                                                          //
// class for PVEC Trigger Parameters                                         //
//.                                                                          //
///////////////////////////////////////////////////////////////////////////////

#include "IlcPVECTriggerParameters.h"
#include "TString.h"

ClassImp(IlcPVECTriggerParameters)

//________________________________________________________________
IlcPVECTriggerParameters::IlcPVECTriggerParameters()
  : TNamed()
{
  // default constructor. Initialises with Reset().
  Reset();
}

//________________________________________________________________
IlcPVECTriggerParameters::IlcPVECTriggerParameters(const char* name)
  : TNamed(name, name)
{
  // Specific constructor, passes name to TNamed base. Initialises with Reset().

  Reset();
}

//________________________________________________________________
IlcPVECTriggerParameters::IlcPVECTriggerParameters(const IlcPVECTriggerParameters& other)
  : TNamed(other)
{
  // copy constructor
  
  for(Int_t mod = 0; mod < kNMods; ++mod) {
    for(Int_t TRURow = 0; TRURow < kNTRURows; TRURow++) {
      for(Int_t branch = 0; branch < kNBranches; branch++) {
	for(Int_t xIdx = 0; xIdx < kNTRUX; xIdx++) {
	  for(Int_t zIdx = 0; zIdx < kNTRUZ; zIdx++) {
	    fTRUPedestals[mod][TRURow][branch][xIdx][zIdx] = other.fTRUPedestals[mod][TRURow][branch][xIdx][zIdx];
	  } // end zIdx
	} // end xIdx
	fTRUTriggerBitReadoutOn[mod][TRURow][branch] = other.fTRUTriggerBitReadoutOn[mod][TRURow][branch];
	fTRUSignalReadoutOn[mod][TRURow][branch] = other.fTRUSignalReadoutOn[mod][TRURow][branch];
	fTRUSignalTimeBinFrom[mod][TRURow][branch] = other.fTRUSignalTimeBinFrom[mod][TRURow][branch];
	fTRUSignalTimeBinTo[mod][TRURow][branch] = other.fTRUSignalTimeBinTo[mod][TRURow][branch];
	fTRUThreshold[mod][TRURow][branch] = other.fTRUThreshold[mod][TRURow][branch];
	fTRUMaskChannel[mod][TRURow][branch] = other.fTRUMaskChannel[mod][TRURow][branch];
      } // end branch 
    } // end tru
    // for(int tor = 0; tor < kNTORs; tor++) {
    //   fTORMaskArray[mod][tor] = other.fTORMaskArray[mod][tor] ;
    //   fTORReadoutMask[mod][tor] = other.fTORReadoutMask[mod][tor];
    // }
  } // end mod
}


//________________________________________________________________
IlcPVECTriggerParameters& IlcPVECTriggerParameters::operator= (const IlcPVECTriggerParameters& other)
{
  // assignement
  
  SetName(other.GetName());
  SetTitle(other.GetTitle());

  for(Int_t mod = 0; mod < kNMods; ++mod) {
    for(Int_t TRURow = 0; TRURow < kNTRURows; TRURow++) {
      for(Int_t branch = 0; branch < kNBranches; branch++) {
	for(Int_t xIdx = 0; xIdx < kNTRUX; xIdx++) {
	  for(Int_t zIdx = 0; zIdx < kNTRUZ; zIdx++) {
	    fTRUPedestals[mod][TRURow][branch][xIdx][zIdx] = other.fTRUPedestals[mod][TRURow][branch][xIdx][zIdx];
	  }
	}
	fTRUTriggerBitReadoutOn[mod][TRURow][branch] = other.fTRUTriggerBitReadoutOn[mod][TRURow][branch];
	fTRUSignalReadoutOn[mod][TRURow][branch] = other.fTRUSignalReadoutOn[mod][TRURow][branch];
	fTRUSignalTimeBinFrom[mod][TRURow][branch] = other.fTRUSignalTimeBinFrom[mod][TRURow][branch];
	fTRUSignalTimeBinTo[mod][TRURow][branch] = other.fTRUSignalTimeBinTo[mod][TRURow][branch];
	fTRUThreshold[mod][TRURow][branch] = other.fTRUThreshold[mod][TRURow][branch];
	fTRUMaskChannel[mod][TRURow][branch] = other.fTRUMaskChannel[mod][TRURow][branch];
      }
    }
    // for(Int_t tor = 0; tor < kNTORs; tor++) {
    //   fTORMaskArray[mod][tor] = other.fTORMaskArray[mod][tor] ;
    //   fTORReadoutMask[mod][tor] = other.fTORReadoutMask[mod][tor];
    // }
  }
  return *this;
}

//________________________________________________________________
IlcPVECTriggerParameters::~IlcPVECTriggerParameters()
{
  // destructor
}


//________________________________________________________________
UShort_t IlcPVECTriggerParameters::GetTRUPedestal(Int_t mod, Int_t TRURow, Int_t branch, Int_t xIdx, Int_t zIdx) const
{
  // Returns the pedestals of the TRU FakeALTRO Trigger signal, is in the range
  // spanned by 10 bits, i.e. [0,1023]. Ideal value is is given by.
  //
  // mod: [0,4], module index, clockwise, as of 2011 only 2:4 exists
  // TRURow: [0,3], TRU Row, indexed in rising number in PVEC x direction
  // branch: [0,1], branch, indexed in rising number in z direction
  // xIdx: [0,7], TRU 2x2 cell x index, indexed in rising number in PVEC x direction
  // zIdx: [0,13], TRU 2x2 cell z index, indexed in rising number in PVEC z direction
  
  return fTRUPedestals[mod][TRURow][branch][xIdx][zIdx];
}
//________________________________________________________________
Bool_t IlcPVECTriggerParameters::GetTRUReadoutOn(Int_t mod, Int_t TRURow, Int_t branch) const
{
  // Returns the TRU specific flag that specifies for the TRU whether 
  // or not it should be read out.
  //
  // mod: [0,4], module index, clockwise, as of 2011 only 2:4 exists
  // TRURow: [0,3], TRU Row, indexed in rising number in PVEC x direction
  // branch: [0,1], branch, indexed in rising number in z direction

  return fTRUTriggerBitReadoutOn[mod][TRURow][branch];
}
//________________________________________________________________
Bool_t IlcPVECTriggerParameters::GetTRUSignalReadoutOn(Int_t mod, Int_t TRURow, Int_t branch) const
{
  // Returns the TRU specific flag that specifies for the TRU whether 
  // or not it readout should include full  FakeALTRO Trigger Signal.
  //
  // mod: [0,4], module index, clockwise, as of 2011 only 2:4 exists
  // TRURow: [0,3], TRU Row, indexed in rising number in PVEC x direction
  // branch: [0,1], branch, indexed in rising number in z direction

  return fTRUSignalReadoutOn[mod][TRURow][branch];
}
//________________________________________________________________
UShort_t IlcPVECTriggerParameters::GetTRUSignalTimeBinFrom(Int_t mod, Int_t TRURow, Int_t branch) const
{
  // Returns the TRU specific value that specifies from (including), 
  // which timebin TRU should read out.
  //
  // mod: [0,4], module index, clockwise, as of 2011 only 2:4 exists
  // TRURow: [0,3], TRU Row, indexed in rising number in PVEC x direction
  // branch: [0,1], branch, indexed in rising number in z direction

  return fTRUSignalTimeBinFrom[mod][TRURow][branch];
}
//________________________________________________________________
UShort_t IlcPVECTriggerParameters::GetTRUSignalTimeBinTo(Int_t mod, Int_t TRURow, Int_t branch) const
{
  // Returns the TRU specific value that specifies to (including), 
  // which timebin TRU should read out.
  //
  // mod: [0,4], module index, clockwise, as of 2011 only 2:4 exists
  // TRURow: [0,3], TRU Row, indexed in rising number in PVEC x direction
  // branch: [0,1], branch, indexed in rising number in z direction

  return fTRUSignalTimeBinTo[mod][TRURow][branch];
}
//________________________________________________________________
UShort_t IlcPVECTriggerParameters::GetTRUThreshold(Int_t mod, Int_t TRURow, Int_t branch) const
{
  // Returns the TRU specific value that specifies the trigger threshold, 
  // the 4x4 Sliding Window (signal - pedestal) value to which the tru 
  // sends a L0.
  //
  // mod: [0,4], module index, clockwise, as of 2011 only 2:4 exists
  // TRURow: [0,3], TRU Row, indexed in rising number in PVEC x direction
  // branch: [0,1], branch, indexed in rising number in z direction
  
  return fTRUThreshold[mod][TRURow][branch];
}
//________________________________________________________________
UShort_t IlcPVECTriggerParameters::GetTRUMaskChannel(Int_t mod, Int_t TRURow, Int_t branch) const
{
  // TODO: description
  //
  // mod: [0,4], module index, clockwise, as of 2011 only 2:4 exists
  // TRURow: [0,3], TRU Row, indexed in rising number in PVEC x direction
  // branch: [0,1], branch, indexed in rising number in z direction

  return fTRUMaskChannel[mod][TRURow][branch];
}
//________________________________________________________________
const UShort_t* IlcPVECTriggerParameters::GetTORMaskArray(Int_t mod, Int_t tor) const
{
  // TODO: description
  //
  // mod: [0,4], module index, clockwise, as of 2011 only 2:4 exists
  // tor: [0,1], TOR index, rising in PVEC x direction
  //
  // returns a array of UShort_t of size 3.

  return fTORMaskArray[mod][tor];
}
//________________________________________________________________
const UShort_t* IlcPVECTriggerParameters::GetTORReadoutMask(Int_t mod, Int_t tor) const
{
  // TODO: description
  //
  // mod: [0,4], module index, clockwise, as of 2011 only 2:4 exists
  // tor: [0,1], TOR index, rising in PVEC x direction
  //
  // returns a array of UShort_t of size 2.

  return fTORReadoutMask[mod][tor];
}


//________________________________________________________________
void IlcPVECTriggerParameters::SetTRUPedestal(UShort_t pedestal, Int_t mod, Int_t TRURow, Int_t branch, Int_t xIdx, Int_t zIdx)
{
  // Returns the pedestals of the TRU FakeALTRO Trigger signal, 
  // is in the range spanned by 10 bits
  // , i.e. [0,1023]. Ideal value is is given by.
  //
  // mod: [0,4], module index, clockwise, as of 2011 only 2:4 exists
  // TRURow: [0,3], TRU Row, indexed in rising number in PVEC x direction
  // branch: [0,1], branch, indexed in rising number in z direction
  // xIdx: [0,7], TRU 2x2 cell x index, indexed in rising number in PVEC x direction
  // zIdx: [0,7], TRU 2x2 cell z index, indexed in rising number in PVEC z direction
  
  fTRUPedestals[mod][TRURow][branch][xIdx][zIdx] = pedestal;
}
//________________________________________________________________
void IlcPVECTriggerParameters::SetTRUReadoutOn(Bool_t isOn, Int_t mod, Int_t TRURow, Int_t branch)
{
  // Returns the TRU specific flag that specifies for the TRU 
  // whether or not it should be read out.
  //
  // mod: [0,4], module index, clockwise, as of 2011 only 2:4 exists
  // TRURow: [0,3], TRU Row, indexed in rising number in PVEC x direction
  // branch: [0,1], branch, indexed in rising number in z direction

  fTRUTriggerBitReadoutOn[mod][TRURow][branch] = isOn;
}
//________________________________________________________________
void IlcPVECTriggerParameters::SetTRUSignalReadoutOn(Bool_t isOn, Int_t mod, Int_t TRURow, Int_t branch)
{
  // Returns the TRU specific flag that specifies for the TRU whether or not it
  // readout should include full FakeALTRO Trigger Signal.
  //
  // mod: [0,4], module index, clockwise, as of 2011 only 2:4 exists
  // TRURow: [0,3], TRU Row, indexed in rising number in PVEC x direction
  // branch: [0,1], branch, indexed in rising number in z direction

   fTRUSignalReadoutOn[mod][TRURow][branch] = isOn;
}
//________________________________________________________________
void IlcPVECTriggerParameters::SetTRUSignalTimeBinFrom(UShort_t fromBin, Int_t mod, Int_t TRURow, Int_t branch)
{
  // Returns the TRU specific value that specifies from (including), 
  // which timebin TRU should read out.
  //
  // mod: [0,4], module index, clockwise, as of 2011 only 2:4 exists
  // TRURow: [0,3], TRU Row, indexed in rising number in PVEC x direction
  // branch: [0,1], branch, indexed in rising number in z direction

  fTRUSignalTimeBinFrom[mod][TRURow][branch] = fromBin;
}
//________________________________________________________________
void IlcPVECTriggerParameters::SetTRUSignalTimeBinTo(UShort_t toBin, Int_t mod, Int_t TRURow, Int_t branch)
{
  // Returns the TRU specific value that specifies to (including), 
  // which timebin TRU should read out.
  //
  // mod: [0,4], module index, clockwise, as of 2011 only 2:4 exists
  // TRURow: [0,3], TRU Row, indexed in rising number in PVEC x direction
  // branch: [0,1], branch, indexed in rising number in z direction

  fTRUSignalTimeBinTo[mod][TRURow][branch] = toBin;
}
//________________________________________________________________
void IlcPVECTriggerParameters::SetTRUThreshold(UShort_t threshold, Int_t mod, Int_t TRURow, Int_t branch)
{
  // Returns the TRU specific value that specifies the trigger threshold, 
  // the (signal - pedestal) value to which the tru sends a L0.
  //
  // mod: [0,4], module index, clockwise, as of 2011 only 2:4 exists
  // TRURow: [0,3], TRU Row, indexed in rising number in PVEC x direction
  // branch: [0,1], branch, indexed in rising number in z direction
  
  fTRUThreshold[mod][TRURow][branch] = threshold;
}
//________________________________________________________________
void IlcPVECTriggerParameters::SetTRUMaskChannel(UShort_t mask, Int_t mod, Int_t TRURow, Int_t branch)
{
  // TODO: description
  //
  // mod: [0,4], module index, clockwise, as of 2011 only 2:4 exists
  // TRURow: [0,3], TRU Row, indexed in rising number in PVEC x direction
  // branch: [0,1], branch, indexed in rising number in z direction

  fTRUMaskChannel[mod][TRURow][branch] = mask;
}
//________________________________________________________________
void IlcPVECTriggerParameters::SetTORMaskArray(const UShort_t ma[3], Int_t mod, Int_t tor)
{
  // TODO: description
  //
  // mod: [0,4], module index, clockwise, as of 2011 only 2:4 exists
  // tor: [0,1], TOR index, rising in PVEC x direction

  fTORMaskArray[mod][tor][0] = ma[0];
  fTORMaskArray[mod][tor][1] = ma[1];
  fTORMaskArray[mod][tor][2] = ma[2];
}
//________________________________________________________________
void IlcPVECTriggerParameters::SetTORReadoutMask(const UShort_t rm[2], Int_t mod, Int_t tor)
{
  // TODO: description
  //
  // mod: [0,4], module index, clockwise, as of 2011 only 2:4 exists
  // tor: [0,1], TOR index, rising in PVEC x direction
  
  fTORReadoutMask[mod][tor][0] = rm[0];
  fTORReadoutMask[mod][tor][1] = rm[1];
}


//________________________________________________________________
void IlcPVECTriggerParameters::Print(Option_t *option) const
{
  // Will Write uppon request (Henrik Qvigstad <henrik.qvigstad@cern.ch>)
  if(option)
    Printf("IlcPVECTriggerParameters::Print is not implemented.");
}

//________________________________________________________________
void IlcPVECTriggerParameters::Reset()
{
  // Resets the values to Null/Ideal

    for(Int_t mod = 0; mod < kNMods; ++mod) {
    for(Int_t TRURow = 0; TRURow < kNTRURows; TRURow++) {
      for(Int_t branch = 0; branch < kNBranches; branch++) {
	fTRUTriggerBitReadoutOn[mod][TRURow][branch] = 0;
	fTRUSignalReadoutOn[mod][TRURow][branch] = 0;
	fTRUSignalTimeBinFrom[mod][TRURow][branch] = 0;
	fTRUSignalTimeBinTo[mod][TRURow][branch] = kDefaultNTRUTimeBins-1;
	fTRUThreshold[mod][TRURow][branch] = 0;
	fTRUMaskChannel[mod][TRURow][branch] = 0;
	for(Int_t xIdx = 0; xIdx < kNTRUX; xIdx++) {
	  for(Int_t zIdx = 0; zIdx < kNTRUZ; zIdx++) {
	    fTRUPedestals[mod][TRURow][branch][xIdx][zIdx] = kIdealTRUPedestal;
	  }
	}
      }
    }
    for(Int_t tor = 0; tor < kNTORs; tor++) {
      fTORMaskArray[mod][tor][0] = 0;
      fTORMaskArray[mod][tor][1] = 0;
      fTORMaskArray[mod][tor][2] = 0;
      fTORReadoutMask[mod][tor][0] = 0;
      fTORReadoutMask[mod][tor][1] = 0;
    }
  }
}


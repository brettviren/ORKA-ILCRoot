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

///////////////////////////////////////////////////////////////////////////////
//
// ESD format for the TRD tracks calculated in the
// Global Tracking Unit, used for the TRD L1 trigger
//
// Author: Jochen Klein <jochen.klein@cern.ch>
//
///////////////////////////////////////////////////////////////////////////////

#include "TMath.h"

#include "IlcESDTrdTrack.h"
#include "IlcESDTrdTracklet.h"

ClassImp(IlcESDTrdTrack)

//_____________________________________________________________________________
IlcESDTrdTrack::IlcESDTrdTrack():
  TObject(),
  fSector(-1),
  fStack(-1),
  fA(0),
  fB(0),
  fC(0),
  fY(0),
  fPID(0),
  fLayerMask(0),
  fFlags(0),
  fReserved(0),
  fLabel(-1)
{
  //
  // Default constructor
  for (int i=fgkNlayers;i--;) fTrackletIndex[i] = 0;
  //

}

IlcESDTrdTrack::IlcESDTrdTrack(const IlcESDTrdTrack& track):
  TObject(track),
  fSector(track.fSector),
  fStack(track.fStack),
  fA(track.fA),
  fB(track.fB),
  fC(track.fC),
  fY(track.fY),
  fPID(track.fPID),
  fLayerMask(track.fLayerMask),
  fFlags(track.fFlags),
  fReserved(track.fReserved),
  fLabel(track.fLabel)
{
  //
  // Copy contructor
  //

  for (Int_t iLayer = 0; iLayer < 6; iLayer++) {
    fTrackletIndex[iLayer] = track.fTrackletIndex[iLayer];

    if (track.fTrackletRefs[iLayer].GetObject())
      fTrackletRefs[iLayer] = track.fTrackletRefs[iLayer];
  }
}

IlcESDTrdTrack& IlcESDTrdTrack::operator=(const IlcESDTrdTrack& track)
{
  //
  // assignment operator
  //

  if (this == &track)
    return *this;
  TObject::operator=(track);
  fLabel        = track.fLabel;
  fSector       = track.fSector;
  for (Int_t iLayer = 0; iLayer < 6; iLayer++) {
    fTrackletIndex[iLayer] = track.fTrackletIndex[iLayer];
    fTrackletRefs[iLayer] = track.fTrackletRefs[iLayer];
  }

  return *this;
}

void IlcESDTrdTrack::Copy(TObject& obj) const
{
  // this overwrites the virtual TOBject::Copy()
  // to allow run time copying without casting
  // in IlcESDEvent

  if(this==&obj)return;
  IlcESDTrdTrack *robj = dynamic_cast<IlcESDTrdTrack*>(&obj);
  if(!robj)return; // not an ilcesdtrdtrack
  *robj = *this;
}

ULong64_t IlcESDTrdTrack::GetTrackWord(Int_t /* rev */) const
{
  // return track word representation as in raw data
  // MSB always 1

  ULong64_t trackWord = 1;
  AppendBits(trackWord,   1, 0);
  AppendBits(trackWord,   6, fLayerMask);
  AppendBits(trackWord,  18, fA);
  AppendBits(trackWord,  18, fB);
  AppendBits(trackWord,  12, fC);
  AppendBits(trackWord,   8, fPID);
  return trackWord;
}

ULong64_t IlcESDTrdTrack::GetExtendedTrackWord(Int_t /* rev */) const
{
  // return extended track word representation as in raw data
  // MSB always 0

  ULong64_t trackWord = 0;
  AppendBits(trackWord,  11, fFlags);
  AppendBits(trackWord,  3, fReserved);
  AppendBits(trackWord, 13, fY);
  AppendBits(trackWord,  6, fTrackletIndex[5]);
  AppendBits(trackWord,  6, fTrackletIndex[4]);
  AppendBits(trackWord,  6, fTrackletIndex[3]);
  AppendBits(trackWord,  6, fTrackletIndex[2]);
  AppendBits(trackWord,  6, fTrackletIndex[1]);
  AppendBits(trackWord,  6, fTrackletIndex[0]);
  return trackWord;
}

Int_t IlcESDTrdTrack::GetPt() const
{
  // calculate pt from a as done in hardware

  const Int_t maskIdLut[64] = {
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  0,
    -1, -1, -1, -1, -1, -1, -1,  1, -1, -1, -1,  2, -1,  3,  4,  5,
    -1, -1, -1, -1, -1, -1, -1,  6, -1, -1, -1,  7, -1,  8,  9, 10,
    -1, -1, -1, 11, -1, 12, 13, 14, -1, 15, 16, 17, 18, 19, 20, 21
  };

  const Int_t c1Lut[32] = {
    -2371, -2474, -2474, -2474, -2563, -2448, -2578, -2578,
    -2578, -2670, -2557, -2578, -2578, -2670, -2557, -2578,
    -2670, -2557, -2763, -2557, -2644, -2523,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1
  };

  if (this->GetA() != 0) {
    Int_t layerMaskId = maskIdLut[this->GetLayerMask()];
    Int_t c1 = c1Lut[layerMaskId];
    Int_t c1Ext = c1 << 8;
    Int_t ptRawStage4 = c1Ext / ((this->GetA() >> 2) != 0 ? (this->GetA() >> 2) : 1 );
    Int_t ptRawComb4 = ptRawStage4;
    Int_t ptExtComb4 = (ptRawComb4 > 0) ? ptRawComb4 + 33 : ptRawComb4 - 30;

    return -ptExtComb4/2;
  }
  else
    return 0;
}

Int_t IlcESDTrdTrack::Compare(const TObject* obj) const
{
  if (this == obj)
    return 0;
  else if (TMath::Abs(Pt()) < TMath::Abs(((IlcESDTrdTrack*)(obj))->Pt()))
    return 1;
  else
    return -1;
}

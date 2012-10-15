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
// ESD format for TRD tracklet from FEE used for triggering
//
// Author: Jochen Klein <jochen.klein@cern.ch>
//
///////////////////////////////////////////////////////////////////////////////

#include "IlcESDTrdTracklet.h"

ClassImp(IlcESDTrdTracklet)

const Float_t IlcESDTrdTracklet::fgkBinWidthY   = 160e-4; // 160 um
const Float_t IlcESDTrdTracklet::fgkBinWidthDy  = 140e-4; // 140 um
const Float_t IlcESDTrdTracklet::fgkX0          = 2.21;
const Float_t IlcESDTrdTracklet::fgkDriftLength = 3.;

IlcESDTrdTracklet::IlcESDTrdTracklet() :
  TObject(),
  fHCId(-1),
  fTrackletWord(0),
  fLabel(-1)
{
  // default ctor

}

IlcESDTrdTracklet::IlcESDTrdTracklet(UInt_t trackletWord, Short_t hcid, Int_t label) :
  TObject(),
  fHCId(hcid),
  fTrackletWord(trackletWord),
  fLabel(label)
{
  // ctor with given tracklet word (and label)
}

IlcESDTrdTracklet::~IlcESDTrdTracklet()
{
  // dtor
}

IlcESDTrdTracklet::IlcESDTrdTracklet(const IlcESDTrdTracklet &trkl) :
  TObject(trkl),
  fHCId(trkl.fHCId),
  fTrackletWord(trkl.fTrackletWord),
  fLabel(trkl.fLabel)
{
  // copy ctor

}

IlcESDTrdTracklet& IlcESDTrdTracklet::operator=(const IlcESDTrdTracklet &trkl)
{
  // assignment operator

  if (this == &trkl)
    return *this;

  TObject::operator=(trkl);
  fHCId = trkl.fHCId;
  fTrackletWord = trkl.fTrackletWord;
  fLabel = trkl.fLabel;

  return *this;
}

Int_t IlcESDTrdTracklet::GetBinY() const
{
  // returns (signed) value of Y

  if (fTrackletWord & 0x1000) {
    return -((~(fTrackletWord-1)) & 0x1fff);
  }
  else {
    return (fTrackletWord & 0x1fff);
  }
}

Int_t IlcESDTrdTracklet::GetBinDy() const
{
  // returns (signed) value of the deflection length

  if (fTrackletWord & (1 << 19)) {
    return -((~((fTrackletWord >> 13) - 1)) & 0x7f);
  }
  else {
    return ((fTrackletWord >> 13) & 0x7f);
  }
}

Float_t IlcESDTrdTracklet::GetDyDx() const
{
  // returns the deflection over 3 cm drift length

  return GetBinDy() * fgkBinWidthDy/fgkDriftLength;
}

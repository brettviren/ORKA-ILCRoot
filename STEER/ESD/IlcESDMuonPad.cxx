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

// $Id: IlcESDMuonPad.cxx 50615 2011-07-16 23:19:19Z hristov $

//-----------------------------------------------------------------------------
/// \class IlcESDMuonPad
///
/// Class to describe the MUON pads in the Event Summary Data
///
/// \author Philippe Pillot, Subatech
//-----------------------------------------------------------------------------

#include "IlcESDMuonPad.h"

#include "IlcLog.h"

#include <Riostream.h>

/// \cond CLASSIMP
ClassImp(IlcESDMuonPad)
/// \endcond

//_____________________________________________________________________________
IlcESDMuonPad::IlcESDMuonPad()
: TObject(),
  fADC(0),
  fCharge(0.)
{
  /// default constructor
}

//_____________________________________________________________________________
IlcESDMuonPad::IlcESDMuonPad (const IlcESDMuonPad& pad)
: TObject(pad),
  fADC(pad.fADC),
  fCharge(pad.fCharge)
{
  /// Copy constructor
}

//_____________________________________________________________________________
IlcESDMuonPad& IlcESDMuonPad::operator=(const IlcESDMuonPad& pad)
{
  /// Equal operator
  if (this == &pad) return *this;
  
  TObject::operator=(pad); // don't forget to invoke the base class' assignment operator
  
  fADC = pad.fADC;
  fCharge = pad.fCharge;
  
  return *this;
}

//_____________________________________________________________________________
void IlcESDMuonPad::Print(Option_t */*option*/) const
{
  /// print cluster content
  UInt_t cId = GetUniqueID();
  
  cout<<Form("padID=%u (det=%d, manu=%d, manuChannel=%d, cathode=%d)",
	     cId, GetDetElemId(), GetManuId(), GetManuChannel(), GetCathode())<<endl;
  
  cout<<Form("    raw charge=%d, calibrated charge=%5.2f", GetADC(), GetCharge())<<endl;
}


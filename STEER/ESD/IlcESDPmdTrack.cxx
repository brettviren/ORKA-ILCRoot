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

/* $Id: IlcESDPmdTrack.cxx 50615 2011-07-16 23:19:19Z hristov $ */

#include "IlcESDPmdTrack.h"

// Event Data Summary Class
// For pmd tracks
// This is part of the reconstructed
// ESD events
// for the PMD detector

ClassImp(IlcESDPmdTrack)

//--------------------------------------------------------------------------//
IlcESDPmdTrack::IlcESDPmdTrack () :
  TObject(),
  fX(0),
  fY(0),
  fZ(0),
  fCluADC(0),
  fCluPID(0),
  fDet(0),
  fNcell(0),
  fSmn(-1),
  fTrackNo(-1),
  fTrackPid(-1),
  fClMatching(0),
  fSigX(9999),
  fSigY(9999)
{
  // Default Constructor
}

//--------------------------------------------------------------------------//
IlcESDPmdTrack::IlcESDPmdTrack (const IlcESDPmdTrack& PMDTrack) : 
  TObject(PMDTrack),
  fX(PMDTrack.fX),
  fY(PMDTrack.fY),
  fZ(PMDTrack.fZ),
  fCluADC(PMDTrack.fCluADC),
  fCluPID(PMDTrack.fCluPID),
  fDet(PMDTrack.fDet),
  fNcell(PMDTrack.fNcell),
  fSmn(PMDTrack.fSmn),
  fTrackNo(PMDTrack.fTrackNo),
  fTrackPid(PMDTrack.fTrackPid),
  fClMatching(PMDTrack.fClMatching),
  fSigX(PMDTrack.fSigX),
  fSigY(PMDTrack.fSigY)
{
  // Copy Constructor
}

//--------------------------------------------------------------------------//
IlcESDPmdTrack &IlcESDPmdTrack::operator=(const IlcESDPmdTrack& PMDTrack)
{
  // Copy constructor
  if(&PMDTrack == this) return *this;
  TObject::operator=(PMDTrack);
  fX      = PMDTrack.fX;
  fY      = PMDTrack.fY;
  fZ      = PMDTrack.fZ;
  fCluADC = PMDTrack.fCluADC;
  fCluPID = PMDTrack.fCluPID;
  fDet    = PMDTrack.fDet;
  fNcell  = PMDTrack.fNcell;
  fSmn    = PMDTrack.fSmn;
  fTrackNo= PMDTrack.fTrackNo;
  fTrackPid = PMDTrack.fTrackPid;
  fClMatching = PMDTrack.fClMatching;
  fSigX = PMDTrack.fSigX;
  fSigY = PMDTrack.fSigY;
  return *this;
}

void IlcESDPmdTrack::Copy(TObject& obj) const {

   // this overwrites the virtual TOBject::Copy()
  // to allow run time copying without casting
  // in IlcESDEvent

  if(this==&obj)return;
  IlcESDPmdTrack *robj = dynamic_cast<IlcESDPmdTrack*>(&obj);
  if(!robj)return; // not an ilcesdpmdtrack
  *robj = *this;
}

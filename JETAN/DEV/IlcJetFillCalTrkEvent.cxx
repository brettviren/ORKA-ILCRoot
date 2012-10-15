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

/* $Id$ */

//--------------------------------------------------
// Filling of CalTrkEvent objects in the reader task
//
// Author: magali.estienne@subatech.in2p3.fr
//         alexandre.shabetai@cern.ch
//-------------------------------------------------

#include "IlcJetFillCalTrkEvent.h"

// --- ROOT system ---
class TSystem;
class TLorentzVector;
class TVector3;
class TGeoManager;

// --- IlcRoot header files ---
class IlcJetFinder;
class IlcJetReader;
class IlcJetReader;
class IlcJetReaderHeader;
class IlcJetCalTrkEvent;

ClassImp(IlcJetFillCalTrkEvent)

/////////////////////////////////////////////////////////////////////

IlcJetFillCalTrkEvent::IlcJetFillCalTrkEvent() :
    fOpt(0),
    fDebug(0),
    fReaderHeader(0x0),
    fCalTrkEvent(0x0),
    fGeom(0x0)
{
  // constructor
}

//-----------------------------------------------------------------------
IlcJetFillCalTrkEvent::IlcJetFillCalTrkEvent(const IlcJetFillCalTrkEvent& cpfrom) :
    fOpt(0),
    fDebug(0),
    fReaderHeader(0x0),
    fCalTrkEvent(0x0),
    fGeom(0x0)
{
    // Copy constructor
    fOpt              = cpfrom.fOpt;
    fDebug            = cpfrom.fDebug;
    fReaderHeader     = cpfrom.fReaderHeader;
    fCalTrkEvent     = cpfrom.fCalTrkEvent;
    fGeom             = cpfrom.fGeom;
}

//-----------------------------------------------------------------------
IlcJetFillCalTrkEvent& IlcJetFillCalTrkEvent::operator=(const IlcJetFillCalTrkEvent& rhs)
{
   // Assignment operator
  if (this != &rhs) {
    fOpt              = rhs.fOpt;
    fDebug            = rhs.fDebug;
    fReaderHeader     = rhs.fReaderHeader;
    fCalTrkEvent     = rhs.fCalTrkEvent;
    fGeom             = rhs.fGeom;
  }
  
  return *this;
    
}

//-----------------------------------------------------------------------
IlcJetFillCalTrkEvent::~IlcJetFillCalTrkEvent()
{
  // destructor
}

//-----------------------------------------------------------------------
Float_t  IlcJetFillCalTrkEvent::EtaToTheta(Float_t arg)
{
  // Transform eta to theta
  return 2.*atan(exp(-arg));
}








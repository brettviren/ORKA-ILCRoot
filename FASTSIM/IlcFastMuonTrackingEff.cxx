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

/* $Id: IlcFastMuonTrackingEff.cxx 16417 2007-01-15 14:50:02Z alibrary $ */
//
// Class for fast simulation of the ILC Muon Spectrometer
// Tracking Efficiency.
// The efficiency depends on trasverse momentum pt, polar angle theta and azimuthal angle phi.
//
// Author: Alessandro de Falco 
// alessandro.de.falco@ca.infn.it
// 

#include <TMath.h>

#include "IlcFastMuonTrackingEff.h"
#include "IlcMUONFastTracking.h"

ClassImp(IlcFastMuonTrackingEff)


IlcFastMuonTrackingEff::IlcFastMuonTrackingEff() :
    IlcFastResponse("Efficiency", "Muon Tracking Efficiency"),
    fBackground(1.),
    fCharge(1.),
    fFastTracking(0)
{
//
// Constructor
}

IlcFastMuonTrackingEff::IlcFastMuonTrackingEff(const IlcFastMuonTrackingEff& eff)
    :IlcFastResponse(eff),
    fBackground(1.),
    fCharge(1.),
    fFastTracking(0)
{
// Copy constructor
    eff.Copy(*this);
}

void IlcFastMuonTrackingEff::Init()
{
//
// Initialization
    fFastTracking = IlcMUONFastTracking::Instance();
    fFastTracking->Init(fBackground);
}



Float_t IlcFastMuonTrackingEff::Evaluate(Float_t /*charge*/, Float_t pt, Float_t theta, Float_t phi)
{
//
// Evaluate the efficience for muon with 3-vector (pt, theta, phi)
    Float_t p = pt / TMath::Sin(theta*TMath::Pi()/180.);
    Float_t eff =  fFastTracking->Efficiency(p, theta, phi, Int_t(fCharge));
    return eff;
}


IlcFastMuonTrackingEff& IlcFastMuonTrackingEff::operator=(const  IlcFastMuonTrackingEff& rhs)
{
// Assignment operator
    rhs.Copy(*this);
    return *this;
}


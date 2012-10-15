#ifndef ILCFASTMUONTRACKINGACC_H
#define ILCFASTMUONTRACKINGACC_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcFastMuonTrackingAcc.h 13628 2006-04-27 10:39:06Z morsch $ */

// Realisation of IlcFastResponse for the
// fast simulation of the muon spectrometer acceptance.
// The acceptance depends on the muon 3-vector which can be passed as (pt, theta, phi), 
// where pt is the transverse momentum, theta the polar angle and phi the azimuthal angle.
// Author: Andreas Morsch
// andreas.morsch@cern.ch 

#include "IlcFastResponse.h"
class IlcMUONFastTracking;

class IlcFastMuonTrackingAcc :  public IlcFastResponse {
 public:
    IlcFastMuonTrackingAcc();
    IlcFastMuonTrackingAcc(const IlcFastMuonTrackingAcc& acc);
    virtual ~IlcFastMuonTrackingAcc(){;}
    void SetBackground(Float_t bg = 1.) {fBackground = bg;}
    void SetCharge(Float_t charge = 1.) {fCharge     = charge;}
    virtual void    Init();
    virtual Float_t Evaluate(Float_t charge, Float_t pt, Float_t theta, Float_t phi);
    virtual void    Evaluate(Float_t charge, Float_t   p,  Float_t  theta , Float_t   phi,
			     Float_t& pS,  Float_t& thetaS, Float_t&  phiS)
	{IlcFastResponse::Evaluate(charge, p, theta, phi, pS, thetaS, phiS);}
    virtual void    Evaluate(Float_t   p,  Float_t  theta , Float_t   phi,
			     Float_t& pS,  Float_t& thetaS, Float_t&  phiS)
	{IlcFastResponse::Evaluate(p, theta, phi, pS, thetaS, phiS);}
    
    // Copy
    IlcFastMuonTrackingAcc& operator=(const IlcFastMuonTrackingAcc& rhs);
 protected:
    Float_t              fBackground;   // Background level
    Float_t              fCharge;       // Current charge
    
    IlcMUONFastTracking* fFastTracking; //!Pointer to Fast Tracking Data Handler
    ClassDef(IlcFastMuonTrackingAcc,1)  // Fast MUON Tracking Acceptance
};

#endif






#ifndef ILCFASTMUONTRACKINGEFF_H
#define ILCFASTMUONTRACKINGEFF_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcFastMuonTrackingEff.h 13628 2006-04-27 10:39:06Z morsch $ */
//
// Class for fast simulation of the ILC Muon Spectrometer
// Tracking Efficiency.
// The efficiency depends on transverse momentum pt, polar angle theta and azimuthal angle phi.
//
// Author: Alessandro de Falco 
// alessandro.de.falco@ca.infn.it

#include "IlcFastResponse.h"
class IlcMUONFastTracking;

class IlcFastMuonTrackingEff :  public IlcFastResponse {
 public:
    IlcFastMuonTrackingEff();
    virtual ~IlcFastMuonTrackingEff(){;}
    IlcFastMuonTrackingEff(const IlcFastMuonTrackingEff& eff);
    void SetBackground(Float_t bg = 1.) {fBackground = bg;}
    void SetCharge(Float_t charge = 1.) {fCharge     = charge;}
    virtual void Init();
    virtual Float_t Evaluate(Float_t charge, Float_t pt, Float_t theta, Float_t phi);
    virtual void    Evaluate(Float_t charge, Float_t   p,  Float_t  theta , Float_t   phi,
			     Float_t& pS,  Float_t& thetaS, Float_t&  phiS)
	{IlcFastResponse::Evaluate(charge, p, theta, phi, pS, thetaS, phiS);}
    virtual void    Evaluate(Float_t   p,  Float_t  theta , Float_t   phi,
			     Float_t& pS,  Float_t& thetaS, Float_t&  phiS)
	{IlcFastResponse::Evaluate(p, theta, phi, pS, thetaS, phiS);}
    
    // Copy
    IlcFastMuonTrackingEff& operator=(const IlcFastMuonTrackingEff& rhs);
 protected:
    Float_t              fBackground;   // Background level
    Float_t              fCharge;       // Current charge
    
    IlcMUONFastTracking* fFastTracking; //!Pointer to Fast Tracking Data Handler
    ClassDef(IlcFastMuonTrackingEff,1)  // Fast MUON Tracking Efficiency
};

#endif






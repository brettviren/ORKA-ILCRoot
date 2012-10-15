#ifndef ILCFASTMUONTRACKINGRES_H
#define ILCFASTMUONTRACKINGRES_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcFastMuonTrackingRes.h 13628 2006-04-27 10:39:06Z morsch $ */

// Implementation of IlcFastResponse for the Muon Spectrometer resolution.
// The response depends on the charge of the muon and
// the background level.
// The class uses the instance of an object of type IlcMUONFastTracking to 
// obtain the smearing parameters.
// Author: andreas.morsch@cern.ch

#include "IlcFastResponse.h"
class IlcMUONFastTracking;

class IlcFastMuonTrackingRes :  public IlcFastResponse {
 public:
    IlcFastMuonTrackingRes();
    IlcFastMuonTrackingRes(const IlcFastMuonTrackingRes& glauber);
    virtual ~IlcFastMuonTrackingRes(){;}
    void SetBackground(Float_t bg = 1.) {fBackground = bg;}
    void SetCharge(Float_t charge = 1.) {fCharge     = charge;}
    virtual void Init();
    void Evaluate(Float_t   p,  Float_t  theta , Float_t   phi,
		  Float_t& pS,  Float_t& thetaS, Float_t&  phiS);

    void Evaluate(Float_t /*charge*/, Float_t   p,  Float_t  theta , Float_t   phi,
		  Float_t& pS,  Float_t& thetaS, Float_t&  phiS)
	{Evaluate(p, theta, phi, pS, thetaS, phiS);}
    virtual Float_t Evaluate(Float_t charge, Float_t pt, Float_t theta, Float_t phi)
	{return IlcFastResponse::Evaluate(charge, pt, theta, phi);}
    
    // Copy
    IlcFastMuonTrackingRes& operator=(const IlcFastMuonTrackingRes & rhs);

 protected:
    Float_t              fBackground;   // Background level
    Float_t              fCharge;       // Current charge
    
    IlcMUONFastTracking* fFastTracking; //!Pointer to Fast Tracking Data Handler
    ClassDef(IlcFastMuonTrackingRes,1)  // Fast MUON Tracking 
};

#endif



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

/* $Id: IlcFastResponse.cxx 13628 2006-04-27 10:39:06Z morsch $ */

// Basic implementation of a fast detector response. 
// The 3-vector of the particle can be passes as
// a TParticle or as
// transverse momentum pt, polar angle theta and azimuthal angle phi
// Author: Andreas Morsch
// andreas.morsch@cern.ch
//
#include "IlcFastResponse.h"
#include "IlcFastParticle.h"

ClassImp(IlcFastResponse)


Float_t IlcFastResponse::Evaluate(Float_t /*charge*/, Float_t /*pt*/, Float_t /*theta*/, Float_t /*phi*/)
{
//
//  Dummy implementation of this method
// 
    return 0.;
}


void IlcFastResponse::Evaluate(Float_t /*charge*/, Float_t   p,  Float_t  theta , Float_t   phi,
			       Float_t& pS,  Float_t& thetaS, Float_t&  phiS)
{
//    
// Basic implementation of this method 
//
    pS     = p;
    thetaS = theta;
    phiS   = phi;
}

void IlcFastResponse::Evaluate(Float_t   p,  Float_t  theta , Float_t   phi,
			       Float_t& pS,  Float_t& thetaS, Float_t&  phiS)
{
//    
// Basic implementation of this method 
//
    pS     = p;
    thetaS = theta;
    phiS   = phi;
}

void IlcFastResponse::Copy(TObject&) const
{
    //
    // Copy 
    //
    Fatal("Copy","Not implemented!\n");
}


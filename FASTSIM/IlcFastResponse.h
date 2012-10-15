#ifndef ILCFASTRESPONSE_H
#define ILCFASTRESPONSE_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcFastResponse.h 13628 2006-04-27 10:39:06Z morsch $ */

// Base class for fast detector response simulation. 
// The 3-vector of the particle can be passes as
// a TParticle or as
// transverse momentum pt, polar angle theta and azimuthal angle phi
// Author: Andreas Morsch
// andreas.morsch@cern.ch

#include <TNamed.h>
class IlcFastParticle;

class IlcFastResponse : public TNamed {
 public:
    IlcFastResponse(){;}
    IlcFastResponse(const char* Name, const char* Title): TNamed(Name, Title){}
    virtual ~IlcFastResponse(){}
    virtual void    Init()                                                  = 0;

    virtual void    Evaluate(Float_t charge, Float_t   p,  Float_t  theta , Float_t   phi,
			     Float_t& pS,  Float_t& thetaS, Float_t&  phiS);
    virtual void    Evaluate(Float_t   p,  Float_t  theta , Float_t   phi,
			     Float_t& pS,  Float_t& thetaS, Float_t&  phiS);
 
    virtual Float_t Evaluate(Float_t charge, Float_t pt, Float_t theta, Float_t phi);
 
    void Copy(TObject&) const;
 protected:
    ClassDef(IlcFastResponse,1)    // Base class for fast response
};

#endif

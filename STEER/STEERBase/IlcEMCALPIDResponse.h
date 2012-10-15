#ifndef IlcEMCALPIDResponse_h
#define IlcEMCALPIDResponse_h

/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// IlcEMCALPIDResponse                                                  //
//                                                                      //
// EMCAL class to perfom PID                                            //
// This is a prototype and still under development                      //
//                                                                      //
// Author: Michael Weber (m.weber@cern.ch)                              //
//////////////////////////////////////////////////////////////////////////

#include "IlcPID.h"
#include <TVectorD.h>

class TF1;

class IlcEMCALPIDResponse: public TObject 
{
public : 
    IlcEMCALPIDResponse();    //ctor
    IlcEMCALPIDResponse( const IlcEMCALPIDResponse& other);                //copy ructor
    IlcEMCALPIDResponse &operator=( const IlcEMCALPIDResponse& other);     //assignment operator

    virtual ~IlcEMCALPIDResponse();     //dtor
  

    // Getters
    Double_t  GetNumberOfSigmas( Float_t pt,  Float_t eop, IlcPID::EParticleType n,  Int_t charge) const;
    Double_t  GetExpectedNorm  ( Float_t pt, IlcPID::EParticleType n,  Int_t charge) const;
  
    //Setters
    void   SetPIDParams(const TObjArray * params) { fkPIDParams = params; }
    

    // EMCAL probability -> should go to another place?
    Double_t ComputeEMCALProbability( Float_t pt, Float_t eop, Int_t charge, Double_t *pEMCAL) const;

protected:
  
private:

  TF1 *fNorm;                            // Gauss function for normalizing NON electron probabilities 

  const TObjArray *fkPIDParams;               // PID Params

  const TVectorD* GetParams(Int_t nParticle, Float_t fPt, Int_t charge) const; 

  ClassDef(IlcEMCALPIDResponse, 1)
};

#endif // #ifdef IlcEMCALPIDResponse_cxx


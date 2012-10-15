#ifndef IlcHMPIDPIDResponse_h
#define IlcHMPIDPIDResponse_h

/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// IlcHMPIDPIDResponse                                                          //
//                                                                      //
// HMPID class to perfom pattern recognition based on Hough transfrom   //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#include <TNamed.h>        //base class

class IlcESDtrack;

class IlcHMPIDPIDResponse : public TNamed 
{
public : 
             IlcHMPIDPIDResponse();    //ctor
    virtual ~IlcHMPIDPIDResponse() {;} //dtor
    Double_t CosTheta(Float_t *mod, Int_t species);
    Double_t Resolution(Double_t thetaCerTh, IlcESDtrack *pTrk);   //Find the sigma for a given ThetaCerTh

//
protected:
  
private:
  IlcHMPIDPIDResponse(const IlcHMPIDPIDResponse& r);                //dummy copy constructor
  IlcHMPIDPIDResponse &operator=(const IlcHMPIDPIDResponse& r);     //dummy assignment operator
//
  ClassDef(IlcHMPIDPIDResponse,0)
};

#endif // #ifdef IlcHMPIDPIDResponse_cxx


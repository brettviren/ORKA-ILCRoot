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
//
// PID Response class for the TRD detector
// Based on 1D Likelihood approach
// For further information see implementation file
//
#ifndef ILCTRDPIDRESPONSE_H
#define ILCTRDPIDRESPONSE_H

#ifndef ROOT_TObject
#include <TObject.h>
#endif

#ifndef ILCPID_H
#include "IlcPID.h"
#endif

class TObjArray;
class IlcVTrack;
class IlcTRDPIDResponseObject;

class IlcTRDPIDResponse : public TObject {
  public:
    enum ETRDPIDResponseStatus {
      kIsOwner = BIT(14)
    };
    enum ETRDPIDResponseDef {
	kNlayer = 6
	,kNPBins = 6
    };
    enum ETRDPIDMethod {
	kNN   = 0,
	kLQ2D = 1,
	kLQ1D = 2
    };
    enum ETRDPIDNMethod {
	kNMethod=3
    };
    enum ETRDNslices {
	kNslicesLQ1D = 1,
      kNslicesLQ2D = 2,
      kNslicesNN = 7
    };
    IlcTRDPIDResponse();
    IlcTRDPIDResponse(const IlcTRDPIDResponse &ref);
    IlcTRDPIDResponse& operator=(const IlcTRDPIDResponse &ref);
    ~IlcTRDPIDResponse();
    
    ETRDPIDMethod     GetPIDmethod() const { return fPIDmethod;}
    Bool_t    GetResponse(Int_t n, const Double_t * const dedx, const Float_t * const p, Double_t prob[IlcPID::kSPECIES], Bool_t kNorm=kTRUE) const;
    inline ETRDNslices  GetNumberOfSlices() const;
    
    Bool_t    IsOwner() const {return TestBit(kIsOwner);}
    
    void      SetOwner();
    void      SetPIDmethod(ETRDPIDMethod m) {fPIDmethod=m;}
    void      SetGainNormalisationFactor(Double_t gainFactor) { fGainNormalisationFactor = gainFactor; }

    Bool_t SetPIDResponseObject(const IlcTRDPIDResponseObject * obj);
    
    Bool_t    Load(const Char_t *filename = NULL);
  
    Bool_t    IdentifiedAsElectron(Int_t nTracklets, const Double_t *like, Double_t p, Double_t level) const;
  
  private:
    Bool_t    CookdEdx(Int_t nSlice, const Double_t * const in, Double_t *out) const;
    Double_t  GetProbabilitySingleLayer(Int_t species, Double_t plocal, Double_t *dEdx) const;
    
    const IlcTRDPIDResponseObject *fkPIDResponseObject;   // PID References and Params
    Double_t  fGainNormalisationFactor;         // Gain normalisation factor
    ETRDPIDMethod   fPIDmethod;                 // PID method selector
      
  
  ClassDef(IlcTRDPIDResponse, 3)    // Tool for TRD PID
};

IlcTRDPIDResponse::ETRDNslices IlcTRDPIDResponse::GetNumberOfSlices() const {
  // Get the current number of slices
  ETRDNslices slices = kNslicesLQ1D;
  switch(fPIDmethod){
    case kLQ1D: slices = kNslicesLQ1D; break;
    case kLQ2D: slices = kNslicesLQ2D; break;
    case kNN:   slices = kNslicesNN; break;
  };
  return slices;
}
#endif


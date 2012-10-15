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
// Container for TRD PID Response Objects stored in the OADB
//
#ifndef ILCTRDPIDRESPONSEOBJECT_H
#define ILCTRDPIDRESPONSEOBJECT_H

#ifndef ROOT_TNamed
#include <TNamed.h>
#endif

#ifndef IlcTRDPIDRESPONSE_H
#include "IlcTRDPIDResponse.h"
#endif


class IlcTRDPIDParams;
class IlcTRDPIDReference;
class IlcTRDPIDResponse;

class IlcTRDPIDResponseObject : public TNamed{
public:
    enum ETRDPIDResponseObjectStatus {
	kIsOwner = BIT(14)
    };

    IlcTRDPIDResponseObject();
    IlcTRDPIDResponseObject(const char *name);
    IlcTRDPIDResponseObject(const IlcTRDPIDResponseObject &ref);
    IlcTRDPIDResponseObject &operator=(const IlcTRDPIDResponseObject &ref);
    virtual ~IlcTRDPIDResponseObject();

    virtual void Print(Option_t *opt) const;

    void SetPIDParams(IlcTRDPIDParams *params,IlcTRDPIDResponse::ETRDPIDMethod method=IlcTRDPIDResponse::kLQ1D);
    void SetPIDReference(IlcTRDPIDReference *params,IlcTRDPIDResponse::ETRDPIDMethod method=IlcTRDPIDResponse::kLQ1D);

    // Derive reference
    TObject *GetLowerReference(IlcPID::EParticleType spec, Float_t p, Float_t &pLower,IlcTRDPIDResponse::ETRDPIDMethod method=IlcTRDPIDResponse::kLQ1D) const;
    TObject *GetUpperReference(IlcPID::EParticleType spec, Float_t p, Float_t &pUpper,IlcTRDPIDResponse::ETRDPIDMethod method=IlcTRDPIDResponse::kLQ1D) const;

    Int_t GetNumberOfMomentumBins(IlcTRDPIDResponse::ETRDPIDMethod method=IlcTRDPIDResponse::kLQ1D) const;

    // Derive threshold params
    Bool_t GetThresholdParameters(Int_t ntracklets, Double_t efficiency, Double_t *params,IlcTRDPIDResponse::ETRDPIDMethod method=IlcTRDPIDResponse::kLQ1D) const;

    // Number of SlicesQ0
    Int_t GetNSlicesQ0() const{return fNSlicesQ0;}
    void SetNSlicesQ0(Int_t nsl){fNSlicesQ0=nsl;}

private:

      IlcTRDPIDParams *fPIDParams[IlcTRDPIDResponse::kNMethod]; // Contains Thresholds
      IlcTRDPIDReference *fPIDReference[IlcTRDPIDResponse::kNMethod]; // Contains References
      Int_t fNSlicesQ0; // Number of Slices for Q0

    ClassDef(IlcTRDPIDResponseObject, 1);
};
#endif

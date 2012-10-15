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
// Container class for the reference distributions for TRD PID
// The class contains the reference distributions and the momentum steps
// the references are taken at. Mapping is done inside. To derive references,
// the functions GetUpperReference and GetLowerReference return the next
// reference distribution object and the momentum step above respectively below
// the tracklet momentum.
//
// Authors:
//    Markus Fasel <M.Fasel@gsi.de>
//    Daniel Lohner <Daniel.Lohner@cern.ch>

#include "IlcLog.h"

#include "IlcTRDPIDResponseObject.h"

#ifndef IlcTRDPIDREFERENCE_H
#include "IlcTRDPIDReference.h"
#endif

#ifndef IlcTRDPIDPARAMS_H
#include "IlcTRDPIDParams.h"
#endif


ClassImp(IlcTRDPIDResponseObject)

//____________________________________________________________
IlcTRDPIDResponseObject::IlcTRDPIDResponseObject():
    TNamed(),
    fNSlicesQ0(4)
{
    //
    // Dummy constructor
    //
    SetBit(kIsOwner, kTRUE);

    for(Int_t method=0;method<IlcTRDPIDResponse::kNMethod;method++){
	fPIDParams[method]=NULL;
	fPIDReference[method]=NULL;
    }
}

//____________________________________________________________
IlcTRDPIDResponseObject::IlcTRDPIDResponseObject(const Char_t *name):
TNamed(name, "TRD PID Response Object"),
fNSlicesQ0(4)
{
	//
	// Default constructor
	//
	SetBit(kIsOwner, kTRUE);

	for(Int_t method=0;method<IlcTRDPIDResponse::kNMethod;method++){
	    fPIDParams[method]=NULL;
	    fPIDReference[method]=NULL;
	}
}

//____________________________________________________________
IlcTRDPIDResponseObject::IlcTRDPIDResponseObject(const IlcTRDPIDResponseObject &ref):
TNamed(ref),
fNSlicesQ0(ref.fNSlicesQ0)
{
    //
    // Copy constructor
    // Only copies pointers, object is not the owner of the references
    //
    SetBit(kIsOwner, kFALSE);

    for(Int_t method=0;method<IlcTRDPIDResponse::kNMethod;method++){
	fPIDParams[method]=ref.fPIDParams[method];       // new Object is not owner, copy only pointer
	fPIDReference[method]=ref.fPIDReference[method];    // new Object is not owner, copy only pointer
    }
}
//____________________________________________________________
IlcTRDPIDResponseObject &IlcTRDPIDResponseObject::operator=(const IlcTRDPIDResponseObject &ref){
	//
	// Assginment operator
	// Only copies poiters, object is not the owner of the references
	//
	if(this != &ref){
	    TNamed::operator=(ref);
	    fNSlicesQ0=ref.fNSlicesQ0;
	    for(Int_t method=0;method<IlcTRDPIDResponse::kNMethod;method++){
	      if(TestBit(kIsOwner) && fPIDParams[method]){
		delete fPIDParams[method];
		fPIDParams[method]= 0;
	      }
	      if(TestBit(kIsOwner) && fPIDReference[method]){
		delete fPIDReference[method];
		fPIDReference[method] = 0;
	      }
	      printf("Assignment");
	      fPIDParams[method]=ref.fPIDParams[method];       // new Object is not owner, copy only pointer
	      fPIDReference[method]=ref.fPIDReference[method];    // new Object is not owner, copy only pointer
	    }
	    SetBit(kIsOwner, kFALSE);
	}
	return *this;
}

//____________________________________________________________
IlcTRDPIDResponseObject::~IlcTRDPIDResponseObject(){
	//
	// Destructor
	// references are deleted if the object is the owner
	//
    for(Int_t method=0;method<IlcTRDPIDResponse::kNMethod;method++){
      if(fPIDParams && TestBit(kIsOwner)){
	delete fPIDParams[method];fPIDParams[method] = 0;
      }
      if(fPIDReference && TestBit(kIsOwner)){
	delete fPIDReference[method];
	fPIDReference[method] = 0;
      }
    }
}

//____________________________________________________________
void IlcTRDPIDResponseObject::SetPIDParams(IlcTRDPIDParams *params,IlcTRDPIDResponse::ETRDPIDMethod method){

    if(Int_t(method)>=Int_t(IlcTRDPIDResponse::kNMethod)||Int_t(method)<0){
	IlcError("Method does not exist");
	return;
    }
    if(fPIDParams[method]){
	delete fPIDParams[method];
        fPIDParams[method]=NULL;
    }

    fPIDParams[method]=new IlcTRDPIDParams(*params);
}

//____________________________________________________________
void IlcTRDPIDResponseObject::SetPIDReference(IlcTRDPIDReference *reference,IlcTRDPIDResponse::ETRDPIDMethod method){

    if(Int_t(method)>=Int_t(IlcTRDPIDResponse::kNMethod)||Int_t(method)<0){
        IlcError("Method does not exist");
	return;
    }
    if(fPIDReference[method]){
	delete fPIDReference[method];
	fPIDReference[method]=NULL;
    }
    fPIDReference[method]=new IlcTRDPIDReference(*reference);
}

//____________________________________________________________
TObject *IlcTRDPIDResponseObject::GetUpperReference(IlcPID::EParticleType spec, Float_t p, Float_t &pUpper,IlcTRDPIDResponse::ETRDPIDMethod method) const{

    if(Int_t(method)>=Int_t(IlcTRDPIDResponse::kNMethod)||Int_t(method)<0){
	IlcError("Method does not exist");
	return NULL;
    }
   
    if(fPIDReference[method]){
	return fPIDReference[method]->GetUpperReference(spec,p,pUpper);
    }
    return NULL;
}


//____________________________________________________________
TObject *IlcTRDPIDResponseObject::GetLowerReference(IlcPID::EParticleType spec, Float_t p, Float_t &pLower,IlcTRDPIDResponse::ETRDPIDMethod method) const{

    if(Int_t(method)>=Int_t(IlcTRDPIDResponse::kNMethod)||Int_t(method)<0){
	IlcError("Method does not exist");
	return NULL;
    }

    if(fPIDReference[method]){
	 return fPIDReference[method]->GetLowerReference(spec,p,pLower);
     }
    return NULL;
}

//____________________________________________________________
Bool_t IlcTRDPIDResponseObject::GetThresholdParameters(Int_t ntracklets, Double_t efficiency, Double_t *params,IlcTRDPIDResponse::ETRDPIDMethod method) const{

    if(Int_t(method)>=Int_t(IlcTRDPIDResponse::kNMethod)||Int_t(method)<0){
	IlcError("Method does not exist");
	return kFALSE;
    }

    if(fPIDParams[method]){
	return fPIDParams[method]->GetThresholdParameters(ntracklets,efficiency,params);
    }
    return kFALSE;
}

//____________________________________________________________
Int_t IlcTRDPIDResponseObject::GetNumberOfMomentumBins(IlcTRDPIDResponse::ETRDPIDMethod method) const{

    if(Int_t(method)>=Int_t(IlcTRDPIDResponse::kNMethod)||Int_t(method)<0){
	IlcError("Method does not exist");
	return 0;
    }

    if(fPIDReference[method]){
	return fPIDReference[method]->GetNumberOfMomentumBins();
    }
    return 0;
}

//____________________________________________________________
void IlcTRDPIDResponseObject::Print(const Option_t* opt) const{
	//
	// Print content of the PID object
	//
    printf("Content of IlcTRDPIDResponseObject \n\n");
   
    for(Int_t method=0;method<IlcTRDPIDResponse::kNMethod;method++){
	if(fPIDReference[method])fPIDReference[method]->Print(opt);
	if(fPIDParams[method])printf("+ Threshold Parameters \n");
    }
}

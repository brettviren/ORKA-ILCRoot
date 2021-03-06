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

/* $Id: IlcMCParticle.cxx 50674 2011-07-19 09:58:05Z morsch $ */

//-------------------------------------------------------------------------
//     Realisation of IlcVParticle for MC Particles
//     Implementation wraps a TParticle and delegates the methods
//     Author: Andreas Morsch, CERN
//-------------------------------------------------------------------------

#include <TObjArray.h>

#include "IlcMCParticle.h"
#include "IlcExternalTrackParam.h" 


ClassImp(IlcMCParticle)

IlcMCParticle::IlcMCParticle():
    IlcVParticle(),
    fParticle(0),
    fTrackReferences(0),
    fNTrackRef(0),
    fLabel(-1),
    fMother(-1),
    fFirstDaughter(-1),
    fLastDaughter(-1)
{
    // Constructor
}

    
IlcMCParticle::IlcMCParticle(TParticle* part, TObjArray* rarray, Int_t index):
    IlcVParticle(),
    fParticle(part),
    fTrackReferences(rarray),
    fNTrackRef(0),
    fLabel(index),
    fMother(-1),
    fFirstDaughter(-1),
    fLastDaughter(-1)
{
    // Constructor
    if (rarray != 0) {
      fNTrackRef = fTrackReferences->GetEntriesFast();
    }
}
    
    
IlcMCParticle::IlcMCParticle(const IlcMCParticle& mcPart) :
    IlcVParticle(mcPart),
    fParticle(0),    
    fTrackReferences(0),
    fNTrackRef(0),
    fLabel(-1),
    fMother(-1),
    fFirstDaughter(-1),
    fLastDaughter(-1)
{
// Copy constructor
}

IlcMCParticle& IlcMCParticle::operator=(const IlcMCParticle& mcPart)
{ 
// Copy constructor
  if (this!=&mcPart) { 
    IlcVParticle::operator=(mcPart);
  }
  
  return *this; 
}

IlcMCParticle::~IlcMCParticle()
{ 
  // delete the track references passed externally
  // fParticle should be handled by the user
  // IlcStack in case of IlcMCEventHandler
  if(fTrackReferences){
    fTrackReferences->Clear();
      delete fTrackReferences;
      fTrackReferences = 0;
  }
}



Float_t  IlcMCParticle::GetTPCTrackLength(Float_t bz,  Float_t ptmin, Int_t &counter, Float_t deadWidth){
  //
  // return track length in geometrically active volume of TPC.
  // z nad rphi acceptance is included
  // doesn't take into account dead channel and ExB  
  // Intput:
  // trackRefs
  // bz - magnetic field
  // deadWidth - dead zone in r-phi
  // Additional output:
  // counter   - number of circles

    if (fNTrackRef == 0) return 0.; 

    const Float_t kRMin = 90;
    const Float_t kRMax = 245;
    const Float_t kZMax = 250;
    const Float_t kMinPt= ptmin; 

    Float_t length =0;
    Int_t nrefs = fNTrackRef;

    
    IlcExternalTrackParam param;
    Double_t cv[21];
    for (Int_t i = 0; i < 21; i++) cv[i]=0;
    counter=0; 
    //
    //
    
    IlcTrackReference *ref0 = (IlcTrackReference*) (fTrackReferences->At(0));
    Float_t direction = 0;
  //
    for (Int_t iref = 1; iref < nrefs; iref++){
	IlcTrackReference *ref = (IlcTrackReference*) (fTrackReferences->At(iref));
	if (!ref) continue;
	if (!ref0 || ref0->DetectorId()!= IlcTrackReference::kTPC){
	    ref0 = ref;
	    direction = ((ref0->X() * ref0->Px() + ref0->Y() * ref0->Py()) > 0)? 1. : -1.;
	    continue;
	}
	
	Float_t newdirection = ((ref->X() * ref->Px() + ref->Y() * ref->Py()) > 0)? 1. : -1.;
	if (newdirection*direction<0) {
	    counter++;  //circle counter 
	    direction = newdirection;
	    continue;
	}
	if (counter>0) continue;
	if (ref0->Pt() < kMinPt) break;
	Float_t radius0 = TMath::Max(TMath::Min(ref0->R(),kRMax),kRMin);;
	Float_t radius1 = TMath::Max(TMath::Min(ref->R(),kRMax),kRMin);
	Double_t xyz[3] = {ref0->X(), ref0->Y(), ref0->Z()};
	Double_t pxyz[3]= {ref0->Px(), ref0->Py(), ref0->Pz()};
	Double_t alpha;
	param.Set(xyz,pxyz,cv,TMath::Nint(fParticle->GetPDG()->Charge()/3.));
	
	for (Float_t radius = radius0; radius < radius1; radius+=1){
	    param.GetXYZAt(radius, bz, xyz);
	    if (TMath::Abs(xyz[2]) > kZMax) continue;
	    Float_t gradius = TMath::Sqrt(xyz[1] * xyz[1] + xyz[0] * xyz[0]);
	    if (gradius > kRMax) continue;
	    alpha = TMath::ATan2(xyz[1],xyz[0]);
	    if (alpha<0) alpha += TMath::TwoPi();
	    //
	    Int_t sector   = Int_t(9 * alpha / TMath::Pi());
	    Float_t lalpha = alpha - ((sector + 0.5) * TMath::Pi() / 9.);
	    Float_t dedge  = (TMath::Tan(TMath::Pi() / 18.) - TMath::Abs(TMath::Tan(lalpha))) * gradius; 
	    if (dedge>deadWidth) length++;
	}
	if (ref->DetectorId()!= IlcTrackReference::kTPC) break; 
	ref0 = ref;
    }
    return length;
}

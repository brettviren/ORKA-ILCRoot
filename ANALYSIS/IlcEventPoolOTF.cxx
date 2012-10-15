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

/* $Id$ */
 

// Realisation of an IlcVEventPool via
// on the flight (OTF) generation of the bin using IlcTagAnalysis.
// Author Andreas Morsch
// andreas.morsch@cern.ch

#include "IlcEventPoolOTF.h"

#include "IlcRunTagCuts.h"
#include "IlcLHCTagCuts.h"
#include "IlcDetectorTagCuts.h"
#include "IlcEventTagCuts.h"
#include "IlcTagAnalysis.h"

#include <TMath.h>
#include <TChain.h>
#include <TGridResult.h>

ClassImp(IlcEventPoolOTF)


////////////////////////////////////////////////////////////////////////

IlcEventPoolOTF::IlcEventPoolOTF():
    IlcVEventPool(),
    fTagAnalysis(0),
    fRunCuts(0),
    fLHCCuts(0),
    fDetectorCuts(0),
    fEventCuts(0),
    fGridTags(0),
    fChain(0),
    fTagDirectory(0),
    fValueMin(),
    fValueMax(),
    fValueStep(),
    fValue(),    
    fBinNumber(0),
    fNoMore(0)

{
  // Default constructor
    InitArrays();
}

IlcEventPoolOTF::IlcEventPoolOTF(const char* name, const char* title):
    IlcVEventPool(name, title),
    fTagAnalysis(new IlcTagAnalysis(title)),
    fRunCuts(new IlcRunTagCuts()),
    fLHCCuts(new IlcLHCTagCuts()),
    fDetectorCuts(new IlcDetectorTagCuts()),
    fEventCuts(new IlcEventTagCuts()),
    fGridTags(0),
    fChain(0),
    fTagDirectory("."),
    fValueMin(),
    fValueMax(),
    fValueStep(),
    fValue(),    
    fBinNumber(0),
    fNoMore(0)

{
  // Constructor
    InitArrays();
}


IlcEventPoolOTF::IlcEventPoolOTF(const IlcEventPoolOTF& obj):
    IlcVEventPool(obj),
    fTagAnalysis(0),
    fRunCuts(0),
    fLHCCuts(0),
    fDetectorCuts(0),
    fEventCuts(0),
    fGridTags(0),
    fChain(0),
    fTagDirectory(0),
    fValueMin(),
    fValueMax(),
    fValueStep(),
    fValue(),    
    fBinNumber(0),
    fNoMore(0)
{
    // Copy constructor
    InitArrays();
}


IlcEventPoolOTF::~IlcEventPoolOTF()
{
    // Destructor
    delete fTagAnalysis;
    delete fRunCuts;
    delete fEventCuts;
    delete fLHCCuts;
    delete fDetectorCuts;
    delete fChain;
}

IlcEventPoolOTF& IlcEventPoolOTF::operator=(const IlcEventPoolOTF& other)
{
// Assignment operator
    IlcVEventPool::operator=(other);
    return *this;
}


void IlcEventPoolOTF::Init()
{
    // Initialisation
    if (!fGridTags) {
	fTagAnalysis->ChainLocalTags(fTagDirectory);
    } else {
	fTagAnalysis->ChainGridTags(fGridTags);
    }
    
    
    for (Int_t i = 0; i < 5; i++) fValue[i] = fValueMin[i];    
}

TChain* IlcEventPoolOTF::GetNextChain()
{
    // Get Next Chain
    if (fChain) {
	delete fChain;
	fChain = 0;
    }

    fBinNumber++;
    if (fNoMore) {
 	return 0;
    } else {
    printf("Current bin (lower) %13.3f %13.3f %13.3f %13.3f %13.3f \n", fValue[kMultiplicity], fValue[kZVertex], fValue[kEventPlane],fValue[kLeadingParticleEta],fValue[kLeadingParticlePhi]);
    printf("Current bin (upper) %13.3f %13.3f %13.3f %13.3f %13.3f \n", fValue[kMultiplicity] + fValueStep[kMultiplicity] - 1, 
	   fValue[kZVertex]      + fValueStep[kZVertex], 
	   fValue[kEventPlane]   + fValueStep[kEventPlane],
	   fValue[kLeadingParticleEta]   + fValueStep[kLeadingParticleEta],
           fValue[kLeadingParticlePhi]   + fValueStep[kLeadingParticlePhi]
    
	   );

	fEventCuts->SetMultiplicityRange(Int_t(fValue[kMultiplicity]) , Int_t(fValue[kMultiplicity] + fValueStep[kMultiplicity] - 1));
	fEventCuts->SetPrimaryVertexZRange(fValue[kZVertex] , fValue[kZVertex] + fValueStep[kZVertex]);
        fEventCuts->SetEtaLeadingParticleRange(fValue[kLeadingParticleEta] , fValue[kLeadingParticleEta] + fValueStep[kLeadingParticleEta]);
        fEventCuts->SetPhiLeadingParticleRange(fValue[kLeadingParticlePhi] , fValue[kLeadingParticlePhi] + fValueStep[kLeadingParticlePhi]);
        fEventCuts->SetEventPlaneAngleRange(fValue[kEventPlane] , fValue[kEventPlane] + fValueStep[kEventPlane]);
    
	fChain = fTagAnalysis->QueryTags(fRunCuts, fLHCCuts, fDetectorCuts, fEventCuts);
//
//      Next bin 
//
	for (Int_t i = 5; i >= 0; i--) 
	{
	    fValue[i] += fValueStep[i];
	    if (i > 0  && fValue[i] >= fValueMax[i]) {
		fValue[i] = fValueMin[i];
	    } else if (i == 0 && fValue[i] >= fValueMax[i]) {
		fNoMore = kTRUE;
	    } else {
		break;
	    }
	}
	return fChain;
    }
}

void  IlcEventPoolOTF::GetCurrentBin(Float_t* /*bin*/)
{
    //
}

Int_t IlcEventPoolOTF::GetDimension()
{
    //
    return (5);
}

void IlcEventPoolOTF::InitArrays()
{
    // Initializes the pool axis
    
    SetMultiplicityBinning(0, 20000, 20000);
    SetZVertexBinning(-1000., 1000., 2000.);
    SetEventPlaneBinning(-1000., 1000., 2000.);
    SetLeadingParticleEtaBinning(-13.0, 13.0, 27.);
    SetLeadingParticlePhiBinning(0., 2*(TMath::Pi()),2*(TMath::Pi()));
    for (Int_t i = 0; i < 5; i++) fValue[i] = fValueMin[i];
}



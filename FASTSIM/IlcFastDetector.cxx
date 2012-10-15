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

/* $Id: IlcFastDetector.cxx 23931 2008-02-15 10:22:38Z morsch $ */
//
// Base class for fast simulation of a detctor
// or a system of subdetectors.
// The detector response is described by resolution and efficiency.
// Author:
// Andreas Morsch
// andreas.morsch@cern.ch

#include "IlcFastDetector.h"
#include "IlcFastResponse.h"
#include "IlcGeometry.h"

#include <TList.h>
#include <TIterator.h>
#include <TString.h>

ClassImp(IlcFastDetector)


IlcFastDetector::IlcFastDetector():
    fSubdetectors(0),
    fResponses(0),
    fLnkD(0),
    fLnkR(0),
    fEfficiency(0),
    fResolution(0),
    fGeometry(0)
{
// Default Constructor
    fName  = "FastDetector";
    fTitle = "Fast Detector Base Class";
}

IlcFastDetector::IlcFastDetector(char* Name, char* Title):
    TNamed(Name, Title),
    fSubdetectors(new TList()),
    fResponses(new TList()),
    fLnkD(0),
    fLnkR(0),
    fEfficiency(0),
    fResolution(0),
    fGeometry(0)
{
// Constructor
 }

IlcFastDetector::IlcFastDetector(const IlcFastDetector & det)
    :TNamed(det),
    fSubdetectors(0),
    fResponses(0),
    fLnkD(0),
    fLnkR(0),
    fEfficiency(0),
    fResolution(0),
    fGeometry(0)
{
// Copy constructor
    det.Copy(*this);
}

IlcFastDetector::~IlcFastDetector()
{
// Destructor
    delete fSubdetectors;
    delete fResponses;
}


void IlcFastDetector::Init()
{
//
// Initialisation
//
    TIter nextRes(fResponses);
    IlcFastResponse *res;
    //
    // Loop over responses  and initialize
    while((res = (IlcFastResponse*)nextRes())) {
	res->Init();
    }  

    TIter nextDet(fSubdetectors);
    IlcFastDetector *det;
    //
    // Loop over subdetectors  and initialize
    while((det = (IlcFastDetector*)nextDet())) {
	det->Init();
    }  
    //
    TObject* obj;
    
    if ((obj = fResponses->FindObject("Efficiency")))
    {

	fEfficiency = (IlcFastResponse*) obj;
	printf("Detector %s provides Efficiency: %s\n",
	       fName.Data(), fEfficiency->GetTitle());
    }
    
    if ((obj = fResponses->FindObject("Resolution"))) 
    {
	fResolution = (IlcFastResponse*) obj;
	printf("Detector %s provides Resolution: %s\n",
	       fName.Data(), fResolution->GetTitle());
    }
}

Float_t IlcFastDetector::EvaluateEfficiency(IlcFastParticle* part)
{
//
//  Evaluate the efficiency for detecting particle part
//
    TIter nextDet(fSubdetectors);
    IlcFastDetector *det;
    //
    // Loop over subdetectors  
    Float_t eff = 1;
    while((det = (IlcFastDetector*)nextDet())) {
	eff *= det->EvaluateEfficiency(part);
    }  
    return eff;
}

Bool_t  IlcFastDetector::EvaluateAcceptance(IlcFastParticle* part)
{
    //
    // Loop over subdetectors 
    Bool_t acc = kFALSE;

    if (fSubdetectors) {
	TIter nextDet(fSubdetectors);
	IlcFastDetector *det;
	while((det = (IlcFastDetector*)nextDet()) && !acc) {
	    acc = (acc ||  det->EvaluateAcceptance(part));
	}  
    } else {
        if (fGeometry)
	acc = fGeometry->Impact((TParticle*) part);
    }
    
    return acc;
}

void    IlcFastDetector::EvaluateResponse(IlcFastParticle* /*part*/)
{
    ;
}

void IlcFastDetector::
AddSubdetector(IlcFastDetector *Detector, char* /*Name*/)
{
//
//  Add detector to list   
     fSubdetectors->Add(Detector);
}



void IlcFastDetector::
AddResponse(IlcFastResponse *Response)
{
//
//  Add detector to list   
     fResponses->Add(Response);
}


IlcFastDetector*  IlcFastDetector::FirstSubdetector()
{
// Iterator over generators: Initialisation
    fLnkD = fSubdetectors->FirstLink();
    if (fLnkD) {
	return (IlcFastDetector*) (fLnkD->GetObject());
    } else {
	return 0;
    }
}

IlcFastDetector*  IlcFastDetector::NextSubdetector()
{
// Iterator over generators: Increment
    fLnkD = fLnkD->Next();
    if (fLnkD) {
	return (IlcFastDetector*) (fLnkD->GetObject());
    } else {
	return 0;
    }
}


IlcFastResponse*  IlcFastDetector::FirstResponse()
{
// Iterator over generators: Initialisation
    fLnkR = fResponses->FirstLink();
    if (fLnkR) {
	return (IlcFastResponse*) (fLnkR->GetObject());
    } else {
	return 0;
    }
}

IlcFastResponse*  IlcFastDetector::NextResponse()
{
// Iterator over generators: Increment
    fLnkR = fLnkR->Next();
    if (fLnkR) {
	return (IlcFastResponse*) (fLnkR->GetObject());
    } else {
	return 0;
    }
}


IlcFastDetector& IlcFastDetector::operator=(const  IlcFastDetector& rhs)
{
// Assignment operator
    rhs.Copy(*this);
    return *this;
}

void IlcFastDetector::Copy(TObject&) const
{
    //
    // Copy 
    //
    Fatal("Copy","Not implemented!\n");
}


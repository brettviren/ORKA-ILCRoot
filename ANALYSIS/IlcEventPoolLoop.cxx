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
 

// Realisation of an IlcVEventPool which allows the user to
// run the analysis in a loop, i.e. passing several times over 
// the same event chain.
// Author Andreas Morsch
// andreas.morsch@cern.ch


#include "IlcEventPoolLoop.h"
#include <TChain.h>
#include <TFile.h>
#include <TObjArray.h>

ClassImp(IlcEventPoolLoop)


////////////////////////////////////////////////////////////////////////

IlcEventPoolLoop::IlcEventPoolLoop():
    IlcVEventPool(),
    fMaxIterations(0),
    fNIteration(1),
    fChainClone(0)
{
  // Default constructor
}

IlcEventPoolLoop::IlcEventPoolLoop(Int_t nit):
    IlcVEventPool(),
    fMaxIterations(nit),
    fNIteration(1),
    fChainClone(0)
{
  // Default constructor
}

IlcEventPoolLoop::IlcEventPoolLoop(const char* name, const char* title):
    IlcVEventPool(name, title),
    fMaxIterations(0),
    fNIteration(1),
    fChainClone(0)
{
  // Constructor
}


IlcEventPoolLoop::IlcEventPoolLoop(const IlcEventPoolLoop& obj):
    IlcVEventPool(obj),
    fMaxIterations(obj.fMaxIterations),
    fNIteration(obj.fNIteration),
    fChainClone(0)
{
    // Copy constructor
}

IlcEventPoolLoop& IlcEventPoolLoop::operator=(const IlcEventPoolLoop& other)
{
// Assignment operator
    IlcVEventPool::operator=(other);
    fMaxIterations = other.fMaxIterations;
    fNIteration    = other.fNIteration;
    return *this;
}


void IlcEventPoolLoop::Init()
{
// Initialisation

    fMaxIterations = 0;
    fNIteration    = 1;
}

TChain* IlcEventPoolLoop::GetNextChain()
{
    // Get the next chain
    if (fNIteration > fMaxIterations) {
	return (0);
    } else {
	fNIteration++;
	/*
	if (fChainClone) {
	  fChainClone->Reset();
	  new (fChainClone) TChain(fChain->GetName());
	} else {
	  fChainClone = new TChain(fChain->GetName());
	}
	TObjArray* files = fChain->GetListOfFiles();
	Int_t n = files->GetEntriesFast();
	for (Int_t i = 0; i < n; i++) {
	  TFile* file = (TFile*) (files->At(i));
	  fChainClone->AddFile(file->GetTitle());
	  printf("Adding %s %s %s\n", fChainClone->GetName(), file->GetName(), file->GetTitle());

	}
	*/
	fChainClone = (TChain*) (fChain->Clone());
	return fChainClone;
    }
}

void  IlcEventPoolLoop::GetCurrentBin(Float_t* /*bin*/)
{
    //
}

Int_t IlcEventPoolLoop::GetDimension()
{
    //
    return (0);
}


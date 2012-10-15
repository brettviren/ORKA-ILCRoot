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

/* $Id: IlcGenCocktail.cxx 55692 2012-04-11 09:03:15Z hristov $ */

// Container class for IlcGenerator through recursion.
// Container is itself an IlcGenerator.
// What is stored are not the pointers to the generators directly but to objects of type
// IlcGenCocktail entry.   
// The class provides also iterator functionality.  
// Author: andreas.morsch@cern.ch 
//

#include <TList.h>
#include <TObjArray.h>
#include <TFormula.h>

#include "IlcGenCocktail.h"
#include "IlcGenCocktailEntry.h"
#include "IlcCollisionGeometry.h"
#include "IlcRun.h"
#include "IlcLog.h"
#include "IlcMC.h"
#include "IlcGenCocktailEventHeader.h"

ClassImp(IlcGenCocktail)

IlcGenCocktail::IlcGenCocktail()
    :IlcGenerator(), 
     fNGenerators(0),
     fTotalRate(0.),
     fSRandom(kFALSE),
     fUsePerEventRate(kFALSE),
     fProb(0),
     fEntries(0),
     flnk1(0),
     flnk2(0), 
     fHeader(0)
{
// Constructor
    fName = "Cocktail";
    fTitle= "Particle Generator using cocktail of generators";
}

IlcGenCocktail::~IlcGenCocktail()
{
// Destructor
    delete fEntries;
    fEntries = 0;
    //    delete fHeader; // It is removed in IlcRunLoader
    fHeader = 0;
}

void IlcGenCocktail::
AddGenerator(IlcGenerator *Generator, const char* Name, Float_t RateExp, TFormula* formula)
{
//
// Add a generator to the list 
// First check that list exists
    if (!fEntries) fEntries = new TList();
    fTotalRate += RateExp;
//
//  Forward parameters to the new generator
    if(TestBit(kPtRange) && !(Generator->TestBit(kPtRange)) && !(Generator->TestBit(kMomentumRange))) 
	Generator->SetPtRange(fPtMin,fPtMax);
    if(TestBit(kMomentumRange) && !(Generator->TestBit(kPtRange)) && !(Generator->TestBit(kMomentumRange)))
	Generator->SetMomentumRange(fPMin,fPMax);
    
    if (TestBit(kYRange) && !(Generator->TestBit(kYRange)))
	Generator->SetYRange(fYMin,fYMax);
    if (TestBit(kPhiRange) && !(Generator->TestBit(kPhiRange)))
	Generator->SetPhiRange(fPhiMin*180/TMath::Pi(),fPhiMax*180/TMath::Pi());
    if (TestBit(kThetaRange) && !(Generator->TestBit(kThetaRange)) && !(Generator->TestBit(kEtaRange)))
	Generator->SetThetaRange(fThetaMin*180/TMath::Pi(),fThetaMax*180/TMath::Pi());
    if (!(Generator->TestBit(kVertexRange))) {
	Generator->SetOrigin(fOrigin[0], fOrigin[1], fOrigin[2]);
	Generator->SetSigma(fOsigma[0], fOsigma[1], fOsigma[2]);
	Generator->SetVertexSmear(fVertexSmear);
	Generator->SetVertexSource(kContainer);
    }
    Generator->SetTrackingFlag(fTrackIt);
    Generator->SetContainer(this);

        
//
//  Add generator to list   
    char theName[256];
    snprintf(theName, 256, "%s_%d",Name, fNGenerators);
    Generator->SetName(theName);

    IlcGenCocktailEntry *entry = 
	new IlcGenCocktailEntry(Generator, Name, RateExp);
    if (formula) entry->SetFormula(formula);    
     fEntries->Add(entry);
     fNGenerators++;
     flnk1 = 0;
     flnk2 = 0;
     fSRandom  = kFALSE;
     fHeader  = 0;
}

  void IlcGenCocktail::Init()
{
// Initialisation
    TIter next(fEntries);
    IlcGenCocktailEntry *entry;
    //
    // Loop over generators and initialize
    while((entry = (IlcGenCocktailEntry*)next())) {
	if (fStack)  entry->Generator()->SetStack(fStack);
	entry->Generator()->Init();
    }  

    next.Reset();

    if (fSRandom) {
	fProb.Set(fNGenerators);
	next.Reset();
	Float_t sum = 0.;
	while((entry = (IlcGenCocktailEntry*)next())) {
	    sum += entry->Rate();
	} 

	next.Reset();
	Int_t i = 0;
	Float_t psum = 0.;
	while((entry = (IlcGenCocktailEntry*)next())) {
	    psum +=  entry->Rate() / sum;
	    fProb[i++] = psum;
	}
    }
	next.Reset();
}

  void IlcGenCocktail::FinishRun()
{
// Initialisation
    TIter next(fEntries);
    IlcGenCocktailEntry *entry;
    //
    // Loop over generators and initialize
    while((entry = (IlcGenCocktailEntry*)next())) {
	entry->Generator()->FinishRun();
    }  
}

 void IlcGenCocktail::Generate()
{
//
// Generate event 
    TIter next(fEntries);
    IlcGenCocktailEntry *entry = 0;
    IlcGenCocktailEntry *preventry = 0;
    IlcGenCocktailEntry *collentry = 0;
    IlcGenerator* gen = 0;
    if (fHeader) delete fHeader;

    
    fHeader = new IlcGenCocktailEventHeader("Cocktail Header");

    const TObjArray *partArray = gIlc->GetMCApp()->Particles();

//
//  Generate the vertex position used by all generators
//    
    if(fVertexSmear == kPerEvent) Vertex();

    TArrayF eventVertex;
    eventVertex.Set(3);
    for (Int_t j=0; j < 3; j++) eventVertex[j] = fVertex[j];

    if (!fSRandom) {
	//
	// Loop over generators and generate events
	Int_t igen   = 0;
	while((entry = (IlcGenCocktailEntry*)next())) {
          Int_t ntimes = 1;
	  if (fUsePerEventRate && (gRandom->Rndm() > entry->Rate())) continue;
	  
	  igen++;
	  if (igen ==1) {
	    entry->SetFirst(0);
	  } else {
	    entry->SetFirst((partArray->GetEntriesFast())+1);
	  }
	  gen = entry->Generator();
	  if (gen->ProvidesCollisionGeometry()) collentry = entry; 
	  //
	  //      Handle case in which current generator needs collision geometry from previous generator
          //
	  if (gen->NeedsCollisionGeometry() && (entry->Formula() == 0))
	    {
	      if (preventry && preventry->Generator()->ProvidesCollisionGeometry())
		{
		  gen->SetCollisionGeometry(preventry->Generator()->CollisionGeometry());
		} else {
		Fatal("Generate()", "No Collision Geometry Provided");
	      }
	    }
	  //
	  //      Number of signals is calculated from Collision Geometry
	  //      and entry with given centrality bin is selected
	  //
	  if (entry->Formula() != 0)
	    {
	      if (!collentry) {
		Fatal("Generate()", "No Collision Geometry Provided");
		return;
	      }
	      IlcCollisionGeometry* coll = (collentry->Generator())->CollisionGeometry();
	      Float_t b  = coll->ImpactParameter();
	      Int_t nsig = Int_t(entry->Formula()->Eval(b));
	      Int_t bin = entry->Bin() - 100;
	      if (bin > 0) {
		if (bin != nsig) continue;
	      } else {
		if (nsig < 1) nsig = 1;
		IlcInfo(Form("Signal Events %13.3f %5d %5d\n", b, coll->HardScatters(), nsig));
		ntimes = nsig;
	      }
	    }
	  gen->SetVertex(fVertex.At(0), fVertex.At(1), fVertex.At(2), fTime);
	  
	  for (Int_t i = 0; i < ntimes; i++) gen->Generate();
	  entry->SetLast(partArray->GetEntriesFast());
	  preventry = entry;
	}
    } else if (fSRandom) {
	//
	// Select a generator randomly
	//
	Int_t i;
	Float_t p0 =  gRandom->Rndm();

	for (i = 0; i < fNGenerators; i++) {
	    if (p0 < fProb[i]) break;
	}

	entry = (IlcGenCocktailEntry*) fEntries->At(i);
	entry->SetFirst(0);
	gen = entry->Generator();
	gen->SetVertex(fVertex.At(0), fVertex.At(1), fVertex.At(2), fTime);
	gen->Generate();
	entry->SetLast(partArray->GetEntriesFast());
    } 
    
    next.Reset();

// Event Vertex
    fHeader->SetPrimaryVertex(eventVertex);
    fHeader->CalcNProduced();
    gIlc->SetGenEventHeader(fHeader); 
}

void IlcGenCocktail::SetVertexSmear(VertexSmear_t smear)
{
// Set vertex smearing and propagate it to the generators

  IlcGenerator::SetVertexSmear(smear);
  TIter next(fEntries);
  while (IlcGenCocktailEntry* entry = (IlcGenCocktailEntry*)next()) {
    entry->Generator()->SetVertexSmear(smear);
  }
}

IlcGenCocktailEntry *  IlcGenCocktail::FirstGenerator()
{
// Iterator over generators: Initialisation
    flnk1 = fEntries->FirstLink();
    if (flnk1) {
	return (IlcGenCocktailEntry*) (flnk1->GetObject());
    } else {
	return 0;
    }
}

IlcGenCocktailEntry*  IlcGenCocktail::NextGenerator()
{
// Iterator over generators: Increment
    flnk1 = flnk1->Next();
    if (flnk1) {
	return (IlcGenCocktailEntry*) (flnk1->GetObject());
    } else {
	return 0;
    }
}

void IlcGenCocktail::
FirstGeneratorPair(IlcGenCocktailEntry*& e1, IlcGenCocktailEntry*& e2)
{
// Iterator over generator pairs: Initialisation
    flnk2 = flnk1 = fEntries->FirstLink();
    if (flnk1) {
	e2 = e1 = (IlcGenCocktailEntry*) (flnk1->GetObject());
    } else {
	e2= e1 = 0;
    }
}

void IlcGenCocktail::
NextGeneratorPair(IlcGenCocktailEntry*& e1, IlcGenCocktailEntry*& e2)
{
// Iterator over generators: Increment
    flnk2 = flnk2->Next();
    if (flnk2) {
	e1 = (IlcGenCocktailEntry*) (flnk1->GetObject());
	e2 = (IlcGenCocktailEntry*) (flnk2->GetObject());	
    } else {
	flnk2 = flnk1 = flnk1->Next();
	if (flnk1) {
	    e1 = (IlcGenCocktailEntry*) (flnk1->GetObject());
	    e2 = (IlcGenCocktailEntry*) (flnk2->GetObject());
	} else {
	    e1=0;
	    e2=0;
	}
    }
}

void IlcGenCocktail::AddHeader(IlcGenEventHeader* header)
{
// Add a header to the list 
    if (fHeader) fHeader->AddHeader(header);
}





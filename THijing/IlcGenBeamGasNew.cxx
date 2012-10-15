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

/* $Id: IlcGenBeamGasNew.cxx 54863 2012-02-28 12:10:29Z hristov $ */



//-------------------------------------------------------------
//  Generate Beam-Gas-Events 
//  Default: underlying event: pO @ 7 TeV (fixed target)
//
//  underlying event can be changed by adding generators 
//  in the same way as to IlcGenCocktail before calling 
//  the Init()
//  Author: Jochen Klein
//-------------------------------------------------------------

#include <TList.h>
#include <TObjArray.h>
#include <TParticle.h>

#include "IlcGenBeamGasNew.h"
#include "IlcGenCocktail.h" 
#include "IlcGenCocktailEntry.h"
#include "IlcGenCocktailEventHeader.h"
#include "IlcGenHijing.h" 
#include "IlcCollisionGeometry.h"
#include "IlcRun.h"
#include "IlcMC.h"
#include "IlcStack.h"
#include "IlcLog.h"

#include "IlcHeader.h"
#include "IlcGenEventHeader.h"

ClassImp(IlcGenBeamGasNew)

Float_t EtaToTheta(Float_t arg) { return (180./TMath::Pi())*2.*atan(exp(-arg)); }

IlcGenBeamGasNew::IlcGenBeamGasNew() : 
    IlcGenCocktail(), 
    fItime(0), 
    fTwindow(88e-6), 
    fZwindow(2000), 
    fRate(1e3)
{
  // Default constructor
}

IlcGenBeamGasNew::~IlcGenBeamGasNew()
{
}

void IlcGenBeamGasNew::SetTimeWindow(Float_t twindow) { fTwindow = twindow; }

bool IlcGenBeamGasNew::SetRate(Float_t rate) {
// Set the Rate
  if (rate >= 0) {  
    fRate = rate; 
    return true;
  } else
    return false;
}

void IlcGenBeamGasNew::SetZWindow(Float_t zwindow) { fZwindow = zwindow; }

void IlcGenBeamGasNew::Init()
{
  //  Initialisation of the class
  //  if no generators were added before calling Init()
  //  IlcGenHijing is added configured for pO

  fVertexSmear = kPerEvent;
  fVertexSource = kInternal;
  fSRandom = kTRUE;

  // Adding default underlying event in case none was specified
  // p-O-collision at 7 TeV (fixed target)
  if (!fEntries) {
    IlcGenHijing *gen = new IlcGenHijing(-1);
    gen->SetReferenceFrame("LAB");
    gen->SetEnergyCMS(7000);
    gen->SetProjectile("P",1,1);
    gen->SetTarget("A",16,8);
    gen->SetPhiRange(0,360);
    Float_t thmin = EtaToTheta(8);
    Float_t thmax = EtaToTheta(-8);
    gen->SetThetaRange(thmin, thmax);
    gen->SetOrigin(0,0,0);
    gen->SetSigma(0,0,0);
    gen->KeepFullEvent();
    gen->SetShadowing(1);
    gen->SetDecaysOff(1);
    gen->SetSpectators(1);
    gen->SetSelectAll(1);
    gen->SetRandomPz(kFALSE);
    gen->SetPileUpTimeWindow(0.);
    AddGenerator(gen,"Hijing pO",1);
  }

  TIter next(fEntries);
  IlcGenCocktailEntry *entry;

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

void IlcGenBeamGasNew::VertexInternal()
{
  // calculation of the interaction vertex for the beam gas event
  // both spatial and time coordinate are adjusted.
  
  Float_t random[2];
  Float_t nbunch;
  Float_t bunch;
  
  gRandom->RndmArray(2,random);
  fVertex[0] = fVertex[1] = 0;
  fVertex[2] = random[0] * 2 * fZwindow - fZwindow;
  nbunch = fTwindow/25e-9;
  bunch = floor(2*nbunch * random[1] - nbunch);
  fItime = fVertex[2] / 100 / 3e8 + bunch * 25e-9;
  IlcInfo(Form("fItime: %13.3e \n", fItime));
}

void IlcGenBeamGasNew::Generate()
{
//    
// Generate the collisions for one event
//
  Int_t numbg = gRandom->Poisson(fRate*fZwindow/100*2*fTwindow);
  if (numbg == 0) return;
  
  TIter next(fEntries);
  IlcGenCocktailEntry *entry = 0;
  IlcGenCocktailEntry *preventry = 0;
  IlcGenerator* gen = 0;
  TLorentzVector v;
  TArrayF eventVertex;
  Int_t lastpart=0;
  Float_t sign;
  
  if (fHeader) delete fHeader;
  fHeader = new IlcGenCocktailEventHeader("Beamgas Header");
  
  const TObjArray *partArray = gIlc->GetMCApp()->Particles();
  IlcStack *stack = IlcRunLoader::Instance()->Stack();
  
  for (Int_t l = 0; l < numbg; l++) {
    Vertex();
    sign = (gRandom->Rndm() < 0.5)? -1. : 1.;
    eventVertex.Set(3);
    for (Int_t j=0; j < 3; j++) eventVertex[j] = fVertex[j];
    
    if (!fRandom) {
      Int_t igen=0;
      while((entry = (IlcGenCocktailEntry*)next())) {
	if (fUsePerEventRate && (gRandom->Rndm() > entry->Rate())) continue;
	
	igen++;
	if (igen ==1) {
	  entry->SetFirst(lastpart);
	} else {
	  entry->SetFirst((partArray->GetEntriesFast())+1);
	}
	//
	//      Handle case in which current generator needs collision geometry from previous generator
	//
	gen = entry->Generator();
	if (gen->NeedsCollisionGeometry())
	  {
	    if (preventry && preventry->Generator()->ProvidesCollisionGeometry())
	      {
		gen->SetCollisionGeometry(preventry->Generator()->CollisionGeometry());
	      } else {
		Fatal("Generate()", "No Collision Geometry Provided");
	      }
	  }
	entry->Generator()->SetVertex(fVertex.At(0), fVertex.At(1), fVertex.At(2));
	entry->Generator()->Generate();
	entry->SetLast(partArray->GetEntriesFast());
	preventry = entry;
      }  
    } else {
      //
      // Select a generator randomly
      //
      Int_t i = 0;
      Float_t p0 =  gRandom->Rndm();
      
      for (i = 0; i < fNGenerators; i++) {
	if (p0 < fProb[i]) break;
      }
      
      entry = (IlcGenCocktailEntry*) fEntries->At(i);
      entry->SetFirst(lastpart);
      gen = entry->Generator();
      gen->SetVertex(fVertex.At(0), fVertex.At(1), fVertex.At(2));
      gen->Generate();
      entry->SetLast(partArray->GetEntriesFast());
    } 
    
    for (Int_t k = lastpart; k < stack->GetNprimary(); k++) {
      stack->Particle(k)->ProductionVertex(v);
      v[2] *= sign;
      eventVertex[2] *= sign;
      v[3] = fItime;  // ??? +=
      stack->Particle(k)->SetProductionVertex(v);
      stack->Particle(k)->Momentum(v);
      v[2] *= sign;
      stack->Particle(k)->SetMomentum(v);
    }

    ((IlcGenEventHeader*) fHeader->GetHeaders()->Last())->SetPrimaryVertex(eventVertex);

    lastpart = stack->GetNprimary();
    next.Reset();
  } 

  fHeader->CalcNProduced();

  if (fContainer) {
    fContainer->AddHeader(fHeader);
  } else {
    gIlc->SetGenEventHeader(fHeader);	
  }
}

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

/* $Id: IlcGenEMCocktail.cxx 40702 2010-04-26 13:09:52Z morsch $ */

// Class to create cocktails for physics with electrons, di-electrons,
// and photons from the decay of the following sources:
// pizero, eta, rho, omega, etaprime, phi
// Kinematic distributions of the sources are taken from IlcGenEMlib.
// Decay channels can be selected via the method SetDecayMode.
// Particles can be generated flat in pT with weights according to the
// chosen pT distributions from IlcGenEMlib (weighting mode: kNonAnalog),
// or they are generated according to the pT distributions themselves
// (weighting mode: kAnalog)  
 
#include <TObjArray.h>
#include <TParticle.h>
#include <TF1.h>
#include <TVirtualMC.h>
#include <TPDGCode.h>
#include "IlcGenCocktailEventHeader.h"

#include "IlcGenCocktailEntry.h"
#include "IlcGenEMCocktail.h"
#include "IlcGenEMlib.h"
#include "IlcGenBox.h"
#include "IlcGenParam.h"
#include "IlcMC.h"
#include "IlcRun.h"
#include "IlcStack.h"
#include "IlcDecayer.h"
#include "IlcDecayerPythia.h"
#include "IlcLog.h"
#include "IlcGenCorrHF.h"

ClassImp(IlcGenEMCocktail)  
  
//________________________________________________________________________
IlcGenEMCocktail::IlcGenEMCocktail()
  :IlcGenCocktail(),
   fDecayer(0),
   fDecayMode(kAll),
   fWeightingMode(kNonAnalog),
   fNPart(1000),
   fYieldArray()
{
// Constructor

}

//_________________________________________________________________________
IlcGenEMCocktail::~IlcGenEMCocktail()
{
// Destructor

}

//_________________________________________________________________________
void IlcGenEMCocktail::CreateCocktail()
{
// create and add sources to the cocktail

  fDecayer->SetForceDecay(fDecayMode);
  fDecayer->ForceDecay();

// Set kinematic limits
  Double_t ptMin  = fPtMin;
  Double_t ptMax  = fPtMax;
  Double_t yMin   = fYMin;;
  Double_t yMax   = fYMax;;
  Double_t phiMin = fPhiMin*180./TMath::Pi();
  Double_t phiMax = fPhiMax*180./TMath::Pi();
  IlcInfo(Form("Ranges pT:%4.1f : %4.1f GeV/c, y:%4.2f : %4.2f, Phi:%5.1f : %5.1f degres",ptMin,ptMax,yMin,yMax,phiMin,phiMax));
  IlcInfo(Form("the parametrised sources uses the decay mode %d",fDecayMode));

// Create and add electron sources to the generator

// pizero
  IlcGenParam * genpizero=0;
  Char_t namePizero[10];    
  snprintf(namePizero,10, "Pizero");    
  genpizero = new IlcGenParam(fNPart, new IlcGenEMlib(), IlcGenEMlib::kPizero, "DUMMY");
  AddSource2Generator(namePizero,genpizero);
  TF1 *fPtPizero = genpizero->GetPt();
  fYieldArray[kGenPizero] = fPtPizero->Integral(fPtMin,fPtMax,(Double_t *) 0x0,1.e-6);
// eta  
  IlcGenParam * geneta=0;
  Char_t nameEta[10];    
  snprintf(nameEta,10, "Eta");    
  geneta = new IlcGenParam(fNPart, new IlcGenEMlib(), IlcGenEMlib::kEta, "DUMMY");
  AddSource2Generator(nameEta,geneta);
  TF1 *fPtEta = geneta->GetPt();
  fYieldArray[kGenEta] = fPtEta->Integral(fPtMin,fPtMax,(Double_t *) 0x0,1.e-6);
// rho  
  IlcGenParam * genrho=0;
  Char_t nameRho[10];    
  snprintf(nameRho,10, "Rho");    
  genrho = new IlcGenParam(fNPart, new IlcGenEMlib(), IlcGenEMlib::kRho, "DUMMY");
  AddSource2Generator(nameRho,genrho);
  TF1 *fPtRho = genrho->GetPt();
  fYieldArray[kGenRho] = fPtRho->Integral(fPtMin,fPtMax,(Double_t *) 0x0,1.e-6);
// omega
  IlcGenParam * genomega=0;
  Char_t nameOmega[10];    
  snprintf(nameOmega,10, "Omega");    
  genomega = new IlcGenParam(fNPart, new IlcGenEMlib(), IlcGenEMlib::kOmega, "DUMMY");
  AddSource2Generator(nameOmega,genomega);
  TF1 *fPtOmega = genomega->GetPt();
  fYieldArray[kGenOmega] = fPtOmega->Integral(fPtMin,fPtMax,(Double_t *) 0x0,1.e-6);
// etaprime
  IlcGenParam * genetaprime=0;
  Char_t nameEtaprime[10];    
  snprintf(nameEtaprime,10, "Etaprime");    
  genetaprime = new IlcGenParam(fNPart, new IlcGenEMlib(), IlcGenEMlib::kEtaprime, "DUMMY");
  AddSource2Generator(nameEtaprime,genetaprime);
  TF1 *fPtEtaprime = genetaprime->GetPt();
  fYieldArray[kGenEtaprime] = fPtEtaprime->Integral(fPtMin,fPtMax,(Double_t *) 0x0,1.e-6);
// phi  
  IlcGenParam * genphi=0;
  Char_t namePhi[10];    
  snprintf(namePhi, 10, "Phi");    
  genphi = new IlcGenParam(fNPart, new IlcGenEMlib(), IlcGenEMlib::kPhi, "DUMMY");
  AddSource2Generator(namePhi,genphi);
  TF1 *fPtPhi = genphi->GetPt();
  fYieldArray[kGenPhi] = fPtPhi->Integral(fPtMin,fPtMax,(Double_t *) 0x0,1.e-6);
}

//-------------------------------------------------------------------
void IlcGenEMCocktail::AddSource2Generator(Char_t* nameSource, 
					 IlcGenParam* const genSource)
{
// add sources to the cocktail
  Double_t phiMin = fPhiMin*180./TMath::Pi();
  Double_t phiMax = fPhiMax*180./TMath::Pi();

  genSource->SetPtRange(fPtMin, fPtMax);  
  genSource->SetYRange(fYMin, fYMax);
  genSource->SetPhiRange(phiMin, phiMax);
  genSource->SetWeighting(fWeightingMode);
  if (!gMC) genSource->SetDecayer(fDecayer);  
  genSource->Init();
    
  AddGenerator(genSource,nameSource,1.); // Adding Generator    
}

//-------------------------------------------------------------------
void IlcGenEMCocktail::Init()
{
// Initialisation
  TIter next(fEntries);
  IlcGenCocktailEntry *entry;
  if (fStack) {
    while((entry = (IlcGenCocktailEntry*)next())) {
      entry->Generator()->SetStack(fStack);
    }
  }
}

//_________________________________________________________________________
void IlcGenEMCocktail::Generate()
{
// Generate event 
  TIter next(fEntries);
  IlcGenCocktailEntry *entry = 0;
  IlcGenCocktailEntry *preventry = 0;
  IlcGenerator* gen = 0;

  if (fHeader) delete fHeader;
  fHeader = new IlcGenCocktailEventHeader("Electromagnetic Cocktail Header");

  const TObjArray *partArray = gIlc->GetMCApp()->Particles();
    
// Generate the vertex position used by all generators    
  if(fVertexSmear == kPerEvent) Vertex();

//Reseting stack
  IlcRunLoader * runloader = IlcRunLoader::Instance();
  if (runloader)
    if (runloader->Stack())
      runloader->Stack()->Clean();
  
// Loop over generators and generate events
  Int_t igen = 0;
  while((entry = (IlcGenCocktailEntry*)next())) {
    gen = entry->Generator();
    gen->SetVertex(fVertex.At(0), fVertex.At(1), fVertex.At(2));
    gen->SetTime(fTime);
    
    if (fNPart > 0) {
      igen++;	
      if (igen == 1) entry->SetFirst(0);		
      else  entry->SetFirst((partArray->GetEntriesFast())+1);
      gen->SetNumberParticles(fNPart);		
      gen->Generate();
      entry->SetLast(partArray->GetEntriesFast());
      preventry = entry;
    }
  }  
  next.Reset();

// Setting weights for proper absolute normalization
  Int_t iPart, iMother;
  Int_t pdgMother = 0;
  Double_t weight = 0.;
  Double_t dNdy = 0.;
  Int_t maxPart = partArray->GetEntriesFast();
  for(iPart=0; iPart<maxPart; iPart++){      
    TParticle *part = gIlc->GetMCApp()->Particle(iPart);
    iMother = part->GetFirstMother();
    TParticle *mother = 0;
    if (iMother>=0){
      mother = gIlc->GetMCApp()->Particle(iMother);
      pdgMother = mother->GetPdgCode();
    }
    else
      pdgMother = part->GetPdgCode();
    switch (pdgMother){
    case 111:
      dNdy = fYieldArray[kGenPizero];
      break;
    case 221:
      dNdy = fYieldArray[kGenEta];
      break;
    case 113:
      dNdy = fYieldArray[kGenRho];
      break;
    case 223:
      dNdy = fYieldArray[kGenOmega];
      break;
    case 331:
      dNdy = fYieldArray[kGenEtaprime];
      break;
    case 333:
      dNdy = fYieldArray[kGenPhi];
      break;
      
    default:
      dNdy = 0.;
    }
    
    weight = dNdy*part->GetWeight();
    part->SetWeight(weight);
  }	
  
  fHeader->SetNProduced(maxPart);


  TArrayF eventVertex;
  eventVertex.Set(3);
  for (Int_t j=0; j < 3; j++) eventVertex[j] = fVertex[j];
  
  fHeader->SetPrimaryVertex(eventVertex);
  fHeader->SetInteractionTime(fTime);

  gIlc->SetGenEventHeader(fHeader);
}

    

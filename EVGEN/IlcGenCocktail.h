#ifndef ILCGENCOCKTAIL_H
#define ILCGENCOCKTAIL_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcGenCocktail.h 54863 2012-02-28 12:10:29Z hristov $ */

// Container class for IlcGenerator through recursion.
// (Container is itself an IlcGenerator)
// Author: andreas.morsch@cern.ch 
//

#include "IlcGenerator.h"
#include <TArrayF.h>
#include <TList.h>

class IlcGenCocktailEntry;
class IlcGenCocktailEventHeader;
class TArrayF;
class TFormula;

class IlcGenCocktail : public IlcGenerator
{
 public:
    IlcGenCocktail();
     
    virtual ~IlcGenCocktail();
    virtual void Init();
    virtual void FinishRun();
    virtual void Generate();
    virtual void SetVertexSmear(VertexSmear_t smear);
    virtual void SetRandomise(Bool_t flag) {fSRandom = flag;}
    virtual void UsePerEventRates() {fUsePerEventRate  = kTRUE;}
	    
    //
    // Add a new generator to the list
    virtual void AddGenerator
      (IlcGenerator *Generator, const char* Name, Float_t RateExp, TFormula* formula = 0);
    virtual TList* Entries() {return fEntries;}
    // Iterators
    IlcGenCocktailEntry*  FirstGenerator();
    IlcGenCocktailEntry*  NextGenerator();
    void FirstGeneratorPair(IlcGenCocktailEntry*&e1, IlcGenCocktailEntry*&e2);
    void NextGeneratorPair (IlcGenCocktailEntry*&e1, IlcGenCocktailEntry*&e2);
    virtual void AddHeader(IlcGenEventHeader* header);
	    
 protected:
    Int_t fNGenerators;                 // Number of generators booked
    Float_t fTotalRate;                 // Total rate of the generators booked
    Bool_t fSRandom;                    // Flag to select random generator from list
    Bool_t fUsePerEventRate;            // Flag to generate the events according to the rate per event    
    TArrayF  fProb;                     // Probability of an event (if fSRandom == kTRUE)
    TList  *fEntries;                   // List of Generators
    TObjLink *flnk1;                    // ! Iterator for first generator
    TObjLink *flnk2;                    // ! Iterator for second generator
    IlcGenCocktailEventHeader* fHeader; // !Header container  
			   
//
 private:
    IlcGenCocktail(const IlcGenCocktail &cocktail);
    IlcGenCocktail & operator=(const IlcGenCocktail & rhs);

    ClassDef(IlcGenCocktail, 2) // Particle cocktail generator a la SHAKER
};

#endif






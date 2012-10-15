#ifndef ILCGENEMCOCKTAIL_H
#define ILCGENEMCOCKTAIL_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

//
// Class to create the cocktail for physics with electrons, di-electrons,
// and photons from the decay of the following sources:
// pizero, eta, rho, omega, etaprime, phi
//

#include "IlcGenCocktail.h"
#include "IlcGenParam.h"
#include "IlcDecayer.h"
#include "IlcPythia.h"

class IlcGenCocktailEntry;

class IlcGenEMCocktail : public IlcGenCocktail
{
 public:

    IlcGenEMCocktail();
    enum GeneratorCode { kGenPizero, kGenEta, kGenRho, kGenOmega, kGenEtaprime, kGenPhi, kGENs };    

    virtual ~IlcGenEMCocktail();    
    virtual void Init();
    virtual void CreateCocktail();
    virtual void Generate();    
    Float_t GetDecayMode()         const {return fDecayMode;}
    Float_t GetWeightingMode()     const {return fWeightingMode;}
    
    void    SetDecayer(IlcDecayer* const decayer){fDecayer = decayer;}
    void    SetDecayMode(Decay_t decay){ fDecayMode = decay;}
    void    SetWeightingMode(Weighting_t weight){ fWeightingMode = weight;}
    void    SetNPart(Int_t npart){ fNPart = npart;}
    
 protected:

    //
 private:
    IlcGenEMCocktail(const IlcGenEMCocktail &cocktail); 
    IlcGenEMCocktail & operator=(const IlcGenEMCocktail &cocktail); 

    void AddSource2Generator(Char_t *nameReso, IlcGenParam* const genReso);
    
    IlcDecayer* fDecayer;        // External decayer
    Decay_t fDecayMode;          // decay mode in which resonances are forced to decay, default: kAll
    Weighting_t fWeightingMode;  // weighting mode: kAnalog or kNonAnalog
    
    Int_t    fNPart;             // multiplicity of each source per event
    Double_t fYieldArray[kGENs]; // array of dN/dy for each source

    ClassDef(IlcGenEMCocktail,1)  //  cocktail for EM physics
};

#endif




#ifndef ILCGENCOCKTAILENTRY_H
#define ILCGENCOCKTAILENTRY_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcGenCocktailEntry.h 50152 2011-06-16 15:22:11Z morsch $ */

// Entry for AleGenCocktail container class. 
// See also comments there
// Author: andreas.morsch@cern.ch

#include <TNamed.h>

class IlcGenerator;
class TFormula;

class IlcGenCocktailEntry : public TNamed
{
 public:
    IlcGenCocktailEntry();
    IlcGenCocktailEntry(const IlcGenCocktailEntry &entry);
    IlcGenCocktailEntry
      (IlcGenerator* pGenerator, const char* Name, Float_t RateExp);
    ~IlcGenCocktailEntry(){;}
    IlcGenerator* Generator() {return fGenerator;}
    void SetGenerator(IlcGenerator* generator){fGenerator=generator;}
    void SetFormula(TFormula* formula) {fFormula = formula;}
    void SetFirst(Int_t first) {fFirst=first;}
    void SetLast (Int_t last ) {fLast =last;}
    Int_t GetFirst() const {return fFirst;}
    Int_t GetLast () const {return fLast;}
    Float_t Rate()   const {return fRate;}
    Int_t   Bin()    const {return Int_t(fRate);}
    void  PrintInfo() const;
    TFormula* Formula() const {return fFormula;}
    IlcGenCocktailEntry & operator =(const IlcGenCocktailEntry & rhs);
 protected:
    IlcGenerator *fGenerator;   // Pointer to generator
    Int_t fNGenerated;          // Number of primaries generated
    Int_t fFirst;               // First index in list of primaries
    Int_t fLast;                // Last index in list of primaries
    Float_t fRate;              // Rate per event
    Float_t fKineBias;          // Bias due to kinematic selecion
    Float_t fBias;              // Bias
    TFormula* fFormula;         // Formula to calculate number of signals per event    
    void Copy(TObject&) const;
 private:
    ClassDef(IlcGenCocktailEntry, 2) // Generator entry of IlcGenCocktail
};
#endif






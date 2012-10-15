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

/* $Id: IlcGenCocktailEntry.cxx 48118 2011-03-06 16:54:02Z morsch $ */

// Entry for IlcGenCocktail container class. 
// See also comments there.
// In addition to the pointer to the generator the class provides some 
// bookkeeping functionality (weights, links to particle list, ...)
// Author: andreas.morsch@cern.ch

#include <TString.h>
#include <TFormula.h>
#include "IlcGenCocktailEntry.h"
#include "IlcGenerator.h"


ClassImp(IlcGenCocktailEntry)

IlcGenCocktailEntry::IlcGenCocktailEntry():
    fGenerator(0),
    fNGenerated(0),
    fFirst(-1),
    fLast(-1),
    fRate(0),
    fKineBias(1),
    fBias(1),
    fFormula(0)
{
// Default constructor

}

IlcGenCocktailEntry:: IlcGenCocktailEntry
(IlcGenerator* pGenerator, const char* Name, Float_t RateExp) : 
  TNamed(Name, "Generator Cocktail Entry"),
  fGenerator(pGenerator),
  fNGenerated(0),
  fFirst(-1),
  fLast(-1),
  fRate(RateExp),
  fKineBias(1),
  fBias(1),
  fFormula(0)
{
    // Constructor
}

IlcGenCocktailEntry::IlcGenCocktailEntry(const IlcGenCocktailEntry &entry):
    TNamed(entry),
    fGenerator(0),
    fNGenerated(0),
    fFirst(-1),
    fLast(-1),
    fRate(0),
    fKineBias(1),
    fBias(1),
    fFormula(0)
{
// Dummy copy constructor
    entry.Copy(*this);
}


void IlcGenCocktailEntry::PrintInfo() const
{
// Print out information about generator entry
    printf("\n Generator: %s Generated Events: %d First: %d Last: %d",
	   (const char *) fName, fGenerator->NumberParticles(), fFirst, fLast);
}
 
IlcGenCocktailEntry& IlcGenCocktailEntry::operator
=(const  IlcGenCocktailEntry& rhs)
{
// Assignment operator
    rhs.Copy(*this);
    return (*this);
}

void IlcGenCocktailEntry::Copy(TObject&) const
{
    //
    // Copy 
    //
    Fatal("Copy","Not implemented!\n");
}

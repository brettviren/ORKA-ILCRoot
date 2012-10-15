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

/* $Id: IlcGenCocktailEventHeader.cxx 50615 2011-07-16 23:19:19Z hristov $ */

#include "IlcGenCocktailEventHeader.h"
#include "IlcGenEventHeader.h"
#include <TList.h>

ClassImp(IlcGenCocktailEventHeader)


IlcGenCocktailEventHeader::IlcGenCocktailEventHeader():
    fHeaders(0)
{
// Default Constructor
}

IlcGenCocktailEventHeader::IlcGenCocktailEventHeader(const char* name):IlcGenEventHeader(name),
    fHeaders(0)
{
// Constructor
}

IlcGenCocktailEventHeader::IlcGenCocktailEventHeader(const IlcGenCocktailEventHeader &header):
    IlcGenEventHeader(header),
    fHeaders(0)
{
// Copy Constructor
    header.Copy(*this);
}

IlcGenCocktailEventHeader::~IlcGenCocktailEventHeader()
{
// Constructor
    if (fHeaders) {
	fHeaders->Delete();
	delete fHeaders;
    }
}

void IlcGenCocktailEventHeader::AddHeader(const IlcGenEventHeader* header)
{
// Add a header to the list
    if (!fHeaders) fHeaders = new TList();
    fHeaders->Add(header->Clone());
}

Int_t IlcGenCocktailEventHeader::CalcNProduced()
{
    // Calculate the total number of produced and stored particles
    TIter next(fHeaders);
    IlcGenEventHeader *entry;
    //
    Int_t np = 0;
    while((entry = (IlcGenEventHeader*)next())) {np += (entry->NProduced());}
    fNProduced = np;
    return (np);
}

IlcGenCocktailEventHeader& IlcGenCocktailEventHeader::operator=(const  IlcGenCocktailEventHeader& rhs)
{
// Assignment operator
    rhs.Copy(*this); 
    return (*this);
}


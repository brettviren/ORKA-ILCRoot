#ifndef ILCGENCOCKTAILEVENTHEADER_H
#define ILCGENCOCKTAILEVENTHEADER_H

/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcGenCocktailEventHeader.h 50615 2011-07-16 23:19:19Z hristov $ */

#include "IlcGenEventHeader.h"


class IlcGenCocktailEventHeader : public IlcGenEventHeader
{
 public:
    IlcGenCocktailEventHeader();
    IlcGenCocktailEventHeader(const char* name);
    IlcGenCocktailEventHeader(const IlcGenCocktailEventHeader &header);
    virtual ~IlcGenCocktailEventHeader();
    virtual void AddHeader(const IlcGenEventHeader* header);
    virtual TList* GetHeaders() {return fHeaders;}
    virtual Int_t CalcNProduced();
    IlcGenCocktailEventHeader & operator=(const IlcGenCocktailEventHeader & rhs);
protected:
    TList  *fHeaders;     // List of Headers
    ClassDef(IlcGenCocktailEventHeader,1)  // Event header for Cocktail event
};

#endif

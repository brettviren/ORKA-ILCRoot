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

/* $Id: IlcTenderSupply.cxx 36639 2009-11-11 07:47:07Z agheata $ */
 
#include "IlcTender.h"
#include "IlcTenderSupply.h"

ClassImp(IlcTenderSupply)

//______________________________________________________________________________
IlcTenderSupply::IlcTenderSupply()
                :TNamed(),
                 fTender(NULL)
{
// Dummy constructor
}

//______________________________________________________________________________
IlcTenderSupply::IlcTenderSupply(const char* name, const IlcTender *tender)
                :TNamed(name, "ESD analysis tender car"),
                 fTender(tender)
{
// Default constructor
}

//______________________________________________________________________________
IlcTenderSupply::IlcTenderSupply(const IlcTenderSupply &other)
                :TNamed(other),
                 fTender(other.fTender)
                 
{
// Copy constructor
}

//______________________________________________________________________________
IlcTenderSupply::~IlcTenderSupply()
{
// Destructor
}

//______________________________________________________________________________
IlcTenderSupply& IlcTenderSupply::operator=(const IlcTenderSupply &other)
{
// Assignment
   if (&other == this) return *this;
   TNamed::operator=(other);
   fTender = other.fTender;
   return *this;
}

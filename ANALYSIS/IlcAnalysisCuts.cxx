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

/* $Id: IlcAnalysisCuts.cxx 54863 2012-02-28 12:10:29Z hristov $ */
 
// Base class for analysis cuts
// Author Andreas Morsch
// andreas.morsch@cern.ch

#include <TObject.h>
#include "IlcAnalysisCuts.h"


ClassImp(IlcAnalysisCuts)


////////////////////////////////////////////////////////////////////////

IlcAnalysisCuts::IlcAnalysisCuts():
    IlcVCuts(), fFilterMask(0), fSelected(kFALSE)
{
  // Default constructor
}

IlcAnalysisCuts::IlcAnalysisCuts(const char* name, const char* title):
    IlcVCuts(name, title), fFilterMask(0), fSelected(kFALSE)
{
  // Constructor
}

IlcAnalysisCuts::IlcAnalysisCuts(const IlcAnalysisCuts& obj):
    IlcVCuts(obj), fFilterMask(obj.fFilterMask), fSelected(obj.fSelected)
{
}

IlcAnalysisCuts& IlcAnalysisCuts::operator=(const IlcAnalysisCuts& obj)
{
  // Assignment operator
  if ( this != &obj ) { 
    IlcVCuts::operator=(obj);
    fFilterMask = obj.fFilterMask;
    fSelected = obj.fSelected;
  }
  return *this;
}


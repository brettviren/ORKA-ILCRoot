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
// Base class for selection classes for the correction framework. 
// Inherits from IlcAnalysisCuts. It includes additional methods 
// to export QA histograms & study the cut statistics & correlations 
// through the bitmap of the cuts embedded in each class, if needed
// Author S.Arcelli
// silvia.Arcelli@cern.ch

#include "IlcCFCutBase.h"


ClassImp(IlcCFCutBase)


//___________________________________________________________________________
IlcCFCutBase::IlcCFCutBase():
  IlcAnalysisCuts(),
  fIsQAOn(kFALSE)
{
  //
  // Default constructor
  //
}

//___________________________________________________________________________
IlcCFCutBase::IlcCFCutBase(const char* name, const char* title):
  IlcAnalysisCuts(name, title),
  fIsQAOn(kFALSE)
{
  //
  // Constructor
  //
}

//___________________________________________________________________________
IlcCFCutBase::IlcCFCutBase(const IlcCFCutBase& obj):
  IlcAnalysisCuts(obj),
  fIsQAOn(obj.fIsQAOn)
{
  //
  // Copy Constructor
  //
}

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

/* $Id$ */

//-------------------------------------------------------------------------
//     Event cuts base class
//     Author: Markus Oldenburg, CERN
//-------------------------------------------------------------------------

#include "IlcVCuts.h"

ClassImp(IlcVCuts)

//______________________________________________________________________________
IlcVCuts::IlcVCuts() : 
  TNamed("Cuts","") { } // default constructor 

IlcVCuts::IlcVCuts(const char* name, const char* title) :
  TNamed(name, title) { } 
//______________________________________________________________________________
IlcVCuts::IlcVCuts(const IlcVCuts& cuts) :
  TNamed(cuts) {} // Copy constructor

//______________________________________________________________________________
IlcVCuts& IlcVCuts::operator=(const IlcVCuts& cuts)
{
  // Assignment operator
  if(this!=&cuts) { 
    TNamed::operator=(cuts);
  }
  return *this;
}

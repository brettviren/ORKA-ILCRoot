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
 
// Base class for event pool.
// This class is needed by the AnalysisManager to steer a mixing analysis.
// Author Andreas Morsch
// andreas.morsch@cern.ch

#include "TChain.h"

#include "IlcVEventPool.h"

ClassImp(IlcVEventPool)


////////////////////////////////////////////////////////////////////////

IlcVEventPool::IlcVEventPool():
    TNamed(),
    fChain(0)
{
  // Default constructor
}

IlcVEventPool::IlcVEventPool(const char* name, const char* title):
    TNamed(name, title), 
    fChain()
{
  // Constructor
}


IlcVEventPool::IlcVEventPool(const IlcVEventPool& obj):
    TNamed(obj),
    fChain(obj.fChain)
{
  //
  // Copy constructor
  //
}

IlcVEventPool& IlcVEventPool::operator=(const IlcVEventPool& other)
{
  //
  // Assignment operator
  //
  if(this != &other) {
    TNamed::operator=(other);
    delete fChain;
    fChain = other.fChain;
  }
  return *this;
}

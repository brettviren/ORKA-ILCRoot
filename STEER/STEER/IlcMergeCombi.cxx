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

/* $Id: IlcMergeCombi.cxx 50615 2011-07-16 23:19:19Z hristov $ */

//----------------------------------------------------------------------
// IlcMergeCombi.cxx
// class to
// return combinations of input event numbers
//----------------------------------------------------------------------

#include "IlcMergeCombi.h"

ClassImp(IlcMergeCombi)

//_______________________________________________________________________
IlcMergeCombi::IlcMergeCombi():
  fDim(1),
  fSperb(1),
  fCounter(0)
{
  //
  // default ctor
  //
}

//_______________________________________________________________________
IlcMergeCombi::IlcMergeCombi(Int_t dim, Int_t sperb):
  fDim(dim),
  fSperb(sperb),
  fCounter(0)
{
  //
  // Standard ctor
  //
}

//_______________________________________________________________________
IlcMergeCombi::~IlcMergeCombi()
{
  // default dtor
}

//_______________________________________________________________________
Bool_t IlcMergeCombi::Combination(Int_t /* evNumber */ [], Int_t delta[])
{
  //
  // Combination method
  //
  delta[0] = 1;
  for (Int_t i=1; i<fDim; i++) {
    if (fCounter%fSperb == 0) {
      delta[i] = 1;
    } else {
      delta[i] = 0;
    }
    fCounter++;
  }      
  return kTRUE;
}

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

//_________________________________________________________________________
// Version of IlcRSTACKv1 which keeps all hits in TreeH
// AddHit, StepManager,and FinishEvent are redefined 
//                  
//*-- Author: Gines MARTINEZ (SUBATECH)
//*-- Modified Nov. 22 2000 by Dmitri Peressounko
// All hits are stored.
// Note, that primaries will not be assigned to digits:
// because of tiny energy deposition at each step.
//  

// --- ROOT system ---
#include "TClonesArray.h"

// --- Standard library ---


// --- IlcRoot header files ---

#include "IlcRSTACKv2.h"
#include "IlcRSTACKHit.h"

ClassImp(IlcRSTACKv2)

//____________________________________________________________________________
IlcRSTACKv2::IlcRSTACKv2()
{
  // default ctor

}

//____________________________________________________________________________
IlcRSTACKv2::IlcRSTACKv2(const char *name, const char *title):
IlcRSTACKv1(name,title)
{
  // ctor
}
//__________________________________________________________________________
IlcRSTACKv2::~IlcRSTACKv2()
{
  // dtor
}

//____________________________________________________________________________
IlcRSTACKv2 & IlcRSTACKv2::operator = (const IlcRSTACKv2 &)
{
  Fatal("operator =", "not implemented");
  return *this;
}

//____________________________________________________________________________
void IlcRSTACKv2::AddHit(Int_t shunt, Int_t primary, Int_t Id, Float_t * hits)
{
  // Add a hit to the hit list.

  IlcRSTACKHit *newHit ;

  newHit = new IlcRSTACKHit(shunt, primary, Id, hits) ;

  new((*fHits)[fNhits]) IlcRSTACKHit(*newHit) ;    
  fNhits++ ;

  delete newHit;

}



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
/* $Id: IlcPVECv2.cxx 53186 2011-11-25 14:28:01Z kharlov $ */

/* History of cvs commits:
 *
 * $Log$
 * Revision 1.24  2005/05/28 14:19:05  schutz
 * Compilation warnings fixed by T.P.
 *
 */

//_________________________________________________________________________
// Version of IlcPVECv1 which keeps all hits in TreeH
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

#include "IlcPVECv2.h"
#include "IlcPVECHit.h"

ClassImp(IlcPVECv2)

//____________________________________________________________________________
IlcPVECv2::IlcPVECv2()
{
  // default ctor

}

//____________________________________________________________________________
IlcPVECv2::IlcPVECv2(const char *name, const char *title):
IlcPVECv1(name,title)
{
  // ctor
}
//__________________________________________________________________________
IlcPVECv2::~IlcPVECv2()
{
  // dtor
}

//____________________________________________________________________________
IlcPVECv2 & IlcPVECv2::operator = (const IlcPVECv2 &)
{
  Fatal("operator =", "not implemented");
  return *this;
}

//____________________________________________________________________________
void IlcPVECv2::AddHit(Int_t shunt, Int_t primary, Int_t Id, Float_t * hits)
{
  // Add a hit to the hit list.

  IlcPVECHit *newHit ;

  newHit = new IlcPVECHit(shunt, primary, Id, hits) ;

  new((*fHits)[fNhits]) IlcPVECHit(*newHit) ;    
  fNhits++ ;

  delete newHit;

}



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

/* $Id: IlcRSTACKCPVDigit.cxx 15036 2006-09-13 07:31:01Z kharlov $ */

////////////////////////////////////////////////
//  Digit class for set CPV                   //
//                                            //
//  Author: Yuri Kharlov, IHEP, Protvino      //
//  e-mail: Yuri.Kharlov@cern.ch              //
//  Last modified: 2 November 2000            //
////////////////////////////////////////////////
 
// --- ilcroot header files ---
#include "IlcRSTACKCPVDigit.h"

ClassImp(IlcRSTACKCPVDigit)

//______________________________________________________________________________
IlcRSTACKCPVDigit::IlcRSTACKCPVDigit():
  fXpad(0),
  fYpad(0),
  fQpad(0.)
{
  //
  // Create a CPV digit object
  //
}


//______________________________________________________________________________
IlcRSTACKCPVDigit::IlcRSTACKCPVDigit(Int_t x, Int_t y, Float_t q):
  fXpad(x),
  fYpad(y),
  fQpad(q)
{
  //
  // Create a CPV digit object
  //
}

//______________________________________________________________________________

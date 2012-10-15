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

/* $Id: IlcGenGeVSimEventHeader.cxx 14845 2006-08-15 12:25:37Z morsch $ */


//
// Event header for GeVSim event generator
// support event plane and elliptic flow
// in next release will suport full differential 
// directed and elliptic flow
//
// Sylwester Radomski, GSI
// mail: S.Radomski@gsi
// 31 Oct, 2002
//
//

#include "IlcGenGeVSimEventHeader.h"


ClassImp(IlcGenGeVSimEventHeader)


////////////////////////////////////////////////////////////////////////////////

IlcGenGeVSimEventHeader::IlcGenGeVSimEventHeader()
  :IlcGenEventHeader(),
   fEventPlane(0),
   fEllipticFlow(0)
{
  //
  // Default Constructor 
  //
}


////////////////////////////////////////////////////////////////////////////////

IlcGenGeVSimEventHeader::IlcGenGeVSimEventHeader(const char *name)
  :IlcGenEventHeader(name),
   fEventPlane(0),
   fEllipticFlow(0)
 {
  //
  // Standard constructor
  //
}

////////////////////////////////////////////////////////////////////////////////

void IlcGenGeVSimEventHeader::SetEventPlane(Float_t psi) {
  //
  // Psi in rad.
  //

  fEventPlane = psi;
}

////////////////////////////////////////////////////////////////////////////////

void IlcGenGeVSimEventHeader::SetEllipticFlow(Float_t v2) {
  //
  // Set elliptic flow
  //

  fEllipticFlow = v2;
}

////////////////////////////////////////////////////////////////////////////////

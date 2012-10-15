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

/* $Id: IlcGenHydjetEventHeader.cxx 18049 2007-04-20 06:38:54Z hristov $ */

// Event Header for Hydjet generator
// Output generator parameters are accessable
// for the user through this interface.
// Author: Rafael Diaz Valdes
// (rafael.diaz.valdes@cern.ch)
//

#include "IlcGenHydjetEventHeader.h"
ClassImp(IlcGenHydjetEventHeader)

IlcGenHydjetEventHeader::IlcGenHydjetEventHeader():
    fNjet(0),
    fImpactParam(0),
    fNbcol(0),
    fNpart(0),
    fNpyt(0),
    fNhyd(0)
{
    // Constructor
}

IlcGenHydjetEventHeader::IlcGenHydjetEventHeader(const char* name):
    IlcGenEventHeader(name),
    fNjet(0),
    fImpactParam(0),
    fNbcol(0),
    fNpart(0),
    fNpyt(0),
    fNhyd(0)
{
    // Copy Constructor
}

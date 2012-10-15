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

//---------------------------------------------------------------------
// Jet ESD Reader Header
// Header for the ESD reader in the jet analysis
// Author: Mercedes Lopez Noriega (mercedes.lopez.noriega@cern.ch)
//---------------------------------------------------------------------
 
#include "IlcJetESDReaderHeader.h"

ClassImp(IlcJetESDReaderHeader)

//____________________________________________________________________________
IlcJetESDReaderHeader::IlcJetESDReaderHeader():
  IlcJetReaderHeader("IlcJetESDReaderHeader"), 
  fDCA(0),
  fTLength(0),
  fReadSignalOnly(kFALSE),
  fReadBkgdOnly(kFALSE),
  fNesd(0)
{
  // Default constructor
  
}

//____________________________________________________________________________
IlcJetESDReaderHeader::~IlcJetESDReaderHeader()
{
  // destructor
}

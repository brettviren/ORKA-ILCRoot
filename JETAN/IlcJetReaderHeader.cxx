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
 
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
// base class for Jet Reader Header 
// Author: jgcn@mda.cinvestav.mx
//-------------------------------------------------------------------------

#include <TMath.h>

#include "IlcJetReaderHeader.h"

ClassImp(IlcJetReaderHeader)

////////////////////////////////////////////////////////////////////////

IlcJetReaderHeader::IlcJetReaderHeader():  
 TNamed("IlcJetReaderHeader", "Jet Reader Header"),
 fFirst(0),
 fLast(-1),
 fOption(0),
 fCluster(0),
 fDebug(0),
 fDZ(0),
 fSignalPerBg(0),
 fFiducialEtaMin(-0.9),
 fFiducialEtaMax(0.9),
 fFiducialPhiMin(0.),
 fFiducialPhiMax(2*TMath::Pi()),
 fPtCut(2.0),
 fComment("No comment"),
 fDir(""),
 fBgDir(""),
 fPattern(""),
 fMatricesEMCAL("survey11"),
 fGeomEMCAL("EMCAL_COMPLETEV1"),
 fMyOADBfile("")
{
  // Default constructor
}

////////////////////////////////////////////////////////////////////////

IlcJetReaderHeader::IlcJetReaderHeader(const char * name):  
 TNamed(name, "Jet Reader Header"),
 fFirst(0),
 fLast(-1),
 fOption(0),
 fCluster(0),
 fDebug(0),
 fDZ(0),
 fSignalPerBg(0),
 fFiducialEtaMin(-0.9),
 fFiducialEtaMax(0.9),
 fFiducialPhiMin(0.),
 fFiducialPhiMax(2*TMath::Pi()),
 fPtCut(2.0),
 fComment("No comment"),
 fDir(""),
 fBgDir(""),
 fPattern(""),
 fMatricesEMCAL("survey11"),
 fGeomEMCAL("EMCAL_COMPLETEV1"),
 fMyOADBfile("")
{
  // Constructor
}

////////////////////////////////////////////////////////////////////////

IlcJetReaderHeader::~IlcJetReaderHeader()
{
  // destructor

}

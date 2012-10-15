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
// base class for Jet Reader Header 
// Author: jgcn@mda.cinvestav.mx
//-------------------------------------------------------------------------

#include <TMath.h>

#include "IlcJetReaderHeader.h"
// #include "IlcPVBARRecoUtils.h"

ClassImp(IlcJetReaderHeader)

////////////////////////////////////////////////////////////////////////

IlcJetReaderHeader::IlcJetReaderHeader():  
  TNamed("IlcJetReaderHeader", "Jet Reader Header"),
  fOption(0),
  fCluster(0),
  fDebug(0),
  fFiducialEtaMin(-0.9),
  fFiducialEtaMax(0.9),
  fFiducialPhiMin(0.),
  fFiducialPhiMax(2*TMath::Pi()),
  fPtCut(2.0),
  fEtCellCut(0.0),
  fComment("No comment"),
  fDir(""),
  fMatricesEMCAL("survey11"),
  fGeomEMCAL("EMCAL_COMPLETEV1"),
  fMyOADBfile(""),
  fTestFilterMask(0),
  fFilterType(0),
  fReadSignalOnly(kFALSE),
  fReadBkgdOnly(kFALSE),
  fDataType(""),
  fIsHighMult(kFALSE)
{
  // Default constructor
}

//-----------------------------------------------------------------------
IlcJetReaderHeader::IlcJetReaderHeader(Int_t det):
  TNamed("IlcJetReaderHeader", "Jet Reader Header"),
  fOption(det),
  fCluster(0),
  fDebug(0),
  fFiducialEtaMin(-0.9),
  fFiducialEtaMax(0.9),
  fFiducialPhiMin(0.),
  fFiducialPhiMax(2*TMath::Pi()),
  fPtCut(2.0),
  fEtCellCut(0.0),
  fComment("No comment"),
  fDir(""),
  fMatricesEMCAL("survey11"),
  fGeomEMCAL("EMCAL_COMPLETEV1"),
  fMyOADBfile(""),
  fTestFilterMask(0),
  fFilterType(0),
  fReadSignalOnly(kFALSE),
  fReadBkgdOnly(kFALSE),
  fDataType(""),
  fIsHighMult(kFALSE)
{
  // Constructor 

}

//-----------------------------------------------------------------------
IlcJetReaderHeader::IlcJetReaderHeader(const char * name):
  TNamed(name, "Jet Reader Header"),
  fOption(0),
  fCluster(0),
  fDebug(0),
  fFiducialEtaMin(-0.9),
  fFiducialEtaMax(0.9),
  fFiducialPhiMin(0.),
  fFiducialPhiMax(2*TMath::Pi()),
  fPtCut(2.0),
  fEtCellCut(0.0),
  fComment("No comment"),
  fDir(""),
  fMatricesEMCAL("survey11"),
  fGeomEMCAL("EMCAL_COMPLETEV1"),
  fMyOADBfile(""),
  fTestFilterMask(0),
  fFilterType(0),
  fReadSignalOnly(kFALSE),
  fReadBkgdOnly(kFALSE),
  fDataType(""),
  fIsHighMult(kFALSE)
{
  // Constructor 2

}


//-----------------------------------------------------------------------
IlcJetReaderHeader::~IlcJetReaderHeader()
{
  // destructor

}

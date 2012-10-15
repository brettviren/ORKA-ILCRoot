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

/* $Id: IlcRndm.cxx 50615 2011-07-16 23:19:19Z hristov $ */

///////////////////////////////////////////////////////////////////////////////
// Random number class for IlcRoot
// This class allows to have different 
// random number generator for different
// elements of IlcRoot                                                                          //
// It also allows saving and retrieving of the random number seeds
///////////////////////////////////////////////////////////////////////////////

#include <TClass.h>
#include <TFile.h>
#include <TError.h>
#include <TRandom3.h>
#include <TSystem.h>

#include "IlcRndm.h"
#include "IlcLog.h"

ClassImp(IlcRndm)

//_______________________________________________________________________
IlcRndm::IlcRndm():
  fRandom(gRandom)
{
  // 
  // Default ctor
  //
}

//_______________________________________________________________________
IlcRndm::IlcRndm(const IlcRndm& rn):
  fRandom(gRandom)
{
  //
  // Copy constructor
  //
  rn.Copy(*this);
}

//_______________________________________________________________________
void IlcRndm::Copy(IlcRndm&) const
{
  IlcFatalClass("Not implemented");
}


//_____________________________________________________________________________
void IlcRndm::Rndm(Float_t* array, Int_t size) const
{
  //
  // Return an array of n random numbers uniformly distributed 
  // between 0 and 1 not included
  //
  for(Int_t i=0; i<size; i++) 
#ifdef CKNONE
    array[i]=fRandom->Rndm();
#else
    do array[i]=fRandom->Rndm(); while(0>=array[i] || array[i]>=1);
#endif
}

//_____________________________________________________________________________
void IlcRndm::ReadRandom(const char *filename)
{
  //
  // Reads saved random generator status from filename
  //
  char *fntmp = gSystem->ExpandPathName(filename);
  TFile *file = new TFile(fntmp,"r");
  delete [] fntmp;
  if(!file) {
    IlcErrorClass(Form("Could not open file %s",filename));
  } else {
    if(!fRandom) fRandom = new TRandom();
    fRandom->Read("Random");
    file->Close();
    delete file;
  }
}

//_____________________________________________________________________________
void IlcRndm::WriteRandom(const char *filename) const
{
  //
  // Writes random generator status to filename
  //
  char *fntmp = gSystem->ExpandPathName(filename);
  TFile *file = new TFile(fntmp,"new");
  delete [] fntmp;
  if(!file) {
    IlcErrorClass(Form("Could not open file %s",filename));
  } else {
    fRandom->Write();
    file->Close();
    delete file;
  }
}

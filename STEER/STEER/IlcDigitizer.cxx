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

/* $Id: IlcDigitizer.cxx 52261 2011-10-23 15:46:57Z hristov $ */

//----------------------------------------------------------------------
//  Base Class for Detector specific Merging/Digitization   
//  Collaborates with IlcDigitizationInput class                
//  Author: Jiri Chudoba (CERN)
//----------------------------------------------------------------------

// system includes
#include <Riostream.h>

// ROOT includes

// IlcROOT includes
#include "IlcLog.h"
#include "IlcDigitizer.h"
#include "IlcDigitizationInput.h"

ClassImp(IlcDigitizer)

//_______________________________________________________________________
IlcDigitizer::IlcDigitizer(const Text_t* name, const Text_t* title):
  TNamed(name,title),
  fDigInput(0)
{
  //
  // Default ctor with name and title
  //
}

//_______________________________________________________________________
IlcDigitizer::IlcDigitizer(const IlcDigitizer &dig):
  TNamed(dig.GetName(),dig.GetTitle()),
  fDigInput(0)
{
  //
  // Copy ctor with
  //
  dig.Copy(*this);
}

//_______________________________________________________________________
void IlcDigitizer::Copy(TObject &) const
{
  IlcFatal("Not yet implemented");
}

//_______________________________________________________________________
IlcDigitizer::IlcDigitizer(IlcDigitizationInput *digInput, 
                           const Text_t* name, const Text_t* title):
  TNamed(name,title),
  fDigInput(digInput)
{
  //
  // ctor with name and title
}

//_______________________________________________________________________
IlcDigitizer::~IlcDigitizer() 
{
}

//_______________________________________________________________________
Int_t IlcDigitizer::GetNInputStreams() const
{
  //
  // return number of input streams
  //
  Int_t nInputStreams = 0 ;
  if (fDigInput) nInputStreams = fDigInput->GetNinputs() ;
  return nInputStreams ; 
}

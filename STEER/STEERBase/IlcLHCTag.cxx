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

/* $Id: IlcLHCTag.cxx 50615 2011-07-16 23:19:19Z hristov $ */

//-----------------------------------------------------------------
//           Implementation of the LHCTag class
//   This is the class to deal with the tags in the LHC level
//   Origin: Panos Christakoglou, UOA-CERN, Panos.Christakoglou@cern.ch
//-----------------------------------------------------------------

#include "IlcLHCTag.h"

ClassImp(IlcLHCTag)

//___________________________________________________________________________
  IlcLHCTag::IlcLHCTag() : 
    TObject(),  
    fLHCState(0),
    fLHCLuminosity(-10.),
    fNBunches(0),
    fFillingScheme(),
    fFillNo(-1),
    fBeamEnergy(-1.0),
    fBunchIntensity(0.0)
{
  // IlcLHCTag default constructor
}

IlcLHCTag::IlcLHCTag(const IlcLHCTag &tag):
  TObject(tag),
  fLHCState(tag.fLHCState),
  fLHCLuminosity(tag.fLHCLuminosity),
  fNBunches(tag.fNBunches),
  fFillingScheme(tag.fFillingScheme),
  fFillNo(tag.fFillNo),
  fBeamEnergy(tag.fBeamEnergy),
  fBunchIntensity(tag.fBunchIntensity)
{
}

IlcLHCTag &IlcLHCTag::operator=(const IlcLHCTag &tag)
{
  if (this != &tag) {
    TObject::operator=(tag);

    TString state(tag.GetLHCState());
    SetLHCState(state);
    SetLuminosity(tag.GetLuminosity());
    SetNBunches(tag.GetNBunches());
    SetFillingScheme(tag.GetFillingScheme());
    SetFillNo(tag.GetFillNo());
    SetBeamEnergy(tag.GetBeamEnergy());
    SetBunchIntensity(tag.GetBunchIntensity());
  }

  return *this;
}

void IlcLHCTag::UpdateFromRunTable(IlcLHCTag &tag)
{
  SetNBunches(tag.GetNBunches());
  SetFillingScheme(tag.GetFillingScheme());
  SetFillNo(tag.GetFillNo());
  SetBeamEnergy(tag.GetBeamEnergy());
  SetBunchIntensity(tag.GetBunchIntensity());  
}

//___________________________________________________________________________
IlcLHCTag::~IlcLHCTag() {
  // IlcEventTag destructor
}

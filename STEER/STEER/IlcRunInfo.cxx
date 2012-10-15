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

//////////////////////////////////////////////////////////////////////////////
//                          Class IlcRunInfo                                //
//   Container class for all the information related to LHCstate, run type, //
//   active detectors, beam energy etc.                                     //
//   It is used together with the IlcEventInfo in order to provide          //
//   the IlcRecoParam object with                                           //
//   the necessary information so that it can decide which instance of      //
//   IlcDetectorRecoParam objects to use in reconstruction one particular   //
//   event.                                                                 //
//                                                                          //
//   cvetan.cheshkov@cern.ch 12/06/2008                                     //
//////////////////////////////////////////////////////////////////////////////

#include "IlcRunInfo.h"

ClassImp(IlcRunInfo)

//______________________________________________________________________________
IlcRunInfo::IlcRunInfo():
  TObject(),
  fLHCState("UNKNOWN"),
  fBeamType("UNKNOWN"),
  fBeamEnergy(0),
  fRunType("UNKNOWN"),
  fActiveDetectors(0)
{
  // default constructor
  // ...
}

//______________________________________________________________________________
IlcRunInfo::IlcRunInfo(const char *lhcState,
		       const char *beamType,
		       Float_t beamEnergy,
		       const char *runType,
		       UInt_t activeDetectors):
  TObject(),
  fLHCState(lhcState),
  fBeamType(beamType),
  fBeamEnergy(beamEnergy),
  fRunType(runType),
  fActiveDetectors(activeDetectors)
{
  // constructor
  // ...
}

//______________________________________________________________________________
IlcRunInfo::IlcRunInfo(const IlcRunInfo &evInfo):
  TObject(evInfo),
  fLHCState(evInfo.fLHCState),
  fBeamType(evInfo.fBeamType),
  fBeamEnergy(evInfo.fBeamEnergy),
  fRunType(evInfo.fRunType),
  fActiveDetectors(evInfo.fActiveDetectors)
{
  // Copy constructor
  // ...
}

//_____________________________________________________________________________
IlcRunInfo &IlcRunInfo::operator =(const IlcRunInfo& evInfo)
{
  // assignment operator
  // ...
  if(this==&evInfo) return *this;
  ((TObject *)this)->operator=(evInfo);

  fLHCState = evInfo.fLHCState;
  fBeamType = evInfo.fBeamType;
  fBeamEnergy = evInfo.fBeamEnergy;
  fRunType = evInfo.fRunType;
  fActiveDetectors = evInfo.fActiveDetectors;

  return *this;
}

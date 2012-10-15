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
//                          Class IlcEventInfo                              //
//   Container class for all the information related to                     //
//   event type, trigger mask and trigger clusters.                         //
//   It is used together with IlcRunInfo in order to provide the detector's //
//   IlcRecoParam object with                                               //
//   the necessary information so that it can decide which instance of      //
//   IlcDetectorRecoParam objects to use in reconstruction one particular   //
//   event.                                                                 //
//                                                                          //
//   cvetan.cheshkov@cern.ch 12/06/2008                                     //
//////////////////////////////////////////////////////////////////////////////

#include "IlcEventInfo.h"

ClassImp(IlcEventInfo)

//______________________________________________________________________________
IlcEventInfo::IlcEventInfo():
  TObject(),
  fEventType(0),
  fTriggerClasses(""),
  fTriggerMask(0),
  fTriggerCluster(""),
  fHLTDecision("")
{
  // default constructor
  // ...
}

//______________________________________________________________________________
IlcEventInfo::IlcEventInfo(UInt_t evType,
			   const char *classes,
			   ULong64_t mask,
			   const char *cluster,
			   const char *decision):
  TObject(),
  fEventType(evType),
  fTriggerClasses(classes),
  fTriggerMask(mask),
  fTriggerCluster(cluster),
  fHLTDecision(decision)
{
  // constructor
  // ...
}

//______________________________________________________________________________
IlcEventInfo::IlcEventInfo(const IlcEventInfo &evInfo):
  TObject(evInfo),
  fEventType(evInfo.fEventType),
  fTriggerClasses(evInfo.fTriggerClasses),
  fTriggerMask(evInfo.fTriggerMask),
  fTriggerCluster(evInfo.fTriggerCluster),
  fHLTDecision(evInfo.fHLTDecision)
{
  // Copy constructor
  // ...
}

//_____________________________________________________________________________
IlcEventInfo &IlcEventInfo::operator =(const IlcEventInfo& evInfo)
{
  // assignment operator
  // ...
  if(this==&evInfo) return *this;
  ((TObject *)this)->operator=(evInfo);

  fEventType = evInfo.fEventType;
  fTriggerClasses = evInfo.fTriggerClasses;
  fTriggerMask = evInfo.fTriggerMask; 
  fTriggerCluster = evInfo.fTriggerCluster;
  fHLTDecision = evInfo.fHLTDecision;

  return *this;
}

//______________________________________________________________________________
void IlcEventInfo::Reset()
{
  // Reset the contents
  // ...
  fEventType = 0;
  fTriggerClasses = "";
  fTriggerMask = 0;
  fTriggerCluster = "";
  fHLTDecision = "";
}

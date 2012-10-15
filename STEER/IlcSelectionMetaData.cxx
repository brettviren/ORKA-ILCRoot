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

/* $Id: IlcSelectionMetaData.cxx 11658 2005-05-20 07:05:03Z hristov $ */

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// subsample of the object metadata, used to retrieve                        //
// a stored database object:                                                 // 
// name="Detector/DBType/DetSpecType", run validity, version                 //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////


#include <TSystem.h>

#include "IlcSelectionMetaData.h"
#include "IlcMetaData.h"
#include "IlcLog.h"


ClassImp(IlcSelectionMetaData)


//_____________________________________________________________________________
IlcSelectionMetaData::IlcSelectionMetaData() :
  IlcMetaData()
{
// default constructor
// the default values mean no selection
}

//_____________________________________________________________________________
IlcSelectionMetaData::IlcSelectionMetaData(const char* name, Int_t firstRun, Int_t lastRun, Int_t Version) :
  IlcMetaData(name, firstRun, lastRun, Version)
{
// constructor
}

//_____________________________________________________________________________
IlcSelectionMetaData::IlcSelectionMetaData(const IlcSelectionMetaData& entry) :
  IlcMetaData(entry)
{
// copy constructor
}

//_____________________________________________________________________________
IlcSelectionMetaData::IlcSelectionMetaData(const IlcMetaData& entry) :
  IlcMetaData(entry)
{
// constructor of IlcSelectionMetaData from IlcMetaData
}


//_____________________________________________________________________________
IlcSelectionMetaData& IlcSelectionMetaData::operator = (const IlcSelectionMetaData& entry)
{
// assignment operator
  fName = entry.fName,
  fFirstRun = entry.fFirstRun;
  fLastRun = entry.fLastRun;
  fVersion = entry.fVersion;
  return *this;
}


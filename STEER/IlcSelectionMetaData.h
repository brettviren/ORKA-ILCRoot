#ifndef ILCSELECTIONMETADATA_H
#define ILCSELECTIONMETADATA_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcSelectionMetaData.h 11658 2005-05-20 07:05:03Z hristov $ */

///
/// subsample of the object metadata, used to retrieve a stored database object: name="Detector/DBType/DetSpecType", run validity, version
///

#include <TString.h>

#include "IlcMetaData.h"


class IlcSelectionMetaData: public IlcMetaData {
public:
  IlcSelectionMetaData();	// default constructor
  IlcSelectionMetaData(const char* name, Int_t firstRun = -1, Int_t lastRun = -1, Int_t version = -1);	// constructor
  virtual ~IlcSelectionMetaData() {};	// destructor

  IlcSelectionMetaData(const IlcSelectionMetaData& entry);	// copy constructor
  IlcSelectionMetaData(const IlcMetaData& entry);	// selection metadata from base class
  IlcSelectionMetaData& operator = (const IlcSelectionMetaData& entry);	// assignment operator
  
ClassDef(IlcSelectionMetaData, 1)   // metadata used to retrieve a DB object
};

#endif

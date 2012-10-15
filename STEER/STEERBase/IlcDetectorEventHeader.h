#ifndef ILCDETECTOREVENTHEADER_H
#define ILCDETECTOREVENTHEADER_H

/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcDetectorEventHeader.h 50615 2011-07-16 23:19:19Z hristov $ */

//---------------------------------------------------------------------
// Event header base class for detetors 
// Stores detector specific information
// Author: andreas.morsch@cern.ch
//---------------------------------------------------------------------

#include <TNamed.h>
class IlcDetectorEventHeader : public TNamed
{
 public:

  IlcDetectorEventHeader(const char* name);
  IlcDetectorEventHeader();
  virtual ~IlcDetectorEventHeader() {}
protected:
  ClassDef(IlcDetectorEventHeader,0) // Event header for detectors
};

#endif

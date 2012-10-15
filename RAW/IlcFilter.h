#ifndef ILCFILTER_H
#define ILCFILTER_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcFilter.h 32401 2009-05-15 14:01:03Z cvetan $ */

//
// base class for high level filter algorithms
// Derived classes should implement a default constructor and
// the virtual method Filter
//

#include <TObject.h>

class IlcRawVEvent;
class IlcESDEvent;


class IlcFilter: public TObject {
public:
  virtual Bool_t       Filter(IlcRawVEvent* event, IlcESDEvent* esd) = 0;

private:
  ClassDef(IlcFilter, 0)   // base class for high level filter algorithms
};

#endif

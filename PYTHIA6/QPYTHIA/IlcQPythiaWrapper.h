#ifndef ILCQPYTHIAWRAPPER_H
#define ILCQPYTHIAWRAPPER_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id$ */

#include <Rtypes.h>
#include <TError.h>

class IlcFastGlauber;

class IlcQPythiaWrapper {
 public:
    IlcQPythiaWrapper() {
	// Default constructor. The static data member is initialized 
	// in the implementation file
    }
    IlcQPythiaWrapper(const IlcQPythiaWrapper & /*rn*/) {
	// Copy constructor: no copy allowed for the object
	::Fatal("Copy constructor","Not allowed\n");
    }
    virtual ~IlcQPythiaWrapper() {
	// Destructor
    }
    IlcQPythiaWrapper & operator=(const IlcQPythiaWrapper& /*rn*/) {
	// Assignment operator: no assignment allowed
	::Fatal("Assignment operator","Not allowed\n");
	return (*this);
    }
  
private:

  ClassDef(IlcQPythiaWrapper, 0)  // Wrappers for C++ functionalities needed by the QPythia Fortran Code
};

#endif 


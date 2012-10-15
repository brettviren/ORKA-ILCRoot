#ifndef ILCGENBEAMGAS_H
#define ILCGENBEAMGAS_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcGenBeamGas.h 16640 2007-01-31 09:18:26Z hristov $ */

//
// Generator to simulate beam gas interactions.
// At present single interactions are read from an external file. 
// Author: andreas.morsch@cern.ch

#include "IlcGenExtFile.h"

class IlcGenBeamGas : public IlcGenExtFile
{
 public:
    IlcGenBeamGas();
    virtual ~IlcGenBeamGas();
    //
    virtual void SetNumberOfInteractions(Int_t n) 
	{fInteractions = n;}
    // Initialise 
    virtual void Init();
    // generate event
    virtual void Generate();
 protected:
    Int_t fInteractions;    // Number of interactions
 private:
    IlcGenBeamGas(const IlcGenBeamGas &beamgas);
    IlcGenBeamGas & operator=(const IlcGenBeamGas &beamgas);
    
    ClassDef(IlcGenBeamGas,1) //Generator for beam gas interactions
	
};
#endif







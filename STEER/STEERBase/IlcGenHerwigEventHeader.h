#ifndef ILCGENHERWIGEVENTHEADER_H
#define ILCGENHERWIGEVENTHEADER_H

/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id$ */

#include "IlcGenEventHeader.h"


class IlcGenHerwigEventHeader : public IlcGenEventHeader
{
 public:
    IlcGenHerwigEventHeader();
    IlcGenHerwigEventHeader(const char* name);
    virtual ~IlcGenHerwigEventHeader() {}
    Int_t    ProcessType() const  {return fProcessType;}
    void     SetProcessType(Int_t type)  {fProcessType = type;}
    Int_t    Trials() const {return fTrials;}
    void     SetTrials(Int_t trials) {fTrials = trials;}
    Float_t  Weight() const {return fWeight;}
    void     SetWeight(Float_t weight) {fWeight = weight;}
protected:
    Int_t    fProcessType;               // HERWIG process id for this event 
    Int_t    fTrials;                    // Number of trials to fulfill trigger condition
    Float_t  fWeight;                    // Event weight (= cross section in nb for unweighted events)
    ClassDef(IlcGenHerwigEventHeader, 2)  // Event header for Herwig event
};
	
	

#endif

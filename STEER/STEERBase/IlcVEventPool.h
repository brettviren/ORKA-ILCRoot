#ifndef ILCVEVENTPOOL_H
#define ILCVEVENTPOOL_H
 
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcAnalysisCuts.h 25775 2008-05-15 08:08:39Z morsch $ */

// Base class for event pool.
// This class is needed by the AnalysisManager to steer a mixing analysis.
// Author Andreas Morsch
// andreas.morsch@cern.ch

#include <TNamed.h>
class TChain;

class IlcVEventPool : public TNamed
{
 public:
    IlcVEventPool();
    IlcVEventPool(const char* name, const char* title);

    virtual ~IlcVEventPool() {;}
    // Interface
    virtual TChain* GetNextChain()                 = 0;
    virtual void   GetCurrentBin(Float_t* /*bin*/) = 0;
    virtual Int_t  GetDimension()                  = 0;
    virtual void   Init()                          = 0;
    virtual Int_t  BinNumber() const               = 0;
    // Basic functionality
    virtual void SetChain(TChain* chain) {fChain = chain;}
    IlcVEventPool(const IlcVEventPool& obj);
    IlcVEventPool& operator=(const IlcVEventPool& other);
 protected:
    TChain* fChain;   // Chain of trees
    ClassDef(IlcVEventPool, 1); 
};
 
#endif

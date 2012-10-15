#ifndef ILCEVENTPOOLLOOP_H
#define ILCEVENTPOOLLOOP_H
 
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id$ */

// Realisation of an IlcVEventPool which allows the user to
// run the analysis in a loop, i.e. passing several times over 
// the same event chain.
// Author Andreas Morsch
// andreas.morsch@cern.ch

#include <IlcVEventPool.h>
class IlcRunTagCuts;
class IlcLHCTagCuts;
class IlcDetectorTagCuts;
class IlcEventTagCuts;
class IlcTagAnalysis;
class TChain;

class IlcEventPoolLoop : public IlcVEventPool
{
 public:
    IlcEventPoolLoop();
    IlcEventPoolLoop(Int_t nit);
    IlcEventPoolLoop(const char* name, const char* title);

    virtual ~IlcEventPoolLoop() {;}
    // Interface
    virtual TChain* GetNextChain();
    virtual void  GetCurrentBin(Float_t* /*bin*/);
    virtual Int_t GetDimension();
    virtual void  Init();
    virtual Int_t BinNumber() const {return fNIteration;}
	    
 private:
    IlcEventPoolLoop(const IlcEventPoolLoop& obj);
    IlcEventPoolLoop& operator=(const IlcEventPoolLoop& other);
 protected:
    Int_t fMaxIterations; // Maximum number of iterations 
    Int_t fNIteration;    // Number of iterations
    TChain* fChainClone; // Clone of the original 
    ClassDef(IlcEventPoolLoop, 0); 
};
 
#endif

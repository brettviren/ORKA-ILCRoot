#ifndef ILCANALYSISCUTS_H
#define ILCANALYSISCUTS_H
 
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcAnalysisCuts.h 54863 2012-02-28 12:10:29Z hristov $ */

// Base class for analysis cuts
// Author Andreas Morsch
// andreas.morsch@cern.ch

#include "IlcVCuts.h"

class TList;
class TCollection;

class IlcAnalysisCuts : public IlcVCuts
{
 public:
    IlcAnalysisCuts();
    IlcAnalysisCuts(const char* name, const char* title);
    IlcAnalysisCuts(const IlcAnalysisCuts& obj);
    IlcAnalysisCuts& operator=(const IlcAnalysisCuts& obj);
    virtual ~IlcAnalysisCuts() {;}
    virtual Bool_t IsSelected(TObject* /* obj  */ )  {return kFALSE;}
    virtual Bool_t IsSelected(TList*   /* list */ ) = 0;
    virtual void   Init() {;}
    virtual void   SetFilterMask(UInt_t mask) {fFilterMask = mask;}
    virtual UInt_t GetFilterMask()   const    {return fFilterMask;}
    virtual void   SetSelected(Bool_t dec)    {fSelected = dec;}
    virtual UInt_t Selected()        const    {return fSelected;}	    
    virtual Long64_t Merge(TCollection* /* list */)      { return 0; }
    virtual TList* GetOutput()                { return 0; }
    virtual TObject *GetStatistics(Option_t *) const {return 0;}
 private:
    UInt_t fFilterMask; // Mask to use one of the previous decisions inside a filter
    Bool_t fSelected;   // Final decision on selction
    ClassDef(IlcAnalysisCuts, 5); // Base class for filter decisions on ESD objects
};
 
#endif

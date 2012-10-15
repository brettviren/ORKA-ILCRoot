#ifndef ILCANALYSISFILTER_H
#define ILCANALYSISFILTER_H
 
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcAnalysisFilter.h 53615 2011-12-15 15:52:21Z agheata $ */

//
// Manager class for filter decisions based on cuts
// Author: Andreas Morsch
// andreas.morsch@cern.ch

#include <TNamed.h>

class IlcAnalysisCuts;
class TList;

class IlcAnalysisFilter : public TNamed
{
 public:
    IlcAnalysisFilter();
    IlcAnalysisFilter(const char* name, const char* title = "AnalysisFilter");
    IlcAnalysisFilter(const IlcAnalysisFilter& obj);
    IlcAnalysisFilter& operator=(const IlcAnalysisFilter& other);
    virtual ~IlcAnalysisFilter();
    virtual UInt_t IsSelected(TObject* obj);
    virtual UInt_t IsSelected(TList* obj);
    virtual Bool_t IsSelected(char* name);
    virtual void AddCuts(IlcAnalysisCuts* cuts);
    virtual void Init();
    TList*  GetCuts() const {return fCuts;}
	    
 private:
    TList* fCuts;    // List of cuts
    ClassDef(IlcAnalysisFilter, 2); // Manager class for filter decisions
};
 
#endif

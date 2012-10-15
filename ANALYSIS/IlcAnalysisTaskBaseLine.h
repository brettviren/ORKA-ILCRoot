/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcAnalysisTaskBaseLine.cxx 46301 2011-01-06 14:25:27Z agheata $ */

//
// This empty task is used for the analysis train to estimate the memory and CPU consumption without any user code
//

#ifndef ILCANALYSISTASKBASELINE_H
#define ILCANALYSISTASKBASELINE_H

#include "IlcAnalysisTaskSE.h"

class IlcAnalysisTaskBaseLine : public IlcAnalysisTaskSE {
 public:
    IlcAnalysisTaskBaseLine();
    IlcAnalysisTaskBaseLine(const char *name);
    virtual ~IlcAnalysisTaskBaseLine();
    
    virtual void     UserExec(Option_t *option);
    
 private:
    IlcAnalysisTaskBaseLine(const IlcAnalysisTaskBaseLine&); // not implemented
    IlcAnalysisTaskBaseLine& operator=(const IlcAnalysisTaskBaseLine&); // not implemented
    
    ClassDef(IlcAnalysisTaskBaseLine, 1); // empty analysis
};

#endif


/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcAnalysisTaskEx01.h 45956 2010-12-10 12:55:37Z agheata $ */
/* IlcAnalysisTaskEx01.h
 *
 * Template task producing a P_t spectrum and pseudorapidity distribution.
 * Includes explanations of physics and primary track selections
 *
 * Based on tutorial example from offline pages
 * Edited by Arvinder Palaha
 */
#ifndef ILCANALYSISTASKEX01_H
#define ILCANALYSISTASKEX01_H

class TH1F;
class TList;
class IlcESDtrackCuts;

#ifndef ILCANALYSISTASKSE_H
#include "IlcAnalysisTaskSE.h"
#endif

class IlcAnalysisTaskEx01 : public IlcAnalysisTaskSE {
 public:
    IlcAnalysisTaskEx01();
    IlcAnalysisTaskEx01(const char *name);
    virtual ~IlcAnalysisTaskEx01();
    
    virtual void     UserCreateOutputObjects();
    virtual void     UserExec(Option_t *option);
    virtual void     Terminate(Option_t *);
    
 private:
    TList           *fOutput;        // Output list
    IlcESDtrackCuts *fTrackCuts;     // Track cuts
    TH1F            *fHistPt;        // Pt spectrum
    TH1F            *fHistEta;       // pseudorapidity spectrum
    // NEW HISTO to be declared here
    
    IlcAnalysisTaskEx01(const IlcAnalysisTaskEx01&); // not implemented
    IlcAnalysisTaskEx01& operator=(const IlcAnalysisTaskEx01&); // not implemented
    
    ClassDef(IlcAnalysisTaskEx01, 1); // example of analysis
};

#endif


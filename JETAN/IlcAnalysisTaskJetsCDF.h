//
// CDF jet finding
//
#ifndef ILCANALYSISTASKJETSCDF_H
#define ILCANALYSISTASKJETSCDF_H

/*
 * 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * Author: The International Lepton Collider Off-line Project.            *
 *                                                                        *
 * Part of the code has been developed by ILC Off-line Project.         *
 * Contributors are mentioned in the code where appropriate.              *
 * See cxx source for full Copyright notice
 *
*/

#include "IlcAnalysisTaskSE.h"

class IlcCdfJetFinder;
class TList;

class IlcAnalysisTaskJetsCDF : public IlcAnalysisTaskSE
{
 public:
    IlcAnalysisTaskJetsCDF();
    IlcAnalysisTaskJetsCDF(const char* name);
    virtual ~IlcAnalysisTaskJetsCDF() {;}
    // Implementation of interface methods
    virtual void UserCreateOutputObjects();
    virtual void LocalInit();
    virtual void UserExec(Option_t *option);
    virtual void Terminate(Option_t *option);
 private:
  IlcAnalysisTaskJetsCDF(const IlcAnalysisTaskJetsCDF &det);
  IlcAnalysisTaskJetsCDF &operator=(const IlcAnalysisTaskJetsCDF &det);

 private:
    IlcCdfJetFinder* fJetFinder;    //  Pointer to the jet finder
    TList*           fListOfHistos; //  Output list of histograms

    ClassDef(IlcAnalysisTaskJetsCDF, 1); // Analysis task for CDF jet analysis
};

#endif

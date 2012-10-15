#ifndef ILCANALYSISTASKSTAT_H
#define ILCANALYSISTASKSTAT_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcAnalysisTaskStat.h 46299 2011-01-06 11:17:06Z agheata $ */
// Author: Andrei Gheata, 20/12/2010

//==============================================================================
//   IlcAnalysisTaskStat - Analysis task that ataches an IlcAnalysisStatistics
//      to the analysis manager
//==============================================================================

#ifndef ILCANALYSISTASKSE_H
#include "IlcAnalysisTaskSE.h"
#endif

class IlcAnalysisStatistics;

class IlcAnalysisTaskStat : public IlcAnalysisTaskSE
{

protected:
  IlcAnalysisStatistics      *fStatistics; // Statistics object
  TList                      *fOutputList; // Output list

private:
  IlcAnalysisTaskStat(const IlcAnalysisTaskStat& other);
  IlcAnalysisTaskStat& operator= (const IlcAnalysisTaskStat& other);

public:
  IlcAnalysisTaskStat() : IlcAnalysisTaskSE(), fStatistics(0), fOutputList(0) {}
  IlcAnalysisTaskStat(const char *name);
  virtual ~IlcAnalysisTaskStat();

  // Static method to add to the analysis manager
  static IlcAnalysisTaskStat *AddToManager(UInt_t offlineMask=0);

  // Implementation of interface methods
  virtual void UserCreateOutputObjects();
  virtual void UserExec(Option_t *option);
  virtual void Terminate(Option_t *option);
  
  // Getters
  IlcAnalysisStatistics *GetStatistics() const {return fStatistics;}

  ClassDef(IlcAnalysisTaskStat, 1); // Statistics task
};
#endif

#ifndef ILCPVECSURVEY1_H
#define ILCPVECSURVEY1_H

/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcPVECSurvey1.h 19604 2007-07-10 13:14:58Z kharlov $ */

/* History of cvs commits:
 *
 * $Log$
 * Revision 1.1  2007/07/10 12:41:38  kharlov
 * Added a new class IlcPVECSurvet1 which read survey data from EDMS files
 *
 */

// A survey "reader" class, based on IlcSurveyObj class.
// The source of input data is a text file in a standartized format
// downloaded from EDMS

#include "IlcPVECSurvey.h"

//  IlcPVECSurvey1 class is survey "reader" class
//  based on IlcSurveyObj class.

class IlcPVECSurvey1 : public IlcPVECSurvey {
public:
  IlcPVECSurvey1(const TString &surveyFileName, const TString &namePrefix);
  virtual ~IlcPVECSurvey1();

private:
  enum EHardcoded {kNumberOfPoints = 452, kStartingPoint = 4};

  IlcPVECSurvey1(const IlcPVECSurvey1 &rhs);
  IlcPVECSurvey1 &operator = (const IlcPVECSurvey1 &rhs);

  ClassDef(IlcPVECSurvey1, 1) //Data reader, based on IlcSurveyObj
};

#endif

#ifndef ILCPVBARSURVEY1_H
#define ILCPVBARSURVEY1_H

/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcPVBARSurvey1.h 19604 2007-07-10 13:14:58Z kharlov $ */

/* History of cvs commits:
 *
 * $Log$
 * Revision 1.1  2007/07/10 12:41:38  kharlov
 * Added a new class IlcPVBARSurvet1 which read survey data from EDMS files
 *
 */

// A survey "reader" class, based on IlcSurveyObj class.
// The source of input data is a text file in a standartized format
// downloaded from EDMS

#include "IlcPVBARSurvey.h"

//  IlcPVBARSurvey1 class is survey "reader" class
//  based on IlcSurveyObj class.

class IlcPVBARSurvey1 : public IlcPVBARSurvey {
public:
  IlcPVBARSurvey1(const TString &surveyFileName, const TString &namePrefix);
  virtual ~IlcPVBARSurvey1();

private:
  enum EHardcoded {kNumberOfPoints = 452, kStartingPoint = 4};

  IlcPVBARSurvey1(const IlcPVBARSurvey1 &rhs);
  IlcPVBARSurvey1 &operator = (const IlcPVBARSurvey1 &rhs);

  ClassDef(IlcPVBARSurvey1, 1) //Data reader, based on IlcSurveyObj
};

#endif

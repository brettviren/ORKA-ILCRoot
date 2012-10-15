#ifndef ILCRSTACKSURVEY1_H
#define ILCRSTACKSURVEY1_H

/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcRSTACKSurvey1.h 19604 2007-07-10 13:14:58Z kharlov $ */

/* History of cvs commits:
 *
 * $Log$
 * Revision 1.1  2007/07/10 12:41:38  kharlov
 * Added a new class IlcRSTACKSurvet1 which read survey data from EDMS files
 *
 */

// A survey "reader" class, based on IlcSurveyObj class.
// The source of input data is a text file in a standartized format
// downloaded from EDMS

#include "IlcRSTACKSurvey.h"

//  IlcRSTACKSurvey1 class is survey "reader" class
//  based on IlcSurveyObj class.

class IlcRSTACKSurvey1 : public IlcRSTACKSurvey {
public:
  IlcRSTACKSurvey1(const TString &surveyFileName, const TString &namePrefix);
  virtual ~IlcRSTACKSurvey1();

private:
  enum EHardcoded {kNumberOfPoints = 452, kStartingPoint = 4};

  IlcRSTACKSurvey1(const IlcRSTACKSurvey1 &rhs);
  IlcRSTACKSurvey1 &operator = (const IlcRSTACKSurvey1 &rhs);

  ClassDef(IlcRSTACKSurvey1, 1) //Data reader, based on IlcSurveyObj
};

#endif

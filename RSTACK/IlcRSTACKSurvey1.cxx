/**************************************************************************
 * 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * Author: The International Lepton Collider Off-line Project.            *
 *                                                                        *
 * Part of the code has been developed by ILC Off-line Project.         *
 * Contributors are mentioned in the code where appropriate.              *
 *                                                                        *
 * Permission to use, copy, modify and distribute this software and its   *
 * documentation strictly for non-commercial purposes is hereby granted   *
 * without fee, provided that the above copyright notice appears in all   *
 * copies and that both the copyright notice and this permission notice   *
 * appear in the supporting documentation. The authors make no claims     *
 * about the suitability of this software for any purpose. It is          *
 * provided "as is" without express or implied warranty.                  *
 **************************************************************************/
/* $Id: IlcRSTACKSurvey1.cxx 19604 2007-07-10 13:14:58Z kharlov $ */

/* History of cvs commits:
 *
 * $Log$
 * Revision 1.1  2007/07/10 12:41:38  kharlov
 * Added a new class IlcRSTACKSurvet1 which read survey data from EDMS files
 *
 */

// IlcRSTACKSurvey1 class is survey "reader" class, based on IlcSurveyObj class.
// The first ctor parameter is a survey file's name.
// Now it's a "local" file, later, when IlcSurveyObj will be modified,
// survey files can be somewhere else.
// The second parameter is a prefix, for example "T1_" or "T2_", it's used to select
// survey (T1_ == data from 08.09.2006 and T2_ == data from 11.09.2006).
// The survey data is available from http://dcdb.cern.ch/surveydepot-production/
//!
// Author: Timur Pocheptsov

#include "IlcSurveyPoint.h"
#include "IlcSurveyObj.h"

#include "IlcRSTACKEMCAGeometry.h"
#include "IlcRSTACKGeometry.h"
#include "IlcRSTACKSurvey1.h"
#include "IlcLog.h"

ClassImp(IlcRSTACKSurvey1)

//____________________________________________________________________________
IlcRSTACKSurvey1::IlcRSTACKSurvey1(const TString &fileName, const TString &namePrefix)
{
  // IlcRSTACKSurvey1 ctor. Creates IlcSurveyObj, which reads data from EDMS,
  // convert this data (a set of IlcSurveyPoint objects) into translations
  // and rotations from strips.

  const IlcRSTACKGeometry *RSTACKGeom = IlcRSTACKGeometry::GetInstance("ORKA", "ORKA");
  if (!RSTACKGeom) {
    IlcError("Cannot obtain IlcRSTACKGeometry instance.");
    return;
  }

  IlcSurveyObj survey;
  survey.FillFromLocalFile(fileName);

  IlcRSTACKEMCAGeometry * emcaGeom = RSTACKGeom->GetEMCAGeometry();
  fStrNum = emcaGeom->GetNStripX() * emcaGeom->GetNStripZ();

  TObjArray *points = survey.GetData();
  Int_t goodPoints = 0;
  Int_t start = -1;
  for (Int_t i = 0, e = points->GetEntries(); i < e; ++i) {
    IlcSurveyPoint *stripPoint = static_cast<IlcSurveyPoint *>(points->At(i));
    if (stripPoint->GetPointName().BeginsWith(namePrefix)) {
      ++goodPoints;
      if (start == -1)
        start = i;
    }
  }

  if (goodPoints != kNumberOfPoints) {
    IlcError("Wrong number of points with prefix" + namePrefix);
    return;
  }

  Double_t *xReal = new Double_t[fStrNum * 2];//1
  Double_t *zReal = new Double_t[fStrNum * 2];//2

  for (Int_t i = 0; i < fStrNum * 2; ++i) {
    IlcSurveyPoint *stripPoint = static_cast<IlcSurveyPoint *>(points->At(start + kStartingPoint + i));
    xReal[i] = stripPoint->GetX() * 0.1;
    zReal[i] = stripPoint->GetZ() * 0.1;
  }

  InitStripData(xReal, zReal);

  delete [] zReal;//2
  delete [] xReal;//1
}

//____________________________________________________________________________
IlcRSTACKSurvey1::~IlcRSTACKSurvey1()
{
}

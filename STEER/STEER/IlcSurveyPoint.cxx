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

/////////////////////////////////////////////////////////////////////
//                                                                 //
//  class IlcSurveyPoint					   //
//  Retrieve and Convert survey data into ROOT Objects		   //
//                                                                 //
/////////////////////////////////////////////////////////////////////

#include "IlcSurveyPoint.h"

ClassImp(IlcSurveyPoint)
  
//_____________________________________________________________________________
IlcSurveyPoint::IlcSurveyPoint():
  fPointName(""),
  fX(0.0),
  fY(0.0),
  fZ(0.0),
  fPrecisionX(0.0),
  fPrecisionY(0.0),
  fPrecisionZ(0.0),
  fType(' '),
  fTargetUsed(kTRUE)
{
  // Constructor
  
}

//_____________________________________________________________________________
IlcSurveyPoint::IlcSurveyPoint(TString name, Float_t x, Float_t y,
                               Float_t z, Float_t precX, Float_t precY,
                               Float_t precZ, Char_t type, Bool_t Target):
  fPointName(name),
  fX(x),
  fY(y),
  fZ(z),
  fPrecisionX(precX),
  fPrecisionY(precY),
  fPrecisionZ(precZ),
  fType(type),
  fTargetUsed(Target)
{
  // Constructor
  
}

//_____________________________________________________________________________
IlcSurveyPoint::~IlcSurveyPoint() {
  //destructor
  
}

//_____________________________________________________________________________
void IlcSurveyPoint::PrintPoint() {
  // Prints X, Y and Z coordinates of the point
  printf("Point Coordinates \"%s\": %f %f %f\n", (const char*) fPointName, fX, fY, fZ);
  return;
}


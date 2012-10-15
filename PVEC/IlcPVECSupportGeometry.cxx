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

/* $Id: IlcPVECSupportGeometry.cxx 46104 2010-12-16 18:00:01Z policheh $ */

//_________________________________________________________________________
// Geometry class  for PVEC : Support which holds all PVEC modules.
// Its data members provide geometry parametrization of
// the PVEC support which can be changed in the constructor only.
// Author:   Yuri Kharlov (IHEP, Protvino)
// 13 November 2000

// --- IlcRoot header files ---

#include "IlcPVECSupportGeometry.h"

ClassImp(IlcPVECSupportGeometry)

//____________________________________________________________________________
IlcPVECSupportGeometry::IlcPVECSupportGeometry() :
  fRailLength(1200.0),
  fDistanceBetwRails(420.0),
  fRailsDistanceFromIP(590.),
  fCradleWallThickness(2.0)
{

  // Initializes the PVEC support parameters
  

  fRailPart1[0] =   28.0;
  fRailPart1[1] =    3.0;
  fRailPart1[2] = fRailLength;

  fRailPart2[0] =    1.5;
  fRailPart2[1] =   34.0;
  fRailPart2[2] = fRailLength;

  fRailPart3[0] =    6.0;
  fRailPart3[1] =    5.0;
  fRailPart3[2] = fRailLength;

  fRailPos[0] = 0.;
  fRailPos[1] = 0.;
  fRailPos[2] = 0.;

  fRailOuterSize[0] = fRailPart1[0];
  fRailOuterSize[1] = fRailPart1[1]*2 + fRailPart2[1] + fRailPart3[1];
  fRailOuterSize[2] = fRailLength;

  fRailRoadSize[0] = fDistanceBetwRails + fRailOuterSize[0];
  fRailRoadSize[1] = fRailOuterSize[1];
  fRailRoadSize[2] = fRailOuterSize[2];

  fCradleWall[0] =   0.;  // Inner radius, to be defined from PVEC parameters
  fCradleWall[1] =  65.;  // Diff. between outer and inner radii
  fCradleWall[2] =  18.;
  fCradleWall[3] = 270. - 50.;
  fCradleWall[4] = 270. + 50.;

  fCradleWheel[0] = 30.0;
  fCradleWheel[1] = 80.0;
  fCradleWheel[2] = 30.0;
}

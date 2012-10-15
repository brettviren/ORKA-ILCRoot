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

/* $Id: IlcDigit.cxx 50615 2011-07-16 23:19:19Z hristov $ */

//------------------------------------------------------------------------
//             Main digit class
//  Base class for Ilc digits
//  Author:
//------------------------------------------------------------------------

#include "IlcDigit.h"
 
ClassImp(IlcDigit)

IlcDigit::IlcDigit()
{
  //
  // Default constructor
  //
  fTracks[0]=fTracks[1]=fTracks[2]=-1;  
}

IlcDigit::IlcDigit(Int_t *tracks)
{
  //
  // Standard constructor
  //
  fTracks[0] = tracks[0];
  fTracks[1] = tracks[1];
  fTracks[2] = tracks[2];
}

	 

/**************************************************************************
 * Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 *                                                                        *
// Author: The ILC Off-line Project. 
 // Part of the code has been developed by Alice Off-line Project. 
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


//////////////////////////////////////////////////////
//  Base Response class for TARGET                     //
//  Specific subdetector implementation is done in  //
//  IlcTARGETresponseSPD                               //
//  IlcTARGETresponseSDD                               //
//  IlcTARGETresponseSSD                               //
//////////////////////////////////////////////////////

#include "IlcTARGETresponse.h"

ClassImp(IlcTARGETresponse)

//______________________________________________________________________
IlcTARGETresponse::IlcTARGETresponse(){
  
  // Default Constructor

  SetFilenames();
  SetOutputOption();
  fDiffCoeff=0;
  fDiffCoeff1=0;
}


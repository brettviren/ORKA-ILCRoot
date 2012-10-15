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

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//  DCH calibration class for position parameters of the supermodules        //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#include "IlcDCHCalSuperModulePos.h"

ClassImp(IlcDCHCalSuperModulePos)

//_____________________________________________________________________________
IlcDCHCalSuperModulePos::IlcDCHCalSuperModulePos():TNamed()
{
  //
  // IlcDCHCalSuperModulePos default constructor
  //

  for (Int_t idet = 0; idet < kNsect; idet++) {
    for (Int_t i = 0; i < 3; ++i) {
      fPos[idet][i] = 0;
      fRot[idet][i] = 0;
    }
  }
}

//_____________________________________________________________________________
IlcDCHCalSuperModulePos::IlcDCHCalSuperModulePos(const Text_t *name, const Text_t *title)
                :TNamed(name,title)
{
  //
  // IlcDCHCalSuperModulePos constructor
  //

  for (Int_t idet = 0; idet < kNsect; idet++) {
    for (Int_t i = 0; i < 3; ++i) {
      fPos[idet][i] = 0;
      fRot[idet][i] = 0;
    }
  }
}


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
//  DCH calibration class for status of supermodules                         //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#include "IlcDCHCalSuperModuleStatus.h"

ClassImp(IlcDCHCalSuperModuleStatus)

//_____________________________________________________________________________
IlcDCHCalSuperModuleStatus::IlcDCHCalSuperModuleStatus():TNamed()
{
  //
  // IlcDCHCalSuperModuleStatus default constructor
  //

  for (Int_t idet = 0; idet < kNsect; idet++) {
    fStatus[idet] = 0;
  }
}

//_____________________________________________________________________________
IlcDCHCalSuperModuleStatus::IlcDCHCalSuperModuleStatus(const Text_t *name, const Text_t *title)
                :TNamed(name,title)
{
  //
  // IlcDCHCalSuperModuleStatus constructor
  //

  for (Int_t idet = 0; idet < kNsect; idet++) {
    fStatus[idet] = 0;
  }
}


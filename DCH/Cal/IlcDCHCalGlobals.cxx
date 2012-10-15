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

/* $Id: IlcDCHCalGlobals.cxx,v 1.2 2009/12/11 10:51:33 vitomeg Exp $ */

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//  DCH calibration class for global DCH parameters //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#include "IlcDCHCalGlobals.h"

ClassImp(IlcDCHCalGlobals)

//_____________________________________________________________________________
IlcDCHCalGlobals::IlcDCHCalGlobals():TNamed()
{
  //
  // IlcDCHCalGlobals default constructor
  //
  
  
  Init();
}

//_____________________________________________________________________________
IlcDCHCalGlobals::IlcDCHCalGlobals(const Text_t *name, const Text_t *title)
                :TNamed(name,title)
{
  //
  // IlcDCHCalGlobals constructor
  //

  Init();
}

//_____________________________________________________________________________
void IlcDCHCalGlobals::Init()
{
  //
  // default initialization
  //
  
  fSamplingFrequency = 0;
  fNumberOfTimeBins = 0;
}

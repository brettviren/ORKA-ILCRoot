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

/* $Id: IlcDCHclusterMI.cxx,v 1.1.1.1 2008/03/11 14:53:01 vitomeg Exp $ */

 
///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//  DCH cluster                                                              //
//                                                                           //
/////////////////////////////////////////////////////////////////////////////// 

#include "IlcDCHclusterMI.h"
#include "IlcDCHrecPoint.h"

ClassImp(IlcDCHclusterMI)

  //___________________________________________________________________________

IlcDCHclusterMI::IlcDCHclusterMI() : IlcDCHcluster() 
{ 
  fNPads  = 0;
  fRelPos = 0;
  fRmsY   = 0;
}

IlcDCHclusterMI::IlcDCHclusterMI(IlcDCHcluster & cl): IlcDCHcluster(cl)
{
  fNPads  = 0;
  fRelPos = 0;
  fRmsY   = 0;
}
//_____________________________________________________________________________
  IlcDCHclusterMI::IlcDCHclusterMI(const IlcDCHrecPoint &p):IlcDCHcluster(p)
{
  //
  // Constructor from IlcDCHrecPoint
  //
  fNPads = 0;
  fRelPos =0;
  
}


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

/* $Id: IlcDCHsegmentID.cxx,v 1.1.1.1 2008/03/11 14:53:01 vitomeg Exp $ */

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//     Base class for a detector segment                                     // 
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#include "IlcDCHsegmentID.h"

ClassImp(IlcDCHsegmentID)

//_____________________________________________________________________________
IlcDCHsegmentID::IlcDCHsegmentID():TObject()
{
  //
  // IlcDCHsegmentID default constructor
  //

  fSegmentID = 0;

}

//_____________________________________________________________________________
IlcDCHsegmentID::IlcDCHsegmentID(Int_t index):TObject()
{
  //
  // Defines a detector segment
  //

  fSegmentID = index;

}

//_____________________________________________________________________________
IlcDCHsegmentID::~IlcDCHsegmentID()
{
  //
  // IlcDCHsegmentID destructor
  //

}

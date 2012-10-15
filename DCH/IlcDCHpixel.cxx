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

/* $Id: IlcDCHpixel.cxx,v 1.1.1.1 2008/03/11 14:53:01 vitomeg Exp $ */

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//  Contains the information for one MUD pixel                               //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#include "IlcDCHpixel.h"

ClassImp(IlcDCHpixel)

//_____________________________________________________________________________

  // Maximal number of stored tracks
  const Int_t IlcDCHpixel::fgkNTrackPixel = kNTrackPixel;

//_____________________________________________________________________________
IlcDCHpixel::IlcDCHpixel():TObject()
{
  //
  // Create a MUD pixel
  // 

  fSignal   =  0;
  fTrack[0] = -1;
  fTrack[1] = -1;
  fTrack[2] = -1;

}

//_____________________________________________________________________________
IlcDCHpixel::~IlcDCHpixel()
{
  //
  // IlcDCHpixel destructor
  //

}

//_____________________________________________________________________________
void IlcDCHpixel::Copy(TObject &p) const
{
  //
  // Copy function
  //

  ((IlcDCHpixel &) p).fSignal = fSignal;
  for (Int_t iTrackPixel = 0; iTrackPixel < kNTrackPixel; iTrackPixel++) {
    ((IlcDCHpixel &) p).fTrack[iTrackPixel] = fTrack[iTrackPixel];
  }

}

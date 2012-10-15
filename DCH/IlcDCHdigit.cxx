

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

/* $Id: IlcDCHdigit.cxx,v 1.1.1.1 2008/03/11 14:53:01 vitomeg Exp $ */

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//  The DCH digit                                                            //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#include "IlcDCHdigit.h"

ClassImp(IlcDCHdigit)

//_____________________________________________________________________________
  
  // Marks a raw digit
  const UInt_t IlcDCHdigit::fgkRawDigit = 0x00000001; 

//_____________________________________________________________________________
IlcDCHdigit::IlcDCHdigit():IlcDigitNew()
{
  //
  // Default constructor
  //

  fRow  = 0;
  fCol  = 0;
  fTime = 0;

}

//_____________________________________________________________________________
IlcDCHdigit::IlcDCHdigit(Bool_t isRaw, Int_t *digits, Int_t *amp):IlcDigitNew()
{
  //
  // Create a DCH digit
  //

  // Store the volume hierarchy
  fId   = digits[0];

  // Store the row, pad, and time bucket number
  fRow  = digits[1];
  fCol  = digits[2];
  fTime = digits[3];

  // Store the signal amplitude
  fAmp  = amp[0];

  if (isRaw) SetBit(fgkRawDigit);

}

//_____________________________________________________________________________
IlcDCHdigit::~IlcDCHdigit()
{
  //
  // IlcDCHdigit destructor
  //

}

//_____________________________________________________________________________
Int_t IlcDCHdigit::DecodeAmp() const
{
  //
  // Decodes the digit amplitude
  //

  return 0;

}

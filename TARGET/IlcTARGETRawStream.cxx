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

/* $Id: IlcTARGETRawStream.cxx,v 1.1.1.1 2008/03/11 14:52:51 vitomeg Exp $ */

///////////////////////////////////////////////////////////////////////////////
///
/// This is a base class for providing access to TARGET digits in raw data.
///
/// Derived class should implement the Next method.
///
/// It loops over all TARGET digits in the raw data given by the IlcRawReader.
/// The Next method goes to the next digit. If there are no digits left
/// it returns kFALSE.
/// Several getters provide information about the current digit.
///
///////////////////////////////////////////////////////////////////////////////

#include "IlcTARGETRawStream.h"

ClassImp(IlcTARGETRawStream)


IlcTARGETRawStream::IlcTARGETRawStream(IlcRawReader* rawReader)
{
// create an object to read TARGET raw digits

  fRawReader = rawReader;
  fModuleID = fPrevModuleID = fCoord1 = fCoord2 = fSignal = -1;
}

IlcTARGETRawStream::IlcTARGETRawStream(const IlcTARGETRawStream& stream) :
  TObject(stream)
{
  Fatal("IlcTARGETRawStream", "copy constructor not implemented");
}

IlcTARGETRawStream& IlcTARGETRawStream::operator = (const IlcTARGETRawStream& 
					      /* stream */)
{
  Fatal("operator =", "assignment operator not implemented");
  return *this;
}


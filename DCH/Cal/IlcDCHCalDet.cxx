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

/* $Id: IlcDCHCalDet.cxx,v 1.1.1.1 2008/03/11 14:53:01 vitomeg Exp $ */

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//  DCH calibration class for parameters which saved per detector            //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#include "IlcDCHCalDet.h"

ClassImp(IlcDCHCalDet)

//_____________________________________________________________________________
IlcDCHCalDet::IlcDCHCalDet():TNamed()
{
  //
  // IlcDCHCalDet default constructor
  //

  for (Int_t idet = 0; idet < kNdet; idet++) {
    fData[idet] = 0;
  }

}

//_____________________________________________________________________________
IlcDCHCalDet::IlcDCHCalDet(const Text_t *name, const Text_t *title)
                :TNamed(name,title)
{
  //
  // IlcDCHCalDet constructor
  //

  for (Int_t idet = 0; idet < kNdet; idet++) {
    fData[idet] = 0;
  }

}


//_____________________________________________________________________________
IlcDCHCalDet::IlcDCHCalDet(const IlcDCHCalDet &c):TNamed(c)
{
  //
  // IlcDCHCalDet copy constructor
  //

  ((IlcDCHCalDet &) c).Copy(*this);

}

///_____________________________________________________________________________
IlcDCHCalDet::~IlcDCHCalDet()
{
  //
  // IlcDCHCalDet destructor
  //

}

//_____________________________________________________________________________
IlcDCHCalDet &IlcDCHCalDet::operator=(const IlcDCHCalDet &c)
{
  //
  // Assignment operator
  //

  if (this != &c) ((IlcDCHCalDet &) c).Copy(*this);
  return *this;

}

//_____________________________________________________________________________
void IlcDCHCalDet::Copy(TObject &c) const
{
  //
  // Copy function
  //

  for (Int_t idet = 0; idet < kNdet; idet++) {
    ((IlcDCHCalDet &) c).fData[idet] = fData[idet];
  }

  TObject::Copy(c);

}


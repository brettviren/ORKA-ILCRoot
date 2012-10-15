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

/* $Id: IlcDCHCalPadStatus.cxx,v 1.1.1.1 2008/03/11 14:53:01 vitomeg Exp $ */

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//  DCH calibration class for MCM status                                     //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#include "IlcDCHCalPadStatus.h"

#include "IlcDCHCalSingleChamberStatus.h"

ClassImp(IlcDCHCalPadStatus)

//_____________________________________________________________________________
IlcDCHCalPadStatus::IlcDCHCalPadStatus():TNamed()
{
  //
  // IlcDCHCalPadStatus default constructor
  //

  for (Int_t idet = 0; idet < kNdet; idet++) {
    fROC[idet] = 0;
  }

}

//_____________________________________________________________________________
IlcDCHCalPadStatus::IlcDCHCalPadStatus(const Text_t *name, const Text_t *title)
                :TNamed(name,title)
{
  //
  // IlcDCHCalPadStatus constructor
  //

  for (Int_t isec = 0; isec < kNsect; isec++) {
    for (Int_t ipla = 0; ipla < kNplan; ipla++) {
      for (Int_t icha = 0; icha < kNcham; icha++) {
        Int_t idet = IlcDCHgeometry::GetDetector(ipla,icha,isec);
        fROC[idet] = new IlcDCHCalSingleChamberStatus(ipla,icha,144);
      }
    }
  }

}


//_____________________________________________________________________________
IlcDCHCalPadStatus::IlcDCHCalPadStatus(const IlcDCHCalPadStatus &c):TNamed(c)
{
  //
  // IlcDCHCalPadStatus copy constructor
  //

  ((IlcDCHCalPadStatus &) c).Copy(*this);

}

///_____________________________________________________________________________
IlcDCHCalPadStatus::~IlcDCHCalPadStatus()
{
  //
  // IlcDCHCalPadStatus destructor
  //

  for (Int_t idet = 0; idet < kNdet; idet++) {
    if (fROC[idet]) {
      delete fROC[idet];
      fROC[idet] = 0;
    }
  }

}

//_____________________________________________________________________________
IlcDCHCalPadStatus &IlcDCHCalPadStatus::operator=(const IlcDCHCalPadStatus &c)
{
  //
  // Assignment operator
  //

  if (this != &c) ((IlcDCHCalPadStatus &) c).Copy(*this);
  return *this;

}

//_____________________________________________________________________________
void IlcDCHCalPadStatus::Copy(TObject &c) const
{
  //
  // Copy function
  //

  for (Int_t idet = 0; idet < kNdet; idet++) {
    if (fROC[idet]) {
      fROC[idet]->Copy(*((IlcDCHCalPadStatus &) c).fROC[idet]);
    }
  }

  TObject::Copy(c);
}


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

/* $Id: IlcDCHCalPad.cxx,v 1.1.1.1 2008/03/11 14:53:01 vitomeg Exp $ */

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//  DCH calibration class for parameters which saved per pad                 //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#include "IlcDCHCalPad.h"
#include "IlcDCHCalROC.h"
#include "IlcDCHCalDet.h"

ClassImp(IlcDCHCalPad)

//_____________________________________________________________________________
IlcDCHCalPad::IlcDCHCalPad():TNamed()
{
  //
  // IlcDCHCalPad default constructor
  //

  for (Int_t idet = 0; idet < kNdet; idet++) {
    fROC[idet] = 0;
  }

}

//_____________________________________________________________________________
IlcDCHCalPad::IlcDCHCalPad(const Text_t *name, const Text_t *title)
                :TNamed(name,title)
{
  //
  // IlcDCHCalPad constructor
  //

  for (Int_t isec = 0; isec < kNsect; isec++) {
    for (Int_t ipla = 0; ipla < kNplan; ipla++) {
      for (Int_t icha = 0; icha < kNcham; icha++) {
        Int_t idet = GetDet(ipla,icha,isec);
        fROC[idet] = new IlcDCHCalROC(ipla,icha);
      }
    }
  }

}


//_____________________________________________________________________________
IlcDCHCalPad::IlcDCHCalPad(const IlcDCHCalPad &c):TNamed(c)
{
  //
  // IlcDCHCalPad copy constructor
  //

  ((IlcDCHCalPad &) c).Copy(*this);

}

///_____________________________________________________________________________
IlcDCHCalPad::~IlcDCHCalPad()
{
  //
  // IlcDCHCalPad destructor
  //

  for (Int_t idet = 0; idet < kNdet; idet++) {
    if (fROC[idet]) {
      delete fROC[idet];
      fROC[idet] = 0;
    }
  }

}

//_____________________________________________________________________________
IlcDCHCalPad &IlcDCHCalPad::operator=(const IlcDCHCalPad &c)
{
  //
  // Assignment operator
  //

  if (this != &c) ((IlcDCHCalPad &) c).Copy(*this);
  return *this;

}

//_____________________________________________________________________________
void IlcDCHCalPad::Copy(TObject &c) const
{
  //
  // Copy function
  //

  for (Int_t idet = 0; idet < kNdet; idet++) {
    if (fROC[idet]) {
      fROC[idet]->Copy(*((IlcDCHCalPad &) c).fROC[idet]);
    }
  }

  TObject::Copy(c);
}

//_____________________________________________________________________________
void IlcDCHCalPad::ScaleROCs(IlcDCHCalDet* values)
{
  // 
  // Scales ROCs of this class with the values from the class <values>
  // Is used if an IlcDCHCalPad object defines local variations of a parameter
  // defined per detector using a IlcDCHCalDet class
  //
  
  if (!values)
    return;
  
  for (Int_t idet = 0; idet < kNdet; idet++) {
    if (fROC[idet]) { 
      fROC[idet]->Scale(values->GetValue(idet));
    }
  }
}


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

/* $Id: IlcDCHCommonParam.cxx,v 1.1.1.1 2008/03/11 14:53:01 vitomeg Exp $ */

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// Class containing constant common parameters                           //
//                                                                           //
// Request an instance with IlcDCHCommonParam::Instance()                 //
// Then request the needed values                                            //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#include "IlcRun.h"

#include "IlcDCHCommonParam.h"
#include "IlcDCHpadPlane.h"


ClassImp(IlcDCHCommonParam)

IlcDCHCommonParam* IlcDCHCommonParam::fgInstance = 0;
Bool_t IlcDCHCommonParam::fgTerminated = kFALSE;

//_ singleton implementation __________________________________________________
IlcDCHCommonParam* IlcDCHCommonParam::Instance()
{
  //
  // Singleton implementation
  // Returns an instance of this class, it is created if neccessary
  // 
  
  if (fgTerminated != kFALSE)
    return 0;

  if (fgInstance == 0)
    fgInstance = new IlcDCHCommonParam();
  
  return fgInstance;
}

void IlcDCHCommonParam::Terminate()
{
  //
  // Singleton implementation
  // Deletes the instance of this class and sets the terminated flag, instances cannot be requested anymore
  // This function can be called several times.
  //
  
  fgTerminated = kTRUE;
  
  if (fgInstance != 0)
  {
    delete fgInstance;
    fgInstance = 0;
  }
}

//_____________________________________________________________________________
IlcDCHCommonParam::IlcDCHCommonParam()
{
  //
  // constructor
  //
  
  fField              = 0.0;

  fExBOn              = kFALSE;
  
  fPadPlaneArray      = 0;
  
  Init();
}

//_____________________________________________________________________________
void IlcDCHCommonParam::Init()
{
  //
  // constructor helper
  //
  
  // E x B effects
  fExBOn          = kTRUE;

  // The magnetic field strength in Tesla
  Double_t x[3] = { 0.0, 0.0, 0.0 };
  Double_t b[3];
  gIlc->Field(x,b);  // b[] is in kilo Gauss
  fField = b[2] * 0.1; // Tesla
  
  // ----------------------------------------------------------------------------
  // The pad planes
  // ----------------------------------------------------------------------------
  
  fPadPlaneArray = new TObjArray(kNplan * kNcham);
  
  for (Int_t iplan = 0; iplan < kNplan; iplan++) {
    for (Int_t icham = 0; icham < kNcham; icham++) {
      Int_t ipp = iplan + icham * kNplan;
      fPadPlaneArray->AddAt(new IlcDCHpadPlane(iplan,icham),ipp);
    }
  }
}

//_____________________________________________________________________________
IlcDCHCommonParam::~IlcDCHCommonParam() 
{
  //
  // destructor
  //
  
  if (fPadPlaneArray) {
    fPadPlaneArray->Delete();
    delete fPadPlaneArray;
    fPadPlaneArray = 0;
  }
}

//_____________________________________________________________________________
IlcDCHCommonParam::IlcDCHCommonParam(const IlcDCHCommonParam &p):TObject(p)
{
  //
  // copy constructor
  //

  ((IlcDCHCommonParam &) p).Copy(*this);
}


//_____________________________________________________________________________
IlcDCHCommonParam &IlcDCHCommonParam::operator=(const IlcDCHCommonParam &p)
{
  //
  // Assignment operator
  //

  if (this != &p) ((IlcDCHCommonParam &) p).Copy(*this);
  return *this;
}

//_____________________________________________________________________________
void IlcDCHCommonParam::Copy(TObject &p) const
{
  //
  // Copy function
  //
  
  IlcDCHCommonParam* target = dynamic_cast<IlcDCHCommonParam*> (&p);
  if (!target)
    return;
  
  target->fExBOn              = fExBOn;
  target->fField              = fField;
}

//_____________________________________________________________________________
IlcDCHpadPlane *IlcDCHCommonParam::GetPadPlane(Int_t p, Int_t c) const
{
  //
  // Returns the pad plane for a given plane <p> and chamber <c> number
  //

  Int_t ipp = p + c * kNplan;
  return ((IlcDCHpadPlane *) fPadPlaneArray->At(ipp));

}

//_____________________________________________________________________________
Int_t IlcDCHCommonParam::GetRowMax(Int_t p, Int_t c, Int_t /*s*/) const
{
  //
  // Returns the number of rows on the pad plane
  //

  return GetPadPlane(p,c)->GetNrows();

}

//_____________________________________________________________________________
Int_t IlcDCHCommonParam::GetColMax(Int_t p) const
{
  //
  // Returns the number of rows on the pad plane
  //

  return GetPadPlane(p,0)->GetNcols();

}

//_____________________________________________________________________________
Double_t IlcDCHCommonParam::GetRow0(Int_t p, Int_t c, Int_t /*s*/) const
{
  //
  // Returns the position of the border of the first pad in a row
  //

  return GetPadPlane(p,c)->GetRow0();

}

//_____________________________________________________________________________
Double_t IlcDCHCommonParam::GetCol0(Int_t p) const
{
  //
  // Returns the position of the border of the first pad in a column
  //

  return GetPadPlane(p,0)->GetCol0();

}

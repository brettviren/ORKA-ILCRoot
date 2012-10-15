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

/* $Id: IlcDCHarrayI.cxx,v 1.1.1.1 2008/03/11 14:53:01 vitomeg Exp $ */

///////////////////////////////////////////////////////////////////////
//                                                                   //  
// Added additional functionilcty to the original TArrayI.           //
//  - Multiple inheritance from TObject                              //
//  - Function Expand() allows to expand the array without           //
//    deleting the array contents                                    //
//                                                                   //
// Origin:  Marian Ivanov, Uni. of Bratislava, ivanov@fmph.uniba.sk  // 
//                                                                   //  
///////////////////////////////////////////////////////////////////////

#include "IlcDCHarrayI.h"

ClassImp(IlcDCHarrayI)

//_____________________________________________________________________________
IlcDCHarrayI::IlcDCHarrayI():TArrayI()
{
  //
  // Default constructor
  //

}

//_____________________________________________________________________________
IlcDCHarrayI::~IlcDCHarrayI()
{
  //
  // Default destructor
  //

}

//_____________________________________________________________________________
void IlcDCHarrayI::Copy(TObject &a) const
{
  //
  // Copy function
  //

  TObject::Copy(a);
  TArrayI::Copy(((TArrayI &) a));

}

//_____________________________________________________________________________
void IlcDCHarrayI::Expand(Int_t n)
{
  //
  // Sets the  array size of the TArrayI object to <n> integers and copies
  // the old array.
  // If n < 0 leave the array unchanged.
  // The user is responsible for the appropriate size of the array.
  //

  if (n < 0) return;  
  fArray = (Int_t*) TStorage::ReAlloc(fArray
                                     ,n  * sizeof(Int_t)
                                     ,fN * sizeof(Int_t));
  if (fArray != 0) fN = n;
 
}


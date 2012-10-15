/**************************************************************************
 * 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * Author: The International Lepton Collider Off-line Project.            *
 *                                                                        *
 * Part of the code has been developed by ILC Off-line Project.         *
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

//-------------------------------------------------------------------------
//     Base class for ESD and AOD VZERO data
//     Author: Cvetan Cheshkov
//     cvetan.cheshkov@cern.ch 2/02/2011
//-------------------------------------------------------------------------

#include "IlcVVZERO.h"
#include "IlcLog.h"

ClassImp(IlcVVZERO)

//__________________________________________________________________________
IlcVVZERO::IlcVVZERO(const IlcVVZERO& source) :
  TObject(source) { } // Copy constructor

//__________________________________________________________________________
IlcVVZERO& IlcVVZERO::operator=(const IlcVVZERO& source)
{
  // Assignment operator
  //
  if (this!=&source) { 
    TObject::operator=(source); 
  }
  
  return *this; 
}

//__________________________________________________________________________
Bool_t IlcVVZERO::OutOfRange(Int_t i, const char* s, Int_t upper) const
{
  // checks if i is a valid index.
  // s = name of calling method
  if (i > upper || i < 0) {
    IlcInfo(Form("%s: Index %d out of range",s,i));
    return kTRUE;
  }
  return kFALSE;
}

// $Id: IlcEveMagField.cxx 31761 2009-03-31 18:26:37Z mtadel $
// Author: Matevz Tadel 2009

/**************************************************************************
 * 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * Author: The International Lepton Collider Off-line Project.            *
 *                                                                        *
 * Part of the code has been developed by ILC Off-line Project.         *
 * Contributors are mentioned in the code where appropriate.              *
 * See http://ilcinfo.cern.ch/Offline/IlcRoot/License.html for          *
 * full copyright notice.                                                 *
 **************************************************************************/

#include "IlcEveMagField.h"
#include "IlcEveEventManager.h"

#include <IlcMagF.h>

//______________________________________________________________________________
// Full description of IlcEveMagField
//

ClassImp(IlcEveMagField)

//______________________________________________________________________________
IlcEveMagField::IlcEveMagField(IlcMagF* mf) :
  TEveMagField(),
  fField(mf)
{
  // Constructor.

  if (fField == 0)
  {
    fField = IlcEveEventManager::AssertMagField();
  }
}

//______________________________________________________________________________
TEveVector IlcEveMagField::GetField(Float_t x, Float_t y, Float_t z) const
{
  // Return magnetic field at requested point.

  Double_t rb[3] = { x, y, z };
  Double_t bb[3];

  fField->Field(rb, bb);

  TEveVector b(bb);
  b *= -0.1f;
  return b;
}

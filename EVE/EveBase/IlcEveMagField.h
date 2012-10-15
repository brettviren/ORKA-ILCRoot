// $Id: IlcEveMagField.h 31764 2009-04-01 10:40:24Z mtadel $
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

#ifndef IlcEveMagField_H
#define IlcEveMagField_H

#include "TEveTrackPropagator.h"

class IlcMagF;

//______________________________________________________________________________
// Short description of IlcEveMagField
//

class IlcEveMagField : public TEveMagField
{
public:
  IlcEveMagField(IlcMagF* mf=0);
  virtual ~IlcEveMagField() {}

  using TEveMagField::GetField;
  virtual TEveVector GetField(Float_t x, Float_t y, Float_t z) const;

protected:
  IlcMagF *fField; //! Pointer to the magnetic field.

private:
  IlcEveMagField(const IlcEveMagField&);            // Not implemented
  IlcEveMagField& operator=(const IlcEveMagField&); // Not implemented

  ClassDef(IlcEveMagField, 0); // Short description.
};

#endif

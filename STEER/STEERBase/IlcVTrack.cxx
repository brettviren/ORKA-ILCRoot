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
//     base class for ESD and AOD tracks
//     Author: A. Dainese
//-------------------------------------------------------------------------

#include <TGeoGlobalMagField.h>

#include "IlcMagF.h"
#include "IlcVTrack.h"

ClassImp(IlcVTrack)

IlcVTrack::IlcVTrack(const IlcVTrack& vTrack) :
  IlcVParticle(vTrack) { } // Copy constructor

IlcVTrack& IlcVTrack::operator=(const IlcVTrack& vTrack)
{ if (this!=&vTrack) { 
    IlcVParticle::operator=(vTrack); 
  }
  
  return *this; 
}

Double_t IlcVTrack::GetBz() const 
{
  // returns Bz component of the magnetic field (kG)
  IlcMagF* fld = (IlcMagF*)TGeoGlobalMagField::Instance()->GetField();
  if (!fld) return 0.5*kAlmost0Field;
  double bz;
  if (fld->IsUniform()) bz = fld->SolenoidField();
  else {
    Double_t r[3]; 
    GetXYZ(r); 
    bz = fld->GetBz(r);
  }
  return TMath::Sign(0.5*kAlmost0Field,bz) + bz;
}

void IlcVTrack::GetBxByBz(Double_t b[3]) const 
{
  // returns the Bx, By and Bz components of the magnetic field (kG)
  IlcMagF* fld = (IlcMagF*)TGeoGlobalMagField::Instance()->GetField();
  if (!fld) {
     b[0] = b[1] = 0.;
     b[2] = 0.5*kAlmost0Field;
     return;
  }

  if (fld->IsUniform()) {
     b[0] = b[1] = 0.;
     b[2] = fld->SolenoidField();
  }  else {
     Double_t r[3]; GetXYZ(r);
     fld->Field(r,b);
  }
  b[2] = (TMath::Sign(0.5*kAlmost0Field,b[2]) + b[2]);
  return;
}


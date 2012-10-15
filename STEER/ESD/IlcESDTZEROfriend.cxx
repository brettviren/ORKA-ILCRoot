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

///////////////////////////////////////////////////////////////////////////////
///
/// This is a class for containing T0 time corrected by SPD vertex and amplitude
///
///////////////////////////////////////////////////////////////////////////////

#include "IlcESDTZEROfriend.h"

ClassImp(IlcESDTZEROfriend)

//_____________________________________________________________________________
IlcESDTZEROfriend::IlcESDTZEROfriend():
  TObject()
{
  for(int i = 0;i<24;i++)fT0time[i] = fT0ampQTC[i] = fT0ampLEDminCFD[i] = 0;
} 
IlcESDTZEROfriend::IlcESDTZEROfriend(const IlcESDTZEROfriend &tzero ) :
  TObject(tzero)
{
  // copy constuctor
  for(int i = 0;i<24;i++){
    fT0time[i] = tzero.fT0time[i]; 
    fT0ampQTC[i] = tzero.fT0ampQTC[i];
    fT0ampLEDminCFD[i] = tzero.fT0ampLEDminCFD[i];
  }
}

IlcESDTZEROfriend& IlcESDTZEROfriend::operator=(const IlcESDTZEROfriend& tzero){
  // assigmnent operator
  if(this!=&tzero) {
    TObject::operator=(tzero);
    for(int i = 0;i<24;i++){
      fT0time[i] = tzero.fT0time[i]; 
      fT0ampQTC[i] = tzero.fT0ampQTC[i];
      fT0ampLEDminCFD[i] = tzero.fT0ampLEDminCFD[i];
    }
  } 
  return *this;
}

void IlcESDTZEROfriend::Copy(TObject &obj) const {
  
  // this overwrites the virtual TOBject::Copy()
  // to allow run time copying without casting
  // in IlcESDEvent

  if(this==&obj)return;
  IlcESDTZEROfriend *robj = dynamic_cast<IlcESDTZEROfriend*>(&obj);
  if(!robj)return; // not an IlcESDTZEROfriend
  *robj = *this;

}
void IlcESDTZEROfriend::Reset()
{
  // Reset the contents of the object
    for(int i = 0;i<24;i++)
      fT0time[i]= fT0ampQTC[i] = fT0ampLEDminCFD[i] =0 ;
    
}



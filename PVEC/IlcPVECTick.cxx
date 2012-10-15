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

/* $Id: IlcPVECTick.cxx 15036 2006-09-13 07:31:01Z kharlov $ */

//_________________________________________________________________________
// Auxiliary class to help calculate the time of crossing 
// of the threshold by the front edge of the time signal
//
//*-- Author :  Dmitri Peressounko (SUBATECH) 
//////////////////////////////////////////////////////////////////////////////

// --- ROOT system ---

// --- Standard library ---

// --- IlcRoot header files ---
#include "IlcPVECTick.h"

ClassImp(IlcPVECTick)


//____________________________________________________________________________ 
IlcPVECTick::IlcPVECTick():
  fTime(0),
  fA(0),
  fB(0)
{
}

//____________________________________________________________________________ 
IlcPVECTick::IlcPVECTick(Float_t time, Float_t a, Float_t slope):
  fTime(time),
  fA(a),
  fB(slope)
{
}

//____________________________________________________________________________ 
Int_t IlcPVECTick::Compare(const TObject * obj) const {
  if(obj->InheritsFrom("IlcPVECTick")){
    IlcPVECTick * tick = (IlcPVECTick *) obj ;
    if(fTime < tick->fTime)
      return -1 ;
    else
      if(fTime == tick->fTime)
	return 0 ;
      else
	return 1 ;
  }
  else
    return 1 ;
} 
//____________________________________________________________________________
void IlcPVECTick::operator+=(IlcPVECTick const & tick) 
{
  // Adds the amplitude of digits and completes the list of primary particles
  // if amplitude is larger than 
    
  fA = fA + fB*(tick.fTime - fTime) + tick.fA ;
  fB = fB + tick.fB ;
  if(tick.fTime > fTime) 
    fTime = tick.fTime ;
  
}

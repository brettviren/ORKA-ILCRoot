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
//     Container class for AOD TZERO data
//     Author: Filip Krizek 
//     filip.krizek@cern.ch 23/02/2012
//-------------------------------------------------------------------------

#include "IlcAODTZERO.h"
#include "IlcLog.h"

ClassImp(IlcAODTZERO)

//__________________________________________________________________________
IlcAODTZERO::IlcAODTZERO()
  :TObject(),
   fPileup(0),
   fSattelite(0),
   fBackground(0),
   fT0VertexRaw(999)
{   
  // Default constructor 
  for(Int_t j=0; j<3; j++){ 
    fT0TOF[j]     = 999;
    fT0TOFbest[j] = 999;
  }
}

//__________________________________________________________________________
IlcAODTZERO::IlcAODTZERO(const IlcAODTZERO &source)
  :TObject(source),
   fPileup(source.fPileup),
   fSattelite(source.fSattelite),
   fBackground(source.fBackground),
   fT0VertexRaw(source.fT0VertexRaw)
{   
  // Default constructor 
  for(Int_t j=0; j<3; j++) {
    fT0TOF[j]     = source.fT0TOF[j];
    fT0TOFbest[j] = source.fT0TOFbest[j];
  }
}

//__________________________________________________________________________
IlcAODTZERO& IlcAODTZERO::operator=(const IlcAODTZERO& source)
{
  // Assignment operator
  //
  if(this==&source) return *this;
  // Assignment operator
  fPileup      = source.fPileup;
  fSattelite   = source.fSattelite;
  fBackground  = source.fBackground;
  fT0VertexRaw = source.fT0VertexRaw;

  for(Int_t j=0; j<3; j++){
    fT0TOF[j]     = source.fT0TOF[j];
    fT0TOFbest[j] = source.fT0TOFbest[j];
  }
  return *this;
}


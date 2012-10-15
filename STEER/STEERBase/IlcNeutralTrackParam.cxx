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
//                                                                           //
// Implementation of the "neutral" track parameterisation class.             //
//                                                                           //
// At the moment we use a standard IlcExternalTrackParam with 0 curvature.   //
//                                                                           //
//        Origin: A.Dainese, I.Belikov                                       //
///////////////////////////////////////////////////////////////////////////////

#include "IlcExternalTrackParam.h"
#include "IlcNeutralTrackParam.h"

ClassImp(IlcNeutralTrackParam)
 
//_____________________________________________________________________________
IlcNeutralTrackParam::IlcNeutralTrackParam() :
IlcExternalTrackParam()
{
  //
  // default constructor
  //
}

//_____________________________________________________________________________
IlcNeutralTrackParam::IlcNeutralTrackParam(const IlcNeutralTrackParam &track):
IlcExternalTrackParam(track)
{
  //
  // copy constructor
  //
}

//_____________________________________________________________________________
IlcNeutralTrackParam& IlcNeutralTrackParam::operator=(const IlcNeutralTrackParam &trkPar)
{
  //
  // assignment operator
  //
  
  if (this!=&trkPar) {
    IlcExternalTrackParam::operator=(trkPar);
  }

  return *this;
}

//_____________________________________________________________________________
IlcNeutralTrackParam::IlcNeutralTrackParam(Double_t x, Double_t alpha, 
					     const Double_t param[5], 
					     const Double_t covar[15]) :
IlcExternalTrackParam(x,alpha,param,covar)
{
  //
  // create external track parameters from given arguments
  //
}

//_____________________________________________________________________________
IlcNeutralTrackParam::IlcNeutralTrackParam(const IlcVTrack *vTrack) :
IlcExternalTrackParam(vTrack)
{
  //
  // Constructor from virtual track,
  // This is not a copy contructor !
  //

}

//_____________________________________________________________________________
IlcNeutralTrackParam::IlcNeutralTrackParam(Double_t xyz[3],Double_t pxpypz[3],
					     Double_t cv[21],Short_t sign) :
IlcExternalTrackParam(xyz,pxpypz,cv,sign)
{
  //
  // constructor from the global parameters
  //
}


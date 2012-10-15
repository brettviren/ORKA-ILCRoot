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
//
// Origin Marian.Ivanov@cern.ch                                                                 //
// Base Class for detector Reconstruction Parameters                         //
//                                                                           //  
// 
// IlcDetectorRecoParam are identified according fEventSpecie - event specie(s)
// for which the object is valid.  
// The owner of the IlcDetectorRecoParam is the IlcRecoParam
// More than one RecoParam per recon can be registered.
//          
//  
//
///////////////////////////////////////////////////////////////////////////////


#include "IlcDetectorRecoParam.h"

ClassImp(IlcDetectorRecoParam)


IlcDetectorRecoParam::IlcDetectorRecoParam():
  TNamed(),
fEventSpecie(IlcRecoParam::kDefault)
{
  //
  // default constructor
  //
}

IlcDetectorRecoParam::~IlcDetectorRecoParam(){
  //
  // destructor
  //
}


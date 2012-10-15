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
 
#include "IlcTARGETRawClusterSSD.h"

////////////////////////////////////////////////////
//  Cluster classes for set:TARGET                   //
//  Raw Clusters for SSD                          //
//                                                //
////////////////////////////////////////////////////

ClassImp(IlcTARGETRawClusterSSD)

//______________________________________________________________________
IlcTARGETRawClusterSSD::IlcTARGETRawClusterSSD(){
  // Default constructor
  fMultiplicityN=0;
  fQErr=0; 
  fSignalP=0;
  fSignalN=0;
  fStatus=-1;
  fNtracks=0;
}
//______________________________________________________________________
IlcTARGETRawClusterSSD::IlcTARGETRawClusterSSD(Float_t Prob,Int_t Sp,Int_t Sn) {  
    // constructor

    Prob = 0.0; // added to remove unused variable warning.
    //fProbability   = Prob;
    fMultiplicity  = Sp;
    fMultiplicityN = Sn;
}

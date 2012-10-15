#ifndef ILCTARGETRECO_H
#define ILCTARGETRECO_H
/* Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 * See cxx source for full Copyright notice                               */

//-------------------------------------------------------------------------
//                   TARGET reconstruction name space
//
//       Origin: Iouri Belikov, CERN, Jouri.Belikov@cern.ch 
//-------------------------------------------------------------------------
#include <Rtypes.h>
#include "IlcESDfriendTrack.h"

//namespace IlcTARGETreco {    
   const Int_t kMaxClusterPerLayer=7000*10;
   const Int_t kMaxClusterPerLayer5=7000*10*2/5;
   const Int_t kMaxClusterPerLayer10=7000*10*2/10;
   const Int_t kMaxClusterPerLayer20=7000*10*2/20;
   const Int_t kMaxDetectorPerLayer=1000;

   const Int_t kLayersNotToSkip[40]={
     0,0,0,0,0,0,0,0,0,0,
     0,0,0,0,0,0,0,0,0,0,
     0,0,0,0,0,0,0,0,0,0,
     0,0,0,0,0,0,0,0,0,0};     

   const Int_t kLastLayerToTrackTo=0;
   const Int_t kMaxLaddersSkipped=4;

   const Int_t kMaxLayer  = IlcESDfriendTrack::kMaxVXDcluster;
   const Int_t kMaxClusters  = IlcESDfriendTrack::kMaxVXDcluster;
   const Double_t kMaxSnp = 30.;
   const Double_t kSigmaY2[40]={
     1.44e-6, 1.44e-6, 1.44e-6, 1.44e-6, 1.44e-6,
     1.44e-6, 1.44e-6, 1.44e-6, 1.44e-6, 1.44e-6,
     1.44e-6, 1.44e-6, 1.44e-6, 1.44e-6, 1.44e-6,
     1.44e-6, 1.44e-6, 1.44e-6, 1.44e-6, 1.44e-6,
     1.44e-6, 1.44e-6, 1.44e-6, 1.44e-6, 1.44e-6,
     1.44e-6, 1.44e-6, 1.44e-6, 1.44e-6, 1.44e-6,
     1.44e-6, 1.44e-6, 1.44e-6, 1.44e-6, 1.44e-6,
     1.44e-6, 1.44e-6, 1.44e-6, 1.44e-6, 1.44e-6
   };
   const Double_t kSigmaZ2[40]={
     1.44e-6, 1.44e-6, 1.44e-6, 1.44e-6, 1.44e-6,
     1.44e-6, 1.44e-6, 1.44e-6, 1.44e-6, 1.44e-6,
     1.44e-6, 1.44e-6, 1.44e-6, 1.44e-6, 1.44e-6,
     1.44e-6, 1.44e-6, 1.44e-6, 1.44e-6, 1.44e-6,
     1.44e-6, 1.44e-6, 1.44e-6, 1.44e-6, 1.44e-6,
     1.44e-6, 1.44e-6, 1.44e-6, 1.44e-6, 1.44e-6,
     1.44e-6, 1.44e-6, 1.44e-6, 1.44e-6, 1.44e-6,
     1.44e-6, 1.44e-6, 1.44e-6, 1.44e-6, 1.44e-6
   };

   const Double_t kChi2PerCluster=9.;
   const Double_t kMaxChi2PerCluster[40]={
     25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,
     25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,
     25,25,25,25,25,25,25,25,25,25
   };
   const Double_t kMaxNormChi2NonC[40]  = {
     25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,
     25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,
     25,25,25,25,25,25,25,25,25,25
   };  //max norm chi2 for non constrained tracks
   const Double_t kMaxNormChi2C[40]  = {
     25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,
     25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,
     25,25,25,25,25,25,25,25,25,25
   };     //max norm chi2 for constrained tracks

   const Double_t kMaxChi2=35.;
//   const Double_t kMaxChi2s[6]={40,40,40,40,40,40};   
   const Double_t kMaxChi2s[40]={
     25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,
     25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,
     25,25,25,25,25,25,25,25,25,25
   };   
   const Double_t kMaxChi2sR[40]={
     25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,
     25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,
     25,25,25,25,25,25,25,25,25,25
   };   
   const Double_t kMaxChi2In=25.;
   const Double_t kVertexCut=25;
   const Double_t kMaxRoad=6.0;

   const Double_t kXV=0.0e+0;
   const Double_t kYV=0.0e+0;
   const Double_t kZV=0.0e+0;
   const Double_t kSigmaXV=0.005e+0;
   const Double_t kSigmaYV=0.005e+0;
   const Double_t kSigmaZV=0.010e+0;
//}

//using namespace IlcTARGETreco;

#endif

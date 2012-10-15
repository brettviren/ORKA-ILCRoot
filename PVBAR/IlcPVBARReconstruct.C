// YS Subatech Mai 2002
// YK Subatech 6 Aug 2002

// PVBAR Reconstruction chain:
// Hits -> SDigits -> Digits -> RecPoints -> TrackSegments -> RecParticles

//Root
#if !defined(__CINT__) || defined(__MAKECINT__)
#include "TString.h"

//IlcRoot
#include "STEER/IlcRun.h"
#include "PVBAR/IlcPVBARSDigitizer.h"
#include "PVBAR/IlcPVBARDigitizer.h"
#include "PVBAR/IlcPVBARClusterizerv1.h"
#include "PVBAR/IlcPVBARTrackSegmentMakerv1.h"
#include "PVBAR/IlcPVBARPIDv1.h"
#include "EMCAL/IlcEMCALSDigitizer.h"
#include "EMCAL/IlcEMCALDigitizer.h"
#include "EMCAL/IlcEMCALClusterizerv1.h"
#endif

void PVBARHits2SDigits( Bool_t split=kFALSE, TString fileName = "gilc.root") {

  // usage : 
  // 1. write SDigits in the same file as Hits --------------- (OK)
  //root [0] .L Reconstruct.C++
  //root [1] SDigits2Digits()
  // 2. write SDigits in a separate file, one per detector, from Hits --------------- (OK)
  //root [0] .L Reconstruct.C++
  //root [1] SDigits2Digits(kTRUE) // SDigits saved in [DET}.SDigits.root (DET=PVBAR, EMCAL)

  delete gIlc ; 
  gIlc = 0 ; 
  
  IlcPVBARSDigitizer * sdp = new IlcPVBARSDigitizer(fileName) ; 
  if (split) 
    sdp->SetSplitFile() ;
  sdp->ExecuteTask("deb") ; 

  delete sdp ;
}

//________________________________________________________________________
void PVBARSDigits2Digits( Bool_t split=kFALSE, TString fileName = "gilc.root") {
  
 // usage : 
  // 1. write SDigits in the same file as SDigits --------------- (OK)
  //root [0] .L Reconstruct.C++
  //root [1] SDigits2Digits()
  // 2. write SDigits in a separate file, one per detector, from SDigits --------------- (OK)
  //root [0] .L Reconstruct.C++
  //root [1] SDigitsDigits(kTRUE) // Digits saved in [DET}.Digits.root (DET=PVBAR, EMCAL)

  delete gIlc ; 
  gIlc = 0 ; 
  
  // PVBAR
  IlcPVBARDigitizer * dp = 0 ; 
 
  if (split) {
    dp = new IlcPVBARDigitizer("PVBAR.SDigits.root") ; 
    dp->SetSplitFile() ; } 
  else 
    dp = new IlcPVBARDigitizer(fileName) ; 
  
  dp->ExecuteTask("deb") ; 
  
  delete dp ;
}

//________________________________________________________________________
void PVBARDigits2RecPoints( Bool_t split=kFALSE, TString fileName = "gilc.root") {
  
 // usage : 
  // 1. write RecPoints in the same file as Digits --------------- OK 
  //root [0] .L Reconstruct.C++
  //root [1] Digits2RecPoints()
  // 2. write RecPoints in a separate file, one per detector, from Digits --------------- OK 
  //root [0] .L Reconstruct.C++
  //root [1] Digits2RecPoints(kTRUE) // RecPoints saved in [DET}.RecPoints.root (DET=PVBAR, EMCAL)

  delete gIlc ; 
  gIlc = 0 ; 
 
  IlcPVBARClusterizer * cp = 0 ; 
 
  if (split) {
    cp = new IlcPVBARClusterizerv1("PVBAR.Digits.root") ; 
    cp->SetSplitFile() ; } 
  else 
    cp = new IlcPVBARClusterizerv1(fileName) ; 
  
  cp->ExecuteTask("deb") ; 
  
  delete cp ;
}

//________________________________________________________________________
void PVBARRecPoints2TrackSegments( Bool_t split=kFALSE, TString fileName = "gilc.root") {
  
 // usage : 
  // 1. write TrackSegments in the same file as RecPoints --------------- (OK) 
  //root [0] .L Reconstruct.C++
  //root [1] RecPoints2TrackSegments()
  // 2. write TrackSegments in a separate file, one per detector, from RecPoints --------------- (Not needed) 
  //root [0] .L Reconstruct.C++
  //root [1] RecPoints2TrackSegments(kTRUE) // TrackSegments saved in [DET}.RecData.root (DET=PVBAR, EMCAL)

  delete gIlc ; 
  gIlc = 0 ; 
  
  IlcPVBARTrackSegmentMaker * tmp = 0 ; 
 
  if (split)
    tmp = new IlcPVBARTrackSegmentMakerv1("PVBAR.RecData.root") ; 
  else 
    tmp = new IlcPVBARTrackSegmentMakerv1(fileName) ; 
  
  tmp->ExecuteTask("deb") ; 
  
  delete tmp ;
}

//________________________________________________________________________
void PVBARTrackSegments2RecParticles( Bool_t split=kFALSE, TString fileName = "gilc.root") {
  
 // usage : 
  // 1. write RecParticles in the same file as TrackSegments ---------------  (OK)
  //root [0] .L Reconstruct.C++
  //root [1] TrackSegments2RecParticles()
  // 2. write RecParticles in a separate file, one per detector, from TrackSegments --------------- (Not needed) 
  //root [0] .L Reconstruct.C++
  //root [1] TrackSegments2RecParticles(kTRUE) // RecParticles saved in [DET}.RecData.root (DET=PVBAR, EMCAL)

  delete gIlc ; 
  gIlc = 0 ; 
  
  IlcPVBARPID * pp = 0 ; 
 
  if (split) 
    pp = new IlcPVBARPIDv1("PVBAR.RecData.root") ; 
  else 
    pp = new IlcPVBARPIDv1(fileName) ; 
  
  pp->ExecuteTask("deb") ; 
  
  delete pp ;
}

//________________________________________________________________________
void PVBARDigits2RecParticles( Bool_t split=kFALSE, TString fileName = "gilc.root") {
  
 // usage : 
  // 1. write RecPoints, TrackSegments and RecParticles in the same file as Digits --------------- (OK)
  //root [0] .L Reconstruct.C++
  //root [1] Digits2RecParticles()
  // 2. write RecPoints , TrackSegments and RecParticles in a separate file, one per detector, from Digits --------------- (OK)
  //root [0] .L Reconstruct.C++
  //root [1] Digits2RecParticles(kTRUE) // TrackSegments saved in [DET}.RecData.root (DET=PVBAR, EMCAL)

 
  delete gIlc ; 
  gIlc = 0 ; 
 
  // PVBAR
  IlcPVBARClusterizer * cp = 0 ; 
 
  if (split) {
    cp = new IlcPVBARClusterizerv1("PVBAR.Digits.root") ; 
    cp->SetSplitFile() ; } 
  else 
    cp = new IlcPVBARClusterizerv1(fileName) ; 
  
  cp->ExecuteTask("deb") ; 

  if (split) 
    delete cp ;
  
  IlcPVBARTrackSegmentMaker * tmp = 0 ; 
  
  if (split) 
    tmp = new IlcPVBARTrackSegmentMakerv1("PVBAR.RecData.root") ; 
  else 
    tmp = new IlcPVBARTrackSegmentMakerv1(fileName) ; 
  
  tmp->ExecuteTask("deb") ; 
  
  IlcPVBARPID * pp = 0 ; 
 
  if (split) 
    pp = new IlcPVBARPIDv1("PVBAR.RecData.root") ; 
  else 
    pp = new IlcPVBARPIDv1(fileName) ; 
  
  pp->ExecuteTask("deb") ; 
  
  delete tmp; 
  delete pp ; 
}

//________________________________________________________________________
void PVBARHits2Digits (Bool_t split=kFALSE, TString fileName = "gilc.root") {
  // usage : 
  // 1. write (S)Digits in the same file as Hits --------------- (OK)
  //root [0] .L Reconstruct.C++
  //root [1] Hits2Digits()
  // 2. write (S)Digits in a separate file, one per detector, from Hits --------------- (OK)
  //root [0] .L Reconstruct.C++
  //root [1] Hits2Digits(kTRUE) // SDigits saved in [DET}.SDigits.root (DET=PVBAR, EMCAL)
                                // Digits  saved in [DET}.Digits.root  (DET=PVBAR, EMCAL)

  delete gIlc ; 
  gIlc = 0 ; 
  
  //PVBAR
  IlcPVBARSDigitizer * sdp = new IlcPVBARSDigitizer(fileName) ; 
  if (split) 
    sdp->SetSplitFile() ;
  sdp->ExecuteTask("deb") ; 

  if (split) 
    delete sdp ; 

  IlcPVBARDigitizer * dp = 0 ; 
 
  if (split) {
    dp = new IlcPVBARDigitizer("PVBAR.SDigits.root") ; 
    dp->SetSplitFile() ; } 
  else 
    dp = new IlcPVBARDigitizer(fileName) ; 
  
  dp->ExecuteTask("deb") ; 

  if (split) 
    delete dp ; 

  if (!split) { 
    delete sdp ; 
    delete dp ; 
  }
}


 

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
/* $Id: AnaESD.C 9754 2004-04-16 14:14:26Z schutz $ */
//_________________________________________________________________________
// Macros analyzing the ESD file
// Use Case : 
//          root> .L AnaESD.C++
//          root> ana() --> prints the objects stored in ESD
//                                              
// author  : Yves Schutz (CERN/SUBATECH)
// February 2004
//_________________________________________________________________________
#if !defined(__CINT__) || defined(__MAKECINT__)
#include "TFile.h"
#include "TMath.h"
#include "TH1D.h"
#include "TROOT.h"
#include "TBrowser.h"
#include "IlcPVBARGetter.h"
#include "IlcPVBARGeometry.h"
#include "Riostream.h"
#include "IlcESD.h"
#include "IlcESDtrack.h"
#include "IlcESDCaloTrack.h"
#include "IlcEMCALRecParticle.h"
#include "IlcPVBARRecParticle.h"
#include "IlcKalmanTrack.h"
#include "IlcPVBARGridFile.h"
#endif

Bool_t AnaESD(TString filename) ; 
void   Match(IlcESDtrack * ct, IlcESDtrack * cp, Double_t * dist) ; 


//__________________________________________________________________________
Bool_t Ana(const TString type = "per5", const Int_t run = 1, const Int_t nOfEvt = 1) 
{ 
  // Analyzes data from the AliEn data catalog

  // Data Challenge identification
  const TString kYear("2004") ; 
  const TString kProd("02") ; 
  const TString kVers("V4.01.Rev.00") ; 

  // get the LFN file name in the Grid catalogue ; 
  IlcPVBARGridFile lfn ; 
  if (!lfn.IsConnected()) 
    return kFALSE ; 
  lfn.SetPath(kYear, kProd, kVers, type) ;  
  lfn.SetRun(run) ; 

  //loop over the events 
  Int_t nevt, evt = 0 ; 
  for (nevt = 0 ; nevt < nOfEvt ; nevt++) { 
    evt++ ; 
    lfn.SetEvt(evt) ;
    TString fileName = lfn.GetLFN() ; 
    
    if (fileName.IsNull()) {
      nevt-- ; 
      continue ; 
    }

    printf(">>>>>>>>>>>> Processing %s-%s/%s/%s : run # %d event # %d \n", 
	   kYear.Data(), kProd.Data(), kVers.Data(), type.Data(), run, evt) ;
    AnaESD(fileName) ;
  } 
  return kTRUE ; 
}

//__________________________________________________________________________
Bool_t AnaESD(TString fileName) 
{
  // Analyze ESD from file fileName
  // calo track histograms ;
  
  TFile * out = new TFile("AOD.root", "RECREATE") ; 
  TH1D * hCaloEnergyA = 0 ; 
  TH1D * hCaloEnergyE = 0 ; 
  TH1D * hCaloEnergyG = 0 ; 
  TH1D * hCaloEnergyP = 0 ; 
  TH1D * heta = 0 ;
  TH1D * hphi = 0 ; 
  Double_t dist[3] ; 

  IlcPVBARGetter * gime = IlcPVBARGetter::Instance(fileName) ; 
  
  Int_t nEvent = gime->MaxEvent() ;  
  Int_t event ; 
  IlcESD * esd = 0 ;
  for (event = 0 ; event < nEvent; event++) {
    cout << "AnaESD : Processing event # " << event << endl ; 
    esd = gime->ESD(event) ; 
    if (!esd) 
      return kFALSE ; 
    
    //esd->Print();  
    // Calorimeter tracks 
    IlcESDtrack * ct ; 
    Int_t caloindex ; 
    for (caloindex = 0 ; caloindex < esd->GetNumberOfTracks() ; caloindex++) {
      // get the tracks and check if it is from PVBAR 
      ct = esd->GetTrack(caloindex) ;
      if ( !ct->IsPVBAR() ) 
	continue ; 
      Double_t energy = ct->GetPVBARsignal() ; 
      // check if CPV bit is set (see IlcPVBARFastRecParticle) 
      Double_t type[IlcESDtrack::kSPECIES+4] ;
      ct->GetPVBARpid(type) ; 
      
      if ( (type[IlcESDtrack::kElectron] == 1.0) || (type[IlcESDtrack::kPhoton] == 1.0) ) {
	if(!hCaloEnergyA) {
	  out->cd() ; 
	  hCaloEnergyA = new TH1D("CaloEnergyA", "Energy in calorimeter Electron/Photon like", 500, 0., 50.) ;
	}
	hCaloEnergyA->Fill(energy) ;  
      }
      
      if ( type[IlcESDtrack::kElectron] == 1.0 ) {
	if(!hCaloEnergyE) {
	  out->cd() ; 
	  hCaloEnergyE = new TH1D("CaloEnergyE", "Energy in calorimeter Electron like", 500, 0., 50.) ;
	}
	hCaloEnergyE->Fill(energy) ;  
      } 
  
      if ( type[IlcESDtrack::kPhoton] == 1.0 ) {
	if(!hCaloEnergyG) {
	  out->cd() ; 
	  hCaloEnergyG = new TH1D("CaloEnergyG", "Energy in calorimeter Gamma like", 500, 0., 50.) ;
	}
	hCaloEnergyG->Fill(energy) ;
      }  
     
      if ( type[IlcESDtrack::kPi0] == 1.0 ) {
	if(!hCaloEnergyP) {
	  out->cd() ; 
	  hCaloEnergyP = new TH1D("CaloEnergyP", "Energy in calorimeter Pi0 like", 500, 0., 100.) ;
	}
	hCaloEnergyP->Fill(energy) ;
      }  
      
      IlcESDtrack * cp ; 
      Int_t cpindex ; 
      for (cpindex = 0 ; cpindex < esd->GetNumberOfTracks() ; cpindex++) {
	// get the charged tracks from central tracking
	cp = esd->GetTrack(cpindex) ;
	if ( cp->IsPVBAR() ) 
	  continue ; 
	Match(ct, cp, dist) ; 
	if (!heta && !hphi) {
	  heta = new TH1D("Correta", "neutral-charged correlation in eta" , 100, 0., 360.) ; 
	  hphi = new TH1D("Corrphi", "neutral-charged correlation in phi" , 100, 0., 360.) ; 
	}
	heta->Fill(dist[1]) ; 
	hphi->Fill(dist[2]) ;
      }
    }
  }
  TBrowser * bs = new TBrowser("Root Memory Bowser", gROOT->FindObjectAny("AOD.root") ) ;
  bs->Show() ;
  out->Write() ; 
  return kTRUE ; 
}

//__________________________________________________________________________
void Match(IlcESDtrack * ct, IlcESDtrack * cp, Double_t * dist) 
{
  // Calculates the distance (x,z) between  the particle detected by PVBAR and 
  // the charged particle reconstructed by the global tracking 

      
 //  Int_t phN ; 
//   Double_t phZ, phX ; 
  
//   if (ct->IsPVBAR()) { // it is a PVBAR particle 
//     Double_t cpTheta,  cpPhi ;  
//     Double_t phTheta,  phPhi ; 
//     cpTheta = cpPhi = phTheta = phPhi = 0. ; 
//    //    cout << "PVBAR particle # " << " pos (" 
//     // 	 << pp->GetPos().X() << ", " << pp->GetPos().Y() << ", " << pp->GetPos().Z() << ")" << endl ;
    
//     IlcPVBARGetter * gime = IlcPVBARGetter::Instance() ; 
//     gime->PVBARGeometry()->ImpactOnEmc(*pp, phN, phZ, phX) ; 
//     Double_t xyzAtPVBAR[3] ; 
//     cp->GetOuterXYZ(xyzAtPVBAR) ; 
//     if ( (xyzAtPVBAR[0] +  xyzAtPVBAR[1] + xyzAtPVBAR[2]) != 0.) { //it has reached PVBAR
//       //the next check are only if we want high quality tracks 
//       //       ULong_t status = cp->GetStatus() ;  
//       //       if ((status & IlcESDtrack::kTRDput)==0) 
//       // 	do not continue;
//       //       if ((status & IlcESDtrack::kTRDStop)!=0) 
//       // 	do not continue;  
//       //       cout << "Charged particle # " << " pos (" 
//       // 	   << xyzAtPVBAR[0] << ", " << xyzAtPVBAR[1] << ", " << xyzAtPVBAR[2] << ")" <<  endl ;     
//       TVector3 poscp(xyzAtPVBAR[0], xyzAtPVBAR[1], xyzAtPVBAR[2]) ;
//       Int_t cpN ;
//       Double_t cpZ,cpX ; 
//       gime->PVBARGeometry()->ImpactOnEmc(poscp, cpN, cpZ, cpX) ; 
//       if (cpN) {// we are inside the PVBAR acceptance 
// 	// 	cout << "Charged Matching 1: " << cpN << " " << cpZ << " " << cpX << endl ; 
// 	// 	cout << "Charged Matching 2: " << phN << " " << phZ << " " << phX << endl ; 
// 	dist[0] = TMath::Sqrt( (cpZ-phZ)*(cpZ-phZ) + (cpX-phX)*(cpX-phX)) ;  
//       } 
//       phTheta = pp->Theta() ; 
//       phPhi   = pp->Phi() ;
//       TParticle tempo ; 
//       tempo.SetMomentum(xyzAtPVBAR[0], xyzAtPVBAR[1], xyzAtPVBAR[2], 0.) ;  
//       cpTheta = tempo.Theta() ; 
//       cpPhi   = tempo.Phi() ;
//       //cout << phTheta << " " << phPhi << " " << endl 
//       //cout <<	 cpTheta << " " << cpPhi-phPhi << " " << endl ; 
//     }
//     dist[1] = (phTheta - cpTheta)*TMath::RadToDeg() ; 
//     dist[2] = (phPhi - cpPhi)*TMath::RadToDeg() ; 
//   }
  
//   if (ep) {
//     //cout << "EMCAL particle # " << endl ; 
//   }
}

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
//_________________________________________________________________________
// Cheking PVBARHistos procedure of PVBAR
//*-- Author : Gines MARTINEZ  SUBATECH january 2000
//////////////////////////////////////////////////////////////////////////////

// --- ROOT system ---
#include "TCanvas.h"
#include "TFile.h"
#include "TH1.h"
#include "TPad.h"
#include "TTree.h"

// --- Standard library ---

#include <cstdio>


// --- IlcRoot header files ---
#include "IlcRun.h"
#include "TFile.h"
#include "IlcPVBARGeometry.h"
#include "IlcPVBARv0.h"
#include "IlcPVBARDigit.h"
#include "IlcPVBARRecPoint.h"
#include "IlcPVBAREmcRecPoint.h"
#include "IlcPVBARPpsdRecPoint.h"
#include "IlcPVBARClusterizerv1.h"
#include "IlcPVBARReconstructor.h"
#include "IlcPVBARTrackSegment.h"
#include "IlcPVBARTrackSegmentMakerv1.h"
#include "IlcPVBARPIDv1.h"
#include "PVBARHistos.h"


void PVBARHistos (Text_t* infile, Int_t nevent, Int_t Module)  
{
//========== Opening gilc.root file  
  TFile * file   = new TFile(infile); 
//========== Get IlcRun object from file 
  gIlc = (IlcRun*) file->Get("gIlc");//=========== Gets the PVBAR object and associated geometry from the file 

  IlcPVBARv0 * PVBAR  = (IlcPVBARv0 *)gIlc->GetDetector("PVBAR");
  IlcPVBARGeometry * Geom = IlcPVBARGeometry::GetInstance(PVBAR->GetGeometry()->GetName(),PVBAR->GetGeometry()->GetTitle());
//========== Creates the track segment maker
  IlcPVBARTrackSegmentMakerv1 * tracksegmentmaker = new IlcPVBARTrackSegmentMakerv1() ;
  //========== Creates the particle identifier
  IlcPVBARPIDv1 * particleidentifier = new IlcPVBARPIDv1 ;
  Info("PVBARHistos", "AnalyzeOneEvent > using particle identifier %s\n", particleidentifier->GetName() ) ; 
    
  TH1F * hEmcDigit       = new TH1F("hEmcDigit","hEmcDigit",1000,0.,5.);
  TH1F * hVetoDigit      = new TH1F("hVetoDigit","hVetoDigit",1000,0.,3.e-5);
  TH1F * hConvertorDigit = new TH1F("hConvertorDigit","hConvertorDigit",1000,0.,3.e-5);
  TH1F * hEmcCluster       = new TH1F("hEmcCluster","hEmcCluster",1000,-5.,5.);
  TH1F * hVetoCluster      = new TH1F("hVetoCluster","hVetoCluster",1000,0.,3.e-5);
  TH1F * hConvertorCluster = new TH1F("hConvertorCluster","hConvertorCluster",1000,0.,3.e-5);
  TH2F * hConvertorEmc = new TH2F("hConvertorEmc","hConvertorEmc",100,2.,3., 100, 0., 1.e-5);

  IlcPVBARDigit * digit ;

//========== Loop on events
  Int_t ievent;
  for(ievent=0;ievent<nevent; ievent++)
  { 
//========== Creates the Clusterizer
    IlcPVBARClusterizerv1 * clusterizer = new IlcPVBARClusterizerv1() ; 
    clusterizer->SetEmcEnergyThreshold(0.01) ; 
    clusterizer->SetEmcClusteringThreshold(0.1) ; 
    clusterizer->SetPpsdEnergyThreshold(0.00000005) ; 
    clusterizer->SetPpsdClusteringThreshold(0.00000005) ; 
    clusterizer->SetLocalMaxCut(0.03) ;
    clusterizer->SetCalibrationParameters(0., 0.00000001) ;

  //========== Creates the Reconstructioner  
    IlcPVBARReconstructor * Reconstructioner = new IlcPVBARReconstructor(clusterizer, tracksegmentmaker, particleidentifier) ;
     
    Info("PVBARHistos", "Event %d\n", ievent);

    Int_t RelId[4] ;
    //=========== Connects the various Tree's for evt
    gIlc->GetEvent(ievent);
    //=========== Gets the Digit TTree
    gIlc->TreeD()->GetEvent(0) ;     
    //=========== Gets the number of entries in the Digits array
    //    Int_t nId = PVBAR->Digits()->GetEntries();      
    TIter next(PVBAR->Digits()) ;
    Float_t Etot=0 ;
    Int_t nVeto=0 ;
    Int_t nConvertor=0 ;

    while( ( digit = (IlcPVBARDigit *)next() ) )
    {
       Etot+=clusterizer->Calibrate(digit->GetAmp()) ;
       Geom->AbsToRelNumbering(digit->GetId(), RelId) ;        

       if (clusterizer->IsInEmc(digit))
       {   hEmcDigit->Fill(clusterizer->Calibrate(digit->GetAmp())) ; }
       else    
       {  
	 if (RelId[1]==9) {nVeto++; hVetoDigit->Fill(clusterizer->Calibrate(digit->GetAmp()));} 
	 if (RelId[1]==25) {nConvertor++; hConvertorDigit->Fill(clusterizer->Calibrate(digit->GetAmp()));}
       }
    }
   
     PVBAR->Reconstruction(Reconstructioner); 
 
//=========== Cluster in Module
     TClonesArray * EmcRP = PVBAR->EmcClusters() ;
     TIter nextemc(EmcRP) ;
     IlcPVBAREmcRecPoint * emc ;
     Float_t Energy;
     while((emc = (IlcPVBAREmcRecPoint *)nextemc())) 
     {
       if ( emc->GetPVBARMod() == Module )
       {  
         Energy = emc->GetEnergy() ;
	 hEmcCluster->Fill(Energy); 
         printf("Energy of the EMC cluster is %f \n",Energy);
	 TClonesArray * PpsdRP = PVBAR->PpsdClusters() ;
	 TIter nextPpsd(PpsdRP) ;
	 IlcPVBARPpsdRecPoint * Ppsd ;
	 Float_t Energy2;
	 while((Ppsd = (IlcPVBARPpsdRecPoint *)nextPpsd())) 
	   {
	     if ( Ppsd->GetPVBARMod() == Module )
	       { 
		 Energy2 = Ppsd->GetEnergy() ;
		 
		 if (!Ppsd->GetUp()) hConvertorEmc->Fill(Energy,Energy2) ;
	       }
	   }
    
       }
     }
 
    //=========== Cluster in Module PPSD Down
     TClonesArray * PpsdRP = PVBAR->PpsdClusters() ;
     TIter nextPpsd(PpsdRP) ;
     IlcPVBARPpsdRecPoint * Ppsd ;
     while((Ppsd = (IlcPVBARPpsdRecPoint *)nextPpsd())) 
     {
       if ( Ppsd->GetPVBARMod() == Module )
       { 
         Energy = Ppsd->GetEnergy() ;

	 if (!Ppsd->GetUp()) hConvertorCluster->Fill(Energy) ;
	 if (Ppsd->GetUp()) hVetoCluster->Fill(Energy) ;
   
       }
     }
  } 

  TCanvas * cVetoDigit = new TCanvas("VetoDigit","VetoDigit");  
  hVetoDigit->Draw();
  TCanvas * cConvertorDigit = new TCanvas("ConvertorDigit","ConvertorDigit");  
  hConvertorDigit->Draw();
  TCanvas * cEmcDigit = new TCanvas("EmcDigit","EmcDigit");  
  hEmcDigit->Draw();
  TCanvas * cVetoCluster = new TCanvas("VetoCluster","VetoCluster");  
  hVetoCluster->Draw();
  TCanvas * cConvertorCluster = new TCanvas("ConvertorCluster","ConvertorCluster");  
  hConvertorCluster->Draw();
  TCanvas * cEmcCluster = new TCanvas("EmcCluster","EmcCluster");  
  hEmcCluster->Draw();
  TCanvas * cConvertorEmc = new TCanvas("ConvertorEmc","ConvertorEmc");  
  hConvertorEmc->Draw("col1");


}



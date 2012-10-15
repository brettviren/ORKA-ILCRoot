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
// Algorythm class to analyze RSTACKv1 events:
// Construct histograms and displays them.
// Use the macro EditorBar.C for best access to the functionnalities
//*--
//*-- Author: B. Polichtchouk (IHEP)
//////////////////////////////////////////////////////////////////////////////

// --- ROOT system ---

#include "TFile.h"
#include "TH1.h"
#include "TPad.h"
#include "TH2.h"
#include "TParticle.h"
#include "TClonesArray.h"
#include "TTree.h"
#include "TMath.h"
#include "TCanvas.h" 
#include "TStyle.h" 

// --- Standard library ---

// --- IlcRoot header files ---

#include "IlcRunLoader.h"
#include "IlcHeader.h"

// --- RSTACK header files ---
#include "IlcLog.h"
#include "IlcRSTACKIhepAnalyze.h"
#include "IlcRSTACKDigit.h"
#include "IlcRSTACKRecParticle.h"
#include "IlcRSTACKLoader.h"
#include "IlcRSTACKHit.h"
#include "IlcRSTACKImpact.h"
#include "IlcRSTACKvImpacts.h"
#include "IlcRSTACKCpvRecPoint.h"
#include "IlcRun.h"
#include "IlcRSTACKGeometry.h"
#include "IlcRSTACKEvalRecPoint.h"

ClassImp(IlcRSTACKIhepAnalyze)

//____________________________________________________________________________
IlcRSTACKIhepAnalyze::IlcRSTACKIhepAnalyze():
  fRunLoader(0),
  fFileName()
{
}

//____________________________________________________________________________
IlcRSTACKIhepAnalyze::IlcRSTACKIhepAnalyze(Text_t * name) : 
  fRunLoader(0),
  fFileName(name)
{
  // Constructor: open a header file
  fRunLoader = IlcRunLoader::Open(fFileName);
  if (fRunLoader == 0x0)
   {
     IlcFatal(Form("Can not load event from file %s",name));
   }
}

//____________________________________________________________________________
void IlcRSTACKIhepAnalyze::AnalyzeCPV1(Int_t Nevents)
{
  //
  // Analyzes CPV characteristics: resolutions, cluster multiplicity,
  // cluster lengths along Z and Phi.
  // Author: Yuri Kharlov
  // 9 October 2000
  // Modified by Boris Polichtchouk, 3.07.2001
  //

  // Book histograms

  TH1F *hDx   = new TH1F("hDx"  ,"CPV x-resolution@reconstruction",100,-5. , 5.);
  TH1F *hDz   = new TH1F("hDz"  ,"CPV z-resolution@reconstruction",100,-5. , 5.);
  TH1F *hChi2 = new TH1F("hChi2"  ,"Chi2/dof of one-gamma fit",30, 0. , 10.);
  TH1S *hNrp  = new TH1S("hNrp" ,"CPV rec.point multiplicity",      21,-0.5,20.5);
  TH1S *hNrpX = new TH1S("hNrpX","CPV rec.point Phi-length"  ,      21,-0.5,20.5);
  TH1S *hNrpZ = new TH1S("hNrpZ","CPV rec.point Z-length"    ,      21,-0.5,20.5);

  TH1F *hEg   = new TH1F("hEg"  ,"Energies of impacts",30,0.,6.);
  TH1F *hEr   = new TH1F("hEr"  ,"Amplitudes of rec. points",50,0.,20.);
  
  TList * fCpvImpacts ;
  TBranch * branchCPVimpacts;

  
  
  IlcRSTACKLoader* please = dynamic_cast<IlcRSTACKLoader*>(fRunLoader->GetLoader("RSTACKLoader"));
  if ( please == 0 )
   {
     IlcError(Form("Could not obtain the Loader object !"));
     return ;
   }

  IlcRSTACKGeometry * RSTACKgeom =  IlcRSTACKGeometry::GetInstance() ;

  IlcInfo(Form("Start CPV Analysis-1. Resolutions, cluster multiplicity and lengths")) ;
  for ( Int_t ievent=0; ievent<Nevents; ievent++) {  
    
    Int_t nTotalGen = 0;
    Int_t nChargedGen = 0;

    Int_t ntracks = gIlc->GetEvent(ievent);
    IlcInfo(Form(">>>>>>>Event %d .<<<<<<<", ievent)) ;
    
  /******************************************************************/
      TTree* treeH = please->TreeH();
      if (treeH == 0x0)
       {
        IlcError(Form("Can not get TreeH"));
         return;
       }
/******************************************************************/     

    // Get branch of CPV impacts
    if (! (branchCPVimpacts =treeH->GetBranch("RSTACKCpvImpacts")) ) {
      IlcWarning(Form("Couldn't find branch RSTACKCpvImpacts. Exit.")) ;
      return;
    }
 
    // Create and fill arrays of hits for each CPV module
      
    Int_t nOfModules = RSTACKgeom->GetNModules();
    TClonesArray **hitsPerModule = new TClonesArray *[nOfModules];
    Int_t iModule = 0; 	
    for (iModule=0; iModule < nOfModules; iModule++)
      hitsPerModule[iModule] = new TClonesArray("IlcRSTACKImpact",100);

    TClonesArray    *impacts;
    IlcRSTACKImpact   *impact;
    TLorentzVector   p;

    // First go through all primary tracks and fill the arrays
    // of hits per each CPV module

    for (Int_t itrack=0; itrack<ntracks; itrack++) {
      branchCPVimpacts ->SetAddress(&fCpvImpacts);
      branchCPVimpacts ->GetEntry(itrack,0);

      for (iModule=0; iModule < nOfModules; iModule++) {
	impacts = (TClonesArray *)fCpvImpacts->At(iModule);
	// Do loop over impacts in the module
	for (Int_t iImpact=0; iImpact<impacts->GetEntries(); iImpact++) {
	  impact=(IlcRSTACKImpact*)impacts->At(iImpact);
	  hEg->Fill(impact->GetMomentum().E());
	  TClonesArray &lhits = *(TClonesArray *)hitsPerModule[iModule];
	  if(IsCharged(impact->GetPid())) nChargedGen++;
	  new(lhits[hitsPerModule[iModule]->GetEntriesFast()]) IlcRSTACKImpact(*impact);
	}
      }
      fCpvImpacts->Clear();
    }
    for (iModule=0; iModule < nOfModules; iModule++) {
      Int_t nsum = hitsPerModule[iModule]->GetEntriesFast();
      printf("CPV module %d has %d impacts\n",iModule,nsum);
      nTotalGen += nsum;
    }

    // Then go through reconstructed points and for each find
    // the closeset hit
    // The distance from the rec.point to the closest hit
    // gives the coordinate resolution of the CPV

    fRunLoader->GetEvent(ievent);
    TIter nextRP(please->CpvRecPoints()) ;
    IlcRSTACKCpvRecPoint *cpvRecPoint ;
    Float_t xgen, ygen, zgen;
    while( ( cpvRecPoint = (IlcRSTACKCpvRecPoint *)nextRP() ) ) {
      
      Float_t chi2dof = ((IlcRSTACKEvalRecPoint*)cpvRecPoint)->Chi2Dof();
      hChi2->Fill(chi2dof);
      hEr->Fill(cpvRecPoint->GetEnergy());

      TVector3  locpos;
      cpvRecPoint->GetLocalPosition(locpos);
      Int_t RSTACKModule = cpvRecPoint->GetRSTACKMod();
      Int_t rpMult     = cpvRecPoint->GetMultiplicity();
      Int_t rpMultX, rpMultZ;
      cpvRecPoint->GetClusterLengths(rpMultX,rpMultZ);
      Float_t xrec  = locpos.X();
      Float_t zrec  = locpos.Z();
      Float_t dxmin = 1.e+10;
      Float_t dzmin = 1.e+10;
      Float_t r2min = 1.e+10;
      Float_t r2;

      Int_t nCPVhits  = (hitsPerModule[RSTACKModule-1])->GetEntriesFast();
      Float_t locImpX=1e10,locImpZ=1e10;         // local coords of closest impact
      Float_t gImpX=1e10, gImpZ=1e10,gImpY=1e10; // global coords of closest impact
      for (Int_t ihit=0; ihit<nCPVhits; ihit++) {
	impact = (IlcRSTACKImpact*)(hitsPerModule[RSTACKModule-1])->UncheckedAt(ihit);
	xgen   = impact->X();
	zgen   = impact->Z();
	ygen   = impact->Y();
      	
	//Transform to the local ref.frame
	Float_t phig = RSTACKgeom->GetRSTACKAngle(RSTACKModule);
	Float_t phi = TMath::Pi()/180*phig;
	Float_t distanceIPtoCPV = RSTACKgeom->GetIPtoOuterCoverDistance() -
	  (RSTACKgeom->GetFTPosition(1)+
	   RSTACKgeom->GetFTPosition(2)+
	   RSTACKgeom->GetCPVTextoliteThickness())/2;
	Float_t xoL,yoL,zoL ;
//  	xoL = xgen*TMath::Cos(phig)+ygen*TMath::Sin(phig) ;
//  	yoL = -xgen*TMath::Sin(phig)+ygen*TMath::Cos(phig) + distanceIPtoCPV;
	xoL = xgen*TMath::Cos(phi)-ygen*TMath::Sin(phi) ;
	yoL = xgen*TMath::Sin(phi)+ygen*TMath::Cos(phi) + distanceIPtoCPV;
	zoL = zgen;

	r2 = TMath::Power((xoL-xrec),2) + TMath::Power((zoL-zrec),2);
	if ( r2 < r2min ) {
	  r2min = r2;
	  dxmin = xoL - xrec;
	  dzmin = zoL - zrec;
	  locImpX = xoL;
	  locImpZ = zoL;
	  gImpX = xgen;
	  gImpZ = zgen;
	  gImpY = ygen;
	}
      }
      IlcInfo(Form("Impact global (X,Z,Y) = %f %f %f", gImpX, gImpZ, gImpY));
      IlcInfo(Form("Impact local (X,Z) = %f %f", locImpX, locImpZ));
      IlcInfo(Form("Reconstructed (X,Z) = %f %f", xrec, zrec));
      IlcInfo(Form("dxmin %f dzmin %f", dxmin, dzmin));
      hDx  ->Fill(dxmin);
      hDz  ->Fill(dzmin);
//        hDr  ->Fill(TMath::Sqrt(r2min));
      hNrp ->Fill(rpMult);
      hNrpX->Fill(rpMultX);
      hNrpZ->Fill(rpMultZ);
    }
    delete [] hitsPerModule;

    IlcInfo(Form("++++ Event %d : total %d impacts, %d charged impacts and %d  rec. points.", 
          ievent, nTotalGen, nChargedGen, please->CpvRecPoints()->GetEntries())) ;
  }
  // Save histograms

  Text_t outputname[80] ;
  snprintf(outputname,80,"%s.analyzed",GetFileName().Data());
  TFile output(outputname,"RECREATE");
  output.cd();
  
  // Plot histograms

  TCanvas *cpvCanvas = new TCanvas("Cpv1","CPV analysis-I",20,20,800,600);
  gStyle->SetOptStat(111111);
  gStyle->SetOptFit(1);
  gStyle->SetOptDate(1);
  cpvCanvas->Divide(3,3);

  cpvCanvas->cd(1);
  gPad->SetFillColor(10);
  hNrp->SetFillColor(16);
  hNrp->Draw();

  cpvCanvas->cd(2);
  gPad->SetFillColor(10);
  hNrpX->SetFillColor(16);
  hNrpX->Draw();

  cpvCanvas->cd(3);
  gPad->SetFillColor(10);
  hNrpZ->SetFillColor(16);
  hNrpZ->Draw();

  cpvCanvas->cd(4);
  gPad->SetFillColor(10);
  hDx->SetFillColor(16);
  hDx->Fit("gaus");
  hDx->Draw();

  cpvCanvas->cd(5);
  gPad->SetFillColor(10);
  hDz->SetFillColor(16);
  hDz->Fit("gaus");
  hDz->Draw();

  cpvCanvas->cd(6);
  gPad->SetFillColor(10);
  hChi2->SetFillColor(16);
  hChi2->Draw();

  cpvCanvas->cd(7);
  gPad->SetFillColor(10);
  hEg->SetFillColor(16);
  hEg->Draw();

  cpvCanvas->cd(8);
  gPad->SetFillColor(10);
  hEr->SetFillColor(16);
  hEr->Draw();

  cpvCanvas->Write(0,kOverwrite);

}


void IlcRSTACKIhepAnalyze::AnalyzeEMC1(Int_t Nevents)
{
  //
  // Analyzes Emc characteristics: resolutions, cluster multiplicity,
  // cluster lengths along Z and Phi.
  // Author: Boris Polichtchouk, 24.08.2001
  //

  // Book histograms

  TH1F *hDx   = new TH1F("hDx"  ,"EMC x-resolution@reconstruction",100,-5. , 5.);
  TH1F *hDz   = new TH1F("hDz"  ,"EMC z-resolution@reconstruction",100,-5. , 5.);
  TH1F *hChi2   = new TH1F("hChi2"  ,"Chi2/dof of one-gamma fit",30, 0. , 3.);
  TH1S *hNrp  = new TH1S("hNrp" ,"EMC rec.point multiplicity",      21,-0.5,20.5);
  TH1S *hNrpX = new TH1S("hNrpX","EMC rec.point Phi-length"  ,      21,-0.5,20.5);
  TH1S *hNrpZ = new TH1S("hNrpZ","EMC rec.point Z-length"    ,      21,-0.5,20.5);

  TList * fEmcImpacts ;
  TBranch * branchEMCimpacts;

  IlcRSTACKLoader* please = dynamic_cast<IlcRSTACKLoader*>(fRunLoader->GetLoader("RSTACKLoader"));
  if ( please == 0 )
   {
     IlcError(Form("Could not obtain the Loader object !"));
     return ;
   }

  IlcRSTACKGeometry * RSTACKgeom =  IlcRSTACKGeometry::GetInstance() ;

  IlcInfo(Form("Start EMC Analysis-1. Resolutions, cluster multiplicity and lengths"));
  for ( Int_t ievent=0; ievent<Nevents; ievent++) {  
    
    Int_t nTotalGen = 0;

    Int_t ntracks = gIlc->GetEvent(ievent);

    IlcInfo(Form(" >>>>>>>Event %d .<<<<<<<", ievent)) ;

    TTree* treeH = please->TreeH();
    if (treeH == 0x0)
     {
      IlcError(Form("Can not get TreeH"));
       return;
     }

    
    // Get branch of EMC impacts
    if (! (branchEMCimpacts =treeH->GetBranch("RSTACKEmcImpacts")) ) {
      IlcWarning(Form(" Couldn't find branch RSTACKEmcImpacts. Exit."));
      return;
    }
 
    // Create and fill arrays of hits for each EMC module
      
    Int_t nOfModules = RSTACKgeom->GetNModules();
    TClonesArray **hitsPerModule = new TClonesArray *[nOfModules];
    Int_t iModule = 0; 	
    for (iModule=0; iModule < nOfModules; iModule++)
      hitsPerModule[iModule] = new TClonesArray("IlcRSTACKImpact",100);

    TClonesArray    *impacts;
    IlcRSTACKImpact   *impact;
    TLorentzVector   p;

    // First go through all primary tracks and fill the arrays
    // of hits per each EMC module

    for (Int_t itrack=0; itrack<ntracks; itrack++) {
      branchEMCimpacts ->SetAddress(&fEmcImpacts);
      branchEMCimpacts ->GetEntry(itrack,0);

      for (iModule=0; iModule < nOfModules; iModule++) {
	impacts = (TClonesArray *)fEmcImpacts->At(iModule);
	// Do loop over impacts in the module
	for (Int_t iImpact=0; iImpact<impacts->GetEntries(); iImpact++) {
	  impact=(IlcRSTACKImpact*)impacts->At(iImpact);
	  TClonesArray &lhits = *(TClonesArray *)hitsPerModule[iModule];
	  new(lhits[hitsPerModule[iModule]->GetEntriesFast()]) IlcRSTACKImpact(*impact);
	}
      }
      fEmcImpacts->Clear();
    }
    for (iModule=0; iModule < nOfModules; iModule++) {
      Int_t nsum = hitsPerModule[iModule]->GetEntriesFast();
      printf("EMC module %d has %d hits\n",iModule,nsum);
      nTotalGen += nsum;
    }

    // Then go through reconstructed points and for each find
    // the closeset hit
    // The distance from the rec.point to the closest hit
    // gives the coordinate resolution of the EMC

    fRunLoader->GetEvent(ievent);
    TIter nextRP(please->EmcRecPoints()) ;
    IlcRSTACKEmcRecPoint *emcRecPoint ;
    Float_t xgen, ygen, zgen;
    while( ( emcRecPoint = (IlcRSTACKEmcRecPoint *)nextRP() ) ) {
      
      Float_t chi2dof = ((IlcRSTACKEvalRecPoint*)emcRecPoint)->Chi2Dof();
      hChi2->Fill(chi2dof);
      
      TVector3  locpos;
      emcRecPoint->GetLocalPosition(locpos);
      Int_t RSTACKModule = emcRecPoint->GetRSTACKMod();
      Int_t rpMult     = emcRecPoint->GetMultiplicity();
      Int_t rpMultX, rpMultZ;
      ((IlcRSTACKEvalRecPoint*)emcRecPoint)->GetClusterLengths(rpMultX,rpMultZ);
      Float_t xrec  = locpos.X();
      Float_t zrec  = locpos.Z();
      Float_t dxmin = 1.e+10;
      Float_t dzmin = 1.e+10;
      Float_t r2min = 1.e+10;
      Float_t r2;

      Int_t nEMChits  = (hitsPerModule[RSTACKModule-1])->GetEntriesFast();
      Float_t locImpX=1e10,locImpZ=1e10;         // local coords of closest impact
      Float_t gImpX=1e10, gImpZ=1e10,gImpY=1e10; // global coords of closest impact
      for (Int_t ihit=0; ihit<nEMChits; ihit++) {
	impact = (IlcRSTACKImpact*)(hitsPerModule[RSTACKModule-1])->UncheckedAt(ihit);
	xgen   = impact->X();
	zgen   = impact->Z();
	ygen   = impact->Y();
      
	
	//Transform to the local ref.frame
	Float_t phig = RSTACKgeom->GetRSTACKAngle(RSTACKModule);
	Float_t phi = TMath::Pi()/180*phig;
	Float_t distanceIPtoEMC = RSTACKgeom->GetIPtoCrystalSurface();
	Float_t xoL,yoL,zoL ;
//  	xoL = xgen*TMath::Cos(phig)+ygen*TMath::Sin(phig) ;
//  	yoL = -xgen*TMath::Sin(phig)+ygen*TMath::Cos(phig) + distanceIPtoEMC;
	xoL = xgen*TMath::Cos(phi)-ygen*TMath::Sin(phi) ;
	yoL = xgen*TMath::Sin(phi)+ygen*TMath::Cos(phi) + distanceIPtoEMC;
	zoL = zgen;

	r2 = TMath::Power((xoL-xrec),2) + TMath::Power((zoL-zrec),2);
	if ( r2 < r2min ) {
	  r2min = r2;
	  dxmin = xoL - xrec;
	  dzmin = zoL - zrec;
	  locImpX = xoL;
	  locImpZ = zoL;
	  gImpX = xgen;
	  gImpZ = zgen;
	  gImpY = ygen;
	}
      }
      IlcInfo(Form(" Impact global (X,Z,Y) = %f %f %f", gImpX, gImpZ, gImpY));
      IlcInfo(Form(" Impact local (X,Z) = %f %f", locImpX, locImpZ));
      IlcInfo(Form(" Reconstructed (X,Z) = %f %f", xrec, zrec));
      IlcInfo(Form(" dxmin %f dzmin %f", dxmin, dzmin)) ;
      hDx  ->Fill(dxmin);
      hDz  ->Fill(dzmin);
//        hDr  ->Fill(TMath::Sqrt(r2min));
      hNrp ->Fill(rpMult);
      hNrpX->Fill(rpMultX);
      hNrpZ->Fill(rpMultZ);
    }
    delete [] hitsPerModule;

    IlcInfo(Form("++++ Event %d : total  %d impacts,  %d Emc rec. points.", 
	 ievent, nTotalGen, please->EmcRecPoints()->GetEntriesFast())) ; 

  }
  // Save histograms

  Text_t outputname[80] ;
  snprintf(outputname,80,"%s.analyzed",GetFileName().Data());
  TFile output(outputname,"update");
  output.cd();
  
  // Plot histograms

  TCanvas *emcCanvas = new TCanvas("Emc1","EMC analysis-I",20,20,800,400);
  gStyle->SetOptStat(111111);
  gStyle->SetOptFit(1);
  gStyle->SetOptDate(1);
  emcCanvas->Divide(3,2);

  emcCanvas->cd(1);
  gPad->SetFillColor(10);
  hNrp->SetFillColor(16);
  hNrp->Draw();

  emcCanvas->cd(2);
  gPad->SetFillColor(10);
  hNrpX->SetFillColor(16);
  hNrpX->Draw();

  emcCanvas->cd(3);
  gPad->SetFillColor(10);
  hNrpZ->SetFillColor(16);
  hNrpZ->Draw();

  emcCanvas->cd(4);
  gPad->SetFillColor(10);
  hDx->SetFillColor(16);
  hDx->Fit("gaus");
  hDx->Draw();

  emcCanvas->cd(5);
  gPad->SetFillColor(10);
  hDz->SetFillColor(16);
  hDz->Fit("gaus");
  hDz->Draw();

  emcCanvas->cd(6);
  gPad->SetFillColor(10);
  hChi2->SetFillColor(16);
  hChi2->Draw();

  emcCanvas->Write(0,kOverwrite);
}

//____________________________________________________________________________
void IlcRSTACKIhepAnalyze::AnalyzeCPV2(Int_t Nevents)
{
  // CPV analysis - part II.
  // Ratio of combinatoric distances between generated
  // and reconstructed hits.
  // Author: Boris Polichtchouk (polishchuk@mx.ihep.su)
  // 24 March 2001


  TH1F* hDrijCPVr = new TH1F("Drij_cpv_r","Distance between reconstructed hits in CPV",140,0,50);
  TH1F* hDrijCPVg = new TH1F("Drij_cpv_g","Distance between generated hits in CPV",140,0,50);
  TH1F* hDrijCPVratio = new TH1F("Drij_cpv_ratio","R_{ij}^{rec}/R_{ij}^{gen} in CPV",140,0,50);

//    TH1F* hT0 = new TH1F("hT0","Type of entering particle",20000,-10000,10000);

  hDrijCPVr->Sumw2();
  hDrijCPVg->Sumw2();
  hDrijCPVratio->Sumw2(); //correct treatment of errors

  TList * fCpvImpacts = new TList();
  TBranch * branchCPVimpacts;

  IlcRSTACKLoader* please = dynamic_cast<IlcRSTACKLoader*>(fRunLoader->GetLoader("RSTACKLoader"));
  if ( please == 0 )
   {
     IlcError(Form("Could not obtain the Loader object !"));
     return ;
   }
  IlcRSTACKGeometry * RSTACKgeom =  IlcRSTACKGeometry::GetInstance() ;
  fRunLoader->LoadHeader();

  for (Int_t nev=0; nev<Nevents; nev++) 
    { 
      printf("\n=================== Event %10d ===================\n",nev);
      fRunLoader->GetEvent(nev);
      Int_t ntracks = fRunLoader->GetHeader()->GetNtrack();
    
      Int_t nRecCPV = 0; // Reconstructed points in event
      Int_t nGenCPV = 0; // Impacts in event

      // Get branch of CPV impacts
      TTree* treeH = please->TreeH();
      if (treeH == 0x0)
       {
	 IlcError(Form("Can not get TreeH"));
        return;
       }

      if (! (branchCPVimpacts =treeH->GetBranch("RSTACKCpvImpacts")) )  return;
      
      // Create and fill arrays of hits for each CPV module
      Int_t nOfModules = RSTACKgeom->GetNModules();
      TClonesArray **hitsPerModule = new TClonesArray *[nOfModules];
      Int_t iModule = 0; 	
      for (iModule=0; iModule < nOfModules; iModule++)
	hitsPerModule[iModule] = new TClonesArray("IlcRSTACKImpact",100);

      TClonesArray    *impacts;
      IlcRSTACKImpact   *impact;
          
      for (Int_t itrack=0; itrack<ntracks; itrack++) {
	branchCPVimpacts ->SetAddress(&fCpvImpacts);
	IlcInfo(Form(" branchCPVimpacts ->SetAddress(&fCpvImpacts) OK."));
	branchCPVimpacts ->GetEntry(itrack,0);

	for (iModule=0; iModule < nOfModules; iModule++) {
	  impacts = (TClonesArray *)fCpvImpacts->At(iModule);
	  // Do loop over impacts in the module
	  for (Int_t iImpact=0; iImpact<impacts->GetEntries(); iImpact++) {
	    impact=(IlcRSTACKImpact*)impacts->At(iImpact);
	    TClonesArray &lhits = *(TClonesArray *)hitsPerModule[iModule];
	    if(IsCharged(impact->GetPid()))
	      new(lhits[hitsPerModule[iModule]->GetEntriesFast()]) IlcRSTACKImpact(*impact);
	  }
	}
	fCpvImpacts->Clear();
      }

      for (iModule=0; iModule < nOfModules; iModule++) {
	Int_t nsum = hitsPerModule[iModule]->GetEntriesFast();
	printf("CPV module %d has %d hits\n",iModule,nsum);

        IlcRSTACKImpact* genHit1;
        IlcRSTACKImpact* genHit2;
        Int_t irp1,irp2;
	for(irp1=0; irp1< nsum; irp1++)
	  {
	    genHit1 = (IlcRSTACKImpact*)((hitsPerModule[iModule])->At(irp1));
	    for(irp2 = irp1+1; irp2<nsum; irp2++)
	      {
		genHit2 = (IlcRSTACKImpact*)((hitsPerModule[iModule])->At(irp2));
		Float_t dx = genHit1->X() - genHit2->X();
  		Float_t dz = genHit1->Z() - genHit2->Z();
		Float_t dr = TMath::Sqrt(dx*dx + dz*dz);
		hDrijCPVg->Fill(dr);
//      		IlcInfo(Form("(dx dz dr): %f %f", dx, dz));
	      }
	  }
      }

 
  //--------- Combinatoric distance between rec. hits in CPV

      TObjArray* cpvRecPoints = please->CpvRecPoints();
      nRecCPV =  cpvRecPoints->GetEntriesFast();

      if(nRecCPV)
	{
	  IlcRSTACKCpvRecPoint* recHit1;
	  IlcRSTACKCpvRecPoint* recHit2;
	  TIter nextCPVrec1(cpvRecPoints);
	  while(TObject* obj1 = nextCPVrec1() )
	    {
	      TIter nextCPVrec2(cpvRecPoints);
	      while (TObject* obj2 = nextCPVrec2())
		{
		  if(!obj2->IsEqual(obj1))
		    {
		      recHit1 = (IlcRSTACKCpvRecPoint*)obj1;
		      recHit2 = (IlcRSTACKCpvRecPoint*)obj2;
		      TVector3 locpos1;
		      TVector3 locpos2;
		      recHit1->GetLocalPosition(locpos1);
		      recHit2->GetLocalPosition(locpos2);
		      Float_t dx = locpos1.X() - locpos2.X();
		      Float_t dz = locpos1.Z() - locpos2.Z();		      
		      Float_t dr = TMath::Sqrt(dx*dx + dz*dz);
		      if(recHit1->GetRSTACKMod() == recHit2->GetRSTACKMod())
			hDrijCPVr->Fill(dr);
		    }
		}
	    }	
	}
      
      IlcInfo(Form(" Event %d . Total of %d hits, %d rec.points.", 
	   nev, nGenCPV, nRecCPV)) ; 
    
      delete [] hitsPerModule;

    } // End of loop over events.


//    hDrijCPVg->Draw();
//    hDrijCPVr->Draw();
  hDrijCPVratio->Divide(hDrijCPVr,hDrijCPVg);
  hDrijCPVratio->Draw();

//    hT0->Draw();

}


void IlcRSTACKIhepAnalyze::CpvSingle(Int_t nevents)
{
  // Distributions of coordinates and cluster lengths of rec. points
  // in the case of single initial particle.

  TH1F* hZr = new TH1F("Zrec","Reconstructed Z distribution",150,-5,5);
  TH1F* hXr = new TH1F("Xrec","Reconstructed X distribution",150,-14,-2);
  TH1F *hChi2   = new TH1F("hChi2"  ,"Chi2/dof of one-gamma fit",100, 0. , 20.);

  TH1S *hNrp  = new TH1S("hNrp" ,"CPV rec.point multiplicity",21,-0.5,20.5);
  TH1S *hNrpX = new TH1S("hNrpX","CPV rec.point Phi-length"  ,21,-0.5,20.5);
  TH1S *hNrpZ = new TH1S("hNrpZ","CPV rec.point Z-length"    ,21,-0.5,20.5);
 
  IlcRSTACKLoader* gime = dynamic_cast<IlcRSTACKLoader*>(fRunLoader->GetLoader("RSTACKLoader"));
  if ( gime == 0 )
   {
     IlcError(Form("Could not obtain the Loader object !"));
     return ;
   }
  
  for(Int_t ievent=0; ievent<nevents; ievent++)
    {
      fRunLoader->GetEvent(ievent);
      if(gime->CpvRecPoints()->GetEntriesFast()>1) continue;

      IlcRSTACKCpvRecPoint* pt = (IlcRSTACKCpvRecPoint*)(gime->CpvRecPoints())->At(0);
      if(pt) {
	TVector3 lpos;
	pt->GetLocalPosition(lpos);
	hXr->Fill(lpos.X());
	hZr->Fill(lpos.Z());

	Int_t rpMult = pt->GetMultiplicity();
	hNrp->Fill(rpMult);
	Int_t rpMultX, rpMultZ;
	pt->GetClusterLengths(rpMultX,rpMultZ);
	hNrpX->Fill(rpMultX);
	hNrpZ->Fill(rpMultZ);
	hChi2->Fill(((IlcRSTACKEvalRecPoint*)pt)->Chi2Dof());
	IlcInfo(Form("+++++ Event %d . (Mult,MultX,MultZ) = %d %d %d +++++", 
	     ievent, rpMult, rpMultX, rpMultZ)) ;

      }

    }
	
  Text_t outputname[80] ;
  snprintf(outputname,80,"%s.analyzed.single",GetFileName().Data());
  TFile output(outputname,"RECREATE");
  output.cd();
    
  // Plot histograms
  TCanvas *cpvCanvas = new TCanvas("SingleParticle","Single particle events",20,20,800,400);
  gStyle->SetOptStat(111111);
  gStyle->SetOptFit(1);
  gStyle->SetOptDate(1);
  cpvCanvas->Divide(3,2);

  cpvCanvas->cd(1);
  gPad->SetFillColor(10);
  hXr->SetFillColor(16);
  hXr->Draw();

  cpvCanvas->cd(2);
  gPad->SetFillColor(10);
  hZr->SetFillColor(16);
  hZr->Draw();

  cpvCanvas->cd(3);
  gPad->SetFillColor(10);
  hChi2->SetFillColor(16);
  hChi2->Draw();

  cpvCanvas->cd(4);
  gPad->SetFillColor(10);
  hNrp->SetFillColor(16);
  hNrp->Draw();

  cpvCanvas->cd(5);
  gPad->SetFillColor(10);
  hNrpX->SetFillColor(16);
  hNrpX->Draw();

  cpvCanvas->cd(6);
  gPad->SetFillColor(10);
  hNrpZ->SetFillColor(16);
  hNrpZ->Draw();

  cpvCanvas->Write(0,kOverwrite);
  
}

void IlcRSTACKIhepAnalyze::HitsCPV(Int_t nev)
{
  // Cumulative list of charged CPV impacts in event nev.

  IlcRSTACKLoader* please = dynamic_cast<IlcRSTACKLoader*>(fRunLoader->GetLoader("RSTACKLoader"));
  if ( please == 0 )
   {
     IlcError(Form("Could not obtain the Loader object !"));
     return ;
   }

     
  printf("\n=================== Event %10d ===================\n",nev);
  //16.03.2011: DP. Code below seems to be obsollete
  //Comment it to sutisfy Coverity
/* 
  TList * fCpvImpacts ;
  TBranch * branchCPVimpacts;

  IlcRSTACKGeometry * RSTACKgeom =  IlcRSTACKGeometry::GetInstance() ;

  fRunLoader->GetEvent(nev);
  Int_t ntracks = fRunLoader->GetHeader()->GetNtrack();
    
//    Int_t nRecCPV = 0; // Reconstructed points in event // 01.10.2001
//    Int_t nGenCPV = 0; // Impacts in event

  // Get branch of CPV impacts
   TTree* treeH = please->TreeH();
   if (treeH == 0x0)
    {
      IlcError(Form("Can not get TreeH"));
      return;
    }

  if (! (branchCPVimpacts =treeH->GetBranch("RSTACKCpvImpacts")) )  return;
      
  // Create and fill arrays of hits for each CPV module
  Int_t nOfModules = RSTACKgeom->GetNModules();
  TClonesArray **hitsPerModule = new TClonesArray *[nOfModules];
  Int_t iModule = 0; 	
  for (iModule=0; iModule < nOfModules; iModule++)
    hitsPerModule[iModule] = new TClonesArray("IlcRSTACKImpact",100);
  
  TClonesArray    *impacts;
  IlcRSTACKImpact   *impact;
          
  for (Int_t itrack=0; itrack<ntracks; itrack++) {
    branchCPVimpacts ->SetAddress(&fCpvImpacts);
    IlcInfo(Form(" branchCPVimpacts ->SetAddress(&fCpvImpacts) OK."));
    branchCPVimpacts ->GetEntry(itrack,0);

    for (iModule=0; iModule < nOfModules; iModule++) {
      impacts = (TClonesArray *)fCpvImpacts->At(iModule);
      // Do loop over impacts in the module
      for (Int_t iImpact=0; iImpact<impacts->GetEntries(); iImpact++) {
	impact=(IlcRSTACKImpact*)impacts->At(iImpact);
	TClonesArray &lhits = *(TClonesArray *)hitsPerModule[iModule];
	new(lhits[hitsPerModule[iModule]->GetEntriesFast()]) IlcRSTACKImpact(*impact);
      }
    }
    fCpvImpacts->Clear();
  }

  for (iModule=0; iModule < nOfModules; iModule++) {
    Int_t nsum = hitsPerModule[iModule]->GetEntriesFast();
    printf("CPV module %d has %d hits\n",iModule,nsum);
  }

*/


//    TList * fCpvImpacts ;
//    TBranch * branchCPVimpacts;
//    IlcRSTACKImpact* impact;
//    TClonesArray* impacts;

//    IlcRSTACKLoader * please = IlcRSTACKLoader::GetInstance(GetFileName().Data(),"RSTACK");
//    const IlcRSTACKGeometry *  fGeom  = please->RSTACKGeometry();

//    Int_t ntracks = gIlc->GetEvent(ievent);
//    Int_t nOfModules = fGeom->GetNModules();
//    IlcInfo(Form(" Tracks: "<<ntracks<<"  Modules: "<<nOfModules));

//    if (! (branchCPVimpacts =gIlc->TreeH()->GetBranch("RSTACKCpvImpacts")) )  return;

//    for (Int_t itrack=0; itrack<ntracks; itrack++) {
//      branchCPVimpacts ->SetAddress(&fCpvImpacts);
//      Info("AnalyzeCPV1", " branchCPVimpacts ->SetAddress(&fCpvImpacts) OK.");
//      branchCPVimpacts ->GetEntry(itrack,0);
//      Info(Form(" branchCPVimpacts ->GetEntry(itrack,0) OK."));
    
//      for (Int_t iModule=0; iModule < nOfModules; iModule++) {
//        impacts = (TClonesArray *)fCpvImpacts->At(iModule);
//        Info(Form(" fCpvImpacts->At(iModule) OK."));
//        // Do loop over impacts in the module
//        for (Int_t iImpact=0; iImpact<impacts->GetEntries(); iImpact++) {
//  	impact=(IlcRSTACKImpact*)impacts->At(iImpact);
//  	impact->Print();
//  	if(IsCharged(impact->GetPid()))
//  	  {
//  	    Info(Form(" Add charged hit.."));
//  	    new(hits[hits.GetEntriesFast()]) IlcRSTACKImpact(*impact);
//  	    Info(Form("done."));
//  	  }
//        }
//      }
//      fCpvImpacts->Clear();
//    }

//    Info(Form(" RSTACK event "<<ievent<<": "<<hits.GetEntries()<<" charged CPV hits."));

}


//  void IlcRSTACKIhepAnalyze::ChargedHitsCPV(TClonesArray* hits, Int_t ievent, Int_t iModule)
//  {
//    // Cumulative list of charged CPV hits in event ievent 
//    // in RSTACK module iModule.

//    HitsCPV(hits,ievent,iModule);
//    TIter next(hits);

//    while(IlcRSTACKCPVHit* cpvhit = (IlcRSTACKCPVHit*)next())
//      {
//        if(!IsCharged(cpvhit->GetIpart()))
//  	{
//  	  hits->Remove(cpvhit);
//  	  delete cpvhit;
//  	  hits->Compress();
//  	}
//      }

//    Info(Form(" RSTACK module "<<iModule<<": "<<hits->GetEntries()<<" charged CPV hits."));
//  }

Bool_t IlcRSTACKIhepAnalyze::IsCharged(Int_t pdgCode)
{
  // For HIJING
  IlcInfo(Form("pdgCode %d", pdgCode));
  if(pdgCode==211 || pdgCode==-211 || pdgCode==321 || pdgCode==-321 || pdgCode==11 || pdgCode==-11 || pdgCode==2212 || pdgCode==-2212) return kTRUE;
  else
    return kFALSE;
}
//---------------------------------------------------------------------------







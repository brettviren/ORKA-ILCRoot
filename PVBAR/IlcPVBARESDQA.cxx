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

/* $Id: IlcPVBARESDQA.cxx 32313 2009-05-12 10:43:13Z hristov $ */

//_________________________________________________________________________
// An analysis task to check the PVBAR photon data in simulated data
//
//*-- Yves Schutz 
//////////////////////////////////////////////////////////////////////////////

#include <TCanvas.h>
#include <TChain.h>
#include <TFile.h> 
#include <TH1.h>
#include <TH1F.h>
#include <TH1I.h>
#include <TLegend.h> 
#include <TNtuple.h>
#include <TROOT.h> 
#include <TVector3.h> 
#include <TString.h> 

#include "IlcPVBARESDQA.h" 
#include "IlcESD.h" 
#include "IlcLog.h"

//______________________________________________________________________________
IlcPVBARESDQA::IlcPVBARESDQA(const char *name) : 
  IlcAnalysisTask(name,""),  
  fChain(0),
  fESD(0), 
  fhPVBAR(0),
  fhPVBAREnergy(0),
  fhPVBARDigits(0),
  fhPVBARRecParticles(0),
  fhPVBARPhotons(0),
  fhPVBARInvariantMass(0),
  fhPVBARDigitsEvent(0)
{
  // Constructor.
  // Input slot #0 works with an Ntuple
  DefineInput(0, TChain::Class());
  // Output slot #0 writes into a TH1 container
  DefineOutput(0,  TObjArray::Class()) ; 
}

//______________________________________________________________________________
IlcPVBARESDQA::~IlcPVBARESDQA()
{
  // dtor
  fOutputContainer->Clear() ; 
  delete fOutputContainer ;

  delete fhPVBARPos ;
  delete fhPVBAR ;
  delete fhPVBAREnergy ;
  delete fhPVBARDigits ;
  delete fhPVBARRecParticles ;
  delete fhPVBARPhotons ;
  delete fhPVBARInvariantMass ;
  delete fhPVBARDigitsEvent ;
}


//______________________________________________________________________________
void IlcPVBARESDQA::ConnectInputData(const Option_t*)
{
  // Initialisation of branch container and histograms 
    
  IlcInfo(Form("*** Initialization of %s", GetName())) ; 
  
  // Get input data
  fChain = dynamic_cast<TChain *>(GetInputData(0)) ;
  if (!fChain) {
    IlcError(Form("Input 0 for %s not found\n", GetName()));
    return ;
  }
  
  // One should first check if the branch address was taken by some other task
  char ** address = (char **)GetBranchAddress(0, "ESD");
  if (address) {
    fESD = (IlcESD*)(*address);
  } else {
    fESD = new IlcESD();
    SetBranchAddress(0, "ESD", &fESD);
  }
}

//________________________________________________________________________
void IlcPVBARESDQA::CreateOutputObjects()
{  
  // create histograms 
  
  OpenFile(0) ; 

  fhPVBARPos            = new TNtuple("PVBARPos"         , "Position in PVBAR"  , "x:y:z");
  fhPVBAR               = new TNtuple("PVBAR"            , "PVBAR"  , "event:digits:clusters:photons");
  fhPVBAREnergy         = new TH1D("PVBAREnergy"         , "PVBAREnergy"        , 1000, 0., 10. ) ;
  fhPVBARDigits         = new TH1I("PVBARDigitsCluster"  , "PVBARDigits"        , 20 , 0 , 20  ) ;
  fhPVBARRecParticles   = new TH1D("PVBARRecParticles"   , "PVBARRecParticles" , 20 , 0., 20. ) ;
  fhPVBARPhotons        = new TH1I("PVBARPhotons"        , "PVBARPhotons"       , 20 , 0 , 20  ) ;
  fhPVBARInvariantMass  = new TH1D("PVBARInvariantMass"  , "PVBARInvariantMass" , 400, 0., 400.) ;
  fhPVBARDigitsEvent    = new TH1I("PVBARDigitsEvent"    , "PVBARDigitsEvent"   , 30 , 0 , 30  ) ;
  
  // create output container
  
  fOutputContainer = new TObjArray(8) ; 
  fOutputContainer->SetName(GetName()) ; 

  fOutputContainer->AddAt(fhPVBARPos,             0) ; 
  fOutputContainer->AddAt(fhPVBAR,                1) ; 
  fOutputContainer->AddAt(fhPVBAREnergy,          2) ; 
  fOutputContainer->AddAt(fhPVBARDigits,          3) ; 
  fOutputContainer->AddAt(fhPVBARRecParticles,    4) ; 
  fOutputContainer->AddAt(fhPVBARPhotons,         5) ; 
  fOutputContainer->AddAt(fhPVBARInvariantMass,   6) ; 
  fOutputContainer->AddAt(fhPVBARDigitsEvent,     7) ; 
}

//______________________________________________________________________________
void IlcPVBARESDQA::Exec(Option_t *) 
{
  // Processing of one event
    
  Long64_t entry = fChain->GetReadEntry() ;
  
  if (!fESD) {
    IlcError("fESD is not connected to the input!") ; 
    return ; 
  }
  
  if ( !((entry-1)%100) ) 
    IlcDebug(IlcQAv1::GetQADebugLevel(), Form("%s ----> Processing event # %lld",  (dynamic_cast<TChain *>(fChain))->GetFile()->GetName(), entry)) ; 
  
  //************************  PVBAR *************************************
      
  Int_t       firstPVBARCluster       = fESD->GetFirstPVBARCluster() ;
  const Int_t numberOfPVBARClusters   = fESD->GetNumberOfPVBARClusters() ;
  
  TVector3 ** PVBARVector       = new TVector3*[numberOfPVBARClusters] ;
  Float_t  * PVBARPhotonsEnergy = new Float_t[numberOfPVBARClusters] ;
  Int_t      PVBARCluster ; 
  Int_t      numberOfPhotonsInPVBAR  = 0 ;
  Int_t      numberOfDigitsInPVBAR   = 0 ;
  
  // loop over the PVBAR Cluster
  for(PVBARCluster = firstPVBARCluster ; PVBARCluster < firstPVBARCluster + numberOfPVBARClusters ; PVBARCluster++) {
    IlcESDCaloCluster * caloCluster = fESD->GetCaloCluster(PVBARCluster) ;
    if (caloCluster) {
      Float_t pos[3] ;
      caloCluster->GetPosition( pos ) ;
      fhPVBAREnergy->Fill( caloCluster->E() ) ;
      fhPVBARPos->Fill( pos[0], pos[1], pos[2] ) ;
      fhPVBARDigits->Fill(entry, caloCluster->GetNumberOfDigits() ) ;
      numberOfDigitsInPVBAR += caloCluster->GetNumberOfDigits() ;
      Float_t * pid = caloCluster->GetPid() ;
      if(pid[IlcPID::kPhoton] > 0.9) {
	PVBARVector[numberOfPhotonsInPVBAR] = new TVector3(pos[0],pos[1],pos[2]) ;
	PVBARPhotonsEnergy[numberOfPhotonsInPVBAR]=caloCluster->E() ;
	numberOfPhotonsInPVBAR++;
      }
    }
  } //PVBAR clusters
    
  fhPVBARRecParticles->Fill(numberOfPVBARClusters);
  fhPVBARPhotons->Fill(numberOfPhotonsInPVBAR);
  fhPVBARDigitsEvent->Fill(numberOfDigitsInPVBAR);
  fhPVBAR->Fill(entry, numberOfDigitsInPVBAR, numberOfPVBARClusters, numberOfPhotonsInPVBAR) ; 

  // invariant Mass
  if (numberOfPhotonsInPVBAR > 1 ) {
    Int_t PVBARPhoton1, PVBARPhoton2 ; 
    for(PVBARPhoton1 = 0 ; PVBARPhoton1 < numberOfPhotonsInPVBAR ; PVBARPhoton1++) {
      for(PVBARPhoton2 = PVBARPhoton1 + 1 ; PVBARPhoton2 < numberOfPhotonsInPVBAR ; PVBARPhoton2++) {      
	Float_t tempMass = TMath::Sqrt( 2 * PVBARPhotonsEnergy[PVBARPhoton1] * PVBARPhotonsEnergy[PVBARPhoton2] *
					( 1 - TMath::Cos(PVBARVector[PVBARPhoton1]->Angle(*PVBARVector[PVBARPhoton2])) ) 
					);
	fhPVBARInvariantMass->Fill(tempMass*1000.);
      }
    }    
  }
  
  PostData(0, fOutputContainer);

  delete [] PVBARVector ; 
  delete [] PVBARPhotonsEnergy ; 
  
}

//______________________________________________________________________________
void IlcPVBARESDQA::Terminate(Option_t *)
{
  // Processing when the event loop is ended

  fOutputContainer = (TObjArray*)GetOutputData(0);  
  fhPVBARPos            = (TNtuple*)fOutputContainer->At(0);
  fhPVBAR               = (TNtuple*)fOutputContainer->At(1);
  fhPVBAREnergy         = (TH1D*)fOutputContainer->At(2);
  fhPVBARDigits         = (TH1I*)fOutputContainer->At(3);
  fhPVBARRecParticles   = (TH1D*)fOutputContainer->At(4);
  fhPVBARPhotons        = (TH1I*)fOutputContainer->At(5);
  fhPVBARInvariantMass  = (TH1D*)fOutputContainer->At(6);
  fhPVBARDigitsEvent    = (TH1I*)fOutputContainer->At(7);

  Bool_t problem = kFALSE ; 
  IlcDebug(IlcQAv1::GetQADebugLevel(), Form(" *** %s Report:", GetName())) ; 
  if ( IlcDebugLevel()  == IlcQAv1::GetQADebugLevel() ) {
    printf("        PVBAREnergy Mean         : %5.3f , RMS : %5.3f \n", fhPVBAREnergy->GetMean(),         fhPVBAREnergy->GetRMS()         ) ;
    printf("        PVBARDigits Mean         : %5.3f , RMS : %5.3f \n", fhPVBARDigits->GetMean(),         fhPVBARDigits->GetRMS()         ) ;
    printf("        PVBARRecParticles Mean   : %5.3f , RMS : %5.3f \n", fhPVBARRecParticles->GetMean(),   fhPVBARRecParticles->GetRMS()   ) ;
    printf("        PVBARPhotons Mean        : %5.3f , RMS : %5.3f \n", fhPVBARPhotons->GetMean(),        fhPVBARPhotons->GetRMS()        ) ;
    printf("        PVBARInvariantMass Mean  : %5.3f , RMS : %5.3f \n", fhPVBARInvariantMass->GetMean(),  fhPVBARInvariantMass->GetRMS()  ) ;
    printf("        PVBARDigitsEvent Mean    : %5.3f , RMS : %5.3f \n", fhPVBARDigitsEvent->GetMean(),    fhPVBARDigitsEvent->GetRMS()    ) ;
  }
  
  TCanvas  * cPVBAR = new TCanvas("cPVBAR", "PVBAR ESD Test", 400, 10, 600, 700) ;
  cPVBAR->Divide(3, 2);

  cPVBAR->cd(1) ; 
  if ( fhPVBAREnergy->GetMaximum() > 0. ) 
    gPad->SetLogy();
  fhPVBAREnergy->SetAxisRange(0, 25.);
  fhPVBAREnergy->SetLineColor(2);
  fhPVBAREnergy->Draw();

  cPVBAR->cd(2) ; 
  fhPVBARDigits->SetAxisRange(0,25.);
  fhPVBARDigits->SetLineColor(2);
  fhPVBARDigits->Draw();

  cPVBAR->cd(3) ; 
  if ( fhPVBARRecParticles->GetMaximum() > 0. ) 
    gPad->SetLogy();
  fhPVBARRecParticles->SetAxisRange(0, 25.);
  fhPVBARRecParticles->SetLineColor(2);
  fhPVBARRecParticles->Draw();

  cPVBAR->cd(4) ; 
  if ( fhPVBARPhotons->GetMaximum() > 0. ) 
    gPad->SetLogy();
  fhPVBARPhotons->SetAxisRange(0,25.);
  fhPVBARPhotons->SetLineColor(2);
  fhPVBARPhotons->Draw();

  cPVBAR->cd(5) ; 
  fhPVBARInvariantMass->SetLineColor(2);
  fhPVBARInvariantMass->Draw();
 
  cPVBAR->cd(6) ; 
  if ( fhPVBARDigitsEvent->GetMaximum() > 0. ) 
    gPad->SetLogy();
  fhPVBARDigitsEvent->SetAxisRange(0,40.);
  fhPVBARDigitsEvent->SetLineColor(2);
  fhPVBARDigitsEvent->Draw();
 
  cPVBAR->Print("PVBAR.eps");
 
  char line[1024] ; 
  sprintf(line, ".!tar -zcf %s.tar.gz *.eps", GetName()) ; 
  gROOT->ProcessLine(line);
  sprintf(line, ".!rm -fR *.eps"); 
  gROOT->ProcessLine(line);
 
  IlcInfo(Form("!!! All the eps files are in %s.tar.gz !!!", GetName())) ;

  TString report ; 
  if(problem)
    report="Problems found, please check!!!";  
  else 
    report="OK";

  IlcInfo(Form("*** %s Summary Report: %s \n",GetName(), report.Data())) ; 
}

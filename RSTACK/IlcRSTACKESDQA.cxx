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

/* $Id: IlcRSTACKESDQA.cxx 32313 2009-05-12 10:43:13Z hristov $ */

//_________________________________________________________________________
// An analysis task to check the RSTACK photon data in simulated data
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

#include "IlcRSTACKESDQA.h" 
#include "IlcESD.h" 
#include "IlcLog.h"

//______________________________________________________________________________
IlcRSTACKESDQA::IlcRSTACKESDQA(const char *name) : 
  IlcAnalysisTask(name,""),  
  fChain(0),
  fESD(0), 
  fhRSTACK(0),
  fhRSTACKEnergy(0),
  fhRSTACKDigits(0),
  fhRSTACKRecParticles(0),
  fhRSTACKPhotons(0),
  fhRSTACKInvariantMass(0),
  fhRSTACKDigitsEvent(0)
{
  // Constructor.
  // Input slot #0 works with an Ntuple
  DefineInput(0, TChain::Class());
  // Output slot #0 writes into a TH1 container
  DefineOutput(0,  TObjArray::Class()) ; 
}

//______________________________________________________________________________
IlcRSTACKESDQA::~IlcRSTACKESDQA()
{
  // dtor
  fOutputContainer->Clear() ; 
  delete fOutputContainer ;

  delete fhRSTACKPos ;
  delete fhRSTACK ;
  delete fhRSTACKEnergy ;
  delete fhRSTACKDigits ;
  delete fhRSTACKRecParticles ;
  delete fhRSTACKPhotons ;
  delete fhRSTACKInvariantMass ;
  delete fhRSTACKDigitsEvent ;
}


//______________________________________________________________________________
void IlcRSTACKESDQA::ConnectInputData(const Option_t*)
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
void IlcRSTACKESDQA::CreateOutputObjects()
{  
  // create histograms 
  
  OpenFile(0) ; 

  fhRSTACKPos            = new TNtuple("RSTACKPos"         , "Position in RSTACK"  , "x:y:z");
  fhRSTACK               = new TNtuple("RSTACK"            , "RSTACK"  , "event:digits:clusters:photons");
  fhRSTACKEnergy         = new TH1D("RSTACKEnergy"         , "RSTACKEnergy"        , 1000, 0., 10. ) ;
  fhRSTACKDigits         = new TH1I("RSTACKDigitsCluster"  , "RSTACKDigits"        , 20 , 0 , 20  ) ;
  fhRSTACKRecParticles   = new TH1D("RSTACKRecParticles"   , "RSTACKRecParticles" , 20 , 0., 20. ) ;
  fhRSTACKPhotons        = new TH1I("RSTACKPhotons"        , "RSTACKPhotons"       , 20 , 0 , 20  ) ;
  fhRSTACKInvariantMass  = new TH1D("RSTACKInvariantMass"  , "RSTACKInvariantMass" , 400, 0., 400.) ;
  fhRSTACKDigitsEvent    = new TH1I("RSTACKDigitsEvent"    , "RSTACKDigitsEvent"   , 30 , 0 , 30  ) ;
  
  // create output container
  
  fOutputContainer = new TObjArray(8) ; 
  fOutputContainer->SetName(GetName()) ; 

  fOutputContainer->AddAt(fhRSTACKPos,             0) ; 
  fOutputContainer->AddAt(fhRSTACK,                1) ; 
  fOutputContainer->AddAt(fhRSTACKEnergy,          2) ; 
  fOutputContainer->AddAt(fhRSTACKDigits,          3) ; 
  fOutputContainer->AddAt(fhRSTACKRecParticles,    4) ; 
  fOutputContainer->AddAt(fhRSTACKPhotons,         5) ; 
  fOutputContainer->AddAt(fhRSTACKInvariantMass,   6) ; 
  fOutputContainer->AddAt(fhRSTACKDigitsEvent,     7) ; 
}

//______________________________________________________________________________
void IlcRSTACKESDQA::Exec(Option_t *) 
{
  // Processing of one event
    
  Long64_t entry = fChain->GetReadEntry() ;
  
  if (!fESD) {
    IlcError("fESD is not connected to the input!") ; 
    return ; 
  }
  
  if ( !((entry-1)%100) ) 
    IlcDebug(IlcQAv1::GetQADebugLevel(), Form("%s ----> Processing event # %lld",  (dynamic_cast<TChain *>(fChain))->GetFile()->GetName(), entry)) ; 
  
  //************************  RSTACK *************************************
      
  Int_t       firstRSTACKCluster       = fESD->GetFirstRSTACKCluster() ;
  const Int_t numberOfRSTACKClusters   = fESD->GetNumberOfRSTACKClusters() ;
  
  TVector3 ** RSTACKVector       = new TVector3*[numberOfRSTACKClusters] ;
  Float_t  * RSTACKPhotonsEnergy = new Float_t[numberOfRSTACKClusters] ;
  Int_t      RSTACKCluster ; 
  Int_t      numberOfPhotonsInRSTACK  = 0 ;
  Int_t      numberOfDigitsInRSTACK   = 0 ;
  
  // loop over the RSTACK Cluster
  for(RSTACKCluster = firstRSTACKCluster ; RSTACKCluster < firstRSTACKCluster + numberOfRSTACKClusters ; RSTACKCluster++) {
    IlcESDCaloCluster * caloCluster = fESD->GetCaloCluster(RSTACKCluster) ;
    if (caloCluster) {
      Float_t pos[3] ;
      caloCluster->GetPosition( pos ) ;
      fhRSTACKEnergy->Fill( caloCluster->E() ) ;
      fhRSTACKPos->Fill( pos[0], pos[1], pos[2] ) ;
      fhRSTACKDigits->Fill(entry, caloCluster->GetNumberOfDigits() ) ;
      numberOfDigitsInRSTACK += caloCluster->GetNumberOfDigits() ;
      Float_t * pid = caloCluster->GetPid() ;
      if(pid[IlcPID::kPhoton] > 0.9) {
	RSTACKVector[numberOfPhotonsInRSTACK] = new TVector3(pos[0],pos[1],pos[2]) ;
	RSTACKPhotonsEnergy[numberOfPhotonsInRSTACK]=caloCluster->E() ;
	numberOfPhotonsInRSTACK++;
      }
    }
  } //RSTACK clusters
    
  fhRSTACKRecParticles->Fill(numberOfRSTACKClusters);
  fhRSTACKPhotons->Fill(numberOfPhotonsInRSTACK);
  fhRSTACKDigitsEvent->Fill(numberOfDigitsInRSTACK);
  fhRSTACK->Fill(entry, numberOfDigitsInRSTACK, numberOfRSTACKClusters, numberOfPhotonsInRSTACK) ; 

  // invariant Mass
  if (numberOfPhotonsInRSTACK > 1 ) {
    Int_t RSTACKPhoton1, RSTACKPhoton2 ; 
    for(RSTACKPhoton1 = 0 ; RSTACKPhoton1 < numberOfPhotonsInRSTACK ; RSTACKPhoton1++) {
      for(RSTACKPhoton2 = RSTACKPhoton1 + 1 ; RSTACKPhoton2 < numberOfPhotonsInRSTACK ; RSTACKPhoton2++) {      
	Float_t tempMass = TMath::Sqrt( 2 * RSTACKPhotonsEnergy[RSTACKPhoton1] * RSTACKPhotonsEnergy[RSTACKPhoton2] *
					( 1 - TMath::Cos(RSTACKVector[RSTACKPhoton1]->Angle(*RSTACKVector[RSTACKPhoton2])) ) 
					);
	fhRSTACKInvariantMass->Fill(tempMass*1000.);
      }
    }    
  }
  
  PostData(0, fOutputContainer);

  delete [] RSTACKVector ; 
  delete [] RSTACKPhotonsEnergy ; 
  
}

//______________________________________________________________________________
void IlcRSTACKESDQA::Terminate(Option_t *)
{
  // Processing when the event loop is ended

  fOutputContainer = (TObjArray*)GetOutputData(0);  
  fhRSTACKPos            = (TNtuple*)fOutputContainer->At(0);
  fhRSTACK               = (TNtuple*)fOutputContainer->At(1);
  fhRSTACKEnergy         = (TH1D*)fOutputContainer->At(2);
  fhRSTACKDigits         = (TH1I*)fOutputContainer->At(3);
  fhRSTACKRecParticles   = (TH1D*)fOutputContainer->At(4);
  fhRSTACKPhotons        = (TH1I*)fOutputContainer->At(5);
  fhRSTACKInvariantMass  = (TH1D*)fOutputContainer->At(6);
  fhRSTACKDigitsEvent    = (TH1I*)fOutputContainer->At(7);

  Bool_t problem = kFALSE ; 
  IlcDebug(IlcQAv1::GetQADebugLevel(), Form(" *** %s Report:", GetName())) ; 
  if ( IlcDebugLevel()  == IlcQAv1::GetQADebugLevel() ) {
    printf("        RSTACKEnergy Mean         : %5.3f , RMS : %5.3f \n", fhRSTACKEnergy->GetMean(),         fhRSTACKEnergy->GetRMS()         ) ;
    printf("        RSTACKDigits Mean         : %5.3f , RMS : %5.3f \n", fhRSTACKDigits->GetMean(),         fhRSTACKDigits->GetRMS()         ) ;
    printf("        RSTACKRecParticles Mean   : %5.3f , RMS : %5.3f \n", fhRSTACKRecParticles->GetMean(),   fhRSTACKRecParticles->GetRMS()   ) ;
    printf("        RSTACKPhotons Mean        : %5.3f , RMS : %5.3f \n", fhRSTACKPhotons->GetMean(),        fhRSTACKPhotons->GetRMS()        ) ;
    printf("        RSTACKInvariantMass Mean  : %5.3f , RMS : %5.3f \n", fhRSTACKInvariantMass->GetMean(),  fhRSTACKInvariantMass->GetRMS()  ) ;
    printf("        RSTACKDigitsEvent Mean    : %5.3f , RMS : %5.3f \n", fhRSTACKDigitsEvent->GetMean(),    fhRSTACKDigitsEvent->GetRMS()    ) ;
  }
  
  TCanvas  * cRSTACK = new TCanvas("cRSTACK", "RSTACK ESD Test", 400, 10, 600, 700) ;
  cRSTACK->Divide(3, 2);

  cRSTACK->cd(1) ; 
  if ( fhRSTACKEnergy->GetMaximum() > 0. ) 
    gPad->SetLogy();
  fhRSTACKEnergy->SetAxisRange(0, 25.);
  fhRSTACKEnergy->SetLineColor(2);
  fhRSTACKEnergy->Draw();

  cRSTACK->cd(2) ; 
  fhRSTACKDigits->SetAxisRange(0,25.);
  fhRSTACKDigits->SetLineColor(2);
  fhRSTACKDigits->Draw();

  cRSTACK->cd(3) ; 
  if ( fhRSTACKRecParticles->GetMaximum() > 0. ) 
    gPad->SetLogy();
  fhRSTACKRecParticles->SetAxisRange(0, 25.);
  fhRSTACKRecParticles->SetLineColor(2);
  fhRSTACKRecParticles->Draw();

  cRSTACK->cd(4) ; 
  if ( fhRSTACKPhotons->GetMaximum() > 0. ) 
    gPad->SetLogy();
  fhRSTACKPhotons->SetAxisRange(0,25.);
  fhRSTACKPhotons->SetLineColor(2);
  fhRSTACKPhotons->Draw();

  cRSTACK->cd(5) ; 
  fhRSTACKInvariantMass->SetLineColor(2);
  fhRSTACKInvariantMass->Draw();
 
  cRSTACK->cd(6) ; 
  if ( fhRSTACKDigitsEvent->GetMaximum() > 0. ) 
    gPad->SetLogy();
  fhRSTACKDigitsEvent->SetAxisRange(0,40.);
  fhRSTACKDigitsEvent->SetLineColor(2);
  fhRSTACKDigitsEvent->Draw();
 
  cRSTACK->Print("RSTACK.eps");
 
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

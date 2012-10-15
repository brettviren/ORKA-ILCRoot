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

/* $Id: IlcPVECESDQA.cxx 32313 2009-05-12 10:43:13Z hristov $ */

//_________________________________________________________________________
// An analysis task to check the PVEC photon data in simulated data
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

#include "IlcPVECESDQA.h" 
#include "IlcESD.h" 
#include "IlcLog.h"

//______________________________________________________________________________
IlcPVECESDQA::IlcPVECESDQA(const char *name) : 
  IlcAnalysisTask(name,""),  
  fChain(0),
  fESD(0), 
  fhPVEC(0),
  fhPVECEnergy(0),
  fhPVECDigits(0),
  fhPVECRecParticles(0),
  fhPVECPhotons(0),
  fhPVECInvariantMass(0),
  fhPVECDigitsEvent(0)
{
  // Constructor.
  // Input slot #0 works with an Ntuple
  DefineInput(0, TChain::Class());
  // Output slot #0 writes into a TH1 container
  DefineOutput(0,  TObjArray::Class()) ; 
}

//______________________________________________________________________________
IlcPVECESDQA::~IlcPVECESDQA()
{
  // dtor
  fOutputContainer->Clear() ; 
  delete fOutputContainer ;

  delete fhPVECPos ;
  delete fhPVEC ;
  delete fhPVECEnergy ;
  delete fhPVECDigits ;
  delete fhPVECRecParticles ;
  delete fhPVECPhotons ;
  delete fhPVECInvariantMass ;
  delete fhPVECDigitsEvent ;
}


//______________________________________________________________________________
void IlcPVECESDQA::ConnectInputData(const Option_t*)
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
void IlcPVECESDQA::CreateOutputObjects()
{  
  // create histograms 
  
  OpenFile(0) ; 

  fhPVECPos            = new TNtuple("PVECPos"         , "Position in PVEC"  , "x:y:z");
  fhPVEC               = new TNtuple("PVEC"            , "PVEC"  , "event:digits:clusters:photons");
  fhPVECEnergy         = new TH1D("PVECEnergy"         , "PVECEnergy"        , 1000, 0., 10. ) ;
  fhPVECDigits         = new TH1I("PVECDigitsCluster"  , "PVECDigits"        , 20 , 0 , 20  ) ;
  fhPVECRecParticles   = new TH1D("PVECRecParticles"   , "PVECRecParticles" , 20 , 0., 20. ) ;
  fhPVECPhotons        = new TH1I("PVECPhotons"        , "PVECPhotons"       , 20 , 0 , 20  ) ;
  fhPVECInvariantMass  = new TH1D("PVECInvariantMass"  , "PVECInvariantMass" , 400, 0., 400.) ;
  fhPVECDigitsEvent    = new TH1I("PVECDigitsEvent"    , "PVECDigitsEvent"   , 30 , 0 , 30  ) ;
  
  // create output container
  
  fOutputContainer = new TObjArray(8) ; 
  fOutputContainer->SetName(GetName()) ; 

  fOutputContainer->AddAt(fhPVECPos,             0) ; 
  fOutputContainer->AddAt(fhPVEC,                1) ; 
  fOutputContainer->AddAt(fhPVECEnergy,          2) ; 
  fOutputContainer->AddAt(fhPVECDigits,          3) ; 
  fOutputContainer->AddAt(fhPVECRecParticles,    4) ; 
  fOutputContainer->AddAt(fhPVECPhotons,         5) ; 
  fOutputContainer->AddAt(fhPVECInvariantMass,   6) ; 
  fOutputContainer->AddAt(fhPVECDigitsEvent,     7) ; 
}

//______________________________________________________________________________
void IlcPVECESDQA::Exec(Option_t *) 
{
  // Processing of one event
    
  Long64_t entry = fChain->GetReadEntry() ;
  
  if (!fESD) {
    IlcError("fESD is not connected to the input!") ; 
    return ; 
  }
  
  if ( !((entry-1)%100) ) 
    IlcDebug(IlcQAv1::GetQADebugLevel(), Form("%s ----> Processing event # %lld",  (dynamic_cast<TChain *>(fChain))->GetFile()->GetName(), entry)) ; 
  
  //************************  PVEC *************************************
      
  Int_t       firstPVECCluster       = fESD->GetFirstPVECCluster() ;
  const Int_t numberOfPVECClusters   = fESD->GetNumberOfPVECClusters() ;
  
  TVector3 ** PVECVector       = new TVector3*[numberOfPVECClusters] ;
  Float_t  * PVECPhotonsEnergy = new Float_t[numberOfPVECClusters] ;
  Int_t      PVECCluster ; 
  Int_t      numberOfPhotonsInPVEC  = 0 ;
  Int_t      numberOfDigitsInPVEC   = 0 ;
  
  // loop over the PVEC Cluster
  for(PVECCluster = firstPVECCluster ; PVECCluster < firstPVECCluster + numberOfPVECClusters ; PVECCluster++) {
    IlcESDCaloCluster * caloCluster = fESD->GetCaloCluster(PVECCluster) ;
    if (caloCluster) {
      Float_t pos[3] ;
      caloCluster->GetPosition( pos ) ;
      fhPVECEnergy->Fill( caloCluster->E() ) ;
      fhPVECPos->Fill( pos[0], pos[1], pos[2] ) ;
      fhPVECDigits->Fill(entry, caloCluster->GetNumberOfDigits() ) ;
      numberOfDigitsInPVEC += caloCluster->GetNumberOfDigits() ;
      Float_t * pid = caloCluster->GetPid() ;
      if(pid[IlcPID::kPhoton] > 0.9) {
	PVECVector[numberOfPhotonsInPVEC] = new TVector3(pos[0],pos[1],pos[2]) ;
	PVECPhotonsEnergy[numberOfPhotonsInPVEC]=caloCluster->E() ;
	numberOfPhotonsInPVEC++;
      }
    }
  } //PVEC clusters
    
  fhPVECRecParticles->Fill(numberOfPVECClusters);
  fhPVECPhotons->Fill(numberOfPhotonsInPVEC);
  fhPVECDigitsEvent->Fill(numberOfDigitsInPVEC);
  fhPVEC->Fill(entry, numberOfDigitsInPVEC, numberOfPVECClusters, numberOfPhotonsInPVEC) ; 

  // invariant Mass
  if (numberOfPhotonsInPVEC > 1 ) {
    Int_t PVECPhoton1, PVECPhoton2 ; 
    for(PVECPhoton1 = 0 ; PVECPhoton1 < numberOfPhotonsInPVEC ; PVECPhoton1++) {
      for(PVECPhoton2 = PVECPhoton1 + 1 ; PVECPhoton2 < numberOfPhotonsInPVEC ; PVECPhoton2++) {      
	Float_t tempMass = TMath::Sqrt( 2 * PVECPhotonsEnergy[PVECPhoton1] * PVECPhotonsEnergy[PVECPhoton2] *
					( 1 - TMath::Cos(PVECVector[PVECPhoton1]->Angle(*PVECVector[PVECPhoton2])) ) 
					);
	fhPVECInvariantMass->Fill(tempMass*1000.);
      }
    }    
  }
  
  PostData(0, fOutputContainer);

  delete [] PVECVector ; 
  delete [] PVECPhotonsEnergy ; 
  
}

//______________________________________________________________________________
void IlcPVECESDQA::Terminate(Option_t *)
{
  // Processing when the event loop is ended

  fOutputContainer = (TObjArray*)GetOutputData(0);  
  fhPVECPos            = (TNtuple*)fOutputContainer->At(0);
  fhPVEC               = (TNtuple*)fOutputContainer->At(1);
  fhPVECEnergy         = (TH1D*)fOutputContainer->At(2);
  fhPVECDigits         = (TH1I*)fOutputContainer->At(3);
  fhPVECRecParticles   = (TH1D*)fOutputContainer->At(4);
  fhPVECPhotons        = (TH1I*)fOutputContainer->At(5);
  fhPVECInvariantMass  = (TH1D*)fOutputContainer->At(6);
  fhPVECDigitsEvent    = (TH1I*)fOutputContainer->At(7);

  Bool_t problem = kFALSE ; 
  IlcDebug(IlcQAv1::GetQADebugLevel(), Form(" *** %s Report:", GetName())) ; 
  if ( IlcDebugLevel()  == IlcQAv1::GetQADebugLevel() ) {
    printf("        PVECEnergy Mean         : %5.3f , RMS : %5.3f \n", fhPVECEnergy->GetMean(),         fhPVECEnergy->GetRMS()         ) ;
    printf("        PVECDigits Mean         : %5.3f , RMS : %5.3f \n", fhPVECDigits->GetMean(),         fhPVECDigits->GetRMS()         ) ;
    printf("        PVECRecParticles Mean   : %5.3f , RMS : %5.3f \n", fhPVECRecParticles->GetMean(),   fhPVECRecParticles->GetRMS()   ) ;
    printf("        PVECPhotons Mean        : %5.3f , RMS : %5.3f \n", fhPVECPhotons->GetMean(),        fhPVECPhotons->GetRMS()        ) ;
    printf("        PVECInvariantMass Mean  : %5.3f , RMS : %5.3f \n", fhPVECInvariantMass->GetMean(),  fhPVECInvariantMass->GetRMS()  ) ;
    printf("        PVECDigitsEvent Mean    : %5.3f , RMS : %5.3f \n", fhPVECDigitsEvent->GetMean(),    fhPVECDigitsEvent->GetRMS()    ) ;
  }
  
  TCanvas  * cPVEC = new TCanvas("cPVEC", "PVEC ESD Test", 400, 10, 600, 700) ;
  cPVEC->Divide(3, 2);

  cPVEC->cd(1) ; 
  if ( fhPVECEnergy->GetMaximum() > 0. ) 
    gPad->SetLogy();
  fhPVECEnergy->SetAxisRange(0, 25.);
  fhPVECEnergy->SetLineColor(2);
  fhPVECEnergy->Draw();

  cPVEC->cd(2) ; 
  fhPVECDigits->SetAxisRange(0,25.);
  fhPVECDigits->SetLineColor(2);
  fhPVECDigits->Draw();

  cPVEC->cd(3) ; 
  if ( fhPVECRecParticles->GetMaximum() > 0. ) 
    gPad->SetLogy();
  fhPVECRecParticles->SetAxisRange(0, 25.);
  fhPVECRecParticles->SetLineColor(2);
  fhPVECRecParticles->Draw();

  cPVEC->cd(4) ; 
  if ( fhPVECPhotons->GetMaximum() > 0. ) 
    gPad->SetLogy();
  fhPVECPhotons->SetAxisRange(0,25.);
  fhPVECPhotons->SetLineColor(2);
  fhPVECPhotons->Draw();

  cPVEC->cd(5) ; 
  fhPVECInvariantMass->SetLineColor(2);
  fhPVECInvariantMass->Draw();
 
  cPVEC->cd(6) ; 
  if ( fhPVECDigitsEvent->GetMaximum() > 0. ) 
    gPad->SetLogy();
  fhPVECDigitsEvent->SetAxisRange(0,40.);
  fhPVECDigitsEvent->SetLineColor(2);
  fhPVECDigitsEvent->Draw();
 
  cPVEC->Print("PVEC.eps");
 
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

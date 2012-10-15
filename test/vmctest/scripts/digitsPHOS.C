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

// $Id: digitsPHOS.C 36464 2009-11-06 14:18:47Z ivana $

// Macro to generate histograms from digits
// By E. Sicking, CERN


void digitsPHOS(Int_t nevents, Int_t nfiles)
{

 TH1F * hadc = new TH1F ("hadc", "hadc", 100, -10, 200);
 TH1F * hadcLog = new TH1F ("hadclog", "hadclog", 100, -2, 4);
 IlcRunLoader* runLoader = IlcRunLoader::Open("gilc.root","Event","READ");
 IlcPHOSLoader * phosLoader = dynamic_cast<IlcPHOSLoader*>(runLoader->GetLoader("PHOSLoader"));
 
 for (Int_t ievent=0; ievent <nevents; ievent++) {
  // for (Int_t ievent = 0; ievent < runLoader->GetNumberOfEvents(); ievent++) {
   runLoader->GetEvent(ievent) ;
   phosLoader->CleanDigits() ; 
   phosLoader->LoadDigits("READ") ;
   TClonesArray * digits    = phosLoader->Digits() ;
   printf("Event %d contains %d digits\n",ievent,digits->GetEntriesFast());
   
   
   for (Int_t j = 0; j < digits->GetEntries(); j++) {
     
     IlcPHOSDigit* dig = dynamic_cast<IlcPHOSDigit*> (digits->At(j));
     //cout << dig->GetEnergy() << endl;
     hadc->Fill(dig->GetEnergy());
     if(dig->GetEnergy()>0)
       hadcLog->Fill(TMath::Log10(dig->GetEnergy()));
     
   }
   
 }
 
 TFile fc("digits.PHOS.root","RECREATE");
 fc.cd();
 hadc->Write();
 hadcLog->Write();
 fc.Close();


}

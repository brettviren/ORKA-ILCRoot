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


/* $Id: IlcRSTACKQADataMakerRec.cxx 57196 2012-06-15 08:08:57Z hristov $ */

/*
  Produces the data needed to calculate the quality assurance. 
  All data must be mergeable objects.
  Y. Schutz CERN July 2007
*/

// --- ROOT system ---
#include <TClonesArray.h>
#include <TFile.h> 
#include <TH1F.h> 
#include <TH1I.h> 
#include <TH2F.h> 
#include <TParameter.h>

// --- Standard library ---

// --- IlcRoot header files ---
#include "IlcCaloRawStreamV3.h"
#include "IlcESDCaloCluster.h"
#include "IlcESDEvent.h"
#include "IlcLog.h"
#include "IlcRSTACKQADataMakerRec.h"
#include "IlcRSTACKDigit.h"
#include "IlcRSTACKCpvRecPoint.h" 
#include "IlcRSTACKEmcRecPoint.h" 
#include "IlcRSTACKRawFitterv0.h"
#include "IlcRSTACKRawFitterv1.h"
#include "IlcRSTACKRawFitterv2.h"
#include "IlcRSTACKReconstructor.h"
#include "IlcRSTACKRecParticle.h" 
#include "IlcRSTACKTrackSegment.h" 
#include "IlcQAChecker.h"
#include "IlcRawReader.h"

ClassImp(IlcRSTACKQADataMakerRec)
           
//____________________________________________________________________________ 
  IlcRSTACKQADataMakerRec::IlcRSTACKQADataMakerRec() : 
  IlcQADataMakerRec(IlcQAv1::GetDetName(IlcQAv1::kRSTACK), "RSTACK Quality Assurance Data Maker")
{
  // ctor
}

//____________________________________________________________________________ 
IlcRSTACKQADataMakerRec::IlcRSTACKQADataMakerRec(const IlcRSTACKQADataMakerRec& qadm) :
  IlcQADataMakerRec()
{
  //copy ctor 
  SetName((const char*)qadm.GetName()) ; 
  SetTitle((const char*)qadm.GetTitle()); 
}

//__________________________________________________________________
IlcRSTACKQADataMakerRec& IlcRSTACKQADataMakerRec::operator = (const IlcRSTACKQADataMakerRec& qadm )
{
  // Equal operator.
  this->~IlcRSTACKQADataMakerRec();
  new(this) IlcRSTACKQADataMakerRec(qadm);
  return *this;
}
 
//____________________________________________________________________________ 
void IlcRSTACKQADataMakerRec::EndOfDetectorCycle(IlcQAv1::TASKINDEX_t task, TObjArray ** list)
{
  //Detector specific actions at end of cycle
  ResetEventTrigClasses();

  for (Int_t specie = 0 ; specie < IlcRecoParam::kNSpecies ; specie++) {
    if (! IsValidEventSpecie(specie, list)) continue;
    SetEventSpecie(IlcRecoParam::ConvertIndex(specie)); 
    //
    for (int itc=-1;itc<GetNTrigClasses();itc++) {  // RS: loop over all trigger clones
      //
      if(GetRawsData(kHGqualMod0,itc) && GetRawsData(kHGmod0,itc))
	GetRawsData(kHGqualMod0,itc)->Divide( GetRawsData(kHGmod0,itc) );
      if(GetRawsData(kHGqualMod1,itc) && GetRawsData(kHGmod1,itc))
	GetRawsData(kHGqualMod1,itc)->Divide( GetRawsData(kHGmod1,itc) );
      if(GetRawsData(kHGqualMod2,itc) && GetRawsData(kHGmod2,itc))
	GetRawsData(kHGqualMod2,itc)->Divide( GetRawsData(kHGmod2,itc) );
      if(GetRawsData(kHGqualMod3,itc) && GetRawsData(kHGmod3,itc))
	GetRawsData(kHGqualMod3,itc)->Divide( GetRawsData(kHGmod3,itc) );
      if(GetRawsData(kHGqualMod4,itc) && GetRawsData(kHGmod4,itc))
	GetRawsData(kHGqualMod4,itc)->Divide( GetRawsData(kHGmod4,itc) );
      if(GetRawsData(kNRCUHG,itc)) {
	Int_t norm = GetEvCountTotalRaws();
	if (norm < 1) norm = 1;
	TH1F* tmp = (TH1F*)GetRawsData(kNRCUHGnorm,itc);
	tmp->Reset();
	tmp->Add((TH1F*)GetRawsData(kNRCUHG,itc),1./norm);
      }
    } // RS: loop over all trigger clones
  }
  // do the QA checking
  IlcQAChecker::Instance()->Run(IlcQAv1::kRSTACK, task, list) ;  
}

//____________________________________________________________________________ 
void IlcRSTACKQADataMakerRec::InitESDs()
{
  //Create histograms to controll ESD
 
  const Bool_t expert   = kTRUE ; 
  const Bool_t image    = kTRUE ; 

  TH1F * h1 = new TH1F("hESDRSTACKSpectrum",  "ESDs spectrum in RSTACK; Energy [MeV];Counts"                ,  200, 0.,   20.) ;
  h1->Sumw2() ;
  Add2ESDsList(h1, kESDSpec, !expert, image)  ;

  TH1I * h2 = new TH1I("hESDRSTACKMul",       "ESDs multiplicity distribution in RSTACK; # of clusters;Counts", 100, 0,   100 ) ; 
  h2->Sumw2() ;
  Add2ESDsList(h2, kESDNtot, !expert, image) ;
 
  TH1F * h3 = new TH1F("hESDRSTACKEtot",      "ESDs total energy;Energy [MeV];Counts"                     , 2000, 0,  200.) ; 
  h3->Sumw2() ;
  Add2ESDsList(h3, kESDEtot, !expert, image) ;  //Expert histo
 
  TH1F * h4 = new TH1F("hESDpid",           "ESDs PID distribution in RSTACK;Particle Id;Counts"         , 100, 0.,    1.) ;
  h4->Sumw2() ;
  Add2ESDsList(h4, kESDpid, !expert, image) ; //Expert histo
  //
  ClonePerTrigClass(IlcQAv1::kESDS); // this should be the last line	
}

//____________________________________________________________________________ 
void IlcRSTACKQADataMakerRec::InitDigits()
{
  // create Digits histograms in Digits subdir
  const Bool_t expert   = kTRUE ; 
  const Bool_t image    = kTRUE ; 
  TH1I * h0 = new TH1I("hRSTACKDigits",    "Digits amplitude distribution in RSTACK;Amplitude [ADC counts];Counts",    500, 0, 1000) ; 
  h0->Sumw2() ;
  Add2DigitsList(h0, kDigits, !expert, image) ;
  TH1I * h1 = new TH1I("hRSTACKDigitsMul", "Digits multiplicity distribution in RSTACK;# of Digits;Entries", 2000, 0, 10000) ; 
  h1->Sumw2() ;
  Add2DigitsList(h1, kDigitsMul, !expert, image) ;
  //
  ClonePerTrigClass(IlcQAv1::kDIGITS); // this should be the last line
}

//____________________________________________________________________________ 
void IlcRSTACKQADataMakerRec::InitRecPoints()
{
  // create Reconstructed Points histograms in RecPoints subdir
  const Bool_t expert   = kTRUE ; 
  const Bool_t image    = kTRUE ; 
  
  TH2I * h0 = new TH2I("hRpRSTACKxyMod1","RecPoints Rows x Columns for RSTACK module 1;Row #;Column #", 64, -72., 72., 56, -63., 63.) ;                             
  Add2RecPointsList(h0,kRPmod1, expert, !image) ;
  TH2I * h1 = new TH2I("hRpRSTACKxyMod2","RecPoints Rows x Columns for RSTACK module 2;Row #;Column #", 64, -72., 72., 56, -63., 63.) ;                             
  Add2RecPointsList(h1,kRPmod2, expert, !image) ;
  TH2I * h2 = new TH2I("hRpRSTACKxyMod3","RecPoints Rows x Columns for RSTACK module 3;Row #;Column #", 64, -72., 72., 56, -63., 63.) ;                             
  Add2RecPointsList(h2,kRPmod3, expert, !image) ;
  TH2I * h3 = new TH2I("hRpRSTACKxyMod4","RecPoints Rows x Columns for RSTACK module 4;Row #;Column #", 64, -72., 72., 56, -63., 63.) ;                             
  Add2RecPointsList(h3,kRPmod4, expert, !image) ;
  TH2I * h4 = new TH2I("hRpRSTACKxyMod5","RecPoints Rows x Columns for RSTACK module 5;Row #;Column #", 64, -72., 72., 56, -63., 63.) ;                             
  Add2RecPointsList(h4,kRPmod5, expert, !image) ;
 
  TH1F * h5 = new TH1F("hEmcRSTACKRecPointsSpectrum",  "EMC RecPoints spectrum in RSTACK;Energy [MeV];Counts",   2000, 0., 20.) ; 
  h5->Sumw2() ;
  Add2RecPointsList(h5, kRPSpec, !expert, image)  ;

  TH1I * h6 = new TH1I("hEmcRSTACKRecPointsMul", "EMC RecPoints multiplicity distribution in RSTACK;# of EMC Clusters;Entries", 100, 0,  100) ; 
  h6->Sumw2() ;
  Add2RecPointsList(h6, kRPNtot, !expert, image) ;

  TH1I * h7 = new TH1I("hEmcRSTACKRecPointsEtot", "EMC RecPoints Etot;Energy [MeV];Counts", 200, 0,  200.) ; 
  h7->Sumw2() ;
  Add2RecPointsList(h7, kRPEtot, !expert, image) ;

  TH1I * h8 = new TH1I("hCpvRSTACKRecPointsMul", "CPV RecPoints multiplicity distribution in RSTACK;# of CPV clusters;Counts", 100, 0,  100) ; 
  h8->Sumw2() ;
  Add2RecPointsList(h8, kRPNcpv, !expert, image) ;
  //
  ClonePerTrigClass(IlcQAv1::kRECPOINTS); // this should be the last line
}

//____________________________________________________________________________ 
void IlcRSTACKQADataMakerRec::InitRaws()
{
  // create Raws histograms in Raws subdir
  const Bool_t expert   = kTRUE ; 
  const Bool_t saveCorr = kTRUE ; 
  const Bool_t image    = kTRUE ; 

  TH2I * h0 = new TH2I("hHighRSTACKxyMod0","High Gain in RSTACK module 0", 64, 0, 64, 56, 0, 56) ;
  h0->SetXTitle("x, cells"); h0->SetYTitle("z, cells");
  Add2RawsList(h0,kHGmod0, expert, !image, !saveCorr) ;
  TH2I * h1 = new TH2I("hHighRSTACKxyMod1","High Gain in RSTACK module 1", 64, 0, 64, 56, 0, 56) ;
  h1->SetXTitle("x, cells"); h1->SetYTitle("z, cells");
  Add2RawsList(h1,kHGmod1, expert, !image, !saveCorr) ;
  TH2I * h2 = new TH2I("hHighRSTACKxyMod2","High Gain in RSTACK module 2", 64, 0, 64, 56, 0, 56) ;
  h2->SetXTitle("x, cells"); h2->SetYTitle("z, cells");
  Add2RawsList(h2,kHGmod2, expert, !image, !saveCorr) ;
  TH2I * h3 = new TH2I("hHighRSTACKxyMod3","High Gain in RSTACK module 3", 64, 0, 64, 56, 0, 56) ;
  h3->SetXTitle("x, cells"); h3->SetYTitle("z, cells");
  Add2RawsList(h3,kHGmod3, expert, !image, !saveCorr) ;
  TH2I * h4 = new TH2I("hHighRSTACKxyMod4","High Gain in RSTACK module 4", 64, 0, 64, 56, 0, 56) ;
  h4->SetXTitle("x, cells"); h4->SetYTitle("z, cells");
  Add2RawsList(h4,kHGmod4, expert, !image, !saveCorr) ;

  TH2I * h5 = new TH2I("hLowRSTACKxyMod0","Low Gain in RSTACK module 0", 64, 0, 64, 56, 0, 56) ;
  h5->SetXTitle("x, cells"); h5->SetYTitle("z, cells");
  Add2RawsList(h5,kLGmod0, expert, !image, !saveCorr) ;
  TH2I * h6 = new TH2I("hLowRSTACKxyMod1","Low Gain in RSTACK module 1", 64, 0, 64, 56, 0, 56) ;
  h6->SetXTitle("x, cells"); h6->SetYTitle("z, cells");
  Add2RawsList(h6,kLGmod1, expert, !image, !saveCorr) ;
  TH2I * h7 = new TH2I("hLowRSTACKxyMod2","Low Gain in RSTACK module 2", 64, 0, 64, 56, 0, 56) ;
  h7->SetXTitle("x, cells"); h7->SetYTitle("z, cells");
  Add2RawsList(h7,kLGmod2, expert, !image, !saveCorr) ;
  TH2I * h8 = new TH2I("hLowRSTACKxyMod3","Low Gain in RSTACK module 3", 64, 0, 64, 56, 0, 56) ;
  h8->SetXTitle("x, cells"); h8->SetYTitle("z, cells");
  Add2RawsList(h8,kLGmod3, expert, !image, !saveCorr) ;
  TH2I * h9 = new TH2I("hLowRSTACKxyMod4","Low Gain in RSTACK module 4", 64, 0, 64, 56, 0, 56) ;
  h9->SetXTitle("x, cells"); h9->SetYTitle("z, cells");
  Add2RawsList(h9,kLGmod4, expert, !image, !saveCorr) ;

  TH1I * h10 = new TH1I("hLowRSTACKModules",    "Low Gain Hits in EMCA RSTACK modules", 5, 0, 5) ;
  h10->SetXTitle("Module number");
  Add2RawsList(h10, kNmodLG, !expert, image, !saveCorr) ;
  TH1I * h11 = new TH1I("hHighRSTACKModules",   "High Gain Hits in EMCA RSTACK modules",5, 0, 5) ;
  h11->SetXTitle("Module number");
  Add2RawsList(h11, kNmodHG, !expert, image, !saveCorr) ;

  TH1I * h11_RCU = new TH1I("hHighRSTACKRCU",   "RSTACK RCU occupancy with HG channels",12, 0, 12) ;
  h11_RCU->SetXTitle("RCU number");
  Add2RawsList(h11_RCU, kNRCUHG, expert, image, !saveCorr) ;
  TH1F * h11_RCUnorm = new TH1F("hHighRSTACKRCUnorm",   "RSTACK RCU normalized occupancy with HG channels",12, 0, 12) ;
  h11_RCUnorm->SetXTitle("RCU number");
  h11_RCUnorm->SetYTitle("N hits per event");
  h11_RCUnorm->Sumw2();
  h11_RCUnorm->SetMarkerStyle(20);
  Add2RawsList(h11_RCUnorm, kNRCUHGnorm, !expert, image, !saveCorr) ;

  TH1F * h12 = new TH1F("hLowRSTACKRawtime" , "Low Gain Time of raw hits in RSTACK" , 500, -50., 200.) ;
  h12->SetXTitle("Time [samples]");
  h12->Sumw2() ;
  Add2RawsList(h12, kLGtime, expert, !image, !saveCorr) ;
  TH1F * h13 = new TH1F("hHighRSTACKRawtime", "High Gain Time of raw hits in RSTACK", 500, -50., 200.) ;
  h13->SetXTitle("Time [samples]");
  h13->Sumw2() ;
  Add2RawsList(h13, kHGtime, expert, !image, !saveCorr) ;

  TH1F * h14 = new TH1F("hLowRSTACKRawEnergy" , "Low Gain Energy of raw hits in RSTACK" , 512, 0., 1024.) ;
  h14->SetXTitle("Energy [ADC counts]");
  h14->Sumw2() ;
  Add2RawsList(h14, kSpecLG, !expert, image, !saveCorr) ;
  TH1F * h15 = new TH1F("hHighRSTACKRawEnergy", "High Gain Energy of raw hits in RSTACK", 512, 0., 1024.) ;
  h15->SetXTitle("Energy [ADC counts]");
  h15->Sumw2() ;
  Add2RawsList(h15, kSpecHG, !expert, image, !saveCorr) ;

  TH1F * h16 = new TH1F("hLowNtot" , "Low Gain Total Number of raw hits in RSTACK" , 500, 0., 5000.) ;
  h16->SetXTitle("Number of hits");
  h16->Sumw2() ;
  Add2RawsList(h16, kNtotLG, !expert, image, saveCorr) ;
  TH1F * h17 = new TH1F("hHighNtot", "High Gain Total Number of raw hits in RSTACK", 500, 0., 5000.) ;
  h17->SetXTitle("Number of hits");
  h17->Sumw2() ;
  Add2RawsList(h17, kNtotHG, !expert, image, saveCorr) ;

  TH1F * h17_1_0 = new TH1F("hHighNtot_1_0", "High Gain Total Number of raw hits in module 1 RCU0", 500, 0., 1000.) ;
  h17_1_0->SetXTitle("Number of hits");
  h17_1_0->Sumw2() ;
  Add2RawsList(h17_1_0, kNtotHG_1_0, expert, image, saveCorr) ;

  TH1F * h17_1_1 = new TH1F("hHighNtot_1_1", "High Gain Total Number of raw hits in module 1 RCU1", 500, 0., 1000.) ;
  h17_1_1->SetXTitle("Number of hits");
  h17_1_1->Sumw2() ;
  Add2RawsList(h17_1_1, kNtotHG_1_1, expert, image, saveCorr) ;

  TH1F * h17_1_2 = new TH1F("hHighNtot_1_2", "High Gain Total Number of raw hits in module 1 RCU2", 500, 0., 1000.) ;
  h17_1_2->SetXTitle("Number of hits");
  h17_1_2->Sumw2() ;
  Add2RawsList(h17_1_2, kNtotHG_1_2, expert, image, saveCorr) ;

  TH1F * h17_1_3 = new TH1F("hHighNtot_1_3", "High Gain Total Number of raw hits in module 1 RCU3", 500, 0., 1000.) ;
  h17_1_3->SetXTitle("Number of hits");
  h17_1_3->Sumw2() ;
  Add2RawsList(h17_1_3, kNtotHG_1_3, expert, image, saveCorr) ;

  TH1F * h17_2_0 = new TH1F("hHighNtot_2_0", "High Gain Total Number of raw hits in module 2 RCU0", 500, 0., 1000.) ;
  h17_2_0->SetXTitle("Number of hits");
  h17_2_0->Sumw2() ;
  Add2RawsList(h17_2_0, kNtotHG_2_0, expert, image, saveCorr) ;

  TH1F * h17_2_1 = new TH1F("hHighNtot_2_1", "High Gain Total Number of raw hits in module 2 RCU1", 500, 0., 1000.) ;
  h17_2_1->SetXTitle("Number of hits");
  h17_2_1->Sumw2() ;
  Add2RawsList(h17_2_1, kNtotHG_2_1, expert, image, saveCorr) ;

  TH1F * h17_2_2 = new TH1F("hHighNtot_2_2", "High Gain Total Number of raw hits in module 2 RCU2", 500, 0., 1000.) ;
  h17_2_2->SetXTitle("Number of hits");
  h17_2_2->Sumw2() ;
  Add2RawsList(h17_2_2, kNtotHG_2_2, expert, image, saveCorr) ;

  TH1F * h17_2_3 = new TH1F("hHighNtot_2_3", "High Gain Total Number of raw hits in module 2 RCU3", 500, 0., 1000.) ;
  h17_2_3->SetXTitle("Number of hits");
  h17_2_3->Sumw2() ;
  Add2RawsList(h17_2_3, kNtotHG_2_3, expert, image, saveCorr) ;

  TH1F * h17_3_0 = new TH1F("hHighNtot_3_0", "High Gain Total Number of raw hits in module 3 RCU0", 500, 0., 1000.) ;
  h17_3_0->SetXTitle("Number of hits");
  h17_3_0->Sumw2() ;
  Add2RawsList(h17_3_0, kNtotHG_3_0, expert, image, saveCorr) ;

  TH1F * h17_3_1 = new TH1F("hHighNtot_3_1", "High Gain Total Number of raw hits in module 3 RCU1", 500, 0., 1000.) ;
  h17_3_1->SetXTitle("Number of hits");
  h17_3_1->Sumw2() ;
  Add2RawsList(h17_3_1, kNtotHG_3_1, expert, image, saveCorr) ;

  TH1F * h17_3_2 = new TH1F("hHighNtot_3_2", "High Gain Total Number of raw hits in module 3 RCU2", 500, 0., 1000.) ;
  h17_3_2->SetXTitle("Number of hits");
  h17_3_2->Sumw2() ;
  Add2RawsList(h17_3_2, kNtotHG_3_2, expert, image, saveCorr) ;

  TH1F * h17_3_3 = new TH1F("hHighNtot_3_3", "High Gain Total Number of raw hits in module 3 RCU3", 500, 0., 1000.) ;
  h17_3_3->SetXTitle("Number of hits");
  h17_3_3->Sumw2() ;
  Add2RawsList(h17_3_3, kNtotHG_3_3, expert, image, saveCorr) ;

  TH1F * h18 = new TH1F("hLowEtot" , "Low Gain Total Energy of raw hits in RSTACK" , 500, 0., 100000.) ;
  h18->SetXTitle("Energy [ADC counts]");
  h18->Sumw2() ;
  Add2RawsList(h18, kEtotLG, !expert, image, saveCorr) ;
  TH1F * h19 = new TH1F("hHighEtot", "High Gain Total Energy of raw hits in RSTACK", 500, 0., 100000.) ;
  h19->SetXTitle("Energy [ADC counts]");
  h19->Sumw2() ;
  Add2RawsList(h19, kEtotHG, !expert, image, saveCorr) ;

  TH2F * h20 = new TH2F("hQualHGxyMod0","High Gain signal quality in module 0", 64, 0, 64, 56, 0, 56) ;
  h20->SetXTitle("x, cells"); h20->SetYTitle("z, cells");
  Add2RawsList(h20,kHGqualMod0, expert, !image, !saveCorr) ;
  TH2F * h21 = new TH2F("hQualHGxyMod1","High Gain signal quality in module 1", 64, 0, 64, 56, 0, 56) ;
  h21->SetXTitle("x, cells"); h21->SetYTitle("z, cells");
  Add2RawsList(h21,kHGqualMod1, expert, !image, !saveCorr) ;
  TH2F * h22 = new TH2F("hQualHGxyMod2","High Gain signal quality in module 2", 64, 0, 64, 56, 0, 56) ;
  h22->SetXTitle("x, cells"); h22->SetYTitle("z, cells");
  Add2RawsList(h22,kHGqualMod2, expert, !image, !saveCorr) ;
  TH2F * h23 = new TH2F("hQualHGxyMod3","High Gain signal quality in module 3", 64, 0, 64, 56, 0, 56) ;
  h23->SetXTitle("x, cells"); h23->SetYTitle("z, cells");
  Add2RawsList(h23,kHGqualMod3, expert, !image, !saveCorr) ;
  TH2F * h24 = new TH2F("hQualHGxyMod4","High Gain signal quality in module 4", 64, 0, 64, 56, 0, 56) ;
  h24->SetXTitle("x, cells"); h24->SetYTitle("z, cells");
  Add2RawsList(h24,kHGqualMod4, expert, !image, !saveCorr) ;

  TH2F * h25 = new TH2F("hQualLGxyMod0","Low Gain signal quality in module 0", 64, 0, 64, 56, 0, 56) ;
  h25->SetXTitle("x, cells"); h25->SetYTitle("z, cells");
  Add2RawsList(h25,kLGqualMod0, expert, !image, !saveCorr) ;
  TH2F * h26 = new TH2F("hQualLGxyMod1","Low Gain signal quality in module 1", 64, 0, 64, 56, 0, 56) ;
  h26->SetXTitle("x, cells"); h26->SetYTitle("z, cells");
  Add2RawsList(h26,kLGqualMod1, expert, !image, !saveCorr) ;
  TH2F * h27 = new TH2F("hQualLGxyMod2","Low Gain signal quality in module 2", 64, 0, 64, 56, 0, 56) ;
  h27->SetXTitle("x, cells"); h27->SetYTitle("z, cells");
  Add2RawsList(h27,kLGqualMod2, expert, !image, !saveCorr) ;
  TH2F * h28 = new TH2F("hQualLGxyMod3","Low Gain signal quality in module 3", 64, 0, 64, 56, 0, 56) ;
  h28->SetXTitle("x, cells"); h28->SetYTitle("z, cells");
  Add2RawsList(h28,kLGqualMod3, expert, !image, !saveCorr) ;
  TH2F * h29 = new TH2F("hQualLGxyMod4","Low Gain signal quality in module 4", 64, 0, 64, 56, 0, 56) ;
  h29->SetXTitle("x, cells"); h29->SetYTitle("z, cells");
  Add2RawsList(h29,kLGqualMod4, expert, !image, !saveCorr) ;

  TH1F * h30 = new TH1F("hLGpedRMS" ,"Low Gain pedestal RMS" ,200,0.,20.) ;
  h30->SetXTitle("RMS [ADC counts]");
  h30->Sumw2() ;
  Add2RawsList(h30,kLGpedRMS, expert, !image, !saveCorr) ;
  TH1F * h31 = new TH1F("hHGpedRMS" ,"High Gain pedestal RMS",200,0.,20.) ;
  h31->SetXTitle("RMS [ADC counts]");
  h31->Sumw2() ;
  Add2RawsList(h31,kHGpedRMS, expert, !image, !saveCorr) ;

  TH2F * h32 = new TH2F("hpedRMSHGxyMod0","High Gain pedestal RMS in module 0", 64, 0, 64, 56, 0, 56) ;
  h32->SetXTitle("x, cells"); h32->SetYTitle("z, cells");
  Add2RawsList(h32,kHGpedRMSMod0, expert, !image, !saveCorr) ;
  TH2F * h33 = new TH2F("hpedRMSHGxyMod1","High Gain pedestal RMS in module 1", 64, 0, 64, 56, 0, 56) ;
  h33->SetXTitle("x, cells"); h33->SetYTitle("z, cells");
  Add2RawsList(h33,kHGpedRMSMod1, expert, !image, !saveCorr) ;
  TH2F * h34 = new TH2F("hpedRMSHGxyMod2","High Gain pedestal RMS in module 2", 64, 0, 64, 56, 0, 56) ;
  h34->SetXTitle("x, cells"); h34->SetYTitle("z, cells");
  Add2RawsList(h34,kHGpedRMSMod2, expert, !image, !saveCorr) ;
  TH2F * h35 = new TH2F("hpedRMSHGxyMod3","High Gain pedestal RMS in module 3", 64, 0, 64, 56, 0, 56) ;
  h35->SetXTitle("x, cells"); h35->SetYTitle("z, cells");
  Add2RawsList(h35,kHGpedRMSMod3, expert, !image, !saveCorr) ;
  TH2F * h36 = new TH2F("hpedRMSHGxyMod4","High Gain pedestal RMS in module 4", 64, 0, 64, 56, 0, 56) ;
  h36->SetXTitle("x, cells"); h36->SetYTitle("z, cells");
  Add2RawsList(h36,kHGpedRMSMod4, expert, !image, !saveCorr) ;

  TH2F * h37 = new TH2F("hpedRMSLGxyMod0","Low Gain pedestal RMS in module 0", 64, 0, 64, 56, 0, 56) ;
  h37->SetXTitle("x, cells"); h37->SetYTitle("z, cells");
  Add2RawsList(h37,kLGpedRMSMod0, expert, !image, !saveCorr) ;
  TH2F * h38 = new TH2F("hpedRMSLGxyMod1","Low Gain pedestal RMS in module 1", 64, 0, 64, 56, 0, 56) ;
  h38->SetXTitle("x, cells"); h38->SetYTitle("z, cells");
  Add2RawsList(h38,kLGpedRMSMod1, expert, !image, !saveCorr) ;
  TH2F * h39 = new TH2F("hpedRMSLGxyMod2","Low Gain pedestal RMS in module 2", 64, 0, 64, 56, 0, 56) ;
  h39->SetXTitle("x, cells"); h39->SetYTitle("z, cells");
  Add2RawsList(h39,kLGpedRMSMod2, expert, !image, !saveCorr) ;
  TH2F * h40 = new TH2F("hpedRMSLGxyMod3","Low Gain pedestal RMS in module 3", 64, 0, 64, 56, 0, 56) ;
  h40->SetXTitle("x, cells"); h40->SetYTitle("z, cells");
  Add2RawsList(h40,kLGpedRMSMod3, expert, !image, !saveCorr) ;
  TH2F * h41 = new TH2F("hpedRMSLGxyMod4","Low Gain pedestal RMS in module 4", 64, 0, 64, 56, 0, 56) ;
  h41->SetXTitle("x, cells"); h41->SetYTitle("z, cells");
  Add2RawsList(h41,kLGpedRMSMod4, expert, !image, !saveCorr) ;
  //
  ClonePerTrigClass(IlcQAv1::kRAWS); // this should be the last line
}

//____________________________________________________________________________
void IlcRSTACKQADataMakerRec::MakeESDs(IlcESDEvent * esd)
{
  // make QA data from ESDs 
  Int_t nTot = 0 ; 
  Double_t eTot = 0 ; 
  for ( Int_t index = 0; index < esd->GetNumberOfCaloClusters() ; index++ ) {
    IlcESDCaloCluster * clu = esd->GetCaloCluster(index) ;
    if( clu->IsPHOS() ) {
      FillESDsData(kESDSpec,clu->E()) ;
      const Double_t * pid = clu->GetPID() ;
      FillESDsData(kESDpid,pid[IlcPID::kPhoton]) ;
      eTot+=clu->E() ;
      nTot++ ;
    } 
  }
  FillESDsData(kESDNtot,nTot) ;
  FillESDsData(kESDEtot,eTot) ;
  //
  IncEvCountCycleESDs();
  IncEvCountTotalESDs();
  //
}

//____________________________________________________________________________
void IlcRSTACKQADataMakerRec::MakeRaws(IlcRawReader* rawReader)
{
  //Fill prepared histograms with Raw digit properties

  rawReader->Reset() ;

  const TObjArray* maps = IlcRSTACKRecoParam::GetMappings();
  if(!maps) IlcFatal("Cannot retrieve ALTRO mappings!!");

  IlcAltroMapping *mapping[20];
  for(Int_t i = 0; i < 20; i++) {
    mapping[i] = (IlcAltroMapping*)maps->At(i);
  }

  IlcCaloRawStreamV3 fRawStream(rawReader,"RSTACK",mapping);

  IlcRSTACKRawFitterv0 * fitter ;
  if     (strcmp(GetRecoParam()->EMCFitterVersion(),"v1")==0)
    fitter=new IlcRSTACKRawFitterv1();
  else if(strcmp(GetRecoParam()->EMCFitterVersion(),"v2")==0)
    fitter=new IlcRSTACKRawFitterv2();
  else
    fitter=new IlcRSTACKRawFitterv0();
  Double_t lgEtot=0. ;
  Double_t hgEtot=0. ;
  Int_t    lgNtot=0 ;
  Int_t    hgNtot=0 ;
  Int_t    hgNtotM1RCU0=0, hgNtotM1RCU1=0, hgNtotM1RCU2=0, hgNtotM1RCU3=0;
  Int_t    hgNtotM2RCU0=0, hgNtotM2RCU1=0, hgNtotM2RCU2=0, hgNtotM2RCU3=0;
  Int_t    hgNtotM3RCU0=0, hgNtotM3RCU1=0, hgNtotM3RCU2=0, hgNtotM3RCU3=0;


  while (fRawStream.NextDDL()) {
    Int_t RCUnum = fRawStream.GetDDLNumber() - 8;

    while (fRawStream.NextChannel()) {
      Int_t module   = fRawStream.GetModule();
      Int_t cellX    = fRawStream.GetCellX();
      Int_t cellZ    = fRawStream.GetCellZ();
      Int_t caloFlag = fRawStream.GetCaloFlag(); // 0=LG, 1=HG, 2=TRU

      if(caloFlag!=0 && caloFlag!=1) continue; //TRU data!

      fitter->SetChannelGeo(module+1,cellX+1,cellZ+1,caloFlag);

      if(fitter->GetAmpOffset()==0 && fitter->GetAmpThreshold()==0) {
	Short_t altroCFG1 = fRawStream.GetAltroCFG1();
	Bool_t ZeroSuppressionEnabled = (altroCFG1 >> 15) & 0x1;
	if(ZeroSuppressionEnabled) {
	  Short_t offset = (altroCFG1 >> 10) & 0xf;
	  Short_t threshold = altroCFG1 & 0x3ff;
	  fitter->SubtractPedestals(kFALSE);
	  fitter->SetAmpOffset(offset);
	  fitter->SetAmpThreshold(threshold);
	}
	else
	  fitter->SubtractPedestals(kTRUE);
      }

      Int_t nBunches = 0;
      while (fRawStream.NextBunch()) {
	nBunches++;
	if (nBunches > 1) continue;
	const UShort_t *sig = fRawStream.GetSignals();
	Int_t sigStart      = fRawStream.GetStartTimeBin();
	Int_t sigLength     = fRawStream.GetBunchLength();
	fitter->Eval(sig,sigStart,sigLength);
      } // End of NextBunch()

      Double_t energy = fitter->GetEnergy() ; 
      Double_t time   = fitter->GetTime() ;

      if (caloFlag == 0) { // LG
	FillRawsData(kLGpedRMS,fitter->GetPedestalRMS()) ;
	switch(module){
        case 0: FillRawsData(kLGmod0,cellX,cellZ) ; break ;
        case 1: FillRawsData(kLGmod1,cellX,cellZ) ; break ;
        case 2: FillRawsData(kLGmod2,cellX,cellZ) ; break ;
        case 3: FillRawsData(kLGmod3,cellX,cellZ) ; break ;
        case 4: FillRawsData(kLGmod4,cellX,cellZ) ; break ;
	}
	switch (module){
        case 0: FillRawsData(kLGpedRMSMod0,cellX,cellZ,fitter->GetPedestalRMS()) ; break ;
        case 1: FillRawsData(kLGpedRMSMod1,cellX,cellZ,fitter->GetPedestalRMS()) ; break ;
        case 2: FillRawsData(kLGpedRMSMod2,cellX,cellZ,fitter->GetPedestalRMS()) ; break ;
        case 3: FillRawsData(kLGpedRMSMod3,cellX,cellZ,fitter->GetPedestalRMS()) ; break ;
        case 4: FillRawsData(kLGpedRMSMod4,cellX,cellZ,fitter->GetPedestalRMS()) ; break ;
	}
	//if quality was evaluated, fill histo
	if(strcmp(GetRecoParam()->EMCFitterVersion(),"v1")==0){
	  switch (module){
	  case 0: FillRawsData(kLGqualMod0,cellX,cellZ,fitter->GetSignalQuality()) ; break ;
	  case 1: FillRawsData(kLGqualMod1,cellX,cellZ,fitter->GetSignalQuality()) ; break ;
	  case 2: FillRawsData(kLGqualMod2,cellX,cellZ,fitter->GetSignalQuality()) ; break ;
	  case 3: FillRawsData(kLGqualMod3,cellX,cellZ,fitter->GetSignalQuality()) ; break ;
	  case 4: FillRawsData(kLGqualMod4,cellX,cellZ,fitter->GetSignalQuality()) ; break ;
	  }
	}                                  
	FillRawsData(kNmodLG,module) ;
	FillRawsData(kLGtime,time) ; 
	FillRawsData(kSpecLG,energy) ;    
	lgEtot+=energy ;
	lgNtot++ ;   
      }
      else if (caloFlag == 1) { // HG        
	FillRawsData(kHGpedRMS,fitter->GetPedestalRMS()) ;
	switch (module){
	case 0: FillRawsData(kHGmod0,cellX,cellZ) ; break ;
	case 1: FillRawsData(kHGmod1,cellX,cellZ) ; break ;
	case 2: FillRawsData(kHGmod2,cellX,cellZ) ; break ;
	case 3: FillRawsData(kHGmod3,cellX,cellZ) ; break ;
	case 4: FillRawsData(kHGmod4,cellX,cellZ) ; break ;
	}
	switch (module){
	case 0: FillRawsData(kHGpedRMSMod0,cellX,cellZ,fitter->GetPedestalRMS()) ; break ;
	case 1: FillRawsData(kHGpedRMSMod1,cellX,cellZ,fitter->GetPedestalRMS()) ; break ;
	case 2: FillRawsData(kHGpedRMSMod2,cellX,cellZ,fitter->GetPedestalRMS()) ; break ;
	case 3: FillRawsData(kHGpedRMSMod3,cellX,cellZ,fitter->GetPedestalRMS()) ; break ;
	case 4: FillRawsData(kHGpedRMSMod4,cellX,cellZ,fitter->GetPedestalRMS()) ; break ;
	}               
	//if quality was evaluated, fill histo
	if(strcmp(GetRecoParam()->EMCFitterVersion(),"v1")==0){
	  switch (module){
	  case 0: FillRawsData(kHGqualMod0,cellX,cellZ,fitter->GetSignalQuality()) ; break ;
	  case 1: FillRawsData(kHGqualMod1,cellX,cellZ,fitter->GetSignalQuality()) ; break ;
	  case 2: FillRawsData(kHGqualMod2,cellX,cellZ,fitter->GetSignalQuality()) ; break ;
	  case 3: FillRawsData(kHGqualMod3,cellX,cellZ,fitter->GetSignalQuality()) ; break ;
	  case 4: FillRawsData(kHGqualMod4,cellX,cellZ,fitter->GetSignalQuality()) ; break ;
	  }	  
	}
	FillRawsData(kNmodHG,module) ; 
	FillRawsData(kNRCUHG,RCUnum) ; 
	FillRawsData(kHGtime,time) ;  
	FillRawsData(kSpecHG,energy) ;
	hgEtot+=energy ; 
	hgNtot++ ;  
        if(RCUnum==0)hgNtotM1RCU0++;
        if(RCUnum==1)hgNtotM1RCU1++;
        if(RCUnum==2)hgNtotM1RCU2++;
        if(RCUnum==3)hgNtotM1RCU3++;
        if(RCUnum==4)hgNtotM2RCU0++;
        if(RCUnum==5)hgNtotM2RCU1++;
        if(RCUnum==6)hgNtotM2RCU2++;
        if(RCUnum==7)hgNtotM2RCU3++;
        if(RCUnum==8)hgNtotM3RCU0++;
        if(RCUnum==9)hgNtotM3RCU1++;
        if(RCUnum==10)hgNtotM3RCU2++;
        if(RCUnum==11)hgNtotM3RCU3++;
      }
    }  // End of NextChannel
  } // End of NextDDL
  delete fitter;

  FillRawsData(kEtotLG,lgEtot) ; 
  TParameter<double> * p;
  p = dynamic_cast<TParameter<double>*>(GetParameterList()->
					FindObject(Form("%s_%s_%s", GetName(), 
							IlcQAv1::GetTaskName(IlcQAv1::kRAWS).Data(), 
							GetRawsData(kEtotLG)->GetName()))) ; 
  if (p) p->SetVal(lgEtot) ; 
  FillRawsData(kEtotHG,hgEtot) ;  
  p = dynamic_cast<TParameter<double>*>(GetParameterList()->
					FindObject(Form("%s_%s_%s", GetName(), 
							IlcQAv1::GetTaskName(IlcQAv1::kRAWS).Data(), 
							GetRawsData(kEtotHG)->GetName()))) ; 
  if (p) p->SetVal(hgEtot) ; 
  FillRawsData(kNtotLG,lgNtot) ;
  p = dynamic_cast<TParameter<double>*>(GetParameterList()->
					FindObject(Form("%s_%s_%s", GetName(), 
							IlcQAv1::GetTaskName(IlcQAv1::kRAWS).Data(), 
							GetRawsData(kNtotLG)->GetName()))) ; 
  if (p) p->SetVal(lgNtot) ; 
  FillRawsData(kNtotHG,hgNtot) ;
  p = dynamic_cast<TParameter<double>*>(GetParameterList()->
					FindObject(Form("%s_%s_%s", 
							GetName(), IlcQAv1::GetTaskName(IlcQAv1::kRAWS).Data(), 
							GetRawsData(kNtotHG)->GetName()))) ; 
  if (p) p->SetVal(hgNtot) ; 

  FillRawsData(kNtotHG_1_0,hgNtotM1RCU0) ;
  p = dynamic_cast<TParameter<double>*>(GetParameterList()->
                                        FindObject(Form("%s_%s_%s",
                                                        GetName(), IlcQAv1::GetTaskName(IlcQAv1::kRAWS).Data(),
                                                        GetRawsData(kNtotHG_1_0)->GetName()))) ;
  if (p) p->SetVal(hgNtotM1RCU0) ;

  FillRawsData(kNtotHG_1_1,hgNtotM1RCU1) ;
  p = dynamic_cast<TParameter<double>*>(GetParameterList()->
                                        FindObject(Form("%s_%s_%s",
                                                        GetName(), IlcQAv1::GetTaskName(IlcQAv1::kRAWS).Data(),
                                                        GetRawsData(kNtotHG_1_1)->GetName()))) ;
  if (p) p->SetVal(hgNtotM1RCU1) ;

  FillRawsData(kNtotHG_1_2,hgNtotM1RCU2) ;
  p = dynamic_cast<TParameter<double>*>(GetParameterList()->
                                        FindObject(Form("%s_%s_%s",
                                                        GetName(), IlcQAv1::GetTaskName(IlcQAv1::kRAWS).Data(),
                                                        GetRawsData(kNtotHG_1_2)->GetName()))) ;
  if (p) p->SetVal(hgNtotM1RCU2) ;

  FillRawsData(kNtotHG_1_3,hgNtotM1RCU3) ;
  p = dynamic_cast<TParameter<double>*>(GetParameterList()->
                                        FindObject(Form("%s_%s_%s",
                                                        GetName(), IlcQAv1::GetTaskName(IlcQAv1::kRAWS).Data(),
                                                        GetRawsData(kNtotHG_1_3)->GetName()))) ;
  if (p) p->SetVal(hgNtotM1RCU3) ;

  FillRawsData(kNtotHG_2_0,hgNtotM2RCU0) ;
  p = dynamic_cast<TParameter<double>*>(GetParameterList()->
                                        FindObject(Form("%s_%s_%s",
                                                        GetName(), IlcQAv1::GetTaskName(IlcQAv1::kRAWS).Data(),
                                                        GetRawsData(kNtotHG_2_0)->GetName()))) ;
  if (p) p->SetVal(hgNtotM2RCU0) ;

  FillRawsData(kNtotHG_2_1,hgNtotM2RCU1) ;
  p = dynamic_cast<TParameter<double>*>(GetParameterList()->
                                        FindObject(Form("%s_%s_%s",
                                                        GetName(), IlcQAv1::GetTaskName(IlcQAv1::kRAWS).Data(),
                                                        GetRawsData(kNtotHG_2_1)->GetName()))) ;
  if (p) p->SetVal(hgNtotM2RCU1) ;

  FillRawsData(kNtotHG_2_2,hgNtotM2RCU2) ;
  p = dynamic_cast<TParameter<double>*>(GetParameterList()->
                                        FindObject(Form("%s_%s_%s",
                                                        GetName(), IlcQAv1::GetTaskName(IlcQAv1::kRAWS).Data(),
                                                        GetRawsData(kNtotHG_2_2)->GetName()))) ;
  if (p) p->SetVal(hgNtotM2RCU2) ;

  FillRawsData(kNtotHG_2_3,hgNtotM2RCU3) ;
  p = dynamic_cast<TParameter<double>*>(GetParameterList()->
                                        FindObject(Form("%s_%s_%s",
                                                        GetName(), IlcQAv1::GetTaskName(IlcQAv1::kRAWS).Data(),
                                                        GetRawsData(kNtotHG_2_3)->GetName()))) ;
  if (p) p->SetVal(hgNtotM2RCU3) ;

  FillRawsData(kNtotHG_3_0,hgNtotM3RCU0) ;
  p = dynamic_cast<TParameter<double>*>(GetParameterList()->
                                        FindObject(Form("%s_%s_%s",
                                                        GetName(), IlcQAv1::GetTaskName(IlcQAv1::kRAWS).Data(),
                                                        GetRawsData(kNtotHG_3_0)->GetName()))) ;
  if (p) p->SetVal(hgNtotM3RCU0) ;

  FillRawsData(kNtotHG_3_1,hgNtotM3RCU1) ;
  p = dynamic_cast<TParameter<double>*>(GetParameterList()->
                                        FindObject(Form("%s_%s_%s",
                                                        GetName(), IlcQAv1::GetTaskName(IlcQAv1::kRAWS).Data(),
                                                        GetRawsData(kNtotHG_3_1)->GetName()))) ;
  if (p) p->SetVal(hgNtotM3RCU1) ;

  FillRawsData(kNtotHG_3_2,hgNtotM3RCU2) ;
  p = dynamic_cast<TParameter<double>*>(GetParameterList()->
                                        FindObject(Form("%s_%s_%s",
                                                        GetName(), IlcQAv1::GetTaskName(IlcQAv1::kRAWS).Data(),
                                                        GetRawsData(kNtotHG_3_2)->GetName()))) ;
  if (p) p->SetVal(hgNtotM3RCU2) ;

  FillRawsData(kNtotHG_3_3,hgNtotM3RCU3) ;
  p = dynamic_cast<TParameter<double>*>(GetParameterList()->
                                        FindObject(Form("%s_%s_%s",
                                                        GetName(), IlcQAv1::GetTaskName(IlcQAv1::kRAWS).Data(),
                                                        GetRawsData(kNtotHG_3_3)->GetName()))) ;
  if (p) p->SetVal(hgNtotM3RCU3) ;

  //
  IncEvCountCycleRaws();
  IncEvCountTotalRaws();
  //
}

//____________________________________________________________________________
void IlcRSTACKQADataMakerRec::MakeDigits()
{
  // makes data from Digits
  
  if ( ! GetDigitsData(kDigitsMul) ) InitDigits() ;
  FillDigitsData(kDigitsMul,fDigitsArray->GetEntriesFast()) ; 
  TIter next(fDigitsArray) ; 
  IlcRSTACKDigit * digit ; 
  while ( (digit = dynamic_cast<IlcRSTACKDigit *>(next())) ) {
    FillDigitsData(kDigits, digit->GetEnergy()) ;
  }  
  //
}

//____________________________________________________________________________
void IlcRSTACKQADataMakerRec::MakeDigits(TTree * digitTree)
{
  // makes data from Digit Tree
  if (fDigitsArray) 
    fDigitsArray->Clear() ; 
  else 
    fDigitsArray = new TClonesArray("IlcRSTACKDigit", 1000) ; 
  
  TBranch * branch = digitTree->GetBranch("RSTACK") ;
  if ( ! branch ) {IlcWarning("RSTACK branch in Digit Tree not found"); return;} 
  branch->SetAddress(&fDigitsArray) ;
  branch->GetEntry(0) ; 
  MakeDigits() ; 
  //
  IncEvCountCycleDigits();
  IncEvCountTotalDigits();
  //
}

//____________________________________________________________________________
void IlcRSTACKQADataMakerRec::MakeRecPoints(TTree * clustersTree)
{
  {
    // makes data from RecPoints
    TBranch *emcbranch = clustersTree->GetBranch("RSTACKEmcRP");
    if (!emcbranch) { 
      IlcError("can't get the branch with the RSTACK EMC clusters !");
      return;
    }
    
    TObjArray * emcrecpoints = new TObjArray(100) ;
    emcbranch->SetAddress(&emcrecpoints);
    emcbranch->GetEntry(0);
    
    FillRecPointsData(kRPNtot,emcrecpoints->GetEntriesFast()) ; 
    TIter next(emcrecpoints) ; 
    IlcRSTACKEmcRecPoint * rp ; 
    Double_t eTot = 0. ; 
    while ( (rp = static_cast<IlcRSTACKEmcRecPoint *>(next())) ) {
      FillRecPointsData(kRPSpec, rp->GetEnergy()) ;
      Int_t mod = rp->GetRSTACKMod() ;
      TVector3 pos ;
      rp->GetLocalPosition(pos) ;
      switch(mod){
        case 1: FillRecPointsData(kRPmod1,pos.X(),pos.Z()) ; break ;
        case 2: FillRecPointsData(kRPmod2,pos.X(),pos.Z()) ; break ;
        case 3: FillRecPointsData(kRPmod3,pos.X(),pos.Z()) ; break ;
        case 4: FillRecPointsData(kRPmod4,pos.X(),pos.Z()) ; break ;
        case 5: FillRecPointsData(kRPmod5,pos.X(),pos.Z()) ; break ;
      }
      eTot+= rp->GetEnergy() ;
    }
    FillRecPointsData(kRPEtot,eTot) ;
    emcrecpoints->Delete();
    delete emcrecpoints;
  }
  {
    TBranch *cpvbranch = clustersTree->GetBranch("RSTACKCpvRP");
    if (!cpvbranch) { 
      IlcError("can't get the branch with the RSTACK CPV clusters !");
      return;
    }
    TObjArray *cpvrecpoints = new TObjArray(100) ;
    cpvbranch->SetAddress(&cpvrecpoints);
    cpvbranch->GetEntry(0);
    
    FillRecPointsData(kRPNcpv,cpvrecpoints->GetEntriesFast()) ; 
    cpvrecpoints->Delete();
    delete cpvrecpoints;
  }
  //
  IncEvCountCycleRecPoints();
  IncEvCountTotalRecPoints();
  //
}

//____________________________________________________________________________ 
void IlcRSTACKQADataMakerRec::StartOfDetectorCycle()
{
  //Detector specific actions at start of cycle
  
}

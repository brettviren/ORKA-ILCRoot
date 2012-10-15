
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

/* $Id$ */
#include <TString.h>
#include <TVector3.h>
#include <TMath.h>

#include "IlcPythia8.h"
#include "IlcLog.h"
#include "IlcStack.h"
#include "IlcPythiaRndm.h"


ClassImp(IlcPythia8)
    
// Particles produced in string fragmentation point directly to either of the two endpoints
// of the string (depending in the side they were generated from).
//    SetMSTU(16,2); // ????
//  String drawing almost completely minimizes string length.
//  Probability that an additional interaction gives two gluons
//  ... with color connection to nearest neighbours
//    SetPARP(85,0.9);
//  ... as closed gluon loop
//    SetPARP(86,0.95);
// Lambda_FSR scale.
//	SetPARJ(81, 0.29);
// Baryon production model
//	SetMSTJ(12,3); 
// String fragmentation
//	SetMSTJ(1,1);  
// sea quarks can be used for baryon formation
//      SetMSTP(88,2); 
// choice of max. virtuality for ISR
//	SetMSTP(68,1);
// regularisation scheme of ISR 
//	SetMSTP(70,2);
// all resonance decays switched on
//    SetMSTP(41,1);   
IlcPythia8* IlcPythia8::fgIlcPythia8=NULL;

IlcPythia8::IlcPythia8():
    IlcTPythia8(),
    IlcPythiaBase(),
    fProcess(kPyMb),
    fEcms(0.),
    fStrucFunc(kCTEQ5L),
    fCellJet(),
    fEtSeed(0.),
    fMinEtJet(0.),
    fRJet(0.),
    fClusterJet(),
    fYScale(0.),
    fPtScale(0.),
    fNJetMin(0),
    fNJetMax(0)
{
// Default Constructor
//
//  Set random number
    if (!IlcPythiaRndm::GetPythiaRandom()) 
	IlcPythiaRndm::SetPythiaRandom(GetRandom());
}

IlcPythia8::IlcPythia8(const IlcPythia8& pythia):
    IlcTPythia8(), 
    IlcPythiaBase(),
    fProcess(kPyMb),
    fEcms(0.),
    fStrucFunc(kCTEQ5L),
    fCellJet(),
    fEtSeed(0.),
    fMinEtJet(0.),
    fRJet(0.),
    fClusterJet(),
    fYScale(0.),
    fPtScale(0.),
    fNJetMin(0),
    fNJetMax(0)
{
    // Copy Constructor
    pythia.Copy(*this);
}

void IlcPythia8::ProcInit(Process_t process, Float_t energy, StrucFunc_t strucfunc)
{
// Initialise the process to generate 
    if (!IlcPythiaRndm::GetPythiaRandom()) 
      IlcPythiaRndm::SetPythiaRandom(GetRandom());
    
    fProcess   = process;
    fEcms      = energy;
    fStrucFunc = strucfunc;
//...Switch off decay of pi0, K0S, Lambda, Sigma+-, Xi0-, Omega-.
    ReadString("111:mayDecay  = off");
    ReadString("310:mayDecay  = off");
    ReadString("3122:mayDecay = off");
    ReadString("3112:mayDecay = off");
    ReadString("3212:mayDecay = off");
    ReadString("3222:mayDecay = off");
    ReadString("3312:mayDecay = off");
    ReadString("3322:mayDecay = off");
    ReadString("3334:mayDecay = off");
    // Select structure function 

    ReadString("PDF:useLHAPDF = on");
    ReadString(Form("PDF:LHAPDFset = %s", IlcStructFuncType::PDFsetName(fStrucFunc).Data()));

    // Particles produced in string fragmentation point directly to either of the two endpoints
    // of the string (depending in the side they were generated from).

//    SetMSTU(16,2); // ????

//
// Pythia initialisation for selected processes//
//
    switch (process) 
    {
    case kPyOldUEQ2ordered:  //Old underlying events with Q2 ordered QCD processes
//        Multiple interactions on.
	ReadString("PartonLevel:MI = on");
// Double Gaussian matter distribution.
	ReadString("MultipleInteractions:bProfile = 2");
	ReadString("MultipleInteractions:coreFraction = 0.5");
	ReadString("MultipleInteractions:coreRadius = 0.4");
//  pT0.
	ReadString("MultipleInteractions:pTmin = 2.0");
//  Reference energy for pT0 and energy rescaling pace.
	ReadString("MultipleInteractions:ecmRef = 1800.");
	ReadString("MultipleInteractions:ecmPow = 0.25");
//  String drawing almost completely minimizes string length.
//	SetPARP(85,0.9);
//	SetPARP(86,0.95);
// ISR and FSR activity.
// Q^2 scale of the hard scattering
	ReadString("SigmaProcess:factorMultFac = 4.");
// Lambda_FSR scale.
//	SetPARJ(81, 0.29);
	break;
    case kPyOldUEQ2ordered2:   
// Old underlying events with Q2 ordered QCD processes
// Multiple interactions on.
	ReadString("PartonLevel:MI = on");
// Double Gaussian matter distribution.
	ReadString("MultipleInteractions:bProfile = 2");
	ReadString("MultipleInteractions:coreFraction = 0.5");
	ReadString("MultipleInteractions:coreRadius = 0.4");
// pT0.
	ReadString("MultipleInteractions:pTmin = 2.0");
// Reference energy for pT0 and energy rescaling pace.
	ReadString("MultipleInteractions:ecmRef = 1800.");
	ReadString("MultipleInteractions:ecmPow = 0.16");
// String drawing almost completely minimizes string length.
//	SetPARP(85,0.9);
//	SetPARP(86,0.95);
// ISR and FSR activity.
	ReadString("SigmaProcess:factorMultFac = 4.");
// Lambda_FSR scale.
//	SetPARJ(81,0.29);	
	break;
    case kPyOldPopcorn:  
// Old production mechanism: Old Popcorn
	ReadString("HardQCD:all = on");
//	SetMSTJ(12,3); 
// (D=2) Like MSTJ(12)=2 but added prod ofthe 1er rank baryon
//	SetMSTP(88,2); 
// (D=1)see can be used to form  baryons (BARYON JUNCTION)
//	SetMSTJ(1,1);  
	AtlasTuning();
	break;
    case kPyCharm:
	ReadString("HardQCD:gg2ccbar = on");
	ReadString("HardQCD:qqbar2ccbar = on");
//  heavy quark masses
	ReadString("ParticleData:mcRun = 1.2");
//
//    primordial pT
	ReadString("BeamRemnants:primordialKT = on");
	ReadString("BeamRemnants:primordialKTsoft = 0.");
	ReadString("BeamRemnants:primordialKThard = 1.");
	ReadString("BeamRemnants:halfScaleForKT = 0.");
	ReadString("BeamRemnants:halfMassForKT = 0.");
	break;
    case kPyBeauty:
	ReadString("HardQCD:gg2bbbar = on");
	ReadString("HardQCD:qqbar2bbbar = on");
	ReadString("ParticleData:mbRun = 4.75");
	break;
    case kPyJpsi:
// gg->J/Psi g
	ReadString("Charmonium:gg2QQbar[3S1(1)]g = on");
	break;
    case kPyJpsiChi:
	ReadString("Charmonium:all = on");
	break;
    case kPyCharmUnforced:
// gq->qg   
	ReadString("HardQCD:gq2qg = on");
// gg->qq
	ReadString("HardQCD:gg2qq = on");
// gg->gg
	ReadString("HardQCD:gg2gg = on");
	break;
    case kPyBeautyUnforced:
// gq->qg   
	ReadString("HardQCD:gq2qg = on");
// gg->qq
	ReadString("HardQCD:gg2qq = on");
// gg->gg
	ReadString("HardQCD:gg2gg = on");
	break;
    case kPyMb:
// Minimum Bias pp-Collisions
//
//   
//      select Pythia min. bias model
// single diffraction AB-->XB
	ReadString("SoftQCD:minBias = on");
	ReadString("SoftQCD:singleDiffractive = on");
	ReadString("SoftQCD:doubleDiffractive = on");
	AtlasTuning();
	break;
    case kPyMbDefault:
// Minimum Bias pp-Collisions
//
//   
//      select Pythia min. bias model
	ReadString("SoftQCD:minBias = on");
	ReadString("SoftQCD:singleDiffractive = on");
	ReadString("SoftQCD:doubleDiffractive = on");
	break;
    case kPyLhwgMb:
// Les Houches Working Group 05 Minimum Bias pp-Collisions: hep-ph/0604120
//  -> Pythia 6.3 or above is needed
//   
	ReadString("SoftQCD:minBias = on");
	ReadString("SoftQCD:singleDiffractive = on");
	ReadString("SoftQCD:doubleDiffractive = on");
	ReadString(Form("PDF:LHAPDFset = %s", IlcStructFuncType::PDFsetName(kCTEQ6ll).Data()));

//	SetMSTP(68,1);
//	SetMSTP(70,2);
//	ReadString("PartonLevel:MI = on");
// Double Gaussian matter distribution.
	ReadString("MultipleInteractions:bProfile = 2");
	ReadString("MultipleInteractions:coreFraction = 0.5");
	ReadString("MultipleInteractions:coreRadius = 0.5");
	ReadString("MultipleInteractions:expPow = 0.16");
	ReadString("MultipleInteractions:pTmin = 2.3");
//	SetMSTP(88,1);
//	SetPARP(85,0.9);           // Regulates gluon prod. mechanism
	break;
    case kPyMbNonDiffr:
// Minimum Bias pp-Collisions
//
//   
//      select Pythia min. bias model
	ReadString("SoftQCD:minBias = on");
	AtlasTuning();
	break;
    case kPyMbMSEL1:
	ConfigHeavyFlavor();
// Intrinsic <kT^2>
	ReadString("BeamRemnants:primordialKT = on");
	ReadString("BeamRemnants:primordialKTsoft = 0.");
	ReadString("BeamRemnants:primordialKThard = 1.");
	ReadString("BeamRemnants:halfScaleForKT = 0.");
	ReadString("BeamRemnants:halfMassForKT = 0.");
// Set Q-quark mass
	ReadString("ParticleData:mcRun = 1.20");
	ReadString("ParticleData:mbRun = 4.78");
// Atlas Tuning
	AtlasTuning();
	break;
    case kPyJets:
//
//  QCD Jets
//
	ReadString("HardQCD:all = on");
//
//  Pythia Tune A (CDF)
//
	ReadString("PartonLevel:MI = on");
	ReadString("MultipleInteractions:pTmin = 2.0");
	ReadString("MultipleInteractions:pT0Ref = 2.8");
	ReadString("MultipleInteractions:ecmRef = 1800.");
	ReadString("MultipleInteractions:expPow = 0.25");
	ReadString("MultipleInteractions:bProfile = 2");
	ReadString("MultipleInteractions:coreFraction = 0.16");
	ReadString("MultipleInteractions:coreRadius = 0.4");
	ReadString("SigmaProcess:factorMultFac = 2.5");
//	SetPARP(85,0.90) ;         // Regulates gluon prod. mechanism
//	SetPARP(86,0.95);          // Regulates gluon prod. mechanism
       break;
    case kPyDirectGamma:
	ReadString("PromptPhoton:all = on");
	break;
    case kPyCharmPbPbMNR:
    case kPyD0PbPbMNR:
    case kPyDPlusPbPbMNR:
    case kPyDPlusStrangePbPbMNR:
      // Tuning of Pythia parameters aimed to get a resonable agreement
      // between with the NLO calculation by Mangano, Nason, Ridolfi for the
      // c-cbar single inclusive and double differential distributions.
      // This parameter settings are meant to work with Pb-Pb collisions
      // (IlcGenPythia::SetNuclei) and with kCTEQ4L PDFs.
      // To get a good agreement the minimum ptHard (IlcGenPythia::SetPtHard)
      // has to be set to 2.1GeV. Example in ConfigCharmPPR.C.
	ConfigHeavyFlavor();
      // Intrinsic <kT>
	ReadString("BeamRemnants:primordialKT = on");
	ReadString("BeamRemnants:primordialKTsoft = 0.");
	ReadString("BeamRemnants:primordialKThard = 1.304");
	ReadString("BeamRemnants:halfScaleForKT = 0.");
	ReadString("BeamRemnants:halfMassForKT = 0.");
      // Set c-quark mass
	ReadString("ParticleData:mcRun = 1.20");
	break;
    case kPyCharmpPbMNR:
    case kPyD0pPbMNR:
    case kPyDPluspPbMNR:
    case kPyDPlusStrangepPbMNR:
      // Tuning of Pythia parameters aimed to get a resonable agreement
      // between with the NLO calculation by Mangano, Nason, Ridolfi for the
      // c-cbar single inclusive and double differential distributions.
      // This parameter settings are meant to work with p-Pb collisions
      // (IlcGenPythia::SetNuclei) and with kCTEQ4L PDFs.
      // To get a good agreement the minimum ptHard (IlcGenPythia::SetPtHard)
      // has to be set to 2.1GeV. Example in ConfigCharmPPR.C.
	ConfigHeavyFlavor();
      // Intrinsic <kT>
	ReadString("BeamRemnants:primordialKT = on");
	ReadString("BeamRemnants:primordialKTsoft = 0.");
	ReadString("BeamRemnants:primordialKThard = 1.16");
	ReadString("BeamRemnants:halfScaleForKT = 0.");
	ReadString("BeamRemnants:halfMassForKT = 0.");
      // Set c-quark mass
	ReadString("ParticleData:mcRun = 1.20");
      break;
    case kPyCharmppMNR:
    case kPyD0ppMNR:
    case kPyDPlusppMNR:
    case kPyDPlusStrangeppMNR:
    case kPyLambdacppMNR:
      // Tuning of Pythia parameters aimed to get a resonable agreement
      // between with the NLO calculation by Mangano, Nason, Ridolfi for the
      // c-cbar single inclusive and double differential distributions.
      // This parameter settings are meant to work with pp collisions
      // (IlcGenPythia::SetNuclei) and with kCTEQ4L PDFs.
      // To get a good agreement the minimum ptHard (IlcGenPythia::SetPtHard)
      // has to be set to 2.1GeV. Example in ConfigCharmPPR.C.
	ConfigHeavyFlavor();
      // Intrinsic <kT^2>
	ReadString("BeamRemnants:primordialKT = on");
	ReadString("BeamRemnants:primordialKTsoft = 0.");
	ReadString("BeamRemnants:primordialKThard = 1.");
	ReadString("BeamRemnants:halfScaleForKT = 0.");
	ReadString("BeamRemnants:halfMassForKT = 0.");
      // Set c-quark mass
	ReadString("ParticleData:mcRun = 1.20");
      break;
    case kPyCharmppMNRwmi:
      // Tuning of Pythia parameters aimed to get a resonable agreement
      // between with the NLO calculation by Mangano, Nason, Ridolfi for the
      // c-cbar single inclusive and double differential distributions.
      // This parameter settings are meant to work with pp collisions
      // and with kCTEQ5L PDFs.
      // Added multiple interactions according to ATLAS tune settings.
      // To get a "reasonable" agreement with MNR results, events have to be 
      // generated with the minimum ptHard (IlcGenPythia::SetPtHard)
      // set to 2.76 GeV.
      // To get a "perfect" agreement with MNR results, events have to be 
      // generated in four ptHard bins with the following relative 
      // normalizations:
      // 2.76-3 GeV: 25%
      //    3-4 GeV: 40%
      //    4-8 GeV: 29%
      //     >8 GeV:  6%
	ConfigHeavyFlavor();
      // Intrinsic <kT^2>
	ReadString("BeamRemnants:primordialKT = on");
	ReadString("BeamRemnants:primordialKTsoft = 0.");
	ReadString("BeamRemnants:primordialKThard = 1.");
	ReadString("BeamRemnants:halfScaleForKT = 0.");
	ReadString("BeamRemnants:halfMassForKT = 0.");
      // Set c-quark mass
	ReadString("ParticleData:mcRun = 1.20");
	AtlasTuning();
	break;
    case kPyBeautyPbPbMNR:
      // Tuning of Pythia parameters aimed to get a resonable agreement
      // between with the NLO calculation by Mangano, Nason, Ridolfi for the
      // b-bbar single inclusive and double differential distributions.
      // This parameter settings are meant to work with Pb-Pb collisions
      // (IlcGenPythia::SetNuclei) and with kCTEQ4L PDFs.
      // To get a good agreement the minimum ptHard (IlcGenPythia::SetPtHard)
      // has to be set to 2.75GeV. Example in ConfigBeautyPPR.C.
	ConfigHeavyFlavor();
      // QCD scales
	ReadString("SigmaProcess:factorMultFac = 1.");
      // Intrinsic <kT>
	ReadString("BeamRemnants:primordialKT = on");
	ReadString("BeamRemnants:primordialKTsoft = 0.");
	ReadString("BeamRemnants:primordialKThard = 2.035");
	ReadString("BeamRemnants:halfScaleForKT = 0.");
	ReadString("BeamRemnants:halfMassForKT = 0.");
      // Set b-quark mass
	ReadString("ParticleData:mbRun = 4.75");
      break;
    case kPyBeautypPbMNR:
      // Tuning of Pythia parameters aimed to get a resonable agreement
      // between with the NLO calculation by Mangano, Nason, Ridolfi for the
      // b-bbar single inclusive and double differential distributions.
      // This parameter settings are meant to work with p-Pb collisions
      // (IlcGenPythia::SetNuclei) and with kCTEQ4L PDFs.
      // To get a good agreement the minimum ptHard (IlcGenPythia::SetPtHard)
      // has to be set to 2.75GeV. Example in ConfigBeautyPPR.C.
	ConfigHeavyFlavor();
      // QCD scales
	ReadString("SigmaProcess:factorMultFac = 1.");
      // Intrinsic <kT>
	ReadString("BeamRemnants:primordialKT = on");
	ReadString("BeamRemnants:primordialKTsoft = 0.");
	ReadString("BeamRemnants:primordialKThard = 1.6");
	ReadString("BeamRemnants:halfScaleForKT = 0.");
	ReadString("BeamRemnants:halfMassForKT = 0.");
      // Set b-quark mass
	ReadString("ParticleData:mbRun = 4.75");
      break;
    case kPyBeautyppMNR:
      // Tuning of Pythia parameters aimed to get a resonable agreement
      // between with the NLO calculation by Mangano, Nason, Ridolfi for the
      // b-bbar single inclusive and double differential distributions.
      // This parameter settings are meant to work with pp collisions
      // (IlcGenPythia::SetNuclei) and with kCTEQ4L PDFs.
      // To get a good agreement the minimum ptHard (IlcGenPythia::SetPtHard)
      // has to be set to 2.75GeV. Example in ConfigBeautyPPR.C.
	ConfigHeavyFlavor();
	// QCD scales
	ReadString("SigmaProcess:factorMultFac = 1.");
	// Intrinsic <kT>
	ReadString("BeamRemnants:primordialKT = on");
	ReadString("BeamRemnants:primordialKTsoft = 0.");
	ReadString("BeamRemnants:primordialKThard = 1.0");
	ReadString("BeamRemnants:halfScaleForKT = 0.");
	ReadString("BeamRemnants:halfMassForKT = 0.");
	// Set b-quark mass
	ReadString("ParticleData:mbRun = 4.75");
      break;
     case kPyBeautyppMNRwmi:
      // Tuning of Pythia parameters aimed to get a resonable agreement
      // between with the NLO calculation by Mangano, Nason, Ridolfi for the
      // b-bbar single inclusive and double differential distributions.
      // This parameter settings are meant to work with pp collisions
      // and with kCTEQ5L PDFs.
      // Added multiple interactions according to ATLAS tune settings.
      // To get a "reasonable" agreement with MNR results, events have to be 
      // generated with the minimum ptHard (IlcGenPythia::SetPtHard)
      // set to 2.76 GeV.
      // To get a "perfect" agreement with MNR results, events have to be 
      // generated in four ptHard bins with the following relative 
      // normalizations:
      // 2.76-4 GeV:  5% 
      //    4-6 GeV: 31%
      //    6-8 GeV: 28%
      //     >8 GeV: 36%
	 ConfigHeavyFlavor();
	 // QCD scales
	 ReadString("SigmaProcess:factorMultFac = 1.");
	 // Intrinsic <kT>
	ReadString("BeamRemnants:primordialKT = on");
	ReadString("BeamRemnants:primordialKTsoft = 0.");
	ReadString("BeamRemnants:primordialKThard = 1.0");
	ReadString("BeamRemnants:halfScaleForKT = 0.");
	ReadString("BeamRemnants:halfMassForKT = 0.");
	// Set b-quark mass
	ReadString("ParticleData:mbRun = 4.75");
	AtlasTuning();
	break; 
    case kPyW:
	//Inclusive production of W+/-
	//f fbar -> W+ 
	ReadString("WeakSingleBoson:ffbar2W = on");
	// Initial/final parton shower on (Pythia default)
	// With parton showers on we are generating "W inclusive process"
	ReadString("PartonLevel:ISR = on");
	ReadString("PartonLevel:FSR = on");
	break;  
    case kPyZ:
	//Inclusive production of Z
	//f fbar -> Z/gamma
	ReadString("WeakSingleBoson:ffbar2gmZ = on");
        //only Z included, not gamma 
	ReadString("WeakZ0:gmZmode = 2");
	// Initial/final parton shower on (Pythia default)
	// With parton showers on we are generating "Z inclusive process"
	ReadString("PartonLevel:ISR = on");
	ReadString("PartonLevel:FSR = on");
	break;  
    case kPyMbWithDirectPhoton:
    case kPyBeautyJets:
    case kPyMbAtlasTuneMC09: 
      break;
    }
//
//  Initialize PYTHIA
//    SetMSTP(41,1);   // all resonance decays switched on
    Initialize(2212, 2212, fEcms);
}

void IlcPythia8::SetNuclei(Int_t /*a1*/, Int_t /*a2*/)
{
// Treat protons as inside nuclei with mass numbers a1 and a2  
//    The MSTP array in the PYPARS common block is used to enable and 
//    select the nuclear structure functions. 
//    MSTP(52)  : (D=1) choice of proton and nuclear structure-function library
//            =1: internal PYTHIA acording to MSTP(51) 
//            =2: PDFLIB proton  s.f., with MSTP(51)  = 1000xNGROUP+NSET
//    If the following mass number both not equal zero, nuclear corrections of the stf are used.
//    MSTP(192) : Mass number of nucleus side 1
//    MSTP(193) : Mass number of nucleus side 2
//    SetMSTP(52,2);
//    SetMSTP(192, a1);
//    SetMSTP(193, a2);  
}
	

IlcPythia8* IlcPythia8::Instance()
{ 
// Set random number generator 
    if (fgIlcPythia8) {
	return fgIlcPythia8;
    } else {
	fgIlcPythia8 = new IlcPythia8();
	return fgIlcPythia8;
    }
}

void IlcPythia8::PrintParticles()
{ 
// Print list of particl properties
    ReadString("Main:showAllParticleData");
}

void  IlcPythia8::ResetDecayTable()
{
//  Set default values for pythia decay switches
//    Int_t i;
//    for (i = 1; i <  501; i++) SetMDCY(i,1,fDefMDCY[i]);
//    for (i = 1; i < 2001; i++) SetMDME(i,1,fDefMDME[i]);
}

void  IlcPythia8::SetDecayTable()
{
//  Set default values for pythia decay switches
//
//    Int_t i;
//    for (i = 1; i <  501; i++) fDefMDCY[i] = GetMDCY(i,1);
//    for (i = 1; i < 2001; i++) fDefMDME[i] = GetMDME(i,1);
}

void  IlcPythia8::Pyclus(Int_t& njet)
{
//  Call Pythia clustering algorithm
//
    Bool_t ok = fClusterJet.analyze(Pythia8()->event, fYScale, fPtScale, fNJetMin, fNJetMax);
    njet = 0;
    if (ok) njet = fClusterJet.size();
}

void  IlcPythia8::Pycell(Int_t& njet)
{
//  Call Pythia jet reconstruction algorithm
//
    Bool_t ok = fCellJet.analyze(Pythia8()->event, fMinEtJet, fRJet, fEtSeed);
    njet = 0;
    if (ok) njet = fCellJet.size();
}

void IlcPythia8::GetJet(Int_t i, Float_t& px, Float_t& py, Float_t& pz, Float_t& e)
{
    // Get jet number i
    Float_t et = fCellJet.eT(i);
    px = et * TMath::Cos(fCellJet.phiWeighted(i));
    py = et * TMath::Sin(fCellJet.phiWeighted(i));
    pz = et * TMath::SinH(fCellJet.etaWeighted(i));
    e  = et * TMath::CosH(fCellJet.etaWeighted(i));
}

void IlcPythia8::GenerateEvent()
{
    // Generate one event
    IlcTPythia8::GenerateEvent();
}

void IlcPythia8::GenerateMIEvent()
{
    // New multiple interaction scenario
    IlcWarning("Not implemented. No event will be generated");
}

void IlcPythia8::PrintStatistics()
{
    // End of run statistics
    IlcTPythia8::PrintStatistics();
}

void IlcPythia8::EventListing()
{
    // End of run statistics
    IlcTPythia8::EventListing();
}

Int_t IlcPythia8::ProcessCode()
{
    // Returns the subprocess code for the current event
    return Pythia8()->info.code();
}

void IlcPythia8::ConfigHeavyFlavor()
{
    //
    // Default configuration for Heavy Flavor production
    //
    // All QCD processes
    //
    ReadString("HardQCD:all = on");

    // No multiple interactions
    ReadString("PartonLevel:MI = off");
    ReadString("MultipleInteractions:pTmin = 0.0");
    ReadString("MultipleInteractions:pT0Ref = 0.0");

    // Initial/final parton shower on (Pythia default)
    ReadString("PartonLevel:ISR = on");
    ReadString("PartonLevel:FSR = on");
    
    // 2nd order alpha_s
    ReadString("SigmaProcess:alphaSorder = 2");

    // QCD scales 
    ReadString("SigmaProcess:renormScale2 = 2");
    ReadString("SigmaProcess:renormMultFac = 1.");
}

void IlcPythia8::AtlasTuning()
{
    //
    // Configuration for the ATLAS tuning
    ReadString(Form("PDF:LHAPDFset = %s", IlcStructFuncType::PDFsetName(kCTEQ5L).Data()));
    ReadString("PartonLevel:MI = on");
    ReadString("MultipleInteractions:pTmin = 1.9");
    ReadString("MultipleInteractions:pT0Ref = 1.8");
    ReadString("MultipleInteractions:ecmRef = 1000.");
    ReadString("MultipleInteractions:expPow = 0.16");
    ReadString("MultipleInteractions:bProfile = 2");
    ReadString("MultipleInteractions:coreFraction = 0.16");
    ReadString("MultipleInteractions:coreRadius = 0.5");
//	SetPARP(85,0.33);          // Regulates gluon prod. mechanism
//	SetPARP(86,0.66);          // Regulates gluon prod. mechanism
    ReadString("SigmaProcess:factorMultFac = 1.");
}

void IlcPythia8::SetPtHardRange(Float_t ptmin, Float_t ptmax)
{
    // Set the pt hard range
    ReadString(Form("PhaseSpace:pTHatMin = %13.3f", ptmin));
    ReadString(Form("PhaseSpace:pTHatMax = %13.3f", ptmax));
}

void IlcPythia8::SetYHardRange(Float_t /*ymin*/, Float_t /*ymax*/)
{
    // Set the y hard range
    printf("YHardRange not implemented in Pythia8 !!!\n");
    
}


void IlcPythia8::SetFragmentation(Int_t flag)
{
    // Switch fragmentation on/off
    if (flag) {
	ReadString("HadronLevel:Hadronize = on");
    } else {
	ReadString("HadronLevel:Hadronize = off");
    }
}

void IlcPythia8::SetInitialAndFinalStateRadiation(Int_t flag1, Int_t flag2)
{
//  initial state radiation    
    if (flag1) {
	ReadString("PartonLevel:ISR = on");
    } else {
	ReadString("PartonLevel:ISR = off");
    }
// final state radiation    
    if (flag2) {
	ReadString("PartonLevel:FSR = on");
    } else {
	ReadString("PartonLevel:FSR = off");
    }
}

void IlcPythia8::SetIntrinsicKt(Float_t kt)
{
// Set the intrinsic kt
	ReadString("BeamRemnants:primordialKT = on");
	ReadString("BeamRemnants:primordialKTsoft = 0.");
	ReadString(Form("BeamRemnants:primordialKThard = %13.3f", kt));
	ReadString("BeamRemnants:halfScaleForKT = 0.");
	ReadString("BeamRemnants:halfMassForKT = 0.");
}

void IlcPythia8::SwitchHFOff()
{
    // Switch off heavy flavor
    // Maximum number of quark flavours used in pdf 
    ReadString("PDFinProcess:nQuarkIn = 3");
    // Maximum number of flavors that can be used in showers
    ReadString("TimeShower:nGluonToQuark = 3");
    ReadString("SpaceShower:nQuarkIn = 3");
    

}

void IlcPythia8::SetPycellParameters(Float_t etaMax, Int_t nEta, Int_t nPhi,
				       Float_t thresh, Float_t etseed, Float_t minet, Float_t r)
{
// Set pycell parameters
    fCellJet  = Pythia8::CellJet( etaMax, nEta, nPhi, 2, 0, 0., 0., thresh);
    fEtSeed   = etseed;
    fMinEtJet = minet;
    fRJet     = r;
}

void IlcPythia8::ModifiedSplitting()
{
//
// We have to see how to implement this in Pythia8 !!!
//
    // Modified splitting probability as a model for quenching
//    SetPARJ(200, 0.8);
//    SetMSTJ(41, 1);  // QCD radiation only
//    SetMSTJ(42, 2);  // angular ordering
//    SetMSTJ(44, 2);  // option to run alpha_s
//    SetMSTJ(47, 0);  // No correction back to hard scattering element
//    SetMSTJ(50, 0);  // No coherence in first branching
//    SetPARJ(82, 1.); // Cut off for parton showers
}

    
void IlcPythia8::InitQuenching(Float_t /*cMin*/, Float_t /*cMax*/, Float_t /*k*/, Int_t /*iECMethod*/, Float_t /*zmax*/, Int_t /*ngmax*/)
{
    //
    //
    IlcWarning("Not implemented !");
}

void IlcPythia8::SwitchHadronisationOff()
{
    // Switch off hadronisation
    ReadString("HadronLevel:Hadronize = off");
}

void IlcPythia8::SwitchHadronisationOn()
{
    // Switch on hadronisarion
    ReadString("HadronLevel:Hadronize = on");
}


void IlcPythia8::GetXandQ(Float_t& x1, Float_t& x2, Float_t& q)
{
    // Get x1, x2 and Q for this event
    
    q  = Pythia8()->info.QFac();
    x1 = Pythia8()->info.x1();
    x2 = Pythia8()->info.x2();
    
}

Float_t IlcPythia8::GetXSection()
{
    // Get the total cross-section
    return Pythia8()->info.sigmaGen();
}

Float_t IlcPythia8::GetPtHard()
{
    // Get the pT hard for this event
    return Pythia8()->info.pTHat();
}




IlcPythia8& IlcPythia8::operator=(const  IlcPythia8& rhs)
{
// Assignment operator
    rhs.Copy(*this);
    return *this;
}

 void IlcPythia8::Copy(TObject&) const
{
    //
    // Copy 
    //
    Fatal("Copy","Not implemented!\n");
}

//
// To be implemented
//
void IlcPythia8::SetNumberOfParticles(Int_t /*i*/)
{
    IlcWarning("Not implemented");
}

void IlcPythia8::EditEventList(Int_t /*i*/)
{
    IlcWarning("Not implemented");
}

void IlcPythia8::Pyquen(Double_t /*a*/, Int_t /*b*/, Double_t /*c*/)
{
    IlcWarning("Cannot be used with Pythia8");
}

void IlcPythia8::HadronizeEvent()
{
    // Needs access to HadronLevel ?
    IlcWarning("Not yet implemented");
}

void IlcPythia8::GetQuenchingParameters(Double_t& /*xp*/, Double_t& /*yp*/, Double_t* /*z[4]*/)
{
    IlcWarning("Not yet implemented");
}

void IlcPythia8::LoadEvent(IlcStack* /*stack*/, Int_t /*flag*/, Int_t /*reHadr*/)
{
    IlcWarning("Not yet implemented");
}

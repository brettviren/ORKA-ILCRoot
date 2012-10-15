// One can use the configuration macro in compiled mode by
// root [0] gSystem->Load("libgeant321");
// root [0] gSystem->SetIncludePath("-I$ROOTSYS/include -I$ILC_ROOT/include\
//                   -I$ILC_ROOT -I$ILC/geant3/TGeant3");
// root [0] .x grun.C(1,"ConfigPPR.C++")

#if !defined(__CINT__) || defined(__MAKECINT__)
#include <Riostream.h>
#include <TRandom.h>
#include <TSystem.h>
#include <TVirtualMC.h>
#include <TGeant3TGeo.h>
#include <TPDGCode.h>
#include <TF1.h>
#include <TGeoGlobalMagField.h>
#include "STEER/STEER/IlcRunLoader.h"
#include "STEER/STEER/IlcRun.h"
#include "STEER/STEER/IlcConfig.h"
#include "STEER/STEER/IlcGenerator.h"
#include "STEER/STEER/IlcSimulation.h"
#include "STEER/STEERBase/IlcLog.h"
#include "STEER/STEERBase/IlcMagF.h"
#include "PYTHIA6/IlcDecayerPythia.h"
#include "EVGEN/IlcGenHIJINGpara.h"
#include "THijing/IlcGenHijing.h"
#include "EVGEN/IlcGenCocktail.h"
#include "EVGEN/IlcGenSlowNucleons.h"
#include "EVGEN/IlcSlowNucleonModelExp.h"
#include "EVGEN/IlcGenParam.h"
#include "EVGEN/IlcGenMUONlib.h"
#include "EVGEN/IlcGenSTRANGElib.h"
#include "EVGEN/IlcGenMUONCocktail.h"
#include "EVGEN/IlcGenCocktail.h"
#include "EVGEN/IlcGenGeVSim.h"
#include "EVGEN/IlcGeVSimParticle.h"
#include "PYTHIA6/IlcGenPythia.h"
#include "STRUCT/IlcBODY.h"
#include "STRUCT/IlcMAG.h"
#include "STRUCT/IlcABSOv3.h"
#include "STRUCT/IlcDIPOv3.h"
#include "STRUCT/IlcHALLv3.h"
#include "STRUCT/IlcFRAMEv2.h"
#include "STRUCT/IlcSHILv3.h"
#include "STRUCT/IlcPIPEv3.h"
#include "ITS/IlcITSv11.h"
#include "TPC/IlcTPCv2.h"
#include "TOF/IlcTOFv6T0.h"
#include "HMPID/IlcHMPIDv3.h"
#include "ZDC/IlcZDCv4.h"
#include "TRD/IlcTRDv1.h"
#include "FMD/IlcFMDv1.h"
#include "MUON/IlcMUONv1.h"
#include "PHOS/IlcPHOSv1.h"
#include "PMD/IlcPMDv1.h"
#include "T0/IlcT0v1.h"
#include "EMCAL/IlcEMCALv2.h"
#include "ACORDE/IlcACORDEv1.h"
#include "VZERO/IlcVZEROv7.h"
#endif

enum PprRun_t 
{
    test50,
    kParam_8000,   kParam_4000,  kParam_2000, 
    kHijing_cent1, kHijing_cent2, 
    kHijing_per1,  kHijing_per2, kHijing_per3, kHijing_per4,  kHijing_per5,
    kHijing_jj25,  kHijing_jj50, kHijing_jj75, kHijing_jj100, kHijing_jj200, 
    kHijing_gj25,  kHijing_gj50, kHijing_gj75, kHijing_gj100, kHijing_gj200,
    kHijing_pA, kPythia6, 
    kPythia6Jets20_24,   kPythia6Jets24_29,   kPythia6Jets29_35,
    kPythia6Jets35_42,   kPythia6Jets42_50,   kPythia6Jets50_60,
    kPythia6Jets60_72,   kPythia6Jets72_86,   kPythia6Jets86_104,
    kPythia6Jets104_125, kPythia6Jets125_150, kPythia6Jets150_180,
    kD0PbPb5500, kCharmSemiElPbPb5500, kBeautySemiElPbPb5500,
    kCocktailTRD, kPyJJ, kPyGJ, 
    kMuonCocktailCent1, kMuonCocktailPer1, kMuonCocktailPer4, 
    kMuonCocktailCent1HighPt, kMuonCocktailPer1HighPt, kMuonCocktailPer4HighPt,
    kMuonCocktailCent1Single, kMuonCocktailPer1Single, kMuonCocktailPer4Single,
    kFlow_2_2000, kFlow_10_2000, kFlow_6_2000, kFlow_6_5000,
    kHIJINGplus, kRunMax
};

const char* pprRunName[] = {
    "test50",
    "kParam_8000",   "kParam_4000",  "kParam_2000", 
    "kHijing_cent1", "kHijing_cent2", 
    "kHijing_per1",  "kHijing_per2", "kHijing_per3", "kHijing_per4",  
    "kHijing_per5",
    "kHijing_jj25",  "kHijing_jj50", "kHijing_jj75", "kHijing_jj100", 
    "kHijing_jj200", 
    "kHijing_gj25",  "kHijing_gj50", "kHijing_gj75", "kHijing_gj100", 
    "kHijing_gj200", "kHijing_pA", "kPythia6", 
    "kPythia6Jets20_24",   "kPythia6Jets24_29",   "kPythia6Jets29_35",
    "kPythia6Jets35_42",   "kPythia6Jets42_50",   "kPythia6Jets50_60",
    "kPythia6Jets60_72",   "kPythia6Jets72_86",   "kPythia6Jets86_104",
    "kPythia6Jets104_125", "kPythia6Jets125_150", "kPythia6Jets150_180",
    "kD0PbPb5500", "kCharmSemiElPbPb5500", "kBeautySemiElPbPb5500",
    "kCocktailTRD", "kPyJJ", "kPyGJ", 
    "kMuonCocktailCent1", "kMuonCocktailPer1", "kMuonCocktailPer4",  
    "kMuonCocktailCent1HighPt", "kMuonCocktailPer1HighPt", "kMuonCocktailPer4HighPt",
    "kMuonCocktailCent1Single", "kMuonCocktailPer1Single", "kMuonCocktailPer4Single",
    "kFlow_2_2000", "kFlow_10_2000", "kFlow_6_2000", "kFlow_6_5000", "kHIJINGplus"
};

enum PprRad_t
{
    kGluonRadiation, kNoGluonRadiation
};

enum PprTrigConf_t
{
    kDefaultPPTrig, kDefaultPbPbTrig
};

const char * pprTrigConfName[] = {
    "p-p","Pb-Pb"
};

// This part for configuration    

static PprRun_t srun = kPythia6;
static PprRad_t srad = kGluonRadiation;
static IlcMagF::BMap_t smag = IlcMagF::k5kG;
static IlcMagF::BeamType_t beamType = IlcMagF::kBeamTypeAA;
static Double_t            beamEnergy = 7000.*82./208;
static Int_t    sseed = 12345; //Set 0 to use the current time
static PprTrigConf_t strig = kDefaultPbPbTrig; // default pp trigger configuration

// Comment line 
static TString  comment;

// Functions
Float_t EtaToTheta(Float_t arg);
IlcGenerator* GeneratorFactory(PprRun_t srun);
IlcGenerator* HijingStandard();
IlcGenGeVSim* GeVSimStandard(Float_t, Float_t);
void ProcessEnvironmentVars();

void Config()
{
    // ThetaRange is (0., 180.). It was (0.28,179.72) 7/12/00 09:00
    // Theta range given through pseudorapidity limits 22/6/2001

    // Get settings from environment variables
    ProcessEnvironmentVars();

    // Set Random Number seed
    gRandom->SetSeed(sseed);
    cout<<"Seed for random number generation= "<<gRandom->GetSeed()<<endl; 


   // libraries required by geant321
#if defined(__CINT__)
    gSystem->Load("liblhapdf");
    gSystem->Load("libEGPythia6");
    gSystem->Load("libpythia6");
    gSystem->Load("libIlcPythia6");
    gSystem->Load("libgeant321");
    
    if (srun >= kHijing_cent1 && srun <= kHijing_pA) 
      {
	gSystem->Load("libhijing");
	gSystem->Load("libTHijing");
      }
#endif

    new     TGeant3TGeo("C++ Interface to Geant3");

  // Output every 100 tracks
  ((TGeant3*)gMC)->SetSWIT(4,100);

    IlcRunLoader* rl=0x0;

    IlcLog::Message(IlcLog::kInfo, "Creating Run Loader", "", "", "Config()"," ConfigPPR.C", __LINE__);

    rl = IlcRunLoader::Open("gilc.root",
			    IlcConfig::GetDefaultEventFolderName(),
			    "recreate");
    if (rl == 0x0)
      {
	gIlc->Fatal("Config.C","Can not instatiate the Run Loader");
	return;
      }
    rl->SetCompressionLevel(2);
    rl->SetNumberOfEventsPerFile(100);
    gIlc->SetRunLoader(rl);

    // Set the trigger configuration
    IlcSimulation::Instance()->SetTriggerConfig(pprTrigConfName[strig]);
    cout<<"Trigger configuration is set to  "<<pprTrigConfName[strig]<<endl;

    //
    // Set External decayer
    IlcDecayer *decayer = new IlcDecayerPythia();


    switch (srun) {
    case kD0PbPb5500:
      decayer->SetForceDecay(kHadronicD);
      break;
    case kCharmSemiElPbPb5500:
      decayer->SetForceDecay(kSemiElectronic);
      break;
    case kBeautySemiElPbPb5500:
      decayer->SetForceDecay(kSemiElectronic);
      break;
    default:
      decayer->SetForceDecay(kAll);
      break;
    }
    decayer->Init();
    gMC->SetExternalDecayer(decayer);
    //
    //
    //=======================================================================
    //
    //=======================================================================
    // ************* STEERING parameters FOR ILC SIMULATION **************
    // --- Specify event type to be tracked through the ILC setup
    // --- All positions are in cm, angles in degrees, and P and E in GeV

    gMC->SetProcess("DCAY",1);
    gMC->SetProcess("PAIR",1);
    gMC->SetProcess("COMP",1);
    gMC->SetProcess("PHOT",1);
    gMC->SetProcess("PFIS",0);
    gMC->SetProcess("DRAY",0);
    gMC->SetProcess("ANNI",1);
    gMC->SetProcess("BREM",1);
    gMC->SetProcess("MUNU",1);
    gMC->SetProcess("CKOV",1);
    gMC->SetProcess("HADR",1);
    gMC->SetProcess("LOSS",2);
    gMC->SetProcess("MULS",1);
    gMC->SetProcess("RAYL",1);

    Float_t cut = 1.e-3;        // 1MeV cut by default
    Float_t tofmax = 1.e10;

    gMC->SetCut("CUTGAM", cut);
    gMC->SetCut("CUTELE", cut);
    gMC->SetCut("CUTNEU", cut);
    gMC->SetCut("CUTHAD", cut);
    gMC->SetCut("CUTMUO", cut);
    gMC->SetCut("BCUTE",  cut); 
    gMC->SetCut("BCUTM",  cut); 
    gMC->SetCut("DCUTE",  cut); 
    gMC->SetCut("DCUTM",  cut); 
    gMC->SetCut("PPCUTM", cut);
    gMC->SetCut("TOFMAX", tofmax); 

    // Generator Configuration
    IlcGenerator* gener = GeneratorFactory(srun);
    gener->SetOrigin(0, 0, 0);    // vertex position
    gener->SetSigma(0, 0, 5.3);   // Sigma in (X,Y,Z) (cm) on IP position
    gener->SetCutVertexZ(1.);     // Truncate at 1 sigma
    gener->SetVertexSmear(kPerEvent); 
    gener->SetTrackingFlag(1);
    gener->Init();
    
    if (smag == IlcMagF::k2kG) {
	comment = comment.Append(" | L3 field 0.2 T");
    } else if (smag == IlcMagF::k5kG) {
	comment = comment.Append(" | L3 field 0.5 T");
    }
    
    
    if (srad == kGluonRadiation)
    {
	comment = comment.Append(" | Gluon Radiation On");
	
    } else {
	comment = comment.Append(" | Gluon Radiation Off");
    }

    printf("\n \n Comment: %s \n \n", comment.Data());
    
    
// Field
    TGeoGlobalMagField::Instance()->SetField
      (new IlcMagF("Maps","Maps", -1., -1., smag,beamType,beamEnergy));
    rl->CdGAFile();
//
    Int_t   iABSO   = 1;
    Int_t   iDIPO   = 1;
    Int_t   iFMD    = 1;
    Int_t   iFRAME  = 1;
    Int_t   iHALL   = 1;
    Int_t   iITS    = 1;
    Int_t   iMAG    = 1;
    Int_t   iMUON   = 1;
    Int_t   iPHOS   = 1;
    Int_t   iPIPE   = 1;
    Int_t   iPMD    = 1;
    Int_t   iHMPID  = 1;
    Int_t   iSHIL   = 1;
    Int_t   iT0     = 1;
    Int_t   iTOF    = 1;
    Int_t   iTPC    = 1;
    Int_t   iTRD    = 1;
    Int_t   iZDC    = 1;
    Int_t   iEMCAL  = 1;
    Int_t   iVZERO  = 1;
    Int_t   iACORDE    = 0;

    //=================== Ilc BODY parameters =============================
    IlcBODY *BODY = new IlcBODY("BODY", "Ilc envelop");


    if (iMAG)
    {
        //=================== MAG parameters ============================
        // --- Start with Magnet since detector layouts may be depending ---
        // --- on the selected Magnet dimensions ---
        IlcMAG *MAG = new IlcMAG("MAG", "Magnet");
    }


    if (iABSO)
    {
        //=================== ABSO parameters ============================
        IlcABSO *ABSO = new IlcABSOv3("ABSO", "Muon Absorber");
    }

    if (iDIPO)
    {
        //=================== DIPO parameters ============================

        IlcDIPO *DIPO = new IlcDIPOv3("DIPO", "Dipole version 3");
    }

    if (iHALL)
    {
        //=================== HALL parameters ============================

        IlcHALL *HALL = new IlcHALLv3("HALL", "Ilc Hall");
    }


    if (iFRAME)
    {
        //=================== FRAME parameters ============================

        IlcFRAMEv2 *FRAME = new IlcFRAMEv2("FRAME", "Space Frame");
	FRAME->SetHoles(1);
    }

    if (iSHIL)
    {
        //=================== SHIL parameters ============================

        IlcSHIL *SHIL = new IlcSHILv3("SHIL", "Shielding Version 3");
    }


    if (iPIPE)
    {
        //=================== PIPE parameters ============================

        IlcPIPE *PIPE = new IlcPIPEv3("PIPE", "Beam Pipe");
    }
 
    if (iITS)
    {
        //=================== ITS parameters ============================

	IlcITS *ITS  = new IlcITSv11("ITS","ITS v11");
    }

    if (iTPC)
    {
      //============================ TPC parameters =====================
        IlcTPC *TPC = new IlcTPCv2("TPC", "Default");
    }


    if (iTOF) {
        //=================== TOF parameters ============================
	IlcTOF *TOF = new IlcTOFv6T0("TOF", "normal TOF");
    }


    if (iHMPID)
    {
        //=================== HMPID parameters ===========================
        IlcHMPID *HMPID = new IlcHMPIDv3("HMPID", "normal HMPID");

    }


    if (iZDC)
    {
        //=================== ZDC parameters ============================

        IlcZDC *ZDC = new IlcZDCv4("ZDC", "normal ZDC");
    }

    if (iTRD)
    {
        //=================== TRD parameters ============================

        IlcTRD *TRD = new IlcTRDv1("TRD", "TRD slow simulator");
    }

    if (iFMD)
    {
        //=================== FMD parameters ============================
	IlcFMD *FMD = new IlcFMDv1("FMD", "normal FMD");
   }

    if (iMUON)
    {
        //=================== MUON parameters ===========================
        // New MUONv1 version (geometry defined via builders)
        IlcMUON *MUON = new IlcMUONv1("MUON", "default");
    }
    //=================== PHOS parameters ===========================

    if (iPHOS)
    {
        IlcPHOS *PHOS = new IlcPHOSv1("PHOS", "IHEP");
    }


    if (iPMD)
    {
        //=================== PMD parameters ============================
        IlcPMD *PMD = new IlcPMDv1("PMD", "normal PMD");
    }

    if (iT0)
    {
        //=================== T0 parameters ============================
        IlcT0 *T0 = new IlcT0v1("T0", "T0 Detector");
    }

    if (iEMCAL)
    {
        //=================== EMCAL parameters ============================
        IlcEMCAL *EMCAL = new IlcEMCALv2("EMCAL", "EMCAL_COMPLETEV1");
    }

     if (iACORDE)
    {
        //=================== ACORDE parameters ============================
        IlcACORDE *ACORDE = new IlcACORDEv1("ACORDE", "normal ACORDE");
    }

     if (iVZERO)
    {
        //=================== VZERO parameters ============================
        IlcVZERO *VZERO = new IlcVZEROv7("VZERO", "normal VZERO");
    }
 
             
}

Float_t EtaToTheta(Float_t arg){
  return (180./TMath::Pi())*2.*atan(exp(-arg));
}



IlcGenerator* GeneratorFactory(PprRun_t srun) {
  Int_t isw = 3;
  if (srad == kNoGluonRadiation) isw = 0;
  
  
  IlcGenerator * gGener = 0x0;
  switch (srun) {
    case test50:
    {
      comment = comment.Append(":HIJINGparam test 50 particles");
      IlcGenHIJINGpara *gener = new IlcGenHIJINGpara(50);
      gener->SetMomentumRange(0, 999999.);
      gener->SetPhiRange(0., 360.);
      // Set pseudorapidity range from -8 to 8.
      Float_t thmin = EtaToTheta(8);   // theta min. <---> eta max
      Float_t thmax = EtaToTheta(-8);  // theta max. <---> eta min 
      gener->SetThetaRange(thmin,thmax);
      gGener=gener;
    }
      break;
    case kParam_8000:
    {
      comment = comment.Append(":HIJINGparam N=8000");
      IlcGenHIJINGpara *gener = new IlcGenHIJINGpara(86030);
      gener->SetMomentumRange(0, 999999.);
      gener->SetPhiRange(0., 360.);
      // Set pseudorapidity range from -8 to 8.
      Float_t thmin = EtaToTheta(8);   // theta min. <---> eta max
      Float_t thmax = EtaToTheta(-8);  // theta max. <---> eta min 
      gener->SetThetaRange(thmin,thmax);
      gGener=gener;
    }
      break;
    case kParam_4000:
    {
      comment = comment.Append("HIJINGparam N=4000");
      IlcGenHIJINGpara *gener = new IlcGenHIJINGpara(43015);
      gener->SetMomentumRange(0, 999999.);
      gener->SetPhiRange(0., 360.);
      // Set pseudorapidity range from -8 to 8.
      Float_t thmin = EtaToTheta(8);   // theta min. <---> eta max
      Float_t thmax = EtaToTheta(-8);  // theta max. <---> eta min 
      gener->SetThetaRange(thmin,thmax);
      gGener=gener;
    }
      break;
    case kParam_2000:
    {
      comment = comment.Append("HIJINGparam N=2000");
      IlcGenHIJINGpara *gener = new IlcGenHIJINGpara(21507);
      gener->SetMomentumRange(0, 999999.);
      gener->SetPhiRange(0., 360.);
      // Set pseudorapidity range from -8 to 8.
      Float_t thmin = EtaToTheta(8);   // theta min. <---> eta max
      Float_t thmax = EtaToTheta(-8);  // theta max. <---> eta min 
      gener->SetThetaRange(thmin,thmax);
      gGener=gener;
    }
      break;
      //
      //  Hijing Central
      //
    case kHijing_cent1:
    {
      comment = comment.Append("HIJING cent1");
      IlcGenHijing *gener = (IlcGenHijing*)HijingStandard();
      // impact parameter range
      gener->SetImpactParameterRange(0., 5.);
      gGener=gener;
    }
      break;
    case kHijing_cent2:
    {
      comment = comment.Append("HIJING cent2");
      IlcGenHijing *gener = (IlcGenHijing*)HijingStandard();
      // impact parameter range
      gener->SetImpactParameterRange(0., 2.);
      gGener=gener;
    }
      break;
      //
      // Hijing Peripheral 
      //
    case kHijing_per1:
    {
      comment = comment.Append("HIJING per1");
      IlcGenHijing *gener = (IlcGenHijing*)HijingStandard();
      // impact parameter range
      gener->SetImpactParameterRange(5., 8.6);
      gGener=gener;
    }
      break;
    case kHijing_per2:
    {
      comment = comment.Append("HIJING per2");
      IlcGenHijing *gener = (IlcGenHijing*)HijingStandard();
      // impact parameter range
      gener->SetImpactParameterRange(8.6, 11.2);
      gGener=gener;
    }
      break;
    case kHijing_per3:
    {
      comment = comment.Append("HIJING per3");
      IlcGenHijing *gener = (IlcGenHijing*)HijingStandard();
      // impact parameter range
      gener->SetImpactParameterRange(11.2, 13.2);
      gGener=gener;
    }
      break;
    case kHijing_per4:
    {
      comment = comment.Append("HIJING per4");
      IlcGenHijing *gener = (IlcGenHijing*)HijingStandard();
      // impact parameter range
      gener->SetImpactParameterRange(13.2, 15.);
      gGener=gener;
    }
      break;
    case kHijing_per5:
    {
      comment = comment.Append("HIJING per5");
      IlcGenHijing *gener = (IlcGenHijing*)HijingStandard();
      // impact parameter range
      gener->SetImpactParameterRange(15., 100.);
      gGener=gener;
    }
      break;
      //
      //  Jet-Jet
      //
    case kHijing_jj25:
    {
      comment = comment.Append("HIJING Jet 25 GeV");
      IlcGenHijing *gener = (IlcGenHijing*)HijingStandard();
      // impact parameter range
      gener->SetImpactParameterRange(0., 5.);
      // trigger
      gener->SetTrigger(1);
      gener->SetPtJet(25.);
      gener->SetRadiation(isw);
      gener->SetSimpleJets(!isw);
      gener->SetJetEtaRange(-0.3,0.3);
      gener->SetJetPhiRange(75., 165.);   
      gGener=gener;
    }
      break;
      
    case kHijing_jj50:
    {
      comment = comment.Append("HIJING Jet 50 GeV");
      IlcGenHijing *gener = (IlcGenHijing*)HijingStandard();
      // impact parameter range
      gener->SetImpactParameterRange(0., 5.);
      // trigger
      gener->SetTrigger(1);
      gener->SetPtJet(50.);
      gener->SetRadiation(isw);
      gener->SetSimpleJets(!isw);
      gener->SetJetEtaRange(-0.3,0.3);
      gener->SetJetPhiRange(75., 165.);   
      gGener=gener;
    }
      break;
      
    case kHijing_jj75:
    {
      comment = comment.Append("HIJING Jet 75 GeV");
      IlcGenHijing *gener = (IlcGenHijing*)HijingStandard();
      // impact parameter range
      gener->SetImpactParameterRange(0., 5.);
      // trigger
      gener->SetTrigger(1);
      gener->SetPtJet(75.);
      gener->SetRadiation(isw);
      gener->SetSimpleJets(!isw);
      gener->SetJetEtaRange(-0.3,0.3);
      gener->SetJetPhiRange(75., 165.);   
      gGener=gener;
    }
      break;
      
    case kHijing_jj100:
    {
      comment = comment.Append("HIJING Jet 100 GeV");
      IlcGenHijing *gener = (IlcGenHijing*)HijingStandard();
      // impact parameter range
      gener->SetImpactParameterRange(0., 5.);
      // trigger
      gener->SetTrigger(1);
      gener->SetPtJet(100.);
      gener->SetRadiation(isw);
      gener->SetSimpleJets(!isw);
      gener->SetJetEtaRange(-0.3,0.3);
      gener->SetJetPhiRange(75., 165.);   
      gGener=gener;
    }
      break;
      
    case kHijing_jj200:
    {
      comment = comment.Append("HIJING Jet 200 GeV");
      IlcGenHijing *gener = (IlcGenHijing*)HijingStandard();
      // impact parameter range
      gener->SetImpactParameterRange(0., 5.);
      // trigger
      gener->SetTrigger(1);
      gener->SetPtJet(200.);
      gener->SetRadiation(isw);
      gener->SetSimpleJets(!isw);
      gener->SetJetEtaRange(-0.3,0.3);
      gener->SetJetPhiRange(75., 165.);   
      gGener=gener;
    }
      break;
      //
      // Gamma-Jet
      //
    case kHijing_gj25:
    {
      comment = comment.Append("HIJING Gamma 25 GeV");
      IlcGenHijing *gener = (IlcGenHijing*)HijingStandard();
      // impact parameter range
      gener->SetImpactParameterRange(0., 5.);
      // trigger
      gener->SetTrigger(2);
      gener->SetPtJet(25.);
      gener->SetRadiation(isw);
      gener->SetSimpleJets(!isw);
      gener->SetJetEtaRange(-0.12, 0.12);
      gener->SetJetPhiRange(220., 320.);
      gGener=gener;
    }
      break;
      
    case kHijing_gj50:
    {
      comment = comment.Append("HIJING Gamma 50 GeV");
      IlcGenHijing *gener = (IlcGenHijing*)HijingStandard();
      // impact parameter range
      gener->SetImpactParameterRange(0., 5.);
      // trigger
      gener->SetTrigger(2);
      gener->SetPtJet(50.);
      gener->SetRadiation(isw);
      gener->SetSimpleJets(!isw);
      gener->SetJetEtaRange(-0.12, 0.12);
      gener->SetJetPhiRange(220., 320.);
      gGener=gener;
    }
      break;
      
    case kHijing_gj75:
    {
      comment = comment.Append("HIJING Gamma 75 GeV");
      IlcGenHijing *gener = (IlcGenHijing*)HijingStandard();
      // impact parameter range
      gener->SetImpactParameterRange(0., 5.);
      // trigger
      gener->SetTrigger(2);
      gener->SetPtJet(75.);
      gener->SetRadiation(isw);
      gener->SetSimpleJets(!isw);
      gener->SetJetEtaRange(-0.12, 0.12);
      gener->SetJetPhiRange(220., 320.);
      gGener=gener;
    }
      break;
      
    case kHijing_gj100:
    {
      comment = comment.Append("HIJING Gamma 100 GeV");
      IlcGenHijing *gener = (IlcGenHijing*)HijingStandard();
      // impact parameter range
      gener->SetImpactParameterRange(0., 5.);
      // trigger
      gener->SetTrigger(2);
      gener->SetPtJet(100.);
      gener->SetRadiation(isw);
      gener->SetSimpleJets(!isw);
      gener->SetJetEtaRange(-0.12, 0.12);
      gener->SetJetPhiRange(220., 320.);
      gGener=gener;
    }
      break;
      
    case kHijing_gj200:
    {
      comment = comment.Append("HIJING Gamma 200 GeV");
      IlcGenHijing *gener = (IlcGenHijing*)HijingStandard();
      // impact parameter range
      gener->SetImpactParameterRange(0., 5.);
      // trigger
      gener->SetTrigger(2);
      gener->SetPtJet(200.);
      gener->SetRadiation(isw);
      gener->SetSimpleJets(!isw);
      gener->SetJetEtaRange(-0.12, 0.12);
      gener->SetJetPhiRange(220., 320.);
      gGener=gener;
    }
      break;
    case kHijing_pA:
    {
      comment = comment.Append("HIJING pA");
      
      IlcGenCocktail *gener  = new IlcGenCocktail();
      
      IlcGenHijing   *hijing = new IlcGenHijing(-1);
      // centre of mass energy 
      hijing->SetEnergyCMS(TMath::Sqrt(82./208.) * 14000.);
      // impact parameter range
      hijing->SetImpactParameterRange(0., 15.);
      // reference frame
      hijing->SetReferenceFrame("CMS");
      hijing->SetBoostLHC(1);
      // projectile
      hijing->SetProjectile("P", 1, 1);
      hijing->SetTarget    ("A", 208, 82);
      // tell hijing to keep the full parent child chain
      hijing->KeepFullEvent();
      // enable jet quenching
      hijing->SetJetQuenching(0);
      // enable shadowing
      hijing->SetShadowing(1);
      // Don't track spectators
      hijing->SetSpectators(0);
      // kinematic selection
      hijing->SetSelectAll(0);
      //
      IlcGenSlowNucleons*  gray    = new IlcGenSlowNucleons(1);
      IlcSlowNucleonModel* model   = new IlcSlowNucleonModelExp();
      gray->SetSlowNucleonModel(model);
      gray->SetDebug(1);
      gener->AddGenerator(hijing,"Hijing pPb", 1);
      gener->AddGenerator(gray,  "Gray Particles",1);
      gGener=gener;
    }
      break;
    case kPythia6:
    {
      comment = comment.Append(":Pythia p-p @ 14 TeV");
      IlcGenPythia *gener = new IlcGenPythia(-1); 
      gener->SetMomentumRange(0,999999);
      gener->SetThetaRange(0., 180.);
      gener->SetYRange(-12,12);
      gener->SetPtRange(0,1000);
      gener->SetProcess(kPyMb);
      gener->SetEnergyCMS(14000.);
      gener->SetProjectile("p", 1, 1) ; 
      gener->SetTarget("p", 1, 1) ; 
      gGener=gener;
    }
      break;
    case kPythia6Jets20_24:
    {
      comment = comment.Append(":Pythia jets 20-24 GeV @ 5.5 TeV");
      IlcGenPythia * gener = new IlcGenPythia(-1);
      gener->SetEnergyCMS(5500.);//        Centre of mass energy
      gener->SetProcess(kPyJets);//        Process type
      gener->SetJetEtaRange(-0.5, 0.5);//  Final state kinematic cuts
      gener->SetJetPhiRange(0., 360.);
      gener->SetJetEtRange(10., 1000.);
      gener->SetGluonRadiation(1,1);
      //    gener->SetPtKick(0.);
      //   Structure function
      gener->SetStrucFunc(kCTEQ4L);
      gener->SetPtHard(20., 24.);// Pt transfer of the hard scattering
      gener->SetPycellParameters(2., 274, 432, 0., 4., 5., 1.0);
      gener->SetForceDecay(kAll);//  Decay type (semielectronic, etc.)
      gener->SetProjectile("p", 1, 1) ; 
      gener->SetTarget("p", 1, 1) ; 
      gGener=gener;
    }
      break;
    case kPythia6Jets24_29:
    {
      comment = comment.Append(":Pythia jets 24-29 GeV @ 5.5 TeV");
      IlcGenPythia * gener = new IlcGenPythia(-1);
      gener->SetEnergyCMS(5500.);//        Centre of mass energy
      gener->SetProcess(kPyJets);//        Process type
      gener->SetJetEtaRange(-0.5, 0.5);//  Final state kinematic cuts
      gener->SetJetPhiRange(0., 360.);
      gener->SetJetEtRange(10., 1000.);
      gener->SetGluonRadiation(1,1);
      //    gener->SetPtKick(0.);
      //   Structure function
      gener->SetStrucFunc(kCTEQ4L);
      gener->SetPtHard(24., 29.);// Pt transfer of the hard scattering
      gener->SetPycellParameters(2., 274, 432, 0., 4., 5., 1.0);
      gener->SetForceDecay(kAll);//  Decay type (semielectronic, etc.)
      gener->SetProjectile("p", 1, 1) ; 
      gener->SetTarget("p", 1, 1) ; 
      gGener=gener;
    }
      break;
    case kPythia6Jets29_35:
    {
      comment = comment.Append(":Pythia jets 29-35 GeV @ 5.5 TeV");
      IlcGenPythia * gener = new IlcGenPythia(-1);
      gener->SetEnergyCMS(5500.);//        Centre of mass energy
      gener->SetProcess(kPyJets);//        Process type
      gener->SetJetEtaRange(-0.5, 0.5);//  Final state kinematic cuts
      gener->SetJetPhiRange(0., 360.);
      gener->SetJetEtRange(10., 1000.);
      gener->SetGluonRadiation(1,1);
      //    gener->SetPtKick(0.);
      //   Structure function
      gener->SetStrucFunc(kCTEQ4L);
      gener->SetPtHard(29., 35.);// Pt transfer of the hard scattering
      gener->SetPycellParameters(2., 274, 432, 0., 4., 5., 1.0);
      gener->SetForceDecay(kAll);//  Decay type (semielectronic, etc.)
      gener->SetProjectile("p", 1, 1) ; 
      gener->SetTarget("p", 1, 1) ; 
      gGener=gener;
    }
      break;
    case kPythia6Jets35_42:
    {
      comment = comment.Append(":Pythia jets 35-42 GeV @ 5.5 TeV");
      IlcGenPythia * gener = new IlcGenPythia(-1);
      gener->SetEnergyCMS(5500.);//        Centre of mass energy
      gener->SetProcess(kPyJets);//        Process type
      gener->SetJetEtaRange(-0.5, 0.5);//  Final state kinematic cuts
      gener->SetJetPhiRange(0., 360.);
      gener->SetJetEtRange(10., 1000.);
      gener->SetGluonRadiation(1,1);
      //    gener->SetPtKick(0.);
      //   Structure function
      gener->SetStrucFunc(kCTEQ4L);
      gener->SetPtHard(35., 42.);// Pt transfer of the hard scattering
      gener->SetPycellParameters(2., 274, 432, 0., 4., 5., 1.0);
      gener->SetForceDecay(kAll);//  Decay type (semielectronic, etc.)
      gener->SetProjectile("p", 1, 1) ; 
      gener->SetTarget("p", 1, 1) ; 
      gGener=gener;
    }
      break;
    case kPythia6Jets42_50:
    {
      comment = comment.Append(":Pythia jets 42-50 GeV @ 5.5 TeV");
      IlcGenPythia * gener = new IlcGenPythia(-1);
      gener->SetEnergyCMS(5500.);//        Centre of mass energy
      gener->SetProcess(kPyJets);//        Process type
      gener->SetJetEtaRange(-0.5, 0.5);//  Final state kinematic cuts
      gener->SetJetPhiRange(0., 360.);
      gener->SetJetEtRange(10., 1000.);
      gener->SetGluonRadiation(1,1);
      //    gener->SetPtKick(0.);
      //   Structure function
      gener->SetStrucFunc(kCTEQ4L);
      gener->SetPtHard(42., 50.);// Pt transfer of the hard scattering
      gener->SetPycellParameters(2., 274, 432, 0., 4., 5., 1.0);
      gener->SetForceDecay(kAll);//  Decay type (semielectronic, etc.)
      gener->SetProjectile("p", 1, 1) ; 
      gener->SetTarget("p", 1, 1) ; 
      gGener=gener;
    }
      break;
    case kPythia6Jets50_60:
    {
      comment = comment.Append(":Pythia jets 50-60 GeV @ 5.5 TeV");
      IlcGenPythia * gener = new IlcGenPythia(-1);
      gener->SetEnergyCMS(5500.);//        Centre of mass energy
      gener->SetProcess(kPyJets);//        Process type
      gener->SetJetEtaRange(-0.5, 0.5);//  Final state kinematic cuts
      gener->SetJetPhiRange(0., 360.);
      gener->SetJetEtRange(10., 1000.);
      gener->SetGluonRadiation(1,1);
      //    gener->SetPtKick(0.);
      //   Structure function
      gener->SetStrucFunc(kCTEQ4L);
      gener->SetPtHard(50., 60.);// Pt transfer of the hard scattering
      gener->SetPycellParameters(2., 274, 432, 0., 4., 5., 1.0);
      gener->SetForceDecay(kAll);//  Decay type (semielectronic, etc.)
      gGener=gener;
    }
      break;
    case kPythia6Jets60_72:
    {
      comment = comment.Append(":Pythia jets 60-72 GeV @ 5.5 TeV");
      IlcGenPythia * gener = new IlcGenPythia(-1);
      gener->SetEnergyCMS(5500.);//        Centre of mass energy
      gener->SetProcess(kPyJets);//        Process type
      gener->SetJetEtaRange(-0.5, 0.5);//  Final state kinematic cuts
      gener->SetJetPhiRange(0., 360.);
      gener->SetJetEtRange(10., 1000.);
      gener->SetGluonRadiation(1,1);
      //    gener->SetPtKick(0.);
      //   Structure function
      gener->SetStrucFunc(kCTEQ4L);
      gener->SetPtHard(60., 72.);// Pt transfer of the hard scattering
      gener->SetPycellParameters(2., 274, 432, 0., 4., 5., 1.0);
      gener->SetForceDecay(kAll);//  Decay type (semielectronic, etc.)
      gener->SetProjectile("p", 1, 1) ; 
      gener->SetTarget("p", 1, 1) ; 
      gGener=gener;
    }
      break;
    case kPythia6Jets72_86:
    {
      comment = comment.Append(":Pythia jets 72-86 GeV @ 5.5 TeV");
      IlcGenPythia * gener = new IlcGenPythia(-1);
      gener->SetEnergyCMS(5500.);//        Centre of mass energy
      gener->SetProcess(kPyJets);//        Process type
      gener->SetJetEtaRange(-0.5, 0.5);//  Final state kinematic cuts
      gener->SetJetPhiRange(0., 360.);
      gener->SetJetEtRange(10., 1000.);
      gener->SetGluonRadiation(1,1);
      //    gener->SetPtKick(0.);
      //   Structure function
      gener->SetStrucFunc(kCTEQ4L);
      gener->SetPtHard(72., 86.);// Pt transfer of the hard scattering
      gener->SetPycellParameters(2., 274, 432, 0., 4., 5., 1.0);
      gener->SetForceDecay(kAll);//  Decay type (semielectronic, etc.)
      gener->SetProjectile("p", 1, 1) ; 
      gener->SetTarget("p", 1, 1) ; 
      gGener=gener;
    }
      break;
    case kPythia6Jets86_104:
    {
      comment = comment.Append(":Pythia jets 86-104 GeV @ 5.5 TeV");
      IlcGenPythia * gener = new IlcGenPythia(-1);
      gener->SetEnergyCMS(5500.);//        Centre of mass energy
      gener->SetProcess(kPyJets);//        Process type
      gener->SetJetEtaRange(-0.5, 0.5);//  Final state kinematic cuts
      gener->SetJetPhiRange(0., 360.);
      gener->SetJetEtRange(10., 1000.);
      gener->SetGluonRadiation(1,1);
      //    gener->SetPtKick(0.);
      //   Structure function
      gener->SetStrucFunc(kCTEQ4L);
      gener->SetPtHard(86., 104.);// Pt transfer of the hard scattering
      gener->SetPycellParameters(2., 274, 432, 0., 4., 5., 1.0);
      gener->SetForceDecay(kAll);//  Decay type (semielectronic, etc.)
      gener->SetProjectile("p", 1, 1) ; 
      gener->SetTarget("p", 1, 1) ; 
      gGener=gener;
    }
      break;
    case kPythia6Jets104_125:
    {
      comment = comment.Append(":Pythia jets 105-125 GeV @ 5.5 TeV");
      IlcGenPythia * gener = new IlcGenPythia(-1);
      gener->SetEnergyCMS(5500.);//        Centre of mass energy
      gener->SetProcess(kPyJets);//        Process type
      gener->SetJetEtaRange(-0.5, 0.5);//  Final state kinematic cuts
      gener->SetJetPhiRange(0., 360.);
      gener->SetJetEtRange(10., 1000.);
      gener->SetGluonRadiation(1,1);
      //    gener->SetPtKick(0.);
      //   Structure function
      gener->SetStrucFunc(kCTEQ4L);
      gener->SetPtHard(104., 125.);// Pt transfer of the hard scattering
      gener->SetPycellParameters(2., 274, 432, 0., 4., 5., 1.0);
      gener->SetForceDecay(kAll);//  Decay type (semielectronic, etc.)
      gener->SetProjectile("p", 1, 1) ; 
      gener->SetTarget("p", 1, 1) ; 
      gGener=gener;
    }
      break;
    case kPythia6Jets125_150:
    {
      comment = comment.Append(":Pythia jets 125-150 GeV @ 5.5 TeV");
      IlcGenPythia * gener = new IlcGenPythia(-1);
      gener->SetEnergyCMS(5500.);//        Centre of mass energy
      gener->SetProcess(kPyJets);//        Process type
      gener->SetJetEtaRange(-0.5, 0.5);//  Final state kinematic cuts
      gener->SetJetPhiRange(0., 360.);
      gener->SetJetEtRange(10., 1000.);
      gener->SetGluonRadiation(1,1);
      //    gener->SetPtKick(0.);
      //   Structure function
      gener->SetStrucFunc(kCTEQ4L);
      gener->SetPtHard(125., 150.);// Pt transfer of the hard scattering
      gener->SetPycellParameters(2., 274, 432, 0., 4., 5., 1.0);
      gener->SetForceDecay(kAll);//  Decay type (semielectronic, etc.)
      gGener=gener;
    }
      break;
    case kPythia6Jets150_180:
    {
      comment = comment.Append(":Pythia jets 150-180 GeV @ 5.5 TeV");
      IlcGenPythia * gener = new IlcGenPythia(-1);
      gener->SetEnergyCMS(5500.);//        Centre of mass energy
      gener->SetProcess(kPyJets);//        Process type
      gener->SetJetEtaRange(-0.5, 0.5);//  Final state kinematic cuts
      gener->SetJetPhiRange(0., 360.);
      gener->SetJetEtRange(10., 1000.);
      gener->SetGluonRadiation(1,1);
      //    gener->SetPtKick(0.);
      //   Structure function
      gener->SetStrucFunc(kCTEQ4L);
      gener->SetPtHard(150., 180.);// Pt transfer of the hard scattering
      gener->SetPycellParameters(2., 274, 432, 0., 4., 5., 1.0);
      gener->SetForceDecay(kAll);//  Decay type (semielectronic, etc.)
      gener->SetProjectile("p", 1, 1) ; 
      gener->SetTarget("p", 1, 1) ; 
      gGener=gener;
    }
      break;
    case kD0PbPb5500:
    {
      comment = comment.Append(" D0 in Pb-Pb at 5.5 TeV");
      IlcGenPythia * gener = new IlcGenPythia(10);
      gener->SetProcess(kPyD0PbPbMNR);
      gener->SetStrucFunc(kCTEQ4L);
      gener->SetPtHard(2.1,-1.0);
      gener->SetEnergyCMS(5500.);
      gener->SetNuclei(208,208);
      gener->SetForceDecay(kHadronicD);
      gener->SetYRange(-2,2);
      gener->SetFeedDownHigherFamily(kFALSE);
      gener->SetStackFillOpt(IlcGenPythia::kParentSelection);
      gener->SetCountMode(IlcGenPythia::kCountParents);
      gener->SetProjectile("A", 208, 82) ; 
      gener->SetTarget("A", 208, 82) ; 
      gGener=gener;
    }
      break;
    case kCharmSemiElPbPb5500:
    {
      comment = comment.Append(" Charm in Pb-Pb at 5.5 TeV");
      IlcGenPythia * gener = new IlcGenPythia(10);
      gener->SetProcess(kPyCharmPbPbMNR);
      gener->SetStrucFunc(kCTEQ4L);
      gener->SetPtHard(2.1,-1.0);
      gener->SetEnergyCMS(5500.);
      gener->SetNuclei(208,208);
      gener->SetForceDecay(kSemiElectronic);
      gener->SetYRange(-2,2);
      gener->SetFeedDownHigherFamily(kFALSE);
      gener->SetCountMode(IlcGenPythia::kCountParents);
      gener->SetProjectile("A", 208, 82) ; 
      gener->SetTarget("A", 208, 82) ; 
      gGener=gener;
    }
      break;
    case kBeautySemiElPbPb5500:
    {
      comment = comment.Append(" Beauty in Pb-Pb at 5.5 TeV");
      IlcGenPythia *gener = new IlcGenPythia(10);
      gener->SetProcess(kPyBeautyPbPbMNR);
      gener->SetStrucFunc(kCTEQ4L);
      gener->SetPtHard(2.75,-1.0);
      gener->SetEnergyCMS(5500.);
      gener->SetNuclei(208,208);
      gener->SetForceDecay(kSemiElectronic);
      gener->SetYRange(-2,2);
      gener->SetFeedDownHigherFamily(kFALSE);
      gener->SetCountMode(IlcGenPythia::kCountParents);
      gener->SetProjectile("A", 208, 82) ; 
      gener->SetTarget("A", 208, 82) ; 
      gGener=gener;
    }
      break;
    case kCocktailTRD:
    {
      comment = comment.Append(" Cocktail for TRD at 5.5 TeV");
      IlcGenCocktail *gener  = new IlcGenCocktail();
      
      IlcGenParam *phi = new IlcGenParam(10,
                                         new IlcGenMUONlib(),
                                         IlcGenMUONlib::kPhi,
                                         "Vogt PbPb");
      
      phi->SetPtRange(0, 100);
      phi->SetYRange(-1., +1.);
      phi->SetForceDecay(kDiElectron);
      
      IlcGenParam *omega = new IlcGenParam(10,
                                           new IlcGenMUONlib(),
                                           IlcGenMUONlib::kOmega,
                                           "Vogt PbPb");
      
      omega->SetPtRange(0, 100);
      omega->SetYRange(-1., +1.);
      omega->SetForceDecay(kDiElectron);
      
      IlcGenParam *jpsi = new IlcGenParam(10,
                                          new IlcGenMUONlib(),
                                          IlcGenMUONlib::kJpsiFamily,
                                          "Vogt PbPb");
      
      jpsi->SetPtRange(0, 100);
      jpsi->SetYRange(-1., +1.);
      jpsi->SetForceDecay(kDiElectron);
      
      IlcGenParam *ups = new IlcGenParam(10,
                                         new IlcGenMUONlib(),
                                         IlcGenMUONlib::kUpsilonFamily,
                                         "Vogt PbPb");
      ups->SetPtRange(0, 100);
      ups->SetYRange(-1., +1.);
      ups->SetForceDecay(kDiElectron);
      
      IlcGenParam *charm = new IlcGenParam(10,
                                           new IlcGenMUONlib(), 
                                           IlcGenMUONlib::kCharm,
                                           "central");
      charm->SetPtRange(0, 100);
      charm->SetYRange(-1.5, +1.5);
      charm->SetForceDecay(kSemiElectronic);
      
      
      IlcGenParam *beauty = new IlcGenParam(10,
                                            new IlcGenMUONlib(), 
                                            IlcGenMUONlib::kBeauty,
                                            "central");
      beauty->SetPtRange(0, 100);
      beauty->SetYRange(-1.5, +1.5);
      beauty->SetForceDecay(kSemiElectronic);
      
      IlcGenParam *beautyJ = new IlcGenParam(10,
                                             new IlcGenMUONlib(), 
                                             IlcGenMUONlib::kBeauty,
                                             "central");
      beautyJ->SetPtRange(0, 100);
      beautyJ->SetYRange(-1.5, +1.5);
      beautyJ->SetForceDecay(kBJpsiDiElectron);
      
      gener->AddGenerator(phi,"Phi",1);
      gener->AddGenerator(omega,"Omega",1);
      gener->AddGenerator(jpsi,"J/psi",1);
      gener->AddGenerator(ups,"Upsilon",1);
      gener->AddGenerator(charm,"Charm",1);
      gener->AddGenerator(beauty,"Beauty",1);
      gener->AddGenerator(beautyJ,"J/Psi from Beauty",1);
      gener->SetProjectile("A", 208, 82) ; 
      gener->SetTarget("A", 208, 82) ; 
      gGener=gener;
    }
      break;
    case kPyJJ:
    {
      comment = comment.Append(" Jet-jet at 5.5 TeV");
      IlcGenPythia *gener = new IlcGenPythia(-1);
      gener->SetEnergyCMS(5500.);
      gener->SetProcess(kPyJets);
      Double_t ptHardMin=10.0, ptHardMax=-1.0;
      gener->SetPtHard(ptHardMin,ptHardMax);
      gener->SetYHard(-0.7,0.7);
      gener->SetJetEtaRange(-0.2,0.2);
      gener->SetEventListRange(0,1);
      gener->SetProjectile("p", 1, 1) ; 
      gener->SetTarget("p", 1, 1) ; 
      gGener=gener;
    }
      break;
    case kPyGJ:
    {
      comment = comment.Append(" Gamma-jet at 5.5 TeV");
      IlcGenPythia *gener = new IlcGenPythia(-1);
      gener->SetEnergyCMS(5500.);
      gener->SetProcess(kPyDirectGamma);
      Double_t ptHardMin=10.0, ptHardMax=-1.0;
      gener->SetPtHard(ptHardMin,ptHardMax);
      gener->SetYHard(-1.0,1.0);
      gener->SetGammaEtaRange(-0.13,0.13);
      gener->SetGammaPhiRange(210.,330.);
      gener->SetEventListRange(0,1);
      gener->SetProjectile("p", 1, 1) ; 
      gener->SetTarget("p", 1, 1) ; 
      gGener=gener;
    }
      break;
    case kMuonCocktailCent1:
    {
      comment = comment.Append(" Muon Cocktail Cent1");
      IlcGenMUONCocktail * gener = new IlcGenMUONCocktail();
      gener->SetPtRange(0.4,100.);       // Transverse momentum range   
      gener->SetPhiRange(0.,360.);    // Azimuthal angle range  
      gener->SetYRange(-4.0,-2.4);
      gener->SetMuonPtCut(0.8);
      gener->SetMuonThetaCut(171.,178.);
      gener->SetMuonMultiplicity(2);
      gener->SetImpactParameterRange(0.,5.);  //Centrality class Cent1 for PDC04
      gGener=gener;
      gener->SetProjectile("A", 208, 82) ; 
      gener->SetTarget("A", 208, 82) ; 
    }
      break;
    case kMuonCocktailPer1:
    {
      comment = comment.Append(" Muon Cocktail Per1");
      IlcGenMUONCocktail * gener = new IlcGenMUONCocktail();
      gener->SetPtRange(0.0,100.);       // Transverse momentum range   
      gener->SetPhiRange(0.,360.);    // Azimuthal angle range  
      gener->SetYRange(-4.0,-2.4);
      gener->SetMuonPtCut(0.8);
      gener->SetMuonThetaCut(171.,178.);
      gener->SetMuonMultiplicity(2);
      gener->SetImpactParameterRange(5.,8.6);//Centrality class Per1 for PDC04
      gener->SetProjectile("A", 208, 82) ; 
      gener->SetTarget("A", 208, 82) ; 
      gGener=gener;
    }
      break;
    case kMuonCocktailPer4:
    {
      comment = comment.Append(" Muon Cocktail Per4");
      IlcGenMUONCocktail * gener = new IlcGenMUONCocktail();
      gener->SetPtRange(0.0,100.);       // Transverse momentum range   
      gener->SetPhiRange(0.,360.);    // Azimuthal angle range  
      gener->SetYRange(-4.0,-2.4);
      gener->SetMuonPtCut(0.8);
      gener->SetMuonThetaCut(171.,178.);
      gener->SetMuonMultiplicity(2);
      gener->SetImpactParameterRange(13.2,15.0);//Centrality class Per4 for PDC04
      gener->SetProjectile("A", 208, 82) ; 
      gener->SetTarget("A", 208, 82) ; 
      gGener=gener;
    }
      break;
    case kMuonCocktailCent1HighPt:
    {
      comment = comment.Append(" Muon Cocktail HighPt Cent1");
      IlcGenMUONCocktail * gener = new IlcGenMUONCocktail();
      gener->SetPtRange(0.0,100.);       // Transverse momentum range   
      gener->SetPhiRange(0.,360.);    // Azimuthal angle range  
      gener->SetYRange(-4.0,-2.4);
      gener->SetMuonPtCut(2.5);
      gener->SetMuonThetaCut(171.,178.);
      gener->SetMuonMultiplicity(2);
      gener->SetImpactParameterRange(0.,5.);  //Centrality class Cent1 for PDC04
      gener->SetProjectile("A", 208, 82) ; 
      gener->SetTarget("A", 208, 82) ; 
      gGener=gener;
    }
      break;
    case kMuonCocktailPer1HighPt :
    {
      comment = comment.Append(" Muon Cocktail HighPt Per1");
      IlcGenMUONCocktail * gener = new IlcGenMUONCocktail();
      gener->SetPtRange(0.0,100.);       // Transverse momentum range   
      gener->SetPhiRange(0.,360.);    // Azimuthal angle range  
      gener->SetYRange(-4.0,-2.4);
      gener->SetMuonPtCut(2.5);
      gener->SetMuonThetaCut(171.,178.);
      gener->SetMuonMultiplicity(2);
      gener->SetImpactParameterRange(5.,8.6);//Centrality class Per1 for PDC04
      gener->SetProjectile("A", 208, 82) ; 
      gener->SetTarget("A", 208, 82) ; 
      gGener=gener;
    }
      break;
    case kMuonCocktailPer4HighPt:
    {
      comment = comment.Append(" Muon Cocktail HighPt Per4");
      IlcGenMUONCocktail * gener = new IlcGenMUONCocktail();
      gener->SetPtRange(0.0,100.);       // Transverse momentum range   
      gener->SetPhiRange(0.,360.);    // Azimuthal angle range  
      gener->SetYRange(-4.0,-2.4);
      gener->SetMuonPtCut(2.5);
      gener->SetMuonThetaCut(171.,178.);
      gener->SetMuonMultiplicity(2);
      gener->SetImpactParameterRange(13.2,15.0);//Centrality class Per4 for PDC04
      gener->SetProjectile("A", 208, 82) ; 
      gener->SetTarget("A", 208, 82) ; 
      gGener=gener;
    }
      break;
    case kMuonCocktailCent1Single:
    {
      comment = comment.Append(" Muon Cocktail Single Cent1");
      IlcGenMUONCocktail * gener = new IlcGenMUONCocktail();
      gener->SetPtRange(0.0,100.);       // Transverse momentum range   
      gener->SetPhiRange(0.,360.);    // Azimuthal angle range  
      gener->SetYRange(-4.0,-2.4);
      gener->SetMuonPtCut(0.8);
      gener->SetMuonThetaCut(171.,178.);
      gener->SetMuonMultiplicity(1);
      gener->SetImpactParameterRange(0.,5.);  //Centrality class Cent1 for PDC04
      gener->SetProjectile("A", 208, 82) ; 
      gener->SetTarget("A", 208, 82) ; 
      gGener=gener;
    }
      break;
    case kMuonCocktailPer1Single :
    {
      comment = comment.Append(" Muon Cocktail Single Per1");
      IlcGenMUONCocktail * gener = new IlcGenMUONCocktail();
      gener->SetPtRange(0.0,100.);       // Transverse momentum range   
      gener->SetPhiRange(0.,360.);    // Azimuthal angle range  
      gener->SetYRange(-4.0,-2.4);
      gener->SetMuonPtCut(0.8);
      gener->SetMuonThetaCut(171.,178.);
      gener->SetMuonMultiplicity(1);
      gener->SetImpactParameterRange(5.,8.6);//Centrality class Per1 for PDC04
      gener->SetNumberOfParticipants(229.3);//Centrality class Per1 for PDC04
      gener->SetProjectile("A", 208, 82) ; 
      gener->SetTarget("A", 208, 82) ; 
      gGener=gener;
    }
      break;
    case kMuonCocktailPer4Single:
    {
      comment = comment.Append(" Muon Cocktail Single Per4");
      IlcGenMUONCocktail * gener = new IlcGenMUONCocktail();
      gener->SetPtRange(0.0,100.);       // Transverse momentum range   
      gener->SetPhiRange(0.,360.);    // Azimuthal angle range  
      gener->SetYRange(-4.0,-2.4);
      gener->SetMuonPtCut(0.8);
      gener->SetMuonThetaCut(171.,178.);
      gener->SetMuonMultiplicity(1);
      gener->SetImpactParameterRange(13.2,15.0);//Centrality class Per4 for PDC04
      gener->SetProjectile("A", 208, 82) ; 
      gener->SetTarget("A", 208, 82) ; 
      gGener=gener;
    }
      break;
    case kFlow_2_2000:
    {
      comment = comment.Append(" Flow with dN/deta  = 2000, vn = 2%");
      gGener = GeVSimStandard(2000., 2.);
    }
      break;
      
    case kFlow_10_2000:
    {
      comment = comment.Append(" Flow with dN/deta  = 2000, vn = 10%");
      gGener = GeVSimStandard(2000., 10.);
    }
      break;
      
    case kFlow_6_2000:
    {
      comment = comment.Append(" Flow with dN/deta  = 2000, vn = 6%");
      gGener = GeVSimStandard(2000., 6.);
    }
      break;
      
    case kFlow_6_5000:
    {
      comment = comment.Append(" Flow with dN/deta  = 5000, vn = 6%");
      gGener = GeVSimStandard(5000., 6.);
    }
      break;
    case kHIJINGplus:
    {
      //
      // The cocktail
      IlcGenCocktail *gener  = new IlcGenCocktail();
      
      //
      // Charm production by Pythia
      IlcGenPythia * genpyc = new IlcGenPythia(230);
      genpyc->SetProcess(kPyCharmPbPbMNR);
      genpyc->SetStrucFunc(kCTEQ4L);
      genpyc->SetPtHard(2.1,-1.0);
      genpyc->SetEnergyCMS(5500.);
      genpyc->SetNuclei(208,208);
      genpyc->SetYRange(-999,999);
      genpyc->SetForceDecay(kAll);
      genpyc->SetFeedDownHigherFamily(kFALSE);
      genpyc->SetCountMode(IlcGenPythia::kCountParents);
      //
      // Beauty production by Pythia
      IlcGenPythia * genpyb = new IlcGenPythia(9);
      genpyb->SetProcess(kPyBeautyPbPbMNR);
      genpyb->SetStrucFunc(kCTEQ4L);
      genpyb->SetPtHard(2.75,-1.0);
      genpyb->SetEnergyCMS(5500.);
      genpyb->SetNuclei(208,208);
      genpyb->SetYRange(-999,999);
      genpyb->SetForceDecay(kAll);
      genpyb->SetFeedDownHigherFamily(kFALSE);
      genpyb->SetCountMode(IlcGenPythia::kCountParents);
      //
      // Hyperons
      //
      IlcGenSTRANGElib *lib = new IlcGenSTRANGElib();
      Int_t particle;
      // Xi
      particle = kXiMinus;
      IlcGenParam *genXi = new IlcGenParam(16,particle,lib->GetPt(particle),lib->GetY(particle),lib->GetIp(particle));
      genXi->SetPtRange(0., 12.);
      genXi->SetYRange(-1.1, 1.1);
      genXi->SetForceDecay(kNoDecay);	
      
      //
      // Omega
      particle = kOmegaMinus;
      IlcGenParam *genOmega = new IlcGenParam(10,particle,lib->GetPt(particle),lib->GetY(particle),lib->GetIp(particle));     
      genOmega->SetPtRange(0, 12.);
      genOmega->SetYRange(-1.1, 1.1);
      genOmega->SetForceDecay(kNoDecay);
      
      //
      // Central Hijing 
      IlcGenHijing *genHi = (IlcGenHijing*)HijingStandard();
      genHi->SwitchOffHeavyQuarks(kTRUE);
      genHi->SetImpactParameterRange(0.,5.);
      //
      // Add everything to the cocktail and shake ...
      gener->AddGenerator(genHi,    "Hijing cent1", 1);
      gener->AddGenerator(genpyc,   "Extra charm",  1);
      gener->AddGenerator(genpyb,   "Extra beauty", 1);
      gener->AddGenerator(genXi,    "Xi"          , 1);
      gener->AddGenerator(genOmega, "Omega",        1);
      gener->SetProjectile("A", 208, 82) ; 
      gener->SetTarget("A", 208, 82) ; 
      gGener = gener;
    }
      break;
    default: break;
  }
  
  return gGener;
}

IlcGenerator* HijingStandard()
{
  
    IlcGenHijing *gener = new IlcGenHijing(-1);
// centre of mass energy 
    gener->SetEnergyCMS(5500.);
// reference frame
    gener->SetReferenceFrame("CMS");
// projectile
     gener->SetProjectile("A", 208, 82);
     gener->SetTarget    ("A", 208, 82);
// tell hijing to keep the full parent child chain
     gener->KeepFullEvent();
// enable jet quenching
     gener->SetJetQuenching(1);
// enable shadowing
     gener->SetShadowing(1);
// neutral pion and heavy particle decays switched off
     gener->SetDecaysOff(1);
// Don't track spectators
     gener->SetSpectators(0);
// kinematic selection
     gener->SetSelectAll(0);
     return gener;
}

IlcGenGeVSim* GeVSimStandard(Float_t mult, Float_t vn)
{
    IlcGenGeVSim* gener = new IlcGenGeVSim(0);
//
// Mult is the number of charged particles in |eta| < 0.5
// Vn is in (%)
//
// Sigma of the Gaussian dN/deta
    Float_t sigma_eta  = 2.75;
//
// Maximum eta
    Float_t etamax     = 7.00;
//
//
// Scale from multiplicity in |eta| < 0.5 to |eta| < |etamax|	
    Float_t mm = mult * (TMath::Erf(etamax/sigma_eta/sqrt(2.)) / TMath::Erf(0.5/sigma_eta/sqrt(2.))); 
//
// Scale from charged to total multiplicity
// 
    mm *= 1.587;
//
// Vn 
    vn /= 100.;    	 
//
// Define particles
//
//
// 78% Pions (26% pi+, 26% pi-, 26% p0)              T = 250 MeV
    IlcGeVSimParticle *pp =  new IlcGeVSimParticle(kPiPlus,  1, 0.26 * mm, 0.25, sigma_eta) ;
    IlcGeVSimParticle *pm =  new IlcGeVSimParticle(kPiMinus, 1, 0.26 * mm, 0.25, sigma_eta) ;
    IlcGeVSimParticle *p0 =  new IlcGeVSimParticle(kPi0,     1, 0.26 * mm, 0.25, sigma_eta) ;
//
// 12% Kaons (3% K0short, 3% K0long, 3% K+, 3% K-)   T = 300 MeV
    IlcGeVSimParticle *ks =  new IlcGeVSimParticle(kK0Short, 1, 0.03 * mm, 0.30, sigma_eta) ;
    IlcGeVSimParticle *kl =  new IlcGeVSimParticle(kK0Long,  1, 0.03 * mm, 0.30, sigma_eta) ;
    IlcGeVSimParticle *kp =  new IlcGeVSimParticle(kKPlus,   1, 0.03 * mm, 0.30, sigma_eta) ;
    IlcGeVSimParticle *km =  new IlcGeVSimParticle(kKMinus,  1, 0.03 * mm, 0.30, sigma_eta) ;
//
// 10% Protons / Neutrons (5% Protons, 5% Neutrons)  T = 250 MeV
    IlcGeVSimParticle *pr =  new IlcGeVSimParticle(kProton,  1, 0.05 * mm, 0.25, sigma_eta) ;
    IlcGeVSimParticle *ne =  new IlcGeVSimParticle(kNeutron, 1, 0.05 * mm, 0.25, sigma_eta) ;
//
// Set Elliptic Flow properties 	

    Float_t pTsaturation = 2. ;

    pp->SetEllipticParam(vn,pTsaturation,0.) ;
    pm->SetEllipticParam(vn,pTsaturation,0.) ;
    p0->SetEllipticParam(vn,pTsaturation,0.) ;
    pr->SetEllipticParam(vn,pTsaturation,0.) ;
    ne->SetEllipticParam(vn,pTsaturation,0.) ;
    ks->SetEllipticParam(vn,pTsaturation,0.) ;
    kl->SetEllipticParam(vn,pTsaturation,0.) ;
    kp->SetEllipticParam(vn,pTsaturation,0.) ;
    km->SetEllipticParam(vn,pTsaturation,0.) ;
//
// Set Direct Flow properties	
    pp->SetDirectedParam(vn,1.0,0.) ;
    pm->SetDirectedParam(vn,1.0,0.) ;
    p0->SetDirectedParam(vn,1.0,0.) ;
    pr->SetDirectedParam(vn,1.0,0.) ;
    ne->SetDirectedParam(vn,1.0,0.) ;
    ks->SetDirectedParam(vn,1.0,0.) ;
    kl->SetDirectedParam(vn,1.0,0.) ;
    kp->SetDirectedParam(vn,1.0,0.) ;
    km->SetDirectedParam(vn,1.0,0.) ;
//
// Add particles to the list
    gener->AddParticleType(pp) ;
    gener->AddParticleType(pm) ;
    gener->AddParticleType(p0) ;
    gener->AddParticleType(pr) ;
    gener->AddParticleType(ne) ;
    gener->AddParticleType(ks) ;
    gener->AddParticleType(kl) ;
    gener->AddParticleType(kp) ;
    gener->AddParticleType(km) ;
//	
// Random Ev.Plane ----------------------------------
    TF1 *rpa = new TF1("gevsimPsiRndm","1", 0, 360);
// --------------------------------------------------
    gener->SetPtRange(0., 9.) ; // Use a resonable range! (used for bin size in numerical integration)
    gener->SetPhiRange(0, 360);
    //
    // Set pseudorapidity range 
    Float_t thmin = EtaToTheta(+etamax);   
    Float_t thmax = EtaToTheta(-etamax);   
    gener->SetThetaRange(thmin,thmax);     
    return gener;
}



void ProcessEnvironmentVars()
{
    // Run type
    if (gSystem->Getenv("CONFIG_RUN_TYPE")) {
      for (Int_t iRun = 0; iRun < kRunMax; iRun++) {
	if (strcmp(gSystem->Getenv("CONFIG_RUN_TYPE"), pprRunName[iRun])==0) {
	  srun = (PprRun_t)iRun;
	  cout<<"Run type set to "<<pprRunName[iRun]<<endl;
	}
      }
    }

    // Random Number seed
    if (gSystem->Getenv("CONFIG_SEED")) {
      sseed = atoi(gSystem->Getenv("CONFIG_SEED"));
    }
}

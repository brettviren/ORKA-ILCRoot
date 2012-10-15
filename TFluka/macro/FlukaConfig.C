// One can use the configuration macro in compiled mode by
// root [0] gSystem->Load("libgeant321");
// root [0] gSystem->SetIncludePath("-I$ROOTSYS/include -I$ILC_ROOT/include\
//                   -I$ILC_ROOT -I$ILC/geant3/TGeant3");
// root [0] .x grun.C(1,"Config.C++")

#if !defined(__CINT__) || defined(__MAKECINT__)
#include <Riostream.h>
#include <TPDGCode.h>
#include <TRandom.h>
#include <TSystem.h>
#include <TVirtualMC.h>
#include <TGeant3TGeo.h>
#include "STEER/IlcRunLoader.h"
#include "STEER/IlcRun.h"
#include "STEER/IlcConfig.h"
#include "PYTHIA6/IlcDecayerPythia.h"
#include "EVGEN/IlcGenCocktail.h"
#include "EVGEN/IlcGenHIJINGpara.h"
#include "STEER/IlcMagF.h"
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
#include "ZDC/IlcZDCv3.h"
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

Float_t EtaToTheta(Float_t arg);
void    LoadPythia();


void Config()
{
    // ThetaRange is (0., 180.). It was (0.28,179.72) 7/12/00 09:00
    // Theta range given through pseudorapidity limits 22/6/2001

    // Set Random Number seed
  //gRandom->SetSeed(123456); // Set 0 to use the current time
  
  IlcLog::Message(IlcLog::kInfo, Form("Seed for random number generation = %d",gRandom->GetSeed()), "Config.C", "Config.C", "Config()","Config.C", __LINE__);

  // Load Pythia libraries
  LoadPythia();
  // Libraries required by geant321
  Bool_t isFluka = kTRUE;
    if (isFluka) {
      gSystem->Load("libGeom");
      cout << "\t* Loading TFluka..." << endl;  
      gSystem->Load("libfluka");    
      
      cout << "\t* Instantiating TFluka..." << endl;
      new  TFluka("C++ Interface to Fluka", 0/*verbositylevel*/);
    }
    else {
      cout << "\t* Loading Geant3..." << endl;  
      gSystem->Load("libgeant321");
      
      cout << "\t* Instantiating Geant3TGeo..." << endl;
      new     TGeant3TGeo("C++ Interface to Geant3");
    }
    IlcRunLoader* rl=0x0;

    IlcLog::Message(IlcLog::kInfo, "Creating Run Loader", "Config.C", "Config.C", "Config()"," Config.C", __LINE__);

    rl = IlcRunLoader::Open("gilc.root",
			    IlcConfig::GetDefaultEventFolderName(),
			    "recreate");
    if (rl == 0x0)
      {
	gIlc->Fatal("Config.C","Can not instatiate the Run Loader");
	return;
      }
    rl->SetCompressionLevel(2);
    rl->SetNumberOfEventsPerFile(3);
    gIlc->SetRunLoader(rl);
    
    // gIlc->SetGeometryFromFile("geometry.root");

    // Uncomment if you want to load geometry from OCDB!   >>>>
/*    
    if(!IlcCDBManager::Instance()->IsDefaultStorageSet()){
	 cout << "#####################################################" << endl;
	 cout << "#                                                   #" << endl;
	 cout << "#     WARNING: CDB DEFAULT STORAGE NOT SET !!!      #" << endl;
	 cout << "#     SETTING IT TO local://$ILC_ROOT/OCDB !!!         #" << endl;
	 cout << "#                                                   #" << endl;
	 cout << "#####################################################" << endl;
          
         IlcCDBManager::Instance()->SetDefaultStorage("local://$ILC_ROOT/OCDB");
    }
    
    if(IlcCDBManager::Instance()->GetRun() < 0){
	 cout << "#####################################################" << endl;
	 cout << "#                                                   #" << endl;
	 cout << "#     WARNING: RUN NUMBER NOT SET !!!               #" << endl;
	 cout << "#     SETTING IT TO 0 !!!                           #" << endl;
	 cout << "#                                                   #" << endl;
	 cout << "#####################################################" << endl;
          
         IlcCDBManager::Instance()->SetRun(0);
    }
    gIlc->SetGeometryFromCDB();
*/
    // Uncomment if you want to load geometry from OCDB!   <<<<

    // Set the trigger configuration
    IlcSimulation::Instance()->SetTriggerConfig("Pb-Pb");
    cout<<"Trigger configuration is set to  Pb-Pb"<<endl;

    //
    // Set External decayer
    TVirtualMCDecayer *decayer = new IlcDecayerPythia();

    decayer->SetForceDecay(kAll);
    decayer->Init();
    gMC->SetExternalDecayer(decayer);
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


    int     nParticles = 100;
    if (gSystem->Getenv("CONFIG_NPARTICLES"))
    {
        nParticles = atoi(gSystem->Getenv("CONFIG_NPARTICLES"));
    }

    IlcGenCocktail *gener = new IlcGenCocktail();
    gener->SetPhiRange(0, 360);
    // Set pseudorapidity range from -8 to 8.
    Float_t thmin = EtaToTheta(8);   // theta min. <---> eta max
    Float_t thmax = EtaToTheta(-8);  // theta max. <---> eta min 
    gener->SetThetaRange(thmin,thmax);
    gener->SetOrigin(0, 0, 0);  //vertex position
    gener->SetSigma(0, 0, 0);   //Sigma in (X,Y,Z) (cm) on IP position

    IlcGenHIJINGpara *hijingparam = new IlcGenHIJINGpara(nParticles);
    hijingparam->SetMomentumRange(0.2, 999);
    gener->AddGenerator(hijingparam,"HIJING PARAM",1);

//    IlcGenBox *genbox = new IlcGenBox(nParticles);
//    genbox->SetPart(kGamma);
//    genbox->SetPtRange(0.3, 10.00);
//    gener->AddGenerator(genbox,"GENBOX GAMMA for PHOS",1);
    gener->Init();


    // 
    // Activate this line if you want the vertex smearing to happen
    // track by track
    //
    //gener->SetVertexSmear(perTrack); 
    // Field (L3 0.4 T)
    TGeoGlobalMagField::Instance()->SetField(new IlcMagF("Maps","Maps", -1., -1., IlcMagF::k5kG));

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
    Int_t   iACORDE = 1;
    Int_t   iVZERO  = 1;
    rl->CdGAFile();
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
        //============================ TPC parameters ===================
        IlcTPC *TPC = new IlcTPCv2("TPC", "Default");
	TPC->SetPrimaryIonisation();
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

        IlcZDC *ZDC = new IlcZDCv3("ZDC", "normal ZDC");
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
        IlcEMCAL *EMCAL = new IlcEMCALv2("EMCAL", "EMCAL_COMPLETE");
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

     IlcLog::Message(IlcLog::kInfo, "End of Config", "Config.C", "Config.C", "Config()"," Config.C", __LINE__);

}

Float_t EtaToTheta(Float_t arg){
  return (180./TMath::Pi())*2.*atan(exp(-arg));
}


void LoadPythia()
{
    // Load Pythia related libraries
    gSystem->Load("liblhapdf.so");      // Parton density functions
    gSystem->Load("libEGPythia6.so");   // TGenerator interface
    gSystem->Load("libpythia6.so");     // Pythia
    gSystem->Load("libIlcPythia6.so");  // ILC specific implementations
}

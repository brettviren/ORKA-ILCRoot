// $Id: commonConfig.C 54339 2012-02-01 16:16:37Z hristov $
//
// IlcRoot Configuration for running ilcroot with Monte Carlo.
// commonConfig() includes the common setting for all MCs
// which has to be called before MC is instantiated.
// Called from MC specific configs (g3Config.C, g4Config.C).
//
// Extracted from G3 specific Config.C 
// by I. Hrivnacova, IPN Orsay

enum PprTrigConf_t {
    kDefaultPPTrig, kDefaultPbPbTrig
};

const char * pprTrigConfName[] = {
    "p-p","Pb-Pb"
};

enum ConfigVersion_t {
    kConfigV0,  // default configuration  
    kConfigV1   // configuration for LHC 2010 production
};

// Options 
static IlcMagF::BMap_t smag = IlcMagF::k5kG;
static PprTrigConf_t strig = kDefaultPPTrig; // default PP trigger configuration
static TString comment;

// Functions
void  LoadPythia();

void commonConfig(ConfigVersion_t configVersion = kConfigV0)
{
  cout << "Running commonConfig.C ... " << endl;

    // Set Random Number seed
  gRandom->SetSeed(123456); // Set 0 to use the currecnt time
  IlcLog::Message(IlcLog::kInfo, Form("Seed for random number generation = %d",gRandom->GetSeed()), "Config.C", "Config.C", "Config()","Config.C", __LINE__);


  //=======================================================================
  // Load Pythia libraries
  //=======================================================================

  LoadPythia();

  //=======================================================================
  // ILC steering object (IlcRunLoader)
  //=======================================================================

  IlcRunLoader* rl 
    = IlcRunLoader::Open("gilc.root",
			  IlcConfig::GetDefaultEventFolderName(),
			  "recreate");
  if ( ! rl ) {
    gIlc->Fatal("Config.C","Can not instatiate the Run Loader");
    return;
  }
  rl->SetCompressionLevel(2);
  rl->SetNumberOfEventsPerFile(3);
  gIlc->SetRunLoader(rl);
  
  //======================================================================
  // Trigger configuration
  //=======================================================================

  IlcSimulation::Instance()->SetTriggerConfig(pprTrigConfName[strig]);
  cout << "Trigger configuration is set to  " << pprTrigConfName[strig] << endl;

  // ============================= 
  // Magnetic field
  // ============================= 

  // Field (L3 0.5 T)
  IlcMagF* field = new IlcMagF("Maps","Maps", -1., -1., IlcMagF::k5kG);
  TGeoGlobalMagField::Instance()->SetField(field);

  printf("\n \n Comment: %s \n \n", comment.Data());

  // ============================= 
  // Modules
  // ============================= 

  rl->CdGAFile();

  Int_t   iABSO  =  1;
  Int_t   iDIPO  =  1;
  Int_t   iFMD   =  1;
  Int_t   iFRAME =  1;
  Int_t   iHALL  =  1;
  Int_t   iITS   =  1;
  Int_t   iMAG   =  1;
  Int_t   iMUON  =  1;
  Int_t   iPHOS  =  1;
  Int_t   iPIPE  =  1;
  Int_t   iPMD   =  1;
  Int_t   iHMPID =  1;
  Int_t   iSHIL  =  1;
  Int_t   iT0    =  1;
  Int_t   iTOF   =  1;
  Int_t   iTPC   =  1;
  Int_t   iTRD   =  1;
  Int_t   iZDC   =  1;
  Int_t   iEMCAL =  1;
  Int_t   iACORDE = 1;
  Int_t   iVZERO =  1;

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
      if ( configVersion == kConfigV1 ) {
        IlcTRDgeometry *geoTRD = TRD->GetGeometry();
        // Partial geometry: modules at 0,1,7,8,9,16,17
        // starting at 3h in positive direction
        geoTRD->SetSMstatus(2,0);
        geoTRD->SetSMstatus(3,0);
        geoTRD->SetSMstatus(4,0);
        geoTRD->SetSMstatus(5,0);
        geoTRD->SetSMstatus(6,0);
        geoTRD->SetSMstatus(11,0);
        geoTRD->SetSMstatus(12,0);
        geoTRD->SetSMstatus(13,0);
        geoTRD->SetSMstatus(14,0);
        geoTRD->SetSMstatus(15,0);
        geoTRD->SetSMstatus(16,0);
      }
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
     if ( configVersion == kConfigV0 ) 
       IlcPHOS *PHOS = new IlcPHOSv1("PHOS", "IHEP");
     else if ( configVersion == kConfigV1 )  
       IlcPHOS *PHOS = new IlcPHOSv1("PHOS", "noCPV_Modules123"); 
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
    if ( configVersion == kConfigV0 ) 
      IlcEMCAL *EMCAL = new IlcEMCALv2("EMCAL", "EMCAL_COMPLETEV1");
    else if ( configVersion == kConfigV1 )  
      IlcEMCAL *EMCAL = new IlcEMCALv2("EMCAL", "EMCAL_FIRSTYEARV1");
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

  cout << "Running commonConfig.C finished ... " << endl;
}

void LoadPythia()
{
  // Load Pythia related libraries
  gSystem->Load("liblhapdf.so");      // Parton density functions
  gSystem->Load("libEGPythia6.so");   // TGenerator interface
  gSystem->Load("libpythia6.so");     // Pythia
  gSystem->Load("libIlcPythia6.so");  // ILC specific implementations
}

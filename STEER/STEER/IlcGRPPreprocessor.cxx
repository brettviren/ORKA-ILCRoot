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

/* $Id: IlcGRPPreprocessor.cxx 55680 2012-04-10 21:06:26Z hristov $ */

//-------------------------------------------------------------------------
//                          Class IlcGRPPreprocessor
//                  Global Run Parameters (GRP) preprocessor
//    Origin: Panos Christakoglou, UOA-CERN, Panos.Christakoglou@cern.ch
//    Modified: Ernesto.Lopez.Torres@cern.ch  CEADEN-CERN
//    Modified: Chiara.Zampolli@cern.ch  CERN
//-------------------------------------------------------------------------

#include <TChain.h>
#include <TList.h>
#include <TMap.h>
#include <THashList.h>
#include <TObjString.h>
#include <TObjArray.h>
#include <TGraph.h>
#include <TString.h>
#include <TFile.h>
#include <TPRegexp.h>

#include <float.h>

#include "IlcGRPPreprocessor.h"
#include "IlcGRPObject.h"
#include "IlcDCSSensor.h"
#include "IlcSplineFit.h"
#include "IlcDCSSensorArray.h"
#include "IlcRawEventHeaderVersions.h"

#include "IlcTriggerConfiguration.h"
#include "IlcTriggerRunScalers.h"
#include "IlcTriggerInput.h"

#include "IlcCDBMetaData.h"
#include "IlcESDVertex.h"
#include "IlcLHCReader.h"
#include "IlcLHCData.h"
#include "IlcDCSArray.h"
#include "IlcDAQ.h"
#include "IlcLTUConfig.h"
#include "IlcQAThresholds.h"

class IlcLog;
class IlcDCSValue;
class IlcShuttleInterface;

// needed for ReceivePromptRecoParameters

#include <TSQLServer.h>
#include <TSQLResult.h>
#include <TSQLRow.h>
#include <IlcCDBManager.h>
#include <IlcCDBMetaData.h>
#include <IlcCDBId.h>
#include <IlcTriggerConfiguration.h>
#include "IlcCTPTimeParams.h"
#include "IlcLHCClockPhase.h"

ClassImp(IlcGRPPreprocessor)


const Double_t kFitFraction = -1.;                 // Fraction of DCS sensor fits required

//_______________________________________________________________

  const Int_t IlcGRPPreprocessor::fgknDAQLbPar = 6; // num parameters in the logbook used to fill the GRP object
  const Int_t IlcGRPPreprocessor::fgknDCSDP = 48;   // number of dcs dps
  const Int_t IlcGRPPreprocessor::fgknDCSDPHallProbes = 40;   // number of dcs dps
  const Int_t IlcGRPPreprocessor::fgknLHCDP = 9;   // number of dcs dps from LHC data
  const Int_t IlcGRPPreprocessor::fgkDCSDPHallTopShift = 4;   // shift from the top to get tp the Hall Probes names in the list of DCS DPs
  const Int_t IlcGRPPreprocessor::fgkDCSDPNonWorking = 5; // number of non working DCS DPs
  const char* IlcGRPPreprocessor::fgkDCSDataPoints[IlcGRPPreprocessor::fgknDCSDP] = {
                   "L3Polarity",
                   "DipolePolarity",
                   "L3Current",
                   "DipoleCurrent",
		   "L3_BSF17_H1",
		   "L3_BSF17_H2",
		   "L3_BSF17_H3",
		   "L3_BSF17_Temperature",
		   "L3_BSF4_H1",
		   "L3_BSF4_H2",
		   "L3_BSF4_H3",
		   "L3_BSF4_Temperature",
		   "L3_BKF17_H1",
		   "L3_BKF17_H2",
		   "L3_BKF17_H3",
		   "L3_BKF17_Temperature",
		   "L3_BKF4_H1",
		   "L3_BKF4_H2",
		   "L3_BKF4_H3",
		   "L3_BKF4_Temperature",
		   "L3_BSF13_H1",
		   "L3_BSF13_H2",
		   "L3_BSF13_H3",
		   "L3_BSF13_Temperature",
		   "L3_BSF8_H1",
		   "L3_BSF8_H2",
		   "L3_BSF8_H3",
		   "L3_BSF8_Temperature",
		   "L3_BKF13_H1",
		   "L3_BKF13_H2",
		   "L3_BKF13_H3",
		   "L3_BKF13_Temperature",
		   "L3_BKF8_H1",
		   "L3_BKF8_H2",
		   "L3_BKF8_H3",
		   "L3_BKF8_Temperature",
		   "Dipole_Inside_H1",
		   "Dipole_Inside_H2",
		   "Dipole_Inside_H3",
		   "Dipole_Inside_Temperature",
		   "Dipole_Outside_H1",
		   "Dipole_Outside_H2",
		   "Dipole_Outside_H3",
		   "Dipole_Outside_Temperature",
                   "CavernTemperature",
                   "CavernAtmosPressure",
                   "SurfaceAtmosPressure",
                   "CavernAtmosPressure2"
                 };

  const char* IlcGRPPreprocessor::fgkDCSDataPointsHallProbes[IlcGRPPreprocessor::fgknDCSDPHallProbes] = {
		   "L3_BSF17_H1",
		   "L3_BSF17_H2",
		   "L3_BSF17_H3",
		   "L3_BSF17_Temperature",
		   "L3_BSF4_H1",
		   "L3_BSF4_H2",
		   "L3_BSF4_H3",
		   "L3_BSF4_Temperature",
		   "L3_BKF17_H1",
		   "L3_BKF17_H2",
		   "L3_BKF17_H3",
		   "L3_BKF17_Temperature",
		   "L3_BKF4_H1",
		   "L3_BKF4_H2",
		   "L3_BKF4_H3",
		   "L3_BKF4_Temperature",
		   "L3_BSF13_H1",
		   "L3_BSF13_H2",
		   "L3_BSF13_H3",
		   "L3_BSF13_Temperature",
		   "L3_BSF8_H1",
		   "L3_BSF8_H2",
		   "L3_BSF8_H3",
		   "L3_BSF8_Temperature",
		   "L3_BKF13_H1",
		   "L3_BKF13_H2",
		   "L3_BKF13_H3",
		   "L3_BKF13_Temperature",
		   "L3_BKF8_H1",
		   "L3_BKF8_H2",
		   "L3_BKF8_H3",
		   "L3_BKF8_Temperature",
		   "Dipole_Inside_H1",
		   "Dipole_Inside_H2",
		   "Dipole_Inside_H3",
		   "Dipole_Inside_Temperature",
		   "Dipole_Outside_H1",
		   "Dipole_Outside_H2",
		   "Dipole_Outside_H3",
		   "Dipole_Outside_Temperature"
                 };
                 
  const Short_t kSensors = 45; // start index position of sensor in DCS DPs
  const Short_t kNumSensors = 3; // Number of sensors in DCS DPs (CavernAtmosPressure, SurfaceAtmosPressure, CavernAtmosPressure2)


  const char* IlcGRPPreprocessor::fgkLHCDataPoints[IlcGRPPreprocessor::fgknLHCDP] = {
	  "LHC_Beam_Energy",
	  "LHC_MachineMode",
	  "LHC_BeamMode",
          "LHC_Beams_Particle_Type",
	  "BPTX_Phase_Shift_B1",
	  "BPTX_Phase_Shift_B2",
	  "LHC_Particle_Type_B1",
	  "LHC_Particle_Type_B2",
	  "LHC_Data_Quality_Flag"
  };

  const char* kppError[] = {
                   "",
                   "(DAQ logbook ERROR)",
                   "(DAQ FXS ERROR)",
                   "(Trigger Scalers not found in FXS - ERROR)",
                   "(DCS data points ERROR)",
                   "(Trigger Configuration ERROR)",
                   "(DAQ logbook ERROR determining partition of the run)",
                   "(CTP timing ERROR)",
		   "(SPD Mean Vertex ERROR)",
		   "(FXS Error for LHC Data)",
		   "(LHC Data Error)",
		   "(LHC Clock Phase Error (from LHC Data))",
		   "(LTU Configuration Error)",
		   "(DQM Failure)",
                   "(Trigger Aliases wrong or not found in DCS FXS - ERROR)"
  };

//_______________________________________________________________

IlcGRPPreprocessor::IlcGRPPreprocessor(IlcShuttleInterface* shuttle):
	IlcPreprocessor("GRP",shuttle),  fPressure(0), fmaxFloat(0), fminFloat(0),fmaxDouble(0), fminDouble(0), fmaxInt(0), fminInt(0), fmaxUInt(0), fminUInt(0),fdaqStartEndTimeOk(kTRUE),ffailedDPs(new TObjArray(fgknDCSDP))
{
	// constructor - shuttle must be instantiated!

	AddRunType("COSMIC");
	AddRunType("LASER");
	AddRunType("PHYSICS");
	AddRunType("CALIBRATION_BC");
	AddRunType("CALIBRATION_CENTRAL");
	AddRunType("CALIBRATION_EMD");
	AddRunType("CALIBRATION_MB");
	AddRunType("CALIBRATION_SEMICENTRAL");
	AddRunType("CALIBRATION");
	AddRunType("PEDESTAL");
	AddRunType("STANDALONE");
	AddRunType("GAIN");
	AddRunType("NOISE");
	AddRunType("PULSER");
        AddRunType("STANDALONE_PULSER");
        AddRunType("STANDALONE_BC");

	fmaxFloat = FLT_MAX;
	fminFloat = -FLT_MAX;
	fmaxDouble = DBL_MAX;
	fminDouble = -DBL_MAX;
	fmaxInt = kMaxInt;
	fminInt = kMinInt;
	fmaxUInt = kMaxUInt;
	fminUInt = 0;

	IlcInfo(Form("Max allowed float = %6.5e",fmaxFloat));
	IlcInfo(Form("Min allowed float = %6.5e",fminFloat));
	IlcInfo(Form("Max allowed double = %6.5e",fmaxDouble));
	IlcInfo(Form("Min allowed double = %6.5e",fminDouble));
	IlcInfo(Form("Max allowed integer = %d",fmaxInt));
	IlcInfo(Form("Min allowed integer = %d",fminInt));
	IlcInfo(Form("Max allowed unsigned integer = %u",(Int_t)fmaxUInt));
	IlcInfo(Form("Min allowed unsigned integer = %u",(Int_t)fminUInt));

	ffailedDPs->SetOwner(kTRUE);
}

//_______________________________________________________________

IlcGRPPreprocessor::~IlcGRPPreprocessor()
{
	//destructor
	
	delete fPressure;
	delete ffailedDPs;

}

//_______________________________________________________________

void IlcGRPPreprocessor::Initialize(Int_t run, UInt_t startTime, UInt_t endTime)
{
	// Initialize preprocessor

	IlcPreprocessor::Initialize(run, startTime, endTime);
	
	IlcInfo("Initialization of the GRP preprocessor.");
	IlcInfo(Form("Start Time DCS = %d",GetStartTimeDCSQuery()));
	IlcInfo(Form("End Time DCS = %d",GetEndTimeDCSQuery()));
	TClonesArray * array = new TClonesArray("IlcDCSSensor",kNumSensors); 
	for(Int_t j = 0; j < kNumSensors; j++) {
		IlcDCSSensor * sens = new ((*array)[j])IlcDCSSensor;
		sens->SetStringID(fgkDCSDataPoints[j+kSensors]);
	}
	IlcInfo(Form("Pressure Entries: %d",array->GetEntries()));
	
	fPressure = new IlcDCSSensorArray(GetStartTimeDCSQuery(), GetEndTimeDCSQuery(), array);

	ffailedDPs->Clear(); // cleaning ffailedDPs for current run
	for (Int_t iDP=0; iDP < fgknDCSDP; iDP++){
		TObjString* dp = new TObjString(fgkDCSDataPoints[iDP]);
		ffailedDPs->AddAt(dp,iDP);
	}

}

//_______________________________________________________________

UInt_t IlcGRPPreprocessor::Process(TMap* valueMap)
{
	// process data retrieved by the Shuttle
	
	// retrieving "partition" and "detector" fields from DAQ logbook to 
	// determine the partition in which the run was taken
	// the partition is used to decide how to react in case of errors for CTP

	TString partition = (TString)GetRunParameter("partition");  
	TString detector = (TString)GetRunParameter("detector");   

	IlcGRPObject *grpobj = new IlcGRPObject();  // object to store data
	grpobj->SetBeamEnergyIsSqrtSHalfGeV(); // new format

	//=================//
	// DAQ logbook     //
	//=================//

	Log("*************** Processing DAQ logbook");

	UInt_t error = 0;
	
	Int_t iDaqLB = ProcessDaqLB(grpobj);
	TString runType = (TString)GetRunType();
	TString beamType = (TString)GetRunParameter("beamType");
	if(iDaqLB == fgknDAQLbPar) {
		Log(Form("DAQ Logbook, successful! Retrieved %d/%d entries",iDaqLB,fgknDAQLbPar));
	} else {
		Log(Form("DAQ Logbook, could not get all expected entries!!! Retrieved only %d/%d entries",iDaqLB,fgknDAQLbPar));
		error |= 1;
	}

	//=================//
	// DAQ FXS         //
	//=================//

	Log("*************** Processing DAQ FXS");

	UInt_t iDaqFxs = ProcessDaqFxs();
	if( iDaqFxs == 0 ) {
		Log(Form("DAQ FXS, successful!"));
	} else {
		Log(Form("DAQ FXS, could not store run raw tag file!!!"));
		error |= 2;
	}
	
	//=================//
	// DCS FXS         //
	//=================//

	Log("*************** Processing DCS FXS");

	UInt_t iDcsFxs = ProcessDcsFxs(partition, detector);
	if( iDcsFxs == 0 ) {
		Log(Form("DCS FXS, successful!"));
	} else  if (iDcsFxs ==1) {
		Log(Form("Could not store CTP scalers!!!"));
		error |= 4;
	} else  if (iDcsFxs == 2) {
		Log(Form("Could not store CTP aliases!!!"));
		error |= 8192;
	} else{
		Log(Form("Incorrect field in DAQ logbook for partition = %s and detector = %s, going into error without CTP scalers...",partition.Data(),detector.Data()));
		error |= 32;
	}
	
	//=================//
	// DCS data points //
	//=================//

	Log("*************** Processing DCS DPs");

	Log(Form("Starting DCS Query at %d and finishing at %d",GetStartTimeDCSQuery(),GetEndTimeDCSQuery()));
	Int_t entries = ProcessDcsDPs( valueMap, grpobj );
	Log(Form("entries found = %d (should be %d)",entries, fgknDCSDP-fgkDCSDPNonWorking));
	if (fdaqStartEndTimeOk){
		if( entries < fgknDCSDP - fgkDCSDPNonWorking ) { // L3_BSF4_H3, L3_BSF17_H1, L3_BSF17_H2, L3_BSF17_H3, L3_BSF17_Temperature are not working yet...  
			Log(Form("Possible problem with the DCS data points!!! Only %d/%d entries found - Please read further for more details",entries,fgknDCSDP-fgkDCSDPNonWorking));
			Log(Form("The DPs giving problems were:"));
			for (Int_t iDP = 0; iDP < fgknDCSDP; iDP++){
				TObjString *dpString = (TObjString*)ffailedDPs->At(iDP);
				if (dpString){
					TString name = dpString->String();
					if (name != "L3_BSF4_H3" && name != "L3_BSF17_H1" && name != "L3_BSF17_H2" && name != "L3_BSF17_H3" && name != "L3_BSF17_Temperature" ){
						Log(Form("******** %s ******** not present, but foreseen --> causing an ERROR",name.Data()));
					}
					else {
						Log(Form(" %s is not present, but was not generating any error since it is not ready in DCS - check the other DPs in this list!",name.Data()));
					}
				}
			}
			error |= 8;
		}
		else  Log(Form("DCS data points, successful!"));
	}
	else Log(Form("Statistical values for DCS DPs could not be computed due to missing DAQ_time_start and DAQ_time_end fields in DAQ logbook")); 
	
	//=======================//
	// Trigger Configuration //
	//=======================//

	Log("*************** Processing Trigger Configuration");

	const char * triggerConf = GetTriggerConfiguration();

	if (partition.IsNull() && !detector.IsNull()){ // standalone partition
		Log("STANDALONE partition for current run, using Trigger Configuration dummy value");
		IlcCDBEntry *cdbEntry = GetFromOCDB("CTP","DummyConfig");
		if (!cdbEntry) {
			Log(Form("No dummy CTP configuration entry found, going into error..."));
			error |= 16;
		}
		else{
			IlcTriggerConfiguration *runcfg = (IlcTriggerConfiguration*)cdbEntry->GetObject();
			if (!runcfg){
				Log(Form("dummy CTP config not found in OCDB entry, going into error..."));
				error |= 16;
			}
			else {
				TString titleCTPcfg = Form("CTP cfg for run %i from Dummy entry in OCDB",fRun);
				runcfg->SetTitle(titleCTPcfg);
				IlcCDBMetaData metaData;
				metaData.SetResponsible("Roman Lietava");
				metaData.SetComment("CTP run configuration from dummy entry in OCDB");
				if (!Store("CTP","Config", runcfg, &metaData, 0, 0)) {
					Log("Unable to store the dummy CTP run configuration object to OCDB!");
					error |= 16;
				}
			}
		}
	}

	else if (!partition.IsNull() && detector.IsNull()){ // global partition
		Log("GLOBAL partition for current run, using Trigger Configuration from DAQ Logbook");
		if (triggerConf!= NULL) {
			Log("Found trigger configuration in DAQ logbook");
			IlcTriggerConfiguration *runcfg = IlcTriggerConfiguration::LoadConfigurationFromString(triggerConf);	  
			if (!runcfg) {
				Log("Bad CTP run configuration file from DAQ logbook! The corresponding CDB entry will not be filled!");
				error |= 16;
			}
			else {
				TString titleCTPcfg = Form("CTP cfg for run %i from DAQ",fRun);
				runcfg->SetTitle(titleCTPcfg);
				IlcCDBMetaData metaData;
				metaData.SetBeamPeriod(0);
				metaData.SetResponsible("Roman Lietava");
				metaData.SetComment("CTP run configuration from DAQ logbook");
				if (!Store("CTP","Config", runcfg, &metaData, 0, 0)) {
					Log("Unable to store the CTP run configuration object to OCDB!");
					error |= 16;
				}
			}
		}

		else {
			Log("Trigger configuration NULL in DAQ logbook");
			error |= 16;
		}
	}

	else {
		Log(Form("Incorrect field in DAQ logbook for partition = %s and detector = %s, going into error without trigger configuration...",partition.Data(),detector.Data()));
		error |= 32;
	}

        //===========================//
	// Trigger Timing Parameters //
        //===========================//

	Log("*************** Processing Trigger Time Params");
	
	const char * triggerCTPtiming = GetCTPTimeParams();

	if (partition.IsNull() && !detector.IsNull()){ // standalone partition
		Log("STANDALONE partition for current run, using CTP timing params dummy value");
		IlcCDBEntry *cdbEntry = GetFromOCDB("CTP","DummyCTPtime");
		if (!cdbEntry) {
			Log(Form("No dummy CTP timing parameters entry found, going into error..."));
			error |= 64;
		}
		else{
			IlcCTPTimeParams *runCTPtiming = (IlcCTPTimeParams*)cdbEntry->GetObject();
			if (!runCTPtiming){
				Log(Form("dummy CTP timing parameters not found in OCDB entry, going into error..."));
				error |= 64;
			}
			else {
				TString titleCTPtiming = Form("CTP timing params for run %i from Dummy entry in OCDB",fRun);
				runCTPtiming->SetTitle(titleCTPtiming);
				IlcCDBMetaData metadata;
				metadata.SetResponsible("Roman Lietava");
				metadata.SetComment("CTP run timing parameters from dummy entry in OCDB");
				if (!Store("CTP","CTPtiming", runCTPtiming, &metadata, 0, 0)) {
					Log("Unable to store the dummy CTP timing params object to OCDB!");
					error |= 64;
				}
			}
		}
	}

	else if (!partition.IsNull() && detector.IsNull()){ // global partition
		Log("GLOBAL partition for current run, using Trigger Timing Parameters from DAQ Logbook");
		if (triggerCTPtiming!= NULL) {
			Log("Found trigger timing params in DAQ logbook");
			IlcDebug(2,Form("%s",triggerCTPtiming));
			IlcCTPTimeParams *runCTPtiming = IlcCTPTimeParams::LoadCTPTimeParamsFromString(triggerCTPtiming);	  
			if (!runCTPtiming) {
				Log("Bad CTP trigger timing params file from DAQ logbook! The corresponding CDB entry will not be filled!");
				error |= 64;
			}
			else {
				TString titleCTPtiming = Form("CTP timing params for run %i from DAQ",fRun);
				runCTPtiming->SetTitle(titleCTPtiming);
				IlcCDBMetaData metadata;
				metadata.SetBeamPeriod(0);
				metadata.SetResponsible("Roman Lietava");
				metadata.SetComment("CTP timing params from DAQ logbook");
				if (!Store("CTP","CTPtiming", runCTPtiming, &metadata, 0, 0)) {
					Log("Unable to store the CTP timing params object to OCDB!");
					error |= 64;
				}
			}
		}

		else {
			Log("Trigger timing params NULL in DAQ logbook");
			error |= 64;
		}
	}

	else {
		Log(Form("Incorrect field in DAQ logbook for partition = %s and detector = %s, going into error without trigger timing parameters...",partition.Data(),detector.Data()));
		error |= 32;
	}

        //===========================//
	// LTU Configuration         //
        //===========================//

	Log("*************** Processing LTU Configuration");
	
	if (partition.IsNull() && !detector.IsNull()){ // standalone partition
		Log("STANDALONE partition for current run, using LTU configuration dummy value");
		IlcCDBEntry *cdbEntry = GetFromOCDB("CTP","DummyLTUConfig");
		if (!cdbEntry) {
			Log(Form("No dummy LTU Config entry found, going into error..."));
			error |= 2048;
		}
		else{
			TObjArray *ltuConfig = (TObjArray*)cdbEntry->GetObject();
			if (!ltuConfig){
				Log(Form("dummy LTU Config not found in OCDB entry, going into error..."));
				error |= 2048;
			}
			else {
				IlcCDBMetaData metadata;
				metadata.SetResponsible("Roman Lietava");
				metadata.SetComment("LTU Config from dummy entry in OCDB");
				if (!Store("CTP","LTUConfig", ltuConfig, &metadata, 0, 0)) {
					Log("Unable to store the dummy LTU Config object to OCDB!");
					error |= 2048;
				}
			}
		}
	}

	else if (!partition.IsNull() && detector.IsNull()){ // global partition
	
		Log("GLOBAL partition for current run, getting LTU Config from DAQ Logbook (logbook_detectors table)");
		UInt_t  detectorMask = (UInt_t)(((TString)GetRunParameter("detectorMask")).Atoi());
		Printf ("detectormask = %d",detectorMask);
		TObjArray * ltuarray = new TObjArray();
		ltuarray->SetOwner(1);
		Bool_t isLTUok = kTRUE;
		for(Int_t i = 0; i<IlcDAQ::kNDetectors-2; i++){
			if ((detectorMask >> i) & 0x1) {
				TString det = IlcDAQ::OfflineModuleName(i);
				TString detCTPName = IlcTriggerInput::fgkCTPDetectorName[i];
				if (detCTPName == "CTP") {
					detCTPName="TRG"; // converting according to what is found in DAQ logbook_detectors					
					Printf("Processing CTP (CTP Detector name %s) --> SKIPPING, CTP does not have any LTU!!!!!!",detCTPName.Data());
					continue;
				}				
				Printf("Processing detector %s (CTP Detector name %s)",det.Data(),detCTPName.Data());
				TString* ltu = GetLTUConfig(detCTPName.Data());
				if (!ltu){
					Log(Form("No LTU Configuration from DAQ logbook for detector %s (BUT it was expected)! The corresponding CDB entry will not be filled!",detCTPName.Data()));
					error |= 2048;
					isLTUok = kFALSE;
					break;
				}
				else{
					Float_t ltuFineDelay1 = ltu[0].Atof();
					Float_t ltuFineDelay2 = ltu[1].Atof();
					Float_t ltuBCDelayAdd = ltu[2].Atof();
					const char* name = IlcDAQ::DetectorName(i);
					IlcLTUConfig* ltuConfig = new IlcLTUConfig((UChar_t)IlcDAQ::DetectorID(name),ltuFineDelay1,ltuFineDelay2,ltuBCDelayAdd);
					ltuarray->AddAtAndExpand(ltuConfig,i);
				}				
			}
		}
		if (isLTUok){
			IlcCDBMetaData metadata;
			metadata.SetBeamPeriod(0);
			metadata.SetResponsible("Roman Lietava");
			metadata.SetComment("LTU Configuration for current run");
			if (!Store("CTP","LTUConfig", ltuarray, &metadata, 0, 0)) {
				Log("Unable to store the LTU Config object to OCDB!");
				error |= 2048;
			}		
		}
		delete ltuarray;
	}

	else {
		Log(Form("Incorrect field in DAQ logbook for partition = %s and detector = %s, going into error without trigger timing parameters...",partition.Data(),detector.Data()));
		error |= 32;
	}


	//=================//
	// LHC Data        //
	//=================//

	if (runType == "PHYSICS"){  // processing the LHC file only in PHYSICS runs
		Log("*************** Processing LHC Data");

		UInt_t iLHCData = ProcessLHCData(grpobj);
		
		if( iLHCData == 0 ) {
			Log(Form("LHC Data from FXS, successful!"));
		} else  if (iLHCData == 1) {
			Log(Form("LHC Data, problems with FXS!"));
			error |= 256;
		} else  if (iLHCData == 2) {
			Log(Form("LHC Data, problems with DAQ_time_start/DAQ_time_end!"));
			error |= 512;
		} else if (iLHCData ==3){
			Log(Form("Problems in storing LHC Phase - going into Error"));
			error |= 1024;
		} else if (iLHCData ==4){
			Log(Form("Problems with LHC Phase - going into Error"));
			error |= 1024;
		} else{
			Log(Form("LHC Data problems"));
			error |= 512;
		}
	
	}

	//==================//
	// SPD Mean Vertex  //
	//==================//

	Log("*************** Processing SPD Mean Vertex");

	if (runType == "PHYSICS"){
		UInt_t iSPDMeanVertex = ProcessSPDMeanVertex();
		if( iSPDMeanVertex == 1 ) {
			Log(Form("SPD Mean Vertex, successful!"));
		} else {
			Log(Form("SPD Mean Vertex failed!!!"));
			error |= 128; 
		}
	}
	else {
		Log("SPD Mean Vertex not processed since runType != PHYSICS");
	}

	//=================//
	// DQM FXS         //
	//=================//

	Log("*************** Processing DQM FXS");

	UInt_t iDqmFxs = ProcessDqmFxs();
	if( iDqmFxs == 1 ) {
		Log(Form("DQM FXS, successful!"));
	} else {
		Log(Form("DQM FXS failed!!!"));
		error |= 4096;
	}

	// storing IlcGRPObject in OCDB

	IlcCDBMetaData md;
	md.SetResponsible("Chiara Zampolli");
	md.SetComment("Output parameters from the GRP preprocessor.");
	
	Bool_t result = kTRUE;
	result = Store("GRP", "Data", grpobj, &md); 
	delete grpobj;

	if (result && !error ) {
		Log("GRP Preprocessor Success");
		return 0;
	} else {
		Log( Form("GRP Preprocessor FAILS!!! %s%s%s%s%s%s%s%s%s%s%s%s%s%s",
			  kppError[(error&1)?1:0],
			  kppError[(error&2)?2:0],
			  kppError[(error&4)?3:0],
			  kppError[(error&8)?4:0],
			  kppError[(error&16)?5:0],
			  kppError[(error&32)?6:0],
			  kppError[(error&64)?7:0],
			  kppError[(error&128)?8:0],
			  kppError[(error&256)?9:0],
			  kppError[(error&512)?10:0],
			  kppError[(error&1024)?11:0],
			  kppError[(error&2048)?12:0],
			  kppError[(error&4096)?13:0],
			  kppError[(error&8192)?14:0]
			  ));
		return error;
	}


}

//_______________________________________________________________

UInt_t IlcGRPPreprocessor::ProcessLHCData(IlcGRPObject *grpobj)
{
	//
	//Getting the LHC Data from DCS FXS
	//

	TString timeStartString = (TString)GetRunParameter("DAQ_time_start");
	TString timeEndString = (TString)GetRunParameter("DAQ_time_end");
	if (timeStartString.IsNull() || timeEndString.IsNull()){
		if (timeStartString.IsNull()){ 
			IlcError("DAQ_time_start not set in logbook! Setting statistical values for current DP to invalid");
		}
		else if (timeEndString.IsNull()){
			IlcError("DAQ_time_end not set in logbook! Setting statistical values for current DP to invalid");
		}
		return 2;
	}  

	Double_t timeStart = timeStartString.Atof();
	Double_t timeEnd = timeEndString.Atof();

	TString fileName = GetFile(kDCS, "LHCData","");
	if (fileName.Length()>0){
		IlcInfo("Got The LHC Data file");
		IlcLHCReader lhcReader;

		// Processing data to be put in IlcGRPObject

		// Energy
		Log("*************Energy ");
		TObjArray* energyArray = lhcReader.ReadSingleLHCDP(fileName.Data(),fgkLHCDataPoints[0]);
		if (energyArray){			
			Float_t energy = ProcessEnergy(energyArray,timeStart);
			if (energy != -1.) {
				grpobj->SetBeamEnergy(energy);
				grpobj->SetBeamEnergyIsSqrtSHalfGeV(kTRUE);
			}
			delete energyArray;
		}
		else {
			IlcError("Energy not found in LHC Data file!!!");
		}	

		Double_t timeBeamModeEnd = timeEnd;        // max validity for Beam Mode 
		Double_t timeMachineModeEnd = timeEnd;     // max validity for Machine Mode
 		Double_t timeBeamEnd = timeEnd;            // max validity for Beam Type
 		Double_t timeBeamTypeEnd[2] = {timeEnd, timeEnd}; // max validity for Beam Type1,2
		Double_t timeBeamModeStart = -1;    // min validity for Beam Mode
		Double_t timeMachineModeStart = -1; // min validity for Machine Mode
		Double_t timeBeamStart = -1;        // min validity for Beam Type
		Double_t timeBeamTypeStart[2] = {-1,-1};        // min validity for Beam Type1,2
		Int_t indexBeamMode = -1;                  // index of measurement used to set Beam Mode
	        Int_t indexMachineMode = -1;               // index of measurement used to set Machine Mode
		Int_t indexBeam = -1;                      // index of measurement used to set Beam Type
		Int_t indexBeamType[2] = {-1, -1};                      // index of measurement used to set Beam Type1,2
		Bool_t foundBeamModeStart = kFALSE;        // flag to be set in case an entry for the Beam Mode is found before (or at) SOR
		Bool_t foundMachineModeStart = kFALSE;     // flag to be set in case an entry for the Machine Mode is found before (or at) SOR
		Bool_t foundBeamStart = kFALSE;            // flag to be set in case an entry for the Beam Type is found before (or at) SOR
		Bool_t foundBeamTypeStart[2] = {kFALSE, kFALSE};            // flag to be set in case an entry for the Beam Type1,2 is found before (or at) SOR
		Bool_t flagBeamMode = kFALSE;  //flag set true if a changed occurred in BeamMode
		Bool_t flagMachineMode = kFALSE;  //flag set true if a changed occurred in MachineMode
		Bool_t flagBeam = kFALSE;  //flag set true if a changed occurred in BeamType
		Bool_t flagBeamType[2] = {kFALSE, kFALSE};  //flag set true if a changed occurred in BeamType1,2
		
		Double_t arrayTimes[5]={2.E9, 2.E9, 2.E9, 2.E9, 2.E9}; // array to keep track of the times of the possible changes of the LHC DPs; each entry set to Wed May 18 2033, 03:33:20 GMT (ILC should not be running anymore...)
		                                                       // arrayTimes elements order correspond to the one used in the array of the strings fgkLHCDataPoints, i.e.:
		                                                       // arrayTimes[0] --> MachineMode
		                                                       // arrayTimes[1] --> BeamMode
		                                                       // arrayTimes[2] --> BeamType (when written together)
		                                                       // arrayTimes[3] --> BeamType1 (when written separate)
		                                                       // arrayTimes[4] --> BeamType2 (when written separate)

		// BeamMode
		Log("*************BeamMode (LHCState) ");
		TObjArray* beamModeArray = lhcReader.ReadSingleLHCDP(fileName.Data(),fgkLHCDataPoints[2]);
		Int_t nBeamMode = -1;
		if (beamModeArray){	
			nBeamMode = beamModeArray->GetEntries();	
			if (nBeamMode==0){
				IlcInfo("Found zero entries for the Beam Mode, leaving it empty");
			}
			else{
				for (Int_t iBeamMode = 0; iBeamMode<nBeamMode; iBeamMode++){
					IlcDCSArray* beamMode = (IlcDCSArray*)beamModeArray->At(iBeamMode);
					if (beamMode){
						if (beamMode->GetTimeStamp()<=timeStart && beamMode->GetTimeStamp()>=timeBeamModeStart){// taking always the very last entry: of two measurements have the same timestamp, the last one is taken
							timeBeamModeStart = beamMode->GetTimeStamp();
							indexBeamMode = iBeamMode;
							foundBeamModeStart = kTRUE;
						}
						else {
							break;

						}
					}
				}
				if (!foundBeamModeStart){
					IlcInfo("No value for the Beam Mode found before start of run, the Beam Mode will remain empty");
				}
				else {
					IlcDCSArray* beamMode = (IlcDCSArray*)beamModeArray->At(indexBeamMode);
					TObjString* beamModeString = beamMode->GetStringArray(0);
					IlcInfo(Form("LHC State (corresponding to BeamMode) = %s (set at %f)",(beamModeString->String()).Data(),beamMode->GetTimeStamp()));
					grpobj->SetLHCState(beamModeString->String());
					if (indexBeamMode < nBeamMode-1){
						IlcDCSArray* beamMode1 = (IlcDCSArray*)beamModeArray->At(indexBeamMode+1);
						if (beamMode1){
							if (beamMode1->GetTimeStamp()<=timeStart){
								IlcError("you did not choose the correct value! there is still something before (or at) SOR, but later than this!");
							}
							else if (beamMode1->GetTimeStamp()>timeStart && beamMode1->GetTimeStamp()<=timeEnd){
								timeBeamModeEnd = beamMode1->GetTimeStamp();
								TObjString* beamModeString1 = beamMode1->GetStringArray(0);
							        TString bmString0 = beamModeString->String();
							        TString bmString1 = beamModeString1->String();
								if (bmString0.CompareTo(bmString1.Data(),TString::kIgnoreCase) == -1){
									IlcWarning(Form("The beam mode changed from %s to %s during the run at timestamp %f! Setting it to %s and keeping track of the time of the change to set MaxTimeLHCValidity afterward",bmString0.Data(), bmString1.Data(), timeBeamModeEnd, bmString0.Data()));
									flagBeamMode = kTRUE;
									arrayTimes[1]=timeBeamModeEnd;

								}
							}
						}
						else {
							IlcInfo("Invalid pointer for the first entry for Beam Mode after the first valid one, not considering anything after what has already been found");
						}
					}
				}
			}
			delete beamModeArray;
		}
		else{
			IlcError("Beam mode array not found in LHC Data file!!!");
		}
		
		// MachineMode
		Log("*************MachineMode ");
		TObjArray* machineModeArray = lhcReader.ReadSingleLHCDP(fileName.Data(),fgkLHCDataPoints[1]);
		Int_t nMachineMode = -1;
		if (machineModeArray){
			nMachineMode = machineModeArray->GetEntries();
			if (nMachineMode==0){
				IlcInfo("No Machine Mode found, leaving it empty");
			}
			else{
				for (Int_t iMachineMode = 0; iMachineMode<nMachineMode; iMachineMode++){
					IlcDCSArray* machineMode = (IlcDCSArray*)machineModeArray->At(iMachineMode);
					if (machineMode){
						if (machineMode->GetTimeStamp()<=timeStart && machineMode->GetTimeStamp()>=timeMachineModeStart){// taking always the very last entry: of two measurements have the same timestamp, the last one is taken
							timeMachineModeStart = machineMode->GetTimeStamp();
							indexMachineMode = iMachineMode;
							foundMachineModeStart = kTRUE;
						}
						else{
							break;
						}
					}
				}
				if (!foundMachineModeStart){
					IlcInfo("No value for the Machine Mode found before start of run, the Machine Mode will remain empty");
				}
				else {
					IlcDCSArray* machineMode = (IlcDCSArray*)machineModeArray->At(indexMachineMode);
					TObjString* machineModeString = machineMode->GetStringArray(0);
					IlcInfo(Form("MachineMode = %s (set at %f)",(machineModeString->String()).Data(),machineMode->GetTimeStamp()));
					grpobj->SetMachineMode(machineModeString->String());
					if (indexMachineMode < nMachineMode-1){
						IlcDCSArray* machineMode1 = (IlcDCSArray*)machineModeArray->At(indexMachineMode+1);
						if (machineMode1){
							if (machineMode1->GetTimeStamp()>timeStart && machineMode1->GetTimeStamp()<=timeEnd){
								timeMachineModeEnd = machineMode1->GetTimeStamp();
								TObjString* machineModeString1 = machineMode1->GetStringArray(0);
							        TString mmString0 = machineModeString->String();
							        TString mmString1 = machineModeString1->String();
								if (mmString0.CompareTo(mmString1.Data(),TString::kIgnoreCase) == -1){
									IlcWarning(Form("The machine mode changed from %s to %s during the run at timestamp %f! Setting it to %s and keeping track of the time of the change to set MaxTimeLHCValidity afterward",mmString0.Data(),mmString1.Data(),timeMachineModeEnd,mmString0.Data()));
									flagMachineMode = kTRUE;
									arrayTimes[0]=timeMachineModeEnd;
								}
							}
						}
						else {
							IlcInfo("Invalid pointer for the first entry for Machine Mode after the first valid one, not considering anything after what has already been found");
						}
					}
				}
			}
			delete machineModeArray;
		}
		else{
			IlcError("Machine mode array not found in LHC Data file!!!");
		}
		
		// BeamType1 and BeamType2 - both put in the same string
		Log("*************BeamType ");
		TObjArray* beamArray = lhcReader.ReadSingleLHCDP(fileName.Data(),fgkLHCDataPoints[3]);
		if (beamArray){			
			Int_t nBeam = beamArray->GetEntries();
			if (nBeam==0){
				IlcInfo("No Beam Type found, leaving it empty");
			}
			else{
				for (Int_t iBeam = 0; iBeam<nBeam; iBeam++){
					IlcDCSArray* beam = (IlcDCSArray*)beamArray->At(iBeam);
					if (beam){
						if (beam->GetTimeStamp()<=timeStart && beam->GetTimeStamp()>=timeBeamStart){// taking always the very last entry: of two measurements have the same timestamp, the last one is taken
							timeBeamStart = beam->GetTimeStamp();
							indexBeam = iBeam;
							foundBeamStart = kTRUE;
						}
						else{
							break;
						}
					}
				}
				if (!foundBeamStart){
					IlcInfo("No value for the Beam Type found before start of run, the (common) Beam Type will remain empty");
				}
				else {
					IlcDCSArray* beam = (IlcDCSArray*)beamArray->At(indexBeam);
					TObjString* beamString = beam->GetStringArray(0);
					TString beamType = beamString->String();
					IlcInfo(Form("Beam Type = %s",beamType.Data()));	
					if (beamType.CompareTo("PROTON",TString::kIgnoreCase) == 0){
						IlcInfo("Setting beam type to p-p");
						grpobj->SetBeamType("p-p");
					}
					else { // if there is no PROTON beam, we suppose it is Pb, and we put A-A
						IlcInfo("Setting beam type to A-A");
						grpobj->SetBeamType("A-A");
					}
					/*
					  else if (beamType.CompareTo("LEAD82",TString::kIgnoreCase) == 0){
						IlcInfo("Setting beam type to Pb-Pb");
						grpobj->SetBeamType("Pb-Pb");
					}
					else{
						IlcError("Beam Type not known, leaving it empty");
					}
					*/
					if (indexBeam < nBeam-1){
						IlcDCSArray* beam1 = (IlcDCSArray*)beamArray->At(indexBeam+1);
						if (beam1){
							if (beam1->GetTimeStamp()>timeStart && beam1->GetTimeStamp()<=timeEnd){
								timeBeamEnd = beam1->GetTimeStamp();
								TObjString* beamString1 = beam1->GetStringArray(0);
								TString beamType1 = beamString1->String();
								if (beamType.CompareTo(beamType1.Data(),TString::kIgnoreCase) == -1){
									IlcWarning(Form("The Beam Type changed from %s to %s during the run at timestamp %f! Setting it to %s and keeping track of the time of the change to set MaxTimeLHCValidity afterward",beamType.Data(),(beamString1->String()).Data(),timeBeamEnd,beamType.Data()));
									flagBeam = kTRUE;
									arrayTimes[2] = timeBeamEnd;
								}
							}
						}
						else {
							IlcInfo("Invalid pointer for the first entry for Beam Type after the first valid one, not considering anything after what has already been found");
						}
					}
				}
			}
			delete beamArray;
		}
		else{
			IlcError("Beam Type array not found in LHC Data file!!!");
		}		

		// BeamType1 and BeamType2 - in separete string
		Log("*************BeamType, 1 and 2 ");
		Int_t indexBeamTypeString = 6;  // index of the string with the alias of BeanType1 in the array fgkLHCDataPoints
		TString combinedBeamType = "-";  // combined beam type, built from beam type 1 and beam type 2
		TString combinedBeamTypeFromLHC = "-";  // combined beam type, built from beam type 1 and beam type 2 AS SENT FROM LHC
		for (Int_t ibeamType = 0; ibeamType<2; ibeamType++){
			beamArray = lhcReader.ReadSingleLHCDP(fileName.Data(),fgkLHCDataPoints[indexBeamTypeString+ibeamType]);
			if (beamArray){			
				Int_t nBeam = beamArray->GetEntries();
				if (nBeam==0){
					IlcInfo(Form("No Beam Type %s found, leaving it empty",fgkLHCDataPoints[indexBeamTypeString+ibeamType]));
				}
				else{
					for (Int_t iBeam = 0; iBeam<nBeam; iBeam++){
						IlcDCSArray* beam = (IlcDCSArray*)beamArray->At(iBeam);
						if (beam){
							if (beam->GetTimeStamp()<=timeStart && beam->GetTimeStamp()>=timeBeamTypeStart[ibeamType]){// taking always the very last entry: of two measurements have the same timestamp, the last one is taken
								timeBeamTypeStart[ibeamType] = beam->GetTimeStamp();
								indexBeamType[ibeamType] = iBeam;
								foundBeamTypeStart[ibeamType] = kTRUE;
							}
							else{
								break;
							}
						}
					}
					if (!foundBeamTypeStart[ibeamType]){
						IlcInfo(Form("No value for the Beam Type %s found before start of run, the Beam Type %d will remain empty", fgkLHCDataPoints[indexBeamTypeString+ibeamType], ibeamType));
					}
					else {
						IlcDCSArray* beam = (IlcDCSArray*)beamArray->At(indexBeam);
						TObjString* beamString = beam->GetStringArray(0);
						TString beamType = beamString->String();
						IlcInfo(Form("Beam Type (for %s) = %s", fgkLHCDataPoints[indexBeamTypeString+ibeamType], beamType.Data()));
						TString singleBeam = ParseBeamTypeString(beamType,ibeamType);
						IlcInfo(Form("Single Beam Type for beam %d set to %s", ibeamType, singleBeam.Data()));
						grpobj->SetSingleBeamType(ibeamType, singleBeam);
						if (beamType.CompareTo("PROTON",TString::kIgnoreCase) == 0){
							IlcInfo(Form("Setting beam %d for combined beam type to p", ibeamType));
							if (ibeamType == 0) combinedBeamType.Prepend("p"); 
							else combinedBeamType.Append("p");
						}
						else { // if there is no PROTON beam, we suppose it is Pb, and we put A-A
							IlcInfo(Form("Setting beam %d for combined beam type to A",ibeamType));
							if (ibeamType == 0) combinedBeamType.Prepend("A");
							else combinedBeamType.Append("A");
						}
						if (ibeamType == 0) combinedBeamTypeFromLHC.Prepend(beamType); 
						else combinedBeamTypeFromLHC.Append(beamType);
						/*
						  else if (beamType.CompareTo("LEAD82",TString::kIgnoreCase) == 0){
						  IlcInfo("Setting beam type to Pb-Pb");
						  grpobj->SetSingleBeamType(ibeamType, "Pb-Pb");
						  }
						  else{
						  IlcError("Beam Type not known, leaving it empty");
						  }
						*/
						if (indexBeamType[ibeamType] < nBeam-1){
							IlcDCSArray* beam1 = (IlcDCSArray*)beamArray->At(indexBeam+1);
							if (beam1){
								if (beam1->GetTimeStamp()>timeStart && beam1->GetTimeStamp()<=timeEnd){
									timeBeamTypeEnd[ibeamType] = beam1->GetTimeStamp();
									TObjString* beamString1 = beam1->GetStringArray(0);
									TString beamType1 = beamString1->String();
									if (beamType.CompareTo(beamType1.Data(),TString::kIgnoreCase) == -1){
										IlcWarning(Form("The Beam Type for %s changed from %s to %s during the run at timestamp %f! Setting it to %s and keeping track of the time of the change to set MaxTimeLHCValidity afterward",fgkLHCDataPoints[indexBeamTypeString+ibeamType],beamType.Data(),(beamString1->String()).Data(),timeBeamEnd,beamType.Data()));
										flagBeamType[ibeamType] = kTRUE;
										arrayTimes[3+ibeamType] = timeBeamTypeEnd[ibeamType];
									}
								}
							}
							else {
								IlcInfo(Form("Invalid pointer for the first entry for Beam Type %s after the first valid one, not considering anything after what has already been found",fgkLHCDataPoints[indexBeamTypeString+ibeamType]));
							}
						}
					}
				}
				delete beamArray;
			}
			else{
				IlcError(Form("Beam Type %s array not found in LHC Data file!!!",fgkLHCDataPoints[indexBeamTypeString+ibeamType]));
			}		
		}
		IlcInfo(Form("Setting combined beam type to %s",combinedBeamType.Data()));
		grpobj->SetBeamType(combinedBeamType);
		IlcInfo(Form("Setting combined beam type form LHC to %s",combinedBeamTypeFromLHC.Data()));
		grpobj->SetBeamTypeFromLHC(combinedBeamTypeFromLHC);
		
		// Setting minTimeLHCValidity
		if (flagBeamMode == kTRUE || flagMachineMode == kTRUE || flagBeam == kTRUE || flagBeamType[0] == kTRUE || flagBeamType[1] == kTRUE){ 
			Double_t minTimeLHCValidity= TMath::MinElement(5,arrayTimes);
			IlcWarning(Form("Setting MaxTimeLHCValidity to %f",minTimeLHCValidity));
			grpobj->SetMaxTimeLHCValidity(minTimeLHCValidity);
		}
		/* 
		   // Old way to determine the Maximum Time during which the LHC info is valid
		if (timeBeamModeEnd!=0 || timeMachineModeEnd!=0 || timeBeamEnd !=0){
			Double_t minTimeLHCValidity;
			if (flagBeamMode == kFALSE && flagMachineMode == kFALSE && flagBeam == kTRUE){ // flagBeam only true --> it is the only one that changed
				minTimeLHCValidity = timeBeamEnd;
			}
			else if (flagBeamMode == kFALSE && flagMachineMode == kTRUE && flagBeam == kFALSE){ // flagMachineMode only true
				minTimeLHCValidity = timeMachineModeEnd;
			}
			else if (flagBeamMode == kTRUE && flagMachineMode == kFALSE && flagBeam == kFALSE){ // flagBeamMode only true
				minTimeLHCValidity = timeBeamModeEnd;
			}
			else if (flagBeamMode == kFALSE && flagMachineMode == kTRUE && flagBeam == kTRUE){ // flagBeam and flagMachineMode only true
				minTimeLHCValidity= TMath::Min(timeBeamEnd,timeMachineModeEnd);
			}
			else if (flagBeamMode == kTRUE && flagMachineMode == kFALSE && flagBeam == kTRUE){ // flagBeam and flagBeamMode only true
				minTimeLHCValidity= TMath::Min(timeBeamEnd,timeBeamModeEnd);
			}
			else if (flagBeamMode == kTRUE && flagMachineMode == kTRUE && flagBeam == kFALSE){ // flagMachineMode and flagBeamMode only true
				minTimeLHCValidity= TMath::Min(timeMachineModeEnd,timeBeamModeEnd);
			}
			else {
				Double_t arrayTimes[3] = {timeBeamModeEnd,timeMachineModeEnd,timeBeamEnd};// flagMachineMode and flagBeamMode and flagBeam 
				minTimeLHCValidity= TMath::MinElement(3,arrayTimes);
			}
			IlcWarning(Form("Setting MaxTimeLHCValidity to %f",minTimeLHCValidity));
			grpobj->SetMaxTimeLHCValidity(minTimeLHCValidity);
		}
		*/
		
		// Data Quality Flag --> storing start and end values of periods within the run during which the value was found to be FALSE
		Log("*************Data Quality Flag ");
		TObjArray* dataQualityArray = lhcReader.ReadSingleLHCDP(fileName.Data(),fgkLHCDataPoints[8]);
		Int_t nDataQuality = -1;
		Double_t timeDataQualityStart = -1; // min validity for Data Quality Flag
	        Int_t indexDataQuality = -1;               // index of first measurement used to set Data Quality Flag
		Bool_t foundDataQualityStart = kFALSE;     // flag to be set in case an entry for the Data Quality Flag is found before (or at) SOR

		if (dataQualityArray){
			nDataQuality = dataQualityArray->GetEntries();
			if (nDataQuality==0){
				IlcInfo("No Data Quality Flag found, leaving it empty");
			}
			else{
				for (Int_t iDataQuality = 0; iDataQuality<nDataQuality; iDataQuality++){
					IlcDCSArray* dataQuality = (IlcDCSArray*)dataQualityArray->At(iDataQuality);
					if (dataQuality){
						if (dataQuality->GetTimeStamp()<=timeStart && dataQuality->GetTimeStamp()>=timeDataQualityStart){// taking always the very last entry: if two measurements have the same timestamp, the last one is taken
							timeDataQualityStart = dataQuality->GetTimeStamp();
							indexDataQuality = iDataQuality;
							foundDataQualityStart = kTRUE;
						}
						else{
							// we suppose here that if the first measurement is not before SOR, then none will be (they MUST be in chronological order!!!) 
							break;
						}
					}
				}
				if (!foundDataQualityStart){
					// The Data Quality Flag should be found and TRUE at the start of the run. For the time being, if it is not found, don't do anything, but it means there is a problem..
					IlcInfo("No value for the Data Quality Flag found before start of run, the Data Quality Flag will remain empty");
				}
				else {
					// counting how many FALSE values there are
					Bool_t foundEndOfFalse = kFALSE;
					Int_t nFalse = 0;
					for (Int_t iDataQuality = indexDataQuality; iDataQuality < nDataQuality; iDataQuality ++){
						IlcDCSArray* dataQuality = (IlcDCSArray*)dataQualityArray->At(iDataQuality);
						IlcDebug(4,Form("dataQuality->GetTimeStamp() = %f, timeDataQualityStart = %f, timeEnd = %f", dataQuality->GetTimeStamp(), timeDataQualityStart, timeEnd ));
						if (dataQuality->GetTimeStamp()>=timeDataQualityStart && dataQuality->GetTimeStamp()<=timeEnd){ // considering only values between the first valid and the end of the run
							Bool_t dataQualityFlag = dataQuality->GetBool(0);
							IlcDebug(3,Form("DataQuality = %d (set at %f)",(Int_t)dataQualityFlag,dataQuality->GetTimeStamp()));
							if (dataQualityFlag != kTRUE){
								if (iDataQuality == indexDataQuality) {  // the first Data Quality value should be TRUE, but ignoring the problem now...
									IlcError("The first value for the Data Quality MUST be TRUE! Ignoring for now...");
								}
								nFalse++;
							}
						}
					}

					IlcInfo(Form("Found %d FALSE values for the Data Quality Flag",nFalse));
					Double_t falses[nFalse*2];  // dimensioning this to the maximum possible, as if each false value was followed by a true one --> the false periods correspond to the number of falses

					Int_t iDataQuality = indexDataQuality;
					if (nFalse > 0){
						Int_t iFalse = 0;
						// filling the info about the periods when the flag was set to FALSE
						// starting, like for the other DPS, from the measurement closest to SOR (the index of which is iDataQuality)
						while (iDataQuality < nDataQuality){
							IlcDebug(3,Form("iDataQuality = %d",iDataQuality));
							IlcDCSArray* dataQuality = (IlcDCSArray*)dataQualityArray->At(iDataQuality);
							if (dataQuality->GetTimeStamp()>=timeDataQualityStart && dataQuality->GetTimeStamp()<=timeEnd){ // considering only values between the first valid and the end of the run
								Bool_t dataQualityFlag = dataQuality->GetBool(0);
								IlcDebug(3,Form("DataQuality = %d (set at %f)",(Int_t)dataQualityFlag,dataQuality->GetTimeStamp()));
								if (dataQualityFlag == kTRUE){
									// found TRUE value, continuing
									iDataQuality++;
									continue;
								}
								else{
									/*
									// the check was already done before
									if (iDataQuality == indexDataQuality) {  // the first Data Quality value should be TRUE, but ignoring the problem now...
									IlcError("The first value for the Data Quality MUST be TRUE! Ignoring for now...");
									}
									*/
									falses[iFalse*2] = dataQuality->GetTimeStamp();
									foundEndOfFalse = kFALSE;
									Int_t iDataQualityNext = iDataQuality+1;
									while (iDataQualityNext < nDataQuality){
										IlcDCSArray* dataQualityNext = (IlcDCSArray*)dataQualityArray->At(iDataQualityNext);
										if (dataQualityNext->GetTimeStamp()>timeDataQualityStart && dataQualityNext->GetTimeStamp()<=timeEnd && dataQualityNext->GetTimeStamp() > dataQuality->GetTimeStamp()){ // considering only values between the first valid and the end of the run, and subsequent to the current value
											Bool_t dataQualityFlagNext = dataQualityNext->GetBool(0);
											IlcDebug(3,Form("DataQualityNext = %d (set at %f)",(Int_t)dataQualityFlagNext,dataQualityNext->GetTimeStamp()));
											if (dataQualityFlagNext == kTRUE){
												// found TRUE value, first FALSE period completed
												foundEndOfFalse = kTRUE;
												falses[iFalse*2+1] = dataQualityNext->GetTimeStamp();
												iFalse++;
												break;
											}
											iDataQualityNext++;
										}
									}
									if (!foundEndOfFalse) {
										IlcInfo("Please, note that the last FALSE value lasted until the end of the run");
										falses[iFalse*2+1] = timeEnd;
										iFalse++;
										break;
									}
									iDataQuality = iDataQualityNext+1;
								}
							}
						}
						grpobj->SetNFalseDataQualityFlag(iFalse);
						grpobj->SetFalseDataQualityFlagPeriods(falses);
					}
				}
			}
			delete dataQualityArray;
		}
		else{
			IlcError("Data Quality Flag array not found in LHC Data file!!!");
		}

		// Processing data to go to IlcLHCData object
		IlcLHCData* dt = new IlcLHCData(fileName.Data(),timeStart,timeEnd);
		// storing IlcLHCData in OCDB
		if (dt){
		  IlcInfo(Form("Filled %d records to IlcLHCData object",dt->GetData().GetEntriesFast()));
		  IlcCDBMetaData md;
		  md.SetResponsible("Ruben Shahoyan");
		  md.SetComment("LHC data from the GRP preprocessor.");
		  Bool_t result = kTRUE;
		  result = Store("GRP", "LHCData", dt, &md); 
		  delete dt;
		  if (!result){
			Log(Form("Problems in storing LHC Data - but not going into Error"));
		  }
		}

		// processing LHC Phase

		TObjArray *beam1phase = lhcReader.ReadSingleLHCDP(fileName.Data(),fgkLHCDataPoints[4]);
		TObjArray *beam2phase = lhcReader.ReadSingleLHCDP(fileName.Data(),fgkLHCDataPoints[5]);
		if (beam1phase == 0x0 || beam2phase == 0x0){
			Log(Form("Problems in retrieving LHC Clock data from LHC file"));
			return 4;
		}			
		IlcLHCClockPhase *phaseObj = ProcessLHCClockPhase(beam1phase,beam2phase,timeEnd);
		delete beam1phase;
		delete beam2phase;
		if (phaseObj){
			IlcInfo(Form("LHC Phase found"));
			IlcCDBMetaData mdPhase;
			mdPhase.SetResponsible("Cvetan Cheshkov");
			mdPhase.SetComment("LHC Clock Phase");
			Bool_t result = kTRUE;
			result = Store("Calib", "LHCClockPhase", phaseObj, &mdPhase); 
			delete phaseObj;
			if (!result) return 3;
		}
		else return 4;
	}
	
	else {
		IlcError("No LHCData file found in FXS");
		return 1;
	}

	return 0;
}

//_______________________________________________________________

UInt_t IlcGRPPreprocessor::ProcessSPDMeanVertex()
{
	//
	//Getting the SPD Mean Vertex
	//

	TList* list = GetForeignFileSources("SPD", kDAQ, "VertexDiamond");
	Bool_t storeResult = kTRUE;
	if (list !=0x0 && list->GetEntries()!=0)
		{
			IlcInfo("The following sources produced files with the id VertexDiamond from SPD");
			list->Print();
			for (Int_t jj=0;jj<list->GetEntries();jj++){
				TObjString * str = dynamic_cast<TObjString*> (list->At(jj)); 
				if (!str){
					IlcError(Form("Expecting a TObjString in the list for the %d-th source, but something else was found.",jj));
					continue;
				}
				IlcInfo(Form("found source %s", str->String().Data()));
				TString fileNameRun = GetForeignFile("SPD", kDAQ, "VertexDiamond", str->GetName());
				if (fileNameRun.Length()>0){
					IlcInfo(Form("Got the file %s", fileNameRun.Data()));
					TFile daqFile(fileNameRun.Data(),"READ");
					if (daqFile.IsOpen()) {
						IlcESDVertex* meanVtx = dynamic_cast<IlcESDVertex*>(daqFile.Get("MeanVertexPos"));
						if (meanVtx){
							meanVtx->Print();	
							// storing in the OCDB 
							IlcCDBMetaData md;
							md.SetResponsible("Cvetan Cheshkov");
							md.SetComment("SPD Mean Vertex");					
							storeResult = Store("Calib", "MeanVertexSPD", meanVtx, &md, 0, kTRUE); 
						}
						else{
							IlcWarning("No SPD Mean Vertex object found in file");
						} 
					}
					else {
						IlcError("Can't open file");
						storeResult = kFALSE;
					}
				}
				else{
					IlcWarning("No file found for current source for SPD Mean Vertex");
				}
			}
		}
	else {
		IlcWarning("No list found for SPD Mean Vertex");
	}

	if (list) delete list;

	return storeResult;
}
//_______________________________________________________________

UInt_t IlcGRPPreprocessor::ProcessDqmFxs()
{
	//
	// Processing DQM fxs information
	//

	// TriggerClassesAndHistosToClone
	TList* list = GetFileSources(kDQM, "TriggerClassesAndHistosToClone");
	Bool_t storeResult = kTRUE;
	Bool_t storeResultQAThr = kTRUE;
	if (list !=0x0 && list->GetEntries()!=0){
		IlcInfo("The following sources produced files with the id TriggerClassesAndHistosToClone for GRP");
		list->Print();
		for (Int_t jj=0;jj<list->GetEntries();jj++){
			TObjString * str = dynamic_cast<TObjString*> (list->At(jj)); 
			if (!str){
				IlcError(Form("Expecting a TObjString in the list for the %d-th source, but something else was found.",jj));
				continue;
			}
			IlcInfo(Form("found source %s", str->String().Data()));
			TString fileNameRun = GetFile(kDQM, "TriggerClassesAndHistosToClone", str->GetName());
			if (fileNameRun.Length()>0){
				IlcInfo(Form("Got the file %s", fileNameRun.Data()));
				TFile dqmFile(fileNameRun.Data(),"READ");
				if (dqmFile.IsOpen()) {
					dqmFile.ls();
				}
				else {
					IlcError("Can't open file");
					storeResult = kFALSE;
				}
			}
			else{
				IlcWarning("No file found for current source for DQM TriggerClassesAndHistosToClone");
			}
		}
	}
	else {
		IlcWarning("No list found for DQM TriggerClassesAndHistosToClone");
	}
	
	if (list) delete list;

	// QAThresholds
	TObjArray* qaThrArray = new TObjArray();
	for (Int_t idet = 0; idet < IlcDAQ::kNDetectors; idet++){
		TString detName = IlcDAQ::OnlineName(idet);
		if (detName == "TRI" || detName == "HLT" || detName == "TST") continue;   // skipping TRI, HLT, TST since they do not produce QAThresholds
		IlcDebug(2, Form("Processing QAThreshold for detector %s",detName.Data())); 
		TList* listQAThr = GetForeignFileSources(detName.Data(), kDQM, "QAThresholds");
		if (listQAThr !=0x0){
			if (listQAThr->GetEntries() > 1){
				IlcError(Form("More than one sources found for QAThresholds from detector %s, skipping",detName.Data()));
				continue;
			}
			else if (listQAThr->GetEntries()==1){
				IlcInfo(Form("The following source produced files with the id QAThresholds for GRP, coming from detector %s:",detName.Data()));
				listQAThr->Print();
				TObjString * str = dynamic_cast<TObjString*> (listQAThr->At(0)); 
				if (!str){
					IlcError(Form("Expecting a TObjString in the list for detector %s, but something else was found.",detName.Data()));
					delete listQAThr;
					continue;
				}
				IlcInfo(Form("found source %s", str->String().Data()));
				TString fileNameRun = GetForeignFile(detName.Data(), kDQM, "QAThresholds", str->GetName());
				if (fileNameRun.Length()>0){
					IlcInfo(Form("Got the file %s", fileNameRun.Data()));
					TFile dqmFile(fileNameRun.Data(),"READ");
					if (dqmFile.IsOpen()) {
						IlcQAThresholds* qaThr = dynamic_cast<IlcQAThresholds*>(dqmFile.Get(detName.Data()));
						if (qaThr){
							Int_t qaThrId = qaThr->GetDetectorId();
							if (qaThrId != idet){
								IlcError(Form("Expecting QA threshold for detector %s, but found that for detector %s, skipping",detName.Data(), IlcDAQ::OnlineName(qaThrId)));
								delete listQAThr;
								continue;
							}
							else{
								qaThrArray->AddAtAndExpand(qaThr, qaThrId);
								delete listQAThr;
							}
						}
						else {
							IlcError(Form("No QAThresholds object found in the file for detector %s, skipping",detName.Data()));
							delete listQAThr;
							continue;
						}
					}			      
					else {
						IlcError(Form("Can't open QAThreshold file for detector %s, skipping",detName.Data()));
						delete listQAThr;
						continue;					
					}
				}
				else{
					IlcWarning(Form("No file found for DQM QAThreshold for detector %s, skipping",detName.Data()));
					delete listQAThr;
					continue;
				}
			}
			else {
				IlcError(Form("No sources found for QAThresholds from detector %s, skipping",detName.Data()));
				delete listQAThr;
				continue;
			}
		}
		else {
			IlcWarning(Form("No list found for DQM QAThreshold for detector %s, skipping",detName.Data()));
			continue;
		}
	}
	if (qaThrArray->GetEntries() > 0){
		IlcCDBMetaData md;
		md.SetResponsible("Barthélémy von Haller");
		md.SetComment("QA Threshold TObjArray");					
		storeResultQAThr = Store("Calib", "QAThresholds", qaThrArray, &md, 0, kTRUE); 
	}
	else{
		Printf("No valid QAThresholds entries found, storing nothing in the OCDB");
	}

	//	return storeResult;
	return kTRUE;  // temporary!!
}


//_______________________________________________________________

Int_t IlcGRPPreprocessor::ProcessDaqLB(IlcGRPObject* grpObj)
{
	//Getting the DAQ lb information
	
	time_t timeStart = (time_t)(((TString)GetRunParameter("DAQ_time_start")).Atoi());
	time_t timeEnd = (time_t)(((TString)GetRunParameter("DAQ_time_end")).Atoi());
	Float_t beamEnergy = (Float_t)(((TString)GetRunParameter("beamEnergy")).Atof());
	TString beamType = (TString)GetRunParameter("beamType");
	Char_t numberOfDetectors = (Char_t)(((TString)GetRunParameter("numberOfDetectors")).Atoi());
	UInt_t  detectorMask = (UInt_t)(((TString)GetRunParameter("detectorMask")).Atoi());
	TString lhcPeriod = (TString)GetRunParameter("LHCperiod");
	TString runType = (TString)GetRunType();

	if (timeEnd >= 2.E9) IlcFatal("ILC run finshed later than Wed May 18 2033, 03:33:20 GMT, maximum time allowed for LHC data --> fix the GRP preprocessor!!!");

	UInt_t nparameter = 0;
	if (timeStart != 0){
		grpObj->SetTimeStart(timeStart);
		Log(Form("Start time for run %d: %d",fRun, (Int_t)timeStart));
		nparameter++;
	} 
	else {
		Log(Form("Start time not put in logbook, setting to invalid in GRP entry, and causing an error!"));
	}

	if (timeEnd != 0){
		grpObj->SetTimeEnd(timeEnd);
		Log(Form("End time for run %d: %i",fRun, (Int_t)timeEnd));
		nparameter++;
	} 
	else {
		Log(Form("End time not put in logbook, setting to invalid in GRP entry, and causing an error!"));
	}

	if (beamEnergy != 0){
		Log(Form("Beam Energy for run %d: %f (NOT USING IT TO FILL THE GRP OBJECT, taking it from the LHC file)",fRun, beamEnergy));
	} 
	else {
		Log(Form("Beam Energy not put in logbook, but not using it anyway for the GRP object (taking it from the LHC file)"));
	}

		
	if (beamType.Length() != 0){
		Log(Form("Beam Type for run %d: %s (NOT USING IT TO FILL THE GRP OBJECT, taking it from the LHC file)",fRun, beamType.Data()));
	} 
	else {
		Log(Form("Beam Type not put in logbook, but not using it anyway for the GRP entry (taking it from the LHC file)"));
	}
		
	if (numberOfDetectors != 0){
		grpObj->SetNumberOfDetectors(numberOfDetectors);
		Log(Form("Number Of Detectors for run %d: %d",fRun, (Int_t)numberOfDetectors));
		nparameter++;
	} 
	else {
		Log(Form("Number Of Detectors not put in logbook, setting to invalid in GRP entry, and causing an error!"));
	}

	if (detectorMask != 0){
		grpObj->SetDetectorMask(detectorMask);
		Log(Form("Detector Mask for run %d: %d",fRun, detectorMask));
		nparameter++;
	} 
	else {
		Log(Form("Detector Mask not put in logbook, setting to invalid in GRP entry, and causing an error!"));
	}

	if (lhcPeriod.Length() != 0) {
		grpObj->SetLHCPeriod(lhcPeriod);
		Log(Form("LHC period (DAQ) for run %d: %s",fRun, lhcPeriod.Data()));
		nparameter++;
	} 
	else {
		Log(Form("LHCperiod not put in logbook, setting to invalid in GRP entry, and causing an error!"));
	}
	if (runType.Length() != 0) {
		grpObj->SetRunType(runType);
		Log(Form("Run Type (DAQ) for run %d: %s",fRun, runType.Data()));
		nparameter++;
	} 
	else {
		Log(Form("Run Type not put in logbook, setting to invalid in GRP entry, and causing an error!"));
	}

	return nparameter;
}
//_______________________________________________________________

UInt_t IlcGRPPreprocessor::ProcessDaqFxs()
{
	//======DAQ FXS======//
	
	IlcRawEventHeaderV3_9::Class()->IgnoreTObjectStreamer(); // to avoid trying reading TObject store in IlcRawEventHeaderV3_9 - temporary fix 
	IlcRawEventHeaderV3_11::Class()->IgnoreTObjectStreamer(); // to avoid trying reading TObject store in IlcRawEventHeaderV3_11 - temporary fix 
	IlcRawEventHeaderV3_12::Class()->IgnoreTObjectStreamer(); // to avoid trying reading TObject store in IlcRawEventHeaderV3_12 - temporary fix 
	IlcRawEventHeaderV3_13::Class()->IgnoreTObjectStreamer(); // to avoid trying reading TObject store in IlcRawEventHeaderV3_13 - temporary fix 
	Log("Processing DAQ FXS");
	TList* list = GetFileSources(kDAQ);  	
	if (!list) {
		Log("No raw data tag list: connection problems with DAQ FXS logbook!");
		return 1;
	}
	
	if (list->GetEntries() == 0) {
		Log("no raw data tags in this run: nothing to merge!");
		delete  list; list=0;
		return 0;
	}
	
	TChain *fRawTagChain = new TChain("T");
	Int_t nFiles=0;
	TIterator* iter = list->MakeIterator();
	TObject* obj = 0;
	while ((obj = iter->Next())) {
		TObjString* objStr = dynamic_cast<TObjString*> (obj);
		if (objStr) {
			Log(Form("Found source %s", objStr->String().Data()));
			TList* list2 = GetFileIDs(kDAQ, objStr->String());
			if (!list2) {
				Log("No list with ids from DAQ was found: connection problems with DAQ FXS logbook!");
				delete fRawTagChain; fRawTagChain=0;
				return 1;
			}
			Log(Form("Number of ids: %d",list2->GetEntries()));
			for(Int_t i = 0; i < list2->GetEntries(); i++) {
				TObjString *idStr = (TObjString *)list2->At(i);
				if (idStr->String().CompareTo("QAThreshold") == 0 || idStr->String().CompareTo("TriggerClassesAndHistosToClone") == 0) {
					Log(Form("Skipping file with Id %s",idStr->String().Data()));
					continue; 
				}
				TString fileName = GetFile(kDAQ,idStr->String().Data(),objStr->String().Data());
				if (fileName.Length() > 0) {
					Log(Form("Adding file in the chain: %s",fileName.Data()));
					fRawTagChain->Add(fileName.Data());
					nFiles++;
				} else {
					Log(Form("Could not retrieve file with id %s from source %s: "
						 "connection problems with DAQ FXS!",
						 idStr->String().Data(), objStr->String().Data()));
					delete list; list=0;
					delete list2; list2=0;
					delete fRawTagChain; fRawTagChain=0;
					return 2;
				}
			}
			delete list2;
		}
	}

	if (nFiles == 0){
		Log("no raw data tags in this run: it could be that one or more files were found in the DAQ FXS, but they were ignored, since not interesting for the raw data tag: nothing to merge!");
		delete iter;
		iter = 0;
		delete list;
		list = 0;
		delete fRawTagChain; 
		fRawTagChain=0;
		return 0;
	}
	
	TString fRawDataFileName = "GRP_Merged.tag.root";
	Log(Form("Merging %d raw data tags into file: %s", nFiles, fRawDataFileName.Data()));
	
	if (fRawTagChain->Merge(fRawDataFileName) < 1 ) {
		Log(Form("Error merging %d raw data files!!!",nFiles));
		delete iter;
		iter = 0;
		delete list;
		list = 0;
		delete fRawTagChain; 
		fRawTagChain=0;
		return 3;
	}
		
	TString outputfile = Form("Run%d.Merged.RAW.tag.root", fRun);
	Bool_t result = StoreRunMetadataFile(fRawDataFileName.Data(),outputfile.Data());
	
	if (!result) {
		Log("Problem storing raw data tags in local file!!!");
	} else {
		Log("Raw data tags merged successfully!!");
	}
	
	delete iter;
	iter = 0;
	delete list;
	list = 0;
	delete fRawTagChain; fRawTagChain=0;
	
	if (result == kFALSE) {
		return 4;
	}
	
	return 0;
	
}

//_______________________________________________________________
UInt_t IlcGRPPreprocessor::ProcessDcsFxs(TString partition, TString detector)
{

	// processing the info
	// stored in the DCS FXS
	// coming from the trigger

	// Get the CTP counters information
	//              +
	// Get the CTP aliases information

	if (partition.IsNull() && !detector.IsNull()){ // standalone partition
		Log("STANDALONE partition for current run, using Trigger Scalers and Trigger Aliases dummy values");

		IlcCDBEntry *cdbEntryScalers = GetFromOCDB("CTP","DummyScalers");
		if (!cdbEntryScalers) {
			Log(Form("No dummy CTP scalers entry found, going into error..."));
			return 1;
		}
		else{
			IlcTriggerRunScalers *scalers = (IlcTriggerRunScalers*)cdbEntryScalers->GetObject();
			if (!scalers){
				Log(Form("CTP dummy scalers not found in OCDB entry, going into error..."));
				return 1;
			}
			else {
				IlcCDBMetaData metaData;
				metaData.SetResponsible("Roman Lietava");
				metaData.SetComment("CTP scalers from dummy entry in OCDB");
				if (!Store("CTP","Scalers", scalers, &metaData, 0, 0)) {
					Log("Unable to store the dummy CTP scalers object to OCDB!");
					delete scalers;
					return 1;
				}
			}
		}

		IlcCDBEntry *cdbEntryAliases = GetFromOCDB("CTP","DummyAliases");
		if (!cdbEntryAliases) {
			Log(Form("No dummy CTP aliases entry found, going into error..."));
			return 2;
		}
		else{
			THashList *aliases = dynamic_cast<THashList*>(cdbEntryAliases->GetObject());
			if (!aliases){
				Log(Form("CTP dummy aliases not found in OCDB entry, going into error..."));
				return 2;
			}
			else {
				IlcCDBMetaData metaData;
				metaData.SetResponsible("Evgeny Kryshen");
				metaData.SetComment("CTP mapping of trigger classes to trigger aliases");
				if (!Store("CTP","Aliases", aliases, &metaData, 0, 0)) {
					Log("Unable to store the dummy CTP aliases object to OCDB!");
					delete aliases;					
					return 2;
				}
			}
		}
	}

	else if (!partition.IsNull() && detector.IsNull()){ // global partition
		Log("GLOBAL partition for current run, using CTP scalers from DCS FXS");
		TString countersfile = GetFile(kDCS, "CTP_xcounters","");
		if (countersfile.IsNull()) {
			Log("No CTP counters files has been found: empty source!");
			return 1;
		}
		else {
			Log(Form("File with Id CTP_xcounters found in DCS FXS! Copied to %s",countersfile.Data()));
			IlcTriggerRunScalers *scalers = IlcTriggerRunScalers::ReadScalers(countersfile);
			if (!scalers) {
				Log("Bad CTP counters file! The corresponding CDB entry will not be filled!");
				return 1;
			}
			else {
				IlcCDBMetaData metaData;
				metaData.SetBeamPeriod(0);
				metaData.SetResponsible("Roman Lietava");
				metaData.SetComment("CTP scalers");
				if (!Store("CTP","Scalers", scalers, &metaData, 0, 0)) {
					Log("Unable to store the CTP scalers object to OCDB!");
					delete scalers;					
					return 1;
				}
			}
			delete scalers;
		}



		TString aliasesFile = GetFile(kDCS, "CTP_aliases","");
		if (aliasesFile.IsNull()) {
			Log("No CTP aliases files has been found: empty source!");
			return 2;
		}
		else {
			Log(Form("File with Id CTP_aliases found in DCS FXS! Copied to %s",aliasesFile.Data()));
			// We build the THashList of TNamed("triggerclass","comma_separated_list_of_corresponding_aliases")
			THashList* trClasses2Aliases = ProcessAliases(aliasesFile);
			if (!trClasses2Aliases) {
				Log("Bad CTP aliases file! The corresponding CDB entry will not be filled!");
				return 2;
			}
			else {
				IlcCDBMetaData metaData;
				metaData.SetBeamPeriod(0);
				metaData.SetResponsible("Evgeny Kryshen");
				metaData.SetComment("CTP mapping of trigger classes to trigger aliases");
				if (!Store("CTP","Aliases", trClasses2Aliases, &metaData, 0, 0)) {
					Log("Unable to store the CTP aliases object to OCDB!");
					delete trClasses2Aliases;					
					return 2;
				}
			}
			delete trClasses2Aliases;
		}
	}
	

	else{	
		Log(Form("Incorrect field in DAQ logbook for partition = %s and detector = %s, going into error...",partition.Data(),detector.Data()));
		return 3;
	}

	return 0;

}

//_______________________________________________________________
THashList* IlcGRPPreprocessor::ProcessAliases(const char* aliasesFile)
{

	//
	// build the THashList of triggerclasses-to-triggeraliases from text file  
	// each line of the file is supposed to be a string composed by
	// triggerclass+spaces+commaseparatedlistofcorrespondingaliases\n
	// it will add a TNamed("triggerclass","commaseparatedlistofcorrespondingaliases")
	// to the hashlist
	//

	if (gSystem->AccessPathName(aliasesFile)) {
		Printf("file (%s) not found", aliasesFile);
		return 0;
	}

	ifstream *file = new ifstream(aliasesFile);
	if (!*file) {
		Printf("Error opening file (%s) !",aliasesFile);
		file->close();
		delete file;
		return 0;
	}

	THashList *hList = new THashList(10);
	hList->SetName("List of trigger classes to trigger aliases strings");

	TString strLine;
	while (strLine.ReadLine(*file)) {

	    // safety for null lines, tabs instead of whitespaces, trailing carriage return, leading or trailing spaces/tabs
		if (strLine.IsNull()) continue;
		strLine.ReplaceAll('\t',' ');
		strLine.Remove(TString::kLeading,' ');
		strLine.Remove(TString::kTrailing,'\r');
		strLine.Remove(TString::kTrailing,' ');

		TObjArray* arr = strLine.Tokenize(' ');
		if(arr->GetEntries() != 2){
			Printf("The line:\n%s\nunexpectedly contains %d tokens, instead of two.",strLine.Data(),arr->GetEntries());
			return 0;
		}
		TObjString *osTC = (TObjString*) arr->At(0);
		TObjString *osTAlist = (TObjString*) arr->At(1);
		TNamed *ctoa = new TNamed(osTC->GetName(),osTAlist->GetName());
		hList->Add(ctoa);
	}

	file->close();
	delete file;

	return hList;
}

//_______________________________________________________________
Int_t IlcGRPPreprocessor::ProcessDcsDPs(TMap* valueMap, IlcGRPObject* grpObj)
{

	//
	// processing DCS DPs
	//

	Int_t entries = 0;  // counting the entries that are in the DCS DB, not taking care whether they have values or not
	Int_t nL3Entries = 0;
	Int_t nDipoleEntries = 0;
	Int_t nEnvEntries = 0;
	Int_t nHallProbesEntries = 0;
	nL3Entries = ProcessL3DPs(valueMap, grpObj);
	nDipoleEntries = ProcessDipoleDPs(valueMap, grpObj);
	nEnvEntries = ProcessEnvDPs(valueMap, grpObj);
	nHallProbesEntries = ProcessHPDPs(valueMap, grpObj);
	grpObj->SetPolarityConventionLHC();  // after the dipole cables swap we comply with LHC convention
	Log(Form("L3Entries = %d, nDipoleEntries =%d, nEnvEntries = %d, nHallProbesEntries = %d", nL3Entries, nDipoleEntries, nEnvEntries, nHallProbesEntries));
	entries = nL3Entries + nDipoleEntries + nEnvEntries + nHallProbesEntries;
	return entries;

}

//_______________________________________________________________

Int_t IlcGRPPreprocessor::ProcessL3DPs(const TMap* valueMap, IlcGRPObject* grpObj)
{

	// processing DPs
	// related to 
	// L3 info

	Int_t nL3Entries = 0;

	TObjArray *array = 0x0;
	Int_t indexDP = -1;
	Bool_t isZero = kTRUE; // flag to monitor L3Current. If set to true, the magnet is OFF, and the polarity can change

	IlcInfo(Form("==========L3Current==========="));
	Bool_t outOfRange = kFALSE;  // flag to monitor if any value collected by DCS is out of range
	indexDP = kL3Current;
	array = (TObjArray *)valueMap->GetValue(fgkDCSDataPoints[indexDP]);
	if(!array) {
		Log(Form("%s not found in the map!!!",fgkDCSDataPoints[indexDP]));
	} 
	else {
		if (array->GetEntries() == 0){
			IlcError(Form("No entries found in array! setting %s to invalid...",fgkDCSDataPoints[indexDP]));
		}
		else {
			Float_t *floatDCS = ProcessFloatAllMagnet(array, indexDP, isZero);
			if (floatDCS != NULL){
				grpObj->SetL3Current(floatDCS);
			}
			else{
				outOfRange = kTRUE;
			}	
			if (floatDCS){
				delete[] floatDCS;
				floatDCS = 0x0;
			}
		}
		if (!outOfRange) {
			nL3Entries++;
			ffailedDPs->RemoveAt(indexDP);
		}
	}

	if (array) array = 0x0;

	IlcInfo(Form("==========L3Polarity==========="));
	indexDP = kL3Polarity;
	array = (TObjArray *)valueMap->GetValue(fgkDCSDataPoints[indexDP]);
	if(!array) {
		Log(Form("%s not found in the map!!!",fgkDCSDataPoints[indexDP]));
	} 
	else {
		if (array->GetEntries() == 0){
			IlcError(Form("No entries found in array! setting %s Polarity to invalid...",fgkDCSDataPoints[indexDP]));
		}
		else {
			Bool_t change = kFALSE;
			Char_t charDCS = ProcessBool(array,change);
			if (change == kFALSE){
				grpObj->SetL3Polarity(charDCS);
				IlcInfo(Form("%s set to %d",fgkDCSDataPoints[indexDP],(Int_t)(grpObj->GetL3Polarity())));
				ffailedDPs->RemoveAt(indexDP);
				nL3Entries++;
			}
			else if (isZero){
				IlcInfo(Form("%s set to invalid, but magnet was OFF (according to the current), DP not considered wrong",fgkDCSDataPoints[indexDP]));
				ffailedDPs->RemoveAt(indexDP);
				nL3Entries++;
			}
			else {
				IlcError(Form("%s value changed within the run, while the magnet was ON (according to the current), setting it to invalid and considering the DP as wrong",fgkDCSDataPoints[indexDP]));
			}
		}
	}

	return nL3Entries;

}
//_______________________________________________________________

Int_t IlcGRPPreprocessor::ProcessDipoleDPs(const TMap* valueMap, IlcGRPObject* grpObj)
{
	// processing DPs
	// related to 
	// the Dipole info

	Int_t nDipoleEntries = 0;
	TObjArray *array = 0x0;
	Int_t indexDP = -1;
	Bool_t isZero = kTRUE; // flag to monitor L3Current. If set to true, the magnet is OFF, and the polarity can change

	IlcInfo(Form("==========DipoleCurrent==========="));
	Bool_t outOfRange = kFALSE;  // flag to monitor if any value collected by DCS is out of range
	indexDP = kDipoleCurrent;
	array = (TObjArray *)valueMap->GetValue(fgkDCSDataPoints[indexDP]);
	if(!array) {
		Log(Form("%s not found in the map!!!",fgkDCSDataPoints[indexDP]));
	} 
	else {
		if (array->GetEntries() == 0){
			IlcError(Form("No entries found in array! setting %s to invalid...",fgkDCSDataPoints[indexDP]));
		}
		else {
			Float_t *floatDCS = ProcessFloatAllMagnet(array, indexDP, isZero);
			if (floatDCS != NULL){
				grpObj->SetDipoleCurrent(floatDCS);
			} 
			else{
				outOfRange=kTRUE;
			}
			if (floatDCS){
				delete[] floatDCS;
				floatDCS = 0x0;
			}
		}
		if (!outOfRange) {
			nDipoleEntries++;
			ffailedDPs->RemoveAt(indexDP);
		}
	}

	if (array) array = 0x0;

	IlcInfo(Form("==========DipolePolarity==========="));
	indexDP = kDipolePolarity;
	array = (TObjArray *)valueMap->GetValue(fgkDCSDataPoints[indexDP]);
	if(!array) {
		Log(Form("%s not found in the map!!!",fgkDCSDataPoints[indexDP]));
	} 
	else {
		if (array->GetEntries() == 0){
			IlcError(Form("No entries found in array! setting %s to invalid...",fgkDCSDataPoints[indexDP]));
		}
		else {
			Bool_t change = kFALSE;
			Char_t charDCS = ProcessBool(array,change);
			if (!change){
				grpObj->SetDipolePolarity(charDCS);
				IlcInfo(Form("%s set to %d",fgkDCSDataPoints[indexDP],(Int_t)(grpObj->GetDipolePolarity())));
				ffailedDPs->RemoveAt(indexDP);
				nDipoleEntries++;
			}
			else if (isZero){
				IlcInfo(Form("%s set to invalid, but magnet was OFF (according to the current), DP not considered wrong",fgkDCSDataPoints[indexDP]));
				ffailedDPs->RemoveAt(indexDP);
				nDipoleEntries++;
			}
			else{
				IlcError(Form("%s value changed within the run while the magnet was ON (according to the current), setting it to invalid and considering the DP as wrong", fgkDCSDataPoints[indexDP]));
			}
		}
	}

	return nDipoleEntries;

}
//_______________________________________________________________

Int_t IlcGRPPreprocessor::ProcessEnvDPs(TMap* valueMap, IlcGRPObject* grpObj)
{
	// processing DPs
	// related to 
	// evironment conditions (temperature, pressure) info

	Int_t nEnvEntries = 0;
	TObjArray *array = 0x0;
	Int_t indexDP = -1;

	IlcInfo(Form("==========CavernTemperature==========="));
	Bool_t outOfRange = kFALSE;  // flag to monitor if any value collected by DCS is out of range
	indexDP = kCavernTemperature;
	array = (TObjArray *)valueMap->GetValue(fgkDCSDataPoints[indexDP]);
	if(!array) {
		Log(Form("%s not found in the map!!!",fgkDCSDataPoints[indexDP]));
	} 
	else {
		if (array->GetEntries() == 0){
			IlcError(Form("No entries found in array! setting %s to invalid...",fgkDCSDataPoints[indexDP]));
		}
		else {
			Float_t *floatDCS = ProcessFloatAll(array);
			if (floatDCS != NULL){
				grpObj->SetCavernTemperature(floatDCS);
			}
			else{
				outOfRange = kTRUE;
			}
			if (floatDCS){
				delete[] floatDCS;
				floatDCS = 0x0;
			}
		}
		if (!outOfRange) {
			ffailedDPs->RemoveAt(indexDP);
			nEnvEntries++;
		}
	}

	if (array) array = 0x0;

	IlcInfo(Form("========== AtmosPressures (Cavern + Surface + Cavern2) ==========="));
	IlcDCSSensorArray *dcsSensorArray = GetPressureMap(valueMap);
	//dcsSensorArray->Print();
	if( fPressure->NumFits()<kNumSensors ) {
		Log(Form("Check the pressure sensor values! Not all the %d pressure sensors have been fit",kNumSensors));
	} 
	Log(Form("Number of fits performed = %d",fPressure->NumFits()));

	IlcInfo(Form("==========CavernAtmosPressure==========="));
	indexDP = kCavernAtmosPressure;
	IlcDCSSensor* sensorCavernP2 = dcsSensorArray->GetSensor(fgkDCSDataPoints[indexDP]);
	TGraph* graph = sensorCavernP2->GetGraph();
	IlcDebug(3,Form("index = %d",indexDP));
	IlcDebug(3,Form("name = %s",fgkDCSDataPoints[indexDP]));
	IlcDebug(2,Form("graph = %p",graph));
	IlcDebug(3,Form("sensorCavernP2 = %p", sensorCavernP2));
	if(sensorCavernP2->GetFit() || graph) {
		if (sensorCavernP2->GetFit()){
			Log(Form("Fit for sensor %s found",fgkDCSDataPoints[indexDP]));
		}
		else {
			Log(Form("Fit for sensor %s not found, but the graph is there - NOT going into error",fgkDCSDataPoints[indexDP]));
		}
		grpObj->SetCavernAtmosPressure(sensorCavernP2);
		ffailedDPs->RemoveAt(indexDP);
		nEnvEntries++;
	} 
	//if (sensorP2) delete sensorP2;
	else {
		Log(Form("ERROR!!! Neither graph nor fit found for sensor %s - this will not increase the number of found DCS DPs and will cause an error", fgkDCSDataPoints[indexDP] ));
	}
	
	IlcInfo(Form("==========SurfaceAtmosPressure==========="));
	indexDP = kSurfaceAtmosPressure;
	IlcDCSSensor* sensorP2 = dcsSensorArray->GetSensor(fgkDCSDataPoints[indexDP]);
	graph = sensorP2->GetGraph();
	IlcDebug(3,Form("index = %d",indexDP));
	IlcDebug(3,Form("name = %s",fgkDCSDataPoints[indexDP]));
	IlcDebug(2,Form("graph = %p",graph));	
	IlcDebug(3,Form("sensorP2 = %p", sensorP2));
	if(sensorP2->GetFit() || graph) {
		if (sensorP2->GetFit()){
			Log(Form("Fit for sensor %s found",fgkDCSDataPoints[indexDP]));
		}
		else {
			Log(Form("Fit for sensor %s not found, but the graph is there - NOT going into error",fgkDCSDataPoints[indexDP]));
		}
		grpObj->SetSurfaceAtmosPressure(sensorP2);
		ffailedDPs->RemoveAt(indexDP);
		nEnvEntries++;
	} 
	//if (sensorP2) delete sensorP2;
	else {
		Log(Form("ERROR!!! Neither graph nor fit found for sensor %s - this will not increase the number of found DCS DPs and will cause an error", fgkDCSDataPoints[indexDP] ));
	}

	IlcInfo(Form("==========CavernAtmosPressure2==========="));
	indexDP = kCavernAtmosPressure2;
	IlcDCSSensor* sensorCavernP22 = dcsSensorArray->GetSensor(fgkDCSDataPoints[indexDP]);
	graph = sensorCavernP22->GetGraph();
	IlcDebug(3,Form("index = %d",indexDP));
	IlcDebug(3,Form("name = %s",fgkDCSDataPoints[indexDP]));
	IlcDebug(2,Form("graph = %p",graph));	
	IlcDebug(3,Form("sensorCavernP2_2 = %p", sensorCavernP22));
	if(sensorCavernP22->GetFit() || graph) {
		if (sensorCavernP22->GetFit()){
			Log(Form("Fit for sensor %s found",fgkDCSDataPoints[indexDP]));
		}
		else {
			Log(Form("Fit for sensor %s not found, but the graph is there - NOT going into error",fgkDCSDataPoints[indexDP]));
		}
		grpObj->SetCavernAtmosPressure2(sensorCavernP22);
		ffailedDPs->RemoveAt(indexDP);
		nEnvEntries++;
	} 
	//if (sensorP2) delete sensorP2;
	else {
		Log(Form("ERROR!!! Neither graph nor fit found for sensor %s - this will not increase the number of found DCS DPs and will cause an error", fgkDCSDataPoints[indexDP] ));
	}
	
	
	return nEnvEntries;
}
//_______________________________________________________________

Int_t IlcGRPPreprocessor::ProcessHPDPs(const TMap* valueMap, IlcGRPObject* grpObj)
{
	// processing DPs
	// related to 
	// Hall Probes info

	Int_t nHPEntries = 0;
	TObjArray *array = 0x0;
	Int_t indexDP = -1;
	Bool_t outOfRange; // flag to monitor if any value collected by DCS is out of range

	if (fgknDCSDPHallProbes != IlcGRPObject::GetNumberOfHP()){
		IlcError(Form("Number of Hall probes expected in GRP Preprocessor (i.e. %d) different from number of Hall Probes foreseen in GRP object (i.e. %d). Looping on entries from GRP object anyway.", fgknDCSDPHallProbes, IlcGRPObject::GetNumberOfHP()));
	}
	for (indexDP = 0; indexDP < IlcGRPObject::GetNumberOfHP(); indexDP++){
		outOfRange = kFALSE; // resetting outOfRange flag at each HP
		IlcInfo(Form("==========%s===========",IlcGRPObject::GetHPDP(indexDP)));
		array = (TObjArray *)valueMap->GetValue(IlcGRPObject::GetHPDP(indexDP));
		if(!array) {
			Log(Form("%s not found in the map!!!",IlcGRPObject::GetHPDP(indexDP)));
		} 
		else {
			if (array->GetEntries() == 0){
				IlcError(Form("No entries found in array! setting %s to invalid...",IlcGRPObject::GetHPDP(indexDP)));
			}
			else {
				Float_t *floatDCS = ProcessFloatAll(array);
				if (floatDCS != NULL){
					IlcDebug(2,Form("value[0] = %f, value[1] = %f, value[2] = %f, value[3] = %f, value[4] = %f",floatDCS[0],floatDCS[1],floatDCS[2],floatDCS[3],floatDCS[4])); 
					grpObj->SetHallProbes((IlcGRPObject::DP_HallProbes)indexDP,floatDCS);
					for (Int_t kk = 0 ; kk< 5; kk++){
						IlcDebug(2,Form("HallProbe[%d][%d]=%f",indexDP,kk,grpObj->GetHallProbes((IlcGRPObject::DP_HallProbes)indexDP,(IlcGRPObject::Stats)kk)));
					}
				}
				else{
					outOfRange = kTRUE;
				}
				if (floatDCS){
					delete[] floatDCS;
					floatDCS = 0x0;
				}
			}
			if (!outOfRange) {
				ffailedDPs->RemoveAt(indexDP + fgkDCSDPHallTopShift);  // 7 = shift in the complete list of DPs to get to the Hall Probes
				nHPEntries++;
			}
		}
	}
		
	Log(Form("Hall Probes = %d ", nHPEntries));
	return nHPEntries;
}

//_________________________________________________________________________

IlcSplineFit* IlcGRPPreprocessor::GetSplineFit(const TObjArray *array, const TString& stringID){


	// 
	// returning Spline Fit 
	// 

	Int_t entriesarray = array->GetEntries();
	Float_t* value = new Float_t[entriesarray];
	Float_t* time = new Float_t[entriesarray];
	IlcDCSValue* v = 0x0;
	for (Int_t iarray = 0; iarray < entriesarray; iarray++){
		v = (IlcDCSValue*)array->At(iarray);
		value[iarray] = v->GetFloat();
		time[iarray] = v->GetTimeStamp();
		IlcDebug(2,Form("iarray = %d, value = %f, time = %f",iarray,value[iarray],time[iarray]));
	}
	TGraph* gr = new TGraph(entriesarray,value,time);
	if (!gr ) {
		IlcWarning(Form("%s: no input graph to compute SplineFit",stringID.Data()));
		return NULL;
	}
	IlcSplineFit *fit = new IlcSplineFit();
	fit->SetMinPoints(10);
	fit->InitKnots(gr,10,10,0.0);
	fit->SplineFit(2);
	fit->Cleanup();
	return fit;
}

//_________________________________________________________________________

TString IlcGRPPreprocessor::ProcessChar(const TObjArray *array)
{

	// 
	// processing char
	//

	TString aDCSString="";
	
	IlcDCSValue *v = 0x0;
	for(Int_t iCount = 0; iCount < array->GetEntries(); iCount++) {
		v = (IlcDCSValue *)array->At(iCount);
		if (((Int_t)(v->GetTimeStamp()) < (Int_t)GetStartTimeDCSQuery()) || ((Int_t)(v->GetTimeStamp()) > (Int_t)GetEndTimeDCSQuery())) {
			IlcError(Form("DCS values for the parameter outside the queried interval"));
			continue;
		}
		if (iCount > 0) {
			if (aDCSString != v->GetChar())
			IlcError(Form("DCS values for the parameter changed from %s to %c within the queried interval", aDCSString.Data(), (Char_t)v->GetChar()));
		}
		aDCSString = (TString)v->GetChar();  // keeping always last value in the array
	}
	return aDCSString;
}

//__________________________________________________________________________________________________________________

Float_t* IlcGRPPreprocessor::ProcessFloatAll(const TObjArray* array)
{
	// 
	// processing Float values using Mean, Median, Standard Deviation wrt Mean, Standar Deviation wrt Median 
	//
	// parameters[0] = mean
	// parameters[1] = truncated mean (calculated excluding points outside +/- 3RMS from mean
	// parameters[2] = median
	// parameters[3] = standard deviation wrt mean
	// parameters[4] = standard deviation wrt median
	//

	TString timeStartString = (TString)GetRunParameter("DAQ_time_start");
	TString timeEndString = (TString)GetRunParameter("DAQ_time_end");
	if (timeStartString.IsNull() || timeStartString.IsNull()){
		if (timeStartString.IsNull()){ 
			IlcError("DAQ_time_start not set in logbook! Setting statistical values for current DP to invalid");
		}
		else if (timeStartString.IsNull()){
			IlcError("DAQ_time_end not set in logbook! Setting statistical values for current DP to invalid");
		}
		fdaqStartEndTimeOk = kFALSE;
		return 0;
	}  

	Int_t timeStart = (Int_t)(timeStartString.Atoi());
	Int_t timeEnd = (Int_t)(timeEndString.Atoi());
	Float_t* parameters = new Float_t[5];
	Int_t iCounts = 0;
	Int_t iCountsRun = 0;
	Int_t nCounts = array->GetEntries();
	Float_t valueBeforeSOR = 0;
	Float_t valueAfterEOR = 0;
	Int_t timestampBeforeSOR = -1;
	Int_t timestampAfterEOR = -1;
	Int_t ientrySOR = -1;
	Int_t ientryEOR = -1;
	Float_t* arrayValues = 0x0; 
	Double_t* arrayWeights = 0x0; 
	Bool_t truncMeanFlag = kTRUE;  // flag to indicate whether Truncated Mean should be calculated or not
	Bool_t sdFlag = kTRUE;  // flag to indicate whether SD (wrt Mean/Median) should be calculated or not

	for(Int_t i = 0; i < nCounts; i++) {
		IlcDCSValue *v = (IlcDCSValue *)array->At(i);
		if ((v->GetFloat() <= fminFloat) || (v->GetFloat() >= fmaxFloat)) {
			IlcError(Form("Error! Float value found in DCS map at %d-th entry is OUT OF RANGE: value = %6.5e",i,v->GetFloat()));
			if (v->GetFloat() < fminFloat) IlcInfo(Form("The value is smaller than %6.5e",fminFloat));
			if (v->GetFloat() > fmaxFloat) IlcInfo(Form("The value is greater than %6.5e",fmaxFloat));
			delete [] parameters;
			return NULL;
		}
		if(((Int_t)(v->GetTimeStamp()) >= (Int_t)GetStartTimeDCSQuery()) &&((Int_t)(v->GetTimeStamp()) <= (Int_t)GetEndTimeDCSQuery())) {
			IlcDebug(2,Form("%d-th entry = %f at timestamp %i",i,v->GetFloat(),v->GetTimeStamp()));
			iCounts += 1;
			// look for the last value before SOR and the first value before EOR
			if (((Int_t)(v->GetTimeStamp()) >= (Int_t)GetStartTimeDCSQuery()) && (Int_t)(v->GetTimeStamp()) < timeStart) {
				timestampBeforeSOR = (Int_t)(v->GetTimeStamp());
				IlcDebug(2,Form("timestamp of last value before SOR = %d, with DAQ_time_start = %d",timestampBeforeSOR,timeStart));
				valueBeforeSOR = v->GetFloat();
			}
			else if ((Int_t)(v->GetTimeStamp()) <= (Int_t)GetEndTimeDCSQuery() && (Int_t)(v->GetTimeStamp()) > timeEnd && timestampAfterEOR == -1){
				timestampAfterEOR = (Int_t)(v->GetTimeStamp());
				valueAfterEOR = v->GetFloat();
				IlcDebug(2,Form("timestamp of first value after EOR = %d, with DAQ_time_end = %d",timestampAfterEOR,timeEnd));
			}
			// check if there are DPs between DAQ_time_start and DAQ_time_end
			if(((Int_t)(v->GetTimeStamp()) >= timeStart) &&((Int_t)(v->GetTimeStamp()) <= timeEnd)) {
				if (ientrySOR == -1) ientrySOR = i;  // first entry after SOR
				if (ientryEOR < i) ientryEOR = i;  // last entry before EOR
				IlcDebug(2,Form("entry between SOR and EOR"));
				iCountsRun += 1;
			}
		}
		else {
			IlcError(Form("DCS values for the parameter outside the queried interval: timestamp = %d",v->GetTimeStamp()));
		}
	}

	if (timestampBeforeSOR == -1){
		IlcWarning("No value found before SOR");
	}
	if (timestampAfterEOR == -1){
		IlcWarning("No value found after EOR");
	}

	IlcDebug(2,Form("Number of valid entries (within DCS query interval) = %i, from a total amount of %i entries",iCounts,nCounts));
	IlcDebug(2,Form("Last value before DAQ_time_start (SOR) = %f at timestamp = %d",valueBeforeSOR,timestampBeforeSOR));
	IlcDebug(2,Form("First value after DAQ_time_end (EOR)   = %f at timestamp = %d",valueAfterEOR,timestampAfterEOR));
	IlcInfo(Form("Found %d entries between DAQ_time_start (SOR) and DAQ_time_end (EOR)",iCountsRun));
	IlcDebug(2,Form("Index of first entry after DAQ_time_start (SOR) = %d ",ientrySOR));
	IlcDebug(2,Form("Index of first entry before DAQ_time_end (EOR) = %d ",ientryEOR));

	Int_t nentriesUsed = 0;
	if (iCountsRun > 1){
		IlcInfo("Using entries between DAQ_time_start (SOR) and DAQ_time_end (EOR)");
		IlcDebug(2,"Calculating (weighted) Mean and Median");
		arrayValues = new Float_t[iCountsRun]; 
		arrayWeights = new Double_t[iCountsRun]; 
		nentriesUsed = iCountsRun;
		for (Int_t i = ientrySOR; i <= ientryEOR; i++){
			IlcDCSValue *v = (IlcDCSValue *)array->At(i);
			Int_t timestamp2 = 0;
			if (i < ientryEOR){
				IlcDCSValue *v1 = (IlcDCSValue *)array->At(i+1);
				timestamp2 = (Int_t)v1->GetTimeStamp();
			}
			else {
				timestamp2 = timeEnd+1;
			}
			arrayWeights[i-ientrySOR] = (Double_t)(timestamp2 - (Int_t)v->GetTimeStamp());
			arrayValues[i-ientrySOR] = v->GetFloat();
		}
		parameters[0] = TMath::Mean(iCountsRun,arrayValues,arrayWeights);
		parameters[2] = TMath::Median(iCountsRun,arrayValues,arrayWeights);
	}
	else if (iCountsRun == 1){
		IlcDCSValue* v = (IlcDCSValue *)array->At(ientrySOR);
		nentriesUsed = 2;
		if (timestampBeforeSOR != -1 && timestampBeforeSOR != (Int_t)v->GetTimeStamp()){
			IlcWarning("Using single entry between DAQ_time_start (SOR) and DAQ_time_end (EOR) and last entry before SOR. Truncated mean won't be calculated.");
			arrayValues = new Float_t[2];
			arrayWeights = new Double_t[2];
			arrayValues[0] = valueBeforeSOR;
			arrayWeights[0] = (Double_t)((Int_t)v->GetTimeStamp()-timestampBeforeSOR);
			arrayValues[1] = v->GetFloat();
			arrayWeights[1] = (Double_t)(timeEnd+1-(Int_t)v->GetTimeStamp());
			IlcDebug(2, Form("value0 = %f, with weight = %f",arrayValues[0],arrayWeights[0])); 
			IlcDebug(2, Form("value1 = %f, with weight = %f",arrayValues[1],arrayWeights[1])); 
			parameters[0] = TMath::Mean(2,arrayValues,arrayWeights);
			parameters[2] = TMath::Median(2,arrayValues,arrayWeights);
			truncMeanFlag = kFALSE;
		}
		else{
			IlcError("Cannot calculate mean, truncated mean, median, SD wrt mean, SD wrt median for current DP - only one value collected during the run, but no value before with which to calculate the statistical quantities");
			parameters[0] = IlcGRPObject::GetInvalidFloat();
			parameters[1] = IlcGRPObject::GetInvalidFloat();
			parameters[2] = IlcGRPObject::GetInvalidFloat();
			parameters[3] = IlcGRPObject::GetInvalidFloat();
			parameters[4] = IlcGRPObject::GetInvalidFloat();
			return parameters;
		}
	}
	else { // iCountsRun == 0, using only the point immediately before SOR
		if (timestampBeforeSOR == -1){
			IlcError("Cannot set mean, truncated mean, median, SD wrt mean, SD wrt median for current DP - no points during the run collected, and point before SOR missing");
			parameters[0] = IlcGRPObject::GetInvalidFloat();
			parameters[1] = IlcGRPObject::GetInvalidFloat();
			parameters[2] = IlcGRPObject::GetInvalidFloat();
			parameters[3] = IlcGRPObject::GetInvalidFloat();
			parameters[4] = IlcGRPObject::GetInvalidFloat();
			return parameters;
		}
		else {
			IlcWarning("Using only last entry before SOR. Truncated mean and Standard deviations (wrt mean/median) won't be calculated.");
			IlcDebug(2,Form("value = %f",valueBeforeSOR)); 
			parameters[0] = valueBeforeSOR;
			parameters[2] = valueBeforeSOR;
			truncMeanFlag = kFALSE;
			sdFlag = kFALSE;
		}
	}

	Float_t temp = 0;
	Float_t temp1 = 0;
	Float_t sumweights = 0; 
	Int_t entriesTruncMean = 0;
	Float_t* arrayValuesTruncMean = new Float_t[nentriesUsed]; 
	Double_t* arrayWeightsTruncMean = new Double_t[nentriesUsed]; 

	// calculating SD wrt Mean and Median
	IlcDebug(2,"Calculating SD wrt Mean and SD wrt Median");
	if (sdFlag){
		for (Int_t i =0; i< nentriesUsed; i++){
			IlcDebug(2,Form("Entry %d: value = %f, weight = %f",i,arrayValues[i],arrayWeights[i]));
			temp += (arrayValues[i]-parameters[2])*(arrayValues[i]-parameters[2]);
			temp1 += arrayWeights[i]*(arrayValues[i]-parameters[0])*(arrayValues[i]-parameters[0]);
			sumweights += arrayWeights[i];
		}
		// setting SD wrt Mean 
		if (sumweights != 0 ){
			parameters[3] = TMath::Sqrt(temp1/sumweights);
		}
		else {
			IlcError("Sum of weights to calculate Standard Deviation (wrt mean) <= 0, setting the SD to invalid");
			parameters[3] = IlcGRPObject::GetInvalidFloat();
		}
		// setting SD wrt Median
		if (nentriesUsed != 0){
			parameters[4] = TMath::Sqrt(temp/nentriesUsed);
		}
		else{
			IlcError("Number of entries used to calculate Standard Deviation (wrt median) <= 0, setting the SD to invalid");
			parameters[4] = IlcGRPObject::GetInvalidFloat();
		}
	}
	else {
		parameters[3] = IlcGRPObject::GetInvalidFloat();
		parameters[4] = IlcGRPObject::GetInvalidFloat();
	}		

	// calculating truncated mean (this comes afterwards since you need the SD wrt Mean)
	if (truncMeanFlag){
		IlcDebug(2,"Calculating Truncated Mean");
		for (Int_t i =0; i< nentriesUsed; i++){
			IlcDebug(2,Form("Entry %d: value = %f, weight = %f",i,arrayValues[i],arrayWeights[i]));
			if ((arrayValues[i]<=parameters[0]+3*parameters[3]) && (arrayValues[i]>=parameters[0]-3*parameters[3])){
				arrayValuesTruncMean[entriesTruncMean]=arrayValues[i];
				arrayWeightsTruncMean[entriesTruncMean]=arrayWeights[i];
				IlcDebug(2,Form("For Truncated Mean: Entry %d: value = %f, weight = %f",entriesTruncMean,arrayValuesTruncMean[entriesTruncMean],arrayWeightsTruncMean[entriesTruncMean]));
				entriesTruncMean++;			
			}
			else{
				IlcDebug(2,"Discarding entry");
			}
		}
		// setting truncated mean 
		if (entriesTruncMean >1){
			IlcDebug(2,Form("%d entries used for truncated mean",entriesTruncMean));
			parameters[1] = TMath::Mean(entriesTruncMean,arrayValuesTruncMean,arrayWeightsTruncMean);
		}
		else{	
			IlcDebug(2,Form("Too few entries (%d) to calculate truncated mean",entriesTruncMean));
			parameters[1] = IlcGRPObject::GetInvalidFloat();
		}
	}
	else{
			parameters[1] = IlcGRPObject::GetInvalidFloat();
	}

	if (arrayValues){
		delete [] arrayValues;
	} 
	if (arrayWeights){
		delete [] arrayWeights;
	} 
	delete [] arrayValuesTruncMean;
	delete [] arrayWeightsTruncMean;

	IlcInfo(Form("(weighted) mean = %f ",parameters[0]));
	IlcInfo(Form("(weighted) truncated mean = %f ",parameters[1]));
	IlcInfo(Form("median = %f ",parameters[2]));
	IlcInfo(Form("(weighted) standard deviation with (weighted) mean = %f ",parameters[3]));
	IlcInfo(Form("standard deviation with median = %f ",parameters[4]));
	
	return parameters;
}

//__________________________________________________________________________________________________________________

Float_t* IlcGRPPreprocessor::ProcessFloatAllMagnet(const TObjArray* array, Int_t indexDP, Bool_t &isZero)
{
	// 
	// processing Float values using Mean, Median, Standard Deviation wrt Mean, Standar Deviation wrt Median 
	// used for L3 and Dipole magnets, using isZero flag to decide whther the magnet was OFF/ON
	// the flag is set according to the L3/Dipole current value
	// current threshold for L3 = 350 A (value provided by DCS)
	// current threshold for Dipole = 450 A (value provided by DCS)
	//
	// parameters[0] = mean
	// parameters[1] = truncated mean (calculated excluding points outside +/- 3RMS from mean
	// parameters[2] = median
	// parameters[3] = standard deviation wrt mean
	// parameters[4] = standard deviation wrt median
	//

	IlcInfo(Form("indexDP = %d",indexDP)); 

	Int_t nCounts = array->GetEntries();
	for(Int_t i = 0; i < nCounts; i++) {
		IlcDCSValue *v = (IlcDCSValue *)array->At(i);
		if ((v->GetFloat() <= fminFloat) || (v->GetFloat() >= fmaxFloat)) {
			IlcError(Form("Error! Float value found in DCS map at %d-th entry is OUT OF RANGE: value = %6.5e",i,v->GetFloat()));
			if (v->GetFloat() < fminFloat) IlcInfo(Form("The value is smaller than %6.5e",fminFloat));
			if (v->GetFloat() > fmaxFloat) IlcInfo(Form("The value is greater than %6.5e",fmaxFloat));
			return NULL;
		}
		if(((Int_t)(v->GetTimeStamp()) >= (Int_t)GetStartTimeDCSQuery()) &&((Int_t)(v->GetTimeStamp()) <= (Int_t)GetEndTimeDCSQuery())) {
			IlcDebug(2,Form("%d-th entry = %f",i,v->GetFloat()));
			if (indexDP == kL3Current && v->GetFloat() > 350 && isZero == kTRUE) isZero=kFALSE; 
			if (indexDP == kDipoleCurrent && v->GetFloat() > 450 && isZero == kTRUE) isZero=kFALSE; 
		}
		else {
			IlcError(Form("DCS values for the parameter outside the queried interval"));
		}
	}

	return ProcessFloatAll(array);
}


//_______________________________________________________________

Char_t IlcGRPPreprocessor::ProcessBool(const TObjArray* array, Bool_t &change)
{
	// 
	// processing Boolean values
	//

	Bool_t aDCSBool = kTRUE;

	IlcDCSValue *v = 0x0;

	for(Int_t iCount = 0; iCount < array->GetEntries(); iCount++) {
		v = (IlcDCSValue *)array->At(iCount);
		if (((Int_t)(v->GetTimeStamp()) < (Int_t)GetStartTimeDCSQuery()) || ((Int_t)(v->GetTimeStamp()) > (Int_t)GetEndTimeDCSQuery())) {
			IlcError(Form("DCS values for the parameter outside the queried interval"));
			continue;
		}
		if (iCount > 0) {
			if (aDCSBool != v->GetBool()) {
				IlcError(Form("DCS values for the parameter changed from %d to %d within the queried interval", (UInt_t)aDCSBool, (UInt_t)v->GetBool()));
				change = kTRUE;
			}
		}
		aDCSBool = v->GetBool(); // always keeping last value
		IlcDebug(2,Form("Bool = %d",(Int_t)aDCSBool));
	}
	
	Char_t caDCSBool = (Char_t) aDCSBool;
	return caDCSBool;
	
}

//_______________________________________________________________

Float_t IlcGRPPreprocessor::ProcessInt(const TObjArray* array)
{
	// 
	// processing Int values, returning mean
	// IlcGRPObject::GetInvalidFloat() is returned if any of the DCS values
	// are outside the queried time interval or their value is out of range
	//

	TString timeStartString = (TString)GetRunParameter("DAQ_time_start");
	TString timeEndString = (TString)GetRunParameter("DAQ_time_end");
	if (timeStartString.IsNull() || timeStartString.IsNull()){
		if (timeStartString.IsNull()){ 
			IlcError("DAQ_time_start not set in logbook! Setting statistical values for current DP to invalid");
		}
		else if (timeStartString.IsNull()){
			IlcError("DAQ_time_end not set in logbook! Setting statistical values for current DP to invalid");
		}
		return 0;
	}  

	Int_t timeStart = (Int_t)(timeStartString.Atoi());
	Int_t timeEnd = (Int_t)(timeEndString.Atoi());
	Float_t aDCSArrayMean = 0.0;
	Int_t iCounts = 0;
	Float_t valueBeforeSOR = 0;
	Float_t valueAfterEOR = 0;
	Int_t timestampBeforeSOR = -1;
	Int_t timestampAfterEOR = -1;
	Int_t ientrySOR = -1;
	Int_t ientryEOR = -1;
	Float_t* arrayValues = 0x0; 
	Double_t* arrayWeights = 0x0; 
	Int_t iCountsRun = 0;
	Int_t nCounts = array->GetEntries();

	for(Int_t i = 0; i < nCounts; i++) {
		IlcDCSValue* v = (IlcDCSValue *)array->At(i);
		if ((v->GetInt() < fminInt) || (v->GetInt() > fmaxInt)) {
			IlcError(Form("Error! Int value found in DCS map at %d-th entry is OUT OF RANGE: value = %d",i, v->GetInt()));
			return IlcGRPObject::GetInvalidFloat();
		}
		if(((Int_t)(v->GetTimeStamp()) >= (Int_t)GetStartTimeDCSQuery()) &&((Int_t)(v->GetTimeStamp()) <= (Int_t)GetEndTimeDCSQuery())) {
			IlcDebug(2,Form("%d-th entry = %d at timestamp %i",i,v->GetInt(),v->GetTimeStamp()));
			iCounts += 1;
			// look for the last value before SOR and the first value before EOR
			if (((Int_t)(v->GetTimeStamp()) >= (Int_t)GetStartTimeDCSQuery()) && (Int_t)(v->GetTimeStamp()) < timeStart) {
				timestampBeforeSOR = (Int_t)(v->GetTimeStamp());
				IlcDebug(2,Form("timestamp of last entry before SOR = %d, with DAQ_time_start = %d",timestampBeforeSOR,timeStart));
				valueBeforeSOR = (Float_t) v->GetInt();
			}
			else if ((Int_t)(v->GetTimeStamp()) <= (Int_t)GetEndTimeDCSQuery() && (Int_t)(v->GetTimeStamp()) > timeEnd && timestampAfterEOR == -1){
				timestampAfterEOR = (Int_t)(v->GetTimeStamp());
				valueAfterEOR = (Float_t) v->GetInt();
				IlcDebug(2,Form("timestamp of first entry after EOR = %d, with DAQ_time_end = %d",timestampAfterEOR,timeEnd));
			}
			// check if there are DPs between DAQ_time_start and DAQ_time_end
			if(((Int_t)(v->GetTimeStamp()) >= timeStart) &&((Int_t)(v->GetTimeStamp()) <= timeEnd)) {
				if (ientrySOR == -1) ientrySOR = i;  // first entry after SOR
				if (ientryEOR < i) ientryEOR = i;  // last entry before EOR
				IlcDebug(2,Form("entry between SOR and EOR"));
				iCountsRun += 1;
			}
		}
		else {
			IlcError(Form("DCS values for the parameter outside the queried interval: timestamp = %d",v->GetTimeStamp()));
		}
	}

	if (timestampBeforeSOR == -1){
		IlcWarning("No value found before SOR!");
	}
	if (timestampAfterEOR == -1){
		IlcWarning("No value found after EOR!");
	}

	IlcDebug(2,Form("Number of valid entries (within query interval) = %i, starting from %i entries",iCounts,nCounts));
	IlcDebug(2,Form("Last value before DAQ_time_start (SOR) = %f at timestamp = %d",valueBeforeSOR,timestampBeforeSOR));
	IlcDebug(2,Form("First value after DAQ_time_end (EOR)   = %f at timestamp = %d",valueAfterEOR,timestampAfterEOR));
	IlcInfo(Form("Found %d entries between DAQ_time_start (SOR) and DAQ_time_end (EOR)",iCountsRun));
	IlcDebug(2,Form("Index of first entry after DAQ_time_start (SOR) = %d ",ientrySOR));
	IlcDebug(2,Form("Index of first entry before DAQ_time_end (EOR) = %d ",ientryEOR));

	Int_t nentriesUsed = 0;
	if (iCountsRun > 1){
		IlcInfo("Using entries between DAQ_time_start (SOR) and DAQ_time_end (EOR)");
		IlcDebug(2,"Calculating (weighted) Mean");
		arrayValues = new Float_t[iCountsRun]; 
		arrayWeights = new Double_t[iCountsRun]; 
		nentriesUsed = iCountsRun;
		for (Int_t i = ientrySOR; i <= ientryEOR; i++){
			IlcDCSValue *v = (IlcDCSValue *)array->At(i);
			Int_t timestamp2 = 0;
			if (i < ientryEOR){
				IlcDCSValue *v1 = (IlcDCSValue *)array->At(i+1);
				timestamp2 = (Int_t)v1->GetTimeStamp();
			}
			else {
				timestamp2 = timeEnd+1;
			}
			arrayWeights[i-ientrySOR] = (Double_t)(timestamp2 - (Int_t)v->GetTimeStamp());
			arrayValues[i-ientrySOR] = (Float_t)v->GetInt();
		}
		aDCSArrayMean = TMath::Mean(iCountsRun,arrayValues,arrayWeights);
		delete [] arrayValues;
		delete [] arrayWeights;
	}
	else if (iCountsRun == 1){
		IlcDCSValue* v = (IlcDCSValue *)array->At(ientrySOR);
		nentriesUsed = 2;
		if (timestampBeforeSOR != -1 && timestampBeforeSOR != (Int_t)v->GetTimeStamp()){
			IlcWarning("Using single entry between DAQ_time_start (SOR) and DAQ_time_end (EOR) and last entry before SOR.");
			arrayValues = new Float_t[2];
			arrayWeights = new Double_t[2];
			arrayValues[0] = valueBeforeSOR;
			arrayWeights[0] = (Double_t)((Int_t)v->GetTimeStamp()-timestampBeforeSOR);
			arrayValues[1] = (Float_t)v->GetInt();
			arrayWeights[1] = (Double_t)(timeEnd+1-(Int_t)v->GetTimeStamp());
			IlcDebug(2,Form("value0 = %f, with weight = %f",arrayValues[0],arrayWeights[0])); 
			IlcDebug(2,Form("value1 = %f, with weight = %f",arrayValues[1],arrayWeights[1])); 
			aDCSArrayMean = TMath::Mean(2,arrayValues,arrayWeights);
			delete [] arrayValues;
			delete [] arrayWeights;
		}
		else{
			IlcError("Cannot calculate mean - only one value collected during the run, but no value before with which to calculate the statistical quantities");
			return IlcGRPObject::GetInvalidFloat();
		}
	}
	else { // iCountsRun == 0, using the point immediately before SOR and the one immediately after EOR
		if (timestampBeforeSOR == -1 || timestampAfterEOR == -1){
			if (timestampBeforeSOR == -1){
				IlcError("Cannot calculate mean - no points during the run collected, and point before SOR missing");
			}
			if (timestampAfterEOR == -1){
				IlcError("Cannot calculate maen - no points during the run collected, and point after EOR missing");
			}
			return IlcGRPObject::GetInvalidFloat();
		}
		else {
			IlcWarning("Using last entry before SOR and first entry after EOR.");
			nentriesUsed = 2;
			arrayValues = new Float_t[2];
			arrayWeights = new Double_t[2];
			arrayValues[0] = valueBeforeSOR;
			arrayWeights[0] = (Double_t)(timestampAfterEOR - timestampBeforeSOR);
			arrayValues[1] = valueAfterEOR;
			arrayWeights[1] = 1.;
			IlcDebug(2,Form("value0 = %f, with weight = %f",arrayValues[0],arrayWeights[0])); 
			IlcDebug(2,Form("value1 = %f, with weight = %f",arrayValues[1],arrayWeights[1])); 
			aDCSArrayMean = TMath::Mean(1,arrayValues,arrayWeights);
			delete [] arrayValues;
			delete [] arrayWeights;
		}
	}

	IlcInfo(Form("mean = %f ", aDCSArrayMean));
	return aDCSArrayMean;

}
//_______________________________________________________________

Float_t IlcGRPPreprocessor::ProcessUInt(const TObjArray* array)
{
	// 
	// processing Int values, returning mean 
	// IlcGRPObject::GetInvalidFloat() is returned if any of the DCS values
	// are outside the queried time interval or their value is out of range
	//

	TString timeStartString = (TString)GetRunParameter("DAQ_time_start");
	TString timeEndString = (TString)GetRunParameter("DAQ_time_end");
	if (timeStartString.IsNull() || timeStartString.IsNull()){
		if (timeStartString.IsNull()){ 
			IlcError("DAQ_time_start not set in logbook! Setting statistical values for current DP to invalid");
		}
		else if (timeStartString.IsNull()){
			IlcError("DAQ_time_end not set in logbook! Setting statistical values for current DP to invalid");
		}
		return 0;
	}  

	Int_t timeStart = (Int_t)(timeStartString.Atoi());
	Int_t timeEnd = (Int_t)(timeEndString.Atoi());
	Float_t aDCSArrayMean = 0.0;
	Int_t iCounts = 0;
	Float_t valueBeforeSOR = 0;
	Float_t valueAfterEOR = 0;
	Int_t timestampBeforeSOR = -1;
	Int_t timestampAfterEOR = -1;
	Int_t ientrySOR = -1;
	Int_t ientryEOR = -1;
	Float_t* arrayValues = 0x0; 
	Double_t* arrayWeights = 0x0; 
	Int_t iCountsRun = 0;
	Int_t nCounts = array->GetEntries();

	for(Int_t i = 0; i < nCounts; i++) {
		IlcDCSValue* v = (IlcDCSValue *)array->At(i);
		if ((v->GetUInt() < fminUInt) || (v->GetUInt() > fmaxUInt)) {
			IlcError(Form("Error! UInt value found in DCS map at %d-th entry is OUT OF RANGE: value = %u",i,v->GetUInt()));
			return IlcGRPObject::GetInvalidFloat();
		}
		if(((Int_t)(v->GetTimeStamp()) >= (Int_t)GetStartTimeDCSQuery()) &&((Int_t)(v->GetTimeStamp()) <= (Int_t)GetEndTimeDCSQuery())) {
			IlcDebug(2,Form("%d-th entry = %d at timestamp %i",i,v->GetUInt(),v->GetTimeStamp()));
			iCounts += 1;
			// look for the last value before SOR and the first value before EOR
			if (((Int_t)(v->GetTimeStamp()) >= (Int_t)GetStartTimeDCSQuery()) && (Int_t)(v->GetTimeStamp()) < timeStart) {
				timestampBeforeSOR = (Int_t)(v->GetTimeStamp());
				IlcDebug(2,Form("timestamp of last entry before SOR = %d, with DAQ_time_start = %d",timestampBeforeSOR,timeStart));
				valueBeforeSOR = (Float_t)v->GetUInt();
			}
			else if ((Int_t)(v->GetTimeStamp()) <= (Int_t)GetEndTimeDCSQuery() && (Int_t)(v->GetTimeStamp()) > timeEnd && timestampAfterEOR == -1){
				timestampAfterEOR = (Int_t)(v->GetTimeStamp());
				valueAfterEOR = (Float_t)v->GetUInt();
				IlcDebug(2,Form("timestamp of first entry after EOR = %d, with DAQ_time_end = %d",timestampAfterEOR,timeEnd));
			}
			// check if there are DPs between DAQ_time_start and DAQ_time_end
			if(((Int_t)(v->GetTimeStamp()) >= timeStart) &&((Int_t)(v->GetTimeStamp()) <= timeEnd)) {
				if (ientrySOR == -1) ientrySOR = i;  // first entry after SOR
				if (ientryEOR < i) ientryEOR = i;  // last entry before EOR
				IlcDebug(2,Form("entry between SOR and EOR"));
				iCountsRun += 1;
			}
		}
		else {
			IlcError(Form("DCS values for the parameter outside the queried interval: timestamp = %d",v->GetTimeStamp()));
		}
	}

	if (timestampBeforeSOR == -1){
		IlcWarning("No value found before SOR!");
	}
	if (timestampAfterEOR == -1){
		IlcWarning("No value found after EOR!");
	}

	IlcDebug(2,Form("Number of valid entries (within query interval) = %i, starting from %i entries",iCounts,nCounts));
	IlcDebug(2,Form("Last value before DAQ_time_start (SOR) = %f at timestamp = %d",valueBeforeSOR,timestampBeforeSOR));
	IlcDebug(2,Form("First value after DAQ_time_end (EOR)   = %f at timestamp = %d",valueAfterEOR,timestampAfterEOR));
	IlcInfo(Form("Found %d entries between DAQ_time_start (SOR) and DAQ_time_end (EOR)",iCountsRun));
	IlcDebug(2,Form("Index of first entry after DAQ_time_start (SOR) = %d ",ientrySOR));
	IlcDebug(2,Form("Index of first entry before DAQ_time_end (EOR) = %d ",ientryEOR));

	Int_t nentriesUsed = 0;
	if (iCountsRun > 1){
		IlcInfo("Using entries between DAQ_time_start (SOR) and DAQ_time_end (EOR)");
		IlcDebug(2,"Calculating (weighted) Mean");
		arrayValues = new Float_t[iCountsRun]; 
		arrayWeights = new Double_t[iCountsRun]; 
		nentriesUsed = iCountsRun;
		for (Int_t i = ientrySOR; i <= ientryEOR; i++){
			IlcDCSValue *v = (IlcDCSValue *)array->At(i);
			Int_t timestamp2 = 0;
			if (i < ientryEOR){
				IlcDCSValue *v1 = (IlcDCSValue *)array->At(i+1);
				timestamp2 = (Int_t)v1->GetTimeStamp();
			}
			else {
				timestamp2 = timeEnd+1;
			}
			arrayWeights[i-ientrySOR] = (Double_t)(timestamp2 - (Int_t)v->GetTimeStamp());
			arrayValues[i-ientrySOR] = (Float_t)v->GetUInt();
		}
		aDCSArrayMean = TMath::Mean(iCountsRun,arrayValues,arrayWeights);
		delete [] arrayValues;
		delete [] arrayWeights;
	}
	else if (iCountsRun == 1){
		IlcDCSValue* v = (IlcDCSValue *)array->At(ientrySOR);
		nentriesUsed = 2;
		if (timestampBeforeSOR != -1 && timestampBeforeSOR != (Int_t)v->GetTimeStamp()){
			IlcWarning("Using single entry between DAQ_time_start (SOR) and DAQ_time_end (EOR) and last entry before SOR.");
			arrayValues = new Float_t[2];
			arrayWeights = new Double_t[2];
			arrayValues[0] = valueBeforeSOR;
			arrayWeights[0] = (Double_t)((Int_t)v->GetTimeStamp()-timestampBeforeSOR);
			arrayValues[1] = (Float_t)v->GetUInt();
			arrayWeights[1] = (Double_t)(timeEnd+1-(Int_t)v->GetTimeStamp());
			IlcDebug(2,Form("value0 = %f, with weight = %f",arrayValues[0],arrayWeights[0])); 
			IlcDebug(2,Form("value1 = %f, with weight = %f",arrayValues[1],arrayWeights[1])); 
			aDCSArrayMean = TMath::Mean(2,arrayValues,arrayWeights);
			delete [] arrayValues;
			delete [] arrayWeights;
		}
		else{
			IlcError("Cannot calculate mean - only one value collected during the run, but no value before with which to calculate the statistical quantities");
			return IlcGRPObject::GetInvalidFloat();
		}
	}
	else { // iCountsRun == 0, using the point immediately before SOR and the one immediately after EOR
		if (timestampBeforeSOR == -1 || timestampAfterEOR == -1){
			if (timestampBeforeSOR == -1){
				IlcError("Cannot calculate mean - no points during the run collected, and point before SOR missing");
			}
			if (timestampAfterEOR == -1){
				IlcError("Cannot calculate maen - no points during the run collected, and point after EOR missing");
			}
			return IlcGRPObject::GetInvalidFloat();
		}
		else {
			IlcWarning("Using last entry before SOR and first entry after EOR.");
			nentriesUsed = 2;
			arrayValues = new Float_t[2];
			arrayWeights = new Double_t[2];
			arrayValues[0] = valueBeforeSOR;
			arrayWeights[0] = (Double_t)(timestampAfterEOR - timestampBeforeSOR);
			arrayValues[1] = valueAfterEOR;
			arrayWeights[1] = 1.;
			IlcDebug(2,Form("value0 = %f, with weight = %f",arrayValues[0],arrayWeights[0])); 
			IlcDebug(2,Form("value1 = %f, with weight = %f",arrayValues[1],arrayWeights[1])); 
			aDCSArrayMean = TMath::Mean(1,arrayValues,arrayWeights);
			delete [] arrayValues;
			delete [] arrayWeights;
		}
	}

	IlcInfo(Form("mean = %f ",aDCSArrayMean));
	return aDCSArrayMean;

}


//_______________________________________________________________

IlcDCSSensorArray *IlcGRPPreprocessor::GetPressureMap(TMap* dcsAliasMap)
{
	// extract DCS pressure maps. Perform fits to save space
	
	TMap *map = fPressure->ExtractDCS(dcsAliasMap);
	if (map) {
		IlcDebug(2,Form("Map has %d entries",map->GetEntries()));
		fPressure->MakeSplineFit(map);
		Double_t fitFraction = fPressure->NumFits()/fPressure->NumSensors(); 
		if (fitFraction > kFitFraction ) {
			IlcInfo(Form("Pressure values extracted, %d fits performed for %d sensors.", fPressure->NumFits(),fPressure->NumSensors()));
		} else { 
			IlcInfo("Too few pressure maps fitted!!!");
		}
	} else {
		IlcInfo("no atmospheric pressure map extracted!!!");
	}
	delete map;
	
	return fPressure;
}


  
//_______________________________________________________________
Int_t IlcGRPPreprocessor::ReceivePromptRecoParameters(UInt_t run, const char* dbHost, Int_t dbPort, const char* dbName, const char* user, const char* password, const char *cdbRoot, TString &gdc)
{
	//
	// Retrieves logbook and trigger information from the online logbook
	// This information is needed for prompt reconstruction
	//
	// Parameters are:
	// Run number
	// DAQ params: dbHost, dbPort, dbName, user, password, logbookTable, triggerTable
	// cdbRoot
	//
	// returns:
	//         positive on success: the return code is the run number of last run processed of the same run type already processed by the SHUTTLE
	//         0 on success and no run was found
	//         negative on error
	//
	// This function is NOT called during the preprocessor run in the Shuttle!
	//
	
	// defaults
	if (dbPort == 0)
		dbPort = 3306;
	
	// CDB connection
	IlcCDBManager* cdb = IlcCDBManager::Instance();
	cdb->SetDefaultStorage(cdbRoot);
	
	// SQL connection
	TSQLServer* server = TSQLServer::Connect(Form("mysql://%s:%d/%s", dbHost, dbPort, dbName), user, password);
	
	if (!server)
		{
			Printf("ERROR: Could not connect to DAQ LB");
			return -1;
		}
	
	// main logbook
	TString sqlQuery;
	sqlQuery.Form("SELECT DAQ_time_start, run_type, detectorMask, L3_magnetCurrent, Dipole_magnetCurrent,beamType FROM logbook WHERE run = %d", run);
	TSQLResult* result = server->Query(sqlQuery);
	if (!result)
		{
			Printf("ERROR: Can't execute query <%s>!", sqlQuery.Data());
			return -2;
		}
	
	if (result->GetRowCount() == 0)
		{
			Printf("ERROR: Run %d not found", run);
			delete result;
			return -3;
		}
	
	TSQLRow* row = result->Next();
	if (!row)
		{
			Printf("ERROR: Could not receive data from run %d", run);
			delete result;
			return -4;
		}
	
	TString timeStartString(row->GetField(0));
	TString runType(row->GetField(1));
	TString detectorMaskString(row->GetField(2));
	TString l3CurrentString(row->GetField(3));
	TString dipoleCurrentString(row->GetField(4));
	TString beamTypeString(row->GetField(5));
	time_t timeStart = (time_t)(timeStartString.Atoi());
	UInt_t detectorMask = (UInt_t)(detectorMaskString.Atoi());
	Float_t l3Current = (Float_t)(TMath::Abs(l3CurrentString.Atof()));
	Float_t dipoleCurrent = (Float_t)(TMath::Abs(dipoleCurrentString.Atof()));
	Char_t l3Polarity = (l3CurrentString.Atof() < 0) ? 1 : 0;
	Char_t dipolePolarity = (dipoleCurrentString.Atof() < 0) ? 1 : 0;
	if (beamTypeString.CompareTo("Pb-Pb",TString::kIgnoreCase) == 0){
		beamTypeString="A-A";
	}
	
	IlcGRPObject * grpObj = new IlcGRPObject();
	grpObj->SetTimeStart(timeStart); 
	grpObj->SetRunType((TString)(row->GetField(1)));
	grpObj->SetDetectorMask(detectorMask);
	grpObj->SetL3Current(l3Current,(IlcGRPObject::Stats)0);
	grpObj->SetDipoleCurrent(dipoleCurrent,(IlcGRPObject::Stats)0);
	grpObj->SetL3Polarity(l3Polarity);
	grpObj->SetDipolePolarity(dipolePolarity);
	grpObj->SetPolarityConventionLHC();  // after the dipole cables swap we comply with LHC convention
	grpObj->SetBeamType(beamTypeString);

	delete row;
	row = 0;
	
	delete result;
	result = 0;
	
	Printf("Storing GRP/GRP/Data object with the following content");
	grpObj->Dump();
	
	IlcCDBMetaData metadata;
	metadata.SetResponsible("Jan Fiete Grosse-Oetringhaus & Chiara Zampolli");
	metadata.SetComment("GRP Output parameters received during online running");
	
	IlcCDBId id("GRP/GRP/Data", run, run);
	Bool_t success = cdb->Put(grpObj, id, &metadata);
	
	delete grpObj;
	
	if (!success)
		{
			Printf("ERROR: Could not store GRP/GRP/Data into OCDB");
			return -5;
		}
	
	// Receive trigger information
	sqlQuery.Form("SELECT configFile FROM logbook_trigger_config WHERE run = %d", run);
	result = server->Query(sqlQuery);
	if (!result)
		{
			Printf("ERROR: Can't execute query <%s>!", sqlQuery.Data());
			return -11;
		}
	
	if (result->GetRowCount() == 0)
		{
			Printf("ERROR: Run %d not found in logbook_trigger_config", run);
			delete result;
			return -12;
		}
	
	row = result->Next();
	if (!row)
		{
			Printf("ERROR: Could not receive logbook_trigger_config data from run %d", run);
			delete result;
			return -13;
		}
	
	TString triggerConfig(row->GetField(0));
	
	delete row;
	row = 0;
	
	delete result;
	result = 0;
	
	Printf("Found trigger configuration: %s", triggerConfig.Data());
	
	IlcTriggerConfiguration *runcfg = IlcTriggerConfiguration::LoadConfigurationFromString(triggerConfig);
	if (!runcfg)
		{
			Printf("ERROR: Could not create CTP configuration object");
			return -14;
		}
	
	metadata.SetComment("CTP run configuration received during online running");
	
	IlcCDBId id2("GRP/CTP/Config", run, run);
	success = cdb->Put(runcfg, id2, &metadata);
	
	delete runcfg;
	runcfg = 0;
	
	if (!success)
		{
			Printf("ERROR: Could not store GRP/CTP/Config into OCDB");
			return -15;
		}
	

	// Receive list of GDCs for this run
	sqlQuery.Form("SELECT GDC FROM logbook_stats_GDC WHERE run = %d", run);
	result = server->Query(sqlQuery);
	if (!result)
		{
			Printf("ERROR: Can't execute query <%s>!", sqlQuery.Data());
			return -24;
		}
	
	if (result->GetRowCount() == 0)
		{
			Printf("ERROR: Run %d not found in logbook_stats_GDC", run);
			delete result;
			return -25;
		}

	gdc = "";
	for (Int_t iGDC = 0; iGDC < result->GetRowCount(); iGDC++) {
	  row = result->Next();
	  if (!row)
	    {
	      Printf("ERROR: Could not receive logbook_stats_GDC data from run %d", run);
	      delete result;
	      return -26;
	    }
	  gdc += row->GetField(0);
	  gdc += " ";
	}

	delete row;
	row = 0;
	
	delete result;
	result = 0;
	
	Printf("Found GDC: %s", gdc.Data());

	// get last run with same run type that was already processed by the SHUTTLE
	
	sqlQuery.Form("SELECT max(logbook.run) FROM logbook LEFT JOIN logbook_shuttle ON logbook_shuttle.run = logbook.run WHERE run_type = '%s' AND shuttle_done = 1", runType.Data());
	result = server->Query(sqlQuery);
	if (!result)
		{
			Printf("ERROR: Can't execute query <%s>!", sqlQuery.Data());
			return -21;
		}
	
	if (result->GetRowCount() == 0)
		{
			Printf("ERROR: No result with query <%s>", sqlQuery.Data());
			delete result;
			return -22;
		}
	
	row = result->Next();
	if (!row)
		{
			Printf("ERROR: Could not receive data for query <%s>", sqlQuery.Data());
			delete result;
			return -23;
		}
	
	TString lastRunStr(row->GetField(0));
	Int_t lastRun = lastRunStr.Atoi();
	
	Printf("Last run with same run type %s is %d", runType.Data(), lastRun);
	
	delete row;
	row = 0;
	
	delete result;
	result = 0;
	
	server->Close();
	delete server;
	server = 0;
	
	return lastRun;
}
//------------------------------------------------------------------------------------------------------
Float_t IlcGRPPreprocessor::ProcessEnergy(TObjArray* const array, Double_t timeStart){

	//
	// Method to processo LHC Energy information
	// Only the first value is returned, provided that it is withing DAQ_time_start and DAQ_time_end
	//

	Int_t nCounts = array->GetEntries();
	Float_t energy = -1;
	Double_t timeEnergy = -1;
	Int_t indexEnergy = -1;
	Bool_t foundEnergy = kFALSE;

	IlcDebug(2,Form("Energy measurements = %d\n",nCounts));
	if (nCounts ==0){
		IlcWarning("No Energy values found! Beam Energy remaining invalid!");
	}
	else{
		for (Int_t i = 0; i < nCounts; i++){
			IlcDCSArray *dcs = (IlcDCSArray*)array->At(i);
			if (dcs){
				if (dcs->GetTimeStamp()<=timeStart && dcs->GetTimeStamp()>=timeEnergy){// taking always the very last entry: of two measurements have the same timestamp, the last one is taken
					timeEnergy = dcs->GetTimeStamp();
					indexEnergy = i;
					foundEnergy = kTRUE;
				}
				else{
					break;
				}
			}
		}
		if (!foundEnergy){
			IlcInfo("No value for the Energy found before start of run, the Energy will remain invalid");
		}
		else {
			IlcDCSArray* dcs = (IlcDCSArray*)array->At(indexEnergy);
			energy = (Float_t)(TMath::Nint(((Double_t)(dcs->GetInt(0)))*120/1000)); // sqrt(s)/2 energy in GeV
			IlcInfo(Form("Energy value found = %d (at %f), converting --> sqrt(s)/2 = %f (GeV)", dcs->GetInt(0),dcs->GetTimeStamp(),energy));
		}
	}

	return energy;
}
//------------------------------------------------------------------------------------------------------
IlcLHCClockPhase* IlcGRPPreprocessor::ProcessLHCClockPhase(TObjArray *beam1phase,TObjArray *beam2phase, Double_t timeEnd)
{
  //
  // Method to process LHC-Clock Phase data
  // Only the values between DAQ_time_start and DAQ_time_end are kept
  //
  IlcLHCClockPhase *phaseObj = new IlcLHCClockPhase;

  Bool_t foundBeam1Phase = kFALSE, foundBeam2Phase = kFALSE;
  const Float_t threshold = 0.050; // we store the measurement only in case they differ with more 50ps from the previous one 

  TString timeCreatedStr = GetRunParameter("time_created");
  Double_t timeCreated = timeCreatedStr.Atof();

  Int_t nCounts = beam1phase->GetEntries();
  IlcDebug(2,Form("Beam1 phase measurements = %d\n",nCounts));
  if (nCounts ==0){
    IlcWarning("No beam1 LHC clock phase values found!");
    delete phaseObj;
    return NULL;
  }
  else{
    Double_t prevPhase = 0;
    for (Int_t i = 0; i < nCounts; i++){
      IlcDCSArray *dcs = (IlcDCSArray*)beam1phase->At(i);
      if (dcs){
	      //if (dcs->GetTimeStamp()>=timeStart && dcs->GetTimeStamp()<=timeEnd) {
	      if (dcs->GetTimeStamp()>=timeCreated && dcs->GetTimeStamp()<=timeEnd) {
	  if ((i == 0) || (i == (nCounts-1)) ||
	      !foundBeam1Phase ||
	      (TMath::Abs(dcs->GetDouble(0)-prevPhase) > threshold)) {
	    prevPhase = dcs->GetDouble(0);
	    foundBeam1Phase = kTRUE;
	    IlcInfo(Form("B1 Clk Phase = %f at TS = %f",
			 (Float_t)dcs->GetDouble(0),dcs->GetTimeStamp()));  
	    phaseObj->AddPhaseB1DP((UInt_t)dcs->GetTimeStamp(),(Float_t)dcs->GetDouble(0));
	  }
	}
      }
    }
    if (!foundBeam1Phase){
      IlcError("No beam1 LHC clock phase values found within the run!");
      delete phaseObj;
      return NULL;
    }
  }

  nCounts = beam2phase->GetEntries();
  IlcDebug(2,Form("Beam2 phase measurements = %d\n",nCounts));
  if (nCounts ==0){
    IlcWarning("No beam2 LHC clock phase values found!");
    delete phaseObj;
    return NULL;
  }
  else{
    Double_t prevPhase = 0;
    for (Int_t i = 0; i < nCounts; i++){
      IlcDCSArray *dcs = (IlcDCSArray*)beam2phase->At(i);
      if (dcs){
	if (dcs->GetTimeStamp()>=timeCreated && dcs->GetTimeStamp()<=timeEnd) {
	  if ((i == 0) || (i == (nCounts-1)) ||
	      !foundBeam2Phase ||
	      (TMath::Abs(dcs->GetDouble(0)-prevPhase) > threshold)) {
	    prevPhase = dcs->GetDouble(0);
	    foundBeam2Phase = kTRUE;
	    IlcInfo(Form("B2 Clk Phase = %f at TS = %f",
			 (Float_t)dcs->GetDouble(0),dcs->GetTimeStamp()));  
	    phaseObj->AddPhaseB2DP((UInt_t)dcs->GetTimeStamp(),(Float_t)dcs->GetDouble(0));
	  }
	}
      }
    }
    if (!foundBeam2Phase){
      IlcError("No beam2 LHC clock phase values found within the run!");
      delete phaseObj;
      return NULL;
    }
  }

  return phaseObj;
}
//------------------------------------------------------------------------------------------------------
TString IlcGRPPreprocessor::ParseBeamTypeString(TString beamType, Int_t iBeamType)
{
	// Method to return the convention for the separate beam type
	// in the form A*1000+Z
	// e.g.: Pb82 --> 208000 + 82 = 208082
	//       p --> 1000 + 1 = 1001

	Int_t a = 0;
	Int_t z = 0;
	TString separateString("");
	Log(Form("Setting Beam Type for beam %d to A*1000+Z",iBeamType));
	if (beamType.CompareTo("PROTON",TString::kIgnoreCase) == 0){
		Log(Form("Beam type %d is PROTON --> The single beam type will be set to 1001 (A = 1, Z = 1)",iBeamType));
		separateString = "1001";
		return separateString;
	}
	else { 
		TPRegexp regexpA("\\D+");
		TPRegexp regexpZ("\\d+");
		TObjArray* arrayA = regexpA.MatchS(beamType);
		TObjArray* arrayZ = regexpZ.MatchS(beamType);
		if (arrayA->GetEntries() != 1 || arrayZ->GetEntries() != 1){
			Log(Form("The beamType string for beam %d does not contain the necessary information! Returning the info as published by LHC (i.e. %s)",iBeamType, beamType.Data()));
			return beamType;
		}
		else{
			TString strA = ((TObjString*)(arrayA->At(0)))->String();
			TString strZ = ((TObjString*)(arrayZ->At(0)))->String();
			if (strA.CompareTo("LEAD",TString::kIgnoreCase) == 0 || strA.CompareTo("PB",TString::kIgnoreCase) == 0){
				Log(Form("Beam %d is %s --> A = 208",iBeamType, strA.Data()));
				a = 208;
			}
			else{
				Log(Form("This beam was not foreseen so far, leaving A=0"));
			}
			z = strZ.Atoi();
			Log(Form("Beam %d has Z = %d",iBeamType, z));
			separateString = Form("%d",a*1000+z);
			return separateString;
		}					     
	}
	       
	return separateString;

}
	    

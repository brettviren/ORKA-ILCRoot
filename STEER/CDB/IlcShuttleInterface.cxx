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

/* $Id: IlcShuttleInterface.cxx 58464 2012-09-05 15:46:16Z hristov $ */

//
// abstract interface class to IlcShuttle
// This class is implemented by IlcTestShuttle for testing and
// by IlcShuttle for the full setup
//

#include "IlcShuttleInterface.h"
#include "IlcLog.h"
#include <TClass.h>
#include <TSystem.h>

ClassImp(IlcShuttleInterface)

	const char* IlcShuttleInterface::fkSystemNames[4] = { "DAQ", "DCS", "HLT", "DQM" };

// names of the detectors preprocessors
const char* IlcShuttleInterface::fgkDetName[kNDetectors] = {"SPD", "SDD", "SSD", "TPC", "TRD", "DCH", "TOF",
       "PHS", "CPV", "HMP", "EMC", "MCH", "MTR", "FMD", "ZDC", "PMD", "T00", "V00", "GRP", "HLT", "ACO", "TRI"
};

// names of the detectors in OCDB
const char* IlcShuttleInterface::fgkOfflineDetName[kNDetectors] = {"ITS", "ITS", "ITS", "TPC", "TRD", "DCH", "TOF",
       "PHOS", "PHOS", "HMPID", "EMCAL", "MUON", "MUON", "FMD", "ZDC", "PMD", "T0", "VZERO", "GRP", "HLT", "ACORDE", "TRIGGER"
};

TString IlcShuttleInterface::fgkMainCDB("alien://folder=ShuttleCDB");
TString IlcShuttleInterface::fgkLocalCDB("local://LocalShuttleCDB");
TString IlcShuttleInterface::fgkMainRefStorage("alien://folder=ShuttleReference");
TString IlcShuttleInterface::fgkLocalRefStorage("local://LocalReferenceStorage");
TString IlcShuttleInterface::fgkMirrorSEs("ILC::CERN::OCDB");

TString IlcShuttleInterface::fgkShuttleTempDir("/tmp");
TString IlcShuttleInterface::fgkShuttleLogDir("/tmp/log");

//______________________________________________________________________________________________
const char* IlcShuttleInterface::GetOfflineDetName(const char* detName){
// Return "offline" detector name

	Int_t detPos = GetDetPos(detName);
	if(detPos < 0) {
		IlcErrorClass(Form("Unknown detector: %s",detName));
		return 0;
	}

	return fgkOfflineDetName[detPos];
}

//______________________________________________________________________________________________
const char* IlcShuttleInterface::GetDetName(UInt_t detPos){
// Return detector code

	if(detPos >= kNDetectors) {
		IlcErrorClass(Form("Parameter out of bound: %d", detPos));
		return 0;
	}

	return fgkDetName[detPos];
}

//______________________________________________________________________________________________
Int_t IlcShuttleInterface::GetDetPos(const char* detName){
// Return detector position in the detector code array

	for(UInt_t iDet=0; iDet < kNDetectors; iDet++){
		if(!strcmp(fgkDetName[iDet], detName)) return iDet;
	}
	return -1;
}

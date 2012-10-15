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

/* $Id: IlcSimulation.cxx 57725 2012-07-06 14:43:09Z hristov $ */

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// class for running generation, simulation and digitization                 //
//                                                                           //
// Hits, sdigits and digits are created for all detectors by typing:         //
//                                                                           //
//   IlcSimulation sim;                                                      //
//   sim.Run();                                                              //
//                                                                           //
// The Run method returns kTRUE in case of successful execution.             //
// The number of events can be given as argument to the Run method or it     //
// can be set by                                                             //
//                                                                           //
//   sim.SetNumberOfEvents(n);                                               //
//                                                                           //
// The name of the configuration file can be passed as argument to the       //
// IlcSimulation constructor or can be specified by                          //
//                                                                           //
//   sim.SetConfigFile("...");                                               //
//                                                                           //
// The generation of particles and the simulation of detector hits can be    //
// switched on or off by                                                     //
//                                                                           //
//   sim.SetRunGeneration(kTRUE);   // generation of primary particles       //
//   sim.SetRunSimulation(kFALSE);  // but no tracking                       //
//                                                                           //
// For which detectors sdigits and digits will be created, can be steered    //
// by                                                                        //
//                                                                           //
//   sim.SetMakeSDigits("ALL");     // make sdigits for all detectors        //
//   sim.SetMakeDigits("ITS TPC");  // make digits only for ITS and TPC      //
//                                                                           //
// The argument is a (case sensitive) string with the names of the           //
// detectors separated by a space. An empty string ("") can be used to       //
// disable the creation of sdigits or digits. The special string "ALL"       //
// selects all available detectors. This is the default.                     //
//                                                                           //
// The creation of digits from hits instead of from sdigits can be selected  //
// by                                                                        //
//                                                                           //
//   sim.SetMakeDigitsFromHits("TRD");                                       //
//                                                                           //
// The argument is again a string with the selected detectors. Be aware that //
// this feature is not available for all detectors and that merging is not   //
// possible, when digits are created directly from hits.                     //
//                                                                           //
// Background events can be merged by calling                                //
//                                                                           //
//   sim.MergeWith("background/gilc.root", 2);                             //
//                                                                           //
// The first argument is the file name of the background gilc file. The    //
// second argument is the number of signal events per background event.      //
// By default this number is calculated from the number of available         //
// background events. MergeWith can be called several times to merge more    //
// than two event streams. It is assumed that the sdigits were already       //
// produced for the background events.                                       //
//                                                                           //
// The output of raw data can be switched on by calling                      //
//                                                                           //
//   sim.SetWriteRawData("MUON");   // write raw data for MUON               //
//                                                                           //
// The default output format of the raw data are DDL files. They are         //
// converted to a DATE file, if a file name is given as second argument.     //
// For this conversion the program "dateStream" is required. If the file     //
// name has the extension ".root", the DATE file is converted to a root      //
// file. The program "ilcmdc" is used for this purpose. For the conversion   //
// to DATE and root format the two conversion programs have to be installed. //
// Only the raw data in the final format is kept if the third argument is    //
// kTRUE.                                                                    //
//                                                                           //
// The methods RunSimulation, RunSDigitization, RunDigitization,             //
// RunHitsDigitization and WriteRawData can be used to run only parts of     //
// the full simulation chain. The creation of raw data DDL files and their   //
// conversion to the DATE or root format can be run directly by calling      //
// the methods WriteRawFiles, ConvertRawFilesToDate and ConvertDateToRoot.   //
//                                                                           //
// The default number of events per file, which is usually set in the        //
// config file, can be changed for individual detectors and data types       //
// by calling                                                                //
//                                                                           //
//   sim.SetEventsPerFile("PHOS", "Reconstructed Points", 3);                //
//                                                                           //
// The first argument is the detector, the second one the data type and the  //
// last one the number of events per file. Valid data types are "Hits",      //
// "Summable Digits", "Digits", "Reconstructed Points" and "Tracks".         //
// The number of events per file has to be set before the simulation of      //
// hits. Otherwise it has no effect.                                         //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#include <TCint.h>
#include <TFile.h>
#include <TGeoGlobalMagField.h>
#include <TGeoManager.h>
#include <TObjString.h>
#include <TROOT.h>
#include <TSystem.h>
#include <TVirtualMC.h>
#include <TVirtualMCApplication.h>
#include <TDatime.h>
#include <TInterpreter.h>

#include "IlcAlignObj.h"
#include "IlcCDBEntry.h"
#include "IlcCDBManager.h"
#include "IlcGRPManager.h"
#include "IlcCDBStorage.h"
#include "IlcCTPRawData.h"
#include "IlcCentralTrigger.h"
#include "IlcCentralTrigger.h"
#include "IlcCodeTimer.h"
#include "IlcDAQ.h"
#include "IlcDigitizer.h"
#include "IlcESDEvent.h"
#include "IlcGRPObject.h"
#include "IlcGenEventHeader.h"
#include "IlcGenerator.h"
#include "IlcGeomManager.h"
// #include "IlcHLTSimulation.h"
#include "IlcHeader.h"
#include "IlcLego.h"
#include "IlcLegoGenerator.h"
#include "IlcLog.h"
#include "IlcMC.h"
#include "IlcMagF.h"
#include "IlcModule.h"
#include "IlcPDG.h"
#include "IlcRawReaderDate.h"
#include "IlcRawReaderFile.h"
#include "IlcRawReaderRoot.h"
#include "IlcRun.h"
#include "IlcDigitizationInput.h"
#include "IlcRunLoader.h"
#include "IlcSimulation.h"
#include "IlcSysInfo.h"
#include "IlcVertexGenFile.h"

ClassImp(IlcSimulation)

IlcSimulation *IlcSimulation::fgInstance = 0;
const char* IlcSimulation::fgkDetectorName[IlcSimulation::fgkNDetectors] = {"TARGET", "DCH", "PVBAR", "PVEC", "RSTACK"};

//_____________________________________________________________________________
IlcSimulation::IlcSimulation(const char* configFileName,
			     const char* name, const char* title) :
  TNamed(name, title),

  fRunGeneration(kTRUE),
  fRunSimulation(kTRUE),
  fLoadAlignFromCDB(kTRUE),
  fLoadAlObjsListOfDets("ALL"),
  fMakeSDigits("ALL"),
  fMakeDigits("ALL"),
  fTriggerConfig(""),
  fMakeDigitsFromHits(""),
  fWriteRawData(""),
  fRawDataFileName(""),
  fDeleteIntermediateFiles(kFALSE),
  fWriteSelRawData(kFALSE),
  fStopOnError(kFALSE),
  fNEvents(1),
  fConfigFileName(configFileName),
  fGIlcFileName("gilc.root"),
  fEventsPerFile(),
  fBkgrdFileNames(NULL),
  fIlcgnObjArray(NULL),
  fUseBkgrdVertex(kTRUE),
  fRegionOfInterest(kFALSE),
  fCDBUri("local://$ILC_ROOT/OCDB"),
  fQARefUri(""), 
  fSpecCDBUri(),
  fRun(-1),
  fSeed(0),
  fInitCDBCalled(kFALSE),
  fInitRunNumberCalled(kFALSE),
  fSetRunNumberFromDataCalled(kFALSE),
  fEmbeddingFlag(kFALSE),
  fLego(NULL),
  fKey(0),
  fUseVertexFromCDB(0),
  fUseMagFieldFromGRP(0),
  fGRPWriteLocation(Form("local://%s", gSystem->pwd())),
  fUseTimeStampFromCDB(0),
  fTimeStart(0),
  fTimeEnd(0),
  fQADetectors("ALL"),                  
  fQATasks("ALL"),	
  fRunQA(kTRUE), 
  fEventSpecie(IlcRecoParam::kDefault),
  fWriteQAExpertData(kTRUE), 
  fGeometryFile(),
  fRunHLT("default"),
  fpHLT(NULL),
  fWriteGRPEntry(kTRUE)
{
// create simulation object with default parameters
  fgInstance = this;
  SetGIlcFile("gilc.root");
  
// for QA
	IlcQAManager * qam = IlcQAManager::QAManager(IlcQAv1::kSIMMODE) ; 
	qam->SetActiveDetectors(fQADetectors) ; 
	fQATasks = Form("%d %d %d", IlcQAv1::kHITS, IlcQAv1::kSDIGITS, IlcQAv1::kDIGITS) ; 
	qam->SetTasks(fQATasks) ; 	
}

//_____________________________________________________________________________
IlcSimulation::~IlcSimulation()
{
// clean up

  fEventsPerFile.Delete();
//  if(fIlcgnObjArray) fIlcgnObjArray->Delete(); // fIlcgnObjArray->RemoveAll() ???
//  delete fIlcgnObjArray; fIlcgnObjArray=0;

  if (fBkgrdFileNames) {
    fBkgrdFileNames->Delete();
    delete fBkgrdFileNames;
  }

  fSpecCDBUri.Delete();
  if (fgInstance==this) fgInstance = 0;

  IlcQAManager::QAManager()->ShowQA() ; 
  IlcQAManager::Destroy() ; 	
  IlcCodeTimer::Instance()->Print();
}


//_____________________________________________________________________________
void IlcSimulation::SetNumberOfEvents(Int_t nEvents)
{
// set the number of events for one run

  fNEvents = nEvents;
}

//_____________________________________________________________________________
void IlcSimulation::InitQA()
{
  // activate a default CDB storage
  // First check if we have any CDB storage set, because it is used 
  // to retrieve the calibration and alignment constants
  
  if (fInitCDBCalled) return;
  fInitCDBCalled = kTRUE;

  IlcQAManager * qam = IlcQAManager::QAManager(IlcQAv1::kSIMMODE) ; 
  qam->SetActiveDetectors(fQADetectors) ; 
  fQATasks = Form("%d %d %d", IlcQAv1::kHITS, IlcQAv1::kSDIGITS, IlcQAv1::kDIGITS) ; 
  qam->SetTasks(fQATasks) ;
 	if (fWriteQAExpertData)
    qam->SetWriteExpert() ; 
  
  if (qam->IsDefaultStorageSet()) {
    IlcWarning("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
    IlcWarning("Default QA reference storage has been already set !");
    IlcWarning(Form("Ignoring the default storage declared in IlcSimulation: %s",fQARefUri.Data()));
    IlcWarning("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
    fQARefUri = qam->GetDefaultStorage()->GetURI();
  } else {
      if (fQARefUri.Length() > 0) {
        IlcDebug(2,"!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
        IlcDebug(2, Form("Default QA reference storage is set to: %s", fQARefUri.Data()));
        IlcDebug(2, "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
      } else {
        fQARefUri="local://$ILC_ROOT/QARef";
        IlcWarning("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
        IlcWarning("Default QA reference storage not yet set !!!!");
        IlcWarning(Form("Setting it now to: %s", fQARefUri.Data()));
        IlcWarning("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
      }
    qam->SetDefaultStorage(fQARefUri);
  }
}

//_____________________________________________________________________________
void IlcSimulation::InitCDB()
{
// activate a default CDB storage
// First check if we have any CDB storage set, because it is used 
// to retrieve the calibration and alignment constants

  IlcWarning("InitCDB method disabled");
  return;

  if (fInitCDBCalled) return;
  fInitCDBCalled = kTRUE;

  IlcCDBManager* man = IlcCDBManager::Instance();
  if (man->IsDefaultStorageSet())
  {
    IlcWarning("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
    IlcWarning("Default CDB storage has been already set !");
    IlcWarning(Form("Ignoring the default storage declared in IlcSimulation: %s",fCDBUri.Data()));
    IlcWarning("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
    fCDBUri = man->GetDefaultStorage()->GetURI();
  }
  else {
    if (fCDBUri.Length() > 0) 
    {
    	IlcDebug(2,"!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
    	IlcDebug(2, Form("Default CDB storage is set to: %s", fCDBUri.Data()));
    	IlcDebug(2, "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
    } else {
    	fCDBUri="local://$ILC_ROOT/OCDB";
    	IlcWarning("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
    	IlcWarning("Default CDB storage not yet set !!!!");
    	IlcWarning(Form("Setting it now to: %s", fCDBUri.Data()));
    	IlcWarning("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
    		
    }
    man->SetDefaultStorage(fCDBUri);
  }

  // Now activate the detector specific CDB storage locations
  for (Int_t i = 0; i < fSpecCDBUri.GetEntriesFast(); i++) {
    TObject* obj = fSpecCDBUri[i];
    if (!obj) continue;
    IlcDebug(2, "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
    IlcDebug(2, Form("Specific CDB storage for %s is set to: %s",obj->GetName(),obj->GetTitle()));
    IlcDebug(2, "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
    man->SetSpecificStorage(obj->GetName(), obj->GetTitle());
  }
      
}

//_____________________________________________________________________________
void IlcSimulation::InitRunNumber(){
// check run number. If not set, set it to 0 !!!!
  
  if (fInitRunNumberCalled) return;
  fInitRunNumberCalled = kTRUE;
  
  IlcCDBManager* man = IlcCDBManager::Instance();
  if (man->GetRun() >= 0)
  {
    	IlcFatal(Form("Run number cannot be set in IlcCDBManager before start of simulation: "
			"Use external variable DC_RUN or IlcSimulation::SetRun()!"));
  }
    
  if(fRun >= 0) {
    	IlcDebug(2, Form("Setting CDB run number to: %d",fRun));
  } else {
    	fRun=0;
    	IlcWarning(Form("Run number not yet set !!!! Setting it now to: %d",
			fRun));
  }
  man->SetRun(fRun);

  man->Print();

}

//_____________________________________________________________________________
void IlcSimulation::SetCDBLock() {
  // Set CDB lock: from now on it is forbidden to reset the run number
  // or the default storage or to activate any further storage!
  
  ULong_t key = IlcCDBManager::Instance()->SetLock(1);
  if (key) fKey = key;
}

//_____________________________________________________________________________
void IlcSimulation::SetDefaultStorage(const char* uri) {
  // Store the desired default CDB storage location
  // Activate it later within the Run() method
  
  fCDBUri = uri;
  
}

//_____________________________________________________________________________
void IlcSimulation::SetQARefDefaultStorage(const char* uri) {
  // Store the desired default CDB storage location
  // Activate it later within the Run() method
  
  fQARefUri = uri;
  IlcQAv1::SetQARefStorage(fQARefUri.Data()) ;
}

//_____________________________________________________________________________
void IlcSimulation::SetSpecificStorage(const char* calibType, const char* uri) {
// Store a detector-specific CDB storage location
// Activate it later within the Run() method

  IlcCDBPath aPath(calibType);
  if(!aPath.IsValid()){
  	IlcError(Form("Not a valid path: %s", calibType));
  	return;
  }

  TObject* obj = fSpecCDBUri.FindObject(calibType);
  if (obj) fSpecCDBUri.Remove(obj);
  fSpecCDBUri.Add(new TNamed(calibType, uri));

}

//_____________________________________________________________________________
void IlcSimulation::SetRunNumber(Int_t run)
{
// sets run number
// Activate it later within the Run() method

	fRun = run;
}

//_____________________________________________________________________________
void IlcSimulation::SetSeed(Int_t seed)
{
// sets seed number
// Activate it later within the Run() method

	fSeed = seed;
}

//_____________________________________________________________________________
Bool_t IlcSimulation::SetRunNumberFromData()
{
  // Set the CDB manager run number
  // The run number is retrieved from gIlc

    if (fSetRunNumberFromDataCalled) return kTRUE;
    fSetRunNumberFromDataCalled = kTRUE;    
  
    IlcCDBManager* man = IlcCDBManager::Instance();
    Int_t runData = -1, runCDB = -1;
  
    IlcRunLoader* runLoader = LoadRun("READ");
    if (!runLoader) return kFALSE;
    else {
    	runData = runLoader->GetHeader()->GetRun();
	delete runLoader;
    }
  
    runCDB = man->GetRun();
    if(runCDB >= 0) {
	if (runCDB != runData) {
    		IlcWarning("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
    		IlcWarning(Form("A run number was previously set in IlcCDBManager: %d !", runCDB));
    		IlcWarning(Form("It will be replaced with the run number got from run header: %d !", runData));
    		IlcWarning("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");	
	}
   	
    }
      
    man->SetRun(runData);
    fRun = runData;
    
    if(man->GetRun() < 0) {
    	IlcError("Run number not properly initalized!");
	return kFALSE;
    }
  
    man->Print();
    
    return kTRUE;
}

//_____________________________________________________________________________
void IlcSimulation::SetConfigFile(const char* fileName)
{
// set the name of the config file

  fConfigFileName = fileName;
}

//_____________________________________________________________________________
void IlcSimulation::SetGIlcFile(const char* fileName)
{
// set the name of the gilc file
// the path is converted to an absolute one if it is relative

  fGIlcFileName = fileName;
  if (!gSystem->IsAbsoluteFileName(fGIlcFileName)) {
    char* absFileName = gSystem->ConcatFileName(gSystem->WorkingDirectory(),
						fGIlcFileName);
    fGIlcFileName = absFileName;
    delete[] absFileName;
  }

  IlcDebug(2, Form("gilc file name set to %s", fileName));
}

//_____________________________________________________________________________
void IlcSimulation::SetEventsPerFile(const char* detector, const char* type, 
				     Int_t nEvents)
{
// set the number of events per file for the given detector and data type
// ("Hits", "Summable Digits", "Digits", "Reconstructed Points" or "Tracks")

  TNamed* obj = new TNamed(detector, type);
  obj->SetUniqueID(nEvents);
  fEventsPerFile.Add(obj);
}

//_____________________________________________________________________________
Bool_t IlcSimulation::MisalignGeometry(IlcRunLoader *runLoader)
{

  IlcWarning("MisalignGeometry method disabled");
  return kTRUE;
  // Read the alignment objects from CDB.
  // Each detector is supposed to have the
  // alignment objects in DET/Align/Data CDB path.
  // All the detector objects are then collected,
  // sorted by geometry level (starting from ILCM) and
  // then applied to the TGeo geometry.
  // Finally an overlaps check is performed.

  if (!IlcGeomManager::GetGeometry() || !IlcGeomManager::GetGeometry()->IsClosed()) {
    IlcError("Can't apply the misalignment! Geometry is not loaded or it is still opened!");
    return kFALSE;
  }  
  
  // initialize CDB storage, run number, set CDB lock
  InitCDB();
//  if (!SetRunNumberFromData()) if (fStopOnError) return kFALSE;
  SetCDBLock();
    
  Bool_t delRunLoader = kFALSE;
  if (!runLoader) {
    runLoader = LoadRun("READ");
    if (!runLoader) return kFALSE;
    delRunLoader = kTRUE;
  }
  
  // Export ideal geometry 
  if(!IsGeometryFromFile()) IlcGeomManager::GetGeometry()->Export("geometry.root");

  // Load alignment data from CDB and apply to geometry through IlcGeomManager
  if(fLoadAlignFromCDB){
    
    TString detStr = fLoadAlObjsListOfDets;
    TString loadAlObjsListOfDets = "";
    
    TObjArray* detArray = runLoader->GetIlcRun()->Detectors();
    for (Int_t iDet = 0; iDet < detArray->GetEntriesFast(); iDet++) {
      IlcModule* det = (IlcModule*) detArray->At(iDet);
      if (!det || !det->IsActive()) continue;
      if (IsSelected(det->GetName(), detStr)) {
        //add det to list of dets to be aligned from CDB
        loadAlObjsListOfDets += det->GetName();
        loadAlObjsListOfDets += " ";
      }
    } // end loop over detectors
    loadAlObjsListOfDets.Prepend("GRP "); //add alignment objects for non-sensitive modules
    IlcGeomManager::ApplyAlignObjsFromCDB(loadAlObjsListOfDets.Data());
  }else{
    // Check if the array with alignment objects was
    // provided by the user. If yes, apply the objects
    // to the present TGeo geometry
    if (fIlcgnObjArray) {
      if (IlcGeomManager::ApplyAlignObjsToGeom(*fIlcgnObjArray) == kFALSE) {
        IlcError("The misalignment of one or more volumes failed!"
                 "Compare the list of simulated detectors and the list of detector alignment data!");
        if (delRunLoader) delete runLoader;
        return kFALSE;
      }
    }
  }

  // Update the internal geometry of modules (ITS needs it)
  TString detStr = fLoadAlObjsListOfDets;
  TObjArray* detArray = runLoader->GetIlcRun()->Detectors();
  for (Int_t iDet = 0; iDet < detArray->GetEntriesFast(); iDet++) {

    IlcModule* det = (IlcModule*) detArray->At(iDet);
    if (!det || !det->IsActive()) continue;
    if (IsSelected(det->GetName(), detStr)) {
      det->UpdateInternalGeometry();
    }
  } // end loop over detectors


  if (delRunLoader) delete runLoader;

  return kTRUE;
}

//_____________________________________________________________________________
void IlcSimulation::MergeWith(const char* fileName, Int_t nSignalPerBkgrd)
{
// add a file with background events for merging

  TObjString* fileNameStr = new TObjString(fileName);
  fileNameStr->SetUniqueID(nSignalPerBkgrd);
  if (!fBkgrdFileNames) fBkgrdFileNames = new TObjArray;
  fBkgrdFileNames->Add(fileNameStr);
}

void IlcSimulation::EmbedInto(const char* fileName, Int_t nSignalPerBkgrd)
{
// add a file with background events for embeddin
  MergeWith(fileName, nSignalPerBkgrd);
  fEmbeddingFlag = kTRUE;
}

//_____________________________________________________________________________
Bool_t IlcSimulation::Run(Int_t nEvents)
{
// run the generation, simulation and digitization

 
  IlcCodeTimerAuto("",0)
  IlcSysInfo::AddStamp("Start_Run");
  
  // Load run number and seed from environmental vars
  ProcessEnvironmentVars();
  IlcSysInfo::AddStamp("ProcessEnvironmentVars");

  gRandom->SetSeed(fSeed);
   
  if (nEvents > 0) fNEvents = nEvents;

  // create and setup the HLT instance
  if (!fRunHLT.IsNull() && !CreateHLT()) {
    if (fStopOnError) return kFALSE;
    // disable HLT
    fRunHLT="";
  }
  
  // generation and simulation -> hits
  if (fRunGeneration) {
    if (!RunSimulation()) if (fStopOnError) return kFALSE;
  }
  IlcSysInfo::AddStamp("RunSimulation");
           
  // initialize CDB storage from external environment
  // (either CDB manager or IlcSimulation setters),
  // if not already done in RunSimulation()
  InitCDB();
  IlcSysInfo::AddStamp("InitCDB");
  
  // Set run number in CDBManager from data 
  // From this point on the run number must be always loaded from data!
  if (!SetRunNumberFromData()) if (fStopOnError) return kFALSE;
  
  // Set CDB lock: from now on it is forbidden to reset the run number
  // or the default storage or to activate any further storage!
  SetCDBLock();

  // If RunSimulation was not called, load the geometry and misalign it
  if (!IlcGeomManager::GetGeometry()) {
    // Initialize the geometry manager
    IlcGeomManager::LoadGeometry("geometry.root");
    IlcSysInfo::AddStamp("GetGeometry");
//    // Check that the consistency of symbolic names for the activated subdetectors
//    // in the geometry loaded by IlcGeomManager
//    IlcRunLoader* runLoader = LoadRun("READ");
//    if (!runLoader) return kFALSE;
//
//    TString detsToBeChecked = "";
//    TObjArray* detArray = runLoader->GetIlcRun()->Detectors();
//    for (Int_t iDet = 0; iDet < detArray->GetEntriesFast(); iDet++) {
//      IlcModule* det = (IlcModule*) detArray->At(iDet);
//      if (!det || !det->IsActive()) continue;
//      detsToBeChecked += det->GetName();
//      detsToBeChecked += " ";
//    } // end loop over detectors
//    if(!IlcGeomManager::CheckSymNamesLUT(detsToBeChecked.Data()))
    if(!IlcGeomManager::CheckSymNamesLUT("ALL"))
	IlcFatalClass("Current loaded geometry differs in the definition of symbolic names!");
	
    if (!IlcGeomManager::GetGeometry()) if (fStopOnError) return kFALSE;
    // Misalign geometry
    if(!MisalignGeometry()) if (fStopOnError) return kFALSE;
  }
  IlcSysInfo::AddStamp("MissalignGeometry");


  // hits -> summable digits
  IlcSysInfo::AddStamp("Start_sdigitization");
  if (!fMakeSDigits.IsNull()) {
    if (!RunSDigitization(fMakeSDigits)) if (fStopOnError) return kFALSE;
 
  }
  IlcSysInfo::AddStamp("Stop_sdigitization");
  
  IlcSysInfo::AddStamp("Start_digitization");  
  // summable digits -> digits  
  if (!fMakeDigits.IsNull()) {
    if (!RunDigitization(fMakeDigits, fMakeDigitsFromHits)) {
      if (fStopOnError) return kFALSE;
    }
   }
  IlcSysInfo::AddStamp("Stop_digitization");

  
  
  // hits -> digits
  if (!fMakeDigitsFromHits.IsNull()) {
    if (fBkgrdFileNames && (fBkgrdFileNames->GetEntriesFast() > 0)) {
      IlcWarning(Form("Merging and direct creation of digits from hits " 
                 "was selected for some detectors. "
                 "No merging will be done for the following detectors: %s",
                 fMakeDigitsFromHits.Data()));
    }
    if (!RunHitsDigitization(fMakeDigitsFromHits)) {
      if (fStopOnError) return kFALSE;
    }
  }

  IlcSysInfo::AddStamp("Hits2Digits");
  
  
  // digits -> trigger
  if (!fTriggerConfig.IsNull() && !RunTrigger(fTriggerConfig,fMakeDigits)) {
    if (fStopOnError) return kFALSE;
  }

  IlcSysInfo::AddStamp("RunTrigger");
  
  
  // digits -> raw data
  if (!fWriteRawData.IsNull()) {
    if (!WriteRawData(fWriteRawData, fRawDataFileName, 
		      fDeleteIntermediateFiles,fWriteSelRawData)) {
      if (fStopOnError) return kFALSE;
    }
  }

  IlcSysInfo::AddStamp("WriteRaw");
  
  // run HLT simulation on simulated digit data if raw data is not
  // simulated, otherwise its called as part of WriteRawData
  if (!fRunHLT.IsNull() && fWriteRawData.IsNull()) {
    if (!RunHLT()) {
      if (fStopOnError) return kFALSE;
    }
  }

  IlcSysInfo::AddStamp("RunHLT");
  
  //QA
  if (fRunQA) {
      Bool_t rv = RunQA() ; 
      if (!rv)
	  if (fStopOnError) 
	      return kFALSE ;
  }

  IlcSysInfo::AddStamp("RunQA");

  TString snapshotFileOut("");
  if(TString(gSystem->Getenv("OCDB_SNAPSHOT_CREATE")) == TString("kTRUE")){ 
      IlcInfo(" ******** Creating the snapshot! *********");
      TString snapshotFile(gSystem->Getenv("OCDB_SNAPSHOT_FILENAME")); 
      if(!(snapshotFile.IsNull() || snapshotFile.IsWhitespace())) 
	  snapshotFileOut = snapshotFile;
      else 
	  snapshotFileOut="OCDB.root"; 
      IlcCDBManager::Instance()->DumpToSnapshotFile(snapshotFileOut.Data(),kFALSE); 
  }

  // Cleanup of CDB manager: cache and active storages!
  IlcCDBManager::Instance()->ClearCache();

  return kTRUE;
}

//_______________________________________________________________________
Bool_t IlcSimulation::RunLego(const char *setup, Int_t nc1, Float_t c1min,
		     Float_t c1max,Int_t nc2,Float_t c2min,Float_t c2max,
		     Float_t rmin,Float_t rmax,Float_t zmax, IlcLegoGenerator* gener, Int_t nev)
{
  //
  // Generates lego plots of:
  //    - radiation length map phi vs theta
  //    - radiation length map phi vs eta
  //    - interaction length map
  //    - g/cm2 length map
  //
  //  ntheta    bins in theta, eta
  //  themin    minimum angle in theta (degrees)
  //  themax    maximum angle in theta (degrees)
  //  nphi      bins in phi
  //  phimin    minimum angle in phi (degrees)
  //  phimax    maximum angle in phi (degrees)
  //  rmin      minimum radius
  //  rmax      maximum radius
  //  
  //
  //  The number of events generated = ntheta*nphi
  //  run input parameters in macro setup (default="Config.C")
  //
  //  Use macro "lego.C" to visualize the 3 lego plots in spherical coordinates
  //Begin_Html
  /*
    <img src="picts/IlcRunLego1.gif">
  */
  //End_Html
  //Begin_Html
  /*
    <img src="picts/IlcRunLego2.gif">
  */
  //End_Html
  //Begin_Html
  /*
    <img src="picts/IlcRunLego3.gif">
  */
  //End_Html
  //

// run the generation and simulation

  IlcCodeTimerAuto("",0)

  // initialize CDB storage and run number from external environment
  // (either CDB manager or IlcSimulation setters)
  InitCDB();
  InitRunNumber();
  SetCDBLock();
  
  if (!gIlc) {
    IlcError("no gIlc object. Restart ilcroot and try again.");
    return kFALSE;
  }
  if (gIlc->Modules()->GetEntries() > 0) {
    IlcError("gIlc was already run. Restart ilcroot and try again.");
    return kFALSE;
  }

  IlcInfo(Form("initializing gIlc with config file %s",
          fConfigFileName.Data()));

  // Number of events 
    if (nev == -1) nev  = nc1 * nc2;
    
  // check if initialisation has been done
  // If runloader has been initialized, set the number of events per file to nc1 * nc2
    
  // Set new generator
  if (!gener) gener  = new IlcLegoGenerator();
  //
  // Configure Generator

  gener->SetRadiusRange(rmin, rmax);
  gener->SetZMax(zmax);
  gener->SetCoor1Range(nc1, c1min, c1max);
  gener->SetCoor2Range(nc2, c2min, c2max);
  
  
  //Create Lego object  
  fLego = new IlcLego("lego",gener);

  //__________________________________________________________________________

  gIlc->Announce();

  gROOT->LoadMacro(setup);
  gInterpreter->ProcessLine(gIlc->GetConfigFunction());

  if(IlcCDBManager::Instance()->GetRun() >= 0) { 
    SetRunNumber(IlcCDBManager::Instance()->GetRun());
  } else {
    IlcWarning("Run number not initialized!!");
  }

  IlcRunLoader::Instance()->CdGAFile();
  
  IlcPDG::AddParticlesToPdgDataBase();  
  
  gMC->SetMagField(TGeoGlobalMagField::Instance()->GetField());

  gIlc->GetMCApp()->Init();
  
  
  //Must be here because some MCs (G4) adds detectors here and not in Config.C
  gIlc->InitLoaders();
  IlcRunLoader::Instance()->MakeTree("E");
  
  //
  // Save stuff at the beginning of the file to avoid file corruption
  IlcRunLoader::Instance()->CdGAFile();
  gIlc->Write();

  //Save current generator
  IlcGenerator *gen=gIlc->GetMCApp()->Generator();
  gIlc->GetMCApp()->ResetGenerator(gener);
  //Prepare MC for Lego Run
  gMC->InitLego();
  
  //Run Lego Object
  
  
  IlcRunLoader::Instance()->SetNumberOfEventsPerFile(nev);
  gMC->ProcessRun(nev);
  
  // End of this run, close files
  FinishRun();
  // Restore current generator
  gIlc->GetMCApp()->ResetGenerator(gen);
  // Delete Lego Object
  delete fLego;

  return kTRUE;
}

//_____________________________________________________________________________
Bool_t IlcSimulation::RunTrigger(const char* config, const char* detectors)
{
  // run the trigger

  IlcCodeTimerAuto("",0)

  // initialize CDB storage from external environment
  // (either CDB manager or IlcSimulation setters),
  // if not already done in RunSimulation()
  InitCDB();
  
  // Set run number in CDBManager from data 
  // From this point on the run number must be always loaded from data!
  if (!SetRunNumberFromData()) if (fStopOnError) return kFALSE;
  
  // Set CDB lock: from now on it is forbidden to reset the run number
  // or the default storage or to activate any further storage!
  SetCDBLock();
   
   IlcRunLoader* runLoader = LoadRun("READ");
   if (!runLoader) return kFALSE;
   TString trconfiguration = config;

   if (trconfiguration.IsNull()) {
     if(!fTriggerConfig.IsNull()) {
       trconfiguration = fTriggerConfig;
     }
     else
       IlcWarning("No trigger descriptor is specified. Loading the one that is in the CDB.");
   }

   runLoader->MakeTree( "GG" );
   IlcCentralTrigger* aCTP = runLoader->GetTrigger();
   // Load Configuration
   if (!aCTP->LoadConfiguration( trconfiguration ))
     return kFALSE;

   // digits -> trigger
   if( !aCTP->RunTrigger( runLoader , detectors ) ) {
      if (fStopOnError) {
	//  delete aCTP;
	return kFALSE;
      }
   }

   delete runLoader;

   return kTRUE;
}

//_____________________________________________________________________________
Bool_t IlcSimulation::WriteTriggerRawData()
{
  // Writes the CTP (trigger) DDL raw data
  // Details of the format are given in the
  // trigger TDR - pages 134 and 135.
  IlcCTPRawData writer;
  writer.RawData();

  return kTRUE;
}

//_____________________________________________________________________________
Bool_t IlcSimulation::RunSimulation(Int_t nEvents)
{
// run the generation and simulation

  IlcCodeTimerAuto("",0)

  // initialize CDB storage and run number from external environment
  // (either CDB manager or IlcSimulation setters)
  IlcSysInfo::AddStamp("RunSimulation_Begin");
  InitCDB();
  IlcSysInfo::AddStamp("RunSimulation_InitCDB");
  InitRunNumber();

  SetCDBLock();
  IlcSysInfo::AddStamp("RunSimulation_SetCDBLock");
  
  if (!gIlc) {
    IlcError("no gIlc object. Restart ilcroot and try again.");
    return kFALSE;
  }
  if (gIlc->Modules()->GetEntries() > 0) {
    IlcError("gIlc was already run. Restart ilcroot and try again.");
    return kFALSE;
  }

  IlcInfo(Form("initializing gIlc with config file %s",
          fConfigFileName.Data()));

  //
  // Initialize ILC Simulation run
  //
  gIlc->Announce();

  //
  // If requested set the mag. field from the GRP entry.
  // After this the field is loccked and cannot be changed by Config.C
  if (fUseMagFieldFromGRP) {
      IlcGRPManager grpM;
      grpM.ReadGRPEntry();
      grpM.SetMagField();
      IlcInfo("Field is locked now. It cannot be changed in Config.C");
  }
//
// Execute Config.C
  TInterpreter::EErrorCode interpreterError=TInterpreter::kNoError;
  gROOT->LoadMacro(fConfigFileName.Data());
  Long_t interpreterResult=gInterpreter->ProcessLine(gIlc->GetConfigFunction(), &interpreterError);
  if (interpreterResult!=0 || interpreterError!=TInterpreter::kNoError) {
    IlcFatal(Form("execution of config file \"%s\" failed with error %d", fConfigFileName.Data(), (int)interpreterError));
  }
  IlcSysInfo::AddStamp("RunSimulation_Config");

//
// If requested obtain the vertex position and vertex sigma_z from the CDB
// This overwrites the settings from the Config.C  
  if (fUseVertexFromCDB) {
      Double_t vtxPos[3] = {0., 0., 0.}; 
      Double_t vtxSig[3] = {0., 0., 0.};
      IlcCDBEntry* entry = IlcCDBManager::Instance()->Get("GRP/Calib/MeanVertex");
      if (entry) {
	  IlcESDVertex* vertex = dynamic_cast<IlcESDVertex*> (entry->GetObject());
	  if (vertex) {
	      if(vertex->GetXRes()>2.8) { // > pipe radius --> it's a dummy object, don't use it 
		  entry = IlcCDBManager::Instance()->Get("GRP/Calib/MeanVertexSPD");
		  if (entry) vertex = dynamic_cast<IlcESDVertex*> (entry->GetObject());
	      }
	  }
	  if (vertex) {
	      vertex->GetXYZ(vtxPos);
	      vertex->GetSigmaXYZ(vtxSig);
	      IlcInfo("Overwriting Config.C vertex settings !");
	      IlcInfo(Form("Vertex position from OCDB entry: x = %13.3f, y = %13.3f, z = %13.3f (sigma = %13.3f)\n",
			   vtxPos[0], vtxPos[1], vtxPos[2], vtxSig[2]));
	      
	      IlcGenerator *gen = gIlc->GetMCApp()->Generator();
	      gen->SetOrigin(vtxPos[0], vtxPos[1], vtxPos[2]);   // vertex position
	      gen->SetSigmaZ(vtxSig[2]);
	  }
      }
  }

  // If requested we take the SOR and EOR time-stamps from the GRP and use them
  // in order to generate the event time-stamps
  if (fUseTimeStampFromCDB) {
    IlcGRPManager grpM;
    grpM.ReadGRPEntry();
    const IlcGRPObject *grpObj = grpM.GetGRPData();
    if (!grpObj || (grpObj->GetTimeEnd() <= grpObj->GetTimeStart())) {
      IlcError("Missing GRP or bad SOR/EOR time-stamps! Switching off the time-stamp generation from GRP!");
      fTimeStart = fTimeEnd = 0;
      fUseTimeStampFromCDB = kFALSE;
    }
    else {
      fTimeStart = grpObj->GetTimeStart();
      fTimeEnd = grpObj->GetTimeEnd();
    }
  }
  
  if(IlcCDBManager::Instance()->GetRun() >= 0) { 
    IlcRunLoader::Instance()->SetRunNumber(IlcCDBManager::Instance()->GetRun());
    IlcRunLoader::Instance()->SetNumberOfEventsPerRun(fNEvents);
  } else {
  	IlcWarning("Run number not initialized!!");
  }
  
   IlcRunLoader::Instance()->CdGAFile();
   

   IlcPDG::AddParticlesToPdgDataBase();  

   gMC->SetMagField(TGeoGlobalMagField::Instance()->GetField());
   IlcSysInfo::AddStamp("RunSimulation_GetField");
   
   gIlc->GetMCApp()->Init();
   IlcSysInfo::AddStamp("RunSimulation_InitMCApp");

   //Must be here because some MCs (G4) adds detectors here and not in Config.C
   gIlc->InitLoaders();
   IlcRunLoader::Instance()->MakeTree("E");
   IlcRunLoader::Instance()->LoadKinematics("RECREATE");
   IlcRunLoader::Instance()->LoadTrackRefs("RECREATE");
   IlcRunLoader::Instance()->LoadHits("all","RECREATE");
   //
   // Save stuff at the beginning of the file to avoid file corruption
   IlcRunLoader::Instance()->CdGAFile();
   gIlc->Write();
   gIlc->SetEventNrInRun(-1); //important - we start Begin event from increasing current number in run
   IlcSysInfo::AddStamp("RunSimulation_InitLoaders");
  //___________________________________________________________________________________________
  
  IlcSysInfo::AddStamp("RunSimulation_TriggerDescriptor");

  // Set run number in CDBManager
  IlcInfo(Form("Run number: %d",IlcCDBManager::Instance()->GetRun()));

  IlcRunLoader* runLoader = IlcRunLoader::Instance();
  if (!runLoader) {
             IlcError(Form("gIlc has no run loader object. "
        		     "Check your config file: %s", fConfigFileName.Data()));
             return kFALSE;
  }
  SetGIlcFile(runLoader->GetFileName());
      
  // Misalign geometry
#if ROOT_VERSION_CODE < 331527
  IlcGeomManager::SetGeometry(gGeoManager);
  
  // Check that the consistency of symbolic names for the activated subdetectors
  // in the geometry loaded by IlcGeomManager
  TString detsToBeChecked = "";
  TObjArray* detArray = runLoader->GetIlcRun()->Detectors();
  for (Int_t iDet = 0; iDet < detArray->GetEntriesFast(); iDet++) {
    IlcModule* det = (IlcModule*) detArray->At(iDet);
    if (!det || !det->IsActive()) continue;
    detsToBeChecked += det->GetName();
    detsToBeChecked += " ";
  } // end loop over detectors
  if(!IlcGeomManager::CheckSymNamesLUT(detsToBeChecked.Data()))
    IlcFatalClass("Current loaded geometry differs in the definition of symbolic names!");
  MisalignGeometry(runLoader);
  IlcSysInfo::AddStamp("RunSimulation_MisalignGeometry");
#endif

//   IlcRunLoader* runLoader = IlcRunLoader::Instance();
//   if (!runLoader) {
//     IlcError(Form("gIlc has no run loader object. "
//                   "Check your config file: %s", fConfigFileName.Data()));
//     return kFALSE;
//   }
//   SetGIlcFile(runLoader->GetFileName());

  if (!gIlc->GetMCApp()->Generator()) {
    IlcError(Form("gIlc has no generator object. "
                  "Check your config file: %s", fConfigFileName.Data()));
    return kFALSE;
  }

  // Write GRP entry corresponding to the setting found in Cofig.C
  if (fWriteGRPEntry)
    WriteGRPEntry();
  IlcSysInfo::AddStamp("RunSimulation_WriteGRP");

  if (nEvents <= 0) nEvents = fNEvents;

  // get vertex from background file in case of merging
  if (fUseBkgrdVertex &&
      fBkgrdFileNames && (fBkgrdFileNames->GetEntriesFast() > 0)) {
    Int_t signalPerBkgrd = GetNSignalPerBkgrd(nEvents);
    const char* fileName = ((TObjString*)
			    (fBkgrdFileNames->At(0)))->GetName();
    IlcInfo(Form("The vertex will be taken from the background "
                 "file %s with nSignalPerBackground = %d", 
                 fileName, signalPerBkgrd));
    IlcVertexGenFile* vtxGen = new IlcVertexGenFile(fileName, signalPerBkgrd);
    gIlc->GetMCApp()->Generator()->SetVertexGenerator(vtxGen);
  }

  if (!fRunSimulation) {
    gIlc->GetMCApp()->Generator()->SetTrackingFlag(0);
  }

  // set the number of events per file for given detectors and data types
  for (Int_t i = 0; i < fEventsPerFile.GetEntriesFast(); i++) {
    if (!fEventsPerFile[i]) continue;
    const char* detName = fEventsPerFile[i]->GetName();
    const char* typeName = fEventsPerFile[i]->GetTitle();
    TString loaderName(detName);
    loaderName += "Loader";
    IlcLoader* loader = runLoader->GetLoader(loaderName);
    if (!loader) {
      IlcError(Form("RunSimulation no loader for %s found\n Number of events per file not set for %s %s", 
                    detName, typeName, detName));
      continue;
    }
    IlcDataLoader* dataLoader = 
      loader->GetDataLoader(typeName);
    if (!dataLoader) {
      IlcError(Form("no data loader for %s found\n"
                    "Number of events per file not set for %s %s", 
                    typeName, detName, typeName));
      continue;
    }
    dataLoader->SetNumberOfEventsPerFile(fEventsPerFile[i]->GetUniqueID());
    IlcDebug(1, Form("number of events per file set to %d for %s %s",
                     fEventsPerFile[i]->GetUniqueID(), detName, typeName));
  }

  IlcInfo("running gIlc");
  IlcSysInfo::AddStamp("Start_ProcessRun");

  // Create the Root Tree with one branch per detector
  //Hits moved to begin event -> now we are crating separate tree for each event

  gMC->ProcessRun(nEvents);

  // End of this run, close files
  if(nEvents>0) FinishRun();

  IlcSysInfo::AddStamp("Stop_ProcessRun");
  delete runLoader;

  return kTRUE;
}

//_____________________________________________________________________________
Bool_t IlcSimulation::RunSDigitization(const char* detectors)
{
// run the digitization and produce summable digits
  static Int_t eventNr=0;
  IlcCodeTimerAuto("",0) ;

  // initialize CDB storage, run number, set CDB lock
  InitCDB();
  if (!SetRunNumberFromData()) if (fStopOnError) return kFALSE;
  SetCDBLock();
  
  IlcRunLoader* runLoader = LoadRun();
  if (!runLoader) return kFALSE;

  TString detStr = detectors;
  TObjArray* detArray = runLoader->GetIlcRun()->Detectors();
  for (Int_t iDet = 0; iDet < detArray->GetEntriesFast(); iDet++) {
    IlcModule* det = (IlcModule*) detArray->At(iDet);
    if (!det || !det->IsActive()) continue;
    if (IsSelected(det->GetName(), detStr)) {
      IlcInfo(Form("creating summable digits for %s", det->GetName()));
      IlcCodeTimerStart(Form("creating summable digits for %s", det->GetName()));
      det->Hits2SDigits();
      IlcCodeTimerStop(Form("creating summable digits for %s", det->GetName()));
      IlcSysInfo::AddStamp(Form("Digit_%s_%d",det->GetName(),eventNr), 0,1, eventNr);
    }
  }

  if ((detStr.CompareTo("ALL") != 0) && !detStr.IsNull()) {
    IlcError(Form("the following detectors were not found: %s",
                  detStr.Data()));
    if (fStopOnError) return kFALSE;
  }
  eventNr++;
  delete runLoader;

  return kTRUE;
}


//_____________________________________________________________________________
Bool_t IlcSimulation::RunDigitization(const char* detectors, 
				      const char* excludeDetectors)
{
// run the digitization and produce digits from sdigits

  IlcCodeTimerAuto("",0)

  // initialize CDB storage, run number, set CDB lock
  InitCDB();
  if (!SetRunNumberFromData()) if (fStopOnError) return kFALSE;
  SetCDBLock();
  
  delete IlcRunLoader::Instance();
  delete gIlc;
  gIlc = NULL;

  Int_t nStreams = 1;
  if (fBkgrdFileNames) nStreams = fBkgrdFileNames->GetEntriesFast() + 1;
  Int_t signalPerBkgrd = GetNSignalPerBkgrd();
  IlcDigitizationInput digInp(nStreams, signalPerBkgrd);
  // digInp.SetEmbeddingFlag(fEmbeddingFlag);
  digInp.SetRegionOfInterest(fRegionOfInterest);
  digInp.SetInputStream(0, fGIlcFileName.Data());
  for (Int_t iStream = 1; iStream < nStreams; iStream++) {
    const char* fileName = ((TObjString*)(fBkgrdFileNames->At(iStream-1)))->GetName();
    digInp.SetInputStream(iStream, fileName);
  }
  TObjArray detArr;
  detArr.SetOwner(kTRUE);
  TString detStr = detectors;
  TString detExcl = excludeDetectors;
  if (!static_cast<IlcStream*>(digInp.GetInputStream(0))->ImportgIlc()) {
    IlcError("Error occured while getting gIlc from Input 0");
    return kFALSE;
  }
  IlcRunLoader* runLoader = IlcRunLoader::GetRunLoader(digInp.GetInputStream(0)->GetFolderName());
  SetNumberOfEvents(runLoader->GetNumberOfEvents());
  TObjArray* detArray = runLoader->GetIlcRun()->Detectors();
  for (Int_t iDet = 0; iDet < detArray->GetEntriesFast(); iDet++) {
    IlcModule* det = (IlcModule*) detArray->At(iDet);
    if (!det || !det->IsActive()) continue;
    if (!IsSelected(det->GetName(), detStr) || IsSelected(det->GetName(), detExcl)) continue;
    IlcDigitizer* digitizer = det->CreateDigitizer(&digInp);
    if (!digitizer || !digitizer->Init()) {
      IlcError(Form("no digitizer for %s", det->GetName()));
      if (fStopOnError) return kFALSE;
      else continue;
    }
    detArr.AddLast(digitizer);    
    IlcInfo(Form("Created digitizer from SDigits -> Digits for %s", det->GetName()));    
  }
  //
  if ((detStr.CompareTo("ALL") != 0) && !detStr.IsNull()) {
    IlcError(Form("the following detectors were not found: %s", detStr.Data()));
    if (fStopOnError) return kFALSE;
  }
  //
  Int_t ndigs = detArr.GetEntriesFast();
  Int_t eventsCreated = 0;
  IlcRunLoader* outRl =  digInp.GetOutRunLoader();
  while ((eventsCreated++ < fNEvents) || (fNEvents < 0)) {
    if (!digInp.ConnectInputTrees()) break;
    digInp.InitEvent(); //this must be after call of Connect Input tress.
    if (outRl) outRl->SetEventNumber(eventsCreated-1);
    static_cast<IlcStream*>(digInp.GetInputStream(0))->ImportgIlc(); // use gIlc of the first input stream
    for (int id=0;id<ndigs;id++) ((IlcDigitizer*)detArr[id])->Digitize("");
    digInp.FinishEvent();
  };
  digInp.FinishGlobal();
  //
  return kTRUE;
}

//_____________________________________________________________________________
Bool_t IlcSimulation::RunHitsDigitization(const char* detectors)
{
// run the digitization and produce digits from hits

  IlcCodeTimerAuto("",0)

  // initialize CDB storage, run number, set CDB lock
  InitCDB();
  if (!SetRunNumberFromData()) if (fStopOnError) return kFALSE;
  SetCDBLock();
  
  IlcRunLoader* runLoader = LoadRun("READ");
  if (!runLoader) return kFALSE;

  TString detStr = detectors;
  TObjArray* detArray = runLoader->GetIlcRun()->Detectors();
  for (Int_t iDet = 0; iDet < detArray->GetEntriesFast(); iDet++) {
    IlcModule* det = (IlcModule*) detArray->At(iDet);
    if (!det || !det->IsActive()) continue;
    if (IsSelected(det->GetName(), detStr)) {
      IlcInfo(Form("creating digits from hits for %s", det->GetName()));
      det->Hits2Digits();
    }
  }

  if ((detStr.CompareTo("ALL") != 0) && !detStr.IsNull()) {
    IlcError(Form("the following detectors were not found: %s", 
                  detStr.Data()));
    if (fStopOnError) return kFALSE;
  }

  return kTRUE;
}

//_____________________________________________________________________________
Bool_t IlcSimulation::WriteRawData(const char* detectors, 
				   const char* fileName,
				   Bool_t deleteIntermediateFiles,
				   Bool_t selrawdata)
{
// convert the digits to raw data
// First DDL raw data files for the given detectors are created.
// If a file name is given, the DDL files are then converted to a DATE file.
// If deleteIntermediateFiles is true, the DDL raw files are deleted 
// afterwards.
// If the file name has the extension ".root", the DATE file is converted
// to a root file.
// If deleteIntermediateFiles is true, the DATE file is deleted afterwards.
// 'selrawdata' flag can be used to enable writing of detectors raw data
// accoring to the trigger cluster.

  IlcCodeTimerAuto("",0)
  IlcSysInfo::AddStamp("WriteRawData_Start");
  
  TString detStr = detectors;
  if (!WriteRawFiles(detStr.Data())) {
    if (fStopOnError) return kFALSE;
  }
  IlcSysInfo::AddStamp("WriteRawFiles");

  // run HLT simulation on simulated DDL raw files
  // and produce HLT ddl raw files to be included in date/root file
  // bugfix 2009-06-26: the decision whether to write HLT raw data
  // is taken in RunHLT. Here HLT always needs to be run in order to
  // create HLT digits, unless its switched off. This is due to the
  // special placement of the HLT between the generation of DDL files
  // and conversion to DATE/Root file.
  detStr.ReplaceAll("HLT", "");
  if (!fRunHLT.IsNull()) {
    if (!RunHLT()) {
      if (fStopOnError) return kFALSE;
    }
  }
  IlcSysInfo::AddStamp("WriteRawData_RunHLT");

  TString dateFileName(fileName);
  if (!dateFileName.IsNull()) {
    Bool_t rootOutput = dateFileName.EndsWith(".root");
    if (rootOutput) dateFileName += ".date";
    TString selDateFileName;
    if (selrawdata) {
      selDateFileName = "selected.";
      selDateFileName+= dateFileName;
    }
    if (!ConvertRawFilesToDate(dateFileName,selDateFileName)) {
      if (fStopOnError) return kFALSE;
    }
    IlcSysInfo::AddStamp("ConvertRawFilesToDate");
    if (deleteIntermediateFiles) {
      IlcRunLoader* runLoader = LoadRun("READ");
      if (runLoader) for (Int_t iEvent = 0; 
			  iEvent < runLoader->GetNumberOfEvents(); iEvent++) {
	char command[256];
	snprintf(command, 256, "rm -r raw%d", iEvent);
	gSystem->Exec(command);
      }
      delete runLoader;
    }

    if (rootOutput) {
      if (!ConvertDateToRoot(dateFileName, fileName)) {
	if (fStopOnError) return kFALSE;
      }
      IlcSysInfo::AddStamp("ConvertDateToRoot");
      if (deleteIntermediateFiles) {
	gSystem->Unlink(dateFileName);
      }
      if (selrawdata) {
	TString selFileName = "selected.";
	selFileName        += fileName;
	if (!ConvertDateToRoot(selDateFileName, selFileName)) {
	  if (fStopOnError) return kFALSE;
	}
	if (deleteIntermediateFiles) {
	  gSystem->Unlink(selDateFileName);
	}
      }
    }
  }

  return kTRUE;
}

//_____________________________________________________________________________
Bool_t IlcSimulation::WriteRawFiles(const char* detectors)
{
// convert the digits to raw data DDL files

  IlcCodeTimerAuto("",0)
  
  IlcRunLoader* runLoader = LoadRun("READ");
  if (!runLoader) return kFALSE;

  // write raw data to DDL files
  for (Int_t iEvent = 0; iEvent < runLoader->GetNumberOfEvents(); iEvent++) {
    IlcInfo(Form("processing event %d", iEvent));
    runLoader->GetEvent(iEvent);
    TString baseDir = gSystem->WorkingDirectory();
    char dirName[256];
    snprintf(dirName, 256, "raw%d", iEvent);
    gSystem->MakeDirectory(dirName);
    if (!gSystem->ChangeDirectory(dirName)) {
      IlcError(Form("couldn't change to directory %s", dirName));
      if (fStopOnError) return kFALSE; else continue;
    }

    ofstream runNbFile(Form("run%u",runLoader->GetHeader()->GetRun()));
    runNbFile.close();

    TString detStr = detectors;
    if (IsSelected("HLT", detStr)) {
      // Do nothing. "HLT" will be removed from detStr and HLT raw
      // data files are generated in RunHLT.
    }

    TObjArray* detArray = runLoader->GetIlcRun()->Detectors();
    for (Int_t iDet = 0; iDet < detArray->GetEntriesFast(); iDet++) {
      IlcModule* det = (IlcModule*) detArray->At(iDet);
      if (!det || !det->IsActive()) continue;
      if (IsSelected(det->GetName(), detStr)) {
	IlcInfo(Form("creating raw data from digits for %s", det->GetName()));
	det->Digits2Raw();
      }
    }

    if (!WriteTriggerRawData())
      if (fStopOnError) return kFALSE;

    gSystem->ChangeDirectory(baseDir);
    if ((detStr.CompareTo("ALL") != 0) && !detStr.IsNull()) {
      IlcError(Form("the following detectors were not found: %s", 
                    detStr.Data()));
      if (fStopOnError) return kFALSE;
    }
  }

  delete runLoader;
  
  return kTRUE;
}

//_____________________________________________________________________________
Bool_t IlcSimulation::ConvertRawFilesToDate(const char* dateFileName,
					    const char* selDateFileName)
{
// convert raw data DDL files to a DATE file with the program "dateStream"
// The second argument is not empty when the user decides to write
// the detectors raw data according to the trigger cluster.

  IlcCodeTimerAuto("",0)
  
  char* path = gSystem->Which(gSystem->Getenv("PATH"), "dateStream");
  if (!path) {
    IlcError("the program dateStream was not found");
    if (fStopOnError) return kFALSE;
  } else {
    delete[] path;
  }

  IlcRunLoader* runLoader = LoadRun("READ");
  if (!runLoader) return kFALSE;

  IlcInfo(Form("converting raw data DDL files to DATE file %s", dateFileName));
  Bool_t selrawdata = kFALSE;
  if (strcmp(selDateFileName,"") != 0) selrawdata = kTRUE;

  char command[256];
  // Note the option -s. It is used in order to avoid
  // the generation of SOR/EOR events.
  snprintf(command, 256, "dateStream -c -s -D -o %s -# %d -C -run %d", 
	  dateFileName, runLoader->GetNumberOfEvents(),runLoader->GetHeader()->GetRun());
  FILE* pipe = gSystem->OpenPipe(command, "w");

  if (!pipe) {
    IlcError(Form("Cannot execute command: %s",command));
    return kFALSE;
  }

  Int_t selEvents = 0;
  for (Int_t iEvent = 0; iEvent < runLoader->GetNumberOfEvents(); iEvent++) {

    UInt_t detectorPattern = 0;
    runLoader->GetEvent(iEvent);
    if (!runLoader->LoadTrigger()) {
      IlcCentralTrigger *aCTP = runLoader->GetTrigger();
      detectorPattern = aCTP->GetClusterMask();
      // Check if the event was triggered by CTP
      if (selrawdata) {
	if (aCTP->GetClassMask()) selEvents++;
      }
    }
    else {
      IlcWarning("No trigger can be loaded! Some fields in the event header will be empty !");
      if (selrawdata) {
	IlcWarning("No trigger can be loaded! Writing of selected raw data is abandoned !");
	selrawdata = kFALSE;
      }
    }

    fprintf(pipe, "GDC DetectorPattern %u Timestamp %ld\n", detectorPattern, runLoader->GetHeader()->GetTimeStamp());
    Float_t ldc = 0;
    Int_t prevLDC = -1;

    // loop over detectors and DDLs
    for (Int_t iDet = 0; iDet < IlcDAQ::kNDetectors; iDet++) {
      for (Int_t iDDL = 0; iDDL < IlcDAQ::NumberOfDdls(iDet); iDDL++) {

        Int_t ddlID = IlcDAQ::DdlID(iDet,iDDL);
        Int_t ldcID = Int_t(ldc + 0.0001);
        ldc += IlcDAQ::NumberOfLdcs(iDet) / IlcDAQ::NumberOfDdls(iDet);

        char rawFileName[256];
        snprintf(rawFileName, 256, "raw%d/%s", 
                iEvent, IlcDAQ::DdlFileName(iDet,iDDL));

	// check existence and size of raw data file
        FILE* file = fopen(rawFileName, "rb");
        if (!file) continue;
        fseek(file, 0, SEEK_END);
        unsigned long size = ftell(file);
	fclose(file);
        if (!size) continue;

        if (ldcID != prevLDC) {
          fprintf(pipe, " LDC Id %d\n", ldcID);
          prevLDC = ldcID;
        }
        fprintf(pipe, "  Equipment Id %d Payload %s\n", ddlID, rawFileName);
      }
    }
  }

  Int_t result = gSystem->ClosePipe(pipe);

  if (!(selrawdata && selEvents > 0)) {
    delete runLoader;
    return (result == 0);
  }

  IlcInfo(Form("converting selected by trigger cluster raw data DDL files to DATE file %s", selDateFileName));
  
  snprintf(command, 256, "dateStream -c -s -D -o %s -# %d -C -run %d", 
	  selDateFileName,selEvents,runLoader->GetHeader()->GetRun());
  FILE* pipe2 = gSystem->OpenPipe(command, "w");

  for (Int_t iEvent = 0; iEvent < runLoader->GetNumberOfEvents(); iEvent++) {

    // Get the trigger decision and cluster
    UInt_t detectorPattern = 0;
    TString detClust;
    runLoader->GetEvent(iEvent);
    if (!runLoader->LoadTrigger()) {
      IlcCentralTrigger *aCTP = runLoader->GetTrigger();
      if (aCTP->GetClassMask() == 0) continue;
      detectorPattern = aCTP->GetClusterMask();
      detClust = IlcDAQ::ListOfTriggeredDetectors(detectorPattern);
      IlcInfo(Form("List of detectors to be read out: %s",detClust.Data()));
    }

    fprintf(pipe2, "GDC DetectorPattern %u Timestamp %ld\n", detectorPattern, runLoader->GetHeader()->GetTimeStamp());
    Float_t ldc = 0;
    Int_t prevLDC = -1;

    // loop over detectors and DDLs
    for (Int_t iDet = 0; iDet < IlcDAQ::kNDetectors; iDet++) {
      // Write only raw data from detectors that
      // are contained in the trigger cluster(s)
      if (!IsSelected(IlcDAQ::DetectorName(iDet),detClust)) continue;

      for (Int_t iDDL = 0; iDDL < IlcDAQ::NumberOfDdls(iDet); iDDL++) {

        Int_t ddlID = IlcDAQ::DdlID(iDet,iDDL);
        Int_t ldcID = Int_t(ldc + 0.0001);
        ldc += IlcDAQ::NumberOfLdcs(iDet) / IlcDAQ::NumberOfDdls(iDet);

        char rawFileName[256];
        snprintf(rawFileName, 256, "raw%d/%s", 
                iEvent, IlcDAQ::DdlFileName(iDet,iDDL));

	// check existence and size of raw data file
        FILE* file = fopen(rawFileName, "rb");
        if (!file) continue;
        fseek(file, 0, SEEK_END);
        unsigned long size = ftell(file);
	fclose(file);
        if (!size) continue;

        if (ldcID != prevLDC) {
          fprintf(pipe2, " LDC Id %d\n", ldcID);
          prevLDC = ldcID;
        }
        fprintf(pipe2, "  Equipment Id %d Payload %s\n", ddlID, rawFileName);
      }
    }
  }

  Int_t result2 = gSystem->ClosePipe(pipe2);

  delete runLoader;
  return ((result == 0) && (result2 == 0));
}

//_____________________________________________________________________________
Bool_t IlcSimulation::ConvertDateToRoot(const char* dateFileName,
					const char* rootFileName)
{
// convert a DATE file to a root file with the program "ilcmdc"

  // ILCMDC setup
  const Int_t kDBSize = 2000000000;
  const Int_t kTagDBSize = 1000000000;
  const Bool_t kFilter = kFALSE;
  const Int_t kCompression = 1;

  char* path = gSystem->Which(gSystem->Getenv("PATH"), "ilcmdc");
  if (!path) {
    IlcError("the program ilcmdc was not found");
    if (fStopOnError) return kFALSE;
  } else {
    delete[] path;
  }

  IlcInfo(Form("converting DATE file %s to root file %s", 
               dateFileName, rootFileName));

  const char* rawDBFS[2] = { "/tmp/mdc1", "/tmp/mdc2" };
  const char* tagDBFS    = "/tmp/mdc1/tags";

  // User defined file system locations
  if (gSystem->Getenv("ILCMDC_RAWDB1")) 
    rawDBFS[0] = gSystem->Getenv("ILCMDC_RAWDB1");
  if (gSystem->Getenv("ILCMDC_RAWDB2")) 
    rawDBFS[1] = gSystem->Getenv("ILCMDC_RAWDB2");
  if (gSystem->Getenv("ILCMDC_TAGDB")) 
    tagDBFS = gSystem->Getenv("ILCMDC_TAGDB");

  gSystem->Exec(Form("rm -rf %s",rawDBFS[0]));
  gSystem->Exec(Form("rm -rf %s",rawDBFS[1]));
  gSystem->Exec(Form("rm -rf %s",tagDBFS));

  gSystem->Exec(Form("mkdir %s",rawDBFS[0]));
  gSystem->Exec(Form("mkdir %s",rawDBFS[1]));
  gSystem->Exec(Form("mkdir %s",tagDBFS));

  Int_t result = gSystem->Exec(Form("ilcmdc %d %d %d %d %s", 
				    kDBSize, kTagDBSize, kFilter, kCompression, dateFileName));
  gSystem->Exec(Form("mv %s/*.root %s", rawDBFS[0], rootFileName));

  gSystem->Exec(Form("rm -rf %s",rawDBFS[0]));
  gSystem->Exec(Form("rm -rf %s",rawDBFS[1]));
  gSystem->Exec(Form("rm -rf %s",tagDBFS));

  return (result == 0);
}


//_____________________________________________________________________________
IlcRunLoader* IlcSimulation::LoadRun(const char* mode) const
{
// delete existing run loaders, open a new one and load gIlc

  delete IlcRunLoader::Instance();
  IlcRunLoader* runLoader = 
    IlcRunLoader::Open(fGIlcFileName.Data(), 
		       IlcConfig::GetDefaultEventFolderName(), mode);
  if (!runLoader) {
    IlcError(Form("no run loader found in file %s", fGIlcFileName.Data()));
    return NULL;
  }
  runLoader->LoadgIlc();
  runLoader->LoadHeader();
  gIlc = runLoader->GetIlcRun();
  if (!gIlc) {
    IlcError(Form("no gIlc object found in file %s", 
                  fGIlcFileName.Data()));
    return NULL;
  }
  return runLoader;
}

//_____________________________________________________________________________
Int_t IlcSimulation::GetNSignalPerBkgrd(Int_t nEvents) const
{
// get or calculate the number of signal events per background event

  if (!fBkgrdFileNames) return 1;
  Int_t nBkgrdFiles = fBkgrdFileNames->GetEntriesFast();
  if (nBkgrdFiles == 0) return 1;

  // get the number of signal events
  if (nEvents <= 0) {
    IlcRunLoader* runLoader = 
	IlcRunLoader::Open(fGIlcFileName.Data(), "SIGNAL");
    if (!runLoader) return 1;
    
    nEvents = runLoader->GetNumberOfEvents();
    delete runLoader;
  }

  Int_t result = 0;
  for (Int_t iBkgrdFile = 0; iBkgrdFile < nBkgrdFiles; iBkgrdFile++) {
    // get the number of background events
    const char* fileName = ((TObjString*)
			    (fBkgrdFileNames->At(iBkgrdFile)))->GetName();
    IlcRunLoader* runLoader =
      IlcRunLoader::Open(fileName, "BKGRD");
    if (!runLoader) continue;
    Int_t nBkgrdEvents = runLoader->GetNumberOfEvents();
    delete runLoader;
  
    // get or calculate the number of signal per background events
    Int_t nSignalPerBkgrd = fBkgrdFileNames->At(iBkgrdFile)->GetUniqueID();
    if (nSignalPerBkgrd <= 0) {
      nSignalPerBkgrd = (nEvents-1) / nBkgrdEvents + 1;
    } else if (result && (result != nSignalPerBkgrd)) {
      IlcInfo(Form("the number of signal events per background event "
                   "will be changed from %d to %d for stream %d", 
                   nSignalPerBkgrd, result, iBkgrdFile+1));
      nSignalPerBkgrd = result;
    }

    if (!result) result = nSignalPerBkgrd;
    if (nSignalPerBkgrd * nBkgrdEvents < nEvents) {
      IlcWarning(Form("not enough background events (%d) for %d signal events "
                      "using %d signal per background events for stream %d",
                      nBkgrdEvents, nEvents, nSignalPerBkgrd, iBkgrdFile+1));
    }
  }

  return result;
}

//_____________________________________________________________________________
Bool_t IlcSimulation::IsSelected(TString detName, TString& detectors) const
{
// check whether detName is contained in detectors
// if yes, it is removed from detectors

  // check if all detectors are selected
  if ((detectors.CompareTo("ALL") == 0) ||
      detectors.BeginsWith("ALL ") ||
      detectors.EndsWith(" ALL") ||
      detectors.Contains(" ALL ")) {
    detectors = "ALL";
    return kTRUE;
  }

  // search for the given detector
  Bool_t result = kFALSE;
  if ((detectors.CompareTo(detName) == 0) ||
      detectors.BeginsWith(detName+" ") ||
      detectors.EndsWith(" "+detName) ||
      detectors.Contains(" "+detName+" ")) {
    detectors.ReplaceAll(detName, "");
    result = kTRUE;
  }

  // clean up the detectors string
  while (detectors.Contains("  ")) detectors.ReplaceAll("  ", " ");
  while (detectors.BeginsWith(" ")) detectors.Remove(0, 1);
  while (detectors.EndsWith(" ")) detectors.Remove(detectors.Length()-1, 1);

  return result;
}

//_____________________________________________________________________________
Int_t IlcSimulation::ConvertRaw2SDigits(const char* rawDirectory, const char* esdFileName, Int_t N) 
{
//
// Steering routine  to convert raw data in directory rawDirectory/ to fake SDigits. 
// These can be used for embedding of MC tracks into RAW data using the standard 
// merging procedure.
//
// If an ESD file is given the reconstructed vertex is taken from it and stored in the event header.
//
  if (!gIlc) {
    IlcError("no gIlc object. Restart ilcroot and try again.");
    return kFALSE;
  }
  if (gIlc->Modules()->GetEntries() > 0) {
    IlcError("gIlc was already run. Restart ilcroot and try again.");
    return kFALSE;
  }
  
  IlcInfo(Form("initializing gIlc with config file %s",fConfigFileName.Data()));
  
  gIlc->Announce();
  
  gROOT->LoadMacro(fConfigFileName.Data());
  gInterpreter->ProcessLine(gIlc->GetConfigFunction());
  
  if(IlcCDBManager::Instance()->GetRun() >= 0) { 
  	SetRunNumber(IlcCDBManager::Instance()->GetRun());
  } else {
  	IlcWarning("Run number not initialized!!");
  }
  
   IlcRunLoader::Instance()->CdGAFile();
    
   IlcPDG::AddParticlesToPdgDataBase();  

   gMC->SetMagField(TGeoGlobalMagField::Instance()->GetField());
   
   gIlc->GetMCApp()->Init();

   //Must be here because some MCs (G4) adds detectors here and not in Config.C
   gIlc->InitLoaders();
   IlcRunLoader::Instance()->MakeTree("E");
   IlcRunLoader::Instance()->LoadKinematics("RECREATE");
   IlcRunLoader::Instance()->LoadTrackRefs("RECREATE");
   IlcRunLoader::Instance()->LoadHits("all","RECREATE");

   //
   // Save stuff at the beginning of the file to avoid file corruption
   IlcRunLoader::Instance()->CdGAFile();
   gIlc->Write();
//
//  Initialize CDB     
    InitCDB();
    //IlcCDBManager* man = IlcCDBManager::Instance();
    //man->SetRun(0); // Should this come from rawdata header ?
    
    Int_t iDet;
    //
    // Get the runloader
    IlcRunLoader* runLoader = IlcRunLoader::Instance();
    //
    // Open esd file if available
    TFile* esdFile = 0;
    TTree* treeESD = 0;
    IlcESDEvent* esd = 0;
    if (esdFileName && (strlen(esdFileName)>0)) {
      esdFile = TFile::Open(esdFileName);
      if (esdFile) {
        esd = new IlcESDEvent();
        esdFile->GetObject("esdTree", treeESD);
        if (treeESD) esd->ReadFromTree(treeESD);
      }
    }

    //
    // Create the RawReader
    TString fileName(rawDirectory);
    IlcRawReader* rawReader = IlcRawReader::Create(fileName.Data());
    if (!rawReader) return (kFALSE);
    
//     if (!fEquipIdMap.IsNull() && fRawReader)
//       fRawReader->LoadEquipmentIdsMap(fEquipIdMap);
    //
    // Get list of detectors
    TObjArray* detArray = runLoader->GetIlcRun()->Detectors();
    //
    // Get Header
    IlcHeader* header = runLoader->GetHeader();
    // Event loop
    Int_t nev = 0;
    while(kTRUE) {
	if (!(rawReader->NextEvent())) break;
	runLoader->SetEventNumber(nev);
        runLoader->GetHeader()->Reset(rawReader->GetRunNumber(), 
                                      nev, nev);
        runLoader->GetEvent(nev);
        IlcInfo(Form("We are at event %d",nev));
	//
	// Detector loop
        TString detStr = fMakeSDigits;
	for (iDet = 0; iDet < detArray->GetEntriesFast(); iDet++) {
	    IlcModule* det = (IlcModule*) detArray->At(iDet);
	    if (!det || !det->IsActive()) continue;
	    if (IsSelected(det->GetName(), detStr)) {
	      IlcInfo(Form("Calling Raw2SDigits for %s", det->GetName()));
	      det->Raw2SDigits(rawReader);
	      rawReader->Reset();
	    }
	} // detectors
        

	//
	//  If ESD information available obtain reconstructed vertex and store in header.
	if (treeESD) {
		IlcInfo(Form("Selected event %d correspond to event %d ins raw and esd",nev,rawReader->GetEventIndex()));
	    treeESD->GetEvent(rawReader->GetEventIndex());
	    const IlcESDVertex* esdVertex = esd->GetPrimaryVertex();
	    Double_t position[3];
	    esdVertex->GetXYZ(position);
	    IlcGenEventHeader* mcHeader = new  IlcGenEventHeader("ESD");
	    TArrayF mcV;
	    mcV.Set(3);
	    for (Int_t i = 0; i < 3; i++) mcV[i] = position[i];
	    mcHeader->SetPrimaryVertex(mcV);
	    header->Reset(0,nev);
	    header->SetGenEventHeader(mcHeader);
	    IlcInfo(Form("***** Saved vertex %f %f %f \n", position[0], position[1], position[2]));
	}
//
//      Finish the event
	runLoader->TreeE()->Fill();
        IlcInfo(Form("Finished event %d",nev));
	nev++;
        if (N>0&&nev>=N)
          break;
    } // events
 
    delete rawReader;
//
//  Finish the run 
    runLoader->CdGAFile();
    runLoader->WriteHeader("OVERWRITE");
    runLoader->WriteRunLoader();

    return nev;
}

//_____________________________________________________________________________
void IlcSimulation::FinishRun()
{
  //
  // Called at the end of the run.
  //

  if(IsLegoRun()) 
   {
    IlcDebug(1, "Finish Lego");
    IlcRunLoader::Instance()->CdGAFile();
    fLego->FinishRun();
   }
  
  // Clean detector information
  TIter next(gIlc->Modules());
  IlcModule *detector;
  while((detector = dynamic_cast<IlcModule*>(next()))) {
    IlcDebug(2, Form("%s->FinishRun()", detector->GetName()));
    detector->FinishRun();
  }
  
  IlcDebug(1, "IlcRunLoader::Instance()->WriteHeader(OVERWRITE)");
  IlcRunLoader::Instance()->WriteHeader("OVERWRITE");

  // Write IlcRun info and all detectors parameters
  IlcRunLoader::Instance()->CdGAFile();
  gIlc->Write(0,TObject::kOverwrite);//write IlcRun
  IlcRunLoader::Instance()->Write(0,TObject::kOverwrite);//write RunLoader itself
  
  if(gIlc->GetMCApp()) gIlc->GetMCApp()->FinishRun();  
  IlcRunLoader::Instance()->Synchronize();
}

//_____________________________________________________________________________
Int_t IlcSimulation::GetDetIndex(const char* detector)
{
  // return the detector index corresponding to detector
  Int_t index = -1 ; 
  for (index = 0; index < fgkNDetectors ; index++) {
    if ( strcmp(detector, fgkDetectorName[index]) == 0 )
	  break ; 
  }	
  return index ; 
}

//_____________________________________________________________________________
Bool_t IlcSimulation::CreateHLT()
{
  // Init the HLT simulation.
  // The function  loads the library and creates the instance of IlcHLTSimulation.
  // the main reason for the decoupled creation is to set the transient OCDB
  // objects before the OCDB is locked

  IlcWarning("CreateHLT() method disabled");
  return kFALSE;

// // // // //   // load the library dynamically
// // // // //   gSystem->Load(ILCHLTSIMULATION_LIBRARY);
// // // // // 
// // // // //   // check for the library version
// // // // //   IlcHLTSimulationGetLibraryVersion_t fctVersion=(IlcHLTSimulationGetLibraryVersion_t)(gSystem->DynFindSymbol(ILCHLTSIMULATION_LIBRARY, ILCHLTSIMULATION_GET_LIBRARY_VERSION));
// // // // //   if (!fctVersion) {
// // // // //     IlcError(Form("can not load library %s", ILCHLTSIMULATION_LIBRARY));
// // // // //     return kFALSE;
// // // // //   }
// // // // //   if (fctVersion()!= ILCHLTSIMULATION_LIBRARY_VERSION) {
// // // // //     IlcWarning(Form("%s version does not match: compiled for version %d, loaded %d", ILCHLTSIMULATION_LIBRARY, ILCHLTSIMULATION_LIBRARY_VERSION, fctVersion()));
// // // // //   }
// // // // // 
// // // // //   // print compile info
// // // // //   typedef void (*CompileInfo)( const char*& date, const char*& time);
// // // // //   CompileInfo fctInfo=(CompileInfo)gSystem->DynFindSymbol(ILCHLTSIMULATION_LIBRARY, "CompileInfo");
// // // // //   if (fctInfo) {
// // // // //     const char* date="";
// // // // //     const char* time="";
// // // // //     (*fctInfo)(date, time);
// // // // //     if (!date) date="unknown";
// // // // //     if (!time) time="unknown";
// // // // //     IlcInfo(Form("%s build on %s (%s)", ILCHLTSIMULATION_LIBRARY, date, time));
// // // // //   } else {
// // // // //     IlcInfo(Form("no build info available for %s", ILCHLTSIMULATION_LIBRARY));
// // // // //   }
// // // // // 
// // // // //   // create instance of the HLT simulation
// // // // //   IlcHLTSimulationCreateInstance_t fctCreate=(IlcHLTSimulationCreateInstance_t)(gSystem->DynFindSymbol(ILCHLTSIMULATION_LIBRARY, ILCHLTSIMULATION_CREATE_INSTANCE));
// // // // //   if (fctCreate==NULL || (fpHLT=(fctCreate()))==NULL) {
// // // // //     IlcError(Form("can not create instance of HLT simulation (creator %p)", fctCreate));
// // // // //     return kFALSE;    
// // // // //   }
// // // // // 
// // // // //   TString specObjects;
// // // // //   for (Int_t i = 0; i < fSpecCDBUri.GetEntriesFast(); i++) {
// // // // //     if (specObjects.Length()>0) specObjects+=" ";
// // // // //     specObjects+=fSpecCDBUri[i]->GetName();
// // // // //   }
// // // // // 
// // // // //   IlcHLTSimulationSetup_t fctSetup=(IlcHLTSimulationSetup_t)(gSystem->DynFindSymbol(ILCHLTSIMULATION_LIBRARY, ILCHLTSIMULATION_SETUP));
// // // // //   if (fctSetup==NULL || fctSetup(fpHLT, this, specObjects.Data())<0) {
// // // // //     IlcWarning(Form("failed to setup HLT simulation (function %p)", fctSetup));
// // // // //   }
// // // // // 
// // // // //   return kTRUE;
}

//_____________________________________________________________________________
Bool_t IlcSimulation::RunHLT()
{
  // Run the HLT simulation
  // HLT simulation is implemented in HLT/sim/IlcHLTSimulation
  // Disabled if fRunHLT is empty, default vaule is "default".
  // IlcSimulation::SetRunHLT can be used to set the options for HLT simulation
  // The default simulation depends on the HLT component libraries and their
  // corresponding agents which define components and chains to run. See
  // http://web.ift.uib.no/~kjeks/doc/ilc-hlt-current/
  // http://web.ift.uib.no/~kjeks/doc/ilc-hlt-current/classIlcHLTModuleAgent.html
  //
  // The libraries to be loaded can be specified as an option.
  // <pre>
  // IlcSimulation sim;
  // sim.SetRunHLT("libIlcHLTSample.so");
  // </pre>
  // will only load <tt>libIlcHLTSample.so</tt>

  // Other available options:
  // \li loglevel=<i>level</i> <br>
  //     logging level for this processing
  // \li ilclog=off
  //     disable redirection of log messages to IlcLog class
  // \li config=<i>macro</i>
  //     configuration macro
  // \li chains=<i>configuration</i>
  //     comma separated list of configurations to be run during simulation
  // \li rawfile=<i>file</i>
  //     source for the RawReader to be created, the default is <i>./</i> if
  //     raw data is simulated

  IlcWarning("RunHLT() method disabled");
  return kFALSE;

// // // //   int iResult=0;
// // // // 
// // // //   if (!fpHLT && !CreateHLT()) {
// // // //     return kFALSE;
// // // //   }
// // // //   IlcHLTSimulation* pHLT=fpHLT;
// // // // 
// // // //   IlcRunLoader* pRunLoader = LoadRun("READ");
// // // //   if (!pRunLoader) return kFALSE;
// // // // 
// // // //   // initialize CDB storage, run number, set CDB lock
// // // //   // thats for the case of running HLT simulation without all the other steps
// // // //   // multiple calls are handled by the function, so we can just call
// // // //   InitCDB();
// // // //   if (!SetRunNumberFromData()) if (fStopOnError) return kFALSE;
// // // //   SetCDBLock();
// // // //   
// // // //   // init the HLT simulation
// // // //   TString options;
// // // //   if (fRunHLT.CompareTo("default")!=0) options=fRunHLT;
// // // //   TString detStr = fWriteRawData;
// // // //   if (!IsSelected("HLT", detStr)) {
// // // //     options+=" writerawfiles=";
// // // //   } else {
// // // //     options+=" writerawfiles=HLT";
// // // //   }
// // // // 
// // // //   if (!detStr.IsNull() && !options.Contains("rawfile=")) {
// // // //     // as a matter of fact, HLT will run reconstruction and needs the RawReader
// // // //     // in order to get detector data. By default, RawReaderFile is used to read
// // // //     // the already simulated ddl files. Date and Root files from the raw data
// // // //     // are generated after the HLT simulation.
// // // //     options+=" rawfile=./";
// // // //   }
// // // // 
// // // //   IlcHLTSimulationInit_t fctInit=(IlcHLTSimulationInit_t)(gSystem->DynFindSymbol(ILCHLTSIMULATION_LIBRARY, ILCHLTSIMULATION_INIT));
// // // //   if (fctInit==NULL || (iResult=(fctInit(pHLT, pRunLoader, options.Data())))<0) {
// // // //     IlcError(Form("can not init HLT simulation: error %d (init %p)", iResult, fctInit));
// // // //   } else {
// // // //     // run the HLT simulation
// // // //     IlcHLTSimulationRun_t fctRun=(IlcHLTSimulationRun_t)(gSystem->DynFindSymbol(ILCHLTSIMULATION_LIBRARY, ILCHLTSIMULATION_RUN));
// // // //     if (fctRun==NULL || (iResult=(fctRun(pHLT, pRunLoader)))<0) {
// // // //       IlcError(Form("can not run HLT simulation: error %d (run %p)", iResult, fctRun));
// // // //     }
// // // //   }
// // // // 
// // // //   // delete the instance
// // // //   IlcHLTSimulationDeleteInstance_t fctDelete=(IlcHLTSimulationDeleteInstance_t)(gSystem->DynFindSymbol(ILCHLTSIMULATION_LIBRARY, ILCHLTSIMULATION_DELETE_INSTANCE));
// // // //   if (fctDelete==NULL || fctDelete(pHLT)<0) {
// // // //     IlcError(Form("can not delete instance of HLT simulation (creator %p)", fctDelete));
// // // //   }
// // // //   pHLT=NULL;
// // // // 
// // // //   return iResult>=0?kTRUE:kFALSE;
}

//_____________________________________________________________________________
Bool_t IlcSimulation::RunQA()
{
	// run the QA on summable hits, digits or digits
	
    //if(!gIlc) return kFALSE;
	IlcQAManager::QAManager()->SetRunLoader(IlcRunLoader::Instance()) ;

	TString detectorsw("") ;  
	Bool_t rv = kTRUE ; 
  IlcQAManager::QAManager()->SetEventSpecie(fEventSpecie) ;
	detectorsw = IlcQAManager::QAManager()->Run(fQADetectors.Data()) ; 
	if ( detectorsw.IsNull() ) 
		rv = kFALSE ; 
	return rv ; 
}

//_____________________________________________________________________________
Bool_t IlcSimulation::SetRunQA(TString detAndAction) 
{
	// Allows to run QA for a selected set of detectors
	// and a selected set of tasks among HITS, SDIGITS and DIGITS
	// all selected detectors run the same selected tasks
	
	if (!detAndAction.Contains(":")) {
		IlcError( Form("%s is a wrong syntax, use \"DetectorList:ActionList\" \n", detAndAction.Data()) ) ;
		fRunQA = kFALSE ;
		return kFALSE ; 		
	}
	Int_t colon = detAndAction.Index(":") ; 
	fQADetectors = detAndAction(0, colon) ; 
	if (fQADetectors.Contains("ALL") ){
    TString tmp = Form("%s %s", fMakeDigits.Data(), fMakeDigitsFromHits.Data()) ; 
    Int_t minus = fQADetectors.Last('-') ; 
    TString toKeep = Form("%s %s", fMakeDigits.Data(), fMakeDigitsFromHits.Data()) ; 
    TString toRemove("") ;
    while (minus >= 0) {
      toRemove = fQADetectors(minus+1, fQADetectors.Length()) ; 
      toRemove = toRemove.Strip() ; 
      toKeep.ReplaceAll(toRemove, "") ; 
      fQADetectors.ReplaceAll(Form("-%s", toRemove.Data()), "") ; 
      minus = fQADetectors.Last('-') ; 
    }
    fQADetectors = toKeep ; 
  }
  fQATasks   = detAndAction(colon+1, detAndAction.Sizeof() ) ; 
	if (fQATasks.Contains("ALL") ) {
		fQATasks = Form("%d %d %d", IlcQAv1::kHITS, IlcQAv1::kSDIGITS, IlcQAv1::kDIGITS) ; 
	} else {
		fQATasks.ToUpper() ; 
		TString tempo("") ; 
		if ( fQATasks.Contains("HIT") ) 
			tempo = Form("%d ", IlcQAv1::kHITS) ; 
		if ( fQATasks.Contains("SDIGIT") ) 
			tempo += Form("%d ", IlcQAv1::kSDIGITS) ; 
		if ( fQATasks.Contains("DIGIT") ) 
			tempo += Form("%d ", IlcQAv1::kDIGITS) ; 
		fQATasks = tempo ; 
		if (fQATasks.IsNull()) {
			IlcInfo("No QA requested\n")  ;
			fRunQA = kFALSE ;
			return kTRUE ; 
		}
	}	
	TString tempo(fQATasks) ; 
    tempo.ReplaceAll(Form("%d", IlcQAv1::kHITS), IlcQAv1::GetTaskName(IlcQAv1::kHITS)) 	;
    tempo.ReplaceAll(Form("%d", IlcQAv1::kSDIGITS), IlcQAv1::GetTaskName(IlcQAv1::kSDIGITS)) ;	
    tempo.ReplaceAll(Form("%d", IlcQAv1::kDIGITS), IlcQAv1::GetTaskName(IlcQAv1::kDIGITS)) ; 	
	IlcInfo( Form("QA will be done on \"%s\" for \"%s\"\n", fQADetectors.Data(), tempo.Data()) ) ;  
	fRunQA = kTRUE ;
	IlcQAManager::QAManager()->SetActiveDetectors(fQADetectors) ; 
	IlcQAManager::QAManager()->SetTasks(fQATasks) ; 
  for (Int_t det = 0 ; det < IlcQAv1::kNDET ; det++) 
    IlcQAManager::QAManager()->SetWriteExpert(IlcQAv1::DETECTORINDEX_t(det)) ;
  
	return kTRUE; 
} 

//_____________________________________________________________________________
void IlcSimulation::ProcessEnvironmentVars()
{
// Extract run number and random generator seed from env variables

    IlcInfo("Processing environment variables");
    
    // Random Number seed
    
    // first check that seed is not already set
    if (fSeed == 0) {
    	if (gSystem->Getenv("CONFIG_SEED")) {
     	 	fSeed = atoi(gSystem->Getenv("CONFIG_SEED"));
    	}
    } else {
    	if (gSystem->Getenv("CONFIG_SEED")) {
    		IlcInfo(Form("Seed for random number generation already set (%d)"
			     ": CONFIG_SEED variable ignored!", fSeed));
    	}
    }
   
    IlcInfo(Form("Seed for random number generation = %d ", fSeed)); 

    // Run Number
    
    // first check that run number is not already set
    if(fRun < 0) {    
    	if (gSystem->Getenv("DC_RUN")) {
		fRun = atoi(gSystem->Getenv("DC_RUN"));
    	}
    } else {
    	if (gSystem->Getenv("DC_RUN")) {
    		IlcInfo(Form("Run number already set (%d): DC_RUN variable ignored!", fRun));
    	}
    }
    
    IlcInfo(Form("Run number = %d", fRun)); 
}

//---------------------------------------------------------------------
void IlcSimulation::WriteGRPEntry()
{
  // Get the necessary information from gilc (generator, trigger etc) and
  // write a GRP entry corresponding to the settings in the Config.C used
  // note that Hall probes and Cavern and Surface Atmos pressures are not simulated.

  IlcWarning("WriteGRPEntry method disabled");
  return;

  IlcInfo("Writing global run parameters entry into the OCDB");

  IlcGRPObject* grpObj = new IlcGRPObject();

  grpObj->SetRunType("PHYSICS");
  grpObj->SetTimeStart(fTimeStart);
  grpObj->SetTimeEnd(fTimeEnd); 
  grpObj->SetBeamEnergyIsSqrtSHalfGeV(); // new format of GRP: store sqrt(s)/2 in GeV

  const IlcGenerator *gen = gIlc->GetMCApp()->Generator();
  Int_t a = 0;
  Int_t z = 0;

  if (gen) {
    TString projectile;
    gen->GetProjectile(projectile,a,z);
    TString target;
    gen->GetTarget(target,a,z);
    TString beamType = projectile + "-" + target;
    beamType.ReplaceAll(" ","");
    if (!beamType.CompareTo("-")) {
      grpObj->SetBeamType("UNKNOWN");
      grpObj->SetBeamEnergy(gen->GetEnergyCMS()/2);
    }
    else {
      grpObj->SetBeamType(beamType);
      if (z != 0) {
	  grpObj->SetBeamEnergy(gen->GetEnergyCMS()/2 * a / z);
      } else {
	  grpObj->SetBeamEnergy(gen->GetEnergyCMS()/2 );
      }
      // Heavy ion run, the event specie is set to kHighMult
      fEventSpecie = IlcRecoParam::kHighMult;
      if ((strcmp(beamType,"p-p") == 0) ||
          (strcmp(beamType,"p-")  == 0) ||
          (strcmp(beamType,"-p")  == 0) ||
          (strcmp(beamType,"P-P") == 0) ||
          (strcmp(beamType,"P-")  == 0) ||
          (strcmp(beamType,"-P")  == 0)) {
        // Proton run, the event specie is set to kLowMult
        fEventSpecie = IlcRecoParam::kLowMult;
      } 
    }
  } else {
    IlcWarning("Unknown beam type and energy! Setting energy to 0");
    grpObj->SetBeamEnergy(0);
    grpObj->SetBeamType("UNKNOWN");
  }

  UInt_t detectorPattern  = 0;
  Int_t nDets = 0;
  TObjArray *detArray = gIlc->Detectors();
  for (Int_t iDet = 0; iDet < IlcDAQ::kNDetectors-1; iDet++) {
    if (detArray->FindObject(IlcDAQ::OfflineModuleName(iDet))) {
      IlcDebug(1, Form("Detector #%d found: %s", iDet, IlcDAQ::OfflineModuleName(iDet)));
      detectorPattern |= (1 << iDet);
      nDets++;
    }
  }
  // CTP
  if (!fTriggerConfig.IsNull())
    detectorPattern |= (1 << IlcDAQ::DetectorID("TRG"));

  // HLT
  if (!fRunHLT.IsNull())
    detectorPattern |= (1 << IlcDAQ::kHLTId);

  grpObj->SetNumberOfDetectors((Char_t)nDets);
  grpObj->SetDetectorMask((Int_t)detectorPattern);
  grpObj->SetLHCPeriod("LHC08c");
  grpObj->SetLHCState("STABLE_BEAMS");
  //
  IlcMagF *field = (IlcMagF*)TGeoGlobalMagField::Instance()->GetField();
  Float_t solenoidField = field ? TMath::Abs(field->SolenoidField()) : 0;

  Float_t factorSol     = field ? field->GetFactorSol() : 0;
  Float_t currentSol    = TMath::Abs(factorSol)>1E-6 ? 
    TMath::Nint(TMath::Abs(solenoidField/factorSol))/5.*30000.*TMath::Abs(factorSol) : 0;
  //
  Float_t factorDip     = field ? field->GetFactorDip() : 0;
  Float_t currentDip    = 6000.*TMath::Abs(factorDip);
  //
  grpObj->SetL3Current(currentSol,(IlcGRPObject::Stats)0);
  grpObj->SetDipoleCurrent(currentDip,(IlcGRPObject::Stats)0);  
  grpObj->SetL3Polarity(factorSol>0 ? 0:1);  
  grpObj->SetDipolePolarity(factorDip>0 ? 0:1);
  if (field) grpObj->SetUniformBMap(field->IsUniform()); // for special MC with k5kGUniform map
  grpObj->SetPolarityConventionLHC();                    // LHC convention +/+ current -> -/- field main components
  //
  grpObj->SetCavernTemperature(0,(IlcGRPObject::Stats)0);
  
  //grpMap->Add(new TObjString("fCavernPressure"),new TObjString("0")); ---> not inserted in simulation with the new object, since it is now an IlcDCSSensor

  // Now store the entry in OCDB
  IlcCDBManager* man = IlcCDBManager::Instance();
  
  man->SetLock(0, fKey);
  
  IlcCDBStorage* sto = man->GetStorage(fGRPWriteLocation.Data());
  

  IlcCDBId id("GRP/GRP/Data", man->GetRun(), man->GetRun(), 1, 1);
  IlcCDBMetaData *metadata= new IlcCDBMetaData();

  metadata->SetResponsible("ilc-off@cern.ch");
  metadata->SetComment("Automatically produced GRP entry for Monte Carlo");
 
  sto->Put(grpObj,id,metadata);
  man->SetLock(1, fKey);
}

//_____________________________________________________________________________
time_t IlcSimulation::GenerateTimeStamp() const
{
  // Generate event time-stamp according to
  // SOR/EOR time from GRP
  if (fUseTimeStampFromCDB)
    return fTimeStart + gRandom->Integer(fTimeEnd-fTimeStart);
  else
    return 0;
}

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
//-------------------------------------------------------------------------
//   Implementation of IlcCDBManager and IlcCDBParam classe
//   Author: Alberto Colla 
//   e-mail: Alberto.Colla@cern.ch
//-------------------------------------------------------------------------

#include <stdlib.h>

#include "IlcCDBManager.h"
#include "IlcCDBStorage.h"
#include "IlcLog.h"
#include "IlcCDBDump.h"
#include "IlcCDBLocal.h"
#include "IlcCDBGrid.h"
#include "IlcCDBEntry.h"
#include "IlcCDBHandler.h"

#include <TObjString.h>
#include <TSAXParser.h>
#include <TFile.h>
#include <TKey.h>
#include <TUUID.h>
#include <TGrid.h>

ClassImp(IlcCDBParam)

ClassImp(IlcCDBManager)

//TODO OCDB and Reference folder should not be fully hardcoded but built from run number (or year/LHC period)
TString IlcCDBManager::fgkCondUri("alien://folder=/ilc/cern.ch/user/a/ilcprod/testCDB/CDB?user=ilcprod");
TString IlcCDBManager::fgkRefUri("alien://folder=/ilc/cern.ch/user/a/ilcprod/testCDB/Reference?user=ilcprod");
TString IlcCDBManager::fgkMCIdealStorage("alien://folder=/ilc/simulation/2008/v4-15-Release/Ideal");
TString IlcCDBManager::fgkMCFullStorage("alien://folder=/ilc/simulation/2008/v4-15-Release/Full");
TString IlcCDBManager::fgkMCResidualStorage("alien://folder=/ilc/simulation/2008/v4-15-Release/Residual");
TString IlcCDBManager::fgkOCDBFolderXMLfile("alien:///ilc/data/OCDBFoldervsRunRange.xml");
IlcCDBManager* IlcCDBManager::fgInstance = 0x0;

//_____________________________________________________________________________
IlcCDBManager* IlcCDBManager::Instance(TMap *entryCache, Int_t run)
{
// returns IlcCDBManager instance (singleton)

	if (!fgInstance) {
		fgInstance = new IlcCDBManager();
		if (!entryCache)
		  fgInstance->Init();
		else
		  fgInstance->InitFromCache(entryCache,run);
	}

	return fgInstance;
}

//_____________________________________________________________________________
void IlcCDBManager::Init() {
// factory registering

	RegisterFactory(new IlcCDBDumpFactory());
	RegisterFactory(new IlcCDBLocalFactory()); 
	// IlcCDBGridFactory is registered only if AliEn libraries are enabled in Root
	if(!gSystem->Exec("root-config --has-alien 2>/dev/null |grep yes 2>&1 > /dev/null")){ // returns 0 if yes
		IlcInfo("AliEn classes enabled in Root. IlcCDBGrid factory registered.");
		RegisterFactory(new IlcCDBGridFactory());
		fCondParam = CreateParameter(fgkCondUri);
		fRefParam = CreateParameter(fgkRefUri);
	}

	InitShortLived();
}

//_____________________________________________________________________________
void IlcCDBManager::InitFromCache(TMap *entryCache, Int_t run) {
// initialize manager from existing cache
// used on the slaves in case of parallel reconstruction
  SetRun(run);

  TIter iter(entryCache->GetTable());
  TPair* pair = 0;

  while((pair = dynamic_cast<TPair*> (iter.Next()))){
    fEntryCache.Add(pair->Key(),pair->Value());
  }
  // fEntry is the new owner of the cache
  fEntryCache.SetOwnerKeyValue(kTRUE,kTRUE);
  entryCache->SetOwnerKeyValue(kFALSE,kFALSE);
  IlcInfo(Form("%d cache entries have been loaded",fEntryCache.GetEntries()));
}

//_____________________________________________________________________________
void  IlcCDBManager::DumpToSnapshotFile(const char* snapshotFileName, Bool_t singleKeys){
// 
// dump the entries map and the ids list to
// the output file

    // open the file
    TFile *f = TFile::Open(snapshotFileName,"RECREATE");
    if (!f || f->IsZombie()){
	IlcError(Form("Cannot open file %s",snapshotFileName));
	return;
    }

    Printf("\ndumping entriesMap (entries'cache) with %d entries!\n", fEntryCache.GetEntries()); 
    Printf("\ndumping entriesList with %d entries!\n", fIds->GetEntries());

    f->cd();                                                                                           

    if(singleKeys){
	f->WriteObject(&fEntryCache,"CDBentriesMap");
	f->WriteObject(fIds,"CDBidsList");
    }else{
	// We write the entries one by one named by their calibration path
	/*
	fEntryCache.Write("CDBentriesMap");
	fIds->Write("CDBidsList");
	*/
	TIter iter(fEntryCache.GetTable());
	TPair* pair = 0;
	while((pair = dynamic_cast<TPair*> (iter.Next()))){
	    TObjString *os = dynamic_cast<TObjString*>(pair->Key());
	    if (!os) continue;
	    TString path = os->GetString();
	    IlcCDBEntry *entry = dynamic_cast<IlcCDBEntry*>(pair->Value());
	    if (!entry) continue;
	    path.ReplaceAll("/","*");
	    entry->Write(path.Data());
	}
    }
    f->Close();
    delete f;

    exit(0);
}

//_____________________________________________________________________________
Bool_t IlcCDBManager::InitFromSnapshot(const char* snapshotFileName, Bool_t overwrite){
// initialize manager from a CDB snapshot, that is add the entries
// to the entries map and the ids to the ids list taking them from
// the map and the list found in the input file

// if the manager is locked it cannot initialize from a snapshot
    if(fLock) {
	IlcError("Being locked I cannot initialize from the snapshot!");
	return kFALSE;
    }	

    // open the file
    TString snapshotFile(snapshotFileName);
    if(snapshotFile.BeginsWith("alien://")){
	if(!gGrid) {
	    TGrid::Connect("alien://","");
	    if(!gGrid) {
		IlcError("Connection to alien failed!");
		return kFALSE;
	    }
	}
    }

    TFile *f = TFile::Open(snapshotFileName);
    if (!f || f->IsZombie()){
	IlcError(Form("Cannot open file %s",snapshotFileName));
	return kFALSE;
    }

    // retrieve entries' map from snapshot file
    TMap *entriesMap = 0;
    TIter next(f->GetListOfKeys());
    TKey *key;
    while ((key = (TKey*)next())) {
	if (strcmp(key->GetClassName(),"TMap") != 0) continue;
	entriesMap = (TMap*)key->ReadObj();
	break;
    }
    if (!entriesMap || entriesMap->GetEntries()==0){
	IlcError("Cannot get valid map of CDB entries from snapshot file");
	return kFALSE;
    }

    // retrieve ids' list from snapshot file
    TList *idsList = 0;
    TIter nextKey(f->GetListOfKeys());
    TKey *keyN;
    while ((keyN = (TKey*)nextKey())) {
	if (strcmp(keyN->GetClassName(),"TList") != 0) continue;
	idsList = (TList*)keyN->ReadObj();
	break;
    }
    if (!idsList || idsList->GetEntries()==0){
	IlcError("Cannot get valid list of CDB entries from snapshot file");
	return kFALSE;
    }

    // Add each (entry,id) from the snapshot to the memory: entry to the cache, id to the list of ids.
    // If "overwrite" is false: add the entry to the cache and its id to the list of ids
    // only if neither of them is already there.
    // If "overwrite" is true: write the snapshot entry,id in any case. If something
    // was already there for that calibration type, remove it and issue a warning
    TIter iterObj(entriesMap->GetTable());
    TPair* pair = 0;
    Int_t nAdded=0;
    while((pair = dynamic_cast<TPair*> (iterObj.Next()))){
	TObjString* os = (TObjString*) pair->Key();
	TString path = os->GetString();
	TIter iterId(idsList);
	IlcCDBId* id=0;
	IlcCDBId* correspondingId=0;
	while((id = dynamic_cast<IlcCDBId*> (iterId.Next()))){
	    TString idpath(id->GetPath());
	    if(idpath==path){
		correspondingId=id;
		break;
	    }
	}
	if(!correspondingId){
	    IlcError(Form("id for \"%s\" not found in the snapshot (while entry was). This entry is skipped!",path.Data()));
	    break;
	}
	Bool_t cached = fEntryCache.Contains(path.Data());
	Bool_t registeredId = kFALSE;
	TIter iter(fIds);
	IlcCDBId *idT = 0;
	while((idT = dynamic_cast<IlcCDBId*> (iter.Next()))){
	    if(idT->GetPath()==path){
		registeredId = kTRUE;
		break;
	    }
	}

	if(overwrite){
	    if(cached || registeredId){
		IlcWarning(Form("An entry was already cached for \"%s\". Removing it before caching from snapshot",path.Data()));
		UnloadFromCache(path.Data());
	    }
	    fEntryCache.Add(pair->Key(),pair->Value());
	    fIds->Add(id);
	    nAdded++;
	}else{
	    if(cached || registeredId){
		IlcWarning(Form("An entry was already cached for \"%s\". Not adding this object from snapshot",path.Data()));
	    }else{
		fEntryCache.Add(pair->Key(),pair->Value());
		fIds->Add(id);
		nAdded++;
	    }
	}
    }

    // fEntry is the new owner of the cache
    fEntryCache.SetOwnerKeyValue(kTRUE,kTRUE);
    entriesMap->SetOwnerKeyValue(kFALSE,kFALSE);
    fIds->SetOwner(kTRUE);
    idsList->SetOwner(kFALSE);
    IlcInfo(Form("%d new (entry,id) cached. Total number %d",nAdded,fEntryCache.GetEntries()));

    f->Close();
    delete f;

    return kTRUE;
}

//_____________________________________________________________________________
void IlcCDBManager::Destroy() {
// delete ALCDBManager instance and active storages

	if (fgInstance) {
		//fgInstance->Delete();
		delete fgInstance;
		fgInstance = 0x0;
	}
}

//_____________________________________________________________________________
IlcCDBManager::IlcCDBManager():
  TObject(),
  fFactories(),
  fActiveStorages(),
  fSpecificStorages(),
  fEntryCache(),
  fIds(0),
  fStorageMap(0),
  fShortLived(0),
  fDefaultStorage(NULL),
  fDrainStorage(NULL),
  fCondParam(0),
  fRefParam(0),
  fRun(-1),
  fCache(kTRUE),
  fLock(kFALSE),
  fSnapshotMode(kFALSE),
  fSnapshotFile(0),
  fRaw(kFALSE),
  fStartRunLHCPeriod(-1),
  fEndRunLHCPeriod(-1),
  fLHCPeriod(""),
  fKey(0)
{
// default constuctor
	fFactories.SetOwner(1);
	fActiveStorages.SetOwner(1);
	fSpecificStorages.SetOwner(1);
	fEntryCache.SetName("CDBEntryCache");
	fEntryCache.SetOwnerKeyValue(kTRUE,kTRUE);

	fStorageMap = new TMap();
	fStorageMap->SetOwner(1);
	fIds = new TList();
	fIds->SetOwner(1);
}

//_____________________________________________________________________________
IlcCDBManager::~IlcCDBManager() {
// destructor
	ClearCache();
	DestroyActiveStorages();
	fFactories.Delete();
	fDrainStorage = 0x0;
	fDefaultStorage = 0x0;
	delete fStorageMap; fStorageMap = 0;
	delete fIds; fIds = 0;
	delete fCondParam;
	delete fRefParam;
	delete fShortLived; fShortLived = 0x0;
	//fSnapshotCache = 0;
	//fSnapshotIdsList = 0;
	if(fSnapshotMode){
	    fSnapshotFile->Close();
	    fSnapshotFile = 0;
	}
}

//_____________________________________________________________________________
void IlcCDBManager::PutActiveStorage(IlcCDBParam* param, IlcCDBStorage* storage){
// put a storage object into the list of active storages

	fActiveStorages.Add(param, storage);
	IlcDebug(1, Form("Active storages: %d", fActiveStorages.GetEntries()));
}

//_____________________________________________________________________________
void IlcCDBManager::RegisterFactory(IlcCDBStorageFactory* factory) {
// add a storage factory to the list of registerd factories
 
	if (!fFactories.Contains(factory)) {
		fFactories.Add(factory);
	}
}

//_____________________________________________________________________________
Bool_t IlcCDBManager::HasStorage(const char* dbString) const {
// check if dbString is a URI valid for one of the registered factories

	TIter iter(&fFactories);

	IlcCDBStorageFactory* factory=0;
	while ((factory = (IlcCDBStorageFactory*) iter.Next())) {

		if (factory->Validate(dbString)) {
			return kTRUE;
		}
	}

	return kFALSE;
}

//_____________________________________________________________________________
IlcCDBParam* IlcCDBManager::CreateParameter(const char* dbString) const {
// create IlcCDBParam object from URI string

	TIter iter(&fFactories);

        IlcCDBStorageFactory* factory=0;
        while ((factory = (IlcCDBStorageFactory*) iter.Next())) {
		IlcCDBParam* param = factory->CreateParameter(dbString);
		if(param) return param;
        }

        return NULL;
}

//_____________________________________________________________________________
IlcCDBStorage* IlcCDBManager::GetStorage(const char* dbString) {
// get storage object from URI string
		
	IlcCDBParam* param = CreateParameter(dbString);
	if (!param) {
		IlcError(Form("Failed to activate requested storage! Check URI: %s", dbString));
		return NULL;
	}

	IlcCDBStorage* aStorage = GetStorage(param);

	delete param;
	return aStorage;
}

//_____________________________________________________________________________
IlcCDBStorage* IlcCDBManager::GetStorage(const IlcCDBParam* param) {
// get storage object from IlcCDBParam object
	
	// if the list of active storages already contains
	// the requested storage, return it
	IlcCDBStorage* aStorage = GetActiveStorage(param);
	if (aStorage) {
		return aStorage;
	}

	// if lock is ON, cannot activate more storages!
	if(fLock) {
		if (fDefaultStorage) {
			IlcFatal("Lock is ON, and default storage is already set: "
				"cannot reset it or activate more storages!");
		}
	}	
	
	TIter iter(&fFactories);

        IlcCDBStorageFactory* factory=0;

	// loop on the list of registered factories
	while ((factory = (IlcCDBStorageFactory*) iter.Next())) {

		// each factory tries to create its storage from the parameter
		aStorage = factory->Create(param);
		if (aStorage) {
			PutActiveStorage(param->CloneParam(), aStorage);
			aStorage->SetURI(param->GetURI());
			if(fRun >= 0) {
				if(aStorage->GetType() == "alien"){
					aStorage->QueryCDB(fRun);
				} else {
					IlcDebug(2,
						"Skipping query for valid files, it is used only in grid...");
				}
			}
			return aStorage;
		}
        }

	IlcError(Form("Failed to activate requested storage! Check URI: %s", param->GetURI().Data()));

        return NULL;
}

//_____________________________________________________________________________
IlcCDBStorage* IlcCDBManager::GetActiveStorage(const IlcCDBParam* param) {
// get a storage object from the list of active storages

        return dynamic_cast<IlcCDBStorage*> (fActiveStorages.GetValue(param));
}

//_____________________________________________________________________________
TList* IlcCDBManager::GetActiveStorages() {
// return list of active storages
// user has responsibility to delete returned object

	TList* result = new TList();

	TIter iter(fActiveStorages.GetTable());
	TPair* aPair=0;
	while ((aPair = (TPair*) iter.Next())) {
		result->Add(aPair->Value());
	}

	return result;
}

//_____________________________________________________________________________
void IlcCDBManager::SetDrain(const char* dbString) {
// set drain storage from URI string

	fDrainStorage = GetStorage(dbString);	
}

//_____________________________________________________________________________
void IlcCDBManager::SetDrain(const IlcCDBParam* param) {
// set drain storage from IlcCDBParam

	fDrainStorage = GetStorage(param);
}

//_____________________________________________________________________________
void IlcCDBManager::SetDrain(IlcCDBStorage* storage) {
// set drain storage from another active storage
	
	fDrainStorage = storage;
}

//_____________________________________________________________________________
Bool_t IlcCDBManager::Drain(IlcCDBEntry *entry) {
// drain retrieved object to drain storage

	IlcDebug(2, "Draining into drain storage...");
	return fDrainStorage->Put(entry);
}

//____________________________________________________________________________
void IlcCDBManager::SetDefaultStorage(const char* dbString) {
// sets default storage from URI string
	
	// checking whether we are in the raw case
	TString dbStringTemp(dbString);
	if (dbStringTemp == "raw://")
	{
		fRaw = kTRUE;
		IlcInfo("Setting the run-number will set the corresponding OCDB for raw data reconstruction.");
		IlcInfo("Connecting to the grid...");
		if(!gGrid) {
			TGrid::Connect("alien://","");
			if(!gGrid) {
				IlcError("Connection to alien failed!");
				return;
			}
		}
		return;
	}

	IlcCDBStorage* bckStorage = fDefaultStorage;
	
	fDefaultStorage = GetStorage(dbString);
	
	if(!fDefaultStorage) return;
	
	if(bckStorage && (fDefaultStorage != bckStorage)){
		IlcWarning("Existing default storage replaced: clearing cache!");
		ClearCache();
	}
	
	if (fStorageMap->Contains("default")) {
		delete fStorageMap->Remove(((TPair*)fStorageMap->FindObject("default"))->Key());
	}
	fStorageMap->Add(new TObjString("default"), new TObjString(fDefaultStorage->GetURI()));
}
//_____________________________________________________________________________
void IlcCDBManager::SetDefaultStorage(const IlcCDBParam* param) {
// set default storage from IlcCDBParam object
	
	IlcCDBStorage* bckStorage = fDefaultStorage;

	fDefaultStorage = GetStorage(param);

	if(!fDefaultStorage) return;

	if(bckStorage && (fDefaultStorage != bckStorage)){
		IlcWarning("Existing default storage replaced: clearing cache!");
		ClearCache();
	}

	if (fStorageMap->Contains("default")) {
	        delete fStorageMap->Remove(((TPair*)fStorageMap->FindObject("default"))->Key());
	}
	fStorageMap->Add(new TObjString("default"), new TObjString(fDefaultStorage->GetURI()));
}

//_____________________________________________________________________________
void IlcCDBManager::SetDefaultStorage(IlcCDBStorage* storage) {
// set default storage from another active storage
	
	// if lock is ON, cannot activate more storages!
	if(fLock) {
		if (fDefaultStorage) {
			IlcFatal("Lock is ON, and default storage is already set: "
				"cannot reset it or activate more storages!");
		}
	}	
	
	if (!storage) {
		UnsetDefaultStorage();
		return;
	}
	
	IlcCDBStorage* bckStorage = fDefaultStorage;

	fDefaultStorage = storage;

	if(bckStorage && (fDefaultStorage != bckStorage)){
		IlcWarning("Existing default storage replaced: clearing cache!");
		ClearCache();
	}

	if (fStorageMap->Contains("default")) {
	        delete fStorageMap->Remove(((TPair*)fStorageMap->FindObject("default"))->Key());
	}
	fStorageMap->Add(new TObjString("default"), new TObjString(fDefaultStorage->GetURI()));
}

//_____________________________________________________________________________
void IlcCDBManager::SetDefaultStorage(const char* mcString, const char* simType) {
// sets default storage for MC data
// mcString MUST be "MC", 
// simType can be "Ideal","Residual","Full"
	
	TString strmcString(mcString);
	TString strsimType(simType);
	TString dbString; 
        if (strmcString != "MC"){
		IlcFatal("Method requires first string to be MC!");
	}
        else {
		if (strsimType == "Ideal"){
			dbString = fgkMCIdealStorage;
		}
		else if (strsimType == "Full"){
			dbString = fgkMCFullStorage;
		}
		else if (strsimType == "Residual"){
			dbString = fgkMCResidualStorage;
		}
		else {
			IlcFatal("Error in setting the storage for MC data, second argument MUST be either \"Ideal\" or \"Full\" or \"Residual\".");
		}

		SetDefaultStorage(dbString.Data());
		if(!fDefaultStorage) IlcFatal(Form("%s storage not there! Please check!",fLHCPeriod.Data()));
	}
}
//_____________________________________________________________________________
void IlcCDBManager::SetDefaultStorageFromRun(Int_t run) {
// set default storage from the run number - to be used only with raw data	

	// if lock is ON, cannot activate more storages!
	if(fLock) {
		if (fDefaultStorage) {
			IlcFatal("Lock is ON, and default storage is already set: "
				"cannot activate default storage from run number");
		}
	}	

	// retrieve XML file from alien
	if(!gGrid) {
	    TGrid::Connect("alien://","");
	    if(!gGrid) {
		IlcError("Connection to alien failed!");
		return;
	    }
	}
	TUUID uuid;
	TString rndname = "/tmp/";
	rndname += "OCDBFolderXML.";
	rndname += uuid.AsString();
	rndname += ".xml";
	IlcDebug(2, Form("file to be copied = %s", fgkOCDBFolderXMLfile.Data()));
	if (!TFile::Cp(fgkOCDBFolderXMLfile.Data(), rndname.Data())) {
		IlcFatal(Form("Cannot make a local copy of OCDBFolder xml file in %s",rndname.Data()));
	}
	IlcCDBHandler* saxcdb = new IlcCDBHandler();
	saxcdb->SetRun(run);
	TSAXParser *saxParser = new TSAXParser();
	saxParser->ConnectToHandler("IlcCDBHandler", saxcdb);  
	saxParser->ParseFile(rndname.Data()); 
	IlcInfo(Form(" LHC folder = %s", saxcdb->GetOCDBFolder().Data()));
	IlcInfo(Form(" LHC period start run = %d", saxcdb->GetStartRunRange()));
	IlcInfo(Form(" LHC period end run = %d", saxcdb->GetEndRunRange()));
	fLHCPeriod = saxcdb->GetOCDBFolder();
	fStartRunLHCPeriod = saxcdb->GetStartRunRange();
	fEndRunLHCPeriod = saxcdb->GetEndRunRange();

	SetDefaultStorage(fLHCPeriod.Data());
	if(!fDefaultStorage) IlcFatal(Form("%s storage not there! Please check!",fLHCPeriod.Data()));

}

//_____________________________________________________________________________
void IlcCDBManager::UnsetDefaultStorage() {
// Unset default storage
	
	// if lock is ON, action is forbidden!
	if(fLock) {
		if (fDefaultStorage) {
			IlcFatal("Lock is ON: cannot unset default storage!");
		}
	}
	
	if (fDefaultStorage) {
		IlcWarning("Clearing cache!");
		ClearCache();
	}

	fRun = fStartRunLHCPeriod = fEndRunLHCPeriod = -1;
	fRaw = kFALSE;
	
	fDefaultStorage = 0x0;
}

//_____________________________________________________________________________
void IlcCDBManager::SetSpecificStorage(const char* calibType, const char* dbString) {
// sets storage specific for detector or calibration type (works with IlcCDBManager::Get(...))

	IlcCDBParam *aPar = CreateParameter(dbString);
	if(!aPar) return;
	SetSpecificStorage(calibType, aPar);
	delete aPar;
}

//_____________________________________________________________________________
void IlcCDBManager::SetSpecificStorage(const char* calibType, const IlcCDBParam* param) {
// sets storage specific for detector or calibration type (works with IlcCDBManager::Get(...))
// Default storage should be defined prior to any specific storages, e.g.:
// IlcCDBManager::instance()->SetDefaultStorage("alien://");
// IlcCDBManager::instance()->SetSpecificStorage("TPC/*","local://DB_TPC");
// IlcCDBManager::instance()->SetSpecificStorage("*/Align/*","local://DB_TPCAlign");
// calibType must be a valid CDB path! (3 level folder structure)


	if(!fDefaultStorage && !fRaw) {
		IlcError("Please activate a default storage first!");
		return;
	}


	IlcCDBPath aPath(calibType);
	if(!aPath.IsValid()){
		IlcError(Form("Not a valid path: %s", calibType));
		return;
	}

	TObjString *objCalibType = new TObjString(aPath.GetPath());
	if(fSpecificStorages.Contains(objCalibType)){
		IlcWarning(Form("Storage \"%s\" already activated! It will be replaced by the new one",
					calibType));
		IlcCDBParam *checkPar = dynamic_cast<IlcCDBParam*> (fSpecificStorages.GetValue(calibType));
		if(checkPar) delete checkPar;
		delete fSpecificStorages.Remove(objCalibType);
	}
	IlcCDBStorage *aStorage = GetStorage(param);
	if(!aStorage) return;

 	fSpecificStorages.Add(objCalibType, param->CloneParam());

	if(fStorageMap->Contains(objCalibType)){
		delete fStorageMap->Remove(objCalibType);
	}
	fStorageMap->Add(objCalibType->Clone(), new TObjString(param->GetURI()));

}

//_____________________________________________________________________________
IlcCDBStorage* IlcCDBManager::GetSpecificStorage(const char* calibType) {
// get storage specific for detector or calibration type 

	IlcCDBPath calibPath(calibType);
	if(!calibPath.IsValid()) return NULL;

	IlcCDBParam *checkPar = (IlcCDBParam*) fSpecificStorages.GetValue(calibPath.GetPath());
	if(!checkPar){
		IlcError(Form("%s storage not found!", calibType));
		return NULL;
	} else {
		return GetStorage(checkPar);
	}

}

//_____________________________________________________________________________
IlcCDBParam* IlcCDBManager::SelectSpecificStorage(const TString& path) {
// select storage valid for path from the list of specific storages

	IlcCDBPath aPath(path);
	if(!aPath.IsValid()) return NULL;

	TIter iter(&fSpecificStorages);
	TObjString *aCalibType=0;
	IlcCDBPath tmpPath("null/null/null");
	IlcCDBParam* aPar=0;
	while((aCalibType = (TObjString*) iter.Next())){
		IlcCDBPath calibTypePath(aCalibType->GetName());
		if(calibTypePath.Comprises(aPath)) {
			if(calibTypePath.Comprises(tmpPath)) continue;
		 	aPar = (IlcCDBParam*) fSpecificStorages.GetValue(aCalibType);
			tmpPath.SetPath(calibTypePath.GetPath());
		}
	}
	return aPar;
}

//_____________________________________________________________________________
IlcCDBEntry* IlcCDBManager::Get(const IlcCDBPath& path, Int_t runNumber,
	Int_t version, Int_t subVersion) {
// get an IlcCDBEntry object from the database

	if(runNumber < 0){
		// RunNumber is not specified. Try with fRun
  		if (fRun < 0){
   	 		IlcError("Run number neither specified in query nor set in IlcCDBManager! Use IlcCDBManager::SetRun.");
    			return NULL;
  		}
		runNumber = fRun;
	}

	return Get(IlcCDBId(path, runNumber, runNumber, version, subVersion));
}

//_____________________________________________________________________________
IlcCDBEntry* IlcCDBManager::Get(const IlcCDBPath& path,
	const IlcCDBRunRange& runRange, Int_t version,
	Int_t subVersion) {
// get an IlcCDBEntry object from the database!

	return Get(IlcCDBId(path, runRange, version, subVersion));
}

//_____________________________________________________________________________
IlcCDBEntry* IlcCDBManager::Get(const IlcCDBId& query, Bool_t forceCaching) {
// get an IlcCDBEntry object from the database
	
	// check if query's path and runRange are valid
	// query is invalid also if version is not specified and subversion is!
	if (!query.IsValid()) {
		IlcError(Form("Invalid query: %s", query.ToString().Data()));
		return NULL;
	}
	
	// query is not specified if path contains wildcard or run range= [-1,-1]
 	if (!query.IsSpecified()) {
		IlcError(Form("Unspecified query: %s",
				query.ToString().Data()));
                return NULL;
	}

	if(fLock && !(fRun >= query.GetFirstRun() && fRun <= query.GetLastRun())) 
		IlcFatal("Lock is ON: cannot use different run number than the internal one!");
	
	if(fCache && !(fRun >= query.GetFirstRun() && fRun <= query.GetLastRun())) 
		IlcWarning("Run number explicitly set in query: CDB cache temporarily disabled!");

  	IlcCDBEntry *entry=0;

  	// first look into map of cached objects
  	if(fCache && query.GetFirstRun() == fRun)
		entry = (IlcCDBEntry*) fEntryCache.GetValue(query.GetPath());
  	if(entry) {
		IlcDebug(2, Form("Object %s retrieved from cache !!",query.GetPath().Data()));
		return entry;
	}

	// if snapshot flag is set, try getting from the snapshot
	// but in the case a specific storage is specified for this path
	IlcCDBParam *aPar=SelectSpecificStorage(query.GetPath());
	if(!aPar){
	    if(fSnapshotMode && query.GetFirstRun() == fRun)
	    {
		entry = GetEntryFromSnapshot(query.GetPath());
		if(entry) {
		    IlcInfo(Form("Object \"%s\" retrieved from the snapshot.",query.GetPath().Data()));
		    if(query.GetFirstRun() == fRun) // no need to check fCache, fSnapshotMode not possible otherwise
			CacheEntry(query.GetPath(), entry);

		    if(!fIds->Contains(&entry->GetId()))
			fIds->Add(entry->GetId().Clone());

		    return entry;
		}
	    }
	}

	// Entry is not in cache (and, in case we are in snapshot mode, not in the snapshot either)
	// => retrieve it from the storage and cache it!!
	if(!fDefaultStorage) {
		IlcError("No storage set!");
		return NULL;
	}

	IlcCDBStorage *aStorage=0;
	if(aPar) {
		aStorage=GetStorage(aPar);
		TString str = aPar->GetURI();
		IlcDebug(2,Form("Looking into storage: %s",str.Data()));
	} else {
		aStorage=GetDefaultStorage();
		IlcDebug(2,"Looking into default storage");
	}

	entry = aStorage->Get(query);

 	if(entry && fCache && (query.GetFirstRun()==fRun || forceCaching)){
		CacheEntry(query.GetPath(), entry);
	}

	if(entry && !fIds->Contains(&entry->GetId())){
		fIds->Add(entry->GetId().Clone());
	}


  	return entry;

}

//_____________________________________________________________________________
IlcCDBEntry* IlcCDBManager::GetEntryFromSnapshot(const char* path) {
    // get the entry from the open snapshot file

    TString sPath(path);
    sPath.ReplaceAll("/","*");
    IlcCDBEntry *entry = dynamic_cast<IlcCDBEntry*>(fSnapshotFile->Get(sPath.Data()));
    if(!entry){
	IlcDebug(2,Form("Cannot get a CDB entry for \"%s\" from snapshot file",path));
	return 0;
    }

    return entry;
}

//_____________________________________________________________________________
Bool_t IlcCDBManager::SetSnapshotMode(const char* snapshotFileName) {
// set the manager in snapshot mode
    
    if(!fCache){
	IlcError("Cannot set the CDB manage in snapshot mode if the cache is not active!");
	return kFALSE;
    }

    //open snapshot file
    TString snapshotFile(snapshotFileName);
    if(snapshotFile.BeginsWith("alien://")){
	if(!gGrid) {
	    TGrid::Connect("alien://","");
	    if(!gGrid) {
		IlcError("Connection to alien failed!");
		return kFALSE;
	    }
	}
    }

    fSnapshotFile = TFile::Open(snapshotFileName);
    if (!fSnapshotFile || fSnapshotFile->IsZombie()){
	IlcError(Form("Cannot open file %s",snapshotFileName));
	return kFALSE;
    }

    Printf("The CDB manager is set in snapshot mode: cache->snapshot->defaultstorage");
    fSnapshotMode = kTRUE;
    return kTRUE;

}

//_____________________________________________________________________________
const char* IlcCDBManager::GetURI(const char* path) {
// return the URI of the storage where to look for path

	if(!IsDefaultStorageSet()) return 0;
	
	IlcCDBParam *aPar=SelectSpecificStorage(path);

	if(aPar) {
		return aPar->GetURI().Data();

	} else {
		return GetDefaultStorage()->GetURI().Data();
	}
	
	return 0;
}

//_____________________________________________________________________________
IlcCDBId* IlcCDBManager::GetId(const IlcCDBPath& path, Int_t runNumber,
	Int_t version, Int_t subVersion) {
// get the IlcCDBId of the valid object from the database (does not retrieve the object)
// User must delete returned object!

	if(runNumber < 0){
		// RunNumber is not specified. Try with fRun
  		if (fRun < 0){
   	 		IlcError("Run number neither specified in query nor set in IlcCDBManager! Use IlcCDBManager::SetRun.");
    			return NULL;
  		}
		runNumber = fRun;
	}

	return GetId(IlcCDBId(path, runNumber, runNumber, version, subVersion));
}

//_____________________________________________________________________________
IlcCDBId* IlcCDBManager::GetId(const IlcCDBPath& path,
	const IlcCDBRunRange& runRange, Int_t version,
	Int_t subVersion) {
// get the IlcCDBId of the valid object from the database (does not retrieve the object)
// User must delete returned object!

	return GetId(IlcCDBId(path, runRange, version, subVersion));
}

//_____________________________________________________________________________
IlcCDBId* IlcCDBManager::GetId(const IlcCDBId& query) {
// get the IlcCDBId of the valid object from the database (does not retrieve the object)
// User must delete returned object!

	if(!fDefaultStorage) {
		IlcError("No storage set!");
		return NULL;
	}

	// check if query's path and runRange are valid
	// query is invalid also if version is not specified and subversion is!
	if (!query.IsValid()) {
		IlcError(Form("Invalid query: %s", query.ToString().Data()));
		return NULL;
	}
	
	// query is not specified if path contains wildcard or run range= [-1,-1]
 	if (!query.IsSpecified()) {
		IlcError(Form("Unspecified query: %s",
				query.ToString().Data()));
                return NULL;
	}

	if(fCache && query.GetFirstRun() != fRun)
		IlcWarning("Run number explicitly set in query: CDB cache temporarily disabled!");

	IlcCDBEntry* entry = 0;

  	// first look into map of cached objects
  	if(fCache && query.GetFirstRun() == fRun)
		entry = (IlcCDBEntry*) fEntryCache.GetValue(query.GetPath());

  	if(entry) {
		IlcDebug(2, Form("Object %s retrieved from cache !!",query.GetPath().Data()));
		return dynamic_cast<IlcCDBId*> (entry->GetId().Clone());
	}

	// Entry is not in cache -> retrieve it from CDB and cache it!!
	IlcCDBStorage *aStorage=0;
	IlcCDBParam *aPar=SelectSpecificStorage(query.GetPath());

	if(aPar) {
		aStorage=GetStorage(aPar);
		TString str = aPar->GetURI();
		IlcDebug(2,Form("Looking into storage: %s",str.Data()));
		
	} else {
		aStorage=GetDefaultStorage();
		IlcDebug(2,"Looking into default storage");
	}

  	return aStorage->GetId(query);

}

//_____________________________________________________________________________
TList* IlcCDBManager::GetAll(const IlcCDBPath& path, Int_t runNumber,
	Int_t version, Int_t subVersion) {
// get multiple IlcCDBEntry objects from the database

	if(runNumber < 0){
		// RunNumber is not specified. Try with fRun
  		if (fRun < 0){
   	 		IlcError("Run number neither specified in query nor set in IlcCDBManager! Use IlcCDBManager::SetRun.");
    			return NULL;
  		}
		runNumber = fRun;
	}

	return GetAll(IlcCDBId(path, runNumber, runNumber, version, 	
			subVersion));
}

//_____________________________________________________________________________
TList* IlcCDBManager::GetAll(const IlcCDBPath& path,
	const IlcCDBRunRange& runRange, Int_t version, Int_t subVersion) {
// get multiple IlcCDBEntry objects from the database

	return GetAll(IlcCDBId(path, runRange, version, subVersion));
}

//_____________________________________________________________________________
TList* IlcCDBManager::GetAll(const IlcCDBId& query) {
// get multiple IlcCDBEntry objects from the database
// Warning: this method works correctly only for queries of the type "Detector/*"
// 		and not for more specific queries e.g. "Detector/Calib/*" !
// Warning #2: Entries are cached, but GetAll will keep on retrieving objects from OCDB!
// 		To get an object from cache use Get() function

	if(!fDefaultStorage) {
		IlcError("No storage set!");
		return NULL;
	}

	if (!query.IsValid()) {
                IlcError(Form("Invalid query: %s", query.ToString().Data()));
                return NULL;
        }

	if((fSpecificStorages.GetEntries()>0) && query.GetPath().BeginsWith('*')){
                // if specific storages are active a query with "*" is ambiguous
		IlcError("Query too generic in this context!");
                return NULL;
	}

	if (query.IsAnyRange()) {
		IlcError(Form("Unspecified run or runrange: %s",
				query.ToString().Data()));
		return NULL;
	}

	if(fLock && query.GetFirstRun() != fRun)
		IlcFatal("Lock is ON: cannot use different run number than the internal one!");
	
	IlcCDBParam *aPar=SelectSpecificStorage(query.GetPath());

	IlcCDBStorage *aStorage;
	if(aPar) {
		aStorage=GetStorage(aPar);
		IlcDebug(2,Form("Looking into storage: %s", aPar->GetURI().Data()));

	} else {
		aStorage=GetDefaultStorage();
		IlcDebug(2,"Looking into default storage");
	}

	TList *result = 0;
	if(aStorage) result = aStorage->GetAll(query);
	if(!result) return 0;

       // loop on result to check whether entries should be re-queried with specific storages
	if(fSpecificStorages.GetEntries()>0 && ! (fSpecificStorages.GetEntries() == 1 && aPar)) {
		IlcInfo("Now look into all other specific storages...");

		TIter iter(result);
		IlcCDBEntry* chkEntry=0;

		while((chkEntry = dynamic_cast<IlcCDBEntry*> (iter.Next()))){
			IlcCDBId& chkId = chkEntry->GetId();
			IlcDebug(2, Form("Checking id %s ", chkId.GetPath().Data()));
			IlcCDBParam *chkPar=SelectSpecificStorage(chkId.GetPath());
			if (!chkPar || aPar == chkPar) continue;
			IlcCDBStorage *chkStorage = GetStorage(chkPar);
			IlcDebug(2, Form("Found specific storage! %s", chkPar->GetURI().Data()));

			IlcCDBEntry *newEntry=0;
			chkId.SetRunRange(query.GetFirstRun(), query.GetLastRun());
			chkId.SetVersion(query.GetVersion());
			chkId.SetSubVersion(query.GetSubVersion());

			if(chkStorage) newEntry = chkStorage->Get(chkId);
			if(!newEntry) continue;

			// object is found in specific storage: replace entry in the result list!
			chkEntry->SetOwner(1);
			delete result->Remove(chkEntry);
			result->AddFirst(newEntry);
		}

		Int_t nEntries = result->GetEntries();
		IlcInfo("After look into other specific storages, result list is:");
		for(int i=0; i<nEntries;i++){
			IlcCDBEntry *entry = (IlcCDBEntry*) result->At(i);
			IlcInfo(Form("%s",entry->GetId().ToString().Data()));
		}
	}

	// caching entries
	TIter iter(result);
	IlcCDBEntry* entry=0;
	while((entry = dynamic_cast<IlcCDBEntry*> (iter.Next()))){

		if(!fIds->Contains(&entry->GetId())){
			fIds->Add(entry->GetId().Clone());
		}
		if(fCache && (query.GetFirstRun() == fRun)){
			CacheEntry(entry->GetId().GetPath(), entry);
		}
	}


	return result;
}

//_____________________________________________________________________________
Bool_t IlcCDBManager::Put(TObject* object, const IlcCDBId& id, IlcCDBMetaData* metaData, const char* mirrors, const DataType type){
// store an IlcCDBEntry object into the database

	if (object==0x0) {
		IlcError("Null Entry! No storage will be done!");
		return kFALSE;
	} 

	IlcCDBEntry anEntry(object, id, metaData);
	return Put(&anEntry, mirrors, type);

}


//_____________________________________________________________________________
Bool_t IlcCDBManager::Put(IlcCDBEntry* entry, const char* mirrors, DataType type){
// store an IlcCDBEntry object into the database

	if(type == kPrivate && !fDefaultStorage) {
		IlcError("No storage set!");
		return kFALSE;
	}

	if (!entry){
		IlcError("No entry!");
		return kFALSE;
	}

	if (entry->GetObject()==0x0){
		IlcError("No valid object in CDB entry!");
		return kFALSE;
	}

	if (!entry->GetId().IsValid()) {
		IlcError(Form("Invalid entry ID: %s", 
			entry->GetId().ToString().Data()));
		return kFALSE;
	}	

	if (!entry->GetId().IsSpecified()) {
		IlcError(Form("Unspecified entry ID: %s", 
			entry->GetId().ToString().Data()));
		return kFALSE;
	}

	IlcCDBId id = entry->GetId();
	IlcCDBParam *aPar = SelectSpecificStorage(id.GetPath());

	IlcCDBStorage *aStorage=0;
	
	if(aPar) {
		aStorage=GetStorage(aPar);
	} else {
		switch(type){
			case kCondition:
				aStorage = GetStorage(fCondParam);
				break;
			case kReference:
				aStorage = GetStorage(fRefParam);
				break;
			case kPrivate:
				aStorage = GetDefaultStorage();
				break;
		}
	}

	IlcDebug(2,Form("Storing object into storage: %s", aStorage->GetURI().Data()));

	TString strMirrors(mirrors);
	Bool_t result = "kFALSE";
	if(!strMirrors.IsNull() && !strMirrors.IsWhitespace())
	    result = aStorage->Put(entry, mirrors, type);
	else
	    result = aStorage->Put(entry, "", type);

	if(fRun >= 0) QueryCDB();

	return result;


}

//_____________________________________________________________________________
void IlcCDBManager::SetMirrorSEs(const char* mirrors)
{
// set mirror Storage Elements for the default storage, if it is of type "alien"
    if(fDefaultStorage->GetType() != "alien"){
	IlcInfo("The default storage is not of type \"alien\". Settings for Storage Elements are not taken into account!");
	return;
    }
    fDefaultStorage->SetMirrorSEs(mirrors);
}

//_____________________________________________________________________________
const char* IlcCDBManager::GetMirrorSEs() const {
// get mirror Storage Elements for the default storage, if it is of type "alien"
    if(fDefaultStorage->GetType() != "alien"){
	IlcInfo("The default storage is not of type \"alien\". Settings for Storage Elements are not taken into account!");
	return "";
    }
    return fDefaultStorage->GetMirrorSEs();
}

//_____________________________________________________________________________
void IlcCDBManager::CacheEntry(const char* path, IlcCDBEntry* entry)
{
// cache IlcCDBEntry. Cache is valid until run number is changed.

	IlcCDBEntry *chkEntry = dynamic_cast<IlcCDBEntry*> (fEntryCache.GetValue(path));

  	if(chkEntry) {
		IlcDebug(2, Form("Object %s already in cache !!", path));
		return;
	} else {
		IlcDebug(2,Form("Caching entry %s", path));
	}

	fEntryCache.Add(new TObjString(path), entry);
	IlcDebug(2,Form("Cache entries: %d", fEntryCache.GetEntries()));

}

//_____________________________________________________________________________
void IlcCDBManager::Print(Option_t* /*option*/) const
{
// Print list of active storages and their URIs

	TString output=Form("Run number = %d; ",fRun);
	output += "Cache is ";
	if(!fCache) output += "NOT ";
	output += Form("ACTIVE; Number of active storages: %d\n",fActiveStorages.GetEntries());

	if(fDefaultStorage) {
		output += Form("\t*** Default Storage URI: \"%s\"\n",fDefaultStorage->GetURI().Data());
//		IlcInfo(output.Data());
	}
	if(fSpecificStorages.GetEntries()>0) {
		TIter iter(fSpecificStorages.GetTable());
		TPair *aPair=0;
		Int_t i=1;
		while((aPair = (TPair*) iter.Next())){
			output += Form("\t*** Specific storage %d: Path \"%s\" -> URI \"%s\"\n",
				i++, ((TObjString*) aPair->Key())->GetName(),
				((IlcCDBParam*) aPair->Value())->GetURI().Data());
		}
	}
	if(fDrainStorage) {
		output += Form("*** Drain Storage URI: %s\n",fDrainStorage->GetURI().Data());
	}
	IlcInfo(output.Data());
}

//_____________________________________________________________________________
void IlcCDBManager::SetRun(Int_t run)
{
// Sets current run number.
// When the run number changes the caching is cleared.
  	
	if(fRun == run)
		return;
  
	if(fLock && fRun >= 0) {
		IlcFatal("Lock is ON, cannot reset run number!");
	}	
		
	fRun = run;
	if(fRaw){
		// here the LHCPeriod xml file is parsed; the string containing the correct period is returned; the default storage is set
		if (fStartRunLHCPeriod <= run && fEndRunLHCPeriod >= run){
			IlcInfo("LHCPeriod alien folder for current run already in memory");
		}else{
			SetDefaultStorageFromRun(run);
			if(fEntryCache.GetEntries()!=0) ClearCache();
			return;
		}
	}
	ClearCache();
	QueryCDB();
}

//_____________________________________________________________________________
void IlcCDBManager::ClearCache(){
// clear IlcCDBEntry cache

	IlcDebug(2, Form("Cache entries to be deleted: %d",fEntryCache.GetEntries()));
	
	/*
	// To clean entries one by one
	TIter iter(fEntryCache.GetTable());
	TPair* pair=0;
	while((pair= dynamic_cast<TPair*> (iter.Next()))){
	
		TObjString* key = dynamic_cast<TObjString*> (pair->Key());
		IlcCDBEntry* entry = dynamic_cast<IlcCDBEntry*> (pair->Value());
		IlcDebug(2, Form("Deleting entry: %s", key->GetName()));
		if (entry) delete entry;
		delete fEntryCache.Remove(key);
	}
	*/
	fEntryCache.DeleteAll();
	IlcDebug(2, Form("After deleting - Cache entries: %d",fEntryCache.GetEntries()));
}

//_____________________________________________________________________________
void IlcCDBManager::UnloadFromCache(const char* path){
// unload cached object
// that is remove the entry from the cache and the id from the list of ids
//
	if(!fActiveStorages.GetEntries()) {
		IlcDebug(2, Form("No active storages. Object \"%s\" is not unloaded from cache", path));
		return;
	}

	IlcCDBPath queryPath(path);
	if(!queryPath.IsValid()) return;

	if(!queryPath.IsWildcard()) { // path is not wildcard, get it directly from the cache and unload it!
		if(fEntryCache.Contains(path)){
			IlcDebug(2, Form("Unloading object \"%s\" from cache and from list of ids", path));
			TObjString pathStr(path);
			delete fEntryCache.Remove(&pathStr);
			// we do not remove from the list of Id's (it's not very coherent but we leave the
			// id for the benefit of the userinfo)
			/*
			TIter iter(fIds);
			IlcCDBId *id = 0;
			while((id = dynamic_cast<IlcCDBId*> (iter.Next()))){
			    if(queryPath.Comprises(id->GetPath()))
				delete fIds->Remove(id);
			}*/
		} else {
		  IlcError(Form("Cache does not contain object \"%s\"!", path));
		}
		IlcDebug(2, Form("Cache entries: %d",fEntryCache.GetEntries()));
		return;
	}

	// path is wildcard: loop on the cache and unload all comprised objects!
	TIter iter(fEntryCache.GetTable());
	TPair* pair = 0;
	Int_t removed=0;

	while((pair = dynamic_cast<TPair*> (iter.Next()))){
		IlcCDBPath entryPath = pair->Key()->GetName();
		if(queryPath.Comprises(entryPath)) {
			IlcDebug(2, Form("Unloading object \"%s\" from cache and from list of ids", entryPath.GetPath().Data()));
			TObjString pathStr(entryPath.GetPath());
			delete fEntryCache.Remove(&pathStr);
			removed++;

			// we do not remove from the list of Id's (it's not very coherent but we leave the
			// id for the benefit of the userinfo)
			/*
			TIter iterids(fIds);
			IlcCDBId *anId = 0;
			while((anId = dynamic_cast<IlcCDBId*> (iterids.Next()))){
			    IlcCDBPath aPath = anId->GetPath();
			    TString aPathStr = aPath.GetPath();
			    if(queryPath.Comprises(aPath)) {
				delete fIds->Remove(anId);
			    }
			}*/
		}
	}
	IlcDebug(2,Form("Cache entries and ids removed: %d   Remaining: %d",removed,fEntryCache.GetEntries()));
}

//_____________________________________________________________________________
void IlcCDBManager::DestroyActiveStorages() {
// delete list of active storages

	fActiveStorages.DeleteAll();
	fSpecificStorages.DeleteAll();
}

//_____________________________________________________________________________
void IlcCDBManager::DestroyActiveStorage(IlcCDBStorage* /*storage*/) {
// destroys active storage

/*
	TIter iter(fActiveStorages.GetTable());
	TPair* aPair;
	while ((aPair = (TPair*) iter.Next())) {
		if(storage == (IlcCDBStorage*) aPair->Value())
			delete fActiveStorages.Remove(aPair->Key());
			storage->Delete(); storage=0x0;
	}
*/

}

//_____________________________________________________________________________
void IlcCDBManager::QueryCDB() {
// query default and specific storages for files valid for fRun. Every storage loads the Ids into its list.

	if (fRun < 0){
		IlcError("Run number not yet set! Use IlcCDBManager::SetRun.");
	return;
	}
	if (!fDefaultStorage){
		IlcError("Default storage is not set! Use IlcCDBManager::SetDefaultStorage");
	return;
	}
	if(fDefaultStorage->GetType() == "alien"){
		fDefaultStorage->QueryCDB(fRun);
	} else {
		IlcDebug(2,"Skipping query for valid files, it used only in grid...");
	}

	TIter iter(&fSpecificStorages);
	TObjString *aCalibType=0;
	IlcCDBParam* aPar=0;
	while((aCalibType = dynamic_cast<TObjString*> (iter.Next()))){
		aPar = (IlcCDBParam*) fSpecificStorages.GetValue(aCalibType);
		if(aPar) {
			IlcDebug(2,Form("Querying specific storage %s",aCalibType->GetName()));
			IlcCDBStorage *aStorage = GetStorage(aPar);
			if(aStorage->GetType() == "alien"){
				aStorage->QueryCDB(fRun,aCalibType->GetName());
			} else {
				IlcDebug(2,
					"Skipping query for valid files, it is used only in grid...");
			}
		}
	}
}

//______________________________________________________________________________________________
const char* IlcCDBManager::GetDataTypeName(DataType type)
{
  // returns the name (string) of the data type

      switch (type){
	    case kCondition: return "Conditions";
	    case kReference: return "Reference";
	    case kPrivate: return "Private";
     }
     return 0;

}

//______________________________________________________________________________________________
void IlcCDBManager::InitShortLived()
{
  // Init the list of short-lived objects
  // currently disabled

	fShortLived=0x0;

// 	fShortLived = new TList();
// 	fShortLived->SetOwner(1);
//
// 	fShortLived->Add(new TObjString("EMCAL/Calib/Data"));
// 
// 	fShortLived->Add(new TObjString("HMPID/Calib/Nmean"));
// 	fShortLived->Add(new TObjString("HMPID/Calib/Qthre"));
// 
// 	fShortLived->Add(new TObjString("ITS/Calib/CalibSPD"));
// 
// 	fShortLived->Add(new TObjString("MUON/Calib/Gains"));
// 	fShortLived->Add(new TObjString("MUON/Calib/HV"));
// 	fShortLived->Add(new TObjString("MUON/Calib/Pedestals"));
// 
// 	fShortLived->Add(new TObjString("PHOS/Calib/CpvGainPedestals"));
// 	fShortLived->Add(new TObjString("PHOS/Calib/EmcGainPedestals"));
// 
// 	fShortLived->Add(new TObjString("PMD/Calib/Data"));
// 
// 	fShortLived->Add(new TObjString("TRD/Calib/ChamberGainFactor"));
// 	fShortLived->Add(new TObjString("TRD/Calib/LocalGainFactor"));
// 	fShortLived->Add(new TObjString("TRD/Calib/ChamberT0"));
// 	fShortLived->Add(new TObjString("TRD/Calib/LocalT0"));
// 	fShortLived->Add(new TObjString("TRD/Calib/ChamberVdrift"));
// 	fShortLived->Add(new TObjString("TRD/Calib/LocalVdrift"));
// 
// 	fShortLived->Add(new TObjString("ZDC/Calib/Data"));

}

//______________________________________________________________________________________________
Bool_t IlcCDBManager::IsShortLived(const char* path)
{
  // returns the name (string) of the data type

	if(!fShortLived) return kFALSE;

	IlcCDBPath aPath(path);
	if(!aPath.IsValid()){
		IlcError(Form("Not a valid path: %s", path));
		return kFALSE;
	}

	return fShortLived->Contains(path);

}

//______________________________________________________________________________________________
ULong_t IlcCDBManager::SetLock(Bool_t lock, ULong_t key){
  // To lock/unlock user must provide the key. A new key is provided after
  // each successful lock. User should always backup the returned key and
  // use it on next access.
  if (fLock == lock) return 0;  // nothing to be done
  if (lock) {
    // User wants to lock - check his identity
    if (fKey) {
      // Lock has a user - check his key
      if (fKey != key) {
        IlcFatal("Wrong key provided to lock CDB. Please remove CDB lock access from your code !");
        return 0;
      }  
    }  
    // Provide new key 
    fKey = gSystem->Now();
    fLock = kTRUE;
    return fKey;
  }
  // User wants to unlock - check the provided key
  if (key != fKey) {
    IlcFatal("Lock is ON: wrong key provided");
    return 0;
  }  
  fLock = kFALSE;
  return key;  
}

///////////////////////////////////////////////////////////
// IlcCDBManager Parameter class                         //
// interface to specific IlcCDBParameter class           //
// (IlcCDBGridParam, IlcCDBLocalParam, IlcCDBDumpParam)  //
///////////////////////////////////////////////////////////

IlcCDBParam::IlcCDBParam():
  fType(),
  fURI()
{
// constructor

}

//_____________________________________________________________________________
IlcCDBParam::~IlcCDBParam() {
// destructor

}


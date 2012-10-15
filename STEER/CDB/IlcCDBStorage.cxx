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

#include <TKey.h>
#include <TH1.h>
#include <TTree.h>
#include <TNtuple.h>
#include <TFile.h>
#include "IlcCDBStorage.h"
#include "IlcCDBGrid.h"

#include "IlcCDBEntry.h"
#include "IlcLog.h"

ClassImp(IlcCDBStorage)

//_____________________________________________________________________________
IlcCDBStorage::IlcCDBStorage():
fValidFileIds(),
fRun(-1),
fPathFilter(),
fVersion(-1),
fMetaDataFilter(0),
fSelections(),
fURI(),
fType(),
fBaseFolder(),
fNretry(0),
fInitRetrySeconds(0)
{
// constructor

	fValidFileIds.SetOwner(1);
	fSelections.SetOwner(1);
}

//_____________________________________________________________________________
IlcCDBStorage::~IlcCDBStorage() {
// destructor

	RemoveAllSelections();
	fValidFileIds.Clear();
	delete fMetaDataFilter;

}

//_____________________________________________________________________________
void IlcCDBStorage::GetSelection(/*const*/ IlcCDBId* id) {
// return required version and subversion from the list of selection criteria
	
	TIter iter(&fSelections);
	IlcCDBId* aSelection;
        
	// loop on the list of selection criteria
	while ((aSelection = (IlcCDBId*) iter.Next())) {
		// check if selection element contains id's path and run (range) 
		if (aSelection->Comprises(*id)) {
			IlcDebug(2,Form("Using selection criterion: %s ", aSelection->ToString().Data()));
			// return required version and subversion
			
			id->SetVersion(aSelection->GetVersion());
			id->SetSubVersion(aSelection->GetSubVersion());
			return;  
		}
	}
	
	// no valid element is found in the list of selection criteria -> return
	IlcDebug(2,"Looking for objects with most recent version");
	return;
}

//_____________________________________________________________________________
void IlcCDBStorage::ReadSelectionFromFile(const char *fileName){
// read selection criteria list from file
	
	RemoveAllSelections();
	
	TList *list = GetIdListFromFile(fileName);
	if(!list) return;
	
	list->SetOwner();	
	Int_t nId = list->GetEntries();
	IlcCDBId *id;
	TKey *key;
	
	for(int i=nId-1;i>=0;i--){
		key = (TKey*) list->At(i);
		id = (IlcCDBId*) key->ReadObj();
		if(id) AddSelection(*id);
	}
	delete list;
	IlcInfo(Form("Selection criteria list filled with %d entries",fSelections.GetEntries()));
	PrintSelectionList();
	
}

//_____________________________________________________________________________
void IlcCDBStorage::AddSelection(const IlcCDBId& selection) {
// add a selection criterion

	IlcCDBPath path = selection.GetPath();
	if(!path.IsValid()) return;
	
	TIter iter(&fSelections);
	const IlcCDBId *anId;
	while((anId = (IlcCDBId*) iter.Next())){
		if(selection.Comprises(*anId)){
			IlcWarning("This selection is more general than a previous one and will hide it!");
			IlcWarning(Form("%s", (anId->ToString()).Data()));
			fSelections.AddBefore(anId, new IlcCDBId(selection));
			return;
		}
	
	}
	fSelections.AddFirst(new IlcCDBId(selection));
}

//_____________________________________________________________________________
void IlcCDBStorage::AddSelection(const IlcCDBPath& path,
	const IlcCDBRunRange& runRange, Int_t version, Int_t subVersion){
// add a selection criterion

	AddSelection(IlcCDBId(path, runRange, version, subVersion));
}

//_____________________________________________________________________________
void IlcCDBStorage::AddSelection(const IlcCDBPath& path,
	Int_t firstRun, Int_t lastRun, Int_t version, Int_t subVersion){
// add a selection criterion
	
	AddSelection(IlcCDBId(path, firstRun, lastRun, version, subVersion));
}

//_____________________________________________________________________________
void IlcCDBStorage::RemoveSelection(const IlcCDBId& selection) {
// remove a selection criterion

	TIter iter(&fSelections);
	IlcCDBId* aSelection;

	while ((aSelection = (IlcCDBId*) iter.Next())) {
		if (selection.Comprises(*aSelection)) {
			fSelections.Remove(aSelection);
		}
	}
}

//_____________________________________________________________________________
void IlcCDBStorage::RemoveSelection(const IlcCDBPath& path, 
	const IlcCDBRunRange& runRange){
// remove a selection criterion

	RemoveSelection(IlcCDBId(path, runRange, -1, -1));
}

//_____________________________________________________________________________
void IlcCDBStorage::RemoveSelection(const IlcCDBPath& path,
	Int_t firstRun, Int_t lastRun){
// remove a selection criterion

	RemoveSelection(IlcCDBId(path, firstRun, lastRun, -1, -1));
}

//_____________________________________________________________________________
void IlcCDBStorage::RemoveSelection(int position){
// remove a selection criterion from its position in the list

	delete fSelections.RemoveAt(position);
}

//_____________________________________________________________________________
void IlcCDBStorage::RemoveAllSelections(){
// remove all selection criteria

	fSelections.Clear();
}

//_____________________________________________________________________________
void IlcCDBStorage::PrintSelectionList(){
// prints the list of selection criteria

	TIter iter(&fSelections);
	IlcCDBId* aSelection;

	// loop on the list of selection criteria
	int index=0;
	while ((aSelection = (IlcCDBId*) iter.Next())) {
		IlcInfo(Form("index %d -> selection: %s",index++, aSelection->ToString().Data()));
	}

}

//_____________________________________________________________________________
IlcCDBEntry* IlcCDBStorage::Get(const IlcCDBId& query) {
// get an IlcCDBEntry object from the database

	// check if query's path and runRange are valid
	// query is invalid also if version is not specified and subversion is!
	if (!query.IsValid()) {
		IlcError(Form("Invalid query: %s", query.ToString().Data()));
		return NULL;
	}

	// query is not specified if path contains wildcard or runrange = [-1,-1]
	if (!query.IsSpecified()) {
		IlcError(Form("Unspecified query: %s",
				query.ToString().Data()));
                return NULL;
	}

	// This is needed otherwise TH1  objects (histos, TTree's) are lost when file is closed!
	Bool_t oldStatus = TH1::AddDirectoryStatus();
	TH1::AddDirectory(kFALSE);

	IlcCDBEntry* entry = GetEntry(query);

	if (oldStatus != kFALSE)
  		TH1::AddDirectory(kTRUE);

  	if (entry) {
		// this is to make the SHUTTLE output lighter
		if(!(query.GetPath().Contains("SHUTTLE/STATUS")))
    			IlcDebug(2, Form("CDB object retrieved: %s", entry->GetId().ToString().Data()));
  	} else {
		// this is to make the SHUTTLE output lighter
		if(!(query.GetPath().Contains("SHUTTLE/STATUS"))){

			if(!(query.GetPath().Contains("SHUTTLE"))){
				IlcFatal(Form("No valid CDB object found! request was: %s", query.ToString().Data()));
			}
			else {
				IlcInfo(Form("No valid CDB object found! request was: %s", query.ToString().Data()));
			}
		}
  	}

	// if drain storage is set, drain entry into drain storage
	if(entry && (IlcCDBManager::Instance())->IsDrainSet())
		IlcCDBManager::Instance()->Drain(entry);

	return entry;
}

//_____________________________________________________________________________
IlcCDBEntry* IlcCDBStorage::Get(const IlcCDBPath& path, Int_t runNumber,
	Int_t version, Int_t subVersion) {
// get an IlcCDBEntry object from the database

	return Get(IlcCDBId(path, runNumber, runNumber, version, subVersion));
}

//_____________________________________________________________________________
IlcCDBEntry* IlcCDBStorage::Get(const IlcCDBPath& path,
	const IlcCDBRunRange& runRange, Int_t version,
	Int_t subVersion) {
// get an IlcCDBEntry object from the database

	return Get(IlcCDBId(path, runRange, version, subVersion));
}

//_____________________________________________________________________________
TList* IlcCDBStorage::GetAll(const IlcCDBId& query) {
// get multiple IlcCDBEntry objects from the database


	if (!query.IsValid()) {
                IlcError(Form("Invalid query: %s", query.ToString().Data()));
                return NULL;
        }

	if (query.IsAnyRange()) {
		IlcError(Form("Unspecified run or runrange: %s",
				query.ToString().Data())); 	
		return NULL;
	}	
        
	// This is needed otherwise TH1  objects (histos, TTree's) are lost when file is closed!
	Bool_t oldStatus = TH1::AddDirectoryStatus();
	TH1::AddDirectory(kFALSE);

	TList *result = GetEntries(query);

	if (oldStatus != kFALSE)
  		TH1::AddDirectory(kTRUE);

 	Int_t nEntries = result->GetEntries();

 	IlcInfo(Form("%d objects retrieved. Request was: %s",
		 	nEntries, query.ToString().Data()));
	for(int i=0; i<nEntries;i++){
		IlcCDBEntry *entry = (IlcCDBEntry*) result->At(i);
		IlcInfo(Form("%s",entry->GetId().ToString().Data()));
	}


	// if drain storage is set, drain entries into drain storage
	if((IlcCDBManager::Instance())->IsDrainSet()){
		for(int i = 0; i<result->GetEntries(); i++){
			IlcCDBEntry* entry = (IlcCDBEntry*) result->At(i);
			IlcCDBManager::Instance()->Drain(entry);
		}
	}
	

        return result;
}

//_____________________________________________________________________________
TList* IlcCDBStorage::GetAll(const IlcCDBPath& path, Int_t runNumber, 
	Int_t version, Int_t subVersion) {
// get multiple IlcCDBEntry objects from the database

	return GetAll(IlcCDBId(path, runNumber, runNumber, version, 	
			subVersion));
}

//_____________________________________________________________________________
TList* IlcCDBStorage::GetAll(const IlcCDBPath& path, 
	const IlcCDBRunRange& runRange, Int_t version, Int_t subVersion) {
// get multiple IlcCDBEntry objects from the database

	return GetAll(IlcCDBId(path, runRange, version, subVersion));
}

//_____________________________________________________________________________
IlcCDBId* IlcCDBStorage::GetId(const IlcCDBId& query) {
// get the Id of the valid object from the database (does not open the file)

	// check if query's path and runRange are valid
	// query is invalid also if version is not specified and subversion is!
	if (!query.IsValid()) {
		IlcError(Form("Invalid query: %s", query.ToString().Data()));
		return NULL;
	}

	// query is not specified if path contains wildcard or runrange = [-1,-1]
	if (!query.IsSpecified()) {
		IlcError(Form("Unspecified query: %s",
				query.ToString().Data()));
                return NULL;
	}

	IlcCDBId* id = GetEntryId(query);

	return id;
}

//_____________________________________________________________________________
IlcCDBId* IlcCDBStorage::GetId(const IlcCDBPath& path, Int_t runNumber,
	Int_t version, Int_t subVersion) {
// get the Id of the valid object from the database (does not open the file)

	return GetId(IlcCDBId(path, runNumber, runNumber, version, subVersion));
}

//_____________________________________________________________________________
IlcCDBId* IlcCDBStorage::GetId(const IlcCDBPath& path,
	const IlcCDBRunRange& runRange, Int_t version,
	Int_t subVersion) {
// get the Id of the valid object from the database (does not open the file)

	return GetId(IlcCDBId(path, runRange, version, subVersion));
}

//_____________________________________________________________________________
Bool_t IlcCDBStorage::Put(TObject* object, IlcCDBId& id, IlcCDBMetaData* metaData, const char* mirrors, IlcCDBManager::DataType type) {
// store an IlcCDBEntry object into the database
	
	if (object==0x0) {
		IlcError("Null Entry! No storage will be done!");
		return kFALSE;
	} 

	IlcCDBEntry anEntry(object, id, metaData);

	return Put(&anEntry, mirrors, type);
}

//_____________________________________________________________________________
Bool_t IlcCDBStorage::Put(IlcCDBEntry* entry, const char* mirrors, IlcCDBManager::DataType type) {
// store an IlcCDBEntry object into the database

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

	IlcCDBManager::DataType expectedType = GetDataType();

	if(expectedType != IlcCDBManager::kPrivate && type != expectedType) {
		IlcError(Form("It is forbidden to store %s data into a folder of type %s!",
			IlcCDBManager::GetDataTypeName(type),
			IlcCDBManager::GetDataTypeName(expectedType)));
			return 0;
	}

	
	TString strMirrors(mirrors);
	if(!strMirrors.IsNull() && !strMirrors.IsWhitespace())
	    return PutEntry(entry, mirrors);
	else
	    return PutEntry(entry);
}

//_____________________________________________________________________________
void IlcCDBStorage::QueryCDB(Int_t run, const char* pathFilter,
				Int_t version, IlcCDBMetaData* md){
// query CDB for files valid for given run, and fill list fValidFileIds
// Actual query is done in virtual function QueryValidFiles()

	fRun = run;

	fPathFilter = pathFilter;
	if(!fPathFilter.IsValid()) {
		IlcError(Form("Filter not valid: %s", pathFilter));
		fPathFilter = "*";
		return;
	}

	fVersion = version;

	IlcInfo(Form("Querying files valid for run %d and path \"%s\" into CDB storage  \"%s://%s\"",
				fRun, pathFilter, fType.Data(), fBaseFolder.Data()));

	// Clear fValidFileIds list (it cannot be filled twice!
	IlcDebug(2, "Clearing list of CDB Id's previously loaded");
	fValidFileIds.Clear();

	if(fMetaDataFilter) {delete fMetaDataFilter; fMetaDataFilter=0;}
	if(md) fMetaDataFilter = dynamic_cast<IlcCDBMetaData*> (md->Clone());

	QueryValidFiles();

	IlcInfo(Form("%d valid files found!", fValidFileIds.GetEntriesFast()));

}

//_____________________________________________________________________________
void IlcCDBStorage::PrintQueryCDB(){
// print parameters used to load list of CDB Id's (fRun, fPathFilter, fVersion)

	IlcCDBId paramId(fPathFilter, fRun, fRun, fVersion);
	IlcInfo(Form("**** QueryCDB Parameters **** \n\t<%s>\n",
				paramId.ToString().Data()));

	if(fMetaDataFilter) fMetaDataFilter->PrintMetaData();


	TString message = "**** Id's of valid objects found *****\n";
	TIter iter(&fValidFileIds);
	IlcCDBId* anId=0;

	// loop on the list of selection criteria
	while ((anId = dynamic_cast<IlcCDBId*>(iter.Next()))) {
		message += Form("\t%s\n", anId->ToString().Data());
	}
	message += Form("\n\tTotal: %d objects found\n", fValidFileIds.GetEntriesFast());
	IlcInfo(Form("%s", message.Data()));
}

//_____________________________________________________________________________
IlcCDBManager::DataType IlcCDBStorage::GetDataType() const {
// returns the type of the data that should be stored into this storage:
// kConditions: conditions data; kReference: reference data; kPrivate: private (user-defined) data type

	if(GetType() != "alien") return IlcCDBManager::kPrivate;

	TString condFolder = ((IlcCDBGridParam*) IlcCDBManager::Instance()->GetCondParam())->GetDBFolder();
	TString refFolder = ((IlcCDBGridParam*) IlcCDBManager::Instance()->GetRefParam())->GetDBFolder();

	if(GetBaseFolder().Contains(condFolder)) return IlcCDBManager::kCondition;
	if(GetBaseFolder().Contains(refFolder)) return IlcCDBManager::kReference;

	return IlcCDBManager::kPrivate;
}

//_____________________________________________________________________________
void IlcCDBStorage::SetMirrorSEs(const char* mirrors) {
// if the current storage is not of "alien" type, just issue a warning
// IlcCDBGrid implements its own SetMirrorSEs method

        TString storageType = GetType();
	if(storageType != "alien"){
	    IlcWarning(Form("The current storage is of type \"%s\" and cannot handle SEs. Setting skipped!",storageType.Data()));
	    return;
	}
	IlcError("We should never get here!! IlcCDBGrid must have masked this virtual method!");
	return;
}

//_____________________________________________________________________________
const char* IlcCDBStorage::GetMirrorSEs() const {
// if the current storage is not of "alien" type, just issue a warning
// IlcCDBGrid implements its own GetMirrorSEs method

        TString storageType = GetType();
	if(storageType != "alien"){
	    IlcWarning(Form("The current storage is of type \"%s\" and cannot handle SEs. Returning empty string!",storageType.Data()));
	    return "";
	}
	IlcError("We should never get here!! IlcCDBGrid must have masked this virtual method!");
	return "";
}

//_____________________________________________________________________________
void IlcCDBStorage::LoadTreeFromFile(IlcCDBEntry *entry) const {
// Checks whether entry contains a TTree and in case loads it into memory

	TObject *obj = (TObject*) entry->GetObject();
	if (!obj) {
	  IlcError("Cannot retrieve the object:");
	  entry->PrintMetaData();
	  return;
	}

	if (!strcmp(obj->ClassName(),TTree::Class_Name())) {

		IlcWarning("Entry contains a TTree! Loading baskets...");

		TTree* tree = dynamic_cast<TTree*> (obj);

		if(!tree) return;

		tree->LoadBaskets();
		tree->SetDirectory(0);
	}
	else if (!strcmp(obj->ClassName(),TNtuple::Class_Name())){

		IlcWarning("Entry contains a TNtuple! Loading baskets...");

		TNtuple* ntu = dynamic_cast<TNtuple*> (obj);

		if(!ntu) return;

		ntu->LoadBaskets();
		ntu->SetDirectory(0);
	}

	return;
}

// //_____________________________________________________________________________
// void IlcCDBStorage::SetTreeToFile(IlcCDBEntry *entry, TFile* file) const {
// // Checks whether entry contains a TTree and in case assigns it to memory
// 
// 	IlcCDBMetaData *md = dynamic_cast<IlcCDBMetaData*> (entry->GetMetaData());
// 	if(!md) return;
// 	TString objStr = md->GetObjectClassName();
// 	if(objStr != "TTree") return;
// 	IlcWarning("Entry contains a TTree! Setting file...");
// 
// 	TTree* tree = dynamic_cast<TTree*> (entry->GetObject());
// 
// 	if(!tree) return;
// 
// //	tree->SetDirectory(file);
// 	tree->SetDirectory(0);
// 
// 	return;
// }

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

/////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                             //
// IlcCDBLocal										       //
// access class to a DataBase in a local storage  			                       //
//                                                                                             //
/////////////////////////////////////////////////////////////////////////////////////////////////

#include <cstdlib>
#include <TSystem.h>
#include <TObjString.h>
#include <TRegexp.h>
#include <TFile.h>
#include <TKey.h>

#include "IlcCDBLocal.h"
#include "IlcCDBEntry.h"
#include "IlcLog.h"

ClassImp(IlcCDBLocal)

//_____________________________________________________________________________
IlcCDBLocal::IlcCDBLocal(const char* baseDir):
fBaseDirectory(baseDir) 
{
// constructor

  IlcDebug(1, Form("fBaseDirectory = %s",fBaseDirectory.Data()));

	// check baseDire: trying to cd to baseDir; if it does not exist, create it
	void* dir = gSystem->OpenDirectory(baseDir);
	if (dir == NULL) {
		if (gSystem->mkdir(baseDir, kTRUE)) {
			IlcError(Form("Can't open directory <%s>!", baseDir));
		}

	} else {
		IlcDebug(2,Form("Folder <%s> found",fBaseDirectory.Data()));
		gSystem->FreeDirectory(dir);
	}
	fType="local";
	fBaseFolder = fBaseDirectory;
}

//_____________________________________________________________________________
IlcCDBLocal::~IlcCDBLocal() {
// destructor

}


//_____________________________________________________________________________
Bool_t IlcCDBLocal::FilenameToId(const char* filename, IlcCDBRunRange& runRange,
	Int_t& version, Int_t& subVersion) {
// build IlcCDBId from filename numbers


        Ssiz_t mSize;

	// valid filename: Run#firstRun_#lastRun_v#version_s#subVersion.root
        TRegexp keyPattern("^Run[0-9]+_[0-9]+_v[0-9]+_s[0-9]+.root$");
        keyPattern.Index(filename, &mSize);
        if (!mSize) {
		IlcDebug(2, Form("Bad filename <%s>.", filename));
                return kFALSE;
        }

	TString idString(filename);
	idString.Resize(idString.Length() - sizeof(".root") + 1);

        TObjArray* strArray = (TObjArray*) idString.Tokenize("_");

	TString firstRunString(((TObjString*) strArray->At(0))->GetString());
	runRange.SetFirstRun(atoi(firstRunString.Data() + 3));
	runRange.SetLastRun(atoi(((TObjString*) strArray->At(1))->GetString()));
	
	TString verString(((TObjString*) strArray->At(2))->GetString());
	version = atoi(verString.Data() + 1);

	TString subVerString(((TObjString*) strArray->At(3))->GetString());
        subVersion = atoi(subVerString.Data() + 1);

        delete strArray;

        return kTRUE;
}


//_____________________________________________________________________________
Bool_t IlcCDBLocal::IdToFilename(const IlcCDBId& id, TString& filename) const {
// build file name from IlcCDBId data (run range, version, subVersion)

  IlcDebug(1, Form("fBaseDirectory = %s",fBaseDirectory.Data()));

	if (!id.GetIlcCDBRunRange().IsValid()) {
		IlcDebug(2,Form("Invalid run range <%d, %d>.", 
			id.GetFirstRun(), id.GetLastRun()));
		return kFALSE;
	}

	if (id.GetVersion() < 0) {
		IlcDebug(2,Form("Invalid version <%d>.", id.GetVersion()));
                return kFALSE;
	}

	if (id.GetSubVersion() < 0) {
		IlcDebug(2,Form("Invalid subversion <%d>.", id.GetSubVersion()));
		return kFALSE;
	}
 
	filename = Form("Run%d_%d_v%d_s%d.root", id.GetFirstRun(), id.GetLastRun(),
						 id.GetVersion(), id.GetSubVersion());

	filename.Prepend(fBaseDirectory +'/' + id.GetPath() + '/');

        return kTRUE;
}

//_____________________________________________________________________________
Bool_t IlcCDBLocal::PrepareId(IlcCDBId& id) {
// prepare id (version, subVersion) of the object that will be stored (called by PutEntry)

	TString dirName = Form("%s/%s", fBaseDirectory.Data(), id.GetPath().Data());

	// go to the path; if directory does not exist, create it
	void* dirPtr = gSystem->OpenDirectory(dirName);
	if (!dirPtr) {
		gSystem->mkdir(dirName, kTRUE);
                dirPtr = gSystem->OpenDirectory(dirName);

                if (!dirPtr) {
                        IlcError(Form("Can't create directory <%s>!", 
					dirName.Data()));
                        return kFALSE;
                }
	}

	const char* filename;
	IlcCDBRunRange aRunRange; // the runRange got from filename
 	IlcCDBRunRange lastRunRange(-1,-1); // highest runRange found
	Int_t aVersion, aSubVersion; // the version subVersion got from filename
	Int_t lastVersion = 0, lastSubVersion = -1; // highest version and subVersion found

	if (!id.HasVersion()) { // version not specified: look for highest version & subVersion
				
		while ((filename = gSystem->GetDirEntry(dirPtr))) { // loop on the files

			TString aString(filename);
			if (aString == "." || aString == "..") continue;
	
			if (!FilenameToId(filename, aRunRange, aVersion, 
				aSubVersion)) {
				IlcDebug(2,Form(
					"Bad filename <%s>! I'll skip it.", 
					filename));
				continue;
			}
			
			if (!aRunRange.Overlaps(id.GetIlcCDBRunRange())) continue;
			if(aVersion < lastVersion) continue;
			if(aVersion > lastVersion) lastSubVersion = -1;
			if(aSubVersion < lastSubVersion) continue;
			lastVersion = aVersion;
			lastSubVersion = aSubVersion;
			lastRunRange = aRunRange;
		}

		id.SetVersion(lastVersion);
		id.SetSubVersion(lastSubVersion + 1);

	} else { // version specified, look for highest subVersion only
		
		while ((filename = gSystem->GetDirEntry(dirPtr))) { // loop on the files
			
			TString aString(filename);
			if (aString == "." || aString == "..") {
				continue;
			}

			if (!FilenameToId(filename, aRunRange, aVersion, 
				aSubVersion)) {
				IlcDebug(2,Form(
					"Bad filename <%s>!I'll skip it.",
					filename));	
				continue;
			}

			if (aRunRange.Overlaps(id.GetIlcCDBRunRange()) 
				&& aVersion == id.GetVersion()
				&& aSubVersion > lastSubVersion) {
				lastSubVersion = aSubVersion;
				lastRunRange = aRunRange;
			}
	
		}
		
		id.SetSubVersion(lastSubVersion + 1);
	}

	gSystem->FreeDirectory(dirPtr);

	TString lastStorage = id.GetLastStorage();
	if(lastStorage.Contains(TString("grid"), TString::kIgnoreCase) &&
	   id.GetSubVersion() > 0 ){
		IlcError(Form("Grid to Local Storage error! local object with version v%d_s%d found:",id.GetVersion(), id.GetSubVersion()-1));
		IlcError(Form("This object has been already transferred from Grid (check v%d_s0)!",id.GetVersion()));
		return kFALSE;
	}

	if(lastStorage.Contains(TString("new"), TString::kIgnoreCase) &&
	   id.GetSubVersion() > 0 ){
		IlcDebug(2, Form("A NEW object is being stored with version v%d_s%d",
					id.GetVersion(),id.GetSubVersion()));
		IlcDebug(2, Form("and it will hide previously stored object with v%d_s%d!",
					id.GetVersion(),id.GetSubVersion()-1));
	}

 	if(!lastRunRange.IsAnyRange() && !(lastRunRange.IsEqual(& id.GetIlcCDBRunRange()))) 
    		IlcWarning(Form("Run range modified w.r.t. previous version (Run%d_%d_v%d_s%d)",
    		     	lastRunRange.GetFirstRun(), lastRunRange.GetLastRun(), 
			id.GetVersion(), id.GetSubVersion()-1));

	return kTRUE;
}

// //_____________________________________________________________________________
// Bool_t IlcCDBLocal::GetId(const IlcCDBId& query, IlcCDBId& result) {
// // look for filename matching query (called by GetEntry)
// 
// 	TString dirName = Form("%s/%s", fBaseDirectory.Data(), query.GetPath().Data());
// 
// 	void* dirPtr = gSystem->OpenDirectory(dirName);
// 	if (!dirPtr) {
//    	 	IlcDebug(2,Form("Directory <%s> not found", (query.GetPath()).Data()));
//    	 	IlcDebug(2,Form("in DB folder %s", fBaseDirectory.Data()));
// 		return kFALSE;
// 	}
// 
// 	const char* filename;
// 
// 	IlcCDBRunRange aRunRange; // the runRange got from filename
// 	Int_t aVersion, aSubVersion; // the version and subVersion got from filename
// 
// 	if (!query.HasVersion()) { // neither version and subversion specified -> look for highest version and subVersion
// 
// 		while ((filename = gSystem->GetDirEntry(dirPtr))) { // loop on files
// 
// 			TString aString(filename);
// 			if (aString == "." || aString == "..") continue;
// 
// 			if (!FilenameToId(filename, aRunRange, aVersion, aSubVersion)) continue;
//                         // aRunRange, aVersion, aSubVersion filled from filename
// 
// 			if (!aRunRange.Comprises(query.GetIlcCDBRunRange())) continue;
// 			// aRunRange contains requested run!
// 
// 			if (result.GetVersion() < aVersion) {
// 				result.SetVersion(aVersion);
// 				result.SetSubVersion(aSubVersion);
// 
// 				result.SetFirstRun(
// 					aRunRange.GetFirstRun());
// 				result.SetLastRun(
// 					aRunRange.GetLastRun());
// 
// 			} else if (result.GetVersion() == aVersion
// 				&& result.GetSubVersion()
// 					< aSubVersion) {
// 
// 				result.SetSubVersion(aSubVersion);
// 
//                         	result.SetFirstRun(
// 					aRunRange.GetFirstRun());
//                         	result.SetLastRun(
// 					aRunRange.GetLastRun());
// 			} else if (result.GetVersion() == aVersion
// 				&& result.GetSubVersion() == aSubVersion){
//               			IlcDebug(2,Form("More than one object valid for run %d, version %d_%d!",
// 		       			query.GetFirstRun(), aVersion, aSubVersion));
// 				gSystem->FreeDirectory(dirPtr);
// 	      			return kFALSE;
// 				}
// 		}
// 
// 	} else if (!query.HasSubVersion()) { // version specified but not subversion -> look for highest subVersion
// 
// 		result.SetVersion(query.GetVersion());
// 
// 		while ((filename = gSystem->GetDirEntry(dirPtr))) { // loop on files
// 
//                         TString aString(filename);
//                         if (aString == "." || aString == "..") continue;
// 
// 			if (!FilenameToId(filename, aRunRange, aVersion, aSubVersion)) continue;
//                         // aRunRange, aVersion, aSubVersion filled from filename
// 
//                         if (!aRunRange.Comprises(query.GetIlcCDBRunRange())) continue; 
// 			// aRunRange contains requested run!
// 
// 			if(query.GetVersion() != aVersion) continue;
// 			// aVersion is requested version!
// 
// 	 		if(result.GetSubVersion() == aSubVersion){
//               			IlcDebug(2,Form("More than one object valid for run %d, version %d_%d!",
// 		       			query.GetFirstRun(), aVersion, aSubVersion));
// 				gSystem->FreeDirectory(dirPtr);
// 	     			return kFALSE; 
// 	 		}
// 			if( result.GetSubVersion() < aSubVersion) {
// 
//                                 result.SetSubVersion(aSubVersion);
// 
//                                 result.SetFirstRun(
// 					aRunRange.GetFirstRun());
//                                 result.SetLastRun(
// 					aRunRange.GetLastRun());
// 			} 
//                 }
// 
// 	} else { // both version and subversion specified
// 
// 		while ((filename = gSystem->GetDirEntry(dirPtr))) { // loop on files
// 
//                         TString aString(filename);
//                         if (aString == "." || aString == "..") continue;
// 
//                         if (!FilenameToId(filename, aRunRange, aVersion, aSubVersion)) continue;
//                         // aRunRange, aVersion, aSubVersion filled from filename
// 
// 			if (!aRunRange.Comprises(query.GetIlcCDBRunRange())) continue;
// 			// aRunRange contains requested run!
// 
// 			if(query.GetVersion() != aVersion || query.GetSubVersion() != aSubVersion) continue;
// 			// aVersion and aSubVersion are requested version and subVersion!
// 
// 			if(result.GetVersion() == aVersion && result.GetSubVersion() == aSubVersion){
//               			IlcDebug(2,Form("More than one object valid for run %d, version %d_%d!",
// 		       			query.GetFirstRun(), aVersion, aSubVersion));
// 				gSystem->FreeDirectory(dirPtr);
// 	     			return kFALSE;
// 			}
// 			result.SetVersion(aVersion);
// 		        result.SetSubVersion(aSubVersion);
// 			result.SetFirstRun(aRunRange.GetFirstRun());
// 			result.SetLastRun(aRunRange.GetLastRun());
// 
// 		}
// 	}
// 
// 	gSystem->FreeDirectory(dirPtr);
// 
// 	return kTRUE;
// }

//_____________________________________________________________________________
IlcCDBId* IlcCDBLocal::GetId(const IlcCDBId& query) {
// look for filename matching query (called by GetEntryId)

	TString dirName = Form("%s/%s", fBaseDirectory.Data(), query.GetPath().Data());

	void* dirPtr = gSystem->OpenDirectory(dirName);
	if (!dirPtr) {
   	 	IlcDebug(2,Form("Directory <%s> not found", (query.GetPath()).Data()));
   	 	IlcDebug(2,Form("in DB folder %s", fBaseDirectory.Data()));
		return NULL;
	}

	const char* filename;
	IlcCDBId *result = new IlcCDBId();
	result->SetPath(query.GetPath());

	IlcCDBRunRange aRunRange; // the runRange got from filename
	Int_t aVersion, aSubVersion; // the version and subVersion got from filename

	if (!query.HasVersion()) { // neither version and subversion specified -> look for highest version and subVersion

		while ((filename = gSystem->GetDirEntry(dirPtr))) { // loop on files

			TString aString(filename);
			if (aString == "." || aString == "..") continue;

			if (!FilenameToId(filename, aRunRange, aVersion, aSubVersion)) continue;
                        // aRunRange, aVersion, aSubVersion filled from filename

			if (!aRunRange.Comprises(query.GetIlcCDBRunRange())) continue;
			// aRunRange contains requested run!

			IlcDebug(1,Form("Filename %s matches\n",filename));

			if (result->GetVersion() < aVersion) {
				result->SetVersion(aVersion);
				result->SetSubVersion(aSubVersion);

				result->SetFirstRun(
					aRunRange.GetFirstRun());
				result->SetLastRun(
					aRunRange.GetLastRun());

			} else if (result->GetVersion() == aVersion
				&& result->GetSubVersion()
					< aSubVersion) {

				result->SetSubVersion(aSubVersion);

                        	result->SetFirstRun(
					aRunRange.GetFirstRun());
                        	result->SetLastRun(
					aRunRange.GetLastRun());
			} else if (result->GetVersion() == aVersion
				&& result->GetSubVersion() == aSubVersion){
              			IlcError(Form("More than one object valid for run %d, version %d_%d!",
		       			query.GetFirstRun(), aVersion, aSubVersion));
				gSystem->FreeDirectory(dirPtr);
				delete result;
	      			return NULL;
				}
		}

	} else if (!query.HasSubVersion()) { // version specified but not subversion -> look for highest subVersion

		result->SetVersion(query.GetVersion());

		while ((filename = gSystem->GetDirEntry(dirPtr))) { // loop on files

                        TString aString(filename);
                        if (aString == "." || aString == "..") continue;

			if (!FilenameToId(filename, aRunRange, aVersion, aSubVersion)) continue;
                        // aRunRange, aVersion, aSubVersion filled from filename

                        if (!aRunRange.Comprises(query.GetIlcCDBRunRange())) continue; 
			// aRunRange contains requested run!

			if(query.GetVersion() != aVersion) continue;
			// aVersion is requested version!

	 		if(result->GetSubVersion() == aSubVersion){
              			IlcError(Form("More than one object valid for run %d, version %d_%d!",
		       			query.GetFirstRun(), aVersion, aSubVersion));
				gSystem->FreeDirectory(dirPtr);
				delete result;
	     			return NULL;
	 		}
			if( result->GetSubVersion() < aSubVersion) {

                                result->SetSubVersion(aSubVersion);

                                result->SetFirstRun(
					aRunRange.GetFirstRun());
                                result->SetLastRun(
					aRunRange.GetLastRun());
			} 
                }

	} else { // both version and subversion specified

		//IlcCDBId dataId(queryId.GetIlcCDBPath(), -1, -1, -1, -1);
        //Bool_t result;
	while ((filename = gSystem->GetDirEntry(dirPtr))) { // loop on files

                        TString aString(filename);
                        if (aString == "." || aString == "..") continue;

                        if (!FilenameToId(filename, aRunRange, aVersion, aSubVersion)) continue;
                        // aRunRange, aVersion, aSubVersion filled from filename

			if (!aRunRange.Comprises(query.GetIlcCDBRunRange())) continue;
			// aRunRange contains requested run!

			if(query.GetVersion() != aVersion || query.GetSubVersion() != aSubVersion) continue;
			// aVersion and aSubVersion are requested version and subVersion!

			if(result->GetVersion() == aVersion && result->GetSubVersion() == aSubVersion){
              			IlcError(Form("More than one object valid for run %d, version %d_%d!",
		       			query.GetFirstRun(), aVersion, aSubVersion));
				gSystem->FreeDirectory(dirPtr);
				delete result;
	     			return NULL;
			}
			result->SetVersion(aVersion);
		        result->SetSubVersion(aSubVersion);
			result->SetFirstRun(aRunRange.GetFirstRun());
			result->SetLastRun(aRunRange.GetLastRun());
			
		}
	}

	gSystem->FreeDirectory(dirPtr);

	return result;
}

//_____________________________________________________________________________
IlcCDBEntry* IlcCDBLocal::GetEntry(const IlcCDBId& queryId) {
// get IlcCDBEntry from the database

	IlcCDBId* dataId = GetEntryId(queryId);

	if (!dataId || !dataId->IsSpecified()) return NULL;

	TString filename;
	if (!IdToFilename(*dataId, filename)) {

		IlcDebug(2,Form("Bad data ID encountered! Subnormal error!"));
		delete dataId;
		return NULL;
	}

	TFile file(filename, "READ"); // open file
	if (!file.IsOpen()) {
		IlcDebug(2,Form("Can't open file <%s>!", filename.Data()));
		delete dataId;
		return NULL;
	}

	// get the only IlcCDBEntry object from the file
	// the object in the file is an IlcCDBEntry entry named "IlcCDBEntry"

	IlcCDBEntry* anEntry = dynamic_cast<IlcCDBEntry*> (file.Get("IlcCDBEntry"));
	if (!anEntry) {
		IlcDebug(2,Form("Bad storage data: No IlcCDBEntry in file!"));
		file.Close();
		delete dataId;
		return NULL;
	}

 	IlcCDBId& entryId = anEntry->GetId();

	// The object's Id are not reset during storage
	// If object's Id runRange or version do not match with filename,
	// it means that someone renamed file by hand. In this case a warning msg is issued.

	anEntry-> SetLastStorage("local");

 	if(!entryId.IsEqual(dataId)){
		IlcWarning(Form("Mismatch between file name and object's Id!"));
		IlcWarning(Form("File name: %s", dataId->ToString().Data()));
		IlcWarning(Form("Object's Id: %s", entryId.ToString().Data()));
        }

	// Check whether entry contains a TTree. In case load the tree in memory!
	LoadTreeFromFile(anEntry);

	// close file, return retieved entry
	file.Close();
	delete dataId;
	return anEntry;
}

//_____________________________________________________________________________
IlcCDBId* IlcCDBLocal::GetEntryId(const IlcCDBId& queryId) {
// get IlcCDBId from the database

	IlcCDBId* dataId = 0;

	// look for a filename matching query requests (path, runRange, version, subVersion)
	if (!queryId.HasVersion()) {
		// if version is not specified, first check the selection criteria list
		IlcCDBId selectedId(queryId);
		GetSelection(&selectedId);
		dataId = GetId(selectedId);
	} else {
		dataId = GetId(queryId);
	}

	if (dataId && !dataId->IsSpecified()) {
		delete dataId;
		return NULL;
	}

	return dataId;
}

//_____________________________________________________________________________
void IlcCDBLocal::GetEntriesForLevel0(const char* level0,
	const IlcCDBId& queryId, TList* result) {
// multiple request (IlcCDBStorage::GetAll)

	TString level0Dir = Form("%s/%s", fBaseDirectory.Data(), level0);

	void* level0DirPtr = gSystem->OpenDirectory(level0Dir);
	if (!level0DirPtr) {
		IlcDebug(2,Form("Can't open level0 directory <%s>!",
			level0Dir.Data()));
                return;
	}

	const char* level1;
	Long_t flag=0;
	while ((level1 = gSystem->GetDirEntry(level0DirPtr))) {

		TString level1Str(level1);
		if (level1Str == "." || level1Str == "..") {
			continue;
		}
		
		TString fullPath = Form("%s/%s",level0Dir.Data(), level1); 

		Int_t res=gSystem->GetPathInfo(fullPath.Data(), 0, (Long64_t*) 0, &flag, 0);
		
		if(res){
			IlcDebug(2, Form("Error reading entry %s !",level1Str.Data()));
			continue;
		}
		if(!(flag&2)) continue; // bit 1 of flag = directory!
		
                if (queryId.GetIlcCDBPath().Level1Comprises(level1)) {
			GetEntriesForLevel1(level0, level1, queryId, result);
        	}
	}

	gSystem->FreeDirectory(level0DirPtr);
}

//_____________________________________________________________________________
void IlcCDBLocal::GetEntriesForLevel1(const char* level0, const char* level1,
	const IlcCDBId& queryId, TList* result) {
// multiple request (IlcCDBStorage::GetAll)

	TString level1Dir = Form("%s/%s/%s", fBaseDirectory.Data(), level0,level1);

	void* level1DirPtr = gSystem->OpenDirectory(level1Dir);
	if (!level1DirPtr) {
		IlcDebug(2,Form("Can't open level1 directory <%s>!",
			level1Dir.Data()));
                return;
	}

	const char* level2;
	Long_t flag=0;
	while ((level2 = gSystem->GetDirEntry(level1DirPtr))) {

		TString level2Str(level2);
		if (level2Str == "." || level2Str == "..") {
			continue;
		}

		TString fullPath = Form("%s/%s",level1Dir.Data(), level2); 

		Int_t res=gSystem->GetPathInfo(fullPath.Data(), 0, (Long64_t*) 0, &flag, 0);
		
		if(res){
			IlcDebug(2, Form("Error reading entry %s !",level2Str.Data()));
			continue;
		}
		if(!(flag&2)) continue; // bit 1 of flag = directory!
		
		if (queryId.GetIlcCDBPath().Level2Comprises(level2)) {

			IlcCDBPath entryPath(level0, level1, level2);
        		IlcCDBId entryId(entryPath, queryId.GetIlcCDBRunRange(),
		                queryId.GetVersion(), queryId.GetSubVersion());

		        IlcCDBEntry* anEntry = GetEntry(entryId);
	        	if (anEntry) {
        		        result->Add(anEntry);
		        }
		}
	}

	gSystem->FreeDirectory(level1DirPtr);
}

//_____________________________________________________________________________
TList* IlcCDBLocal::GetEntries(const IlcCDBId& queryId) {
// multiple request (IlcCDBStorage::GetAll)
	
	void* storageDirPtr = gSystem->OpenDirectory(fBaseDirectory);
	if (!storageDirPtr) {
		IlcDebug(2,Form("Can't open storage directory <%s>",
			fBaseDirectory.Data()));
		return NULL;
	}

	TList* result = new TList();
	result->SetOwner();

	const char* level0;
	Long_t flag=0;
	while ((level0 = gSystem->GetDirEntry(storageDirPtr))) {

		TString level0Str(level0);
		if (level0Str == "." || level0Str == "..") {
			continue;
		}
		
		TString fullPath = Form("%s/%s",fBaseDirectory.Data(), level0); 

		Int_t res=gSystem->GetPathInfo(fullPath.Data(), 0, (Long64_t*) 0, &flag, 0);
		
		if(res){
			IlcDebug(2, Form("Error reading entry %s !",level0Str.Data()));
			continue;
		}
		
		if(!(flag&2)) continue; // bit 1 of flag = directory!				

		if (queryId.GetIlcCDBPath().Level0Comprises(level0)) {
			GetEntriesForLevel0(level0, queryId, result);
		}
	}

	gSystem->FreeDirectory(storageDirPtr);

	return result;	
}

//_____________________________________________________________________________
Bool_t IlcCDBLocal::PutEntry(IlcCDBEntry* entry, const char* mirrors) {
// put an IlcCDBEntry object into the database

	IlcCDBId& id = entry->GetId();

	// set version and subVersion for the entry to be stored
	if (!PrepareId(id)) return kFALSE;

	
	// build filename from entry's id
	TString filename="";
	if (!IdToFilename(id, filename)) {

		IlcDebug(2,Form("Bad ID encountered! Subnormal error!"));
		return kFALSE;
	}

	TString mirrorsString(mirrors);
	if(!mirrorsString.IsNull())
		IlcWarning("IlcCDBLocal storage cannot take mirror SEs into account. They will be ignored.");

	// open file
	TFile file(filename, "CREATE");
	if (!file.IsOpen()) {
		IlcError(Form("Can't open file <%s>!", filename.Data()));
		return kFALSE;
	}
	
	//SetTreeToFile(entry, &file);

	entry->SetVersion(id.GetVersion());
	entry->SetSubVersion(id.GetSubVersion());

	// write object (key name: "IlcCDBEntry")
	Bool_t result = file.WriteTObject(entry, "IlcCDBEntry");
	if (!result) IlcDebug(2,Form("Can't write entry to file: %s", filename.Data()));

	file.Close();
        if(result) {
		if(!(id.GetPath().Contains("SHUTTLE/STATUS")))
			IlcInfo(Form("CDB object stored into file %s",filename.Data()));
	}

	return result;
}

//_____________________________________________________________________________
TList* IlcCDBLocal::GetIdListFromFile(const char* fileName){

	TString fullFileName(fileName);
	fullFileName.Prepend(fBaseDirectory+'/');
	TFile *file = TFile::Open(fullFileName);
	if (!file) {
		IlcError(Form("Can't open selection file <%s>!", fullFileName.Data()));
		return NULL;
	}
	file->cd();

	TList *list = new TList();
	list->SetOwner();
	int i=0;
	TString keycycle;
	
	IlcCDBId *id;
	while(1){
		i++;
		keycycle = "IlcCDBId;";
		keycycle+=i;
		
		id = (IlcCDBId*) file->Get(keycycle);
		if(!id) break;
		list->AddFirst(id);
	}
	file->Close(); delete file; file=0;	
	return list;
}

//_____________________________________________________________________________
Bool_t IlcCDBLocal::Contains(const char* path) const{
// check for path in storage's fBaseDirectory

	TString dirName = Form("%s/%s", fBaseDirectory.Data(), path);
	Bool_t result=kFALSE;

	void* dirPtr = gSystem->OpenDirectory(dirName); 
	if (dirPtr) result=kTRUE;
	gSystem->FreeDirectory(dirPtr);

	return result;
}

//_____________________________________________________________________________
void IlcCDBLocal::QueryValidFiles()
{
// Query the CDB for files valid for IlcCDBStorage::fRun
// fills list fValidFileIds with IlcCDBId objects created from file name

	if(fVersion != -1) IlcWarning ("Version parameter is not used by local storage query!");
	if(fMetaDataFilter) {
		IlcWarning ("CDB meta data parameters are not used by local storage query!");
		delete fMetaDataFilter; fMetaDataFilter=0;
	}

	void* storageDirPtr = gSystem->OpenDirectory(fBaseDirectory);

	const char* level0;
	while ((level0 = gSystem->GetDirEntry(storageDirPtr))) {

		TString level0Str(level0);
		if (level0Str == "." || level0Str == "..") {
			continue;
		}

		if (fPathFilter.Level0Comprises(level0)) {
			TString level0Dir = Form("%s/%s",fBaseDirectory.Data(),level0);
			void* level0DirPtr = gSystem->OpenDirectory(level0Dir);
			const char* level1;
			while ((level1 = gSystem->GetDirEntry(level0DirPtr))) {

				TString level1Str(level1);
				if (level1Str == "." || level1Str == "..") {
					continue;
				}

                		if (fPathFilter.Level1Comprises(level1)) {
					TString level1Dir = Form("%s/%s/%s",
							fBaseDirectory.Data(),level0,level1);

					void* level1DirPtr = gSystem->OpenDirectory(level1Dir);
					const char* level2;
					while ((level2 = gSystem->GetDirEntry(level1DirPtr))) {

						TString level2Str(level2);
						if (level2Str == "." || level2Str == "..") {
							continue;
						}

						if (fPathFilter.Level2Comprises(level2)) {
							TString dirName = Form("%s/%s/%s/%s",
									fBaseDirectory.Data(),level0,level1,level2);

							void* dirPtr = gSystem->OpenDirectory(dirName);

							const char* filename;

							IlcCDBRunRange aRunRange; // the runRange got from filename
							Int_t aVersion, aSubVersion; // the version and subVersion got from filename

							while ((filename = gSystem->GetDirEntry(dirPtr))) { // loop on files

								TString aString(filename);
								if (aString == "." || aString == "..") continue;

								if (!FilenameToId(filename, aRunRange, aVersion, aSubVersion)) continue;
								IlcCDBRunRange runrg(fRun,fRun);
								if (!aRunRange.Comprises(runrg)) continue;
								// aRunRange contains requested run!
								IlcCDBPath validPath(level0,level1,level2);
								IlcCDBId validId(validPath,aRunRange,aVersion,aSubVersion);
      		        					fValidFileIds.AddLast(validId.Clone());
		        				}
							gSystem->FreeDirectory(dirPtr);
						}
					}
					gSystem->FreeDirectory(level1DirPtr);
				}
			}
			gSystem->FreeDirectory(level0DirPtr);
		}
	}
	gSystem->FreeDirectory(storageDirPtr);

}

//_____________________________________________________________________________
Int_t IlcCDBLocal::GetLatestVersion(const char* path, Int_t run){
// get last version found in the database valid for run and path

	IlcCDBPath aCDBPath(path);
	if(!aCDBPath.IsValid() || aCDBPath.IsWildcard()) {
		IlcError(Form("Invalid path in request: %s", path));
		return -1;
	}

	IlcCDBId query(path, run, run, -1, -1);
	IlcCDBId* dataId = GetId(query);

	if(!dataId) return -1;

	Int_t version = dataId->GetVersion();
	delete dataId;

	return version;

}

//_____________________________________________________________________________
Int_t IlcCDBLocal::GetLatestSubVersion(const char* path, Int_t run, Int_t version){
// get last version found in the database valid for run and path

	IlcCDBPath aCDBPath(path);
	if(!aCDBPath.IsValid() || aCDBPath.IsWildcard()) {
		IlcError(Form("Invalid path in request: %s", path));
		return -1;
	}

	IlcCDBId query(path, run, run, version, -1);
	IlcCDBId *dataId = GetId(query);

	if(!dataId) return -1;

	Int_t subVersion = dataId->GetSubVersion();

	delete dataId;
	return subVersion;

}

/////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                             //
// IlcCDBLocal factory  			                                               //
//                                                                                             //
/////////////////////////////////////////////////////////////////////////////////////////////////

ClassImp(IlcCDBLocalFactory)

//_____________________________________________________________________________
Bool_t IlcCDBLocalFactory::Validate(const char* dbString) {
// check if the string is valid local URI

        TRegexp dbPatternLocal("^local://.+$");

        return (TString(dbString).Contains(dbPatternLocal) || TString(dbString).BeginsWith("snapshot://folder="));
}

//_____________________________________________________________________________
IlcCDBParam* IlcCDBLocalFactory::CreateParameter(const char* dbString) {
// create IlcCDBLocalParam class from the URI string

	if (!Validate(dbString)) {
		return NULL;
	}

	TString checkSS(dbString);
	if(checkSS.BeginsWith("snapshot://"))
	{
	    TString snapshotPath("OCDB");
	    snapshotPath.Prepend(TString(gSystem->WorkingDirectory()) + '/');
	    checkSS.Remove(0,checkSS.First(':')+3);
	    return new IlcCDBLocalParam(snapshotPath,checkSS);
	}
		
	// if the string argument is not a snapshot URI, than it is a plain local URI
	TString pathname(dbString + sizeof("local://") - 1);
	
	if(gSystem->ExpandPathName(pathname))
	    return NULL;

	if (pathname[0] != '/') {
		pathname.Prepend(TString(gSystem->WorkingDirectory()) + '/');
	}
	//pathname.Prepend("local://");

	return new IlcCDBLocalParam(pathname);
}

//_____________________________________________________________________________
IlcCDBStorage* IlcCDBLocalFactory::Create(const IlcCDBParam* param) {
// create IlcCDBLocal storage instance from parameters
	
	if (IlcCDBLocalParam::Class() == param->IsA()) {
		
		const IlcCDBLocalParam* localParam = 
			(const IlcCDBLocalParam*) param;
		
		return new IlcCDBLocal(localParam->GetPath());
	}

	return NULL;
}
//_____________________________________________________________________________
void IlcCDBLocal::SetRetry(Int_t /* nretry */, Int_t /* initsec */) {

	// Function to set the exponential retry for putting entries in the OCDB

	IlcInfo("This function sets the exponential retry for putting entries in the OCDB - to be used ONLY for IlcCDBGrid --> returning without doing anything");
	return;
} 



/////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                             //
// IlcCDBLocal Parameter class  			                                       //                                          //
//                                                                                             //
/////////////////////////////////////////////////////////////////////////////////////////////////

ClassImp(IlcCDBLocalParam)

//_____________________________________________________________________________
IlcCDBLocalParam::IlcCDBLocalParam():
 IlcCDBParam(),
 fDBPath()
 {
// default constructor

}

//_____________________________________________________________________________
IlcCDBLocalParam::IlcCDBLocalParam(const char* dbPath):
 IlcCDBParam(),
 fDBPath(dbPath)
{
// constructor

	SetType("local");
	SetURI(TString("local://") + dbPath);
}

//_____________________________________________________________________________
IlcCDBLocalParam::IlcCDBLocalParam(const char* dbPath, const char* uri):
 IlcCDBParam(),
 fDBPath(dbPath)
{
// constructor

	SetType("local");
	SetURI(TString("alien://") + uri);
}

//_____________________________________________________________________________
IlcCDBLocalParam::~IlcCDBLocalParam() {
// destructor

}

//_____________________________________________________________________________
IlcCDBParam* IlcCDBLocalParam::CloneParam() const {
// clone parameter

        return new IlcCDBLocalParam(fDBPath);
}

//_____________________________________________________________________________
ULong_t IlcCDBLocalParam::Hash() const {
// return Hash function

       return fDBPath.Hash();
}

//_____________________________________________________________________________
Bool_t IlcCDBLocalParam::IsEqual(const TObject* obj) const {
// check if this object is equal to IlcCDBParam obj

        if (this == obj) {
                return kTRUE;
        }

        if (IlcCDBLocalParam::Class() != obj->IsA()) {
                return kFALSE;
        }

        IlcCDBLocalParam* other = (IlcCDBLocalParam*) obj;

        return fDBPath == other->fDBPath;
}


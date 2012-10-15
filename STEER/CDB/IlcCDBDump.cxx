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

/////////////////////////////////////////////////////////////////////
//                                                                 //
//  class IlcCDBDump						   //
//  access class to a DataBase in a dump storage (single file)     //
//                                                                 //
/////////////////////////////////////////////////////////////////////

#include <cstdlib>
#include <TSystem.h>
#include <TKey.h>
#include <TFile.h>
#include <TRegexp.h>
#include <TObjString.h>
#include <TList.h>

#include "IlcCDBDump.h"
#include "IlcCDBEntry.h"
#include "IlcLog.h"

ClassImp(IlcCDBDump)

//_____________________________________________________________________________
IlcCDBDump::IlcCDBDump(const char* dbFile, Bool_t readOnly):
fFile(NULL), fReadOnly(readOnly) {
// constructor

	// opening file
	fFile = TFile::Open(dbFile, fReadOnly ? "READ" : "UPDATE");	
	if (!fFile) {
		IlcError(Form("Can't open file <%s>!" , dbFile));
	} else {
		IlcDebug(2,Form("File <%s> opened",dbFile));
		if(fReadOnly) IlcDebug(2,Form("in read-only mode"));
	}

	fType="dump";
	fBaseFolder = dbFile;
}

//_____________________________________________________________________________
IlcCDBDump::~IlcCDBDump() {
// destructor

	if (fFile) {
		fFile->Close();
		delete fFile;
	}
}


//_____________________________________________________________________________
Bool_t IlcCDBDump::KeyNameToId(const char* keyname, IlcCDBRunRange& runRange,
	Int_t& version, Int_t& subVersion) {
// build IlcCDBId from keyname numbers

        Ssiz_t mSize;

	// valid keyname: Run#firstRun_#lastRun_v#version_s#subVersion.root
        TRegexp keyPattern("^Run[0-9]+_[0-9]+_v[0-9]+_s[0-9]+$");
        keyPattern.Index(keyname, &mSize);
        if (!mSize) {
                IlcDebug(2,Form("Bad keyname <%s>.", keyname));
                return kFALSE;
        }

        TObjArray* strArray = (TObjArray*) TString(keyname).Tokenize("_");

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
Bool_t IlcCDBDump::IdToKeyName(const IlcCDBRunRange& runRange, Int_t version,
        Int_t subVersion, TString& keyname) {
// build key name from IlcCDBId data (run range, version, subVersion)

        if (!runRange.IsValid()) {
                IlcDebug(2,Form("Invalid run range <%d, %d>.",
                        runRange.GetFirstRun(), runRange.GetLastRun()));
                return kFALSE;
        }

        if (version < 0) {
                IlcDebug(2,Form("Invalid version <%d>.", version));
                return kFALSE;
        }

	if (subVersion < 0) {
		IlcDebug(2,Form("Invalid subversion <%d>.", subVersion));
		return kFALSE;
	}
    
        keyname += "Run";
	keyname += runRange.GetFirstRun();
        keyname += "_";
        keyname += runRange.GetLastRun();
        keyname += "_v";
        keyname += version;
	keyname += "_s";
	keyname += subVersion;

        return kTRUE;
}

//_____________________________________________________________________________
Bool_t IlcCDBDump::MkDir(const TString& path) {
// descend into TDirectory, making TDirectories if they don't exist 
	TObjArray* strArray = (TObjArray*) path.Tokenize("/");
	
	TIter iter(strArray);
	TObjString* str;

	while ((str = (TObjString*) iter.Next())) {
		
		TString dirName(str->GetString());
		if (!dirName.Length()) {
			continue;
		}

		if (gDirectory->cd(dirName)) {
			continue;
		}

		TDirectory* aDir = gDirectory->mkdir(dirName, "");
		if (!aDir) {
			IlcError(Form("Can't create directory <%s>!", 
					dirName.Data()));
			delete strArray;

			return kFALSE;
		}

		aDir->cd();
	}	
	
	delete strArray;

	return kTRUE;
}

//_____________________________________________________________________________
Bool_t IlcCDBDump::PrepareId(IlcCDBId& id) {
// prepare id (version, subVersion) of the object that will be stored (called by PutEntry)

	IlcCDBRunRange aRunRange; // the runRange got from filename 
 	IlcCDBRunRange lastRunRange(-1,-1); // highest runRange found
	Int_t aVersion, aSubVersion; // the version subVersion got from filename
	Int_t lastVersion = 0, lastSubVersion = -1; // highest version and subVersion found

	
	TIter iter(gDirectory->GetListOfKeys());
	TKey* key;

	if (!id.HasVersion()) {	// version not specified: look for highest version & subVersion	
				
		while ((key = (TKey*) iter.Next())) { // loop on keys

			const char* keyName = key->GetName();
	
			if (!KeyNameToId(keyName, aRunRange, aVersion, 
			   aSubVersion)) {
				IlcDebug(2,Form(
					"Bad keyname <%s>!I'll skip it.", keyName));
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
		
		while ((key = (TKey*) iter.Next())) { // loop on the keys

			const char* keyName = key->GetName();
	
			if (!KeyNameToId(keyName, aRunRange, aVersion, 
			   aSubVersion)) {
				IlcDebug(2,Form(
					"Bad keyname <%s>!I'll skip it.", keyName));
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

	TString lastStorage = id.GetLastStorage();
	if(lastStorage.Contains(TString("grid"), TString::kIgnoreCase) &&
	   id.GetSubVersion() > 0 ){
		IlcError(Form("Grid to Dump Storage error! local object with version v%d_s%d found:",id.GetVersion(), id.GetSubVersion()-1));
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
// Bool_t IlcCDBDump::GetId(const IlcCDBId& query, IlcCDBId& result) {
// // look for filename matching query (called by GetEntry)
// 
// 
//         IlcCDBRunRange aRunRange; // the runRange got from filename
//         Int_t aVersion, aSubVersion; // the version and subVersion got from filename
// 
// 	TIter iter(gDirectory->GetListOfKeys());
// 	TKey* key;
// 
// 	if (!query.HasVersion()) { // neither version and subversion specified -> look for highest version and subVersion
// 
//                 while ((key = (TKey*) iter.Next())) { // loop on the keys
// 
// 			if (!KeyNameToId(key->GetName(), aRunRange, aVersion, aSubVersion)) continue;
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
// 	      			result.SetRunRange(-1,-1); result.SetVersion(-1); result.SetSubVersion(-1);
// 	      			return kFALSE;
// 				}
// 		}
// 
// 	} else if (!query.HasSubVersion()) { // version specified but not subversion -> look for highest subVersion
// 
// 		result.SetVersion(query.GetVersion());
// 
//                 while ((key = (TKey*) iter.Next())) { // loop on the keys
// 
// 			if (!KeyNameToId(key->GetName(), aRunRange, aVersion, aSubVersion)) continue;
//                         // aRunRange, aVersion, aSubVersion filled from filename
// 
//                        if (!aRunRange.Comprises(query.GetIlcCDBRunRange())) continue;
// 			// aRunRange contains requested run!
// 			
// 			if(query.GetVersion() != aVersion) continue;
// 			// aVersion is requested version!
// 
// 	 		if(result.GetSubVersion() == aSubVersion){
//               			IlcDebug(2,Form("More than one object valid for run %d, version %d_%d!",
// 		       			query.GetFirstRun(), aVersion, aSubVersion));
// 	     			result.SetRunRange(-1,-1); result.SetVersion(-1); result.SetSubVersion(-1);
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
//                 while ((key = (TKey*) iter.Next())) { // loop on the keys
// 
// 			if (!KeyNameToId(key->GetName(), aRunRange, aVersion, aSubVersion)) continue;
//                         // aRunRange, aVersion, aSubVersion filled from filename
// 
// 			if (!aRunRange.Comprises(query.GetIlcCDBRunRange())) continue;
//  			// aRunRange contains requested run!
// 
// 			if(query.GetVersion() != aVersion || query.GetSubVersion() != aSubVersion) continue;
// 			// aVersion and aSubVersion are requested version and subVersion!
// 
// 			if(result.GetVersion() == aVersion && result.GetSubVersion() == aSubVersion){
//               			IlcDebug(2,Form("More than one object valid for run %d, version %d_%d!",
// 		       			query.GetFirstRun(), aVersion, aSubVersion));
// 	     			result.SetRunRange(-1,-1); result.SetVersion(-1); result.SetSubVersion(-1);
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
// 	return kTRUE;
// }

//_____________________________________________________________________________
IlcCDBId* IlcCDBDump::GetId(const IlcCDBId& query) {
// look for filename matching query (called by GetEntry)


        IlcCDBRunRange aRunRange; // the runRange got from filename
        Int_t aVersion, aSubVersion; // the version and subVersion got from filename

	TIter iter(gDirectory->GetListOfKeys());
	TKey* key;

	IlcCDBId* result = new IlcCDBId();
	result->SetPath(query.GetPath());

	if (!query.HasVersion()) { // neither version and subversion specified -> look for highest version and subVersion

                while ((key = (TKey*) iter.Next())) { // loop on the keys
			
			if (!KeyNameToId(key->GetName(), aRunRange, aVersion, aSubVersion)) continue;
                        // aRunRange, aVersion, aSubVersion filled from filename

			if (!aRunRange.Comprises(query.GetIlcCDBRunRange())) continue;
			// aRunRange contains requested run!

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
				delete result;
				return NULL;
				}
		}

	} else if (!query.HasSubVersion()) { // version specified but not subversion -> look for highest subVersion

		result->SetVersion(query.GetVersion());

                while ((key = (TKey*) iter.Next())) { // loop on the keys

			if (!KeyNameToId(key->GetName(), aRunRange, aVersion, aSubVersion)) continue;
                        // aRunRange, aVersion, aSubVersion filled from filename

                       if (!aRunRange.Comprises(query.GetIlcCDBRunRange())) continue; 
			// aRunRange contains requested run!
			
			if(query.GetVersion() != aVersion) continue;
			// aVersion is requested version!

	 		if(result->GetSubVersion() == aSubVersion){
              			IlcError(Form("More than one object valid for run %d, version %d_%d!",
		       			query.GetFirstRun(), aVersion, aSubVersion));
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

                while ((key = (TKey*) iter.Next())) { // loop on the keys
			
			if (!KeyNameToId(key->GetName(), aRunRange, aVersion, aSubVersion)) continue;
                        // aRunRange, aVersion, aSubVersion filled from filename

			if (!aRunRange.Comprises(query.GetIlcCDBRunRange())) continue;
 			// aRunRange contains requested run!

			if(query.GetVersion() != aVersion || query.GetSubVersion() != aSubVersion) continue;
			// aVersion and aSubVersion are requested version and subVersion!

			if(result->GetVersion() == aVersion && result->GetSubVersion() == aSubVersion){
              			IlcError(Form("More than one object valid for run %d, version %d_%d!",
		       			query.GetFirstRun(), aVersion, aSubVersion));
	     			delete result;
	     			return NULL;
			}
			result->SetVersion(aVersion);
		        result->SetSubVersion(aSubVersion);
			result->SetFirstRun(aRunRange.GetFirstRun());
			result->SetLastRun(aRunRange.GetLastRun());
			
		}
	}

	return result;
}

//_____________________________________________________________________________
IlcCDBEntry* IlcCDBDump::GetEntry(const IlcCDBId& queryId) {
// get IlcCDBEntry from the database

	TDirectory::TContext context(gDirectory, fFile);

	if (!(fFile && fFile->IsOpen())) {
                IlcError("IlcCDBDump storage is not initialized properly");
                return NULL;
        }

        if (!gDirectory->cd(queryId.GetPath())) {
                return NULL;
        }

	IlcCDBId *dataId = GetEntryId(queryId);

	if (!dataId || !dataId->IsSpecified()) {
		if(dataId) delete dataId;
		return NULL;
	}

	TString keyname;
	if (!IdToKeyName(dataId->GetIlcCDBRunRange(), dataId->GetVersion(),
		dataId->GetSubVersion(), keyname)) {
		IlcDebug(2,Form("Bad ID encountered! Subnormal error!"));
		delete dataId;
		return NULL;
	}

	// get the only IlcCDBEntry object from the file
	// the object in the file is an IlcCDBEntry entry named keyname
	// keyName = Run#firstRun_#lastRun_v#version_s#subVersion

	TObject* anObject = gDirectory->Get(keyname);
	if (!anObject) {
		IlcDebug(2,Form("Bad storage data: NULL entry object!"));
		delete dataId;
		return NULL;
	} 

	if (IlcCDBEntry::Class() != anObject->IsA()) {
		IlcDebug(2,Form("Bad storage data: Invalid entry object!"));
		delete dataId;
		return NULL;
	}

	((IlcCDBEntry*) anObject)->SetLastStorage("dump");

	delete dataId;
	return (IlcCDBEntry*) anObject;
}

//_____________________________________________________________________________
IlcCDBId* IlcCDBDump::GetEntryId(const IlcCDBId& queryId) {
// get IlcCDBEntry from the database

	TDirectory::TContext context(gDirectory, fFile);

	if (!(fFile && fFile->IsOpen())) {
                IlcError("IlcCDBDump storage is not initialized properly");
                return NULL;
        }

        if (!gDirectory->cd(queryId.GetPath())) {
                return NULL;
        }

	IlcCDBId* dataId = 0;

	// look for a filename matching query requests (path, runRange, version, subVersion)
	if (!queryId.HasVersion()) {
		// if version is not specified, first check the selection criteria list
		IlcCDBId selectedId(queryId);
		GetSelection(&selectedId);
		dataId = GetId(queryId);
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
void IlcCDBDump::GetEntriesForLevel0(const IlcCDBId& queryId, TList* result) {
// multiple request (IlcCDBStorage::GetAll)

	TDirectory* saveDir = gDirectory;

	TIter iter(gDirectory->GetListOfKeys());
	TKey* key;

	while ((key = (TKey*) iter.Next())) {

		TString keyNameStr(key->GetName());
		if (queryId.GetIlcCDBPath().Level1Comprises(keyNameStr)) {
			gDirectory->cd(keyNameStr);
			GetEntriesForLevel1(queryId, result);
			
			saveDir->cd();
		}
	}
}

//_____________________________________________________________________________
void IlcCDBDump::GetEntriesForLevel1(const IlcCDBId& queryId, TList* result) {
// multiple request (IlcCDBStorage::GetAll)

        TIter iter(gDirectory->GetListOfKeys());
        TKey* key;

	TDirectory* level0Dir = (TDirectory*) gDirectory->GetMother();

        while ((key = (TKey*) iter.Next())) {

                TString keyNameStr(key->GetName());
                if (queryId.GetIlcCDBPath().Level2Comprises(keyNameStr)) {
			
			IlcCDBPath aPath(level0Dir->GetName(), 
					gDirectory->GetName(), keyNameStr);
			IlcCDBId anId(aPath, queryId.GetIlcCDBRunRange(), 
					queryId.GetVersion(), -1);

			IlcCDBEntry* anEntry = GetEntry(anId);
			if (anEntry) {
				result->Add(anEntry);
			}
	
                }
        }

}

                        
//_____________________________________________________________________________
TList* IlcCDBDump::GetEntries(const IlcCDBId& queryId) {
// multiple request (IlcCDBStorage::GetAll)

	TDirectory::TContext context(gDirectory, fFile);

	if (!(fFile && fFile->IsOpen())) {
                IlcError("IlcCDBDump storage is not initialized properly");
                return NULL;
        }

	TList* result = new TList();
	result->SetOwner();

	TIter iter(gDirectory->GetListOfKeys());
	TKey* key;

	while ((key = (TKey*) iter.Next())) {
		
		TString keyNameStr(key->GetName());
		if (queryId.GetIlcCDBPath().Level0Comprises(keyNameStr)) {
			gDirectory->cd(keyNameStr);
			GetEntriesForLevel0(queryId, result);

			fFile->cd();
		}
	}

	return result;
}

//_____________________________________________________________________________
Bool_t IlcCDBDump::PutEntry(IlcCDBEntry* entry, const char* mirrors) {
// put an IlcCDBEntry object into the database

	TDirectory::TContext context(gDirectory, fFile);

	if (!(fFile && fFile->IsOpen())) {
		IlcError("IlcCDBDump storage is not initialized properly");
		return kFALSE;
	}

	if (fReadOnly) {
		IlcError("IlcCDBDump storage is read only!");
		return kFALSE;
	}

	TString mirrorsString(mirrors);
	if(!mirrorsString.IsNull())
		IlcWarning("IlcCDBLocal storage cannot take mirror SEs into account. They will be ignored.");

	IlcCDBId& id = entry->GetId();
	
        if (!gDirectory->cd(id.GetPath())) {
                if (!MkDir(id.GetPath())) {
                        IlcError(Form("Can't open directory <%s>!", 
					id.GetPath().Data()));
                        return kFALSE;
                }
        }

	// set version and subVersion for the entry to be stored
	if (!PrepareId(id)) {
		return kFALSE;		
	}

	// build keyname from entry's id
	TString keyname;
	if (!IdToKeyName(id.GetIlcCDBRunRange(), id.GetVersion(), id.GetSubVersion(), keyname)) {
		IlcError("Invalid ID encountered! Subnormal error!");
		return kFALSE;
	}	

	// write object (key name: Run#firstRun_#lastRun_v#version_s#subVersion)
	Bool_t result = gDirectory->WriteTObject(entry, keyname);
	if (!result) {
		IlcError(Form("Can't write entry to file: %s",
				fFile->GetName()));
	}

        if(result) {
    		IlcInfo(Form("CDB object stored into file %s",fFile->GetName()));
    		IlcInfo(Form("TDirectory/key name: %s/%s",id.GetPath().Data(),keyname.Data()));
        }

	return result;
}
//_____________________________________________________________________________
TList* IlcCDBDump::GetIdListFromFile(const char* fileName){

	TString turl(fileName);
	if (turl[0] != '/') {
		turl.Prepend(TString(gSystem->WorkingDirectory()) + '/');
	}
	TFile *file = TFile::Open(turl);
	if (!file) {
		IlcError(Form("Can't open selection file <%s>!", turl.Data()));
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
Bool_t IlcCDBDump::Contains(const char* path) const{
// check for path in storage

	TDirectory::TContext context(gDirectory, fFile);
	if (!(fFile && fFile->IsOpen())) {
                IlcError("IlcCDBDump storage is not initialized properly");
                return kFALSE;
        }
	
	return gDirectory->cd(path);

}

//_____________________________________________________________________________
void IlcCDBDump::QueryValidFiles()
{
// Query the CDB for files valid for IlcCDBStorage::fRun
// fills list fValidFileIds with IlcCDBId objects created from file name

	IlcError("Not yet (and maybe never) implemented");
}

//_____________________________________________________________________________
Bool_t IlcCDBDump::IdToFilename(const IlcCDBId& /*id*/, TString& /*filename*/) const {
// build file name from IlcCDBId (path, run range, version) and fDBFolder

	IlcError("Not implemented");
        return kFALSE;
}

//_____________________________________________________________________________
Int_t IlcCDBDump::GetLatestVersion(const char* path, Int_t run){
// get last version found in the database valid for run and path

	IlcCDBPath aCDBPath(path);
	if(!aCDBPath.IsValid() || aCDBPath.IsWildcard()) {
		IlcError(Form("Invalid path in request: %s", path));
		return -1;
	}

	IlcCDBId query(path, run, run, -1, -1);
	IlcCDBId *dataId = GetId(query);

	if(!dataId) return -1;
	Int_t version = dataId->GetVersion();

	delete dataId;
	return version;
}

//_____________________________________________________________________________
Int_t IlcCDBDump::GetLatestSubVersion(const char* path, Int_t run, Int_t version){
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

//_____________________________________________________________________________
void IlcCDBDump::SetRetry(Int_t /* nretry */, Int_t /* initsec */) {

	// Function to set the exponential retry for putting entries in the OCDB

	IlcInfo("This function sets the exponential retry for putting entries in the OCDB - to be used ONLY for IlcCDBGrid --> returning without doing anything");
	return;
} 

/////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                             //
// IlcCDBDump factory  			                                                       //
//                                                                                             //
/////////////////////////////////////////////////////////////////////////////////////////////////

ClassImp(IlcCDBDumpFactory)

//_____________________________________________________________________________
Bool_t IlcCDBDumpFactory::Validate(const char* dbString) {
// check if the string is valid dump URI

        TRegexp dbPattern("^dump://.+$");

        return TString(dbString).Contains(dbPattern);
}

//_____________________________________________________________________________
IlcCDBParam* IlcCDBDumpFactory::CreateParameter(const char* dbString) {
// create IlcCDBDumpParam class from the URI string

        if (!Validate(dbString)) {
                return NULL;
        }

        TString pathname(dbString + sizeof("dump://") - 1);

	Bool_t readOnly;

	if (pathname.Contains(TRegexp(";ReadOnly$"))) {
		pathname.Resize(pathname.Length() - sizeof(";ReadOnly") + 1);
		readOnly = kTRUE;
	} else {
		readOnly = kFALSE;
	}

	gSystem->ExpandPathName(pathname);
	
	if (pathname[0] != '/') {
		pathname.Prepend(TString(gSystem->WorkingDirectory()) + '/');
	}

        return new IlcCDBDumpParam(pathname, readOnly);
}

//_____________________________________________________________________________
IlcCDBStorage* IlcCDBDumpFactory::Create(const IlcCDBParam* param) {
// create IlcCDBDump storage instance from parameters

        if (IlcCDBDumpParam::Class() == param->IsA()) {

                const IlcCDBDumpParam* dumpParam = 
			(const IlcCDBDumpParam*) param;

                return new IlcCDBDump(dumpParam->GetPath(),
				dumpParam->IsReadOnly());
        }

        return NULL;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                             //
// IlcCDBDump parameter class  			                                               //
//                                                                                             //
/////////////////////////////////////////////////////////////////////////////////////////////////

ClassImp(IlcCDBDumpParam)

//_____________________________________________________________________________
IlcCDBDumpParam::IlcCDBDumpParam():
fDBPath(), fReadOnly(kFALSE)
{
// default constructor

}

//_____________________________________________________________________________
IlcCDBDumpParam::IlcCDBDumpParam(const char* dbPath, Bool_t readOnly):
        fDBPath(dbPath), fReadOnly(readOnly)
{
// constructor

	TString uri;
	uri += "dump://";
	uri += dbPath;

	if (fReadOnly) {
		uri += ";ReadOnly";
	}
	
        SetURI(uri);
        SetType("dump");
}

//_____________________________________________________________________________
IlcCDBDumpParam::~IlcCDBDumpParam() {
// destructor

}

//_____________________________________________________________________________
IlcCDBParam* IlcCDBDumpParam::CloneParam() const {
// clone parameter

	return new IlcCDBDumpParam(fDBPath, fReadOnly);
}

//_____________________________________________________________________________
ULong_t IlcCDBDumpParam::Hash() const {
// return Hash function

	return fDBPath.Hash();
}

//_____________________________________________________________________________
Bool_t IlcCDBDumpParam::IsEqual(const TObject* obj) const {
// check if this object is equal to IlcCDBParam obj
	
	if (this == obj) {
		return kTRUE;
	}

	if (IlcCDBDumpParam::Class() != obj->IsA()) {
		return kFALSE;
	}

	IlcCDBDumpParam* other = (IlcCDBDumpParam*) obj;

	return fDBPath == other->fDBPath;
}

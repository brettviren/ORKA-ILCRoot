#ifndef ILC_CDB_STORAGE_H
#define ILC_CDB_STORAGE_H

/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/////////////////////////////////////////////////////////////////////
//                                                                 //
//  class IlcCDBStorage						   //
//  interface to specific storage classes                          //
//  (IlcCDBGrid, IlcCDBLocal, IlcCDBDump)			   //
//                                                                 //
/////////////////////////////////////////////////////////////////////

#include "IlcCDBId.h"
#include "IlcCDBMetaData.h"
#include "IlcCDBManager.h"

#include <TList.h>
#include <TObjArray.h>

class IlcCDBEntry;
class IlcCDBPath;
class IlcCDBParam;
class TFile;

class IlcCDBStorage: public TObject {

public:

	IlcCDBStorage();

	void SetURI(const TString& uri) {fURI = uri;}
	const TString& GetURI() const {return fURI;}
	const TString& GetType() const {return fType;}
	const TString& GetBaseFolder() const {return fBaseFolder;}
	IlcCDBManager::DataType GetDataType() const;


	void ReadSelectionFromFile(const char *fileName);
	
	void AddSelection(const IlcCDBId& selection);

	void AddSelection(const IlcCDBPath& path, 
			const IlcCDBRunRange& runRange,
			Int_t version,
			Int_t subVersion = -1);
	
	void AddSelection(const IlcCDBPath& path,
			Int_t firstRun,
			Int_t lastRun,
			Int_t version,
			Int_t subVersion = -1);
			
	void RemoveSelection(const IlcCDBId& selection);

	void RemoveSelection(const IlcCDBPath& path,
			const IlcCDBRunRange& runRange);
	
	void RemoveSelection(const IlcCDBPath& path,
			Int_t firstRun = -1,
			Int_t lastRun = -1);

	void RemoveSelection(int position);
	void RemoveAllSelections();
		
	void PrintSelectionList();

	IlcCDBEntry* Get(const IlcCDBId& query);
	IlcCDBEntry* Get(const IlcCDBPath& path, Int_t runNumber,
				Int_t version = -1, Int_t subVersion = -1);
	IlcCDBEntry* Get(const IlcCDBPath& path, const IlcCDBRunRange& runRange,
				 Int_t version = -1, Int_t subVersion = -1);

	TList* GetAll(const IlcCDBId& query);
	TList* GetAll(const IlcCDBPath& path, Int_t runNumber, 
				Int_t version = -1, Int_t subVersion = -1);
	TList* GetAll(const IlcCDBPath& path, const IlcCDBRunRange& runRange,
				 Int_t version = -1, Int_t subVersion = -1);
	
	IlcCDBId* GetId(const IlcCDBId& query);
	IlcCDBId* GetId(const IlcCDBPath& path, Int_t runNumber,
				Int_t version = -1, Int_t subVersion = -1);
	IlcCDBId* GetId(const IlcCDBPath& path, const IlcCDBRunRange& runRange,
				 Int_t version = -1, Int_t subVersion = -1);

	Bool_t Put(TObject* object, IlcCDBId& id,  IlcCDBMetaData* metaData, const char* mirrors="",
				IlcCDBManager::DataType type=IlcCDBManager::kPrivate);
	Bool_t Put(IlcCDBEntry* entry, const char* mirrors="", IlcCDBManager::DataType type=IlcCDBManager::kPrivate);

	virtual void SetMirrorSEs(const char* mirrors);
	virtual const char* GetMirrorSEs() const;

	virtual Bool_t IsReadOnly() const = 0;
	virtual Bool_t HasSubVersion() const = 0;
	virtual Bool_t Contains(const char* path) const = 0;
	virtual Bool_t IdToFilename(const IlcCDBId& id, TString& filename) const = 0;

	void QueryCDB(Int_t run, const char* pathFilter="*",
			Int_t version=-1, IlcCDBMetaData *mdFilter=0);
	void PrintQueryCDB();
	TObjArray* GetQueryCDBList() {return &fValidFileIds;}

	virtual Int_t GetLatestVersion(const char* path, Int_t run)=0;
	virtual Int_t GetLatestSubVersion(const char* path, Int_t run, Int_t version=-1)=0;
	virtual void SetRetry(Int_t nretry, Int_t initsec) = 0;

protected:

	virtual ~IlcCDBStorage();
	void    GetSelection(/*const*/ IlcCDBId* id);
	virtual IlcCDBEntry* GetEntry(const IlcCDBId& query) = 0;
	virtual IlcCDBId* GetEntryId(const IlcCDBId& query) = 0;
	virtual TList* GetEntries(const IlcCDBId& query) = 0;
	virtual Bool_t PutEntry(IlcCDBEntry* entry, const char* mirrors="") = 0;
	virtual TList *GetIdListFromFile(const char* fileName)=0;
	virtual void   QueryValidFiles() = 0;
	void 	LoadTreeFromFile(IlcCDBEntry* entry) const;
	//void 	SetTreeToFile(IlcCDBEntry* entry, TFile* file) const;

	TObjArray fValidFileIds; 	// list of Id's of the files valid for a given run (cached as fRun)
	Int_t fRun;		        // run number, used to manage list of valid files
	IlcCDBPath fPathFilter;	        // path filter, used to manage list of valid files
	Int_t fVersion;		        // version, used to manage list of valid files
	IlcCDBMetaData* fMetaDataFilter; // metadata, used to manage list of valid files

	TList fSelections; 	// list of selection criteria
	TString fURI;		// storage URI;
	TString fType;    //! Local, Grid: base folder name - Dump: file name
	TString fBaseFolder;    //! Local, Grid: base folder name - Dump: file name
	Short_t    fNretry;              // Number of retries in opening the file
        Short_t    fInitRetrySeconds;        // Seconds for first retry

private:
	IlcCDBStorage(const IlcCDBStorage & source);
	IlcCDBStorage & operator=(const IlcCDBStorage & source);

	ClassDef(IlcCDBStorage, 0);
};

#endif

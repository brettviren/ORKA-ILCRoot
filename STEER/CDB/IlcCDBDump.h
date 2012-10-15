#ifndef ILC_CDB_DUMP_H
#define ILC_CDB_DUMP_H

/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/////////////////////////////////////////////////////////////////////
//                                                                 //
//  class IlcCDBDump						   //
//  access class to a DataBase in a dump storage (single file)     //
//                                                                 //
/////////////////////////////////////////////////////////////////////

#include "IlcCDBStorage.h"
#include "IlcCDBManager.h"

class TDirectory;
class TFile;

class IlcCDBDump: public IlcCDBStorage {
	friend class IlcCDBDumpFactory;

public:

	virtual Bool_t IsReadOnly() const {return fReadOnly;};
	virtual Bool_t HasSubVersion() const {return kFALSE;};
	virtual Bool_t Contains(const char* path) const;
	virtual Int_t GetLatestVersion(const char* path, Int_t run);
	virtual Int_t GetLatestSubVersion(const char* path, Int_t run, Int_t version=-1);
	virtual Bool_t IdToFilename(const IlcCDBId& id, TString& filename) const;
	virtual void SetRetry(Int_t /* nretry */, Int_t /* initsec */);

protected:

	virtual IlcCDBEntry* 	GetEntry(const IlcCDBId& query);
	virtual IlcCDBId* 	GetEntryId(const IlcCDBId& query);
        virtual TList* 		GetEntries(const IlcCDBId& query);
        virtual Bool_t 		PutEntry(IlcCDBEntry* entry, const char* mirrors="");
	virtual TList* 		GetIdListFromFile(const char* fileName);

private:

	IlcCDBDump(const IlcCDBDump & source);
	IlcCDBDump & operator=(const IlcCDBDump & source);
	IlcCDBDump(const char* dbFile, Bool_t readOnly);
	virtual ~IlcCDBDump();	

	Bool_t KeyNameToId(const char* keyname, IlcCDBRunRange& runRange,
			Int_t& version, Int_t& subVersion);
	Bool_t IdToKeyName(const IlcCDBRunRange& runRange, Int_t version,
		        Int_t subVersion, TString& keyname); 	

	Bool_t MkDir(const TString& dir);


	Bool_t PrepareId(IlcCDBId& id);
//	Bool_t GetId(const IlcCDBId& query, IlcCDBId& result);
	IlcCDBId* GetId(const IlcCDBId& query);

	virtual void QueryValidFiles();

	void GetEntriesForLevel0(const IlcCDBId& query, TList* result);
	void GetEntriesForLevel1(const IlcCDBId& query, TList* result);

	TFile* fFile;		// Dump file
	Bool_t fReadOnly;	// ReadOnly flag

	ClassDef(IlcCDBDump, 0);
};

/////////////////////////////////////////////////////////////////////
//                                                                 //
//  class IlcCDBDumpFactory					   //
//                                                                 //
/////////////////////////////////////////////////////////////////////

class IlcCDBDumpFactory: public IlcCDBStorageFactory {

public:

        virtual Bool_t Validate(const char* dbString);
        virtual IlcCDBParam* CreateParameter(const char* dbString);

protected:
        virtual IlcCDBStorage* Create(const IlcCDBParam* param);

        ClassDef(IlcCDBDumpFactory, 0);
};

/////////////////////////////////////////////////////////////////////
//                                                                 //
//  class IlcCDBDumpParam					   //
//                                                                 //
/////////////////////////////////////////////////////////////////////

class IlcCDBDumpParam: public IlcCDBParam {

public:
        IlcCDBDumpParam();
        IlcCDBDumpParam(const char* dbPath, Bool_t readOnly = kFALSE);
        
        virtual ~IlcCDBDumpParam();

        const TString& GetPath() const {return fDBPath;};
	Bool_t IsReadOnly() const {return fReadOnly;};

	virtual IlcCDBParam* CloneParam() const;

	virtual ULong_t Hash() const;
	virtual Bool_t IsEqual(const TObject* obj) const;
	
private:

        TString fDBPath;	// Dump file path name
	Bool_t fReadOnly;	// ReadOnly flag

	ClassDef(IlcCDBDumpParam, 0);
};

#endif

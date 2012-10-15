#ifndef ILC_CDB_LOCAL_H
#define ILC_CDB_LOCAL_H

/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/////////////////////////////////////////////////////////////////////
//                                                                 //
//  class IlcCDBLocal						   //
//  access class to a DataBase in a local storage                  //
//                                                                 //
/////////////////////////////////////////////////////////////////////

#include "IlcCDBStorage.h"
#include "IlcCDBManager.h"

class IlcCDBLocal: public IlcCDBStorage {
	friend class IlcCDBLocalFactory;

public:

	virtual Bool_t IsReadOnly() const {return kFALSE;};
	virtual Bool_t HasSubVersion() const {return kTRUE;};
	virtual Bool_t Contains(const char* path) const;
	virtual Int_t  GetLatestVersion(const char* path, Int_t run);
	virtual Int_t  GetLatestSubVersion(const char* path, Int_t run, Int_t version=-1);
	virtual Bool_t IdToFilename(const IlcCDBId& id, TString& filename) const;
	virtual void SetRetry(Int_t /* nretry */, Int_t /* initsec */);

protected:

	virtual IlcCDBEntry*    GetEntry(const IlcCDBId& queryId);
	virtual IlcCDBId* 	GetEntryId(const IlcCDBId& queryId);
        virtual TList* 		GetEntries(const IlcCDBId& queryId);
        virtual Bool_t 		PutEntry(IlcCDBEntry* entry, const char* mirrors="");
	virtual TList* 		GetIdListFromFile(const char* fileName);

private:

	IlcCDBLocal(const IlcCDBLocal & source);
	IlcCDBLocal & operator=(const IlcCDBLocal & source);
	IlcCDBLocal(const char* baseDir);
	virtual ~IlcCDBLocal();
	
	Bool_t FilenameToId(const char* filename, IlcCDBRunRange& runRange, 
			Int_t& version, Int_t& subVersion);

	Bool_t PrepareId(IlcCDBId& id);
//	Bool_t GetId(const IlcCDBId& query, IlcCDBId& result);
	IlcCDBId* GetId(const IlcCDBId& query);

	virtual void QueryValidFiles();

	void GetEntriesForLevel0(const char* level0, const IlcCDBId& query, TList* result);
	void GetEntriesForLevel1(const char* level0, const char* Level1,
			const IlcCDBId& query, TList* result);

	TString fBaseDirectory; // path of the DB folder

	ClassDef(IlcCDBLocal, 0); // access class to a DataBase in a local storage
};

/////////////////////////////////////////////////////////////////////
//                                                                 //
//  class IlcCDBLocalFactory					   //
//                                                                 //
/////////////////////////////////////////////////////////////////////

class IlcCDBLocalFactory: public IlcCDBStorageFactory {

public:

	virtual Bool_t Validate(const char* dbString);
        virtual IlcCDBParam* CreateParameter(const char* dbString);

protected:
        virtual IlcCDBStorage* Create(const IlcCDBParam* param);

        ClassDef(IlcCDBLocalFactory, 0);
};

/////////////////////////////////////////////////////////////////////
//                                                                 //
//  class IlcCDBLocalParam					   //
//                                                                 //
/////////////////////////////////////////////////////////////////////

class IlcCDBLocalParam: public IlcCDBParam {
	
public:
	IlcCDBLocalParam();
	IlcCDBLocalParam(const char* dbPath);
	IlcCDBLocalParam(const char* dbPath, const char* uri);
	
	virtual ~IlcCDBLocalParam();

	const TString& GetPath() const {return fDBPath;};

	virtual IlcCDBParam* CloneParam() const;

        virtual ULong_t Hash() const;
        virtual Bool_t IsEqual(const TObject* obj) const;

private:

	TString fDBPath; // path of the DB folder

	ClassDef(IlcCDBLocalParam, 0);
};

#endif

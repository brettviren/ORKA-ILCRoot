#ifndef ILCCDBGRID_H
#define ILCCDBGRID_H

/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/////////////////////////////////////////////////////////////////////
//                                                                 //
//  class IlcCDBGrid						   //
//  access class to a DataBase in an AliEn storage  		   //
//                                                                 //
/////////////////////////////////////////////////////////////////////

#include "IlcCDBStorage.h"
#include "IlcCDBManager.h"
#include "IlcCDBMetaData.h"

class IlcCDBGrid: public IlcCDBStorage {
	friend class IlcCDBGridFactory;

public:
		  
	virtual Bool_t IsReadOnly() const {return kFALSE;}
	virtual Bool_t HasSubVersion() const {return kFALSE;}
	virtual Bool_t Contains(const char* path) const;
	virtual Int_t  GetLatestVersion(const char* path, Int_t run);
	virtual Int_t  GetLatestSubVersion(const char* path, Int_t run, Int_t version);
	virtual Bool_t IdToFilename(const IlcCDBId& id, TString& filename) const;
	virtual void SetRetry(Int_t nretry, Int_t initsec);
	virtual void SetMirrorSEs(const char* mirrors) {fMirrorSEs=mirrors;}
	virtual const char* GetMirrorSEs() const {return fMirrorSEs;}


protected:

	virtual IlcCDBEntry*	GetEntry(const IlcCDBId& queryId);
	virtual IlcCDBId*	GetEntryId(const IlcCDBId& queryId);
	virtual TList* 		GetEntries(const IlcCDBId& queryId);
	virtual Bool_t          PutEntry(IlcCDBEntry* entry, const char* mirrors="");
	virtual TList* 		GetIdListFromFile(const char* fileName);

private:

	IlcCDBGrid(const char *gridUrl, const char *user, const char* dbFolder,
	           const char *se, const char* cacheFolder, Bool_t operateDisconnected,
		   Long64_t cacheSize, Long_t cleanupInterval);

	virtual ~IlcCDBGrid();

	IlcCDBGrid(const IlcCDBGrid& db);
	IlcCDBGrid& operator = (const IlcCDBGrid& db);

	Bool_t FilenameToId(TString& filename, IlcCDBId& id);

	Bool_t PrepareId(IlcCDBId& id);
	IlcCDBId* GetId(const TObjArray& validFileIds, const IlcCDBId& query);
	IlcCDBEntry* GetEntryFromFile(TString& filename, IlcCDBId* dataId);

	// TODO  use AliEnTag classes!
	Bool_t AddTag(TString& foldername, const char* tagname);
	Bool_t TagFileId(TString& filename, const IlcCDBId* id);
	Bool_t TagFileMetaData(TString& filename, const IlcCDBMetaData* md);
	Bool_t TagShortLived(TString& filename, Bool_t value);

	void MakeQueryFilter(Int_t firstRun, Int_t lastRun, const IlcCDBMetaData* md, TString& result) const;

	virtual void QueryValidFiles();

	TString    fGridUrl;	 // Grid Url ("alien://aliendb4.cern.ch:9000")
	TString    fUser;	 // User
	TString    fDBFolder;    // path of the DB folder
	TString    fSE;	  	 // Storage Element
	TString    fMirrorSEs;	 // Mirror Storage Elements
	TString    fCacheFolder; // local cache folder
	Bool_t     fOperateDisconnected; // Operate disconnected flag
	Long64_t   fCacheSize;           // local cache size (in bytes)
	Long_t     fCleanupInterval;     // local cache cleanup interval

ClassDef(IlcCDBGrid, 0)      // access class to a DataBase in an AliEn storage
};

/////////////////////////////////////////////////////////////////////
//                                                                 //
//  class IlcCDBGridFactory					   //
//                                                                 //
/////////////////////////////////////////////////////////////////////

class IlcCDBGridFactory: public IlcCDBStorageFactory {

public:

	virtual Bool_t Validate(const char* gridString);
        virtual IlcCDBParam* CreateParameter(const char* gridString);
	virtual ~IlcCDBGridFactory(){}

protected:
        virtual IlcCDBStorage* Create(const IlcCDBParam* param);

        ClassDef(IlcCDBGridFactory, 0);
};

/////////////////////////////////////////////////////////////////////
//                                                                 //
//  class IlcCDBGridParam					   //
//                                                                 //
/////////////////////////////////////////////////////////////////////

class IlcCDBGridParam: public IlcCDBParam {
	
public:
	IlcCDBGridParam();
	IlcCDBGridParam(const char* gridUrl, const char* user,
			const char* dbFolder, const char* se,
			const char* cacheFolder, Bool_t operateDisconnected,
			Long64_t cacheSize, Long_t cleanupInterval);
	
	virtual ~IlcCDBGridParam();

	const TString& GridUrl() const {return fGridUrl;}
	const TString& GetUser() const {return fUser;}
	const TString& GetDBFolder() const {return fDBFolder;}
	const TString& GetSE() 	 const {return fSE;}
	const TString& GetCacheFolder() const {return fCacheFolder;}
	Bool_t  GetOperateDisconnected() const {return fOperateDisconnected;}
	Long64_t  GetCacheSize() const {return fCacheSize;}
	Long_t  GetCleanupInterval() const {return fCleanupInterval;}

	virtual IlcCDBParam* CloneParam() const;

        virtual ULong_t Hash() const;
        virtual Bool_t IsEqual(const TObject* obj) const;

private:
	TString  fGridUrl;     // Grid url "Host:port"
	TString  fUser;	      // User
	TString  fDBFolder;    // path of the DB folder
	TString  fSE;	      // Storage Element
	TString  fCacheFolder; // Cache folder
	Bool_t   fOperateDisconnected; // Operate disconnected flag
	Long64_t fCacheSize;           // local cache size (in bytes)
	Long_t   fCleanupInterval;     // local cache cleanup interval

	ClassDef(IlcCDBGridParam, 0);
};


#endif

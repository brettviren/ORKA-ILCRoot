#ifndef ILC_CDB_MANAGER_H
#define ILC_CDB_MANAGER_H

/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/////////////////////////////////////////////////////////////////////
//                                                                 //
//  class IlcCDBManager                                            //
//                                                                 //
/////////////////////////////////////////////////////////////////////

#include <TObject.h>
#include <TList.h>
#include <TMap.h>
#include <TSystem.h>
#include <TFile.h>

class IlcCDBEntry;
class IlcCDBId;
class IlcCDBPath;
class IlcCDBRunRange;
class IlcCDBMetaData;
class IlcCDBStorage;
class IlcCDBStorageFactory;
class IlcCDBParam;

class IlcCDBManager: public TObject {

 public:
	enum DataType {kCondition=0, kReference, kPrivate};

	void RegisterFactory(IlcCDBStorageFactory* factory);

	Bool_t HasStorage(const char* dbString) const;

	IlcCDBParam* CreateParameter(const char* dbString) const;
	IlcCDBParam* GetCondParam() const {return fCondParam;}
	IlcCDBParam* GetRefParam() const {return fRefParam;}
	static const char* GetDataTypeName(DataType type);

	IlcCDBStorage* GetStorage(const char* dbString);
	IlcCDBStorage* GetStorage(const IlcCDBParam* param);

	TList* GetActiveStorages();

	const TMap* GetStorageMap() const {return fStorageMap;}
	const TList* GetRetrievedIds() const {return fIds;}

	void SetDefaultStorage(const char* dbString);
	void SetDefaultStorage(const IlcCDBParam* param);
	void SetDefaultStorage(IlcCDBStorage *storage);
	void SetDefaultStorage(const char* runType, const char* simType);
	void SetDefaultStorageFromRun(Int_t run);

	Bool_t IsDefaultStorageSet() const {return fDefaultStorage != 0;}
	IlcCDBStorage* GetDefaultStorage() const {return fDefaultStorage;}
	void UnsetDefaultStorage();

	void SetSpecificStorage(const char* calibType, const char* dbString);
	void SetSpecificStorage(const char* calibType, const IlcCDBParam* param);

	IlcCDBStorage* GetSpecificStorage(const char* calibType);

	void SetDrain(const char* dbString);
	void SetDrain(const IlcCDBParam* param);
	void SetDrain(IlcCDBStorage *storage);

	Bool_t IsDrainSet() const {return fDrainStorage != 0;}

	Bool_t Drain(IlcCDBEntry* entry);

	void UnsetDrain(){fDrainStorage = 0x0;}

	IlcCDBEntry* Get(const IlcCDBId& query, Bool_t forceCaching=kFALSE);
	IlcCDBEntry* Get(const IlcCDBPath& path, Int_t runNumber=-1,
				Int_t version = -1, Int_t subVersion = -1);
	IlcCDBEntry* Get(const IlcCDBPath& path, const IlcCDBRunRange& runRange,
				 Int_t version = -1, Int_t subVersion = -1);
	IlcCDBEntry* GetEntryFromSnapshot(const char* path);

	const char* GetURI(const char* path);				 

	TList* GetAll(const IlcCDBId& query);
	TList* GetAll(const IlcCDBPath& path, Int_t runNumber=-1,
				Int_t version = -1, Int_t subVersion = -1);
	TList* GetAll(const IlcCDBPath& path, const IlcCDBRunRange& runRange,
				 Int_t version = -1, Int_t subVersion = -1); 

	Bool_t Put(TObject* object, const IlcCDBId& id, IlcCDBMetaData* metaData,
			const char* mirrors="", const DataType type=kPrivate);
	Bool_t Put(IlcCDBEntry* entry, const char* mirrors="", DataType type=kPrivate);

	void SetCacheFlag(Bool_t cacheFlag) {fCache=cacheFlag;}
	Bool_t GetCacheFlag() const {return fCache;}
	
	ULong_t SetLock(Bool_t lockFlag=kTRUE, ULong_t key=0);
	Bool_t GetLock() const {return fLock;}

	void SetRaw(Bool_t rawFlag){fRaw=rawFlag;}
	Bool_t GetRaw() const {return fRaw;}

	void SetRun(Int_t run);
	Int_t GetRun() const {return fRun;}

	void SetMirrorSEs(const char* mirrors);
	const char* GetMirrorSEs() const;

	void DestroyActiveStorages();
	void DestroyActiveStorage(IlcCDBStorage* storage);

	void QueryCDB();

	void Print(Option_t* option="") const;

	static void Destroy();
	~IlcCDBManager();

  	void ClearCache();
  	void UnloadFromCache(const char* path);
	const TMap* GetEntryCache() const {return &fEntryCache;}

	Bool_t IsShortLived(const char* path);

	static IlcCDBManager* Instance(TMap *entryCache = NULL, Int_t run = -1);

	void Init();
	void InitFromCache(TMap *entryCache, Int_t run);
	Bool_t InitFromSnapshot(const char* snapshotFileName, Bool_t overwrite=kTRUE);
	Bool_t SetSnapshotMode(const char* snapshotFileName="OCDB.root");
	void UnsetSnapshotMode() {fSnapshotMode=kFALSE;}
	void DumpToSnapshotFile(const char* snapshotFileName, Bool_t singleKeys);
  
protected:

	static TString fgkCondUri;	// URI of the Conditions data base folder
	static TString fgkRefUri;	// URI of the Reference data base folder
	static TString fgkMCIdealStorage;	// URI of the MC-Ideal Conditions data base folder form MC data
	static TString fgkMCFullStorage;	// URI of the MC-Full Conditions data base folder form MC data
	static TString fgkMCResidualStorage;	// URI of the MC-Residual Conditions data base folder form MC data
	static TString fgkOCDBFolderXMLfile;	// alien path of the XML file for OCDB folder <--> Run range correspondance

	IlcCDBManager() ; 
	IlcCDBManager(const IlcCDBManager & source);
	IlcCDBManager & operator=(const IlcCDBManager & source);

	static IlcCDBManager* fgInstance; // IlcCDBManager instance
	
	IlcCDBStorage* GetActiveStorage(const IlcCDBParam* param);
	void PutActiveStorage(IlcCDBParam* param, IlcCDBStorage* storage);

  	void CacheEntry(const char* path, IlcCDBEntry* entry);

	IlcCDBParam* SelectSpecificStorage(const TString& path);
				 
	IlcCDBId* GetId(const IlcCDBId& query);
	IlcCDBId* GetId(const IlcCDBPath& path, Int_t runNumber=-1,
				Int_t version = -1, Int_t subVersion = -1);
	IlcCDBId* GetId(const IlcCDBPath& path, const IlcCDBRunRange& runRange,
				 Int_t version = -1, Int_t subVersion = -1);
	

//	void Init();
	void InitShortLived();
//	void InitFromCache(TMap *entryCache, Int_t run);


	TList fFactories; 		//! list of registered storage factories
	TMap fActiveStorages;		//! list of active storages
	TMap fSpecificStorages;         //! list of detector-specific storages
	TMap fEntryCache;    	  	//! cache of the retrieved objects

	TList* fIds;           	//! List of the retrieved object Id's (to be streamed to file)
	TMap* fStorageMap;      //! list of storages (to be streamed to file)
	TList* fShortLived; 	//! List of short lived objects

	IlcCDBStorage *fDefaultStorage;	//! pointer to default storage
	IlcCDBStorage *fDrainStorage;	//! pointer to drain storage

	IlcCDBParam* fCondParam; 	// Conditions data storage parameters
	IlcCDBParam* fRefParam;		// Reference data storage parameters

	Int_t fRun;			//! The run number
	Bool_t fCache;			//! The cache flag
	Bool_t fLock; 	//! Lock flag, if ON default storage and run number cannot be reset

        Bool_t fSnapshotMode;           //! flag saying if we are in snapshot mode
	TFile *fSnapshotFile;

	Bool_t fRaw;   // flag to say whether we are in the raw case
	Int_t fStartRunLHCPeriod; // 1st run of the LHC period set
	Int_t fEndRunLHCPeriod;   // last run of the LHC period set
	TString fLHCPeriod;       // LHC period alien folder

private:
   ULong_t fKey;  //! Key for locking/unlocking


	ClassDef(IlcCDBManager, 0);
};


/////////////////////////////////////////////////////////////////////
//                                                                 //
//  class IlcCDBStorageFactory                                     //
//                                                                 //
/////////////////////////////////////////////////////////////////////

class IlcCDBParam;
class IlcCDBStorageFactory: public TObject {
	friend class IlcCDBManager;
	
public:
	virtual ~IlcCDBStorageFactory(){}
	virtual Bool_t Validate(const char* dbString) = 0;
	virtual IlcCDBParam* CreateParameter(const char* dbString) = 0;	

protected:
	virtual IlcCDBStorage* Create(const IlcCDBParam* param) = 0;

	ClassDef(IlcCDBStorageFactory, 0);
};

/////////////////////////////////////////////////////////////////////
//                                                                 //
//  class IlcCDBParam                                              //
//                                                                 //
/////////////////////////////////////////////////////////////////////

class IlcCDBParam: public TObject {

public:

	IlcCDBParam();
	virtual ~IlcCDBParam();

	const TString& GetType() const {return fType;};
	const TString& GetURI() const {return fURI;};

	virtual IlcCDBParam* CloneParam() const = 0;

protected:

	void SetType(const char* type) {fType = type;};
	void SetURI(const char* uri) {fURI = uri;};

private:

	TString fType; //! CDB type
	TString fURI;  //! CDB URI

	ClassDef(IlcCDBParam, 0);
};

#endif

#ifndef ILC_CDB_ENTRY_H
#define ILC_CDB_ENTRY_H

/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/////////////////////////////////////////////////////////////////////
//                                                                 //
//  class IlcCDBEntry						   //
//  container for an object, it identity (IlcCDBId)  		   //
//  and its metaData (IlcCDBMetaData) 				   //
//                                                                 //
/////////////////////////////////////////////////////////////////////

#include "IlcCDBId.h"
#include "IlcCDBMetaData.h"

class IlcCDBEntry: public TObject {

public:
	IlcCDBEntry();

	IlcCDBEntry(TObject* object, const IlcCDBId& id,  
			IlcCDBMetaData* metaData, Bool_t owner = kFALSE);

	IlcCDBEntry(TObject* object, const IlcCDBPath& path, const IlcCDBRunRange& runRange,
			IlcCDBMetaData* metaData, Bool_t owner = kFALSE);

	IlcCDBEntry(TObject* object, const IlcCDBPath& path, const IlcCDBRunRange& runRange,
			Int_t version, IlcCDBMetaData* metaData, Bool_t owner = kFALSE);

	IlcCDBEntry(TObject* object, const IlcCDBPath& path, const IlcCDBRunRange& runRange,
			Int_t version, Int_t subVersion, 
			IlcCDBMetaData* metaData, Bool_t owner = kFALSE);

	IlcCDBEntry(TObject* object, const IlcCDBPath& path, Int_t firstRun, Int_t lastRun,
			IlcCDBMetaData* metaData, Bool_t owner = kFALSE);

	IlcCDBEntry(TObject* object, const IlcCDBPath& path, Int_t firstRun, Int_t lastRun,
			Int_t version, IlcCDBMetaData* metaData, Bool_t owner = kFALSE);

	IlcCDBEntry(TObject* object, const IlcCDBPath& path, Int_t firstRun, Int_t lastRun,
			Int_t version, Int_t subVersion, 
			IlcCDBMetaData* metaData, Bool_t owner = kFALSE);

	virtual ~IlcCDBEntry();


	void 		SetId(const IlcCDBId& id) {fId = id;};
	IlcCDBId& 	GetId() {return fId;};
	const IlcCDBId& GetId() const {return fId;};
	void 		PrintId() const;
	
	void 		SetObject(TObject* object) {fObject = object;};
	TObject* 	GetObject() {return fObject;};
	const TObject* 	GetObject() const {return fObject;};	

	void 			SetMetaData(IlcCDBMetaData* metaData) {fMetaData = metaData;};
	IlcCDBMetaData* 	GetMetaData() {return fMetaData;};
	const IlcCDBMetaData* 	GetMetaData() const {return fMetaData;};
	void 			PrintMetaData() const {fMetaData->PrintMetaData();}

	void 	SetOwner(Bool_t owner) {fIsOwner = owner;};
	Bool_t 	IsOwner() const {return fIsOwner;};
	
  	void 	SetVersion(Int_t version) {fId.SetVersion(version);}
  	void 	SetSubVersion(Int_t subVersion) {fId.SetSubVersion(subVersion);}
	
	const TString 	GetLastStorage() const {return fId.GetLastStorage();};
	void  		SetLastStorage(TString lastStorage) {fId.SetLastStorage(lastStorage);};

private:
	
	IlcCDBEntry(const IlcCDBEntry& other); // no copy ctor
	void operator= (const IlcCDBEntry& other); // no assignment op

	TObject* fObject;		// object
	IlcCDBId fId;			// entry ID
	IlcCDBMetaData* fMetaData; 	// metaData
	Bool_t fIsOwner; 		// ownership flag
	
	ClassDef(IlcCDBEntry, 1);
};

#endif

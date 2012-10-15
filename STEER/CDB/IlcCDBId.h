#ifndef ILC_CDB_ID_H
#define ILC_CDB_ID_H

/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/////////////////////////////////////////////////////////////////////
//                                                                 //
//  class IlcCDBId						   //
//  Identity of an object stored into a database:  		   //
//  path, run validity range, version, subVersion 		   //
//                                                                 //
/////////////////////////////////////////////////////////////////////

#include "IlcCDBPath.h"
#include "IlcCDBRunRange.h"

#include <TObject.h>

class IlcCDBId: public TObject {

public:

	IlcCDBId();

	IlcCDBId(const IlcCDBId& other);

	IlcCDBId(const IlcCDBPath& path, const IlcCDBRunRange& runRange,
		Int_t version = -1, Int_t subVersion = -1);

	IlcCDBId(const IlcCDBPath& path, 
		Int_t firstRun , Int_t lastRun , Int_t verison = -1, 
		Int_t subVersion = -1); 

	static IlcCDBId* MakeFromString(const TString& idString); 		
		
	virtual ~IlcCDBId();

	const IlcCDBPath& 	GetIlcCDBPath() const {return fPath;}
	const TString& 		GetPath() const {return fPath.GetPath();}
	const TString 		GetPathLevel(Int_t i) const {return fPath.GetLevel(i);}
	Bool_t 			IsWildcard() const {return fPath.IsWildcard();}

	void 	SetPath(const char* path) {fPath.SetPath(path);}

	const 		IlcCDBRunRange& GetIlcCDBRunRange() const {return fRunRange;}
	IlcCDBRunRange& GetIlcCDBRunRange() {return fRunRange;}
	Int_t 		GetFirstRun() const {return fRunRange.GetFirstRun();}
	Int_t 		GetLastRun() const {return fRunRange.GetLastRun();}
	void 		SetFirstRun(Int_t firstRun) {fRunRange.SetFirstRun(firstRun);}
	void 		SetLastRun(Int_t lastRun) {fRunRange.SetLastRun(lastRun);}
	void 		SetRunRange(Int_t firstRun, Int_t lastRun)
			{fRunRange.SetRunRange(firstRun, lastRun);}


	Bool_t 	IsAnyRange() const {return fRunRange.IsAnyRange();}


	Int_t 	GetVersion() const {return fVersion;}
	Int_t 	GetSubVersion() const {return fSubVersion;}
	void 	SetVersion(Int_t version) {fVersion = version;}
	void 	SetSubVersion(Int_t subVersion) {fSubVersion = subVersion;}

	const TString& 	GetLastStorage() const {return fLastStorage;}
	void 		SetLastStorage(TString& lastStorage){fLastStorage = lastStorage;}

	Bool_t IsValid() const; 
	Bool_t IsSpecified() const {return !(IsWildcard() || IsAnyRange());}

	Bool_t HasVersion() const {return fVersion >= 0;}
	Bool_t HasSubVersion() const {return fSubVersion >= 0;}

	Bool_t Comprises(const IlcCDBId& other) const
		{return fPath.Comprises(other.fPath)
		         && fRunRange.Comprises(other.fRunRange);}

	virtual Bool_t IsEqual(const TObject *obj) const;

	TString ToString() const;
	void Print(Option_t* option="") const;

private:

	IlcCDBPath fPath;		// path	
	IlcCDBRunRange fRunRange;	// run range
	Int_t fVersion;			// version
	Int_t fSubVersion;		// subversion
	TString fLastStorage;		// previous storage place (new, grid, local, dump)

	ClassDef(IlcCDBId, 1);
};

#endif

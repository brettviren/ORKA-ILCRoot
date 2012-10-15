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
//  class IlcCDBEntry						   //
//  container for an object, it identity (IlcCDBId)  		   //
//  and its metaData (IlcCDBMetaData) 				   //
//                                                                 //
/////////////////////////////////////////////////////////////////////

#include "IlcCDBEntry.h"
#include "IlcLog.h"

ClassImp(IlcCDBEntry)

//_____________________________________________________________________________
IlcCDBEntry::IlcCDBEntry():
fObject(NULL),
fId(),
fMetaData(NULL), 
fIsOwner(kFALSE){
// default constructor

}

//_____________________________________________________________________________
IlcCDBEntry::IlcCDBEntry(TObject* object, const IlcCDBId& id, 
			IlcCDBMetaData* metaData, Bool_t owner):
fObject(object), 
fId(id), 
fMetaData(metaData), 
fIsOwner(owner){
// constructor
    fMetaData->SetObjectClassName(fObject->ClassName());
}

//_____________________________________________________________________________
IlcCDBEntry::IlcCDBEntry(TObject* object, const IlcCDBPath& path, 
			const IlcCDBRunRange& runRange,
                        IlcCDBMetaData* metaData,Bool_t owner):
fObject(object), 
fId(path, runRange, -1, -1), 
fMetaData(metaData),
fIsOwner(owner){
// constructor
    fMetaData->SetObjectClassName(fObject->ClassName());
}

//_____________________________________________________________________________
IlcCDBEntry::IlcCDBEntry(TObject* object, const IlcCDBPath& path, 
			const IlcCDBRunRange& runRange,
			Int_t version, IlcCDBMetaData* metaData, Bool_t owner):
fObject(object), 
fId(path, runRange, version, -1), 
fMetaData(metaData),
fIsOwner(owner){
// constructor
    fMetaData->SetObjectClassName(fObject->ClassName());
}

//_____________________________________________________________________________
IlcCDBEntry::IlcCDBEntry(TObject* object, const IlcCDBPath& path, 
			const IlcCDBRunRange& runRange,
			Int_t version, Int_t subVersion, 
			IlcCDBMetaData* metaData, Bool_t owner):
fObject(object),
fId(path, runRange, version, subVersion), 
fMetaData(metaData), 
fIsOwner(owner){
// constructor
    fMetaData->SetObjectClassName(fObject->ClassName());
}


//_____________________________________________________________________________
IlcCDBEntry::IlcCDBEntry(TObject* object, const IlcCDBPath& path, 
			Int_t firstRun, Int_t lastRun, 
			IlcCDBMetaData* metaData, Bool_t owner):
fObject(object),
fId(path, firstRun, lastRun, -1, -1), 
fMetaData(metaData), 
fIsOwner(owner){
// constructor
    fMetaData->SetObjectClassName(fObject->ClassName());
}

//_____________________________________________________________________________
IlcCDBEntry::IlcCDBEntry(TObject* object, const IlcCDBPath& path, 
			Int_t firstRun, Int_t lastRun,
			Int_t version, IlcCDBMetaData* metaData,
			Bool_t owner):
fObject(object),
fId(path, firstRun, lastRun, version, -1),
fMetaData(metaData), 
fIsOwner(owner){
// constructor
    fMetaData->SetObjectClassName(fObject->ClassName());
}

//_____________________________________________________________________________
IlcCDBEntry::IlcCDBEntry( TObject* object, const IlcCDBPath& path, 
			Int_t firstRun, Int_t lastRun,
			Int_t version, Int_t subVersion,
			IlcCDBMetaData* metaData, Bool_t owner):
fObject(object),
fId(path, firstRun, lastRun, version, subVersion),
fMetaData(metaData), fIsOwner(owner){
// constructor
    fMetaData->SetObjectClassName(fObject->ClassName());
}

//_____________________________________________________________________________
IlcCDBEntry::~IlcCDBEntry() {
// destructor

	if (fIsOwner) {
		if (fObject) {
			delete fObject;
		}

		if (fMetaData) {
			delete fMetaData;
		}
	}
}

//_____________________________________________________________________________
void IlcCDBEntry::PrintId() const {
 
	IlcInfo(Form("%s",fId.ToString().Data()));

}

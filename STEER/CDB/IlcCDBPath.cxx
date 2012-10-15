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
//  class IlcCDBPath						   //
//  Path string identifying the object:  			   //
//  "level0/level1/level2" 					   //
//  (was: "Detector/DBType/DetSpecType") 		           //
//  (example: "ZDC/Calib/Pedestals") 		         	   //
//                                                                 //
/////////////////////////////////////////////////////////////////////

#include "IlcCDBPath.h"

#include <TObjArray.h>
#include <TObjString.h>
#include <TRegexp.h>

#include "IlcLog.h"

ClassImp(IlcCDBPath)

//_____________________________________________________________________________
IlcCDBPath::IlcCDBPath() :
  TObject(),
  fPath(""),
  fLevel0(""),
  fLevel1(""),
  fLevel2(""),
  fIsValid(kTRUE),
  fIsWildcard(kFALSE)
{
// default constructor

}

//_____________________________________________________________________________
IlcCDBPath::IlcCDBPath(const IlcCDBPath& other):
  TObject(other),
  fPath(other.fPath),
  fLevel0(""),
  fLevel1(""),
  fLevel2(""),
  fIsValid(other.fIsValid),
  fIsWildcard(other.fIsWildcard)
{
// constructor
	Init();
	InitPath();

}

//_____________________________________________________________________________
IlcCDBPath::IlcCDBPath(const char* level0, const char* level1,
	const char* level2):
  TObject(),
  fPath(""),
  fLevel0(level0), 
  fLevel1(level1), 
  fLevel2(level2),
  fIsValid(kTRUE),
  fIsWildcard(kFALSE)
{
// constructor

	fPath += level0;
	fPath += '/';
	fPath += level1;
	fPath += '/';
	fPath += level2;

	if ((IsWord(fLevel0) || fLevel0 == "*")
		&& (IsWord(fLevel1) || fLevel1 == "*")
		&& (IsWord(fLevel2) || fLevel2 == "*")) {

		fIsValid = kTRUE;
	} else {
		fIsValid = kFALSE;
		IlcError(Form("Invalid IlcCDBPath <%s/%s/%s>!", 
			level0, level1, level2));
	}

	Init();
}

//_____________________________________________________________________________
IlcCDBPath::IlcCDBPath(const char* path):
  TObject(),
  fPath(path),
  fLevel0(""),
  fLevel1(""),
  fLevel2(""),
  fIsValid(kTRUE),
  fIsWildcard(kFALSE)
{
// constructor

	Init();
	InitPath();	
}

//_____________________________________________________________________________
IlcCDBPath::IlcCDBPath(const TString& path):
  TObject(),
  fPath(path),
  fLevel0(""),
  fLevel1(""),
  fLevel2(""),
  fIsValid(kTRUE),
  fIsWildcard(kFALSE)
{
	Init();
	InitPath();
}

//_____________________________________________________________________________
void IlcCDBPath::InitPath() {
// sets fLevel0, fLevel1, fLevel2, validity flagss from fPath

	TSubString strippedString = fPath.Strip(TString::kBoth);
	TString aString(strippedString);
	strippedString = aString.Strip(TString::kBoth, '/');

	TObjArray* anArray = TString(strippedString).Tokenize("/");
	Int_t paramCount = anArray->GetEntriesFast();
	
	if (paramCount == 1) {
		if (fPath == "*") {
			fLevel0 = "*";
			fLevel1 = "*";
			fLevel2 = "*";
			
			fIsValid = kTRUE;
		} else {
			fIsValid = kFALSE;
		}

	} else if (paramCount == 2) {
		fLevel0 = ((TObjString*) anArray->At(0))->GetString();
		TString bString =  ((TObjString*) anArray->At(1))->GetString();

		if (IsWord(fLevel0) && bString == "*") {
			fLevel1 = "*";
			fLevel2 = "*";
		
			fIsValid = kTRUE;			

		} else {
			fIsValid = kFALSE;
		}
		
	} else if (paramCount == 3) {
		fLevel0 = ((TObjString*) anArray->At(0))->GetString();
                fLevel1 = ((TObjString*) anArray->At(1))->GetString();
                fLevel2 = ((TObjString*) anArray->At(2))->GetString();

		if ((IsWord(fLevel0) || fLevel0 == "*")
	                && (IsWord(fLevel1) || fLevel1 == "*")
        	        && (IsWord(fLevel2) || fLevel2 == "*")) {
			
			fIsValid = kTRUE;
		} else {
			fIsValid = kFALSE;
		}

	} else {
		fIsValid = kFALSE;

	}
	
	if (!fIsValid) {
		IlcInfo(Form("Invalid IlcCDBPath <%s>!", fPath.Data()));
	} else {	
		fPath = Form("%s/%s/%s", fLevel0.Data(), fLevel1.Data(), fLevel2.Data());
	}
	
	delete anArray;
	
	Init();
}

//_____________________________________________________________________________
IlcCDBPath::~IlcCDBPath() {
// destructor

}

//_____________________________________________________________________________
Bool_t IlcCDBPath::IsWord(const TString& str) {
// check if string is a word

	TRegexp pattern("^[a-zA-Z0-9_.-]+$");

	return str.Contains(pattern);	
}

//_____________________________________________________________________________
void IlcCDBPath::Init() {
// set fIsWildcard flag

	fIsWildcard = fPath.MaybeWildcard();	
}

//_____________________________________________________________________________
Bool_t IlcCDBPath::Level0Comprises(const TString& str) const {
// check if Level0 is wildcard or is equal to str
	
	if (fLevel0 == "*") {
		return kTRUE;
	}

	return fLevel0 == str;
}

//_____________________________________________________________________________
Bool_t IlcCDBPath::Level1Comprises(const TString& str) const {
// check if Level1 is wildcard or is equal to str

	if (fLevel1 == "*") {
		return kTRUE;
	}

	return fLevel1 == str;
}

//_____________________________________________________________________________
Bool_t IlcCDBPath::Level2Comprises(const TString& str) const {
// check if Level2 is wildcard or is equal to str
	
	if (fLevel2 == "*") {
		return kTRUE;
	}

	return fLevel2 == str;
}

//_____________________________________________________________________________
Bool_t IlcCDBPath::Comprises(const IlcCDBPath& other) const {
// check if path is wildcard and comprises other

	return Level0Comprises(other.fLevel0)
		&& Level1Comprises(other.fLevel1)
		&& Level2Comprises(other.fLevel2);
}

//_____________________________________________________________________________
const char* IlcCDBPath::GetLevel(Int_t i) const {
// return level i of the path

	switch (i) {
		case 0:
			return fLevel0.Data();
			break;
		case 1:
			return fLevel1.Data();
			break;
		case 2:
			return fLevel2.Data();
			break;
		default:
			return 0;
	}

}

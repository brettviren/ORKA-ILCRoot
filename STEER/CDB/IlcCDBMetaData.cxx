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
//  class IlcCDBMetaData					   //
//  Set of data describing the object  				   //
//  but not used to identify the object 			   //
//                                                                 //
/////////////////////////////////////////////////////////////////////

#include "IlcCDBMetaData.h"
#include "IlcLog.h"

#include <TObjString.h>
#include <TTimeStamp.h>

ClassImp(IlcCDBMetaData)

//_____________________________________________________________________________
IlcCDBMetaData::IlcCDBMetaData() :
TObject(),
fObjectClassName(""),
fResponsible(""),
fBeamPeriod(0),
fIlcRootVersion(""),
fComment(""),
fProperties()	
{
// default constructor

	fProperties.SetOwner(1);
}

//_____________________________________________________________________________
IlcCDBMetaData::IlcCDBMetaData(const char *responsible, UInt_t beamPeriod,
				const char* ilcrootVersion, const char* comment) :
TObject(),
fObjectClassName(""),
fResponsible(responsible),
fBeamPeriod(beamPeriod),
fIlcRootVersion(ilcrootVersion),
fComment(comment),
fProperties()	
{
// constructor

	fProperties.SetOwner(1);
}

//_____________________________________________________________________________
IlcCDBMetaData::~IlcCDBMetaData() {
// destructor

}

//_____________________________________________________________________________
void IlcCDBMetaData::SetProperty(const char* property, TObject* object) {
// add something to the list of properties

	fProperties.Add(new TObjString(property), object);
}

//_____________________________________________________________________________
TObject* IlcCDBMetaData::GetProperty(const char* property) const {
// get a property specified by its name (property)

	return fProperties.GetValue(property);
}

//_____________________________________________________________________________
Bool_t IlcCDBMetaData::RemoveProperty(const char* property) {
// removes a property

	TObjString objStrProperty(property);
	TObjString* aKey = (TObjString*) fProperties.Remove(&objStrProperty);	

	if (aKey) {
		delete aKey;
		return kTRUE;
	} else {
		return kFALSE;
	}
}

//_____________________________________________________________________________
void IlcCDBMetaData::AddDateToComment() {
// add the date to the comment.
// This method is supposed to be useful if called at the time when the object
// is created, so that later it can more easily be tracked, in particular
// when the date of the file can be lost or when one is interested in the
// date of creation, irrespective of a later copy of it

	TTimeStamp ts(time(0));
	TString comment(GetComment());
	comment += Form("\tDate of production: %s\n", ts.AsString());
	comment.Remove(comment.Last('+'));
	SetComment(comment);

}

//_____________________________________________________________________________
void IlcCDBMetaData::PrintMetaData() {
// print the object's metaData

	TString message;
	if(fObjectClassName != "")
		message += Form("\tObject's class name:	%s\n", fObjectClassName.Data());
	if(fResponsible != "")
		message += Form("\tResponsible:		%s\n", fResponsible.Data());
	if(fBeamPeriod != 0)
		message += Form("\tBeam period:		%d\n", fBeamPeriod);
	if(fIlcRootVersion != "")
		message += Form("\tIlcRoot version:	%s\n", fIlcRootVersion.Data());
	if(fComment != "")
		message += Form("\tComment:		%s\n", fComment.Data());
	if(fProperties.GetEntries() > 0){
		message += "\tProperties key names:";

		TIter iter(fProperties.GetTable());
		TPair* aPair;
		while ((aPair = (TPair*) iter.Next())) {
			message += Form("\t\t%s\n", ((TObjString* ) aPair->Key())->String().Data());
		}
	}
	message += '\n';
	IlcInfo(Form("**** Object's MetaData parameters **** \n%s", message.Data()));
}

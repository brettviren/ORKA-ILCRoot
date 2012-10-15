#ifndef ILC_META_DATA_H
#define ILC_META_DATA_H

/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/////////////////////////////////////////////////////////////////////
//                                                                 //
//  class IlcCDBMetaData					   //
//  Set of data describing the object  				   //
//  but not used to identify the object 			   //
//                                                                 //
/////////////////////////////////////////////////////////////////////

#include <TObject.h>
#include <TMap.h>

class IlcCDBMetaData: public TObject {
	
public:
	IlcCDBMetaData();
	IlcCDBMetaData(const char *responsible, UInt_t beamPeriod=0, const char* ilcrootVersion="", const char* comment="");
	virtual ~IlcCDBMetaData();

	void 		SetObjectClassName(const char* name) 
				{fObjectClassName = name;};
	const char* 	GetObjectClassName() const 
				{return fObjectClassName.Data();};
	
        void 		SetResponsible(const char* yourName) 
				{fResponsible = yourName;};
	const char* 	GetResponsible() const 
				{return fResponsible.Data();};

	void 		SetBeamPeriod(UInt_t period) 
				{fBeamPeriod = period;};
	UInt_t 		GetBeamPeriod() const 
				{return fBeamPeriod;};

	void 		SetIlcRootVersion(const char* version)
				{fIlcRootVersion = version;};
	const char* 	GetIlcRootVersion() const 
				{return fIlcRootVersion.Data();};

	void 		SetComment(const char* comment) 
				{fComment = comment;};
	const char* 	GetComment() const 
				{return fComment.Data();};
	void		AddDateToComment();

	void 		SetProperty(const char* property, TObject* object);
	TObject* 	GetProperty(const char* property) const;
	Bool_t 		RemoveProperty(const char* property);
	
	void PrintMetaData();
	
private:

	TString fObjectClassName; 	// object's class name
	TString fResponsible; 		// object's responsible person
	UInt_t  fBeamPeriod;		// beam period
	TString fIlcRootVersion;	// IlcRoot version
	TString fComment;		// extra comments
	//TList fCalibRuns;		
	
	TMap fProperties;		// list of object specific properties

	ClassDef(IlcCDBMetaData, 1);
};

#endif

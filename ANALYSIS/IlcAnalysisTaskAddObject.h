#ifndef ILCANALYSISTASKADDOBJECT_H
#define ILCANALYSISTASKADDOBJECT_H

/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/*$Id$*/

//*************************************************************************
// Class IlcAnalysisTaskAddObject
// Test Task to add an object to the new ESDfriends file 
//*************************************************************************

class TH1D;

#include "IlcAnalysisTask.h"

class IlcESDInputHandler;
class IlcESDEvent;
class IlcESDfriend;

class IlcAnalysisTaskAddObject : public IlcAnalysisTask
{
 public:

	IlcAnalysisTaskAddObject();
	IlcAnalysisTaskAddObject(const char *name);
	virtual ~IlcAnalysisTaskAddObject();
	// Implementation of interface methods
	virtual void CreateOutputObjects();
	virtual void Exec(Option_t *option);
	virtual void Terminate(Option_t *option);
	virtual void ConnectInputData(Option_t *option = "");
		
 private:
	
	IlcAnalysisTaskAddObject(const IlcAnalysisTaskAddObject &);
	IlcAnalysisTaskAddObject& operator=(const IlcAnalysisTaskAddObject&);
	
	IlcESDEvent  *fESDInput;        // ESD input object
	IlcESDfriend *fESDfriendInput;  // ESD input friend object
	IlcESDInputHandler *fESDhandler;     // Pointer to ESD input handler
	TH1D* fh; // histogram
	
 ClassDef(IlcAnalysisTaskAddObject,1); // IlcAnalysisTask to create an extra object
};

#endif


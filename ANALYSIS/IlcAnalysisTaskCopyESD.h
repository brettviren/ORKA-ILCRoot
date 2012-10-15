#ifndef ILCANALYSISTASKCOPYESD_H
#define ILCANALYSISTASKCOPYESD_H
 
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id$ */

#include "IlcAnalysisTaskFilter.h"
#include "IlcESDtrack.h"
#include "IlcESDEvent.h"
#include "IlcESDfriend.h"

class IlcAnalysisTaskCopyESD : public IlcAnalysisTaskFilter
{
 public:
	IlcAnalysisTaskCopyESD();
	IlcAnalysisTaskCopyESD(const char* name);
	virtual ~IlcAnalysisTaskCopyESD() {;}
	// Implementation of interface methods
	virtual void   UserCreateOutputObjects();
	virtual void   Init();
	virtual void   LocalInit() {Init();}
	virtual void   UserExec(Option_t *option);
	virtual void   Terminate(Option_t *option);
	
	
 private:
	IlcAnalysisTaskCopyESD(const IlcAnalysisTaskCopyESD&);
	IlcAnalysisTaskCopyESD& operator=(const IlcAnalysisTaskCopyESD&);
	
	IlcESDEvent* fESDEvent;
	IlcESDfriend* fESDfriend;

	
	ClassDef(IlcAnalysisTaskCopyESD, 1);
};

#endif

#ifndef ILCANALYSISTASKFILTER_H
#define ILCANALYSISTASKFILTER_H
 
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id$ */

//////////////////////////////////////////////////////////////////////////
//
//  Base class for filtering friends
//
//////////////////////////////////////////////////////////////////////////

#include "IlcAnalysisTask.h"
class IlcVEvent;
class IlcESDEvent;
class IlcESDfriend;
class IlcInputEventHandler;
class IlcESDfriendTrack;

class TTree;

class IlcAnalysisTaskFilter : public IlcAnalysisTask
{
 public:
	IlcAnalysisTaskFilter();
	IlcAnalysisTaskFilter(const char* name);
	IlcAnalysisTaskFilter(const IlcAnalysisTaskFilter& obj);
	IlcAnalysisTaskFilter& operator=(const IlcAnalysisTaskFilter& other);
	virtual ~IlcAnalysisTaskFilter() {;}

	// Implementation of interface methods
	virtual void ConnectInputData(Option_t *option = "");
	virtual void CreateOutputObjects();
	virtual void Exec(Option_t* option);
	virtual void SetDebugLevel(Int_t level) {fDebug = level;}
	virtual void Init() {;}
	virtual void UserCreateOutputObjects()  {;}
	virtual void UserExec(Option_t* /*option*/) {;}

	// To be implemented by user
	virtual Bool_t UserSelectESDfriendForCurrentEvent(){return kTRUE;}

	// Getters
	virtual Int_t         DebugLevel()  {return fDebug;     }
	virtual IlcVEvent*    InputEvent()  {return fInputEvent;}
	virtual IlcESDfriend* InputFriend() {return fInputESDfriend;}
	virtual IlcESDfriend* ESDfriend()   {return fOutputESDfriend; }
	virtual TTree*        OutputTree()  {return fTreeEF;     }
	virtual Long64_t      Entry()       {return fEntry;     }
	virtual const char*   CurrentFileName();

	// To add/skip a friend track
	void AddFriendTrackAt(IlcESDfriendTrack* t, Int_t index);
	void SkipFriendTrackAt(Int_t index);

 protected:
	Int_t                 fDebug;           //  Debug flag
	Int_t                 fEntry;           //  Current entry in the chain
	IlcVEvent*            fInputEvent;      //! VEvent Input
	IlcInputEventHandler* fInputHandler;    //! Input Handler
	IlcESDfriend*         fOutputESDfriend; //! ESD friend out 
	TTree*                fTreeEF;          //  ESD friend output Tree
	IlcESDfriend*         fInputESDfriend;  //! ESD friend input
	
	ClassDef(IlcAnalysisTaskFilter, 3); // Analysis task for filtering friends
};

#endif

#ifndef ILCESDHANDLER_H
#define ILCESDHANDLER_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id$ */

//-------------------------------------------------------------------------
//
//     Implementation of the Event Handler Interface for ESD
//
//-------------------------------------------------------------------------

#include "IlcVEventHandler.h"

class IlcESDEvent;
class IlcESDfriend;
class TFile;
class TTree;

class IlcESDHandler : public IlcVEventHandler {
    
 public:
	IlcESDHandler();
	IlcESDHandler(const char* name, const char* title);
	virtual ~IlcESDHandler();
	virtual void SetOutputFileName(const char* fname){fFileName = fname;}
	virtual const char* GetOutputFileName(){return fFileName.Data();}
	virtual Bool_t Init(Option_t* option);
	virtual Bool_t Init(TTree* /*tree*/, Option_t* /*option*/)  {return kTRUE;}
	virtual Bool_t GetEntry() {return kTRUE;}
	virtual Bool_t BeginEvent(Long64_t /*entry*/){fIsEventSelectedForFriends = kFALSE; return kTRUE;}
	virtual Bool_t Notify() {return IlcVEventHandler::Notify(); };
	virtual Bool_t Notify(const char * /* path */) {return kTRUE;}
	virtual Bool_t FinishEvent();
	virtual Bool_t Terminate();
	virtual Bool_t TerminateIO();
	
	IlcESDfriend* GetESDfriend()  {return fesdf;}
	virtual TTree* GetTree() const {return fTreeEF;}
	void FillTree();
	void SetInputTree(TTree* /*tree*/) {;}
	void SelectEventForFriends() {fIsEventSelectedForFriends = kTRUE;}

 private:

	IlcESDHandler(const IlcESDHandler&);             // Not implemented
	IlcESDHandler& operator=(const IlcESDHandler&);  // Not implemented
	
	IlcESDfriend* fesdf;    //! Pointer to the ESD friend
	TTree* fTreeEF;         //! Output tree for friends
	TFile* fFileEF;         //! Output file for friends
	TString fFileName;      //! Output file name for friends
	Bool_t fIsEventSelectedForFriends; //! flag to indicate if the event was selected to have the friends kept 

    ClassDef(IlcESDHandler, 3)
};
#endif

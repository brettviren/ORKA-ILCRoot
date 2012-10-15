  // -*- mode: C++ -*-
#ifndef ILCMCEVENTHANDLER_H
#define ILCMCEVENTHANDLER_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */


/* $Id: IlcMCEventHandler.h 58336 2012-08-28 15:07:04Z hristov $ */

//-------------------------------------------------------------------------
//                          Class IlcMCEvent
// This class gives access to MC truth during the analysis.
// Monte Carlo truth is contained in the kinematics tree (produced particles) and 
// the tree of reference hits.
//      
// Origin: Andreas Morsch, CERN, andreas.morsch@cern.ch 
//-------------------------------------------------------------------------
#include "IlcInputEventHandler.h"
#include "IlcHeader.h"
#include <TExMap.h>

class TFile;
class TTree;
class TList;

class TParticle;
class TString;
class TClonesArray;
class TDirectoryFile;

class IlcMCEvent;



class IlcMCEventHandler : public IlcInputEventHandler
{
public:

    enum PreReadMode_t {kNoPreRead = 0, kLmPreRead = 1, kHmPreRead = 2};

    IlcMCEventHandler();
    IlcMCEventHandler(const char* name, const char* title);
    virtual ~IlcMCEventHandler();
    virtual void         SetOutputFileName(const char* /* fname */) {;}
    virtual const char*  GetOutputFileName() {return 0;}
    virtual void         SetInputPath(const char* fname); 
    virtual void         SetInputTree(TTree* /*tree*/) {;}
    virtual TString*     GetInputPath() const {return fPathName;}
    virtual Bool_t       Init(Option_t* opt);
    virtual Bool_t       GetEntry() {return kTRUE;}
    virtual Bool_t       InitIO(Option_t* opt) {return Init(opt);};
    virtual Bool_t       Init(TTree* /*tree*/, Option_t* /*opt*/) {return kTRUE;}
    virtual Bool_t       BeginEvent(Long64_t entry);
    virtual Bool_t       Notify() { return IlcVEventHandler::Notify(); };
    virtual Bool_t       Notify(const char* path);
    virtual Bool_t       FinishEvent();
    virtual Bool_t       Terminate();
    virtual Bool_t       TerminateIO();
    virtual void         ResetIO();
    virtual Bool_t       LoadEvent(Int_t iev);
    virtual void         SetReadTR(Bool_t flag) { Changed(); fReadTR = flag; }
    virtual void         AddSubsidiaryHandler(IlcMCEventHandler* handler);
    virtual void         SetNumberOfEventsInContainer(Int_t nev) {fEventsInContainer = nev;}
    virtual void         SetPreReadMode(PreReadMode_t mode) {Changed(); fPreReadMode = mode;}
    //
    IlcMCEvent* MCEvent() const {return fMCEvent;}
    TTree*      TreeTR()  const {return fTreeTR;}
    TTree*      TreeK()   const {return fTreeK;}
    virtual TTree*      GetTree() const {return fTreeE;}
    Int_t       GetParticleAndTR(Int_t i, TParticle*& particle, TClonesArray*& trefs);
    void        DrawCheck(Int_t i, Int_t search=0);
    Bool_t      InitOk() const {return fInitOk;}
    // Label manipulation
    void   SelectParticle(Int_t i);
    Bool_t IsParticleSelected(Int_t i);
    void   CreateLabelMap();
    Int_t  GetNewLabel(Int_t i);

private:
    Bool_t      OpenFile(Int_t i);
    void  VerifySelectedParticles();
    IlcMCEventHandler(const IlcMCEventHandler& handler);             
    IlcMCEventHandler& operator=(const IlcMCEventHandler& handler);  
private:
    IlcMCEvent            *fMCEvent;            //! MC Event
    TFile                 *fFileE;              //! File with TreeE
    TFile                 *fFileK;              //! File with TreeK
    TFile                 *fFileTR;             //! File with TreeTR
    TTree                 *fTreeE;              //! TreeE  (Event Headers)
    TTree                 *fTreeK;              //! TreeK  (kinematics tree)
    TTree                 *fTreeTR;             //! TreeTR (track references tree)
    TDirectoryFile        *fDirK;               //! Directory for Kine Tree
    TDirectoryFile        *fDirTR;              //! Directory for TR Tree
    TExMap                 fParticleSelected;   //! List of selected MC particles for t
    TExMap                 fLabelMap;           //! Stores the Map of MC (ESDLabel,AODlabel)  
    Int_t                  fNEvent;             //! Number of events
    Int_t                  fEvent;              //! Current event
    TString               *fPathName;           //! Input file path 
    const Char_t          *fkExtension;         //! File name extension 
    Int_t                  fFileNumber;         //! Input file number
    Int_t                  fEventsPerFile;      //! Number of events per file
    Bool_t                 fReadTR;             // determines if TR shall be read
    Bool_t                 fInitOk;             // Initialization ok
    TList                 *fSubsidiaryHandlers; //! List of subsidiary MC handlers (for example for Background)
    Int_t                  fEventsInContainer;  //! Number of events in container class
    PreReadMode_t          fPreReadMode;        //! Pre reading mode
    
    ClassDef(IlcMCEventHandler,1)  //MC Truth EventHandler class
};
#endif 


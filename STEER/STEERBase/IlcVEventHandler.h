#ifndef ILCVEVENTHANDLER_H
#define ILCVEVENTHANDLER_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcVEventHandler.h 58336 2012-08-28 15:07:04Z hristov $ */

//-------------------------------------------------------------------------
//     Event Handler base class
//     Author: Andreas Morsch, CERN
//-------------------------------------------------------------------------

#include <TNamed.h>

class TTree;

class IlcVEventHandler : public TNamed {

 public:
enum EEventHandlerFlags {
   kHandlerLocked       = BIT(14)
};
    IlcVEventHandler();
    IlcVEventHandler(const char* name, const char* title);
    virtual ~IlcVEventHandler();
    // Handled tree
    virtual TTree       *GetTree() const { return NULL; }
    virtual Option_t    *GetDataType() const { return NULL; }
    virtual void         SetFillAOD(Bool_t) {}
    virtual void         SetFillExtension(Bool_t) {}
    // Output
    virtual void         SetOutputFileName(const char* fname)   = 0;
    virtual const char*  GetOutputFileName()              = 0;
    // Extra outputs as a string separated by commas
    virtual const char*  GetExtraOutputs() const {return NULL;}
    // Input
    virtual void         SetInputTree(TTree* tree)        = 0;
    // Steering 
    virtual Bool_t       Init(Option_t* opt)              = 0;
    virtual Bool_t       Init(TTree* tree, Option_t* opt) = 0;
    virtual Bool_t       BeginEvent(Long64_t entry)       = 0;
    virtual Bool_t       GetEntry()                       = 0;
    virtual Bool_t       Notify(const char *path)         = 0;
    virtual Bool_t       FinishEvent()                    = 0;
    virtual Bool_t       Terminate()                      = 0;
    virtual Bool_t       TerminateIO()                    = 0;
    //
    virtual Bool_t       Notify() { return TNamed::Notify(); };
    // Security
    Bool_t               IsLocked() const {return TObject::TestBit(kHandlerLocked);}
    void                 Lock();
    void                 UnLock();
    void                 Changed();
 private :
  ClassDef(IlcVEventHandler, 1);
};

#endif

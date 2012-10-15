#ifndef ILCANALYSISTASKTAGCREATOR_H
#define ILCANALYSISTASKTAGCREATOR_H
 
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id$ */

#include "IlcAnalysisTaskSE.h"
class IlcRunTag;
class IlcAODTagCreator;
class TTree;


class IlcAnalysisTaskTagCreator : public IlcAnalysisTaskSE
{
 public:
    IlcAnalysisTaskTagCreator();
    IlcAnalysisTaskTagCreator(const char* name);
    virtual ~IlcAnalysisTaskTagCreator() {;}
    // Implementation of interface methods
    virtual void   ConnectInputData(Option_t *option = "");
    virtual void   UserCreateOutputObjects();
    virtual void   Init();
    virtual void   LocalInit() {Init();}
    virtual void   UserExec(Option_t *option);
    virtual Bool_t Notify();
    virtual void   Terminate(Option_t *option);
    virtual void   FinishTaskOutput();
 private:
    IlcAnalysisTaskTagCreator(const IlcAnalysisTaskTagCreator&);
    IlcAnalysisTaskTagCreator& operator=(const IlcAnalysisTaskTagCreator&);
    void GetGUID(TString &guid);
    
 private:
    Bool_t                   fCreateTags;             //  Flag for tag creation
    Bool_t                   fFirstFile;              //! To flag the first file   
    IlcRunTag               *fRunTag;                 //! Pointer to run tag
    TTree                   *fTreeT;                  //! tree for  aod tags
    IlcAODTagCreator        *fTagCreator;             //! The tag creator
    TString                  fAODFileName;            //! Name of the AOD file
    TString                  fGUID;                   //! GUID of the AOD file
    ClassDef(IlcAnalysisTaskTagCreator, 1); // Analysis task for standard ESD filtering
};
 
#endif

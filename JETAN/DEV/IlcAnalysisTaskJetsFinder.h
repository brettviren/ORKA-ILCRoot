#ifndef ILCANALYSISTASKJETSFINDER_H
#define ILCANALYSISTASKJETSFINDER_H
 
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

//----------------------------------------------------------------
// Analysis task for interfacing the jet finders with the analysis framework
//
// Author: magali.estienne@subatech.in2p3.fr 
//	   alexandre.shabetai@cern.ch
//----------------------------------------------------------------
 
#include "IlcAnalysisTaskSE.h"
#include "IlcJetCalTrk.h"

class IlcJetFinder;
class IlcJetHistos;
class IlcAODExtension;
class TTree;
class TString;

class IlcAnalysisTaskJetsFinder : public IlcAnalysisTaskSE
{
 public:
  IlcAnalysisTaskJetsFinder();
  IlcAnalysisTaskJetsFinder(const char* name);
  virtual             ~IlcAnalysisTaskJetsFinder();
  // Implementation of interface methods
  virtual void        UserCreateOutputObjects();
  virtual void        Init();
  virtual Bool_t      Notify();
  virtual void        LocalInit() {Init();}
  virtual void        ConnectInputData(Option_t *);
  virtual void        UserExec(Option_t *option);
  virtual void        SetConfigFile(const char *c) {fConfigFile = c;}
  virtual void        SetJetFinder(IlcJetFinder *finder) {fJetFinder = finder;}
  virtual void        SetNonStdBranch(const char *c){fNonStdBranch = c;}
  virtual const char* GetNonStdBranch(){return fNonStdBranch.Data();}
  virtual void        SetNonStdOutputFile(const char *c){fNonStdFile = c;}
  virtual void        SetBookAODBackground(Bool_t b){fUseAODBackground = b;}
  virtual void        Terminate(Option_t *option);
  virtual void        SetFilterPt(Float_t f){fFilterPt = f;}
    
  IlcJetFinder*       GetJetFinder() {return fJetFinder;}

 private:
  IlcAnalysisTaskJetsFinder(const IlcAnalysisTaskJetsFinder &det);
  IlcAnalysisTaskJetsFinder &operator=(const IlcAnalysisTaskJetsFinder &det);
    
  TString             fConfigFile;        //  The name of the ConfigFile
  TString             fNonStdBranch;      //  The name of the non-std branch name
  TString             fNonStdFile;        //  The optional name of the output file the non-std brnach is written to
  IlcJetFinder*       fJetFinder;         //  Pointer to the jet finder 
  IlcJetHistos*       fHistos;            //! Histogram manager class
  IlcAODExtension*    fAODExtension;      //  AOD extension in case we write a non-sdt brnach to a separate file and the aod is standard
  TList*              fListOfHistos;      //! Output list of histograms
  TTree*              fTreeI;             //! Input Tree 
  IlcJetCalTrkEvent*  fEvent;             //! Pointer to jet input objects
  Bool_t              fUseAODBackground;  //  Decide wether we book the backround branch or not
  Float_t             fFilterPt;          //  Use this as a switch for writing the AOD, minium p_T of leading jet

  ClassDef(IlcAnalysisTaskJetsFinder, 1)  // Jet Finder Analysis task

};
 
#endif

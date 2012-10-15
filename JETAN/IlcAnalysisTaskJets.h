//
// Taks for jet finding in ILC 
//
#ifndef ILCANALYSISTASKJETS_H
#define ILCANALYSISTASKJETS_H
 
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */
 
#include "IlcAnalysisTaskSE.h"


class IlcJetFinder;
class IlcESDEvent;
class IlcAODEvent;
class IlcJetHistos;
class IlcAODExtension;
class TTree;
class TChain;
class TString;



class IlcAnalysisTaskJets : public IlcAnalysisTaskSE
{
 public:
    IlcAnalysisTaskJets();
    IlcAnalysisTaskJets(const char* name);
    IlcAnalysisTaskJets(const char* name, TChain* chain);
    virtual ~IlcAnalysisTaskJets() {;}
    // Implementation of interface methods
    virtual void UserCreateOutputObjects();
    virtual void Init();
    virtual void LocalInit() {Init();}
    virtual void UserExec(Option_t *option);
    virtual void SetConfigFile(const char *c) {fConfigFile = c;}
    virtual void SetJetFinder(IlcJetFinder *finder) {fJetFinder = finder;}
    virtual void SetNonStdBranch(const char *c){fNonStdBranch = c;}
    virtual const char* GetNonStdBranch(){return fNonStdBranch.Data();}
    virtual void SetNonStdOutputFile(const char *c){fNonStdFile = c;}
    virtual void SetBookAODBackground(Bool_t b){fUseAODBackground = b;}
    virtual void Terminate(Option_t *option);
    virtual void ReadAODFromOutput() {fReadAODFromOutput = kTRUE;}
    virtual void SetFilterPt(Float_t f){fFilterPt = f;}
    
 private:
  IlcAnalysisTaskJets(const IlcAnalysisTaskJets &det);
  IlcAnalysisTaskJets &operator=(const IlcAnalysisTaskJets &det);
    
 private:
  TString       fConfigFile;        // the name of the ConfigFile
  TString       fNonStdBranch;      // the name of the non-std branch name
  TString       fNonStdFile;        // The optional name of the output file the non-std brnach is written to
  IlcJetFinder* fJetFinder;         //  Pointer to the jet finder 
  IlcJetHistos* fHistos;            //  Histogram manager class
  IlcAODExtension* fAODExtension;   //  AOD extension we in case we write a non-sdt brnach to a separate file and the aod is standard
  TList*        fListOfHistos;      //  Output list of histograms
  TChain*       fChain;             //  Chain 
  Int_t         fOpt;               //  Detector configuration used
  Bool_t        fReadAODFromOutput; //  Force reading of the AOD from the output
  Bool_t        fUseAODBackground;  // decide wether we book the backround branch or not 
  Float_t       fFilterPt;          // use this as a switch for writing the AOD, minium p_T of leading jet   
  ClassDef(IlcAnalysisTaskJets, 7); // Analysis task for standard jet analysis
};
 
#endif

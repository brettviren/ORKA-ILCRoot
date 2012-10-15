#ifndef ILCANALYSISTASKJETSREADER_H
#define ILCANALYSISTASKJETSREADER_H
 
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

//----------------------------------------------------------------
// Analysis task for interfacing the jet reader with the analysis framework
//
// Authors: magali.estienne@subatech.in2p3.fr 
//	    alexandre.shabetai@cern.ch
//----------------------------------------------------------------
 
#include "IlcAnalysisTaskSE.h"

class IlcJetReader;
class TTree;
class TString;
class IlcJetCalTrkEvent;

class IlcAnalysisTaskJetsReader : public IlcAnalysisTaskSE
{
 public:
  IlcAnalysisTaskJetsReader();
  IlcAnalysisTaskJetsReader(const char* name);
  virtual               ~IlcAnalysisTaskJetsReader();
  // Implementation of interface methods
  virtual void          UserCreateOutputObjects();
  virtual void          Init();
  virtual void          LocalInit() {Init();}
  virtual void          UserExec(Option_t *option);
  virtual void          SetConfigFile(const char *c) {fConfigFile = c;}
  virtual void          SetJetReader(IlcJetReader *reader) {fJetReader = reader;}
  virtual void          Terminate(Option_t *option);
  virtual void          ReadAODFromOutput() {fReadAODFromOutput = kTRUE;}
  virtual IlcJetReader* GetJetReader() {return fJetReader;}
    
 private:
  IlcAnalysisTaskJetsReader(const IlcAnalysisTaskJetsReader& rd);
  IlcAnalysisTaskJetsReader& operator = (const IlcAnalysisTaskJetsReader& rd);
  TString               fConfigFile;        //  The name of the ConfigFile
  IlcJetReader*         fJetReader;         //  Pointer to the jet finder
  Bool_t                fReadAODFromOutput; //  Force reading of the AOD from the output 
  IlcJetCalTrkEvent*    fReaderEvent;       //! Pointer to IlcJetCalTrkEvent
  TTree*	        fExchangeTree;      //! Tree of IlcJetCalTrkEvent
 
  ClassDef(IlcAnalysisTaskJetsReader, 1)    // Jet reader Analysis task

};
#endif

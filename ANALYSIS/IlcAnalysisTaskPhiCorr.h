#ifndef ILCANALYSISTASKPHICORR_cxx
#define ILCANALYSISTASKPHICORR_cxx
/*
 Simple use case for mixed event analysis
 based on ESD or AOD
 Delta_phi correlation analysis is performed on charged tracks 
 for same and mixed events
 Author: andreas.morsch@cern.ch 
*/


class TH1F;
class TList;
class IlcESDEvent;

#include "IlcAnalysisTaskME.h"
#include "IlcMixedEvent.h"

class IlcAnalysisTaskPhiCorr : public IlcAnalysisTaskME {
 public:
  IlcAnalysisTaskPhiCorr(const char *name = "IlcAnalysisTaskPt");
  virtual ~IlcAnalysisTaskPhiCorr() {}
  
  virtual void   UserCreateOutputObjects();
  virtual void   UserExec(Option_t *option);
  virtual void   Terminate(Option_t *);
  
 private:
  TList       *fHists;        // List with histos
  TH1F        *fHistDphiCO;   // Pt spectrum
  TH1F        *fHistDphiUC;   // Pt spectrum
  IlcMixedEvent fMixedEvent;  // Mixed event
  
  IlcAnalysisTaskPhiCorr(const IlcAnalysisTaskPhiCorr&); // not implemented
  IlcAnalysisTaskPhiCorr& operator=(const IlcAnalysisTaskPhiCorr&); // not implemented
  
  ClassDef(IlcAnalysisTaskPhiCorr, 1); // example of analysis
};

#endif

#ifndef ILCANALYSISTASKDIJETS_H
#define ILCANALYSISTASKDIJETS_H
 
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

#include "IlcAnalysisTaskSE.h"
#include <TList.h>
#include <TH1F.h>
#include <TH2F.h>

class IlcAnalysisTaskDiJets : public IlcAnalysisTaskSE
{
 public:
    IlcAnalysisTaskDiJets();
    IlcAnalysisTaskDiJets(const char* name);
    virtual ~IlcAnalysisTaskDiJets() {;}

    // Implementation of interface methods
    virtual void UserCreateOutputObjects();
    virtual void Init();
    virtual void LocalInit() {Init();}
    virtual void UserExec(Option_t *option);
    virtual void Terminate(Option_t *option);

    void SetAODInput(Bool_t b)   {fUseAODInput = b;}
    void SetFillAOD(Bool_t fill) { fFillAOD=fill; }
    void SetJetBranch(const Char_t *br){ fJetBranch=br; }

 private:
  IlcAnalysisTaskDiJets(const IlcAnalysisTaskDiJets &det);
  IlcAnalysisTaskDiJets &operator=(const IlcAnalysisTaskDiJets &det);

 private:
  TClonesArray* fDiJets;    // Array of dijets
  TClonesArray* fDiJetsIn;  // Array of dijets

  Bool_t        fUseAODInput; // read jets from input AOD
  Bool_t        fFillAOD;   // option to fill AOD branch
  TString       fJetBranch; // jet branch to read

  IlcAODEvent  *fAOD;       // AOD event
  TList        *fHistList;  // Output list

  TH1F         *fH1DeltaPt;  // Pt difference
  TH1F         *fH1DeltaPhi; // delta phi plot in (0,pi)
  TH1F         *fH1PhiImbal; // phi imbalance (-pi,pi)
  TH1F         *fH1Asym;     // asymmetry of the dijet
  TH2F         *fH2Pt2vsPt1; // scatter plot with the two jets' Pt
  TH2F         *fH2DifvsSum; // Pt difference vs Pt sum

  ClassDef(IlcAnalysisTaskDiJets, 2); // Analysis task for standard dijet analysis
};

#endif

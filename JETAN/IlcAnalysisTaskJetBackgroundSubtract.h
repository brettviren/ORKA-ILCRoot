#ifndef ILCANALYSISTASKJETBACKGROUNDSUBTRACT_H
#define ILCANALYSISTASKJETBACKGROUNDSUBTRACT_H
 
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

// **************************************
// task used for background subtration of all jets found with clustering algos
// *******************************************

#include  "IlcAnalysisTaskSE.h"
#include  "TObjString.h"
#include  "TString.h"


////////////////
class IlcJetHeader;
class IlcESDEvent;
class IlcAODEvent;
class IlcAODExtension;
class IlcAODJet;
class IlcAODJetEventBackground;
class IlcJetFinder;
class TList;
class TChain;
class TH2F;
class TH1F;
class TH3F;
class TProfile;
class TRandom3;
class TRefArray;
class TObjArray;

class IlcAnalysisTaskJetBackgroundSubtract : public IlcAnalysisTaskSE
{
 public:
    IlcAnalysisTaskJetBackgroundSubtract();
    IlcAnalysisTaskJetBackgroundSubtract(const char* name);
    virtual ~IlcAnalysisTaskJetBackgroundSubtract();
    // Implementation of interface methods
    virtual void UserCreateOutputObjects();
    virtual void Init();
    virtual void LocalInit() { Init(); }
    virtual void UserExec(Option_t *option);
    virtual void Terminate(Option_t *option);
    virtual Bool_t Notify();

    // Task specific methods...
    virtual void   AddJetBranch(const char* c); 
    virtual void   SetSubtractionMethod(Int_t i){fSubtraction = i;}
    virtual Int_t  GetSubtractionMethod(){return fSubtraction;}
    virtual void   SetKeepJets(Bool_t b = kTRUE){fKeepJets = b;}
    virtual void   SetBackgroundBranch(char* c){fBackgroundBranch = c;}  
    virtual void   SetNonStdOutputFile(char* c){fNonStdFile = c;}  
    virtual void   SetToReplace(char* c){fReplaceString1 = c;}  
    const char*    GetToReplace(){return fReplaceString1.Data();}  
    virtual void   SetReplacementMask(char* c){fReplaceString2 = c;}  
    const char*    GetReplacementMask(){return fReplaceString2.Data();}  
   
    enum {kNoSubtract = 0,kArea,k4Area,kRhoRecalc,kRhoRC};
 
 private:

    

    IlcAnalysisTaskJetBackgroundSubtract(const IlcAnalysisTaskJetBackgroundSubtract&);
    IlcAnalysisTaskJetBackgroundSubtract& operator=(const IlcAnalysisTaskJetBackgroundSubtract&);
    Bool_t RescaleJetMomentum(IlcAODJet *jet,Float_t pT);
    Bool_t RescaleJet4vector(IlcAODJet *jet,TLorentzVector backgroundv);
    Int_t  MultFromJetRefs(TClonesArray* jets);
    Double_t RecalcRho(TClonesArray* fbkgclusters,Double_t meanarea);
    Double_t RhoRC(TClonesArray* fbkgclustersRC);
    void ResetOutJets();
    void PrintAODContents();

    IlcAODEvent     *fAODOut;                // ! where we take the jets from and they are modified
    IlcAODEvent     *fAODIn;                 // ! where we may take the background from, only in case we do not find it in the output
    IlcAODExtension *fAODExtension;          // ! where we take the jets from can be input or output AOD
    TObjArray       *fJBArray;               // Array that stores the name of all jet branches to be subtracted   
    TString         fBackgroundBranch;       // name of the branch used for background subtraction
    //
    TString         fNonStdFile;        // The optional name of the output file the non-std brnach is written to
    TString         fReplaceString1;     // To construct the new output name  
    TString         fReplaceString2;     // To construct the new output name   
    Int_t           fSubtraction;       // Parameter for subtraction mode 
    Bool_t          fKeepJets;          // keeps the jets with negative p_t rescaled to 0.1 GeV
    TList *fInJetArrayList; //! transient list to make ease the handling of input jets
    TList *fOutJetArrayList; //! transient list to make ease the reset of output jets

    TH2F*         fh2CentvsRho;   //! centrality vs background density  
    TH2F*         fh2CentvsSigma;   //! centrality vs background sigma  
    TH2F*         fh2MultvsRho;   //! centrality vs background density  
    TH2F*         fh2MultvsSigma;   //! centrality vs background sigma  
    TH2F*         fh2ShiftEta;   //! extended correction Eta
    TH2F*         fh2ShiftPhi;   //! extended correction Phi
    TH2F*         fh2ShiftEtaLeading;   //! extended correction Eta leading jet
    TH2F*         fh2ShiftPhiLeading;   //! extended correction Phi leading jet

 
    TList *fHistList; //! the histograms output list
   
    ClassDef(IlcAnalysisTaskJetBackgroundSubtract, 6) 
};
 
#endif

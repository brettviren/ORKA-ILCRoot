#ifndef IlcAnalysisTaskCluster_cxx
#define IlcAnalysisTaskCluster_cxx
 

class TH1F;
class TH2F;
class TH3F;
class TList;
class TNtuple;

class IlcESDEvent;
class IlcESDtrack;
class IlcESDtrackCuts;


#include "IlcAnalysisTaskSE.h"
#include "TFile.h"
#include "TNtuple.h"

class IlcAnalysisTaskCluster : public IlcAnalysisTaskSE {
 public:
  IlcAnalysisTaskCluster(const char *name = "IlcAnalysisTaskCluster");
  virtual ~IlcAnalysisTaskCluster() {}
  
  virtual void   UserCreateOutputObjects();
  virtual void   UserExec(Option_t *option);
  virtual void   Terminate(Option_t *);
  virtual void   SetTrackType(Int_t type) {fTrackType = type;}  
  
  
  virtual void   SetCuts(IlcESDtrackCuts* cuts)
     {fCuts = cuts;}

  virtual void   SetFieldOn(Bool_t b = kTRUE){fFieldOn = b;} 

  
 private:

  Int_t       fTrackType;       // setter for track types (not implemented yet)
  Bool_t      fFieldOn;         // magnetic field

  TList       *fHists;          // List of histos

  //old
  TH1F        * fHistRECpt;      // pt 
  TH1F        * fITSncl;         // number of clusters in ITS (from global track)
  TH1F        * fTPCncl;         // number of clusters in TPC ("")
  TH1F        * fTPCnclSA;       // number of clusters in TPC from TPC track
  TH1F        * fTRDncl;         // number of clusters in TRD

  TProfile    * pEtaITSncl;      // average ncl vs eta
  TProfile    * pEtaTPCncl;      // average ncl vs eta
  TProfile    * pEtaTPCnclR;     // average ncl vs eta
  TProfile    * pEtaTRDncl;      // average ncl vs eta

  TProfile    * pPhiITSncl;      // average ncl vs phi
  TProfile    * pPhiTPCncl;      // average ncl vs phi
  TProfile    * pPhiTPCnclR;     // average ncl vs phi
  TProfile    * pPhiTRDncl;      // average ncl vs phi

  TProfile    * pPtITSncl;       // average ncl vs pt
  TProfile    * pPtTPCncl;       // average ncl vs pt
  TProfile    * pPtTPCnclR;      // average ncl vs pt
  TProfile    * pPtTRDncl;       // average ncl vs pt

  TH1F        * fITSlayer;       //hits per ITS layer
  TH2F        * fITSlayerPhi;    //hits per ITS layer vs phi
  TH2F        * fITSlayerEta;    //hits per ITS layer vs eta



  IlcESDtrackCuts* fCuts;        // List of cuts

 


  IlcAnalysisTaskCluster(const IlcAnalysisTaskCluster&); // not implemented
  IlcAnalysisTaskCluster& operator=(const IlcAnalysisTaskCluster&); // not implemented
  
  ClassDef(IlcAnalysisTaskCluster, 1); // Basic QA exploiting cluster numbers
};

#endif

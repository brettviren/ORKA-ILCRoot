#ifndef IlcTARGETSTEERPID_H
#define IlcTARGETSTEERPID_H
////////////////////////////////////////////////////////////////////////
//Steering lass for PID in TARGET                                          //
//The PID is based on the likelihood of all the four TARGET' layers,       //
//without using the truncated mean for the dE/dx. The response          //
//functions for each layer are convoluted Landau-Gaussian functions.    //                  
//Origin: Elena Bruna bruna@to.infn.it, Massimo Masera masera@to.infn.it//
////////////////////////////////////////////////////////////////////////

#include <TObject.h>
#include <Riostream.h>
#include <TF1.h>
#include <TTree.h>
#include <TFile.h>
#include "IlcTARGETPidParItem.h"


class IlcTARGETSteerPid : public TObject{

 public:
  IlcTARGETSteerPid();
  virtual ~IlcTARGETSteerPid();
  void InitLayer(TString fileTARGET="$ILC_ROOT/TARGET/farrfitits_clu6_new5.root",TString fileFitPar="$ILC_ROOT/TARGET/ffitparams_new5.root");
  
  IlcTARGETPidParItem* GetItemLayer(Int_t nolay,Float_t mom);
  void GetParFitLayer(Int_t nolay,Float_t mom,Double_t *parp,Double_t *park,Double_t *parpi);
  static void GetLangausProPars(Float_t mom,Double_t *parfit0,Double_t *parfit1,Double_t *parfit3,Double_t *par);
  static void GetLangausKaoPars(Float_t mom,Double_t *parfit0,Double_t *parfit1,Double_t *parfit3,Double_t *par);
  static void GetLangausPiPars(Float_t mom,Double_t *parfit0,Double_t *parfit1,Double_t *parfit3,Double_t *par);

 
 private:

  IlcTARGETPidParItem* Item(TClonesArray *Vect,Float_t mom);
  IlcTARGETSteerPid(const IlcTARGETSteerPid &ob); // copy constructor
  IlcTARGETSteerPid& operator=(const IlcTARGETSteerPid & /* source */); // ass. op.


  TFile *fClonarr2;              // file of TClones Array
  TClonesArray *fVect2;          // TClonesArray fVect
  TClonesArray *fVect2lay1;     // TClonesArray for layer 1 (SDD1)
  TClonesArray *fVect2lay2;     // TClonesArray for layer 2 (SDD2)
  TClonesArray *fVect2lay3;     // TClonesArray for layer 3 (SSD1)
  TClonesArray *fVect2lay4;     // TClonesArray for layer 4 (SSD2)
  TTree* fFitTree;               // tree
  IlcTARGETPidParItem *fItem;       //element of TClonesArray of interest
  Float_t fPCenter;              //value for center
  Float_t fPWidth;               //value for width
  Double_t fBuff[52];            //array for
  
 
  ClassDef(IlcTARGETSteerPid,1);
 

};
#endif

#ifndef ILCPHOSTENDERSUPPLY_H
#define ILCPHOSTENDERSUPPLY_H

/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

////////////////////////////////////////////////////////////////////////
//                                                                    //
//  PHOS tender, apply corrections to PHOS clusters                   //
//  and do track matching                                             //
//  Author : Dmitri Peressounko (RRC KI)                              //
//                                                                    //
////////////////////////////////////////////////////////////////////////

#include <IlcTenderSupply.h>

class TVector3;
class IlcPHOSGeometry;
class IlcPHOSCalibData ;
class TH2I ;

class IlcPHOSTenderSupply: public IlcTenderSupply {
  
public:
  IlcPHOSTenderSupply();
  IlcPHOSTenderSupply(const char *name, const IlcTender *tender=NULL);
  virtual ~IlcPHOSTenderSupply();

  virtual void   Init(){}
  virtual void   ProcessEvent();
  
  void  SetNonlinearityVersion(const char * ver="Gustavo2005"){fNonlinearityVersion=ver;}
  void  SetNonlinearityParams(Int_t n, const Double_t * par){
            if(n>10){printf("Only 10 parameters allowed \n"); return ;}
            for(Int_t i=0;i<n;i++)fNonlinearityParams[i]=par[i]; }
  void  SetReconstructionPass(Int_t ipass=2){fRecoPass=ipass;}
  
  //If you want to override automatic choise of bad maps and calibration
  void ForceUsingBadMap(const char * filename="alien:///ilc/cern.ch/user/p/prsnko/BadMaps/BadMap_LHC10b.root") ;
  void ForceUsingCalibration(const char * filename="alien:///ilc/cern.ch/user/p/prsnko/Recalibrations/LHC10b_pass1.root") ;

protected:
  IlcPHOSTenderSupply(const IlcPHOSTenderSupply&c);
  IlcPHOSTenderSupply& operator= (const IlcPHOSTenderSupply&c);
  void   InitTender();
  void   FindTrackMatching(Int_t mod,TVector3 *locpos,Double_t &dx, Double_t &dz, Double_t &pttrack, Int_t &charge); 
  Float_t CorrectNonlinearity(Float_t en) ;
  Double_t TestCPV(Double_t dx, Double_t dz, Double_t pt, Int_t charge) ;
  Double_t TestLambda(Double_t pt,Double_t l1,Double_t l2) ;
  Bool_t IsGoodChannel(Int_t mod, Int_t ix, Int_t iz) ;

private:

  TString fOCDBpass ;                        //! Pass to OCDB recalibration object, local or alien
  TString fNonlinearityVersion;              //! Version of non-linearity correction to aaply
  IlcPHOSGeometry   *fPHOSGeo;               //! PHOS geometry
  Double_t fNonlinearityParams[10] ;         //! Parameters for non-linearity calculation
  TH2I * fPHOSBadMap[5] ;                    //! Bad channels map
  Int_t fRunNumber ;                         //! Current run number
  Int_t fRecoPass ;                          //! Reconstruction pass
  Bool_t fUsePrivateBadMap ;
  Bool_t fUsePrivateCalib ;
  
  IlcPHOSCalibData *fPHOSCalibData;          //! PHOS calibration object

 
  ClassDef(IlcPHOSTenderSupply, 1); // PHOS tender task
};


#endif


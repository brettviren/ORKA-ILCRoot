#ifndef ILCTARGETCLUSTERFINDERSSD_H
#define ILCTARGETCLUSTERFINDERSSD_H
/* Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 * See cxx source for full Copyright notice                               */

//#include <TMath.h>
#include "IlcTARGETClusterFinder.h"
//#include "IlcTARGETsegmentationSSD.h"
#include "IlcTARGETDetTypeRec.h"

class TArrayI;
class IlcTARGETclusterSSD;
class IlcTARGETpackageSSD;
class IlcTARGETsegmentation;
class IlcTARGETsegmentationSSD;
class IlcTARGETresponse;
class IlcTARGETresponseSSD;
class IlcTARGETCalibration;
class IlcTARGETCalibrationSSD;

class IlcTARGETClusterFinderSSD: public IlcTARGETClusterFinder{

 public:
  IlcTARGETClusterFinderSSD();
  IlcTARGETClusterFinderSSD(IlcTARGETDetTypeRec* dettyp);
  IlcTARGETClusterFinderSSD(IlcTARGETDetTypeRec* dettyp, TClonesArray *digits);
  //   IlcTARGETClusterFinderSSD(IlcTARGETsegmentation *seg, TClonesArray *digits);
  virtual ~IlcTARGETClusterFinderSSD();
  void FindRawClusters(Int_t module);    
  
 protected:
    virtual IlcTARGETsegmentationSSD* GetSeg()const{
        return (IlcTARGETsegmentationSSD*)fDetTypeRec->GetSegmentationModelSPD(2);}
  void      InitReconstruction();
  Bool_t    CreateNewRecPoint(Float_t P, Float_t dP, Float_t N, Float_t dN,
			      Float_t Sig,Float_t dSig,
			      IlcTARGETclusterSSD *clusterP,
			      IlcTARGETclusterSSD *clusterN,Stat_t prob);
  IlcTARGETclusterSSD* GetPSideCluster(Int_t idx);
  IlcTARGETclusterSSD* GetNSideCluster(Int_t idx);
  IlcTARGETclusterSSD* GetCluster(Int_t idx, Bool_t side);
  void      FindNeighbouringDigits(Int_t module);
  void      SeparateOverlappedClusters();
  void      SplitCluster(TArrayI *list,Int_t nsplits,Int_t indx,Bool_t side);
  Int_t     SortDigitsP(Int_t start, Int_t end);
  Int_t     SortDigitsN(Int_t start, Int_t end);
  void      FillDigitsIndex();
  void      SortDigits();
  void      FillClIndexArrays(Int_t* arrayP, Int_t *arrayN) const;
  void      SortClusters(Int_t* arrayP, Int_t *arrayN);
  Int_t     SortClustersP(Int_t start, Int_t end,Int_t *array);
  Int_t     SortClustersN(Int_t start, Int_t end,Int_t *array);
  void      ClustersToPackages();
  Int_t     GetDiff(Float_t */*retx*/, Float_t */*rety*/) const {return 0;}
  void      CalcStepFactor(Float_t Psteo, Float_t Nsteo );
  Bool_t GetCrossing(Float_t &x, Float_t &z); //x, y of strips crossing
  void   GetCrossingError(Float_t& dp, Float_t& dn);//x, y of strips crossing err.
  virtual IlcTARGETCalibrationSSD* GetResp(Int_t mod)const{
    return (IlcTARGETCalibrationSSD*) fDetTypeRec->GetCalibrationModel(mod);}//Return Response
  
  // Data memebers
  //  IlcTARGET          *fTARGET;           //!Pointer to IlcTARGET object
  TClonesArray    *fClusterP;      //!
  Int_t            fNClusterP;     //!Number of P side clusters in the array
  TClonesArray    *fClusterN;      //!Number of N side clusters in the array
  Int_t            fNClusterN;     //!
  TClonesArray    *fPackages;      //!packages  
  Int_t            fNPackages;     //!
  TArrayI         *fDigitsIndexP;  //!Digits on P side
  Int_t            fNDigitsP;      //!Number of Digits on P side
  TArrayI         *fDigitsIndexN;  //!Digits on N side
  Int_t            fNDigitsN;      //!Number of Digits on N side
  
  Float_t          fPitch;         //!Strip pitch
  Float_t          fTanP;          //!Pside stereo angle tangent
  Float_t          fTanN;          //!Nside stereo angle tangent
  
  //Float_t         **fNoise;
  
  /*************************************************/
  /**  parameters for reconstruction            ****/
  /**  to be tune when slow simulation rilcable ****/
  /*************************************************/
  
  //Float_t fAlpha1;         //!
  //Float_t fAlpha2;         //!
  //Float_t fAlpha3;         //!
  Float_t fPNsignalRatio;  //!
  
  static const Bool_t fgkSIDEP;  //!
  static const Bool_t fgkSIDEN;  //!
  
  static const Int_t  fgkNoiseThreshold; // at least one strip in the cluster has to have a signal > fgkNoiseThresold*noise 
  
  Int_t fSFF;              //!forward stepping factor 
  Int_t fSFB;              //!backward stepping factor 
  
  ClassDef(IlcTARGETClusterFinderSSD, 1) //Class for clustering and reconstruction of space points in SSDs 
    
    };
    
    
#endif

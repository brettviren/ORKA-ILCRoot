#ifndef ILCILCGNMENTTRACKS_H
#define ILCILCGNMENTTRACKS_H

//*************************************************************************
// IlcAlignmentTracks: main steering class which deals with the alignment *
// procedures based on reconstructed tracks.                              *
// More comments will come with the development of the interfaces and     *
// functionalities of the class.                                          *
//*************************************************************************

#include <TObject.h>

#include "IlcAlignObj.h"

class TChain;
class IlcTrackPointArray;
class IlcAlignObj;
class IlcTrackFitter;
class IlcTrackResiduals;

class IlcAlignmentTracks : public TObject {

 public:

  IlcAlignmentTracks();
  IlcAlignmentTracks(TChain *esdchain);
  IlcAlignmentTracks(const char *esdfilename, const char *esdtreename = "esdTree");
  virtual ~IlcAlignmentTracks();

  void AddESD(TChain *esdchain);
  void AddESD(const char *esdfilename, const char *esdtreename = "esdTree");

  void SetPointsFilename(const char *pointsfilename = "IlcTrackPoints.root") { fPointsFilename = pointsfilename; }

  void ProcessESD(TSelector *selector);
  void ProcessESD(Bool_t onlyITS=kFALSE,Int_t minITSpts=0,
		  Bool_t cuts=kTRUE,
		  Float_t minAngleWrtITSModulePlanes=0.,
		  Float_t minMom=0.3,Float_t maxMom=1.e9,
		  Float_t minAbsSinPhi=0.,Float_t maxAbsSinPhi=1.,
		  Float_t minSinTheta=0.,Float_t maxSinTheta=1.);
  void ProcessESDCosmics(Bool_t onlyITS=kFALSE,Int_t minITSpts=0,
			 Float_t maxMatchingAngle=0.17, // 10 deg
			 Bool_t cuts=kTRUE,
			 Float_t minAngleWrtITSModulePlanes=0.,
			 Float_t minMom=0.3,Float_t maxMom=1.e9,
			 Float_t minAbsSinPhi=0.,Float_t maxAbsSinPhi=1.,
			 Float_t minSinTheta=0.,Float_t maxSinTheta=1.);

  void BuildIndex();

  Bool_t ReadAlignObjs(const char *alignObjFileName = "AlignObjs.root", const char* arrayName = "Alignment");

  void SetTrackFitter(IlcTrackFitter *fitter) { fTrackFitter = fitter; }
  void SetMinimizer(IlcTrackResiduals *minimizer) { fMinimizer = minimizer; }

  Bool_t AlignDetector(IlcGeomManager::ELayerID firstLayer,
		       IlcGeomManager::ELayerID lastLayer,
		       IlcGeomManager::ELayerID layerRangeMin = IlcGeomManager::kFirstLayer,
		       IlcGeomManager::ELayerID layerRangeMax = IlcGeomManager::kLastLayer,Int_t iterations = 1);
  Bool_t AlignLayer(IlcGeomManager::ELayerID layer,
		    IlcGeomManager::ELayerID layerRangeMin = IlcGeomManager::kFirstLayer,
		    IlcGeomManager::ELayerID layerRangeMax = IlcGeomManager::kLastLayer,
		    Int_t iterations = 1);
  Bool_t AlignVolume(UShort_t volId, UShort_t volIdFit,
		     Int_t iterations);
  Bool_t AlignVolumes(const TArrayI *volids, const TArrayI *volidsfit = 0x0,
		      IlcGeomManager::ELayerID layerRangeMin = IlcGeomManager::kFirstLayer,
		      IlcGeomManager::ELayerID layerRangeMax = IlcGeomManager::kLastLayer,
		      Int_t iterations = 1);

  IlcAlignObj* GetAlignObj(UShort_t volid) const {
    Int_t iModule;
    IlcGeomManager::ELayerID iLayer = IlcGeomManager::VolUIDToLayer(volid,iModule);
    return fIlcgnObjs[iLayer-IlcGeomManager::kFirstLayer][iModule];
  }
  void    SetUpdate(Bool_t update){fDoUpdate = update;}
  void SetCovIsUsed(Bool_t covisused){fCovIsUsed=covisused;}
  Bool_t  GetUpdate() const { return fDoUpdate;}
  void WriteRealignObjArray(TString outfilename,IlcGeomManager::ELayerID layerRangeMin,IlcGeomManager::ELayerID layerRangeMax);
  Int_t GetLastIndex(Int_t iLayer,Int_t iModule) const { return fLastIndex[iLayer][iModule]; }  

  Bool_t Misalign(const char *misalignObjFileName, const char* arrayName);

 protected:

  void InitIndex();
  void ResetIndex();
  void DeleteIndex();

  void InitAlignObjs();
  void ResetAlignObjs();
  void DeleteAlignObjs();

  Int_t LoadPoints(const TArrayI *volids, IlcTrackPointArray** &points,Int_t &pointsdim);
  void  UnloadPoints(Int_t n, IlcTrackPointArray **points);

  IlcTrackFitter *CreateFitter();
  IlcTrackResiduals *CreateMinimizer();

  TChain           *fESDChain;       //! Chain with ESDs
  TString           fPointsFilename; //  Name of the file containing the track point arrays
  TFile            *fPointsFile;     //  File containing the track point arrays
  TTree            *fPointsTree;     //  Tree with the track point arrays
  Int_t           **fLastIndex;      //! Last filled index in volume arrays
  TArrayI        ***fArrayIndex;     //! Volume arrays which contains the tree index
  Bool_t            fIsIndexBuilt;   //  Is points tree index built
  IlcAlignObj    ***fIlcgnObjs;      //  Array with alignment objects
  IlcAlignObj    ***fMisalignObjs;   //  Array with alignment objects used to introduce misalignment of the space-points
  IlcTrackFitter   *fTrackFitter;    //  Pointer to the track fitter
  IlcTrackResiduals*fMinimizer;      //  Pointer to track residuals minimizer
  Bool_t            fDoUpdate;       //  Indicator - update Alignment object after minimization
  Bool_t            fCovIsUsed;      //  Indicator - use AlignObjs' Cov matrices

 private:
  IlcAlignmentTracks(const IlcAlignmentTracks & alignment);
  IlcAlignmentTracks& operator= (const IlcAlignmentTracks& alignment);

  ClassDef(IlcAlignmentTracks,2)

};

#endif

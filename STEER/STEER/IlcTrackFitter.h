#ifndef ILCTRACKFITTER_H
#define ILCTRACKFITTER_H

/*************************************************************************
 * IlcTrackFitter: base class for the fast track fitters                 *
 *                                                                       *
 * Supposed to be used for alignment framework                           *
 * More information is available in 
 * http://agenda.cern.ch/askArchive.php?base=agenda&categ=a057717&id=a057717s1t6/transparencies  
 * Author: C.Cheskov                                                     *
 *                                                                       *
 *                                                                       *
 *************************************************************************/

#include <TObject.h>
#include <TMatrixDSymfwd.h>

#include "IlcTrackPointArray.h"
#include "IlcAlignObj.h"

class TArrayI;

class IlcTrackFitter : public TObject {

 public:

  IlcTrackFitter();
  IlcTrackFitter(IlcTrackPointArray *array, Bool_t owner = kTRUE);
  IlcTrackFitter(const IlcTrackFitter &fitter);
  IlcTrackFitter& operator= (const IlcTrackFitter& fitter);
  virtual ~IlcTrackFitter();

  virtual void   Reset();
  virtual void   SetTrackPointArray(IlcTrackPointArray *array, Bool_t owner = kTRUE);
  virtual Bool_t Fit(const TArrayI *volIds,const TArrayI *volIdsFit = 0x0,
		     IlcGeomManager::ELayerID layerRangeMin = IlcGeomManager::kFirstLayer,
		     IlcGeomManager::ELayerID layerRangeMax = IlcGeomManager::kLastLayer);

  virtual Bool_t Begin(Int_t, Int_t) = 0;
  virtual Bool_t AddPoint(const IlcTrackPoint *p) = 0;
  virtual Bool_t Update() = 0;

  virtual Bool_t GetPCA(const IlcTrackPoint &pIn, IlcTrackPoint &pOut) const = 0;

  Bool_t         FindVolId(const TArrayI *array, UShort_t volid) const;

  void           SetMinNPoints(Int_t n) { fMinNPoints = n;}

  const Float_t* GetX() const {return fPoints->GetX();}
  const Float_t* GetY() const {return fPoints->GetY();}
  const Float_t* GetZ() const {return fPoints->GetZ();}
  const Double_t* GetParam() const {return &fParams[0];}
  const TMatrixDSym &  GetCovariance() const {return *fCov;}
  Float_t        GetChi2() const {return fChi2;}
  Int_t          GetNdf()  const {return fNdf;}
  Int_t          GetMinNPoints()  const {return fMinNPoints;}
  Float_t        GetNormChi2() const { return (fNdf != 0) ? fChi2/fNdf : 0; }
  void           GetTrackResiduals(IlcTrackPointArray*& pVolId, IlcTrackPointArray*& pTrack) const
    { pVolId = fPVolId; pTrack = fPTrack; }

 protected:

  Double_t      fParams[6];    // Track parameters
  TMatrixDSym  *fCov;          // Track cov matrix
  IlcTrackPointArray *fPoints; // Pointer to the array with track space points
  IlcTrackPointArray *fPVolId; // Pointer to the array with space-points in volId
  IlcTrackPointArray *fPTrack; // Pointer to the array with track extrapolation points in volId
  Float_t       fChi2;         // Chi squared of the fit
  Int_t         fNdf;          // Number of degrees of freedom
  Int_t         fMinNPoints;   // Minimum allowed number of points
  Bool_t  fIsOwner;            // Is the object owner of the space points array

 private:

  ClassDef(IlcTrackFitter,1) // Abstract class of fast track fitters

};

#endif

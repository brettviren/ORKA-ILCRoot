#ifndef ILCILCGNOBJ_H
#define ILCILCGNOBJ_H

//************************************************************************
// IlcAlignObj: alignment base class for the storage of the alignment    *
//   constants for a single volume:                                      *
//   -  a displacement (a shift and a rotation) either as                *
//      - the 6 doubles which identify it or as                          *
//      - the matrix which identifies it                                 *
//   -  the identity of the volume itself in form of a symbolic volume   *
//      name for alignable volumes, in form of a TGeo path otherwise,    *
//      and as a unique integer identifier                               *
//************************************************************************
#include <TObject.h>
#include <TString.h>
#include <Rtypes.h>
#include "IlcGeomManager.h"
#include <TMatrixD.h>
#include <TMatrixDSym.h>


class IlcTrackPoint;
class IlcTrackPointArray;
class TGeoMatrix;

class IlcAlignObj : public TObject {

 public:

  IlcAlignObj();
  IlcAlignObj(const char* symname, UShort_t voluid);
  IlcAlignObj(const char* symname, UShort_t voluid, Double_t* cmat);
  IlcAlignObj(const char* symname, IlcGeomManager::ELayerID detId, Int_t modId);
  IlcAlignObj(const IlcAlignObj& theAlignObj);
  IlcAlignObj& operator= (const IlcAlignObj& theAlignObj);
  IlcAlignObj& operator*=(const IlcAlignObj& theAlignObj);
  virtual ~IlcAlignObj();

  //Setters
  virtual void SetTranslation(Double_t x, Double_t y, Double_t z) = 0;
  virtual void SetTranslation(const TGeoMatrix& m) = 0;
  virtual Bool_t SetLocalTranslation(Double_t x, Double_t y, Double_t z);
  virtual Bool_t SetLocalTranslation(const TGeoMatrix& m);
  virtual void SetRotation(Double_t psi, Double_t theta, Double_t phi) = 0;
  virtual Bool_t SetRotation(const TGeoMatrix& m) = 0;
  virtual Bool_t SetLocalRotation(Double_t psi, Double_t theta, Double_t phi);
  virtual Bool_t SetLocalRotation(const TGeoMatrix& m);
  virtual void SetPars(Double_t x, Double_t y, Double_t z, Double_t psi,
               Double_t theta, Double_t phi);
  virtual Bool_t SetLocalPars(Double_t x, Double_t y, Double_t z,
			      Double_t psi, Double_t theta, Double_t phi);
  virtual Bool_t SetMatrix(const TGeoMatrix& m);
  virtual Bool_t SetLocalMatrix(const TGeoMatrix& m);
  void  SetSymName(const TString& symname) {fVolPath=symname;}
  void  SetVolUID(UShort_t voluid) {fVolUID=voluid;}
  void  SetVolUID(IlcGeomManager::ELayerID layerId, Int_t modId);
  void  SetCorrMatrix(Double_t *cov);
  void  SetCorrMatrix(TMatrixDSym& mcov);
  Bool_t SetFromLocalCov(Double_t *lCov);
  Bool_t SetFromLocalCov(TMatrixDSym& lCov);

  //Getters
  const char  *GetSymName()    const {return fVolPath.Data();}
  UShort_t     GetVolUID()     const {return fVolUID;}
  void         GetVolUID(IlcGeomManager::ELayerID &layerId, Int_t &modId) const;
  virtual void GetTranslation(Double_t* tr)  const=0;
  virtual Bool_t GetLocalTranslation(Double_t* tr) const;
  virtual Bool_t GetAngles(Double_t* angles) const=0;
  virtual Bool_t GetLocalAngles(Double_t* angles) const;
  virtual Bool_t GetPars(Double_t transl[], Double_t angles[]) const;
  virtual Bool_t GetLocalPars(Double_t transl[], Double_t angles[]) const;
  virtual void GetMatrix(TGeoHMatrix& m) const=0;
  virtual Bool_t GetLocalMatrix(TGeoHMatrix& m) const;
  void  GetCovMatrix(Double_t *cov) const;
  void  GetCovMatrix(TMatrixDSym& mcov) const;
  Bool_t  GetJacobian(TMatrixD& mJ) const;
  Bool_t  GetLocalCovMatrix(Double_t *cov) const;
  Bool_t  GetLocalCovMatrix(TMatrixDSym& lCov) const;

  Bool_t   IsSortable() const {return kTRUE;}
  Int_t         GetLevel() const;
  virtual Int_t Compare(const TObject* obj) const;

  virtual IlcAlignObj& Inverse() const=0;

  void  Transform(IlcTrackPoint &p, Bool_t copycov=kFALSE) const;
  void  Transform(IlcTrackPointArray &array) const;

  void  Print(Option_t *) const;

  static Int_t       LayerSize(Int_t layerId) {return IlcGeomManager::LayerSize(layerId);}
  static const char* LayerName(Int_t layerId) {return IlcGeomManager::LayerName(layerId);}

  static UShort_t LayerToVolUID(IlcGeomManager::ELayerID layerId, Int_t modId) {return IlcGeomManager::LayerToVolUID(layerId, modId);}
  static UShort_t LayerToVolUID(Int_t layerId, Int_t modId) {return IlcGeomManager::LayerToVolUID(layerId, modId);}
  static IlcGeomManager::ELayerID VolUIDToLayer(UShort_t voluid, Int_t &modId) {return IlcGeomManager::VolUIDToLayer(voluid, modId);}
  static IlcGeomManager::ELayerID VolUIDToLayer(UShort_t voluid) {return IlcGeomManager::VolUIDToLayer(voluid);}
  static UShort_t LayerToVolUIDSafe(IlcGeomManager::ELayerID layerId, Int_t modId) {return IlcGeomManager::LayerToVolUIDSafe(layerId, modId);}
  static UShort_t LayerToVolUIDSafe(Int_t layerId, Int_t modId) {return IlcGeomManager::LayerToVolUIDSafe(layerId, modId);}
  static IlcGeomManager::ELayerID VolUIDToLayerSafe(UShort_t voluid, Int_t &modId) {return IlcGeomManager::VolUIDToLayerSafe(voluid, modId);}
  static IlcGeomManager::ELayerID VolUIDToLayerSafe(UShort_t voluid) {return IlcGeomManager::VolUIDToLayerSafe(voluid);}

  static const char* SymName(UShort_t voluid) {return IlcGeomManager::SymName(voluid);}
  static const char* SymName(IlcGeomManager::ELayerID layerId, Int_t modId) {return IlcGeomManager::SymName(layerId, modId);}

  Bool_t ApplyToGeometry(Bool_t ovlpcheck=kFALSE);
  static Bool_t   GetFromGeometry(const char *symname, IlcAlignObj &alobj) {return IlcGeomManager::GetFromGeometry(symname, alobj);}

  static IlcAlignObj* GetAlignObj(UShort_t voluid) {return IlcGeomManager::GetAlignObj(voluid);}
  static IlcAlignObj* GetAlignObj(IlcGeomManager::ELayerID layerId, Int_t modId) {return IlcGeomManager::GetAlignObj(layerId, modId);}

  static Bool_t   GetOrigGlobalMatrix(const char *symname, TGeoHMatrix &m) {return IlcGeomManager::GetOrigGlobalMatrix(symname, m);}

 protected:

  void AnglesToMatrix(const Double_t *angles, Double_t *rot) const;
  Bool_t MatrixToAngles(const Double_t *rot, Double_t *angles) const;

  //Volume identifiers
  TString  fVolPath; // Symbolic volume name; in case could coincide with
      // the volume path inside TGeo geometry (for non-alignable volumes)
  UShort_t fVolUID;  // Unique volume ID

  Double32_t   fDiag[6];  // diagonal elements of the correlation matrix for shifts and rotations (dx,dy,dz,dpsi,dtheta,dphi)
  Double32_t   fODia[15]; // [-1, 1,8] off-diagonal elements (in 8 bit precision) of the correlation matrix
	
  ClassDef(IlcAlignObj, 3)
};

#endif

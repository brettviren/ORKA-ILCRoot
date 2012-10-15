#ifndef ILCDCHGEOMETRY_H
#define ILCDCHGEOMETRY_H
/* Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 * See cxx source for full Copyright notice                               */ 
 
/* $Id: IlcDCHgeometry.h,v 1.6 2009/07/06 10:30:07 tassiell Exp $ */ 
 
/////////////////////////////////////////////////////////////////////////////// 
//                                                                           // 
//  DCH geometry class                                                       // 
//                                                                           // 
/////////////////////////////////////////////////////////////////////////////// 
 
#include "IlcGeometry.h" 
#include "IlcDCHParam.h" 
#include "TObjArray.h" 
class IlcRunLoader; 
class TGeoHMatrix; 
 
class IlcDCHgeometry : public IlcGeometry { 
 
 public: 
 
  enum { kNplan = 4, kNcham = 3, kNsect =12 , kNdet = 144, kNdets = 5, kNlayers=10}; 
 
  IlcDCHgeometry(); 
  virtual ~IlcDCHgeometry(); 
 
//   virtual void     CreateGeometry(Int_t *idtmed);
  virtual void     CreateGeometry();
  virtual Int_t    IsVersion() { return 1; }; 
 virtual void     Init(); 
  virtual Bool_t   Impact(const TParticle* ) const { return kTRUE; }; 
  // virtual void     ImportGDMLGeometry(const char *filename="/afs/le.infn.it/user/g/giuter/ILC/Ilcv4-04-11/aaa.gdml"); 
 
//   virtual Bool_t   Local2Global(Int_t d, Double_t *local, Double_t *global) const; 
//   virtual Bool_t   Local2Global(Int_t p, Int_t c, Int_t s, Double_t *local, Double_t *global) const; 
//  virtual Bool_t   Global2Local(Int_t mode, Double_t *local, Double_t *global, Int_t* index) const; 
//  virtual Bool_t   Global2Detector(Double_t global[3], Int_t index[3]); 
//  virtual Bool_t   Rotate(Int_t d, Double_t *pos, Double_t *rot) const; 
//  virtual Bool_t   RotateBack(Int_t d, Double_t *rot, Double_t *pos) const; 
 
// 	  void     GroupChamber(Int_t iplan, Int_t icham, Int_t *idtmed); 
// 	  void     CreateFrame(Int_t *idtmed); 
// 	  void     CreateServices(Int_t *idtmed); 
 
//          Bool_t   ReadGeoMatrices();   
  TGeoHMatrix *    GetGeoMatrix(Int_t det)       { return (TGeoHMatrix *) fMatrixGeo->At(det);             } 
  TGeoHMatrix *    GetMatrix(Int_t det)          { return (TGeoHMatrix *) fMatrixArray->At(det);           } 
  TGeoHMatrix *    GetCorrectionMatrix(Int_t det){ return (TGeoHMatrix *) fMatrixCorrectionArray->At(det); } 
 
  void SetParam(IlcDCHParam *param){fDCHParam=param;}
  Double_t GetBaseExag(Int_t superlayer){return fBaseExag[superlayer];}
 
  void     SetSMstatus(Int_t sm, Char_t status)     {  
            sm += 5; if (sm > kNsect-1) sm -= kNsect; fSMstatus[sm] = status; }; 
 
  virtual Bool_t   IsHole(Int_t /*iplan*/, Int_t /*icham*/, Int_t /*isect*/) const { return kFALSE; }; 
 
  Char_t   GetSMstatus(Int_t sm) const  { sm += 5; if (sm > 17) sm -= 18;  return fSMstatus[sm];  }; 
          Float_t  GetChamberWidth(Int_t p,Int_t s=0) const           { return fCWidth[p][s];}

          Float_t  GetChamberLength(Int_t p, Int_t c) const { return fClength[p][c]; };  
 
  virtual void     GetGlobal(const IlcRecPoint* , TVector3& , TMatrixF& ) const { };  
  virtual void     GetGlobal(const IlcRecPoint* , TVector3& ) const { }; 
 
 
 
  IlcDCHgeometry* GetGeometry(IlcRunLoader* runLoader = NULL); 

  Float_t  GetDriftVelocity()                       const { return fDriftVelocity; }; 
 
 private: 
 
 
 protected: 
  
   Double_t fBaseExag[40];
  IlcDCHParam         *fDCHParam;           //  pointer to DCH parameters 

  Char_t               fSMstatus[kNsect];                   // Super module status byte 
 
  Float_t              fCWidth[kNplan][kNsect];                     // Outer widths of the chambers

  Float_t              fClength[kNplan][kNcham];            // Outer lengths of the chambers 
 
  Float_t              fRotA11[kNsect];                     // Matrix elements for the rotation 
  Float_t              fRotA12[kNsect];                     // Matrix elements for the rotation 
  Float_t              fRotA21[kNsect];                     // Matrix elements for the rotation 
  Float_t              fRotA22[kNsect];                     // Matrix elements for the rotation 
 
  Float_t              fRotB11[kNsect];                     // Matrix elements for the backward rotation 
  Float_t              fRotB12[kNsect];                     // Matrix elements for the backward rotation 
  Float_t              fRotB21[kNsect];                     // Matrix elements for the backward rotation 
  Float_t              fRotB22[kNsect];                     // Matrix elements for the backward rotation 
 
   
  Float_t              fChamberUAorig[3*kNdets][3];         // Volumes origin in 
  Float_t              fChamberUDorig[3*kNdets][3];         // the chamber 
  Float_t              fChamberUForig[3*kNdets][3];         // [3] = x, y, z 
  Float_t              fChamberUUorig[3*kNdets][3];         // 
 
  Float_t              fChamberUAboxd[3*kNdets][3];         // Volumes box 
  Float_t              fChamberUDboxd[3*kNdets][3];         // dimensions (half) 
  Float_t              fChamberUFboxd[3*kNdets][3];         // [3] = x, y, z 
  Float_t              fChamberUUboxd[3*kNdets][3];         //  
 
  TObjArray *          fMatrixArray;                        //! array of matrix - Transformation Global to Local 
  TObjArray *          fMatrixCorrectionArray;              //! array of Matrix - Transformation Cluster to  Tracking systerm 
  TObjArray *          fMatrixGeo;                          //! geo matrices 
 
 
Float_t              fDriftVelocity;                        //  Drift velocity (cm / mus) 
 
 
  ClassDef(IlcDCHgeometry,2)                                // DCH geometry class 
 
}; 
 
#endif 
 
 
 
 
 
 
 
 
 
 

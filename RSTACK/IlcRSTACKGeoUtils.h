#ifndef ILCRSTACKGEOUTILS_H
#define ILCRSTACKGEOUTILS_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

//_________________________________________________________________________
// class for geometry transformations in RSTACK
// this class contains AiRoot-independent transformations,
// IlcRoot part is in IlcRSTACKGeometry
// RSTACK consists of the electromagnetic calorimeter (EMCA)
// charged particle veto (CPV) and support
// 
//*-- Author: Dmitri Peressounko (RRC "KI")

// --- ROOT system ---
#include "TNamed.h"

// --- IlcRoot header files ---

class TGeoHMatrix ;
class TVector3;
class TParticle ;
class IlcRSTACKEMCAGeometry;
class IlcRSTACKCPVGeometry;
class IlcRSTACKSupportGeometry;

class IlcRSTACKGeoUtils : public TNamed {

public: 

  IlcRSTACKGeoUtils() ;
  IlcRSTACKGeoUtils(const Text_t* name, const Text_t* title="") ;
  IlcRSTACKGeoUtils(const IlcRSTACKGeoUtils & geom) ;
  
  virtual ~IlcRSTACKGeoUtils(void) ; 
  IlcRSTACKGeoUtils & operator = (const IlcRSTACKGeoUtils  & rvalue) ;

  Bool_t AbsToRelNumbering(Int_t AbsId, Int_t * RelId) const ; 
                                         // converts the absolute RSTACK cell numbering to a relative 
  Bool_t RelToAbsNumbering(const Int_t * RelId, Int_t & AbsId) const ; 
                                         // converts the absolute RSTACK numbering to a relative         
                                         
  void RelPosInModule(const Int_t * relId, Float_t & y, Float_t & z) const ; 
                                         // gets the position of element (pad or Xtal) relative to 
                                         // center of RSTACK module  
  void RelPosToAbsId(Int_t module, Double_t x, Double_t z, Int_t & AbsId) const; 
                                         // converts local RSTACK-module (x, z) coordinates to absId 
  void RelPosToRelId(Int_t module, Double_t x, Double_t z, Int_t * relId) const; 
                                         // converts local RSTACK-module (x, z) coordinates to relId 
  void RelPosInIlc(Int_t AbsId, TVector3 &  pos) const ;             
                                         // gets the position of element (pad or Xtal) relative to Ilc

  void Local2Global(Int_t module, Float_t x, Float_t z, TVector3 &globaPos) const ;
              
  void Global2Local(TVector3& localPosition,const TVector3& globalPosition,Int_t module) const ;

  Bool_t GlobalPos2RelId(TVector3 & global, Int_t * relId) ;
        //Tranfers global position into numbers in RSTACK cols-raws
        //Returns FALSE if local x,z beyond RSTACK. Distance in perpendicular to RSTACK direction is not checked.

  Bool_t ImpactOnEmc(const TParticle * particle,Int_t & ModuleNumber, Double_t & z, Double_t & x) const ;
                                         //Checks if NEUTRAL particle hits RSTACK
  Bool_t ImpactOnEmc(const Double_t * vtx, const TVector3& vec,                
		   Int_t & ModuleNumber, Double_t & z, Double_t & x) const ; 
                                         // calculates the impact coordinates of a neutral particle  
                                         // emitted in direction theta and phi in ILC
  Bool_t ImpactOnEmc(const Double_t * vtx, const Double_t theta, const Double_t phi, 
		   Int_t & ModuleNumber, Double_t & z, Double_t & x) const ; 
                                         // calculates the impact coordinates of a neutral particle  
                                         // emitted in direction theta and phi in ILC
  void  GetIncidentVector(const TVector3 &vtx, Int_t module, Float_t x, Float_t z, TVector3& vInc) const ;
                                         //calculates vector from vertex to current point in module local frame

  Bool_t IsInEMC(Int_t id) const { if (id > fNModules *  fNCristalsInModule ) return kFALSE; return kTRUE; } 

  //Method to set shift-rotational matrixes from ESDHeader
  void SetMisalMatrix(const TGeoHMatrix * m, Int_t mod) ;

protected:
  //Returns shift-rotational matrixes for different volumes
  const TGeoHMatrix * GetMatrixForModule(Int_t mod)const ;
  const TGeoHMatrix * GetMatrixForStrip(Int_t mod, Int_t strip)const ;
  const TGeoHMatrix * GetMatrixForCPV(Int_t mod)const ;
  const TGeoHMatrix * GetMatrixForRSTACK(Int_t mod)const ;

protected:

  IlcRSTACKEMCAGeometry     *fGeometryEMCA ;   // Geometry object for Electromagnetic calorimeter
  IlcRSTACKCPVGeometry      *fGeometryCPV ;    // Geometry object for CPV  (IHEP)
  IlcRSTACKSupportGeometry  *fGeometrySUPP ;   // Geometry object for RSTACK support
 

  Int_t fNModules ;          //Maximal designed number of modules in RSTACK
  Int_t fNCristalsInModule ; //Number of crystals in one module
  Int_t fNPhi ;              //Number of crystals along Phi direction
  Int_t fNZ ;                //Number of crystals along Z direction
  Int_t fNumberOfCPVPadsPhi; //Number of CPV pads along Phi direction
  Int_t fNumberOfCPVPadsZ ;  //Number of CPV pads along Z direction
  Int_t fNCellsXInStrip ;    //Number of crystals in strip
  Int_t fNCellsZInStrip ;    //Number of crystals in strip in Z direction 
  Int_t fNStripZ ;           //Number of strips in Z direction

  Float_t fCrystalShift ;    //Distance between center of module and crystal surface
  Float_t fXtlArrSize[3] ;   //Total size of cristals array
  Float_t fCryCellShift ; 
  Float_t fCryStripShift ;
  Float_t fCellStep ; 
  Float_t fPadSizePhi ;      //Size of CPV pad in Phi direction
  Float_t fPadSizeZ ;        //Size of CPV pad in Z direction
  Float_t fCPVBoxSizeY ;

  TGeoHMatrix* fEMCMatrix[5] ; //Orientations of crystalls array in modules
  TGeoHMatrix* fStripMatrix[5][224];  //Orientations of strip units
  TGeoHMatrix* fCPVMatrix[5] ; //Orientations of CPV
  const TGeoHMatrix* fRSTACKMatrix[5] ; //Orientations of RSTACK modules

  TClonesArray * fMisalArray ;

  ClassDef(IlcRSTACKGeoUtils,2)       // RSTACK geometry class 

} ;

#endif // IlcRSTACKGEOUTILS_H

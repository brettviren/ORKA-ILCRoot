#ifndef ILCPVECGEOMETRY_H
#define ILCPVECGEOMETRY_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcPVECGeometry.h 29570 2008-10-28 08:12:56Z hristov $ */

//_________________________________________________________________________
// Geometry class  for PVEC : singleton
// PVEC consists of the electromagnetic calorimeter (EMCA)
// and a charged particle veto either in the Subatech's version (PPSD)
// or in the IHEP's one (CPV).
// The EMCA/PPSD/CPV modules are parametrized so that any configuration
// can be easily implemented 
// The title is used to identify the version of CPV used.
// 
//*-- Author: Yves Schutz (SUBATECH)

// --- ROOT system ---

// --- IlcRoot header files ---

#include "IlcLog.h"

#include "IlcPVECGeoUtils.h"
#include <TMatrixFfwd.h>

class IlcRecPoint ;
class IlcPVECRecPoint;
class TVector3;

class IlcPVECGeometry : public TNamed {

public: 

  IlcPVECGeometry() ;
  IlcPVECGeometry(const IlcPVECGeometry & geom) ;
  
  virtual ~IlcPVECGeometry(void) ; 
  static IlcPVECGeometry * GetInstance(const Text_t* name, const Text_t* title="") ; 
  static IlcPVECGeometry * GetInstance() ; 

  void Init(void) ;            // steering method for PVEC

  IlcPVECGeometry & operator = (const IlcPVECGeometry  & /*rvalue*/) {
    Fatal("operator =", "not implemented") ;
    return *this ;    
  }
 

  Bool_t IsInitialized(void)                  const { return fgInit ; }  
  Bool_t AbsToRelNumbering(Int_t AbsId, Int_t * RelId) const ; 
        // converts the absolute PVEC tile numbering to a relative
  Bool_t RelToAbsNumbering(const Int_t * RelId, Int_t & AbsId) const ; 
        // converts the absolute PVEC numbering to a relative
                                                                       
  // Return general PVEC parameters
  
  Double_t GetPVECRmin(void)        const { return fPVECRmin; }
  Double_t GetPVECRmax(void)        const { return fPVECRmax; }
  Double_t GetPVECZPosition()       const { return fPVECZPosition; }
  Double_t GetPVECLength(void)      const { return fPVECLength; }
  Int_t    GetPVECNSlicePhi(void)   const { return fPVECNSlicePhi; }
  Int_t    GetPVECNLayers(void)     const { return fPVECNLayers; }
  Int_t    GetPVECNXtalPhi(Int_t index) const { return fPVECNXtalPhi[index-1];} 
  Double_t GetPVECLayerThickness(Int_t index) const { return fPVECLayerThickness[index-1];} 
  Double_t GetPVECXTalEnvelopThickness(void)     const { return fPVECXTalEnvelopThickness; }
protected:

  IlcPVECGeometry(const Text_t* name, const Text_t* title="") ;
private:

  Float_t                  fAngle ;          // Position angles between modules

  Double_t fPVECRmin;
  Double_t fPVECRmax;
  Double_t fPVECZPosition;
  Double_t fPVECLength;
  Int_t    fPVECNSlicePhi;
  Int_t    fPVECNLayers;
  Int_t   *fPVECNXtalPhi; //[fPVECNLayers]
  Double_t *fPVECLayerThickness; //[fPVECNLayers]
  Double_t fPVECXTalEnvelopThickness;

  static IlcPVECGeometry * fgGeom ; // pointer to the unique instance of the singleton 
  static Bool_t fgInit ;            // Tells if geometry has been succesfully set up 

  ClassDef(IlcPVECGeometry,3)       // PVEC geometry class 

} ;

#endif // IlcPVECGEOMETRY_H

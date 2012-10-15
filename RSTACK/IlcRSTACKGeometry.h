#ifndef ILCRSTACKGEOMETRY_H
#define ILCRSTACKGEOMETRY_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

//_________________________________________________________________________
// Geometry class  for RSTACK : singleton
// RSTACK consists of scintillator tiles
// The RSTACK modules are parametrized so that any configuration
// can be easily implemented 
// The title is used to identify the version used.
// 

// --- ROOT system ---
#include "TNamed.h"

// --- IlcRoot header files ---
#include "IlcLog.h"

#include "IlcRSTACKGeoUtils.h"
#include <TMatrixFfwd.h>

class IlcRecPoint ;
class IlcRSTACKRecPoint;
class TVector3;

class IlcRSTACKGeometry : public TNamed {

public: 

  IlcRSTACKGeometry() ;
  IlcRSTACKGeometry(const IlcRSTACKGeometry & geom) ;
  
  virtual ~IlcRSTACKGeometry(void) ; 
  static IlcRSTACKGeometry * GetInstance(const Text_t* name, const Text_t* title="") ; 
  static IlcRSTACKGeometry * GetInstance() ; 

  void Init(void) ;            // steering method for RSTACK

  IlcRSTACKGeometry & operator = (const IlcRSTACKGeometry  & /*rvalue*/) {
    Fatal("operator =", "not implemented") ;
    return *this ;    
  }
 

  Bool_t IsInitialized(void)                  const { return fgInit ; }  
  Bool_t AbsToRelNumbering(Int_t AbsId, Int_t * RelId) const ; 
        // converts the absolute RSTACK tile numbering to a relative
  Bool_t RelToAbsNumbering(const Int_t * RelId, Int_t & AbsId) const ; 
        // converts the absolute RSTACK numbering to a relative

  // Return general RSTACK parameters
  Double_t GetRSTACKRmin(void)               const { return fRSTACKRmin; }
  Double_t GetRSTACKRmax(void)               const { return fRSTACKRmax; }
  Double_t GetRSTACKLength(void)             const { return fRSTACKLength; }
  Double_t GetRSTACKTileScintThickness(void) const { return fRSTACKTileScintThickness; }
  Int_t    GetRSTACKNSectorPhi(void)          const { return fRSTACKNSectorPhi; }
  Int_t    GetRSTACKNTilesInSector(void)      const {return  Int_t( (GetRSTACKRmax() - GetRSTACKRmin()) / GetRSTACKTileScintThickness()); }
  

protected:

  IlcRSTACKGeometry(const Text_t* name, const Text_t* title="") ;
private:

  Float_t  fAngle ;             // Position angles between modules

  Double_t fRSTACKRmin;
  Double_t fRSTACKRmax;
  Double_t fRSTACKLength;
  Double_t fRSTACKTileScintThickness;
  Int_t    fRSTACKNSectorPhi;


  static IlcRSTACKGeometry * fgGeom ; // pointer to the unique instance of the singleton 
  static Bool_t fgInit ;            // Tells if geometry has been succesfully set up 

  ClassDef(IlcRSTACKGeometry,3)       // RSTACK geometry class 

} ;

#endif // IlcRSTACKGEOMETRY_H

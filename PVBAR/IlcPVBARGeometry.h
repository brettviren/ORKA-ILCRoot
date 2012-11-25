#ifndef ILCPVBARGEOMETRY_H
#define ILCPVBARGEOMETRY_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcPVBARGeometry.h 29570 2008-10-28 08:12:56Z hristov $ */

//_________________________________________________________________________
// Geometry class  for PVBAR : singleton
// PVBAR consists of the electromagnetic calorimeter (EMCA)
// and a charged particle veto either in the Subatech's version (PPSD)
// or in the IHEP's one (CPV).
// The EMCA/PPSD/CPV modules are parametrized so that any configuration
// can be easily implemented 
// The title is used to identify the version of CPV used.
// 
//*-- Author: Yves Schutz (SUBATECH)

// --- ROOT system ---

// --- IlcRoot header files ---

#include "IlcPVBARGeoUtils.h"
#include "IlcPVBAREMCAGeometry.h"
#include "IlcPVBARCPVGeometry.h"
#include "IlcPVBARSupportGeometry.h"
#include <TMatrixFfwd.h>
#include <TMath.h>

class IlcRecPoint ;
class IlcPVBARRecPoint;
class TVector3;

class IlcPVBARGeometry : public IlcPVBARGeoUtils {

public: 

  IlcPVBARGeometry() ;
  IlcPVBARGeometry(const IlcPVBARGeometry & geom) ;
  
  virtual ~IlcPVBARGeometry(void) ; 
  static IlcPVBARGeometry * GetInstance(const Text_t* name, const Text_t* title="") ; 
  static IlcPVBARGeometry * GetInstance() ; 

  virtual void   GetGlobal(const IlcRecPoint* RecPoint, TVector3 & gpos, TMatrixF & /* gmat */) const 
                 {GetGlobal(RecPoint,gpos); }
  virtual void   GetGlobal(const IlcRecPoint* RecPoint, TVector3 & gpos) const ;
  virtual void   GetGlobalPVBAR(const IlcPVBARRecPoint* RecPoint, TVector3 & gpos) const ;
  virtual void   GetGlobalPVBAR(const IlcPVBARRecPoint* RecPoint, TVector3 & gpos, TMatrixF & /* gmat */) const 
                 {GetGlobalPVBAR(RecPoint,gpos); }

  IlcPVBARGeometry & operator = (const IlcPVBARGeometry  & /*rvalue*/) {
    Fatal("operator =", "not implemented") ;
    return *this ;    
  }
 

  Bool_t IsInitialized(void)                  const { return fgInit ; }  
                                                                       
  // Return general PVBAR parameters
  Int_t    GetNModules(void)                    const { return fNModules ; }
  Float_t* GetPVBARParams(void)                        { return fPVBARParams;}  //Half-sizes of PVBAR trapecoid
  Float_t  GetIPtoUpperCPVsurface(void)         const { return fIPtoUpperCPVsurface ; }
  Float_t  GetOuterBoxSize(Int_t index)         const { return 2.*fPVBARParams[index]; }
  Float_t  GetCrystalSize(Int_t index)          const { return fGeometryEMCA->GetCrystalSize(index) ;  }
  Float_t  GetCellStep(void)                    const { return 2.*fGeometryEMCA->GetAirCellHalfSize()[0];}

  Float_t GetModuleCenter(Int_t module, Int_t axis) const {
    return fModuleCenter[module][axis];}
  Float_t GetModuleAngle(Int_t module, Int_t axis, Int_t angle) const {
    return fModuleAngle[module][axis][angle];}
  

  // Return ideal EMCA geometry parameters

  IlcPVBAREMCAGeometry * GetEMCAGeometry()      const {return fGeometryEMCA ;}
  Float_t   GetIPtoCrystalSurface(void)        const { return fGeometryEMCA->GetIPtoCrystalSurface() ; }
  Float_t   GetIPtoOuterCoverDistance(void)    const { return fGeometryEMCA->GetIPtoOuterCoverDistance() ; }
  Int_t     GetNPhi(void)                      const { return fGeometryEMCA->GetNPhi() ; }
  Int_t     GetNZ(void)                        const { return fGeometryEMCA->GetNZ() ; }
  Int_t     GetNCristalsInModule(void)         const { return fGeometryEMCA->GetNPhi() * fGeometryEMCA->GetNZ() ; }

  // Return ideal CPV geometry parameters
  Int_t   GetNumberOfCPVLayers(void)           const { return fGeometryCPV ->GetNumberOfCPVLayers();      }
  Float_t GetCPVActiveSize(Int_t index)        const { return fGeometryCPV->GetCPVActiveSize(index);      }
  Int_t   GetNumberOfCPVChipsPhi(void)         const { return fGeometryCPV->GetNumberOfCPVChipsPhi();     }
  Int_t   GetNumberOfCPVChipsZ(void)           const { return fGeometryCPV->GetNumberOfCPVChipsZ();       }
  Int_t   GetNumberOfCPVPadsPhi(void)          const { return fGeometryCPV->GetNumberOfCPVPadsPhi();      }
  Int_t   GetNumberOfCPVPadsZ(void)            const { return fGeometryCPV->GetNumberOfCPVPadsZ();        }
  Float_t GetPadSizePhi(void)                  const { return fGeometryCPV->GetCPVPadSizePhi();           }
  Float_t GetPadSizeZ(void)                    const { return fGeometryCPV->GetCPVPadSizeZ();             }
  Float_t GetGassiplexChipSize(Int_t index)    const { return fGeometryCPV->GetGassiplexChipSize(index);  }
  Float_t GetCPVGasThickness(void)             const { return fGeometryCPV->GetCPVGasThickness();         }
  Float_t GetCPVTextoliteThickness(void)       const { return fGeometryCPV->GetCPVTextoliteThickness();   }
  Float_t GetCPVCuNiFoilThickness(void)        const { return fGeometryCPV->GetCPVCuNiFoilThickness();    }
  Float_t GetFTPosition(Int_t index)           const { return fGeometryCPV->GetFTPosition(index);         }
  Float_t GetCPVFrameSize(Int_t index)         const { return fGeometryCPV->GetCPVFrameSize(index);       }
  Float_t GetCPVBoxSize(Int_t index)           const { return fGeometryCPV ->GetCPVBoxSize(index);        } 
  Float_t GetIPtoCPVDistance(void)             const { return  GetIPtoOuterCoverDistance() - 
							       GetCPVBoxSize(1) - 1.0; }


  // Return real CPV geometry parameters
  void GetModuleCenter(TVector3& center, const char *det, Int_t module) const;

  // Return PVBAR' support geometry parameters

  Float_t GetRailOuterSize(Int_t index)  const { return fGeometrySUPP->GetRailOuterSize(index); }
  Float_t GetRailPart1    (Int_t index)  const { return fGeometrySUPP->GetRailPart1    (index); }
  Float_t GetRailPart2    (Int_t index)  const { return fGeometrySUPP->GetRailPart2    (index); }
  Float_t GetRailPart3    (Int_t index)  const { return fGeometrySUPP->GetRailPart3    (index); }
  Float_t GetRailPos      (Int_t index)  const { return fGeometrySUPP->GetRailPos      (index); }
  Float_t GetRailLength   (void)         const { return fGeometrySUPP->GetRailLength   ();      }
  Float_t GetDistanceBetwRails(void)     const { return fGeometrySUPP->GetDistanceBetwRails();  }
  Float_t GetRailsDistanceFromIP(void)   const { return fGeometrySUPP->GetRailsDistanceFromIP();}
  Float_t GetRailRoadSize (Int_t index)  const { return fGeometrySUPP->GetRailRoadSize (index); }
  Float_t GetCradleWallThickness(void)   const { return fGeometrySUPP->GetCradleWallThickness();}
  Float_t GetCradleWall   (Int_t index)  const { return fGeometrySUPP->GetCradleWall   (index); }
  Float_t GetCradleWheel  (Int_t index)  const { return fGeometrySUPP->GetCradleWheel  (index); }
  void Init(void) ;            // steering method for PVBAR and PPSD/CPV

  Bool_t AbsToRelNumbering(Int_t AbsId, Int_t * RelId) const ; 
        // converts the absolute PVBAR tile numbering to a relative
  Bool_t RelToAbsNumbering(const Int_t * RelId, Int_t & AbsId) const ; 
        // converts the absolute PVBAR numbering to a relative

  Double_t GetPVBARRmin(void)              const { return fPVBARRmin; }
  Double_t GetPVBARRmax(void)              const { return fPVBARRmax; }
  Double_t GetPVBARLength(void)            const { return fPVBARLength; }
  Double_t GetPVBARTileScintThickness(void)const { return fPVBARTileScintThickness; }
  Double_t GetPVBARTileSF57Thickness(void) const { return fPVBARTileSF57Thickness; }
  Double_t GetWrapThick(void)       const { return fWrapThick;}
  Int_t GetPVBARNSubSect(void)      const { return fPVBARNSubSect;}
  Int_t *GetPVBARNSectorsPhi(void)  const { return fPVBARNSectorsPhi;}
  Int_t GetNTilesPerSubSector(void) const { return fNTilesPerSubSector;}
  Int_t GetNTotalElements(void)     const { return fNTotalElements;}

protected:

  IlcPVBARGeometry(const Text_t* name, const Text_t* title="") ;
private:


  Float_t                  fAngle ;          // Position angles between modules
  Float_t                  fPVBARParams[4] ;  // Half-sizes of PVBAR trapecoid
  Float_t                  fIPtoUpperCPVsurface; // Minimal distance from IP to PVBAR
  Float_t                  fCrystalShift ;   //Distance from crystal center to front surface
  Float_t                  fCryCellShift ;   //Distance from crystal center to front surface
  TObjArray               *fRotMatrixArray ; // Liste of rotation matrices (one per PVBAR module)
  Float_t fModuleCenter[5][3];   // xyz-position of the module center
  Float_t fModuleAngle[5][3][2]; // polar and azymuth angles for 3 axes of modules

  Double_t fPVBARRmin;
  Double_t fPVBARRmax;
  Double_t fPVBARLength;
  Double_t fPVBARTileScintThickness; //cm
  Double_t fPVBARTileSF57Thickness; //cm
  Double_t fWrapThick; //cm
  Int_t    fPVBARNSubSect;
  Int_t   *fPVBARNSectorsPhi; //[fPVBARNSubSect]
  Int_t    fNTilesPerSubSector;
  Int_t    fNTotalElements;

  static IlcPVBARGeometry * fgGeom ; // pointer to the unique instance of the singleton 
  static Bool_t fgInit ;            // Tells if geometry has been succesfully set up 

  ClassDef(IlcPVBARGeometry,3)       // PVBAR geometry class 

} ;

#endif // IlcPVBARGEOMETRY_H

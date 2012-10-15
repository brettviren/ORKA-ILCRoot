#ifndef ILCPVBARSUPPORTGEOMETRY_H
#define ILCPVBARSUPPORTGEOMETRY_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcPVBARSupportGeometry.h 14911 2006-08-28 10:01:56Z kharlov $ */

//_________________________________________________________________________
// Geometry class  for PVBAR : Support which holds all PVBAR modules.
// Its data members provide geometry parametrization of
// the PVBAR support which can be changed in the constructor only.
// Author:   Yuri Kharlov (IHEP, Protvino)
// 13 November 2000

// --- ROOT system ---

#include "TObject.h" 

// --- IlcRoot header files ---

class IlcPVBARSupportGeometry : public TObject {

public: 

  IlcPVBARSupportGeometry();
  virtual ~IlcPVBARSupportGeometry(void) {}

  Float_t GetRailOuterSize(Int_t index)     const { return fRailOuterSize[index]; }
  Float_t GetRailPart1    (Int_t index)     const { return fRailPart1[index]; }
  Float_t GetRailPart2    (Int_t index)     const { return fRailPart2[index]; }
  Float_t GetRailPart3    (Int_t index)     const { return fRailPart3[index]; }
  Float_t GetRailPos      (Int_t index)     const { return fRailPos[index]; }
  Float_t GetRailLength   ()                const { return fRailLength; }
  Float_t GetDistanceBetwRails()            const { return fDistanceBetwRails; }
  Float_t GetRailsDistanceFromIP()          const { return fRailsDistanceFromIP; }
  Float_t GetRailRoadSize (Int_t index)     const { return fRailRoadSize[index]; }
  Float_t GetCradleWallThickness()          const { return fCradleWallThickness; }
  Float_t GetCradleWall   (Int_t index)     const { return fCradleWall[index]; }
  Float_t GetCradleWheel  (Int_t index)     const { return fCradleWheel[index]; }
 
private:

  Float_t fRailOuterSize[3] ;       // Outer size of a rail                 +-------+
  Float_t fRailPart1[3] ;           // Upper & bottom parts of the rail     |--+ +--|
  Float_t fRailPart2[3] ;           // Vertical middle parts of the rail       | |
  Float_t fRailPart3[3] ;           // Vertical upper parts of the rail        | |
  Float_t fRailPos[3] ;             // Rail position vs. the ILC center   |--+ +--|
  Float_t fRailLength ;             // Length of the rail under the support +-------+
  Float_t fDistanceBetwRails ;      // Distance between rails
  Float_t fRailsDistanceFromIP ;    // Distance of rails from IP
  Float_t fRailRoadSize[3] ;        // Outer size of the dummy box with rails
  Float_t fCradleWallThickness ;    // PVBAR cradle wall thickness
  Float_t fCradleWall[5] ;          // Size of the wall of the PVBAR cradle (shape TUBS)
  Float_t fCradleWheel[3] ;         // "Wheels" by which the cradle rolls over the rails

  ClassDef(IlcPVBARSupportGeometry,1) // PVBAR support geometry class 

} ;

#endif // IlcPVBARSUPPORTGEOMETRY_H

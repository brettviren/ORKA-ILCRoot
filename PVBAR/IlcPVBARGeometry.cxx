/**************************************************************************
 * 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * Author: The International Lepton Collider Off-line Project.            *
 *                                                                        *
 * Part of the code has been developed by ILC Off-line Project.         *
 * Contributors are mentioned in the code where appropriate.              *
 *                                                                        *
 * Permission to use, copy, modify and distribute this software and its   *
 * documentation strictly for non-commercial purposes is hereby granted   *
 * without fee, provided that the above copyright notice appears in all   *
 * copies and that both the copyright notice and this permission notice   *
 * appear in the supporting documentation. The authors make no claims     *
 * about the suitability of this software for any purpose. It is          *
 * provided "as is" without express or implied warranty.                  *
 **************************************************************************/

/* $Id: IlcPVBARGeometry.cxx 46104 2010-12-16 18:00:01Z policheh $ */

//_________________________________________________________________________
// Geometry class  for PVBAR : singleton  
// PVBAR consists of the electromagnetic calorimeter (EMCA)
//                  
// -- Author: Yves Schutz (SUBATECH) & Dmitri Peressounko (RRC "KI" & SUBATECH)

// --- ROOT system ---

#include "TVector3.h"
#include "TRotation.h" 
#include "TParticle.h"
#include <TGeoManager.h>
#include <TGeoMatrix.h>

// --- Standard library ---

// --- IlcRoot header files ---
#include "IlcLog.h"
#include "IlcPVBARGeometry.h"
#include "IlcPVBAREMCAGeometry.h" 
#include "IlcPVBARRecPoint.h"

ClassImp(IlcPVBARGeometry)

// these initialisations are needed for a singleton
IlcPVBARGeometry  * IlcPVBARGeometry::fgGeom = 0 ;
Bool_t             IlcPVBARGeometry::fgInit = kFALSE ;

//____________________________________________________________________________
IlcPVBARGeometry::IlcPVBARGeometry() : 
                    IlcPVBARGeoUtils(),
	            fAngle(0.f),
	            fIPtoUpperCPVsurface(0),
		    fCrystalShift(0),
		    fCryCellShift(0),
	            fRotMatrixArray(0),
	            fPVBARRmin(0.),
	            fPVBARRmax(0.),
	            fPVBARLength(0.),
	            fPVBARTileScintThickness(0.),
	            fPVBARTileSF57Thickness(0.),
	            fWrapThick(0.),
	            fPVBARNSubSect(0),
	            fPVBARNSectorsPhi(0),
	            fNTilesPerSubSector(0),
	            fNTotalElements(0)
{
    // default ctor 
    // must be kept public for root persistency purposes, but should never be called by the outside world
    fgGeom          = 0 ;

    fPVBARParams[0] = 0.;
    fPVBARParams[1] = 0.;
    fPVBARParams[2] = 0.;
    fPVBARParams[3] = 0.;
}  

//____________________________________________________________________________
IlcPVBARGeometry::IlcPVBARGeometry(const IlcPVBARGeometry & rhs)
		    : IlcPVBARGeoUtils(rhs),
		      fAngle(rhs.fAngle),
		      fIPtoUpperCPVsurface(rhs.fIPtoUpperCPVsurface),
		      fCrystalShift(rhs.fCrystalShift),
		      fCryCellShift(rhs.fCryCellShift),
		      fRotMatrixArray(0),
		      fPVBARRmin(rhs.fPVBARRmin),
		      fPVBARRmax(rhs.fPVBARRmax),
		      fPVBARLength(rhs.fPVBARLength),
		      fPVBARTileScintThickness(rhs.fPVBARTileScintThickness),
		      fPVBARTileSF57Thickness(rhs.fPVBARTileSF57Thickness),
		      fWrapThick(rhs.fWrapThick),
		      fPVBARNSubSect(rhs.fPVBARNSubSect),
		      fPVBARNSectorsPhi(rhs.fPVBARNSectorsPhi),
		      fNTilesPerSubSector(rhs.fNTilesPerSubSector),
		      fNTotalElements(rhs.fNTotalElements)
{
  Fatal("cpy ctor", "not implemented") ; 
}

//____________________________________________________________________________
IlcPVBARGeometry::IlcPVBARGeometry(const Text_t* name, const Text_t* title) 
	          : IlcPVBARGeoUtils(name, title),
	            fAngle(0.f),
	            fIPtoUpperCPVsurface(0),
		    fCrystalShift(0),
		    fCryCellShift(0),
	            fRotMatrixArray(0),
	            fPVBARRmin(0.),
	            fPVBARRmax(0.),
	            fPVBARLength(0.),
	            fPVBARTileScintThickness(0.),
	            fPVBARTileSF57Thickness(0.),
	            fWrapThick(0.),
	            fPVBARNSubSect(0),
	            fPVBARNSectorsPhi(0),
	            fNTilesPerSubSector(0),
	            fNTotalElements(0)
{ 
  // ctor only for internal usage (singleton)
  Init() ; 
  fgGeom = this;
}

//____________________________________________________________________________
IlcPVBARGeometry::~IlcPVBARGeometry(void)
{
  // dtor

  if (fRotMatrixArray) fRotMatrixArray->Delete() ; 
  if (fRotMatrixArray) delete fRotMatrixArray ; 
}

//____________________________________________________________________________
void IlcPVBARGeometry::Init(void)
{
  // Initializes the PVBAR parameters :
  //  IHEP is the Protvino CPV (cathode pad chambers)
  
  fgInit     = kTRUE ; 

  fPVBARRmin = 75.;
  fPVBARRmax = 129.;
  fPVBARLength = 350.;
  fPVBARTileScintThickness = 0.15; //cm
  fPVBARTileSF57Thickness = 0.2; //cm
  fWrapThick = 0.01; //cm
  fPVBARNSubSect = 10;
  fPVBARNSectorsPhi = new Int_t[fPVBARNSubSect] ;

  Int_t nSect[10] = {48, 51, 54, 58, 61, 64, 68, 71, 74, 78};
  for(Int_t idx=0; idx<fPVBARNSubSect; idx++) fPVBARNSectorsPhi[idx] = nSect[idx];

  fNTilesPerSubSector = 15;

  fNTotalElements=0;
  for(Int_t idx=0; idx<fPVBARNSubSect; idx++)
    fNTotalElements += fPVBARNSectorsPhi[idx]*fNTilesPerSubSector;

}

//____________________________________________________________________________
IlcPVBARGeometry *  IlcPVBARGeometry::GetInstance() 
{ 
  // Returns the pointer of the unique instance; singleton specific
  
  return static_cast<IlcPVBARGeometry *>( fgGeom ) ; 
}

//____________________________________________________________________________
IlcPVBARGeometry *  IlcPVBARGeometry::GetInstance(const Text_t* name, const Text_t* title) 
{
  // Returns the pointer of the unique instance
  // Creates it with the specified options (name, title) if it does not exist yet

  IlcPVBARGeometry * rv = 0  ; 
  if ( fgGeom == 0 ) {
    if ( strcmp(name,"") == 0 ) 
      rv = 0 ;
    else {    
      fgGeom = new IlcPVBARGeometry(name, title) ;
      if ( fgInit )
	rv = (IlcPVBARGeometry * ) fgGeom ;
      else {
	rv = 0 ; 
	delete fgGeom ; 
	fgGeom = 0 ; 
      }
    }
  }
  else {
    if ( strcmp(fgGeom->GetName(), name) != 0 ) 
      ::Error("GetInstance", "Current geometry is %s. You cannot call %s", 
		      fgGeom->GetName(), name) ; 
    else
      rv = (IlcPVBARGeometry *) fgGeom ; 
  } 
  return rv ; 
}

//____________________________________________________________________________
void IlcPVBARGeometry::GetGlobal(const IlcRecPoint* , TVector3 & ) const
{
  IlcFatal(Form("Please use GetGlobalPVBAR(recPoint,gpos) instead of GetGlobal!"));
}

//____________________________________________________________________________
void IlcPVBARGeometry::GetGlobalPVBAR(const IlcPVBARRecPoint* recPoint, TVector3 & gpos) const
{
  // Calculates the coordinates of a RecPoint and the error matrix in the ILC global coordinate system
 
  const IlcPVBARRecPoint * tmpPVBAR = recPoint ;  
  TVector3 localposition ;

  tmpPVBAR->GetLocalPosition(gpos) ;

  if (!gGeoManager){
    IlcFatal("Geo manager not initialized\n");
  }
  //construct module name
  char path[100] ; 
  Double_t dy ;
  if(tmpPVBAR->IsEmc()){
    TString spath="/ILCM_1/PVBAR_%d/PEMC_1/PCOL_1/PTIO_1/PCOR_1/PAGA_1/PTII_1";
    snprintf(path,spath.Length(),spath.Data(),tmpPVBAR->GetPVBARMod()) ;
//    sprintf(path,"/ILCM_1/PVBAR_%d",tmpPVBAR->GetPVBARMod()) ;
    dy=fCrystalShift ;
  }
  else{
    TString spath="/ILCM_1/PVBAR_%d/PCPV_1";
    snprintf(path,spath.Length(),spath.Data(),tmpPVBAR->GetPVBARMod()) ;
    dy= GetCPVBoxSize(1)/2. ; //center of CPV module 
  }
  Double_t pos[3]={gpos.X(),gpos.Y()-dy,gpos.Z()} ;
  if(tmpPVBAR->IsEmc())
    pos[2]=-pos[2] ; //Opposite z directions in EMC matrix and local frame!!!
  Double_t posC[3] = {};
  //now apply possible shifts and rotations
  if (!gGeoManager->cd(path)){
    IlcFatal("Geo manager can not find path \n");
  }
  TGeoHMatrix *m = gGeoManager->GetCurrentMatrix();
  if (m){
     m->LocalToMaster(pos,posC);
  }
  else{
    IlcFatal("Geo matrixes are not loaded \n") ;
  }
  gpos.SetXYZ(posC[0],posC[1],posC[2]) ;

}
//____________________________________________________________________________

void IlcPVBARGeometry::GetModuleCenter(TVector3& center, 
				      const char *det,
				      Int_t module) const
{
  // Returns a position of the center of the CPV or EMC module
  // in ideal (not misaligned) geometry
  Float_t rDet = 0.;
  if      (strcmp(det,"CPV") == 0) rDet  = GetIPtoCPVDistance   ();
  else if (strcmp(det,"EMC") == 0) rDet  = GetIPtoCrystalSurface();
  else 
    IlcFatal(Form("Wrong detector name %s",det));

  Float_t angle = 0.;//GetPVBARAngle(module); // (40,20,0,-20,-40) degrees
  angle *= TMath::Pi()/180;
  angle += 3*TMath::Pi()/2.;
  center.SetXYZ(rDet*TMath::Cos(angle), rDet*TMath::Sin(angle), 0.);
}


//____________________________________________________________________________
Bool_t IlcPVBARGeometry::AbsToRelNumbering(Int_t absId, Int_t * relid) const
{
  
  // Converts the absolute Id number into the following array
  //  relid[0] = PVBAR subsector 1:fPVBARNSubSect
  //  relid[1] = PVBAR sector 1:fPVBARNSectorsPhi
  //  relid[2] = tile number inside PVBAR subsector
  //  relid[3] = tile type: 0 Cer; 1 Sci
  
  Bool_t rv  = kTRUE ;

  Int_t TotalSect=0;
  for(Int_t idx=0; idx<GetPVBARNSubSect(); idx++)
    TotalSect += GetPVBARNSectorsPhi()[idx]*GetNTilesPerSubSector();
  
  relid[3] = 0;
  
  if(absId > TotalSect){
    relid[3] = 1;
    absId -= TotalSect;
  }

  Int_t SubSect=1;
  while(absId>GetPVBARNSectorsPhi()[SubSect-1]*GetNTilesPerSubSector()){
    absId -= GetPVBARNSectorsPhi()[SubSect-1]*GetNTilesPerSubSector();
    SubSect++;
  }
  relid[0] = SubSect;

  Int_t Sect=1;
  while(absId>GetNTilesPerSubSector()){
    absId -= GetNTilesPerSubSector();
    Sect++;
  }
  relid[1] = Sect;

  relid[2] = absId;

  return rv;  

}

//____________________________________________________________________________
Bool_t IlcPVBARGeometry::RelToAbsNumbering(const Int_t * relid, Int_t &  absId) const
{
  // Converts the relative array Id number into the absolute Id number
  //  relid[0] = PVBAR subsector 1:fPVBARNSubSect
  //  relid[1] = PVBAR sector 1:fPVBARNSectorsPhi
  //  relid[2] = tile number inside PVBAR subsector
  //  relid[3] = tile type: 0 Cer; 1 Sci

  Bool_t rv = kTRUE ;

  absId = 0;
  for(Int_t idx=0; idx<relid[0]-1; idx++)
    absId += GetPVBARNSectorsPhi()[idx]*GetNTilesPerSubSector();

  for(Int_t idx=0; idx<relid[1]-1; idx++)
    absId += GetNTilesPerSubSector();

  absId += relid[2];
  
  if(relid[3])
    for(Int_t idx=0; idx<GetPVBARNSubSect(); idx++)
      absId += GetPVBARNSectorsPhi()[idx]*GetNTilesPerSubSector();
 
  return rv;

}


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
	            fPVBARAngle(0),
	            fIPtoUpperCPVsurface(0),
		    fCrystalShift(0),
		    fCryCellShift(0),
	            fRotMatrixArray(0),
	            fPVBARRmin(0.),
	            fPVBARRmax(0.),
	            fPVBARLength(0.),
	            fPVBARTileScintThickness(0.),
	            fPVBARTileSF57Thickness(0.),
	            fPVBARNSlicePhi(0)
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
		      fPVBARAngle(0),
		      fIPtoUpperCPVsurface(rhs.fIPtoUpperCPVsurface),
		      fCrystalShift(rhs.fCrystalShift),
		      fCryCellShift(rhs.fCryCellShift),
		      fRotMatrixArray(0),
		      fPVBARRmin(rhs.fPVBARRmin),
		      fPVBARRmax(rhs.fPVBARRmax),
		      fPVBARLength(rhs.fPVBARLength),
		      fPVBARTileScintThickness(rhs.fPVBARTileScintThickness),
		      fPVBARTileSF57Thickness(rhs.fPVBARTileSF57Thickness),
		      fPVBARNSlicePhi(rhs.fPVBARNSlicePhi)
{
  Fatal("cpy ctor", "not implemented") ; 
}

//____________________________________________________________________________
IlcPVBARGeometry::IlcPVBARGeometry(const Text_t* name, const Text_t* title) 
	          : IlcPVBARGeoUtils(name, title),
	            fAngle(0.f),
	            fPVBARAngle(0),
	            fIPtoUpperCPVsurface(0),
		    fCrystalShift(0),
		    fCryCellShift(0),
	            fRotMatrixArray(0),
	            fPVBARRmin(0.),
	            fPVBARRmax(0.),
	            fPVBARLength(0.),
	            fPVBARTileScintThickness(0.),
	            fPVBARTileSF57Thickness(0.),
	            fPVBARNSlicePhi(0)
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
  if (fPVBARAngle     ) delete[] fPVBARAngle ; 
}

//____________________________________________________________________________
void IlcPVBARGeometry::Init(void)
{
  // Initializes the PVBAR parameters :
  //  IHEP is the Protvino CPV (cathode pad chambers)
  
  fgInit     = kTRUE ; 

  fPVBARRmin = 75.;
  fPVBARRmax = 127.5;
  fPVBARLength = 350.;
  fPVBARNSlicePhi = 48;
  fPVBARTileScintThickness = 0.15; //cm
  fPVBARTileSF57Thickness = 0.2; //cm


//   fAngle        = 20;
// 
//   
//   fPVBARAngle = new Float_t[fNModules] ;
//   
//   const Float_t * emcParams = fGeometryEMCA->GetEMCParams() ;
//   
//   fPVBARParams[0] =  TMath::Max((Double_t)fGeometryCPV->GetCPVBoxSize(0)/2., 
//  			       (Double_t)(emcParams[0] - (emcParams[1]-emcParams[0])*
// 					  fGeometryCPV->GetCPVBoxSize(1)/2/emcParams[3]));
//   fPVBARParams[1] = emcParams[1] ;
//   fPVBARParams[2] = TMath::Max((Double_t)emcParams[2], (Double_t)fGeometryCPV->GetCPVBoxSize(2)/2.);
//   fPVBARParams[3] = emcParams[3] + fGeometryCPV->GetCPVBoxSize(1)/2. ;
//   
//   fIPtoUpperCPVsurface = fGeometryEMCA->GetIPtoOuterCoverDistance() - fGeometryCPV->GetCPVBoxSize(1) ;
// 
//   //calculate offset to crystal surface
//   const Float_t * inthermo = fGeometryEMCA->GetInnerThermoHalfSize() ;
//   const Float_t * strip = fGeometryEMCA->GetStripHalfSize() ;
//   const Float_t * splate = fGeometryEMCA->GetSupportPlateHalfSize();
//   const Float_t * crystal = fGeometryEMCA->GetCrystalHalfSize() ;
//   const Float_t * pin = fGeometryEMCA->GetAPDHalfSize() ;
//   const Float_t * preamp = fGeometryEMCA->GetPreampHalfSize() ;
//   fCrystalShift=-inthermo[1]+strip[1]+splate[1]+crystal[1]-fGeometryEMCA->GetAirGapLed()/2.+pin[1]+preamp[1] ;
//   fCryCellShift=crystal[1]-(fGeometryEMCA->GetAirGapLed()-2*pin[1]-2*preamp[1])/2;
//  
//   Int_t index ;
//   for ( index = 0; index < fNModules; index++ )
//     fPVBARAngle[index] = 0.0 ; // Module position angles are set in CreateGeometry()
//   
//   fRotMatrixArray = new TObjArray(fNModules) ; 
// 
//   // Geometry parameters are calculated
// 
//   SetPVBARAngles();
//   Double_t const kRADDEG = 180.0 / TMath::Pi() ;
//   Float_t r = GetIPtoOuterCoverDistance() + fPVBARParams[3] - GetCPVBoxSize(1) ;
//   for (Int_t iModule=0; iModule<fNModules; iModule++) {
//     fModuleCenter[iModule][0] = r * TMath::Sin(fPVBARAngle[iModule] / kRADDEG );
//     fModuleCenter[iModule][1] =-r * TMath::Cos(fPVBARAngle[iModule] / kRADDEG );
//     fModuleCenter[iModule][2] = 0.;
//     
//     fModuleAngle[iModule][0][0] =  90;
//     fModuleAngle[iModule][0][1] =   fPVBARAngle[iModule];
//     fModuleAngle[iModule][1][0] =   0;
//     fModuleAngle[iModule][1][1] =   0;
//     fModuleAngle[iModule][2][0] =  90;
//     fModuleAngle[iModule][2][1] = 270 + fPVBARAngle[iModule];
//   }

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
void IlcPVBARGeometry::SetPVBARAngles() 
{ 
  // Calculates the position of the PVBAR modules in ILC global coordinate system
  // in ideal geometry
  
  Double_t const kRADDEG = 180.0 / TMath::Pi() ;
  Float_t pphi =  2 * TMath::ATan( GetOuterBoxSize(0)  / ( 2.0 * GetIPtoUpperCPVsurface() ) ) ;
  pphi *= kRADDEG ;
  if (pphi > fAngle){ 
    IlcError(Form("PVBAR modules overlap!\n pphi = %f fAngle = %f", 
		  pphi, fAngle));

  }
  pphi = fAngle;
  
  for( Int_t i = 1; i <= fNModules ; i++ ) {
    Float_t angle = pphi * ( i - fNModules / 2.0 - 0.5 ) ;
    fPVBARAngle[i-1] = -  angle ;
  } 
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

  Float_t angle = GetPVBARAngle(module); // (40,20,0,-20,-40) degrees
  angle *= TMath::Pi()/180;
  angle += 3*TMath::Pi()/2.;
  center.SetXYZ(rDet*TMath::Cos(angle), rDet*TMath::Sin(angle), 0.);
}


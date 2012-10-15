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

/* $Id: IlcPVECGeometry.cxx 25590 2008-05-06 07:09:11Z prsnko $ */

//_________________________________________________________________________
// Geometry class  for PVEC 
// PVEC consists of the electromagnetic calorimeter (EMCA)
// and a charged particle veto (CPV)
// The EMCA/CPV modules are parametrized so that any configuration
// can be easily implemented 
// The title is used to identify the version of CPV used.
//                  
// -- Author: Yves Schutz (SUBATECH) & Dmitri Peressounko (RRC "KI" & SUBATECH)

// --- ROOT system ---

#include "TClonesArray.h"
#include "TVector3.h"
#include "TParticle.h"
#include <TGeoManager.h>
#include <TGeoMatrix.h>

// --- Standard library ---

// --- IlcRoot header files ---
#include "IlcLog.h"
#include "IlcPVECGeometry.h"
#include "IlcPVECEMCAGeometry.h"
#include "IlcPVECCPVGeometry.h"
#include "IlcPVECSupportGeometry.h"
#include "IlcPVECGeoUtils.h"

ClassImp(IlcPVECGeoUtils)

//____________________________________________________________________________
IlcPVECGeoUtils::IlcPVECGeoUtils():
  fGeometry(0x0),fGeometryEMCA(0x0),fGeometryCPV(0x0),fGeometrySUPP(0x0),
  fPVECNLayers(0),fPVECNXtalPhi(0x0),
  fNModules(0),fNCristalsInModule(0),fNPhi(0),fNZ(0),
  fNumberOfCPVPadsPhi(0),fNumberOfCPVPadsZ(0),
  fNCellsXInStrip(0),fNCellsZInStrip(0),fNStripZ(0),
  fCrystalShift(0.),fCryCellShift(0.),fCryStripShift(0.),fCellStep(0.),
  fPadSizePhi(0.),fPadSizeZ(0.),fCPVBoxSizeY(0.),fMisalArray(0x0)
 
{
    // default ctor 
    // must be kept public for root persistency purposes, but should never be called by the outside world
  
  fXtlArrSize[0]=0.;   
  fXtlArrSize[1]=0.;                                                                           
  fXtlArrSize[2]=0.; 
  
  for(Int_t mod=0; mod<5; mod++){
    fEMCMatrix[mod]=0 ;
    for(Int_t istrip=0; istrip<224; istrip++)
      fStripMatrix[mod][istrip]=0 ;
    fCPVMatrix[mod]=0;
    fPVECMatrix[mod]=0 ;
  }

}  

//____________________________________________________________________________
IlcPVECGeoUtils::IlcPVECGeoUtils(const IlcPVECGeoUtils & rhs)
  : TNamed(rhs),
  fGeometry(0x0),fGeometryEMCA(0x0),fGeometryCPV(0x0),fGeometrySUPP(0x0),
  fPVECNLayers(0),fPVECNXtalPhi(0x0),
  fNModules(0),fNCristalsInModule(0),fNPhi(0),fNZ(0),
  fNumberOfCPVPadsPhi(0),fNumberOfCPVPadsZ(0),
  fNCellsXInStrip(0),fNCellsZInStrip(0),fNStripZ(0),
  fCrystalShift(0.),fCryCellShift(0.),fCryStripShift(0.),fCellStep(0.),
  fPadSizePhi(0.),fPadSizeZ(0.),fCPVBoxSizeY(0.),fMisalArray(0x0)
{
  Fatal("cpy ctor", "not implemented") ; 
}

//____________________________________________________________________________
IlcPVECGeoUtils::IlcPVECGeoUtils(const Text_t* name, const Text_t* title) 
    : TNamed(name, title),
  fGeometry(0x0),fGeometryEMCA(0x0),fGeometryCPV(0x0),fGeometrySUPP(0x0),
  fPVECNLayers(0),fPVECNXtalPhi(0x0),
  fNModules(0),fNCristalsInModule(0),fNPhi(0),fNZ(0),
  fNumberOfCPVPadsPhi(0),fNumberOfCPVPadsZ(0),
  fNCellsXInStrip(0),fNCellsZInStrip(0),fNStripZ(0),
  fCrystalShift(0.),fCryCellShift(0.),fCryStripShift(0.),fCellStep(0.),
  fPadSizePhi(0.),fPadSizeZ(0.),fCPVBoxSizeY(0.),fMisalArray(0x0)
{ 
  // ctor only for normal usage 

  fGeometry = new IlcPVECGeometry();
  fGeometryEMCA = new IlcPVECEMCAGeometry() ;
  fGeometryCPV  = new IlcPVECCPVGeometry() ;
  fGeometrySUPP = new IlcPVECSupportGeometry() ;

  fPVECNLayers = fGeometry->GetPVECNLayers();
  fPVECNXtalPhi = new Int_t [fPVECNLayers];
  for(Int_t iLay=1; iLay<=fPVECNLayers; iLay++)
    fPVECNXtalPhi[iLay-1] = fGeometry->GetPVECNXtalPhi(iLay);



  fNModules     = 5;
  fNPhi  = fGeometryEMCA->GetNPhi() ;
  fNZ    = fGeometryEMCA->GetNZ() ;
  fNCristalsInModule = fNPhi*fNZ ;
  fNCellsXInStrip= fGeometryEMCA->GetNCellsXInStrip() ;
  fNCellsZInStrip= fGeometryEMCA->GetNCellsZInStrip() ;
  fNStripZ = fGeometryEMCA->GetNStripZ() ;
  fXtlArrSize[0]=fGeometryEMCA->GetInnerThermoHalfSize()[0] ; //Wery close to the zise of the Xtl set
  fXtlArrSize[1]=fGeometryEMCA->GetInnerThermoHalfSize()[1] ; //Wery close to the zise of the Xtl set
  fXtlArrSize[2]=fGeometryEMCA->GetInnerThermoHalfSize()[2] ; //Wery close to the zise of the Xtl set

  //calculate offset to crystal surface
  const Float_t * inthermo = fGeometryEMCA->GetInnerThermoHalfSize() ;
  const Float_t * strip    = fGeometryEMCA->GetStripHalfSize() ;
  const Float_t * splate   = fGeometryEMCA->GetSupportPlateHalfSize();
  const Float_t * crystal  = fGeometryEMCA->GetCrystalHalfSize() ;
  const Float_t * pin      = fGeometryEMCA->GetAPDHalfSize() ;
  const Float_t * preamp   = fGeometryEMCA->GetPreampHalfSize() ;
  fCrystalShift=-inthermo[1]+strip[1]+splate[1]+crystal[1]-fGeometryEMCA->GetAirGapLed()/2.+pin[1]+preamp[1] ;
  fCryCellShift=crystal[1]-(fGeometryEMCA->GetAirGapLed()-2*pin[1]-2*preamp[1])/2;
  fCryStripShift=fCryCellShift+splate[1] ;
  fCellStep = 2.*fGeometryEMCA->GetAirCellHalfSize()[0] ;

  fNumberOfCPVPadsPhi = fGeometryCPV->GetNumberOfCPVPadsPhi() ;
  fNumberOfCPVPadsZ   = fGeometryCPV->GetNumberOfCPVPadsZ() ;
  fPadSizePhi = fGeometryCPV->GetCPVPadSizePhi() ;
  fPadSizeZ   = fGeometryCPV->GetCPVPadSizeZ() ; 
  fCPVBoxSizeY= fGeometryCPV->GetCPVBoxSize(1) ;

  for(Int_t mod=0; mod<5; mod++){
    fEMCMatrix[mod]=0 ;
    for(Int_t istrip=0; istrip<224; istrip++)
      fStripMatrix[mod][istrip]=0 ;
    fCPVMatrix[mod]=0;
    fPVECMatrix[mod]=0 ;
  }
 
}

//____________________________________________________________________________
IlcPVECGeoUtils & IlcPVECGeoUtils::operator = (const IlcPVECGeoUtils  & /*rvalue*/) { 

  Fatal("assignment operator", "not implemented") ; 
    return *this ;
}

//____________________________________________________________________________
IlcPVECGeoUtils::~IlcPVECGeoUtils(void)
{
  // dtor
  if(fGeometry){
    delete fGeometry; fGeometry = 0 ;
  }
  if(fGeometryEMCA){
    delete fGeometryEMCA; fGeometryEMCA = 0 ;
  }
  if(fGeometryCPV){
    delete fGeometryCPV; fGeometryCPV=0 ;
  }
  if(fGeometrySUPP){
    delete fGeometrySUPP ; fGeometrySUPP=0 ;
  }
  if(fMisalArray){
    delete fMisalArray; fMisalArray=0 ;
  }

  for(Int_t mod=0; mod<5; mod++){
    delete fEMCMatrix[mod] ;
    for(Int_t istrip=0; istrip<224; istrip++)
      delete fStripMatrix[mod][istrip];
    delete fCPVMatrix[mod];
    delete fPVECMatrix[mod];
  }
}
//____________________________________________________________________________
Bool_t IlcPVECGeoUtils::AbsToRelNumbering(Int_t absId, Int_t * relid) const
{
  // Converts the absolute Cell Id number into the following array
  //  relid[0] = PVEC Module number 1 for left endcap, 2 for right endcap
  //  relid[1] = ring number in PVEC module
  //  relid[2] = Xtal number inside PVEC  ring
  //  relid[3] = 0 not used
  
  relid[3] = 0;

  Bool_t rv  = kTRUE ;

  const Int_t nLay = fPVECNLayers;
  Int_t NXtalPhi[nLay];
  Int_t NXtalInModule=0;
  for(Int_t iLay=0; iLay<nLay; iLay++){
    NXtalPhi[iLay] = fPVECNXtalPhi[iLay+1];
    NXtalInModule += NXtalPhi[iLay];
  }

  if(absId>2*NXtalInModule){
      rv = kFALSE;
      return rv;
  }
  
  if(absId<=NXtalInModule)
      relid[0] = 1;
  else{
      relid[0] = 2;
      absId -= NXtalInModule;
  }

  relid[1] = 1;
  for(Int_t iLay=0; iLay<nLay; iLay++){
      if(absId>NXtalPhi[iLay]){
          relid[1]++;
          absId -= NXtalPhi[iLay];
      }
      else
          break;
  
  }
 
  relid[2] = absId;

 return rv ;

}
//____________________________________________________________________________
Bool_t IlcPVECGeoUtils::RelToAbsNumbering(const Int_t * relid, Int_t &  absId) const
{
  // Converts the relative Cell Id number into the absolute cell number
  //  relid[0] = PVEC Module number 1 for left endcap, 2 for right endcap
  //  relid[1] = ring number in PVEC module
  //  relid[2] = Xtal number inside PVEC  ring
  //  relid[3] = 0 not used

  Bool_t rv = kTRUE ;

  absId = 0;
  
  const Int_t nLay = fPVECNLayers;
  Int_t NXtalPhi[nLay];
  Int_t NXtalInModule=0;
  for(Int_t iLay=0; iLay<nLay; iLay++){
    NXtalPhi[iLay] = fPVECNXtalPhi[iLay+1];
    NXtalInModule += NXtalPhi[iLay];
  }

  if(relid[0] == 2) absId += NXtalInModule;
//   IlcInfo(Form("relid[0]: %d AbsId:%d   NXtalInModule: %d", relid[0], absId, NXtalInModule));
  for(Int_t iLay=0; iLay<relid[1]-1; iLay++){
    absId += NXtalPhi[iLay];
//     IlcInfo(Form("relid[1]: %d AbsId:%d   NXtalPhi[iLay]: %d", relid[1], absId, NXtalPhi[iLay]));
  }

  absId += relid[2];
//   IlcInfo(Form("relid[2]: %d AbsId:%d ", relid[2], absId));
  
  return rv;  
}

//____________________________________________________________________________
void IlcPVECGeoUtils::RelPosInModule(const Int_t * relid, Float_t & x, Float_t & z) const 
{
  // Converts the relative numbering into the local PVEC-module (x, z) coordinates

  if(relid[1]==0){ //this is PVEC

    Double_t pos[3]= {0.0,-fCryCellShift,0.}; //Position incide the crystal 
    Double_t posC[3]={0.0,0.0,0.}; //Global position

    //Shift and possibly apply misalignment corrections
    Int_t strip=1+((Int_t) TMath::Ceil((Double_t)relid[2]/fNCellsXInStrip))*fNStripZ-
                (Int_t) TMath::Ceil((Double_t)relid[3]/fNCellsZInStrip) ;
    pos[0]=((relid[2]-1)%fNCellsXInStrip-fNCellsXInStrip/2+0.5)*fCellStep ;
    pos[2]=(-(relid[3]-1)%fNCellsZInStrip+fNCellsZInStrip/2-0.5)*fCellStep ;

    Int_t mod = relid[0] ;
    const TGeoHMatrix * m2 = GetMatrixForStrip(mod, strip) ;
    m2->LocalToMaster(pos,posC);

    //Return to PVEC local system  
    Double_t posL2[3]={posC[0],posC[1],posC[2]};
    const TGeoHMatrix *mPVEC2 = GetMatrixForModule(mod) ;
    mPVEC2->MasterToLocal(posC,posL2);
    x=posL2[0] ;
    z=-posL2[2];
    return ;
  }
  else{//CPV
    //first calculate position with respect to CPV plain 
    Int_t row        = relid[2] ; //offset along x axis
    Int_t column     = relid[3] ; //offset along z axis
    Double_t pos[3]= {0.0,0.0,0.}; //Position incide the CPV printed circuit
    Double_t posC[3]={0.0,0.0,0.}; //Global position
    pos[0] = - ( fNumberOfCPVPadsPhi/2. - row    - 0.5 ) * fPadSizePhi  ; // position of pad  with respect
    pos[2] = - ( fNumberOfCPVPadsZ  /2. - column - 0.5 ) * fPadSizeZ  ; // of center of PVEC module

    //now apply possible shifts and rotations
    const TGeoHMatrix *m = GetMatrixForCPV(relid[0]) ;
    m->LocalToMaster(pos,posC);
    //Return to PVEC local system
    Double_t posL[3]={0.,0.,0.,} ;
    const TGeoHMatrix *mPVEC = GetMatrixForPVEC(relid[0]) ;
    mPVEC->MasterToLocal(posC,posL);
    x=posL[0] ;
    z=posL[1];
    return ;
 
  }
  
}
//____________________________________________________________________________
void IlcPVECGeoUtils::RelPosToAbsId(Int_t module, Double_t x, Double_t z, Int_t & absId) const
{
  // converts local PVEC-module (x, z) coordinates to absId 

  //Calculate AbsId using ideal geometry. Should be sufficient for primary particles calculation
  //(the only place where this method used currently)
  Int_t relid[4]={module,0,1,1} ;
  relid[2] = static_cast<Int_t>(TMath::Ceil( x/ fCellStep + fNPhi / 2.) );
  relid[3] = fNZ+1-static_cast<Int_t>(TMath::Ceil(-z/ fCellStep + fNZ   / 2.) ) ;
  if(relid[2]<1)relid[2]=1 ;
  if(relid[3]<1)relid[3]=1 ;
  if(relid[2]>fNPhi)relid[2]=fNPhi ;
  if(relid[3]>fNZ)relid[3]=fNZ ;
  RelToAbsNumbering(relid,absId) ;


 
}

//____________________________________________________________________________
void IlcPVECGeoUtils::RelPosToRelId(Int_t module, Double_t x, Double_t z, Int_t * relId) const
{
  //Evaluates RelId of the crystall with given coordinates

  Int_t absId ;
  RelPosToAbsId(module, x,z,absId) ;
  AbsToRelNumbering(absId,relId) ;
}

//____________________________________________________________________________
void IlcPVECGeoUtils::RelPosInIlc(Int_t id, TVector3 & pos ) const
{
  // Converts the absolute numbering into the global ILC coordinate system
  
  if (!gGeoManager){
    printf("Geo manager not initialized\n");
    abort();
  }
    
  Int_t relid[4] ;
    
  AbsToRelNumbering(id , relid) ;
    
  //construct module name
  if(relid[1]==0){ //this is EMC
 
    Double_t ps[3]= {0.0,-fCryStripShift,0.}; //Position incide the crystal
    Double_t psC[3]={0.0,0.0,0.}; //Global position
 
    //Shift and possibly apply misalignment corrections
    Int_t strip=1+((Int_t) TMath::Ceil((Double_t)relid[2]/fNCellsXInStrip))*fNStripZ-
                (Int_t) TMath::Ceil((Double_t)relid[3]/fNCellsZInStrip) ;
    ps[0]=((relid[2]-1)%fNCellsXInStrip-fNCellsXInStrip/2+0.5)*fCellStep ;
    ps[2]=(-(relid[3]-1)%fNCellsZInStrip+fNCellsZInStrip/2-0.5)*fCellStep ;
 
    Int_t mod = relid[0] ;
    const TGeoHMatrix * m2 = GetMatrixForStrip(mod, strip) ;
    m2->LocalToMaster(ps,psC);
    pos.SetXYZ(psC[0],psC[1],psC[2]) ; 
 
  }
  else{
    //first calculate position with respect to CPV plain
    Int_t row        = relid[2] ; //offset along x axis
    Int_t column     = relid[3] ; //offset along z axis
    Double_t ps[3]= {0.0,fCPVBoxSizeY/2.,0.}; //Position on top of CPV
    Double_t psC[3]={0.0,0.0,0.}; //Global position
    pos[0] = - ( fNumberOfCPVPadsPhi/2. - row    - 0.5 ) * fPadSizePhi  ; // position of pad  with respect
    pos[2] = - ( fNumberOfCPVPadsZ  /2. - column - 0.5 ) * fPadSizeZ  ; // of center of PVEC module
 
    //now apply possible shifts and rotations
    const TGeoHMatrix *m = GetMatrixForCPV(relid[0]) ;
    m->LocalToMaster(ps,psC);
    pos.SetXYZ(psC[0],psC[1],-psC[2]) ; 
  }
} 

//____________________________________________________________________________
void IlcPVECGeoUtils::Local2Global(Int_t mod, Float_t x, Float_t z,
				   TVector3& globalPosition) const 
{
  Double_t posL[3]={x,-fCrystalShift,-z} ; //Only for EMC!!!
  Double_t posG[3] ;
  const TGeoHMatrix *mPVEC = GetMatrixForModule(mod) ;
  mPVEC->LocalToMaster(posL,posG);
  globalPosition.SetXYZ(posG[0],posG[1],posG[2]) ;
}
//____________________________________________________________________________
void IlcPVECGeoUtils::Global2Local(TVector3& localPosition,
				   const TVector3& globalPosition,
				   Int_t module) const
{
  // Transforms a global position to the local coordinate system
  // of the module 
  //Return to PVEC local system
  Double_t posG[3]={globalPosition.X(),globalPosition.Y(),globalPosition.Z()} ;
  Double_t posL[3]={0.,0.,0.} ;
  const TGeoHMatrix *mPVEC = GetMatrixForModule(module) ;
  if(mPVEC){
    mPVEC->MasterToLocal(posG,posL);
    localPosition.SetXYZ(posL[0],posL[1]+fCrystalShift,-posL[2]) ;  
  }
  else{
    localPosition.SetXYZ(999.,999.,999.) ; //module does not exist in given configuration
  }
 
}
//____________________________________________________________________________
Bool_t IlcPVECGeoUtils::GlobalPos2RelId(TVector3 & global, Int_t * relId){
  //Converts position in global ILC coordinates to relId 
  //returns false if x,z coordinates are beyond PVEC
  //distande to PVEC surface is NOT calculated 
  TVector3 loc ;
  for(Int_t mod=1; mod<=fNModules; mod++){
    Global2Local(loc,global,mod) ;
    //If in Acceptance
    if((TMath::Abs(loc.Z())<fXtlArrSize[2]) && (TMath::Abs(loc.X())<fXtlArrSize[0])){
       RelPosToRelId(mod,loc.X(),loc.Z(),relId);
       return kTRUE ;
    }
  }
  return kFALSE ; 

}
//____________________________________________________________________________
Bool_t IlcPVECGeoUtils::ImpactOnEmc(const TParticle * particle,
       Int_t & moduleNumber, Double_t & z, Double_t & x) const
{
  // Tells if a particle enters PVEC and evaluates hit position
  Double_t vtx[3]={particle->Vx(),particle->Vy(),particle->Vz()} ;
  return ImpactOnEmc(vtx,particle->Theta(),particle->Phi(),moduleNumber,z,x);
}
 
//____________________________________________________________________________
Bool_t IlcPVECGeoUtils::ImpactOnEmc(const Double_t * vtx, Double_t theta, Double_t phi, 
                                  Int_t & moduleNumber, Double_t & z, Double_t & x) const
{
  // calculates the impact coordinates on PVEC of a neutral particle  
  // emitted in the vertex vtx[3] with direction vec(p) in the ILC global coordinate system
  TVector3 p(TMath::Sin(theta)*TMath::Cos(phi),TMath::Sin(theta)*TMath::Sin(phi),TMath::Cos(theta)) ;
  return ImpactOnEmc(vtx,p,moduleNumber,z,x) ;

}
//____________________________________________________________________________
Bool_t IlcPVECGeoUtils::ImpactOnEmc(const Double_t * vtx, const TVector3 &p,
                                  Int_t & moduleNumber, Double_t & z, Double_t & x) const
{
  // calculates the impact coordinates on PVEC of a neutral particle  
  // emitted in the vertex vtx[3] with direction theta and phi in the ILC global coordinate system
  TVector3 v(vtx[0],vtx[1],vtx[2]) ;

  for(Int_t imod=1; imod<=fNModules ; imod++){
    //create vector from (0,0,0) to center of crystal surface of imod module
    Double_t tmp[3]={0.,-fCrystalShift,0.} ;
 
    const TGeoHMatrix *m = GetMatrixForModule(imod) ;
    if(!m) //module does not exist in given configuration
      continue ; 
    Double_t posG[3]={0.,0.,0.} ;
    m->LocalToMaster(tmp,posG);
    TVector3 n(posG[0],posG[1],posG[2]) ; 
    Double_t direction=n.Dot(p) ;
    if(direction<=0.)
      continue ; //momentum directed FROM module
    Double_t fr = (n.Mag2()-n.Dot(v))/direction ;  
    //Calculate direction in module plain
    n-=v+fr*p ;
    n*=-1. ;
    if(TMath::Abs(TMath::Abs(n.Z())<fXtlArrSize[2]) && n.Pt()<fXtlArrSize[0]){
      moduleNumber = imod ;
      z=n.Z() ;
      x=TMath::Sign(n.Pt(),n.X()) ;
      //no need to return to local system since we calcilated distance from module center
      //and tilts can not be significant.
      return kTRUE ;
    }
  }
  //Not in acceptance
  x=0; z=0 ;
  moduleNumber=0 ;
  return kFALSE ;

}
//____________________________________________________________________________
void IlcPVECGeoUtils::GetIncidentVector(const TVector3 &vtx, Int_t module, Float_t x,Float_t z, TVector3 &vInc) const {
  //Calculates vector pointing from vertex to current poisition in module local frame
  //Note that PVEC local system and ILC global have opposite z directions

  Global2Local(vInc,vtx,module) ; 
  vInc.SetXYZ(vInc.X()+x,vInc.Y(),vInc.Z()+z) ;
}
//____________________________________________________________________________
const TGeoHMatrix * IlcPVECGeoUtils::GetMatrixForModule(Int_t mod)const {
  //Provides shift-rotation matrix for module mod

  //If GeoManager exists, take matrixes from it
  if(gGeoManager){
    char path[255] ;
    snprintf(path,255,"/ILCM_1/PVEC_%d/PEMC_1/PCOL_1/PTIO_1/PCOR_1/PAGA_1/PTII_1",mod) ;
    //    sprintf(path,"/ILCM_1/PVEC_%d",relid[0]) ;
    if (!gGeoManager->cd(path)){
      IlcWarning(Form("Geo manager can not find path %s \n",path));
      return 0;
    }
    return gGeoManager->GetCurrentMatrix();
  }
  if(fEMCMatrix[mod-1]){
    return fEMCMatrix[mod-1] ;
  }
  else{
    IlcWarning("Can not find PVEC misalignment matrixes\n") ;
    IlcWarning("Either import TGeoManager from geometry.root or \n");
    IlcWarning("read stored matrixes from IlcESD Header: \n") ;
    IlcWarning("IlcPVECGeoUtils::SetMisalMatrixes(header->GetPVECMisalMatrix()) \n") ; 
    return 0 ;
  }
  return 0 ;
}
//____________________________________________________________________________
const TGeoHMatrix * IlcPVECGeoUtils::GetMatrixForStrip(Int_t mod, Int_t strip)const {
  //Provides shift-rotation matrix for strip unit of the module mod

  //If GeoManager exists, take matrixes from it
  if(gGeoManager){
    char path[255] ;
    snprintf(path,255,"/ILCM_1/PVEC_%d/PEMC_1/PCOL_1/PTIO_1/PCOR_1/PAGA_1/PTII_1/PSTR_%d",mod,strip) ;
    if (!gGeoManager->cd(path)){
      IlcWarning(Form("Geo manager can not find path %s \n",path));
      return 0 ;
    }
    return gGeoManager->GetCurrentMatrix();
  } 
  if(fStripMatrix[mod-1][strip-1]){
    return fStripMatrix[mod-1][strip-1] ;
  }
  else{
    IlcWarning("Can not find PVEC misalignment matrixes\n") ;
    IlcWarning("Either import TGeoManager from geometry.root or \n");
    IlcWarning("read stored matrixes from IlcESD Header: \n") ; 
    IlcWarning("IlcPVECGeoUtils::SetMisalMatrixes(header->GetPVECMisalMatrix()) \n") ;
    return 0 ;
  } 
  return 0 ;
}
//____________________________________________________________________________
const TGeoHMatrix * IlcPVECGeoUtils::GetMatrixForCPV(Int_t mod)const {
  //Provides shift-rotation matrix for CPV of the module mod

  //If GeoManager exists, take matrixes from it
  if(gGeoManager){ 
    char path[255] ;
    //now apply possible shifts and rotations
    snprintf(path,255,"/ILCM_1/PVEC_%d/PCPV_1",mod) ;
    if (!gGeoManager->cd(path)){
      IlcWarning(Form("Geo manager can not find path %s \n",path));
      return 0 ;
    }
    return gGeoManager->GetCurrentMatrix();
  }
  if(fCPVMatrix[mod-1]){
    return fCPVMatrix[mod-1] ;
  }
  else{
    IlcWarning("Can not find PVEC misalignment matrixes\n") ;
    IlcWarning("Either import TGeoManager from geometry.root or \n");
    IlcWarning("read stored matrixes from IlcESD Header: \n") ;  
    IlcWarning("IlcPVECGeoUtils::SetMisalMatrixes(header->GetPVECMisalMatrix()) \n") ;
    return 0 ;
  }
  return 0 ;
} 
//____________________________________________________________________________
const TGeoHMatrix * IlcPVECGeoUtils::GetMatrixForPVEC(Int_t mod)const {
  //Provides shift-rotation matrix for PVEC (EMC+CPV) 

  //If GeoManager exists, take matrixes from it
  if(gGeoManager){

    char path[255] ;
    snprintf(path,255,"/ILCM_1/PVEC_%d",mod) ;
    
    if (!gGeoManager->cd(path)){
      IlcWarning(Form("Geo manager can not find path %s \n",path));
      return 0 ;
    }
    return gGeoManager->GetCurrentMatrix();
  }
  if(fPVECMatrix[mod-1]){
    return fPVECMatrix[mod-1] ;
  }
  else{
    IlcWarning("Can not find PVEC misalignment matrixes\n") ;
    IlcWarning("Either import TGeoManager from geometry.root or \n");
    IlcWarning("read stored matrixes from IlcESD Header:  \n") ;   
    IlcWarning("IlcPVECGeoUtils::SetMisalMatrixes(header->GetPVECMisalMatrix()) \n") ;
    return 0 ;
  }
  return 0 ;
}
//____________________________________________________________________________
void IlcPVECGeoUtils::SetMisalMatrix(const TGeoHMatrix * m, Int_t mod){
  //Fills pointers to geo matrixes
 
  if(fPVECMatrix[mod]){ //have been set already. Can not be changed any more
    return ;
  }
  if(m==NULL) //Matrix for non-existing modules? Remain zero, no need to re-set
    return ;
  fPVECMatrix[mod]= new TGeoHMatrix(*m) ;
  
  //Calculate maxtrices for PTII
  if(!fMisalArray)
    fMisalArray = new TClonesArray("TGeoHMatrix",1120+10) ;
  Int_t nr = fMisalArray->GetEntriesFast() ;
  Double_t rotEMC[9]={1.,0.,0.,0.,0.,-1.,0.,1.,0.} ;
  const Float_t * inthermo = fGeometryEMCA->GetInnerThermoHalfSize() ;
  const Float_t * strip    = fGeometryEMCA->GetStripHalfSize() ;
  const Float_t * covparams = fGeometryEMCA->GetAlCoverParams() ;
  const Float_t * warmcov = fGeometryEMCA->GetWarmAlCoverHalfSize() ;
  Float_t z = fGeometryCPV->GetCPVBoxSize(1) / 2. - warmcov[2] + covparams[3]-inthermo[1] ;
  Double_t locTII[3]={0.,0.,z} ; 
  Double_t globTII[3] ;

  if (fEMCMatrix[mod] == NULL)
    fEMCMatrix[mod] = new((*fMisalArray)[nr])TGeoHMatrix() ;
  nr++ ;
  fEMCMatrix[mod]->SetRotation(rotEMC) ;
  fEMCMatrix[mod]->MultiplyLeft(fPVECMatrix[mod]) ;
  fPVECMatrix[mod]->LocalToMaster(locTII,globTII) ;
  fEMCMatrix[mod]->SetTranslation(globTII) ;
 
  //Now calculate ideal matrixes for strip misalignment.
  //For the moment we can not store them in ESDHeader

  Double_t loc[3]={0.,inthermo[1] - strip[1],0.} ; 
  Double_t glob[3] ;

  Int_t istrip=0 ;
  for(Int_t irow = 0; irow < fGeometryEMCA->GetNStripX(); irow ++){
    loc[0] = (2*irow + 1 - fGeometryEMCA->GetNStripX())* strip[0] ;
    for(Int_t icol = 0; icol < fGeometryEMCA->GetNStripZ(); icol ++){
      loc[2] = (2*icol + 1 - fGeometryEMCA->GetNStripZ()) * strip[2] ;
      fEMCMatrix[mod]->LocalToMaster(loc,glob) ;
      if (fStripMatrix[mod][istrip] == NULL)
	fStripMatrix[mod][istrip] = new((*fMisalArray)[nr])TGeoHMatrix(*(fEMCMatrix[mod])) ; //Use same rotation as PVEC module
      nr++ ;
      fStripMatrix[mod][istrip]->SetTranslation(glob) ;
      istrip++;
    }
  }
 
  //Now calculate CPV matrixes
  const Float_t * emcParams = fGeometryEMCA->GetEMCParams() ;
  Double_t globCPV[3] ;
  Double_t locCPV[3]={0.,0.,- emcParams[3]} ;
  Double_t rot[9]={1.,0.,0.,0.,0.,1.,0.,-1.,0.} ;

  if (fCPVMatrix[mod] == NULL)
    fCPVMatrix[mod] = new((*fMisalArray)[nr])TGeoHMatrix() ;
  nr++ ;
  fCPVMatrix[mod]->SetRotation(rot) ;
  fCPVMatrix[mod]->MultiplyLeft(fPVECMatrix[mod]) ;
  fCPVMatrix[mod]->ReflectY(kFALSE) ;
  fPVECMatrix[mod]->LocalToMaster(locCPV,globCPV) ;
  fCPVMatrix[mod]->SetTranslation(globCPV) ;

}
 

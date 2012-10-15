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

//_________________________________________________________________________
//  IlcESDCaloCluster extension for RSTACK to recalculate cluster 
//  parameters in case of recalibration.
//*--
//*-- Author: Dmitri Peressounko (RRC KI)


// --- ROOT system ---
#include "TVector3.h"
#include "TMath.h"

// --- Standard library ---

// --- IlcRoot header files ---
#include "IlcLog.h" 
#include "IlcRSTACKGeometry.h" 
#include "IlcRSTACKPIDv1.h" 
#include "IlcRSTACKReconstructor.h"
#include "IlcRSTACKEsdCluster.h" 
#include "IlcRSTACKCalibData.h"
#include "IlcESDCaloCells.h"

ClassImp(IlcRSTACKEsdCluster)

//____________________________________________________________________________
IlcRSTACKEsdCluster::IlcRSTACKEsdCluster() : 
  IlcESDCaloCluster(),fRecalibrated(0)
{
  // ctor
}
//____________________________________________________________________________
IlcRSTACKEsdCluster::IlcRSTACKEsdCluster(const IlcESDCaloCluster & clu) : 
  IlcESDCaloCluster(clu),fRecalibrated(0)
{
  // cpy ctor
}

//____________________________________________________________________________
IlcRSTACKEsdCluster::~IlcRSTACKEsdCluster()
{
  // dtor
}
//____________________________________________________________________________
void IlcRSTACKEsdCluster::Recalibrate(IlcRSTACKCalibData * calibData,IlcESDCaloCells *phsCells){
  //If not done yet, apply recalibration coefficients to energies list
  //NOTE that after recalibration fCellsAmpFraction contains not FRACTION but FULL energy 
  
  if(fRecalibrated)
    return ;
  
  if(!calibData)
    return ;
  
  IlcRSTACKGeometry * RSTACKgeom =  IlcRSTACKGeometry::GetInstance() ;
  if(!RSTACKgeom)
    IlcFatal("IlcRSTACKGeometry was not contructed\n") ;
  
  for(Int_t i=0; i<fNCells; i++){
    Int_t relId[4];
    RSTACKgeom->AbsToRelNumbering(fCellsAbsId[i],relId) ;
    Int_t   module = relId[0];
    Int_t   column = relId[3];
    Int_t   row    = relId[2];
    Double_t energy = phsCells->GetCellAmplitude(fCellsAbsId[i]) ;
    fCellsAmpFraction[i]*=energy*calibData->GetADCchannelEmc(module,column,row);
  }
  
  fRecalibrated=kTRUE; 
}
//____________________________________________________________________________
void  IlcRSTACKEsdCluster::EvalAll(Float_t logWeight, TVector3 &vtx){
    //If recalibrated - recalculate all cluster parameters
  if(!fRecalibrated)
    return ;

  EvalEnergy() ; //Energy should be evaluated first
  EvalCoord(logWeight, vtx) ;
  
}
//____________________________________________________________________________
void IlcRSTACKEsdCluster::EvalEnergy(){
  if(!fRecalibrated) // no need to recalibrate
    return ;
    
  fEnergy=0. ;
  for(Int_t iDigit=0; iDigit<fNCells; iDigit++) {
    fEnergy+=fCellsAmpFraction[iDigit] ;
  }
  //Correct for nonlinearity later   
}
//____________________________________________________________________________
void IlcRSTACKEsdCluster::EnergyCorrection(){
  //apply nonlinearity correction 
  fEnergy = IlcRSTACKReconstructor::CorrectNonlinearity(fEnergy) ;
}
//____________________________________________________________________________
void IlcRSTACKEsdCluster::EvalPID(IlcRSTACKPIDv1 * /*pid*/){           
  //re-evaluate identification parameters
//  pid->CalculatePID(fEnergy,fDispersion,fEmcCpvDistance,tof,fPID) ;  
//  pid->CalculatePID(fEnergy,fDispersion,fM20,fM02,fEmcCpvDistance,tof,fPID) ;
}
//____________________________________________________________________________
void IlcRSTACKEsdCluster::EvalCoord(Float_t logWeight, TVector3 &vtx)
{
  // Calculates new center of gravity in the local RSTACK-module coordinates 
  // and tranfers into global ILC coordinates
  // Calculates Dispersion and main axis
  if(!fRecalibrated) // no need to recalibrate
    return ;
 
  Float_t wtot = 0. ;
  Int_t relid[4] ;
  Int_t RSTACKMod=0 ;
  Float_t xMean = 0. ;
  Float_t zMean = 0. ;

  IlcRSTACKGeometry * RSTACKgeom =  IlcRSTACKGeometry::GetInstance() ;
  if(!RSTACKgeom)
    IlcFatal("IlcRSTACKGeometry was not contructed\n") ;

  for(Int_t iDigit=0; iDigit<fNCells; iDigit++) {
    Float_t xi ;
    Float_t zi ;
    RSTACKgeom->AbsToRelNumbering(fCellsAbsId[iDigit], relid) ;
    RSTACKgeom->RelPosInModule(relid, xi, zi);
    RSTACKMod=relid[0] ;
    Double_t ei=fCellsAmpFraction[iDigit] ;
    if (fEnergy>0 && ei>0) {
      Float_t w = TMath::Max( 0., logWeight + TMath::Log(ei/fEnergy) ) ;
      xMean+= xi * w ;
      zMean+= zi * w ;
      wtot += w ;
    }
    else
      IlcError(Form("Wrong energy %f and/or amplitude %f\n", ei, fEnergy));
  }
  if (wtot>0) {
    xMean /= wtot ;
    zMean /= wtot ;
  }
  else
    IlcError(Form("Wrong weight %f\n", wtot));


// Calculates the dispersion and second momenta
  Double_t d=0. ;
  Double_t dxx  = 0.;
  Double_t dzz  = 0.;
  Double_t dxz  = 0.;
  for(Int_t iDigit=0; iDigit < fNCells; iDigit++) {
    Float_t xi ;
    Float_t zi ;
    RSTACKgeom->AbsToRelNumbering(fCellsAbsId[iDigit], relid) ;
    RSTACKgeom->RelPosInModule(relid, xi, zi);
    Double_t ei=fCellsAmpFraction[iDigit] ;
    if (fEnergy>0 && ei>0) {
      Float_t w = TMath::Max( 0., logWeight + TMath::Log(ei/fEnergy) ) ;
      d += w*((xi-xMean)*(xi-xMean) + (zi-zMean)*(zi-zMean) ) ; 
      dxx  += w * xi * xi ;
      dzz  += w * zi * zi ;
      dxz  += w * xi * zi ; 
   }
    else
      IlcError(Form("Wrong energy %f and/or amplitude %f\n", ei, fEnergy));
  }
  
  if (wtot>0) {
    d /= wtot ;
    dxx /= wtot ;
    dzz /= wtot ;
    dxz /= wtot ;
    dxx -= xMean * xMean ;
    dzz -= zMean * zMean ;
    dxz -= xMean * zMean ;
    fM02 =  0.5 * (dxx + dzz) + TMath::Sqrt( 0.25 * (dxx - dzz) * (dxx - dzz) + dxz * dxz )  ;
    fM20 =  0.5 * (dxx + dzz) - TMath::Sqrt( 0.25 * (dxx - dzz) * (dxx - dzz) + dxz * dxz )  ;
  }
  else{
    IlcError(Form("Wrong weight %f\n", wtot));
    d=0. ;
    fM20=0. ;
    fM02=0. ;
  }

  if (d>=0)
    fDispersion = TMath::Sqrt(d) ;
  else    
    fDispersion = 0 ;


  // Correction for the depth of the shower starting point (TDR p 127)  
  Float_t para = 0.925 ; 
  Float_t parb = 6.52 ; 

  TVector3 vInc ;
  RSTACKgeom->GetIncidentVector(vtx,RSTACKMod,xMean,zMean,vInc) ;

  Float_t depthx = 0.; 
  Float_t depthz = 0.;
  if (fEnergy>0&&vInc.Y()!=0.) {
    depthx = ( para * TMath::Log(fEnergy) + parb ) * vInc.X()/TMath::Abs(vInc.Y()) ;
    depthz = ( para * TMath::Log(fEnergy) + parb ) * vInc.Z()/TMath::Abs(vInc.Y()) ;
  }
  else 
    IlcError(Form("Wrong amplitude %f\n", fEnergy));

  xMean-= depthx  ;
  zMean-= depthz  ;

  //Go to the global system
  TVector3 gps ;
  RSTACKgeom->Local2Global(RSTACKMod, xMean, zMean, gps) ;
  fGlobalPos[0]=gps[0] ;
  fGlobalPos[1]=gps[1] ;  
  fGlobalPos[2]=gps[2] ;
}

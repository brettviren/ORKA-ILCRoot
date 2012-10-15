/**************************************************************************
 * Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 *                                                                        *
// Author: The ILC Off-line Project. 
 // Part of the code has been developed by Alice Off-line Project. 
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

/* $Id: IlcDCHParam.cxx,v 1.9 2009/07/17 16:42:37 tassiell Exp $ */

///////////////////////////////////////////////////////////////////////
//  Manager and of geomety  classes for set: TPC                     //
//                                                                   //
//  !sectors are numbered from  0                                     //
//  !pad rows are numbered from 0                                     //
//  
//  12.6.   changed z relative 
//  Origin:  Marian Ivanov, Uni. of Bratislava, ivanov@fmph.uniba.sk // 
//                                                                   //  
///////////////////////////////////////////////////////////////////////

//

#include <IlcDCHParam.h>

#include <TGeoManager.h>
#include <TGeoPhysicalNode.h>
#include "IlcAlignObj.h"
// #include "IlcAlignObjAngles.h"
#include "IlcLog.h"

ClassImp(IlcDCHParam)


//___________________________________________
IlcDCHParam::IlcDCHParam()
{   
  IlcInfo("Create new DCHParam instance");
  SetDefault();
  fFWMaterialType= 0;
}

IlcDCHParam::~IlcDCHParam()
{

}


void IlcDCHParam::SetDefault()
{
  //
  //set default parameters
  //
  //const static  Int_t kMaxRows=600; 
  //
  //sector default parameters
  //
  //  static const  Float_t kInnerRadiusLow = ScaleILC(83.65);

//--------------- DCH ----------------------------------

  static const  Float_t kInnerRadius=6.5;//19.0;//22.5;
  static const  Float_t kOuterRadius=43.;//150.;
  static const  Float_t kInnerWallThickness=0.02;
  static const  Float_t kOuterWallThickness=1.;
  static const  Float_t kEndCapWallThickness=2.;
  static const  Float_t kFWireDiameter=.008;
  static const  Float_t kSWireDiameter=.0025;
  static const  Int_t   kEndCapType=1;
  static const  Int_t   kExperiment=0;  //0=ORKA
  static const  Int_t   kExperimentSubVer=0;  //0=gas instead of hole; 1=hole with sandwitch inner wall

  static const  Int_t   knum_wire_sense=31;
  static const  Int_t   kdelta_num_wire_sense=21;
  static const  Int_t   knsuperlayer=5;
  static const  Int_t   knring=12;
  static const  Float_t kdrop=2.0;
  static const  Float_t klength=0.5*90.;//0.5*300.;
  static const  Float_t kextra_dim=0.;
  static const  Float_t kMaxEndCapDim=TMath::Sqrt(pow(klength,2)+pow(kOuterRadius,2));//92.14;//212.;//

  static const  Float_t kextra_EndCap_dist = 
TMath::Sqrt(pow(kMaxEndCapDim-kEndCapWallThickness,2)-pow(kInnerRadius,2))-(kMaxEndCapDim-kEndCapWallThickness)*klength/kMaxEndCapDim; 
  static const  Float_t kEndCap_Wall_theta_inner=TMath::ASin(kInnerRadius/(kMaxEndCapDim-kEndCapWallThickness))*180./TMath::Pi();//0.;
  static const  Float_t kEndCap_Wall_theta_outer=TMath::ACos(klength/kMaxEndCapDim)*180./TMath::Pi();//0.;
  

//--------------- DCH ----------------------------------


//--------------- DCH ----------------------------------

  SetInnerRadius(kInnerRadius);
  SetOuterRadius(kOuterRadius);
  SetInnerWallThickness(kInnerWallThickness);
  SetOuterWallThickness(kOuterWallThickness);
  SetEndCapWallThickness(kEndCapWallThickness);
  SetFWireDiameter(kFWireDiameter);
  SetSWireDiameter(kSWireDiameter);
  SetEndCapType(kEndCapType);
  SetExperiment(kExperiment);
  SetExperimentSubVer(kExperimentSubVer);
  SetSWireNum(knum_wire_sense);
  SetSDeltaWireNum(kdelta_num_wire_sense);
  SetSuperLayerNum(knsuperlayer);
  SetRingNum(knring);
  SetDrop(kdrop);
  SetLength(klength);
  SetExtraDim(kextra_dim);
  SetMaxEndCapDim(kMaxEndCapDim);
  SetExtraEndCapDist(kextra_EndCap_dist);
  SetEndCapWallThetaIn(kEndCap_Wall_theta_inner);
  SetEndCapWallThetaOut(kEndCap_Wall_theta_outer);
  
  

//--------------- DCH ----------------------------------
  
 } 


void  IlcDCHParam::SetEndCapType(Int_t EndCapType) {
  fEndCapType=EndCapType;
  if (fEndCapType==0) SetTitle("Plane_EndCap");
  else if (fEndCapType==1) SetTitle("Spheric_EndCap");
  else {
    SetTitle("UnKnown_EndCap");
    IlcError("Unknown endcap type!!! It was resetted to default value (plane type)");
    fEndCapType=0;
  }
}

void  IlcDCHParam::SetExperiment(Int_t Experiment) {
  fExperiment=Experiment;
  if (fExperiment==1) {
  SetInnerRadius(40.);
  SetOuterRadius(76.);
  SetInnerWallThickness(0.0125);
  //SetOuterWallThickness(kOuterWallThickness);
  //SetEndCapWallThickness(kEndCapWallThickness);
  SetFWireDiameter(.008);
  SetSWireDiameter(.0015);
  SetSWireNum(140);
  SetSDeltaWireNum(12);
  SetSuperLayerNum(8);
  SetRingNum(7);
  SetDrop(4.0);
  SetLength(0.5*100.);
  SetMaxEndCapDim(TMath::Sqrt(pow(flength,2)+pow(fOuterRadius,2)));
  SetExtraEndCapDist(TMath::Sqrt(pow(fMaxEndCapDim-fEndCapWallThickness,2)-pow(fInnerRadius,2))-(fMaxEndCapDim-fEndCapWallThickness)*flength/fMaxEndCapDim);
  SetEndCapWallThetaIn(TMath::ASin(fInnerRadius/(fMaxEndCapDim-fEndCapWallThickness))*180./TMath::Pi());
  SetEndCapWallThetaOut(TMath::ACos(flength/fMaxEndCapDim)*180./TMath::Pi());
  }
}

void  IlcDCHParam::SetExperimentSubVer(Int_t ExperimentSubVer) {
  fExperimentSubVer=ExperimentSubVer;
}

void  IlcDCHParam::SetFWMaterialType(Int_t FWMaterialType) {
  fFWMaterialType=FWMaterialType;
}

          






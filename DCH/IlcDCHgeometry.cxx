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
 
/* $Id: IlcDCHgeometry.cxx,v 1.15 2009/07/17 16:43:25 tassiell Exp $ */ 
 
/////////////////////////////////////////////////////////////////////////////// 
//                                                                           // 
//  DCH geometry class                                                       // 
//                                                                           // 
/////////////////////////////////////////////////////////////////////////////// 

 
 
#include <TError.h> 

#include <TGeoPhysicalNode.h> 
#include <TGeoMatrix.h> 
#include <TGeoManager.h>  
#include <TGeoShape.h>
#include <TGeoTube.h>
#include <TGeoHype.h>
#include <TGeoSphere.h>
#include <TGeoArb8.h>
#include <TGeoCompositeShape.h>
 
#include "IlcRunLoader.h" 
#include "IlcDCHgeometry.h" 
#include "IlcDCHwireposition.h" 
//#include "IlcDCHpadPlane.h" 
 
#include "IlcAlignObj.h" 
// #include "IlcAlignObjAngles.h" 
 
#include "IlcRun.h" 
#include "IlcDCH.h" 
//#include "IlcDCHcalibDB.h" 
//#include "IlcDCHCommonParam.h" 
//#include "IlcMAG.h"

 
ClassImp(IlcDCHgeometry) 
 
//_____________________________________________________________________________ 
 
  // 
  // The geometry constants 
  // 
/*   const Int_t   IlcDCHgeometry::fgkNsect   = kNsect; 
  const Int_t   IlcDCHgeometry::fgkNplan   = kNplan; 
  const Int_t   IlcDCHgeometry::fgkNcham   = kNcham; 
  const Int_t   IlcDCHgeometry::fgkNdet    = kNdet; 
 
  // 
  // Dimensions of the detector 
  // 
 
  // Inner and outer radius of the mother volumes  
//const Float_t IlcDCHgeometry::fgkRmin    = 350.0; 
const Float_t IlcDCHgeometry::fgkRmin    = 400.0; 
 
const Float_t IlcDCHgeometry::fgkRmax    = 550.0; 
const Float_t IlcDCHgeometry::fgkCrack    = 0.09; // percentage of cracks 
 
  // Upper and lower length of the mother volumes  
const Float_t IlcDCHgeometry::fgkZmax1   = 600.0;  
const Float_t IlcDCHgeometry::fgkZmax2   = 600.1;  
 
const Float_t IlcDCHgeometry::fgkTradius = 2.3;  
const Float_t  IlcDCHgeometry::fgkstep=4.6; 
 
 // Parameter of the BTR2 and Box1  mother volumes  
  const Float_t IlcDCHgeometry::fgkSheight[2] = { (fgkRmax - fgkRmin)* TMath::Cos(30.*TMath::Pi()/180.), fgkRmax - fgkRmin};   //Delta R 
 
const Float_t IlcDCHgeometry::fgkSwidth1 = 2*fgkRmin/TMath::Cos(GetAlpha2())*TMath::Sin(GetAlpha1());  //Deltax min 
 const Float_t IlcDCHgeometry::fgkSwidth2 = fgkSwidth1+2*fgkSheight[0]*TMath::Tan(30./180.*TMath::Pi());   //Deltax max 
const Float_t IlcDCHgeometry::fgkSlenTR1 = fgkZmax1*2;//Deltaz for DCH  
const Float_t IlcDCHgeometry::fgkSlenTR2 = 2*186.0; //Dz for each wheel 
 // Trapezius Parameters 
 const Float_t IlcDCHgeometry::fgkTRwidth1 = fgkSwidth1;  //Deltax inner Trapezius 
 const Float_t IlcDCHgeometry::fgkTRwidth2 = fgkSwidth2;   //Deltax outer 
 const Float_t IlcDCHgeometry::fgkTRheight = fgkSheight[0];   //Deltar  
  
// Box Parameters 
const Float_t IlcDCHgeometry::fgkBwidth1 =2*fgkRmin*TMath::Tan(GetAlpha2());     //Deltax Box1 
const Float_t IlcDCHgeometry::fgkBSheight = fgkSheight[1];//height Super Box 
const Float_t IlcDCHgeometry::fgkBheight = fgkBSheight/4;//height each Box1 
 
 // The TUBE (Al)  // Amplification region 
  const Float_t IlcDCHgeometry::fgkCamRmin=0.95*fgkTradius; 
  const Float_t IlcDCHgeometry::fgkCamRmax=fgkTradius; 
  
  const Float_t IlcDCHgeometry::fgkCamH    =  2*fgkCamRmin; 
  // Total height for each chamber 
  const Float_t IlcDCHgeometry::fgkCH[2]      = {fgkTradius*2*kNdets,fgkTradius*2*kNdets+2*fgkTradius}; //Tube box height for each plane 
 
  // Vertical spacing of the chambers 
  const Float_t IlcDCHgeometry::fgkVspace[2] = {(fgkSheight[0]-fgkCH[0]*kNplan)/2., (fgkSheight[1]-fgkCH[1]*kNplan)/2.}; 
  // Horizontal spacing of the chambers 
  const Float_t IlcDCHgeometry::fgkHspace  =   2.0; 
 
const Int_t  IlcDCHgeometry::fgkNtub[2] ={int(fgkTRwidth2/fgkTradius/2),int(fgkBwidth1/fgkTradius/2)}; // //Tubes number 4 plane (tr,box) 

    // Thicknesses of different parts of the chamber frame 
  // Lower aluminum frame 
  const Float_t IlcDCHgeometry::fgkCalT    =   0.3; 
 
  // Additional width of the readout wires 
  const Float_t IlcDCHgeometry::fgkCroW    =   0.005; 
 
  // 
  // Thickness of the the material layers 
  // 
  const Float_t IlcDCHgeometry::fgkAmThick = IlcDCHgeometry::fgkCamH; 
 
  // 
  // Position of the material layers 
  // 
  const Float_t IlcDCHgeometry::fgkAmZpos  =  0.0; 
   
  const Double_t IlcDCHgeometry::fgkTime0Base = Rmin() + CamHght()/2.; 
  const Float_t  IlcDCHgeometry::fgkTime0[kNplan]  = { fgkTime0Base + 0 * (Cheight() + Cspace()),  
                                                  fgkTime0Base + 1 * (Cheight() + Cspace()),  
                                                  fgkTime0Base + 2 * Cheight(),  
                                                  fgkTime0Base + 3 * (Cheight() + Cspace())}; 
 */ 
//_____________________________________________________________________________ 
IlcDCHgeometry::IlcDCHgeometry()
{ 
  // 
  // IlcDCHgeometry default constructor 
  // 
 
  fMatrixArray           = 0; 
  fMatrixCorrectionArray = 0;
  fDCHParam              = 0;
  
//  Init(); 
 
} 
 
//_____________________________________________________________________________ 
IlcDCHgeometry::~IlcDCHgeometry() 
{ 
  // 
  // IlcDCHgeometry destructor 
  // 
 
  delete fMatrixArray; 
  delete fMatrixCorrectionArray; 
 
} 
 


//_____________________________________________________________________________ 
 
 void IlcDCHgeometry::Init() 

{ 
 Double_t gra2deg = TMath::Pi()/180.;
 if ( !fDCHParam ) {
   IlcError("No Parameters found!!!!");
   IlcError("Geometry construction aborted!!!!");
   return;
 }
 
  Double_t inner_radius=fDCHParam->GetInnerRadius(); //142.5549;
  Double_t outer_radius=fDCHParam->GetOuterRadius(); //148.0343;
  Double_t fieldwire_diameter=fDCHParam->GetFWireDiameter();//.01;
  Double_t envelop_Inner_thickness=fDCHParam->GetInnerWallThickness();//0.02;
  Double_t envelop_Outer_thickness=fDCHParam->GetOuterWallThickness(); //.;
  Double_t envelop_EndCap_thickness=fDCHParam->GetEndCapWallThickness();//2.;
  Int_t num_wire_sense=fDCHParam->GetSWireNum(); //80;
  Int_t delta_num_wire_sense=fDCHParam->GetSDeltaWireNum(); //20;
  Int_t nsuperlayer=fDCHParam->GetSuperLayerNum(); //20;
  Int_t nring=fDCHParam->GetRingNum(); //11;
  Double_t drop=fDCHParam->GetDrop(); //2.;
  Double_t length=fDCHParam->GetLength(); //0.5*300.;
  Double_t extra_EndCap_dist = fDCHParam->GetExtraEndCapDist();
  Int_t EndCap_type=fDCHParam->GetEndCapType();
  Double_t max_EndCap_dim=fDCHParam->GetMaxEndCapDim(); //212. used if EndCap_type!=0
  if(EndCap_type==0) {
    length = length-envelop_EndCap_thickness;
    extra_EndCap_dist=0.;
  }
  else if(EndCap_type==1){
    length=(max_EndCap_dim-envelop_EndCap_thickness)*length/max_EndCap_dim; ///TMath::Sqrt(2.);
  /*  extra_EndCap_dist=TMath::Sqrt(pow(max_EndCap_dim-envelop_EndCap_thickness,2)-pow(inner_radius,2))-length; 
    EndCap_Wall_theta_outer=TMath::ACos(length/max_EndCap_dim)/gra2deg;
    EndCap_Wall_theta_inner=TMath::ACos((length+extra_EndCap_dist)/(max_EndCap_dim-envelop_EndCap_thickness))/gra2deg;
*/    
  }
  Double_t radius_ring, radius_ring_0, radius_ring2, radius_ring2_0, theta_ring, delta_radius_ring, ringangle;
  Double_t alfa1, alfa2;
  Double_t epsilon1, epsilon2;
  Double_t thz_Sense_0,thz_Sense_2_0;
  Double_t thz_Sense,z= 0.000000;
  Int_t num_wire;
  Double_t kbase_exag=0;
  radius_ring_0 = inner_radius + envelop_Inner_thickness + 0.5*fieldwire_diameter;
  radius_ring = radius_ring_0 + drop;
  radius_ring2 = radius_ring;
  radius_ring2_0 = radius_ring_0;
  TGeoRotation *pMatrix_SenseII = new TGeoRotation("rot_SenseII");
  TGeoRotation *pMatrix_epsilon1 = new TGeoRotation("epsilon1");
  TGeoRotation *pMatrix_epsilon2 = new TGeoRotation("epsilon2");
  Int_t sign_epsilon=-1;
  Double_t wre[24];
  Double_t radiuss=0;
  Double_t Drd=0.000001;
  //Double_t lenwire_endcap=0.;
  Double_t wri=0.;
  Double_t gri=0.;
  Double_t gre=0.;     
  Double_t rwire=0.;
  Double_t phi=0.;
  IlcDCHwireposition *storeWireData = new IlcDCHwireposition(kTRUE);
  Int_t ncel;
  for (Int_t superlayer=0;superlayer<nsuperlayer;superlayer++) {
    // count_node = 0;
//     count_subsector = 0;
    num_wire=num_wire_sense+superlayer*delta_num_wire_sense;

    if(superlayer>=10 && superlayer<18)  {num_wire=num_wire-4*delta_num_wire_sense;
    }
    if(superlayer>=18 && superlayer<24)  num_wire=num_wire-6*delta_num_wire_sense;

    theta_ring=2.*(Double_t) TMath::Pi()/(3*num_wire);
    phi=2.*(Double_t) TMath::Pi()/(num_wire);
    sign_epsilon*=-1;
    ncel = nring-1;
    storeWireData->AddSuperLayer( ncel, num_wire);
    for(Int_t iring=0; iring< nring+1 ; iring++){
      if ((iring%2)==0){
  	ringangle = 0.;
  	
      }
      else{
  	ringangle = -(1.5*theta_ring);
  	
      }
      kbase_exag = 2.*radius_ring_0*TMath::Sin(theta_ring*0.5);
      fBaseExag[superlayer] = kbase_exag;
      delta_radius_ring = kbase_exag * TMath::Cos(30. * gra2deg);
      if(iring!=nring){
	radius_ring2 += delta_radius_ring;
	radius_ring2_0 += delta_radius_ring;
      }
      alfa1 = TMath::ACos(1.-(drop/radius_ring));
      alfa2 = TMath::ACos(1.-(drop/radius_ring2));
      epsilon1 = TMath::ATan(radius_ring/length*TMath::Sin(alfa1));
      epsilon2 = TMath::ATan(radius_ring2/length*TMath::Sin(alfa2));
      pMatrix_epsilon1->SetAngles(0.0,-sign_epsilon*epsilon1/gra2deg,0.0); 
      pMatrix_epsilon2->SetAngles(0.0,-sign_epsilon*epsilon2/gra2deg,0.0); 
      thz_Sense_0 = ringangle;
      thz_Sense_2_0 = ((theta_ring*1.5) + ringangle);
      if(iring==0){
	radiuss=radius_ring_0;
	wri=radiuss;
	wre[iring]=radiuss+2*fieldwire_diameter;
	rwire=radiuss+fieldwire_diameter;
      }else{ 
	radiuss=radius_ring_0;
	if(iring!=nring){
	  wri=radiuss-fieldwire_diameter;
	  wre[iring]=radiuss+fieldwire_diameter;
	  rwire=radiuss;
	}else{
	  wri=radiuss-2*fieldwire_diameter;
	  wre[iring]=radiuss;
	  rwire=radiuss-fieldwire_diameter;
	}
	gri=wre[iring-1]+Drd;
	gre=wri-Drd;
      }
	if(iring==nring && superlayer!=nsuperlayer-1){
	  gri=wre[iring]+Drd;
	  gre=gri+fieldwire_diameter;
	}
	if(superlayer==nsuperlayer-1 && iring==nring ) {
	  gri=wre[iring]+Drd;//+2*delta_radius_ring;
	  gre=outer_radius-envelop_Outer_thickness;
	}
	if(superlayer==0 && iring==0) {
	  gri=inner_radius + envelop_Inner_thickness;
	  gre=radius_ring_0;
	}
	TGeoCombiTrans *pMat;
	TGeoHMatrix *comb;
	phi=ringangle;
	Double_t x1_Sense_2 = rwire;
	Double_t y1_Sense_2 = 0;
	pMat = new TGeoCombiTrans("id_comb_Sense",x1_Sense_2,y1_Sense_2,z,0);
	comb= new TGeoHMatrix((*pMat));
	comb->SetName("rot");
	comb->RegisterYourself();
	if (iring != 0 && iring!=nring) {
	  //	  storeWireData->InsertWireMatrix(iring-1,0,comb);
	  storeWireData->InsertAlfaEpsilon(iring-1, alfa1, sign_epsilon*epsilon1);
	  storeWireData->InsertRadius(iring-1, radius_ring_0);
	}
	for (Int_t i = 0; i< num_wire; i++){ //num_wire
	  thz_Sense = phi+i*3*theta_ring;
	  pMatrix_SenseII->SetAngles(thz_Sense/gra2deg,0.0,0.0);
	  if (iring != 0 && iring!=nring) 
	    storeWireData->InsertWireMatrix(iring-1,i,new TGeoHMatrix((*pMatrix_SenseII)*(*comb)*(*pMatrix_epsilon1)));
	}
	if(iring!=nring){
	  radius_ring = radius_ring2;
	  radius_ring_0 = radius_ring2_0;  
	}
    }
  
    radius_ring+=fieldwire_diameter;
    radius_ring_0+=fieldwire_diameter;
  
    storeWireData->FillData();
  }
  
  storeWireData->WriteData();
  delete storeWireData;

}

//_____________________________________________________________________________ 
void IlcDCHgeometry::CreateGeometry()
{ 

 TGeoMedium *gas_mix=gGeoManager->GetMedium("DCH_Gas-mix"); 
 TGeoMedium *Al=gGeoManager->GetMedium("DCH_Al"); 
 TGeoMedium *Kapton=gGeoManager->GetMedium("DCH_Kapton"); 
 TGeoMedium *Air=gGeoManager->GetMedium("DCH_Air"); 
 TGeoMedium *CarbonFiber=gGeoManager->GetMedium("DCH_CarbonFiber"); 
 TGeoMedium *Tungsten=gGeoManager->GetMedium("DCH_Tungsten"); 
 TGeoMedium *Polypropylene=gGeoManager->GetMedium("DCH_Polypropylene"); 
 TGeoMedium *Vacuum=gGeoManager->GetMedium("DCH_Vacuum"); 


 TGeoMedium *FWmed=0;

  if ( !fDCHParam ) {
    IlcError("No Parameters found!!!!");
    IlcError("Geometry construction aborted!!!!");
    return;
  }
  TGeoVolume *topILC=0x0;
  TGeoVolume *top=0x0;

  //Int_t Flag_geoDCHg4=fDCHParam->GetFlag_geoDCHg4(); //If the flag is 0  you run Axial DCH with Geant4
  Double_t inner_radius=fDCHParam->GetInnerRadius(); //142.5549;
  Double_t endcap_inner_radius=inner_radius;
  Double_t outer_radius=fDCHParam->GetOuterRadius(); //148.0343;
  Double_t fieldwire_diameter=fDCHParam->GetFWireDiameter();//.01;
  Double_t sensewire_diameter=fDCHParam->GetSWireDiameter(); //.0025;
  Double_t envelop_Inner_thickness=fDCHParam->GetInnerWallThickness();//0.02;
  Double_t envelop_Outer_thickness=fDCHParam->GetOuterWallThickness(); //.;
  Double_t envelop_EndCap_thickness=fDCHParam->GetEndCapWallThickness();//2.;
  Double_t extra_EndCap_dist = fDCHParam->GetExtraEndCapDist();
  Double_t length=fDCHParam->GetLength(); //0.5*300.;
  
  Int_t num_wire_sense=fDCHParam->GetSWireNum(); //80;
  
  Int_t delta_num_wire_sense=fDCHParam->GetSDeltaWireNum(); //20;
  Int_t nsuperlayer=fDCHParam->GetSuperLayerNum(); //20;
  Int_t nring=fDCHParam->GetRingNum(); //11;
  Double_t drop=fDCHParam->GetDrop(); //2.;
  
  //Double_t extra_EndCap_dist=fDCHParam->GetExtraEndCapDist();  
  
  
  Double_t extra_dim=fDCHParam->GetExtraDim();
  Double_t EndCap_Wall_theta_inner=fDCHParam->GetEndCapWallThetaIn(); //0.;
  Double_t EndCap_Wall_theta_outer=fDCHParam->GetEndCapWallThetaOut();//0.;

  
  Int_t EndCap_type=fDCHParam->GetEndCapType();

  Double_t max_EndCap_dim=fDCHParam->GetMaxEndCapDim(); //212. used if EndCap_type!=0

  if (fDCHParam->GetExperiment()==0) {
    topILC = gGeoManager->GetVolume("ILCM");
    if ( !topILC ) {
      topILC = gGeoManager->MakeBox("TOP", 0, 1000., 1000.,1000.);
      gGeoManager->SetTopVolume(topILC);
    }
// // // //     TGeoTube *DCHtube = new TGeoTube(0., outer_radius, length);
// // // // 
// // // //     top = new TGeoVolume("DCH_top",DCHtube,Vacuum);
// // // //     
// // // //     topILC->AddNode(top,1);
  
    top = topILC;
  }
  else if (fDCHParam->GetExperiment()==1) {

    topILC = gGeoManager->GetVolume("ILCM");
    if ( !topILC ) {
      topILC = gGeoManager->MakeBox("TOP", 0, 1000., 1000.,1000.);
      gGeoManager->SetTopVolume(topILC);
    }

    TGeoTube *DCHtube = new TGeoTube(0., outer_radius, max_EndCap_dim);

    top = new TGeoVolume("DCH_top",DCHtube,Vacuum);
    topILC->AddNode(top,1);
  }

  Double_t gra2deg = TMath::Pi()/180.;
  
  if (fDCHParam->GetFWMaterialType()==0) FWmed = Al;
  else if (fDCHParam->GetFWMaterialType()==1) FWmed = Kapton;

  
  //if(Flag_geoDCHg4==0) drop=0;
  
  if(EndCap_type==0) {
    length = length-envelop_EndCap_thickness;
    extra_EndCap_dist=0.;
  }
  else if(EndCap_type==1){
    length=(max_EndCap_dim-envelop_EndCap_thickness)*length/max_EndCap_dim; ///TMath::Sqrt(2.);

/*    extra_EndCap_dist=TMath::Sqrt(pow(max_EndCap_dim-envelop_EndCap_thickness,2)-pow(inner_radius,2))-length;
    // extra_EndCap_dist=(max_EndCap_dim-envelop_EndCap_thickness)-length;
    EndCap_Wall_theta_outer=TMath::ACos(length/(max_EndCap_dim-envelop_EndCap_thickness))/gra2deg;
    //EndCap_Wall_theta_inner=TMath::ACos((length+extra_EndCap_dist)/(max_EndCap_dim-envelop_EndCap_thickness))/gra2deg;
    EndCap_Wall_theta_inner=TMath::ASin((inner_radius+ envelop_Inner_thickness)/(max_EndCap_dim-envelop_EndCap_thickness))/gra2deg;    
*/
  }

  Double_t z=0.;
  Double_t radius_ring, radius_ring_0, radius_ring2, radius_ring2_0, theta_ring, delta_radius_ring, ringangle;
  Double_t alfa1, alfa2;
  Double_t epsilon1, epsilon2;
  Double_t thz_Sense;
  Int_t num_wire,color_ring1,color_ring2,idcopy_cell,idcopy_fw1,idcopy_fw2;
  Int_t count_node, count_subsector;
  Double_t kbase_exag=0;
  Char_t shape_name_FD[50], shape_name_SD[50];
  
  //TGeoVolumeAssembly *sector;

  radius_ring_0 = inner_radius + envelop_Inner_thickness + 0.5*fieldwire_diameter;
  radius_ring = radius_ring_0 + drop;
  radius_ring2 = radius_ring;
  radius_ring2_0 = radius_ring_0;
  TGeoRotation *pMatrix1_SenseII = new TGeoRotation("rot1_SenseII");
  TGeoRotation *pMatrix2_SenseII = new TGeoRotation("rot2_SenseII");
  TGeoRotation *pMatrix3_SenseII = new TGeoRotation("rot3_SenseII");
  TGeoRotation *pMatrix_epsilon1 = new TGeoRotation("epsilon1");
  TGeoRotation *pMatrix_epsilon2 = new TGeoRotation("epsilon2");
  
  Char_t name_tr_rot_SenseII[50];


  TGeoShape *shape_Field;
  TGeoShape *shape_Sense;

  TGeoShape *shape_Cut=0;
  TGeoTranslation *posCut_EndCap=0;

  if(EndCap_type==0){
    shape_Cut = new TGeoTube("EndCap_cut",inner_radius,outer_radius, extra_dim);
    posCut_EndCap = new TGeoTranslation("TR_EndCap",0.,0., /*extra_dim+*/length);
    posCut_EndCap->RegisterYourself();
  }

  else if (EndCap_type==1){
    shape_Cut = new TGeoSphere("EndCap_cut",max_EndCap_dim-envelop_EndCap_thickness,max_EndCap_dim/*+extra_dim*/,EndCap_Wall_theta_inner,EndCap_Wall_theta_outer,0.,360.);
    posCut_EndCap = new TGeoTranslation("TR_EndCap",0.,0.,0.);
    posCut_EndCap->RegisterYourself();
  }
  

  TGeoVolume *vol_tube_FD=0;
  TGeoVolume *vol_tube_SD=0;

  TGeoRotation *leftEndCap = new TGeoRotation("rot_EndCap",0.,180.,0.);
  TGeoCombiTrans *comb_tr_rot_EndCap = new TGeoCombiTrans("tr_r_EndCap");
  (*comb_tr_rot_EndCap) = (*leftEndCap) * (*posCut_EndCap);
  TGeoHMatrix *tr_rot_EndCap = new TGeoHMatrix((*comb_tr_rot_EndCap));
  tr_rot_EndCap->SetName("TR_ROT_EndCap");
  tr_rot_EndCap->RegisterYourself();

  Int_t sign_epsilon=-1;



//******************************new geometry***********
  TGeoShape*wring;
  TGeoShape*gring;
  TGeoVolume*vol_gring;
  TGeoVolume*vol_wring;
  Char_t wshape[50];
  Char_t gshape[50];
  Char_t wvol[50];
  Char_t gvol[50];
    
  Double_t wre[40];
  Double_t epsilonRing[40];
  Double_t radiuss=0;
  Double_t Drd=0.000001;
  Double_t lenwire_endcap=0.,zlenwire_endcap=0.;
  Double_t wri=0.;
  Double_t gri=0.;
  Double_t gre=0.;     
  Double_t rwire=0.;
  Double_t phi=0.;
      //******************************************end********

  


  
  IlcDCHwireposition *storeWireData = new IlcDCHwireposition(kTRUE);
  
  Int_t ncel;
  for (Int_t superlayer=0;superlayer<nsuperlayer;superlayer++) {
    cout <<"Building super layer: "<<superlayer+1<<endl;

    count_node = 0;
    count_subsector = 0;
    //sector = new TGeoVolumeAssembly(Form("SuperLayer_%02d_%02d",superlayer,count_subsector));
     num_wire=num_wire_sense+superlayer*delta_num_wire_sense;
    
     if(superlayer>=10 && superlayer<18) num_wire=num_wire-4*delta_num_wire_sense;
     if(superlayer>=18 && superlayer<24) num_wire=num_wire-6*delta_num_wire_sense;
    
     //     cout<<" SL "<<superlayer<<" nwire "<<num_wire<<endl;
    theta_ring=2.*(Double_t) TMath::Pi()/(3*num_wire);


    phi=2.*(Double_t) TMath::Pi()/(num_wire);
    
    sign_epsilon*=-1;

    
    ncel = nring-1;
    storeWireData->AddSuperLayer( ncel, num_wire);

    for(Int_t iring=0; iring< nring+1 ; iring++){

      if ((iring%2)==0){
  	ringangle = 0.;
  	color_ring1=1 + (((superlayer%2)==0) ? 0 : 3) ;
  	color_ring2=2 + (((superlayer%2)==0) ? 0 : 3) ;
      }
      else{
  	ringangle = -(1.5*theta_ring);
  	color_ring1=2 + (((superlayer%2)==0) ? 0 : 3) ;
  	color_ring2=1 + (((superlayer%2)==0) ? 0 : 3) ;
      }

      kbase_exag = 2.*radius_ring_0*TMath::Sin(theta_ring*0.5);
      fBaseExag[superlayer] = kbase_exag;
      delta_radius_ring = kbase_exag * TMath::Cos(30. * gra2deg);
      if(iring!=nring){
	radius_ring2 += delta_radius_ring;
	radius_ring2_0 += delta_radius_ring;
      }
      alfa1 = TMath::ACos(1.-(drop/radius_ring));
      alfa2 = TMath::ACos(1.-(drop/radius_ring2));
      epsilon1 = TMath::ATan(radius_ring/length*TMath::Sin(alfa1));
      epsilon2 = TMath::ATan(radius_ring2/length*TMath::Sin(alfa2));
      epsilonRing[iring]=epsilon1;
      epsilonRing[iring+1]=epsilon2;
      pMatrix_epsilon1->SetAngles(0.0,-sign_epsilon*epsilon1/gra2deg,0.0); 
      pMatrix_epsilon2->SetAngles(0.0,-sign_epsilon*epsilon2/gra2deg,0.0); 
      

      Double_t cord[3]= {radius_ring_0+2*fieldwire_diameter,0.,0.};
      Double_t dir[3]={0,TMath::Sin(epsilon1),TMath::Cos(epsilon1)};
      Int_t iact=1;
      Double_t step=TGeoShape::Big();
      Double_t* safe = 0;
      if(EndCap_type==0) { 
        zlenwire_endcap=length;
	lenwire_endcap=zlenwire_endcap/TMath::Cos(epsilon1);
	}
      else if (EndCap_type==1) {
        lenwire_endcap=shape_Cut->DistFromOutside(cord,dir,iact,step, safe);
        zlenwire_endcap=TMath::Cos(epsilon1)*lenwire_endcap;
      }
      lenwire_endcap-=fieldwire_diameter*TMath::Sin(epsilon1);


      sprintf(wshape,"wS%dR%d",superlayer,iring);
      sprintf(gshape,"gS%dR%d",superlayer,iring);
      sprintf(wvol,"wvolS%02dR%02d",superlayer,iring);
      sprintf(gvol,"gvolS%02dR%02d",superlayer,iring);

      if(iring==0){
	radiuss=radius_ring_0;
	wri=radiuss+Drd;
	wre[iring]=radiuss+2*fieldwire_diameter;
	rwire=radiuss+fieldwire_diameter;
	//	wring=new TGeoTube(wshape,wri,wre[iring],lenwire_endcap);
	wring=new TGeoHype(wshape,wri,epsilon1/gra2deg,wre[iring],epsilon1/gra2deg,zlenwire_endcap);
	vol_wring=new TGeoVolume(wvol,wring,gas_mix);
	vol_wring->SetLineColor(5);
	vol_wring->SetVisibility(1);
	top->AddNode(vol_wring,1,0);
      }else{
	radiuss=radius_ring_0;
	if(iring!=nring){
	  wri=radiuss-fieldwire_diameter;
	  wre[iring]=radiuss+fieldwire_diameter;
	  rwire=radiuss;
	}else{
	  wri=radiuss-2*fieldwire_diameter;
	  wre[iring]=radiuss;
	  rwire=radiuss-fieldwire_diameter;
	}
	gri=wre[iring-1]+Drd;
	gre=wri-Drd;

	//	gring=new TGeoTube(gshape,gri,gre,lenwire_endcap);
	gring=new TGeoHype(gshape,gri,epsilonRing[iring-1]/gra2deg,gre,epsilon1/gra2deg,zlenwire_endcap);
	vol_gring=new TGeoVolume(gvol,gring,gas_mix);
	vol_gring->SetLineColor(3);
	vol_gring->SetVisibility(1);
	top->AddNode(vol_gring,1,0);

	//	wring=new TGeoTube(wshape,wri,wre[iring],lenwire_endcap);
	wring=new TGeoHype(wshape,wri,epsilon1/gra2deg,wre[iring],epsilon1/gra2deg,zlenwire_endcap);
	vol_wring=new TGeoVolume(wvol,wring,gas_mix);
	vol_wring->SetLineColor(5);
	vol_wring->SetVisibility(1);
	top->AddNode(vol_wring,1,0);
     
      }
      if(iring==0 && superlayer!=0){
	gri=radius_ring_0-fieldwire_diameter;
	gre=radius_ring_0;
	sprintf(gvol,"gvolE%02dR%02d",superlayer,iring+1);
	// cout<<" ************* "<<gvol<<end;
	//	gring=new TGeoTube(gshape,gri,gre,lenwire_endcap-Drd2);
	gring=new TGeoHype(gshape,gri,epsilonRing[nring]/gra2deg,gre,epsilon1/gra2deg,zlenwire_endcap);
	vol_gring=new TGeoVolume(gvol,gring,gas_mix);
	vol_gring->SetLineColor(4);
	vol_gring->SetVisibility(1);
	top->AddNode(vol_gring,1,0);

      }
     

	if(superlayer==nsuperlayer-1 && iring==nring ) {
	  gri=wre[iring]+Drd;//+2*delta_radius_ring;
	  gre=outer_radius-envelop_Outer_thickness;

// // // // 	  //	  gring=new TGeoTube(gshape,gri,gre,length);     
// // // //           gre=TMath::Sqrt(TMath::Tan(epsilon1)*TMath::Tan(epsilon1)*length*length+gri*gri);
// // // // //           gre=outer_radius*1.01-envelop_Outer_thickness;
// // // //           cout << Form("%f %f %f\n",TMath::Sqrt(TMath::Tan(epsilon1)*TMath::Tan(epsilon1)*length*length+gri*gri), outer_radius*1.01-envelop_Outer_thickness, outer_radius*1.015-envelop_Outer_thickness);
// // // //           gring=new TGeoHype(gshape,gri,epsilon1/gra2deg,gre,0.,length);
// // // //           vol_gring=new TGeoVolume(Form("gvolES%02dR%02d",superlayer,nring),gring,gas_mix);

	  gring=new TGeoHype(gshape,gri,epsilon1/gra2deg,gre,0.,length);
	  vol_gring=new TGeoVolume("gvolES23R11",gring,gas_mix);

	  vol_gring->SetLineColor(2);
	  vol_gring->SetVisibility(1);
	  top->AddNode(vol_gring,1,0);
	}
     
	if(superlayer==0 && iring==0) {
	  gri=inner_radius + envelop_Inner_thickness;
	  gre=radius_ring_0;
     
	  //	  gring=new TGeoTube(gshape,gri,gre,length);
	  gring=new TGeoHype(gshape,gri,0,gre,epsilon1/gra2deg,zlenwire_endcap);
	  vol_gring=new TGeoVolume("gvolES00R00",gring,gas_mix);
	  vol_gring->SetLineColor(2);
	  vol_gring->SetVisibility(1);
	  top->AddNode(vol_gring,1,0);
	}
     
	//cout<<gre<<endl;
	//matrix cell shape for geant4------------------------------------

	TGeoCombiTrans *pMat;
	TGeoHMatrix *comb;

	phi=ringangle;

	pMat = new TGeoCombiTrans("id_comb_Sense",rwire,0.,z,0);
	comb= new TGeoHMatrix((*pMat));
	comb->SetName("rot");
	comb->RegisterYourself();

     
	if (iring != 0 && iring!=nring) {
	  storeWireData->InsertAlfaEpsilon(iring-1, alfa1,sign_epsilon*epsilon1);
	  storeWireData->InsertRadius(iring-1, radius_ring_0);
	}
	if(iring==nring)
	  cout<<"Max R on the EndCap for Layer "<<rwire<<endl;




	//---------------End--------------------------------------------
      shape_Field=new TGeoTube("tube_Field", 1.0E-10, fieldwire_diameter*0.5, lenwire_endcap);
      shape_Sense=new TGeoTube("tube_Sense", 1.0E-10, sensewire_diameter*0.5, lenwire_endcap);

      if(iring!=nring){
	sprintf(shape_name_FD,"tubeFD_%d_%d",superlayer,iring);
	vol_tube_FD=new TGeoVolume(shape_name_FD,shape_Field,FWmed);
	vol_tube_FD->SetLineColor(3);
	vol_tube_FD->SetVisibility(1);
      }

      if(iring!=0&&iring!=nring){
	sprintf(shape_name_SD,"tubeSD_%d_%d",superlayer,iring);
	vol_tube_SD=new TGeoVolume(shape_name_SD,shape_Sense,Tungsten);
	vol_tube_SD->SetLineColor(2);
      }

      for (Int_t i = 0; i< num_wire; i++){ //num_wire

  	thz_Sense = phi+i*3*theta_ring;
      
  	sprintf(name_tr_rot_SenseII,"combTR_SenseII_%d_%d_%d",superlayer,iring,i);
  	
  	pMatrix1_SenseII->SetAngles(thz_Sense/gra2deg,0.0,0.0);
  	pMatrix2_SenseII->SetAngles((thz_Sense+theta_ring)/gra2deg,0.0,0.0);
  	pMatrix3_SenseII->SetAngles((thz_Sense+2*theta_ring)/gra2deg,0.0,0.0);

	//matrix of  cell volume for geant4---------------------- 

	TGeoHMatrix *vcomb_tr_rot_SenseII;
	TGeoHMatrix *v2comb_tr_rot_SenseII;
	TGeoHMatrix *v3comb_tr_rot_SenseII;

	vcomb_tr_rot_SenseII = new TGeoHMatrix((*pMatrix1_SenseII)*(*comb)*(*pMatrix_epsilon1));
  	vcomb_tr_rot_SenseII->SetName(name_tr_rot_SenseII);
  	vcomb_tr_rot_SenseII->RegisterYourself();

	v2comb_tr_rot_SenseII = new TGeoHMatrix((*pMatrix2_SenseII)*(*comb)*(*pMatrix_epsilon1));
  	v2comb_tr_rot_SenseII->SetName(name_tr_rot_SenseII);
  	v2comb_tr_rot_SenseII->RegisterYourself();

	v3comb_tr_rot_SenseII = new TGeoHMatrix((*pMatrix3_SenseII)*(*comb)*(*pMatrix_epsilon1));
  	v3comb_tr_rot_SenseII->SetName(name_tr_rot_SenseII);
  	v3comb_tr_rot_SenseII->RegisterYourself();
  
	//end----------------------------------------------------------


	if (iring != 0 && iring!=nring) 
	  storeWireData->InsertWireMatrix(iring-1,i,new TGeoHMatrix(*vcomb_tr_rot_SenseII));
	
        idcopy_cell = i+1;

        idcopy_fw1 = 2*i+1;
      
        idcopy_fw2 = idcopy_fw1 + 1;
	if (iring != 0 && iring!=nring)
	  vol_wring->AddNode(vol_tube_SD,idcopy_cell,vcomb_tr_rot_SenseII);
	if (iring!=nring){
	  vol_wring->AddNode(vol_tube_FD,idcopy_fw1,v2comb_tr_rot_SenseII);
	  vol_wring->AddNode(vol_tube_FD,idcopy_fw2,v3comb_tr_rot_SenseII);
	}

      }
      if(iring!=nring){
	radius_ring = radius_ring2;
	radius_ring_0 = radius_ring2_0;  
      }
    }// end cycle over rings
  
    radius_ring+=fieldwire_diameter;
    radius_ring_0+=fieldwire_diameter;
    storeWireData->FillData();
  }
  
  storeWireData->WriteData();
  delete storeWireData;

  double lengthInnerWall=length+extra_EndCap_dist;
  if(EndCap_type==1){
    double zl=TMath::Sqrt(TMath::Power(max_EndCap_dim-envelop_EndCap_thickness,2)
			  -TMath::Power(inner_radius+envelop_Inner_thickness,2));
    if(zl<lengthInnerWall) lengthInnerWall=zl-Drd;
  }

  if (fDCHParam->GetExperiment()==0) {
// // // //    TGeoShape *innerWall=new TGeoTube("InnerWall",inner_radius*0.985,inner_radius*0.985+envelop_Inner_thickness, lengthInnerWall);
    TGeoShape *innerWall=new TGeoTube("InnerWall",inner_radius,inner_radius+envelop_Inner_thickness, lengthInnerWall);

    TGeoVolume *vol_innerWall=new TGeoVolume("InnerWall",innerWall,CarbonFiber);
    vol_innerWall->SetLineColor(10);
    vol_innerWall->SetVisibility(1);
    top->AddNode(vol_innerWall,1,0);
    if (gIlc->GetModule("VXD")==0x0 && gIlc->GetModule("TARGET")==0x0) {
      IlcWarning("VXD | TARGET detector absent, inserting air volume");
      TGeoVolume *innerHole=new TGeoVolume("InnerHole",new TGeoTube("InnerHole",0.0,inner_radius, lengthInnerWall),Air);
      top->AddNode(innerHole,1,0);
    }
  }
  else if (fDCHParam->GetExperiment()==1) {
    if (fDCHParam->GetExperimentSubVer()==0){
      TGeoVolume *innerHole=new TGeoVolume("InnerHole",new TGeoTube("InnerHole",0.0,inner_radius+envelop_Inner_thickness, lengthInnerWall),gas_mix);
      top->AddNode(innerHole,1,0);
      endcap_inner_radius=0.0;
    }
    else if (fDCHParam->GetExperimentSubVer()==1){
      TGeoShape *innerWall=new TGeoTube("InnerWall",inner_radius,inner_radius+envelop_Inner_thickness, lengthInnerWall);
      TGeoShape *innerWall_foam=new TGeoTube("InnerWall_foam",inner_radius-0.48,inner_radius, lengthInnerWall);
      TGeoShape *innerWall_1=new TGeoTube("InnerWall_1",inner_radius-0.48-envelop_Inner_thickness,inner_radius-0.48, lengthInnerWall);

      TGeoVolume *vol_innerWall=new TGeoVolume("InnerWall",innerWall,CarbonFiber);/*Kapton*/
      TGeoVolume *vol_innerWall_foam=new TGeoVolume("InnerWall_foam",innerWall_foam,Polypropylene);
      TGeoVolume *vol_innerWall_1=new TGeoVolume("InnerWall_1",innerWall_1,CarbonFiber);
      TGeoVolume *innerHole=new TGeoVolume("InnerHole",new TGeoTube("InnerHole",0.0,inner_radius-0.48-envelop_Inner_thickness, lengthInnerWall),Vacuum);

      vol_innerWall->SetLineColor(10);
      vol_innerWall->SetVisibility(1);
      top->AddNode(vol_innerWall,1,0);
      //  vol_innerWall_foam->SetLineColor(10);
      vol_innerWall_foam->SetVisibility(1);
      top->AddNode(vol_innerWall_foam,1,0);
      //  vol_innerWall_1->SetLineColor(10);
      vol_innerWall_1->SetVisibility(1);
      top->AddNode(vol_innerWall_1,1,0);

      top->AddNode(innerHole,1,0);
    }

  }

// // // //  TGeoShape *outerWall=new TGeoTube("OuterWall",outer_radius*1.01-envelop_Outer_thickness,outer_radius*1.01, length);
  TGeoShape *outerWall=new TGeoTube("OuterWall",outer_radius-envelop_Outer_thickness,outer_radius, length);

  TGeoVolume *vol_outerWall=new TGeoVolume("OuterWall",outerWall,CarbonFiber);
  vol_outerWall->SetLineColor(10);
  vol_outerWall->SetVisibility(1);
  top->AddNode(vol_outerWall,1,0);
  TGeoShape *EndCapWall;
  TGeoVolume *vol_EndCapWall_Right;
  TGeoVolume *vol_EndCapWall_Left;
  if(EndCap_type==0){
// // // //    EndCapWall=new TGeoTube("EndCapWall",endcap_inner_radius*0.985,outer_radius*1.01, 0.5*envelop_EndCap_thickness);
    EndCapWall=new TGeoTube("EndCapWall",endcap_inner_radius,outer_radius, 0.5*envelop_EndCap_thickness);
    vol_EndCapWall_Right=new TGeoVolume("RightWall",EndCapWall,CarbonFiber);

    TGeoTranslation *pos_EndCapWall = new TGeoTranslation("TR_EndCapWall",0.,0., length + 0.5*envelop_EndCap_thickness);
    TGeoTranslation *pos_EndCapWallL = new TGeoTranslation("TR_EndCapWallL",0.,0., -length -0.5*envelop_EndCap_thickness);
    vol_EndCapWall_Right->SetLineColor(10);
    vol_EndCapWall_Right->SetVisibility(1);
    top->AddNode(vol_EndCapWall_Right,1,pos_EndCapWall);

    vol_EndCapWall_Left=new TGeoVolume("LeftWall",EndCapWall,CarbonFiber);
    vol_EndCapWall_Left->SetLineColor(10);
    vol_EndCapWall_Left->SetVisibility(1);
    top->AddNode(vol_EndCapWall_Left,1,pos_EndCapWallL);
  }

  else if(EndCap_type==1){
    EndCapWall=new TGeoSphere("EndCapWall",max_EndCap_dim-envelop_EndCap_thickness,max_EndCap_dim,EndCap_Wall_theta_inner,EndCap_Wall_theta_outer,0.,360.);
    vol_EndCapWall_Right=new TGeoVolume("RightWall",EndCapWall,CarbonFiber);
    vol_EndCapWall_Right->SetLineColor(3);
    vol_EndCapWall_Right->SetVisibility(1);
    top->AddNode(vol_EndCapWall_Right,1,0);

    vol_EndCapWall_Left=new TGeoVolume("LeftWall",EndCapWall,CarbonFiber);
    vol_EndCapWall_Left->SetLineColor(3);
    vol_EndCapWall_Left->SetVisibility(1);
    top->AddNode(vol_EndCapWall_Left,1,leftEndCap);
  }


}


// //_____________________________________________________________________________ 
// void IlcDCHgeometry::CreateGeometry(Int_t *idtmed) 
// { 
// 
// }

//_____________________________________________________________________________ 
IlcDCHgeometry* IlcDCHgeometry::GetGeometry(IlcRunLoader* runLoader) 
{ 
  // 
  // load the geometry from the gilc file 
  // 
 
 if (!runLoader) runLoader = IlcRunLoader::GetRunLoader(GetTitle());
  if (!runLoader) {
    ::Error("IlcDCHgeometry::GetGeometry", "No run loader");
    return NULL;
  } 
 
  TDirectory* saveDir = gDirectory; 
  runLoader->CdGAFile(); 
  //cout<<gIlc->GetRunLoader->CdGAFile()<<endl;
  // Try from the gilc.root file 
  IlcDCHgeometry* geom =(IlcDCHgeometry*) gDirectory->Get("DCHgeometry"); 
  if (!geom) { 
    // It is not in the file, try to get it from gIlc,  
    // which corresponds to the run loader  
    IlcDCH * dch = (IlcDCH*)runLoader->GetIlcRun()->GetDetector("DCH"); 
    if(dch)
      geom = dch->GetGeometry(); 
  } 
  if (!geom) ::Error("IlcDCHgeometry::GetGeometry", "Geometry not found"); 
 
  saveDir->cd(); 
  return geom; 
 
} 

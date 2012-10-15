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

/* $Id: IlcTARGETv4th.cxx,v 1.8 2009/12/26 21:47:20 vitomeg Exp $ */
///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//  Inner Traking System version PPR  asymmetric for the FMD                 //
//  This class contains the base procedures for the Inner Tracking System    //
//                                                                           //
// Authors: R. Barbera                                                       //
// version 10.                                                               //
// Created  January 15 2001.                                                 //
//                                                                           //
//  NOTE: THIS IS THE  ASYMMETRIC PPR geometry of the TARGET for the PMD.       //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

// See IlcTARGETv4th::StepManager().

#include <TClonesArray.h>
#include <TGeometry.h>
#include <TLorentzVector.h>
#include <TGeoMatrix.h>
#include <TArrayD.h>
#include <TArrayF.h>
#include <TString.h>
#include <TNode.h>
#include <TTUBE.h>
#include <TGeoBBox.h>
#include <TGeoTube.h>
#include <TGeoManager.h>
#include <TGeoVolume.h>
#include <TGeoPcon.h>
#include <TGeoTrd2.h>
#include <Riostream.h>
#include "IlcTARGET.h"
#include "IlcTARGETDetTypeSim.h"
#include "IlcTARGETGeant3Geometry.h"
#include "IlcTARGETgeom.h"
#include "IlcTARGETgeomSDD.h"
#include "IlcTARGETgeomSPD.h"
#include "IlcTARGETgeomSSD.h"
#include "IlcTARGEThit.h"
#include "IlcTARGETresponseSDD.h"
#include "IlcTARGETCalibrationSDD.h"
#include "IlcTARGETCalibrationSPD.h"
#include "IlcTARGETCalibrationSSD.h"
#include "IlcTARGETsegmentationSDD.h"
#include "IlcTARGETsegmentationSPD.h"
#include "IlcTARGETsegmentationSPD2.h"
#include "IlcTARGETsegmentationSSD.h"
#include "IlcTARGETv4th.h"
#include "IlcLog.h"
#include "IlcMC.h"
#include "IlcMagF.h"
#include "IlcRun.h"
#include "IlcTrackReference.h"
#include "IlcTARGETParam.h"


#define GEANTGEOMETRY kTRUE

ClassImp(IlcTARGETv4th)

//______________________________________________________________________
IlcTARGETv4th::IlcTARGETv4th() {
    //    Standard default constructor for the TARGET version 10.
    // Inputs:
    //   none.
    // Outputs:
    //   none.
    // Return:
    //   none.
    Int_t i;
    fPixelX.Set(0);
    fPixelY.Set(0);
    fPixelZ.Set(0);
    fNPixelX.Set(0);
    fNPixelY.Set(0);
    fNPixelZ.Set(0);


    fIdN          = 0;
    fIdName       = 0;
    fIdSens       = 0;
    fEuclidOut    = kFALSE; // Don't write Euclide file
    fGeomDetOut   = kFALSE; // Don't write .det file
    fGeomDetIn    = kFALSE; // Read .det file
    fMajorVersion = IsVersion();
    for (i=0;i<60;i++) fRead[i] = '\0';
    for (i=0;i<60;i++) fWrite[i] = '\0';
    for (i=0;i<60;i++) fEuclidGeomDet[i] = '\0';
    strncpy(fRead,"$ILC_ROOT/TARGET/TARGETgeometry_vPPRasymmFMD.det",60);
}
//______________________________________________________________________
IlcTARGETv4th::IlcTARGETv4th(const char *name, const char *title,Int_t MinorVersion)
        : IlcTARGET("TARGET", title,MinorVersion) {
    //    Standard constructor for the TARGET version 10.
    // Inputs:
    //   const char * name   Ignored, set to "TARGET"
    //   const char * title  Arbitrary title
    // Outputs:
    //   none.
    // Return:
    //   none.
    Int_t i;
    sprintf(fDisplay,"%s","light");

    IlcTARGETParam *Param;
    Param=new IlcTARGETParam();

    fIdN = Param->GetNlayers()+Param->GetNendcaps()+Param->GetNendcapsStrip()+Param->GetNlayersStrip();
    delete Param;
    fPixelX.Set(fIdN);
    fPixelY.Set(fIdN);
    fPixelZ.Set(fIdN);
    fNPixelX.Set(fIdN);
    
    fNPixelY.Set(fIdN);
    
    fNPixelZ.Set(fIdN);
    
    
    
    fIdName = new TString[fIdN];
    fIdName[0] = name; // removes warning message
    fIdSens    = new Int_t[fIdN];
    for (i=0;i<fIdN;i++) fIdSens[i] = 0;
    fMajorVersion = IsVersion();
    fEuclidOut    = kFALSE; // Don't write Euclide file
    fGeomDetOut   = kFALSE; // Don't write .det file
    fGeomDetIn    = kFALSE; // Read .det file
    SetThicknessDet1();
    SetThicknessDet2();
    SetThicknessChip1();
    SetThicknessChip2();
    SetDensityServicesByThickness();

// // // //     fEuclidGeometry="$ILC_ROOT/TARGET/TARGETgeometry_vPPRasymm2.euc";
    strncpy(fEuclidGeomDet,"$ILC_ROOT/TARGET/TARGETgeometry_vPPRasymm2.det",60);
    strncpy(fRead,fEuclidGeomDet,60);
    strncpy(fWrite,fEuclidGeomDet,60);
    strncpy(fRead,"$ILC_ROOT/TARGET/TARGETgeometry_vPPRasymmFMD.det",60);
}
// // // // //______________________________________________________________________
// // // // IlcTARGETv4th::IlcTARGETv4th(const IlcTARGETv4th &source) :
// // // //         IlcTARGET(source) {
// // // //     //     Copy Constructor for TARGET version 10. This function is not to be
// // // //     // used. If any other instance of this function, other than "this" is
// // // //     // passed, an error message is returned.
// // // //     // Inputs:
// // // //     //   const IlcTARGETv4th &source This class
// // // //     // Outputs:
// // // //     //   none.
// // // //     // Return:
// // // //     //   an error message
// // // // 
// // // //     if (&source == this) return;
// // // //     Warning("Copy Constructor","Not allowed to copy IlcTARGETv4th");
// // // //     return;
// // // // }
//______________________________________________________________________
IlcTARGETv4th& IlcTARGETv4th::operator=(const IlcTARGETv4th
                                      &source) {
    //    Assignment operator for the TARGET version 10. This function is not
    // to be used. If any other instance of this function, other than "this"
    // is passed, an error message is returned.
    // Inputs:
    //   const IlcTARGETv4th &source This class
    // Outputs:
    //   none.
    // Return:
    //   an error message

    if (&source == this) return *this;
    Warning("= operator","Not allowed to copy IlcTARGETv4th");
    return *this;
}
//______________________________________________________________________
IlcTARGETv4th::~IlcTARGETv4th() {
    //    Standard destructor for the TARGET version 10.
    // Inputs:
    //   none.
    // Outputs:
    //   none.
    // Return:
    //   none.
}

TList *   IlcTARGETv4th::BuildGeometry2(void){
// creates the geometry for the ROOT display
  if (!gGeoManager){
    new TGeoManager("geometry", "geometry");
    gMC = (TVirtualMC*)gIlc->GetMCApp();
    TGeoMaterial *vacmat = new TGeoMaterial("Vacuum",0,0,0);
    TGeoMedium   *vacmed = new TGeoMedium("Vacuum_med",1,vacmat);
    TGeoVolume *ILCM = gGeoManager->MakeBox("ILCM",vacmed,100.,100.,200.);
    gGeoManager->SetTopVolume(ILCM);
    CreateGeometry();
  }

  IlcTARGETParam *Param=new IlcTARGETParam();
  Double_t Scale=Param->GetScale();

//    TString GeometryFileName = gIlc->GetGeometryFileName();

  TNode *node, *node_layer0, *node_layer1, *node_layer2, *node_layer3, *node_layer4;

  const Int_t kColorTARGET=kWhite;
  const Int_t kColorTARGETEndCap=kGreen;
  const Int_t kColorSiBarrel=kYellow;
  const Int_t kColorSiEndCap=kBlue;
  const Int_t kColorLDCEndCap=kYellow;


  TGeoVolume *TARGETV;
  TGeoBBox *lad_shape;
  TGeoTube *layer_shape, *endcap_shape;
  Char_t name[100],title[100],name_node[100],title_node[100],name_mat[100],title_mat[100];
  Float_t pos_z=0.0,dimx_ladder,dimy_ladder,dimz_ladder, dimz_layer0;
  Float_t dim_rmin, dim_rmax, dim_z;
  Int_t nlayers=Param->GetNlayers();//;5,
  Int_t nendcaps=Param->GetNendcaps();//;8;
  Int_t layer,endcap,iTARGET;
  Int_t nladders;
  Int_t ladder;
  Int_t gradi_rot;
  Float_t radius,radius1,radius2;
  Float_t radiant;
  Float_t x_ladder,y_ladder,z_ladder;
  Float_t rot, rot2;
  TString name_endcap;


  TList *Nodes = new TList();       //List of geometry nodes
  new TMaterial("void","Vacuum",0,0,0);  //Everything is void
  TBRIK *brik = new TBRIK("S_ilc","ilc volume","void",2000,2000,3000);
  brik->SetVisibility(0);
  TNode *top = new TNode("ilc","ilc","S_ilc");

    //version in ladder
  {

    x_ladder=Param->GetX_Ladder() ;//0.0;
    y_ladder=Param->GetY_Ladder() ;//0.0;
    z_ladder=Param->GetZ_Ladder() ;//0.0;
    dimz_layer0=Param->GetDimz();//6.25;
    rot=Param->GetRot();//0.0;
    layer=Param->GetLayer_0();//0;
    nladders=Param->GetNLadders_0();//12;
    gradi_rot=Param->GetGradi_Rot_0();//360/nladders;
    radiant=Param->GetRadiant_0 ();//(2*TMath::Pi())/nladders;
    radius1=Param->GetRadius1_0 ()*Scale;//1.40*(5.0/3.5);//*Scale
    radius2=Param->GetRadius2_0 ()*Scale;//1.5154*(5.0/3.5);//*Scale
    rot2=Param->GetRot2_0 () ;//15.0;


    new TTUBE("layer0_name","layer0_title","void",radius1,radius2,dimz_layer0);
    top->cd();
    new TRotMatrix("layer0_name_mat","layer0_title_mat",90,0-rot2,90,90-rot2,0,0);
    node_layer0=new TNode("layer0_name_node","layer0_title_node","layer0_name",x_ladder,y_ladder,z_ladder,"layer0_name_mat");
    node_layer0->SetVisibility(0);
    Nodes->Add(node_layer0);

    for (ladder=0;ladder<nladders;ladder++) {
      if ((ladder%2)==0) radius=radius2;
      else radius=radius1;
      x_ladder=radius*(TMath::Cos((TMath::Pi()/2)+(ladder*radiant)));
      y_ladder=radius*(TMath::Sin((TMath::Pi()/2)+(ladder*radiant)));
      if ((ladder%2)==0) {
        sprintf(name,"VertexBarrel_layer%d_ladder%douter_volume",layer,ladder);
        sprintf(title,"VertexBarrel_layer%d_ladder%douter_volumeB",layer,ladder);
        sprintf(name_node,"VertexBarrel_layer%d_ladder%douter_volumeC",layer,ladder);
        sprintf(title_node,"VertexBarrel_layer%d_ladder%douter_volumeD",layer,ladder);
        sprintf(name_mat,"VertexBarrel_layer%d_ladder%douter_volumeE",layer,ladder);
        sprintf(title_mat,"VertexBarrel_layer%d_ladder%douter_volumeF",layer,ladder);
      } else {
        sprintf(name,"VertexBarrel_layer%d_ladder%dinner_volume",layer,ladder);
        sprintf(title,"VertexBarrel_layer%d_ladder%dinner_volumeB",layer,ladder);
        sprintf(name_node,"VertexBarrel_layer%d_ladder%dinner_volumeC",layer,ladder);
        sprintf(title_node,"VertexBarrel_layer%d_ladder%dinner_volumeD",layer,ladder);
        sprintf(name_mat,"VertexBarrel_layer%d_ladder%dinner_volumeE",layer,ladder);
        sprintf(title_mat,"VertexBarrel_layer%d_ladder%dinner_volumeF",layer,ladder);
      }
      lad_shape=(TGeoBBox *)gGeoManager->GetVolume(name)->GetShape();
      dimx_ladder=lad_shape->GetDX();
      dimy_ladder=lad_shape->GetDY();
      dimz_ladder=lad_shape->GetDZ();
      new TBRIK(name,title,"void",dimx_ladder,dimy_ladder,dimz_ladder);
      node_layer0->cd();
      new TRotMatrix(name_mat,title_mat,90,0-rot,90,90-rot,0,0);
      node=new TNode(name_node,title_node,name,x_ladder,y_ladder,z_ladder,name_mat);
      node->SetLineColor(kColorTARGET);
      Nodes->Add(node);
      rot=rot-gradi_rot;
    }


    layer=Param->GetLayer_1();//1;
    nladders=Param->GetNLadders_1();//12;
    gradi_rot=Param->GetGradi_Rot_1();//360/nladders;
    radiant=Param->GetRadiant_1 ();//(2*TMath::Pi())/nladders;
    radius1=Param->GetRadius1_1 ()*Scale;//2.20*(5.0/3.5);
    radius2=Param->GetRadius2_1 ()*Scale;//2.3132*(5.0/3.5);
    x_ladder=Param->GetX_Ladder() ;//x_ladder=0.0;
    y_ladder=Param->GetY_Ladder() ;//y_ladder=0.0;
    z_ladder=Param->GetZ_Ladder() ;//z_ladder=0.0;
    rot2=Param->GetRot2_1 () ;// rot2=15.0;
    rot=Param->GetRot();//0.0;//rot=0.0;
    dimz_layer0=Param->GetDimz();//6.25;

    new TTUBE("layer1_name","layer1_title","void",radius1,radius2,dimz_layer0);
    top->cd();
    new TRotMatrix("layer1_name_mat","layer1_title_mat",90,0-rot2,90,90-rot2,0,0);
    node_layer1=new TNode("layer1_name_node","layer1_title_node","layer1_name",x_ladder,y_ladder,z_ladder,"layer1_name_mat");
    node_layer1->SetVisibility(0);
    Nodes->Add(node_layer1);

    for (ladder=0;ladder<nladders;ladder++) {
      if ((ladder%2)==0) radius=radius2;
      else radius=radius1;
      x_ladder=radius*(TMath::Cos((TMath::Pi()/2)+(ladder*radiant)));
      y_ladder=radius*(TMath::Sin((TMath::Pi()/2)+(ladder*radiant)));
      if ((ladder%2)==0) {
        sprintf(name,"VertexBarrel_layer%d_ladder%douter_volume",layer,ladder);
        sprintf(title,"VertexBarrel_layer%d_ladder%douter_volumeB",layer,ladder);
        sprintf(name_node,"VertexBarrel_layer%d_ladder%douter_volumeC",layer,ladder);
        sprintf(title_node,"VertexBarrel_layer%d_ladder%douter_volumeD",layer,ladder);
        sprintf(name_mat,"VertexBarrel_layer%d_ladder%douter_volumeE",layer,ladder);
        sprintf(title_mat,"VertexBarrel_layer%d_ladder%douter_volumeF",layer,ladder);
      } else {
        sprintf(name,"VertexBarrel_layer%d_ladder%dinner_volume",layer,ladder);
        sprintf(title,"VertexBarrel_layer%d_ladder%dinner_volumeB",layer,ladder);
        sprintf(name_node,"VertexBarrel_layer%d_ladder%dinner_volumeC",layer,ladder);
        sprintf(title_node,"VertexBarrel_layer%d_ladder%dinner_volumeD",layer,ladder);
        sprintf(name_mat,"VertexBarrel_layer%d_ladder%dinner_volumeE",layer,ladder);
        sprintf(title_mat,"VertexBarrel_layer%d_ladder%dinner_volumeF",layer,ladder);
      }
      lad_shape=(TGeoBBox *)gGeoManager->GetVolume(name)->GetShape();
      dimx_ladder=lad_shape->GetDX();
      dimy_ladder=lad_shape->GetDY();
      dimz_ladder=lad_shape->GetDZ();
      new TBRIK(name,title,"void",dimx_ladder,dimy_ladder,dimz_ladder);
      node_layer1->cd();
      new TRotMatrix(name_mat,title_mat,90,0-rot,90,90-rot,0,0);
      node=new TNode(name_node,title_node,name,x_ladder,y_ladder,z_ladder,name_mat);
      node->SetLineColor(kColorTARGET);
      Nodes->Add(node);
      rot=rot-gradi_rot;
    }

    layer=Param->GetLayer_2();// layer=2;
    nladders=Param->GetNLadders_2();//nladders=18;
    gradi_rot=Param->GetGradi_Rot_2();//gradi_rot=360/nladders;
    radiant=Param->GetRadiant_2 ();//radiant=(2*TMath::Pi())/nladders;
    radius1=Param->GetRadius1_2 ()*Scale;//radius1=3.50*(5.0/3.5);
    radius2=Param->GetRadius2_2 ()*Scale;//radius2=3.5890*(5.0/3.5);
    x_ladder=Param->GetX_Ladder() ;//x_ladder=0.0;
    y_ladder=Param->GetY_Ladder() ;//y_ladder=0.0;
    z_ladder=Param->GetZ_Ladder() ;//z_ladder=0.0;
    rot2=Param->GetRot2_2 () ;//rot2=10.0;
    rot=Param->GetRot();//0.0;//rot=0.0;
    dimz_layer0=Param->GetDimz();//6.25;

    new TTUBE("layer2_name","layer2_title","void",radius1,radius2,dimz_layer0);
    top->cd();
    new TRotMatrix("layer2_name_mat","layer2_title_mat",90,0-rot2,90,90-rot2,0,0);
    node_layer2=new TNode("layer2_name_node","layer2_title_node","layer2_name",x_ladder,y_ladder,z_ladder,"layer2_name_mat");
    node_layer2->SetVisibility(0);
    Nodes->Add(node_layer2);

    for (ladder=0;ladder<nladders;ladder++) {
      if ((ladder%2)==0) radius=radius2;
      else radius=radius1;
      x_ladder=radius*(TMath::Cos((TMath::Pi()/2)+(ladder*radiant)));
      y_ladder=radius*(TMath::Sin((TMath::Pi()/2)+(ladder*radiant)));
      if ((ladder%2)==0) {
        sprintf(name,"VertexBarrel_layer%d_ladder%douter_volume",layer,ladder);
        sprintf(title,"VertexBarrel_layer%d_ladder%douter_volumeB",layer,ladder);
        sprintf(name_node,"VertexBarrel_layer%d_ladder%douter_volumeC",layer,ladder);
        sprintf(title_node,"VertexBarrel_layer%d_ladder%douter_volumeD",layer,ladder);
        sprintf(name_mat,"VertexBarrel_layer%d_ladder%douter_volumeE",layer,ladder);
        sprintf(title_mat,"VertexBarrel_layer%d_ladder%douter_volumeF",layer,ladder);
      } else {
        sprintf(name,"VertexBarrel_layer%d_ladder%dinner_volume",layer,ladder);
        sprintf(title,"VertexBarrel_layer%d_ladder%dinner_volumeB",layer,ladder);
        sprintf(name_node,"VertexBarrel_layer%d_ladder%dinner_volumeC",layer,ladder);
        sprintf(title_node,"VertexBarrel_layer%d_ladder%dinner_volumeD",layer,ladder);
        sprintf(name_mat,"VertexBarrel_layer%d_ladder%dinner_volumeE",layer,ladder);
        sprintf(title_mat,"VertexBarrel_layer%d_ladder%dinner_volumeF",layer,ladder);
      }
      lad_shape=(TGeoBBox *)gGeoManager->GetVolume(name)->GetShape();
      dimx_ladder=lad_shape->GetDX();
      dimy_ladder=lad_shape->GetDY();
      dimz_ladder=lad_shape->GetDZ();
      new TBRIK(name,title,"void",dimx_ladder,dimy_ladder,dimz_ladder);
      node_layer2->cd();
      new TRotMatrix(name_mat,title_mat,90,0-rot,90,90-rot,0,0);
      node=new TNode(name_node,title_node,name,x_ladder,y_ladder,z_ladder,name_mat);
      node->SetLineColor(kColorTARGET);
      Nodes->Add(node);
      rot=rot-gradi_rot;
    }


    layer=Param->GetLayer_3();// layer=3;
    nladders=Param->GetNLadders_3();//nladders=24;
    gradi_rot=Param->GetGradi_Rot_3();//gradi_rot=360/nladders;
    radiant=Param->GetRadiant_3 ();//radiant=(2*TMath::Pi())/nladders;
    radius1=Param->GetRadius1_3 ()*Scale;//radius1=4.76*(5.0/3.5);
    radius2=Param->GetRadius2_3 ()*Scale;//radius2=4.8409*(5.0/3.5);
    x_ladder=Param->GetX_Ladder() ;//0.0;//x_ladder=0.0;
    y_ladder=Param->GetY_Ladder() ;//0.0;//y_ladder=0.0;
    z_ladder=Param->GetZ_Ladder() ;//0.0;//z_ladder=0.0;
    rot2=Param->GetRot2_3 () ;//rot2=8.0;
    rot=Param->GetRot();//0.0;//rot=0.0;
    dimz_layer0=Param->GetDimz();//6.25;


    new TTUBE("layer3_name","layer3_title","void",radius1,radius2,dimz_layer0);
    top->cd();
    new TRotMatrix("layer3_name_mat","layer3_title_mat",90,0-rot2,90,90-rot2,0,0);
    node_layer3=new TNode("layer3_name_node","layer3_title_node","layer3_name",x_ladder,y_ladder,z_ladder,"layer3_name_mat");
    node_layer3->SetVisibility(0);
    Nodes->Add(node_layer3);

    for (ladder=0;ladder<nladders;ladder++) {
      if ((ladder%2)==0) radius=radius2;
      else radius=radius1;
      x_ladder=radius*(TMath::Cos((TMath::Pi()/2)+(ladder*radiant)));
      y_ladder=radius*(TMath::Sin((TMath::Pi()/2)+(ladder*radiant)));
      if ((ladder%2)==0) {
        sprintf(name,"VertexBarrel_layer%d_ladder%douter_volume",layer,ladder);
        sprintf(title,"VertexBarrel_layer%d_ladder%douter_volumeB",layer,ladder);
        sprintf(name_node,"VertexBarrel_layer%d_ladder%douter_volumeC",layer,ladder);
        sprintf(title_node,"VertexBarrel_layer%d_ladder%douter_volumeD",layer,ladder);
        sprintf(name_mat,"VertexBarrel_layer%d_ladder%douter_volumeE",layer,ladder);
        sprintf(title_mat,"VertexBarrel_layer%d_ladder%douter_volumeF",layer,ladder);
      } else {
        sprintf(name,"VertexBarrel_layer%d_ladder%dinner_volume",layer,ladder);
        sprintf(title,"VertexBarrel_layer%d_ladder%dinner_volumeB",layer,ladder);
        sprintf(name_node,"VertexBarrel_layer%d_ladder%dinner_volumeC",layer,ladder);
        sprintf(title_node,"VertexBarrel_layer%d_ladder%dinner_volumeD",layer,ladder);
        sprintf(name_mat,"VertexBarrel_layer%d_ladder%dinner_volumeE",layer,ladder);
        sprintf(title_mat,"VertexBarrel_layer%d_ladder%dinner_volumeF",layer,ladder);
      }
      lad_shape=(TGeoBBox *)gGeoManager->GetVolume(name)->GetShape();
      dimx_ladder=lad_shape->GetDX();
      dimy_ladder=lad_shape->GetDY();
      dimz_ladder=lad_shape->GetDZ();
      new TBRIK(name,title,"void",dimx_ladder,dimy_ladder,dimz_ladder);
      node_layer3->cd();
      new TRotMatrix(name_mat,title_mat,90,0-rot,90,90-rot,0,0);
      node=new TNode(name_node,title_node,name,x_ladder,y_ladder,z_ladder,name_mat);
      node->SetLineColor(kColorTARGET);
      Nodes->Add(node);
      rot=rot-gradi_rot;
    }


    layer=Param->GetLayer_4();//layer=4;
    nladders=Param->GetNLadders_4();//nladders=30;
    gradi_rot=Param->GetGradi_Rot_4();//gradi_rot=360/nladders;
    radiant=Param->GetRadiant_4 ();//radiant=(2*TMath::Pi())/nladders;
    radius1=Param->GetRadius1_4 ()*Scale;//radius1=6.00*(5.0/3.5);
    radius2=Param->GetRadius2_4 ()*Scale;//radius2=6.0770*(5.0/3.5);
    x_ladder=Param->GetX_Ladder() ;//0.0;//x_ladder=0.0;
    y_ladder=Param->GetY_Ladder() ;//0.0;//y_ladder=0.0;
    z_ladder=Param->GetZ_Ladder() ;//0.0;//z_ladder=0.0;
    rot2=Param->GetRot2_4 () ;//rot2=6.0;
    rot=Param->GetRot();//0.0;//rot=0.0;
    dimz_layer0=Param->GetDimz();//6.25;

    new TTUBE("layer4_name","layer4_title","void",radius1,radius2,dimz_layer0);
    top->cd();
    new TRotMatrix("layer4_name_mat","layer4_title_mat",90,0-rot2,90,90-rot2,0,0);
    node_layer4=new TNode("layer4_name_node","layer4_title_node","layer4_name",x_ladder,y_ladder,z_ladder,"layer4_name_mat");
    node_layer4->SetVisibility(0);
    Nodes->Add(node_layer4);

    for (ladder=0;ladder<nladders;ladder++) {
      if ((ladder%2)==0) radius=radius2;
      else radius=radius1;
      x_ladder=radius*(TMath::Cos((TMath::Pi()/2)+(ladder*radiant)));
      y_ladder=radius*(TMath::Sin((TMath::Pi()/2)+(ladder*radiant)));
      if ((ladder%2)==0) {
        sprintf(name,"VertexBarrel_layer%d_ladder%douter_volume",layer,ladder);
        sprintf(title,"VertexBarrel_layer%d_ladder%douter_volumeB",layer,ladder);
        sprintf(name_node,"VertexBarrel_layer%d_ladder%douter_volumeC",layer,ladder);
        sprintf(title_node,"VertexBarrel_layer%d_ladder%douter_volumeD",layer,ladder);
        sprintf(name_mat,"VertexBarrel_layer%d_ladder%douter_volumeE",layer,ladder);
        sprintf(title_mat,"VertexBarrel_layer%d_ladder%douter_volumeF",layer,ladder);
      } else {
        sprintf(name,"VertexBarrel_layer%d_ladder%dinner_volume",layer,ladder);
        sprintf(title,"VertexBarrel_layer%d_ladder%dinner_volumeB",layer,ladder);
        sprintf(name_node,"VertexBarrel_layer%d_ladder%dinner_volumeC",layer,ladder);
        sprintf(title_node,"VertexBarrel_layer%d_ladder%dinner_volumeD",layer,ladder);
        sprintf(name_mat,"VertexBarrel_layer%d_ladder%dinner_volumeE",layer,ladder);
        sprintf(title_mat,"VertexBarrel_layer%d_ladder%dinner_volumeF",layer,ladder);
      }
      lad_shape=(TGeoBBox *)gGeoManager->GetVolume(name)->GetShape();
      dimx_ladder=lad_shape->GetDX();
      dimy_ladder=lad_shape->GetDY();
      dimz_ladder=lad_shape->GetDZ();
      new TBRIK(name,title,"void",dimx_ladder,dimy_ladder,dimz_ladder);
      node_layer4->cd();
      new TRotMatrix(name_mat,title_mat,90,0-rot,90,90-rot,0,0);
      node=new TNode(name_node,title_node,name,x_ladder,y_ladder,z_ladder,name_mat);
      node->SetLineColor(kColorTARGET);
      Nodes->Add(node);
      rot=rot-gradi_rot;
    }
  } //end version in ladder

    //_________________________________________________________________________________________________

  {

    TARGETV=gGeoManager->GetVolume("TARGETV");
    for (endcap=0;endcap<nendcaps;endcap++) {

      sprintf(name,"VertexEndcap_layer%d_volume",endcap);
      sprintf(title,"VertexEndcap_layer%d_volumeB",endcap);
      sprintf(name_node,"VertexEndcap_layer%d_volumeC",endcap);
      sprintf(title_node,"VertexEndcap_layer%d_volumeD",endcap);

      endcap_shape=(TGeoTube *)gGeoManager->GetVolume(name)->GetShape();
      dim_rmin= endcap_shape->GetRmin();
      dim_rmax= endcap_shape->GetRmax();
      dim_z=    endcap_shape->GetDZ();
      for (iTARGET=0;iTARGET<TARGETV->GetNdaughters();iTARGET++) {
        name_endcap=TARGETV->GetNode(iTARGET)->GetName();
        if (name_endcap.Contains(name)) pos_z=TARGETV->GetNode(iTARGET)->GetMatrix()->GetTranslation()[2];
      }
      new TTUBE(name,title,"void",dim_rmin,dim_rmax,dim_z);
      top->cd();
      node = new TNode(name_node,title_node,name,0,0,pos_z,"");
      node->SetLineColor(kColorTARGETEndCap);
      Nodes->Add(node);
    }
  }
  return Nodes;

}


//______________________________________________________________________
void IlcTARGETv4th::BuildGeometry() {
// // // //     //    Geometry builder for the TARGET version 10. Event Display geometry.
// // // //     // Inputs:
// // // //     //   none.
// // // //     // Outputs:
// // // //     //   none.
// // // //     // Return:
// // // //     //   none.
// // // //  IlcTARGETParam *Param=new IlcTARGETParam();
// // // //  Double_t Scale=Param->GetScale();
// // // // 
// // // // 
// // // // //    TString GeometryFileName = gIlc->GetGeometryFileName();
// // // // 
// // // //     TNode *node, *node_layer0, *node_layer1, *node_layer2, *node_layer3, *node_layer4, *top;
// // // // 
// // // //     const Int_t kColorTARGET=kWhite;
// // // //     const Int_t kColorTARGETEndCap=kGreen;
// // // //     const Int_t kColorSiBarrel=kYellow;
// // // //     const Int_t kColorSiEndCap=kBlue;
// // // //     const Int_t kColorLDCEndCap=kYellow;
// // // // 
// // // // 
// // // //     TGeoVolume *TARGETV;
// // // //     TGeoBBox *lad_shape;
// // // //     TGeoTube *layer_shape, *endcap_shape;
// // // //     Char_t name[100],title[100],name_node[100],title_node[100],name_mat[100],title_mat[100];
// // // //     Float_t pos_z=0.0,dimx_ladder,dimy_ladder,dimz_ladder, dimz_layer0;
// // // //     Float_t dim_rmin, dim_rmax, dim_z;
// // // //     Int_t nlayers=Param->GetNlayers();//;5,
// // // //     Int_t nendcaps=Param->GetNendcaps();//;8;
// // // //     Int_t layer,endcap,iTARGET;
// // // //     Int_t nladders;
// // // //     Int_t ladder;
// // // //     Int_t gradi_rot;
// // // //     Float_t radius,radius1,radius2;
// // // //     Float_t radiant;
// // // //     Float_t x_ladder,y_ladder,z_ladder;
// // // //     Float_t rot, rot2;
// // // //     TString name_endcap;
// // // // 
// // // //     top = gIlc->GetGeometry()->GetNode("ilc");
// // // // 
// // // //     //version in ladder
// // // //     {
// // // // 
// // // // 	x_ladder=Param->GetX_Ladder() ;//0.0;
// // // // 	y_ladder=Param->GetY_Ladder() ;//0.0;
// // // // 	z_ladder=Param->GetZ_Ladder() ;//0.0;
// // // // 	dimz_layer0=Param->GetDimz();//6.25;
// // // // 	rot=Param->GetRot();//0.0;
// // // //         layer=Param->GetLayer_0();//0;
// // // // 	nladders=Param->GetNLadders_0();//12;
// // // //         gradi_rot=Param->GetGradi_Rot_0();//360/nladders;
// // // //         radiant=Param->GetRadiant_0 ();//(2*TMath::Pi())/nladders;
// // // //         radius1=Param->GetRadius1_0 ()*Scale;//1.40*(5.0/3.5);//*Scale
// // // //         radius2=Param->GetRadius2_0 ()*Scale;//1.5154*(5.0/3.5);//*Scale
// // // //         rot2=Param->GetRot2_0 () ;//15.0;
// // // // 
// // // // 
// // // //         new TTUBE("layer0_name","layer0_title","void",radius1,radius2,dimz_layer0);
// // // //         top->cd();
// // // //         new TRotMatrix("layer0_name_mat","layer0_title_mat",90,0-rot2,90,90-rot2,0,0);
// // // //         node_layer0=new TNode("layer0_name_node","layer0_title_node","layer0_name",x_ladder,y_ladder,z_ladder,"layer0_name_mat");
// // // //         node_layer0->SetVisibility(0);
// // // //         fNodes->Add(node_layer0);
// // // // 
// // // //         for (ladder=0;ladder<nladders;ladder++) {
// // // //             if ((ladder%2)==0) radius=radius2;
// // // //             else radius=radius1;
// // // //             x_ladder=radius*(TMath::Cos((TMath::Pi()/2)+(ladder*radiant)));
// // // //             y_ladder=radius*(TMath::Sin((TMath::Pi()/2)+(ladder*radiant)));
// // // //             if ((ladder%2)==0) {
// // // //                 sprintf(name,"VertexBarrel_layer%d_ladder%douter_volume",layer,ladder);
// // // //                 sprintf(title,"VertexBarrel_layer%d_ladder%douter_volumeB",layer,ladder);
// // // //                 sprintf(name_node,"VertexBarrel_layer%d_ladder%douter_volumeC",layer,ladder);
// // // //                 sprintf(title_node,"VertexBarrel_layer%d_ladder%douter_volumeD",layer,ladder);
// // // //                 sprintf(name_mat,"VertexBarrel_layer%d_ladder%douter_volumeE",layer,ladder);
// // // //                 sprintf(title_mat,"VertexBarrel_layer%d_ladder%douter_volumeF",layer,ladder);
// // // //             } else {
// // // //                 sprintf(name,"VertexBarrel_layer%d_ladder%dinner_volume",layer,ladder);
// // // //                 sprintf(title,"VertexBarrel_layer%d_ladder%dinner_volumeB",layer,ladder);
// // // //                 sprintf(name_node,"VertexBarrel_layer%d_ladder%dinner_volumeC",layer,ladder);
// // // //                 sprintf(title_node,"VertexBarrel_layer%d_ladder%dinner_volumeD",layer,ladder);
// // // //                 sprintf(name_mat,"VertexBarrel_layer%d_ladder%dinner_volumeE",layer,ladder);
// // // //                 sprintf(title_mat,"VertexBarrel_layer%d_ladder%dinner_volumeF",layer,ladder);
// // // //             }
// // // //             lad_shape=(TGeoBBox *)gGeoManager->GetVolume(name)->GetShape();
// // // //             dimx_ladder=lad_shape->GetDX();
// // // //             dimy_ladder=lad_shape->GetDY();
// // // //             dimz_ladder=lad_shape->GetDZ();
// // // //             new TBRIK(name,title,"void",dimx_ladder,dimy_ladder,dimz_ladder);
// // // //             node_layer0->cd();
// // // //             new TRotMatrix(name_mat,title_mat,90,0-rot,90,90-rot,0,0);
// // // //             node=new TNode(name_node,title_node,name,x_ladder,y_ladder,z_ladder,name_mat);
// // // //             node->SetLineColor(kColorTARGET);
// // // //             fNodes->Add(node);
// // // //             rot=rot-gradi_rot;
// // // //         }
// // // // 
// // // // 
// // // // 	layer=Param->GetLayer_1();//1;
// // // //         nladders=Param->GetNLadders_1();//12;
// // // //         gradi_rot=Param->GetGradi_Rot_1();//360/nladders;
// // // //         radiant=Param->GetRadiant_1 ();//(2*TMath::Pi())/nladders;
// // // //         radius1=Param->GetRadius1_1 ()*Scale;//2.20*(5.0/3.5);
// // // //         radius2=Param->GetRadius2_1 ()*Scale;//2.3132*(5.0/3.5);
// // // //         x_ladder=Param->GetX_Ladder() ;//x_ladder=0.0;
// // // //         y_ladder=Param->GetY_Ladder() ;//y_ladder=0.0;
// // // //         z_ladder=Param->GetZ_Ladder() ;//z_ladder=0.0;
// // // //         rot2=Param->GetRot2_1 () ;// rot2=15.0;
// // // //         rot=Param->GetRot();//0.0;//rot=0.0;
// // // //         dimz_layer0=Param->GetDimz();//6.25;
// // // // 
// // // // 	new TTUBE("layer1_name","layer1_title","void",radius1,radius2,dimz_layer0);
// // // //         top->cd();
// // // //         new TRotMatrix("layer1_name_mat","layer1_title_mat",90,0-rot2,90,90-rot2,0,0);
// // // //         node_layer1=new TNode("layer1_name_node","layer1_title_node","layer1_name",x_ladder,y_ladder,z_ladder,"layer1_name_mat");
// // // //         node_layer1->SetVisibility(0);
// // // //         fNodes->Add(node_layer1);
// // // // 
// // // //         for (ladder=0;ladder<nladders;ladder++) {
// // // //             if ((ladder%2)==0) radius=radius2;
// // // //             else radius=radius1;
// // // //             x_ladder=radius*(TMath::Cos((TMath::Pi()/2)+(ladder*radiant)));
// // // //             y_ladder=radius*(TMath::Sin((TMath::Pi()/2)+(ladder*radiant)));
// // // //             if ((ladder%2)==0) {
// // // //                 sprintf(name,"VertexBarrel_layer%d_ladder%douter_volume",layer,ladder);
// // // //                 sprintf(title,"VertexBarrel_layer%d_ladder%douter_volumeB",layer,ladder);
// // // //                 sprintf(name_node,"VertexBarrel_layer%d_ladder%douter_volumeC",layer,ladder);
// // // //                 sprintf(title_node,"VertexBarrel_layer%d_ladder%douter_volumeD",layer,ladder);
// // // //                 sprintf(name_mat,"VertexBarrel_layer%d_ladder%douter_volumeE",layer,ladder);
// // // //                 sprintf(title_mat,"VertexBarrel_layer%d_ladder%douter_volumeF",layer,ladder);
// // // //             } else {
// // // //                 sprintf(name,"VertexBarrel_layer%d_ladder%dinner_volume",layer,ladder);
// // // //                 sprintf(title,"VertexBarrel_layer%d_ladder%dinner_volumeB",layer,ladder);
// // // //                 sprintf(name_node,"VertexBarrel_layer%d_ladder%dinner_volumeC",layer,ladder);
// // // //                 sprintf(title_node,"VertexBarrel_layer%d_ladder%dinner_volumeD",layer,ladder);
// // // //                 sprintf(name_mat,"VertexBarrel_layer%d_ladder%dinner_volumeE",layer,ladder);
// // // //                 sprintf(title_mat,"VertexBarrel_layer%d_ladder%dinner_volumeF",layer,ladder);
// // // //             }
// // // //             lad_shape=(TGeoBBox *)gGeoManager->GetVolume(name)->GetShape();
// // // //             dimx_ladder=lad_shape->GetDX();
// // // //             dimy_ladder=lad_shape->GetDY();
// // // //             dimz_ladder=lad_shape->GetDZ();
// // // //             new TBRIK(name,title,"void",dimx_ladder,dimy_ladder,dimz_ladder);
// // // //             node_layer1->cd();
// // // //             new TRotMatrix(name_mat,title_mat,90,0-rot,90,90-rot,0,0);
// // // //             node=new TNode(name_node,title_node,name,x_ladder,y_ladder,z_ladder,name_mat);
// // // //             node->SetLineColor(kColorTARGET);
// // // //             fNodes->Add(node);
// // // //             rot=rot-gradi_rot;
// // // //         }
// // // // 
// // // //         layer=Param->GetLayer_2();// layer=2;
// // // //         nladders=Param->GetNLadders_2();//nladders=18;
// // // //         gradi_rot=Param->GetGradi_Rot_2();//gradi_rot=360/nladders;
// // // //         radiant=Param->GetRadiant_2 ();//radiant=(2*TMath::Pi())/nladders;
// // // //         radius1=Param->GetRadius1_2 ()*Scale;//radius1=3.50*(5.0/3.5);
// // // //         radius2=Param->GetRadius2_2 ()*Scale;//radius2=3.5890*(5.0/3.5);
// // // //         x_ladder=Param->GetX_Ladder() ;//x_ladder=0.0;
// // // //         y_ladder=Param->GetY_Ladder() ;//y_ladder=0.0;
// // // //         z_ladder=Param->GetZ_Ladder() ;//z_ladder=0.0;
// // // //         rot2=Param->GetRot2_2 () ;//rot2=10.0;
// // // //         rot=Param->GetRot();//0.0;//rot=0.0;
// // // // 	dimz_layer0=Param->GetDimz();//6.25;
// // // // 
// // // //         new TTUBE("layer2_name","layer2_title","void",radius1,radius2,dimz_layer0);
// // // //         top->cd();
// // // //         new TRotMatrix("layer2_name_mat","layer2_title_mat",90,0-rot2,90,90-rot2,0,0);
// // // //         node_layer2=new TNode("layer2_name_node","layer2_title_node","layer2_name",x_ladder,y_ladder,z_ladder,"layer2_name_mat");
// // // //         node_layer2->SetVisibility(0);
// // // //         fNodes->Add(node_layer2);
// // // // 
// // // //         for (ladder=0;ladder<nladders;ladder++) {
// // // //             if ((ladder%2)==0) radius=radius2;
// // // //             else radius=radius1;
// // // //             x_ladder=radius*(TMath::Cos((TMath::Pi()/2)+(ladder*radiant)));
// // // //             y_ladder=radius*(TMath::Sin((TMath::Pi()/2)+(ladder*radiant)));
// // // //             if ((ladder%2)==0) {
// // // //                 sprintf(name,"VertexBarrel_layer%d_ladder%douter_volume",layer,ladder);
// // // //                 sprintf(title,"VertexBarrel_layer%d_ladder%douter_volumeB",layer,ladder);
// // // //                 sprintf(name_node,"VertexBarrel_layer%d_ladder%douter_volumeC",layer,ladder);
// // // //                 sprintf(title_node,"VertexBarrel_layer%d_ladder%douter_volumeD",layer,ladder);
// // // //                 sprintf(name_mat,"VertexBarrel_layer%d_ladder%douter_volumeE",layer,ladder);
// // // //                 sprintf(title_mat,"VertexBarrel_layer%d_ladder%douter_volumeF",layer,ladder);
// // // //             } else {
// // // //                 sprintf(name,"VertexBarrel_layer%d_ladder%dinner_volume",layer,ladder);
// // // //                 sprintf(title,"VertexBarrel_layer%d_ladder%dinner_volumeB",layer,ladder);
// // // //                 sprintf(name_node,"VertexBarrel_layer%d_ladder%dinner_volumeC",layer,ladder);
// // // //                 sprintf(title_node,"VertexBarrel_layer%d_ladder%dinner_volumeD",layer,ladder);
// // // //                 sprintf(name_mat,"VertexBarrel_layer%d_ladder%dinner_volumeE",layer,ladder);
// // // //                 sprintf(title_mat,"VertexBarrel_layer%d_ladder%dinner_volumeF",layer,ladder);
// // // //             }
// // // //             lad_shape=(TGeoBBox *)gGeoManager->GetVolume(name)->GetShape();
// // // //             dimx_ladder=lad_shape->GetDX();
// // // //             dimy_ladder=lad_shape->GetDY();
// // // //             dimz_ladder=lad_shape->GetDZ();
// // // //             new TBRIK(name,title,"void",dimx_ladder,dimy_ladder,dimz_ladder);
// // // //             node_layer2->cd();
// // // //             new TRotMatrix(name_mat,title_mat,90,0-rot,90,90-rot,0,0);
// // // //             node=new TNode(name_node,title_node,name,x_ladder,y_ladder,z_ladder,name_mat);
// // // //             node->SetLineColor(kColorTARGET);
// // // //             fNodes->Add(node);
// // // //             rot=rot-gradi_rot;
// // // //         }
// // // // 
// // // // 
// // // //         layer=Param->GetLayer_3();// layer=3;
// // // //         nladders=Param->GetNLadders_3();//nladders=24;
// // // //         gradi_rot=Param->GetGradi_Rot_3();//gradi_rot=360/nladders;
// // // //         radiant=Param->GetRadiant_3 ();//radiant=(2*TMath::Pi())/nladders;
// // // //         radius1=Param->GetRadius1_3 ()*Scale;//radius1=4.76*(5.0/3.5);
// // // //         radius2=Param->GetRadius2_3 ()*Scale;//radius2=4.8409*(5.0/3.5);
// // // //         x_ladder=Param->GetX_Ladder() ;//0.0;//x_ladder=0.0;
// // // //         y_ladder=Param->GetY_Ladder() ;//0.0;//y_ladder=0.0;
// // // //         z_ladder=Param->GetZ_Ladder() ;//0.0;//z_ladder=0.0;
// // // // 	rot2=Param->GetRot2_3 () ;//rot2=8.0;
// // // //      	rot=Param->GetRot();//0.0;//rot=0.0;
// // // // 	dimz_layer0=Param->GetDimz();//6.25;
// // // // 
// // // // 
// // // //         new TTUBE("layer3_name","layer3_title","void",radius1,radius2,dimz_layer0);
// // // //         top->cd();
// // // //         new TRotMatrix("layer3_name_mat","layer3_title_mat",90,0-rot2,90,90-rot2,0,0);
// // // //         node_layer3=new TNode("layer3_name_node","layer3_title_node","layer3_name",x_ladder,y_ladder,z_ladder,"layer3_name_mat");
// // // //         node_layer3->SetVisibility(0);
// // // //         fNodes->Add(node_layer3);
// // // // 
// // // //         for (ladder=0;ladder<nladders;ladder++) {
// // // //             if ((ladder%2)==0) radius=radius2;
// // // //             else radius=radius1;
// // // //             x_ladder=radius*(TMath::Cos((TMath::Pi()/2)+(ladder*radiant)));
// // // //             y_ladder=radius*(TMath::Sin((TMath::Pi()/2)+(ladder*radiant)));
// // // //             if ((ladder%2)==0) {
// // // //                 sprintf(name,"VertexBarrel_layer%d_ladder%douter_volume",layer,ladder);
// // // //                 sprintf(title,"VertexBarrel_layer%d_ladder%douter_volumeB",layer,ladder);
// // // //                 sprintf(name_node,"VertexBarrel_layer%d_ladder%douter_volumeC",layer,ladder);
// // // //                 sprintf(title_node,"VertexBarrel_layer%d_ladder%douter_volumeD",layer,ladder);
// // // //                 sprintf(name_mat,"VertexBarrel_layer%d_ladder%douter_volumeE",layer,ladder);
// // // //                 sprintf(title_mat,"VertexBarrel_layer%d_ladder%douter_volumeF",layer,ladder);
// // // //             } else {
// // // //                 sprintf(name,"VertexBarrel_layer%d_ladder%dinner_volume",layer,ladder);
// // // //                 sprintf(title,"VertexBarrel_layer%d_ladder%dinner_volumeB",layer,ladder);
// // // //                 sprintf(name_node,"VertexBarrel_layer%d_ladder%dinner_volumeC",layer,ladder);
// // // //                 sprintf(title_node,"VertexBarrel_layer%d_ladder%dinner_volumeD",layer,ladder);
// // // //                 sprintf(name_mat,"VertexBarrel_layer%d_ladder%dinner_volumeE",layer,ladder);
// // // //                 sprintf(title_mat,"VertexBarrel_layer%d_ladder%dinner_volumeF",layer,ladder);
// // // //             }
// // // //             lad_shape=(TGeoBBox *)gGeoManager->GetVolume(name)->GetShape();
// // // //             dimx_ladder=lad_shape->GetDX();
// // // //             dimy_ladder=lad_shape->GetDY();
// // // //             dimz_ladder=lad_shape->GetDZ();
// // // //             new TBRIK(name,title,"void",dimx_ladder,dimy_ladder,dimz_ladder);
// // // //             node_layer3->cd();
// // // //             new TRotMatrix(name_mat,title_mat,90,0-rot,90,90-rot,0,0);
// // // //             node=new TNode(name_node,title_node,name,x_ladder,y_ladder,z_ladder,name_mat);
// // // //             node->SetLineColor(kColorTARGET);
// // // //             fNodes->Add(node);
// // // //             rot=rot-gradi_rot;
// // // //         }
// // // // 
// // // // 
// // // // 	layer=Param->GetLayer_4();//layer=4;
// // // //         nladders=Param->GetNLadders_4();//nladders=30;
// // // //         gradi_rot=Param->GetGradi_Rot_4();//gradi_rot=360/nladders;
// // // //         radiant=Param->GetRadiant_4 ();//radiant=(2*TMath::Pi())/nladders;
// // // //         radius1=Param->GetRadius1_4 ()*Scale;//radius1=6.00*(5.0/3.5);
// // // //         radius2=Param->GetRadius2_4 ()*Scale;//radius2=6.0770*(5.0/3.5);
// // // //         x_ladder=Param->GetX_Ladder() ;//0.0;//x_ladder=0.0;
// // // //         y_ladder=Param->GetY_Ladder() ;//0.0;//y_ladder=0.0;
// // // //         z_ladder=Param->GetZ_Ladder() ;//0.0;//z_ladder=0.0;
// // // //         rot2=Param->GetRot2_4 () ;//rot2=6.0;
// // // //         rot=Param->GetRot();//0.0;//rot=0.0;
// // // // 	dimz_layer0=Param->GetDimz();//6.25;
// // // // 
// // // //         new TTUBE("layer4_name","layer4_title","void",radius1,radius2,dimz_layer0);
// // // //         top->cd();
// // // //         new TRotMatrix("layer4_name_mat","layer4_title_mat",90,0-rot2,90,90-rot2,0,0);
// // // //         node_layer4=new TNode("layer4_name_node","layer4_title_node","layer4_name",x_ladder,y_ladder,z_ladder,"layer4_name_mat");
// // // //         node_layer4->SetVisibility(0);
// // // //         fNodes->Add(node_layer4);
// // // // 
// // // //         for (ladder=0;ladder<nladders;ladder++) {
// // // //             if ((ladder%2)==0) radius=radius2;
// // // //             else radius=radius1;
// // // //             x_ladder=radius*(TMath::Cos((TMath::Pi()/2)+(ladder*radiant)));
// // // //             y_ladder=radius*(TMath::Sin((TMath::Pi()/2)+(ladder*radiant)));
// // // //             if ((ladder%2)==0) {
// // // //                 sprintf(name,"VertexBarrel_layer%d_ladder%douter_volume",layer,ladder);
// // // //                 sprintf(title,"VertexBarrel_layer%d_ladder%douter_volumeB",layer,ladder);
// // // //                 sprintf(name_node,"VertexBarrel_layer%d_ladder%douter_volumeC",layer,ladder);
// // // //                 sprintf(title_node,"VertexBarrel_layer%d_ladder%douter_volumeD",layer,ladder);
// // // //                 sprintf(name_mat,"VertexBarrel_layer%d_ladder%douter_volumeE",layer,ladder);
// // // //                 sprintf(title_mat,"VertexBarrel_layer%d_ladder%douter_volumeF",layer,ladder);
// // // //             } else {
// // // //                 sprintf(name,"VertexBarrel_layer%d_ladder%dinner_volume",layer,ladder);
// // // //                 sprintf(title,"VertexBarrel_layer%d_ladder%dinner_volumeB",layer,ladder);
// // // //                 sprintf(name_node,"VertexBarrel_layer%d_ladder%dinner_volumeC",layer,ladder);
// // // //                 sprintf(title_node,"VertexBarrel_layer%d_ladder%dinner_volumeD",layer,ladder);
// // // //                 sprintf(name_mat,"VertexBarrel_layer%d_ladder%dinner_volumeE",layer,ladder);
// // // //                 sprintf(title_mat,"VertexBarrel_layer%d_ladder%dinner_volumeF",layer,ladder);
// // // //             }
// // // //             lad_shape=(TGeoBBox *)gGeoManager->GetVolume(name)->GetShape();
// // // //             dimx_ladder=lad_shape->GetDX();
// // // //             dimy_ladder=lad_shape->GetDY();
// // // //             dimz_ladder=lad_shape->GetDZ();
// // // //             new TBRIK(name,title,"void",dimx_ladder,dimy_ladder,dimz_ladder);
// // // //             node_layer4->cd();
// // // //             new TRotMatrix(name_mat,title_mat,90,0-rot,90,90-rot,0,0);
// // // //             node=new TNode(name_node,title_node,name,x_ladder,y_ladder,z_ladder,name_mat);
// // // //             node->SetLineColor(kColorTARGET);
// // // //             fNodes->Add(node);
// // // //             rot=rot-gradi_rot;
// // // //         }
// // // //     } //end version in ladder
// // // // 
// // // //     //_________________________________________________________________________________________________
// // // // 
// // // //     {
// // // // 
// // // //         TARGETV=gGeoManager->GetVolume("TARGETV");
// // // //         for (endcap=0;endcap<nendcaps;endcap++) {
// // // // 
// // // //             sprintf(name,"VertexEndcap_layer%d_volume",endcap);
// // // //             sprintf(title,"VertexEndcap_layer%d_volumeB",endcap);
// // // //             sprintf(name_node,"VertexEndcap_layer%d_volumeC",endcap);
// // // //             sprintf(title_node,"VertexEndcap_layer%d_volumeD",endcap);
// // // // 
// // // //             endcap_shape=(TGeoTube *)gGeoManager->GetVolume(name)->GetShape();
// // // //             dim_rmin= endcap_shape->GetRmin();
// // // //             dim_rmax= endcap_shape->GetRmax();
// // // //             dim_z=    endcap_shape->GetDZ();
// // // //             for (iTARGET=0;iTARGET<TARGETV->GetNdaughters();iTARGET++) {
// // // //                 name_endcap=TARGETV->GetNode(iTARGET)->GetName();
// // // //                 if (name_endcap.Contains(name)) pos_z=TARGETV->GetNode(iTARGET)->GetMatrix()->GetTranslation()[2];
// // // //             }
// // // //             new TTUBE(name,title,"void",dim_rmin,dim_rmax,dim_z);
// // // //             top->cd();
// // // //             node = new TNode(name_node,title_node,name,0,0,pos_z,"");
// // // //             node->SetLineColor(kColorTARGETEndCap);
// // // //             fNodes->Add(node);
// // // //         }
// // // //     }


}

//______________________________________________________________________

void IlcTARGETv4th::CreateGeometry() {
    // TRANSFORMATION MATRICES
    // Combi transformation:
    Double_t dx,dy,dz,thx,thy,thz,phx,phy,phz,rmin,rmax,phi1,phi2,dphi,z,dx1,dx2,dy1,dy2;
    
    Int_t nz;
    IlcTARGETParam *Param=new IlcTARGETParam();//Scale's Factor
    Double_t Scale=Param->GetScale();
    
    Double_t SiThick=Param->GetSiThickness(); //Silicon Thickness
    Double_t CThick=Param->GetCThickness(); //CarbonFiber Thickness
    
    //Variable for the Correction of the Position of each CarbonFiber Support
    Double_t fRaylay0Outer=-1.0,fRaylay0Inner=-1.0;
    Double_t fRaylay1Outer=-1.0,fRaylay1Inner=-1.0;
    Double_t fRaylay2Outer=-1.0,fRaylay2Inner=-1.0;
    Double_t fRaylay3Outer=-1.0,fRaylay3Inner=-1.0;
    Double_t fRaylay4Outer=-1.0,fRaylay4Inner=-1.0;
    /*
    Double_t dxlay2Inner=-1.0,  dylay2Inner=-1.0;
    Double_t dxlay2Outer=-1.0,dylay2Outer=-1.0;
    Double_t dxlay3Inner=-1.0,dylay3Inner=-1.0;
    Double_t dxlay3Outer=-1.0,dylay3Outer=-1.0;
    Double_t dxlay4Inner=-1.0,dylay4Inner=-1.0;
    Double_t dxlay4Outer=-1.0,dylay4Outer=-1.0;
	*/    
    
    cout<<""<<endl;
    cout<<"---------------"<<endl;
    cout<<"Scale's Factor "<<Scale<<endl;
    cout<<"---------------"<<endl;
    cout<<""<<endl;
   
    //TGeoMedium *pMed1=gGeoManager->GetMedium(1);
    TGeoMedium *pMed2=gGeoManager->GetMedium("TARGET_Air");
    TGeoMedium *pMed3=gGeoManager->GetMedium("TARGET_Copper");
    TGeoMedium *pMed4=gGeoManager->GetMedium("TARGET_Iron");
    TGeoMedium *pMed5=gGeoManager->GetMedium("TARGET_CarbonFiber");
    TGeoMedium *pMed6=gGeoManager->GetMedium("TARGET_G10");
    TGeoMedium *pMed7=gGeoManager->GetMedium("TARGET_Titanium");
    TGeoMedium *pMed8=gGeoManager->GetMedium("TARGET_Silicon");
    TGeoMedium *pMed9=gGeoManager->GetMedium("TARGET_Beryllium");

    dx = 0.000000;
    dy = 0.000000;
    dz = 6.500000;
    TGeoCombiTrans *pMatrix7 = new TGeoCombiTrans("");
    pMatrix7->SetTranslation(dx,dy,dz);
    // Combi transformation:
    dx = 0.000000;
    dy = 0.000000;
    dz = -6.500000;
    // Rotation:
    thx = 90.000000;
    phx = 0.000000;
    thy = 90.000000;
    phy = 270.000000;
    thz = 180.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix9 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix8 = new TGeoCombiTrans("", dx,dy,dz,pMatrix9);
    // Combi transformation:
    dx = 0.000000;
    dy = 0.000000;
    dz = 6.500000;
    TGeoCombiTrans *pMatrix10 = new TGeoCombiTrans("");
    pMatrix10->SetTranslation(dx,dy,dz);
    // Combi transformation:
    dx = 0.000000;
    dy = 0.000000;
    dz = -6.500000;
    // Rotation:
    thx = 90.000000;
    phx = 0.000000;
    thy = 90.000000;
    phy = 270.000000;
    thz = 180.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix12 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix11 = new TGeoCombiTrans("", dx,dy,dz,pMatrix12);
    // Combi transformation:
    dx = 0.000000;
    dy = 0.000000;
    dz = 6.500000;
    TGeoCombiTrans *pMatrix13 = new TGeoCombiTrans("");
    pMatrix13->SetTranslation(dx,dy,dz);
    // Combi transformation:
    dx = 0.000000;
    dy = 0.000000;
    dz = -6.500000;
    // Rotation:
    thx = 90.000000;
    phx = 0.000000;
    thy = 90.000000;
    phy = 270.000000;
    thz = 180.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix15 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix14 = new TGeoCombiTrans("", dx,dy,dz,pMatrix15);
    // Combi transformation:
    dx = 0.000000;
    dy = 0.000000;
    dz = 6.500000;
    TGeoCombiTrans *pMatrix16 = new TGeoCombiTrans("");
    pMatrix16->SetTranslation(dx,dy,dz);
    // Combi transformation:
    dx = 0.000000;
    dy = 0.000000;
    dz = -6.500000;
    // Rotation:
    thx = 90.000000;
    phx = 0.000000;
    thy = 90.000000;
    phy = 270.000000;
    thz = 180.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix18 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix17 = new TGeoCombiTrans("", dx,dy,dz,pMatrix18);
    // Combi transformation:
    dx = 0.000000;
    dy = 0.000000;
    dz = 6.500000;
    TGeoCombiTrans *pMatrix19 = new TGeoCombiTrans("");
    pMatrix19->SetTranslation(dx,dy,dz);
    // Combi transformation:
    dx = 0.000000;
    dy = 0.000000;
    dz = -6.500000;
    // Rotation:
    thx = 90.000000;
    phx = 0.000000;
    thy = 90.000000;
    phy = 270.000000;
    thz = 180.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix21 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix20 = new TGeoCombiTrans("", dx,dy,dz,pMatrix21);
    // Combi transformation:
    dx = 0.000000;
    dy = 0.000000;
    dz = 6.802850;
    TGeoCombiTrans *pMatrix22 = new TGeoCombiTrans("");
    pMatrix22->SetTranslation(dx,dy,dz);
    // Combi transformation:
    dx = 0.000000;
    dy = 0.000000;
    dz = -6.802850;
    // Rotation:
    thx = 90.000000;
    phx = 0.000000;
    thy = 90.000000;
    phy = 270.000000;
    thz = 180.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix24 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix23 = new TGeoCombiTrans("", dx,dy,dz,pMatrix24);
    // Combi transformation:
    dx = 0.000000;
    dy = 0.000000;
    dz = 6.801550;
    TGeoCombiTrans *pMatrix25 = new TGeoCombiTrans("");
    pMatrix25->SetTranslation(dx,dy,dz);
    // Combi transformation:
    dx = 0.000000;
    dy = 0.000000;
    dz = -6.801550;
    // Rotation:
    thx = 90.000000;
    phx = 0.000000;
    thy = 90.000000;
    phy = 270.000000;
    thz = 180.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix27 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix26 = new TGeoCombiTrans("", dx,dy,dz,pMatrix27);
    // Combi transformation:
    dx = 0.000000;
    dy = 0.000000;
    dz = 6.800800;
    TGeoCombiTrans *pMatrix28 = new TGeoCombiTrans("");
    pMatrix28->SetTranslation(dx,dy,dz);
    // Combi transformation:
    dx = 0.000000;
    dy = 0.000000;
    dz = -6.800800;
    // Rotation:
    thx = 90.000000;
    phx = 0.000000;
    thy = 90.000000;
    phy = 270.000000;
    thz = 180.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix30 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix29 = new TGeoCombiTrans("", dx,dy,dz,pMatrix30);
    // Combi transformation:
    dx = 0.000000;
    dy = 0.000000;
    dz = 6.800350;
    TGeoCombiTrans *pMatrix31 = new TGeoCombiTrans("");
    pMatrix31->SetTranslation(dx,dy,dz);
    // Combi transformation:
    dx = 0.000000;
    dy = 0.000000;
    dz = -6.800350;
    // Rotation:
    thx = 90.000000;
    phx = 0.000000;
    thy = 90.000000;
    phy = 270.000000;
    thz = 180.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix33 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix32 = new TGeoCombiTrans("", dx,dy,dz,pMatrix33);
    // Combi transformation:
    dx = 0.000000;
    dy = 0.000000;
    dz = 7.600000;
    TGeoCombiTrans *pMatrix34 = new TGeoCombiTrans("");
    pMatrix34->SetTranslation(dx,dy,dz);
    // Combi transformation:
    dx = 0.000000;
    dy = 0.000000;
    dz = -7.600000;
    // Rotation:
    thx = 90.000000;
    phx = 0.000000;
    thy = 90.000000;
    phy = 270.000000;
    thz = 180.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix36 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix35 = new TGeoCombiTrans("", dx,dy,dz,pMatrix36);
    // Combi transformation:
    dx = 0.000000;
    dy = 0.000000;
    dz = 9.500000;
    TGeoCombiTrans *pMatrix37 = new TGeoCombiTrans("");
    pMatrix37->SetTranslation(dx,dy,dz);
    // Combi transformation:
    dx = 0.000000;
    dy = 0.000000;
    dz = -9.500000;
    // Rotation:
    thx = 90.000000;
    phx = 0.000000;
    thy = 90.000000;
    phy = 270.000000;
    thz = 180.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix39 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix38 = new TGeoCombiTrans("", dx,dy,dz,pMatrix39);
    // Combi transformation:
    dx = 0.000000;
    dy = 0.000000;
    dz = 12.500000;
    TGeoCombiTrans *pMatrix40 = new TGeoCombiTrans("");
    pMatrix40->SetTranslation(dx,dy,dz);
    // Combi transformation:
    dx = 0.000000;
    dy = 0.000000;
    dz = -12.500000;
    // Rotation:
    thx = 90.000000;
    phx = 0.000000;
    thy = 90.000000;
    phy = 270.000000;
    thz = 180.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix42 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix41 = new TGeoCombiTrans("", dx,dy,dz,pMatrix42);
    // Combi transformation:
    dx = 0.000000;
    dy = 0.000000;
    dz = 18.000000;
    TGeoCombiTrans *pMatrix43 = new TGeoCombiTrans("");
    pMatrix43->SetTranslation(dx,dy,dz);
    // Combi transformation:
    dx = 0.000000;
    dy = 0.000000;
    dz = -18.000000;
    // Rotation:
    thx = 90.000000;
    phx = 0.000000;
    thy = 90.000000;
    phy = 270.000000;
    thz = 180.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix45 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix44 = new TGeoCombiTrans("", dx,dy,dz,pMatrix45);
    // Combi transformation:
    dx = 0.000000;
    dy = 0.000000;
    dz = 7.500000;
    TGeoCombiTrans *pMatrix46 = new TGeoCombiTrans("");
    pMatrix46->SetTranslation(dx,dy,dz);
    // Combi transformation:
    dx = 0.000000;
    dy = 0.000000;
    dz = -7.500000;
    // Rotation:
    thx = 90.000000;
    phx = 0.000000;
    thy = 90.000000;
    phy = 270.000000;
    thz = 180.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix48 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix47 = new TGeoCombiTrans("", dx,dy,dz,pMatrix48);
    // Combi transformation:
    dx = 0.000000;
    dy = 0.000000;
    dz = 9.400000;
    TGeoCombiTrans *pMatrix49 = new TGeoCombiTrans("");
    pMatrix49->SetTranslation(dx,dy,dz);
    // Combi transformation:
    dx = 0.000000;
    dy = 0.000000;
    dz = -9.400000;
    // Rotation:
    thx = 90.000000;
    phx = 0.000000;
    thy = 90.000000;
    phy = 270.000000;
    thz = 180.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix51 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix50 = new TGeoCombiTrans("", dx,dy,dz,pMatrix51);
    // Combi transformation:
    dx = 0.000000;
    dy = 0.000000;
    dz = 12.400000;
    TGeoCombiTrans *pMatrix52 = new TGeoCombiTrans("");
    pMatrix52->SetTranslation(dx,dy,dz);
    // Combi transformation:
    dx = 0.000000;
    dy = 0.000000;
    dz = -12.400000;
    // Rotation:
    thx = 90.000000;
    phx = 0.000000;
    thy = 90.000000;
    phy = 270.000000;
    thz = 180.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix54 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix53 = new TGeoCombiTrans("", dx,dy,dz,pMatrix54);
    // Combi transformation:
    dx = 0.000000;
    dy = 0.000000;
    dz = 17.900000;
    TGeoCombiTrans *pMatrix55 = new TGeoCombiTrans("");
    pMatrix55->SetTranslation(dx,dy,dz);
    // Combi transformation:
    dx = 0.000000;
    dy = 0.000000;
    dz = -17.900000;
    // Rotation:
    thx = 90.000000;
    phx = 0.000000;
    thy = 90.000000;
    phy = 270.000000;
    thz = 180.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix57 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix56 = new TGeoCombiTrans("", dx,dy,dz,pMatrix57);
    // Combi transformation:
    dx = 0.000000;
    dy = 0.000000;
    dz = 7.605650;
    TGeoCombiTrans *pMatrix59 = new TGeoCombiTrans("");
    pMatrix59->SetTranslation(dx,dy,dz);
    // Combi transformation:
    dx = 0.000000*Scale;
    dy = 4.422222*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 0.000000;
    thy = 0.000000;
    phy = 0.000000;
    thz = 90.000000;
    phz = 270.000000;
    TGeoRotation *pMatrix129 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix128 = new TGeoCombiTrans("", dx,dy,dz,pMatrix129);
    // Combi transformation:
    dx = 2.211111*Scale;
    dy = 3.829756*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 330.000000;
    thy = 0.000000;
    phy = 0.000000;
    thz = 90.000000;
    phz = 240.000000;
    TGeoRotation *pMatrix131 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix130 = new TGeoCombiTrans("", dx,dy,dz,pMatrix131);
    // Combi transformation:
    dx = 3.829756*Scale;
    dy = 2.211111*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 300.000000;
    thy = 0.000000;
    phy = 0.000000;
    thz = 90.000000;
    phz = 210.000000;
    TGeoRotation *pMatrix133 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix132 = new TGeoCombiTrans("", dx,dy,dz,pMatrix133);
    // Combi transformation:
    dx = 4.422222*Scale;
    dy = 0.000000*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 270.000000;
    thy = 0.000000;
    phy = 0.000000;
    thz = 90.000000;
    phz = 180.000000;
    TGeoRotation *pMatrix135 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix134 = new TGeoCombiTrans("", dx,dy,dz,pMatrix135);
    // Combi transformation:
    dx = 3.829756*Scale;
    dy = -2.211111*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 240.000000;
    thy = 0.000000;
    phy = 0.000000;
    thz = 90.000000;
    phz = 150.000000;
    TGeoRotation *pMatrix137 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix136 = new TGeoCombiTrans("", dx,dy,dz,pMatrix137);
    // Combi transformation:
    dx = 2.211111*Scale;
    dy = -3.829756*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 210.000000;
    thy = 0.000000;
    phy = 0.000000;
    thz = 90.000000;
    phz = 120.000000;
    TGeoRotation *pMatrix139 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix138 = new TGeoCombiTrans("", dx,dy,dz,pMatrix139);
    // Combi transformation:
    dx = 0.000000*Scale;
    dy = -4.422222*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 180.000000;
    thy = 0.000000;
    phy = 0.000000;
    thz = 90.000000;
    phz = 90.000000;
    TGeoRotation *pMatrix141 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix140 = new TGeoCombiTrans("", dx,dy,dz,pMatrix141);
    // Combi transformation:
    dx = -2.211111*Scale;
    dy = -3.829756*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 150.000000;
    thy = 0.000000;
    phy = 0.000000;
    thz = 90.000000;
    phz = 60.000000;
    TGeoRotation *pMatrix143 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix142 = new TGeoCombiTrans("", dx,dy,dz,pMatrix143);
    // Combi transformation:
    dx = -3.829756*Scale;
    dy = -2.211110*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 120.000000;
    thy = 0.000000;
    phy = 0.000000;
    thz = 90.000000;
    phz = 30.000000;
    TGeoRotation *pMatrix145 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix144 = new TGeoCombiTrans("", dx,dy,dz,pMatrix145);
    // Combi transformation:
    dx = -4.422222*Scale;
    dy = 0.000001*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 90.000000;
    thy = 0.000000;
    phy = 0.000000;
    thz = 90.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix147 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix146 = new TGeoCombiTrans("", dx,dy,dz,pMatrix147);
    // Combi transformation:
    dx = -3.829756*Scale;
    dy = 2.211111*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 60.000000;
    thy = 0.000000;
    phy = 0.000000;
    thz = 90.000000;
    phz = 330.000000;
    TGeoRotation *pMatrix149 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix148 = new TGeoCombiTrans("", dx,dy,dz,pMatrix149);
    // Combi transformation:
    dx = -2.211110*Scale;
    dy = 3.829757*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 30.000000;
    thy = 0.000000;
    phy = 0.000000;
    thz = 90.000000;
    phz = 300.000000;
    TGeoRotation *pMatrix151 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix150 = new TGeoCombiTrans("", dx,dy,dz,pMatrix151);
    // Combi transformation:
    dx = 0.000000;
    dy = 0.000000;
    dz = 9.505650;
    TGeoCombiTrans *pMatrix60 = new TGeoCombiTrans("");
    pMatrix60->SetTranslation(dx,dy,dz);
    // Combi transformation:
    dx = 0.000000*Scale;
    dy = 4.422222*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 0.000000;
    thy = 0.000000;
    phy = 0.000000;
    thz = 90.000000;
    phz = 270.000000;
    TGeoRotation *pMatrix165 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix164 = new TGeoCombiTrans("", dx,dy,dz,pMatrix165);
    // Combi transformation:
    dx = 2.211111*Scale;
    dy = 3.829756*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 330.000000;
    thy = 0.000000;
    phy = 0.000000;
    thz = 90.000000;
    phz = 240.000000;
    TGeoRotation *pMatrix167 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix166 = new TGeoCombiTrans("", dx,dy,dz,pMatrix167);
    // Combi transformation:
    dx = 3.829756*Scale;
    dy = 2.211111*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 300.000000;
    thy = 0.000000;
    phy = 0.000000;
    thz = 90.000000;
    phz = 210.000000;
    TGeoRotation *pMatrix169 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix168 = new TGeoCombiTrans("", dx,dy,dz,pMatrix169);
    // Combi transformation:
    dx = 4.422222*Scale;
    dy = 0.000000*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 270.000000;
    thy = 0.000000;
    phy = 0.000000;
    thz = 90.000000;
    phz = 180.000000;
    TGeoRotation *pMatrix171 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix170 = new TGeoCombiTrans("", dx,dy,dz,pMatrix171);
    // Combi transformation:
    dx = 3.829756*Scale;
    dy = -2.211111*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 240.000000;
    thy = 0.000000;
    phy = 0.000000;
    thz = 90.000000;
    phz = 150.000000;
    TGeoRotation *pMatrix173 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix172 = new TGeoCombiTrans("", dx,dy,dz,pMatrix173);
    // Combi transformation:
    dx = 2.211111*Scale;
    dy = -3.829756*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 210.000000;
    thy = 0.000000;
    phy = 0.000000;
    thz = 90.000000;
    phz = 120.000000;
    TGeoRotation *pMatrix175 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix174 = new TGeoCombiTrans("", dx,dy,dz,pMatrix175);
    // Combi transformation:
    dx = 0.000000*Scale;
    dy = -4.422222*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 180.000000;
    thy = 0.000000;
    phy = 0.000000;
    thz = 90.000000;
    phz = 90.000000;
    TGeoRotation *pMatrix177 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix176 = new TGeoCombiTrans("", dx,dy,dz,pMatrix177);
    // Combi transformation:
    dx = -2.211111*Scale;
    dy = -3.829756*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 150.000000;
    thy = 0.000000;
    phy = 0.000000;
    thz = 90.000000;
    phz = 60.000000;
    TGeoRotation *pMatrix179 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix178 = new TGeoCombiTrans("", dx,dy,dz,pMatrix179);
    // Combi transformation:
    dx = -3.829756*Scale;
    dy = -2.211110*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 120.000000;
    thy = 0.000000;
    phy = 0.000000;
    thz = 90.000000;
    phz = 30.000000;
    TGeoRotation *pMatrix181 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix180 = new TGeoCombiTrans("", dx,dy,dz,pMatrix181);
    // Combi transformation:
    dx = -4.422222*Scale;
    dy = 0.000001*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 90.000000;
    thy = 0.000000;
    phy = 0.000000;
    thz = 90.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix183 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix182 = new TGeoCombiTrans("", dx,dy,dz,pMatrix183);
    // Combi transformation:
    dx = -3.829756*Scale;
    dy = 2.211111*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 60.000000;
    thy = 0.000000;
    phy = 0.000000;
    thz = 90.000000;
    phz = 330.000000;
    TGeoRotation *pMatrix185 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix184 = new TGeoCombiTrans("", dx,dy,dz,pMatrix185);
    // Combi transformation:
    dx = -2.211110*Scale;
    dy = 3.829757*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 30.000000;
    thy = 0.000000;
    phy = 0.000000;
    thz = 90.000000;
    phz = 300.000000;
    TGeoRotation *pMatrix187 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix186 = new TGeoCombiTrans("", dx,dy,dz,pMatrix187);
    // Combi transformation:
    dx = 0.000000;
    dy = 0.000000;
    dz = 12.505650;
    TGeoCombiTrans *pMatrix61 = new TGeoCombiTrans("");
    pMatrix61->SetTranslation(dx,dy,dz);
    // Combi transformation:
    dx = 0.000000*Scale;
    dy = 4.622222*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 0.000000;
    thy = 0.000000;
    phy = 0.000000;
    thz = 90.000000;
    phz = 270.000000;
    TGeoRotation *pMatrix201 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix200 = new TGeoCombiTrans("", dx,dy,dz,pMatrix201);
    // Combi transformation:
    dx = 2.311111*Scale;
    dy = 4.002962*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 330.000000;
    thy = 0.000000;
    phy = 0.000000;
    thz = 90.000000;
    phz = 240.000000;
    TGeoRotation *pMatrix203 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix202 = new TGeoCombiTrans("", dx,dy,dz,pMatrix203);
    // Combi transformation:
    dx = 4.002962*Scale;
    dy = 2.311111*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 300.000000;
    thy = 0.000000;
    phy = 0.000000;
    thz = 90.000000;
    phz = 210.000000;
    TGeoRotation *pMatrix205 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix204 = new TGeoCombiTrans("", dx,dy,dz,pMatrix205);
    // Combi transformation:
    dx = 4.622222*Scale;
    dy = 0.000000*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 270.000000;
    thy = 0.000000;
    phy = 0.000000;
    thz = 90.000000;
    phz = 180.000000;
    TGeoRotation *pMatrix207 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix206 = new TGeoCombiTrans("", dx,dy,dz,pMatrix207);
    // Combi transformation:
    dx = 4.002962*Scale;
    dy = -2.311111*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 240.000000;
    thy = 0.000000;
    phy = 0.000000;
    thz = 90.000000;
    phz = 150.000000;
    TGeoRotation *pMatrix209 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix208 = new TGeoCombiTrans("", dx,dy,dz,pMatrix209);
    // Combi transformation:
    dx = 2.311111*Scale;
    dy = -4.002962*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 210.000000;
    thy = 0.000000;
    phy = 0.000000;
    thz = 90.000000;
    phz = 120.000000;
    TGeoRotation *pMatrix211 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix210 = new TGeoCombiTrans("", dx,dy,dz,pMatrix211);
    // Combi transformation:
    dx = 0.000000*Scale;
    dy = -4.622222*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 180.000000;
    thy = 0.000000;
    phy = 0.000000;
    thz = 90.000000;
    phz = 90.000000;
    TGeoRotation *pMatrix213 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix212 = new TGeoCombiTrans("", dx,dy,dz,pMatrix213);
    // Combi transformation:
    dx = -2.311111*Scale;
    dy = -4.002961*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 150.000000;
    thy = 0.000000;
    phy = 0.000000;
    thz = 90.000000;
    phz = 60.000000;
    TGeoRotation *pMatrix215 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix214 = new TGeoCombiTrans("", dx,dy,dz,pMatrix215);
    // Combi transformation:
    dx = -4.002962*Scale;
    dy = -2.311110*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 120.000000;
    thy = 0.000000;
    phy = 0.000000;
    thz = 90.000000;
    phz = 30.000000;
    TGeoRotation *pMatrix217 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix216 = new TGeoCombiTrans("", dx,dy,dz,pMatrix217);
    // Combi transformation:
    dx = -4.622222*Scale;
    dy = 0.000001*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 90.000000;
    thy = 0.000000;
    phy = 0.000000;
    thz = 90.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix219 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix218 = new TGeoCombiTrans("", dx,dy,dz,pMatrix219);
    // Combi transformation:
    dx = -4.002961*Scale;
    dy = 2.311111*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 60.000000;
    thy = 0.000000;
    phy = 0.000000;
    thz = 90.000000;
    phz = 330.000000;
    TGeoRotation *pMatrix221 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix220 = new TGeoCombiTrans("", dx,dy,dz,pMatrix221);
    // Combi transformation:
    dx = -2.311110*Scale;
    dy = 4.002962*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 30.000000;
    thy = 0.000000;
    phy = 0.000000;
    thz = 90.000000;
    phz = 300.000000;
    TGeoRotation *pMatrix223 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix222 = new TGeoCombiTrans("", dx,dy,dz,pMatrix223);
    // Combi transformation:
    dx = 0.000000;
    dy = 0.000000;
    dz = 18.005650;
    TGeoCombiTrans *pMatrix62 = new TGeoCombiTrans("");
    pMatrix62->SetTranslation(dx,dy,dz);
    // Combi transformation:
    dx = 0.000000*Scale;
    dy = 4.622222*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 0.000000;
    thy = 0.000000;
    phy = 0.000000;
    thz = 90.000000;
    phz = 270.000000;
    TGeoRotation *pMatrix237 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix236 = new TGeoCombiTrans("", dx,dy,dz,pMatrix237);
    // Combi transformation:
    dx = 2.311111*Scale;
    dy = 4.002962*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 330.000000;
    thy = 0.000000;
    phy = 0.000000;
    thz = 90.000000;
    phz = 240.000000;
    TGeoRotation *pMatrix239 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix238 = new TGeoCombiTrans("", dx,dy,dz,pMatrix239);
    // Combi transformation:
    dx = 4.002962*Scale;
    dy = 2.311111*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 300.000000;
    thy = 0.000000;
    phy = 0.000000;
    thz = 90.000000;
    phz = 210.000000;
    TGeoRotation *pMatrix241 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix240 = new TGeoCombiTrans("", dx,dy,dz,pMatrix241);
    // Combi transformation:
    dx = 4.622222*Scale;
    dy = 0.000000*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 270.000000;
    thy = 0.000000;
    phy = 0.000000;
    thz = 90.000000;
    phz = 180.000000;
    TGeoRotation *pMatrix243 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix242 = new TGeoCombiTrans("", dx,dy,dz,pMatrix243);
    // Combi transformation:
    dx = 4.002962*Scale;
    dy = -2.311111*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 240.000000;
    thy = 0.000000;
    phy = 0.000000;
    thz = 90.000000;
    phz = 150.000000;
    TGeoRotation *pMatrix245 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix244 = new TGeoCombiTrans("", dx,dy,dz,pMatrix245);
    // Combi transformation:
    dx = 2.311111*Scale;
    dy = -4.002962*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 210.000000;
    thy = 0.000000;
    phy = 0.000000;
    thz = 90.000000;
    phz = 120.000000;
    TGeoRotation *pMatrix247 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix246 = new TGeoCombiTrans("", dx,dy,dz,pMatrix247);
    // Combi transformation:
    dx = 0.000000*Scale;
    dy = -4.622222*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 180.000000;
    thy = 0.000000;
    phy = 0.000000;
    thz = 90.000000;
    phz = 90.000000;
    TGeoRotation *pMatrix249 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix248 = new TGeoCombiTrans("", dx,dy,dz,pMatrix249);
    // Combi transformation:
    dx = -2.311111*Scale;
    dy = -4.002961*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 150.000000;
    thy = 0.000000;
    phy = 0.000000;
    thz = 90.000000;
    phz = 60.000000;
    TGeoRotation *pMatrix251 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix250 = new TGeoCombiTrans("", dx,dy,dz,pMatrix251);
    // Combi transformation:
    dx = -4.002962*Scale;
    dy = -2.311110*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 120.000000;
    thy = 0.000000;
    phy = 0.000000;
    thz = 90.000000;
    phz = 30.000000;
    TGeoRotation *pMatrix253 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix252 = new TGeoCombiTrans("", dx,dy,dz,pMatrix253);
    // Combi transformation:
    dx = -4.622222*Scale;
    dy = 0.000001*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 90.000000;
    thy = 0.000000;
    phy = 0.000000;
    thz = 90.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix255 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix254 = new TGeoCombiTrans("", dx,dy,dz,pMatrix255);
    // Combi transformation:
    dx = -4.002961*Scale;
    dy = 2.311111*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 60.000000;
    thy = 0.000000;
    phy = 0.000000;
    thz = 90.000000;
    phz = 330.000000;
    TGeoRotation *pMatrix257 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix256 = new TGeoCombiTrans("", dx,dy,dz,pMatrix257);
    // Combi transformation:
    dx = -2.311110*Scale;
    dy = 4.002962*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 30.000000;
    thy = 0.000000;
    phy = 0.000000;
    thz = 90.000000;
    phz = 300.000000;
    TGeoRotation *pMatrix259 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix258 = new TGeoCombiTrans("", dx,dy,dz,pMatrix259);
    // Combi transformation:
    dx = 0.000000;
    dy = 0.000000;
    dz = -7.605650;
    // Rotation:
    thx = 90.000000;
    phx = 0.000000;
    thy = 90.000000;
    phy = 270.000000;
    thz = 180.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix64 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix63 = new TGeoCombiTrans("", dx,dy,dz,pMatrix64);
    // Combi transformation:
    dx = 0.000000*Scale;
    dy = 4.422222*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 0.000000;
    thy = 0.000000;
    phy = 0.000000;
    thz = 90.000000;
    phz = 270.000000;
    TGeoRotation *pMatrix273 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix272 = new TGeoCombiTrans("", dx,dy,dz,pMatrix273);
    // Combi transformation:
    dx = 2.211111*Scale;
    dy = 3.829756*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 330.000000;
    thy = 0.000000;
    phy = 0.000000;
    thz = 90.000000;
    phz = 240.000000;
    TGeoRotation *pMatrix275 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix274 = new TGeoCombiTrans("", dx,dy,dz,pMatrix275);
    // Combi transformation:
    dx = 3.829756*Scale;
    dy = 2.211111*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 300.000000;
    thy = 0.000000;
    phy = 0.000000;
    thz = 90.000000;
    phz = 210.000000;
    TGeoRotation *pMatrix277 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix276 = new TGeoCombiTrans("", dx,dy,dz,pMatrix277);
    // Combi transformation:
    dx = 4.422222*Scale;
    dy = 0.000000*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 270.000000;
    thy = 0.000000;
    phy = 0.000000;
    thz = 90.000000;
    phz = 180.000000;
    TGeoRotation *pMatrix279 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix278 = new TGeoCombiTrans("", dx,dy,dz,pMatrix279);
    // Combi transformation:
    dx = 3.829756*Scale;
    dy = -2.211111*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 240.000000;
    thy = 0.000000;
    phy = 0.000000;
    thz = 90.000000;
    phz = 150.000000;
    TGeoRotation *pMatrix281 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix280 = new TGeoCombiTrans("", dx,dy,dz,pMatrix281);
    // Combi transformation:
    dx = 2.211111*Scale;
    dy = -3.829756*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 210.000000;
    thy = 0.000000;
    phy = 0.000000;
    thz = 90.000000;
    phz = 120.000000;
    TGeoRotation *pMatrix283 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix282 = new TGeoCombiTrans("", dx,dy,dz,pMatrix283);
    // Combi transformation:
    dx = 0.000000*Scale;
    dy = -4.422222*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 180.000000;
    thy = 0.000000;
    phy = 0.000000;
    thz = 90.000000;
    phz = 90.000000;
    TGeoRotation *pMatrix285 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix284 = new TGeoCombiTrans("", dx,dy,dz,pMatrix285);
    // Combi transformation:
    dx = -2.211111*Scale;
    dy = -3.829756*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 150.000000;
    thy = 0.000000;
    phy = 0.000000;
    thz = 90.000000;
    phz = 60.000000;
    TGeoRotation *pMatrix287 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix286 = new TGeoCombiTrans("", dx,dy,dz,pMatrix287);
    // Combi transformation:
    dx = -3.829756*Scale;
    dy = -2.211110*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 120.000000;
    thy = 0.000000;
    phy = 0.000000;
    thz = 90.000000;
    phz = 30.000000;
    TGeoRotation *pMatrix289 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix288 = new TGeoCombiTrans("", dx,dy,dz,pMatrix289);
    // Combi transformation:
    dx = -4.422222*Scale;
    dy = 0.000001*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 90.000000;
    thy = 0.000000;
    phy = 0.000000;
    thz = 90.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix291 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix290 = new TGeoCombiTrans("", dx,dy,dz,pMatrix291);
    // Combi transformation:
    dx = -3.829756*Scale;
    dy = 2.211111*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 60.000000;
    thy = 0.000000;
    phy = 0.000000;
    thz = 90.000000;
    phz = 330.000000;
    TGeoRotation *pMatrix293 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix292 = new TGeoCombiTrans("", dx,dy,dz,pMatrix293);
    // Combi transformation:
    dx = -2.211110*Scale;
    dy = 3.829757*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 30.000000;
    thy = 0.000000;
    phy = 0.000000;
    thz = 90.000000;
    phz = 300.000000;
    TGeoRotation *pMatrix295 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix294 = new TGeoCombiTrans("", dx,dy,dz,pMatrix295);
    // Combi transformation:
    dx = 0.000000;
    dy = 0.000000;
    dz = -9.505650;
    // Rotation:
    thx = 90.000000;
    phx = 0.000000;
    thy = 90.000000;
    phy = 270.000000;
    thz = 180.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix66 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix65 = new TGeoCombiTrans("", dx,dy,dz,pMatrix66);
    // Combi transformation:
    dx = 0.000000*Scale;
    dy = 4.422222*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 0.000000;
    thy = 0.000000;
    phy = 0.000000;
    thz = 90.000000;
    phz = 270.000000;
    TGeoRotation *pMatrix309 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix308 = new TGeoCombiTrans("", dx,dy,dz,pMatrix309);
    // Combi transformation:
    dx = 2.211111*Scale;
    dy = 3.829756*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 330.000000;
    thy = 0.000000;
    phy = 0.000000;
    thz = 90.000000;
    phz = 240.000000;
    TGeoRotation *pMatrix311 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix310 = new TGeoCombiTrans("", dx,dy,dz,pMatrix311);
    // Combi transformation:
    dx = 3.829756*Scale;
    dy = 2.211111*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 300.000000;
    thy = 0.000000;
    phy = 0.000000;
    thz = 90.000000;
    phz = 210.000000;
    TGeoRotation *pMatrix313 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix312 = new TGeoCombiTrans("", dx,dy,dz,pMatrix313);
    // Combi transformation:
    dx = 4.422222*Scale;
    dy = 0.000000*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 270.000000;
    thy = 0.000000;
    phy = 0.000000;
    thz = 90.000000;
    phz = 180.000000;
    TGeoRotation *pMatrix315 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix314 = new TGeoCombiTrans("", dx,dy,dz,pMatrix315);
    // Combi transformation:
    dx = 3.829756*Scale;
    dy = -2.211111*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 240.000000;
    thy = 0.000000;
    phy = 0.000000;
    thz = 90.000000;
    phz = 150.000000;
    TGeoRotation *pMatrix317 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix316 = new TGeoCombiTrans("", dx,dy,dz,pMatrix317);
    // Combi transformation:
    dx = 2.211111*Scale;
    dy = -3.829756*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 210.000000;
    thy = 0.000000;
    phy = 0.000000;
    thz = 90.000000;
    phz = 120.000000;
    TGeoRotation *pMatrix319 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix318 = new TGeoCombiTrans("", dx,dy,dz,pMatrix319);
    // Combi transformation:
    dx = 0.000000*Scale;
    dy = -4.422222*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 180.000000;
    thy = 0.000000;
    phy = 0.000000;
    thz = 90.000000;
    phz = 90.000000;
    TGeoRotation *pMatrix321 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix320 = new TGeoCombiTrans("", dx,dy,dz,pMatrix321);
    // Combi transformation:
    dx = -2.211111*Scale;
    dy = -3.829756*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 150.000000;
    thy = 0.000000;
    phy = 0.000000;
    thz = 90.000000;
    phz = 60.000000;
    TGeoRotation *pMatrix323 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix322 = new TGeoCombiTrans("", dx,dy,dz,pMatrix323);
    // Combi transformation:
    dx = -3.829756*Scale;
    dy = -2.211110*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 120.000000;
    thy = 0.000000;
    phy = 0.000000;
    thz = 90.000000;
    phz = 30.000000;
    TGeoRotation *pMatrix325 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix324 = new TGeoCombiTrans("", dx,dy,dz,pMatrix325);
    // Combi transformation:
    dx = -4.422222*Scale;
    dy = 0.000001*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 90.000000;
    thy = 0.000000;
    phy = 0.000000;
    thz = 90.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix327 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix326 = new TGeoCombiTrans("", dx,dy,dz,pMatrix327);
    // Combi transformation:
    dx = -3.829756*Scale;
    dy = 2.211111*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 60.000000;
    thy = 0.000000;
    phy = 0.000000;
    thz = 90.000000;
    phz = 330.000000;
    TGeoRotation *pMatrix329 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix328 = new TGeoCombiTrans("", dx,dy,dz,pMatrix329);
    // Combi transformation:
    dx = -2.211110*Scale;
    dy = 3.829757*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 30.000000;
    thy = 0.000000;
    phy = 0.000000;
    thz = 90.000000;
    phz = 300.000000;
    TGeoRotation *pMatrix331 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix330 = new TGeoCombiTrans("", dx,dy,dz,pMatrix331);
    // Combi transformation:
    dx = 0.000000;
    dy = 0.000000;
    dz = -12.505650;
    // Rotation:
    thx = 90.000000;
    phx = 0.000000;
    thy = 90.000000;
    phy = 270.000000;
    thz = 180.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix68 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix67 = new TGeoCombiTrans("", dx,dy,dz,pMatrix68);
    // Combi transformation:
    dx = 0.000000*Scale;
    dy = 4.622222*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 0.000000;
    thy = 0.000000;
    phy = 0.000000;
    thz = 90.000000;
    phz = 270.000000;
    TGeoRotation *pMatrix345 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix344 = new TGeoCombiTrans("", dx,dy,dz,pMatrix345);
    // Combi transformation:
    dx = 2.311111*Scale;
    dy = 4.002962*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 330.000000;
    thy = 0.000000;
    phy = 0.000000;
    thz = 90.000000;
    phz = 240.000000;
    TGeoRotation *pMatrix347 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix346 = new TGeoCombiTrans("", dx,dy,dz,pMatrix347);
    // Combi transformation:
    dx = 4.002962*Scale;
    dy = 2.311111*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 300.000000;
    thy = 0.000000;
    phy = 0.000000;
    thz = 90.000000;
    phz = 210.000000;
    TGeoRotation *pMatrix349 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix348 = new TGeoCombiTrans("", dx,dy,dz,pMatrix349);
    // Combi transformation:
    dx = 4.622222*Scale;
    dy = 0.000000*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 270.000000;
    thy = 0.000000;
    phy = 0.000000;
    thz = 90.000000;
    phz = 180.000000;
    TGeoRotation *pMatrix351 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix350 = new TGeoCombiTrans("", dx,dy,dz,pMatrix351);
    // Combi transformation:
    dx = 4.002962*Scale;
    dy = -2.311111*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 240.000000;
    thy = 0.000000;
    phy = 0.000000;
    thz = 90.000000;
    phz = 150.000000;
    TGeoRotation *pMatrix353 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix352 = new TGeoCombiTrans("", dx,dy,dz,pMatrix353);
    // Combi transformation:
    dx = 2.311111*Scale;
    dy = -4.002962*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 210.000000;
    thy = 0.000000;
    phy = 0.000000;
    thz = 90.000000;
    phz = 120.000000;
    TGeoRotation *pMatrix355 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix354 = new TGeoCombiTrans("", dx,dy,dz,pMatrix355);
    // Combi transformation:
    dx = 0.000000*Scale;
    dy = -4.622222*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 180.000000;
    thy = 0.000000;
    phy = 0.000000;
    thz = 90.000000;
    phz = 90.000000;
    TGeoRotation *pMatrix357 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix356 = new TGeoCombiTrans("", dx,dy,dz,pMatrix357);
    // Combi transformation:
    dx = -2.311111*Scale;
    dy = -4.002961*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 150.000000;
    thy = 0.000000;
    phy = 0.000000;
    thz = 90.000000;
    phz = 60.000000;
    TGeoRotation *pMatrix359 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix358 = new TGeoCombiTrans("", dx,dy,dz,pMatrix359);
    // Combi transformation:
    dx = -4.002962*Scale;
    dy = -2.311110*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 120.000000;
    thy = 0.000000;
    phy = 0.000000;
    thz = 90.000000;
    phz = 30.000000;
    TGeoRotation *pMatrix361 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix360 = new TGeoCombiTrans("", dx,dy,dz,pMatrix361);
    // Combi transformation:
    dx = -4.622222*Scale;
    dy = 0.000001*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 90.000000;
    thy = 0.000000;
    phy = 0.000000;
    thz = 90.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix363 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix362 = new TGeoCombiTrans("", dx,dy,dz,pMatrix363);
    // Combi transformation:
    dx = -4.002961*Scale;
    dy = 2.311111*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 60.000000;
    thy = 0.000000;
    phy = 0.000000;
    thz = 90.000000;
    phz = 330.000000;
    TGeoRotation *pMatrix365 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix364 = new TGeoCombiTrans("", dx,dy,dz,pMatrix365);
    // Combi transformation:
    dx = -2.311110*Scale;
    dy = 4.002962*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 30.000000;
    thy = 0.000000;
    phy = 0.000000;
    thz = 90.000000;
    phz = 300.000000;
    TGeoRotation *pMatrix367 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix366 = new TGeoCombiTrans("", dx,dy,dz,pMatrix367);
    // Combi transformation:
    dx = 0.000000;
    dy = 0.000000;
    dz = -18.005650;
    // Rotation:
    thx = 90.000000;
    phx = 0.000000;
    thy = 90.000000;
    phy = 270.000000;
    thz = 180.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix70 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix69 = new TGeoCombiTrans("", dx,dy,dz,pMatrix70);
    // Combi transformation:
    dx = 0.000000*Scale;
    dy = 4.622222*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 0.000000;
    thy = 0.000000;
    phy = 0.000000;
    thz = 90.000000;
    phz = 270.000000;
    TGeoRotation *pMatrix381 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix380 = new TGeoCombiTrans("", dx,dy,dz,pMatrix381);
    // Combi transformation:
    dx = 2.311111*Scale;
    dy = 4.002962*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 330.000000;
    thy = 0.000000;
    phy = 0.000000;
    thz = 90.000000;
    phz = 240.000000;
    TGeoRotation *pMatrix383 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix382 = new TGeoCombiTrans("", dx,dy,dz,pMatrix383);
    // Combi transformation:
    dx = 4.002962*Scale;
    dy = 2.311111*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 300.000000;
    thy = 0.000000;
    phy = 0.000000;
    thz = 90.000000;
    phz = 210.000000;
    TGeoRotation *pMatrix385 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix384 = new TGeoCombiTrans("", dx,dy,dz,pMatrix385);
    // Combi transformation:
    dx = 4.622222*Scale;
    dy = 0.000000*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 270.000000;
    thy = 0.000000;
    phy = 0.000000;
    thz = 90.000000;
    phz = 180.000000;
    TGeoRotation *pMatrix387 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix386 = new TGeoCombiTrans("", dx,dy,dz,pMatrix387);
    // Combi transformation:
    dx = 4.002962*Scale;
    dy = -2.311111*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 240.000000;
    thy = 0.000000;
    phy = 0.000000;
    thz = 90.000000;
    phz = 150.000000;
    TGeoRotation *pMatrix389 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix388 = new TGeoCombiTrans("", dx,dy,dz,pMatrix389);
    // Combi transformation:
    dx = 2.311111*Scale;
    dy = -4.002962*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 210.000000;
    thy = 0.000000;
    phy = 0.000000;
    thz = 90.000000;
    phz = 120.000000;
    TGeoRotation *pMatrix391 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix390 = new TGeoCombiTrans("", dx,dy,dz,pMatrix391);
    // Combi transformation:
    dx = 0.000000*Scale;
    dy = -4.622222*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 180.000000;
    thy = 0.000000;
    phy = 0.000000;
    thz = 90.000000;
    phz = 90.000000;
    TGeoRotation *pMatrix393 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix392 = new TGeoCombiTrans("", dx,dy,dz,pMatrix393);
    // Combi transformation:
    dx = -2.311111*Scale;
    dy = -4.002961*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 150.000000;
    thy = 0.000000;
    phy = 0.000000;
    thz = 90.000000;
    phz = 60.000000;
    TGeoRotation *pMatrix395 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix394 = new TGeoCombiTrans("", dx,dy,dz,pMatrix395);
    // Combi transformation:
    dx = -4.002962*Scale;
    dy = -2.311110*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 120.000000;
    thy = 0.000000;
    phy = 0.000000;
    thz = 90.000000;
    phz = 30.000000;
    TGeoRotation *pMatrix397 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix396 = new TGeoCombiTrans("", dx,dy,dz,pMatrix397);
    // Combi transformation:
    dx = -4.622222*Scale;
    dy = 0.000001*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 90.000000;
    thy = 0.000000;
    phy = 0.000000;
    thz = 90.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix399 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix398 = new TGeoCombiTrans("", dx,dy,dz,pMatrix399);
    // Combi transformation:
    dx = -4.002961*Scale;
    dy = 2.311111*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 60.000000;
    thy = 0.000000;
    phy = 0.000000;
    thz = 90.000000;
    phz = 330.000000;
    TGeoRotation *pMatrix401 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix400 = new TGeoCombiTrans("", dx,dy,dz,pMatrix401);
    // Combi transformation:
    dx = -2.311110*Scale;
    dy = 4.002962*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 30.000000;
    thy = 0.000000;
    phy = 0.000000;
    thz = 90.000000;
    phz = 300.000000;
    TGeoRotation *pMatrix403 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix402 = new TGeoCombiTrans("", dx,dy,dz,pMatrix403);
    // Combi transformation:
    dx = 0.000000;
    dy = 0.000000;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 345.000000;
    thy = 90.000000;
    phy = 75.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix77 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix76 = new TGeoCombiTrans("", dx,dy,dz,pMatrix77);

    //Matrix for Ladder of Layer0-Silicon
    
    // Combi transformation:
    dx = 0.000000*Scale;
    dy = 1.515400*Scale;
    dz = 0.000000;
    TGeoCombiTrans *pMatrix416 = new TGeoCombiTrans("");
    pMatrix416->SetTranslation(dx,dy,dz);
    // Combi transformation:
    dx = -0.700000*Scale;
    dy = 1.212435*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 30.000000;
    thy = 90.000000;
    phy = 120.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix418 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix417 = new TGeoCombiTrans("", dx,dy,dz,pMatrix418);
    // Combi transformation:
    dx = -1.312375*Scale;
    dy = 0.757700*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 60.000000;
    thy = 90.000000;
    phy = 150.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix420 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix419 = new TGeoCombiTrans("", dx,dy,dz,pMatrix420);
    // Combi transformation:
    dx = -1.400000*Scale;
    dy = 0.000000*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 90.000000;
    thy = 90.000000;
    phy = 180.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix422 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix421 = new TGeoCombiTrans("", dx,dy,dz,pMatrix422);
    // Combi transformation:
    dx = -1.312375*Scale;
    dy = -0.757700*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 120.000000;
    thy = 90.000000;
    phy = 210.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix424 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix423 = new TGeoCombiTrans("", dx,dy,dz,pMatrix424);
    // Combi transformation:
    dx = -0.700000*Scale;
    dy = -1.212436*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 150.000000;
    thy = 90.000000;
    phy = 240.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix426 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix425 = new TGeoCombiTrans("", dx,dy,dz,pMatrix426);
    // Combi transformation:
    dx = 0.000000*Scale;
    dy = -1.515400*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 180.000000;
    thy = 90.000000;
    phy = 270.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix428 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix427 = new TGeoCombiTrans("", dx,dy,dz,pMatrix428);
    // Combi transformation:
    dx = 0.700000*Scale;
    dy = -1.212435*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 210.000000;
    thy = 90.000000;
    phy = 300.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix430 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix429 = new TGeoCombiTrans("", dx,dy,dz,pMatrix430);
    // Combi transformation:
    dx = 1.31237*Scale;
    dy = -0.757700*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 240.000000;
    thy = 90.000000;
    phy = 330.0000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix432 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix431 = new TGeoCombiTrans("", dx,dy,dz,pMatrix432);
    // Combi transformation:
    dx = 1.400000*Scale;
    dy = 0.000000*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 270.000000;
    thy = 90.000000;
    phy = 0.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix434 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix433 = new TGeoCombiTrans("", dx,dy,dz,pMatrix434);
    
    // Combi transformation:
    dx = 1.312375*Scale;
    dy = 0.757700*Scale;
    dz = 0.000000;
    //Valori per Correzione OuterVolume di Carbonio per il layer0 - Barrel
    fRaylay0Outer=(TMath::Sqrt(pow(dx,2)+pow(dy,2)))+SiThick+CThick;    
    // Rotation:
    thx = 90.000000;
    phx = 300.000000;
    thy = 90.000000;
    phy = 30.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix436 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix435 = new TGeoCombiTrans("", dx,dy,dz,pMatrix436);
    // Combi transformation:
    dx = 0.700000*Scale;
    dy = 1.212436*Scale;
    dz = 0.000000;
    //Valori per Correzione InnerVolume di Carbonio per il layer0 - Barrel
    fRaylay0Inner=(TMath::Sqrt(pow(dx,2)+pow(dy,2)))+SiThick+CThick;
// Rotation:
    thx = 90.000000;
    phx = 330.000000;
    thy = 90.000000;
    phy = 60.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix438 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix437 = new TGeoCombiTrans("", dx,dy,dz,pMatrix438);
    
    
    
    /**/
     //Matrici per i  Ladder del  Layer1 - Silicio
    // Combi transformation:
    dx = 0.000000;
    dy = 2.313200*Scale;
    dz = 0.000000;
    TGeoCombiTrans *pMatrix474 = new TGeoCombiTrans("");
    pMatrix474->SetTranslation(dx,dy,dz);
    // Combi transformation:
    dx = -1.100000*Scale;
    dy = 1.905256*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 30.000000;
    thy = 90.000000;
    phy = 120.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix476 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix475 = new TGeoCombiTrans("", dx,dy,dz,pMatrix476);
    // Combi transformation:
    dx = -2.003290*Scale;
    dy = 1.156600*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 60.000000;
    thy = 90.000000;
    phy = 150.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix478 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix477 = new TGeoCombiTrans("", dx,dy,dz,pMatrix478);
    // Combi transformation:
    dx = -2.200000*Scale;
    dy = 0.000000;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 90.000000;
    thy = 90.000000;
    phy = 180.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix480 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix479 = new TGeoCombiTrans("", dx,dy,dz,pMatrix480);
    // Combi transformation:
    dx = -2.003290*Scale;
    dy = -1.156600*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 120.000000;
    thy = 90.000000;
    phy = 210.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix482 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix481 = new TGeoCombiTrans("", dx,dy,dz,pMatrix482);
    // Combi transformation:
    dx = -1.100000*Scale;
    dy = -1.905256*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 150.000000;
    thy = 90.000000;
    phy = 240.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix484 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix483 = new TGeoCombiTrans("", dx,dy,dz,pMatrix484);
    // Combi transformation:
    dx = 0.000000*Scale;
    dy = -2.313200*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 180.000000;
    thy = 90.000000;
    phy = 270.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix486 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix485 = new TGeoCombiTrans("", dx,dy,dz,pMatrix486);
    // Combi transformation:
    dx = 1.100000*Scale;
    dy = -1.905256*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 210.000000;
    thy = 90.000000;
    phy = 300.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix488 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix487 = new TGeoCombiTrans("", dx,dy,dz,pMatrix488);
    // Combi transformation:
    dx = 2.003290*Scale;
    dy = -1.156600*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 240.000000;
    thy = 90.000000;
    phy = 330.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix490 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix489 = new TGeoCombiTrans("", dx,dy,dz,pMatrix490);
    // Combi transformation:
    dx = 2.200000*Scale;
    dy = 0.000000;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 270.000000;
    thy = 90.000000;
    phy = 0.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix492 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix491 = new TGeoCombiTrans("", dx,dy,dz,pMatrix492);
    // Combi transformation:
    dx = 2.003290*Scale;
    dy = 1.156600*Scale;
    dz = 0.000000;
    //Ultimo silicio esterno
    //Raggio Esterno
    fRaylay1Outer=(TMath::Sqrt(pow(dx,2)+pow(dy,2)))+SiThick+CThick;    
    // Rotation:
    thx = 90.000000;
    phx = 300.000000;
    thy = 90.000000;
    phy = 30.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix494 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix493 = new TGeoCombiTrans("", dx,dy,dz,pMatrix494);
    // Combi transformation:
    dx = 1.100000*Scale;
    dy = 1.905256*Scale;
    dz = 0.000000;
    //Ultimo silicio interno
    //Raggio Interno
    fRaylay1Inner=(TMath::Sqrt(pow(dx,2)+pow(dy,2)))+SiThick+CThick;    
    
    // Rotation:
    thx = 90.000000;
    phx = 330.000000;
    thy = 90.000000;
    phy = 60.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix496 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix495 = new TGeoCombiTrans("", dx,dy,dz,pMatrix496);

   //Matrici per i  Ladder del  Layer2 - Silicio
     // Combi transformation:
    dx = 0.000000*Scale;
    dy = 3.589000*Scale;
    dz = 0.000000;
    TGeoCombiTrans *pMatrix567 = new TGeoCombiTrans("");
    pMatrix567->SetTranslation(dx,dy,dz);
    // Combi transformation:
    dx = -1.197070*Scale;
    dy = 3.288924*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 20.000000;
    thy = 90.000000;
    phy = 110.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix569 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix568 = new TGeoCombiTrans("", dx,dy,dz,pMatrix569);
    // Combi transformation:
    dx = -2.306965*Scale;
    dy = 2.749334*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 40.000000;
    thy = 90.000000;
    phy = 130.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix571 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix570 = new TGeoCombiTrans("", dx,dy,dz,pMatrix571);
    // Combi transformation:
    dx = -3.031089*Scale;
    dy = 1.750000*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 60.000000;
    thy = 90.000000;
    phy = 150.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix573 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix572 = new TGeoCombiTrans("", dx,dy,dz,pMatrix573);
    // Combi transformation:
    dx = -3.534475*Scale;
    dy = 0.623223*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 80.000000;
    thy = 90.000000;
    phy = 170.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix575 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix574 = new TGeoCombiTrans("", dx,dy,dz,pMatrix575);
    // Combi transformation:
    dx = -3.446827*Scale;
    dy = -0.607768*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 100.000000;
    thy = 90.000000;
    phy = 190.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix577 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix576 = new TGeoCombiTrans("", dx,dy,dz,pMatrix577);
    // Combi transformation:
    dx = -3.108165*Scale;
    dy = -1.794500*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 120.000000;
    thy = 90.000000;
    phy = 210.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix579 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix578 = new TGeoCombiTrans("", dx,dy,dz,pMatrix579);
    // Combi transformation:
    dx = -2.249757*Scale;
    dy = -2.681155*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 140.000000;
    thy = 90.000000;
    phy = 230.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix581 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix580 = new TGeoCombiTrans("", dx,dy,dz,pMatrix581);
    // Combi transformation:
    dx = -1.227511*Scale;
    dy = -3.372557*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 160.000000;
    thy = 90.000000;
    phy = 250.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix583 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix582 = new TGeoCombiTrans("", dx,dy,dz,pMatrix583);
    // Combi transformation:
    dx = 0.000000*Scale;
    dy = -3.500000*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 180.000000;
    thy = 90.000000;
    phy = 270.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix585 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix584 = new TGeoCombiTrans("", dx,dy,dz,pMatrix585);
    // Combi transformation:
    dx = 1.227510*Scale;
    dy = -3.372557*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 200.000000;
    thy = 90.000000;
    phy = 290.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix587 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix586 = new TGeoCombiTrans("", dx,dy,dz,pMatrix587);
    // Combi transformation:
    dx = 2.249756*Scale;
    dy = -2.681156*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 220.000000;
    thy = 90.000000;
    phy = 310.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix589 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix588 = new TGeoCombiTrans("", dx,dy,dz,pMatrix589);
    // Combi transformation:
    dx = 3.108165*Scale;
    dy = -1.794500*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 240.000000;
    thy = 90.000000;
    phy = 330.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix591 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix590 = new TGeoCombiTrans("", dx,dy,dz,pMatrix591);
    // Combi transformation:
    dx = 3.446827*Scale;
    dy = -0.607769*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 260.000000;
    thy = 90.000000;
    phy = 350.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix593 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix592 = new TGeoCombiTrans("", dx,dy,dz,pMatrix593);
    // Combi transformation:
    dx = 3.534475*Scale;
    dy = 0.623223*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 280.000000;
    thy = 90.000000;
    phy = 10.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix595 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix594 = new TGeoCombiTrans("", dx,dy,dz,pMatrix595);
    // Combi transformation:
    dx = 3.031089*Scale;
    dy = 1.750000*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 300.000000;
    thy = 90.000000;
    phy = 30.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix597 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix596 = new TGeoCombiTrans("", dx,dy,dz,pMatrix597);
    // Combi transformation:
    dx = 2.306965*Scale;//=dxlay2Outer;
    dy = 2.749333*Scale;//=dylay2Outer;
    dz = 0.000000;
    //dxlay2Outer=2.306965*Scale;//See pMatrix598
    //dylay2Outer=2.749333*Scale;
    fRaylay2Outer=(TMath::Sqrt(pow(dx,2)+pow(dy,2)))+SiThick+CThick;
    
    // Rotation:
    thx = 90.000000;
    phx = 320.000000;
    thy = 90.000000;
    phy = 50.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix599 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix598 = new TGeoCombiTrans("", dx,dy,dz,pMatrix599);
    // Combi transformation:
    dx = 1.197071*Scale;//=dxlay2Inner;
    dy = 3.288924*Scale;//=dylay2Inner;
    dz = 0.000000;
    fRaylay2Inner=(TMath::Sqrt(pow(dx,2)+pow(dy,2)))+SiThick+CThick;
    
    // Rotation:
    thx = 90.000000;
    phx = 340.000000;
    thy = 90.000000;
    phy = 70.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix601 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix600 = new TGeoCombiTrans("", dx,dy,dz,pMatrix601);  
    
    //Matrici per i  Ladder del  Layer3 - Silicio
    // Combi transformation:
    dx = 0.000000*Scale;
    dy = 4.840900*Scale;
    dz = 0.000000;
    TGeoCombiTrans *pMatrix667 = new TGeoCombiTrans("");
    pMatrix667->SetTranslation(dx,dy,dz);
    // Combi transformation:
    dx = -1.231979*Scale;
    dy = 4.597807*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 15.000000;
    thy = 90.000000;
    phy = 105.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix669 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix668 = new TGeoCombiTrans("", dx,dy,dz,pMatrix669);
    // Combi transformation:
    dx = -2.420450*Scale;
    dy = 4.192342*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 30.000000;
    thy = 90.000000;
    phy = 120.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix671 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix670 = new TGeoCombiTrans("", dx,dy,dz,pMatrix671);
    // Combi transformation:
    dx = -3.365829*Scale;
    dy = 3.365828*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 45.000000;
    thy = 90.000000;
    phy = 135.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix673 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix672 = new TGeoCombiTrans("", dx,dy,dz,pMatrix673);
    // Combi transformation:
    dx = -4.192342*Scale;
    dy = 2.420450*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 60.000000;
    thy = 90.000000;
    phy = 150.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix675 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix674 = new TGeoCombiTrans("", dx,dy,dz,pMatrix675);
    // Combi transformation:
    dx = -4.597807*Scale;
    dy = 1.231979*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 75.000000;
    thy = 90.000000;
    phy = 165.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix677 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix676 = new TGeoCombiTrans("", dx,dy,dz,pMatrix677);
    // Combi transformation:
    dx = -4.840900*Scale;
    dy = 0.000000*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 90.000000;
    thy = 90.000000;
    phy = 180.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix679 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix678 = new TGeoCombiTrans("", dx,dy,dz,pMatrix679);
    // Combi transformation:
    dx = -4.597807*Scale;
    dy = -1.231979*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 105.000000;
    thy = 90.000000;
    phy = 195.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix681 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix680 = new TGeoCombiTrans("", dx,dy,dz,pMatrix681);
    // Combi transformation:
    dx = -4.192342*Scale;
    dy = -2.420450*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 120.000000;
    thy = 90.000000;
    phy = 210.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix683 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix682 = new TGeoCombiTrans("", dx,dy,dz,pMatrix683);
    // Combi transformation:
    dx = -3.365828*Scale;
    dy = -3.365829*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 135.000000;
    thy = 90.000000;
    phy = 225.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix685 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix684 = new TGeoCombiTrans("", dx,dy,dz,pMatrix685);
    // Combi transformation:
    dx = -2.420450*Scale;
    dy = -4.192342*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 150.000000;
    thy = 90.000000;
    phy = 240.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix687 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix686 = new TGeoCombiTrans("", dx,dy,dz,pMatrix687);
    // Combi transformation:
    dx = -1.231978*Scale;
    dy = -4.597807*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 165.000000;
    thy = 90.000000;
    phy = 255.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix689 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix688 = new TGeoCombiTrans("", dx,dy,dz,pMatrix689);
    // Combi transformation:
    dx = 0.000000*Scale;
    dy = -4.840900*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 180.000000;
    thy = 90.000000;
    phy = 270.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix691 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix690 = new TGeoCombiTrans("", dx,dy,dz,pMatrix691);
    // Combi transformation:
    dx = 1.231979*Scale;
    dy = -4.597807*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 195.000000;
    thy = 90.000000;
    phy = 285.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix693 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix692 = new TGeoCombiTrans("", dx,dy,dz,pMatrix693);
    // Combi transformation:
    dx = 2.420450*Scale;
    dy = -4.192342*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 210.000000;
    thy = 90.000000;
    phy = 300.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix695 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix694 = new TGeoCombiTrans("", dx,dy,dz,pMatrix695);
    // Combi transformation:
    dx = 3.365829*Scale;
    dy = -3.365828*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 225.000000;
    thy = 90.000000;
    phy = 315.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix697 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix696 = new TGeoCombiTrans("", dx,dy,dz,pMatrix697);
    // Combi transformation:
    dx = 4.192343*Scale;
    dy = -2.420449*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 240.000000;
    thy = 90.000000;
    phy = 330.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix699 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix698 = new TGeoCombiTrans("", dx,dy,dz,pMatrix699);
    // Combi transformation:
    dx = 4.597807*Scale;
    dy = -1.231978*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 255.000000;
    thy = 90.000000;
    phy = 345.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix701 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix700 = new TGeoCombiTrans("", dx,dy,dz,pMatrix701);
    // Combi transformation:
    dx = 4.840900*Scale;
    dy = 0.000001*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 270.000000;
    thy = 90.000000;
    phy = 0.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix703 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix702 = new TGeoCombiTrans("", dx,dy,dz,pMatrix703);
    // Combi transformation:
    dx = 4.597807*Scale;
    dy = 1.231979*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 285.000000;
    thy = 90.000000;
    phy = 15.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix705 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix704 = new TGeoCombiTrans("", dx,dy,dz,pMatrix705);
    // Combi transformation:
    dx = 4.192342*Scale;
    dy = 2.420451*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 300.000000;
    thy = 90.000000;
    phy = 30.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix707 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix706 = new TGeoCombiTrans("", dx,dy,dz,pMatrix707);
    // Combi transformation:
    dx = 3.365828*Scale;
    dy = 3.365829*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 315.000000;
    thy = 90.000000;
    phy = 45.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix709 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix708 = new TGeoCombiTrans("", dx,dy,dz,pMatrix709);
    // Combi transformation:
    dx = 2.420449*Scale;//dxlay3Outer=2.420449*Scale
    dy = 4.192343*Scale;//dylay3Outer=4.192343*Scale
    dz = 0.000000;
    fRaylay3Outer=(TMath::Sqrt(pow(dx,2)+pow(dy,2)))+SiThick+CThick;
    // Rotation:
    thx = 90.000000;
    phx = 330.000000;
    thy = 90.000000;
    phy = 60.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix711 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix710 = new TGeoCombiTrans("", dx,dy,dz,pMatrix711);
    // Combi transformation:
    dx = 1.231978*Scale;//dxlay3Inner=1.231978*Scale;
    dy = 4.597807*Scale;//dylay3Inner=4.597807*Scale;
    dz = 0.000000;
    //dxlay3Inner=1.231978*Scale;//See pMatrix712
    //dylay3Inner=4.597807*Scale;
    fRaylay3Inner=(TMath::Sqrt(pow(dx,2)+pow(dy,2)))+SiThick+CThick;
    
    
    // Rotation:
    thx = 90.000000;
    phx = 345.000000;
    thy = 90.000000;
    phy = 75.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix713 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix712 = new TGeoCombiTrans("", dx,dy,dz,pMatrix713);
    
    //Matrici per i  Ladder del  Layer4 - Silicio
        // Combi transformation:
    dx = 0.000000*Scale;
    dy = 6.077000*Scale;
    dz = 0.000000;
    TGeoCombiTrans *pMatrix797 = new TGeoCombiTrans("");
    pMatrix797->SetTranslation(dx,dy,dz);
    // Combi transformation:
    dx = -1.247470*Scale;
    dy = 5.868886*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 12.000000;
    thy = 90.000000;
    phy = 102.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix799 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix798 = new TGeoCombiTrans("", dx,dy,dz,pMatrix799);
    // Combi transformation:
    dx = -2.471739*Scale;
    dy = 5.551616*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 24.000000;
    thy = 90.000000;
    phy = 114.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix801 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix800 = new TGeoCombiTrans("", dx,dy,dz,pMatrix801);
    // Combi transformation:
    dx = -3.526712*Scale;
    dy = 4.854102*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 36.000000;
    thy = 90.000000;
    phy = 126.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix803 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix802 = new TGeoCombiTrans("", dx,dy,dz,pMatrix803);
    // Combi transformation:
    dx = -4.516091*Scale;
    dy = 4.066307*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 48.000000;
    thy = 90.000000;
    phy = 138.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix805 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix804 = new TGeoCombiTrans("", dx,dy,dz,pMatrix805);
    // Combi transformation:
    dx = -5.196153*Scale;
    dy = 3.000000*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 60.000000;
    thy = 90.000000;
    phy = 150.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix807 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix806 = new TGeoCombiTrans("", dx,dy,dz,pMatrix807);
    // Combi transformation:
    dx = -5.779571*Scale;
    dy = 1.877896*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 72.000000;
    thy = 90.000000;
    phy = 162.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix809 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix808 = new TGeoCombiTrans("", dx,dy,dz,pMatrix809);
    // Combi transformation:
    dx = -5.967132*Scale;
    dy = 0.627171*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 84.000000;
    thy = 90.000000;
    phy = 174.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix811 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix810 = new TGeoCombiTrans("", dx,dy,dz,pMatrix811);
    // Combi transformation:
    dx = -6.043710*Scale;
    dy = -0.635220*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 96.000000;
    thy = 90.000000;
    phy = 186.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix813 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix812 = new TGeoCombiTrans("", dx,dy,dz,pMatrix813);
    // Combi transformation:
    dx = -5.706339*Scale;
    dy = -1.854102*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 108.000000;
    thy = 90.000000;
    phy = 198.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix815 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix814 = new TGeoCombiTrans("", dx,dy,dz,pMatrix815);
    // Combi transformation:
    dx = -5.262836*Scale;
    dy = -3.038500*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 120.000000;
    thy = 90.000000;
    phy = 210.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix817 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix816 = new TGeoCombiTrans("", dx,dy,dz,pMatrix817);
    // Combi transformation:
    dx = -4.458869*Scale;
    dy = -4.014784*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 132.000000;
    thy = 90.000000;
    phy = 222.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix819 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix818 = new TGeoCombiTrans("", dx,dy,dz,pMatrix819);
    // Combi transformation:
    dx = -3.571971*Scale;
    dy = -4.916397*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 144.000000;
    thy = 90.000000;
    phy = 234.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix821 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix820 = new TGeoCombiTrans("", dx,dy,dz,pMatrix821);
    // Combi transformation:
    dx = -2.440419*Scale;
    dy = -5.481273*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 156.000000;
    thy = 90.000000;
    phy = 246.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix823 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix822 = new TGeoCombiTrans("", dx,dy,dz,pMatrix823);
    // Combi transformation:
    dx = -1.263479*Scale;
    dy = -5.944203*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 168.000000;
    thy = 90.000000;
    phy = 258.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix825 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix824 = new TGeoCombiTrans("", dx,dy,dz,pMatrix825);
    // Combi transformation:
    dx = 0.000001*Scale;
    dy = -6.000000*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 180.000000;
    thy = 90.000000;
    phy = 270.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix827 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix826 = new TGeoCombiTrans("", dx,dy,dz,pMatrix827);
    // Combi transformation:
    dx = 1.263480*Scale;
    dy = -5.944203*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 192.000000;
    thy = 90.000000;
    phy = 282.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix829 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix828 = new TGeoCombiTrans("", dx,dy,dz,pMatrix829);
    // Combi transformation:
    dx = 2.440420*Scale;
    dy = -5.481273*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 204.000000;
    thy = 90.000000;
    phy = 294.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix831 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix830 = new TGeoCombiTrans("", dx,dy,dz,pMatrix831);
    // Combi transformation:
    dx = 3.571972*Scale;
    dy = -4.916396*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 216.000000;
    thy = 90.000000;
    phy = 306.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix833 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix832 = new TGeoCombiTrans("", dx,dy,dz,pMatrix833);
    // Combi transformation:
    dx = 4.458869*Scale;
    dy = -4.014783*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 228.000000;
    thy = 90.000000;
    phy = 318.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix835 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix834 = new TGeoCombiTrans("", dx,dy,dz,pMatrix835);
    // Combi transformation:
    dx = 5.262837*Scale;
    dy = -3.038499*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 240.000000;
    thy = 90.000000;
    phy = 330.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix837 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix836 = new TGeoCombiTrans("", dx,dy,dz,pMatrix837);
    // Combi transformation:
    dx = 5.706339*Scale;
    dy = -1.854101*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 252.000000;
    thy = 90.000000;
    phy = 342.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix839 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix838 = new TGeoCombiTrans("", dx,dy,dz,pMatrix839);
    // Combi transformation:
    dx = 6.043710*Scale;
    dy = -0.635219*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 264.000000;
    thy = 90.000000;
    phy = 354.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix841 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix840 = new TGeoCombiTrans("", dx,dy,dz,pMatrix841);
    // Combi transformation:
    dx = 5.967131*Scale;
    dy = 0.627172*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 276.000000;
    thy = 90.000000;
    phy = 6.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix843 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix842 = new TGeoCombiTrans("", dx,dy,dz,pMatrix843);
    // Combi transformation:
    dx = 5.779570*Scale;
    dy = 1.877897*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 288.000000;
    thy = 90.000000;
    phy = 18.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix845 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix844 = new TGeoCombiTrans("", dx,dy,dz,pMatrix845);
    // Combi transformation:
    dx = 5.196152*Scale;
    dy = 3.000001*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 300.000000;
    thy = 90.000000;
    phy = 30.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix847 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix846 = new TGeoCombiTrans("", dx,dy,dz,pMatrix847);
    // Combi transformation:
    dx = 4.516090*Scale;
    dy = 4.066308*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 312.000000;
    thy = 90.000000;
    phy = 42.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix849 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix848 = new TGeoCombiTrans("", dx,dy,dz,pMatrix849);
    // Combi transformation:
    dx = 3.526711*Scale;
    dy = 4.854103*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 324.000000;
    thy = 90.000000;
    phy = 54.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix851 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix850 = new TGeoCombiTrans("", dx,dy,dz,pMatrix851);
    // Combi transformation:
    dx = 2.471738*Scale;
    dy = 5.551616*Scale;
    dz = 0.000000;
    fRaylay4Outer=(TMath::Sqrt(pow(dx,2)+pow(dy,2)))+SiThick+CThick;
    // Rotation:
    thx = 90.000000;
    phx = 336.000000;
    thy = 90.000000;
    phy = 66.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix853 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix852 = new TGeoCombiTrans("", dx,dy,dz,pMatrix853);
    // Combi transformation:
    dx = 1.247469*Scale;
    dy = 5.868886*Scale;
    dz = 0.000000;
    fRaylay4Inner=(TMath::Sqrt(pow(dx,2)+pow(dy,2)))+SiThick+CThick;

    // Rotation:
    thx = 90.000000;
    phx = 348.000000;
    thy = 90.000000;
    phy = 78.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix855 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix854 = new TGeoCombiTrans("", dx,dy,dz,pMatrix855);
    
    
    /**/
     
    
//Matrici per il Carbonio del layer0

// Combi transformation:
    /*
    dx = 0.000000*Scale;
    dy = (1.524200*Scale);
    dz = 0.000000;
    */
    
    dx = fRaylay0Outer*(TMath::Sin(0));
    dy = fRaylay0Outer*(TMath::Cos(0));
    dz =  0.00000;
    TGeoCombiTrans *pMatrix439 = new TGeoCombiTrans("");
    pMatrix439->SetTranslation(dx,dy,dz);
    
    
    
    // Combi transformation:
    /*   
    dx = (0.704400*Scale);//0.404379532;
    dy = (1.220057*Scale);//1.739632832;
    dz = 0.000000;
    */
    
    // Rotation:
    thx = 90.000000;
    phx = 330.000000;
    thy = 90.000000;
    phy = 60.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix441 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    dx = fRaylay0Inner*(pMatrix441->GetRotationMatrix()[1]);
    dy = fRaylay0Inner*(pMatrix441->GetRotationMatrix()[0]);
    dz =  0.00000;
    TGeoCombiTrans *pMatrix440 = new TGeoCombiTrans("", dx,dy,dz,pMatrix441);
    
    /*
   
    // Combi transformation:
    dx = (1.319996*Scale);
    dy = (0.762100*Scale);
    dz = 0.000000;
    
    */
    // Rotation:
    thx = 90.000000;
    phx = 300.000000;
    thy = 90.000000;
    phy = 30.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix443 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    dx = fRaylay0Outer*(pMatrix443->GetRotationMatrix()[1]);
    dy = fRaylay0Outer*(pMatrix443->GetRotationMatrix()[0]);
    dz =  0.00000;
    
    TGeoCombiTrans *pMatrix442 = new TGeoCombiTrans("", dx,dy,dz,pMatrix443);
    // Combi transformation:
    /*dx = 1.408800*Scale;
    dy = 0.000000*Scale;
    dz = 0.000000;*/
    // Rotation:
    thx = 90.000000;
    phx = 270.000000;
    thy = 90.000000;
    phy = 0.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix445 = new TGeoRotation("",thx,phx,thy,phy,thz,phz); 
    dx = fRaylay0Inner*(pMatrix445->GetRotationMatrix()[1]);
    dy = fRaylay0Inner*(pMatrix445->GetRotationMatrix()[0]);
    dz =  0.00000;
    TGeoCombiTrans *pMatrix444 = new TGeoCombiTrans("", dx,dy,dz,pMatrix445);
    /*
    // Combi transformation:
    dx = 1.319996*Scale;
    dy = -0.762100*Scale;
    dz = 0.000000;*/
    // Rotation:
    thx = 90.000000;
    phx = 240.000000;
    thy = 90.000000;
    phy = 330.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix447 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    dx = fRaylay0Outer*(pMatrix447->GetRotationMatrix()[1]);
    dy = fRaylay0Outer*(pMatrix447->GetRotationMatrix()[0]);
    dz =  0.00000;
    TGeoCombiTrans *pMatrix446 = new TGeoCombiTrans("", dx,dy,dz,pMatrix447);
    // Combi transformation:
    /*dx = 0.704400*Scale;
    dy = -1.220057*Scale;
    dz = 0.000000;*/
    // Rotation:
    thx = 90.000000;
    phx = 210.000000;
    thy = 90.000000;
    phy = 300.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix449 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    dx = fRaylay0Inner*(pMatrix449->GetRotationMatrix()[1]);
    dy = fRaylay0Inner*(pMatrix449->GetRotationMatrix()[0]);
    dz =  0.00000;
    TGeoCombiTrans *pMatrix448 = new TGeoCombiTrans("", dx,dy,dz,pMatrix449);
    /*// Combi transformation:
    dx = 0.000000*Scale;
    dy = -1.524200*Scale;
    dz = 0.000000;*/
    // Rotation:
    thx = 90.000000;
    phx = 180.000000;
    thy = 90.000000;
    phy = 270.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix451 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    dx = fRaylay0Outer*(pMatrix451->GetRotationMatrix()[1]);
    dy = fRaylay0Outer*(pMatrix451->GetRotationMatrix()[0]);
    dz =  0.00000;
    TGeoCombiTrans *pMatrix450 = new TGeoCombiTrans("", dx,dy,dz,pMatrix451);
    // Combi transformation:
    /*
    dx = -0.704400*Scale;
    dy = -1.220057*Scale;
    dz = 0.000000;
    */
    // Rotation:
    thx = 90.000000;
    phx = 150.000000;
    thy = 90.000000;
    phy = 240.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix453 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    dx = fRaylay0Inner*(pMatrix453->GetRotationMatrix()[1]);
    dy = fRaylay0Inner*(pMatrix453->GetRotationMatrix()[0]);
    dz =  0.00000;
    TGeoCombiTrans *pMatrix452 = new TGeoCombiTrans("", dx,dy,dz,pMatrix453);
    /*// Combi transformation:
    dx = -1.319996*Scale;
    dy = -0.762100*Scale;
    dz = 0.000000;*/
    // Rotation:
    thx = 90.000000;
    phx = 120.000000;
    thy = 90.000000;
    phy = 210.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix455 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    dx = fRaylay0Outer*(pMatrix455->GetRotationMatrix()[1]);
    dy = fRaylay0Outer*(pMatrix455->GetRotationMatrix()[0]);
    dz =  0.00000;
    
    TGeoCombiTrans *pMatrix454 = new TGeoCombiTrans("", dx,dy,dz,pMatrix455);
   /*
    // Combi transformation:
    dx = (-1.408800*Scale);
    dy = (0.000000*Scale);
    dz = 0.000000;
    */
    // Rotation:
    thx = 90.000000;
    phx = 90.000000;
    thy = 90.000000;
    phy = 180.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix457 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    dx = fRaylay0Inner*(pMatrix457->GetRotationMatrix()[1]);
    dy = fRaylay0Inner*(pMatrix457->GetRotationMatrix()[0]);
    dz =  0.00000;
    TGeoCombiTrans *pMatrix456 = new TGeoCombiTrans("", dx,dy,dz,pMatrix457);
    /*// Combi transformation:
    dx = -1.319996*Scale;
    dy = (0.762100*Scale);
    dz = 0.000000;*/
    // Rotation:
    thx = 90.000000;
    phx = 60.000000;
    thy = 90.000000;
    phy = 150.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix459 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    dx = fRaylay0Outer*(pMatrix459->GetRotationMatrix()[1]);
    dy = fRaylay0Outer*(pMatrix459->GetRotationMatrix()[0]);
    dz =  0.00000;
    TGeoCombiTrans *pMatrix458 = new TGeoCombiTrans("", dx,dy,dz,pMatrix459);
    /*
    // Combi transformation:
    dx = -0.704400*Scale;
    dy = (1.220057*Scale);//-(0.0044);
    dz = 0.000000;
    
    */
    // Rotation:
    thx = 90.000000;
    phx = 30.000000;
    thy = 90.000000;
    phy = 120.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix461 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    dx = fRaylay0Inner*(pMatrix461->GetRotationMatrix()[1]);
    dy = fRaylay0Inner*(pMatrix461->GetRotationMatrix()[0]);
    dz =  0.00000;
    TGeoCombiTrans *pMatrix460 = new TGeoCombiTrans("", dx,dy,dz,pMatrix461);
   //End Matrix for Carbon Fiber Support for Ladder of  Layer0
    
    
    // Combi transformation:
    dx = 0.000000;
    dy = 0.000000;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 345.000000;
    thy = 90.000000;
    phy = 75.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix79 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix78 = new TGeoCombiTrans("", dx,dy,dz,pMatrix79);
    
    /*//Matrix for Ladder of  Layer1
    // Combi transformation:  */
    /*TGeoCombiTrans *pMatrix495 = new TGeoCombiTrans("", dx,dy,dz,pMatrix496);*/
   
    //End Matrix for Ladder of  Layer1
   //Matrix for Carbon Fiber Support for Ladder of  Layer1
    /*
    // Combi transformation:
    dx = 0.000000;
    dy = 2.322000*Scale;
    dz = 0.000000;
    */
    TGeoCombiTrans *pMatrix497 = new TGeoCombiTrans("");
    dx = fRaylay1Outer*(TMath::Sin(0));
    dy = fRaylay1Outer*(TMath::Cos(0));
    dz =  0.00000;
    pMatrix497->SetTranslation(dx,dy,dz);
	/*
    // Combi transformation:
    dx = 1.104400*Scale;
    dy = 1.912877*Scale;
    dz = 0.000000;
  	*/
    // Rotation:
    thx = 90.000000;
    phx = 330.000000;
    thy = 90.000000;
    phy = 60.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix499 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    dx = fRaylay1Inner*(pMatrix499->GetRotationMatrix()[1]);
    dy = fRaylay1Inner*(pMatrix499->GetRotationMatrix()[0]);
    dz =  0.00000;
    
    TGeoCombiTrans *pMatrix498 = new TGeoCombiTrans("", dx,dy,dz,pMatrix499);
    /*
    // Combi transformation:
    dx = 2.010911*Scale;
    dy = 1.161000*Scale;
    dz = 0.000000;
    */
    // Rotation:
    thx = 90.000000;
    phx = 300.000000;
    thy = 90.000000;
    phy = 30.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix501 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    dx = fRaylay1Outer*(pMatrix501->GetRotationMatrix()[1]);
    dy = fRaylay1Outer*(pMatrix501->GetRotationMatrix()[0]);
    dz =  0.00000;
    
    TGeoCombiTrans *pMatrix500 = new TGeoCombiTrans("", dx,dy,dz,pMatrix501);
    /*
    // Combi transformation:
    dx = 2.208800*Scale;
    dy = 0.000000;
    dz = 0.000000;
    */
    // Rotation:
    thx = 90.000000;
    phx = 270.000000;
    thy = 90.000000;
    phy = 0.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix503 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    dx = fRaylay1Inner*(pMatrix503->GetRotationMatrix()[1]);
    dy = fRaylay1Inner*(pMatrix503->GetRotationMatrix()[0]);
    dz =  0.00000;
    TGeoCombiTrans *pMatrix502 = new TGeoCombiTrans("", dx,dy,dz,pMatrix503);
    // Combi transformation:
    /*
    dx = 2.010911*Scale;
    dy = -1.161000*Scale;
    dz = 0.000000;
    */
    // Rotation:
    thx = 90.000000;
    phx = 240.000000;
    thy = 90.000000;
    phy = 330.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix505 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    dx = fRaylay1Outer*(pMatrix505->GetRotationMatrix()[1]);
    dy = fRaylay1Outer*(pMatrix505->GetRotationMatrix()[0]);
    dz =  0.00000;

    TGeoCombiTrans *pMatrix504 = new TGeoCombiTrans("", dx,dy,dz,pMatrix505);
    /*
    // Combi transformation:
    dx = 1.104400*Scale;
    dy = -1.912877*Scale;
    dz = 0.000000;
    */
    // Rotation:
    thx = 90.000000;
    phx = 210.000000;
    thy = 90.000000;
    phy = 300.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix507 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    dx = fRaylay1Inner*(pMatrix507->GetRotationMatrix()[1]);
    dy = fRaylay1Inner*(pMatrix507->GetRotationMatrix()[0]);
    dz =  0.00000;
    TGeoCombiTrans *pMatrix506 = new TGeoCombiTrans("", dx,dy,dz,pMatrix507);
    /*
    // Combi transformation:
    dx = 0.000000;
    dy = -2.322000*Scale;
    dz = 0.000000;
    */
    // Rotation:
    thx = 90.000000;
    phx = 180.000000;
    thy = 90.000000;
    phy = 270.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix509 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    dx = fRaylay1Outer*(pMatrix509->GetRotationMatrix()[1]);
    dy = fRaylay1Outer*(pMatrix509->GetRotationMatrix()[0]);
    dz =  0.00000;

    TGeoCombiTrans *pMatrix508 = new TGeoCombiTrans("", dx,dy,dz,pMatrix509);
    /*
    // Combi transformation:
    dx = -1.104400*Scale;
    dy = -1.912877*Scale;
    dz = 0.000000;
    */
    // Rotation:
    thx = 90.000000;
    phx = 150.000000;
    thy = 90.000000;
    phy = 240.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix511 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    dx = fRaylay1Inner*(pMatrix511->GetRotationMatrix()[1]);
    dy = fRaylay1Inner*(pMatrix511->GetRotationMatrix()[0]);
    dz =  0.00000;
    TGeoCombiTrans *pMatrix510 = new TGeoCombiTrans("", dx,dy,dz,pMatrix511);
    /*
    // Combi transformation:
    dx = -2.010911*Scale;
    dy = -1.161000*Scale;
    dz = 0.000000;
    */
    // Rotation:
    thx = 90.000000;
    phx = 120.000000;
    thy = 90.000000;
    phy = 210.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix513 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    dx = fRaylay1Outer*(pMatrix513->GetRotationMatrix()[1]);
    dy = fRaylay1Outer*(pMatrix513->GetRotationMatrix()[0]);
    dz =  0.00000;

    TGeoCombiTrans *pMatrix512 = new TGeoCombiTrans("", dx,dy,dz,pMatrix513);
    /*// Combi transformation:
    dx = -2.208800*Scale;
    dy = 0.000000;
    dz = 0.000000;
    */
    // Rotation:
    thx = 90.000000;
    phx = 90.000000;
    thy = 90.000000;
    phy = 180.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix515 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    dx = fRaylay1Inner*(pMatrix515->GetRotationMatrix()[1]);
    dy = fRaylay1Inner*(pMatrix515->GetRotationMatrix()[0]);
    dz =  0.00000;
    TGeoCombiTrans *pMatrix514 = new TGeoCombiTrans("", dx,dy,dz,pMatrix515);
    /*
    // Combi transformation:
    dx = -2.010911*Scale;
    dy = 1.161000*Scale;
    dz = 0.000000;
    */
    // Rotation:
    thx = 90.000000;
    phx = 60.000000;
    thy = 90.000000;
    phy = 150.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix517 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    dx = fRaylay1Outer*(pMatrix517->GetRotationMatrix()[1]);
    dy = fRaylay1Outer*(pMatrix517->GetRotationMatrix()[0]);
    dz =  0.00000;

    TGeoCombiTrans *pMatrix516 = new TGeoCombiTrans("", dx,dy,dz,pMatrix517);
    /*
    // Combi transformation:
    dx = -1.104400*Scale;
    dy = 1.912877*Scale;
    dz = 0.000000;
    */
    // Rotation:
    thx = 90.000000;
    phx = 30.000000;
    thy = 90.000000;
    phy = 120.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix519 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    dx = fRaylay1Inner*(pMatrix519->GetRotationMatrix()[1]);
    dy = fRaylay1Inner*(pMatrix519->GetRotationMatrix()[0]);
    dz =  0.00000;
    TGeoCombiTrans *pMatrix518 = new TGeoCombiTrans("", dx,dy,dz,pMatrix519);
     //End Matrix for Carbon Fiber Support for Ladder of  Layer1
    
    // Combi transformation:
    dx = 0.000000;
    dy = 0.000000;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 350.000000;
    thy = 90.000000;
    phy = 80.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix81 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix80 = new TGeoCombiTrans("", dx,dy,dz,pMatrix81);
    
    
    
    
    
    
    
    
    
    // Combi transformation:
    /*
    dx = 0.000000*Scale;
    dy = 3.597800*Scale;
    dz = 0.000000;
    */
    dx = fRaylay2Outer*(TMath::Sin(0));
    dy = fRaylay2Outer*(TMath::Cos(0));
    dz =  0.00000;
    TGeoCombiTrans *pMatrix532 = new TGeoCombiTrans("");
    pMatrix532->SetTranslation(dx,dy,dz);
    // Combi transformation:
    /*
    dx = 1.200080*Scale;
    dy = 3.297194*Scale;
    dz = 0.000000;
    */
    // Rotation:
    thx = 90.000000;
    phx = 340.000000;
    thy = 90.000000;
    phy = 70.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix534 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    dx = fRaylay2Inner*(pMatrix534->GetRotationMatrix()[1]);
    dy = fRaylay2Inner*(pMatrix534->GetRotationMatrix()[0]);
    dz =  0.00000;

    TGeoCombiTrans *pMatrix533 = new TGeoCombiTrans("", dx,dy,dz,pMatrix534);
    /*
    // Combi transformation:
    dx = 2.312621*Scale;
    dy = 2.756075*Scale;
    dz = 0.000000;
    */
    // Rotation:
    thx = 90.000000;
    phx = 320.000000;
    thy = 90.000000;
    phy = 50.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix536 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    dx = fRaylay2Outer*(pMatrix536->GetRotationMatrix()[1]);
    dy = fRaylay2Outer*(pMatrix536->GetRotationMatrix()[0]);
    dz =  0.00000;
    TGeoCombiTrans *pMatrix535 = new TGeoCombiTrans("", dx,dy,dz,pMatrix536);
    // Combi transformation:
    /*
    dx = 3.038710*Scale;
    dy = 1.754400*Scale;
    dz = 0.000000;
    */
    // Rotation:
    thx = 90.000000;
    phx = 300.000000;
    thy = 90.000000;
    phy = 30.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix538 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    dx = fRaylay2Inner*(pMatrix538->GetRotationMatrix()[1]);
    dy = fRaylay2Inner*(pMatrix538->GetRotationMatrix()[0]);
    dz =  0.00000;

    TGeoCombiTrans *pMatrix537 = new TGeoCombiTrans("", dx,dy,dz,pMatrix538);
    /*
    // Combi transformation:
    dx = 3.543141*Scale;
    dy = 0.624752*Scale;
    dz = 0.000000;
    */
    // Rotation:
    thx = 90.000000;
    phx = 280.000000;
    thy = 90.000000;
    phy = 10.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix540 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    dx = fRaylay2Outer*(pMatrix540->GetRotationMatrix()[1]);
    dy = fRaylay2Outer*(pMatrix540->GetRotationMatrix()[0]);
    dz =  0.00000;
    TGeoCombiTrans *pMatrix539 = new TGeoCombiTrans("", dx,dy,dz,pMatrix540);
    // Combi transformation:
    /*
    dx = 3.455493*Scale;
    dy = -0.609297*Scale;
    dz = 0.000000;
    */
    // Rotation:
    thx = 90.000000;
    phx = 260.000000;
    thy = 90.000000;
    phy = 350.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix542 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    dx = fRaylay2Inner*(pMatrix542->GetRotationMatrix()[1]);
    dy = fRaylay2Inner*(pMatrix542->GetRotationMatrix()[0]);
    dz =  0.00000;

    TGeoCombiTrans *pMatrix541 = new TGeoCombiTrans("", dx,dy,dz,pMatrix542);
    /*
    // Combi transformation:
    dx = 3.115786*Scale;
    dy = -1.798900*Scale;
    dz = 0.000000;
    */
    // Rotation:
    thx = 90.000000;
    phx = 240.000000;
    thy = 90.000000;
    phy = 330.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix544 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    dx = fRaylay2Outer*(pMatrix544->GetRotationMatrix()[1]);
    dy = fRaylay2Outer*(pMatrix544->GetRotationMatrix()[0]);
    dz =  0.00000;
    TGeoCombiTrans *pMatrix543 = new TGeoCombiTrans("", dx,dy,dz,pMatrix544);
    // Combi transformation:
    /*
    dx = 2.255413*Scale;
    dy = -2.687896*Scale;
    dz = 0.000000;
    */
    // Rotation:
    thx = 90.000000;
    phx = 220.000000;
    thy = 90.000000;
    phy = 310.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix546 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    dx = fRaylay2Inner*(pMatrix546->GetRotationMatrix()[1]);
    dy = fRaylay2Inner*(pMatrix546->GetRotationMatrix()[0]);
    dz =  0.00000;

    TGeoCombiTrans *pMatrix545 = new TGeoCombiTrans("", dx,dy,dz,pMatrix546);
    // Combi transformation:
    /*
    dx = 1.230520*Scale;
    dy = -3.380826*Scale;
    dz = 0.000000;
    */
    // Rotation:
    thx = 90.000000;
    phx = 200.000000;
    thy = 90.000000;
    phy = 290.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix548 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    dx = fRaylay2Outer*(pMatrix548->GetRotationMatrix()[1]);
    dy = fRaylay2Outer*(pMatrix548->GetRotationMatrix()[0]);
    dz =  0.00000;
    TGeoCombiTrans *pMatrix547 = new TGeoCombiTrans("", dx,dy,dz,pMatrix548);
    // Combi transformation:
    /*
    dx = 0.000000*Scale;
    dy = -3.508800*Scale;
    dz = 0.000000;
    */
    // Rotation:
    thx = 90.000000;
    phx = 180.000000;
    thy = 90.000000;
    phy = 270.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix550 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    dx = fRaylay2Inner*(pMatrix550->GetRotationMatrix()[1]);
    dy = fRaylay2Inner*(pMatrix550->GetRotationMatrix()[0]);
    dz =  0.00000;

    TGeoCombiTrans *pMatrix549 = new TGeoCombiTrans("", dx,dy,dz,pMatrix550);
    // Combi transformation:
    /*
    dx = -1.230520*Scale;
    dy = -3.380826*Scale;
    dz = 0.000000;
    */
    // Rotation:
    thx = 90.000000;
    phx = 160.000000;
    thy = 90.000000;
    phy = 250.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix552 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    dx = fRaylay2Outer*(pMatrix552->GetRotationMatrix()[1]);
    dy = fRaylay2Outer*(pMatrix552->GetRotationMatrix()[0]);
    dz =  0.00000;
    TGeoCombiTrans *pMatrix551 = new TGeoCombiTrans("", dx,dy,dz,pMatrix552);
    // Combi transformation:
    /*
    dx = -2.255413*Scale;
    dy = -2.687897*Scale;
    dz = 0.000000;
    */
    // Rotation:
    thx = 90.000000;
    phx = 140.000000;
    thy = 90.000000;
    phy = 230.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix554 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    dx = fRaylay2Inner*(pMatrix554->GetRotationMatrix()[1]);
    dy = fRaylay2Inner*(pMatrix554->GetRotationMatrix()[0]);
    dz =  0.00000;

    TGeoCombiTrans *pMatrix553 = new TGeoCombiTrans("", dx,dy,dz,pMatrix554);
    // Combi transformation:
    /*
    dx = -3.115786*Scale;
    dy = -1.798900*Scale;
    dz = 0.000000;
    */
    // Rotation:
    thx = 90.000000;
    phx = 120.000000;
    thy = 90.000000;
    phy = 210.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix556 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    dx = fRaylay2Outer*(pMatrix556->GetRotationMatrix()[1]);
    dy = fRaylay2Outer*(pMatrix556->GetRotationMatrix()[0]);
    dz =  0.00000;
    TGeoCombiTrans *pMatrix555 = new TGeoCombiTrans("", dx,dy,dz,pMatrix556);
    // Combi transformation:
    /*
    dx = -3.455493*Scale;
    dy = -0.609297*Scale;
    dz = 0.000000;
    */
    // Rotation:
    thx = 90.000000;
    phx = 100.000000;
    thy = 90.000000;
    phy = 190.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix558 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    dx = fRaylay2Inner*(pMatrix558->GetRotationMatrix()[1]);
    dy = fRaylay2Inner*(pMatrix558->GetRotationMatrix()[0]);
    dz =  0.00000;

    TGeoCombiTrans *pMatrix557 = new TGeoCombiTrans("", dx,dy,dz,pMatrix558);
    // Combi transformation:
    /*
    dx = -3.543141*Scale;
    dy = 0.624751*Scale;
    dz = 0.000000;
    */
    // Rotation:
    thx = 90.000000;
    phx = 80.000000;
    thy = 90.000000;
    phy = 170.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix560 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    dx = fRaylay2Outer*(pMatrix560->GetRotationMatrix()[1]);
    dy = fRaylay2Outer*(pMatrix560->GetRotationMatrix()[0]);
    dz =  0.00000;
    TGeoCombiTrans *pMatrix559 = new TGeoCombiTrans("", dx,dy,dz,pMatrix560);
    // Combi transformation:
    /*
    dx = -3.038710*Scale;
    dy = 1.754400*Scale;
    dz = 0.000000;
    */
    // Rotation:
    thx = 90.000000;
    phx = 60.000000;
    thy = 90.000000;
    phy = 150.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix562 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    dx = fRaylay2Inner*(pMatrix562->GetRotationMatrix()[1]);
    dy = fRaylay2Inner*(pMatrix562->GetRotationMatrix()[0]);
    dz =  0.00000;

    TGeoCombiTrans *pMatrix561 = new TGeoCombiTrans("", dx,dy,dz,pMatrix562);
    // Combi transformation:
    /*
    dx = -2.312622*Scale;
    dy = 2.756074*Scale;
    dz = 0.000000;
    */
    // Rotation:
    thx = 90.000000;
    phx = 40.000000;
    thy = 90.000000;
    phy = 130.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix564 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    dx = fRaylay2Outer*(pMatrix564->GetRotationMatrix()[1]);
    dy = fRaylay2Outer*(pMatrix564->GetRotationMatrix()[0]);
    dz =  0.00000;
    TGeoCombiTrans *pMatrix563 = new TGeoCombiTrans("", dx,dy,dz,pMatrix564);
    // Combi transformation:
    /*
    dx = -1.200081*Scale;
    dy = 3.297193*Scale;
    dz = 0.000000;
    */
    // Rotation:
    thx = 90.000000;
    phx = 20.000000;
    thy = 90.000000;
    phy = 110.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix566 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    dx = fRaylay2Inner*(pMatrix566->GetRotationMatrix()[1]);
    dy = fRaylay2Inner*(pMatrix566->GetRotationMatrix()[0]);
    dz =  0.00000;
    TGeoCombiTrans *pMatrix565 = new TGeoCombiTrans("", dx,dy,dz,pMatrix566);
    /*// Combi transformation:
    dx = 0.000000*Scale;
    dy = 3.589000*Scale;
    dz = 0.000000;
    TGeoCombiTrans *pMatrix567 = new TGeoCombiTrans("");*/
  
    
    /* TGeoCombiTrans *pMatrix600 = new TGeoCombiTrans("", dx,dy,dz,pMatrix601);*/
    // Combi transformation:
    dx = 0.000000*Scale;
    dy = 0.000000*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 352.000000;
    thy = 90.000000;
    phy = 82.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix83 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix82 = new TGeoCombiTrans("", dx,dy,dz,pMatrix83);
    //Matrix for Layer3-Barrel
    // Combi transformation:
    /*
    dx = 0.000000*Scale;
    dy = 4.849700*Scale;
    dz = 0.000000;
    */
    dx = fRaylay3Outer*(TMath::Sin(0));
    dy = fRaylay3Outer*(TMath::Cos(0));
    dz =  0.00000;
    TGeoCombiTrans *pMatrix620 = new TGeoCombiTrans("");
    pMatrix620->SetTranslation(dx,dy,dz);
    // Combi transformation:
    /*
    dx = 1.234256*Scale;
    dy = 4.606307*Scale;
    dz = 0.000000;
    */
    // Rotation:
    thx = 90.000000;
    phx = 345.000000;
    thy = 90.000000;
    phy = 75.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix622 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    dx = fRaylay3Inner*(pMatrix622->GetRotationMatrix()[1]);
    dy = fRaylay3Inner*(pMatrix622->GetRotationMatrix()[0]);
    dz =  0.00000;
    
    TGeoCombiTrans *pMatrix621 = new TGeoCombiTrans("", dx,dy,dz,pMatrix622);
    // Combi transformation:
    /*
    dx = 2.424850*Scale;
    dy = 4.199964*Scale;
    dz = 0.000000;
    */
    // Rotation:
    thx = 90.000000;
    phx = 330.000000;
    thy = 90.000000;
    phy = 60.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix624 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    dx = fRaylay3Outer*(pMatrix624->GetRotationMatrix()[1]);
    dy = fRaylay3Outer*(pMatrix624->GetRotationMatrix()[0]);
    dz =  0.00000;
    TGeoCombiTrans *pMatrix623 = new TGeoCombiTrans("", dx,dy,dz,pMatrix624);
    // Combi transformation:
    /*
    dx = 3.372051*Scale;
    dy = 3.372051*Scale;
    dz = 0.000000;
    */
    // Rotation:
    thx = 90.000000;
    phx = 315.000000;
    thy = 90.000000;
    phy = 45.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix626 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    dx = fRaylay3Inner*(pMatrix626->GetRotationMatrix()[1]);
    dy = fRaylay3Inner*(pMatrix626->GetRotationMatrix()[0]);
    dz =  0.00000;
    TGeoCombiTrans *pMatrix625 = new TGeoCombiTrans("", dx,dy,dz,pMatrix626);
    // Combi transformation:
    /*
    dx = 4.199964*Scale;
    dy = 2.424850*Scale;
    dz = 0.000000;
    */
    // Rotation:
    thx = 90.000000;
    phx = 300.000000;
    thy = 90.000000;
    phy = 30.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix628 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    dx = fRaylay3Outer*(pMatrix628->GetRotationMatrix()[1]);
    dy = fRaylay3Outer*(pMatrix628->GetRotationMatrix()[0]);
    dz =  0.00000;
    TGeoCombiTrans *pMatrix627 = new TGeoCombiTrans("", dx,dy,dz,pMatrix628);
    // Combi transformation:
    /*
    dx = 4.606307*Scale;
    dy = 1.234256*Scale;
    dz = 0.000000;
    */
    // Rotation:
    thx = 90.000000;
    phx = 285.000000;
    thy = 90.000000;
    phy = 15.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix630 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    dx = fRaylay3Inner*(pMatrix630->GetRotationMatrix()[1]);
    dy = fRaylay3Inner*(pMatrix630->GetRotationMatrix()[0]);
    dz =  0.00000;
    TGeoCombiTrans *pMatrix629 = new TGeoCombiTrans("", dx,dy,dz,pMatrix630);
    // Combi transformation:
    /*
    dx = 4.849700*Scale;
    dy = 0.000000*Scale;
    dz = 0.000000;
    */
    // Rotation:
    thx = 90.000000;
    phx = 270.000000;
    thy = 90.000000;
    phy = 0.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix632 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    dx = fRaylay3Outer*(pMatrix632->GetRotationMatrix()[1]);
    dy = fRaylay3Outer*(pMatrix632->GetRotationMatrix()[0]);
    dz =  0.00000;
    TGeoCombiTrans *pMatrix631 = new TGeoCombiTrans("", dx,dy,dz,pMatrix632);
    // Combi transformation:
    /*
    dx = 4.606307*Scale;
    dy = -1.234256*Scale;
    dz = 0.000000;
    */
    // Rotation:
    thx = 90.000000;
    phx = 255.000000;
    thy = 90.000000;
    phy = 345.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix634 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    dx = fRaylay3Inner*(pMatrix634->GetRotationMatrix()[1]);
    dy = fRaylay3Inner*(pMatrix634->GetRotationMatrix()[0]);
    dz =  0.00000;
    TGeoCombiTrans *pMatrix633 = new TGeoCombiTrans("", dx,dy,dz,pMatrix634);
    // Combi transformation:
    /*
    dx = 4.199963*Scale;
    dy = -2.424850*Scale;
    dz = 0.000000;
    */
    // Rotation:
    thx = 90.000000;
    phx = 240.000000;
    thy = 90.000000;
    phy = 330.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix636 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    dx = fRaylay3Outer*(pMatrix636->GetRotationMatrix()[1]);
    dy = fRaylay3Outer*(pMatrix636->GetRotationMatrix()[0]);
    dz =  0.00000;
    TGeoCombiTrans *pMatrix635 = new TGeoCombiTrans("", dx,dy,dz,pMatrix636);
    // Combi transformation:
    /*
    dx = 3.372051*Scale;
    dy = -3.372051*Scale;
    dz = 0.000000;
    */
    // Rotation:
    thx = 90.000000;
    phx = 225.000000;
    thy = 90.000000;
    phy = 315.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix638 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    dx = fRaylay3Inner*(pMatrix638->GetRotationMatrix()[1]);
    dy = fRaylay3Inner*(pMatrix638->GetRotationMatrix()[0]);
    dz =  0.00000;
    TGeoCombiTrans *pMatrix637 = new TGeoCombiTrans("", dx,dy,dz,pMatrix638);
    // Combi transformation:
    /*
    dx = 2.424850*Scale;
    dy = -4.199964*Scale;
    dz = 0.000000;
    */
    // Rotation:
    thx = 90.000000;
    phx = 210.000000;
    thy = 90.000000;
    phy = 300.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix640 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    dx = fRaylay3Outer*(pMatrix640->GetRotationMatrix()[1]);
    dy = fRaylay3Outer*(pMatrix640->GetRotationMatrix()[0]);
    dz =  0.00000;
    TGeoCombiTrans *pMatrix639 = new TGeoCombiTrans("", dx,dy,dz,pMatrix640);
    // Combi transformation:
    /*
    dx = 1.234256*Scale;
    dy = -4.606307*Scale;
    dz = 0.000000;
    */
    // Rotation:
    thx = 90.000000;
    phx = 195.000000;
    thy = 90.000000;
    phy = 285.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix642 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    dx = fRaylay3Inner*(pMatrix642->GetRotationMatrix()[1]);
    dy = fRaylay3Inner*(pMatrix642->GetRotationMatrix()[0]);
    dz =  0.00000;
    TGeoCombiTrans *pMatrix641 = new TGeoCombiTrans("", dx,dy,dz,pMatrix642);
    // Combi transformation:
    /*
    dx = 0.000000*Scale;
    dy = -4.849700*Scale;
    dz = 0.000000;
    */
    // Rotation:
    thx = 90.000000;
    phx = 180.000000;
    thy = 90.000000;
    phy = 270.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix644 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    dx = fRaylay3Outer*(pMatrix644->GetRotationMatrix()[1]);
    dy = fRaylay3Outer*(pMatrix644->GetRotationMatrix()[0]);
    dz =  0.00000;
    TGeoCombiTrans *pMatrix643 = new TGeoCombiTrans("", dx,dy,dz,pMatrix644);
    // Combi transformation:
    /*
    dx = -1.234257*Scale;
    dy = -4.606307*Scale;
    dz = 0.000000;
    */
    // Rotation:
    thx = 90.000000;
    phx = 165.000000;
    thy = 90.000000;
    phy = 255.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix646 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    dx = fRaylay3Inner*(pMatrix646->GetRotationMatrix()[1]);
    dy = fRaylay3Inner*(pMatrix646->GetRotationMatrix()[0]);
    dz =  0.00000;
    TGeoCombiTrans *pMatrix645 = new TGeoCombiTrans("", dx,dy,dz,pMatrix646);
    // Combi transformation:
    /*
    dx = -2.424850*Scale;
    dy = -4.199963*Scale;
    dz = 0.000000;
    */
    // Rotation:
    thx = 90.000000;
    phx = 150.000000;
    thy = 90.000000;
    phy = 240.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix648 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    dx = fRaylay3Outer*(pMatrix648->GetRotationMatrix()[1]);
    dy = fRaylay3Outer*(pMatrix648->GetRotationMatrix()[0]);
    dz =  0.00000;
    TGeoCombiTrans *pMatrix647 = new TGeoCombiTrans("", dx,dy,dz,pMatrix648);
    // Combi transformation:
    /*
    dx = -3.372051*Scale;
    dy = -3.372050*Scale;
    dz = 0.000000;
    */
    // Rotation:
    thx = 90.000000;
    phx = 135.000000;
    thy = 90.000000;
    phy = 225.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix650 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    dx = fRaylay3Inner*(pMatrix650->GetRotationMatrix()[1]);
    dy = fRaylay3Inner*(pMatrix650->GetRotationMatrix()[0]);
    dz =  0.00000;
    TGeoCombiTrans *pMatrix649 = new TGeoCombiTrans("", dx,dy,dz,pMatrix650);
    // Combi transformation:
    /*
    dx = -4.199964*Scale;
    dy = -2.424850*Scale;
    dz = 0.000000;
    */
    // Rotation:
    thx = 90.000000;
    phx = 120.000000;
    thy = 90.000000;
    phy = 210.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix652 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    dx = fRaylay3Outer*(pMatrix652->GetRotationMatrix()[1]);
    dy = fRaylay3Outer*(pMatrix652->GetRotationMatrix()[0]);
    dz =  0.00000;
    TGeoCombiTrans *pMatrix651 = new TGeoCombiTrans("", dx,dy,dz,pMatrix652);
    // Combi transformation:
    /*
    dx = -4.606307*Scale;
    dy = -1.234256*Scale;
    dz = 0.000000;
    */
    // Rotation:
    thx = 90.000000;
    phx = 105.000000;
    thy = 90.000000;
    phy = 195.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix654 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    dx = fRaylay3Inner*(pMatrix654->GetRotationMatrix()[1]);
    dy = fRaylay3Inner*(pMatrix654->GetRotationMatrix()[0]);
    dz =  0.00000;
    TGeoCombiTrans *pMatrix653 = new TGeoCombiTrans("", dx,dy,dz,pMatrix654);
    // Combi transformation:
    /*
    dx = -4.849700*Scale;
    dy = 0.000001*Scale;
    dz = 0.000000;
    */
    // Rotation:
    thx = 90.000000;
    phx = 90.000000;
    thy = 90.000000;
    phy = 180.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix656 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    dx = fRaylay3Outer*(pMatrix656->GetRotationMatrix()[1]);
    dy = fRaylay3Outer*(pMatrix656->GetRotationMatrix()[0]);
    dz =  0.00000;
    TGeoCombiTrans *pMatrix655 = new TGeoCombiTrans("", dx,dy,dz,pMatrix656);
    // Combi transformation:
    /*
    dx = -4.606307*Scale;
    dy = 1.234257*Scale;
    dz = 0.000000;
    */
    // Rotation:
    thx = 90.000000;
    phx = 75.000000;
    thy = 90.000000;
    phy = 165.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix658 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    dx = fRaylay3Inner*(pMatrix658->GetRotationMatrix()[1]);
    dy = fRaylay3Inner*(pMatrix658->GetRotationMatrix()[0]);
    dz =  0.00000;
    TGeoCombiTrans *pMatrix657 = new TGeoCombiTrans("", dx,dy,dz,pMatrix658);
    // Combi transformation:
    /*
    dx = -4.199963*Scale;
    dy = 2.424851*Scale;
    dz = 0.000000;
    */
    // Rotation:
    thx = 90.000000;
    phx = 60.000000;
    thy = 90.000000;
    phy = 150.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix660 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    dx = fRaylay3Outer*(pMatrix660->GetRotationMatrix()[1]);
    dy = fRaylay3Outer*(pMatrix660->GetRotationMatrix()[0]);
    dz =  0.00000;
    TGeoCombiTrans *pMatrix659 = new TGeoCombiTrans("", dx,dy,dz,pMatrix660);
    // Combi transformation:
    /*
    dx = -3.372050*Scale;
    dy = 3.372051*Scale;
    dz = 0.000000;
    */
    // Rotation:
    thx = 90.000000;
    phx = 45.000000;
    thy = 90.000000;
    phy = 135.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix662 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    dx = fRaylay3Inner*(pMatrix662->GetRotationMatrix()[1]);
    dy = fRaylay3Inner*(pMatrix662->GetRotationMatrix()[0]);
    dz =  0.00000;
    TGeoCombiTrans *pMatrix661 = new TGeoCombiTrans("", dx,dy,dz,pMatrix662);
    // Combi transformation:
    /*
    dx = -2.424849*Scale;
    dy = 4.199964*Scale;
    dz = 0.000000;
    */
    // Rotation:
    thx = 90.000000;
    phx = 30.000000;
    thy = 90.000000;
    phy = 120.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix664 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    dx = fRaylay3Outer*(pMatrix664->GetRotationMatrix()[1]);
    dy = fRaylay3Outer*(pMatrix664->GetRotationMatrix()[0]);
    dz =  0.00000;
    TGeoCombiTrans *pMatrix663 = new TGeoCombiTrans("", dx,dy,dz,pMatrix664);
    // Combi transformation:
    dx = -1.234255*Scale;
    dy = 4.606307*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 15.000000;
    thy = 90.000000;
    phy = 105.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix666 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    dx = fRaylay3Inner*(pMatrix666->GetRotationMatrix()[1]);
    dy = fRaylay3Inner*(pMatrix666->GetRotationMatrix()[0]);
    dz =  0.00000;
    TGeoCombiTrans *pMatrix665 = new TGeoCombiTrans("", dx,dy,dz,pMatrix666);
    /*//Matrix for Layer4-Barrel
    // Combi transformation:
    dx = 0.000000*Scale;
    dy = 4.840900*Scale;
    dz = 0.000000;
    TGeoCombiTrans *pMatrix667 = new TGeoCombiTrans("");
    pMatrix667->SetTranslation(dx,dy,dz);
*/
    

    /*TGeoCombiTrans *pMatrix712 = new TGeoCombiTrans("", dx,dy,dz,pMatrix713);*/
    // Combi transformation:
    dx = 0.000000*Scale;
    dy = 0.000000*Scale;
    dz = 0.000000;
    // Rotation:
    thx = 90.000000;
    phx = 354.000000;
    thy = 90.000000;
    phy = 84.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix85 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix84 = new TGeoCombiTrans("", dx,dy,dz,pMatrix85);
    // Combi transformation:
    /*
    dx = 0.000000*Scale;
    dy = 6.085800*Scale;
    dz = 0.000000;
    */
    dx = fRaylay4Outer*(TMath::Sin(0));
    dy = fRaylay4Outer*(TMath::Cos(0));
    dz =  0.00000;
    
    TGeoCombiTrans *pMatrix738 = new TGeoCombiTrans("");
    pMatrix738->SetTranslation(dx,dy,dz);
    // Combi transformation:
    /*
    dx = 1.249300*Scale;
    dy = 5.877493*Scale;
    dz = 0.000000;
    */
    // Rotation:
    thx = 90.000000;
    phx = 348.000000;
    thy = 90.000000;
    phy = 78.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix740 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    dx = fRaylay4Inner*(pMatrix740->GetRotationMatrix()[1]);
    dy = fRaylay4Inner*(pMatrix740->GetRotationMatrix()[0]);
    dz =  0.00000;
    
    TGeoCombiTrans *pMatrix739 = new TGeoCombiTrans("", dx,dy,dz,pMatrix740);
    // Combi transformation:
    /*
    dx = 2.475318*Scale;
    dy = 5.559655*Scale;
    dz = 0.000000;
    */
    // Rotation:
    thx = 90.000000;
    phx = 336.000000;
    thy = 90.000000;
    phy = 66.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix742 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    dx = fRaylay4Outer*(pMatrix742->GetRotationMatrix()[1]);
    dy = fRaylay4Outer*(pMatrix742->GetRotationMatrix()[0]);
    dz =  0.00000;
    TGeoCombiTrans *pMatrix741 = new TGeoCombiTrans("", dx,dy,dz,pMatrix742);
    // Combi transformation:
    /*
    dx = 3.531884*Scale;
    dy = 4.861221*Scale;
    dz = 0.000000;
    */
    // Rotation:
    thx = 90.000000;
    phx = 324.000000;
    thy = 90.000000;
    phy = 54.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix744 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    dx = fRaylay4Inner*(pMatrix744->GetRotationMatrix()[1]);
    dy = fRaylay4Inner*(pMatrix744->GetRotationMatrix()[0]);
    dz =  0.00000;
    TGeoCombiTrans *pMatrix743 = new TGeoCombiTrans("", dx,dy,dz,pMatrix744);
    // Combi transformation:
    /*
    dx = 4.522631*Scale;
    dy = 4.072195*Scale;
    dz = 0.000000;
    */
    // Rotation:
    thx = 90.000000;
    phx = 312.000000;
    thy = 90.000000;
    phy = 42.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix746 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    dx = fRaylay4Outer*(pMatrix746->GetRotationMatrix()[1]);
    dy = fRaylay4Outer*(pMatrix746->GetRotationMatrix()[0]);
    dz =  0.00000;
    TGeoCombiTrans *pMatrix745 = new TGeoCombiTrans("", dx,dy,dz,pMatrix746);
    // Combi transformation:
    /*
    dx = 5.203773*Scale;
    dy = 3.004400*Scale;
    dz = 0.000000;
    */
    // Rotation:
    thx = 90.000000;
    phx = 300.000000;
    thy = 90.000000;
    phy = 30.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix748 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    dx = fRaylay4Inner*(pMatrix748->GetRotationMatrix()[1]);
    dy = fRaylay4Inner*(pMatrix748->GetRotationMatrix()[0]);
    dz =  0.00000;
    TGeoCombiTrans *pMatrix747 = new TGeoCombiTrans("", dx,dy,dz,pMatrix748);
    // Combi transformation:
    /*
    dx = 5.787940*Scale;
    dy = 1.880615*Scale;
    dz = 0.000000;
    */
    // Rotation:
    thx = 90.000000;
    phx = 288.000000;
    thy = 90.000000;
    phy = 18.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix750 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    dx = fRaylay4Outer*(pMatrix750->GetRotationMatrix()[1]);
    dy = fRaylay4Outer*(pMatrix750->GetRotationMatrix()[0]);
    dz =  0.00000;
    TGeoCombiTrans *pMatrix749 = new TGeoCombiTrans("", dx,dy,dz,pMatrix750);
    // Combi transformation:
    /*
    dx = 5.975883*Scale;
    dy = 0.628090*Scale;
    dz = 0.000000;
    */
    // Rotation:
    thx = 90.000000;
    phx = 276.000000;
    thy = 90.000000;
    phy = 6.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix752 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    dx = fRaylay4Inner*(pMatrix752->GetRotationMatrix()[1]);
    dy = fRaylay4Inner*(pMatrix752->GetRotationMatrix()[0]);
    dz =  0.00000;
    TGeoCombiTrans *pMatrix751 = new TGeoCombiTrans("", dx,dy,dz,pMatrix752);
    // Combi transformation:
    /*
    dx = 6.052462*Scale;
    dy = -0.636140*Scale;
    dz = 0.000000;
    */
    // Rotation:
    thx = 90.000000;
    phx = 264.000000;
    thy = 90.000000;
    phy = 354.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix754 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    dx = fRaylay4Outer*(pMatrix754->GetRotationMatrix()[1]);
    dy = fRaylay4Outer*(pMatrix754->GetRotationMatrix()[0]);
    dz =  0.00000;
    TGeoCombiTrans *pMatrix753 = new TGeoCombiTrans("", dx,dy,dz,pMatrix754);
    // Combi transformation:
    /*
    dx = 5.714708*Scale;
    dy = -1.856822*Scale;
    dz = 0.000000;
    */
    // Rotation:
    thx = 90.000000;
    phx = 252.000000;
    thy = 90.000000;
    phy = 342.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix756 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    dx = fRaylay4Inner*(pMatrix756->GetRotationMatrix()[1]);
    dy = fRaylay4Inner*(pMatrix756->GetRotationMatrix()[0]);
    dz =  0.00000;
    TGeoCombiTrans *pMatrix755 = new TGeoCombiTrans("", dx,dy,dz,pMatrix756);
    // Combi transformation:
    /*
    dx = 5.270457*Scale;
    dy = -3.042900*Scale;
    dz = 0.000000;
    */
    // Rotation:
    thx = 90.000000;
    phx = 240.000000;
    thy = 90.000000;
    phy = 330.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix758 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    dx = fRaylay4Outer*(pMatrix758->GetRotationMatrix()[1]);
    dy = fRaylay4Outer*(pMatrix758->GetRotationMatrix()[0]);
    dz =  0.00000;
    TGeoCombiTrans *pMatrix757 = new TGeoCombiTrans("", dx,dy,dz,pMatrix758);
    // Combi transformation:
    /*
    dx = 4.465408*Scale;
    dy = -4.020672*Scale;
    dz = 0.000000;
    */
    // Rotation:
    thx = 90.000000;
    phx = 228.000000;
    thy = 90.000000;
    phy = 318.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix760 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    dx = fRaylay4Inner*(pMatrix760->GetRotationMatrix()[1]);
    dy = fRaylay4Inner*(pMatrix760->GetRotationMatrix()[0]);
    dz =  0.00000;
    TGeoCombiTrans *pMatrix759 = new TGeoCombiTrans("", dx,dy,dz,pMatrix760);
    // Combi transformation:
    /*
    dx = 3.577143*Scale;
    dy = -4.923516*Scale;
    dz = 0.000000;
    */
    // Rotation:
    thx = 90.000000;
    phx = 216.000000;
    thy = 90.000000;
    phy = 306.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix762 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    dx = fRaylay4Outer*(pMatrix762->GetRotationMatrix()[1]);
    dy = fRaylay4Outer*(pMatrix762->GetRotationMatrix()[0]);
    dz =  0.00000;
    
    TGeoCombiTrans *pMatrix761 = new TGeoCombiTrans("", dx,dy,dz,pMatrix762);
    // Combi transformation:
    /*
    dx = 2.443999*Scale;
    dy = -5.489312*Scale;
    dz = 0.000000;
    */
    // Rotation:
    thx = 90.000000;
    phx = 204.000000;
    thy = 90.000000;
    phy = 294.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix764 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    dx = fRaylay4Inner*(pMatrix764->GetRotationMatrix()[1]);
    dy = fRaylay4Inner*(pMatrix764->GetRotationMatrix()[0]);
    dz =  0.00000;
    TGeoCombiTrans *pMatrix763 = new TGeoCombiTrans("", dx,dy,dz,pMatrix764);
    // Combi transformation:
    /*
    dx = 1.265308*Scale;
    dy = -5.952811*Scale;
    dz = 0.000000;
    */
    // Rotation:
    thx = 90.000000;
    phx = 192.000000;
    thy = 90.000000;
    phy = 282.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix766 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    dx = fRaylay4Outer*(pMatrix766->GetRotationMatrix()[1]);
    dy = fRaylay4Outer*(pMatrix766->GetRotationMatrix()[0]);
    dz =  0.00000;
    
    TGeoCombiTrans *pMatrix765 = new TGeoCombiTrans("", dx,dy,dz,pMatrix766);
    // Combi transformation:
    /*
    dx = -0.000001*Scale;
    dy = -6.008800*Scale;
    dz = 0.000000;
    */
    // Rotation:
    thx = 90.000000;
    phx = 180.000000;
    thy = 90.000000;
    phy = 270.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix768 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    dx = fRaylay4Inner*(pMatrix768->GetRotationMatrix()[1]);
    dy = fRaylay4Inner*(pMatrix768->GetRotationMatrix()[0]);
    dz =  0.00000;
    TGeoCombiTrans *pMatrix767 = new TGeoCombiTrans("", dx,dy,dz,pMatrix768);
    // Combi transformation:
    /*
    dx = -1.265310*Scale;
    dy = -5.952811*Scale;
    dz = 0.000000;
    */
    // Rotation:
    thx = 90.000000;
    phx = 168.000000;
    thy = 90.000000;
    phy = 258.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix770 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    dx = fRaylay4Outer*(pMatrix770->GetRotationMatrix()[1]);
    dy = fRaylay4Outer*(pMatrix770->GetRotationMatrix()[0]);
    dz =  0.00000;
    
    TGeoCombiTrans *pMatrix769 = new TGeoCombiTrans("", dx,dy,dz,pMatrix770);
    // Combi transformation:
    /*
    dx = -2.444000*Scale;
    dy = -5.489312*Scale;
    dz = 0.000000;
    */
    // Rotation:
    thx = 90.000000;
    phx = 156.000000;
    thy = 90.000000;
    phy = 246.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix772 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    dx = fRaylay4Inner*(pMatrix772->GetRotationMatrix()[1]);
    dy = fRaylay4Inner*(pMatrix772->GetRotationMatrix()[0]);
    dz =  0.00000;
    TGeoCombiTrans *pMatrix771 = new TGeoCombiTrans("", dx,dy,dz,pMatrix772);
    // Combi transformation:
    /*
    dx = -3.577144*Scale;
    dy = -4.923515*Scale;
    dz = 0.000000;
    */
    // Rotation:
    thx = 90.000000;
    phx = 144.000000;
    thy = 90.000000;
    phy = 234.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix774 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    dx = fRaylay4Outer*(pMatrix774->GetRotationMatrix()[1]);
    dy = fRaylay4Outer*(pMatrix774->GetRotationMatrix()[0]);
    dz =  0.00000;
    
    TGeoCombiTrans *pMatrix773 = new TGeoCombiTrans("", dx,dy,dz,pMatrix774);
    // Combi transformation:
    /*
    dx = -4.465409*Scale;
    dy = -4.020671*Scale;
    dz = 0.000000;
    */
    // Rotation:
    thx = 90.000000;
    phx = 132.000000;
    thy = 90.000000;
    phy = 222.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix776 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    dx = fRaylay4Inner*(pMatrix776->GetRotationMatrix()[1]);
    dy = fRaylay4Inner*(pMatrix776->GetRotationMatrix()[0]);
    dz =  0.00000;
    TGeoCombiTrans *pMatrix775 = new TGeoCombiTrans("", dx,dy,dz,pMatrix776);
    // Combi transformation:
    /*
    dx = -5.270458*Scale;
    dy = -3.042899*Scale;
    dz = 0.000000;
    */
    // Rotation:
    thx = 90.000000;
    phx = 120.000000;
    thy = 90.000000;
    phy = 210.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix778 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    dx = fRaylay4Outer*(pMatrix778->GetRotationMatrix()[1]);
    dy = fRaylay4Outer*(pMatrix778->GetRotationMatrix()[0]);
    dz =  0.00000;
    
    TGeoCombiTrans *pMatrix777 = new TGeoCombiTrans("", dx,dy,dz,pMatrix778);
    // Combi transformation:
    /*
    dx = -5.714709*Scale;
    dy = -1.856821*Scale;
    dz = 0.000000;
    */
    // Rotation:
    thx = 90.000000;
    phx = 108.000000;
    thy = 90.000000;
    phy = 198.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix780 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    dx = fRaylay4Inner*(pMatrix780->GetRotationMatrix()[1]);
    dy = fRaylay4Inner*(pMatrix780->GetRotationMatrix()[0]);
    dz =  0.00000;
    TGeoCombiTrans *pMatrix779 = new TGeoCombiTrans("", dx,dy,dz,pMatrix780);
    // Combi transformation:
    /*
    dx = -6.052462*Scale;
    dy = -0.636139*Scale;
    dz = 0.000000;
    */
    // Rotation:
    thx = 90.000000;
    phx = 96.000000;
    thy = 90.000000;
    phy = 186.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix782 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    dx = fRaylay4Outer*(pMatrix782->GetRotationMatrix()[1]);
    dy = fRaylay4Outer*(pMatrix782->GetRotationMatrix()[0]);
    dz =  0.00000;
    
    TGeoCombiTrans *pMatrix781 = new TGeoCombiTrans("", dx,dy,dz,pMatrix782);
    // Combi transformation:
    /*
    dx = -5.975883*Scale;
    dy = 0.628091*Scale;
    dz = 0.000000;
    */
    // Rotation:
    thx = 90.000000;
    phx = 84.000000;
    thy = 90.000000;
    phy = 174.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix784 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    dx = fRaylay4Inner*(pMatrix784->GetRotationMatrix()[1]);
    dy = fRaylay4Inner*(pMatrix784->GetRotationMatrix()[0]);
    dz =  0.00000;
    TGeoCombiTrans *pMatrix783 = new TGeoCombiTrans("", dx,dy,dz,pMatrix784);
    // Combi transformation:
    /*
    dx = -5.787940*Scale;
    dy = 1.880617*Scale;
    dz = 0.000000;
    */
    // Rotation:
    thx = 90.000000;
    phx = 72.000000;
    thy = 90.000000;
    phy = 162.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix786 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    dx = fRaylay4Outer*(pMatrix786->GetRotationMatrix()[1]);
    dy = fRaylay4Outer*(pMatrix786->GetRotationMatrix()[0]);
    dz =  0.00000;
    
    TGeoCombiTrans *pMatrix785 = new TGeoCombiTrans("", dx,dy,dz,pMatrix786);
    // Combi transformation:
    /*
    dx = -5.203773*Scale;
    dy = 3.004401*Scale;
    dz = 0.000000;
    */
    // Rotation:
    thx = 90.000000;
    phx = 60.000000;
    thy = 90.000000;
    phy = 150.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix788 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    dx = fRaylay4Inner*(pMatrix788->GetRotationMatrix()[1]);
    dy = fRaylay4Inner*(pMatrix788->GetRotationMatrix()[0]);
    dz =  0.00000;
    TGeoCombiTrans *pMatrix787 = new TGeoCombiTrans("", dx,dy,dz,pMatrix788);
    // Combi transformation:
    /*
    dx = -4.522630*Scale;
    dy = 4.072196*Scale;
    dz = 0.000000;
    */
    // Rotation:
    thx = 90.000000;
    phx = 48.000000;
    thy = 90.000000;
    phy = 138.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix790 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    dx = fRaylay4Outer*(pMatrix790->GetRotationMatrix()[1]);
    dy = fRaylay4Outer*(pMatrix790->GetRotationMatrix()[0]);
    dz =  0.00000;
    
    TGeoCombiTrans *pMatrix789 = new TGeoCombiTrans("", dx,dy,dz,pMatrix790);
    // Combi transformation:
    /*
    dx = -3.531883*Scale;
    dy = 4.861222*Scale;
    dz = 0.000000;
    */
    // Rotation:
    thx = 90.000000;
    phx = 36.000000;
    thy = 90.000000;
    phy = 126.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix792 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    dx = fRaylay4Inner*(pMatrix792->GetRotationMatrix()[1]);
    dy = fRaylay4Inner*(pMatrix792->GetRotationMatrix()[0]);
    dz =  0.00000;
    TGeoCombiTrans *pMatrix791 = new TGeoCombiTrans("", dx,dy,dz,pMatrix792);
    // Combi transformation:
    /*
    dx = -2.475317*Scale;
    dy = 5.559656*Scale;
    dz = 0.000000;
    */
    // Rotation:
    thx = 90.000000;
    phx = 24.000000;
    thy = 90.000000;
    phy = 114.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix794 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    dx = fRaylay4Outer*(pMatrix794->GetRotationMatrix()[1]);
    dy = fRaylay4Outer*(pMatrix794->GetRotationMatrix()[0]);
    dz =  0.00000;
    
    TGeoCombiTrans *pMatrix793 = new TGeoCombiTrans("", dx,dy,dz,pMatrix794);
    // Combi transformation:
    /*
    dx = -1.249299*Scale;
    dy = 5.877493*Scale;
    dz = 0.000000;
    */
    // Rotation:
    thx = 90.000000;
    phx = 12.000000;
    thy = 90.000000;
    phy = 102.000000;
    thz = 0.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix796 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    dx = fRaylay4Inner*(pMatrix796->GetRotationMatrix()[1]);
    dy = fRaylay4Inner*(pMatrix796->GetRotationMatrix()[0]);
    dz =  0.00000;
    TGeoCombiTrans *pMatrix795 = new TGeoCombiTrans("", dx,dy,dz,pMatrix796);
    
    /*dx = 0.000000*Scale;
    dy = 6.077000*Scale;
    dz = 0.000000;
    TGeoCombiTrans *pMatrix797 = new TGeoCombiTrans("");
    pMatrix797->SetTranslation(dx,dy,dz);
    */
    

    /*TGeoCombiTrans *pMatrix854 = new TGeoCombiTrans("", dx,dy,dz,pMatrix855);*/
    // Combi transformation:
    dx = 0.000000;
    dy = 0.000000;
    dz = 6.753800;
    TGeoCombiTrans *pMatrix86 = new TGeoCombiTrans("");
    pMatrix86->SetTranslation(dx,dy,dz);
    // Combi transformation:
    dx = 0.000000;
    dy = 0.000000;
    dz = -6.753800;
    // Rotation:
    thx = 90.000000;
    phx = 0.000000;
    thy = 90.000000;
    phy = 270.000000;
    thz = 180.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix88 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix87 = new TGeoCombiTrans("", dx,dy,dz,pMatrix88);
    // Combi transformation:
    dx = 0.000000;
    dy = 0.000000;
    dz = 6.801350;
    TGeoCombiTrans *pMatrix89 = new TGeoCombiTrans("");
    pMatrix89->SetTranslation(dx,dy,dz);
    // Combi transformation:
    dx = 0.000000;
    dy = 0.000000;
    dz = -6.801350;
    // Rotation:
    thx = 90.000000;
    phx = 0.000000;
    thy = 90.000000;
    phy = 270.000000;
    thz = 180.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix91 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix90 = new TGeoCombiTrans("", dx,dy,dz,pMatrix91);
    // Combi transformation:
    dx = 0.000000;
    dy = 0.000000;
    dz = 7.623800;
    TGeoCombiTrans *pMatrix92 = new TGeoCombiTrans("");
    pMatrix92->SetTranslation(dx,dy,dz);
    // Combi transformation:
    dx = 0.000000;
    dy = 0.000000;
    dz = -7.623800;
    // Rotation:
    thx = 90.000000;
    phx = 0.000000;
    thy = 90.000000;
    phy = 270.000000;
    thz = 180.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix94 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix93 = new TGeoCombiTrans("", dx,dy,dz,pMatrix94);
    // Combi transformation:
    dx = 0.000000;
    dy = 0.000000;
    dz = 9.523800;
    TGeoCombiTrans *pMatrix95 = new TGeoCombiTrans("");
    pMatrix95->SetTranslation(dx,dy,dz);
    // Combi transformation:
    dx = 0.000000;
    dy = 0.000000;
    dz = -9.523800;
    // Rotation:
    thx = 90.000000;
    phx = 0.000000;
    thy = 90.000000;
    phy = 270.000000;
    thz = 180.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix97 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix96 = new TGeoCombiTrans("", dx,dy,dz,pMatrix97);
    // Combi transformation:
    dx = 0.000000;
    dy = 0.000000;
    dz = 12.523800;
    TGeoCombiTrans *pMatrix98 = new TGeoCombiTrans("");
    pMatrix98->SetTranslation(dx,dy,dz);
    // Combi transformation:
    dx = 0.000000;
    dy = 0.000000;
    dz = -12.523800;
    // Rotation:
    thx = 90.000000;
    phx = 0.000000;
    thy = 90.000000;
    phy = 270.000000;
    thz = 180.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix100 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix99 = new TGeoCombiTrans("", dx,dy,dz,pMatrix100);
    // Combi transformation:
    dx = 0.000000;
    dy = 0.000000;
    dz = 18.023800;
    TGeoCombiTrans *pMatrix101 = new TGeoCombiTrans("");
    pMatrix101->SetTranslation(dx,dy,dz);
    // Combi transformation:
    dx = 0.000000;
    dy = 0.000000;
    dz = -18.023800;
    // Rotation:
    thx = 90.000000;
    phx = 0.000000;
    thy = 90.000000;
    phy = 270.000000;
    thz = 180.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix103 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix102 = new TGeoCombiTrans("", dx,dy,dz,pMatrix103);
    // Combi transformation:
    dx = 0.000000;
    dy = 0.000000;
    dz = 97.521350;
    TGeoCombiTrans *pMatrix104 = new TGeoCombiTrans("");
    pMatrix104->SetTranslation(dx,dy,dz);
    // Combi transformation:
    dx = 0.000000;
    dy = 0.000000;
    dz = -97.521350;
    // Rotation:
    thx = 90.000000;
    phx = 0.000000;
    thy = 90.000000;
    phy = 270.000000;
    thz = 180.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix106 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix105 = new TGeoCombiTrans("", dx,dy,dz,pMatrix106);
    // Combi transformation:
    dx = 0.000000;
    dy = 0.000000;
    dz = 99.761350;
    TGeoCombiTrans *pMatrix107 = new TGeoCombiTrans("");
    pMatrix107->SetTranslation(dx,dy,dz);
    // Combi transformation:
    dx = 0.000000;
    dy = 0.000000;
    dz = -99.761350;
    // Rotation:
    thx = 90.000000;
    phx = 0.000000;
    thy = 90.000000;
    phy = 270.000000;
    thz = 180.000000;
    phz = 0.000000;
    TGeoRotation *pMatrix109 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
    TGeoCombiTrans *pMatrix108 = new TGeoCombiTrans("", dx,dy,dz,pMatrix109);
//Fine matrici di roto-traslazione    
    
    // Shape: world_box type: TGeoBBox
    dx = 1500.000000;
    dy = 1500.000000;
    dz = 1500.000000;
    TGeoShape *pworld_box_1 = new TGeoBBox("world_box", dx,dy,dz);
    // Volume: ILCM
    //cout <<"gGeoManager::"<<gGeoManager->GetTopVolume()<<endl;
    TGeoVolume *pILCM_a3a7cf8 = gGeoManager->GetVolume("ILCM");
    //cout <<"pILCM_a3a7cf8::"<<pILCM_a3a7cf8<<endl;
    pILCM_a3a7cf8->SetLineColor(23);

    // SET TOP VOLUME OF GEOMETRY
    gGeoManager->SetTopVolume(pILCM_a3a7cf8);

    // SHAPES, VOLUMES AND GEOMETRICAL HIERARCHY
    // Shape: tracking_cylinder type: TGeoTubeSeg
    rmin = 0.000000;
// // // //     rmax = 12.000000*Scale;
    rmax = 8.200000*Scale;
    dz   = 167.900000;
    phi1 = 0.000000;
    phi2 = 360.000000;
    TGeoShape *ptracking_cylinder_2 = new TGeoTubeSeg("tracking_cylinder",rmin,rmax,dz,phi1,phi2);
   // Volume: TARGETV
    TGeoVolume *pTARGETV_a404f40 = new TGeoVolume("TARGETV",ptracking_cylinder_2, pMed2);
    pTARGETV_a404f40->SetLineColor(23);
    pILCM_a3a7cf8->AddNode(pTARGETV_a404f40, 579);

    // Shape: TARGETcableZbackward_envelope_polycone type: TGeoPcon
    phi1  = 0.000000;
    dphi  = 360.000000;
    nz    = 2;
    TGeoPcon *pcon = new TGeoPcon("TARGETcableZbackward_envelope_polycone",phi1,dphi,nz);
    z     = -167.900000;
    rmin  = 8.317645;
    rmax  = 8.321645;
    pcon->DefineSection(0, z,rmin,rmax);
    z     = -6.800000;
    rmin  = 1.300000;
    rmax  = 1.312000;
    pcon->DefineSection(1, z,rmin,rmax);

    TGeoShape *pTARGETcableZbackward_envelope_polycone_3 = pcon;
    // Volume: TARGETcableZbackward_envelope_volume
    TGeoVolume *pTARGETcableZbackward_envelope_volume_a4051d0 = new 		 TGeoVolume("TARGETcableZbackward_envelope_volume",pTARGETcableZbackward_envelope_polycone_3, pMed3);
    pTARGETcableZbackward_envelope_volume_a4051d0->SetLineColor(42);
    pTARGETV_a404f40->AddNode(pTARGETcableZbackward_envelope_volume_a4051d0, 507);

    // Shape: SteelConeZforward_envelope_polycone type: TGeoPcon
    phi1  = 0.000000;
    dphi  = 360.000000;
    nz    = 2;
    pcon = new TGeoPcon("SteelConeZforward_envelope_polycone",phi1,dphi,nz);
    z     = 37.510000;
    rmin  = 2.556643;
    rmax  = 2.644143;
    pcon->DefineSection(0, z,rmin,rmax);
    z     = 167.900000;
    rmin  = 8.217645;
    rmax  = 8.416845;
    pcon->DefineSection(1, z,rmin,rmax);
    TGeoShape *pSteelConeZforward_envelope_polycone_4 = pcon;
    // Volume: SteelConeZforward_envelope_volume
    TGeoVolume *pSteelConeZforward_envelope_volume_a40bbb8 = new TGeoVolume("SteelConeZforward_envelope_volume",pSteelConeZforward_envelope_polycone_4, pMed4);
    pSteelConeZforward_envelope_volume_a40bbb8->SetLineColor(41);
    pTARGETV_a404f40->AddNode(pSteelConeZforward_envelope_volume_a40bbb8, 508);

    // Shape: VertexBarrelSupports_layer5 type: TGeoTubeSeg
    //Strato involucro cilindrico
//     rmin = 11.954000;//*Scale;
//     rmax = 11.996700;//*Scale;
// // // //     rmin = 11.954000*Scale;
// // // //     rmax = 11.996700*Scale;
    rmin = 8.100000*Scale;
    rmax = 8.140000*Scale;
    dz   = 100.000000;
    phi1 = 0.000000;
    phi2 = 360.000000;
    TGeoShape *pVertexBarrelSupports_layer5_5 = new TGeoTubeSeg("VertexBarrelSupports_layer5",rmin,rmax,dz,phi1,phi2);
    // Volume: VertexBarrelSupports_layer5_volume
    TGeoVolume *pVertexBarrelSupports_layer5_volume_a40be28 = new TGeoVolume("VertexBarrelSupports_layer5_volume",pVertexBarrelSupports_layer5_5, pMed2);
    pVertexBarrelSupports_layer5_volume_a40be28->SetLineColor(23);
    pTARGETV_a404f40->AddNode(pVertexBarrelSupports_layer5_volume_a40be28, 509);

//------------------>Ring Verdi
    // Shape: VertexReadout_layer0 type: TGeoTubeSeg
    rmin = 1.460000*Scale;
    rmax = 1.660000*Scale;
    dz   = 0.249000;
    phi1 = 0.000000;
    phi2 = 360.000000;
    TGeoShape *pVertexReadout_layer0_6 = new TGeoTubeSeg("VertexReadout_layer0",rmin,rmax,dz,phi1,phi2);
    // Volume: VertexReadout_layer0_volume
    TGeoVolume *pVertexReadout_layer0_volume_a40c478 = new TGeoVolume("VertexReadout_layer0_volume",pVertexReadout_layer0_6, pMed2);
    pVertexReadout_layer0_volume_a40c478->SetLineColor(23);
    pTARGETV_a404f40->AddNode(pVertexReadout_layer0_volume_a40c478, 510, pMatrix7);
    pTARGETV_a404f40->AddNode(pVertexReadout_layer0_volume_a40c478, 511, pMatrix8);

    // Shape: VertexReadout_layer1 type: TGeoTubeSeg
    rmin = 2.260000*Scale;
    rmax = 2.460000*Scale;
    dz   = 0.249000;
    phi1 = 0.000000;
    phi2 = 360.000000;
    TGeoShape *pVertexReadout_layer1_7 = new TGeoTubeSeg("VertexReadout_layer1",rmin,rmax,dz,phi1,phi2);
    // Volume: VertexReadout_layer1_volume
    TGeoVolume *pVertexReadout_layer1_volume_a40c9b8 = new TGeoVolume("VertexReadout_layer1_volume",pVertexReadout_layer1_7, pMed2);
    pVertexReadout_layer1_volume_a40c9b8->SetLineColor(23);
    pTARGETV_a404f40->AddNode(pVertexReadout_layer1_volume_a40c9b8, 512, pMatrix10);
    pTARGETV_a404f40->AddNode(pVertexReadout_layer1_volume_a40c9b8, 513, pMatrix11);

    // Shape: VertexReadout_layer2 type: TGeoTubeSeg
    rmin = 3.540000*Scale;
    rmax = 3.740000*Scale;
    dz   = 0.249000;
    phi1 = 0.000000;
    phi2 = 360.000000;
    TGeoShape *pVertexReadout_layer2_8 = new TGeoTubeSeg("VertexReadout_layer2",rmin,rmax,dz,phi1,phi2);
    // Volume: VertexReadout_layer2_volume
    TGeoVolume *pVertexReadout_layer2_volume_a40cce8 = new TGeoVolume("VertexReadout_layer2_volume",pVertexReadout_layer2_8, pMed2);
    pVertexReadout_layer2_volume_a40cce8->SetLineColor(23);
    pTARGETV_a404f40->AddNode(pVertexReadout_layer2_volume_a40cce8, 514, pMatrix13);
    pTARGETV_a404f40->AddNode(pVertexReadout_layer2_volume_a40cce8, 515, pMatrix14);

    // Shape: VertexReadout_layer3 type: TGeoTubeSeg
    rmin = 4.800000*Scale;
    rmax = 5.000000*Scale;
    dz   = 0.249000;
    phi1 = 0.000000;
    phi2 = 360.000000;
    TGeoShape *pVertexReadout_layer3_9 = new TGeoTubeSeg("VertexReadout_layer3",rmin,rmax,dz,phi1,phi2);
    // Volume: VertexReadout_layer3_volume
    TGeoVolume *pVertexReadout_layer3_volume_a40d018 = new TGeoVolume("VertexReadout_layer3_volume",pVertexReadout_layer3_9, pMed2);
    pVertexReadout_layer3_volume_a40d018->SetLineColor(23);
    pTARGETV_a404f40->AddNode(pVertexReadout_layer3_volume_a40d018, 516, pMatrix16);
    pTARGETV_a404f40->AddNode(pVertexReadout_layer3_volume_a40d018, 517, pMatrix17);

    // Shape: VertexReadout_layer4 type: TGeoTubeSeg
    rmin = 6.040000*Scale;
    rmax = 6.240000*Scale;
    dz   = 0.249000;
    phi1 = 0.000000;
    phi2 = 360.000000;
    TGeoShape *pVertexReadout_layer4_10 = new TGeoTubeSeg("VertexReadout_layer4",rmin,rmax,dz,phi1,phi2);
    // Volume: VertexReadout_layer4_volume
    TGeoVolume *pVertexReadout_layer4_volume_a40d348 = new TGeoVolume("VertexReadout_layer4_volume",pVertexReadout_layer4_10, pMed2);
    pVertexReadout_layer4_volume_a40d348->SetLineColor(23);
    pTARGETV_a404f40->AddNode(pVertexReadout_layer4_volume_a40d348, 518, pMatrix19);
    pTARGETV_a404f40->AddNode(pVertexReadout_layer4_volume_a40d348, 519, pMatrix20);

    // Shape: VertexReadout_layer5 type: TGeoTubeSeg
    rmin = 1.351000*Scale;//*Scale;//l'ho scalato
    rmax = 2.260000*Scale;
    dz   = 0.002850;
    phi1 = 0.000000;
    phi2 = 360.000000;
    TGeoShape *pVertexReadout_layer5_11 = new TGeoTubeSeg("VertexReadout_layer5",rmin,rmax,dz,phi1,phi2);
    // Volume: VertexReadout_layer5_volume
    TGeoVolume *pVertexReadout_layer5_volume_a40d678 = new TGeoVolume("VertexReadout_layer5_volume",pVertexReadout_layer5_11, pMed2);
    pVertexReadout_layer5_volume_a40d678->SetLineColor(23);
    pTARGETV_a404f40->AddNode(pVertexReadout_layer5_volume_a40d678, 520, pMatrix22);
    pTARGETV_a404f40->AddNode(pVertexReadout_layer5_volume_a40d678, 521, pMatrix23);
    // Shape: VertexReadout_layer6 type: TGeoTubeSeg
    rmin = 2.261000*Scale;
    rmax = 3.540000*Scale;
    dz   = 0.001550;
    phi1 = 0.000000;
    phi2 = 360.000000;
    TGeoShape *pVertexReadout_layer6_12 = new TGeoTubeSeg("VertexReadout_layer6",rmin,rmax,dz,phi1,phi2);
    // Volume: VertexReadout_layer6_volume
    TGeoVolume *pVertexReadout_layer6_volume_a40d9a8 = new TGeoVolume("VertexReadout_layer6_volume",pVertexReadout_layer6_12, pMed2);
    pVertexReadout_layer6_volume_a40d9a8->SetLineColor(23);
    pTARGETV_a404f40->AddNode(pVertexReadout_layer6_volume_a40d9a8, 522, pMatrix25);
    pTARGETV_a404f40->AddNode(pVertexReadout_layer6_volume_a40d9a8, 523, pMatrix26);

    // Shape: VertexReadout_layer7 type: TGeoTubeSeg
    rmin = 3.541000*Scale;
    rmax = 4.800000*Scale;
    dz   = 0.000800;
    phi1 = 0.000000;
    phi2 = 360.000000;
    TGeoShape *pVertexReadout_layer7_13 = new TGeoTubeSeg("VertexReadout_layer7",rmin,rmax,dz,phi1,phi2);

    // Volume: VertexReadout_layer7_volume
    TGeoVolume *pVertexReadout_layer7_volume_a40dcd8 = new TGeoVolume("VertexReadout_layer7_volume",pVertexReadout_layer7_13, pMed2);
    pVertexReadout_layer7_volume_a40dcd8->SetLineColor(23);
    pTARGETV_a404f40->AddNode(pVertexReadout_layer7_volume_a40dcd8, 524, pMatrix28);
    pTARGETV_a404f40->AddNode(pVertexReadout_layer7_volume_a40dcd8, 525, pMatrix29);
    // Shape: VertexReadout_layer8 type: TGeoTubeSeg
    rmin = 4.801000*Scale;
    rmax = 6.040000*Scale;
    dz   = 0.000350;
    phi1 = 0.000000;
    phi2 = 360.000000;
    TGeoShape *pVertexReadout_layer8_14 = new TGeoTubeSeg("VertexReadout_layer8",rmin,rmax,dz,phi1,phi2);
    // Volume: VertexReadout_layer8_volume
    TGeoVolume *pVertexReadout_layer8_volume_a40e008 = new TGeoVolume("VertexReadout_layer8_volume",pVertexReadout_layer8_14, pMed2);
    pVertexReadout_layer8_volume_a40e008->SetLineColor(23);
    pTARGETV_a404f40->AddNode(pVertexReadout_layer8_volume_a40e008, 526, pMatrix31);
    pTARGETV_a404f40->AddNode(pVertexReadout_layer8_volume_a40e008, 527, pMatrix32);

    // Shape: VertexReadout_layer9 type: TGeoTubeSeg
    rmin = 1.500000*Scale;
    rmax = 1.599000*Scale;
    dz   = 0.100000;
    phi1 = 0.000000;
    phi2 = 360.000000;
    TGeoShape *pVertexReadout_layer9_15 = new TGeoTubeSeg("VertexReadout_layer9",rmin,rmax,dz,phi1,phi2);
    // Volume: VertexReadout_layer9_volume
    TGeoVolume *pVertexReadout_layer9_volume_a40e338 = new TGeoVolume("VertexReadout_layer9_volume",pVertexReadout_layer9_15, pMed2);
    pVertexReadout_layer9_volume_a40e338->SetLineColor(23);
    pTARGETV_a404f40->AddNode(pVertexReadout_layer9_volume_a40e338, 528, pMatrix34);
    pTARGETV_a404f40->AddNode(pVertexReadout_layer9_volume_a40e338, 529, pMatrix35);

    // Shape: VertexReadout_layer10 type: TGeoTubeSeg
    rmin = 1.500000*Scale;
    rmax = 1.599000*Scale;
    dz   = 0.100000;
    phi1 = 0.000000;
    phi2 = 360.000000;
    TGeoShape *pVertexReadout_layer10_16 = new TGeoTubeSeg("VertexReadout_layer10",rmin,rmax,dz,phi1,phi2);
    // Volume: VertexReadout_layer10_volume
    TGeoVolume *pVertexReadout_layer10_volume_a40e668 = new TGeoVolume("VertexReadout_layer10_volume",pVertexReadout_layer10_16, pMed2);
    pVertexReadout_layer10_volume_a40e668->SetLineColor(23);
    pTARGETV_a404f40->AddNode(pVertexReadout_layer10_volume_a40e668, 530, pMatrix37);
    pTARGETV_a404f40->AddNode(pVertexReadout_layer10_volume_a40e668, 531, pMatrix38);

    // Shape: VertexReadout_layer11 type: TGeoTubeSeg
    rmin = 1.900000*Scale;
    rmax = 1.999000*Scale;
    dz   = 0.100000;
    phi1 = 0.000000;
    phi2 = 360.000000;
    TGeoShape *pVertexReadout_layer11_17 = new TGeoTubeSeg("VertexReadout_layer11",rmin,rmax,dz,phi1,phi2);
    // Volume: VertexReadout_layer11_volume
    TGeoVolume *pVertexReadout_layer11_volume_a40e9a0 = new TGeoVolume("VertexReadout_layer11_volume",pVertexReadout_layer11_17, pMed2);
    pVertexReadout_layer11_volume_a40e9a0->SetLineColor(23);
    pTARGETV_a404f40->AddNode(pVertexReadout_layer11_volume_a40e9a0, 532, pMatrix40);
    pTARGETV_a404f40->AddNode(pVertexReadout_layer11_volume_a40e9a0, 533, pMatrix41);

    // Shape: VertexReadout_layer12 type: TGeoTubeSeg
    rmin = 1.900000*Scale;
    rmax = 1.999000*Scale;
    dz   = 0.100000;
    phi1 = 0.000000;
    phi2 = 360.000000;
    TGeoShape *pVertexReadout_layer12_18 = new TGeoTubeSeg("VertexReadout_layer12",rmin,rmax,dz,phi1,phi2);
    // Volume: VertexReadout_layer12_volume
    TGeoVolume *pVertexReadout_layer12_volume_a40ecd8 = new TGeoVolume("VertexReadout_layer12_volume",pVertexReadout_layer12_18, pMed2);
    pVertexReadout_layer12_volume_a40ecd8->SetLineColor(23);
    pTARGETV_a404f40->AddNode(pVertexReadout_layer12_volume_a40ecd8, 534, pMatrix43);
    pTARGETV_a404f40->AddNode(pVertexReadout_layer12_volume_a40ecd8, 535, pMatrix44);

    // Shape: VertexReadout_layer13 type: TGeoTubeSeg
    rmin = 7.501000*Scale;
    rmax = 8.000000*Scale;
    dz   = 0.100000;
    phi1 = 0.000000;
    phi2 = 360.000000;
    TGeoShape *pVertexReadout_layer13_19 = new TGeoTubeSeg("VertexReadout_layer13",rmin,rmax,dz,phi1,phi2);
    // Volume: VertexReadout_layer13_volume
    TGeoVolume *pVertexReadout_layer13_volume_a40f010 = new TGeoVolume("VertexReadout_layer13_volume",pVertexReadout_layer13_19, pMed2);
    pVertexReadout_layer13_volume_a40f010->SetLineColor(23);
    pTARGETV_a404f40->AddNode(pVertexReadout_layer13_volume_a40f010, 536, pMatrix46);
    pTARGETV_a404f40->AddNode(pVertexReadout_layer13_volume_a40f010, 537, pMatrix47);

    // Shape: VertexReadout_layer14 type: TGeoTubeSeg
    rmin = 7.501000*Scale;
    rmax = 8.000000*Scale;
    dz   = 0.100000;
    phi1 = 0.000000;
    phi2 = 360.000000;
    TGeoShape *pVertexReadout_layer14_20 = new TGeoTubeSeg("VertexReadout_layer14",rmin,rmax,dz,phi1,phi2);
    // Volume: VertexReadout_layer14_volume
    TGeoVolume *pVertexReadout_layer14_volume_a40f348 = new TGeoVolume("VertexReadout_layer14_volume",pVertexReadout_layer14_20, pMed2);
    pVertexReadout_layer14_volume_a40f348->SetLineColor(23);
    pTARGETV_a404f40->AddNode(pVertexReadout_layer14_volume_a40f348, 538, pMatrix49);
    pTARGETV_a404f40->AddNode(pVertexReadout_layer14_volume_a40f348, 539, pMatrix50);

    // Shape: VertexReadout_layer15 type: TGeoTubeSeg
    rmin = 7.501000*Scale;
    rmax = 8.000000*Scale;
    dz   = 0.100000;
    phi1 = 0.000000;
    phi2 = 360.000000;
    TGeoShape *pVertexReadout_layer15_21 = new TGeoTubeSeg("VertexReadout_layer15",rmin,rmax,dz,phi1,phi2);
    // Volume: VertexReadout_layer15_volume
    TGeoVolume *pVertexReadout_layer15_volume_a40f680 = new TGeoVolume("VertexReadout_layer15_volume",pVertexReadout_layer15_21, pMed2);
    pVertexReadout_layer15_volume_a40f680->SetLineColor(23);
    pTARGETV_a404f40->AddNode(pVertexReadout_layer15_volume_a40f680, 540, pMatrix52);
    pTARGETV_a404f40->AddNode(pVertexReadout_layer15_volume_a40f680, 541, pMatrix53);


    // Shape: VertexReadout_layer16 type: TGeoTubeSeg
    rmin = 7.501000*Scale;
    rmax = 8.000000*Scale;
    dz   = 0.100000;
    phi1 = 0.000000;
    phi2 = 360.000000;
    TGeoShape *pVertexReadout_layer16_22 = new TGeoTubeSeg("VertexReadout_layer16",rmin,rmax,dz,phi1,phi2);
    // Volume: VertexReadout_layer16_volume
    TGeoVolume *pVertexReadout_layer16_volume_a40f9b8 = new TGeoVolume("VertexReadout_layer16_volume",pVertexReadout_layer16_22, pMed2);
    pVertexReadout_layer16_volume_a40f9b8->SetLineColor(23);
    pTARGETV_a404f40->AddNode(pVertexReadout_layer16_volume_a40f9b8, 542, pMatrix55);
    pTARGETV_a404f40->AddNode(pVertexReadout_layer16_volume_a40f9b8, 543, pMatrix56);


//--------------->   

    // Shape: BeamPipeLiner_envelope_polycone type: TGeoPcon
    phi1  = 0.000000;
    dphi  = 360.000000;
    nz    = 6;
    pcon = new TGeoPcon("BeamPipeLiner_envelope_polycone",phi1,dphi,nz);
    z     = -167.900000;
    rmin  = 8.210145;
    rmax  = 8.217645;
    pcon->DefineSection(0, z,rmin,rmax);
    z     = -6.251000;
    rmin  = 1.192500;
    rmax  = 1.200000;
    pcon->DefineSection(1, z,rmin,rmax);
    z     = -6.250000;
    rmin  = 1.197500;
    rmax  = 1.200000;
    pcon->DefineSection(2, z,rmin,rmax);
    z     = 6.250000;
    rmin  = 1.197500;
    rmax  = 1.200000;
    pcon->DefineSection(3, z,rmin,rmax);
    z     = 6.251000;
    rmin  = 1.192500;
    rmax  = 1.200000;
    pcon->DefineSection(4, z,rmin,rmax);
    z     = 167.900000;
    rmin  = 8.210145;
    rmax  = 8.217645;
    pcon->DefineSection(5, z,rmin,rmax);
    TGeoShape *pBeamPipeLiner_envelope_polycone_23 = pcon;
    // Volume: BeamPipeLiner_envelope_volume
    TGeoVolume *pBeamPipeLiner_envelope_volume_a40fcf0 = new TGeoVolume("BeamPipeLiner_envelope_volume",pBeamPipeLiner_envelope_polycone_23, pMed7);
    pBeamPipeLiner_envelope_volume_a40fcf0->SetLineColor(18);
    pTARGETV_a404f40->AddNode(pBeamPipeLiner_envelope_volume_a40fcf0, 544);
 
    // Shape: VertexEndcap_layer0 type: TGeoTubeSeg
    rmin = 1.600000*Scale;
    rmax = 7.500000*Scale;
    dz   = 0.005650;
    phi1 = 0.000000;
    phi2 = 360.000000;
    TGeoShape *pVertexEndcap_layer0_24 = new TGeoTubeSeg("VertexEndcap_layer0",rmin,rmax,dz,phi1,phi2);
    // Volume: VertexEndcap_layer0_volume
    TGeoVolume *pVertexEndcap_layer0_volume_a40ffa8 = new TGeoVolume("VertexEndcap_layer0_volume",pVertexEndcap_layer0_24, pMed2);
    pVertexEndcap_layer0_volume_a40ffa8->SetLineColor(23);
    pTARGETV_a404f40->AddNode(pVertexEndcap_layer0_volume_a40ffa8, 545, pMatrix59);
 
    // Shape: VertexEndcap_layer1 type: TGeoTubeSeg
    rmin = 1.600000*Scale;
    rmax = 7.500000*Scale;
    dz   = 0.005650;
    phi1 = 0.000000;
    phi2 = 360.000000;
    TGeoShape *pVertexEndcap_layer1_25 = new TGeoTubeSeg("VertexEndcap_layer1",rmin,rmax,dz,phi1,phi2);
    // Volume: VertexEndcap_layer1_volume
    TGeoVolume *pVertexEndcap_layer1_volume_a415548 = new TGeoVolume("VertexEndcap_layer1_volume",pVertexEndcap_layer1_25, pMed2);
    pVertexEndcap_layer1_volume_a415548->SetLineColor(23);
    pTARGETV_a404f40->AddNode(pVertexEndcap_layer1_volume_a415548, 546, pMatrix60);
 
    // Shape: VertexEndcap_layer2 type: TGeoTubeSeg
    rmin = 2.000000*Scale;
    rmax = 7.500000*Scale;
    dz   = 0.005650;
    phi1 = 0.000000;
    phi2 = 360.000000;
    TGeoShape *pVertexEndcap_layer2_26 = new TGeoTubeSeg("VertexEndcap_layer2",rmin,rmax,dz,phi1,phi2);
    // Volume: VertexEndcap_layer2_volume
    TGeoVolume *pVertexEndcap_layer2_volume_a418840 = new TGeoVolume("VertexEndcap_layer2_volume",pVertexEndcap_layer2_26, pMed2);
    pVertexEndcap_layer2_volume_a418840->SetLineColor(23);
    pTARGETV_a404f40->AddNode(pVertexEndcap_layer2_volume_a418840, 547, pMatrix61);
 
    // Shape: VertexEndcap_layer3 type: TGeoTubeSeg
    rmin = 2.000000*Scale;
    rmax = 7.500000*Scale;
    dz   = 0.005650;
    phi1 = 0.000000;
    phi2 = 360.000000;
    TGeoShape *pVertexEndcap_layer3_27 = new TGeoTubeSeg("VertexEndcap_layer3",rmin,rmax,dz,phi1,phi2);
    // Volume: VertexEndcap_layer3_volume
    TGeoVolume *pVertexEndcap_layer3_volume_a41bc78 = new TGeoVolume("VertexEndcap_layer3_volume",pVertexEndcap_layer3_27, pMed2);
    pVertexEndcap_layer3_volume_a41bc78->SetLineColor(23);
    pTARGETV_a404f40->AddNode(pVertexEndcap_layer3_volume_a41bc78, 548, pMatrix62);
    // Volume: VertexEndcap_layer4_volume
    TGeoVolume *pVertexEndcap_layer4_volume_a41f010 = new TGeoVolume("VertexEndcap_layer4_volume",pVertexEndcap_layer0_24, pMed2);
    pVertexEndcap_layer4_volume_a41f010->SetLineColor(23);
    pTARGETV_a404f40->AddNode(pVertexEndcap_layer4_volume_a41f010, 549, pMatrix63);
    // Volume: VertexEndcap_layer5_volume
    TGeoVolume *pVertexEndcap_layer5_volume_a422368 = new TGeoVolume("VertexEndcap_layer5_volume",pVertexEndcap_layer1_25, pMed2);
    pVertexEndcap_layer5_volume_a422368->SetLineColor(23);
    pTARGETV_a404f40->AddNode(pVertexEndcap_layer5_volume_a422368, 550, pMatrix65);
    // Volume: VertexEndcap_layer6_volume
    TGeoVolume *pVertexEndcap_layer6_volume_a4256c0 = new TGeoVolume("VertexEndcap_layer6_volume",pVertexEndcap_layer2_26, pMed2);
    pVertexEndcap_layer6_volume_a4256c0->SetLineColor(23);
    pTARGETV_a404f40->AddNode(pVertexEndcap_layer6_volume_a4256c0, 551, pMatrix67);
    // Volume: VertexEndcap_layer7_volume
    TGeoVolume *pVertexEndcap_layer7_volume_a3d5740 = new TGeoVolume("VertexEndcap_layer7_volume",pVertexEndcap_layer3_27, pMed2);
    pVertexEndcap_layer7_volume_a3d5740->SetLineColor(23);
    pTARGETV_a404f40->AddNode(pVertexEndcap_layer7_volume_a3d5740, 552, pMatrix69);
   
//--------------->   
   
    // Shape: SteelConeZbackward_envelope_polycone type: TGeoPcon
    phi1  = 0.000000;
    dphi  = 360.000000;
    nz    = 2;
    pcon = new TGeoPcon("SteelConeZbackward_envelope_polycone",phi1,dphi,nz);
    z     = -167.900000;
    rmin  = 8.217645;
    rmax  = 8.416845;
    pcon->DefineSection(0, z,rmin,rmax);
    z     = -37.510000;
    rmin  = 2.556643;
    rmax  = 2.644143;
    pcon->DefineSection(1, z,rmin,rmax);
    TGeoShape *pSteelConeZbackward_envelope_polycone_28 = pcon;
    // Volume: SteelConeZbackward_envelope_volume
    TGeoVolume *pSteelConeZbackward_envelope_volume_a3d8ae0 = new TGeoVolume("SteelConeZbackward_envelope_volume",pSteelConeZbackward_envelope_polycone_28, pMed4);
    pSteelConeZbackward_envelope_volume_a3d8ae0->SetLineColor(41);
    pTARGETV_a404f40->AddNode(pSteelConeZbackward_envelope_volume_a3d8ae0, 553);

//--------------->

    // Shape: BeamPipe_envelope_polycone type: TGeoPcon
    phi1  = 0.000000;
    dphi  = 360.000000;
    nz    = 6;
    pcon = new TGeoPcon("BeamPipe_envelope_polycone",phi1,dphi,nz);
    z     = -37.500000;
    rmin  = 2.556643;
    rmax  = 2.644143;
    pcon->DefineSection(0, z,rmin,rmax);
    z     = -6.251000;
    rmin  = 1.200000;
    rmax  = 1.287500;
    pcon->DefineSection(1, z,rmin,rmax);
    z     = -6.250000;
    rmin  = 1.200000;
    rmax  = 1.240000;
    pcon->DefineSection(2, z,rmin,rmax);
    z     = 6.250000;
    rmin  = 1.200000;
    rmax  = 1.240000;
    pcon->DefineSection(3, z,rmin,rmax);
    z     = 6.251000;
    rmin  = 1.200000;
    rmax  = 1.300000;
    pcon->DefineSection(4, z,rmin,rmax);
    z     = 37.500000;
    rmin  = 2.556643;
    rmax  = 2.644143;
    pcon->DefineSection(5, z,rmin,rmax);
    TGeoShape *pBeamPipe_envelope_polycone_29 = pcon;
    // Volume: BeamPipe_envelope_volume
    TGeoVolume *pBeamPipe_envelope_volume_a3d8c00 = new TGeoVolume("BeamPipe_envelope_volume",pBeamPipe_envelope_polycone_29, pMed9);
    pBeamPipe_envelope_volume_a3d8c00->SetLineColor(16);
    pTARGETV_a404f40->AddNode(pBeamPipe_envelope_volume_a3d8c00, 554);

//--------------->   

    // Shape: TARGETcableZforward_envelope_polycone type: TGeoPcon
    phi1  = 0.000000;
    dphi  = 360.000000;
    nz    = 2;
    pcon = new TGeoPcon("TARGETcableZforward_envelope_polycone",phi1,dphi,nz);
    z     = 6.800000;
    rmin  = 1.300000;
    rmax  = 1.312000;
    pcon->DefineSection(0, z,rmin,rmax);
    z     = 167.900000;
    rmin  = 8.317645;
    rmax  = 8.321645;
    pcon->DefineSection(1, z,rmin,rmax);
    TGeoShape *pTARGETcableZforward_envelope_polycone_30 = pcon;
    // Volume: TARGETcableZforward_envelope_volume
    TGeoVolume *pTARGETcableZforward_envelope_volume_a3d8eb0 = new TGeoVolume("TARGETcableZforward_envelope_volume",pTARGETcableZforward_envelope_polycone_30, pMed3);
    pTARGETcableZforward_envelope_volume_a3d8eb0->SetLineColor(42);
    pTARGETV_a404f40->AddNode(pTARGETcableZforward_envelope_volume_a3d8eb0, 555);

//--------------->  

    // Shape: TARGETserviceZbackward_envelope_polycone type: TGeoPcon
    phi1  = 0.000000;
    dphi  = 360.000000;
    nz    = 2;
    pcon = new TGeoPcon("TARGETserviceZbackward_envelope_polycone",phi1,dphi,nz);
    z     = -12.000000;
    rmin  = 1.400000;
    rmax  = 1.430000;
    pcon->DefineSection(0, z,rmin,rmax);
    z     = -10.000000;
    rmin  = 1.400000;
    rmax  = 1.430000;
    pcon->DefineSection(1, z,rmin,rmax);
    TGeoShape *pTARGETserviceZbackward_envelope_polycone_31 = pcon;
    // Volume: TARGETserviceZbackward_envelope_volume
    TGeoVolume *pTARGETserviceZbackward_envelope_volume_a3d8fd8 = new TGeoVolume("TARGETserviceZbackward_envelope_volume",pTARGETserviceZbackward_envelope_polycone_31, pMed6);
    pTARGETserviceZbackward_envelope_volume_a3d8fd8->SetLineColor(123);
    pTARGETV_a404f40->AddNode(pTARGETserviceZbackward_envelope_volume_a3d8fd8, 556);

//--------------->   

    // Shape: TARGETserviceZforward_envelope_polycone type: TGeoPcon
    phi1  = 0.000000;
    dphi  = 360.000000;
    nz    = 2;
    pcon = new TGeoPcon("TARGETserviceZforward_envelope_polycone",phi1,dphi,nz);
    z     = 10.000000;
    rmin  = 1.400000;
    rmax  = 1.430000;
    pcon->DefineSection(0, z,rmin,rmax);
    z     = 12.000000;
    rmin  = 1.400000;
    rmax  = 1.430000;
    pcon->DefineSection(1, z,rmin,rmax);
    TGeoShape *pTARGETserviceZforward_envelope_polycone_32 = pcon;
    // Volume: TARGETserviceZforward_envelope_volume
    TGeoVolume *pTARGETserviceZforward_envelope_volume_a3d9100 = new TGeoVolume("TARGETserviceZforward_envelope_volume",pTARGETserviceZforward_envelope_polycone_32, pMed6);
    pTARGETserviceZforward_envelope_volume_a3d9100->SetLineColor(123);
    pTARGETV_a404f40->AddNode(pTARGETserviceZforward_envelope_volume_a3d9100, 557);
   
//--------------->      

    // Shape: VertexBarrel_layer0 type: TGeoTubeSeg
    rmin = 1.350000*Scale;
    rmax = 1.700000*Scale;
    dz   = 6.250000;
    phi1 = 0.000000;
    phi2 = 360.000000;
    TGeoShape *pVertexBarrel_layer0_33 = new TGeoTubeSeg("VertexBarrel_layer0",rmin,rmax,dz,phi1,phi2);
    // Volume: VertexBarrel_layer0_volume
    TGeoVolume *pVertexBarrel_layer0_volume_a3d9220 = new TGeoVolume("VertexBarrel_layer0_volume",pVertexBarrel_layer0_33, pMed2);
    pVertexBarrel_layer0_volume_a3d9220->SetLineColor(23);
    pTARGETV_a404f40->AddNode(pVertexBarrel_layer0_volume_a3d9220, 558, pMatrix76);
    // Shape: VertexBarrel_layer1 type: TGeoTubeSeg
    rmin = 2.100000*Scale;
    rmax = 2.500000*Scale;
    dz   = 6.250000;
    phi1 = 0.000000;
    phi2 = 360.000000;
    TGeoShape *pVertexBarrel_layer1_34 = new TGeoTubeSeg("VertexBarrel_layer1",rmin,rmax,dz,phi1,phi2);
    // Volume: VertexBarrel_layer1_volume
    TGeoVolume *pVertexBarrel_layer1_volume_a3dd910 = new TGeoVolume("VertexBarrel_layer1_volume",pVertexBarrel_layer1_34, pMed2);
    pVertexBarrel_layer1_volume_a3dd910->SetLineColor(23);
    pTARGETV_a404f40->AddNode(pVertexBarrel_layer1_volume_a3dd910, 559, pMatrix78);
    // Shape: VertexBarrel_layer2 type: TGeoTubeSeg
    rmin = 3.400000*Scale;
    rmax = 3.700000*Scale;
    dz   = 6.250000;
    phi1 = 0.000000;
    phi2 = 360.000000;
    TGeoShape *pVertexBarrel_layer2_35 = new TGeoTubeSeg("VertexBarrel_layer2",rmin,rmax,dz,phi1,phi2);
    // Volume: VertexBarrel_layer2_volume
    TGeoVolume *pVertexBarrel_layer2_volume_a4410e8 = new TGeoVolume("VertexBarrel_layer2_volume",pVertexBarrel_layer2_35, pMed2);
    pVertexBarrel_layer2_volume_a4410e8->SetLineColor(23);
    pTARGETV_a404f40->AddNode(pVertexBarrel_layer2_volume_a4410e8, 560, pMatrix80);

// Shape: VertexBarrel_layer3 type: TGeoTubeSeg
    rmin = 4.600000*Scale;
    rmax = 5.000000*Scale;
    dz   = 6.250000;
    phi1 = 0.000000;
    phi2 = 360.000000;
    TGeoShape *pVertexBarrel_layer3_36 = new TGeoTubeSeg("VertexBarrel_layer3",rmin,rmax,dz,phi1,phi2);
    // Volume: VertexBarrel_layer3_volume
    TGeoVolume *pVertexBarrel_layer3_volume_a448080 = new TGeoVolume("VertexBarrel_layer3_volume",pVertexBarrel_layer3_36, pMed2);
    pVertexBarrel_layer3_volume_a448080->SetLineColor(23);
    pTARGETV_a404f40->AddNode(pVertexBarrel_layer3_volume_a448080, 561, pMatrix82);
    // Shape: VertexBarrel_layer4 type: TGeoTubeSeg
    rmin = 5.900000*Scale;
    rmax = 6.200000*Scale;
    dz   = 6.250000;
    phi1 = 0.000000;
    phi2 = 360.000000;
    TGeoShape *pVertexBarrel_layer4_37 = new TGeoTubeSeg("VertexBarrel_layer4",rmin,rmax,dz,phi1,phi2);
    // Volume: VertexBarrel_layer4_volume
    TGeoVolume *pVertexBarrel_layer4_volume_a4512f8 = new TGeoVolume("VertexBarrel_layer4_volume",pVertexBarrel_layer4_37, pMed2);
    pVertexBarrel_layer4_volume_a4512f8->SetLineColor(23);
    pTARGETV_a404f40->AddNode(pVertexBarrel_layer4_volume_a4512f8, 562, pMatrix84);
   
   
    // Shape: VertexEndSupports_layer0 type: TGeoTubeSeg
    rmin = 1.450000*Scale;
    rmax = 6.040000*Scale;
    dz   = 0.003800;
    phi1 = 0.000000;
    phi2 = 360.000000;
    TGeoShape *pVertexEndSupports_layer0_38 = new TGeoTubeSeg("VertexEndSupports_layer0",rmin,rmax,dz,phi1,phi2);
    // Volume: VertexEndSupports_layer0_volume
    TGeoVolume *pVertexEndSupports_layer0_volume_a45d058 = new TGeoVolume("VertexEndSupports_layer0_volume",pVertexEndSupports_layer0_38, pMed2);
    pVertexEndSupports_layer0_volume_a45d058->SetLineColor(23);
    pTARGETV_a404f40->AddNode(pVertexEndSupports_layer0_volume_a45d058, 563, pMatrix86);
    pTARGETV_a404f40->AddNode(pVertexEndSupports_layer0_volume_a45d058, 564, pMatrix87);
    // Shape: VertexEndSupports_layer1 type: TGeoTubeSeg
    rmin = 1.450000*Scale;
    rmax = 11.95300;
    dz   = 0.021350;
    phi1 = 0.000000;
    phi2 = 360.000000;
    TGeoShape *pVertexEndSupports_layer1_39 = new TGeoTubeSeg("VertexEndSupports_layer1",rmin,rmax,dz,phi1,phi2);
    // Volume: VertexEndSupports_layer1_volume
    TGeoVolume *pVertexEndSupports_layer1_volume_a45d398 = new TGeoVolume("VertexEndSupports_layer1_volume",pVertexEndSupports_layer1_39, pMed2);
    pVertexEndSupports_layer1_volume_a45d398->SetLineColor(23);
    pTARGETV_a404f40->AddNode(pVertexEndSupports_layer1_volume_a45d398, 565, pMatrix89);
    pTARGETV_a404f40->AddNode(pVertexEndSupports_layer1_volume_a45d398, 566, pMatrix90);
    // Shape: VertexEndSupports_layer2 type: TGeoTubeSeg
    rmin = 1.600000*Scale;
    rmax = 11.95300;
    dz   = 0.003800;
    phi1 = 0.000000;
    phi2 = 360.000000;
    TGeoShape *pVertexEndSupports_layer2_40 = new TGeoTubeSeg("VertexEndSupports_layer2",rmin,rmax,dz,phi1,phi2);
    // Volume: VertexEndSupports_layer2_volume
    TGeoVolume *pVertexEndSupports_layer2_volume_a45d6d8 = new TGeoVolume("VertexEndSupports_layer2_volume",pVertexEndSupports_layer2_40, pMed2);
    pVertexEndSupports_layer2_volume_a45d6d8->SetLineColor(23);
    pTARGETV_a404f40->AddNode(pVertexEndSupports_layer2_volume_a45d6d8, 567, pMatrix92);
    pTARGETV_a404f40->AddNode(pVertexEndSupports_layer2_volume_a45d6d8, 568, pMatrix93);
    // Shape: VertexEndSupports_layer3 type: TGeoTubeSeg
    rmin = 1.600000*Scale;
    rmax = 11.95300;
    dz   = 0.003800;
    phi1 = 0.000000;
    phi2 = 360.000000;
    TGeoShape *pVertexEndSupports_layer3_41 = new TGeoTubeSeg("VertexEndSupports_layer3",rmin,rmax,dz,phi1,phi2);
    // Volume: VertexEndSupports_layer3_volume
    TGeoVolume *pVertexEndSupports_layer3_volume_a45da18 = new TGeoVolume("VertexEndSupports_layer3_volume",pVertexEndSupports_layer3_41, pMed2);
    pVertexEndSupports_layer3_volume_a45da18->SetLineColor(23);
    pTARGETV_a404f40->AddNode(pVertexEndSupports_layer3_volume_a45da18, 569, pMatrix95);
    pTARGETV_a404f40->AddNode(pVertexEndSupports_layer3_volume_a45da18, 570, pMatrix96);
    // Shape: VertexEndSupports_layer4 type: TGeoTubeSeg
    rmin = 2.000000*Scale;
    rmax = 11.95300;
    dz   = 0.003800;
    phi1 = 0.000000;
    phi2 = 360.000000;
    TGeoShape *pVertexEndSupports_layer4_42 = new TGeoTubeSeg("VertexEndSupports_layer4",rmin,rmax,dz,phi1,phi2);
    // Volume: VertexEndSupports_layer4_volume
    TGeoVolume *pVertexEndSupports_layer4_volume_a45dd58 = new TGeoVolume("VertexEndSupports_layer4_volume",pVertexEndSupports_layer4_42, pMed2);
    pVertexEndSupports_layer4_volume_a45dd58->SetLineColor(23);
    pTARGETV_a404f40->AddNode(pVertexEndSupports_layer4_volume_a45dd58, 571, pMatrix98);
    pTARGETV_a404f40->AddNode(pVertexEndSupports_layer4_volume_a45dd58, 572, pMatrix99);
   
   
    // Shape: VertexEndSupports_layer5 type: TGeoTubeSeg
    rmin = 2.000000*Scale;
    rmax = 11.953000;
    dz   = 0.003800;
    phi1 = 0.000000;
    phi2 = 360.000000;
    TGeoShape *pVertexEndSupports_layer5_43 = new TGeoTubeSeg("VertexEndSupports_layer5",rmin,rmax,dz,phi1,phi2);
    // Volume: VertexEndSupports_layer5_volume
    TGeoVolume *pVertexEndSupports_layer5_volume_a45e098 = new TGeoVolume("VertexEndSupports_layer5_volume",pVertexEndSupports_layer5_43, pMed2);
    pVertexEndSupports_layer5_volume_a45e098->SetLineColor(23);
    pTARGETV_a404f40->AddNode(pVertexEndSupports_layer5_volume_a45e098, 573, pMatrix101);
    pTARGETV_a404f40->AddNode(pVertexEndSupports_layer5_volume_a45e098, 574, pMatrix102);
   
//------------>Supporti piu' esterni sulla Beam Pipe
   
    // Shape: VertexEndSupports_layer6 type: TGeoTubeSeg
    rmin = 5.500000;
    rmax = 11.953000;
    dz   = 0.021350;
    phi1 = 0.000000;
    phi2 = 360.000000;
    TGeoShape *pVertexEndSupports_layer6_44 = new TGeoTubeSeg("VertexEndSupports_layer6",rmin,rmax,dz,phi1,phi2);
    // Volume: VertexEndSupports_layer6_volume
    TGeoVolume *pVertexEndSupports_layer6_volume_a45e3d8 = new TGeoVolume("VertexEndSupports_layer6_volume",pVertexEndSupports_layer6_44, pMed2);
    pVertexEndSupports_layer6_volume_a45e3d8->SetLineColor(23);
    pTARGETV_a404f40->AddNode(pVertexEndSupports_layer6_volume_a45e3d8, 575, pMatrix104);
    pTARGETV_a404f40->AddNode(pVertexEndSupports_layer6_volume_a45e3d8, 576, pMatrix105);
    // Shape: VertexEndSupports_layer7 type: TGeoTubeSeg
    rmin = 5.500000;
//     rmax = 11.953000;
// // // //     rmax = 11.953000*Scale;
    rmax = 8.100000*Scale;
    dz   = 0.021350;
    phi1 = 0.000000;
    phi2 = 360.000000;
    TGeoShape *pVertexEndSupports_layer7_45 = new TGeoTubeSeg("VertexEndSupports_layer7",rmin,rmax,dz,phi1,phi2);
    // Volume: VertexEndSupports_layer7_volume
    TGeoVolume *pVertexEndSupports_layer7_volume_a45e718 = new TGeoVolume("VertexEndSupports_layer7_volume",pVertexEndSupports_layer7_45, pMed2);
    pVertexEndSupports_layer7_volume_a45e718->SetLineColor(23);
    pTARGETV_a404f40->AddNode(pVertexEndSupports_layer7_volume_a45e718, 577, pMatrix107);
    pTARGETV_a404f40->AddNode(pVertexEndSupports_layer7_volume_a45e718, 578, pMatrix108);
//------------>Fine Supporti piu' esterni sulla Beam Pipe  
   
// ------>  Involucro cilindrico
    // Shape: VertexBarrelSupports_layer5_slice0 type: TGeoTubeSeg
//     rmin = 11.954000;//*Scale;
//     rmax = 11.996700;//*Scale;
// // // //     rmin = 11.954000*Scale;
// // // //     rmax = 11.996700*Scale;
    rmin = 8.100000*Scale;
    rmax = 8.140000*Scale;
    dz   = 100.000000;
    phi1 = 0.000000;
    phi2 = 360.000000;
    TGeoShape *pVertexBarrelSupports_layer5_slice0_46 = new TGeoTubeSeg("VertexBarrelSupports_layer5_slice0",rmin,rmax,dz,phi1,phi2);
    // Volume: VertexBarrelSupports_layer5_slice0_volume
    TGeoVolume *pVertexBarrelSupports_layer5_slice0_volume_a40bfd0 = new TGeoVolume("VertexBarrelSupports_layer5_slice0_volume",pVertexBarrelSupports_layer5_slice0_46, pMed5);
    pVertexBarrelSupports_layer5_slice0_volume_a40bfd0->SetLineColor(16);
    pVertexBarrelSupports_layer5_volume_a40be28->AddNode(pVertexBarrelSupports_layer5_slice0_volume_a40bfd0, 289);
// ------>  Fine Involucro cilindrico

//-------------------------------- Layer  (anelli verdi)----------------------------------------   

    // Shape: VertexReadout_layer0_slice0 type: TGeoTubeSeg
    rmin = 1.460000*Scale;
    rmax = 1.660000*Scale;
    dz   = 0.249000;
    phi1 = 0.000000;
    phi2 = 360.000000;
    TGeoShape *pVertexReadout_layer0_slice0_47 = new TGeoTubeSeg("VertexReadout_layer0_slice0",rmin,rmax,dz,phi1,phi2);
    // Volume: VertexReadout_layer0_slice0_volume
    TGeoVolume *pVertexReadout_layer0_slice0_volume_a40c610 = new TGeoVolume("VertexReadout_layer0_slice0_volume",pVertexReadout_layer0_slice0_47, pMed6);
    pVertexReadout_layer0_slice0_volume_a40c610->SetLineColor(123);
    pVertexReadout_layer0_volume_a40c478->AddNode(pVertexReadout_layer0_slice0_volume_a40c610, 290);
    // Shape: VertexReadout_layer1_slice0 type: TGeoTubeSeg
    rmin = 2.260000*Scale;
    rmax = 2.460000*Scale;
    dz   = 0.249000;
    phi1 = 0.000000;
    phi2 = 360.000000;
    TGeoShape *pVertexReadout_layer1_slice0_48 = new TGeoTubeSeg("VertexReadout_layer1_slice0",rmin,rmax,dz,phi1,phi2);
    // Volume: VertexReadout_layer1_slice0_volume
    TGeoVolume *pVertexReadout_layer1_slice0_volume_a40cb50 = new TGeoVolume("VertexReadout_layer1_slice0_volume",pVertexReadout_layer1_slice0_48, pMed6);
    pVertexReadout_layer1_slice0_volume_a40cb50->SetLineColor(123);
    pVertexReadout_layer1_volume_a40c9b8->AddNode(pVertexReadout_layer1_slice0_volume_a40cb50, 291);
    // Shape: VertexReadout_layer2_slice0 type: TGeoTubeSeg
    rmin = 3.540000*Scale;
    rmax = 3.740000*Scale;
    dz   = 0.249000;
    phi1 = 0.000000;
    phi2 = 360.000000;
    TGeoShape *pVertexReadout_layer2_slice0_49 = new TGeoTubeSeg("VertexReadout_layer2_slice0",rmin,rmax,dz,phi1,phi2);
    // Volume: VertexReadout_layer2_slice0_volume
    TGeoVolume *pVertexReadout_layer2_slice0_volume_a40ce80 = new TGeoVolume("VertexReadout_layer2_slice0_volume",pVertexReadout_layer2_slice0_49, pMed6);
    pVertexReadout_layer2_slice0_volume_a40ce80->SetLineColor(123);
    pVertexReadout_layer2_volume_a40cce8->AddNode(pVertexReadout_layer2_slice0_volume_a40ce80, 292);
    // Shape: VertexReadout_layer3_slice0 type: TGeoTubeSeg
    rmin = 4.800000*Scale;
    rmax = 5.000000*Scale;
    dz   = 0.249000;
    phi1 = 0.000000;
    phi2 = 360.000000;
    TGeoShape *pVertexReadout_layer3_slice0_50 = new TGeoTubeSeg("VertexReadout_layer3_slice0",rmin,rmax,dz,phi1,phi2);
    // Volume: VertexReadout_layer3_slice0_volume
    TGeoVolume *pVertexReadout_layer3_slice0_volume_a40d1b0 = new TGeoVolume("VertexReadout_layer3_slice0_volume",pVertexReadout_layer3_slice0_50, pMed6);
    pVertexReadout_layer3_slice0_volume_a40d1b0->SetLineColor(123);
    pVertexReadout_layer3_volume_a40d018->AddNode(pVertexReadout_layer3_slice0_volume_a40d1b0, 293);
    // Shape: VertexReadout_layer4_slice0 type: TGeoTubeSeg
    rmin = 6.040000*Scale;
    rmax = 6.240000*Scale;
    dz   = 0.249000;
    phi1 = 0.000000;
    phi2 = 360.000000;
    TGeoShape *pVertexReadout_layer4_slice0_51 = new TGeoTubeSeg("VertexReadout_layer4_slice0",rmin,rmax,dz,phi1,phi2);
    // Volume: VertexReadout_layer4_slice0_volume
    TGeoVolume *pVertexReadout_layer4_slice0_volume_a40d4e0 = new TGeoVolume("VertexReadout_layer4_slice0_volume",pVertexReadout_layer4_slice0_51, pMed6);
    pVertexReadout_layer4_slice0_volume_a40d4e0->SetLineColor(123);
    pVertexReadout_layer4_volume_a40d348->AddNode(pVertexReadout_layer4_slice0_volume_a40d4e0, 294);
    // Shape: VertexReadout_layer5_slice0 type: TGeoTubeSeg
    rmin = 1.351000*Scale;//*Scale;
    rmax = 2.260000*Scale;
    dz   = 0.002850;
    phi1 = 0.000000;
    phi2 = 360.000000;
    TGeoShape *pVertexReadout_layer5_slice0_52 = new TGeoTubeSeg("VertexReadout_layer5_slice0",rmin,rmax,dz,phi1,phi2);
    // Volume: VertexReadout_layer5_slice0_volume
    TGeoVolume *pVertexReadout_layer5_slice0_volume_a40d810 = new TGeoVolume("VertexReadout_layer5_slice0_volume",pVertexReadout_layer5_slice0_52, pMed3);
    pVertexReadout_layer5_slice0_volume_a40d810->SetLineColor(42);
    pVertexReadout_layer5_volume_a40d678->AddNode(pVertexReadout_layer5_slice0_volume_a40d810, 295);
    // Shape: VertexReadout_layer6_slice0 type: TGeoTubeSeg
    rmin = 2.261000*Scale;
    rmax = 3.540000*Scale;
    dz   = 0.001550;
    phi1 = 0.000000;
    phi2 = 360.000000;
    TGeoShape *pVertexReadout_layer6_slice0_53 = new TGeoTubeSeg("VertexReadout_layer6_slice0",rmin,rmax,dz,phi1,phi2);
    // Volume: VertexReadout_layer6_slice0_volume
    TGeoVolume *pVertexReadout_layer6_slice0_volume_a40db40 = new TGeoVolume("VertexReadout_layer6_slice0_volume",pVertexReadout_layer6_slice0_53, pMed3);
    pVertexReadout_layer6_slice0_volume_a40db40->SetLineColor(42);
    pVertexReadout_layer6_volume_a40d9a8->AddNode(pVertexReadout_layer6_slice0_volume_a40db40, 296);
    // Shape: VertexReadout_layer7_slice0 type: TGeoTubeSeg
    rmin = 3.541000*Scale;
    rmax = 4.800000*Scale;
    dz   = 0.000800;
    phi1 = 0.000000;
    phi2 = 360.000000;
    TGeoShape *pVertexReadout_layer7_slice0_54 = new TGeoTubeSeg("VertexReadout_layer7_slice0",rmin,rmax,dz,phi1,phi2);
    // Volume: VertexReadout_layer7_slice0_volume
    TGeoVolume *pVertexReadout_layer7_slice0_volume_a40de70 = new TGeoVolume("VertexReadout_layer7_slice0_volume",pVertexReadout_layer7_slice0_54, pMed3);
    pVertexReadout_layer7_slice0_volume_a40de70->SetLineColor(42);
    pVertexReadout_layer7_volume_a40dcd8->AddNode(pVertexReadout_layer7_slice0_volume_a40de70, 297);
    // Shape: VertexReadout_layer8_slice0 type: TGeoTubeSeg
    rmin = 4.801000*Scale;
    rmax = 6.040000*Scale;
    dz   = 0.000350;
    phi1 = 0.000000;
    phi2 = 360.000000;
    TGeoShape *pVertexReadout_layer8_slice0_55 = new TGeoTubeSeg("VertexReadout_layer8_slice0",rmin,rmax,dz,phi1,phi2);
    // Volume: VertexReadout_layer8_slice0_volume
    TGeoVolume *pVertexReadout_layer8_slice0_volume_a40e1a0 = new TGeoVolume("VertexReadout_layer8_slice0_volume",pVertexReadout_layer8_slice0_55, pMed3);
    pVertexReadout_layer8_slice0_volume_a40e1a0->SetLineColor(42);
    pVertexReadout_layer8_volume_a40e008->AddNode(pVertexReadout_layer8_slice0_volume_a40e1a0, 298);
    // Shape: VertexReadout_layer9_slice0 type: TGeoTubeSeg
    rmin = 1.500000*Scale;
    rmax = 1.599000*Scale;
    dz   = 0.100000;
    phi1 = 0.000000;
    phi2 = 360.000000;
    TGeoShape *pVertexReadout_layer9_slice0_56 = new TGeoTubeSeg("VertexReadout_layer9_slice0",rmin,rmax,dz,phi1,phi2);
    // Volume: VertexReadout_layer9_slice0_volume
    TGeoVolume *pVertexReadout_layer9_slice0_volume_a40e4d0 = new TGeoVolume("VertexReadout_layer9_slice0_volume",pVertexReadout_layer9_slice0_56, pMed6);
    pVertexReadout_layer9_slice0_volume_a40e4d0->SetLineColor(123);
    pVertexReadout_layer9_volume_a40e338->AddNode(pVertexReadout_layer9_slice0_volume_a40e4d0, 299);
    // Shape: VertexReadout_layer10_slice0 type: TGeoTubeSeg
    rmin = 1.500000*Scale;
    rmax = 1.599000*Scale;
    dz   = 0.100000;
    phi1 = 0.000000;
    phi2 = 360.000000;
    TGeoShape *pVertexReadout_layer10_slice0_57 = new TGeoTubeSeg("VertexReadout_layer10_slice0",rmin,rmax,dz,phi1,phi2);
    // Volume: VertexReadout_layer10_slice0_volume
    TGeoVolume *pVertexReadout_layer10_slice0_volume_a40e808 = new TGeoVolume("VertexReadout_layer10_slice0_volume",pVertexReadout_layer10_slice0_57, pMed6);
    pVertexReadout_layer10_slice0_volume_a40e808->SetLineColor(123);
    pVertexReadout_layer10_volume_a40e668->AddNode(pVertexReadout_layer10_slice0_volume_a40e808, 300);
    // Shape: VertexReadout_layer11_slice0 type: TGeoTubeSeg
    rmin = 1.900000*Scale;
    rmax = 1.999000*Scale;
    dz   = 0.100000;
    phi1 = 0.000000;
    phi2 = 360.000000;
    TGeoShape *pVertexReadout_layer11_slice0_58 = new TGeoTubeSeg("VertexReadout_layer11_slice0",rmin,rmax,dz,phi1,phi2);
    // Volume: VertexReadout_layer11_slice0_volume
    TGeoVolume *pVertexReadout_layer11_slice0_volume_a40eb40 = new TGeoVolume("VertexReadout_layer11_slice0_volume",pVertexReadout_layer11_slice0_58, pMed6);
    pVertexReadout_layer11_slice0_volume_a40eb40->SetLineColor(123);
    pVertexReadout_layer11_volume_a40e9a0->AddNode(pVertexReadout_layer11_slice0_volume_a40eb40, 301);
    // Shape: VertexReadout_layer12_slice0 type: TGeoTubeSeg
    rmin = 1.900000*Scale;
    rmax = 1.999000*Scale;
    dz   = 0.100000;
    phi1 = 0.000000;
    phi2 = 360.000000;
    TGeoShape *pVertexReadout_layer12_slice0_59 = new TGeoTubeSeg("VertexReadout_layer12_slice0",rmin,rmax,dz,phi1,phi2);
    // Volume: VertexReadout_layer12_slice0_volume
    TGeoVolume *pVertexReadout_layer12_slice0_volume_a40ee78 = new TGeoVolume("VertexReadout_layer12_slice0_volume",pVertexReadout_layer12_slice0_59, pMed6);
    pVertexReadout_layer12_slice0_volume_a40ee78->SetLineColor(123);
    pVertexReadout_layer12_volume_a40ecd8->AddNode(pVertexReadout_layer12_slice0_volume_a40ee78, 302);
    // Shape: VertexReadout_layer13_slice0 type: TGeoTubeSeg
    rmin = 7.501000*Scale;
    rmax = 8.000000*Scale;
    dz   = 0.100000;
    phi1 = 0.000000;
    phi2 = 360.000000;
    TGeoShape *pVertexReadout_layer13_slice0_60 = new TGeoTubeSeg("VertexReadout_layer13_slice0",rmin,rmax,dz,phi1,phi2);
    // Volume: VertexReadout_layer13_slice0_volume
    TGeoVolume *pVertexReadout_layer13_slice0_volume_a40f1b0 = new TGeoVolume("VertexReadout_layer13_slice0_volume",pVertexReadout_layer13_slice0_60, pMed6);
    pVertexReadout_layer13_slice0_volume_a40f1b0->SetLineColor(123);
    pVertexReadout_layer13_volume_a40f010->AddNode(pVertexReadout_layer13_slice0_volume_a40f1b0, 303);
    // Shape: VertexReadout_layer14_slice0 type: TGeoTubeSeg
    rmin = 7.501000*Scale;
    rmax = 8.000000*Scale;
    dz   = 0.100000;
    phi1 = 0.000000;
    phi2 = 360.000000;
    TGeoShape *pVertexReadout_layer14_slice0_61 = new TGeoTubeSeg("VertexReadout_layer14_slice0",rmin,rmax,dz,phi1,phi2);
    // Volume: VertexReadout_layer14_slice0_volume
    TGeoVolume *pVertexReadout_layer14_slice0_volume_a40f4e8 = new TGeoVolume("VertexReadout_layer14_slice0_volume",pVertexReadout_layer14_slice0_61, pMed6);
    pVertexReadout_layer14_slice0_volume_a40f4e8->SetLineColor(123);
    pVertexReadout_layer14_volume_a40f348->AddNode(pVertexReadout_layer14_slice0_volume_a40f4e8, 304);
    // Shape: VertexReadout_layer15_slice0 type: TGeoTubeSeg
    rmin = 7.501000*Scale;
    rmax = 8.000000*Scale;
    dz   = 0.100000;
    phi1 = 0.000000;
    phi2 = 360.000000;
    TGeoShape *pVertexReadout_layer15_slice0_62 = new TGeoTubeSeg("VertexReadout_layer15_slice0",rmin,rmax,dz,phi1,phi2);
    // Volume: VertexReadout_layer15_slice0_volume
    TGeoVolume *pVertexReadout_layer15_slice0_volume_a40f820 = new TGeoVolume("VertexReadout_layer15_slice0_volume",pVertexReadout_layer15_slice0_62, pMed6);
    pVertexReadout_layer15_slice0_volume_a40f820->SetLineColor(123);
    pVertexReadout_layer15_volume_a40f680->AddNode(pVertexReadout_layer15_slice0_volume_a40f820, 305);
     // Shape: VertexReadout_layer16_slice0 type: TGeoTubeSeg
    rmin = 7.501000*Scale;
    rmax = 8.000000*Scale;
    dz   = 0.100000;
    phi1 = 0.000000;
    phi2 = 360.000000;
    TGeoShape *pVertexReadout_layer16_slice0_63 = new TGeoTubeSeg("VertexReadout_layer16_slice0",rmin,rmax,dz,phi1,phi2);
    // Volume: VertexReadout_layer16_slice0_volume
    TGeoVolume *pVertexReadout_layer16_slice0_volume_a40fb58 = new TGeoVolume("VertexReadout_layer16_slice0_volume",pVertexReadout_layer16_slice0_63, pMed6);
    pVertexReadout_layer16_slice0_volume_a40fb58->SetLineColor(123);
    pVertexReadout_layer16_volume_a40f9b8->AddNode(pVertexReadout_layer16_slice0_volume_a40fb58, 306);

//-------------------------------- Fine Layer  (anelli verdi)----------------------------------------
    
//-------------------------------- Ladder EndCap (trapezoidali Gialli)----------------------------------------    
    
    // Shape: VertexEndcap_ladderA type: TGeoTrd2
    dx1 = 1.941143*Scale;
    dx2 = 0.414110*Scale;
    dy1 = 0.005650;
    dy2 = 0.005650;
    dz  = 2.822222*Scale;
    
    TGeoShape *pVertexEndcap_ladderA_64 = new TGeoTrd2("VertexEndcap_ladderA", dx1,dx2,dy1,dy2,dz);
   
    // Volume: VertexEndcap_layer0_ladder0_volume
    TGeoVolume *pVertexEndcap_layer0_ladder0_volume_a410140 = new     TGeoVolume("VertexEndcap_layer0_ladder0_volume",pVertexEndcap_ladderA_64, pMed8);
    pVertexEndcap_layer0_ladder0_volume_a410140->SetLineColor(130);
    pVertexEndcap_layer0_volume_a40ffa8->AddNode(pVertexEndcap_layer0_ladder0_volume_a410140, 319, pMatrix128);
   
    // Volume: VertexEndcap_layer0_ladder1_volume
    TGeoVolume *pVertexEndcap_layer0_ladder1_volume_a412748 = new TGeoVolume("VertexEndcap_layer0_ladder1_volume",pVertexEndcap_ladderA_64, pMed8);
    pVertexEndcap_layer0_ladder1_volume_a412748->SetLineColor(130);
    pVertexEndcap_layer0_volume_a40ffa8->AddNode(pVertexEndcap_layer0_ladder1_volume_a412748, 320, pMatrix130);
   
    // Volume: VertexEndcap_layer0_ladder2_volume
    TGeoVolume *pVertexEndcap_layer0_ladder2_volume_a412aa0 = new TGeoVolume("VertexEndcap_layer0_ladder2_volume",pVertexEndcap_ladderA_64, pMed8);
    pVertexEndcap_layer0_ladder2_volume_a412aa0->SetLineColor(130);
    pVertexEndcap_layer0_volume_a40ffa8->AddNode(pVertexEndcap_layer0_ladder2_volume_a412aa0, 321, pMatrix132);
   
    // Volume: VertexEndcap_layer0_ladder3_volume
    TGeoVolume *pVertexEndcap_layer0_ladder3_volume_a412e10 = new TGeoVolume("VertexEndcap_layer0_ladder3_volume",pVertexEndcap_ladderA_64, pMed8);
    pVertexEndcap_layer0_ladder3_volume_a412e10->SetLineColor(130);
    pVertexEndcap_layer0_volume_a40ffa8->AddNode(pVertexEndcap_layer0_ladder3_volume_a412e10, 322, pMatrix134);
   
    // Volume: VertexEndcap_layer0_ladder4_volume
    TGeoVolume *pVertexEndcap_layer0_ladder4_volume_a413180 = new TGeoVolume("VertexEndcap_layer0_ladder4_volume",pVertexEndcap_ladderA_64, pMed8);
    pVertexEndcap_layer0_ladder4_volume_a413180->SetLineColor(130);
    pVertexEndcap_layer0_volume_a40ffa8->AddNode(pVertexEndcap_layer0_ladder4_volume_a413180, 323, pMatrix136);
   
    // Volume: VertexEndcap_layer0_ladder5_volume
    TGeoVolume *pVertexEndcap_layer0_ladder5_volume_a4134f0 = new TGeoVolume("VertexEndcap_layer0_ladder5_volume",pVertexEndcap_ladderA_64, pMed8);
    pVertexEndcap_layer0_ladder5_volume_a4134f0->SetLineColor(130);
    pVertexEndcap_layer0_volume_a40ffa8->AddNode(pVertexEndcap_layer0_ladder5_volume_a4134f0, 324, pMatrix138);
   
    // Volume: VertexEndcap_layer0_ladder6_volume
    TGeoVolume *pVertexEndcap_layer0_ladder6_volume_a413860 = new TGeoVolume("VertexEndcap_layer0_ladder6_volume",pVertexEndcap_ladderA_64, pMed8);
    pVertexEndcap_layer0_ladder6_volume_a413860->SetLineColor(130);
    pVertexEndcap_layer0_volume_a40ffa8->AddNode(pVertexEndcap_layer0_ladder6_volume_a413860, 325, pMatrix140);
   
    // Volume: VertexEndcap_layer0_ladder7_volume
    TGeoVolume *pVertexEndcap_layer0_ladder7_volume_a413bd0 = new TGeoVolume("VertexEndcap_layer0_ladder7_volume",pVertexEndcap_ladderA_64, pMed8);
    pVertexEndcap_layer0_ladder7_volume_a413bd0->SetLineColor(130);
    pVertexEndcap_layer0_volume_a40ffa8->AddNode(pVertexEndcap_layer0_ladder7_volume_a413bd0, 326, pMatrix142);
   
    // Volume: VertexEndcap_layer0_ladder8_volume
    TGeoVolume *pVertexEndcap_layer0_ladder8_volume_a413f40 = new TGeoVolume("VertexEndcap_layer0_ladder8_volume",pVertexEndcap_ladderA_64, pMed8);
    pVertexEndcap_layer0_ladder8_volume_a413f40->SetLineColor(130);
    pVertexEndcap_layer0_volume_a40ffa8->AddNode(pVertexEndcap_layer0_ladder8_volume_a413f40, 327, pMatrix144);
   
    // Volume: VertexEndcap_layer0_ladder9_volume
    TGeoVolume *pVertexEndcap_layer0_ladder9_volume_a4142b0 = new TGeoVolume("VertexEndcap_layer0_ladder9_volume",pVertexEndcap_ladderA_64, pMed8);
    pVertexEndcap_layer0_ladder9_volume_a4142b0->SetLineColor(130);
    pVertexEndcap_layer0_volume_a40ffa8->AddNode(pVertexEndcap_layer0_ladder9_volume_a4142b0, 328, pMatrix146);
   
    // Volume: VertexEndcap_layer0_ladder10_volume
    TGeoVolume *pVertexEndcap_layer0_ladder10_volume_a414620 = new TGeoVolume("VertexEndcap_layer0_ladder10_volume",pVertexEndcap_ladderA_64, pMed8);
    pVertexEndcap_layer0_ladder10_volume_a414620->SetLineColor(130);
    pVertexEndcap_layer0_volume_a40ffa8->AddNode(pVertexEndcap_layer0_ladder10_volume_a414620, 329, pMatrix148);
   
    // Volume: VertexEndcap_layer0_ladder11_volume
    TGeoVolume *pVertexEndcap_layer0_ladder11_volume_a414990 = new TGeoVolume("VertexEndcap_layer0_ladder11_volume",pVertexEndcap_ladderA_64, pMed8);
    pVertexEndcap_layer0_ladder11_volume_a414990->SetLineColor(130);
    pVertexEndcap_layer0_volume_a40ffa8->AddNode(pVertexEndcap_layer0_ladder11_volume_a414990, 330, pMatrix150);


    // Shape: VertexEndcap_detA type: TGeoTrd2
    dx1 = (1.941093*Scale);
    dx2 = (0.414060*Scale);//+0.0005;
    dy1 = 0.005650;
    dy2 = 0.005650;
    dz  = (2.822172*Scale);//+0.00035;
    TGeoShape *pVertexEndcap_detA_65 = new TGeoTrd2("VertexEndcap_detA", dx1,dx2,dy1,dy2,dz);

    // Volume: VertexEndcap_detA_volume
    TGeoVolume *pVertexEndcap_detA_volume_a4102d8 = new TGeoVolume("VertexEndcap_detA_volume",pVertexEndcap_detA_65, pMed8);
    pVertexEndcap_detA_volume_a4102d8->SetLineColor(130);

    pVertexEndcap_layer0_ladder0_volume_a410140->AddNode(pVertexEndcap_detA_volume_a4102d8, 307);
    pVertexEndcap_layer0_ladder1_volume_a412748->AddNode(pVertexEndcap_detA_volume_a4102d8, 308);
    pVertexEndcap_layer0_ladder2_volume_a412aa0->AddNode(pVertexEndcap_detA_volume_a4102d8, 309);
    pVertexEndcap_layer0_ladder3_volume_a412e10->AddNode(pVertexEndcap_detA_volume_a4102d8, 310);
    pVertexEndcap_layer0_ladder4_volume_a413180->AddNode(pVertexEndcap_detA_volume_a4102d8, 311);
    pVertexEndcap_layer0_ladder5_volume_a4134f0->AddNode(pVertexEndcap_detA_volume_a4102d8, 312);
    pVertexEndcap_layer0_ladder6_volume_a413860->AddNode(pVertexEndcap_detA_volume_a4102d8, 313);
    pVertexEndcap_layer0_ladder7_volume_a413bd0->AddNode(pVertexEndcap_detA_volume_a4102d8, 314);
    pVertexEndcap_layer0_ladder8_volume_a413f40->AddNode(pVertexEndcap_detA_volume_a4102d8, 315);
    pVertexEndcap_layer0_ladder9_volume_a4142b0->AddNode(pVertexEndcap_detA_volume_a4102d8, 316);
    pVertexEndcap_layer0_ladder10_volume_a414620->AddNode(pVertexEndcap_detA_volume_a4102d8, 317);
    pVertexEndcap_layer0_ladder11_volume_a414990->AddNode(pVertexEndcap_detA_volume_a4102d8, 318);

    
    
    // Shape: VertexEndcap_ladderB type: TGeoTrd2
    dx1 = 1.941143*Scale;
    dx2 = 0.414110*Scale;
    dy1 = 0.005650;
    dy2 = 0.005650;
    dz  = 2.822222*Scale;
    
    TGeoShape *pVertexEndcap_ladderB_66 = new TGeoTrd2("VertexEndcap_ladderB", dx1,dx2,dy1,dy2,dz);

    // Volume: VertexEndcap_layer1_ladder0_volume
    TGeoVolume *pVertexEndcap_layer1_ladder0_volume_a4156e0 = new TGeoVolume("VertexEndcap_layer1_ladder0_volume",pVertexEndcap_ladderB_66, pMed8);
    pVertexEndcap_layer1_ladder0_volume_a4156e0->SetLineColor(130);
    pVertexEndcap_layer1_volume_a415548->AddNode(pVertexEndcap_layer1_ladder0_volume_a4156e0, 343, pMatrix164);

    // Volume: VertexEndcap_layer1_ladder1_volume
    TGeoVolume *pVertexEndcap_layer1_ladder1_volume_a415a20 = new TGeoVolume("VertexEndcap_layer1_ladder1_volume",pVertexEndcap_ladderB_66, pMed8);
    pVertexEndcap_layer1_ladder1_volume_a415a20->SetLineColor(130);
    pVertexEndcap_layer1_volume_a415548->AddNode(pVertexEndcap_layer1_ladder1_volume_a415a20, 344, pMatrix166);

    // Volume: VertexEndcap_layer1_ladder2_volume
    TGeoVolume *pVertexEndcap_layer1_ladder2_volume_a415d60 = new TGeoVolume("VertexEndcap_layer1_ladder2_volume",pVertexEndcap_ladderB_66, pMed8);
    pVertexEndcap_layer1_ladder2_volume_a415d60->SetLineColor(130);
    pVertexEndcap_layer1_volume_a415548->AddNode(pVertexEndcap_layer1_ladder2_volume_a415d60, 345, pMatrix168);

    // Volume: VertexEndcap_layer1_ladder3_volume
    TGeoVolume *pVertexEndcap_layer1_ladder3_volume_a4160d0 = new TGeoVolume("VertexEndcap_layer1_ladder3_volume",pVertexEndcap_ladderB_66, pMed8);
    pVertexEndcap_layer1_ladder3_volume_a4160d0->SetLineColor(130);
    pVertexEndcap_layer1_volume_a415548->AddNode(pVertexEndcap_layer1_ladder3_volume_a4160d0, 346, pMatrix170);

    // Volume: VertexEndcap_layer1_ladder4_volume
    TGeoVolume *pVertexEndcap_layer1_ladder4_volume_a416440 = new TGeoVolume("VertexEndcap_layer1_ladder4_volume",pVertexEndcap_ladderB_66, pMed8);
    pVertexEndcap_layer1_ladder4_volume_a416440->SetLineColor(130);
    pVertexEndcap_layer1_volume_a415548->AddNode(pVertexEndcap_layer1_ladder4_volume_a416440, 347, pMatrix172);

    // Volume: VertexEndcap_layer1_ladder5_volume
    TGeoVolume *pVertexEndcap_layer1_ladder5_volume_a4167b0 = new TGeoVolume("VertexEndcap_layer1_ladder5_volume",pVertexEndcap_ladderB_66, pMed8);
    pVertexEndcap_layer1_ladder5_volume_a4167b0->SetLineColor(130);
    pVertexEndcap_layer1_volume_a415548->AddNode(pVertexEndcap_layer1_ladder5_volume_a4167b0, 348, pMatrix174);

    // Volume: VertexEndcap_layer1_ladder6_volume
    TGeoVolume *pVertexEndcap_layer1_ladder6_volume_a416b20 = new TGeoVolume("VertexEndcap_layer1_ladder6_volume",pVertexEndcap_ladderB_66, pMed8);
    pVertexEndcap_layer1_ladder6_volume_a416b20->SetLineColor(130);
    pVertexEndcap_layer1_volume_a415548->AddNode(pVertexEndcap_layer1_ladder6_volume_a416b20, 349, pMatrix176);

    // Volume: VertexEndcap_layer1_ladder7_volume
    TGeoVolume *pVertexEndcap_layer1_ladder7_volume_a416e90 = new TGeoVolume("VertexEndcap_layer1_ladder7_volume",pVertexEndcap_ladderB_66, pMed8);
    pVertexEndcap_layer1_ladder7_volume_a416e90->SetLineColor(130);
    pVertexEndcap_layer1_volume_a415548->AddNode(pVertexEndcap_layer1_ladder7_volume_a416e90, 350, pMatrix178);

    // Volume: VertexEndcap_layer1_ladder8_volume
    TGeoVolume *pVertexEndcap_layer1_ladder8_volume_a417200 = new TGeoVolume("VertexEndcap_layer1_ladder8_volume",pVertexEndcap_ladderB_66, pMed8);
    pVertexEndcap_layer1_ladder8_volume_a417200->SetLineColor(130);
    pVertexEndcap_layer1_volume_a415548->AddNode(pVertexEndcap_layer1_ladder8_volume_a417200, 351, pMatrix180);

    // Volume: VertexEndcap_layer1_ladder9_volume
    TGeoVolume *pVertexEndcap_layer1_ladder9_volume_a417570 = new TGeoVolume("VertexEndcap_layer1_ladder9_volume",pVertexEndcap_ladderB_66, pMed8);
    pVertexEndcap_layer1_ladder9_volume_a417570->SetLineColor(130);
    pVertexEndcap_layer1_volume_a415548->AddNode(pVertexEndcap_layer1_ladder9_volume_a417570, 352, pMatrix182);

    // Volume: VertexEndcap_layer1_ladder10_volume
    TGeoVolume *pVertexEndcap_layer1_ladder10_volume_a4178e0 = new TGeoVolume("VertexEndcap_layer1_ladder10_volume",pVertexEndcap_ladderB_66, pMed8);
    pVertexEndcap_layer1_ladder10_volume_a4178e0->SetLineColor(130);
    pVertexEndcap_layer1_volume_a415548->AddNode(pVertexEndcap_layer1_ladder10_volume_a4178e0, 353, pMatrix184);

    // Volume: VertexEndcap_layer1_ladder11_volume
    TGeoVolume *pVertexEndcap_layer1_ladder11_volume_a417c50 = new TGeoVolume("VertexEndcap_layer1_ladder11_volume",pVertexEndcap_ladderB_66, pMed8);
    pVertexEndcap_layer1_ladder11_volume_a417c50->SetLineColor(130);
    pVertexEndcap_layer1_volume_a415548->AddNode(pVertexEndcap_layer1_ladder11_volume_a417c50, 354, pMatrix186);
    
    pVertexEndcap_layer1_ladder0_volume_a4156e0->AddNode(pVertexEndcap_detA_volume_a4102d8, 331);
    pVertexEndcap_layer1_ladder1_volume_a415a20->AddNode(pVertexEndcap_detA_volume_a4102d8, 332);
    pVertexEndcap_layer1_ladder2_volume_a415d60->AddNode(pVertexEndcap_detA_volume_a4102d8, 333);
    pVertexEndcap_layer1_ladder3_volume_a4160d0->AddNode(pVertexEndcap_detA_volume_a4102d8, 334);
    pVertexEndcap_layer1_ladder4_volume_a416440->AddNode(pVertexEndcap_detA_volume_a4102d8, 335);
    pVertexEndcap_layer1_ladder5_volume_a4167b0->AddNode(pVertexEndcap_detA_volume_a4102d8, 336);
    pVertexEndcap_layer1_ladder6_volume_a416b20->AddNode(pVertexEndcap_detA_volume_a4102d8, 337);
    pVertexEndcap_layer1_ladder7_volume_a416e90->AddNode(pVertexEndcap_detA_volume_a4102d8, 338);
    pVertexEndcap_layer1_ladder8_volume_a417200->AddNode(pVertexEndcap_detA_volume_a4102d8, 339);
    pVertexEndcap_layer1_ladder9_volume_a417570->AddNode(pVertexEndcap_detA_volume_a4102d8, 340);
    pVertexEndcap_layer1_ladder10_volume_a4178e0->AddNode(pVertexEndcap_detA_volume_a4102d8, 341);
    pVertexEndcap_layer1_ladder11_volume_a417c50->AddNode(pVertexEndcap_detA_volume_a4102d8, 342);
    
    // Shape: VertexEndcap_ladderC type: TGeoTrd2
    dx1 = 1.941143*Scale;
    dx2 = 0.517638*Scale;
    dy1 = 0.005650;
    dy2 = 0.005650;
    dz  = 2.622222*Scale;
    TGeoShape *pVertexEndcap_ladderC_67 = new TGeoTrd2("VertexEndcap_ladderC", dx1,dx2,dy1,dy2,dz);
    
    // Volume: VertexEndcap_layer2_ladder0_volume
    TGeoVolume *pVertexEndcap_layer2_ladder0_volume_a4189d8 = new TGeoVolume("VertexEndcap_layer2_ladder0_volume",pVertexEndcap_ladderC_67, pMed8);
    pVertexEndcap_layer2_ladder0_volume_a4189d8->SetLineColor(130);
    pVertexEndcap_layer2_volume_a418840->AddNode(pVertexEndcap_layer2_ladder0_volume_a4189d8, 367, pMatrix200);
    
    // Volume: VertexEndcap_layer2_ladder1_volume
    TGeoVolume *pVertexEndcap_layer2_ladder1_volume_a418de8 = new TGeoVolume("VertexEndcap_layer2_ladder1_volume",pVertexEndcap_ladderC_67, pMed8);
    pVertexEndcap_layer2_ladder1_volume_a418de8->SetLineColor(130);
    pVertexEndcap_layer2_volume_a418840->AddNode(pVertexEndcap_layer2_ladder1_volume_a418de8, 368, pMatrix202);
    
    // Volume: VertexEndcap_layer2_ladder2_volume
    TGeoVolume *pVertexEndcap_layer2_ladder2_volume_a419158 = new TGeoVolume("VertexEndcap_layer2_ladder2_volume",pVertexEndcap_ladderC_67, pMed8);
    pVertexEndcap_layer2_ladder2_volume_a419158->SetLineColor(130);
    pVertexEndcap_layer2_volume_a418840->AddNode(pVertexEndcap_layer2_ladder2_volume_a419158, 369, pMatrix204);
    
    // Volume: VertexEndcap_layer2_ladder3_volume
    TGeoVolume *pVertexEndcap_layer2_ladder3_volume_a4194c8 = new TGeoVolume("VertexEndcap_layer2_ladder3_volume",pVertexEndcap_ladderC_67, pMed8);
    pVertexEndcap_layer2_ladder3_volume_a4194c8->SetLineColor(130);
    pVertexEndcap_layer2_volume_a418840->AddNode(pVertexEndcap_layer2_ladder3_volume_a4194c8, 370, pMatrix206);
    
    // Volume: VertexEndcap_layer2_ladder4_volume
    TGeoVolume *pVertexEndcap_layer2_ladder4_volume_a419838 = new TGeoVolume("VertexEndcap_layer2_ladder4_volume",pVertexEndcap_ladderC_67, pMed8);
    pVertexEndcap_layer2_ladder4_volume_a419838->SetLineColor(130);
    pVertexEndcap_layer2_volume_a418840->AddNode(pVertexEndcap_layer2_ladder4_volume_a419838, 371, pMatrix208);
    
    // Volume: VertexEndcap_layer2_ladder5_volume
    TGeoVolume *pVertexEndcap_layer2_ladder5_volume_a419ba8 = new TGeoVolume("VertexEndcap_layer2_ladder5_volume",pVertexEndcap_ladderC_67, pMed8);
    pVertexEndcap_layer2_ladder5_volume_a419ba8->SetLineColor(130);
    pVertexEndcap_layer2_volume_a418840->AddNode(pVertexEndcap_layer2_ladder5_volume_a419ba8, 372, pMatrix210);
    
    // Volume: VertexEndcap_layer2_ladder6_volume
    TGeoVolume *pVertexEndcap_layer2_ladder6_volume_a419f18 = new TGeoVolume("VertexEndcap_layer2_ladder6_volume",pVertexEndcap_ladderC_67, pMed8);
    pVertexEndcap_layer2_ladder6_volume_a419f18->SetLineColor(130);
    pVertexEndcap_layer2_volume_a418840->AddNode(pVertexEndcap_layer2_ladder6_volume_a419f18, 373, pMatrix212);
    
    // Volume: VertexEndcap_layer2_ladder7_volume
    TGeoVolume *pVertexEndcap_layer2_ladder7_volume_a41a288 = new TGeoVolume("VertexEndcap_layer2_ladder7_volume",pVertexEndcap_ladderC_67, pMed8);
    pVertexEndcap_layer2_ladder7_volume_a41a288->SetLineColor(130);
    pVertexEndcap_layer2_volume_a418840->AddNode(pVertexEndcap_layer2_ladder7_volume_a41a288, 374, pMatrix214);
    
    // Volume: VertexEndcap_layer2_ladder8_volume
    TGeoVolume *pVertexEndcap_layer2_ladder8_volume_a41a5f8 = new TGeoVolume("VertexEndcap_layer2_ladder8_volume",pVertexEndcap_ladderC_67, pMed8);
    pVertexEndcap_layer2_ladder8_volume_a41a5f8->SetLineColor(130);
    pVertexEndcap_layer2_volume_a418840->AddNode(pVertexEndcap_layer2_ladder8_volume_a41a5f8, 375, pMatrix216);
    
    // Volume: VertexEndcap_layer2_ladder9_volume
    TGeoVolume *pVertexEndcap_layer2_ladder9_volume_a41a968 = new TGeoVolume("VertexEndcap_layer2_ladder9_volume",pVertexEndcap_ladderC_67, pMed8);
    pVertexEndcap_layer2_ladder9_volume_a41a968->SetLineColor(130);
    pVertexEndcap_layer2_volume_a418840->AddNode(pVertexEndcap_layer2_ladder9_volume_a41a968, 376, pMatrix218);
    
    // Volume: VertexEndcap_layer2_ladder10_volume
    TGeoVolume *pVertexEndcap_layer2_ladder10_volume_a41acd8 = new TGeoVolume("VertexEndcap_layer2_ladder10_volume",pVertexEndcap_ladderC_67, pMed8);
    pVertexEndcap_layer2_ladder10_volume_a41acd8->SetLineColor(130);
    pVertexEndcap_layer2_volume_a418840->AddNode(pVertexEndcap_layer2_ladder10_volume_a41acd8, 377, pMatrix220);
    
    // Volume: VertexEndcap_layer2_ladder11_volume
    TGeoVolume *pVertexEndcap_layer2_ladder11_volume_a41b048 = new TGeoVolume("VertexEndcap_layer2_ladder11_volume",pVertexEndcap_ladderC_67, pMed8);
    pVertexEndcap_layer2_ladder11_volume_a41b048->SetLineColor(130);
    pVertexEndcap_layer2_volume_a418840->AddNode(pVertexEndcap_layer2_ladder11_volume_a41b048, 378, pMatrix222);
    
    // Shape: VertexEndcap_detB type: TGeoTrd2
    dx1 = (1.941093*Scale);
    dx2 = (0.517588*Scale);//-0.0004;
    dy1 = 0.005650;
    dy2 = 0.005650;
    dz  = (2.622172*Scale);//+0.00005;
    TGeoShape *pVertexEndcap_detB_68 = new TGeoTrd2("VertexEndcap_detB", dx1,dx2,dy1,dy2,dz);
    
    // Volume: VertexEndcap_detB_volume
    TGeoVolume *pVertexEndcap_detB_volume_a418b70 = new TGeoVolume("VertexEndcap_detB_volume",pVertexEndcap_detB_68, pMed8);
    pVertexEndcap_detB_volume_a418b70->SetLineColor(130);
    pVertexEndcap_layer2_ladder0_volume_a4189d8->AddNode(pVertexEndcap_detB_volume_a418b70, 355);
    pVertexEndcap_layer2_ladder1_volume_a418de8->AddNode(pVertexEndcap_detB_volume_a418b70, 356);
    pVertexEndcap_layer2_ladder2_volume_a419158->AddNode(pVertexEndcap_detB_volume_a418b70, 357);
    pVertexEndcap_layer2_ladder3_volume_a4194c8->AddNode(pVertexEndcap_detB_volume_a418b70, 358);
    pVertexEndcap_layer2_ladder4_volume_a419838->AddNode(pVertexEndcap_detB_volume_a418b70, 359);
    pVertexEndcap_layer2_ladder5_volume_a419ba8->AddNode(pVertexEndcap_detB_volume_a418b70, 360);
    pVertexEndcap_layer2_ladder6_volume_a419f18->AddNode(pVertexEndcap_detB_volume_a418b70, 361);
    pVertexEndcap_layer2_ladder7_volume_a41a288->AddNode(pVertexEndcap_detB_volume_a418b70, 362);
    pVertexEndcap_layer2_ladder8_volume_a41a5f8->AddNode(pVertexEndcap_detB_volume_a418b70, 363);
    pVertexEndcap_layer2_ladder9_volume_a41a968->AddNode(pVertexEndcap_detB_volume_a418b70, 364);
    pVertexEndcap_layer2_ladder10_volume_a41acd8->AddNode(pVertexEndcap_detB_volume_a418b70, 365);
    pVertexEndcap_layer2_ladder11_volume_a41b048->AddNode(pVertexEndcap_detB_volume_a418b70, 366);
    
    // Shape: VertexEndcap_ladderD type: TGeoTrd2
    dx1 = 1.941143*Scale;
    dx2 = 0.517638*Scale;
    dy1 = 0.005650;
    dy2 = 0.005650;
    dz  = 2.622222*Scale;
    TGeoShape *pVertexEndcap_ladderD_69 = new TGeoTrd2("VertexEndcap_ladderD", dx1,dx2,dy1,dy2,dz);
    
    // Volume: VertexEndcap_layer3_ladder0_volume
    TGeoVolume *pVertexEndcap_layer3_ladder0_volume_a41be10 = new TGeoVolume("VertexEndcap_layer3_ladder0_volume",pVertexEndcap_ladderD_69, pMed8);
    pVertexEndcap_layer3_ladder0_volume_a41be10->SetLineColor(130);
    pVertexEndcap_layer3_volume_a41bc78->AddNode(pVertexEndcap_layer3_ladder0_volume_a41be10, 391, pMatrix236);
    
    // Volume: VertexEndcap_layer3_ladder1_volume
    TGeoVolume *pVertexEndcap_layer3_ladder1_volume_a41c180 = new TGeoVolume("VertexEndcap_layer3_ladder1_volume",pVertexEndcap_ladderD_69, pMed8);
    pVertexEndcap_layer3_ladder1_volume_a41c180->SetLineColor(130);
    pVertexEndcap_layer3_volume_a41bc78->AddNode(pVertexEndcap_layer3_ladder1_volume_a41c180, 392, pMatrix238);
    
    // Volume: VertexEndcap_layer3_ladder2_volume
    TGeoVolume *pVertexEndcap_layer3_ladder2_volume_a41c4f0 = new TGeoVolume("VertexEndcap_layer3_ladder2_volume",pVertexEndcap_ladderD_69, pMed8);
    pVertexEndcap_layer3_ladder2_volume_a41c4f0->SetLineColor(130);
    pVertexEndcap_layer3_volume_a41bc78->AddNode(pVertexEndcap_layer3_ladder2_volume_a41c4f0, 393, pMatrix240);
    
    // Volume: VertexEndcap_layer3_ladder3_volume
    TGeoVolume *pVertexEndcap_layer3_ladder3_volume_a41c860 = new TGeoVolume("VertexEndcap_layer3_ladder3_volume",pVertexEndcap_ladderD_69, pMed8);
    pVertexEndcap_layer3_ladder3_volume_a41c860->SetLineColor(130);
    pVertexEndcap_layer3_volume_a41bc78->AddNode(pVertexEndcap_layer3_ladder3_volume_a41c860, 394, pMatrix242);
    
    // Volume: VertexEndcap_layer3_ladder4_volume
    TGeoVolume *pVertexEndcap_layer3_ladder4_volume_a41cbd0 = new TGeoVolume("VertexEndcap_layer3_ladder4_volume",pVertexEndcap_ladderD_69, pMed8);
    pVertexEndcap_layer3_ladder4_volume_a41cbd0->SetLineColor(130);
    pVertexEndcap_layer3_volume_a41bc78->AddNode(pVertexEndcap_layer3_ladder4_volume_a41cbd0, 395, pMatrix244);
    
    // Volume: VertexEndcap_layer3_ladder5_volume
    TGeoVolume *pVertexEndcap_layer3_ladder5_volume_a41cf40 = new TGeoVolume("VertexEndcap_layer3_ladder5_volume",pVertexEndcap_ladderD_69, pMed8);
    pVertexEndcap_layer3_ladder5_volume_a41cf40->SetLineColor(130);
    pVertexEndcap_layer3_volume_a41bc78->AddNode(pVertexEndcap_layer3_ladder5_volume_a41cf40, 396, pMatrix246);
    
    // Volume: VertexEndcap_layer3_ladder6_volume
    TGeoVolume *pVertexEndcap_layer3_ladder6_volume_a41d2b0 = new TGeoVolume("VertexEndcap_layer3_ladder6_volume",pVertexEndcap_ladderD_69, pMed8);
    pVertexEndcap_layer3_ladder6_volume_a41d2b0->SetLineColor(130);
    pVertexEndcap_layer3_volume_a41bc78->AddNode(pVertexEndcap_layer3_ladder6_volume_a41d2b0, 397, pMatrix248);
    
    // Volume: VertexEndcap_layer3_ladder7_volume
    TGeoVolume *pVertexEndcap_layer3_ladder7_volume_a41d620 = new TGeoVolume("VertexEndcap_layer3_ladder7_volume",pVertexEndcap_ladderD_69, pMed8);
    pVertexEndcap_layer3_ladder7_volume_a41d620->SetLineColor(130);
    pVertexEndcap_layer3_volume_a41bc78->AddNode(pVertexEndcap_layer3_ladder7_volume_a41d620, 398, pMatrix250);
    
    // Volume: VertexEndcap_layer3_ladder8_volume
    TGeoVolume *pVertexEndcap_layer3_ladder8_volume_a41d990 = new TGeoVolume("VertexEndcap_layer3_ladder8_volume",pVertexEndcap_ladderD_69, pMed8);
    pVertexEndcap_layer3_ladder8_volume_a41d990->SetLineColor(130);
    pVertexEndcap_layer3_volume_a41bc78->AddNode(pVertexEndcap_layer3_ladder8_volume_a41d990, 399, pMatrix252);
    
    // Volume: VertexEndcap_layer3_ladder9_volume
    TGeoVolume *pVertexEndcap_layer3_ladder9_volume_a41dd00 = new TGeoVolume("VertexEndcap_layer3_ladder9_volume",pVertexEndcap_ladderD_69, pMed8);
    pVertexEndcap_layer3_ladder9_volume_a41dd00->SetLineColor(130);
    pVertexEndcap_layer3_volume_a41bc78->AddNode(pVertexEndcap_layer3_ladder9_volume_a41dd00, 400, pMatrix254);
    
    // Volume: VertexEndcap_layer3_ladder10_volume
    TGeoVolume *pVertexEndcap_layer3_ladder10_volume_a41e070 = new TGeoVolume("VertexEndcap_layer3_ladder10_volume",pVertexEndcap_ladderD_69, pMed8);
    pVertexEndcap_layer3_ladder10_volume_a41e070->SetLineColor(130);
    pVertexEndcap_layer3_volume_a41bc78->AddNode(pVertexEndcap_layer3_ladder10_volume_a41e070, 401, pMatrix256);
    
    // Volume: VertexEndcap_layer3_ladder11_volume
    TGeoVolume *pVertexEndcap_layer3_ladder11_volume_a41e3e0 = new TGeoVolume("VertexEndcap_layer3_ladder11_volume",pVertexEndcap_ladderD_69, pMed8);
    pVertexEndcap_layer3_ladder11_volume_a41e3e0->SetLineColor(130);
    pVertexEndcap_layer3_volume_a41bc78->AddNode(pVertexEndcap_layer3_ladder11_volume_a41e3e0, 402, pMatrix258);
    
    pVertexEndcap_layer3_ladder0_volume_a41be10->AddNode(pVertexEndcap_detB_volume_a418b70, 379);
    pVertexEndcap_layer3_ladder1_volume_a41c180->AddNode(pVertexEndcap_detB_volume_a418b70, 380);
    pVertexEndcap_layer3_ladder2_volume_a41c4f0->AddNode(pVertexEndcap_detB_volume_a418b70, 381);
    pVertexEndcap_layer3_ladder3_volume_a41c860->AddNode(pVertexEndcap_detB_volume_a418b70, 382);
    pVertexEndcap_layer3_ladder4_volume_a41cbd0->AddNode(pVertexEndcap_detB_volume_a418b70, 383);
    pVertexEndcap_layer3_ladder5_volume_a41cf40->AddNode(pVertexEndcap_detB_volume_a418b70, 384);
    pVertexEndcap_layer3_ladder6_volume_a41d2b0->AddNode(pVertexEndcap_detB_volume_a418b70, 385);
    pVertexEndcap_layer3_ladder7_volume_a41d620->AddNode(pVertexEndcap_detB_volume_a418b70, 386);
    pVertexEndcap_layer3_ladder8_volume_a41d990->AddNode(pVertexEndcap_detB_volume_a418b70, 387);
    pVertexEndcap_layer3_ladder9_volume_a41dd00->AddNode(pVertexEndcap_detB_volume_a418b70, 388);
    pVertexEndcap_layer3_ladder10_volume_a41e070->AddNode(pVertexEndcap_detB_volume_a418b70, 389);
    pVertexEndcap_layer3_ladder11_volume_a41e3e0->AddNode(pVertexEndcap_detB_volume_a418b70, 390);
    
    // Shape: VertexEndcap_ladderE type: TGeoTrd2
    dx1 = 1.941143*Scale;
    dx2 = 0.414110*Scale;
    dy1 = 0.005650;
    dy2 = 0.005650;
    dz  = 2.822222*Scale;
    TGeoShape *pVertexEndcap_ladderE_70 = new TGeoTrd2("VertexEndcap_ladderE", dx1,dx2,dy1,dy2,dz);
    
    // Volume: VertexEndcap_layer4_ladder0_volume
    TGeoVolume *pVertexEndcap_layer4_ladder0_volume_a41f1a8 = new TGeoVolume("VertexEndcap_layer4_ladder0_volume",pVertexEndcap_ladderE_70, pMed8);
    pVertexEndcap_layer4_ladder0_volume_a41f1a8->SetLineColor(130);
    pVertexEndcap_layer4_volume_a41f010->AddNode(pVertexEndcap_layer4_ladder0_volume_a41f1a8, 415, pMatrix272);
    
    // Volume: VertexEndcap_layer4_ladder1_volume
    TGeoVolume *pVertexEndcap_layer4_ladder1_volume_a41f518 = new TGeoVolume("VertexEndcap_layer4_ladder1_volume",pVertexEndcap_ladderE_70, pMed8);
    pVertexEndcap_layer4_ladder1_volume_a41f518->SetLineColor(130);
    pVertexEndcap_layer4_volume_a41f010->AddNode(pVertexEndcap_layer4_ladder1_volume_a41f518, 416, pMatrix274);
    
    // Volume: VertexEndcap_layer4_ladder2_volume
    TGeoVolume *pVertexEndcap_layer4_ladder2_volume_a41f888 = new TGeoVolume("VertexEndcap_layer4_ladder2_volume",pVertexEndcap_ladderE_70, pMed8);
    pVertexEndcap_layer4_ladder2_volume_a41f888->SetLineColor(130);
    pVertexEndcap_layer4_volume_a41f010->AddNode(pVertexEndcap_layer4_ladder2_volume_a41f888, 417, pMatrix276);
    
    // Volume: VertexEndcap_layer4_ladder3_volume
    TGeoVolume *pVertexEndcap_layer4_ladder3_volume_a41fbf8 = new TGeoVolume("VertexEndcap_layer4_ladder3_volume",pVertexEndcap_ladderE_70, pMed8);
    pVertexEndcap_layer4_ladder3_volume_a41fbf8->SetLineColor(130);
    pVertexEndcap_layer4_volume_a41f010->AddNode(pVertexEndcap_layer4_ladder3_volume_a41fbf8, 418, pMatrix278);
    
    // Volume: VertexEndcap_layer4_ladder4_volume
    TGeoVolume *pVertexEndcap_layer4_ladder4_volume_a41ff68 = new TGeoVolume("VertexEndcap_layer4_ladder4_volume",pVertexEndcap_ladderE_70, pMed8);
    pVertexEndcap_layer4_ladder4_volume_a41ff68->SetLineColor(130);
    pVertexEndcap_layer4_volume_a41f010->AddNode(pVertexEndcap_layer4_ladder4_volume_a41ff68, 419, pMatrix280);
    
    // Volume: VertexEndcap_layer4_ladder5_volume
    TGeoVolume *pVertexEndcap_layer4_ladder5_volume_a4202d8 = new TGeoVolume("VertexEndcap_layer4_ladder5_volume",pVertexEndcap_ladderE_70, pMed8);
    pVertexEndcap_layer4_ladder5_volume_a4202d8->SetLineColor(130);
    pVertexEndcap_layer4_volume_a41f010->AddNode(pVertexEndcap_layer4_ladder5_volume_a4202d8, 420, pMatrix282);
    
    // Volume: VertexEndcap_layer4_ladder6_volume
    TGeoVolume *pVertexEndcap_layer4_ladder6_volume_a420648 = new TGeoVolume("VertexEndcap_layer4_ladder6_volume",pVertexEndcap_ladderE_70, pMed8);
    pVertexEndcap_layer4_ladder6_volume_a420648->SetLineColor(130);
    pVertexEndcap_layer4_volume_a41f010->AddNode(pVertexEndcap_layer4_ladder6_volume_a420648, 421, pMatrix284);
    
    // Volume: VertexEndcap_layer4_ladder7_volume
    TGeoVolume *pVertexEndcap_layer4_ladder7_volume_a4209b8 = new TGeoVolume("VertexEndcap_layer4_ladder7_volume",pVertexEndcap_ladderE_70, pMed8);
    pVertexEndcap_layer4_ladder7_volume_a4209b8->SetLineColor(130);
    pVertexEndcap_layer4_volume_a41f010->AddNode(pVertexEndcap_layer4_ladder7_volume_a4209b8, 422, pMatrix286);
    
    // Volume: VertexEndcap_layer4_ladder8_volume
    TGeoVolume *pVertexEndcap_layer4_ladder8_volume_a420d28 = new TGeoVolume("VertexEndcap_layer4_ladder8_volume",pVertexEndcap_ladderE_70, pMed8);
    pVertexEndcap_layer4_ladder8_volume_a420d28->SetLineColor(130);
    pVertexEndcap_layer4_volume_a41f010->AddNode(pVertexEndcap_layer4_ladder8_volume_a420d28, 423, pMatrix288);
    
    // Volume: VertexEndcap_layer4_ladder9_volume
    TGeoVolume *pVertexEndcap_layer4_ladder9_volume_a421098 = new TGeoVolume("VertexEndcap_layer4_ladder9_volume",pVertexEndcap_ladderE_70, pMed8);
    pVertexEndcap_layer4_ladder9_volume_a421098->SetLineColor(130);
    pVertexEndcap_layer4_volume_a41f010->AddNode(pVertexEndcap_layer4_ladder9_volume_a421098, 424, pMatrix290);
    
    // Volume: VertexEndcap_layer4_ladder10_volume
    TGeoVolume *pVertexEndcap_layer4_ladder10_volume_a421408 = new TGeoVolume("VertexEndcap_layer4_ladder10_volume",pVertexEndcap_ladderE_70, pMed8);
    pVertexEndcap_layer4_ladder10_volume_a421408->SetLineColor(130);
    pVertexEndcap_layer4_volume_a41f010->AddNode(pVertexEndcap_layer4_ladder10_volume_a421408, 425, pMatrix292);
    
    // Volume: VertexEndcap_layer4_ladder11_volume
    TGeoVolume *pVertexEndcap_layer4_ladder11_volume_a421778 = new TGeoVolume("VertexEndcap_layer4_ladder11_volume",pVertexEndcap_ladderE_70, pMed8);
    pVertexEndcap_layer4_ladder11_volume_a421778->SetLineColor(130);
    pVertexEndcap_layer4_volume_a41f010->AddNode(pVertexEndcap_layer4_ladder11_volume_a421778, 426, pMatrix294);
    
    pVertexEndcap_layer4_ladder0_volume_a41f1a8->AddNode(pVertexEndcap_detA_volume_a4102d8, 403);
    pVertexEndcap_layer4_ladder1_volume_a41f518->AddNode(pVertexEndcap_detA_volume_a4102d8, 404);
    pVertexEndcap_layer4_ladder2_volume_a41f888->AddNode(pVertexEndcap_detA_volume_a4102d8, 405);
    pVertexEndcap_layer4_ladder3_volume_a41fbf8->AddNode(pVertexEndcap_detA_volume_a4102d8, 406);
    pVertexEndcap_layer4_ladder4_volume_a41ff68->AddNode(pVertexEndcap_detA_volume_a4102d8, 407);
    pVertexEndcap_layer4_ladder5_volume_a4202d8->AddNode(pVertexEndcap_detA_volume_a4102d8, 408);
    pVertexEndcap_layer4_ladder6_volume_a420648->AddNode(pVertexEndcap_detA_volume_a4102d8, 409);
    pVertexEndcap_layer4_ladder7_volume_a4209b8->AddNode(pVertexEndcap_detA_volume_a4102d8, 410);
    pVertexEndcap_layer4_ladder8_volume_a420d28->AddNode(pVertexEndcap_detA_volume_a4102d8, 411);
    pVertexEndcap_layer4_ladder9_volume_a421098->AddNode(pVertexEndcap_detA_volume_a4102d8, 412);
    pVertexEndcap_layer4_ladder10_volume_a421408->AddNode(pVertexEndcap_detA_volume_a4102d8, 413);
    pVertexEndcap_layer4_ladder11_volume_a421778->AddNode(pVertexEndcap_detA_volume_a4102d8, 414);
    // Shape: VertexEndcap_ladderF type: TGeoTrd2
    
    dx1 = 1.941143*Scale;
    dx2 = 0.414110*Scale;
    dy1 = 0.005650;
    dy2 = 0.005650;
    dz  = 2.822222*Scale;
    TGeoShape *pVertexEndcap_ladderF_71 = new TGeoTrd2("VertexEndcap_ladderF", dx1,dx2,dy1,dy2,dz);
    
    // Volume: VertexEndcap_layer5_ladder0_volume
    TGeoVolume *pVertexEndcap_layer5_ladder0_volume_a422500 = new TGeoVolume("VertexEndcap_layer5_ladder0_volume",pVertexEndcap_ladderF_71, pMed8);
    pVertexEndcap_layer5_ladder0_volume_a422500->SetLineColor(130);
    pVertexEndcap_layer5_volume_a422368->AddNode(pVertexEndcap_layer5_ladder0_volume_a422500, 439, pMatrix308);
    
    // Volume: VertexEndcap_layer5_ladder1_volume
    TGeoVolume *pVertexEndcap_layer5_ladder1_volume_a422870 = new TGeoVolume("VertexEndcap_layer5_ladder1_volume",pVertexEndcap_ladderF_71, pMed8);
    pVertexEndcap_layer5_ladder1_volume_a422870->SetLineColor(130);
    pVertexEndcap_layer5_volume_a422368->AddNode(pVertexEndcap_layer5_ladder1_volume_a422870, 440, pMatrix310);
    
    // Volume: VertexEndcap_layer5_ladder2_volume
    TGeoVolume *pVertexEndcap_layer5_ladder2_volume_a422be0 = new TGeoVolume("VertexEndcap_layer5_ladder2_volume",pVertexEndcap_ladderF_71, pMed8);
    pVertexEndcap_layer5_ladder2_volume_a422be0->SetLineColor(130);
    pVertexEndcap_layer5_volume_a422368->AddNode(pVertexEndcap_layer5_ladder2_volume_a422be0, 441, pMatrix312);
    
    // Volume: VertexEndcap_layer5_ladder3_volume
    TGeoVolume *pVertexEndcap_layer5_ladder3_volume_a422f50 = new TGeoVolume("VertexEndcap_layer5_ladder3_volume",pVertexEndcap_ladderF_71, pMed8);
    pVertexEndcap_layer5_ladder3_volume_a422f50->SetLineColor(130);
    pVertexEndcap_layer5_volume_a422368->AddNode(pVertexEndcap_layer5_ladder3_volume_a422f50, 442, pMatrix314);
    
    // Volume: VertexEndcap_layer5_ladder4_volume
    TGeoVolume *pVertexEndcap_layer5_ladder4_volume_a4232c0 = new TGeoVolume("VertexEndcap_layer5_ladder4_volume",pVertexEndcap_ladderF_71, pMed8);
    pVertexEndcap_layer5_ladder4_volume_a4232c0->SetLineColor(130);
    pVertexEndcap_layer5_volume_a422368->AddNode(pVertexEndcap_layer5_ladder4_volume_a4232c0, 443, pMatrix316);
    
    // Volume: VertexEndcap_layer5_ladder5_volume
    TGeoVolume *pVertexEndcap_layer5_ladder5_volume_a423630 = new TGeoVolume("VertexEndcap_layer5_ladder5_volume",pVertexEndcap_ladderF_71, pMed8);
    pVertexEndcap_layer5_ladder5_volume_a423630->SetLineColor(130);
    pVertexEndcap_layer5_volume_a422368->AddNode(pVertexEndcap_layer5_ladder5_volume_a423630, 444, pMatrix318);
    
    // Volume: VertexEndcap_layer5_ladder6_volume
    TGeoVolume *pVertexEndcap_layer5_ladder6_volume_a4239a0 = new TGeoVolume("VertexEndcap_layer5_ladder6_volume",pVertexEndcap_ladderF_71, pMed8);
    pVertexEndcap_layer5_ladder6_volume_a4239a0->SetLineColor(130);
    pVertexEndcap_layer5_volume_a422368->AddNode(pVertexEndcap_layer5_ladder6_volume_a4239a0, 445, pMatrix320);
    
    // Volume: VertexEndcap_layer5_ladder7_volume
    TGeoVolume *pVertexEndcap_layer5_ladder7_volume_a423d10 = new TGeoVolume("VertexEndcap_layer5_ladder7_volume",pVertexEndcap_ladderF_71, pMed8);
    pVertexEndcap_layer5_ladder7_volume_a423d10->SetLineColor(130);
    pVertexEndcap_layer5_volume_a422368->AddNode(pVertexEndcap_layer5_ladder7_volume_a423d10, 446, pMatrix322);
    
    // Volume: VertexEndcap_layer5_ladder8_volume
    TGeoVolume *pVertexEndcap_layer5_ladder8_volume_a424080 = new TGeoVolume("VertexEndcap_layer5_ladder8_volume",pVertexEndcap_ladderF_71, pMed8);
    pVertexEndcap_layer5_ladder8_volume_a424080->SetLineColor(130);
    pVertexEndcap_layer5_volume_a422368->AddNode(pVertexEndcap_layer5_ladder8_volume_a424080, 447, pMatrix324);
    
    // Volume: VertexEndcap_layer5_ladder9_volume
    TGeoVolume *pVertexEndcap_layer5_ladder9_volume_a4243f0 = new TGeoVolume("VertexEndcap_layer5_ladder9_volume",pVertexEndcap_ladderF_71, pMed8);
    pVertexEndcap_layer5_ladder9_volume_a4243f0->SetLineColor(130);
    pVertexEndcap_layer5_volume_a422368->AddNode(pVertexEndcap_layer5_ladder9_volume_a4243f0, 448, pMatrix326);
    
    // Volume: VertexEndcap_layer5_ladder10_volume
    TGeoVolume *pVertexEndcap_layer5_ladder10_volume_a424760 = new TGeoVolume("VertexEndcap_layer5_ladder10_volume",pVertexEndcap_ladderF_71, pMed8);
    pVertexEndcap_layer5_ladder10_volume_a424760->SetLineColor(130);
    pVertexEndcap_layer5_volume_a422368->AddNode(pVertexEndcap_layer5_ladder10_volume_a424760, 449, pMatrix328);
    
    // Volume: VertexEndcap_layer5_ladder11_volume
    TGeoVolume *pVertexEndcap_layer5_ladder11_volume_a424ad0 = new TGeoVolume("VertexEndcap_layer5_ladder11_volume",pVertexEndcap_ladderF_71, pMed8);
    pVertexEndcap_layer5_ladder11_volume_a424ad0->SetLineColor(130);
    pVertexEndcap_layer5_volume_a422368->AddNode(pVertexEndcap_layer5_ladder11_volume_a424ad0, 450, pMatrix330);
    pVertexEndcap_layer5_ladder0_volume_a422500->AddNode(pVertexEndcap_detA_volume_a4102d8, 427);
    pVertexEndcap_layer5_ladder1_volume_a422870->AddNode(pVertexEndcap_detA_volume_a4102d8, 428);
    pVertexEndcap_layer5_ladder2_volume_a422be0->AddNode(pVertexEndcap_detA_volume_a4102d8, 429);
    pVertexEndcap_layer5_ladder3_volume_a422f50->AddNode(pVertexEndcap_detA_volume_a4102d8, 430);
    pVertexEndcap_layer5_ladder4_volume_a4232c0->AddNode(pVertexEndcap_detA_volume_a4102d8, 431);
    pVertexEndcap_layer5_ladder5_volume_a423630->AddNode(pVertexEndcap_detA_volume_a4102d8, 432);
    pVertexEndcap_layer5_ladder6_volume_a4239a0->AddNode(pVertexEndcap_detA_volume_a4102d8, 433);
    pVertexEndcap_layer5_ladder7_volume_a423d10->AddNode(pVertexEndcap_detA_volume_a4102d8, 434);
    pVertexEndcap_layer5_ladder8_volume_a424080->AddNode(pVertexEndcap_detA_volume_a4102d8, 435);
    pVertexEndcap_layer5_ladder9_volume_a4243f0->AddNode(pVertexEndcap_detA_volume_a4102d8, 436);
    pVertexEndcap_layer5_ladder10_volume_a424760->AddNode(pVertexEndcap_detA_volume_a4102d8, 437);
    pVertexEndcap_layer5_ladder11_volume_a424ad0->AddNode(pVertexEndcap_detA_volume_a4102d8, 438);
    
    // Shape: VertexEndcap_ladderG type: TGeoTrd2
    
    dx1 = 1.941143*Scale;
    dx2 = 0.517638*Scale;
    dy1 = 0.005650;
    dy2 = 0.005650;
    dz  = 2.622222*Scale;
    TGeoShape *pVertexEndcap_ladderG_72 = new TGeoTrd2("VertexEndcap_ladderG", dx1,dx2,dy1,dy2,dz);
    
    // Volume: VertexEndcap_layer6_ladder0_volume
    TGeoVolume *pVertexEndcap_layer6_ladder0_volume_a425858 = new TGeoVolume("VertexEndcap_layer6_ladder0_volume",pVertexEndcap_ladderG_72, pMed8);
    pVertexEndcap_layer6_ladder0_volume_a425858->SetLineColor(130);
    pVertexEndcap_layer6_volume_a4256c0->AddNode(pVertexEndcap_layer6_ladder0_volume_a425858, 463, pMatrix344);
    
    // Volume: VertexEndcap_layer6_ladder1_volume
    TGeoVolume *pVertexEndcap_layer6_ladder1_volume_a3d28b0 = new TGeoVolume("VertexEndcap_layer6_ladder1_volume",pVertexEndcap_ladderG_72, pMed8);
    pVertexEndcap_layer6_ladder1_volume_a3d28b0->SetLineColor(130);
    pVertexEndcap_layer6_volume_a4256c0->AddNode(pVertexEndcap_layer6_ladder1_volume_a3d28b0, 464, pMatrix346);
    
    // Volume: VertexEndcap_layer6_ladder2_volume
    TGeoVolume *pVertexEndcap_layer6_ladder2_volume_a3d2c20 = new TGeoVolume("VertexEndcap_layer6_ladder2_volume",pVertexEndcap_ladderG_72, pMed8);
    pVertexEndcap_layer6_ladder2_volume_a3d2c20->SetLineColor(130);
    pVertexEndcap_layer6_volume_a4256c0->AddNode(pVertexEndcap_layer6_ladder2_volume_a3d2c20, 465, pMatrix348);
    
    // Volume: VertexEndcap_layer6_ladder3_volume
    TGeoVolume *pVertexEndcap_layer6_ladder3_volume_a3d2f90 = new TGeoVolume("VertexEndcap_layer6_ladder3_volume",pVertexEndcap_ladderG_72, pMed8);
    pVertexEndcap_layer6_ladder3_volume_a3d2f90->SetLineColor(130);
    pVertexEndcap_layer6_volume_a4256c0->AddNode(pVertexEndcap_layer6_ladder3_volume_a3d2f90, 466, pMatrix350);
    
    // Volume: VertexEndcap_layer6_ladder4_volume
    TGeoVolume *pVertexEndcap_layer6_ladder4_volume_a3d3300 = new TGeoVolume("VertexEndcap_layer6_ladder4_volume",pVertexEndcap_ladderG_72, pMed8);
    pVertexEndcap_layer6_ladder4_volume_a3d3300->SetLineColor(130);
    pVertexEndcap_layer6_volume_a4256c0->AddNode(pVertexEndcap_layer6_ladder4_volume_a3d3300, 467, pMatrix352);
    
    // Volume: VertexEndcap_layer6_ladder5_volume
    TGeoVolume *pVertexEndcap_layer6_ladder5_volume_a3d3670 = new TGeoVolume("VertexEndcap_layer6_ladder5_volume",pVertexEndcap_ladderG_72, pMed8);
    pVertexEndcap_layer6_ladder5_volume_a3d3670->SetLineColor(130);
    pVertexEndcap_layer6_volume_a4256c0->AddNode(pVertexEndcap_layer6_ladder5_volume_a3d3670, 468, pMatrix354);
    
    // Volume: VertexEndcap_layer6_ladder6_volume
    TGeoVolume *pVertexEndcap_layer6_ladder6_volume_a3d39e0 = new TGeoVolume("VertexEndcap_layer6_ladder6_volume",pVertexEndcap_ladderG_72, pMed8);
    pVertexEndcap_layer6_ladder6_volume_a3d39e0->SetLineColor(130);
    pVertexEndcap_layer6_volume_a4256c0->AddNode(pVertexEndcap_layer6_ladder6_volume_a3d39e0, 469, pMatrix356);
    
    // Volume: VertexEndcap_layer6_ladder7_volume
    TGeoVolume *pVertexEndcap_layer6_ladder7_volume_a3d3d50 = new TGeoVolume("VertexEndcap_layer6_ladder7_volume",pVertexEndcap_ladderG_72, pMed8);
    pVertexEndcap_layer6_ladder7_volume_a3d3d50->SetLineColor(130);
    pVertexEndcap_layer6_volume_a4256c0->AddNode(pVertexEndcap_layer6_ladder7_volume_a3d3d50, 470, pMatrix358);
    
    // Volume: VertexEndcap_layer6_ladder8_volume
    TGeoVolume *pVertexEndcap_layer6_ladder8_volume_a3d40c0 = new TGeoVolume("VertexEndcap_layer6_ladder8_volume",pVertexEndcap_ladderG_72, pMed8);
    pVertexEndcap_layer6_ladder8_volume_a3d40c0->SetLineColor(130);
    pVertexEndcap_layer6_volume_a4256c0->AddNode(pVertexEndcap_layer6_ladder8_volume_a3d40c0, 471, pMatrix360);
    
    // Volume: VertexEndcap_layer6_ladder9_volume
    TGeoVolume *pVertexEndcap_layer6_ladder9_volume_a3d4430 = new TGeoVolume("VertexEndcap_layer6_ladder9_volume",pVertexEndcap_ladderG_72, pMed8);
    pVertexEndcap_layer6_ladder9_volume_a3d4430->SetLineColor(130);
    pVertexEndcap_layer6_volume_a4256c0->AddNode(pVertexEndcap_layer6_ladder9_volume_a3d4430, 472, pMatrix362);
    
    // Volume: VertexEndcap_layer6_ladder10_volume
    TGeoVolume *pVertexEndcap_layer6_ladder10_volume_a3d47a0 = new TGeoVolume("VertexEndcap_layer6_ladder10_volume",pVertexEndcap_ladderG_72, pMed8);
    pVertexEndcap_layer6_ladder10_volume_a3d47a0->SetLineColor(130);
    pVertexEndcap_layer6_volume_a4256c0->AddNode(pVertexEndcap_layer6_ladder10_volume_a3d47a0, 473, pMatrix364);
    
    // Volume: VertexEndcap_layer6_ladder11_volume
    TGeoVolume *pVertexEndcap_layer6_ladder11_volume_a3d4b10 = new TGeoVolume("VertexEndcap_layer6_ladder11_volume",pVertexEndcap_ladderG_72, pMed8);
    pVertexEndcap_layer6_ladder11_volume_a3d4b10->SetLineColor(130);
    pVertexEndcap_layer6_volume_a4256c0->AddNode(pVertexEndcap_layer6_ladder11_volume_a3d4b10, 474, pMatrix366);
    
    pVertexEndcap_layer6_ladder0_volume_a425858->AddNode(pVertexEndcap_detB_volume_a418b70, 451);
    pVertexEndcap_layer6_ladder1_volume_a3d28b0->AddNode(pVertexEndcap_detB_volume_a418b70, 452);
    pVertexEndcap_layer6_ladder2_volume_a3d2c20->AddNode(pVertexEndcap_detB_volume_a418b70, 453);
    pVertexEndcap_layer6_ladder3_volume_a3d2f90->AddNode(pVertexEndcap_detB_volume_a418b70, 454);
    pVertexEndcap_layer6_ladder4_volume_a3d3300->AddNode(pVertexEndcap_detB_volume_a418b70, 455);
    pVertexEndcap_layer6_ladder5_volume_a3d3670->AddNode(pVertexEndcap_detB_volume_a418b70, 456);
    pVertexEndcap_layer6_ladder6_volume_a3d39e0->AddNode(pVertexEndcap_detB_volume_a418b70, 457);
    pVertexEndcap_layer6_ladder7_volume_a3d3d50->AddNode(pVertexEndcap_detB_volume_a418b70, 458);
    pVertexEndcap_layer6_ladder8_volume_a3d40c0->AddNode(pVertexEndcap_detB_volume_a418b70, 459);
    pVertexEndcap_layer6_ladder9_volume_a3d4430->AddNode(pVertexEndcap_detB_volume_a418b70, 460);
    pVertexEndcap_layer6_ladder10_volume_a3d47a0->AddNode(pVertexEndcap_detB_volume_a418b70, 461);
    pVertexEndcap_layer6_ladder11_volume_a3d4b10->AddNode(pVertexEndcap_detB_volume_a418b70, 462);
    // Shape: VertexEndcap_ladderH type: TGeoTrd2
    
    dx1 = 1.941143*Scale;
    dx2 = 0.517638*Scale;
    dy1 = 0.005650;
    dy2 = 0.005650;
    dz  = 2.622222*Scale;
    TGeoShape *pVertexEndcap_ladderH_73 = new TGeoTrd2("VertexEndcap_ladderH", dx1,dx2,dy1,dy2,dz);
    // Volume: VertexEndcap_layer7_ladder0_volume
    TGeoVolume *pVertexEndcap_layer7_ladder0_volume_a3d58d8 = new TGeoVolume("VertexEndcap_layer7_ladder0_volume",pVertexEndcap_ladderH_73, pMed8);
    pVertexEndcap_layer7_ladder0_volume_a3d58d8->SetLineColor(130);
    pVertexEndcap_layer7_volume_a3d5740->AddNode(pVertexEndcap_layer7_ladder0_volume_a3d58d8, 487, pMatrix380);
    
    // Volume: VertexEndcap_layer7_ladder1_volume
    TGeoVolume *pVertexEndcap_layer7_ladder1_volume_a3d5c48 = new TGeoVolume("VertexEndcap_layer7_ladder1_volume",pVertexEndcap_ladderH_73, pMed8);
    pVertexEndcap_layer7_ladder1_volume_a3d5c48->SetLineColor(130);
    pVertexEndcap_layer7_volume_a3d5740->AddNode(pVertexEndcap_layer7_ladder1_volume_a3d5c48, 488, pMatrix382);
    
    // Volume: VertexEndcap_layer7_ladder2_volume
    TGeoVolume *pVertexEndcap_layer7_ladder2_volume_a3d5fb8 = new TGeoVolume("VertexEndcap_layer7_ladder2_volume",pVertexEndcap_ladderH_73, pMed8);
    pVertexEndcap_layer7_ladder2_volume_a3d5fb8->SetLineColor(130);
    pVertexEndcap_layer7_volume_a3d5740->AddNode(pVertexEndcap_layer7_ladder2_volume_a3d5fb8, 489, pMatrix384);
    
    // Volume: VertexEndcap_layer7_ladder3_volume
    TGeoVolume *pVertexEndcap_layer7_ladder3_volume_a3d6328 = new TGeoVolume("VertexEndcap_layer7_ladder3_volume",pVertexEndcap_ladderH_73, pMed8);
    pVertexEndcap_layer7_ladder3_volume_a3d6328->SetLineColor(130);
    pVertexEndcap_layer7_volume_a3d5740->AddNode(pVertexEndcap_layer7_ladder3_volume_a3d6328, 490, pMatrix386);
    
    // Volume: VertexEndcap_layer7_ladder4_volume
    TGeoVolume *pVertexEndcap_layer7_ladder4_volume_a3d6698 = new TGeoVolume("VertexEndcap_layer7_ladder4_volume",pVertexEndcap_ladderH_73, pMed8);
    pVertexEndcap_layer7_ladder4_volume_a3d6698->SetLineColor(130);
    pVertexEndcap_layer7_volume_a3d5740->AddNode(pVertexEndcap_layer7_ladder4_volume_a3d6698, 491, pMatrix388);
    
    // Volume: VertexEndcap_layer7_ladder5_volume
    TGeoVolume *pVertexEndcap_layer7_ladder5_volume_a3d6a08 = new TGeoVolume("VertexEndcap_layer7_ladder5_volume",pVertexEndcap_ladderH_73, pMed8);
    pVertexEndcap_layer7_ladder5_volume_a3d6a08->SetLineColor(130);
    pVertexEndcap_layer7_volume_a3d5740->AddNode(pVertexEndcap_layer7_ladder5_volume_a3d6a08, 492, pMatrix390);
    
    // Volume: VertexEndcap_layer7_ladder6_volume
    TGeoVolume *pVertexEndcap_layer7_ladder6_volume_a3d6d78 = new TGeoVolume("VertexEndcap_layer7_ladder6_volume",pVertexEndcap_ladderH_73, pMed8);
    pVertexEndcap_layer7_ladder6_volume_a3d6d78->SetLineColor(130);
    pVertexEndcap_layer7_volume_a3d5740->AddNode(pVertexEndcap_layer7_ladder6_volume_a3d6d78, 493, pMatrix392);
    
    // Volume: VertexEndcap_layer7_ladder7_volume
    TGeoVolume *pVertexEndcap_layer7_ladder7_volume_a3d70e8 = new TGeoVolume("VertexEndcap_layer7_ladder7_volume",pVertexEndcap_ladderH_73, pMed8);
    pVertexEndcap_layer7_ladder7_volume_a3d70e8->SetLineColor(130);
    pVertexEndcap_layer7_volume_a3d5740->AddNode(pVertexEndcap_layer7_ladder7_volume_a3d70e8, 494, pMatrix394);
    
    // Volume: VertexEndcap_layer7_ladder8_volume
    TGeoVolume *pVertexEndcap_layer7_ladder8_volume_a3d7458 = new TGeoVolume("VertexEndcap_layer7_ladder8_volume",pVertexEndcap_ladderH_73, pMed8);
    pVertexEndcap_layer7_ladder8_volume_a3d7458->SetLineColor(130);
    pVertexEndcap_layer7_volume_a3d5740->AddNode(pVertexEndcap_layer7_ladder8_volume_a3d7458, 495, pMatrix396);
    
    // Volume: VertexEndcap_layer7_ladder9_volume
    TGeoVolume *pVertexEndcap_layer7_ladder9_volume_a3d77c8 = new TGeoVolume("VertexEndcap_layer7_ladder9_volume",pVertexEndcap_ladderH_73, pMed8);
    pVertexEndcap_layer7_ladder9_volume_a3d77c8->SetLineColor(130);
    pVertexEndcap_layer7_volume_a3d5740->AddNode(pVertexEndcap_layer7_ladder9_volume_a3d77c8, 496, pMatrix398);
    
    // Volume: VertexEndcap_layer7_ladder10_volume
    TGeoVolume *pVertexEndcap_layer7_ladder10_volume_a3d7b38 = new TGeoVolume("VertexEndcap_layer7_ladder10_volume",pVertexEndcap_ladderH_73, pMed8);
    pVertexEndcap_layer7_ladder10_volume_a3d7b38->SetLineColor(130);
    pVertexEndcap_layer7_volume_a3d5740->AddNode(pVertexEndcap_layer7_ladder10_volume_a3d7b38, 497, pMatrix400);
    
    // Volume: VertexEndcap_layer7_ladder11_volume
    TGeoVolume *pVertexEndcap_layer7_ladder11_volume_a3d7ea8 = new TGeoVolume("VertexEndcap_layer7_ladder11_volume",pVertexEndcap_ladderH_73, pMed8);
    pVertexEndcap_layer7_ladder11_volume_a3d7ea8->SetLineColor(130);
    pVertexEndcap_layer7_volume_a3d5740->AddNode(pVertexEndcap_layer7_ladder11_volume_a3d7ea8, 498, pMatrix402);
    
    pVertexEndcap_layer7_ladder0_volume_a3d58d8->AddNode(pVertexEndcap_detB_volume_a418b70, 475);
    pVertexEndcap_layer7_ladder1_volume_a3d5c48->AddNode(pVertexEndcap_detB_volume_a418b70, 476);
    pVertexEndcap_layer7_ladder2_volume_a3d5fb8->AddNode(pVertexEndcap_detB_volume_a418b70, 477);
    pVertexEndcap_layer7_ladder3_volume_a3d6328->AddNode(pVertexEndcap_detB_volume_a418b70, 478);
    pVertexEndcap_layer7_ladder4_volume_a3d6698->AddNode(pVertexEndcap_detB_volume_a418b70, 479);
    pVertexEndcap_layer7_ladder5_volume_a3d6a08->AddNode(pVertexEndcap_detB_volume_a418b70, 480);
    pVertexEndcap_layer7_ladder6_volume_a3d6d78->AddNode(pVertexEndcap_detB_volume_a418b70, 481);
    pVertexEndcap_layer7_ladder7_volume_a3d70e8->AddNode(pVertexEndcap_detB_volume_a418b70, 482);
    pVertexEndcap_layer7_ladder8_volume_a3d7458->AddNode(pVertexEndcap_detB_volume_a418b70, 483);
    pVertexEndcap_layer7_ladder9_volume_a3d77c8->AddNode(pVertexEndcap_detB_volume_a418b70, 484);
    pVertexEndcap_layer7_ladder10_volume_a3d7b38->AddNode(pVertexEndcap_detB_volume_a418b70, 485);
    pVertexEndcap_layer7_ladder11_volume_a3d7ea8->AddNode(pVertexEndcap_detB_volume_a418b70, 486);

//--------------------------------Fine Ladder EndCap (trapezoidali)----------------------------------------


// ------------------------------  Ladder del BARREL (rettangolari)----------------------------------------
    // Shape: VertexBarrel_ladderA type: TGeoBBox


    //Silicio
    dx = 0.480000*Scale;
    dy = SiThick;//0.005000;
    dz = 6.250000;
    TGeoShape *pVertexBarrel_ladderA_74 = new TGeoBBox("VertexBarrel_ladderA", dx,dy,dz);
    
    // Volume: VertexBarrel_layer0_ladder0outer_volume
    TGeoVolume *pVertexBarrel_layer0_ladder0outer_volume_a3d9420 = new 
TGeoVolume("VertexBarrel_layer0_ladder0outer_volume",pVertexBarrel_ladderA_74, pMed8);
    pVertexBarrel_layer0_ladder0outer_volume_a3d9420->SetLineColor(130);
    pVertexBarrel_layer0_volume_a3d9220->AddNode(pVertexBarrel_layer0_ladder0outer_volume_a3d9420, 13, pMatrix416);
    
    // Volume: VertexBarrel_layer0_ladder1inner_volume
     TGeoVolume *pVertexBarrel_layer0_ladder1inner_volume_a3d97d0 = new TGeoVolume("VertexBarrel_layer0_ladder1inner_volume",pVertexBarrel_ladderA_74, pMed8);
    pVertexBarrel_layer0_ladder1inner_volume_a3d97d0->SetLineColor(130);
    pVertexBarrel_layer0_volume_a3d9220->AddNode(pVertexBarrel_layer0_ladder1inner_volume_a3d97d0, 14, pMatrix417);
    
    // Volume: VertexBarrel_layer0_ladder2outer_volume
    TGeoVolume *pVertexBarrel_layer0_ladder2outer_volume_a3d9b28 = new TGeoVolume("VertexBarrel_layer0_ladder2outer_volume",pVertexBarrel_ladderA_74, pMed8);
    pVertexBarrel_layer0_ladder2outer_volume_a3d9b28->SetLineColor(130);
    pVertexBarrel_layer0_volume_a3d9220->AddNode(pVertexBarrel_layer0_ladder2outer_volume_a3d9b28, 15, pMatrix419);
    
    // Volume: VertexBarrel_layer0_ladder3inner_volume
    TGeoVolume *pVertexBarrel_layer0_ladder3inner_volume_a3d9e80 = new TGeoVolume("VertexBarrel_layer0_ladder3inner_volume",pVertexBarrel_ladderA_74, pMed8);
    pVertexBarrel_layer0_ladder3inner_volume_a3d9e80->SetLineColor(130);
    pVertexBarrel_layer0_volume_a3d9220->AddNode(pVertexBarrel_layer0_ladder3inner_volume_a3d9e80, 16, pMatrix421);
    
    // Volume: VertexBarrel_layer0_ladder4outer_volume
    TGeoVolume *pVertexBarrel_layer0_ladder4outer_volume_a3da1d8 = new TGeoVolume("VertexBarrel_layer0_ladder4outer_volume",pVertexBarrel_ladderA_74, pMed8);
    pVertexBarrel_layer0_ladder4outer_volume_a3da1d8->SetLineColor(130);
    pVertexBarrel_layer0_volume_a3d9220->AddNode(pVertexBarrel_layer0_ladder4outer_volume_a3da1d8, 17, pMatrix423);
    
    // Volume: VertexBarrel_layer0_ladder5inner_volume
    TGeoVolume *pVertexBarrel_layer0_ladder5inner_volume_a3da530 = new TGeoVolume("VertexBarrel_layer0_ladder5inner_volume",pVertexBarrel_ladderA_74, pMed8);
    pVertexBarrel_layer0_ladder5inner_volume_a3da530->SetLineColor(130);
    pVertexBarrel_layer0_volume_a3d9220->AddNode(pVertexBarrel_layer0_ladder5inner_volume_a3da530, 18, pMatrix425);
    
    // Volume: VertexBarrel_layer0_ladder6outer_volume
    TGeoVolume *pVertexBarrel_layer0_ladder6outer_volume_a3da888 = new TGeoVolume("VertexBarrel_layer0_ladder6outer_volume",pVertexBarrel_ladderA_74, pMed8);
    pVertexBarrel_layer0_ladder6outer_volume_a3da888->SetLineColor(130);
    pVertexBarrel_layer0_volume_a3d9220->AddNode(pVertexBarrel_layer0_ladder6outer_volume_a3da888, 19, pMatrix427);
    
    // Volume: VertexBarrel_layer0_ladder7inner_volume
    TGeoVolume *pVertexBarrel_layer0_ladder7inner_volume_a3dabe0 = new TGeoVolume("VertexBarrel_layer0_ladder7inner_volume",pVertexBarrel_ladderA_74, pMed8);
    pVertexBarrel_layer0_ladder7inner_volume_a3dabe0->SetLineColor(130);
    pVertexBarrel_layer0_volume_a3d9220->AddNode(pVertexBarrel_layer0_ladder7inner_volume_a3dabe0, 20, pMatrix429);
    
    // Volume: VertexBarrel_layer0_ladder8outer_volume
    TGeoVolume *pVertexBarrel_layer0_ladder8outer_volume_a3daf38 = new TGeoVolume("VertexBarrel_layer0_ladder8outer_volume",pVertexBarrel_ladderA_74, pMed8);
    pVertexBarrel_layer0_ladder8outer_volume_a3daf38->SetLineColor(130);
    pVertexBarrel_layer0_volume_a3d9220->AddNode(pVertexBarrel_layer0_ladder8outer_volume_a3daf38, 21, pMatrix431);
    
    // Volume: VertexBarrel_layer0_ladder9inner_volume
    TGeoVolume *pVertexBarrel_layer0_ladder9inner_volume_a3db290 = new TGeoVolume("VertexBarrel_layer0_ladder9inner_volume",pVertexBarrel_ladderA_74, pMed8);
    pVertexBarrel_layer0_ladder9inner_volume_a3db290->SetLineColor(130);
    pVertexBarrel_layer0_volume_a3d9220->AddNode(pVertexBarrel_layer0_ladder9inner_volume_a3db290, 22, pMatrix433);
    
    // Volume: VertexBarrel_layer0_ladder10outer_volume
    TGeoVolume *pVertexBarrel_layer0_ladder10outer_volume_a3db5e8 = new TGeoVolume("VertexBarrel_layer0_ladder10outer_volume",pVertexBarrel_ladderA_74, pMed8);
    pVertexBarrel_layer0_ladder10outer_volume_a3db5e8->SetLineColor(130);
    pVertexBarrel_layer0_volume_a3d9220->AddNode(pVertexBarrel_layer0_ladder10outer_volume_a3db5e8, 23, pMatrix435);
    
    // Volume: VertexBarrel_layer0_ladder11inner_volume
    TGeoVolume *pVertexBarrel_layer0_ladder11inner_volume_a3db940 = new TGeoVolume("VertexBarrel_layer0_ladder11inner_volume",pVertexBarrel_ladderA_74, pMed8);
    pVertexBarrel_layer0_ladder11inner_volume_a3db940->SetLineColor(130);
    pVertexBarrel_layer0_volume_a3d9220->AddNode(pVertexBarrel_layer0_ladder11inner_volume_a3db940, 24, pMatrix437);
    

    //------------------------------------Supporto 
    // Shape: VertexBarrelSupports_ladderA type: TGeoBBox
    dx = 0.480000*Scale;
    dy = CThick;//0.003800;
    dz = 6.250000;
    TGeoShape *pVertexBarrelSupports_ladderA_75 = new TGeoBBox("VertexBarrelSupports_ladderA", dx,dy,dz);

    // Volume: VertexBarrelSupports_layer0_ladder0outer_volume
    TGeoVolume *pVertexBarrelSupports_layer0_ladder0outer_volume_a3dbca0 = new TGeoVolume("VertexBarrelSupports_layer0_ladder0outer_volume",pVertexBarrelSupports_ladderA_75, pMed5);
    pVertexBarrelSupports_layer0_ladder0outer_volume_a3dbca0->SetLineColor(16);
    pVertexBarrel_layer0_volume_a3d9220->AddNode(pVertexBarrelSupports_layer0_ladder0outer_volume_a3dbca0, 25, pMatrix439);

    // Volume: VertexBarrelSupports_layer0_ladder1inner_volume
    TGeoVolume *pVertexBarrelSupports_layer0_ladder1inner_volume_a3dbde0 = new TGeoVolume("VertexBarrelSupports_layer0_ladder1inner_volume",pVertexBarrelSupports_ladderA_75, pMed5);
    pVertexBarrelSupports_layer0_ladder1inner_volume_a3dbde0->SetLineColor(16);
    pVertexBarrel_layer0_volume_a3d9220->AddNode(pVertexBarrelSupports_layer0_ladder1inner_volume_a3dbde0, 26, pMatrix440);
    // Volume: VertexBarrelSupports_layer0_ladder2outer_volume
    TGeoVolume *pVertexBarrelSupports_layer0_ladder2outer_volume_a3dbf20 = new TGeoVolume("VertexBarrelSupports_layer0_ladder2outer_volume",pVertexBarrelSupports_ladderA_75, pMed5);
    pVertexBarrelSupports_layer0_ladder2outer_volume_a3dbf20->SetLineColor(16);
    pVertexBarrel_layer0_volume_a3d9220->AddNode(pVertexBarrelSupports_layer0_ladder2outer_volume_a3dbf20, 27, pMatrix442);
    // Volume: VertexBarrelSupports_layer0_ladder3inner_volume
    TGeoVolume *pVertexBarrelSupports_layer0_ladder3inner_volume_a3dc060 = new TGeoVolume("VertexBarrelSupports_layer0_ladder3inner_volume",pVertexBarrelSupports_ladderA_75, pMed5);
    pVertexBarrelSupports_layer0_ladder3inner_volume_a3dc060->SetLineColor(16);
    pVertexBarrel_layer0_volume_a3d9220->AddNode(pVertexBarrelSupports_layer0_ladder3inner_volume_a3dc060, 28, pMatrix444);
    // Volume: VertexBarrelSupports_layer0_ladder4outer_volume
    TGeoVolume *pVertexBarrelSupports_layer0_ladder4outer_volume_a3dc1a0 = new TGeoVolume("VertexBarrelSupports_layer0_ladder4outer_volume",pVertexBarrelSupports_ladderA_75, pMed5);
    pVertexBarrelSupports_layer0_ladder4outer_volume_a3dc1a0->SetLineColor(16);
    pVertexBarrel_layer0_volume_a3d9220->AddNode(pVertexBarrelSupports_layer0_ladder4outer_volume_a3dc1a0, 29, pMatrix446);
    // Volume: VertexBarrelSupports_layer0_ladder5inner_volume
    TGeoVolume *pVertexBarrelSupports_layer0_ladder5inner_volume_a3dc2e0 = new TGeoVolume("VertexBarrelSupports_layer0_ladder5inner_volume",pVertexBarrelSupports_ladderA_75, pMed5);
    pVertexBarrelSupports_layer0_ladder5inner_volume_a3dc2e0->SetLineColor(16);
    pVertexBarrel_layer0_volume_a3d9220->AddNode(pVertexBarrelSupports_layer0_ladder5inner_volume_a3dc2e0, 30, pMatrix448);
    // Volume: VertexBarrelSupports_layer0_ladder6outer_volume
    TGeoVolume *pVertexBarrelSupports_layer0_ladder6outer_volume_a3dc420 = new TGeoVolume("VertexBarrelSupports_layer0_ladder6outer_volume",pVertexBarrelSupports_ladderA_75, pMed5);
    pVertexBarrelSupports_layer0_ladder6outer_volume_a3dc420->SetLineColor(16);
    pVertexBarrel_layer0_volume_a3d9220->AddNode(pVertexBarrelSupports_layer0_ladder6outer_volume_a3dc420, 31, pMatrix450);
    // Volume: VertexBarrelSupports_layer0_ladder7inner_volume
    TGeoVolume *pVertexBarrelSupports_layer0_ladder7inner_volume_a3dc560 = new TGeoVolume("VertexBarrelSupports_layer0_ladder7inner_volume",pVertexBarrelSupports_ladderA_75, pMed5);
    pVertexBarrelSupports_layer0_ladder7inner_volume_a3dc560->SetLineColor(16);
    pVertexBarrel_layer0_volume_a3d9220->AddNode(pVertexBarrelSupports_layer0_ladder7inner_volume_a3dc560, 32, pMatrix452);
    // Volume: VertexBarrelSupports_layer0_ladder8outer_volume
    TGeoVolume *pVertexBarrelSupports_layer0_ladder8outer_volume_a3dc6a0 = new TGeoVolume("VertexBarrelSupports_layer0_ladder8outer_volume",pVertexBarrelSupports_ladderA_75, pMed5);
    pVertexBarrelSupports_layer0_ladder8outer_volume_a3dc6a0->SetLineColor(16);
    pVertexBarrel_layer0_volume_a3d9220->AddNode(pVertexBarrelSupports_layer0_ladder8outer_volume_a3dc6a0, 33, pMatrix454);
    // Volume: VertexBarrelSupports_layer0_ladder9inner_volume
    TGeoVolume *pVertexBarrelSupports_layer0_ladder9inner_volume_a3dc7e0 = new TGeoVolume("VertexBarrelSupports_layer0_ladder9inner_volume",pVertexBarrelSupports_ladderA_75, pMed5);
    pVertexBarrelSupports_layer0_ladder9inner_volume_a3dc7e0->SetLineColor(16);
    pVertexBarrel_layer0_volume_a3d9220->AddNode(pVertexBarrelSupports_layer0_ladder9inner_volume_a3dc7e0, 34, pMatrix456);
    // Volume: VertexBarrelSupports_layer0_ladder10outer_volume
    TGeoVolume *pVertexBarrelSupports_layer0_ladder10outer_volume_a3dc920 = new TGeoVolume("VertexBarrelSupports_layer0_ladder10outer_volume",pVertexBarrelSupports_ladderA_75, pMed5);
    pVertexBarrelSupports_layer0_ladder10outer_volume_a3dc920->SetLineColor(16);
    pVertexBarrel_layer0_volume_a3d9220->AddNode(pVertexBarrelSupports_layer0_ladder10outer_volume_a3dc920, 35, pMatrix458);
    // Volume: VertexBarrelSupports_layer0_ladder11inner_volume
    TGeoVolume *pVertexBarrelSupports_layer0_ladder11inner_volume_a3dca60 = new TGeoVolume("VertexBarrelSupports_layer0_ladder11inner_volume",pVertexBarrelSupports_ladderA_75, pMed5);
    pVertexBarrelSupports_layer0_ladder11inner_volume_a3dca60->SetLineColor(16);
    pVertexBarrel_layer0_volume_a3d9220->AddNode(pVertexBarrelSupports_layer0_ladder11inner_volume_a3dca60, 36, pMatrix460);
    
    
    //Rettangoli di Silicio contenuti nei ladder precedenti
    // Shape: VertexBarrel_ladderA_detA type: TGeoBBox  
    dx = 0.455000*Scale;
    dy = SiThick;//0.005000;
    dz = 6.250000;
    TGeoShape *pVertexBarrel_ladderA_detA_76 = new TGeoBBox("VertexBarrel_ladderA_detA", dx,dy,dz);
    // Volume: VertexBarrel_detA_volume
    TGeoVolume *pVertexBarrel_detA_volume_a3d9570 = new        
    TGeoVolume("VertexBarrel_detA_volume",pVertexBarrel_ladderA_detA_76, pMed8);

    pVertexBarrel_detA_volume_a3d9570->SetLineColor(130);
    pVertexBarrel_layer0_ladder0outer_volume_a3d9420->AddNode(pVertexBarrel_detA_volume_a3d9570, 1);
    pVertexBarrel_layer0_ladder1inner_volume_a3d97d0->AddNode(pVertexBarrel_detA_volume_a3d9570, 2);
    pVertexBarrel_layer0_ladder2outer_volume_a3d9b28->AddNode(pVertexBarrel_detA_volume_a3d9570, 3);
    pVertexBarrel_layer0_ladder3inner_volume_a3d9e80->AddNode(pVertexBarrel_detA_volume_a3d9570, 4);
    pVertexBarrel_layer0_ladder4outer_volume_a3da1d8->AddNode(pVertexBarrel_detA_volume_a3d9570, 5);
    pVertexBarrel_layer0_ladder5inner_volume_a3da530->AddNode(pVertexBarrel_detA_volume_a3d9570, 6);
    pVertexBarrel_layer0_ladder6outer_volume_a3da888->AddNode(pVertexBarrel_detA_volume_a3d9570, 7);
    pVertexBarrel_layer0_ladder7inner_volume_a3dabe0->AddNode(pVertexBarrel_detA_volume_a3d9570, 8);
    pVertexBarrel_layer0_ladder8outer_volume_a3daf38->AddNode(pVertexBarrel_detA_volume_a3d9570, 9);
    pVertexBarrel_layer0_ladder9inner_volume_a3db290->AddNode(pVertexBarrel_detA_volume_a3d9570, 10);
    pVertexBarrel_layer0_ladder10outer_volume_a3db5e8->AddNode(pVertexBarrel_detA_volume_a3d9570, 11);
    pVertexBarrel_layer0_ladder11inner_volume_a3db940->AddNode(pVertexBarrel_detA_volume_a3d9570, 12);
    
    //Silicio Per i Layer sucessivi al primo
    // Shape: VertexBarrel_ladderB type: TGeoBBox
    dx = 0.690000*Scale;
    dy = SiThick;//0.005000;
    dz = 6.250000;
    
    TGeoShape *pVertexBarrel_ladderB_77 = new TGeoBBox("VertexBarrel_ladderB", dx,dy,dz);
    // Volume: VertexBarrel_layer1_ladder0outer_volume
    TGeoVolume *pVertexBarrel_layer1_ladder0outer_volume_a3ddb10 = new TGeoVolume("VertexBarrel_layer1_ladder0outer_volume",pVertexBarrel_ladderB_77, pMed8);
    pVertexBarrel_layer1_ladder0outer_volume_a3ddb10->SetLineColor(130);
    pVertexBarrel_layer1_volume_a3dd910->AddNode(pVertexBarrel_layer1_ladder0outer_volume_a3ddb10, 49, pMatrix474);
    // Volume: VertexBarrel_layer1_ladder1inner_volume
    TGeoVolume *pVertexBarrel_layer1_ladder1inner_volume_a3ddec0 = new TGeoVolume("VertexBarrel_layer1_ladder1inner_volume",pVertexBarrel_ladderB_77, pMed8);
    pVertexBarrel_layer1_ladder1inner_volume_a3ddec0->SetLineColor(130);
    pVertexBarrel_layer1_volume_a3dd910->AddNode(pVertexBarrel_layer1_ladder1inner_volume_a3ddec0, 50, pMatrix475);
    // Volume: VertexBarrel_layer1_ladder2outer_volume
    TGeoVolume *pVertexBarrel_layer1_ladder2outer_volume_a3de218 = new TGeoVolume("VertexBarrel_layer1_ladder2outer_volume",pVertexBarrel_ladderB_77, pMed8);
    pVertexBarrel_layer1_ladder2outer_volume_a3de218->SetLineColor(130);
    pVertexBarrel_layer1_volume_a3dd910->AddNode(pVertexBarrel_layer1_ladder2outer_volume_a3de218, 51, pMatrix477);
    // Volume: VertexBarrel_layer1_ladder3inner_volume
    TGeoVolume *pVertexBarrel_layer1_ladder3inner_volume_a43d708 = new TGeoVolume("VertexBarrel_layer1_ladder3inner_volume",pVertexBarrel_ladderB_77, pMed8);
    pVertexBarrel_layer1_ladder3inner_volume_a43d708->SetLineColor(130);
    pVertexBarrel_layer1_volume_a3dd910->AddNode(pVertexBarrel_layer1_ladder3inner_volume_a43d708, 52, pMatrix479);
    // Volume: VertexBarrel_layer1_ladder4outer_volume
    TGeoVolume *pVertexBarrel_layer1_ladder4outer_volume_a43da30 = new TGeoVolume("VertexBarrel_layer1_ladder4outer_volume",pVertexBarrel_ladderB_77, pMed8);
    pVertexBarrel_layer1_ladder4outer_volume_a43da30->SetLineColor(130);
    pVertexBarrel_layer1_volume_a3dd910->AddNode(pVertexBarrel_layer1_ladder4outer_volume_a43da30, 53, pMatrix481);
    // Volume: VertexBarrel_layer1_ladder5inner_volume
    TGeoVolume *pVertexBarrel_layer1_ladder5inner_volume_a43dd88 = new TGeoVolume("VertexBarrel_layer1_ladder5inner_volume",pVertexBarrel_ladderB_77, pMed8);
    pVertexBarrel_layer1_ladder5inner_volume_a43dd88->SetLineColor(130);
    pVertexBarrel_layer1_volume_a3dd910->AddNode(pVertexBarrel_layer1_ladder5inner_volume_a43dd88, 54, pMatrix483);
    // Volume: VertexBarrel_layer1_ladder6outer_volume
    TGeoVolume *pVertexBarrel_layer1_ladder6outer_volume_a43e0e0 = new TGeoVolume("VertexBarrel_layer1_ladder6outer_volume",pVertexBarrel_ladderB_77, pMed8);
    pVertexBarrel_layer1_ladder6outer_volume_a43e0e0->SetLineColor(130);
    pVertexBarrel_layer1_volume_a3dd910->AddNode(pVertexBarrel_layer1_ladder6outer_volume_a43e0e0, 55, pMatrix485);
    // Volume: VertexBarrel_layer1_ladder7inner_volume
    TGeoVolume *pVertexBarrel_layer1_ladder7inner_volume_a43e438 = new TGeoVolume("VertexBarrel_layer1_ladder7inner_volume",pVertexBarrel_ladderB_77, pMed8);
    pVertexBarrel_layer1_ladder7inner_volume_a43e438->SetLineColor(130);
    pVertexBarrel_layer1_volume_a3dd910->AddNode(pVertexBarrel_layer1_ladder7inner_volume_a43e438, 56, pMatrix487);
    // Volume: VertexBarrel_layer1_ladder8outer_volume
    TGeoVolume *pVertexBarrel_layer1_ladder8outer_volume_a43e790 = new TGeoVolume("VertexBarrel_layer1_ladder8outer_volume",pVertexBarrel_ladderB_77, pMed8);
    pVertexBarrel_layer1_ladder8outer_volume_a43e790->SetLineColor(130);
    pVertexBarrel_layer1_volume_a3dd910->AddNode(pVertexBarrel_layer1_ladder8outer_volume_a43e790, 57, pMatrix489);
    // Volume: VertexBarrel_layer1_ladder9inner_volume
    TGeoVolume *pVertexBarrel_layer1_ladder9inner_volume_a43eae8 = new TGeoVolume("VertexBarrel_layer1_ladder9inner_volume",pVertexBarrel_ladderB_77, pMed8);
    pVertexBarrel_layer1_ladder9inner_volume_a43eae8->SetLineColor(130);
    pVertexBarrel_layer1_volume_a3dd910->AddNode(pVertexBarrel_layer1_ladder9inner_volume_a43eae8, 58, pMatrix491);
    // Volume: VertexBarrel_layer1_ladder10outer_volume
    TGeoVolume *pVertexBarrel_layer1_ladder10outer_volume_a43ee40 = new TGeoVolume("VertexBarrel_layer1_ladder10outer_volume",pVertexBarrel_ladderB_77, pMed8);
    pVertexBarrel_layer1_ladder10outer_volume_a43ee40->SetLineColor(130);
    pVertexBarrel_layer1_volume_a3dd910->AddNode(pVertexBarrel_layer1_ladder10outer_volume_a43ee40, 59, pMatrix493);
    // Volume: VertexBarrel_layer1_ladder11inner_volume
    TGeoVolume *pVertexBarrel_layer1_ladder11inner_volume_a43f198 = new TGeoVolume("VertexBarrel_layer1_ladder11inner_volume",pVertexBarrel_ladderB_77, pMed8);
    pVertexBarrel_layer1_ladder11inner_volume_a43f198->SetLineColor(130);
    pVertexBarrel_layer1_volume_a3dd910->AddNode(pVertexBarrel_layer1_ladder11inner_volume_a43f198, 60, pMatrix495);
    // Shape: VertexBarrelSupports_ladderB type: TGeoBBox
    
    //Supporto
    dx = 0.690000*Scale;
    dy = CThick;//0.003800;
    dz = 6.250000;
    TGeoShape *pVertexBarrelSupports_ladderB_78 = new TGeoBBox("VertexBarrelSupports_ladderB", dx,dy,dz);
    // Volume: VertexBarrelSupports_layer1_ladder0outer_volume
    TGeoVolume *pVertexBarrelSupports_layer1_ladder0outer_volume_a43f4f8 = new TGeoVolume("VertexBarrelSupports_layer1_ladder0outer_volume",pVertexBarrelSupports_ladderB_78, pMed5);
    pVertexBarrelSupports_layer1_ladder0outer_volume_a43f4f8->SetLineColor(16);
    pVertexBarrel_layer1_volume_a3dd910->AddNode(pVertexBarrelSupports_layer1_ladder0outer_volume_a43f4f8, 61, pMatrix497);
    // Volume: VertexBarrelSupports_layer1_ladder1inner_volume
    TGeoVolume *pVertexBarrelSupports_layer1_ladder1inner_volume_a43f638 = new TGeoVolume("VertexBarrelSupports_layer1_ladder1inner_volume",pVertexBarrelSupports_ladderB_78, pMed5);
    pVertexBarrelSupports_layer1_ladder1inner_volume_a43f638->SetLineColor(16);
    pVertexBarrel_layer1_volume_a3dd910->AddNode(pVertexBarrelSupports_layer1_ladder1inner_volume_a43f638, 62, pMatrix498);
    // Volume: VertexBarrelSupports_layer1_ladder2outer_volume
    TGeoVolume *pVertexBarrelSupports_layer1_ladder2outer_volume_a43f778 = new TGeoVolume("VertexBarrelSupports_layer1_ladder2outer_volume",pVertexBarrelSupports_ladderB_78, pMed5);
    pVertexBarrelSupports_layer1_ladder2outer_volume_a43f778->SetLineColor(16);
    pVertexBarrel_layer1_volume_a3dd910->AddNode(pVertexBarrelSupports_layer1_ladder2outer_volume_a43f778, 63, pMatrix500);
    // Volume: VertexBarrelSupports_layer1_ladder3inner_volume
    TGeoVolume *pVertexBarrelSupports_layer1_ladder3inner_volume_a43f8b8 = new TGeoVolume("VertexBarrelSupports_layer1_ladder3inner_volume",pVertexBarrelSupports_ladderB_78, pMed5);
    pVertexBarrelSupports_layer1_ladder3inner_volume_a43f8b8->SetLineColor(16);
    pVertexBarrel_layer1_volume_a3dd910->AddNode(pVertexBarrelSupports_layer1_ladder3inner_volume_a43f8b8, 64, pMatrix502);
    // Volume: VertexBarrelSupports_layer1_ladder4outer_volume
    TGeoVolume *pVertexBarrelSupports_layer1_ladder4outer_volume_a43f9f8 = new TGeoVolume("VertexBarrelSupports_layer1_ladder4outer_volume",pVertexBarrelSupports_ladderB_78, pMed5);
    pVertexBarrelSupports_layer1_ladder4outer_volume_a43f9f8->SetLineColor(16);
    pVertexBarrel_layer1_volume_a3dd910->AddNode(pVertexBarrelSupports_layer1_ladder4outer_volume_a43f9f8, 65, pMatrix504);
    // Volume: VertexBarrelSupports_layer1_ladder5inner_volume
    TGeoVolume *pVertexBarrelSupports_layer1_ladder5inner_volume_a43fb38 = new TGeoVolume("VertexBarrelSupports_layer1_ladder5inner_volume",pVertexBarrelSupports_ladderB_78, pMed5);
    pVertexBarrelSupports_layer1_ladder5inner_volume_a43fb38->SetLineColor(16);
    pVertexBarrel_layer1_volume_a3dd910->AddNode(pVertexBarrelSupports_layer1_ladder5inner_volume_a43fb38, 66, pMatrix506);
    // Volume: VertexBarrelSupports_layer1_ladder6outer_volume
    TGeoVolume *pVertexBarrelSupports_layer1_ladder6outer_volume_a43fc78 = new TGeoVolume("VertexBarrelSupports_layer1_ladder6outer_volume",pVertexBarrelSupports_ladderB_78, pMed5);
    pVertexBarrelSupports_layer1_ladder6outer_volume_a43fc78->SetLineColor(16);
    pVertexBarrel_layer1_volume_a3dd910->AddNode(pVertexBarrelSupports_layer1_ladder6outer_volume_a43fc78, 67, pMatrix508);
    // Volume: VertexBarrelSupports_layer1_ladder7inner_volume
    TGeoVolume *pVertexBarrelSupports_layer1_ladder7inner_volume_a43fdb8 = new TGeoVolume("VertexBarrelSupports_layer1_ladder7inner_volume",pVertexBarrelSupports_ladderB_78, pMed5);
    pVertexBarrelSupports_layer1_ladder7inner_volume_a43fdb8->SetLineColor(16);
    pVertexBarrel_layer1_volume_a3dd910->AddNode(pVertexBarrelSupports_layer1_ladder7inner_volume_a43fdb8, 68, pMatrix510);
    // Volume: VertexBarrelSupports_layer1_ladder8outer_volume
    TGeoVolume *pVertexBarrelSupports_layer1_ladder8outer_volume_a43fef8 = new TGeoVolume("VertexBarrelSupports_layer1_ladder8outer_volume",pVertexBarrelSupports_ladderB_78, pMed5);
    pVertexBarrelSupports_layer1_ladder8outer_volume_a43fef8->SetLineColor(16);
    pVertexBarrel_layer1_volume_a3dd910->AddNode(pVertexBarrelSupports_layer1_ladder8outer_volume_a43fef8, 69, pMatrix512);
    // Volume: VertexBarrelSupports_layer1_ladder9inner_volume
    TGeoVolume *pVertexBarrelSupports_layer1_ladder9inner_volume_a440038 = new TGeoVolume("VertexBarrelSupports_layer1_ladder9inner_volume",pVertexBarrelSupports_ladderB_78, pMed5);
    pVertexBarrelSupports_layer1_ladder9inner_volume_a440038->SetLineColor(16);
    pVertexBarrel_layer1_volume_a3dd910->AddNode(pVertexBarrelSupports_layer1_ladder9inner_volume_a440038, 70, pMatrix514);
    // Volume: VertexBarrelSupports_layer1_ladder10outer_volume
    TGeoVolume *pVertexBarrelSupports_layer1_ladder10outer_volume_a440178 = new TGeoVolume("VertexBarrelSupports_layer1_ladder10outer_volume",pVertexBarrelSupports_ladderB_78, pMed5);
    pVertexBarrelSupports_layer1_ladder10outer_volume_a440178->SetLineColor(16);
    pVertexBarrel_layer1_volume_a3dd910->AddNode(pVertexBarrelSupports_layer1_ladder10outer_volume_a440178, 71, pMatrix516);
    // Volume: VertexBarrelSupports_layer1_ladder11inner_volume
    TGeoVolume *pVertexBarrelSupports_layer1_ladder11inner_volume_a4402b8 = new TGeoVolume("VertexBarrelSupports_layer1_ladder11inner_volume",pVertexBarrelSupports_ladderB_78, pMed5);
    pVertexBarrelSupports_layer1_ladder11inner_volume_a4402b8->SetLineColor(16);
    pVertexBarrel_layer1_volume_a3dd910->AddNode(pVertexBarrelSupports_layer1_ladder11inner_volume_a4402b8, 72, pMatrix518);
    // Shape: VertexBarrel_ladderB_detB type: TGeoBBox
    
    //Rettangoli di Silicio contenuti nel ladder precedenti
    dx = 0.665000*Scale;
    dy = SiThick;//0.005000;
    dz = 6.250000;
    TGeoShape *pVertexBarrel_ladderB_detB_79 = new TGeoBBox("VertexBarrel_ladderB_detB", dx,dy,dz);
    // Volume: VertexBarrel_detB_volume
    TGeoVolume *pVertexBarrel_detB_volume_a3ddc60 = new TGeoVolume("VertexBarrel_detB_volume",pVertexBarrel_ladderB_detB_79, pMed8);
    pVertexBarrel_detB_volume_a3ddc60->SetLineColor(130);
    pVertexBarrel_layer1_ladder0outer_volume_a3ddb10->AddNode(pVertexBarrel_detB_volume_a3ddc60, 37);
    pVertexBarrel_layer1_ladder1inner_volume_a3ddec0->AddNode(pVertexBarrel_detB_volume_a3ddc60, 38);
    pVertexBarrel_layer1_ladder2outer_volume_a3de218->AddNode(pVertexBarrel_detB_volume_a3ddc60, 39);
    pVertexBarrel_layer1_ladder3inner_volume_a43d708->AddNode(pVertexBarrel_detB_volume_a3ddc60, 40);
    pVertexBarrel_layer1_ladder4outer_volume_a43da30->AddNode(pVertexBarrel_detB_volume_a3ddc60, 41);
    pVertexBarrel_layer1_ladder5inner_volume_a43dd88->AddNode(pVertexBarrel_detB_volume_a3ddc60, 42);
    pVertexBarrel_layer1_ladder6outer_volume_a43e0e0->AddNode(pVertexBarrel_detB_volume_a3ddc60, 43);
    pVertexBarrel_layer1_ladder7inner_volume_a43e438->AddNode(pVertexBarrel_detB_volume_a3ddc60, 44);
    pVertexBarrel_layer1_ladder8outer_volume_a43e790->AddNode(pVertexBarrel_detB_volume_a3ddc60, 45);
    pVertexBarrel_layer1_ladder9inner_volume_a43eae8->AddNode(pVertexBarrel_detB_volume_a3ddc60, 46);
    pVertexBarrel_layer1_ladder10outer_volume_a43ee40->AddNode(pVertexBarrel_detB_volume_a3ddc60, 47);
    pVertexBarrel_layer1_ladder11inner_volume_a43f198->AddNode(pVertexBarrel_detB_volume_a3ddc60, 48);
    
    
    // Volume: VertexBarrelSupports_layer2_ladder0outer_volume
    TGeoVolume *pVertexBarrelSupports_layer2_ladder0outer_volume_a4412d8 = new TGeoVolume("VertexBarrelSupports_layer2_ladder0outer_volume",pVertexBarrelSupports_ladderB_78, pMed5);
    pVertexBarrelSupports_layer2_ladder0outer_volume_a4412d8->SetLineColor(16);
    pVertexBarrel_layer2_volume_a4410e8->AddNode(pVertexBarrelSupports_layer2_ladder0outer_volume_a4412d8, 91, pMatrix532);
    // Volume: VertexBarrelSupports_layer2_ladder1inner_volume
    TGeoVolume *pVertexBarrelSupports_layer2_ladder1inner_volume_a441418 = new TGeoVolume("VertexBarrelSupports_layer2_ladder1inner_volume",pVertexBarrelSupports_ladderB_78, pMed5);
    pVertexBarrelSupports_layer2_ladder1inner_volume_a441418->SetLineColor(16);
    pVertexBarrel_layer2_volume_a4410e8->AddNode(pVertexBarrelSupports_layer2_ladder1inner_volume_a441418, 92, pMatrix533);
    // Volume: VertexBarrelSupports_layer2_ladder2outer_volume
    TGeoVolume *pVertexBarrelSupports_layer2_ladder2outer_volume_a441558 = new TGeoVolume("VertexBarrelSupports_layer2_ladder2outer_volume",pVertexBarrelSupports_ladderB_78, pMed5);
    pVertexBarrelSupports_layer2_ladder2outer_volume_a441558->SetLineColor(16);
    pVertexBarrel_layer2_volume_a4410e8->AddNode(pVertexBarrelSupports_layer2_ladder2outer_volume_a441558, 93, pMatrix535);
    // Volume: VertexBarrelSupports_layer2_ladder3inner_volume
    TGeoVolume *pVertexBarrelSupports_layer2_ladder3inner_volume_a441698 = new TGeoVolume("VertexBarrelSupports_layer2_ladder3inner_volume",pVertexBarrelSupports_ladderB_78, pMed5);
    pVertexBarrelSupports_layer2_ladder3inner_volume_a441698->SetLineColor(16);
    pVertexBarrel_layer2_volume_a4410e8->AddNode(pVertexBarrelSupports_layer2_ladder3inner_volume_a441698, 94, pMatrix537);
    // Volume: VertexBarrelSupports_layer2_ladder4outer_volume
    TGeoVolume *pVertexBarrelSupports_layer2_ladder4outer_volume_a4417d8 = new TGeoVolume("VertexBarrelSupports_layer2_ladder4outer_volume",pVertexBarrelSupports_ladderB_78, pMed5);
    pVertexBarrelSupports_layer2_ladder4outer_volume_a4417d8->SetLineColor(16);
    pVertexBarrel_layer2_volume_a4410e8->AddNode(pVertexBarrelSupports_layer2_ladder4outer_volume_a4417d8, 95, pMatrix539);
    // Volume: VertexBarrelSupports_layer2_ladder5inner_volume
    TGeoVolume *pVertexBarrelSupports_layer2_ladder5inner_volume_a441918 = new TGeoVolume("VertexBarrelSupports_layer2_ladder5inner_volume",pVertexBarrelSupports_ladderB_78, pMed5);
    pVertexBarrelSupports_layer2_ladder5inner_volume_a441918->SetLineColor(16);
    pVertexBarrel_layer2_volume_a4410e8->AddNode(pVertexBarrelSupports_layer2_ladder5inner_volume_a441918, 96, pMatrix541);
    // Volume: VertexBarrelSupports_layer2_ladder6outer_volume
    TGeoVolume *pVertexBarrelSupports_layer2_ladder6outer_volume_a441a58 = new TGeoVolume("VertexBarrelSupports_layer2_ladder6outer_volume",pVertexBarrelSupports_ladderB_78, pMed5);
    pVertexBarrelSupports_layer2_ladder6outer_volume_a441a58->SetLineColor(16);
    pVertexBarrel_layer2_volume_a4410e8->AddNode(pVertexBarrelSupports_layer2_ladder6outer_volume_a441a58, 97, pMatrix543);
    // Volume: VertexBarrelSupports_layer2_ladder7inner_volume
    TGeoVolume *pVertexBarrelSupports_layer2_ladder7inner_volume_a441b98 = new TGeoVolume("VertexBarrelSupports_layer2_ladder7inner_volume",pVertexBarrelSupports_ladderB_78, pMed5);
    pVertexBarrelSupports_layer2_ladder7inner_volume_a441b98->SetLineColor(16);
    pVertexBarrel_layer2_volume_a4410e8->AddNode(pVertexBarrelSupports_layer2_ladder7inner_volume_a441b98, 98, pMatrix545);
    // Volume: VertexBarrelSupports_layer2_ladder8outer_volume
    TGeoVolume *pVertexBarrelSupports_layer2_ladder8outer_volume_a441cd8 = new TGeoVolume("VertexBarrelSupports_layer2_ladder8outer_volume",pVertexBarrelSupports_ladderB_78, pMed5);
    pVertexBarrelSupports_layer2_ladder8outer_volume_a441cd8->SetLineColor(16);
    pVertexBarrel_layer2_volume_a4410e8->AddNode(pVertexBarrelSupports_layer2_ladder8outer_volume_a441cd8, 99, pMatrix547);
    // Volume: VertexBarrelSupports_layer2_ladder9inner_volume
    TGeoVolume *pVertexBarrelSupports_layer2_ladder9inner_volume_a441e18 = new TGeoVolume("VertexBarrelSupports_layer2_ladder9inner_volume",pVertexBarrelSupports_ladderB_78, pMed5);
    pVertexBarrelSupports_layer2_ladder9inner_volume_a441e18->SetLineColor(16);
    pVertexBarrel_layer2_volume_a4410e8->AddNode(pVertexBarrelSupports_layer2_ladder9inner_volume_a441e18, 100, pMatrix549);
    // Volume: VertexBarrelSupports_layer2_ladder10outer_volume
    TGeoVolume *pVertexBarrelSupports_layer2_ladder10outer_volume_a441f60 = new TGeoVolume("VertexBarrelSupports_layer2_ladder10outer_volume",pVertexBarrelSupports_ladderB_78, pMed5);
    pVertexBarrelSupports_layer2_ladder10outer_volume_a441f60->SetLineColor(16);
    pVertexBarrel_layer2_volume_a4410e8->AddNode(pVertexBarrelSupports_layer2_ladder10outer_volume_a441f60, 101, pMatrix551);
    // Volume: VertexBarrelSupports_layer2_ladder11inner_volume
    TGeoVolume *pVertexBarrelSupports_layer2_ladder11inner_volume_a4420a8 = new TGeoVolume("VertexBarrelSupports_layer2_ladder11inner_volume",pVertexBarrelSupports_ladderB_78, pMed5);
    pVertexBarrelSupports_layer2_ladder11inner_volume_a4420a8->SetLineColor(16);
    pVertexBarrel_layer2_volume_a4410e8->AddNode(pVertexBarrelSupports_layer2_ladder11inner_volume_a4420a8, 102, pMatrix553);
    // Volume: VertexBarrelSupports_layer2_ladder12outer_volume
    TGeoVolume *pVertexBarrelSupports_layer2_ladder12outer_volume_a4421f0 = new TGeoVolume("VertexBarrelSupports_layer2_ladder12outer_volume",pVertexBarrelSupports_ladderB_78, pMed5);
    pVertexBarrelSupports_layer2_ladder12outer_volume_a4421f0->SetLineColor(16);
    pVertexBarrel_layer2_volume_a4410e8->AddNode(pVertexBarrelSupports_layer2_ladder12outer_volume_a4421f0, 103, pMatrix555);
    // Volume: VertexBarrelSupports_layer2_ladder13inner_volume
    TGeoVolume *pVertexBarrelSupports_layer2_ladder13inner_volume_a442338 = new TGeoVolume("VertexBarrelSupports_layer2_ladder13inner_volume",pVertexBarrelSupports_ladderB_78, pMed5);
    pVertexBarrelSupports_layer2_ladder13inner_volume_a442338->SetLineColor(16);
    pVertexBarrel_layer2_volume_a4410e8->AddNode(pVertexBarrelSupports_layer2_ladder13inner_volume_a442338, 104, pMatrix557);
    // Volume: VertexBarrelSupports_layer2_ladder14outer_volume
    TGeoVolume *pVertexBarrelSupports_layer2_ladder14outer_volume_a442480 = new TGeoVolume("VertexBarrelSupports_layer2_ladder14outer_volume",pVertexBarrelSupports_ladderB_78, pMed5);
    pVertexBarrelSupports_layer2_ladder14outer_volume_a442480->SetLineColor(16);
    pVertexBarrel_layer2_volume_a4410e8->AddNode(pVertexBarrelSupports_layer2_ladder14outer_volume_a442480, 105, pMatrix559);
    // Volume: VertexBarrelSupports_layer2_ladder15inner_volume
    TGeoVolume *pVertexBarrelSupports_layer2_ladder15inner_volume_a4425c8 = new TGeoVolume("VertexBarrelSupports_layer2_ladder15inner_volume",pVertexBarrelSupports_ladderB_78, pMed5);
    pVertexBarrelSupports_layer2_ladder15inner_volume_a4425c8->SetLineColor(16);
    pVertexBarrel_layer2_volume_a4410e8->AddNode(pVertexBarrelSupports_layer2_ladder15inner_volume_a4425c8, 106, pMatrix561);
    // Volume: VertexBarrelSupports_layer2_ladder16outer_volume
    TGeoVolume *pVertexBarrelSupports_layer2_ladder16outer_volume_a442710 = new TGeoVolume("VertexBarrelSupports_layer2_ladder16outer_volume",pVertexBarrelSupports_ladderB_78, pMed5);
    pVertexBarrelSupports_layer2_ladder16outer_volume_a442710->SetLineColor(16);
    pVertexBarrel_layer2_volume_a4410e8->AddNode(pVertexBarrelSupports_layer2_ladder16outer_volume_a442710, 107, pMatrix563);
    // Volume: VertexBarrelSupports_layer2_ladder17inner_volume
    TGeoVolume *pVertexBarrelSupports_layer2_ladder17inner_volume_a442858 = new TGeoVolume("VertexBarrelSupports_layer2_ladder17inner_volume",pVertexBarrelSupports_ladderB_78, pMed5);
    pVertexBarrelSupports_layer2_ladder17inner_volume_a442858->SetLineColor(16);
    pVertexBarrel_layer2_volume_a4410e8->AddNode(pVertexBarrelSupports_layer2_ladder17inner_volume_a442858, 108, pMatrix565);
    // Volume: VertexBarrel_layer2_ladder0outer_volume
    TGeoVolume *pVertexBarrel_layer2_ladder0outer_volume_a442990 = new TGeoVolume("VertexBarrel_layer2_ladder0outer_volume",pVertexBarrel_ladderB_77, pMed8);
    pVertexBarrel_layer2_ladder0outer_volume_a442990->SetLineColor(130);
    pVertexBarrel_layer2_volume_a4410e8->AddNode(pVertexBarrel_layer2_ladder0outer_volume_a442990, 109, pMatrix567);
    // Volume: VertexBarrel_layer2_ladder1inner_volume
    TGeoVolume *pVertexBarrel_layer2_ladder1inner_volume_a442ce8 = new TGeoVolume("VertexBarrel_layer2_ladder1inner_volume",pVertexBarrel_ladderB_77, pMed8);
    pVertexBarrel_layer2_ladder1inner_volume_a442ce8->SetLineColor(130);
    pVertexBarrel_layer2_volume_a4410e8->AddNode(pVertexBarrel_layer2_ladder1inner_volume_a442ce8, 110, pMatrix568);
    // Volume: VertexBarrel_layer2_ladder2outer_volume
    TGeoVolume *pVertexBarrel_layer2_ladder2outer_volume_a443040 = new TGeoVolume("VertexBarrel_layer2_ladder2outer_volume",pVertexBarrel_ladderB_77, pMed8);
    pVertexBarrel_layer2_ladder2outer_volume_a443040->SetLineColor(130);
    pVertexBarrel_layer2_volume_a4410e8->AddNode(pVertexBarrel_layer2_ladder2outer_volume_a443040, 111, pMatrix570);
    // Volume: VertexBarrel_layer2_ladder3inner_volume
    TGeoVolume *pVertexBarrel_layer2_ladder3inner_volume_a443398 = new TGeoVolume("VertexBarrel_layer2_ladder3inner_volume",pVertexBarrel_ladderB_77, pMed8);
    pVertexBarrel_layer2_ladder3inner_volume_a443398->SetLineColor(130);
    pVertexBarrel_layer2_volume_a4410e8->AddNode(pVertexBarrel_layer2_ladder3inner_volume_a443398, 112, pMatrix572);
    // Volume: VertexBarrel_layer2_ladder4outer_volume
    TGeoVolume *pVertexBarrel_layer2_ladder4outer_volume_a4436f0 = new TGeoVolume("VertexBarrel_layer2_ladder4outer_volume",pVertexBarrel_ladderB_77, pMed8);

    pVertexBarrel_layer2_ladder4outer_volume_a4436f0->SetLineColor(130);

    pVertexBarrel_layer2_volume_a4410e8->AddNode(pVertexBarrel_layer2_ladder4outer_volume_a4436f0, 113, pMatrix574);
    // Volume: VertexBarrel_layer2_ladder5inner_volume
    TGeoVolume *pVertexBarrel_layer2_ladder5inner_volume_a443a48 = new TGeoVolume("VertexBarrel_layer2_ladder5inner_volume",pVertexBarrel_ladderB_77, pMed8);

    pVertexBarrel_layer2_ladder5inner_volume_a443a48->SetLineColor(130);

    pVertexBarrel_layer2_volume_a4410e8->AddNode(pVertexBarrel_layer2_ladder5inner_volume_a443a48, 114, pMatrix576);
    // Volume: VertexBarrel_layer2_ladder6outer_volume
    TGeoVolume *pVertexBarrel_layer2_ladder6outer_volume_a443da0 = new TGeoVolume("VertexBarrel_layer2_ladder6outer_volume",pVertexBarrel_ladderB_77, pMed8);
    pVertexBarrel_layer2_ladder6outer_volume_a443da0->SetLineColor(130);
    pVertexBarrel_layer2_volume_a4410e8->AddNode(pVertexBarrel_layer2_ladder6outer_volume_a443da0, 115, pMatrix578);
    // Volume: VertexBarrel_layer2_ladder7inner_volume
    TGeoVolume *pVertexBarrel_layer2_ladder7inner_volume_a4440f8 = new TGeoVolume("VertexBarrel_layer2_ladder7inner_volume",pVertexBarrel_ladderB_77, pMed8);
    pVertexBarrel_layer2_ladder7inner_volume_a4440f8->SetLineColor(130);
    pVertexBarrel_layer2_volume_a4410e8->AddNode(pVertexBarrel_layer2_ladder7inner_volume_a4440f8, 116, pMatrix580);
    // Volume: VertexBarrel_layer2_ladder8outer_volume
    TGeoVolume *pVertexBarrel_layer2_ladder8outer_volume_a444450 = new TGeoVolume("VertexBarrel_layer2_ladder8outer_volume",pVertexBarrel_ladderB_77, pMed8);
    pVertexBarrel_layer2_ladder8outer_volume_a444450->SetLineColor(130);
    pVertexBarrel_layer2_volume_a4410e8->AddNode(pVertexBarrel_layer2_ladder8outer_volume_a444450, 117, pMatrix582);
    // Volume: VertexBarrel_layer2_ladder9inner_volume
    TGeoVolume *pVertexBarrel_layer2_ladder9inner_volume_a4447a8 = new TGeoVolume("VertexBarrel_layer2_ladder9inner_volume",pVertexBarrel_ladderB_77, pMed8);
    pVertexBarrel_layer2_ladder9inner_volume_a4447a8->SetLineColor(130);
    pVertexBarrel_layer2_volume_a4410e8->AddNode(pVertexBarrel_layer2_ladder9inner_volume_a4447a8, 118, pMatrix584);
    // Volume: VertexBarrel_layer2_ladder10outer_volume
    TGeoVolume *pVertexBarrel_layer2_ladder10outer_volume_a444b08 = new TGeoVolume("VertexBarrel_layer2_ladder10outer_volume",pVertexBarrel_ladderB_77, pMed8);
    pVertexBarrel_layer2_ladder10outer_volume_a444b08->SetLineColor(130);
    pVertexBarrel_layer2_volume_a4410e8->AddNode(pVertexBarrel_layer2_ladder10outer_volume_a444b08, 119, pMatrix586);
    // Volume: VertexBarrel_layer2_ladder11inner_volume
    TGeoVolume *pVertexBarrel_layer2_ladder11inner_volume_a444e68 = new TGeoVolume("VertexBarrel_layer2_ladder11inner_volume",pVertexBarrel_ladderB_77, pMed8);
    pVertexBarrel_layer2_ladder11inner_volume_a444e68->SetLineColor(130);
    pVertexBarrel_layer2_volume_a4410e8->AddNode(pVertexBarrel_layer2_ladder11inner_volume_a444e68, 120, pMatrix588);
    // Volume: VertexBarrel_layer2_ladder12outer_volume
    TGeoVolume *pVertexBarrel_layer2_ladder12outer_volume_a4451c8 = new TGeoVolume("VertexBarrel_layer2_ladder12outer_volume",pVertexBarrel_ladderB_77, pMed8);
    pVertexBarrel_layer2_ladder12outer_volume_a4451c8->SetLineColor(130);
    pVertexBarrel_layer2_volume_a4410e8->AddNode(pVertexBarrel_layer2_ladder12outer_volume_a4451c8, 121, pMatrix590);
    // Volume: VertexBarrel_layer2_ladder13inner_volume
    TGeoVolume *pVertexBarrel_layer2_ladder13inner_volume_a445528 = new TGeoVolume("VertexBarrel_layer2_ladder13inner_volume",pVertexBarrel_ladderB_77, pMed8);
    pVertexBarrel_layer2_ladder13inner_volume_a445528->SetLineColor(130);
    pVertexBarrel_layer2_volume_a4410e8->AddNode(pVertexBarrel_layer2_ladder13inner_volume_a445528, 122, pMatrix592);
    // Volume: VertexBarrel_layer2_ladder14outer_volume
    TGeoVolume *pVertexBarrel_layer2_ladder14outer_volume_a445888 = new TGeoVolume("VertexBarrel_layer2_ladder14outer_volume",pVertexBarrel_ladderB_77, pMed8);
    pVertexBarrel_layer2_ladder14outer_volume_a445888->SetLineColor(130);
    pVertexBarrel_layer2_volume_a4410e8->AddNode(pVertexBarrel_layer2_ladder14outer_volume_a445888, 123, pMatrix594);
    // Volume: VertexBarrel_layer2_ladder15inner_volume
    TGeoVolume *pVertexBarrel_layer2_ladder15inner_volume_a445be8 = new TGeoVolume("VertexBarrel_layer2_ladder15inner_volume",pVertexBarrel_ladderB_77, pMed8);
    pVertexBarrel_layer2_ladder15inner_volume_a445be8->SetLineColor(130);
    pVertexBarrel_layer2_volume_a4410e8->AddNode(pVertexBarrel_layer2_ladder15inner_volume_a445be8, 124, pMatrix596);
    // Volume: VertexBarrel_layer2_ladder16outer_volume
    TGeoVolume *pVertexBarrel_layer2_ladder16outer_volume_a445f48 = new TGeoVolume("VertexBarrel_layer2_ladder16outer_volume",pVertexBarrel_ladderB_77, pMed8);
    pVertexBarrel_layer2_ladder16outer_volume_a445f48->SetLineColor(130);
    pVertexBarrel_layer2_volume_a4410e8->AddNode(pVertexBarrel_layer2_ladder16outer_volume_a445f48, 125, pMatrix598);
    // Volume: VertexBarrel_layer2_ladder17inner_volume
    TGeoVolume *pVertexBarrel_layer2_ladder17inner_volume_a4462a8 = new TGeoVolume("VertexBarrel_layer2_ladder17inner_volume",pVertexBarrel_ladderB_77, pMed8);
    pVertexBarrel_layer2_ladder17inner_volume_a4462a8->SetLineColor(130);
    pVertexBarrel_layer2_volume_a4410e8->AddNode(pVertexBarrel_layer2_ladder17inner_volume_a4462a8, 126, pMatrix600);
    pVertexBarrel_layer2_ladder0outer_volume_a442990->AddNode(pVertexBarrel_detB_volume_a3ddc60, 73);
    pVertexBarrel_layer2_ladder1inner_volume_a442ce8->AddNode(pVertexBarrel_detB_volume_a3ddc60, 74);
    pVertexBarrel_layer2_ladder2outer_volume_a443040->AddNode(pVertexBarrel_detB_volume_a3ddc60, 75);
    pVertexBarrel_layer2_ladder3inner_volume_a443398->AddNode(pVertexBarrel_detB_volume_a3ddc60, 76);
    pVertexBarrel_layer2_ladder4outer_volume_a4436f0->AddNode(pVertexBarrel_detB_volume_a3ddc60, 77);
    pVertexBarrel_layer2_ladder5inner_volume_a443a48->AddNode(pVertexBarrel_detB_volume_a3ddc60, 78);
    pVertexBarrel_layer2_ladder6outer_volume_a443da0->AddNode(pVertexBarrel_detB_volume_a3ddc60, 79);
    pVertexBarrel_layer2_ladder7inner_volume_a4440f8->AddNode(pVertexBarrel_detB_volume_a3ddc60, 80);
    pVertexBarrel_layer2_ladder8outer_volume_a444450->AddNode(pVertexBarrel_detB_volume_a3ddc60, 81);
    pVertexBarrel_layer2_ladder9inner_volume_a4447a8->AddNode(pVertexBarrel_detB_volume_a3ddc60, 82);
    pVertexBarrel_layer2_ladder10outer_volume_a444b08->AddNode(pVertexBarrel_detB_volume_a3ddc60, 83);
    pVertexBarrel_layer2_ladder11inner_volume_a444e68->AddNode(pVertexBarrel_detB_volume_a3ddc60, 84);
    pVertexBarrel_layer2_ladder12outer_volume_a4451c8->AddNode(pVertexBarrel_detB_volume_a3ddc60, 85);
    pVertexBarrel_layer2_ladder13inner_volume_a445528->AddNode(pVertexBarrel_detB_volume_a3ddc60, 86);
    pVertexBarrel_layer2_ladder14outer_volume_a445888->AddNode(pVertexBarrel_detB_volume_a3ddc60, 87);
    pVertexBarrel_layer2_ladder15inner_volume_a445be8->AddNode(pVertexBarrel_detB_volume_a3ddc60, 88);
    pVertexBarrel_layer2_ladder16outer_volume_a445f48->AddNode(pVertexBarrel_detB_volume_a3ddc60, 89);
    pVertexBarrel_layer2_ladder17inner_volume_a4462a8->AddNode(pVertexBarrel_detB_volume_a3ddc60, 90);
    // Volume: VertexBarrelSupports_layer3_ladder0outer_volume
    TGeoVolume *pVertexBarrelSupports_layer3_ladder0outer_volume_a4482a0 = new TGeoVolume("VertexBarrelSupports_layer3_ladder0outer_volume",pVertexBarrelSupports_ladderB_78, pMed5);
    pVertexBarrelSupports_layer3_ladder0outer_volume_a4482a0->SetLineColor(16);
    pVertexBarrel_layer3_volume_a448080->AddNode(pVertexBarrelSupports_layer3_ladder0outer_volume_a4482a0, 151, pMatrix620);
    // Volume: VertexBarrelSupports_layer3_ladder1inner_volume
    TGeoVolume *pVertexBarrelSupports_layer3_ladder1inner_volume_a4483e0 = new TGeoVolume("VertexBarrelSupports_layer3_ladder1inner_volume",pVertexBarrelSupports_ladderB_78, pMed5);
    pVertexBarrelSupports_layer3_ladder1inner_volume_a4483e0->SetLineColor(16);
    pVertexBarrel_layer3_volume_a448080->AddNode(pVertexBarrelSupports_layer3_ladder1inner_volume_a4483e0, 152, pMatrix621);
    // Volume: VertexBarrelSupports_layer3_ladder2outer_volume
    TGeoVolume *pVertexBarrelSupports_layer3_ladder2outer_volume_a448520 = new TGeoVolume("VertexBarrelSupports_layer3_ladder2outer_volume",pVertexBarrelSupports_ladderB_78, pMed5);
    pVertexBarrelSupports_layer3_ladder2outer_volume_a448520->SetLineColor(16);
    pVertexBarrel_layer3_volume_a448080->AddNode(pVertexBarrelSupports_layer3_ladder2outer_volume_a448520, 153, pMatrix623);
    // Volume: VertexBarrelSupports_layer3_ladder3inner_volume
    TGeoVolume *pVertexBarrelSupports_layer3_ladder3inner_volume_a448660 = new TGeoVolume("VertexBarrelSupports_layer3_ladder3inner_volume",pVertexBarrelSupports_ladderB_78, pMed5);
    pVertexBarrelSupports_layer3_ladder3inner_volume_a448660->SetLineColor(16);
    pVertexBarrel_layer3_volume_a448080->AddNode(pVertexBarrelSupports_layer3_ladder3inner_volume_a448660, 154, pMatrix625);
    // Volume: VertexBarrelSupports_layer3_ladder4outer_volume
    TGeoVolume *pVertexBarrelSupports_layer3_ladder4outer_volume_a4487a0 = new TGeoVolume("VertexBarrelSupports_layer3_ladder4outer_volume",pVertexBarrelSupports_ladderB_78, pMed5);
    pVertexBarrelSupports_layer3_ladder4outer_volume_a4487a0->SetLineColor(16);
    pVertexBarrel_layer3_volume_a448080->AddNode(pVertexBarrelSupports_layer3_ladder4outer_volume_a4487a0, 155, pMatrix627);
    // Volume: VertexBarrelSupports_layer3_ladder5inner_volume
    TGeoVolume *pVertexBarrelSupports_layer3_ladder5inner_volume_a4488e0 = new TGeoVolume("VertexBarrelSupports_layer3_ladder5inner_volume",pVertexBarrelSupports_ladderB_78, pMed5);
    pVertexBarrelSupports_layer3_ladder5inner_volume_a4488e0->SetLineColor(16);
    pVertexBarrel_layer3_volume_a448080->AddNode(pVertexBarrelSupports_layer3_ladder5inner_volume_a4488e0, 156, pMatrix629);
    // Volume: VertexBarrelSupports_layer3_ladder6outer_volume
    TGeoVolume *pVertexBarrelSupports_layer3_ladder6outer_volume_a448a20 = new TGeoVolume("VertexBarrelSupports_layer3_ladder6outer_volume",pVertexBarrelSupports_ladderB_78, pMed5);
    pVertexBarrelSupports_layer3_ladder6outer_volume_a448a20->SetLineColor(16);
    pVertexBarrel_layer3_volume_a448080->AddNode(pVertexBarrelSupports_layer3_ladder6outer_volume_a448a20, 157, pMatrix631);
    // Volume: VertexBarrelSupports_layer3_ladder7inner_volume
    TGeoVolume *pVertexBarrelSupports_layer3_ladder7inner_volume_a448b60 = new TGeoVolume("VertexBarrelSupports_layer3_ladder7inner_volume",pVertexBarrelSupports_ladderB_78, pMed5);
    pVertexBarrelSupports_layer3_ladder7inner_volume_a448b60->SetLineColor(16);
    pVertexBarrel_layer3_volume_a448080->AddNode(pVertexBarrelSupports_layer3_ladder7inner_volume_a448b60, 158, pMatrix633);
    // Volume: VertexBarrelSupports_layer3_ladder8outer_volume
    TGeoVolume *pVertexBarrelSupports_layer3_ladder8outer_volume_a448ca0 = new TGeoVolume("VertexBarrelSupports_layer3_ladder8outer_volume",pVertexBarrelSupports_ladderB_78, pMed5);
    pVertexBarrelSupports_layer3_ladder8outer_volume_a448ca0->SetLineColor(16);
    pVertexBarrel_layer3_volume_a448080->AddNode(pVertexBarrelSupports_layer3_ladder8outer_volume_a448ca0, 159, pMatrix635);
    // Volume: VertexBarrelSupports_layer3_ladder9inner_volume
    TGeoVolume *pVertexBarrelSupports_layer3_ladder9inner_volume_a448de0 = new TGeoVolume("VertexBarrelSupports_layer3_ladder9inner_volume",pVertexBarrelSupports_ladderB_78, pMed5);
    pVertexBarrelSupports_layer3_ladder9inner_volume_a448de0->SetLineColor(16);
    pVertexBarrel_layer3_volume_a448080->AddNode(pVertexBarrelSupports_layer3_ladder9inner_volume_a448de0, 160, pMatrix637);
    // Volume: VertexBarrelSupports_layer3_ladder10outer_volume
    TGeoVolume *pVertexBarrelSupports_layer3_ladder10outer_volume_a448f28 = new TGeoVolume("VertexBarrelSupports_layer3_ladder10outer_volume",pVertexBarrelSupports_ladderB_78, pMed5);
    pVertexBarrelSupports_layer3_ladder10outer_volume_a448f28->SetLineColor(16);
    pVertexBarrel_layer3_volume_a448080->AddNode(pVertexBarrelSupports_layer3_ladder10outer_volume_a448f28, 161, pMatrix639);
    // Volume: VertexBarrelSupports_layer3_ladder11inner_volume
    TGeoVolume *pVertexBarrelSupports_layer3_ladder11inner_volume_a449070 = new TGeoVolume("VertexBarrelSupports_layer3_ladder11inner_volume",pVertexBarrelSupports_ladderB_78, pMed5);
    pVertexBarrelSupports_layer3_ladder11inner_volume_a449070->SetLineColor(16);
    pVertexBarrel_layer3_volume_a448080->AddNode(pVertexBarrelSupports_layer3_ladder11inner_volume_a449070, 162, pMatrix641);
    // Volume: VertexBarrelSupports_layer3_ladder12outer_volume
    TGeoVolume *pVertexBarrelSupports_layer3_ladder12outer_volume_a4491b8 = new TGeoVolume("VertexBarrelSupports_layer3_ladder12outer_volume",pVertexBarrelSupports_ladderB_78, pMed5);
    pVertexBarrelSupports_layer3_ladder12outer_volume_a4491b8->SetLineColor(16);
    pVertexBarrel_layer3_volume_a448080->AddNode(pVertexBarrelSupports_layer3_ladder12outer_volume_a4491b8, 163, pMatrix643);
    // Volume: VertexBarrelSupports_layer3_ladder13inner_volume
    TGeoVolume *pVertexBarrelSupports_layer3_ladder13inner_volume_a449300 = new TGeoVolume("VertexBarrelSupports_layer3_ladder13inner_volume",pVertexBarrelSupports_ladderB_78, pMed5);
    pVertexBarrelSupports_layer3_ladder13inner_volume_a449300->SetLineColor(16);
    pVertexBarrel_layer3_volume_a448080->AddNode(pVertexBarrelSupports_layer3_ladder13inner_volume_a449300, 164, pMatrix645);
    // Volume: VertexBarrelSupports_layer3_ladder14outer_volume
    TGeoVolume *pVertexBarrelSupports_layer3_ladder14outer_volume_a449448 = new TGeoVolume("VertexBarrelSupports_layer3_ladder14outer_volume",pVertexBarrelSupports_ladderB_78, pMed5);
    pVertexBarrelSupports_layer3_ladder14outer_volume_a449448->SetLineColor(16);
    pVertexBarrel_layer3_volume_a448080->AddNode(pVertexBarrelSupports_layer3_ladder14outer_volume_a449448, 165, pMatrix647);
    // Volume: VertexBarrelSupports_layer3_ladder15inner_volume
    TGeoVolume *pVertexBarrelSupports_layer3_ladder15inner_volume_a449590 = new TGeoVolume("VertexBarrelSupports_layer3_ladder15inner_volume",pVertexBarrelSupports_ladderB_78, pMed5);
    pVertexBarrelSupports_layer3_ladder15inner_volume_a449590->SetLineColor(16);
    pVertexBarrel_layer3_volume_a448080->AddNode(pVertexBarrelSupports_layer3_ladder15inner_volume_a449590, 166, pMatrix649);
    // Volume: VertexBarrelSupports_layer3_ladder16outer_volume
    TGeoVolume *pVertexBarrelSupports_layer3_ladder16outer_volume_a4496d8 = new TGeoVolume("VertexBarrelSupports_layer3_ladder16outer_volume",pVertexBarrelSupports_ladderB_78, pMed5);
    pVertexBarrelSupports_layer3_ladder16outer_volume_a4496d8->SetLineColor(16);
    pVertexBarrel_layer3_volume_a448080->AddNode(pVertexBarrelSupports_layer3_ladder16outer_volume_a4496d8, 167, pMatrix651);
    // Volume: VertexBarrelSupports_layer3_ladder17inner_volume
    TGeoVolume *pVertexBarrelSupports_layer3_ladder17inner_volume_a449820 = new TGeoVolume("VertexBarrelSupports_layer3_ladder17inner_volume",pVertexBarrelSupports_ladderB_78, pMed5);
    pVertexBarrelSupports_layer3_ladder17inner_volume_a449820->SetLineColor(16);
    pVertexBarrel_layer3_volume_a448080->AddNode(pVertexBarrelSupports_layer3_ladder17inner_volume_a449820, 168, pMatrix653);
    // Volume: VertexBarrelSupports_layer3_ladder18outer_volume
    TGeoVolume *pVertexBarrelSupports_layer3_ladder18outer_volume_a449968 = new TGeoVolume("VertexBarrelSupports_layer3_ladder18outer_volume",pVertexBarrelSupports_ladderB_78, pMed5);
    pVertexBarrelSupports_layer3_ladder18outer_volume_a449968->SetLineColor(16);
    pVertexBarrel_layer3_volume_a448080->AddNode(pVertexBarrelSupports_layer3_ladder18outer_volume_a449968, 169, pMatrix655);
    // Volume: VertexBarrelSupports_layer3_ladder19inner_volume
    TGeoVolume *pVertexBarrelSupports_layer3_ladder19inner_volume_a449ab0 = new TGeoVolume("VertexBarrelSupports_layer3_ladder19inner_volume",pVertexBarrelSupports_ladderB_78, pMed5);
    pVertexBarrelSupports_layer3_ladder19inner_volume_a449ab0->SetLineColor(16);
    pVertexBarrel_layer3_volume_a448080->AddNode(pVertexBarrelSupports_layer3_ladder19inner_volume_a449ab0, 170, pMatrix657);
    // Volume: VertexBarrelSupports_layer3_ladder20outer_volume
    TGeoVolume *pVertexBarrelSupports_layer3_ladder20outer_volume_a449bf8 = new TGeoVolume("VertexBarrelSupports_layer3_ladder20outer_volume",pVertexBarrelSupports_ladderB_78, pMed5);
    pVertexBarrelSupports_layer3_ladder20outer_volume_a449bf8->SetLineColor(16);
    pVertexBarrel_layer3_volume_a448080->AddNode(pVertexBarrelSupports_layer3_ladder20outer_volume_a449bf8, 171, pMatrix659);
    // Volume: VertexBarrelSupports_layer3_ladder21inner_volume
    TGeoVolume *pVertexBarrelSupports_layer3_ladder21inner_volume_a449d40 = new TGeoVolume("VertexBarrelSupports_layer3_ladder21inner_volume",pVertexBarrelSupports_ladderB_78, pMed5);
    pVertexBarrelSupports_layer3_ladder21inner_volume_a449d40->SetLineColor(16);
    pVertexBarrel_layer3_volume_a448080->AddNode(pVertexBarrelSupports_layer3_ladder21inner_volume_a449d40, 172, pMatrix661);
    // Volume: VertexBarrelSupports_layer3_ladder22outer_volume
    TGeoVolume *pVertexBarrelSupports_layer3_ladder22outer_volume_a449e88 = new TGeoVolume("VertexBarrelSupports_layer3_ladder22outer_volume",pVertexBarrelSupports_ladderB_78, pMed5);
    pVertexBarrelSupports_layer3_ladder22outer_volume_a449e88->SetLineColor(16);
    pVertexBarrel_layer3_volume_a448080->AddNode(pVertexBarrelSupports_layer3_ladder22outer_volume_a449e88, 173, pMatrix663);
    // Volume: VertexBarrelSupports_layer3_ladder23inner_volume
    TGeoVolume *pVertexBarrelSupports_layer3_ladder23inner_volume_a449fd0 = new TGeoVolume("VertexBarrelSupports_layer3_ladder23inner_volume",pVertexBarrelSupports_ladderB_78, pMed5);
    pVertexBarrelSupports_layer3_ladder23inner_volume_a449fd0->SetLineColor(16);
    pVertexBarrel_layer3_volume_a448080->AddNode(pVertexBarrelSupports_layer3_ladder23inner_volume_a449fd0, 174, pMatrix665);
    // Volume: VertexBarrel_layer3_ladder0outer_volume
    TGeoVolume *pVertexBarrel_layer3_ladder0outer_volume_a44a108 = new TGeoVolume("VertexBarrel_layer3_ladder0outer_volume",pVertexBarrel_ladderB_77, pMed8);
    pVertexBarrel_layer3_ladder0outer_volume_a44a108->SetLineColor(130);
    pVertexBarrel_layer3_volume_a448080->AddNode(pVertexBarrel_layer3_ladder0outer_volume_a44a108, 175, pMatrix667);
    // Volume: VertexBarrel_layer3_ladder1inner_volume
    TGeoVolume *pVertexBarrel_layer3_ladder1inner_volume_a44a468 = new TGeoVolume("VertexBarrel_layer3_ladder1inner_volume",pVertexBarrel_ladderB_77, pMed8);
    pVertexBarrel_layer3_ladder1inner_volume_a44a468->SetLineColor(130);
    pVertexBarrel_layer3_volume_a448080->AddNode(pVertexBarrel_layer3_ladder1inner_volume_a44a468, 176, pMatrix668);
    // Volume: VertexBarrel_layer3_ladder2outer_volume
    TGeoVolume *pVertexBarrel_layer3_ladder2outer_volume_a44a7c8 = new TGeoVolume("VertexBarrel_layer3_ladder2outer_volume",pVertexBarrel_ladderB_77, pMed8);
    pVertexBarrel_layer3_ladder2outer_volume_a44a7c8->SetLineColor(130);
    pVertexBarrel_layer3_volume_a448080->AddNode(pVertexBarrel_layer3_ladder2outer_volume_a44a7c8, 177, pMatrix670);
    // Volume: VertexBarrel_layer3_ladder3inner_volume
    TGeoVolume *pVertexBarrel_layer3_ladder3inner_volume_a44ab28 = new TGeoVolume("VertexBarrel_layer3_ladder3inner_volume",pVertexBarrel_ladderB_77, pMed8);
    pVertexBarrel_layer3_ladder3inner_volume_a44ab28->SetLineColor(130);
    pVertexBarrel_layer3_volume_a448080->AddNode(pVertexBarrel_layer3_ladder3inner_volume_a44ab28, 178, pMatrix672);
    // Volume: VertexBarrel_layer3_ladder4outer_volume
    TGeoVolume *pVertexBarrel_layer3_ladder4outer_volume_a44ae88 = new TGeoVolume("VertexBarrel_layer3_ladder4outer_volume",pVertexBarrel_ladderB_77, pMed8);
    pVertexBarrel_layer3_ladder4outer_volume_a44ae88->SetLineColor(130);
    pVertexBarrel_layer3_volume_a448080->AddNode(pVertexBarrel_layer3_ladder4outer_volume_a44ae88, 179, pMatrix674);
    // Volume: VertexBarrel_layer3_ladder5inner_volume
    TGeoVolume *pVertexBarrel_layer3_ladder5inner_volume_a44b1e8 = new TGeoVolume("VertexBarrel_layer3_ladder5inner_volume",pVertexBarrel_ladderB_77, pMed8);
    pVertexBarrel_layer3_ladder5inner_volume_a44b1e8->SetLineColor(130);
    pVertexBarrel_layer3_volume_a448080->AddNode(pVertexBarrel_layer3_ladder5inner_volume_a44b1e8, 180, pMatrix676);
    // Volume: VertexBarrel_layer3_ladder6outer_volume
    TGeoVolume *pVertexBarrel_layer3_ladder6outer_volume_a44b548 = new TGeoVolume("VertexBarrel_layer3_ladder6outer_volume",pVertexBarrel_ladderB_77, pMed8);
    pVertexBarrel_layer3_ladder6outer_volume_a44b548->SetLineColor(130);
    pVertexBarrel_layer3_volume_a448080->AddNode(pVertexBarrel_layer3_ladder6outer_volume_a44b548, 181, pMatrix678);
    // Volume: VertexBarrel_layer3_ladder7inner_volume
    TGeoVolume *pVertexBarrel_layer3_ladder7inner_volume_a44b8a8 = new TGeoVolume("VertexBarrel_layer3_ladder7inner_volume",pVertexBarrel_ladderB_77, pMed8);
    pVertexBarrel_layer3_ladder7inner_volume_a44b8a8->SetLineColor(130);
    pVertexBarrel_layer3_volume_a448080->AddNode(pVertexBarrel_layer3_ladder7inner_volume_a44b8a8, 182, pMatrix680);
    // Volume: VertexBarrel_layer3_ladder8outer_volume
    TGeoVolume *pVertexBarrel_layer3_ladder8outer_volume_a44bc08 = new TGeoVolume("VertexBarrel_layer3_ladder8outer_volume",pVertexBarrel_ladderB_77, pMed8);
    pVertexBarrel_layer3_ladder8outer_volume_a44bc08->SetLineColor(130);
    pVertexBarrel_layer3_volume_a448080->AddNode(pVertexBarrel_layer3_ladder8outer_volume_a44bc08, 183, pMatrix682);
    // Volume: VertexBarrel_layer3_ladder9inner_volume
    TGeoVolume *pVertexBarrel_layer3_ladder9inner_volume_a44bf68 = new TGeoVolume("VertexBarrel_layer3_ladder9inner_volume",pVertexBarrel_ladderB_77, pMed8);
    pVertexBarrel_layer3_ladder9inner_volume_a44bf68->SetLineColor(130);
    pVertexBarrel_layer3_volume_a448080->AddNode(pVertexBarrel_layer3_ladder9inner_volume_a44bf68, 184, pMatrix684);
    // Volume: VertexBarrel_layer3_ladder10outer_volume
    TGeoVolume *pVertexBarrel_layer3_ladder10outer_volume_a44c2d0 = new TGeoVolume("VertexBarrel_layer3_ladder10outer_volume",pVertexBarrel_ladderB_77, pMed8);
    pVertexBarrel_layer3_ladder10outer_volume_a44c2d0->SetLineColor(130);
    pVertexBarrel_layer3_volume_a448080->AddNode(pVertexBarrel_layer3_ladder10outer_volume_a44c2d0, 185, pMatrix686);
    // Volume: VertexBarrel_layer3_ladder11inner_volume
    TGeoVolume *pVertexBarrel_layer3_ladder11inner_volume_a44c638 = new TGeoVolume("VertexBarrel_layer3_ladder11inner_volume",pVertexBarrel_ladderB_77, pMed8);
    pVertexBarrel_layer3_ladder11inner_volume_a44c638->SetLineColor(130);
    pVertexBarrel_layer3_volume_a448080->AddNode(pVertexBarrel_layer3_ladder11inner_volume_a44c638, 186, pMatrix688);
    // Volume: VertexBarrel_layer3_ladder12outer_volume
    TGeoVolume *pVertexBarrel_layer3_ladder12outer_volume_a44c9a0 = new TGeoVolume("VertexBarrel_layer3_ladder12outer_volume",pVertexBarrel_ladderB_77, pMed8);
    pVertexBarrel_layer3_ladder12outer_volume_a44c9a0->SetLineColor(130);
    pVertexBarrel_layer3_volume_a448080->AddNode(pVertexBarrel_layer3_ladder12outer_volume_a44c9a0, 187, pMatrix690);
    // Volume: VertexBarrel_layer3_ladder13inner_volume
    TGeoVolume *pVertexBarrel_layer3_ladder13inner_volume_a44cd08 = new TGeoVolume("VertexBarrel_layer3_ladder13inner_volume",pVertexBarrel_ladderB_77, pMed8);
    pVertexBarrel_layer3_ladder13inner_volume_a44cd08->SetLineColor(130);
    pVertexBarrel_layer3_volume_a448080->AddNode(pVertexBarrel_layer3_ladder13inner_volume_a44cd08, 188, pMatrix692);
    // Volume: VertexBarrel_layer3_ladder14outer_volume
    TGeoVolume *pVertexBarrel_layer3_ladder14outer_volume_a44d070 = new TGeoVolume("VertexBarrel_layer3_ladder14outer_volume",pVertexBarrel_ladderB_77, pMed8);
    pVertexBarrel_layer3_ladder14outer_volume_a44d070->SetLineColor(130);
    pVertexBarrel_layer3_volume_a448080->AddNode(pVertexBarrel_layer3_ladder14outer_volume_a44d070, 189, pMatrix694);
    // Volume: VertexBarrel_layer3_ladder15inner_volume
    TGeoVolume *pVertexBarrel_layer3_ladder15inner_volume_a44d3d8 = new TGeoVolume("VertexBarrel_layer3_ladder15inner_volume",pVertexBarrel_ladderB_77, pMed8);
    pVertexBarrel_layer3_ladder15inner_volume_a44d3d8->SetLineColor(130);
    pVertexBarrel_layer3_volume_a448080->AddNode(pVertexBarrel_layer3_ladder15inner_volume_a44d3d8, 190, pMatrix696);
    // Volume: VertexBarrel_layer3_ladder16outer_volume
    TGeoVolume *pVertexBarrel_layer3_ladder16outer_volume_a44d740 = new TGeoVolume("VertexBarrel_layer3_ladder16outer_volume",pVertexBarrel_ladderB_77, pMed8);
    pVertexBarrel_layer3_ladder16outer_volume_a44d740->SetLineColor(130);
    pVertexBarrel_layer3_volume_a448080->AddNode(pVertexBarrel_layer3_ladder16outer_volume_a44d740, 191, pMatrix698);
    // Volume: VertexBarrel_layer3_ladder17inner_volume
    TGeoVolume *pVertexBarrel_layer3_ladder17inner_volume_a44daa8 = new TGeoVolume("VertexBarrel_layer3_ladder17inner_volume",pVertexBarrel_ladderB_77, pMed8);
    pVertexBarrel_layer3_ladder17inner_volume_a44daa8->SetLineColor(130);
    pVertexBarrel_layer3_volume_a448080->AddNode(pVertexBarrel_layer3_ladder17inner_volume_a44daa8, 192, pMatrix700);
    // Volume: VertexBarrel_layer3_ladder18outer_volume
    TGeoVolume *pVertexBarrel_layer3_ladder18outer_volume_a44de10 = new TGeoVolume("VertexBarrel_layer3_ladder18outer_volume",pVertexBarrel_ladderB_77, pMed8);
    pVertexBarrel_layer3_ladder18outer_volume_a44de10->SetLineColor(130);
    pVertexBarrel_layer3_volume_a448080->AddNode(pVertexBarrel_layer3_ladder18outer_volume_a44de10, 193, pMatrix702);
    // Volume: VertexBarrel_layer3_ladder19inner_volume
    TGeoVolume *pVertexBarrel_layer3_ladder19inner_volume_a44e178 = new TGeoVolume("VertexBarrel_layer3_ladder19inner_volume",pVertexBarrel_ladderB_77, pMed8);
    pVertexBarrel_layer3_ladder19inner_volume_a44e178->SetLineColor(130);
    pVertexBarrel_layer3_volume_a448080->AddNode(pVertexBarrel_layer3_ladder19inner_volume_a44e178, 194, pMatrix704);
    // Volume: VertexBarrel_layer3_ladder20outer_volume
    TGeoVolume *pVertexBarrel_layer3_ladder20outer_volume_a44e4e0 = new TGeoVolume("VertexBarrel_layer3_ladder20outer_volume",pVertexBarrel_ladderB_77, pMed8);
    pVertexBarrel_layer3_ladder20outer_volume_a44e4e0->SetLineColor(130);
    pVertexBarrel_layer3_volume_a448080->AddNode(pVertexBarrel_layer3_ladder20outer_volume_a44e4e0, 195, pMatrix706);
    // Volume: VertexBarrel_layer3_ladder21inner_volume
    TGeoVolume *pVertexBarrel_layer3_ladder21inner_volume_a44e848 = new TGeoVolume("VertexBarrel_layer3_ladder21inner_volume",pVertexBarrel_ladderB_77, pMed8);
    pVertexBarrel_layer3_ladder21inner_volume_a44e848->SetLineColor(130);
    pVertexBarrel_layer3_volume_a448080->AddNode(pVertexBarrel_layer3_ladder21inner_volume_a44e848, 196, pMatrix708);
    // Volume: VertexBarrel_layer3_ladder22outer_volume
    TGeoVolume *pVertexBarrel_layer3_ladder22outer_volume_a44ebb0 = new TGeoVolume("VertexBarrel_layer3_ladder22outer_volume",pVertexBarrel_ladderB_77, pMed8);
    pVertexBarrel_layer3_ladder22outer_volume_a44ebb0->SetLineColor(130);
    pVertexBarrel_layer3_volume_a448080->AddNode(pVertexBarrel_layer3_ladder22outer_volume_a44ebb0, 197, pMatrix710);
    // Volume: VertexBarrel_layer3_ladder23inner_volume
    TGeoVolume *pVertexBarrel_layer3_ladder23inner_volume_a44ef18 = new TGeoVolume("VertexBarrel_layer3_ladder23inner_volume",pVertexBarrel_ladderB_77, pMed8);
    pVertexBarrel_layer3_ladder23inner_volume_a44ef18->SetLineColor(130);
    
    pVertexBarrel_layer3_volume_a448080->AddNode(pVertexBarrel_layer3_ladder23inner_volume_a44ef18, 198, pMatrix712);
    
    pVertexBarrel_layer3_ladder0outer_volume_a44a108->AddNode(pVertexBarrel_detB_volume_a3ddc60, 127);
    pVertexBarrel_layer3_ladder1inner_volume_a44a468->AddNode(pVertexBarrel_detB_volume_a3ddc60, 128);
    pVertexBarrel_layer3_ladder2outer_volume_a44a7c8->AddNode(pVertexBarrel_detB_volume_a3ddc60, 129);
    pVertexBarrel_layer3_ladder3inner_volume_a44ab28->AddNode(pVertexBarrel_detB_volume_a3ddc60, 130);
    pVertexBarrel_layer3_ladder4outer_volume_a44ae88->AddNode(pVertexBarrel_detB_volume_a3ddc60, 131);
    pVertexBarrel_layer3_ladder5inner_volume_a44b1e8->AddNode(pVertexBarrel_detB_volume_a3ddc60, 132);
    pVertexBarrel_layer3_ladder6outer_volume_a44b548->AddNode(pVertexBarrel_detB_volume_a3ddc60, 133);
    pVertexBarrel_layer3_ladder7inner_volume_a44b8a8->AddNode(pVertexBarrel_detB_volume_a3ddc60, 134);
    pVertexBarrel_layer3_ladder8outer_volume_a44bc08->AddNode(pVertexBarrel_detB_volume_a3ddc60, 135);
    pVertexBarrel_layer3_ladder9inner_volume_a44bf68->AddNode(pVertexBarrel_detB_volume_a3ddc60, 136);
    pVertexBarrel_layer3_ladder10outer_volume_a44c2d0->AddNode(pVertexBarrel_detB_volume_a3ddc60, 137);
    pVertexBarrel_layer3_ladder11inner_volume_a44c638->AddNode(pVertexBarrel_detB_volume_a3ddc60, 138);
    pVertexBarrel_layer3_ladder12outer_volume_a44c9a0->AddNode(pVertexBarrel_detB_volume_a3ddc60, 139);
    pVertexBarrel_layer3_ladder13inner_volume_a44cd08->AddNode(pVertexBarrel_detB_volume_a3ddc60, 140);
    pVertexBarrel_layer3_ladder14outer_volume_a44d070->AddNode(pVertexBarrel_detB_volume_a3ddc60, 141);
    pVertexBarrel_layer3_ladder15inner_volume_a44d3d8->AddNode(pVertexBarrel_detB_volume_a3ddc60, 142);
    pVertexBarrel_layer3_ladder16outer_volume_a44d740->AddNode(pVertexBarrel_detB_volume_a3ddc60, 143);
    pVertexBarrel_layer3_ladder17inner_volume_a44daa8->AddNode(pVertexBarrel_detB_volume_a3ddc60, 144);
    pVertexBarrel_layer3_ladder18outer_volume_a44de10->AddNode(pVertexBarrel_detB_volume_a3ddc60, 145);
    pVertexBarrel_layer3_ladder19inner_volume_a44e178->AddNode(pVertexBarrel_detB_volume_a3ddc60, 146);
    pVertexBarrel_layer3_ladder20outer_volume_a44e4e0->AddNode(pVertexBarrel_detB_volume_a3ddc60, 147);
    pVertexBarrel_layer3_ladder21inner_volume_a44e848->AddNode(pVertexBarrel_detB_volume_a3ddc60, 148);
    pVertexBarrel_layer3_ladder22outer_volume_a44ebb0->AddNode(pVertexBarrel_detB_volume_a3ddc60, 149);
    pVertexBarrel_layer3_ladder23inner_volume_a44ef18->AddNode(pVertexBarrel_detB_volume_a3ddc60, 150);
    // Volume: VertexBarrelSupports_layer4_ladder0outer_volume
    
    TGeoVolume *pVertexBarrelSupports_layer4_ladder0outer_volume_a451548 = new TGeoVolume("VertexBarrelSupports_layer4_ladder0outer_volume",pVertexBarrelSupports_ladderB_78, pMed5);
    pVertexBarrelSupports_layer4_ladder0outer_volume_a451548->SetLineColor(16);
    pVertexBarrel_layer4_volume_a4512f8->AddNode(pVertexBarrelSupports_layer4_ladder0outer_volume_a451548, 229, pMatrix738);
    // Volume: VertexBarrelSupports_layer4_ladder1inner_volume
    TGeoVolume *pVertexBarrelSupports_layer4_ladder1inner_volume_a451688 = new TGeoVolume("VertexBarrelSupports_layer4_ladder1inner_volume",pVertexBarrelSupports_ladderB_78, pMed5);
    pVertexBarrelSupports_layer4_ladder1inner_volume_a451688->SetLineColor(16);
    pVertexBarrel_layer4_volume_a4512f8->AddNode(pVertexBarrelSupports_layer4_ladder1inner_volume_a451688, 230, pMatrix739);
    // Volume: VertexBarrelSupports_layer4_ladder2outer_volume
    TGeoVolume *pVertexBarrelSupports_layer4_ladder2outer_volume_a4517c8 = new TGeoVolume("VertexBarrelSupports_layer4_ladder2outer_volume",pVertexBarrelSupports_ladderB_78, pMed5);
    pVertexBarrelSupports_layer4_ladder2outer_volume_a4517c8->SetLineColor(16);
    pVertexBarrel_layer4_volume_a4512f8->AddNode(pVertexBarrelSupports_layer4_ladder2outer_volume_a4517c8, 231, pMatrix741);
    // Volume: VertexBarrelSupports_layer4_ladder3inner_volume
    TGeoVolume *pVertexBarrelSupports_layer4_ladder3inner_volume_a451908 = new TGeoVolume("VertexBarrelSupports_layer4_ladder3inner_volume",pVertexBarrelSupports_ladderB_78, pMed5);
    pVertexBarrelSupports_layer4_ladder3inner_volume_a451908->SetLineColor(16);
    pVertexBarrel_layer4_volume_a4512f8->AddNode(pVertexBarrelSupports_layer4_ladder3inner_volume_a451908, 232, pMatrix743);
    // Volume: VertexBarrelSupports_layer4_ladder4outer_volume
    TGeoVolume *pVertexBarrelSupports_layer4_ladder4outer_volume_a451a48 = new TGeoVolume("VertexBarrelSupports_layer4_ladder4outer_volume",pVertexBarrelSupports_ladderB_78, pMed5);
    pVertexBarrelSupports_layer4_ladder4outer_volume_a451a48->SetLineColor(16);
    pVertexBarrel_layer4_volume_a4512f8->AddNode(pVertexBarrelSupports_layer4_ladder4outer_volume_a451a48, 233, pMatrix745);
    // Volume: VertexBarrelSupports_layer4_ladder5inner_volume
    TGeoVolume *pVertexBarrelSupports_layer4_ladder5inner_volume_a451b88 = new TGeoVolume("VertexBarrelSupports_layer4_ladder5inner_volume",pVertexBarrelSupports_ladderB_78, pMed5);
    pVertexBarrelSupports_layer4_ladder5inner_volume_a451b88->SetLineColor(16);
    pVertexBarrel_layer4_volume_a4512f8->AddNode(pVertexBarrelSupports_layer4_ladder5inner_volume_a451b88, 234, pMatrix747);
    // Volume: VertexBarrelSupports_layer4_ladder6outer_volume
    TGeoVolume *pVertexBarrelSupports_layer4_ladder6outer_volume_a451cc8 = new TGeoVolume("VertexBarrelSupports_layer4_ladder6outer_volume",pVertexBarrelSupports_ladderB_78, pMed5);
    pVertexBarrelSupports_layer4_ladder6outer_volume_a451cc8->SetLineColor(16);
    pVertexBarrel_layer4_volume_a4512f8->AddNode(pVertexBarrelSupports_layer4_ladder6outer_volume_a451cc8, 235, pMatrix749);
    // Volume: VertexBarrelSupports_layer4_ladder7inner_volume
    TGeoVolume *pVertexBarrelSupports_layer4_ladder7inner_volume_a451e08 = new TGeoVolume("VertexBarrelSupports_layer4_ladder7inner_volume",pVertexBarrelSupports_ladderB_78, pMed5);
    pVertexBarrelSupports_layer4_ladder7inner_volume_a451e08->SetLineColor(16);
    pVertexBarrel_layer4_volume_a4512f8->AddNode(pVertexBarrelSupports_layer4_ladder7inner_volume_a451e08, 236, pMatrix751);
    // Volume: VertexBarrelSupports_layer4_ladder8outer_volume
    TGeoVolume *pVertexBarrelSupports_layer4_ladder8outer_volume_a451f48 = new TGeoVolume("VertexBarrelSupports_layer4_ladder8outer_volume",pVertexBarrelSupports_ladderB_78, pMed5);
    pVertexBarrelSupports_layer4_ladder8outer_volume_a451f48->SetLineColor(16);
    pVertexBarrel_layer4_volume_a4512f8->AddNode(pVertexBarrelSupports_layer4_ladder8outer_volume_a451f48, 237, pMatrix753);
    // Volume: VertexBarrelSupports_layer4_ladder9inner_volume
    TGeoVolume *pVertexBarrelSupports_layer4_ladder9inner_volume_a452088 = new TGeoVolume("VertexBarrelSupports_layer4_ladder9inner_volume",pVertexBarrelSupports_ladderB_78, pMed5);
    pVertexBarrelSupports_layer4_ladder9inner_volume_a452088->SetLineColor(16);
    pVertexBarrel_layer4_volume_a4512f8->AddNode(pVertexBarrelSupports_layer4_ladder9inner_volume_a452088, 238, pMatrix755);
    // Volume: VertexBarrelSupports_layer4_ladder10outer_volume
    TGeoVolume *pVertexBarrelSupports_layer4_ladder10outer_volume_a4521d0 = new TGeoVolume("VertexBarrelSupports_layer4_ladder10outer_volume",pVertexBarrelSupports_ladderB_78, pMed5);
    pVertexBarrelSupports_layer4_ladder10outer_volume_a4521d0->SetLineColor(16);
    pVertexBarrel_layer4_volume_a4512f8->AddNode(pVertexBarrelSupports_layer4_ladder10outer_volume_a4521d0, 239, pMatrix757);
    // Volume: VertexBarrelSupports_layer4_ladder11inner_volume
    TGeoVolume *pVertexBarrelSupports_layer4_ladder11inner_volume_a452318 = new TGeoVolume("VertexBarrelSupports_layer4_ladder11inner_volume",pVertexBarrelSupports_ladderB_78, pMed5);
    pVertexBarrelSupports_layer4_ladder11inner_volume_a452318->SetLineColor(16);
    pVertexBarrel_layer4_volume_a4512f8->AddNode(pVertexBarrelSupports_layer4_ladder11inner_volume_a452318, 240, pMatrix759);
    // Volume: VertexBarrelSupports_layer4_ladder12outer_volume
    TGeoVolume *pVertexBarrelSupports_layer4_ladder12outer_volume_a452460 = new TGeoVolume("VertexBarrelSupports_layer4_ladder12outer_volume",pVertexBarrelSupports_ladderB_78, pMed5);
    pVertexBarrelSupports_layer4_ladder12outer_volume_a452460->SetLineColor(16);
    pVertexBarrel_layer4_volume_a4512f8->AddNode(pVertexBarrelSupports_layer4_ladder12outer_volume_a452460, 241, pMatrix761);
    // Volume: VertexBarrelSupports_layer4_ladder13inner_volume
    TGeoVolume *pVertexBarrelSupports_layer4_ladder13inner_volume_a4525a8 = new TGeoVolume("VertexBarrelSupports_layer4_ladder13inner_volume",pVertexBarrelSupports_ladderB_78, pMed5);
    pVertexBarrelSupports_layer4_ladder13inner_volume_a4525a8->SetLineColor(16);
    pVertexBarrel_layer4_volume_a4512f8->AddNode(pVertexBarrelSupports_layer4_ladder13inner_volume_a4525a8, 242, pMatrix763);
    // Volume: VertexBarrelSupports_layer4_ladder14outer_volume
    TGeoVolume *pVertexBarrelSupports_layer4_ladder14outer_volume_a4526f0 = new TGeoVolume("VertexBarrelSupports_layer4_ladder14outer_volume",pVertexBarrelSupports_ladderB_78, pMed5);
    pVertexBarrelSupports_layer4_ladder14outer_volume_a4526f0->SetLineColor(16);
    pVertexBarrel_layer4_volume_a4512f8->AddNode(pVertexBarrelSupports_layer4_ladder14outer_volume_a4526f0, 243, pMatrix765);
    // Volume: VertexBarrelSupports_layer4_ladder15inner_volume
    TGeoVolume *pVertexBarrelSupports_layer4_ladder15inner_volume_a452838 = new TGeoVolume("VertexBarrelSupports_layer4_ladder15inner_volume",pVertexBarrelSupports_ladderB_78, pMed5);
    pVertexBarrelSupports_layer4_ladder15inner_volume_a452838->SetLineColor(16);
    pVertexBarrel_layer4_volume_a4512f8->AddNode(pVertexBarrelSupports_layer4_ladder15inner_volume_a452838, 244, pMatrix767);
    // Volume: VertexBarrelSupports_layer4_ladder16outer_volume
    TGeoVolume *pVertexBarrelSupports_layer4_ladder16outer_volume_a452980 = new TGeoVolume("VertexBarrelSupports_layer4_ladder16outer_volume",pVertexBarrelSupports_ladderB_78, pMed5);
    pVertexBarrelSupports_layer4_ladder16outer_volume_a452980->SetLineColor(16);
    pVertexBarrel_layer4_volume_a4512f8->AddNode(pVertexBarrelSupports_layer4_ladder16outer_volume_a452980, 245, pMatrix769);
    // Volume: VertexBarrelSupports_layer4_ladder17inner_volume
    TGeoVolume *pVertexBarrelSupports_layer4_ladder17inner_volume_a452ac8 = new TGeoVolume("VertexBarrelSupports_layer4_ladder17inner_volume",pVertexBarrelSupports_ladderB_78, pMed5);
    pVertexBarrelSupports_layer4_ladder17inner_volume_a452ac8->SetLineColor(16);
    pVertexBarrel_layer4_volume_a4512f8->AddNode(pVertexBarrelSupports_layer4_ladder17inner_volume_a452ac8, 246, pMatrix771);
    // Volume: VertexBarrelSupports_layer4_ladder18outer_volume
    TGeoVolume *pVertexBarrelSupports_layer4_ladder18outer_volume_a452c10 = new TGeoVolume("VertexBarrelSupports_layer4_ladder18outer_volume",pVertexBarrelSupports_ladderB_78, pMed5);
    pVertexBarrelSupports_layer4_ladder18outer_volume_a452c10->SetLineColor(16);
    pVertexBarrel_layer4_volume_a4512f8->AddNode(pVertexBarrelSupports_layer4_ladder18outer_volume_a452c10, 247, pMatrix773);
    // Volume: VertexBarrelSupports_layer4_ladder19inner_volume
    TGeoVolume *pVertexBarrelSupports_layer4_ladder19inner_volume_a452d58 = new TGeoVolume("VertexBarrelSupports_layer4_ladder19inner_volume",pVertexBarrelSupports_ladderB_78, pMed5);
    pVertexBarrelSupports_layer4_ladder19inner_volume_a452d58->SetLineColor(16);
    pVertexBarrel_layer4_volume_a4512f8->AddNode(pVertexBarrelSupports_layer4_ladder19inner_volume_a452d58, 248, pMatrix775);
    // Volume: VertexBarrelSupports_layer4_ladder20outer_volume
    TGeoVolume *pVertexBarrelSupports_layer4_ladder20outer_volume_a452ea0 = new TGeoVolume("VertexBarrelSupports_layer4_ladder20outer_volume",pVertexBarrelSupports_ladderB_78, pMed5);
    pVertexBarrelSupports_layer4_ladder20outer_volume_a452ea0->SetLineColor(16);
    pVertexBarrel_layer4_volume_a4512f8->AddNode(pVertexBarrelSupports_layer4_ladder20outer_volume_a452ea0, 249, pMatrix777);
    // Volume: VertexBarrelSupports_layer4_ladder21inner_volume
    TGeoVolume *pVertexBarrelSupports_layer4_ladder21inner_volume_a452fe8 = new TGeoVolume("VertexBarrelSupports_layer4_ladder21inner_volume",pVertexBarrelSupports_ladderB_78, pMed5);
    pVertexBarrelSupports_layer4_ladder21inner_volume_a452fe8->SetLineColor(16);
    pVertexBarrel_layer4_volume_a4512f8->AddNode(pVertexBarrelSupports_layer4_ladder21inner_volume_a452fe8, 250, pMatrix779);
    // Volume: VertexBarrelSupports_layer4_ladder22outer_volume
    TGeoVolume *pVertexBarrelSupports_layer4_ladder22outer_volume_a453130 = new TGeoVolume("VertexBarrelSupports_layer4_ladder22outer_volume",pVertexBarrelSupports_ladderB_78, pMed5);
    pVertexBarrelSupports_layer4_ladder22outer_volume_a453130->SetLineColor(16);
    pVertexBarrel_layer4_volume_a4512f8->AddNode(pVertexBarrelSupports_layer4_ladder22outer_volume_a453130, 251, pMatrix781);
    // Volume: VertexBarrelSupports_layer4_ladder23inner_volume
    TGeoVolume *pVertexBarrelSupports_layer4_ladder23inner_volume_a453278 = new TGeoVolume("VertexBarrelSupports_layer4_ladder23inner_volume",pVertexBarrelSupports_ladderB_78, pMed5);
    pVertexBarrelSupports_layer4_ladder23inner_volume_a453278->SetLineColor(16);
    pVertexBarrel_layer4_volume_a4512f8->AddNode(pVertexBarrelSupports_layer4_ladder23inner_volume_a453278, 252, pMatrix783);
    // Volume: VertexBarrelSupports_layer4_ladder24outer_volume
    TGeoVolume *pVertexBarrelSupports_layer4_ladder24outer_volume_a4533c0 = new TGeoVolume("VertexBarrelSupports_layer4_ladder24outer_volume",pVertexBarrelSupports_ladderB_78, pMed5);
    pVertexBarrelSupports_layer4_ladder24outer_volume_a4533c0->SetLineColor(16);
    pVertexBarrel_layer4_volume_a4512f8->AddNode(pVertexBarrelSupports_layer4_ladder24outer_volume_a4533c0, 253, pMatrix785);
    // Volume: VertexBarrelSupports_layer4_ladder25inner_volume
    TGeoVolume *pVertexBarrelSupports_layer4_ladder25inner_volume_a453508 = new TGeoVolume("VertexBarrelSupports_layer4_ladder25inner_volume",pVertexBarrelSupports_ladderB_78, pMed5);
    pVertexBarrelSupports_layer4_ladder25inner_volume_a453508->SetLineColor(16);
    pVertexBarrel_layer4_volume_a4512f8->AddNode(pVertexBarrelSupports_layer4_ladder25inner_volume_a453508, 254, pMatrix787);
    // Volume: VertexBarrelSupports_layer4_ladder26outer_volume
    TGeoVolume *pVertexBarrelSupports_layer4_ladder26outer_volume_a453650 = new TGeoVolume("VertexBarrelSupports_layer4_ladder26outer_volume",pVertexBarrelSupports_ladderB_78, pMed5);
    pVertexBarrelSupports_layer4_ladder26outer_volume_a453650->SetLineColor(16);
    pVertexBarrel_layer4_volume_a4512f8->AddNode(pVertexBarrelSupports_layer4_ladder26outer_volume_a453650, 255, pMatrix789);
    // Volume: VertexBarrelSupports_layer4_ladder27inner_volume
    TGeoVolume *pVertexBarrelSupports_layer4_ladder27inner_volume_a453798 = new TGeoVolume("VertexBarrelSupports_layer4_ladder27inner_volume",pVertexBarrelSupports_ladderB_78, pMed5);
    pVertexBarrelSupports_layer4_ladder27inner_volume_a453798->SetLineColor(16);
    pVertexBarrel_layer4_volume_a4512f8->AddNode(pVertexBarrelSupports_layer4_ladder27inner_volume_a453798, 256, pMatrix791);
    // Volume: VertexBarrelSupports_layer4_ladder28outer_volume
    TGeoVolume *pVertexBarrelSupports_layer4_ladder28outer_volume_a4538e0 = new TGeoVolume("VertexBarrelSupports_layer4_ladder28outer_volume",pVertexBarrelSupports_ladderB_78, pMed5);
    pVertexBarrelSupports_layer4_ladder28outer_volume_a4538e0->SetLineColor(16);
    pVertexBarrel_layer4_volume_a4512f8->AddNode(pVertexBarrelSupports_layer4_ladder28outer_volume_a4538e0, 257, pMatrix793);
    // Volume: VertexBarrelSupports_layer4_ladder29inner_volume
    TGeoVolume *pVertexBarrelSupports_layer4_ladder29inner_volume_a453a28 = new TGeoVolume("VertexBarrelSupports_layer4_ladder29inner_volume",pVertexBarrelSupports_ladderB_78, pMed5);
    pVertexBarrelSupports_layer4_ladder29inner_volume_a453a28->SetLineColor(16);
    pVertexBarrel_layer4_volume_a4512f8->AddNode(pVertexBarrelSupports_layer4_ladder29inner_volume_a453a28, 258, pMatrix795);
    // Volume: VertexBarrel_layer4_ladder0outer_volume
    TGeoVolume *pVertexBarrel_layer4_ladder0outer_volume_a453b60 = new TGeoVolume("VertexBarrel_layer4_ladder0outer_volume",pVertexBarrel_ladderB_77, pMed8);
    pVertexBarrel_layer4_ladder0outer_volume_a453b60->SetLineColor(130);
    pVertexBarrel_layer4_volume_a4512f8->AddNode(pVertexBarrel_layer4_ladder0outer_volume_a453b60, 259, pMatrix797);
    // Volume: VertexBarrel_layer4_ladder1inner_volume
    TGeoVolume *pVertexBarrel_layer4_ladder1inner_volume_a453ec0 = new TGeoVolume("VertexBarrel_layer4_ladder1inner_volume",pVertexBarrel_ladderB_77, pMed8);
    pVertexBarrel_layer4_ladder1inner_volume_a453ec0->SetLineColor(130);
    pVertexBarrel_layer4_volume_a4512f8->AddNode(pVertexBarrel_layer4_ladder1inner_volume_a453ec0, 260, pMatrix798);
    // Volume: VertexBarrel_layer4_ladder2outer_volume
    TGeoVolume *pVertexBarrel_layer4_ladder2outer_volume_a454220 = new TGeoVolume("VertexBarrel_layer4_ladder2outer_volume",pVertexBarrel_ladderB_77, pMed8);
    pVertexBarrel_layer4_ladder2outer_volume_a454220->SetLineColor(130);
    pVertexBarrel_layer4_volume_a4512f8->AddNode(pVertexBarrel_layer4_ladder2outer_volume_a454220, 261, pMatrix800);
    // Volume: VertexBarrel_layer4_ladder3inner_volume
    TGeoVolume *pVertexBarrel_layer4_ladder3inner_volume_a454580 = new TGeoVolume("VertexBarrel_layer4_ladder3inner_volume",pVertexBarrel_ladderB_77, pMed8);
    pVertexBarrel_layer4_ladder3inner_volume_a454580->SetLineColor(130);
    pVertexBarrel_layer4_volume_a4512f8->AddNode(pVertexBarrel_layer4_ladder3inner_volume_a454580, 262, pMatrix802);
    // Volume: VertexBarrel_layer4_ladder4outer_volume
    TGeoVolume *pVertexBarrel_layer4_ladder4outer_volume_a4548e0 = new TGeoVolume("VertexBarrel_layer4_ladder4outer_volume",pVertexBarrel_ladderB_77, pMed8);
    pVertexBarrel_layer4_ladder4outer_volume_a4548e0->SetLineColor(130);
    pVertexBarrel_layer4_volume_a4512f8->AddNode(pVertexBarrel_layer4_ladder4outer_volume_a4548e0, 263, pMatrix804);
    // Volume: VertexBarrel_layer4_ladder5inner_volume
    TGeoVolume *pVertexBarrel_layer4_ladder5inner_volume_a454c40 = new TGeoVolume("VertexBarrel_layer4_ladder5inner_volume",pVertexBarrel_ladderB_77, pMed8);
    pVertexBarrel_layer4_ladder5inner_volume_a454c40->SetLineColor(130);
    pVertexBarrel_layer4_volume_a4512f8->AddNode(pVertexBarrel_layer4_ladder5inner_volume_a454c40, 264, pMatrix806);
    // Volume: VertexBarrel_layer4_ladder6outer_volume
    TGeoVolume *pVertexBarrel_layer4_ladder6outer_volume_a454fa0 = new TGeoVolume("VertexBarrel_layer4_ladder6outer_volume",pVertexBarrel_ladderB_77, pMed8);
    pVertexBarrel_layer4_ladder6outer_volume_a454fa0->SetLineColor(130);
    pVertexBarrel_layer4_volume_a4512f8->AddNode(pVertexBarrel_layer4_ladder6outer_volume_a454fa0, 265, pMatrix808);
    // Volume: VertexBarrel_layer4_ladder7inner_volume
    TGeoVolume *pVertexBarrel_layer4_ladder7inner_volume_a455300 = new TGeoVolume("VertexBarrel_layer4_ladder7inner_volume",pVertexBarrel_ladderB_77, pMed8);
    pVertexBarrel_layer4_ladder7inner_volume_a455300->SetLineColor(130);
    pVertexBarrel_layer4_volume_a4512f8->AddNode(pVertexBarrel_layer4_ladder7inner_volume_a455300, 266, pMatrix810);
    // Volume: VertexBarrel_layer4_ladder8outer_volume
    TGeoVolume *pVertexBarrel_layer4_ladder8outer_volume_a455660 = new TGeoVolume("VertexBarrel_layer4_ladder8outer_volume",pVertexBarrel_ladderB_77, pMed8);
    pVertexBarrel_layer4_ladder8outer_volume_a455660->SetLineColor(130);
    pVertexBarrel_layer4_volume_a4512f8->AddNode(pVertexBarrel_layer4_ladder8outer_volume_a455660, 267, pMatrix812);
    // Volume: VertexBarrel_layer4_ladder9inner_volume
    TGeoVolume *pVertexBarrel_layer4_ladder9inner_volume_a4559c0 = new TGeoVolume("VertexBarrel_layer4_ladder9inner_volume",pVertexBarrel_ladderB_77, pMed8);
    pVertexBarrel_layer4_ladder9inner_volume_a4559c0->SetLineColor(130);
    pVertexBarrel_layer4_volume_a4512f8->AddNode(pVertexBarrel_layer4_ladder9inner_volume_a4559c0, 268, pMatrix814);
    // Volume: VertexBarrel_layer4_ladder10outer_volume
    TGeoVolume *pVertexBarrel_layer4_ladder10outer_volume_a455d28 = new TGeoVolume("VertexBarrel_layer4_ladder10outer_volume",pVertexBarrel_ladderB_77, pMed8);
    pVertexBarrel_layer4_ladder10outer_volume_a455d28->SetLineColor(130);
    pVertexBarrel_layer4_volume_a4512f8->AddNode(pVertexBarrel_layer4_ladder10outer_volume_a455d28, 269, pMatrix816);
    // Volume: VertexBarrel_layer4_ladder11inner_volume
    TGeoVolume *pVertexBarrel_layer4_ladder11inner_volume_a456090 = new TGeoVolume("VertexBarrel_layer4_ladder11inner_volume",pVertexBarrel_ladderB_77, pMed8);
    pVertexBarrel_layer4_ladder11inner_volume_a456090->SetLineColor(130);
    pVertexBarrel_layer4_volume_a4512f8->AddNode(pVertexBarrel_layer4_ladder11inner_volume_a456090, 270, pMatrix818);
    // Volume: VertexBarrel_layer4_ladder12outer_volume
    TGeoVolume *pVertexBarrel_layer4_ladder12outer_volume_a4563f8 = new TGeoVolume("VertexBarrel_layer4_ladder12outer_volume",pVertexBarrel_ladderB_77, pMed8);
    pVertexBarrel_layer4_ladder12outer_volume_a4563f8->SetLineColor(130);
    pVertexBarrel_layer4_volume_a4512f8->AddNode(pVertexBarrel_layer4_ladder12outer_volume_a4563f8, 271, pMatrix820);
    // Volume: VertexBarrel_layer4_ladder13inner_volume
    TGeoVolume *pVertexBarrel_layer4_ladder13inner_volume_a456760 = new TGeoVolume("VertexBarrel_layer4_ladder13inner_volume",pVertexBarrel_ladderB_77, pMed8);
    pVertexBarrel_layer4_ladder13inner_volume_a456760->SetLineColor(130);
    pVertexBarrel_layer4_volume_a4512f8->AddNode(pVertexBarrel_layer4_ladder13inner_volume_a456760, 272, pMatrix822);
    // Volume: VertexBarrel_layer4_ladder14outer_volume
    TGeoVolume *pVertexBarrel_layer4_ladder14outer_volume_a456ac8 = new TGeoVolume("VertexBarrel_layer4_ladder14outer_volume",pVertexBarrel_ladderB_77, pMed8);
    pVertexBarrel_layer4_ladder14outer_volume_a456ac8->SetLineColor(130);
    pVertexBarrel_layer4_volume_a4512f8->AddNode(pVertexBarrel_layer4_ladder14outer_volume_a456ac8, 273, pMatrix824);
    // Volume: VertexBarrel_layer4_ladder15inner_volume
    TGeoVolume *pVertexBarrel_layer4_ladder15inner_volume_a456e30 = new TGeoVolume("VertexBarrel_layer4_ladder15inner_volume",pVertexBarrel_ladderB_77, pMed8);
    pVertexBarrel_layer4_ladder15inner_volume_a456e30->SetLineColor(130);
    pVertexBarrel_layer4_volume_a4512f8->AddNode(pVertexBarrel_layer4_ladder15inner_volume_a456e30, 274, pMatrix826);
    // Volume: VertexBarrel_layer4_ladder16outer_volume
    TGeoVolume *pVertexBarrel_layer4_ladder16outer_volume_a457198 = new TGeoVolume("VertexBarrel_layer4_ladder16outer_volume",pVertexBarrel_ladderB_77, pMed8);
    pVertexBarrel_layer4_ladder16outer_volume_a457198->SetLineColor(130);
    pVertexBarrel_layer4_volume_a4512f8->AddNode(pVertexBarrel_layer4_ladder16outer_volume_a457198, 275, pMatrix828);
    // Volume: VertexBarrel_layer4_ladder17inner_volume
    TGeoVolume *pVertexBarrel_layer4_ladder17inner_volume_a457500 = new TGeoVolume("VertexBarrel_layer4_ladder17inner_volume",pVertexBarrel_ladderB_77, pMed8);
    pVertexBarrel_layer4_ladder17inner_volume_a457500->SetLineColor(130);
    pVertexBarrel_layer4_volume_a4512f8->AddNode(pVertexBarrel_layer4_ladder17inner_volume_a457500, 276, pMatrix830);
    // Volume: VertexBarrel_layer4_ladder18outer_volume
    TGeoVolume *pVertexBarrel_layer4_ladder18outer_volume_a457868 = new TGeoVolume("VertexBarrel_layer4_ladder18outer_volume",pVertexBarrel_ladderB_77, pMed8);
    pVertexBarrel_layer4_ladder18outer_volume_a457868->SetLineColor(130);
    pVertexBarrel_layer4_volume_a4512f8->AddNode(pVertexBarrel_layer4_ladder18outer_volume_a457868, 277, pMatrix832);
    // Volume: VertexBarrel_layer4_ladder19inner_volume
    TGeoVolume *pVertexBarrel_layer4_ladder19inner_volume_a457bd0 = new TGeoVolume("VertexBarrel_layer4_ladder19inner_volume",pVertexBarrel_ladderB_77, pMed8);
    pVertexBarrel_layer4_ladder19inner_volume_a457bd0->SetLineColor(130);
    pVertexBarrel_layer4_volume_a4512f8->AddNode(pVertexBarrel_layer4_ladder19inner_volume_a457bd0, 278, pMatrix834);
    // Volume: VertexBarrel_layer4_ladder20outer_volume
    TGeoVolume *pVertexBarrel_layer4_ladder20outer_volume_a457f38 = new TGeoVolume("VertexBarrel_layer4_ladder20outer_volume",pVertexBarrel_ladderB_77, pMed8);
    pVertexBarrel_layer4_ladder20outer_volume_a457f38->SetLineColor(130);
    pVertexBarrel_layer4_volume_a4512f8->AddNode(pVertexBarrel_layer4_ladder20outer_volume_a457f38, 279, pMatrix836);
    // Volume: VertexBarrel_layer4_ladder21inner_volume
    TGeoVolume *pVertexBarrel_layer4_ladder21inner_volume_a4582a0 = new TGeoVolume("VertexBarrel_layer4_ladder21inner_volume",pVertexBarrel_ladderB_77, pMed8);
    pVertexBarrel_layer4_ladder21inner_volume_a4582a0->SetLineColor(130);
    pVertexBarrel_layer4_volume_a4512f8->AddNode(pVertexBarrel_layer4_ladder21inner_volume_a4582a0, 280, pMatrix838);
    // Volume: VertexBarrel_layer4_ladder22outer_volume
    TGeoVolume *pVertexBarrel_layer4_ladder22outer_volume_a458608 = new TGeoVolume("VertexBarrel_layer4_ladder22outer_volume",pVertexBarrel_ladderB_77, pMed8);
    pVertexBarrel_layer4_ladder22outer_volume_a458608->SetLineColor(130);
    pVertexBarrel_layer4_volume_a4512f8->AddNode(pVertexBarrel_layer4_ladder22outer_volume_a458608, 281, pMatrix840);
    // Volume: VertexBarrel_layer4_ladder23inner_volume
    TGeoVolume *pVertexBarrel_layer4_ladder23inner_volume_a458970 = new TGeoVolume("VertexBarrel_layer4_ladder23inner_volume",pVertexBarrel_ladderB_77, pMed8);
    pVertexBarrel_layer4_ladder23inner_volume_a458970->SetLineColor(130);
    pVertexBarrel_layer4_volume_a4512f8->AddNode(pVertexBarrel_layer4_ladder23inner_volume_a458970, 282, pMatrix842);
    // Volume: VertexBarrel_layer4_ladder24outer_volume
    TGeoVolume *pVertexBarrel_layer4_ladder24outer_volume_a458cd8 = new TGeoVolume("VertexBarrel_layer4_ladder24outer_volume",pVertexBarrel_ladderB_77, pMed8);
    pVertexBarrel_layer4_ladder24outer_volume_a458cd8->SetLineColor(130);
    pVertexBarrel_layer4_volume_a4512f8->AddNode(pVertexBarrel_layer4_ladder24outer_volume_a458cd8, 283, pMatrix844);
    // Volume: VertexBarrel_layer4_ladder25inner_volume
    TGeoVolume *pVertexBarrel_layer4_ladder25inner_volume_a459040 = new TGeoVolume("VertexBarrel_layer4_ladder25inner_volume",pVertexBarrel_ladderB_77, pMed8);
    pVertexBarrel_layer4_ladder25inner_volume_a459040->SetLineColor(130);
    pVertexBarrel_layer4_volume_a4512f8->AddNode(pVertexBarrel_layer4_ladder25inner_volume_a459040, 284, pMatrix846);
    // Volume: VertexBarrel_layer4_ladder26outer_volume
    TGeoVolume *pVertexBarrel_layer4_ladder26outer_volume_a4593a8 = new TGeoVolume("VertexBarrel_layer4_ladder26outer_volume",pVertexBarrel_ladderB_77, pMed8);
    pVertexBarrel_layer4_ladder26outer_volume_a4593a8->SetLineColor(130);
    pVertexBarrel_layer4_volume_a4512f8->AddNode(pVertexBarrel_layer4_ladder26outer_volume_a4593a8, 285, pMatrix848);
    // Volume: VertexBarrel_layer4_ladder27inner_volume
    TGeoVolume *pVertexBarrel_layer4_ladder27inner_volume_a459710 = new TGeoVolume("VertexBarrel_layer4_ladder27inner_volume",pVertexBarrel_ladderB_77, pMed8);
    pVertexBarrel_layer4_ladder27inner_volume_a459710->SetLineColor(130);
    pVertexBarrel_layer4_volume_a4512f8->AddNode(pVertexBarrel_layer4_ladder27inner_volume_a459710, 286, pMatrix850);
    // Volume: VertexBarrel_layer4_ladder28outer_volume
    TGeoVolume *pVertexBarrel_layer4_ladder28outer_volume_a459a78 = new TGeoVolume("VertexBarrel_layer4_ladder28outer_volume",pVertexBarrel_ladderB_77, pMed8);
    pVertexBarrel_layer4_ladder28outer_volume_a459a78->SetLineColor(130);
    pVertexBarrel_layer4_volume_a4512f8->AddNode(pVertexBarrel_layer4_ladder28outer_volume_a459a78, 287, pMatrix852);
    // Volume: VertexBarrel_layer4_ladder29inner_volume
    TGeoVolume *pVertexBarrel_layer4_ladder29inner_volume_a459de0 = new TGeoVolume("VertexBarrel_layer4_ladder29inner_volume",pVertexBarrel_ladderB_77, pMed8);
    pVertexBarrel_layer4_ladder29inner_volume_a459de0->SetLineColor(130);
    pVertexBarrel_layer4_volume_a4512f8->AddNode(pVertexBarrel_layer4_ladder29inner_volume_a459de0, 288, pMatrix854);
    
    pVertexBarrel_layer4_ladder0outer_volume_a453b60->AddNode(pVertexBarrel_detB_volume_a3ddc60, 199);
    pVertexBarrel_layer4_ladder1inner_volume_a453ec0->AddNode(pVertexBarrel_detB_volume_a3ddc60, 200);
    pVertexBarrel_layer4_ladder2outer_volume_a454220->AddNode(pVertexBarrel_detB_volume_a3ddc60, 201);
    pVertexBarrel_layer4_ladder3inner_volume_a454580->AddNode(pVertexBarrel_detB_volume_a3ddc60, 202);
    pVertexBarrel_layer4_ladder4outer_volume_a4548e0->AddNode(pVertexBarrel_detB_volume_a3ddc60, 203);
    pVertexBarrel_layer4_ladder5inner_volume_a454c40->AddNode(pVertexBarrel_detB_volume_a3ddc60, 204);
    pVertexBarrel_layer4_ladder6outer_volume_a454fa0->AddNode(pVertexBarrel_detB_volume_a3ddc60, 205);
    pVertexBarrel_layer4_ladder7inner_volume_a455300->AddNode(pVertexBarrel_detB_volume_a3ddc60, 206);
    pVertexBarrel_layer4_ladder8outer_volume_a455660->AddNode(pVertexBarrel_detB_volume_a3ddc60, 207);
    pVertexBarrel_layer4_ladder9inner_volume_a4559c0->AddNode(pVertexBarrel_detB_volume_a3ddc60, 208);
    pVertexBarrel_layer4_ladder10outer_volume_a455d28->AddNode(pVertexBarrel_detB_volume_a3ddc60, 209);
    pVertexBarrel_layer4_ladder11inner_volume_a456090->AddNode(pVertexBarrel_detB_volume_a3ddc60, 210);
    pVertexBarrel_layer4_ladder12outer_volume_a4563f8->AddNode(pVertexBarrel_detB_volume_a3ddc60, 211);
    pVertexBarrel_layer4_ladder13inner_volume_a456760->AddNode(pVertexBarrel_detB_volume_a3ddc60, 212);
    pVertexBarrel_layer4_ladder14outer_volume_a456ac8->AddNode(pVertexBarrel_detB_volume_a3ddc60, 213);
    pVertexBarrel_layer4_ladder15inner_volume_a456e30->AddNode(pVertexBarrel_detB_volume_a3ddc60, 214);
    pVertexBarrel_layer4_ladder16outer_volume_a457198->AddNode(pVertexBarrel_detB_volume_a3ddc60, 215);
    pVertexBarrel_layer4_ladder17inner_volume_a457500->AddNode(pVertexBarrel_detB_volume_a3ddc60, 216);
    pVertexBarrel_layer4_ladder18outer_volume_a457868->AddNode(pVertexBarrel_detB_volume_a3ddc60, 217);
    pVertexBarrel_layer4_ladder19inner_volume_a457bd0->AddNode(pVertexBarrel_detB_volume_a3ddc60, 218);
    pVertexBarrel_layer4_ladder20outer_volume_a457f38->AddNode(pVertexBarrel_detB_volume_a3ddc60, 219);
    pVertexBarrel_layer4_ladder21inner_volume_a4582a0->AddNode(pVertexBarrel_detB_volume_a3ddc60, 220);
    pVertexBarrel_layer4_ladder22outer_volume_a458608->AddNode(pVertexBarrel_detB_volume_a3ddc60, 221);
    pVertexBarrel_layer4_ladder23inner_volume_a458970->AddNode(pVertexBarrel_detB_volume_a3ddc60, 222);
    pVertexBarrel_layer4_ladder24outer_volume_a458cd8->AddNode(pVertexBarrel_detB_volume_a3ddc60, 223);
    pVertexBarrel_layer4_ladder25inner_volume_a459040->AddNode(pVertexBarrel_detB_volume_a3ddc60, 224);
    pVertexBarrel_layer4_ladder26outer_volume_a4593a8->AddNode(pVertexBarrel_detB_volume_a3ddc60, 225);
    pVertexBarrel_layer4_ladder27inner_volume_a459710->AddNode(pVertexBarrel_detB_volume_a3ddc60, 226);
    pVertexBarrel_layer4_ladder28outer_volume_a459a78->AddNode(pVertexBarrel_detB_volume_a3ddc60, 227);
    pVertexBarrel_layer4_ladder29inner_volume_a459de0->AddNode(pVertexBarrel_detB_volume_a3ddc60, 228);
//-----------------------------    Fine Barrel




//-------------------------InizioEndSupports (dischi azzurri)
    // Shape: VertexEndSupports_layer0_slice0 type: TGeoTubeSeg
    rmin = 1.450000*Scale;//raggio sulla beam pipe
    rmax = 6.040000*Scale;
    dz   = CThick;//0.003800;
    phi1 = 0.000000;
    phi2 = 360.000000;
    TGeoShape *pVertexEndSupports_layer0_slice0_80 = new TGeoTubeSeg("VertexEndSupports_layer0_slice0",rmin,rmax,dz,phi1,phi2);
    // Volume: VertexEndSupports_layer0_slice0_volume
    TGeoVolume *pVertexEndSupports_layer0_slice0_volume_a45d1f8 = new TGeoVolume("VertexEndSupports_layer0_slice0_volume",pVertexEndSupports_layer0_slice0_80, pMed5);
    pVertexEndSupports_layer0_slice0_volume_a45d1f8->SetLineColor(16);
    pVertexEndSupports_layer0_volume_a45d058->AddNode(pVertexEndSupports_layer0_slice0_volume_a45d1f8, 499);
    // Shape: VertexEndSupports_layer1_slice0 type: TGeoTubeSeg
    rmin = 1.450000*Scale;
    rmax = 11.953000;//*Scale;
    dz   = 0.021350;
    phi1 = 0.000000;
    phi2 = 360.000000;
    TGeoShape *pVertexEndSupports_layer1_slice0_81 = new TGeoTubeSeg("VertexEndSupports_layer1_slice0",rmin,rmax,dz,phi1,phi2);
    // Volume: VertexEndSupports_layer1_slice0_volume
    TGeoVolume *pVertexEndSupports_layer1_slice0_volume_a45d538 = new TGeoVolume("VertexEndSupports_layer1_slice0_volume",pVertexEndSupports_layer1_slice0_81, pMed5);
    pVertexEndSupports_layer1_slice0_volume_a45d538->SetLineColor(16);
    pVertexEndSupports_layer1_volume_a45d398->AddNode(pVertexEndSupports_layer1_slice0_volume_a45d538, 500);
    // Shape: VertexEndSupports_layer2_slice0 type: TGeoTubeSeg
    rmin = 1.600000*Scale;
    rmax = 11.953000;//*Scale;
    dz   = CThick;//0.003800;
    phi1 = 0.000000;
    phi2 = 360.000000;
    TGeoShape *pVertexEndSupports_layer2_slice0_82 = new TGeoTubeSeg("VertexEndSupports_layer2_slice0",rmin,rmax,dz,phi1,phi2);
    // Volume: VertexEndSupports_layer2_slice0_volume
    TGeoVolume *pVertexEndSupports_layer2_slice0_volume_a45d878 = new TGeoVolume("VertexEndSupports_layer2_slice0_volume",pVertexEndSupports_layer2_slice0_82, pMed5);
    pVertexEndSupports_layer2_slice0_volume_a45d878->SetLineColor(16);
    pVertexEndSupports_layer2_volume_a45d6d8->AddNode(pVertexEndSupports_layer2_slice0_volume_a45d878, 501);
    // Shape: VertexEndSupports_layer3_slice0 type: TGeoTubeSeg
    rmin = 1.600000*Scale;
    rmax = 11.953000;//*Scale;
    dz   = CThick;//0.003800;
    phi1 = 0.000000;
    phi2 = 360.000000;
    TGeoShape *pVertexEndSupports_layer3_slice0_83 = new TGeoTubeSeg("VertexEndSupports_layer3_slice0",rmin,rmax,dz,phi1,phi2);
    // Volume: VertexEndSupports_layer3_slice0_volume
    TGeoVolume *pVertexEndSupports_layer3_slice0_volume_a45dbb8 = new TGeoVolume("VertexEndSupports_layer3_slice0_volume",pVertexEndSupports_layer3_slice0_83, pMed5);
    pVertexEndSupports_layer3_slice0_volume_a45dbb8->SetLineColor(16);
    pVertexEndSupports_layer3_volume_a45da18->AddNode(pVertexEndSupports_layer3_slice0_volume_a45dbb8, 502);
    // Shape: VertexEndSupports_layer4_slice0 type: TGeoTubeSeg
    rmin = 2.000000*Scale;
    rmax = 11.953000;//*Scale;
    dz   = CThick;//0.003800;
    phi1 = 0.000000;
    phi2 = 360.000000;
    TGeoShape *pVertexEndSupports_layer4_slice0_84 = new TGeoTubeSeg("VertexEndSupports_layer4_slice0",rmin,rmax,dz,phi1,phi2);
    // Volume: VertexEndSupports_layer4_slice0_volume
    TGeoVolume *pVertexEndSupports_layer4_slice0_volume_a45def8 = new TGeoVolume("VertexEndSupports_layer4_slice0_volume",pVertexEndSupports_layer4_slice0_84, pMed5);
    pVertexEndSupports_layer4_slice0_volume_a45def8->SetLineColor(16);
    pVertexEndSupports_layer4_volume_a45dd58->AddNode(pVertexEndSupports_layer4_slice0_volume_a45def8, 503);
    // Shape: VertexEndSupports_layer5_slice0 type: TGeoTubeSeg
    rmin = 2.000000*Scale;
    rmax = 11.953000;//*Scale;
    dz   = CThick;//0.003800;
    phi1 = 0.000000;
    phi2 = 360.000000;
    TGeoShape *pVertexEndSupports_layer5_slice0_85 = new TGeoTubeSeg("VertexEndSupports_layer5_slice0",rmin,rmax,dz,phi1,phi2);
    // Volume: VertexEndSupports_layer5_slice0_volume
    TGeoVolume *pVertexEndSupports_layer5_slice0_volume_a45e238 = new TGeoVolume("VertexEndSupports_layer5_slice0_volume",pVertexEndSupports_layer5_slice0_85, pMed5);
    pVertexEndSupports_layer5_slice0_volume_a45e238->SetLineColor(16);
    pVertexEndSupports_layer5_volume_a45e098->AddNode(pVertexEndSupports_layer5_slice0_volume_a45e238, 504);

//----------------------- Fine EndSupports centrali

    //6 e 7 sono sulla beam pipe
    // Shape: VertexEndSupports_layer6_slice0 type: TGeoTubeSeg
    rmin = 5.500000;
    rmax = 11.953000;//*Scale;
    dz   = 0.021350;
    phi1 = 0.000000;
    phi2 = 360.000000;
    TGeoShape *pVertexEndSupports_layer6_slice0_86 = new TGeoTubeSeg("VertexEndSupports_layer6_slice0",rmin,rmax,dz,phi1,phi2);
    // Volume: VertexEndSupports_layer6_slice0_volume
    TGeoVolume *pVertexEndSupports_layer6_slice0_volume_a45e578 = new TGeoVolume("VertexEndSupports_layer6_slice0_volume",pVertexEndSupports_layer6_slice0_86, pMed5);
    pVertexEndSupports_layer6_slice0_volume_a45e578->SetLineColor(16);
    pVertexEndSupports_layer6_volume_a45e3d8->AddNode(pVertexEndSupports_layer6_slice0_volume_a45e578, 505);
    // Shape: VertexEndSupports_layer7_slice0 type: TGeoTubeSeg
    rmin = 5.500000;
//     rmax = 11.953000;//*Scale;
// // // //     rmax = 11.953000*Scale;
    rmax = 8.140000*Scale;
    dz   = 0.021350;
    phi1 = 0.000000;
    phi2 = 360.000000;
    TGeoShape *pVertexEndSupports_layer7_slice0_87 = new TGeoTubeSeg("VertexEndSupports_layer7_slice0",rmin,rmax,dz,phi1,phi2);
    // Volume: VertexEndSupports_layer7_slice0_volume
    TGeoVolume *pVertexEndSupports_layer7_slice0_volume_a45e8b8 = new TGeoVolume("VertexEndSupports_layer7_slice0_volume",pVertexEndSupports_layer7_slice0_87, pMed5);
    pVertexEndSupports_layer7_slice0_volume_a45e8b8->SetLineColor(16);
    pVertexEndSupports_layer7_volume_a45e718->AddNode(pVertexEndSupports_layer7_slice0_volume_a45e8b8, 506);

}
//______________________________________________________________________
void IlcTARGETv4th::CreateMaterials() {

    Float_t par[20];
    Int_t nel, numed,isvol,ifield;
    Float_t density,absl,radl;
    Float_t a[5],z[5],w[5];
    Float_t a2,z2;

    // MATERIALS, MIXTURES AND TRACKING MEDIA
// Mixture: TARGET_Air
    nel     = 3;
    density = 0.001200;

    a[0] = 39.948000;
    z[0] = 18.000000;
    w[0] = 0.012000;  // Ar

    a[1] = 15.999400;
    z[1] = 8.000000;
    w[1] = 0.234000;  // O

    a[2] = 14.006700;
    z[2] = 7.000000;
    w[2] = 0.754000;  // N

    IlcMixture(1,"Air",a,z,density,nel,w);

// Medium: TARGET_Air
    numed   = 3;  // medium number
    par[0]  = 0.000000; // isvol
    par[1]  = 2.000000; // ifield
    par[2]  = 50.000000; // fieldm
    par[3]  = 0.100000; // tmaxfd
    par[4]  = 1.000000; // stemax
    par[5]  = 0.100000; // deemax
    par[6]  = 0.000100; // epsil
    par[7]  = 0.000000; // stmin
    isvol=(Int_t)par[0];
    ifield=(Int_t)par[1];


    IlcMedium(1,"Air", 1,isvol, ifield, par[2], par[3], par[4], par[5], par[6], par[7]);

// Mixture: TARGET_CarbonFiber
    nel     = 3;
    density = 1.500000;

    a[0] = 1.007940;
    z[0] = 1.000000;
    w[0] = 0.046128;  // H

    a[1] = 12.010700;
    z[1] = 6.000000;
    w[1] = 0.837385;  // C

    a[2] = 15.999400;
    z[2] = 8.000000;
    w[2] = 0.116487;  // O

    IlcMixture(2,"CarbonFiber",a,z,density,nel,w);

// Medium: TARGET_CarbonFiber
    numed   = 11;  // medium number
    par[0]  = 0.000000; // isvol
    par[1]  = 2.000000; // ifield
    par[2]  = 50.000000; // fieldm
    par[3]  = 0.100000; // tmaxfd
    par[4]  = 1.000000; // stemax
    par[5]  = 0.100000; // deemax
    par[6]  = 0.000100; // epsil
    par[7]  = 0.000000; // stmin
    isvol=(Int_t)par[0];
    ifield=(Int_t)par[1];

    IlcMedium(2,"CarbonFiber", 2,isvol, ifield, par[2], par[3], par[4], par[5], par[6], par[7]);

    // Mixture: TARGET_Rohacell31
    nel     = 4;
    density = 0.032000;

    a[0] = 1.007940;
    z[0] = 1.000000;
    w[0] = 0.078366;  // H

    a[1] = 12.010700;
    z[1] = 6.000000;
    w[1] = 0.646489;  // C

    a[2] = 15.999400;
    z[2] = 8.000000;
    w[2] = 0.191375;  // O

    a[3] = 14.006700;
    z[3] = 7.000000;
    w[3] = 0.083770;  // N

    IlcMixture(3,"Rohacell31",a,z,density,nel,w);

    // Medium: TARGET_Rohacell31
    numed   = 13;  // medium number
    par[0]  = 0.000000; // isvol
    par[1]  = 2.000000; // ifield
    par[2]  = 50.000000; // fieldm
    par[3]  = 0.100000; // tmaxfd
    par[4]  = 1.000000; // stemax
    par[5]  = 0.100000; // deemax
    par[6]  = 0.000100; // epsil
    par[7]  = 0.000000; // stmin
    isvol=(Int_t)par[0];
    ifield=(Int_t)par[1];


    IlcMedium(3,"Rohacell31", 3,isvol, ifield, par[2], par[3], par[4], par[5], par[6], par[7]);
// Mixture: TARGET_G10
    nel     = 5;
    density = 1.700000;

    a[0] = 1.007940;
    z[0] = 1.000000;
    w[0] = 0.019374;  // H

    a[1] = 28.085500;
    z[1] = 14.000000;
    w[1] = 0.361327;  // Si

    a[2] = 12.010700;
    z[2] = 6.000000;
    w[2] = 0.078702;  // C

    a[3] = 15.999400;
    z[3] = 8.000000;
    w[3] = 0.460597;  // O

    a[4] = 35.452700;
    z[4] = 17.000000;
    w[4] = 0.080000;  // Cl

    IlcMixture(4,"G10",a,z,density,nel,w);

// Medium: TARGET_G10
    numed   = 10;  // medium number
    par[0]  = 0.000000; // isvol
    par[1]  = 2.000000; // ifield
    par[2]  = 50.000000; // fieldm
    par[3]  = 0.100000; // tmaxfd
    par[4]  = 1.000000; // stemax
    par[5]  = 0.100000; // deemax
    par[6]  = 0.000100; // epsil
    par[7]  = 0.000000; // stmin
    isvol=(Int_t)par[0];
    ifield=(Int_t)par[1];

    IlcMedium(4,"G10", 4,isvol, ifield, par[2], par[3], par[4], par[5], par[6], par[7]);
// Material: TARGET_Copper
    a2       = 63.546000;
    z2       = 29.000000;
    density = 8.933000;
    radl    = 1.439502;
    absl    = 0.000000;
    IlcMaterial(5,"Copper", a2,z2,density,radl,absl);

// Medium: TARGET_Copper
    numed   = 5;  // medium number
    par[0]  = 0.000000; // isvol
    par[1]  = 2.000000; // ifield
    par[2]  = 50.000000; // fieldm
    par[3]  = 0.100000; // tmaxfd
    par[4]  = 1.000000; // stemax
    par[5]  = 0.100000; // deemax
    par[6]  = 0.000100; // epsil
    par[7]  = 0.000000; // stmin
    isvol=(Int_t)par[0];
    ifield=(Int_t)par[1];

    IlcMedium(5,"Copper", 5,isvol, ifield, par[2], par[3], par[4], par[5], par[6], par[7]);
// Material: TARGET_Silicon
    a2       = 28.085500;
    z2       = 14.000000;
    density = 2.329600;
    radl    = 9.351213;
    absl    = 0.000000;
    IlcMaterial(6,"Silicon", a2,z2,density,radl,absl);

// Medium: TARGET_Silicon
    numed   = 4;  // medium number
    par[0]  = 1.000000; // isvol
    par[1]  = 2.000000; // ifield
    par[2]  = 50.000000; // fieldm
    par[3]  = 0.100000; // tmaxfd
    par[4]  = 0.007500; // stemax
    par[5]  = 0.100000; // deemax
    par[6]  = 0.000100; // epsil
    par[7]  = 0.000000; // stmin
    isvol=(Int_t)par[0];
    ifield=(Int_t)par[1];

    IlcMedium(6,"Silicon", 6,isvol, ifield, par[2], par[3], par[4], par[5], par[6], par[7]);
// Mixture: TARGET_Kapton
    nel     = 4;
    density = 1.430000;

    a[0] = 1.007940;
    z[0] = 1.000000;
    w[0] = 0.026363;  // H

    a[1] = 12.010700;
    z[1] = 6.000000;
    w[1] = 0.691127;  // C

    a[2] = 15.999400;
    z[2] = 8.000000;
    w[2] = 0.209238;  // O

    a[3] = 14.006700;
    z[3] = 7.000000;
    w[3] = 0.073271;  // N

    IlcMixture(7,"Kapton",a,z,density,nel,w);

// Medium: TARGET_Kapton
    numed   = 6;  // medium number
    par[0]  = 0.000000; // isvol
    par[1]  = 2.000000; // ifield
    par[2]  = 50.000000; // fieldm
    par[3]  = 0.100000; // tmaxfd
    par[4]  = 1.000000; // stemax
    par[5]  = 0.100000; // deemax
    par[6]  = 0.000100; // epsil
    par[7]  = 0.000000; // stmin
    isvol=(Int_t)par[0];
    ifield=(Int_t)par[1];

    IlcMedium(7,"Kapton", 7,isvol, ifield, par[2], par[3], par[4], par[5], par[6], par[7]);
// Mixture: TARGET_PEEK
    nel     = 3;
    density = 1.370000;

    a[0] = 1.007940;
    z[0] = 1.000000;
    w[0] = 0.041954;  // H

    a[1] = 12.010700;
    z[1] = 6.000000;
    w[1] = 0.791557;  // C

    a[2] = 15.999400;
    z[2] = 8.000000;
    w[2] = 0.166489;  // O

    IlcMixture(8,"PEEK",a,z,density,nel,w);

// Medium: TARGET_PEEK
    numed   = 12;  // medium number
    par[0]  = 0.000000; // isvol
    par[1]  = 2.000000; // ifield
    par[2]  = 50.000000; // fieldm
    par[3]  = 0.100000; // tmaxfd
    par[4]  = 1.000000; // stemax
    par[5]  = 0.100000; // deemax
    par[6]  = 0.000100; // epsil
    par[7]  = 0.000000; // stmin
    isvol=(Int_t)par[0];
    ifield=(Int_t)par[1];

    IlcMedium(8,"PEEK", 8,isvol, ifield, par[2], par[3], par[4], par[5], par[6], par[7]);
// Mixture: TARGET_Epoxy
    nel     = 3;
    density = 1.300000;

    a[0] = 1.007940;
    z[0] = 1.000000;
    w[0] = 0.131794;  // H

    a[1] = 12.010700;
    z[1] = 6.000000;
    w[1] = 0.535386;  // C

    a[2] = 15.999400;
    z[2] = 8.000000;
    w[2] = 0.332820;  // O

    IlcMixture(9,"Epoxy",a,z,density,nel,w);

// Medium: TARGET_Epoxy
    numed   = 9;  // medium number
    par[0]  = 0.000000; // isvol
    par[1]  = 2.000000; // ifield
    par[2]  = 50.000000; // fieldm
    par[3]  = 0.100000; // tmaxfd
    par[4]  = 1.000000; // stemax
    par[5]  = 0.100000; // deemax
    par[6]  = 0.000100; // epsil
    par[7]  = 0.000000; // stmin
    isvol=(Int_t)par[0];
    ifield=(Int_t)par[1];


    IlcMedium(9,"Epoxy", 9,isvol, ifield, par[2], par[3], par[4], par[5], par[6], par[7]);
// Material: TARGET_Iron
    a2       = 55.845000;
    z2       = 26.000000;
    density = 7.875000;
    radl    = 1.756443;
    absl    = 0.000000;
    IlcMaterial(10,"Iron", a2,z2,density,radl,absl);

// Medium: TARGET_Iron
    numed   = 7;  // medium number
    par[0]  = 0.000000; // isvol
    par[1]  = 2.000000; // ifield
    par[2]  = 50.000000; // fieldm
    par[3]  = 0.100000; // tmaxfd
    par[4]  = 1.000000; // stemax
    par[5]  = 0.100000; // deemax
    par[6]  = 0.000100; // epsil
    par[7]  = 0.000000; // stmin
    isvol=(Int_t)par[0];
    ifield=(Int_t)par[1];

    IlcMedium(10,"Iron", 10,isvol, ifield, par[2], par[3], par[4], par[5], par[6], par[7]);
// Material: TARGET_Titanium
    a2       = 47.867000;
    z2       = 22.000000;
    density = 4.500000;
    radl    = 3.589124;
    absl    = 0.000000;
    IlcMaterial(11,"Titanium", a2,z2,density,radl,absl);

// Medium: TARGET_Titanium
    numed   = 1;  // medium number
    par[0]  = 0.000000; // isvol
    par[1]  = 2.000000; // ifield
    par[2]  = 50.000000; // fieldm
    par[3]  = 0.100000; // tmaxfd
    par[4]  = 1.000000; // stemax
    par[5]  = 0.100000; // deemax
    par[6]  = 0.000100; // epsil
    par[7]  = 0.000000; // stmin
    isvol=(Int_t)par[0];
    ifield=(Int_t)par[1];

    IlcMedium(11,"Titanium", 11,isvol, ifield, par[2], par[3], par[4], par[5], par[6], par[7]);
// Material: TARGET_Beryllium
    a2       = 9.012180;
    z2       = 4.000000;
    density = 1.850000;
    radl    = 34.431033;
    absl    = 0.000000;
    IlcMaterial(12,"TARGET_Beryllium", a2,z2,density,radl,absl);

// Medium: TARGET_Beryllium
    numed   = 8;  // medium number
    par[0]  = 0.000000; // isvol
    par[1]  = 2.000000; // ifield
    par[2]  = 50.000000; // fieldm
    par[3]  = 0.100000; // tmaxfd
    par[4]  = 1.000000; // stemax
    par[5]  = 0.100000; // deemax
    par[6]  = 0.000100; // epsil
    par[7]  = 0.000000; // stmin
    isvol=(Int_t)par[0];
    ifield=(Int_t)par[1];

    IlcMedium(12,"Beryllium", 12,isvol, ifield, par[2], par[3], par[4], par[5], par[6], par[7]);



}
//______________________________________________________________________
void IlcTARGETv4th::InitIlcTARGETgeom() {
    //     Based on the geometry tree defined in Geant 3.21, this
    // routine initilizes the Class IlcTARGETgeom from the Geant 3.21 TARGET geometry
    // sturture.
    // Inputs:
    //   none.
    // Outputs:
    //   none.
    // Return:
    //   none.

    Float_t pixelx,pixely,pixelz;
    Int_t npix;
    IlcTARGETParam *Param=new IlcTARGETParam();
    //da parametrizzare
    const Int_t knlayers = 5;
    const Int_t knendcaps= 8;
    TString path,shapeName,name_ladder3;
    char name[100],name_lad1[100],name_lad2[100];

    TString name_lay,name_endcap,name_ladder;
    TGeoVolume *TARGETV;
    TGeoNode *name_ladder2;
    IlcTARGETDetector *idet=new IlcTARGETDetector[knlayers+knendcaps];
    for (Int_t i=0;i<knlayers+knendcaps;i++)
      idet[i]=kSPD;

    Int_t  *nlad=new Int_t[knlayers+knendcaps],*ndet= new Int_t[knlayers+knendcaps];


    TGeoHMatrix materix,matrixInv;
    Double_t trans[3]= {
                           3*0.0
                       }
                       ,rot[10]={9*0.0,1.0};
    TArrayD shapePar;
    TArrayF shapeParF;
    Int_t       iTARGET,mod,lay,lad=0,det=0,i,nodes;

    mod=0;
    for (i=0; i<knlayers;i++) nlad[i]=Param->GetNladders(i);
    for (i=0; i<knendcaps;i++) nlad[i+knlayers]=Param->GetNladders(i+knlayers);

    for (i=0; i<knlayers;i++) ndet[i]=Param->GetNdetectors(i);
    for (i=0; i<knendcaps;i++) ndet[i+knlayers]=Param->GetNdetectors(i+knlayers);


    for (i=0;i<knlayers+knendcaps;i++) mod=mod+nlad[i]*ndet[i];

    if (GetTARGETgeom()!=0) delete GetTARGETgeom();
    SetTARGETgeom(0);
    if (GetTARGETgeom2()!=0) delete GetTARGETgeom2();
    SetTARGETgeom2(0);


    IlcTARGETgeom* geom = new IlcTARGETgeom(0,knlayers+knendcaps,nlad,ndet,mod,1);
    SetTARGETgeom2(geom);
    IlcTARGETgeom* geom2 = new IlcTARGETgeom(0,knlayers+knendcaps,nlad,ndet,mod,1);
    SetTARGETgeom(geom2);
    mod = 0;


    TARGETV=gGeoManager->GetVolume("TARGETV");
    for (lay=1;lay<=knlayers;lay++) {
        sprintf(name,"VertexBarrel_layer%d_volume",lay-1);
        for (iTARGET=0;iTARGET<TARGETV->GetNdaughters();iTARGET++) {

            name_lay=TARGETV->GetNode(iTARGET)->GetName();
            if (!(name_lay.Contains(name))) continue;
            for (lad=1;lad<=nlad[lay-1];lad++) {
                sprintf(name_lad1,"VertexBarrel_layer%d_ladder%dinner_volume",lay-1,lad-1);
                sprintf(name_lad2,"VertexBarrel_layer%d_ladder%douter_volume",lay-1,lad-1);

                for (nodes=0;nodes<gGeoManager->GetVolume(name)->GetNdaughters();nodes++) {
                    name_ladder=gGeoManager->GetVolume(name)->GetNode(nodes)->GetName();
                    name_ladder2=gGeoManager->GetVolume(name)->GetNode(nodes);

                    if ((!(name_ladder.Contains(name_lad1))) && (!(name_ladder.Contains(name_lad2)))) continue;
                    name_ladder3=name_ladder2->GetVolume()->GetName();
                    path.Form("%s_1/%s_%d/%s/%s/%s",gGeoManager->GetTopVolume()->GetName(),TARGETV->GetName(),gGeoManager->GetVolume("ILCM")->GetNode(0)->GetNumber(),name_lay.Data(),name_ladder.Data(),gGeoManager->GetVolume(name_ladder3.Data())->GetNode(0)->GetName());

                    det=1;
                    gMC->GetTransformation(path.Data(),materix);

                    gMC->GetShape(path.Data(),shapeName,shapePar);
                    shapeParF.Set(shapePar.GetSize());
                    for (i=0;i<shapePar.GetSize();i++) shapeParF[i]=shapePar[i];
                    geom->CreatMatrix(mod,lay,lad,det,idet[lay-1],trans,rot);
                    geom->SetTrans(mod,materix.GetTranslation());
                    for (Int_t s=0;s<3;s++)
                        IlcDebug(1,Form("Trans[%d]: %f",s,materix.GetTranslation()[s]));
                    GetPixelDimension(lay-1,pixelx,pixely,pixelz);
                    geom->SetPixelDimension(lay-1,pixelx,pixely,pixelz);
                    matrixInv=materix.Inverse();
                    geom->SetRotMatrix(mod,matrixInv.GetRotationMatrix());
//geom->ReSetShape2(lay-1,new IlcTARGETgeomSPDPixel(lay-1,geom,shapeParF.GetSize(),shapeParF.GetArray()));
                    
		    geom->ReSetShape2(lay-1,new IlcTARGETgeomSPDPixelv4th(lay-1,geom,
                                      shapeParF.GetSize(),shapeParF.GetArray()));
                    
		    //-------------->Prede il Numero di Pixel
		    geom->GetNPixelX(lay-1,npix);
                    
		    /*cout<<"Sto usando questo 1"<<endl;
		    cout<<"Layer"<<lay-1 <<endl; 
		    cout<<"Numero di Pixel"<<npix<<endl; 
                    */
		    mod++;
                    IlcDebug(1,Form("module: %d",mod));
                }
            }
        }
    }

    TARGETV=gGeoManager->GetVolume("TARGETV");
    for (lay=knlayers+1;lay<=knlayers+knendcaps;lay++) {
        sprintf(name,"VertexEndcap_layer%d_volume",(lay-knlayers)-1);
        for (iTARGET=0;iTARGET<TARGETV->GetNdaughters();iTARGET++) {

            name_lay=TARGETV->GetNode(iTARGET)->GetName();
            //IlcDebug(1,Form("name: %s, name_lay: %s",name,name_lay.Data()));
            if (!(name_lay.Contains(name))) continue;
            for (lad=1;lad<=nlad[lay-1];lad++) {
                sprintf(name_lad1,"VertexEndcap_layer%d_ladder%d_volume",(lay-knlayers)-1,lad-1);

                for (nodes=0;nodes<gGeoManager->GetVolume(name)->GetNdaughters();nodes++) {
                    name_ladder=gGeoManager->GetVolume(name)->GetNode(nodes)->GetName();

                    //IlcDebug(1,Form("name_lad1: %s, name_lad2: %s, name_ladder: %s", name_lad1,name_lad2, name_ladder.Data()));
                    if (!(name_ladder.Contains(name_lad1))) continue;
                    path.Form("%s_1/%s_%d/%s/%s",gGeoManager->GetTopVolume()->GetName(),TARGETV->GetName(),gGeoManager->GetVolume("ILCM")->GetNode(0)->GetNumber(),name_lay.Data(),name_ladder.Data());

                    det=1;
                    IlcDebug(1,Form("path: %s",path.Data()));
                    gMC->GetTransformation(path.Data(),materix);

                    gMC->GetShape(path.Data(),shapeName,shapePar);
                    //IlcDebug(1,Form("Shape Par Size: %d",shapePar.GetSize()));
                    shapeParF.Set(shapePar.GetSize());
                    for (i=0;i<shapePar.GetSize();i++) shapeParF[i]=shapePar[i]; //IlcDebug(1,Form(" shapePar[%d]: %f",i,shapePar[i]));}
                    geom->CreatMatrix(mod,lay,lad,det,idet[lay-1],trans,rot);
                    //for (i=0;i<3;i++) IlcDebug(1,Form(" trans[%d]: %f",i,trans[i]));
                    //for (i=0;i<10;i++) IlcDebug(1,Form(" rot[%d]: %f",i,rot[i]));
                    //for (i=0;i<15;i++) IlcDebug(1,Form(" GetTranslation()[%d]: %f",i,materix.GetTranslation()[i]));
                    //for (i=0;i<12;i++) IlcDebug(1,Form(" GetRotationMatrix()[%d]: %f",i,materix.GetRotationMatrix()[i]));
                    geom->SetTrans(mod,materix.GetTranslation());
                    GetPixelDimension(lay-1,pixelx,pixely,pixelz);
                    geom->SetPixelDimension(lay-1,pixelx,pixely,pixelz);

                    matrixInv=materix.Inverse();
                    geom->SetRotMatrix(mod,matrixInv.GetRotationMatrix());
//geom->ReSetShape2(lay-1,new IlcTARGETgeomSPDPixelEnd(lay-1,geom,shapeParF.GetSize(),shapeParF.GetArray()));
                    geom->ReSetShape2(lay-1,new IlcTARGETgeomSPDPixelEndv4th(lay-1,geom,
                                      shapeParF.GetSize(),shapeParF.GetArray()));
                   mod++;
                    IlcDebug(1,Form("module: %d",mod));
                }
            }
        }
    }
    delete [] idet;
    delete [] nlad;
    delete [] ndet;

    delete Param;
    return;




}

//______________________________________________________________________
void IlcTARGETv4th::Init() {
    //     Initialise the TARGET after it has been created.
    // Inputs:
    //   none.
    // Outputs:
    //   none.
    // Return:
    //   none.



    IlcInfo(Form("Minor version %d",GetMinorVersion()));
    //
    if (fRead[0]=='\0') strncpy(fRead,fEuclidGeomDet,60);
    if (fWrite[0]=='\0') strncpy(fWrite,fEuclidGeomDet,60);
    if (GetTARGETgeom()!=0) SetTARGETgeom(0x0);
    if (GetTARGETgeom2()!=0) SetTARGETgeom2(0x0);
    IlcTARGETgeom* geom = new IlcTARGETgeom();
    SetTARGETgeom(geom);
    IlcTARGETgeom* geom2 = new IlcTARGETgeom();
    SetTARGETgeom2(geom2);

    if (fGeomDetIn) GetTARGETgeom()->ReadNewFile(fRead);
    else if (GetMinorVersion()==1) this->InitIlcTARGETgeom();

    if (fGeomDetOut) GetTARGETgeom()->WriteNewFile(fWrite);

    IlcTARGET::Init();


    //fIDMother = gMC->VolId("TARGETV"); // TARGET Mother Volume ID.

}
//______________________________________________________________________
void IlcTARGETv4th::SetDefaults() {
    // Inputs:
    //   none.
    // Outputs:
    //   none.
    // Return:
    //   none.

    const Double_t kconv = 10000.; // convert cm to microns
    Float_t dimpixelx,dimpixely,dimpixelz;
    Int_t npixelx=0,npixelz=0,lay;
    TArrayF bx,bz;
    IlcInfo("Called");
    //dan-commento cout <<"chiama il set defaults in PPRasymm"<<endl;
    if (!fDetTypeSim) fDetTypeSim = new IlcTARGETDetTypeSim();
    fDetTypeSim->SetDetVersion(GetMinorVersion());
    fDetTypeSim->Init();
    fDetTypeSim->SetTARGETgeom2(GetTARGETgeom2());
    IlcTARGETgeomSPD  *s0;
    IlcTARGETgeomSSD  *s1;

    Int_t nlayers,nendcaps,nlayersStrip,nendcapsStrip;
    //cout <<"qui ancora Ã¨ ok"<<endl;
    fDetTypeSim->SetDefaults();
    //cout <<"2) qui ancora Ã¨ ok"<<endl;
    //cout <<"ha finito con il SetDefaults di fDetTypeSim!"<<endl;
    //dan-commento cout <<"chiama prima il set defaults"<<endl;

//------------->VEDI

    IlcTARGETsegmentationSPD2* seg0 = (IlcTARGETsegmentationSPD2*)fDetTypeSim->GetSegmentationModel2(0);

//if (GetMinorVersion() !=3 ) cout <<"in IlcTARGETv4th: Minor Version: "<<GetMinorVersion()<<endl;
    IlcTARGETParam *Param;
    Param=new IlcTARGETParam();

    nlayers=Param->GetNlayers();
    nendcaps=Param->GetNendcaps();
    nlayersStrip=Param->GetNlayersStrip();
    nendcapsStrip=Param->GetNendcapsStrip();
    
    for (lay=0;lay<nlayers+nendcaps; lay++) {
        s0 = (IlcTARGETgeomSPD*) (GetTARGETgeom2())->GetShape2(lay);// Get shape info. Do it
        //cout <<"qui Ã¨ ok"<<endl;
        seg0->SetDetSize(lay,s0->GetDx()*2.*kconv, // base this on IlcTARGETgeomSPD
                         s0->GetDz()*2.*kconv, // for now.
                         s0->GetDy()*2.*kconv); // x,z,y full width in microns.

//cout<<"Ho chiamato la segmetizzazione SPD2"<<endl;
/*
cout<<""<<endl;
cout<<"Dimensioni in micron del layer "<< lay<<" X totale "<< s0->GetDx()*2.*kconv <<" Z totale "<< s0->GetDz()*2.*kconv<<" Y totale "<< s0->GetDy()*2.*kconv<<endl;
cout<<""<<endl;
*/
//cout <<"2) termina senza problemi"<<endl;
        IlcDebug(1,Form("layer: %d, kSPD Dx= %f, Dy= %f, Dz= %f",lay,s0->GetDx(),s0->GetDy(),s0->GetDz()));
        (GetTARGETgeom2())->GetNPixelZ(lay,npixelz);
        (GetTARGETgeom2())->GetNPixelX(lay,npixelx);
	   
	    
	     //cout<<"Sto usando questo 3"<<endl;
	    cout<<"Layer - Restituiti da GetTARGETGeom"<<lay <<" Numero di Pixel lungo X "<<npixelx<<" Numero di Pixel lungo Z "<<npixelz<<endl;
	    
	    
        seg0->SetNPads(lay,npixelx,npixelz);// Number of Bins in x and z
        GetPixelDimension(lay,dimpixelx,dimpixely,dimpixelz);
        bx.Set(npixelx);
        bz.Set(npixelz);
        for (Int_t i=0;i<npixelx;i++) bx[i]=dimpixelx;
        for (Int_t i=0;i<npixelz;i++) bz[i]=dimpixelz;
//    printf("3)numero pixel in z:%d\n", seg0->Npx(0));
        seg0->SetBinSize(lay,bx,bz); // Based on IlcTARGETgeomSPD for now.
    }


    SetSegmentationModel2(kSPD,seg0);
    Double_t stereop;
    Double_t stereon;
    fDetTypeSim->SetDigitClassName(kSPD,"IlcTARGETdigitSPD");
    if ((nlayersStrip==0) && (nendcapsStrip==0)) delete Param;
    return;

//---------------->VEDI
    IlcTARGETsegmentationSSD* seg1 = (IlcTARGETsegmentationSSD*)fDetTypeSim->GetSegmentationModel2(1);




    for (lay=0;lay<nlayersStrip+nendcapsStrip; lay++) {
        //cout <<"qui va bene"<<endl;
        s1 = (IlcTARGETgeomSSD*) (GetTARGETgeom2())->GetShape2(lay+nlayers+nendcaps);// Get shape info. Do it this way for now.
        //cout <<"qui non va bene"<<endl;
        GetPixelDimension(lay+nlayers+nendcaps,dimpixelx,dimpixely,dimpixelz);
        (GetTARGETgeom2())->GetNPixelX(lay+nlayers+nendcaps,npixelx);
			
  //cout<<"Ho chiamato la segmetizzazione SSD"<<endl;
		    
        (GetTARGETgeom2())->GetNPixelZ(lay+nlayers+nendcaps,npixelz);
        //dan-commento cout <<"in Set Default Dx: "<<s1->GetDx()<<" Dy: "<<s1->GetDy()<<" Dz: "<<s1->GetDz()<<endl;
        seg1->SetDetSize(lay+nlayers+nendcaps,s1->GetDx()*2.*kconv, // base this on IlcTARGETgeomSSD
                         s1->GetDz()*2.*kconv, // for now.
                         s1->GetDy()*2.*kconv); // x,z,y full width in microns.
        //cout <<"3)termina senza problemi"<<endl;
        IlcDebug(1,Form("layer: %d, kSSD Dx= %f, Dy= %f, Dz= %f, dim. pixelx: %f, dim. pixely: %f, dim. pixelz: %f,",lay+nlayers+nendcaps,s1->GetDx(),s1->GetDy(),s1->GetDz(),dimpixelx,dimpixely,dimpixelz));

//cout <<"dimensione pitch in SetDefaults: "<<dimpixelx<<endl;
        
	
	seg1->SetPadSize(lay,dimpixelx,0.); // strip x pitch in microns
        seg1->SetNPads(lay,npixelx,0); // number of strips on each side.
        stereop=Param->GetStereoP(lay+nlayers+nendcaps);
        stereon=Param->GetStereoN(lay+nlayers+nendcaps);

        //cout <<"stereop: "<<stereop<<" stereon: "<<stereon<<endl;
        seg1->SetAngles(lay,stereop,stereon); // strip angels rad P and N side.
    }
    SetSegmentationModel2(kSSD,seg1);
    cout <<"qui sta il problema"<<endl;
    fDetTypeSim->SetDigitClassName(kSSD,"IlcTARGETdigitSSD");
    if (fgkNTYPES>3) {
        Warning("SetDefaults",
                "Only the four basic detector types are initialised!");
    }// end if

    delete Param;

    return;
}

//______________________________________________________________________
void IlcTARGETv4th::DrawModule() const {
  return;
    // //     Draw a shaded view of the FMD version 10.
    // // Inputs:
    // //   none.
    // // Outputs:
    // //   none.
    // // Return:
    // //   none.



    // // Set everything unseen
    // gMC->Gsatt("*", "seen", -1);
    // //
    // // Set ILCM mother visible
    // gMC->Gsatt("ILCM","SEEN",0);
    // //
    // // Set the volumes visible
    // gMC->Gsatt("TARGETD","SEEN",0);
    // gMC->Gsatt("TARGET1","SEEN",1);
    // gMC->Gsatt("TARGET2","SEEN",1);
    // gMC->Gsatt("TARGET3","SEEN",1);
    // gMC->Gsatt("TARGET4","SEEN",1);
    // gMC->Gsatt("TARGET5","SEEN",1);
    // gMC->Gsatt("TARGET6","SEEN",1);
    // //
    // gMC->Gsatt("IPCB","SEEN",1);
    // gMC->Gsatt("ICO2","SEEN",1);
    // gMC->Gsatt("ICER","SEEN",0);
    // gMC->Gsatt("ISI2","SEEN",0);
    // gMC->Gsatt("IPLA","SEEN",0);
    // gMC->Gsatt("ICO3","SEEN",0);
    // gMC->Gsatt("IEPX","SEEN",0);
    // gMC->Gsatt("ISI3","SEEN",1);
    // gMC->Gsatt("ISUP","SEEN",0);
    // gMC->Gsatt("ICHO","SEEN",0);
    // gMC->Gsatt("ICMO","SEEN",0);
    // gMC->Gsatt("ICMD","SEEN",0);
    // gMC->Gsatt("ICCO","SEEN",1);
    // gMC->Gsatt("ICCM","SEEN",0);
    // gMC->Gsatt("ITMD","SEEN",0);
    // gMC->Gsatt("ITTT","SEEN",1);
    // //
    // gMC->Gdopt("hide", "on");
    // gMC->Gdopt("shad", "on");
    // gMC->Gsatt("*", "fill", 7);
    // gMC->SetClipBox(".");
    // gMC->SetClipBox("*", 0, 300, -300, 300, -300, 300);
    // gMC->DefaultRange();
    // gMC->Gdraw("ilcc", 40, 30, 0, 11, 10, .07, .07);
    // gMC->Gdhead(1111, "Inner Tracking System Version 1");
    // gMC->Gdman(17, 6, "MAN");
}
//______________________________________________________________________
void IlcTARGETv4th::StepManager() {
    //    Called for every step in the TARGET, then calles the IlcTARGEThit class
    // creator with the information to be recoreded about that hit.
    //     The value of the macro ILCTARGETPRINTGEOM if set to 1 will allow the
    // printing of information to a file which can be used to create a .det
    // file read in by the routine CreateGeometry(). If set to 0 or any other
    // value except 1, the default behavior, then no such file is created nor
    // it the extra variables and the like used in the printing allocated.
    // Inputs:
    //   none.
    // Outputs:
    //   none.
    // Return:
    //   none.


    //gMC->SetMaxStep(0.01);
    TString VolumeName,VolumeOffName;
    Int_t         copy,id,addladder=0,from=0,node_copy=0;
    TLorentzVector position, momentum;
    static TLorentzVector position0;
    static Int_t stat0=0;
    Int_t s=0;
    TString layer,layer2,volpath,currentvol,volpath2;
    TString ladder,nladder;
    TString detector,detector2;
    VolumeName=gMC->CurrentVolName();



    if (!(this->IsActive())) return;

    if (!(gMC->TrackCharge())) return;

    id=gMC->CurrentVolID(copy);

    Bool_t sensvol = kFALSE;
    if (GetMinorVersion() == 3) {

        if ((VolumeName.Contains("TrackerBarrel_SiTrackerModule_component3_bis"))  ||  (VolumeName.BeginsWith("TrackerEndcap_") && (VolumeName.Contains("detA") || VolumeName.Contains("detB") || VolumeName.Contains("detC") || VolumeName.Contains("detD") || VolumeName.Contains("detE") || VolumeName.Contains("detF") || VolumeName.Contains("detG") || VolumeName.Contains("detH")))) sensvol=kTRUE;

        if ((VolumeName.BeginsWith("VertexBarrel_") && (VolumeName.Contains("detA") || VolumeName.Contains("detB"))) ||  (VolumeName.BeginsWith("VertexEndcap_") && (VolumeName.Contains("detA") || VolumeName.Contains("detB")))) sensvol=kTRUE;

    }

    if (GetMinorVersion() == 1) {
        if ((VolumeName.BeginsWith("VertexBarrel_") && (VolumeName.Contains("detA") || VolumeName.Contains("detB")))) sensvol=kTRUE;
    if ((VolumeName.BeginsWith("VertexEndcap_") && (VolumeName.Contains("detA") || VolumeName.Contains("detB")))) sensvol=kTRUE;
    }

    if (GetMinorVersion() == 2) {
        if ((VolumeName.BeginsWith("VertexBarrel_") && (VolumeName.Contains("detA") || VolumeName.Contains("detB"))) ||  (VolumeName.BeginsWith("VertexEndcap_") && VolumeName.Contains("detA") || VolumeName.Contains("detB") ||  VolumeName.BeginsWith("VertexEndcap_") && (VolumeName.Contains("detA") || VolumeName.Contains("detB") || VolumeName.Contains("detC") || VolumeName.Contains("detD") || VolumeName.Contains("detE") || VolumeName.Contains("detF") || VolumeName.Contains("detG") || VolumeName.Contains("detH") || VolumeName.Contains("detI") || VolumeName.Contains("detL") || VolumeName.Contains("detM") || VolumeName.Contains("detN") || VolumeName.Contains("detO") || VolumeName.Contains("detP")))) sensvol=kTRUE;
    }

    if (GetMinorVersion()==0) {
        if ((VolumeName.BeginsWith("VertexBarrel_") && VolumeName.Contains("slice0")) ||  (VolumeName.BeginsWith("VertexEndcap_") && VolumeName.Contains("slice0"))) sensvol=kTRUE;
    }

// // // //     if (sensvol && (gMC->IsTrackExiting())) {
// // // //         copy = fTrackReferences->GetEntriesFast();
// // // //         TClonesArray &lTR = *fTrackReferences;
// // // //         // Fill TrackReference structure with this new TrackReference.
// // // //         new(lTR[copy]) IlcTrackReference(gIlc->GetMCApp()->GetCurrentTrackNumber());
// // // // 
// // // //     } // if Outer TARGET mother Volume


    //Int_t   copy1,copy2;
    Int_t   vol[5];
    TClonesArray &lhits = *fHits;
    //
    // Track status
    //vol[0] = 0;
    vol[1] = 1;
    vol[2] = 0;
    vol[3] = 0;
    vol[4] = 0;
    if (gMC->IsTrackInside())      vol[3] +=  1;
    if (gMC->IsTrackEntering())    vol[3] +=  2;
    if (gMC->IsTrackExiting())     vol[3] +=  4;
    if (gMC->IsTrackOut())         vol[3] +=  8;
    if (gMC->IsTrackDisappeared()) vol[3] += 16;
    if (gMC->IsTrackStop())        vol[3] += 32;
    if (gMC->IsTrackAlive())       vol[3] += 64;


    //
    // Fill hit structure.
    if (!(gMC->TrackCharge())) return;

    // Only entering charged tracks
    if (GetMinorVersion()==1) {
        if (VolumeName.BeginsWith("VertexBarrel_") && (VolumeName.Contains("detA") || VolumeName.Contains("detB"))) {
            VolumeOffName=gMC->CurrentVolOffName(1);
            
	    //cout<<"Volume Name"<<VolumeOffName<<endl;
	    
	    IlcDebug(1,Form("VolOffName(1): %s",VolumeOffName.Data()));
            layer=VolumeOffName(18,1);
           // cout<<"Layer "<<layer<<endl;
	    
	    vol[0]=atoi(layer)+1;
            nladder=VolumeOffName(27,1);
            
	    
	    if (nladder.IsDigit()) ladder=VolumeOffName(26,2);
            else ladder=VolumeOffName(26,1);
            vol[2]=atoi(ladder)+1;
            
	    IlcDebug(1,Form("ladder: %d", vol[2]));
        } else if (VolumeName.BeginsWith("VertexEndcap_") && ((VolumeName.Contains("detA") || VolumeName.Contains("detB")))) {
            VolumeOffName=gMC->CurrentVolOffName(1);
            layer=VolumeOffName(18,1);
            vol[0]=atoi(layer)+6;
            nladder=VolumeOffName(27,1);
            if (nladder.IsDigit()) ladder=VolumeOffName(26,2);
            else ladder=VolumeOffName(26,1);
            vol[2]=atoi(ladder)+1;
            IlcDebug(1,Form("Encap number; %d",vol[0]));
            IlcDebug(1,Form("endcap: %d", vol[0]));
        } else {
            return; // not an TARGET sensible volume
        }

    }

    if (GetMinorVersion()==2) {
        if (VolumeName.BeginsWith("VertexBarrel_") && (VolumeName.Contains("detA") || VolumeName.Contains("detB"))) {
            VolumeOffName=gMC->CurrentVolOffName(1);
            IlcDebug(1,Form("VolOffName(1): %s",VolumeOffName.Data()));
            layer=VolumeOffName(18,1);
            vol[0]=atoi(layer)+1;
            nladder=VolumeOffName(27,1);
            if (nladder.IsDigit()) ladder=VolumeOffName(26,2);
            else ladder=VolumeOffName(26,1);
            vol[2]=atoi(ladder)+1;
            //IlcDebug(1,Form("ladder: %d", vol[2]));
        } else if (VolumeName.BeginsWith("VertexEndcap_") && ((VolumeName.Contains("detA") || VolumeName.Contains("detB") || VolumeName.Contains("detC") || VolumeName.Contains("detD") || VolumeName.Contains("detE") || VolumeName.Contains("detF") || VolumeName.Contains("detG") || VolumeName.Contains("detH") || VolumeName.Contains("detI") || VolumeName.Contains("detL") || VolumeName.Contains("detM") || VolumeName.Contains("detN") || VolumeName.Contains("detO") || VolumeName.Contains("detP")))) {

            VolumeOffName=gMC->CurrentVolOffName(1);
            layer=VolumeOffName(19,1);
            if (layer.IsDigit()) {
                addladder=1;
                layer=VolumeOffName(18,2);
            } else
                layer=VolumeOffName(18,1);
            vol[0]=atoi(layer)+6;
            nladder=VolumeOffName(27+addladder,1);
            if (nladder.IsDigit()) ladder=VolumeOffName(26+addladder,2);
            else ladder=VolumeOffName(26+addladder,1);
            vol[2]=atoi(ladder)+1;
            IlcDebug(1,Form("Encap number; %d",vol[0]));
            //IlcDebug(1,Form("endcap: %d", vol[0]));

        } else {
            return; // not an TARGET sensible volume
        }

    }


    if (GetMinorVersion()==3) {
        IlcTARGETParam *Param=new IlcTARGETParam("Tracker");
        gMC->TrackPosition(position);
        if (IlcLog::GetGlobalDebugLevel()>1) {
            //cout <<"raggio: "<<TMath::Sqrt(position[0]*position[0]+position[1]*position[1])<<endl;
            //cout <<"volume colpito: "<<VolumeName.Data()<<endl;
            //cout <<"Status dell'hit: "<<vol[3]<<endl;
            //cout <<"Energia depositata: "<<gMC->Edep()<<endl<<endl;
        }
        if (VolumeName.BeginsWith("VertexBarrel_") && (VolumeName.Contains("detA") || VolumeName.Contains("detB"))) {
            //delete Param; return;
            VolumeOffName=gMC->CurrentVolOffName(1);
            IlcDebug(1,Form("VolOffName(1): %s",VolumeOffName.Data()));
            layer=VolumeOffName(18,1);
            vol[0]=atoi(layer)+1;
            nladder=VolumeOffName(27,1);
            if (nladder.IsDigit()) ladder=VolumeOffName(26,2);
            else ladder=VolumeOffName(26,1);
            vol[2]=atoi(ladder)+1;
            //IlcDebug(1,Form("ladder: %d", vol[2]));
        } else if (VolumeName.BeginsWith("VertexEndcap_") && (VolumeName.Contains("detA") || VolumeName.Contains("detB") || VolumeName.Contains("detC") || VolumeName.Contains("detD") || VolumeName.Contains("detE") || VolumeName.Contains("detF") || VolumeName.Contains("detG") || VolumeName.Contains("detH") || VolumeName.Contains("detI") || VolumeName.Contains("detL") || VolumeName.Contains("detM") || VolumeName.Contains("detN") || VolumeName.Contains("detO") || VolumeName.Contains("detP"))) {
            //delete Param; return;
            VolumeOffName=gMC->CurrentVolOffName(1);
            //cout <<"VolOffName: "<<VolumeOffName.Data()<<endl;
            layer=VolumeOffName(18,1);
            vol[0]=atoi(layer)+6;
            nladder=VolumeOffName(27,1);
            if (nladder.IsDigit()) ladder=VolumeOffName(26,2);
            else ladder=VolumeOffName(26,1);
            vol[2]=atoi(ladder)+1;
            IlcDebug(1,Form("Encap number; %d",vol[0]));
            //IlcDebug(1,Form("endcap: %d", vol[0]));
        } else if (VolumeName.BeginsWith("TrackerBarrel_SiTrackerModule_component3_bis")/* || (VolumeName.Contains("TrackerBarrel_SiTrackerModule_component4"))*/) {
            //cout <<"gMC->TrackStep(): "<<gMC->TrackStep()<<endl;
            //delete Param; return;
            volpath=gMC->CurrentVolPath();
            //cout <<"percorso volume: "<< volpath.Data()<<" Status: "<<vol[3]<<endl;
            currentvol=gMC->CurrentVolOffName(3);
            layer=currentvol(19,1);
            vol[0]=atoi(layer)+14;
//     if (vol[0]==18) {delete Param; return;}
            //cout <<"vol[0] per tracker barrel: "<<vol[0]<<endl;
            ///ILCM_1/TARGETV_8302/TrackerBarrel_layer4_8229/TrackerBarrel_SiTrackerModule_5205/TrackerBarrel_SiTrackerModule_component3_388
            if (vol[0]==14) {
                node_copy=atoi(volpath(74,4).Data());
                from=2011;
            }
            if (vol[0]==15) {
                //volpath2=volpath(77,1);
                //if (volpath2.IsDigit()) node_copy=atoi(volpath(74,4).Data());
                /*else*/ node_copy=atoi(volpath(74,4).Data());
                from=2123;
            }
            if (vol[0]==16) {
                node_copy=atoi(volpath(74,4).Data());
                from=2663;
            }
            if (vol[0]==17) {
                node_copy=atoi(volpath(74,4).Data());
                from=3905;
            }
            if (vol[0]==18) {
                node_copy=atoi(volpath(74,4).Data());
                from=5993;
            }
            //cout <<"volume:    "<<currentvol.Data()<<endl;
            //cout <<"node_copy: "<<node_copy<<endl;
            //cout <<"from:      "<<from<<endl;
            //cout <<"det:       "<<Param->GetNdetectors(vol[0]-1)<<endl;

            vol[2]=((node_copy-from)/Param->GetNdetectors(vol[0]-1))+1;
            vol[1]=((node_copy-from)%Param->GetNdetectors(vol[0]-1))+1;
            //cout <<"det colpito!!!!!!!!!!!: "<<vol[1]<<endl;
            //cout <<"ladder colpito!!!!!!!!!!!: "<<vol[2]<<endl;
            //cout <<"per tracker vol[2]: "<<vol[2]<<endl;

        } else if (VolumeName.BeginsWith("TrackerEndcap_")&&VolumeName.Contains("tile")) {
            s=0;
            //delete Param; return;
            VolumeOffName=gMC->CurrentVolOffName(2);
            //cout <<"VolumeOffName: "<<VolumeOffName<<endl;
            layer=VolumeOffName(20,1);
            if (layer.IsDigit()) {
                s=1;
                layer2=VolumeOffName(19,2);
            } else layer2=VolumeOffName(19,1);
            vol[0]=atoi(layer2.Data())+14+5;
            //cout<<"vol[0]:"<<vol[0]<<endl;
            nladder=VolumeOffName(28+s,1);
            if (nladder.IsDigit()) ladder=VolumeOffName(27+s,2);
            else ladder=VolumeOffName(27+s,1);
            vol[2]=atoi(ladder.Data())+1;
            detector=VolumeName(26+s,1);
            if (detector.IsDigit()) {
                detector2=VolumeName(25+s,2);
            } else detector2=VolumeName(25+s,1);
            vol[1]=atoi(detector2.Data())+1;
        } else {
            delete Param;
            return; // not an TARGET sensible volume
        }
        delete Param;
    }


    if (GetMinorVersion()==0) {
        if ((VolumeName.BeginsWith("VertexBarrel_")) && VolumeName.Contains("slice0")) {
            layer=VolumeName(18,1);
            vol[0]=atoi(layer)+1;
            vol[1]=1;
            vol[2]=1;
        } else if ((VolumeName.BeginsWith("VertexEndcap_")) && VolumeName.Contains("slice0")) {
            layer=VolumeName(18,1);
            vol[0]=atoi(layer)+6;
            vol[1]=1;
            vol[2]=1;
            //IlcDebug(1,Form("endcap: %d", vol[0]));

        } else {
            return; // not an TARGET sensible volume
        }
    }

    gMC->TrackPosition(position);
    gMC->TrackMomentum(momentum);
    vol[4] = stat0;
    int p;

    //if (gMC->IsTrackInside()) return;
    if (gMC->IsTrackEntering()) {
        position0 = position;
        for (p=0;p<5;p++) IlcDebug(1,Form("The track is Entering!!!... vol[%d]: %d", p, vol[p]));
        //for (s=0;s<4;s++) IlcDebug(1,Form("position[%d]=%f",s,position[s]));
        //for (s=0;s<4;s++) IlcDebug(1,Form("position0[%d]=%f",s,position[s]));
        stat0 = vol[3];
        return;
    } // end if IsEntering
    // Fill hit structure with this new hit.

    for (p=0;p<5;p++) IlcDebug(1,Form("The track is not entering!!!... vol[%d]: %d", p,vol[p]));

    //for (s=0;s<4;s++) IlcDebug(1,Form("position[%d]=%f",s,position[s]));
    //for (s=0;s<4;s++) IlcDebug(1,Form("position0[%d]=%f",s,position[s]));
    /*Double_t *locals[3];
    locals[0],locals[1],locals[2]=0.;
    Double_t *pos[3];
    pos[0]=position[0];
    pos[1]=position[1];
    pos[2]=position[2];
    GetTARGETgeom2()->GtoL(vol[0],vol[2],vol[1],pos,locals);
    cout << "hit local: x=" << locals[0] << " y=" << locals[1] << " z=" << locals[2] << endl;*/
    new(lhits[fNhits++]) IlcTARGEThit(fIshunt,gIlc->GetMCApp()->GetCurrentTrackNumber(),vol,
                                   gMC->Edep(),gMC->TrackTime(),position,
                                   position0,momentum);
    /*Double_t x=0.,y=0.,z=0.,x0=0.,y0=0.,z0=0.,t=0.;
    hit->GetPositionL(x,y,z);
    hit->GetPositionL0(x0,y0,z0,t);

    cout << "Position: x="<< x << " y=" << y << " z=" << z << " layer=" << hit->GetLayer() << " ladder=" << hit->GetLadder() << " det=" << hit->GetDetector() << endl; 
    cout << "Position 0: x0="<< x0 << " y0=" << y0 << " z0=" << z0 << " layer=" << hit->GetLayer() << " ladder=" << hit->GetLadder() << " det=" << hit->GetDetector() << endl; */

    position0 = position;
    stat0 = vol[3];
    return;
}

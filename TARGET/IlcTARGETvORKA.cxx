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

/* $Id: IlcTARGETvORKA.cxx,v 1.8 2009/12/26 21:47:20 vitomeg Exp $ */
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

// See IlcTARGETvORKA::StepManager().

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
#include <TGeoPgon.h>
#include <TGeoCompositeShape.h>
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
#include "IlcTARGETvORKA.h"
#include "IlcLog.h"
#include "IlcMC.h"
#include "IlcMagF.h"
#include "IlcRun.h"
#include "IlcTrackReference.h"
#include "IlcTARGETParam.h"

#ifdef WIN32
// patch to fix memory overwriting problem in TG4StepManager
#include "TG4StepManager.h"
#include "TG4GeometryServices.h"
#endif


#define GEANTGEOMETRY kTRUE

ClassImp(IlcTARGETvORKA)

//______________________________________________________________________
IlcTARGETvORKA::IlcTARGETvORKA() {
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
IlcTARGETvORKA::IlcTARGETvORKA(const char *name, const char *title,Int_t MinorVersion)
        : IlcTARGET(name, title,MinorVersion) {
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

    IlcTARGETParam *Param = new IlcTARGETParam(MinorVersion);

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
// // // // IlcTARGETvORKA::IlcTARGETvORKA(const IlcTARGETvORKA &source) :
// // // //         IlcTARGET(source) {
// // // //     //     Copy Constructor for TARGET version 10. This function is not to be
// // // //     // used. If any other instance of this function, other than "this" is
// // // //     // passed, an error message is returned.
// // // //     // Inputs:
// // // //     //   const IlcTARGETvORKA &source This class
// // // //     // Outputs:
// // // //     //   none.
// // // //     // Return:
// // // //     //   an error message
// // // // 
// // // //     if (&source == this) return;
// // // //     Warning("Copy Constructor","Not allowed to copy IlcTARGETvORKA");
// // // //     return;
// // // // }
//______________________________________________________________________
IlcTARGETvORKA& IlcTARGETvORKA::operator=(const IlcTARGETvORKA
                                      &source) {
    //    Assignment operator for the TARGET version 10. This function is not
    // to be used. If any other instance of this function, other than "this"
    // is passed, an error message is returned.
    // Inputs:
    //   const IlcTARGETvORKA &source This class
    // Outputs:
    //   none.
    // Return:
    //   an error message

    if (&source == this) return *this;
    Warning("= operator","Not allowed to copy IlcTARGETvORKA");
    return *this;
}
//______________________________________________________________________
IlcTARGETvORKA::~IlcTARGETvORKA() {
    //    Standard destructor for the TARGET version 10.
    // Inputs:
    //   none.
    // Outputs:
    //   none.
    // Return:
    //   none.
}

TList *   IlcTARGETvORKA::BuildGeometry2(void){
  
  // Build the TARGET geometry for the ROOT display
  
  TList *Nodes = new TList();       //List of geometry nodes
  
  new TMaterial("void","Vacuum",0,0,0);  //Everything is void
  TBRIK *brik = new TBRIK("S_ilc","ilc volume","void",2000,2000,3000);
  brik->SetVisibility(0);
  TNode *top = new TNode("ilc","ilc","S_ilc");
  
  
  IlcTARGETParam *Param;
  Param=new IlcTARGETParam(GetMinorVersion());

  Double_t LayerThick = Param->GetLayerThick();
  Double_t TargetThick = Param->GetTargetThick();
  Double_t TargetLength = Param->GetTargetLength();
  Double_t TargetLowerZ = Param->GetTargetLowerZ();
  const Int_t nLayers = Param->GetNlayers();
  Int_t NLadders = Param->GetNLadders();
  Int_t nSections = TMath::Max(NLadders,24);

#ifdef WIN32
		TTUBE **ptarget_layer = (TTUBE **)malloc(nLayers);
		TNode **nNode = (TNode **)malloc(nLayers);
#else
	TTUBE *ptarget_layer[nLayers];
	TNode *nNode[nLayers];
#endif


  for(Int_t iLay=1; iLay<nLayers; iLay++){
    ptarget_layer[iLay] = new TTUBE(Form("starget_layer%02d",iLay), Form("starget_layer%02d",iLay), "void", (iLay-0.5)*LayerThick, (iLay+0.5)*LayerThick, TargetLength/2.);
    ptarget_layer[iLay]->SetNumberOfDivisions(nSections);
    top->cd();
    nNode[iLay] = new TNode(Form("target_layer%02d",iLay), Form("target_layer%02d",iLay), Form("starget_layer%02d",iLay), 0., 0., (TargetLowerZ+TargetLength/2.), "");
    nNode[iLay]->SetLineColor(5);
    Nodes->Add(nNode[iLay]);
  }
#ifdef WIN32
		free(ptarget_layer);
		free(nNode);
#endif    
  return Nodes;
  
}


//______________________________________________________________________
void IlcTARGETvORKA::BuildGeometry() {

  IlcWarning("Not implemented");

}

//______________________________________________________________________
void IlcTARGETvORKA::CreateGeometry() {

  //square fiber version
  if(GetMinorVersion()==1) CreateGeometryV1();

  //poligonal version
  else if(GetMinorVersion()==2) CreateGeometryV2();
  
  else IlcFatal(Form("Geometry version %d not defined", GetMinorVersion()));

}

//______________________________________________________________________
void IlcTARGETvORKA::CreateGeometryV1() {
  //square fiber version

  TGeoMedium *pMed2=gGeoManager->GetMedium("TARGET_Air");
  TGeoMedium *pMed13=gGeoManager->GetMedium("TARGET_Scintillator");
  
  TGeoVolume *ilc = gGeoManager->GetVolume("ILCM");

  IlcTARGETParam *Param = new IlcTARGETParam(GetMinorVersion());
  
  Double_t LayerThick = Param->GetLayerThick();
  Double_t TargetThick = Param->GetTargetThick();
  Double_t TargetLength = Param->GetTargetLength();
  Double_t TargetLowerZ = Param->GetTargetLowerZ();
  const Int_t nLayers = Param->GetNlayers();
  Int_t nLadders = Param->GetNLadders();
  Int_t nSections = nLadders;
  
  // SHAPES, VOLUMES AND GEOMETRICAL HIERARCHY
  // Shape: tracking_cylinder type: TGeoTubeSeg
  Double_t dx, dy, dz,rmin,rmax,phi1,phi2;
  rmin = 0.;
  rmax = TargetThick;
  dx   = LayerThick/2.;
  dz   = TargetLength/2.;
  phi1 = 45.;
  phi2 = 360.;
  TGeoTube *ptarget_cylinder = new TGeoTube("target_cylinder",rmin,rmax,dz);
  // Volume: TARGETV
  TGeoVolume *pTARGETV = new TGeoVolume("TARGET_TARGETV",ptarget_cylinder, pMed2);
  //     pTARGETV->SetLineColor(23);

  TGeoRotation rot("rot", 0., 0., 0.);
  TGeoTranslation trans(0., 0., (TargetLowerZ+TargetLength/2.));
  TGeoCombiTrans *mat = new TGeoCombiTrans(trans, rot);
  mat->RegisterYourself();

  ilc->AddNode(pTARGETV, 1, mat);
  
#ifdef WIN32
	//TGeoPgon **ptarget_layer		= (TGeoPgon **)malloc(nLayers);
	//TGeoBBox **ptarget_layer2	= (TGeoBBox **)malloc(nLayers);
	//TGeoCompositeShape **ptarget_layer_cs = (TGeoCompositeShape **)malloc(nLayers);
	//TGeoVolume **ptarget_layerV	= (TGeoVolume **)malloc(nLayers);

	//TGeoBBox **ptarget_ladder	= (TGeoBBox **)malloc(nLayers);
	//TGeoVolume **ptarget_ladderV = (TGeoVolume **)malloc(nLayers);
	//TGeoRotation **rot_layer		= (TGeoRotation **)malloc(nLayers*4);
	//TGeoCombiTrans **mat_layer	= (TGeoCombiTrans **)malloc(nLayers*4);

	TGeoPgon **ptarget_layer		= new TGeoPgon *[nLayers];
	TGeoBBox **ptarget_layer2	= new TGeoBBox *[nLayers];
	TGeoCompositeShape **ptarget_layer_cs = new TGeoCompositeShape *[nLayers];
	TGeoVolume **ptarget_layerV	= new TGeoVolume *[nLayers];

	TGeoBBox **ptarget_ladder	= new TGeoBBox *[nLayers];
	TGeoVolume **ptarget_ladderV = new TGeoVolume *[nLayers];
	TGeoRotation **rot_layer		= new TGeoRotation *[nLayers*4];
	TGeoCombiTrans **mat_layer	= new TGeoCombiTrans *[nLayers*4];
#else
  TGeoPgon *ptarget_layer[nLayers];
  TGeoBBox *ptarget_layer2[nLayers];
  TGeoCompositeShape *ptarget_layer_cs[nLayers];
  TGeoVolume *ptarget_layerV[nLayers];
  
  TGeoBBox *ptarget_ladder[nLayers];
  TGeoVolume *ptarget_ladderV[nLayers];
  TGeoRotation *rot_layer[nLayers*4];
  TGeoCombiTrans *mat_layer[nLayers*4];
#endif 
  
  for(Int_t iLay=0; iLay<nLayers; iLay++){    
    

    dy = (2*iLay+1)*LayerThick/2.;

    Bool_t checkLen = kTRUE;
    Double_t dytmp =TMath::Min(dy, TMath::Sqrt(TargetThick*TargetThick-(((iLay+1)*LayerThick)*((iLay+1)*LayerThick))));

//     IlcInfo(Form("===> dy: %f %f %f %f", dy,  TMath::Sqrt(TargetThick*TargetThick-(((iLay+1)*LayerThick)*((iLay+1)*LayerThick))), TMath::Min(dy, TMath::Sqrt(TargetThick*TargetThick-(((iLay+1)*LayerThick)*((iLay+1)*LayerThick)))), 0.25*((Int_t)(dytmp/0.25)) ));

    if(dytmp<dy){
      checkLen = kFALSE;
      dy=0.25*((Int_t)(dytmp/0.25));
    }

    if(checkLen){
      
      ptarget_layer[iLay] = new TGeoPgon(Form("target_layer%02d",iLay),phi1,phi2,nSections,2);
      ptarget_layer[iLay]->DefineSection(0,-dz,(iLay)*LayerThick,(iLay+1)*LayerThick);
      ptarget_layer[iLay]->DefineSection(1,dz,(iLay)*LayerThick,(iLay+1)*LayerThick);
      ptarget_layerV[iLay] = new TGeoVolume(Form("TARGET_layer%02d",iLay),ptarget_layer[iLay], pMed2);
      
      pTARGETV->AddNode(ptarget_layerV[iLay], iLay+1);
      
      
      ptarget_ladder[iLay] = new TGeoBBox(Form("target_layer%02d_lad",iLay), dx, dy, dz);
      ptarget_ladderV[iLay] = new TGeoVolume(Form("TARGET_layer%02d_lad",iLay),ptarget_ladder[iLay], pMed13);
      
      Float_t checkx1[4] = {0.5,-0.5,-0.5,0.5};
      Float_t checky1[4] = {0.5,0.5,-0.5,-0.5};
      Float_t checkx2[4] = {1,0,-1,0};
      Float_t checky2[4] = {0,1,0,-1};
      for(Int_t iLad=0; iLad<nLadders; iLad++){
	Float_t ladder_angle=90.*iLad;
	Float_t matdx = (iLay*checkx2[iLad] + checkx1[iLad])*LayerThick;
	Float_t matdy = (iLay*checky2[iLad] + checky1[iLad])*LayerThick;
	rot_layer[iLay+iLad] = new TGeoRotation(Form("rot_lay%d_lad%d",iLay,iLad),90.,ladder_angle,90.,90.+ladder_angle,0.,0.);
	mat_layer[iLay+iLad] = new TGeoCombiTrans(Form("mat_lay%d_lad%d",iLay,iLad), matdx, matdy, 0., rot_layer[iLay+iLad]);
	mat_layer[iLay+iLad]->RegisterYourself();
	
	ptarget_layerV[iLay]->AddNode(ptarget_ladderV[iLay], iLad+1, mat_layer[iLay+iLad]);
      }
    }
    else {



      ptarget_ladder[iLay] = new TGeoBBox(Form("target_layer%02d_lad",iLay), dx, dy, dz);
      ptarget_ladderV[iLay] = new TGeoVolume(Form("TARGET_layer%02d_lad",iLay),ptarget_ladder[iLay], pMed13);

      Float_t checkx1[4] = {0.5,-0.,-0.5,0.};
      Float_t checky1[4] = {0.,0.5,-0.,-0.5};
      Float_t checkx2[4] = {1,0,-1,0};
      Float_t checky2[4] = {0,1,0,-1};
      TString s_layer_cs;
      for(Int_t iLad=0; iLad<nLadders; iLad++){



	Float_t ladder_angle=90.*iLad;
	Float_t matdx = (iLay*checkx2[iLad] + checkx1[iLad])*LayerThick;
	Float_t matdy = (iLay*checky2[iLad] + checky1[iLad])*LayerThick;
	rot_layer[iLay+iLad] = new TGeoRotation(Form("rot_lay%d_lad%d",iLay,iLad),90.,ladder_angle,90.,90.+ladder_angle,0.,0.);
	mat_layer[iLay+iLad] = new TGeoCombiTrans(Form("mat_lay%d_lad%d",iLay,iLad), matdx, matdy, 0., rot_layer[iLay+iLad]);
	mat_layer[iLay+iLad]->RegisterYourself();

	ptarget_layer2[iLay] = new TGeoBBox(Form("target_layer%02d",iLay),dx, dy, dz);
	s_layer_cs.Append(Form("target_layer%02d:mat_lay%d_lad%d",iLay,iLay,iLad));
	if(iLad<nLadders-1) s_layer_cs.Append("+");
      }

//       IlcInfo(Form("--->%s<---",s_layer_cs.Data()));
      ptarget_layer_cs[iLay] = new TGeoCompositeShape(Form("target_layer%02d",iLay), s_layer_cs.Data());
      ptarget_layerV[iLay] = new TGeoVolume(Form("TARGET_layer%02d",iLay),ptarget_layer_cs[iLay], pMed2);

      for(Int_t iLad=0; iLad<nLadders; iLad++){
	ptarget_layerV[iLay]->AddNode(ptarget_ladderV[iLay], iLad+1, mat_layer[iLay+iLad]);
      }
      
      pTARGETV->AddNode(ptarget_layerV[iLay], iLay+1);
      

    }
  }
  
 #ifdef WIN32
		//free(ptarget_layer);
		//free(ptarget_layer2);
		//free(ptarget_layer_cs);
		//free(ptarget_layerV);

		//free(ptarget_ladder);
		//free(ptarget_ladderV);
		//free(rot_layer);
		//free(mat_layer);

		delete[] ptarget_layer;
		delete[] ptarget_layer2;
		delete[] ptarget_layer_cs;
		delete[] ptarget_layerV;

		delete[] ptarget_ladder;
		delete[] ptarget_ladderV;
		delete[] rot_layer;
		delete[] mat_layer;
#endif 

}

//______________________________________________________________________
void IlcTARGETvORKA::CreateGeometryV2() {
  //poligonal version

    TGeoMedium *pMed2=gGeoManager->GetMedium("TARGET_Air");
    TGeoMedium *pMed13=gGeoManager->GetMedium("TARGET_Scintillator");

    
  TGeoVolume *ilc = gGeoManager->GetVolume("ILCM");

  IlcTARGETParam *Param = new IlcTARGETParam(GetMinorVersion());

  Double_t LayerThick = Param->GetLayerThick();
  Double_t TargetThick = Param->GetTargetThick();
  Double_t TargetLength = Param->GetTargetLength();
  Double_t TargetLowerZ = Param->GetTargetLowerZ();
  const Int_t nLayers = Param->GetNlayers();
  Int_t NLadders = Param->GetNLadders();
  Int_t nSections = NLadders;

  // SHAPES, VOLUMES AND GEOMETRICAL HIERARCHY
    // Shape: tracking_cylinder type: TGeoTubeSeg
    Double_t dz,rmin,rmax,phi1,phi2;
    rmin = 0.;
    rmax = TargetThick;
    dz   = TargetLength/2.;
    phi1 = 0.;
    phi2 = 360.;
    TGeoTube *ptarget_cylinder = new TGeoTube("target_cylinder",rmin,rmax,dz);
   // Volume: TARGETV
    TGeoVolume *pTARGETV = new TGeoVolume("TARGET_TARGETV",ptarget_cylinder, pMed2);
//     pTARGETV->SetLineColor(23);

    TGeoRotation rot("rot", 0., 0., 0.);
    TGeoTranslation trans(0., 0., (TargetLowerZ+TargetLength/2.));
    TGeoCombiTrans *mat = new TGeoCombiTrans(trans, rot);
    mat->RegisterYourself();

    ilc->AddNode(pTARGETV, 1, mat);
    
#ifdef WIN32
	TGeoPgon **ptarget_layer		= (TGeoPgon **)malloc(nLayers);
	TGeoVolume **ptarget_layerV	= (TGeoVolume **)malloc(nLayers);
#else
	TGeoPgon *ptarget_layer[nLayers];
    TGeoVolume *ptarget_layerV[nLayers];
#endif 
    
    Int_t iLay=0;
    ptarget_layer[iLay] = new TGeoPgon(Form("target_layer%02d",iLay),phi1,phi2,nSections,2);
    ptarget_layer[iLay]->DefineSection(0,-TargetLength/2.,0.*LayerThick,0.5*LayerThick);
    ptarget_layer[iLay]->DefineSection(1,TargetLength/2.,0.*LayerThick,0.5*LayerThick);
    ptarget_layerV[iLay] = new TGeoVolume(Form("TARGET_layer%02d",iLay),ptarget_layer[iLay], pMed13);
    ptarget_layer[iLay]->Divide(ptarget_layerV[iLay], Form("TARGET_layer%02d_lad",iLay),2,1.,0.,360.);
    pTARGETV->AddNode(ptarget_layerV[iLay], iLay+1);
    
    for(Int_t iLay=1; iLay<nLayers; iLay++){
      ptarget_layer[iLay] = new TGeoPgon(Form("target_layer%02d",iLay),phi1,phi2,nSections,2);
      ptarget_layer[iLay]->DefineSection(0,-TargetLength/2.,(iLay-0.5)*LayerThick,(iLay+0.5)*LayerThick);
      ptarget_layer[iLay]->DefineSection(1,TargetLength/2.,(iLay-0.5)*LayerThick,(iLay+0.5)*LayerThick);
      ptarget_layerV[iLay] = new TGeoVolume(Form("TARGET_layer%02d",iLay),ptarget_layer[iLay], pMed13);
      ptarget_layer[iLay]->Divide(ptarget_layerV[iLay], Form("TARGET_layer%02d_lad",iLay),2,nSections,0.,360./nSections);
      pTARGETV->AddNode(ptarget_layerV[iLay], iLay+1);
    }

#ifdef WIN32
		free(ptarget_layer);
		free(ptarget_layerV);
#endif 
}


//______________________________________________________________________
void IlcTARGETvORKA::CreateMaterials() {

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
    
    
    // --- The polysterene scintillator (CH) ---
    Float_t aP[2] = {12.011, 1.00794} ;
    Float_t zP[2] = {6.0, 1.0} ;
    Float_t wP[2] = {1.0, 1.0} ;
    Float_t dP = 1.032 ;
    IlcMixture(13, "Polystyrene", aP, zP, dP, -2, wP) ;
    
    // The scintillator fibers made of Polystyrene scintillator
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
//     Int_t   isxfld = ((IlcMagF*)TGeoGlobalMagField::Instance()->GetField())->Integ() ;
//     Float_t sxmgmx = ((IlcMagF*)TGeoGlobalMagField::Instance()->GetField())->Max() ;
  IlcMedium(13, "Scintillator", 13,isvol, ifield, par[2], par[3], par[4], par[5], par[6], par[7]);


}
//______________________________________________________________________
void IlcTARGETvORKA::InitIlcTARGETgeom() {

    // routine initializes the Class IlcTARGETgeom from the TARGET geometry
    // sturture.
    // Inputs:
    //   none.
    // Outputs:
    //   none.
    // Return:
    //   none.
  //FIXME in base alla versione
    Float_t pixelx,pixely,pixelz;
    Int_t npix;
    IlcTARGETParam *Param=new IlcTARGETParam(GetMinorVersion());
    const Int_t knlayers = Param->GetNlayers();
    TString path,shapeName,name_ladder;
    char name[100],name_lad[100];

    TString name_lay,name_endcap,name_ladder2;
    TGeoVolume *TARGETV;
    IlcTARGETDetector *idet=new IlcTARGETDetector[knlayers];
    for (Int_t i=0;i<knlayers;i++)
      idet[i]=kSPD;

    Int_t  *nlad=new Int_t[knlayers],*ndet= new Int_t[knlayers];


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

    for (i=0; i<knlayers;i++) ndet[i]=1;//Param->GetNdetectors(i);


    for (i=0;i<knlayers;i++) mod += nlad[i]*ndet[i];

    if (GetTARGETgeom()!=0) delete GetTARGETgeom();
    SetTARGETgeom(0);
    if (GetTARGETgeom2()!=0) delete GetTARGETgeom2();
    SetTARGETgeom2(0);

    IlcTARGETgeom* geom = new IlcTARGETgeom(0,knlayers,nlad,ndet,mod,GetMinorVersion());
    SetTARGETgeom2(geom);
    IlcTARGETgeom* geom2 = new IlcTARGETgeom(0,knlayers,nlad,ndet,mod,GetMinorVersion());
    SetTARGETgeom(geom2);
    mod = 0;

    TARGETV=gGeoManager->GetVolume("TARGET_TARGETV");
    for (lay=1;lay<=knlayers;lay++) {
      sprintf(name,"TARGET_layer%02d",lay-1);
      sprintf(name_lad,"TARGET_layer%02d_lad",lay-1);
      
      name_lay=TARGETV->GetNode(lay-1)->GetName();
      if (!(name_lay.Contains(name))) continue;
      for (lad=1;lad<=nlad[lay-1];lad++) {
	
	name_ladder=gGeoManager->GetVolume(name)->GetNode(lad-1)->GetName();
	if ( !(name_ladder.Contains(name_lad)) ) continue;

	path.Form("/%s_1/%s_%d/%s/%s", gGeoManager->GetTopVolume()->GetName(), TARGETV->GetName(), gGeoManager->GetVolume("ILCM")->GetNode(0)->GetNumber(), TARGETV->GetNode(lay-1)->GetName(), Form("TARGET_layer%02d_lad_%d",lay-1, lad) );
	// // // // 	IlcDebug(1, Form("path: %s",path.Data()));
// 	IlcInfo(Form("path: %s",path.Data()));
	
	det=1;
	gMC->GetTransformation(path.Data(),materix);
	
	gMC->GetShape(path.Data(),shapeName,shapePar);
	shapeParF.Set(shapePar.GetSize());
	for (i=0;i<shapePar.GetSize();i++) shapeParF[i]=shapePar[i];

	if(GetMinorVersion()==1){  //square fiber version
	  shapeParF[0]=shapePar[1];
	  shapeParF[1]=shapePar[0];
	}

// 	IlcInfo(Form("path: %s mod: %d lay: %d lad: %d det: %d Name: %s NParams: %d", path.Data() ,mod,lay,lad,det, shapeName.Data(), shapeParF.GetSize()));

// 	for (i=0;i<shapePar.GetSize();i++)
// 	  cout << Form("shapeParF[%d]=%f\n", i, shapeParF[i]);
	
        geom->CreatMatrix(mod,lay,lad,det,idet[lay-1],trans,rot);

        if(GetMinorVersion()==1){  //square fiber version
          Double_t LayTransMat[3] = {materix.GetTranslation()[0], materix.GetTranslation()[1], 0.};
          geom->SetTrans(mod,LayTransMat);
        }
	else if (GetMinorVersion()==2){  //poligonal version
	  
	  Double_t Rlay = (shapePar[6]+shapePar[5])/2.;
	  Double_t LayTransMat[3]={Rlay*TMath::Cos(shapePar[1]*(lad-0.5)*TMath::DegToRad()), Rlay*TMath::Sin(shapePar[1]*(lad-0.5)*TMath::DegToRad()), 0.};
	  
	  if(lay==1)
	    geom->SetTrans(mod,materix.GetTranslation());
	  else
	    geom->SetTrans(mod,LayTransMat);
	}
	else
	  IlcFatal(Form("Geometry version %d not defined", GetMinorVersion()));

	GetPixelDimension(lay-1,pixelx,pixely,pixelz);
	geom->SetPixelDimension(lay-1,pixelx,pixely,pixelz);
// 	IlcInfo(Form("Layer: %d\t X: %f\t Y:%f\tZ:%f\t\n",lay-1,pixelx,pixely,pixelz));
	
	matrixInv=materix.Inverse();
	geom->SetRotMatrix(mod,matrixInv.GetRotationMatrix());
	
	geom->ReSetShape2(lay-1,new IlcTARGETgeomSPDFiberSegm(lay-1,geom, shapeParF.GetSize(), shapeParF.GetArray()));
	
	//-------------->Prede il Numero di Pixel
	geom->GetNPixelX(lay-1,npix);
	
	mod++;
	// // // // 	IlcDebug(1,Form("module: %d",mod));
      }
    }
    
    delete [] idet;
    delete [] nlad;
    delete [] ndet;

    delete Param;
    return;
}

//______________________________________________________________________
void IlcTARGETvORKA::Init() {
    //     Initialise the TARGET after it has been created.
    // Inputs:
    //   none.
    // Outputs:
    //   none.
    // Return:
    //   none.

    IlcInfo(Form("Minor version %d",GetMinorVersion()));
    //Mionor version: 1=square; 2=poligonal

    IlcTARGETParam *Param = new IlcTARGETParam(GetMinorVersion());

    Float_t TARGETpixelx,TARGETpixely,TARGETpixelz;
    Int_t TARGETNlayersBarrel = Param->GetNlayers();
    if(GetMinorVersion()==2){
  
      TARGETpixely=5.*1.e3;
      TARGETpixelz=50.*1.e3;
      printf("Pixel Dimension in um:\n");
      for (Int_t lay=0;lay<TARGETNlayersBarrel;lay++){
	TARGETpixelx=2.*TMath::Pi()*(lay+0.5)*5.*1.e3/((lay==0)?1.:(24.*lay));
	SetPixelDimension(lay,TARGETpixelx,TARGETpixely,TARGETpixelz);
	GetPixelDimension(lay,TARGETpixelx,TARGETpixely,TARGETpixelz);
	if(lay)
	  printf("Layer: %d\t X: %f\t Y:%f\tZ:%f\t\n",lay+1,TARGETpixelx,TARGETpixely,TARGETpixelz);
	else
	  printf("Layer: %d\t X: %s\t Y:%f\tZ:%f\t\n",lay+1,"none",TARGETpixely,TARGETpixelz);	    
      }

    }
    else if(GetMinorVersion()==1){

      TARGETpixelx=5.*1.e3;
      TARGETpixely=5.*1.e3;
      TARGETpixelz=50.*1.e3;
      printf("Pixel Dimension in um:\n");
      for (Int_t lay=0;lay<TARGETNlayersBarrel;lay++){
	SetPixelDimension(lay,TARGETpixelx,TARGETpixely,TARGETpixelz);
	GetPixelDimension(lay,TARGETpixelx,TARGETpixely,TARGETpixelz);

	printf("Layer: %d\t X: %f\t Y:%f\tZ:%f\t\n",lay+1,TARGETpixelx,TARGETpixely,TARGETpixelz);
      }
    }

    
    
    
    //
    if (fRead[0]=='\0') strncpy(fRead,fEuclidGeomDet,60);
    if (fWrite[0]=='\0') strncpy(fWrite,fEuclidGeomDet,60);
    if (GetTARGETgeom()!=0) SetTARGETgeom(0x0);
    if (GetTARGETgeom2()!=0) SetTARGETgeom2(0x0);
    IlcTARGETgeom* geom = new IlcTARGETgeom();
    geom->SetDetVersion(GetMinorVersion());
    SetTARGETgeom(geom);
    IlcTARGETgeom* geom2 = new IlcTARGETgeom();
    geom2->SetDetVersion(GetMinorVersion());
    SetTARGETgeom2(geom2);

    
//     if (fGeomDetIn) GetTARGETgeom()->ReadNewFile(fRead);
//     else if (GetMinorVersion()==1) 
      
    this->InitIlcTARGETgeom();

//     if (fGeomDetOut) GetTARGETgeom()->WriteNewFile(fWrite);

    IlcTARGET::Init();

}
//______________________________________________________________________
void IlcTARGETvORKA::SetDefaults() {
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


    IlcTARGETsegmentationSPD2* seg0 = (IlcTARGETsegmentationSPD2*)fDetTypeSim->GetSegmentationModel2(0);

//if (GetMinorVersion() !=3 ) cout <<"in IlcTARGETvORKA: Minor Version: "<<GetMinorVersion()<<endl;
    IlcTARGETParam *Param;
    Param=new IlcTARGETParam(GetMinorVersion());

    nlayers=Param->GetNlayers();
    nendcaps=Param->GetNendcaps();
    nlayersStrip=Param->GetNlayersStrip();
    nendcapsStrip=Param->GetNendcapsStrip();
    
    for (lay=0;lay<nlayers+nendcaps; lay++) {
      s0 = (IlcTARGETgeomSPD*) (GetTARGETgeom2())->GetShape2(lay);// Get shape info. Do it
//       IlcInfo(Form("%d %f %f %f\n", lay, s0->GetDx()*2.*kconv, s0->GetDy()*2.*kconv, s0->GetDz()*2.*kconv));
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
// // // //         IlcDebug(1,Form("layer: %d, kSPD Dx= %f, Dy= %f, Dz= %f",lay,s0->GetDx(),s0->GetDy(),s0->GetDz()));
        (GetTARGETgeom2())->GetNPixelZ(lay,npixelz);
        (GetTARGETgeom2())->GetNPixelX(lay,npixelx);
	   
	    
	     //cout<<"Sto usando questo 3"<<endl;
// // // // 	    cout<<"Pixel estituiti da GetTARGETGeom: Layer: "<<lay <<" Numero di Pixel lungo X "<<npixelx<<" Numero di Pixel lungo Z "<<npixelz<<endl;
	    
	    
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

}

//______________________________________________________________________
void IlcTARGETvORKA::DrawModule() const {

  return;

}
//______________________________________________________________________
void IlcTARGETvORKA::StepManager() {
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
    Int_t copy,id;
    TLorentzVector position, momentum;
    static TLorentzVector position0;
    static Int_t stat0=0;
    Int_t layer, ladder;


#ifdef WIN32
// patch to fix memory overwriting problem in TG4StepManager
//	TString VolumeName=gMC->CurrentVolName();
	TString  VolumeName = (TG4StepManager::Instance())->GetCurrentPhysicalVolume()->GetLogicalVolume()->GetName();
    //cout<< "STEMANAGER FOR GEOMETRY GEANT4 - Target " << VolumeName <<endl;
#else
	TString VolumeName;
    VolumeName=gMC->CurrentVolName();
#endif

    if (!(this->IsActive())) return;

    if (!(gMC->TrackCharge())) return;

    id=gMC->CurrentVolID(copy);

    Bool_t sensvol = kFALSE;

    if (GetMinorVersion() == 1 || GetMinorVersion() == 2) {
      if (VolumeName.BeginsWith("TARGET_") && VolumeName.Contains("layer") && VolumeName.Contains("lad") )
	sensvol=kTRUE;      
    }

    if(!sensvol) return;

// // // //     cout << gMC->CurrentVolPath() << endl;

// // // //     if (sensvol && (gMC->IsTrackExiting())) {
// // // //         copy = fTrackReferences->GetEntriesFast();
// // // //         TClonesArray &lTR = *fTrackReferences;
// // // //         // Fill TrackReference structure with this new TrackReference.
// // // //         new(lTR[copy]) IlcTrackReference(gIlc->GetMCApp()->GetCurrentTrackNumber());
// // // // 
// // // //     } // if Outer TARGET mother Volume


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
    if (GetMinorVersion()==1 || GetMinorVersion() == 2) {
        if (VolumeName.BeginsWith("TARGET_") && VolumeName.Contains("lad") ) {
            id=gMC->CurrentVolOffID(1,copy);
            layer=copy;
	    vol[0]=layer;
            
	    id=gMC->CurrentVolID(copy);
	    ladder = copy;
	    vol[2]=ladder;

	    IlcDebug(1,Form("CurrentVolPath(): %s  layer: %d ladder: %d",gMC->CurrentVolPath(), layer, ladder));
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
// // // //         for (p=0;p<5;p++) IlcDebug(1,Form("The track is Entering!!!... vol[%d]: %d", p, vol[p]));
        //for (s=0;s<4;s++) IlcDebug(1,Form("position[%d]=%f",s,position[s]));
        //for (s=0;s<4;s++) IlcDebug(1,Form("position0[%d]=%f",s,position[s]));
        stat0 = vol[3];
        return;
    } // end if IsEntering
    // Fill hit structure with this new hit.

// // // //     for (p=0;p<5;p++) IlcDebug(1,Form("The track is not entering!!!... vol[%d]: %d", p,vol[p]));

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
    Float_t EDep = gMC->Edep();
    if(EDep>0.)
      new(lhits[fNhits++]) IlcTARGEThit(fIshunt,gIlc->GetMCApp()->GetCurrentTrackNumber(),vol,
                                   EDep,gMC->TrackTime(),position,
                                   position0,momentum);
// // // //     Double_t x=0.,y=0.,z=0.,x0=0.,y0=0.,z0=0.,t=0.;
// // // //     hit->GetPositionL(x,y,z);
// // // //     hit->GetPositionL0(x0,y0,z0,t);
// // // // 
// // // //     cout << "Position: x="<< x << " y=" << y << " z=" << z << " layer=" << hit->GetLayer() << " ladder=" << hit->GetLadder() << " det=" << hit->GetDetector() << endl; 
// // // //     cout << "Position 0: x0="<< x0 << " y0=" << y0 << " z0=" << z0 << " layer=" << hit->GetLayer() << " ladder=" << hit->GetLadder() << " det=" << hit->GetDetector() << endl; 

    position0 = position;
    stat0 = vol[3];
    return;
}

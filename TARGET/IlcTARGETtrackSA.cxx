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

/* $Id: IlcTARGETtrackSA.cxx,v 1.2 2009/07/01 11:32:56 cgatto Exp $ */

////////////////////////////////////////////////////
//  Stand alone track class                       //
//  Origin:  Elisabetta Crescio                   //
//  e-mail:  crescio@to.infn.it                   //
//  it is a V2 track with a possible number       //
//  of cluster equal to fgkMaxNumberOfClusters    //
////////////////////////////////////////////////////

#include "IlcTARGETgeom.h"
#include "IlcTARGETtrackSA.h"


ClassImp(IlcTARGETtrackSA)

//_____________________________________
IlcTARGETtrackSA:: IlcTARGETtrackSA() : 
  IlcTARGETtrackMI(),
  fNSA(0),fnLayers(0),fnLayers2(0)
{
// Default constructor  
  SetNumberOfClusters(0);
  SetNumberOfClustersSA(0);
  ResetIndexSA();
  for(Int_t nlay=0;nlay<fgkLayers;nlay++){ 
    SetNumberOfMarked(nlay,0);
  }
  ResetMarked();
}


//___________________________________________________
IlcTARGETtrackSA::IlcTARGETtrackSA(const IlcTARGETtrackMI& t) : 
IlcTARGETtrackMI(t),
fNSA(0),fnLayers(0),fnLayers2(0){
// Copy a V2 track into a SA track
  SetNumberOfClustersSA(0);
  ResetIndexSA();
  for(Int_t nlay=0;nlay<fgkLayers;nlay++){ 
    SetNumberOfMarked(nlay,0);
  }
  ResetMarked();

}
//___________________________________________________
IlcTARGETtrackSA::IlcTARGETtrackSA(const IlcTARGETtrackSA& t) : 
IlcTARGETtrackMI(t),
fNSA(t.fNSA),fnLayers(t.fnLayers),fnLayers2(t.fnLayers2){
// Copy constructor


  ResetIndexSA();
  ResetMarked();
  Int_t number = t.GetNumberOfClustersSA();
  SetNumberOfClustersSA(number);
  for(Int_t nlay=0;nlay<fgkLayers;nlay++){
    SetNumberOfMarked(nlay,t.GetNumberOfMarked(nlay));
  }
  for(Int_t i=0;i<number;i++){
    fSain[i]=t.fSain[i];
  }
  for(Int_t nlay=0;nlay<fgkLayers;nlay++){
    for(Int_t i=0;i<t.GetNumberOfMarked(nlay);i++){
      fCluMark[nlay][i]=t.fCluMark[nlay][i];
    }
  }


}
//____________________________________________________
IlcTARGETtrackSA::IlcTARGETtrackSA(IlcTARGETgeom* geom,Int_t layer, Int_t ladder, Int_t detector, Double_t Ycoor, Double_t Zcoor, Double_t phi, Double_t tanlambda, Double_t curv, Int_t lab ):
fNSA(0) 
{
  // standard constructor. Used for TARGET standalone tracking

  if(!geom){
    Fatal("IlcTARGETtrackSA","TARGET geometry not found - Abort\n");
    return;
  }
  // get the azimuthal angle of the detector containing the innermost
  // cluster of this track (data member fAlpha)
  Float_t rotmatr[9];
  geom->GetRotMatrix(layer,ladder,detector,rotmatr);
  Float_t x=0,y=0,z=0;
  geom->GetTrans(layer,ladder,detector,x,y,z);

  Double_t fi=TMath::ATan2(rotmatr[1],rotmatr[0]);
  fi+=TMath::Pi()/2;
  if(fi<-TMath::Pi()/2) fi+=2*TMath::Pi();
  Double_t cp=TMath::Cos(fi), sp=TMath::Sin(fi);
  Double_t sX=x*cp+y*sp;
  if(sX<0){
    fi-=TMath::Pi();
    if(fi<-TMath::Pi()/2) fi+=2*TMath::Pi();
    sX=-sX;
  }

  Double_t sAlpha=fi;
  fdEdx = 0;

  Double_t conv=GetBz()*kB2C;
  Double_t sC[] = {0.000009, // 0.000009
                   0.,
                   0.000003, //0.000030
                   0.,
	           0.,
	           0.000001, //0.000001
	           0.,
	           0.,
	           0.,
	           0.000002, //0.000002
	           0.,
	           0.,
	           0.,
	           0.,
		   0.000001/(conv*conv)}; //0.0000001

  Double_t sP[] = {Ycoor,
		   Zcoor,
                   TMath::Sin(phi-sAlpha),
		   tanlambda,
		   curv/conv};

  if(fabs(rotmatr[8])<0.5){
    sX=Zcoor;
    sP[1]=z;
  }

  Set(sX,sAlpha,sP,sC,TMath::Sign(1.,TMath::Cos(phi-sAlpha)));

  for(Int_t i=0; i<2*kMaxLayer; i++) fIndex[i] = 0;  // to be set explicitely

  for(Int_t i=0; i<kMaxClusters; i++) fdEdxSample[i] = 0; 

  SetNumberOfClusters(0);
  SetNumberOfClustersSA(0);
  fnLayers=0;
  fnLayers2=0;
  for(Int_t nlay=0;nlay<fgkLayers;nlay++) SetNumberOfMarked(nlay,0);
  ResetIndexSA();
  ResetMarked();
  SetChi2(0);
  SetMass(0.139);    // pion mass
  SetLabel(lab); 
  
}

//____________________________________________________________
void IlcTARGETtrackSA::AddClusterSA(Int_t layer, Int_t clnumb) {
  // add one clusters to the list (maximum number=fgkMaxNumberOfClusters)
  Int_t presnum = GetNumberOfClustersSA();
  if(presnum>=fgkMaxNumberOfClusters){
    Warning("AddClusterSA","Maximum number of clusters already reached. Nothing is done\n");
    return;
  }

  fSain[presnum] = (layer<<kClusterLayerShift)+clnumb;  
  presnum++;
  SetNumberOfClustersSA(presnum);
}

//____________________________________________________________
void IlcTARGETtrackSA::AddClusterMark(Int_t layer, Int_t clnumb) {
  // add one clusters to the list (maximum number=fgkMaxNumberOfClusters)
  Int_t presnum = GetNumberOfMarked(layer);
  //  printf("presnum=%d\n",presnum);
  if(presnum>=fgkMaxNumberOfClustersL){
    Warning("AddClusterMark","Maximum number of clusters already reached. Nothing is done\n");
    return;
  }
  if(presnum==0) fnLayers++;
  if(presnum==1) fnLayers2++;
  fCluMark[layer][presnum] = clnumb;  
  presnum++;
  SetNumberOfMarked(layer,presnum);
}

//____________________________________________________________
void IlcTARGETtrackSA::AddClusterV2(Int_t layer,Int_t clnumb) {
  // add one clusters to the list (maximum number=6)
  Int_t presnum = GetNumberOfClusters();
  if(presnum>=kMaxClusters){
    Warning("AddClusterV2","Maximum number of clusters already reached. Nothing is done\n");
    return;
   }    

  fIndex[presnum] = (layer<<kClusterLayerShift)+clnumb;  
  presnum++;
  SetNumberOfClusters(presnum);
}

//_____________________________________________________________
void IlcTARGETtrackSA::ResetMarked(){

  //Reset array of marked clusters
  for(Int_t nlay=0;nlay<fgkLayers;nlay++){
    for(Int_t k=0; k<fgkMaxNumberOfClustersL; k++) fCluMark[nlay][k]=0;
  }
  fnLayers=0;
  fnLayers2=0;
}















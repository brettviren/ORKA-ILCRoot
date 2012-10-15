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

/* $Id: IlcDCH.cxx,v 1.14 2009/12/11 10:40:28 vitomeg Exp $ */

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//  Transition Radiation Detector                                            //
//  This class contains the basic functions for the Transition Radiation     //
//  Detector.                                                                //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <Riostream.h>


#include <TFile.h>
#include <TGeometry.h>
#include <TLorentzVector.h>
#include <TMath.h>
#include <TNode.h>
#include <TPGON.h> 
#include <TParticle.h>
#include <TROOT.h>
#include <TTree.h>
#include <TVirtualMC.h>
#include <TTRD1.h>  
#include <TRotMatrix.h>
#include <TTUBE.h>
#include <TSPHE.h>
#include <TClonesArray.h>
#include <TGeoGlobalMagField.h>

#include "IlcConst.h"
#include "IlcDigit.h"
#include "IlcLoader.h"
#include "IlcLog.h"
#include "IlcMC.h"
#include "IlcMagF.h"
#include "IlcRun.h"
#include "IlcDCH.h"
//#include "IlcDCHdigit.h"
//#include "IlcDCHdigitizer.h"
//#include "IlcDCHdigitsManager.h"
#include "IlcDCHgeometry.h"
// #include "IlcDCHhit.h"
//#include "IlcDCHpoints.h"
//#include "IlcDCHrawData.h"
#include "IlcTrackReference.h"
//#include "IlcDCHSimParam.h"
//#include "IlcDCHRecParam.h"
//#include "IlcDCHCommonParam.h"
//#include "IlcDCHcalibDB.h"
#include "IlcDCHwireposition.h"
ClassImp(IlcDCH)
 
//_____________________________________________________________________________
IlcDCH::IlcDCH()
{
  //
  // Default constructor
  //

  fIshunt        = 0;
  fHits          = 0;
  fDigits        = 0;

  fDCHGeometry   = 0;
  fDCHwire       = 0;
  fGasDensity    = 0;
  fFoilDensity   = 0;

  fDrawTR        = 0;
  fDisplayType   = 0;
  fVersion       = 2;
  fDCHParam      = 0;
 
}
 
//_____________________________________________________________________________
IlcDCH::IlcDCH(const char *name, const char *title)
       : IlcDetector(name,title)
{
  //
  // Standard constructor for the DCH
  //

  // Check that FRAME is there otherwise we have no place where to
  // put DCH
  /*  IlcModule* frame = gIlc->GetModule("FRAME");
  if (!frame) {
    Error("Ctor","DCH needs FRAME to be present\n");
    exit(1);
  } 

  // Define the DCH geometry
  if ((frame->IsVersion() == 0) ||
      (frame->IsVersion() == 1)) {
   
  }
  else {
    Error("Ctor","Could not find vilcd FRAME version\n");
    exit(1);
    }*/
  // IlcModule* frame = gIlc->GetModule("DCH");
  
  fVersion=2;
  fDCHGeometry   = new IlcDCHgeometry();

  // Allocate the hit array
  if (fVersion==1) fHits          = new TClonesArray("IlcDCHhit",405);
  if (fVersion==2) fHits          = new TClonesArray("IlcDCHhit",405);
  gIlc->GetMCApp()->AddHitList(fHits);


  // Allocate the digits array
  fDigits        = 0;

  fIshunt        = 0;

  fGasDensity    = 0;
  fFoilDensity   = 0;

  fDrawTR        = 0;
  fDisplayType   = 0;
  
  TString STRtitle(title);
  fDCHParam= new IlcDCHParam();

  if (STRtitle.SubString("mu2e")!="" || STRtitle.SubString("ORKA")!="") {
    fDCHParam->SetExperiment(1);
    if (STRtitle.SubString("withhole")!="") {
      fDCHParam->SetExperimentSubVer(1);
    }
  }
  
  if (STRtitle.SubString("efault")!="") {
    fDCHParam->SetEndCapType(0);
  }
  else if (STRtitle.SubString("lane")!="") {
    fDCHParam->SetEndCapType(0);
  }
  else if (STRtitle.SubString("pheric")!="") {
    fDCHParam->SetEndCapType(1);
  }
  else if (STRtitle.SubString("geoDCHg4")!="") {
    fDCHParam->SetEndCapType(1);
    //fDCHParam->SetFlag_geoDCHg4(0);
  }
  else {
    fDCHParam->SetEndCapType(-1);
  }
  
  if (STRtitle.SubString("axial")!="") {
    fDCHParam->SetDrop(0.0);
  }

  if (STRtitle.SubString("Kapton_FW")!="") {
    fDCHParam->SetFWMaterialType(1);
  }
  else {
    fDCHParam->SetFWMaterialType(0);
  }
  
  
  fDCHGeometry->SetParam(fDCHParam);
// // // //   if (title)
// // // //   SetMarkerColor(kWhite);   

}

// // // // //_____________________________________________________________________________
// // // // IlcDCH::IlcDCH(const IlcDCH &dch):IlcDetector(dch)
// // // // {
// // // //   //
// // // //   // Copy constructor
// // // //   //
// // // // 
// // // //   ((IlcDCH &) dch).Copy(*this);
// // // // 
// // // // }

//_____________________________________________________________________________
IlcDCH::~IlcDCH()
{
  //
  // DCH destructor
  //

  fIshunt = 0;

  if (fDCHGeometry) {
    delete fDCHGeometry;
    fDCHGeometry  = 0;
  }
  if (fHits) {
    delete fHits;
    fHits      = 0;
  }
  if (fDCHParam) {
    delete fDCHParam;    
    fDCHParam      = 0;
  }


}

//_____________________________________________________________________________
/* void IlcDCH::Hits2Digits()
{
  //
  // Create digits
  //

  IlcDCHdigitizer digitizer("DCHdigitizer","DCH digitizer class");
  IlcLog::SetClassDebugLevel("DCHdigitizer",IlcDebugLevel());
  
  // Initialization
  digitizer.InitDetector();
    
  if (!fLoader->TreeH()) fLoader->LoadHits("read");
  fLoader->LoadDigits("recreate");
  IlcRunLoader* runLoader = fLoader->GetRunLoader(); 

  for (Int_t iEvent = 0; iEvent < runLoader->GetNumberOfEvents(); iEvent++) {
    digitizer.Open(runLoader->GetFileName().Data(), iEvent);
    digitizer.MakeDigits();
    digitizer.WriteDigits();
  }

  fLoader->UnloadHits();
  fLoader->UnloadDigits();

}

//_____________________________________________________________________________
void IlcDCH::Hits2SDigits()
{
  //
  // Create summable digits
  //

  IlcDCHdigitizer digitizer("DCHdigitizer","DCH digitizer class");
  // For the summable digits
  digitizer.SetSDigits(kTRUE);
  IlcLog::SetClassDebugLevel("DCHdigitizer",IlcDebugLevel());

  // Initialization
  digitizer.InitDetector();

  if (!fLoader->TreeH()) fLoader->LoadHits("read");
  fLoader->LoadSDigits("recreate");
  IlcRunLoader* runLoader = fLoader->GetRunLoader(); 

  for (Int_t iEvent = 0; iEvent < runLoader->GetNumberOfEvents(); iEvent++) {
    digitizer.Open(runLoader->GetFileName().Data(), iEvent);
    digitizer.MakeDigits();
    digitizer.WriteDigits();
  }

  fLoader->UnloadHits();
  fLoader->UnloadSDigits();
  
}

//_____________________________________________________________________________
IlcDigitizer* IlcDCH::CreateDigitizer(IlcRunDigitizer* manager) const
{
  //
  // Creates a new digitizer object
  //

  return new IlcDCHdigitizer(manager);

}

//_____________________________________________________________________________
void IlcDCH::SDigits2Digits()
{
  //
  // Create final digits from summable digits
  //

  // Create the DCH digitizer
  IlcDCHdigitizer digitizer("DCHdigitizer","DCH digitizer class");  
  IlcLog::SetClassDebugLevel("DCHdigitizer",IlcDebugLevel());

  // Set the parameter
  digitizer.SetEvent(gIlc->GetEvNumber());

  // Initialization
  digitizer.InitDetector();

  // Read the s-digits via digits manager
  IlcDCHdigitsManager sdigitsManager;
 
  IlcLog::SetClassDebugLevel("DCHdigitisManager",IlcDebugLevel());
  sdigitsManager.SetSDigits(kTRUE);
  sdigitsManager.CreateArrays();
  
  if (!fLoader->TreeS()) 
    if (fLoader->LoadSDigits("read"))
     {
       Error("SDigits2Digits","Error while reading SDigits for event %d",gIlc->GetEvNumber());
       return;
     }
  if (!fLoader->TreeS()) return;
  
  sdigitsManager.ReadDigits(fLoader->TreeS());

  // Add the s-digits to the input list 
  digitizer.AddSDigitsManager(&sdigitsManager);

  // Convert the s-digits to normal digits
  digitizer.SDigits2Digits();

  // Store the digits
  if (!fLoader->TreeD()) fLoader->MakeTree("D");
  if (digitizer.MakeBranch(fLoader->TreeD())){
    digitizer.WriteDigits();
  }

}

//_____________________________________________________________________________
void IlcDCH::Digits2Raw() 
{
  //
  // convert digits of the current event to raw data
  //

  fLoader->LoadDigits();
  TTree* digits = fLoader->TreeD();
  if (!digits) {
    Error("Digits2Raw", "no digits tree");
    return;
  }

  IlcDCHrawData rawWriter;
  //  rawWriter.SetDebug(2);
  if (!rawWriter.Digits2Raw(digits)) {
    Error("IlcDCH::Digits2Raw","The raw writer could not load the digits tree");
  }

  fLoader->UnloadDigits();

}
 */
// //_____________________________________________________________________________
// void IlcDCH::AddHit(Int_t track, Int_t det, Float_t *hits, Int_t q
// 		    , Bool_t inDrift, Float_t dW,Int_t status,Float_t *Pos_local)
// {
//   //
//   // Add a hit for the DCH
//   // 
// 
//   TClonesArray &lhits = *fHits;
//   IlcDCHhit *hit = new(lhits[fNhits++]) IlcDCHhit(fIshunt,track,det,hits,q,dW,status,Pos_local);
//   if (inDrift) {
//     hit->SetDrift();
//   }
//   else {
//     hit->SetAmplification();
//   }
//   if (q < 0) {
//     hit->SetTRphoton();
//   }
// 
// }

//_____________________________________________________________________________
IlcDCHhit * IlcDCH::AddHit(Int_t track, UShort_t superlayer, UShort_t ring, UShort_t wire, ULong_t celid, UShort_t trkStat, Float_t *hits, UShort_t ClMult)
{
  //
  // Add a hit for the DCH
  // 

  TClonesArray &lhits = *fHits;
  IlcDCHhit *hit = new(lhits[fNhits++]) IlcDCHhit(fIshunt, track,superlayer,ring,wire,celid,trkStat,hits,ClMult);
  
  return hit;

}


//_____________________________________________________________________________
void IlcDCH::BuildGeometry()
{
  //
  // Create the ROOT TNode geometry for the DCH

if (fVersion ==1)
{
/*   TNode *node, *top;
  TPGON *pgon;
  Float_t rmin, rmax;
  Float_t zmax1, zmax2;
  Char_t  cTagV[5]; 
  
  Char_t trap[7];
  Char_t box[7];
  Int_t   iPlan;
  Float_t xpos, ypos, zpos; 
  Double_t   dx, dy, dx1,dy1;
  const Int_t kColorDCH = 50;
  const Int_t kColorDCH1 = 50;

  // Find the top node ilc
  top = gIlc->GetGeometry()->GetNode("ilc");
  if      (fDisplayType == 0) { 
          rmin = fDCHGeometry->Rmin();
          rmax = fDCHGeometry->Rmax();
          const Float_t krad2deg = 180./TMath::Pi();
          const Float_t kdeg2rad = 1./krad2deg;
          Float_t r      = (rmax +rmin)/2.;
          Int_t isec= fDCHGeometry->Nsect()/2;  
          Int_t idet=fDCHGeometry-> Ncham(); 
          Int_t   iplan=fDCHGeometry->Nplan();
//  //BIG BOX and TRAPEZOIDE 
//          for (Int_t j=0;j<idet;j++){
//           for (Int_t i=0; i<isec; i++) {
//             Float_t phi = i*360/isec; 
//             Float_t phi2 =270 +phi; 
//             if (phi2 >= 360.) phi2-=360.; 
// 	    dx =  TMath::Sin(phi*kdeg2rad)*(r); 
//             dy = -TMath::Cos(phi*kdeg2rad)*(r); 
//             Float_t phi1=phi+30; 
//             dx1 =  TMath::Sin(phi1*kdeg2rad)*(r+5.8); 
//             dy1 = -TMath::Cos(phi1*kdeg2rad)*(r+5.8); 
//             new   sprintf(box,"S_TR2%d",i);
//             sprintf(trap,"S_TR1%d",i);
//             TBRIK("Box1","DCH1","void",fDCHGeometry->GetChamberWidth(0,1)/2,fDCHGeometry->Cheight(1)/2,fDCHGeometry->Blen()/2);
//             new TRotMatrix(box,"box_mat",90.,phi1,90.,90.+phi1,0.,0.);
//             node = new TNode("DCH1","DCH1","Box1",dx1,dy1,dz,box,"");
//             node->SetLineColor(kColorDCH);
//             fNodes->Add(node);
//             new TTRD1("BTR2","DCH","void",fDCHGeometry->GetChamberWidth(0)/2,(fDCHGeometry->GetChamberWidth(0)+fDCHGeometry->Cwidcha())/2,      fDCHGeometry->TRlen()/2,fDCHGeometry->Cheight()/2);
//            new TRotMatrix(trap,"title_mat",90.,phi,0.,0,90.,phi2);
//            node = new TNode("DCH","DCH","BTR2",dx,dy,dz,trap,"");
//            node->SetLineColor(kColorDCH);
//            fNodes->Add(node);
//                  } 
//             }
	  Float_t r1;
           for (Int_t j=0;j<idet;j++){
            for (Int_t i=0; i<isec; i++) {
            Double_t   dz=(j-1)*400;
            sprintf(box,"S_TR2%d",i);
            sprintf(trap,"S_TR1%d",i);
            Float_t phi = i*360/isec; 
            Float_t phi2 =270+phi; 
            if (phi2 >= 360.) phi2-=360.; 
            Float_t phi1=phi+30;  
              for(Int_t ip=0; ip<iplan;ip++) {
		r=(rmax+(ip-3)*fDCHGeometry->Cheight()+rmin+(ip)*fDCHGeometry->Cheight()+fDCHGeometry->Cspace()/2)/2;
                r1=(rmax+(ip-3)*fDCHGeometry->Cheight(1)+rmin+(ip)*fDCHGeometry->Cheight(1)+fDCHGeometry->Cspace(1)/2)/2;
	        if(ip%2==0){
r=(rmax+(ip-3)*fDCHGeometry->Cheight()+rmin+(ip)*fDCHGeometry->Cheight()-fDCHGeometry->Cspace()/2)/2;
 r1=(rmax+(ip-3)*fDCHGeometry->Cheight(1)+rmin+ip*fDCHGeometry->Cheight(1)-fDCHGeometry->Cspace(1)/2)/2;}
		 

		//	dx =  TMath::Sin(phi*kdeg2rad)*(rmin*TMath::Cos(fDCHGeometry->GetAlpha1())/TMath::Cos(fDCHGeometry->GetAlpha2())+r*TMath::Cos(30.*kdeg2rad)); 
		//	dy = -TMath::Cos(phi*kdeg2rad)*(rmin*TMath::Cos(fDCHGeometry->GetAlpha1())/TMath::Cos(fDCHGeometry->GetAlpha2())+r*TMath::Cos(30.*kdeg2rad)); 
	        dx =  TMath::Sin(phi*kdeg2rad)*(r-6); 
	        dy = -TMath::Cos(phi*kdeg2rad)*(r-6); 
               dy1 = -TMath::Cos(phi1*kdeg2rad)*(r1+1); 
               dx1 =  TMath::Sin(phi1*kdeg2rad)*(r1+1); 
               sprintf(cTagV,"CTR%01d",ip);
               if(ip%2==0) new TTRD1(cTagV,"DCH","void",fDCHGeometry->GetChamberWidth(ip)/2,fDCHGeometry->GetChamberWidth(ip)/2+fDCHGeometry->Cwidcha()/fDCHGeometry->Cheight()*fDCHGeometry->CTheight()/2, fDCHGeometry->TRlen()/2,fDCHGeometry->CTheight()/2);
	       else new TTRD1(cTagV,"DCH","void",fDCHGeometry->GetChamberWidth(ip)/2+fDCHGeometry->Cwidcha()/fDCHGeometry->Cheight()*fDCHGeometry->Cspace()/4,fDCHGeometry->GetChamberWidth(ip)/2+fDCHGeometry->Cwidcha()/2, fDCHGeometry->TRlen()/2,fDCHGeometry->CTheight()/2);
               xpos=dx;
               zpos=dz;
               ypos=dy; 
               new TRotMatrix(trap,"title_mat",90.,phi,0.,0,90.,phi2);
               node = new TNode("DCH","DCH",cTagV,xpos,ypos,dz,trap,"");
               node->SetLineColor(kColorDCH1);
               fNodes->Add(node);
               sprintf(cTagV,"CBx%01d",ip);
               new TBRIK(cTagV,"DCH1","void",fDCHGeometry->GetChamberWidth(ip,1)/2,fDCHGeometry->CTheight(1)/2,fDCHGeometry->TRlen()/2);
               xpos= dx1;
               ypos= dy1;
               new TRotMatrix(box,"box_mat",90.,phi1,90.,90.+phi1,0.,0.);
               node = new TNode("DCH1","DCH1",cTagV,dx1,dy1,dz,box,"");
               node->SetLineColor(kColorDCH1);
               fNodes->Add(node);
            }
       }
     }
  }
    else if (fDisplayType == 1) {

     Char_t name[7];

    Float_t slope = (fDCHGeometry->Zmax1() - fDCHGeometry->Zmax2())
                  / (fDCHGeometry->Rmax()  - fDCHGeometry->Rmin());

    rmin  = fDCHGeometry->Rmin() + fDCHGeometry->CraHght();
    rmax  = rmin                   + fDCHGeometry->CdrHght();

    Float_t thickness = rmin - fDCHGeometry->Rmin();
    zmax2 = fDCHGeometry->Zmax2() + slope * thickness;
    zmax1 = zmax2 + slope * fDCHGeometry->DrThick();

    for (iPlan = 0; iPlan < IlcDCHgeometry::Nplan(); iPlan++) {

      sprintf(name,"S_TR1%d",iPlan);
      pgon  = new TPGON(name,"DCH","void",0,360,IlcDCHgeometry::Nsect(),4);
      pgon->DefineSection(0,-zmax1,rmax,rmax);
      pgon->DefineSection(1,-zmax2,rmin,rmax);
      pgon->DefineSection(2, zmax2,rmin,rmax);
      pgon->DefineSection(3, zmax1,rmax,rmax);
      top->cd();
      node = new TNode("DCH","DCH",name,0,0,0,"");
      node->SetLineColor(kColorDCH);
      fNodes->Add(node);

      Float_t height = fDCHGeometry->Cheight() + fDCHGeometry->Cspace(); 
      rmin  = rmin  + height;
      rmax  = rmax  + height;
      zmax1 = zmax1 + slope * height;
      zmax2 = zmax2 + slope * height;

    }

    thickness += fDCHGeometry->DrThick();
    rmin  = fDCHGeometry->Rmin() + thickness;
    rmax  = rmin + fDCHGeometry->AmThick();
    zmax2 = fDCHGeometry->Zmax2() + slope * thickness;
    zmax1 = zmax2 + slope * fDCHGeometry->AmThick();

    for (iPlan = 0; iPlan < fDCHGeometry->Nplan(); iPlan++) {

      sprintf(name,"S_TR2%d",iPlan);
      pgon  = new TPGON(name,"DCH","void",0,360,fDCHGeometry->Nsect(),4);
      pgon->DefineSection(0,-zmax1,rmax,rmax);
      pgon->DefineSection(1,-zmax2,rmin,rmax);
      pgon->DefineSection(2, zmax2,rmin,rmax);
      pgon->DefineSection(3, zmax1,rmax,rmax);
      top->cd();
      node = new TNode("DCH","DCH",name,0,0,0,"");
      node->SetLineColor(kColorDCH);
      fNodes->Add(node);

      Float_t height = fDCHGeometry->Cheight() + fDCHGeometry->Cspace(); 
      rmin  = rmin  + height;
      rmax  = rmax  + height;
      zmax1 = zmax1 + slope * height;
      zmax2 = zmax2 + slope * height;

    }

  }

 */
}
if (fVersion==2)
{
// // // //   TNode *top,*node_0, *node_1,*node_2;
// // // //   top = gIlc->GetGeometry()->GetNode("ilc");
// // // // 
// // // //   new TTUBE("Chtube_name","Chtube_title","void",fDCHParam->GetInnerRadius(),fDCHParam->GetOuterRadius(),fDCHParam->GetLength());//19.,150.,150.
// // // //   top->cd();
// // // //   node_0=new TNode("Chtube_node_name","Chtube_node_title","Chtube_name",0.,0.,0.);
// // // //   node_0->SetLineColor(3);
// // // //   fNodes->Add(node_0);
// // // //   if (fDCHParam->GetEndCapType()==1) {
// // // //     Float_t outRadius = fDCHParam->GetMaxEndCapDim();
// // // //     Float_t innRadius = outRadius-fDCHParam->GetEndCapWallThickness();
// // // //     Float_t theta = TMath::ACos(fDCHParam->GetLength()/outRadius)*180./TMath::Pi(); //fDCHParam->GetEndCapWallThetaIn();
// // // //     cout<< "angle "<<theta<<endl;
// // // //     new TSPHE("EndCap_SPHE_right","EndCap_SPHE_title","void",innRadius,outRadius,0.,theta,0.,360.);//210.,212.,45.,0.,360.
// // // //     top->cd();
// // // //     node_1=new TNode("EndCap_SPHE_node_righ","EndCap_SPHE_title_node_right","EndCap_SPHE_right",0.,0.,0.);
// // // //     node_1->SetLineColor(3);
// // // //     fNodes->Add(node_1);
// // // //     new TSPHE("EndCap_SPHE_left","EndCap_SPHE_title1","void",-1.*innRadius,-1.*outRadius,0.,theta,0.,360.);//-210.,-212.,0.,45.,0.,360.
// // // //     node_2=new TNode("EndCap_SPHE_nodet_left","EndCap_SPHE_title_node_left","EndCap_SPHE_left",0.,0.,0.);
// // // //     node_2->SetLineColor(3);
// // // //     fNodes->Add(node_2) ;
// // // //   }

}
}

//_____________________________________________________________________________
TList* IlcDCH::BuildGeometry2()
{
  //
  // Create the ROOT TNode geometry for the DCH

  if (fVersion ==1)
  {
 
  }
  if (fVersion==2)
  {

    TList *Nodes = new TList();       //List of geometry nodes


    new TMaterial("void","Vacuum",0,0,0);  //Everything is void
    TBRIK *brik = new TBRIK("S_ilc","ilc volume","void",2000,2000,3000);
    brik->SetVisibility(0);
    TNode *top = new TNode("ilc","ilc","S_ilc");


    new TTUBE("Chtube_name","Chtube_title","void",fDCHParam->GetInnerRadius(),fDCHParam->GetOuterRadius(),fDCHParam->GetLength());//19.,150.,150.
    top->cd();
    TNode* node_0=new TNode("Chtube_node_name","Chtube_node_title","Chtube_name",0.,0.,0.);
    node_0->SetLineColor(3);
    Nodes->Add(node_0);
    if (fDCHParam->GetEndCapType()==1) {
      Float_t outRadius = fDCHParam->GetMaxEndCapDim();
      Float_t innRadius = outRadius-fDCHParam->GetEndCapWallThickness();
      Float_t theta = TMath::ACos(fDCHParam->GetLength()/outRadius)*180./TMath::Pi(); //fDCHParam->GetEndCapWallThetaIn();
      cout<< "angle "<<theta<<endl;
      new TSPHE("EndCap_SPHE_right","EndCap_SPHE_title","void",innRadius,outRadius,0.,theta,0.,360.);//210.,212.,45.,0.,360.
      top->cd();
      TNode* node_1=new TNode("EndCap_SPHE_node_righ","EndCap_SPHE_title_node_right","EndCap_SPHE_right",0.,0.,0.);
      node_1->SetLineColor(3);
      Nodes->Add(node_1);
      new TSPHE("EndCap_SPHE_left","EndCap_SPHE_title1","void",-1.*innRadius,-1.*outRadius,0.,theta,0.,360.);//-210.,-212.,0.,45.,0.,360.
      TNode* node_2=new TNode("EndCap_SPHE_nodet_left","EndCap_SPHE_title_node_left","EndCap_SPHE_left",0.,0.,0.);
      node_2->SetLineColor(3);
      Nodes->Add(node_2) ;
    }

    return Nodes;

  }
}

//_____________________________________________________________________________
void IlcDCH::Copy(TObject &dch) const
{
  //
  // Copy function
  //

  ((IlcDCH &) dch).fDCHGeometry = fDCHGeometry;       
  ((IlcDCH &) dch).fGasDensity  = fGasDensity;
  ((IlcDCH &) dch).fFoilDensity = fFoilDensity;
  ((IlcDCH &) dch).fDrawTR      = fDrawTR;
  ((IlcDCH &) dch).fDisplayType = fDisplayType;

  //IlcDetector::Copy(dch);

}

//_____________________________________________________________________________
void IlcDCH::CreateGeometry()
{
  //
  // Creates the volumes for the DCH chambers
  //

//   if (fVersion==1)
//   fDCHGeometry->CreateGeometry(fIdtmed->GetArray());
  if (fVersion==2) {
    fDCHGeometry->SetParam(fDCHParam);
    fDCHGeometry->CreateGeometry();
    fDCHwire=new IlcDCHwireposition();
 }


}
 
//_____________________________________________________________________________
void IlcDCH::CreateMaterials()
{
  //
  // Create the materials for the DCH
  //
if (fVersion==1)
{
  Int_t isxfld = ((IlcMagF*)TGeoGlobalMagField::Instance()->GetField())->Integ();     // Field type
  Float_t sxmgmx = ((IlcMagF*)TGeoGlobalMagField::Instance()->GetField())->Max();
  
  // For polyethilene (CH2) 
  Float_t ape[2] = { 12.011 ,  1.0079 };
  Float_t zpe[2] = {  6.0   ,  1.0    };
  Float_t wpe[2] = {  1.0   ,  2.0    };
  Float_t dpe    = 0.95;

  // For mylar (C5H4O2) 
  Float_t amy[3] = { 12.011 ,  1.0079, 15.9994 };
  Float_t zmy[3] = {  6.0   ,  1.0   ,  8.0    };
  Float_t wmy[3] = {  5.0   ,  4.0   ,  2.0    };
  Float_t dmy    = 1.39;

  // For CO2 
  Float_t aco[2] = { 12.011 , 15.9994 };
  Float_t zco[2] = {  6.0   ,  8.0    };
  Float_t wco[2] = {  1.0   ,  2.0    };
  Float_t dco    = 0.00186;

  // For water
  Float_t awa[2] = {  1.0079, 15.9994 };
  Float_t zwa[2] = {  1.0   ,  8.0    };
  Float_t wwa[2] = {  2.0   ,  1.0    };
  Float_t dwa    = 1.0;

  // For isobutane (C4H10)
  Float_t ais[2] = { 12.011 ,  1.0079 };
  Float_t zis[2] = {  6.0   ,  1.0    };
  Float_t wis[2] = {  4.0   , 10.0    };
  Float_t dis    = 0.00267;

  // For plexiglas (C5H8O2)
  Float_t apg[3] = { 12.011 ,  1.0079, 15.9994 };
  Float_t zpg[3] = {  6.0   ,  1.0   ,  8.0    };
  Float_t wpg[3] = {  5.0   ,  8.0   ,  2.0    };
  Float_t dpg    = 1.18; 
  
  // For epoxy (C18H19O3)
  Float_t aEpoxy[3] = { 15.9994,  1.0079, 12.011  }; 
  Float_t zEpoxy[3] = {  8.0   ,  1.0   ,  6.0    }; 
  Float_t wEpoxy[3] = {  3.0   , 19.0   , 18.0    }; 
  Float_t dEpoxy    = 1.8 ; 

  // For Cu/Be mixture
  Float_t aCuBe[2] = {  63.54 , 9.010,};
  Float_t zCuBe[2] = {  29.0 ,  4.0  };
  Float_t wCuBe[2] = {  0.82   ,   0.18};

  // For air  
  Float_t aAir[4]   = { 12.011   , 14.0     , 15.9994  , 36.0      };
  Float_t zAir[4]   = {  6.0     ,  7.0     ,  8.0     , 18.0      };
  Float_t wAir[4]   = {  0.000124,  0.755267,  0.231781,  0.012827 };
  Float_t dAir      = 1.20479E-3;

  // For G10
  Float_t aG10[4]   = {  1.0079  , 12.011   , 15.9994  , 28.086    };
  Float_t zG10[4]   = {  1.0     ,  6.0     ,  8.0     , 14.0      };
  Float_t wG10[4]   = {  0.15201 ,  0.10641 ,  0.49444 ,  0.24714  };
  Float_t dG10      = 1.7;

  // For Xe/CO2-gas-mixture 
  Float_t aXeCO2[3] = { 131.29   ,  12.0107 ,  15.9994  };
  Float_t zXeCO2[3] = {  54.0    ,   6.0    ,   8.0     };
  Float_t wXeCO2[3] = {   0.85   ,   0.0375 ,   0.1125  };


  // For He/C4H10-gas-mixture 
  Float_t aHeC4H10[3] = { 4.002602  ,  12.0107 ,   1.0079 };
  Float_t zHeC4H10[3] = {  2.0    ,   6.0    ,   1.0    };
  Float_t wHeC4H10[3] = {  0.9   ,   0.0827,   0.0173 };


  // Xe-content of the Xe/CO2-mixture (85% / 15%) 
  Float_t fxc       = 0.85;
  Float_t dxe       = 0.00549;
  Float_t dgm       = fxc * dxe + (1.0 - fxc) * dco;

 // He-content of the He/C4H10-mixture (90% / 10%) 
  Float_t fhc       = 0.90;
  Float_t dHe       = 0.0001786;
  Float_t dgmh       = fhc * dHe + (1.0 - fhc) * dis;

// Cu-content of the Cu/Be-mixture (82% / 18%)
  Float_t fcuc       = 0.82;
  Float_t dCu      = 8.96 ;
  Float_t dBe      =1.848;
  Float_t dmCuBe      = fcuc * dCu + (1.0 - fcuc) * dBe;
  
  // General tracking parameter
  Float_t tmaxfd = -10.;
  Float_t stemax = -1e10;
  Float_t deemax = -0.1;
  Float_t epsil  =  1e-4;
  Float_t stmin  = -0.001;
  
  //////////////////////////////////////////////////////////////////////////
  //     Define Materials 
  //////////////////////////////////////////////////////////////////////////

  IlcMaterial( 1, "Al"   ,  26.98, 13.0, 2.7     ,     8.9 ,    37.2);
  IlcMaterial( 18, "Xe"   , 131.29, 54.0, dxe     ,  1546.16,     0.0);
  IlcMaterial( 4, "He"   , 4.002602,  2,  dHe     ,    755,     0.0);
  IlcMaterial( 5, "Cu"   ,  63.54, 29.0, 8.96    ,     1.43,    14.8);
  IlcMaterial( 6, "C"    ,  12.01,  6.0, 2.265   ,    18.8 ,    74.4);
  IlcMaterial(15, "Sn"   , 118.71, 50.0, 7.31    ,     1.21,    14.8);
  IlcMaterial(19, "Si"   ,  28.09, 14.0, 2.33    ,     9.36,    37.2);
  IlcMaterial(21, "W"   ,  183.84, 74.0, 19.25    ,     0.350,    14.8);
  IlcMaterial(20, "Be"   ,  9.010, 4.0, 1.848    ,     35,    14.8);


  // Mixtures 
  IlcMixture(2, "Air"         , aAir,   zAir,   dAir,    4, wAir  );
  IlcMixture(3, "Polyethilene", ape,    zpe,    dpe,    -2, wpe   );
  IlcMixture(7, "Mylar",        amy,    zmy,    dmy,    -3, wmy   );
  IlcMixture(8, "CO2",          aco,    zco,    dco,    -2, wco   );
  IlcMixture(9, "Isobutane",    ais,    zis,    dis,    -2, wis   );
  IlcMixture(16, "Cu-Be",      aCuBe,  zCuBe, dmCuBe,2,     wCuBe);
  IlcMixture(18,"Gas mixture",  aXeCO2, zXeCO2, dgm,     3, wXeCO2);
  IlcMixture(10,"Gas mixture",  aHeC4H10, zHeC4H10, dgmh,     3, wHeC4H10);

  IlcMixture(12,"G10",          aG10,   zG10,   dG10,    4, wG10  );
  IlcMixture(13,"Water",        awa,    zwa,    dwa,    -2, wwa   );
  IlcMixture(14,"Plexiglas",    apg,    zpg,    dpg,    -3, wpg   );
  IlcMixture(17,"Epoxy",        aEpoxy, zEpoxy, dEpoxy, -3, wEpoxy);

  //////////////////////////////////////////////////////////////////////////
  //     Tracking Media Parameters 
  //////////////////////////////////////////////////////////////////////////

  // Al Frame 
  IlcMedium(1, "Al Frame",   1, 0, isxfld, sxmgmx
                , tmaxfd, stemax, deemax, epsil, stmin);
  // Air 
  IlcMedium(2, "Air",        2, 0, isxfld, sxmgmx
                , tmaxfd, stemax, deemax, epsil, stmin);
  // Polyethilene 
  IlcMedium(3, "Radiator",   3, 0, isxfld, sxmgmx
                , tmaxfd, stemax, deemax, epsil, stmin);
  // Xe 
  IlcMedium(26, "Xe",         18, 1, isxfld, sxmgmx
                , tmaxfd, stemax, deemax, epsil, stmin);
 IlcMedium(4, "He",         4, 1, isxfld, sxmgmx
                , tmaxfd, stemax, deemax, epsil, stmin);

  // Cu pads 
  IlcMedium(5, "Padplane",   5, 1, isxfld, sxmgmx
                , tmaxfd, stemax, deemax, epsil, stmin);
  // Fee + cables 
  IlcMedium(6, "Readout",    1, 0, isxfld, sxmgmx
                , tmaxfd, stemax, deemax, epsil, stmin);
  // C frame 
  IlcMedium(7, "C Frame",    6, 0, isxfld, sxmgmx
                , tmaxfd, stemax, deemax, epsil, stmin);
  // Mylar foils 
  IlcMedium(8, "Mylar",      7, 0, isxfld, sxmgmx
                , tmaxfd, stemax, deemax, epsil, stmin);
  // Gas-mixture (Xe/CO2) 
  IlcMedium(9, "Gas-mix",   10, 1, isxfld, sxmgmx
                , tmaxfd, stemax, deemax, epsil, stmin);
  // Nomex-honeycomb (use carbon for the time being) 
  IlcMedium(10, "Nomex",      6, 0, isxfld, sxmgmx
                , tmaxfd, stemax, deemax, epsil, stmin);
  // Kapton foils (use Mylar for the time being) 
  IlcMedium(11, "Kapton",     7, 0, isxfld, sxmgmx
                , tmaxfd, stemax, deemax, epsil, stmin);
  // Gas-filling of the radiator 
  IlcMedium(12, "CO2",        8, 0, isxfld, sxmgmx
                , tmaxfd, stemax, deemax, epsil, stmin);
  // G10-plates
  IlcMedium(13, "G10-plates",12, 0, isxfld, sxmgmx
                , tmaxfd, stemax, deemax, epsil, stmin);
  // Cooling water
  IlcMedium(14, "Water",     13, 0, isxfld, sxmgmx
                , tmaxfd, stemax, deemax, epsil, stmin);
  // Rohacell (plexiglas) for the radiator
  IlcMedium(15, "Rohacell",  14, 0, isxfld, sxmgmx
                , tmaxfd, stemax, deemax, epsil, stmin);
  // Al layer in MCMs
  IlcMedium(16, "MCM-Al"  ,   1, 0, isxfld, sxmgmx
                , tmaxfd, stemax, deemax, epsil, stmin);
  // Sn layer in MCMs
  IlcMedium(17, "MCM-Sn"  ,  15, 0, isxfld, sxmgmx
                , tmaxfd, stemax, deemax, epsil, stmin);
  // Cu layer in MCMs
  IlcMedium(18, "MCM-Cu"  ,   5, 0, isxfld, sxmgmx
                , tmaxfd, stemax, deemax, epsil, stmin);
  // G10 layer in MCMs
  IlcMedium(19, "MCM-G10" ,  12, 0, isxfld, sxmgmx
                , tmaxfd, stemax, deemax, epsil, stmin);
  // Si in readout chips
  IlcMedium(20, "Chip-Si" ,  16, 0, isxfld, sxmgmx
                , tmaxfd, stemax, deemax, epsil, stmin);
  // Epoxy in readout chips
  IlcMedium(21, "Chip-Ep" ,  17, 0, isxfld, sxmgmx
                , tmaxfd, stemax, deemax, epsil, stmin);
  // PE in connectors
  IlcMedium(22, "Conn-PE" ,   3, 0, isxfld, sxmgmx
                , tmaxfd, stemax, deemax, epsil, stmin);
  // Cu in connectors
  IlcMedium(23, "Chip-Cu" ,   5, 0, isxfld, sxmgmx
                , tmaxfd, stemax, deemax, epsil, stmin);
  // Al of cooling pipes
  IlcMedium(24, "Cooling" ,   1, 0, isxfld, sxmgmx
                , tmaxfd, stemax, deemax, epsil, stmin);
  // Cu in services
  IlcMedium(25, "Serv-Cu" ,   5, 0, isxfld, sxmgmx
                , tmaxfd, stemax, deemax, epsil, stmin);

  // Save the density values for the DCH absorbtion
  fFoilDensity = dmy;
  fGasDensity  = dgm;
}
if (fVersion == 2)
{
  Int_t isxfld = ((IlcMagF*)TGeoGlobalMagField::Instance()->GetField())->Integ();     // Field type
  Float_t sxmgmx = ((IlcMagF*)TGeoGlobalMagField::Instance()->GetField())->Max();

  // For polyethilene (CH2) 
  Float_t ape[2] = { 12.011 ,  1.0079 };
  Float_t zpe[2] = {  6.0   ,  1.0    };
  Float_t wpe[2] = {  1.0   ,  2.0    };
  Float_t dpe    = 0.95;

  // For He/C4H10-gas-mixture 
  Float_t aHeC4H10[3] = { 4.002602  ,  12.0107 ,   1.0079 };
  Float_t zHeC4H10[3] = {  2.0    ,   6.0    ,   1.0    };
  Float_t wHeC4H10[3] = {  0.9   ,   0.0827,   0.0173 };




  Float_t dis    = 0.00267;

 // He-content of the He/C4H10-mixture (90% / 10%) 
  Float_t fhc       = 0.90;
  Float_t dHe       = 0.0001786;
  Float_t dgmh       = fhc * dHe + (1.0 - fhc) * dis;

  Float_t a[3],z[3],w[3];

 //For Carbon Fiber
  a[0] = 1.007940;   z[0] = 1.000000;   w[0] = 0.233333;  // H
  a[1] = 12.010700;   z[1] = 6.000000;   w[1] = 0.729545;  // C
  a[2] = 15.999400;   z[2] = 8.000000;   w[2] = 0.037121;  // O

// For Kapton
  Int_t nelkp	  = 4;
  Float_t densitykp = 1.430000;

  Float_t akp[4] = { 1.007940 , 12.010700 , 15.999400 , 14.006700 };
  Float_t zkp[4] = { 1.000000 , 6.000000 , 8.000000 , 7.000000 };
  Float_t wkp[4] = { 0.026363 , 0.691127 , 0.209238 , 0.073271 };  // H-C-O-N

  // For air  
  Float_t aAir[4]   = { 12.011   , 14.0     , 15.9994  , 36.0      };
  Float_t zAir[4]   = {  6.0     ,  7.0     ,  8.0     , 18.0      };
  Float_t wAir[4]   = {  0.000124,  0.755267,  0.231781,  0.012827 };
  Float_t dAir      = 1.20479E-3;

  // For Polypropylene (CH3) 
  Float_t app[2] = { 12.011 ,  1.0079 };
  Float_t zpp[2] = {  6.0   ,  1.0    };
  Float_t wpp[2] = {  1.0   ,  3.0    };
  Float_t dpp    = 0.04;

  // AIR

  Float_t dAir1 = 1.20479E-10;

//  IlcMixture(1,"CarbonFiber",a,z,1.500000,3,w);
  IlcMaterial( 1, "CarbonFiber"    ,  12.01,  6.0, 2.265   ,    18.8 ,    74.4);
  IlcMixture(2,"Gas mixture",  aHeC4H10, zHeC4H10, dgmh,     3, wHeC4H10);
  IlcMaterial(3, "Tungsten ", 183.85, 74., 19.3, .35, 10.3);
  IlcMaterial(4, "Al"   ,  26.98, 13.0, 2.7     ,     8.9 ,    37.2);

  IlcMixture(5, "Polyethilene", ape,    zpe,    dpe,    -2, wpe   );
  IlcMixture(6, "Kapton" , akp, zkp, densitykp, nelkp, wkp);
  IlcMixture(7, "Air"         , aAir,   zAir,   dAir,    4, wAir  );
  IlcMixture(8, "Polypropylene", app,    zpp,    dpp,    -2, wpp   );

  IlcMixture(9,"Vacuum",aAir,zAir,dAir1,4,wAir);

// General tracking parameter
  Float_t tmaxfd = -10.;
  Float_t stemax = -1e10;
  Float_t deemax = -0.1;
  Float_t epsil  =  1e-4;
  Float_t stmin  = -0.001;


  IlcMedium(1,"CarbonFiber", 1,0, isxfld, sxmgmx, tmaxfd, stemax, deemax, epsil, stmin);
  IlcMedium(2, "Gas-mix",2, 1, isxfld, sxmgmx, tmaxfd, stemax, deemax, epsil, stmin);
  IlcMedium(3, "Tungsten",3, 0, isxfld, sxmgmx, tmaxfd, stemax, deemax, epsil, stmin);
  IlcMedium(4, "Al",4, 0, isxfld, sxmgmx, tmaxfd, stemax, deemax, epsil, stmin);

  // Polyethilene 
  IlcMedium(5, "Radiator", 5, 0, isxfld, sxmgmx, tmaxfd, stemax, deemax, epsil, stmin);
  // Kapton
  IlcMedium(6, "Kapton", 6, 0, isxfld, sxmgmx, tmaxfd, stemax, deemax, epsil, stmin);
  // Air 
  IlcMedium(7, "Air", 7, 0, isxfld, sxmgmx, tmaxfd, stemax, deemax, epsil, stmin);
  // Polypropylene
  IlcMedium(8, "Polypropylene", 8, 0, isxfld, sxmgmx, tmaxfd, stemax, deemax, epsil, stmin); 
  //Vacum
  IlcMedium(9, "Vacuum", 9, 0, isxfld, sxmgmx,10,1,0.1,0.1,10);

}
}

//_____________________________________________________________________________
void IlcDCH::DrawModule() const
{
// // // //   //
// // // //   // Draw a shaded view of the Transition Radiation Detector version 0
// // // //   //
// // // // 
// // // //   // Set everything unseen
// // // //   gMC->Gsatt("*"   ,"SEEN",-1);
// // // //   
// // // //   // Set ILCC mother transparent
// // // //   gMC->Gsatt("ILCC","SEEN", 0);
// // // //   
// // // //   // Set the volumes visible
// // // //   if (fDCHGeometry->IsVersion() == 0) {
// // // //     gMC->Gsatt("B071","SEEN", 0);
// // // //     gMC->Gsatt("B074","SEEN", 0);
// // // //     gMC->Gsatt("B075","SEEN", 0);
// // // //     gMC->Gsatt("B077","SEEN", 0);
// // // //     gMC->Gsatt("BTR1","SEEN", 1);
// // // //     gMC->Gsatt("BTR2","SEEN", 1);
// // // //     gMC->Gsatt("BTR3","SEEN", 0);
// // // //     gMC->Gsatt("UTR1","SEEN", 0);
// // // //     gMC->Gsatt("UTR2","SEEN", 0);
// // // //     gMC->Gsatt("UTR3","SEEN", 0);
// // // //     gMC->Gsatt("Box1","SEEN", 1);
// // // //   }
// // // //   else {
// // // //     gMC->Gsatt("B071","SEEN", 0);
// // // //     gMC->Gsatt("B074","SEEN", 0);
// // // //     gMC->Gsatt("B075","SEEN", 0);
// // // //     gMC->Gsatt("B077","SEEN", 0);
// // // //     gMC->Gsatt("BTR1","SEEN", 1);
// // // //     gMC->Gsatt("BTR2","SEEN", 1);
// // // //     gMC->Gsatt("Box1","SEEN", 1);
// // // //     gMC->Gsatt("BTR3","SEEN", 0);
// // // //     gMC->Gsatt("UTR1","SEEN", 0);
// // // //   }
// // // //   
// // // //   gMC->Gdopt("hide", "on");
// // // //   gMC->Gdopt("shad", "on");
// // // //   gMC->Gsatt("*", "fill", 7);
// // // //   gMC->SetClipBox(".");
// // // //   gMC->SetClipBox("*", 0, 2000, -2000, 2000, -2000, 2000);
// // // //   gMC->DefaultRange();
// // // //   gMC->Gdraw("ilcc", 40, 30, 0, 12, 9.4, .021, .021);
// // // //   gMC->Gdhead(1111, "Transition Radiation Detector");
// // // //   gMC->Gdman(18, 4, "MAN");

}

//_____________________________________________________________________________
Int_t IlcDCH::DistancetoPrimitive(Int_t , Int_t )
{
  //
  // Distance between the mouse and the DCH detector on the screen
  // Dummy routine
  
  return 9999;

}
 
//_____________________________________________________________________________
void IlcDCH::Init()
{
  //
  // Initialize the DCH detector after the geometry has been created
  //

  IlcDebug(1,"++++++++++++++++++++++++++++++++++++++++++++++");

  if (fDCHGeometry->IsVersion() != 1) {
    IlcError("Not a vilcd geometry");
  }

  if(!fDCHGeometry) fDCHGeometry = new IlcDCHgeometry();

  fDCHGeometry->Init();
  
// // // //   // Save the geometry
// // // //   TDirectory* saveDir = gDirectory;
// // // //   gIlc->GetRunLoader()->CdGAFile();
// // // // 
// // // //   fDCHGeometry->Write("DCHgeometry");
// // // //   saveDir->cd();

  if(!fDCHwire)
    fDCHwire=new IlcDCHwireposition();

}


//_____________________________________________________________________________
void IlcDCH::MakeBranch(Option_t* option)
{
  //
  // Create Tree branches for the DCH digits.
  //

  Int_t  buffersize = 4000;
  Char_t branchname[15];
  sprintf(branchname,"%s",GetName());

  const char *cD = strstr(option,"D");

  IlcDetector::MakeBranch(option);

  if (fDigits && fLoader->TreeD() && cD) {
    MakeBranchInTree(fLoader->TreeD(),branchname,&fDigits,buffersize,0);
  }	

}

//_____________________________________________________________________________
/* void IlcDCH::ResetDigits()
{
  //
  // Reset number of digits and the digits array for this detector
  //

  fNdigits = 0;
  if (fDigits) fDigits->Clear();

}
 */
//_____________________________________________________________________________
void IlcDCH::SetTreeAddress()
{
  //
  // Set the branch addresses for the trees.
  //

  if ( fLoader->TreeH() && (fHits == 0x0)) {
    if (fVersion==1) fHits = new TClonesArray("IlcDCHhit",405);
    if (fVersion==2) fHits = new TClonesArray("IlcDCHhit",405);
  }
  IlcDetector::SetTreeAddress();

}

//_____________________________________________________________________________
IlcDCH &IlcDCH::operator=(const IlcDCH &dch)
{
  //
  // Assignment operator
  //

  if (this != &dch) ((IlcDCH &) dch).Copy(*this);
  return *this;

} 
















































































































































































































































































































































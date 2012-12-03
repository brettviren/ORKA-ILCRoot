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

/* $Id: IlcTARGETtrackerMI.cxx,v 1.6 2009/07/01 09:31:03 cgatto Exp $ */

//-------------------------------------------------------------------------
//               Implementation of the TARGET tracker class
//    It reads IlcTARGETRecPoint clusters and creates IlcTARGETtrackMI tracks
//                   and fills with them the ESD
//          Origin: Marian Ivanov, CERN, Marian.Ivanov@cern.ch 
//     dEdx analysis by: Boris Batyunya, JINR, Boris.Batiounia@cern.ch
//     
//-------------------------------------------------------------------------

#include <TMatrixD.h>
#include <TTree.h>
#include <TTreeStream.h>
#include <TTree.h>

#include "IlcESDEvent.h"
#include "IlcV0.h"
#include "IlcHelix.h"
#include "IlcTARGETRecPoint.h"
#include "IlcTARGETgeom.h"
#include "IlcTARGETgeomSPD.h"
#include "IlcTARGETtrackerMI.h"
#include "IlcTrackPointArray.h"
#include "IlcGeomManager.h"
#include "IlcTARGETParam.h"
#include "IlcTARGET.h"
#include "IlcRun.h"

ClassImp(IlcTARGETtrackerMI)



IlcTARGETtrackerMI::IlcTARGETtrackerMI():IlcTracker(),
fI(0),
fgLayers(0),
fBestTrack(),
fTrackToFollow(),
fTrackHypothesys(),
fBestHypothesys(),
fOriginal(),
fCurrentEsdTrack(),
fPass(0),
fAfterV0(kFALSE),
fLastLayerToTrackTo(0),
fCoeficients(0),
fEsd(0),
fDebugStreamer(0){


IlcTARGET *ilcTARGET  = (IlcTARGET*)gIlc->GetModule("TARGET");
IlcTARGETgeom *gm = ilcTARGET->GetTARGETgeom2();
// if (gm->GetDetVersion() !=3 ) cout <<"in IlcTARGETtrackerMI: Minor Version: "<<gm->GetDetVersion()<<endl;

fParam=new IlcTARGETParam(ilcTARGET->GetMinorVersion());

// if (gm->GetDetVersion() == 3) fParam=new IlcTARGETParam(gm->GetDetVersion());
// else if (gm->GetDetVersion() == 2) fParam=new IlcTARGETParam(gm->GetDetVersion());
// else fParam=new IlcTARGETParam(gm->GetDetVersion());


  //Default constructor
}



IlcTARGETtrackerMI::IlcTARGETtrackerMI(const IlcTARGETgeom *geom) : IlcTracker(),
fI(kMaxLayer),
fBestTrack(),
fTrackToFollow(),
fTrackHypothesys(),
fBestHypothesys(),
fOriginal(),
fCurrentEsdTrack(),
fPass(0),
fAfterV0(kFALSE),
fLastLayerToTrackTo(kLastLayerToTrackTo),
fCoeficients(0),
fEsd(0),
fDebugStreamer(0){
  //--------------------------------------------------------------------
  //This is the IlcTARGETtrackerMI constructor
  //--------------------------------------------------------------------
  fCoeficients = 0;
  fAfterV0     = kFALSE;
  IlcTARGET *ilcTARGET  = (IlcTARGET*)gIlc->GetModule("TARGET");
  IlcTARGETgeom *g=(IlcTARGETgeom*)geom;
//   if (g->GetDetVersion() !=3 ) cout <<"in IlcTARGETtrackerMI: Minor Version: "<<g->GetDetVersion()<<endl;
  
  fParam=new IlcTARGETParam(ilcTARGET->GetMinorVersion());

//   if (g->GetDetVersion() == 3) fParam=new IlcTARGETParam(g->GetDetVersion());
//   else if (g->GetDetVersion() == 2) fParam=new IlcTARGETParam(g->GetDetVersion());
//   else fParam=new IlcTARGETParam(g->GetDetVersion());
  
  for (Int_t i=0;i<fParam->GetNlayersStrip()+fParam->GetNendcapsStrip();i++) {
    int lay=i+fParam->GetNlayers()+fParam->GetNendcaps();
    IlcTARGETgeomSPD * geomspd=dynamic_cast<IlcTARGETgeomSPD*> (g->GetShape2(lay));
    if(geomspd){
      cout<<"fast fix "<<lay<<" sigmaz "<<50.e-4/sqrt(12.)<<endl;
      fParam->SetSigmaZ(lay,50.e-4/sqrt(12.));
    }
  }
  
//   double width[50]={0.96,1.38,1.38,1.38,1.38};
  Float_t x,y,z;
  Float_t x2,y2,z2;
  double overlap;
  Int_t i;

  int nfakelayers=g->GetNFakeLayers();

  cout<<"Number of layers "<<fParam->GetNlayers()<<" endcaps "<<fParam->GetNendcaps()<<
    " layers strip "<<fParam->GetNlayersStrip()<<" endcaps strip "<<fParam->GetNendcapsStrip()<<
    " fake layers "<<nfakelayers<<endl;
  fnLayers=fParam->GetNlayers()+fParam->GetNendcaps()+fParam->GetNlayersStrip()+fParam->GetNendcapsStrip();
  if(kMaxLayer<fnLayers)
    IlcError(Form("Number of layers=%i in parameters bigger than kMaxLayer=%i",fnLayers,kMaxLayer));

  fgLayers=new IlcTARGETlayer[fnLayers+nfakelayers];
  
  int totalnumberofDetectors=0;
  for (i=1; i<=fnLayers; i++) {
    Int_t nlad=g->GetNladders(i);
    Int_t ndet=g->GetNdetectors(i);


    g->GetTrans(i,1,1,x,y,z); 
    Double_t poff=TMath::ATan2(y,x);
    Double_t zoff=z;

    double range=0;
    Double_t r=g->GetAverageRadiusOfLayer(i,range);
    IlcDebug(1,Form("Average radius forr layer %i = %f ,z = %f , number of ladders = %i, detectors=%i",i,r,z,nlad,ndet));
    IlcDebug(1,Form("for layer %i:  dz = %f , rmin = %f , rmax = %f , zmin=%f, zmax=%f", i,g->Dz(i-1),g->Rmin(i-1),g->Rmax(i-1),g->Posz1(i-1),g->Posz2(i-1)));
    
// // // //     IlcInfo(Form("Average radius forr layer %i = %f ,z = %f , number of ladders = %i, detectors=%i",i,r,z,nlad,ndet));
// // // //     IlcInfo(Form("for layer %i:  dz = %f , rmin = %f , rmax = %f , zmin=%f, zmax=%f", i,g->Dz(i-1),g->Rmin(i-1),g->Rmax(i-1),g->Posz1(i-1),g->Posz2(i-1)));
    
    overlap=0;
// // // //     if(i<5) overlap=width[i-1]*nlad/(r*2*TMath::Pi())-1;
// // // //     IlcInfo(Form("Overlap fraction between ladders forr layer %i = %f ",i,overlap));
    
// // // //     if(overlap<0||overlap>1)
// // // //       IlcWarning(Form("Not good overlap betweens detectors=%f ; layer=%i ; radius= %f ; width=%f", overlap,i,r,width[i-1]));
// // // //     g->GetTrans(i,1,1,x2,y2,z2); 
// // // //     Double_t poff2=TMath::ATan2(y2,x2);

// // // //     fgLayers[i-1].Set(r,poff,zoff,poff2>poff?nlad:-nlad,ndet,overlap,g->Rmin(i-1),g->Rmax(i-1),g->Dz(i-1));
    fgLayers[i-1].Set(r,poff,zoff,nlad,ndet,overlap,g->Rmin(i-1),g->Rmax(i-1),g->Dz(i-1));

// // // //     if((i>fParam->GetNlayers()&&i<=fParam->GetNlayers()+fParam->GetNendcaps())||
// // // //        i>fParam->GetNlayers()+fParam->GetNendcaps()+fParam->GetNlayersStrip()) fgLayers[i-1].SetEndCap();
// // // // 
// // // //     if(fgLayers[i-1].IsEndCap()){
// // // // 	fgLayers[i-1].SetZRange(g->Posz1(i-1),g->Posz2(i-1));
// // // //     }
    for (Int_t j=1; j<nlad+1; j++) {
      for (Int_t k=1; k<ndet+1; k++) { //Fill this layer with detectors
        Float_t xx,yy,zshift; g->GetTrans(i,j,k,xx,yy,zshift);
        Double_t rot[9]; g->GetRotMatrix(i,j,k,rot);

        Double_t phi=TMath::ATan2(rot[1],rot[0]);
// // // // 	phi+=TMath::Pi()/2; // FIXME
	if(phi<-TMath::Pi()/2) phi+=2*TMath::Pi();
        Double_t cp=TMath::Cos(phi), sp=TMath::Sin(phi);
        Double_t rr=xx*cp+yy*sp;
	if(rr<0){
	  phi-=TMath::Pi();
	  if(phi<-TMath::Pi()/2) phi+=2*TMath::Pi();
	  rr=-rr;
	}
	IlcDebug(50,Form("Ladder %i,phi=%f ,r=%f (position phi=%f, r=%f)",
			j,phi,rr,TMath::ATan2(yy,xx),TMath::Hypot(xx,yy)));
// 	IlcInfo(Form("Ladder %i,phi=%f ,r=%f (position phi=%f, r=%f) (cp %f sp: %f xx: %f yy: %f)",
// 			j,phi,rr,TMath::ATan2(yy,xx),TMath::Hypot(xx,yy), cp, sp, xx, yy));
	int ind=fgLayers[i-1].FindDetectorIndex(TMath::ATan2(yy,xx),zshift);
	if(((!fgLayers[i-1].IsEndCap())&&ind!=(j-1)*ndet+k-1)/*||((fgLayers[i-1].IsEndCap())&&ind!=(j-1)*ndet)*/)
	  IlcError(Form("FindDetectorIndex(phi,z) for layer %i return another index=%i, but must be %i ",i,ind,(j-1)*ndet+k-1));
// 	double rdiff=r/TMath::ATan2(y,x);if(rdiff>1.) rdiff=1.;
// 	double dphirot=TMath::ACos(rdiff)-fabs(phi-TMath::ATan2(y,x));
// 	if(fabs(dphirot)>1e-6&&fabs(dphirot+2*TMath::Pi())>1e-6)
// 	  IlcError("Different Phi angles");  
        IlcTARGETdetector &det=fgLayers[i-1].GetDetector((j-1)*ndet + k-1); 
        new(&det) IlcTARGETdetector(rr,phi); 
      } 
    }
    totalnumberofDetectors+=nlad*ndet;
  }

  IlcDebug(5,Form("total number of detectors %i ",totalnumberofDetectors));
  
  fnLayersAll=fnLayers;
      
  for(int ii=0;ii<nfakelayers;ii++){
    fgLayers[fnLayersAll].Set(0,0,0.5*(g->Posz1_Fake(ii)+g->Posz2_Fake(ii)),1,1,0,0,150,0);
    fgLayers[fnLayersAll].SetZRange(g->Posz1_Fake(ii),g->Posz2_Fake(ii));
// // // //     fgLayers[fnLayersAll].SetEndCap();
    IlcDebug(1,Form("Add fake endcap layer z1=%f, z2=%f ",g->Posz1_Fake(ii),g->Posz2_Fake(ii)));
    fnLayersAll++;
  }
  
  fI=fParam->GetNlayers()+fParam->GetNendcaps()+fParam->GetNlayersStrip();

  fPass=0;
  fConstraint[0]=1; fConstraint[1]=0;

  Double_t xyz[]={kXV,kYV,kZV}, ers[]={kSigmaXV,kSigmaYV,kSigmaZV}; 
  SetVertex(xyz,ers);

  for (Int_t ii=0; ii<fnLayers; ii++) fLayersNotToSkip[ii]=kLayersNotToSkip[ii];
  fLastLayerToTrackTo=kLastLayerToTrackTo;
  for (Int_t ii=0;ii<100000;ii++){
    fBestTrackIndex[ii]=0;
  }
  //
  fDebugStreamer = new TTreeSRedirector("TARGETdebug.root");
  
  fmaxR=0;
  fmaxZ=0;
  for(int ii=0;ii<fnLayers;ii++){
    if(fgLayers[ii].GetR()>fmaxR) fmaxR=fgLayers[ii].GetR();
    if(fabs(fgLayers[ii].GetZ())>fmaxZ) fmaxZ=fabs(fgLayers[ii].GetZ());
  }
  //litle more
  fmaxR+=1.;
  fmaxZ+=1.;
}

IlcTARGETtrackerMI::IlcTARGETtrackerMI(const IlcTARGETtrackerMI &tracker):IlcTracker(tracker),
fI(tracker.fI),
fBestTrack(tracker.fBestTrack),
fTrackToFollow(tracker.fTrackToFollow),
fTrackHypothesys(tracker.fTrackHypothesys),
fBestHypothesys(tracker.fBestHypothesys),
fOriginal(tracker.fOriginal),
fCurrentEsdTrack(tracker.fCurrentEsdTrack),
fPass(tracker.fPass),
fAfterV0(tracker.fAfterV0),
fLastLayerToTrackTo(tracker.fLastLayerToTrackTo),
fCoeficients(tracker.fCoeficients),
fEsd(tracker.fEsd),
fDebugStreamer(tracker.fDebugStreamer){
  fParam=new IlcTARGETParam(*tracker.fParam);
  fgLayers=0;
  //Copy constructor
}

IlcTARGETtrackerMI & IlcTARGETtrackerMI::operator=(const IlcTARGETtrackerMI &tracker){
  //Assignment operator
  this->~IlcTARGETtrackerMI();
  new(this) IlcTARGETtrackerMI(tracker);
  return *this;
}


IlcTARGETtrackerMI::~IlcTARGETtrackerMI()
{
  //
  //destructor
  //
  delete fParam;
  if (fCoeficients) delete []fCoeficients;
  if (fDebugStreamer) {
    //fDebugStreamer->Close();
    delete fDebugStreamer;
  }
  
  delete [] fgLayers;
}

void IlcTARGETtrackerMI::SetLayersNotToSkip(Int_t *l) {
  //--------------------------------------------------------------------
  //This function set masks of the layers which must be not skipped
  //--------------------------------------------------------------------
  for (Int_t i=0; i<fnLayers; i++) fLayersNotToSkip[i]=l[i];
}

Int_t IlcTARGETtrackerMI::LoadClusters(TTree *cTree) {
  //--------------------------------------------------------------------
  //This function loads TARGET clusters
  //--------------------------------------------------------------------
  TBranch *branch=cTree->GetBranch("TARGETRecPoints");
  if (!branch) { 
    Error("LoadClusters"," can't get the branch !\n");
    return 1;
  }

  TClonesArray dummy("IlcTARGETRecPoint",10000), *clusters=&dummy;
  branch->SetAddress(&clusters);

  Int_t j=0;
  int ncllayer;
  for (Int_t i=0; i<fnLayers; i++) {
    Int_t ndet=fgLayers[i].GetNdetectors();
    Int_t jmax = j + fgLayers[i].GetNladders()*ndet;
    ncllayer=0;
    for (; j<jmax; j++) {           
      if (!cTree->GetEvent(j)) continue;
      Int_t ncl=clusters->GetEntriesFast();
      ncllayer+=ncl;
      SignDeltas(clusters,GetZ());
      while (ncl--) {
        IlcTARGETRecPoint *c=(IlcTARGETRecPoint*)clusters->UncheckedAt(ncl);
	if(i!=c->GetLayer()) IlcError(Form("Different layers numbers I wait layer=%i but RecPoint layer=%i",i,c->GetLayer()));
        fgLayers[i].InsertCluster(new IlcTARGETRecPoint(*c));

	//IlcInfo(Form("Cluster coord: %f %f %f", c->GetDetLocalX(), c->GetPhiR(), c->GetDetLocalZ()));
// 	IlcInfo(Form("Cluster coord: lay %d mod %d index %d", c->GetLayer(), c->GetModule(), c->GetDetectorIndex()));
      }
      clusters->Delete();
      //add dead zone virtual "cluster"      
//       if (i<2){
// 	for (Float_t ydead = 0; ydead < 1.31 ; ydead+=(i+1.)*0.018){     
// 	  Int_t lab[4] = {0,0,0,detector};
// 	  Int_t info[3] = {0,0,0};
// 	  Float_t hit[5]={0,0,0.004/12.,0.001/12.,0};
// 	  if (i==0) hit[0] =ydead-0.4;
// 	  if (i==1) hit[0]=ydead-3.75; 
// 	  hit[1] =-0.04;
// 	  if (TMath::Abs(fgLayers[i].GetDetector(detector).GetZmax()-hit[1])<2.) 
// 	    fgLayers[i].InsertCluster(new IlcTARGETRecPoint(lab, hit, info));
// 	  hit[1]=-7.05;
// 	  if (TMath::Abs(fgLayers[i].GetDetector(detector).GetZmax()-hit[1])<2.) 
// 	    fgLayers[i].InsertCluster(new IlcTARGETRecPoint(lab, hit, info));
// 	  hit[1]=-7.15;
// 	  if (TMath::Abs(fgLayers[i].GetDetector(detector).GetZmax()-hit[1])<2.) 
// 	    fgLayers[i].InsertCluster(new IlcTARGETRecPoint(lab, hit, info));
// 	  hit[1] =0.06;
// 	  if (TMath::Abs(fgLayers[i].GetDetector(detector).GetZmax()-hit[1])<2.) 
// 	    fgLayers[i].InsertCluster(new IlcTARGETRecPoint(lab, hit, info));
// 	  hit[1]=7.05;
// 	  if (TMath::Abs(fgLayers[i].GetDetector(detector).GetZmax()-hit[1])<2.) 
// 	    fgLayers[i].InsertCluster(new IlcTARGETRecPoint(lab, hit, info));
// 	  hit[1]=7.25;
// 	  if (TMath::Abs(fgLayers[i].GetDetector(detector).GetZmax()-hit[1])<2.) 
// 	    fgLayers[i].InsertCluster(new IlcTARGETRecPoint(lab, hit, info));       
// 	}
//       }
      
    }
    //
    IlcInfo(Form("Load number of Clusters for layer %i = %i ",i,ncllayer));

    fgLayers[i].ResetRoad(); //road defined by the cluster density
    fgLayers[i].SortClusters();
  }

  return 0;
}

void IlcTARGETtrackerMI::UnloadClusters() {
  //--------------------------------------------------------------------
  //This function unloads TARGET clusters
  //--------------------------------------------------------------------
  for (Int_t i=0; i<fnLayers; i++) fgLayers[i].ResetClusters();
}

Int_t IlcTARGETtrackerMI::CorrectForDeadZoneMaterial(IlcTARGETtrackMI *t) {
  //--------------------------------------------------------------------
  // Correction for the material between the TPC and the TARGET
  // (should it belong to the TPC code ?)
  //--------------------------------------------------------------------
  //temporaly solution, I don't know whereis dead material
  const Double_t kMaxStep = 5;   // max step
  if(t->GetX() > fmaxR){
    if(!t->PropagateToWithCorrection(fmaxR,t->GetBz(),t->GetMass(),kMaxStep,kTRUE,kAlmost1))return 1;
  }

  return 1;


  Double_t xyz0[3],xyz1[3],param[7];
  
  double r[3]={10,20,22};

  Double_t rho,x0,distance,d,p0,p1;
  if (t->GetX() > r[2]-0.1) {
    for(int i=2;i>=0;i--){
      t->GetGlobalXYZat(t->GetX(),xyz0[0],xyz0[1],xyz0[2]);
      t->GetGlobalXYZat(r[i],xyz1[0],xyz1[1],xyz1[2]);
      IlcTracker::MeanMaterialBudget(xyz0,xyz1,param);
      rho=param[0];x0=param[1];distance=param[4];
      d=distance*rho/x0;
      /*     cout<<i<<" From "<<xyz0[0]<<","<< xyz0[1]<<","<<xyz0[2]<<
	" R "<<TMath::Sqrt(xyz0[0]*xyz0[0]+xyz0[1]*xyz0[1]+xyz0[2]*xyz0[2])<<
	" To "<<xyz1[0]<<","<< xyz1[1]<<","<<xyz1[2]<<
	" R "<<TMath::Sqrt(xyz1[0]*xyz1[0]+xyz1[1]*xyz1[1]+xyz1[2]*xyz1[2])<<
	endl<<
	" distance="<<distance<<" d="<<d<<" rho="<<rho<<" x0="<<x0<<endl;*/
      p0=TMath::Sqrt((1+t->GetTgl()*t->GetTgl())/(t->Get1Pt()*t->Get1Pt()));
      //      cout<<"P0= "<<p0<<endl;
      if (!t->PropagateTo(r[i],d,x0)) return 1;
      p1=TMath::Sqrt((1+t->GetTgl()*t->GetTgl())/(t->Get1Pt()*t->Get1Pt()));
      //cout<<"P1= "<<p1<<" dP "<<p1-p0<<endl;
    };
  } else if (t->GetX() < r[0]+0.1) {
    for(int i=0;i<3;i++){
      t->GetGlobalXYZat(t->GetX(),xyz0[0],xyz0[1],xyz0[2]);
      t->GetGlobalXYZat(r[i],xyz1[0],xyz1[1],xyz1[2]);
      IlcTracker::MeanMaterialBudget(xyz0,xyz1,param);
      rho=param[0];x0=param[1];distance=param[4];
      d=distance*rho/x0;
      /*cout<<i<<" From "<<xyz0[0]<<","<< xyz0[1]<<","<<xyz0[2]<<
	" R "<<TMath::Sqrt(xyz0[0]*xyz0[0]+xyz0[1]*xyz0[1]+xyz0[2]*xyz0[2])<<
	" To "<<xyz1[0]<<","<< xyz1[1]<<","<<xyz1[2]<<
	" R "<<TMath::Sqrt(xyz1[0]*xyz1[0]+xyz1[1]*xyz1[1]+xyz1[2]*xyz1[2])<<
	endl<<
	" distance="<<distance<<" d="<<d<<" rho="<<rho<<" x0="<<x0<<endl;*/
      p0=TMath::Sqrt((1+t->GetTgl()*t->GetTgl())/(t->Get1Pt()*t->Get1Pt()));
      //      cout<<"P0= "<<p0<<endl;
      if (!t->PropagateTo(r[i],-d,x0)) return 1;
      p1=TMath::Sqrt((1+t->GetTgl()*t->GetTgl())/(t->Get1Pt()*t->Get1Pt()));
      // cout<<"P1= "<<p1<<" dP "<<p1-p0<<endl;
    };
  } else {
  ::Error("CorrectForDeadZoneMaterial","track is already in the dead zone !");
    return 1;
  }
  
  return 0;

}

Int_t IlcTARGETtrackerMI::Clusters2Tracks(IlcESDEvent *event) {
  //--------------------------------------------------------------------
  // This functions reconstructs TARGET tracks
  // The clusters must be already loaded !
  //--------------------------------------------------------------------
  TObjArray itsTracks(15000);
  fOriginal.Clear();
  fEsd = event;         // store pointer to the esd 
  {/* Read ESD tracks */
    Int_t nentr=event->GetNumberOfTracks();
    Info("Clusters2Tracks", "Number of ESD tracks: %d\n", nentr);
    while (nentr--) {
      IlcESDtrack *esd=event->GetTrack(nentr);

      if ((esd->GetStatus()&IlcESDtrack::kTPCin)==0) continue;
      if (esd->GetStatus()&IlcESDtrack::kTPCout) continue;
      if (esd->GetStatus()&IlcESDtrack::kVXDin) continue;
      if (esd->GetKinkIndex(0)>0) continue;   //kink daughter
      IlcTARGETtrackMI *t=0;
      try {
        t=new IlcTARGETtrackMI(*esd);
      } catch (const Char_t *msg) {
        //Warning("Clusters2Tracks",msg);
        delete t;
        continue;
      }
      //t->fD[0] = t->GetD(GetX(),GetY());
      //t->fD[1] = t->GetZat(GetX())-GetZ();
      t->GetDZ(GetX(),GetY(),GetZ(),t->GetDP());              //I.B.
      Double_t vdist = TMath::Sqrt(t->GetD(0)*t->GetD(0)+t->GetD(1)*t->GetD(1));
      if (t->GetMass()<0.13) t->SetMass(0.13957); // MI look to the esd - mass hypothesys  !!!!!!!!!!!
      // write expected q
      t->SetExpQ(TMath::Max(0.8*t->GetESDtrack()->GetTPCsignal(),30.));

      if (esd->GetV0Index(0)>0 && t->GetD(0)<30){
	//track - can be  V0 according to TPC
      }
      else{	
	if (TMath::Abs(t->GetD(0))>10) {
	  delete t;
	  continue;
	}
	
	if (TMath::Abs(vdist)>20) {
	  delete t;
	  continue;
	}
	if (TMath::Abs(1/t->Get1Pt())<0.120) {
	  delete t;
	  continue;
	}
	
	if (CorrectForDeadZoneMaterial(t)!=0) {
	  //Warning("Clusters2Tracks",
	  //        "failed to correct for the material in the dead zone !\n");
// 	  delete t;
// 	  continue;
	}
      }
      t->SetReconstructed(kFALSE);
      itsTracks.AddLast(t);
      fOriginal.AddLast(t);
    }
  } /* End Read ESD tracks */
      
  itsTracks.Sort();
  fOriginal.Sort();
  Int_t nentr=itsTracks.GetEntriesFast();
  fTrackHypothesys.Expand(nentr);
  fBestHypothesys.Expand(nentr);
  MakeCoeficients(nentr);
  Int_t ntrk=0;
  for (fPass=0; fPass<2; fPass++) {
     Int_t &constraint=fConstraint[fPass]; if (constraint<0) continue;
     for (Int_t i=0; i<nentr; i++) {
       //       cout<<fPass<<"    "<<i<<'\r';
       fCurrentEsdTrack = i;
       IlcTARGETtrackMI *t=(IlcTARGETtrackMI*)itsTracks.UncheckedAt(i);
       if (t==0) continue;              //this track has been already tracked
       IlcDebug(1,Form("Follow pass=%i prolongation forr esd track=%i tpcchi2=%f tpcncls=%i",fPass,i,
		       t->GetESDtrack()->GetTPCchi2(),t->GetESDtrack()->GetTPCNcls()));
       
       if (t->GetReconstructed()&&(t->GetNUsed()<1.5)) continue;  //this track was  already  "succesfully" reconstructed
       //if ( (TMath::Abs(t->GetD(GetX(),GetY()))  >3.) && fConstraint[fPass]) continue;
       //if ( (TMath::Abs(t->GetZat(GetX())-GetZ())>3.) && fConstraint[fPass]) continue;
       Float_t dz[2]; t->GetDZ(GetX(),GetY(),GetZ(),dz);              //I.B.
       if ( (TMath::Abs(dz[0])>3.) && fConstraint[fPass]) continue;
       if ( (TMath::Abs(dz[1])>3.) && fConstraint[fPass]) continue;

       Int_t tpcLabel=t->GetLabel(); //save the TPC track label       
       fI = 13;
       ResetTrackToFollow(*t);
       ResetBestTrack();
       FollowProlongationTree(t,i,fConstraint[fPass]);
       SortTrackHypothesys(fCurrentEsdTrack,20,0);  //MI change
       //
       IlcTARGETtrackMI * besttrack = GetBestHypothesys(fCurrentEsdTrack,t,15);
       if (!besttrack) continue;
       besttrack->SetLabel(tpcLabel);
       //       besttrack->CookdEdx();
       CookdEdx(besttrack);
       besttrack->SetFakeRatio(1.);
       CookLabel(besttrack,0.); //For comparison only
       UpdateESDtrack(besttrack,IlcESDtrack::kVXDin);

       /*       
       if ( besttrack->GetNumberOfClusters()<6 && fConstraint[fPass]) {	 
	 continue;
       }
       if (besttrack->fChi2MIP[0]+besttrack->fNUsed>3.5) continue;
       if ( (TMath::Abs(besttrack->fD[0]*besttrack->fD[0]+besttrack->fD[1]*besttrack->fD[1])>0.1) && fConstraint[fPass])  continue;	 
       //delete itsTracks.RemoveAt(i);
       */
       if (fConstraint[fPass]&&(!besttrack->IsGoldPrimary())) continue;  //to be tracked also without vertex constrain 

       IlcDebug(1,Form("Add new TARGET reconstructed track %i",ntrk));

       t->SetReconstructed(kTRUE);
       ntrk++;                     
     }
     GetBestHypothesysMIP(itsTracks); 
  }

  //GetBestHypothesysMIP(itsTracks);
  UpdateTPCV0(event);
  FindV02(event);
  fAfterV0 = kTRUE;
  //GetBestHypothesysMIP(itsTracks);
  //
  itsTracks.Delete();
  //
  Int_t entries = fTrackHypothesys.GetEntriesFast();
  for (Int_t ientry=0;ientry<entries;ientry++){
    TObjArray * array =(TObjArray*)fTrackHypothesys.UncheckedAt(ientry);
    if (array) array->Delete();
    delete fTrackHypothesys.RemoveAt(ientry); 
  }

  fTrackHypothesys.Delete();
  fBestHypothesys.Delete();
  fOriginal.Clear();
  delete []fCoeficients;
  fCoeficients=0;
  Info("Clusters2Tracks","Number of prolonged tracks: %d\n",ntrk);
  
  return 0;
}


Int_t IlcTARGETtrackerMI::PropagateBack(IlcESDEvent *event) {
  //--------------------------------------------------------------------
  // This functions propagates reconstructed TARGET tracks back
  // The clusters must be loaded !
  //--------------------------------------------------------------------
  Int_t nentr=event->GetNumberOfTracks();
  Info("PropagateBack", "Number of ESD tracks: %d\n", nentr);

  Int_t ntrk=0;
  for (Int_t i=0; i<nentr; i++) {
     IlcESDtrack *esd=event->GetTrack(i);

     if ((esd->GetStatus()&IlcESDtrack::kVXDin)==0) continue;
     if (esd->GetStatus()&IlcESDtrack::kVXDout) continue;

     IlcTARGETtrackMI *t=0;
     try {
        t=new IlcTARGETtrackMI(*esd);
     } catch (const Char_t *msg) {
       //Warning("PropagateBack",msg);
        delete t;
        continue;
     }
     t->SetExpQ(TMath::Max(0.8*t->GetESDtrack()->GetTPCsignal(),30.));

     ResetTrackToFollow(*t);

     if (fTrackToFollow.PropagateTo(fgLayers[0].GetRmin(),0.,0.)) {
       if (fTrackToFollow.PropagateToVertex(event->GetVertex())) {
          fTrackToFollow.StartTimeIntegral();
       }
       fTrackToFollow.PropagateTo(fgLayers[0].GetRmin(),0.,0.);
     }

     fTrackToFollow.ResetCovariance(10.); fTrackToFollow.ResetClusters();
     
     
     if (RefitAt(-fmaxR,&fTrackToFollow,t)) {
        if (CorrectForDeadZoneMaterial(&fTrackToFollow)!=0) {
          //Warning("PropagateBack",
          //        "failed to correct for the material in the dead zone !\n");
          //delete t;
          //continue;
        }
	fTrackToFollow.SetLabel(t->GetLabel());
        //fTrackToFollow.CookdEdx();
        CookLabel(&fTrackToFollow,0.); //For comparison only
        fTrackToFollow.UpdateESDtrack(IlcESDtrack::kVXDout);
        //UseClusters(&fTrackToFollow);
        ntrk++;
     }
     delete t;
  }
  Info("PropagateBack","Number of back propagated TARGET tracks: %d\n",ntrk);

  return 0;
}

Int_t IlcTARGETtrackerMI::RefitInward(IlcESDEvent *event) {
  //--------------------------------------------------------------------
  // This functions refits TARGET tracks using the 
  // "inward propagated" TPC tracks
  // The clusters must be loaded !
  //--------------------------------------------------------------------
  RefitV02(event);
  Int_t nentr=event->GetNumberOfTracks();
  Info("RefitInward", "Number of ESD tracks: %d\n", nentr);
  if(IlcDebugLevelClass()>0){
    for (Int_t i=0; i<nentr; i++) {
      IlcESDtrack *esd=event->GetTrack(i);
      cout<<
          "status targetin:"<<(esd->GetStatus()&IlcESDtrack::kVXDin)<<
          ", targetout:"<<(esd->GetStatus()&IlcESDtrack::kVXDout)<<
          ", targetrefit:"<<(esd->GetStatus()&IlcESDtrack::kVXDrefit)<<
	", tpcout:"<<(esd->GetStatus()&IlcESDtrack::kTPCout)<<
	", tpcin:"<<(esd->GetStatus()&IlcESDtrack::kTPCin)<<
	", tpcrefit:"<<(esd->GetStatus()&IlcESDtrack::kTPCrefit)<<endl;
      IlcTARGETtrackMI *t=new IlcTARGETtrackMI(*esd);
      t->Print();
      delete t;
    };
  }

  Int_t ntrk=0;
  for (Int_t i=0; i<nentr; i++) {
    IlcESDtrack *esd=event->GetTrack(i);

    if ((esd->GetStatus()&IlcESDtrack::kVXDout) == 0) continue;
    if (esd->GetStatus()&IlcESDtrack::kVXDrefit) continue;
    if (esd->GetStatus()&IlcESDtrack::kTPCout)
      if ((esd->GetStatus()&IlcESDtrack::kTPCrefit)==0) continue;

    IlcTARGETtrackMI *t=0;
    try {
        t=new IlcTARGETtrackMI(*esd);
    } catch (const Char_t *msg) {
      //Warning("RefitInward",msg);
        delete t;
        continue;
    }
    t->SetExpQ(TMath::Max(0.8*t->GetESDtrack()->GetTPCsignal(),30.));
    if(esd->GetStatus()&IlcESDtrack::kTPCrefit||t->GetX()>19)
      if (CorrectForDeadZoneMaterial(t)!=0) {
	//Warning("RefitInward",
	//         "failed to correct for the material in the dead zone !\n");
      }

    ResetTrackToFollow(*t);
    fTrackToFollow.ResetClusters();

    if ((esd->GetStatus()&IlcESDtrack::kTPCin)==0)
      fTrackToFollow.ResetCovariance(10.);

    //Refitting...
    if (RefitAt(fgLayers[0].GetRmin(), &fTrackToFollow, t)) {
      fTrackToFollow.SetLabel(t->GetLabel());
      //fTrackToFollow.CookdEdx();
      CookdEdx(&fTrackToFollow);
      
      CookLabel(&fTrackToFollow,0.0); //For comparison only

//       if (fTrackToFollow.PropagateTo(.6,0.,0.)) {//The beam pipe   
	IlcESDtrack  *esdTrack =fTrackToFollow.GetESDtrack();
        esdTrack->UpdateTrackParams(&fTrackToFollow,IlcESDtrack::kVXDrefit);
// 	Float_t r[3]={0.,0.,0.};
// 	Double_t maxD=3.;
// 	esdTrack->RelateToVertex(event->GetVertex(),GetBz(r),maxD);
	ntrk++;
//       }
    }
    delete t;
  }

  Info("RefitInward","Number of refitted tracks: %d\n",ntrk);

  return 0;
}

IlcCluster *IlcTARGETtrackerMI::GetCluster(Int_t index) const {
  //--------------------------------------------------------------------
  //       Return pointer to a given cluster
  //--------------------------------------------------------------------
  Int_t l=(index & kClusterLayerMask) >> kClusterLayerShift;
  Int_t c=(index & kClusterCountMask) >> 00;
  return fgLayers[l].GetCluster(c);
}

Bool_t IlcTARGETtrackerMI::GetTrackPoint(Int_t index, IlcTrackPoint& p) const {
  //
  // Get track space point with index i
  //
  Int_t l=(index & kClusterLayerMask) >> kClusterLayerShift;
  Int_t c=(index & kClusterCountMask) >> 00;
  IlcTARGETRecPoint *cl = fgLayers[l].GetCluster(c);
  Int_t idet = cl->GetDetectorIndex();
  const IlcTARGETdetector &det = fgLayers[l].GetDetector(idet);
  Float_t phi = det.GetPhi();
  Float_t r = det.GetR();
  Float_t cp=TMath::Cos(phi), sp=TMath::Sin(phi);
  Float_t xyz[3];
  xyz[0] = r*cp - cl->GetY()*sp;
  xyz[1] = r*sp + cl->GetY()*cp;
  xyz[2] = cl->GetZ();
  Float_t cov[6];
  cov[0] = sp*sp*cl->GetSigmaY2();
  cov[1] = -sp*cp*cl->GetSigmaY2();
  cov[2] = 0.;
  cov[3] = cp*cp*cl->GetSigmaY2();
  cov[4] = 0.;
  cov[5] = cl->GetSigmaZ2();
  p.SetXYZ(xyz[0],xyz[1],xyz[2],cov);
  IlcGeomManager::ELayerID iLayer = IlcGeomManager::kInvalidLayer;
  switch (l) {
  case 0:
    iLayer = IlcGeomManager::kSPD1;
    break;
  case 1:
    iLayer = IlcGeomManager::kSPD2;
    break;
  case 2:
    iLayer = IlcGeomManager::kSDD1;
    break;
  case 3:
    iLayer = IlcGeomManager::kSDD2;
    break;
  case 4:
    iLayer = IlcGeomManager::kSSD1;
    break;
  case 5:
    iLayer = IlcGeomManager::kSSD2;
    break;
  default:
    IlcWarning(Form("Wrong layer index in TARGET (%d) !",l));
    break;
  };
  UShort_t volid = IlcGeomManager::LayerToVolUID(iLayer,idet);
  p.SetVolumeID((UShort_t)volid);
  return kTRUE;
}

void IlcTARGETtrackerMI::FollowProlongationTree(IlcTARGETtrackMI * otrack, Int_t esdindex, Bool_t constrain) 
{
  //--------------------------------------------------------------------
  // Follow prolongation tree
  //--------------------------------------------------------------------
  //
  IlcESDtrack * esd = otrack->GetESDtrack();
  if (esd->GetV0Index(0)>0){
    //
    // TEMPORARY SOLLUTION: map V0 indexes to point to proper track
    //                      mapping of esd track is different as its track in Containers
    //                      Need something more stable
    //                      Indexes are set back againg to the ESD track indexes in UpdateTPCV0
    for (Int_t i=0;i<3;i++){
      Int_t  index = esd->GetV0Index(i);
      if (index==0) break;
      IlcESDv0 * vertex = fEsd->GetV0(index);
      if (vertex->GetStatus()<0) continue;     // rejected V0
      //
      if (esd->GetSign()>0) {
        vertex->SetIndex(0,esdindex);
      }
      else{
        vertex->SetIndex(1,esdindex);
      }
    }
  }
  TObjArray *bestarray = (TObjArray*)fBestHypothesys.At(esdindex);
  if (!bestarray){
    bestarray = new TObjArray(kMaxLayer);
    fBestHypothesys.AddAt(bestarray,esdindex);
  }

  //
  //setup tree of the prolongations
  //
  const int kMaxHypoth=100;
// // // //   const int kMaxClusters=100;
  static IlcTARGETtrackMI tracks[kMaxClusters+1][kMaxHypoth];
  IlcTARGETtrackMI *currenttrack;
  static IlcTARGETtrackMI currenttrack1;
  static IlcTARGETtrackMI currenttrack2;  
  static IlcTARGETtrackMI backuptrack;
  Int_t ntracks[kMaxClusters+1];
  Int_t nLayers[kMaxClusters+1][kMaxHypoth];
  Int_t prevZLayers[kMaxClusters+1][kMaxHypoth];
  Int_t prevRLayers[kMaxClusters+1][kMaxHypoth];
  Int_t nindexes[kMaxClusters+1][kMaxHypoth];
  Float_t normilczedchi2[kMaxHypoth];
  for (Int_t i=0;i<kMaxClusters+1;i++) ntracks[i]=0;
  otrack->SetNSkipped(0);
  new (&(tracks[0][0])) IlcTARGETtrackMI(*otrack);
  ntracks[0]=1;
  for (Int_t i=0;i<kMaxClusters+1;i++) nindexes[i][0]=0;
  for (Int_t i=0;i<kMaxClusters+1;i++)
    for (Int_t j=0;j<kMaxHypoth;j++) {
      nLayers[i][j]=-1;
      prevZLayers[i][j]=-1;
      prevRLayers[i][j]=-1;
    }
  // 
  //
  // follow prolongations
  int iladder=0;
  while(iladder<kMaxClusters){
    IlcDebug(3,Form("Try Ladder %i ",iladder));
    //
    ntracks[iladder+1]=0;
    //
    //
    Int_t nskipped=0;
    Float_t nused =0;
    for (Int_t itrack =0;itrack<ntracks[iladder];itrack++){
      //set current track
      if (ntracks[iladder]>=kMaxHypoth) break;  
      if (tracks[iladder][nindexes[iladder][itrack]].GetNSkipped()>0) nskipped++;
      if (tracks[iladder][nindexes[iladder][itrack]].GetNUsed()>2.) nused++;
//       if (ntracks[ilayer]>15+ilayer){
// 	if (itrack>1&&tracks[iladder][nindexes[iladder][itrack]].GetNSkipped()>0 && nskipped>4+ilayer) continue;
// 	if (itrack>1&&tracks[iladder][nindexes[iladder][itrack]].GetNUsed()>2. && nused>3) continue;
//       }

      new(&currenttrack1)  IlcTARGETtrackMI(tracks[iladder][nindexes[iladder][itrack]]);
      //
      //find intersection with layer
      IlcDebug(5,Form("Try to find new layer forr itrack= %i, iladder=%i ,curren Layer=%i, prev z Layer=%i ,prev r Layer=%i Bz=%f",
		      itrack,iladder,nLayers[iladder][nindexes[iladder][itrack]],
		      prevZLayers[iladder][nindexes[iladder][itrack]],prevRLayers[iladder][nindexes[iladder][itrack]],currenttrack1.GetBz()));
      int iLayer=GetNearestLayer(&currenttrack1,currenttrack1.GetBz(),
				 true,nLayers[iladder][nindexes[iladder][itrack]],
				 prevZLayers[iladder][nindexes[iladder][itrack]],prevRLayers[iladder][nindexes[iladder][itrack]]); 
      IlcDebug(5,Form("find layer =%i ",iLayer));
      if(iLayer<0) continue;
      bool skipLayer=false;
      if(iLayer>=fnLayers) skipLayer=true;
      IlcTARGETlayer &layer=fgLayers[iLayer];
      Double_t rLayer=layer.GetR();
      if(layer.IsEndCap()) {
	if(!currenttrack1.IsEndCap()) currenttrack1.RotateZ(true);
	if (!currenttrack1.Propagate(currenttrack1.GetAlpha(),layer.GetZ())){
	  IlcDebug(3,"Can't Propagate");
	  continue;
	}	  
      }else{
// 	if(currenttrack1.IsEndCap()) currenttrack1.RotateZ(false);
	Double_t x,y,z;  
	if (!currenttrack1.GetGlobalXYZat(rLayer,x,y,z)) continue;
	Double_t phi=TMath::ATan2(y,x);
	Int_t idet=layer.FindDetectorIndex(phi,z);

	if (idet<0) idet=-idet-1;

	//propagate to the intersection
	const IlcTARGETdetector &det=layer.GetDetector(idet);
	phi=det.GetPhi();
	IlcDebug(3,Form("Propagate To detector index=%i phi=%f r=%f",idet,phi,det.GetR()));
	if (!currenttrack1.Propagate(phi,det.GetR(), kFALSE)) {	
	  IlcDebug(3,"Can't Propagate");
	  continue;
	}
	currenttrack1.SetDetectorIndex(idet);
      }

      Double_t msz=1e30,msy=1e30;
      if(!skipLayer){
	Double_t xyz[3];  
	currenttrack1.GetXYZ(xyz,kFALSE);
	Double_t phi=TMath::ATan2(xyz[1],xyz[0]);
	Int_t idet=layer.FindDetectorIndex(phi,xyz[2]);
	if (idet<0) idet=-idet-1;
	phi=layer.GetDetector(idet).GetPhi();
	if(IlcDebugLevelClass()>=5) currenttrack1.Print("params");
// 	if(currenttrack1.IsEndCap()){
// 	  currenttrack1.RotateZ(false);
// 	  currenttrack1.Rotate(phi);
// 	  currenttrack1.RotateZ(true);
// 	}
	if(IlcDebugLevelClass()>=5){
	  cout<<"Track on detector: "<<endl;
	  currenttrack1.Print("params");
	}
	currenttrack1.SetDetectorIndex(idet);
	new(&currenttrack2)  IlcTARGETtrackMI(currenttrack1);
	//
	//
	Double_t dz=7.5*TMath::Sqrt(currenttrack1.GetSigmaZ2() + 16.*fParam->GetSigmaZ(iLayer)*fParam->GetSigmaZ(iLayer));
	Double_t dy=7.5*TMath::Sqrt(currenttrack1.GetSigmaY2() + 16.*fParam->GetSigmaRPhi(iLayer)*fParam->GetSigmaRPhi(iLayer));
	//
	IlcDebug(5,Form("lay %d SigmaRPhi %f SigmaZ: %f", iLayer, fParam->GetSigmaRPhi(iLayer), fParam->GetSigmaZ(iLayer)));

	Double_t zmin=currenttrack1.GetZ() - dz; 
	Double_t zmax=currenttrack1.GetZ() + dz;
	Double_t ymin=currenttrack1.GetY() + rLayer*phi - dy;
	Double_t ymax=currenttrack1.GetY() + rLayer*phi + dy;
	IlcDebug(3,Form("Selects clusters forr y=[%f,%f] z=[%f,%f]",ymin,ymax,zmin,zmax));

	layer.SelectClusters(zmin,zmax,ymin,ymax); 

	//
	// loop over all possible prolongations
	//
	msz=1./(dz/7.5);
	msy=1./(dy/7.5);
	if (constrain){
	  msy/=60; msz/=60.;
	}
	else{
	  msy/=50; msz/=50.;
	}
      }
      //
      const IlcTARGETRecPoint *c=0; Int_t ci=-1;
      Double_t chi2=12345.;
      Int_t deadzone=0;
      currenttrack = &currenttrack1;

      while (!skipLayer&&(c=layer.GetNextCluster(ci))!=0) { 
	if (ntracks[iladder+1]>kMaxHypoth-10) break; //space for skipped clusters  
	Bool_t change =kFALSE;  
	if (c->GetQ()==0 && (deadzone==1)) continue;
	Int_t idet=c->GetDetectorIndex();
	IlcDebug(3,Form("Check forr index=%i ,y=%f ,z= %f",idet,c->GetY(),c->GetZ()));
	if (currenttrack->GetDetectorIndex()!=idet) {
	  const IlcTARGETdetector &det=layer.GetDetector(idet);
	  Double_t y,z;
	  if (!currenttrack2.GetProlongationFast(det.GetPhi(),det.GetR(),y,z)) continue;
	  Float_t pz = (z - c->GetZ()) , py=(y - c->GetY());
	  IlcDebug(3,Form("Delta with rotate forr dy=%f ,dz= %f",py,pz));
	  if (pz*pz*msz+py*py*msy>1.) continue;
	  //
	  new (&backuptrack) IlcTARGETtrackMI(currenttrack2);
	  change = kTRUE;
	  currenttrack =&currenttrack2;
	  if (!currenttrack->Propagate(det.GetPhi(),det.GetR())) {
	    new (currenttrack) IlcTARGETtrackMI(backuptrack);
	    change = kFALSE;
	    continue;
	  }
	  currenttrack->SetDetectorIndex(idet);
	}
	else{
	  Float_t pz = (currenttrack->GetZ() - c->GetZ()) , py=(currenttrack->GetY() - c->GetY());
	  IlcDebug(3,Form("Delta forr dy=%f ,dz= %f",py,pz));
	  if (pz*pz*msz+py*py*msy>1.) continue;
	}

	chi2=GetPredictedChi2MI(currenttrack,c,iLayer); 
	IlcDebug(3,Form("Predicted Chi2=%f ",chi2));
	if (chi2<kMaxChi2s[iLayer]){
	  if (c->GetQ()==0) deadzone=1;	    // take dead zone only once	  
	  if (ntracks[iladder+1]>=kMaxHypoth) continue;
	  IlcTARGETtrackMI * updatetrack = new (&tracks[iladder+1][ntracks[iladder+1]]) IlcTARGETtrackMI(*currenttrack);
	  updatetrack->SetClIndex(iLayer,0);
	  if (change){
	    new (&currenttrack2) IlcTARGETtrackMI(backuptrack);
	  }
	  if (c->GetQ()!=0){
	    if (!UpdateMI(updatetrack,c,chi2,(iLayer<<kClusterLayerShift)+ci)) continue; 
	    updatetrack->SetSampledEdx(c->GetQ(),updatetrack->GetNumberOfClusters()-1); //b.b.
	  }
	  else {
	    updatetrack->SetNDeadZone(updatetrack->GetNDeadZone()+1);
	    updatetrack->SetDeadZoneProbability(GetDeadZoneProbability(updatetrack->GetZ(),TMath::Sqrt(updatetrack->GetSigmaZ2())));
	  }
	  if (c->IsUsed()){
	    updatetrack->IncrementNUsed();
	  }
	  Double_t x0;
	  Double_t dd=layer.GetThickness(updatetrack->GetY(),updatetrack->GetZ(),updatetrack->GetAlpha(),x0);
	  if(!updatetrack->CorrectForMaterial(dd,x0)) continue;	  
	  if (constrain) {
	    updatetrack->SetConstrain(constrain);
	    fI = iLayer;
	    Double_t d=GetEffectiveThickness(0,0); //Think of this !!!!
	    Double_t xyz[]={GetX(),GetY(),GetZ()};
	    Double_t ptfactor = 1;
	    Double_t ers[]={GetSigmaX()*ptfactor,GetSigmaY()*ptfactor,GetSigmaZ()};
	    Bool_t isPrim = kTRUE;
	    if (iLayer<4){
	      //updatetrack->fD[0] = updatetrack->GetD(GetX(),GetY());
	      //updatetrack->fD[1] = updatetrack->GetZat(GetX())-GetZ();
              updatetrack->GetDZ(GetX(),GetY(),GetZ(),updatetrack->GetDP()); //I.B.
	      if ( TMath::Abs(updatetrack->GetD(0)/(1.+iLayer))>0.4 ||  TMath::Abs(updatetrack->GetD(1)/(1.+iLayer))>0.4) isPrim=kFALSE;
	    }
	    if (isPrim) updatetrack->Improve(d,xyz,ers);
	  } //apply vertex constrain	   
	  nLayers[iladder+1][ntracks[iladder+1]]=iLayer;
	  if(layer.IsEndCap()){
	    prevZLayers[iladder+1][ntracks[iladder+1]]=iLayer;
	    prevRLayers[iladder+1][ntracks[iladder+1]]=prevRLayers[iladder][nindexes[iladder][itrack]];
	  }else{
	    prevRLayers[iladder+1][ntracks[iladder+1]]=iLayer;
	    prevZLayers[iladder+1][ntracks[iladder+1]]=prevZLayers[iladder][nindexes[iladder][itrack]];
	  }
	  ntracks[iladder+1]++;
	  IlcDebug(5,Form("Add updated track n=%i, number of clusters=%i",ntracks[iladder+1],updatetrack->GetNumberOfClusters()));
	}  // create new hypothesy 
      } // loop over possible cluster prolongation      
      if(currenttrack1.GetNSkipped()<kMaxLaddersSkipped&&ntracks[iladder+1]<kMaxHypoth){
	IlcTARGETtrackMI* vtrack = new (&tracks[iladder+1][ntracks[iladder+1]]) IlcTARGETtrackMI(currenttrack1);
 	vtrack->SetClIndex(iLayer,0);	
 	if(constrain){
 	  fI = iLayer;
 	  Double_t d=GetEffectiveThickness(0,0); //Think of this !!!!
 	  Double_t xyz[]={GetX(),GetY(),GetZ()};
 	  Double_t ers[]={GetSigmaX(),GetSigmaY(),GetSigmaZ()};
 	  vtrack->Improve(d,xyz,ers);
 	}
	vtrack->IncrementNSkipped();
	nLayers[iladder+1][ntracks[iladder+1]]=iLayer;
	if(layer.IsEndCap()){
	  prevZLayers[iladder+1][ntracks[iladder+1]]=iLayer;
	  prevRLayers[iladder+1][ntracks[iladder+1]]=prevRLayers[iladder][nindexes[iladder][itrack]];
	}else{
	  prevRLayers[iladder+1][ntracks[iladder+1]]=iLayer;
	  prevZLayers[iladder+1][ntracks[iladder+1]]=prevZLayers[iladder][nindexes[iladder][itrack]];
	}
	ntracks[iladder+1]++;
	IlcDebug(5,Form("Add skipped n=%i , number of clusters=%i",ntracks[iladder+1],vtrack->GetNumberOfClusters()));

      }
    } //loop over track candidates
    //
    //
    Int_t accepted=0;
    
    Int_t golds=0;
    for (Int_t itrack=0;itrack<ntracks[iladder+1];itrack++){
      normilczedchi2[itrack] = NormilczedChi2(&tracks[iladder+1][itrack],iladder+1); 
      IlcDebug(5,Form("Normchi2 i=%i , chi2=%f , number of clusters=%i",itrack,normilczedchi2[itrack],tracks[iladder+1][itrack].GetNumberOfClusters()));
      if ( normilczedchi2[itrack]<3) golds++;
      if (iladder<2) accepted++;
      else{
	if ( constrain && normilczedchi2[itrack]<kMaxNormChi2C[iladder+1]+1) accepted++;
	if (!constrain && normilczedchi2[itrack]<kMaxNormChi2NonC[iladder+1]+1) accepted++;
      }
    }
    TMath::Sort(ntracks[iladder+1],normilczedchi2,nindexes[iladder+1],kFALSE);
    ntracks[iladder+1] = TMath::Min(accepted,20);
    if (ntracks[iladder+1]>golds+5) ntracks[iladder+1]=TMath::Min(golds+5,accepted);
    if (ntracks[iladder+1]>90) ntracks[iladder+1]=90; 
    iladder++;
  } //loop over layers
  //printf("%d\t%d\t%d\t%d\t%d\t%d\n",ntracks[0],ntracks[1],ntracks[2],ntracks[3],ntracks[4],ntracks[5]);

  Int_t max[3] = {constrain? 20: 5,constrain? 3: 3,constrain? 3: 3};
  Int_t LastNotZero=0;
  for(int ii=0;ii<kMaxClusters+1;ii++) if(ntracks[ii]!=0) LastNotZero=ii;
  if(IlcDebugLevelClass()>=5){
    cout<<"Number of hypothis per try: ";
    for(int ii=0;ii<kMaxClusters+1;ii++) cout<<ii<<" "<<ntracks[ii]<<" ";
    cout<<endl;
  };
  for(int nlad=0;nlad<TMath::Min(3,LastNotZero);nlad++){
    IlcDebug(5,Form("Number of hypothis on try=%i, nhypot=%i ",nlad,ntracks[LastNotZero-nlad]));
    for (Int_t i=0;i<TMath::Min(max[nlad],ntracks[LastNotZero-nlad]);i++) {
      IlcTARGETtrackMI & track= tracks[LastNotZero-nlad][nindexes[LastNotZero-nlad][i]];
      IlcDebug(5,Form("%i number of clusters=%i, norm chi2=%f ",i,track.GetNumberOfClusters(),track.GetNormChi2(LastNotZero-nlad)));
      track.SetNSkipped(track.GetNSkipped()+nlad);
      if (track.GetNumberOfClusters()<2+nlad) continue;
      if (!constrain&&track.GetNormChi2(LastNotZero-nlad)>7.)continue;
      if(!constrain) track.SetD(0,track.GetD(GetX(),GetY())); 
      /*    if (!constrain) {
	track.SetD(0,track.GetD(GetX(),GetY()));   
	track.SetNSkipped(track.GetNSkipped()+4./(4.+8.*TMath::Abs(track.GetD(0))));
	//	track.SetNSkipped(track.GetNSkipped()+7./(7.+8.*TMath::Abs(track.GetD(0))));
	if (track.GetNumberOfClusters()+track.GetNDeadZone()+track.GetNSkipped()>6) {
	  track.SetNSkipped(6-track.GetNumberOfClusters()+track.GetNDeadZone());
	}
	}*/
      IlcDebug(5,"Add Hypothis");
      if(track.IsEndCap()) track.RotateZ(false);
      AddTrackHypothesys(new IlcTARGETtrackMI(track), esdindex);
    }
  }
    
  if (!constrain){
    //
    // register best tracks - important for V0 finder
    //
    for (Int_t ilayer=0;ilayer<kMaxClusters;ilayer++){
      if (ntracks[ilayer]==0) continue;
      IlcTARGETtrackMI & track= tracks[ilayer][nindexes[ilayer][0]];
      if (track.GetNumberOfClusters()<1) continue;
      CookLabel(&track,0);
      bestarray->AddAt(new IlcTARGETtrackMI(track),ilayer);
    }
  }
  //
  // update TPC V0 information
  //
  if (otrack->GetESDtrack()->GetV0Index(0)>0){    
    Float_t fprimvertex[3]={GetX(),GetY(),GetZ()};
    for (Int_t i=0;i<3;i++){
      Int_t  index = otrack->GetESDtrack()->GetV0Index(i); 
      if (index==0) break;
      IlcV0 * vertex = (IlcV0*)fEsd->GetV0(index);
      if (vertex->GetStatus()<0) continue;     // rejected V0
      //
      if (otrack->GetSign()>0) {
	vertex->SetIndex(0,esdindex);
      }
      else{
	vertex->SetIndex(1,esdindex);
      }
      //find nearest layer with track info
      Double_t Xr[3]; for(Int_t i=0; i<3; i++) Xr[i] = vertex->GetXr(i);
      Int_t nearestold  = GetNearestLayer(Xr);
      Int_t nearest     = nearestold; 
      for (Int_t ilayer =nearest;ilayer<12;ilayer++){
	if (ntracks[nearest]==0){
	  nearest = ilayer;
	}
      }
      //
      IlcTARGETtrackMI & track= tracks[nearest][nindexes[nearest][0]];
      if (nearestold<12&&nearest<12){
	Bool_t accept = track.GetNormChi2(nearest)<10; 
	if (accept){
	  if (track.GetSign()>0) {
	    vertex->SetP(track);
	    vertex->Update(fprimvertex);
	    //	    vertex->SetIndex(0,track.fESDtrack->GetID()); 
	    if (track.GetNumberOfClusters()>2) AddTrackHypothesys(new IlcTARGETtrackMI(track), esdindex);
	  }else{
	    vertex->SetM(track);
	    vertex->Update(fprimvertex);
	    //vertex->SetIndex(1,track.fESDtrack->GetID());
	    if (track.GetNumberOfClusters()>2) AddTrackHypothesys(new IlcTARGETtrackMI(track), esdindex);
	  }
	  vertex->SetStatus(vertex->GetStatus()+1);
	}else{
	  //  vertex->SetStatus(-2);  // reject V0  - not enough clusters
	}
      }
      // if (nearestold>3){
// 	Int_t indexlayer = (ntracks[0]>0)? 0:1;
// 	if (ntracks[indexlayer]>0){
// 	  IlcTARGETtrackMI & track= tracks[indexlayer][nindexes[indexlayer][0]];
// 	  if (track.GetNumberOfClusters()>4&&track.fNormChi2[indexlayer]<4){
// 	    vertex->SetStatus(-1);  // reject V0 - clusters before
// 	  }
// 	}
//      }
    }
  }  
}


const IlcTARGETtrackerMI::IlcTARGETlayer & IlcTARGETtrackerMI::GetLayer(Int_t layer) const
{
  //--------------------------------------------------------------------
  //
  //
  return fgLayers[layer];
}

IlcTARGETtrackerMI::IlcTARGETlayer::IlcTARGETlayer():
fRmin(0),
fRmax(0),
fdZ(0),
fZminGeom(0),
fZmaxGeom(0),
fR(0),
fPhiOffset(0),
fNladders(0),
fZOffset(0),
fNdetectors(0),
fOverlap(0.),
fDetectors(0),
fN(0),
fDy5(0),
fDy10(0),
fDy20(0),
fClustersCs(0),
fClusterIndexCs(0),
fYcs(0),
fZcs(0),
fNcs(0),
fCurrentSlice(-1),
fZmax(0),
fYmin(0),
fYmax(0),
fI(0),
fImax(0),
fSkip(0),
fAccepted(0),
fRoad(0),
kEndCap(false),
fWasPreMaterial(kFALSE),
fDMaterial(0),
fX0Material(1)
{
  //--------------------------------------------------------------------
  //default IlcTARGETlayer constructor
  //--------------------------------------------------------------------
  fYB[0]=10000000;
  fYB[1]=-10000000;
  for (Int_t i=0; i<kMaxClusterPerLayer;i++) {
    fClusters[i]=0;
  }
  for (Int_t i=0; i<kMaxClusterPerLayer;i++) {
    fClusterWeight[i]=0;
    fClusterTracks[0][i]=-1;
    fClusterTracks[1][i]=-1;
    fClusterTracks[2][i]=-1;    
    fClusterTracks[3][i]=-1;    
  }
}

void IlcTARGETtrackerMI::IlcTARGETlayer::Set(Double_t r,Double_t p,Double_t z,Int_t nl,Int_t nd,double overlap,double rmin,double rmax,double dz){
  fRmin=rmin;
  fRmax=rmax;
  fdZ=dz;
  fR=r;
  fPhiOffset=p;
  fNladders=nl;
  fZOffset=z;
  fNdetectors=nd;
  fOverlap=overlap;

  if(TMath::Abs(fNladders)*fNdetectors>0)
    fDetectors=new IlcTARGETdetector[TMath::Abs(fNladders)*fNdetectors];
  fRoad=2*fR*TMath::Sqrt(3.14/1.);//assuming that there's only one cluster
}

IlcTARGETtrackerMI::IlcTARGETlayer::IlcTARGETlayer(const IlcTARGETlayer& layer):
fRmin(layer.fRmin),
fRmax(layer.fRmax),
fdZ(layer.fdZ),
fZminGeom(layer.fZminGeom),
fZmaxGeom(layer.fZmaxGeom),
fR(layer.fR),
fPhiOffset(layer.fPhiOffset),
fNladders(layer.fNladders),
fZOffset(layer.fZOffset),
fNdetectors(layer.fNdetectors),
fOverlap(layer.fOverlap),
fDetectors(0),
fN(0),
fDy5(layer.fDy5),
fDy10(layer.fDy10),
fDy20(layer.fDy20),
fClustersCs(layer.fClustersCs),
fClusterIndexCs(layer.fClusterIndexCs),
fYcs(layer.fYcs),
fZcs(layer.fZcs),
fNcs(layer.fNcs),
fCurrentSlice(layer.fCurrentSlice),
fZmax(layer.fZmax),
fYmin(layer.fYmin),
fYmax(layer.fYmax),
fI(layer.fI),
fImax(layer.fImax),
fSkip(layer.fSkip),
fAccepted(layer.fAccepted),
fRoad(layer.fRoad),
kEndCap(layer.kEndCap),
fWasPreMaterial(kFALSE),
fDMaterial(0),
fX0Material(1)
{
  fYB[0]=10000000;
  fYB[1]=-10000000;
  for (Int_t i=0; i<kMaxClusterPerLayer;i++) {
    fClusters[i]=0;
  }
  if(TMath::Abs(fNladders)*fNdetectors>0)
    fDetectors=new IlcTARGETdetector[TMath::Abs(fNladders)*fNdetectors];
  //Copy constructor
}


IlcTARGETtrackerMI::IlcTARGETlayer::~IlcTARGETlayer() {
  //--------------------------------------------------------------------
  // IlcTARGETlayer destructor
  //--------------------------------------------------------------------
  delete[] fDetectors;
  for (Int_t i=0; i<fN; i++) delete fClusters[i];
  for (Int_t i=0; i<kMaxClusterPerLayer;i++) {
    fClusterWeight[i]=0;
    fClusterTracks[0][i]=-1;
    fClusterTracks[1][i]=-1;
    fClusterTracks[2][i]=-1;    
    fClusterTracks[3][i]=-1;    
  }
}

void IlcTARGETtrackerMI::IlcTARGETlayer::ResetClusters() {
  //--------------------------------------------------------------------
  // This function removes loaded clusters
  //--------------------------------------------------------------------
  for (Int_t i=0; i<fN; i++) delete fClusters[i];
  for (Int_t i=0; i<kMaxClusterPerLayer;i++){
    fClusterWeight[i]=0;
    fClusterTracks[0][i]=-1;
    fClusterTracks[1][i]=-1;
    fClusterTracks[2][i]=-1;    
    fClusterTracks[3][i]=-1;  
  }
  
  fN=0;
  fI=0;
}

void IlcTARGETtrackerMI::IlcTARGETlayer::ResetWeights() {
  //--------------------------------------------------------------------
  // This function reset weights of the clusters
  //--------------------------------------------------------------------
  for (Int_t i=0; i<kMaxClusterPerLayer;i++) {
    fClusterWeight[i]=0;
    fClusterTracks[0][i]=-1;
    fClusterTracks[1][i]=-1;
    fClusterTracks[2][i]=-1;    
    fClusterTracks[3][i]=-1;  
  }
  for (Int_t i=0; i<fN;i++) {
    IlcTARGETRecPoint * cl = (IlcTARGETRecPoint*)GetCluster(i);
    if (cl&&cl->IsUsed()) cl->Use();
  }

}

void IlcTARGETtrackerMI::IlcTARGETlayer::ResetRoad() {
  //--------------------------------------------------------------------
  // This function calculates the road defined by the cluster density
  //--------------------------------------------------------------------
  Int_t n=0;
  for (Int_t i=0; i<fN; i++) {
     if (TMath::Abs(fClusters[i]->GetZ())<fR) n++;
  }
  //if (n>1) fRoad=2*fR*TMath::Sqrt(3.14/n);
  if (n>1) fRoad=2*fR*TMath::Sqrt(3.14/n);
}


Int_t IlcTARGETtrackerMI::IlcTARGETlayer::InsertCluster(IlcTARGETRecPoint *c) {
  //--------------------------------------------------------------------
  //This function adds a cluster to this layer
  //--------------------------------------------------------------------
  if(c->GetDetectorIndex()>=abs(GetNdetectors()*GetNladders())){
    ::Error("InsertCluster",Form("Detector Index=%i bigger than number of modules nladders=%i ndetectors in ladder =%i, total=%i !\n",
				 c->GetDetectorIndex(),GetNladders(),GetNdetectors(),abs(GetNladders()*GetNdetectors())));
    return 1;
  }

  if (fN==kMaxClusterPerLayer) {
    ::Error("InsertCluster","Too many clusters !\n");
    return 1;
  }
  fCurrentSlice=-1;
  fClusters[fN]=c;
  fN++;
  IlcTARGETdetector &det=GetDetector(c->GetDetectorIndex());    
  IlcDebugGeneral("InsertCluster",2,Form(" Insert cluster y=%f , z=%f",c->GetY(),c->GetZ()));
  if (c->GetY()<det.GetYmin()) det.SetYmin(c->GetY());
  if (c->GetY()>det.GetYmax()) det.SetYmax(c->GetY());
  if (c->GetZ()<det.GetZmin()) det.SetZmin(c->GetZ());
  if (c->GetZ()>det.GetZmax()) det.SetZmax(c->GetZ());
			     
  return 0;
}

void  IlcTARGETtrackerMI::IlcTARGETlayer::SortClusters()
{
  //
  //sort clusters
  //
  IlcTARGETRecPoint **clusters = new IlcTARGETRecPoint*[fN];
  Float_t *z                = new Float_t[fN];
  Int_t   * index           = new Int_t[fN];
  //
  for (Int_t i=0;i<fN;i++){
    z[i] = fClusters[i]->GetZ();
  }
  TMath::Sort(fN,z,index,kFALSE);
  for (Int_t i=0;i<fN;i++){
    clusters[i] = fClusters[index[i]];
  }
  //
  for (Int_t i=0;i<fN;i++){
    fClusters[i] = clusters[i];
    fZ[i]        = fClusters[i]->GetZ();
    IlcTARGETdetector &det=GetDetector(fClusters[i]->GetDetectorIndex());    
    Double_t y=fR*det.GetPhi() + fClusters[i]->GetY();
    if (y>2.*fR*TMath::Pi()) y -= 2.*fR*TMath::Pi();
    fY[i] = y;
    IlcDebugGeneral("SortClusters",2,Form("Sorted clusters i=%i fY=%f fZ=%f ",i,fY[i],fZ[i]));
  }
  delete[] index;
  delete[] z;
  delete[] clusters;
  //

  fYB[0]=10000000;
  fYB[1]=-10000000;
  for (Int_t i=0;i<fN;i++){
    if (fY[i]<fYB[0]) fYB[0]=fY[i];
    if (fY[i]>fYB[1]) fYB[1]=fY[i];
    fClusterIndex[i] = i;
  }
  //
  // fill slices
  fDy5 = (fYB[1]-fYB[0])/5.;
  fDy10 = (fYB[1]-fYB[0])/10.;
  fDy20 = (fYB[1]-fYB[0])/20.;
  for (Int_t i=0;i<6;i++)  fN5[i] =0;  
  for (Int_t i=0;i<11;i++) fN10[i]=0;  
  for (Int_t i=0;i<21;i++) fN20[i]=0;
  //  
  for (Int_t i=0;i<6;i++) {fBy5[i][0] =  fYB[0]+(i-0.75)*fDy5; fBy5[i][1] =  fYB[0]+(i+0.75)*fDy5;}
  for (Int_t i=0;i<11;i++) {fBy10[i][0] =  fYB[0]+(i-0.75)*fDy10; fBy10[i][1] =  fYB[0]+(i+0.75)*fDy10;} 
  for (Int_t i=0;i<21;i++) {fBy20[i][0] =  fYB[0]+(i-0.75)*fDy20; fBy20[i][1] =  fYB[0]+(i+0.75)*fDy20;}
  //
  //
  for (Int_t i=0;i<fN;i++)
    for (Int_t irot=-1;irot<=1;irot++){
      Float_t curY = fY[i]+irot*TMath::TwoPi()*fR; 
      // slice 5
      for (Int_t slice=0; slice<6;slice++){
	if (fBy5[slice][0]<curY && curY<fBy5[slice][1]&&fN5[slice]<kMaxClusterPerLayer5){
	  fClusters5[slice][fN5[slice]] = fClusters[i];
	  fY5[slice][fN5[slice]] = curY;
	  fZ5[slice][fN5[slice]] = fZ[i];
	  fClusterIndex5[slice][fN5[slice]]=i;
	  fN5[slice]++;
	}
      }
      // slice 10
      for (Int_t slice=0; slice<11;slice++){
	if (fBy10[slice][0]<curY && curY<fBy10[slice][1]&&fN10[slice]<kMaxClusterPerLayer10){
	  fClusters10[slice][fN10[slice]] = fClusters[i];
	  fY10[slice][fN10[slice]] = curY;
	  fZ10[slice][fN10[slice]] = fZ[i];
	  fClusterIndex10[slice][fN10[slice]]=i;
	  fN10[slice]++;
	}
      }
      // slice 20
      for (Int_t slice=0; slice<21;slice++){
	if (fBy20[slice][0]<curY && curY<fBy20[slice][1]&&fN20[slice]<kMaxClusterPerLayer20){
	  fClusters20[slice][fN20[slice]] = fClusters[i];
	  fY20[slice][fN20[slice]] = curY;
	  fZ20[slice][fN20[slice]] = fZ[i];
	  fClusterIndex20[slice][fN20[slice]]=i;
	  fN20[slice]++;
	}
      }      
    }

  //
  // consistency check
  //
  for (Int_t i=0;i<fN-1;i++){
    if (fZ[i]>fZ[i+1]){
      printf("Bugg\n");
    }
  }
  //
  for (Int_t slice=0;slice<21;slice++)
  for (Int_t i=0;i<fN20[slice]-1;i++){
    if (fZ20[slice][i]>fZ20[slice][i+1]){
      printf("Bugg\n");
    }
  }


}


Int_t IlcTARGETtrackerMI::IlcTARGETlayer::FindClusterIndex(Float_t z) const {
  //--------------------------------------------------------------------
  // This function returns the index of the nearest cluster 
  //--------------------------------------------------------------------
  Int_t ncl=0;
  const Float_t *zcl;  
  if (fCurrentSlice<0) {
    ncl = fN;
    zcl   = fZ;
  }
  else{
    ncl   = fNcs;
    zcl   = fZcs;;
  }
  if (ncl==0) return 0;
  Int_t b=0, e=ncl-1, m=(b+e)/2;
  for (; b<e; m=(b+e)/2) {
    //    if (z > fClusters[m]->GetZ()) b=m+1;
    if (z > zcl[m]) b=m+1;
    else e=m; 
  }

  return m;
}


void IlcTARGETtrackerMI::IlcTARGETlayer::
SelectClusters(Double_t zmin,Double_t zmax,Double_t ymin, Double_t ymax) {
  //--------------------------------------------------------------------
  // This function sets the "window"
  //--------------------------------------------------------------------
 
  Double_t circle=2*TMath::Pi()*fR;
  fYmin = ymin; fYmax =ymax;
  Float_t ymiddle = (fYmin+fYmax)*0.5;
  if (ymiddle<fYB[0]) {fYmin+=circle; fYmax+=circle;ymiddle+=circle;}
  else{
    if (ymiddle>fYB[1]) {fYmin-=circle; fYmax-=circle;ymiddle-=circle;}
  }
  //
  fCurrentSlice =-1;
  // defualt take all
  fClustersCs = fClusters;
  fClusterIndexCs = fClusterIndex;
  fYcs  = fY;
  fZcs  = fZ;
  fNcs  = fN;
  //
  //is in 20 slice?
  if (fCurrentSlice<0&&TMath::Abs(fYmax-fYmin)<1.49*fDy20){
    Int_t slice = int(0.5+(ymiddle-fYB[0])/fDy20);
    if (slice<0) slice=0;
    if (slice>20) slice=20;
    Bool_t isOK = (fYmin>fBy20[slice][0]&&fYmax<fBy20[slice][1]);
    if (isOK) {
      fCurrentSlice=slice;
      fClustersCs = fClusters20[fCurrentSlice];
      fClusterIndexCs = fClusterIndex20[fCurrentSlice];
      fYcs  = fY20[fCurrentSlice];
      fZcs  = fZ20[fCurrentSlice];
      fNcs  = fN20[fCurrentSlice];
    }
  }  
  //
  //is in 10 slice?
  if (fCurrentSlice<0&&TMath::Abs(fYmax-fYmin)<1.49*fDy10){
    Int_t slice = int(0.5+(ymiddle-fYB[0])/fDy10);
    if (slice<0) slice=0;
    if (slice>10) slice=10;
    Bool_t isOK = (fYmin>fBy10[slice][0]&&fYmax<fBy10[slice][1]);
    if (isOK) {
      fCurrentSlice=slice;
      fClustersCs = fClusters10[fCurrentSlice];
      fClusterIndexCs = fClusterIndex10[fCurrentSlice];
      fYcs  = fY10[fCurrentSlice];
      fZcs  = fZ10[fCurrentSlice];
      fNcs  = fN10[fCurrentSlice];
    }
  }  
  //
  //is in 5 slice?
  if (fCurrentSlice<0&&TMath::Abs(fYmax-fYmin)<1.49*fDy5){
    Int_t slice = int(0.5+(ymiddle-fYB[0])/fDy5);
    if (slice<0) slice=0;
    if (slice>5) slice=5;
    Bool_t isOK = (fYmin>fBy5[slice][0]&&fYmax<fBy5[slice][1]);
    if ( isOK){
      fCurrentSlice=slice;
      fClustersCs = fClusters5[fCurrentSlice];
      fClusterIndexCs = fClusterIndex5[fCurrentSlice];
      fYcs  = fY5[fCurrentSlice];
      fZcs  = fZ5[fCurrentSlice];
      fNcs  = fN5[fCurrentSlice];
    }
  }  
  //  
  fI=FindClusterIndex(zmin); fZmin=zmin; fZmax=zmax;
  fImax = TMath::Min(FindClusterIndex(zmax)+1,fNcs);
  fSkip = 0;
  fAccepted =0;
}




Int_t IlcTARGETtrackerMI::IlcTARGETlayer::
FindDetectorIndex(Double_t phi, Double_t z) const {
  //--------------------------------------------------------------------
  //This function finds the detector crossed by the track
  //--------------------------------------------------------------------
  //   IlcDebugGeneral("IlcTARGETlayer",10,Form("Get Index forr phi=%f z=%f",phi,z));
  Double_t dphi=(phi-fPhiOffset);
  if      (dphi <  0) dphi += 2*TMath::Pi();
  else if (dphi >= 2*TMath::Pi()) dphi -= 2*TMath::Pi();
  Int_t np=Int_t(dphi*TMath::Abs(fNladders)*0.5/TMath::Pi()+0.5);
  if(fNladders<0) np=TMath::Abs(fNladders)-np;
  if (np>=TMath::Abs(fNladders)) np-=TMath::Abs(fNladders);
  if (np<0)          np+=TMath::Abs(fNladders);

  Double_t dz=fZOffset-z;
  Int_t nz=0;
  if(fabs(fZOffset)!=0)  
    nz=Int_t(dz*(fNdetectors-1)*0.5/fZOffset+0.5);


  if (nz>=fNdetectors) nz=fNdetectors-1;
  if (nz<0)            nz=0;

  if(!IsEndCap()){
    if(fabs(z)>fdZ) return -1-(np*fNdetectors + nz);
  }
  //  IlcDebugGeneral("IlcTARGETlayer",1,Form("Index %i ",np*fNdetectors + nz));
  return np*fNdetectors + nz;
}


const IlcTARGETRecPoint *IlcTARGETtrackerMI::IlcTARGETlayer::GetNextCluster(Int_t &ci){
  //--------------------------------------------------------------------
  // This function returns clusters within the "window" 
  //--------------------------------------------------------------------
  if (fCurrentSlice<0){
    Double_t rpi2 = 2.*fR*TMath::Pi();
    for (Int_t i=fI; i<fImax; i++) {
      Double_t y = fY[i];
      if (fYmax<y) y -= rpi2;
      if (fYmin>y) y += rpi2;
      if (y<fYmin) continue;
      if (y>fYmax) continue;
      if (fZ[i]<fZmin) continue;
      if (fZ[i]>fZmax) continue;
      
      if (fClusters[i]->GetQ()==0&&fSkip==2) continue;
      ci=i;
      fI=i+1;
      return fClusters[i];
    }
  }
  else{
    for (Int_t i=fI; i<fImax; i++) {
      if (fYcs[i]<fYmin) continue;
      if (fYcs[i]>fYmax) continue;
      if (fZcs[i]<fZmin) continue;
      if (fZcs[i]>fZmax) continue;
      if (fClustersCs[i]->GetQ()==0&&fSkip==2) continue;
      ci=fClusterIndexCs[i];
      fI=i+1;
      return fClustersCs[i];
    }
  }
  return 0;
}

void IlcTARGETtrackerMI::IlcTARGETlayer::PreCalculateMaterial(){
  double xbeg[3],xend[3];
  double mat[7]={0,1,0,0,0,0};    
  if(IsEndCap()){
    xbeg[2]=fZminGeom;
    xend[2]=fZmaxGeom;
    for(int i=0;i<10;i++){
      double cs=TMath::Cos(i*TMath::Pi()*2/fNladders/10.);
      double sn=TMath::Sin(i*TMath::Pi()*2/fNladders/10.);
      for(int j=0;j<10;j++){
	xbeg[0]=(fRmin+(fRmax-fRmin)*(i+1)/11.)*cs;
	xbeg[1]=(fRmin+(fRmax-fRmin)*(i+1)/11.)*sn;
	xend[0]=xbeg[0];
	xend[1]=xbeg[1];
      }
    }
  }else{
    for(int i=0;i<10;i++){
      double cs=TMath::Cos(i*TMath::Pi()*2/fNladders/10.);
      double sn=TMath::Sin(i*TMath::Pi()*2/fNladders/10.);
      xbeg[0]=fRmin*cs;
      xbeg[1]=fRmin*sn;
      xend[0]=fRmax*cs;
      xend[1]=fRmax*sn;
      for(int j=0;j<10;j++){
	xbeg[2]=fZminGeom+(fZmaxGeom-fZminGeom)*(i+1)/11.;
	xend[2]=fZminGeom+(fZmaxGeom-fZminGeom)*(i+1)/11.;
      }
    }
  }
  IlcTracker::MeanMaterialBudget(xbeg,xend,mat);
  fDMaterial=mat[4]*mat[0]/mat[1];
  fX0Material=mat[1];
//   cout<<"Forr Layer r=["<<fRmin<<","<<fRmax<<"] z=["<<fZminGeom<<","<<fZmaxGeom
//       <<" d="<<fDMaterial<<" X0= "<<fX0Material<<endl;
  fWasPreMaterial=kTRUE;
}


Double_t IlcTARGETtrackerMI::IlcTARGETlayer::GetThickness(Double_t y,Double_t z,Double_t alpha,Double_t &x0){
  //--------------------------------------------------------------------
  // This function returns the layer thickness at this point (units X0)
  // and X0 in gm/cm^2
  //--------------------------------------------------------------------
//   double thick=0.01;
//   double width=2*TMath::Pi()*GetR()/GetNladders();

//   Double_t d=2.33/21.82*thick;

//   if(fabs(fabs(y/width)-int(fabs(y/width))-0.5)<fOverlap*0.5) d*=2;
//   x0=21.82;

  double xbeg[3],xend[3];
  double mat[7]={0,1,0,0,0,0};    
  if(IsEndCap()){
    double radius=TMath::Hypot(y,z);
    //    if(radius<fRmax&&
    if(radius<200&&radius>fRmin){
      xbeg[0]=z*TMath::Cos(alpha)-y*TMath::Sin(alpha);
      xbeg[1]=z*TMath::Sin(alpha)+y*TMath::Cos(alpha);
      xbeg[2]=fZminGeom;
      xend[0]=xbeg[0];
      xend[1]=xbeg[1];
      xend[2]=fZmaxGeom;
      IlcTracker::MeanMaterialBudget(xbeg,xend,mat);
    }
  }else{
    if(fabs(z)<fdZ){
      double phi=y/GetR();
      if(fabs(phi)<1e-9) phi=1e-9;//bug somewhere in tgeo forr y=0; 
      xbeg[0]=fRmin*TMath::Cos(phi+alpha);
      xbeg[1]=fRmin*TMath::Sin(phi+alpha);
      xbeg[2]=z;
      xend[0]=fRmax*TMath::Cos(phi+alpha);
      xend[1]=fRmax*TMath::Sin(phi+alpha);
      xend[2]=z;
      IlcTracker::MeanMaterialBudget(xbeg,xend,mat);
//       int prec=cout.precision(10);
//       cout<<" "<<xbeg[0]<<","<<xbeg[1]<<","<<xbeg[2]<<","<<xend[0]<<","<<xend[1]<<","<<xend[2]<<endl;
//       cout<<mat[0]<<" "<<mat[1]<<" "<<mat[2]<<" "<<mat[3]<<" "<<mat[4]<<endl;
//       cout.precision(prec);
    }
  }
  
  double d=mat[4]*mat[0]/mat[1];
  x0=mat[1];

  return d;
}

Double_t IlcTARGETtrackerMI::IlcTARGETlayer::GetThickness(IlcTARGETtrackMI *tr, Double_t &x0,Bool_t useprecalc) {


  double xbeg[3],xend[3];
  double mat[7]={0,1,0,0,0,0};
  double xyz[3];

  if(useprecalc){
    if(!fWasPreMaterial) PreCalculateMaterial();
    if(IsEndCap()){
      x0=fX0Material;
      return fDMaterial*TMath::Sqrt((1.+ 1./(tr->GetTgl()*tr->GetTgl())));
    }else{
      x0=fX0Material;
      return fDMaterial*TMath::Sqrt((1.+ tr->GetTgl()*tr->GetTgl())/(1.- tr->GetSnp()*tr->GetSnp()));
    }
  }

  tr->GetXYZ(xyz,kFALSE);
//   cout<<"xyz "<<xyz[0]<<","<<xyz[1]<<","<<xyz[2]<<endl;
  if(IsEndCap()){
    double radius=TMath::Hypot(xyz[0],xyz[1]);
    if(radius<200&&radius>fRmin){
      bool somethingbad=false;
//       if(!(tr->GetXYZAtZ(GetZ()-2*fdZ,tr->GetBz(),xbeg)&&
// 	   tr->GetXYZAtZ(GetZ()+2*fdZ,tr->GetBz(),xend))){
	xbeg[0]=xyz[0];
	xbeg[1]=xyz[1];
	xbeg[2]=fZminGeom;
	xend[0]=xbeg[0];
	xend[1]=xbeg[1];
	xend[2]=fZmaxGeom;
	somethingbad=true;
//       }
        IlcTracker::MeanMaterialBudget(xbeg,xend,mat);
      if(!somethingbad){
	mat[4]/=TMath::Sqrt((1.+ 1./(tr->GetTgl()*tr->GetTgl())));
      }
//       cout<<" "<<xbeg[0]<<","<<xbeg[1]<<","<<xbeg[2]<<","<<xend[0]<<","<<xend[1]<<","<<xend[2]<<endl;
//       cout<<mat[0]<<" "<<mat[1]<<" "<<mat[2]<<" "<<mat[3]<<" "<<mat[4]<<endl;
    }
  }else{
    if(fabs(xyz[2])<fdZ){
      tr->GetXYZAtR(fRmin,tr->GetBz(),xbeg);
      tr->GetXYZAtR(fRmax,tr->GetBz(),xend);
//       cout<<"xbeg_xend "<<xbeg[0]<<","<<xbeg[1]<<","<<xbeg[2]<<","<<xend[0]<<","<<xend[1]<<","<<xend[2]<<" "<<xyz[0]<<","<<xyz[1]<<","<<xyz[2]<<
// 	" "<<fRmin<<" "<<fRmax<<endl;
      double length=TMath::Sqrt((xbeg[0]-xend[0])*(xbeg[0]-xend[0])+(xbeg[1]-xend[1])*(xbeg[1]-xend[1]));
      bool somethingbad=false;
      if(length>2*(fRmax-fRmin)*TMath::Sqrt(1./(1.- tr->GetSnp()*tr->GetSnp()))){
	double phi=TMath::ATan2(xyz[1],xyz[0]);
	xbeg[0]=fRmin*TMath::Cos(phi);
	xbeg[1]=fRmin*TMath::Sin(phi);
	xbeg[2]=xyz[2];
	xend[0]=fRmax*TMath::Cos(phi);
	xend[1]=fRmax*TMath::Sin(phi);
	xend[2]=xyz[2];
	somethingbad=true;
      }
//       cout<<"xbeg_xend "<<xbeg[0]<<","<<xbeg[1]<<","<<xbeg[2]<<","<<xend[0]<<","<<xend[1]<<","<<xend[2]<<" "<<xyz[0]<<","<<xyz[1]<<","<<xyz[2]<<
// 	" "<<fRmin<<" "<<fRmax<<endl;
      IlcTracker::MeanMaterialBudget(xbeg,xend,mat);
      if(!somethingbad){
	mat[4]/=TMath::Sqrt((1.+ tr->GetTgl()*tr->GetTgl())/(1.- tr->GetSnp()*tr->GetSnp()));
      }
//       cout<<mat[0]<<" "<<mat[1]<<" "<<mat[2]<<" "<<mat[3]<<" "<<mat[4]<<endl;
    }
  }
  double d=mat[4]*mat[0]/mat[1];
  x0=mat[1];

  return d;
}


Double_t IlcTARGETtrackerMI::GetEffectiveThickness(Double_t y,Double_t z) const
{
  //--------------------------------------------------------------------
  //Returns the thickness between the current layer and the vertex (units X0)
  //--------------------------------------------------------------------
  Double_t d=0.0018*1.2*1.2; //beam pipe
  Double_t x0=0;

  int fImax=fI;
  if(fI>=fParam->GetNlayers()) fImax=fParam->GetNlayers()-1;
  Double_t xn=fgLayers[fImax].GetR();
  for (Int_t i=0; i<fImax; i++) {
    Double_t xi=fgLayers[i].GetR();
    d+=fgLayers[i].GetThickness(y,z,0,x0)*xi*xi;
  }

  return d/(xn*xn);
}

Int_t IlcTARGETtrackerMI::IlcTARGETlayer::InRoad() const {
  //--------------------------------------------------------------------
  // This function returns number of clusters within the "window" 
  //--------------------------------------------------------------------
  Int_t ncl=0;
  for (Int_t i=fI; i<fN; i++) {
    const IlcTARGETRecPoint *c=fClusters[i];
    if (c->GetZ() > fZmax) break;
    if (c->IsUsed()) continue;
    const IlcTARGETdetector &det=GetDetector(c->GetDetectorIndex());    
    Double_t y=fR*det.GetPhi() + c->GetY();

    if (y>2.*fR*TMath::Pi()) y -= 2*fR*TMath::Pi();
    if (y>1.*fR*TMath::Pi() && fYmax<y) y -= 2*fR*TMath::Pi();

    if (y<fYmin) continue;
    if (y>fYmax) continue;
    ncl++;
  }
  return ncl;
}

Bool_t IlcTARGETtrackerMI::RefitAt(Double_t xx,IlcTARGETtrackMI *t,
				const IlcTARGETtrackMI *c, Bool_t extra,bool usePrecalcMaterial) {
  //--------------------------------------------------------------------
  // This function refits the track "t" at the position "x" using
  // the clusters from "c"
  // If "extra"==kTRUE, 
  //    the clusters from overlapped modules get attached to "t" 
  //--------------------------------------------------------------------
  int debugLevel=IlcDebugLevelClass();

  Int_t index[kMaxLayer];
  Int_t k;
  for (k=0; k<fnLayers; k++) index[k]=-1;
  Int_t nc=c->GetNumberOfClusters();
  int layermin=fnLayers,layermax=-1;
  for (k=0; k<nc; k++) { 
    Int_t idx=c->GetClusterIndex(k),nl=(idx&kClusterLayerMask)>>kClusterLayerShift;
    index[nl]=idx; 
    if(nl<layermin)layermin=nl;
    if(nl>layermax)layermax=nl;
    
  }
  Int_t step;
  if(debugLevel>=2) IlcDebug(2,Form("RefitAt from=%f to=%f ",t->GetX(),xx));
  if (fabs(xx) > fabs(t->GetX())) {
      step=+1;
      if(extra){
	double xyz[3];
	t->GetXYZ(xyz,kFALSE);
	double zatR;
	if(!t->GetZAt(fgLayers[0].GetRmin(),t->GetBz(),zatR)){
	  zatR=t->GetTgl()*(fgLayers[0].GetRmin()-TMath::Hypot(xyz[0],xyz[1]))+xyz[2];
	}
	if(fabs(zatR)>fmaxZ){
	  if(!t->IsEndCap()) t->RotateZ(true);
	  if(!t->Propagate(t->GetAlpha(),TMath::Sign(fmaxZ,zatR))){
	    t->Propagate(t->GetAlpha(),TMath::Sign(fabs(t->GetX())+1.,t->GetX()));
	  };
	  t->RotateZ(false);
	}else if(!t->Propagate(t->GetAlpha(),fgLayers[0].GetRmin())){
	  t->Propagate(t->GetAlpha(),TMath::Max(t->GetX()-1.,fgLayers[0].GetRmin()));
	}
      }else{
	t->Propagate(t->GetAlpha(),TMath::Max(t->GetX()-1.,fgLayers[0].GetRmin()));
      }
  } else {
      step=-1;
      double alpha=t->GetAlpha(),xcur=t->GetX();
      double dangle=0.5*(TMath::ASin(t->GetSnp())+TMath::ATan2(t->GetY(),t->GetX()));
//       cout<<"before Refit "<<endl;
//       t->Print("params");

      //propgate before to outer part ov TARGET
      double xyz[3];
      t->GetXYZ(xyz);
      double zatR;
      if(extra){
	if(!t->GetZAt(fmaxR,t->GetBz(),zatR)){
	  zatR=t->GetTgl()*(fmaxR-TMath::Hypot(xyz[0],xyz[1]))+xyz[2];
	}
	if(fabs(zatR)>fmaxZ){
	  if(!t->IsEndCap()) t->RotateZ(true);
	  if(!t->Propagate(alpha,TMath::Sign(fmaxZ,zatR))){
	    t->Propagate(alpha,TMath::Sign(fabs(t->GetX())+1.5,t->GetX()));
	  };
	  t->RotateZ(false);
	}else{
	  if(!t->Propagate(alpha,fmaxR)){
	    if(xcur<fmaxR)
	      if(!t->Propagate(alpha,xcur+1.)){
		if(!t->IsEndCap()) t->RotateZ(true);
		t->Propagate(alpha,TMath::Sign(fabs(t->GetX())+1.5,t->GetX()));
		t->RotateZ(false);
	      }
	  }else{ t->Propagate(alpha+dangle,fmaxR);}
	}
      }else{
	if(xcur<fmaxR)
	  if(t->GetDir()<0||!t->Propagate(alpha,xcur+1.5)){
	    if(!t->IsEndCap()) t->RotateZ(true);
	    t->Propagate(alpha,TMath::Sign(fabs(t->GetX())+1.5,t->GetX()));
	    t->RotateZ(false);
	  }
      }
     
  }
  double zbegin=t->GetZ();
  int nextlayer=-1;
  int prevZlayer=-1;
  int prevRlayer=-1;
  double prevRdirection=0.;
  int minRusedlayer=1000;
  int maxRusedlayer=-1;
  int ntries=0;
  double maxrotate=0.5;
  IlcTARGETtrackMI lastupdate_track(*t);
  
  while((t->GetNumberOfClusters()<c->GetNumberOfClusters()||extra)&&ntries++<100){
    if(ntries==100) if(debugLevel>=1) IlcDebug(1,"Maximum number of crossed layers reached ");
    if(debugLevel>=3) IlcDebug(3,Form("From Layer =%i",nextlayer));
    int prevlayer=nextlayer;
    nextlayer=GetNearestLayer(t,t->GetBz(),step==-1,nextlayer,prevZlayer,prevRlayer);
    bool oppositeR=false;
    if(nextlayer<-1){
      nextlayer=-nextlayer-10;
      oppositeR=true;
    }
    if(debugLevel>=3) IlcDebug(3,Form("Nextlayer =%i",nextlayer));
    if(nextlayer<0) break;
    IlcTARGETlayer &layer=fgLayers[nextlayer];
    if(layer.IsEndCap()!=t->IsEndCap()) t->RotateZ(layer.IsEndCap());
    
    Double_t rLayer=layer.GetR();
 
    
    // remember old position [SR, GSI 18.02.2003]
    Double_t oldX=0., oldY=0., oldZ=0.;
    if (t->IsStartedTimeIntegral() && step==1) {
      t->GetGlobalXYZat(t->GetX(),oldX,oldY,oldZ);
    }
    //
    Int_t idet=-1;
    Double_t phi=0;

    bool skipLayer=false;
    if(nextlayer>=fnLayers) skipLayer=true;

    if(layer.IsEndCap()) {
      if(step==-1&&zbegin*layer.GetZ()<0){
	if(debugLevel>=3) IlcDebug(3,Form("Finish refiting because of opposite sign Z layer"));
	break;
      } 
      if(prevlayer==prevZlayer) prevRlayer=-1;
      prevZlayer=nextlayer;
 //      cout<<"Propagate to Z"<<endl;
//       t->Print("params");
      double nloops=lastupdate_track.GetNLoopsToZ(layer.GetZ(),lastupdate_track.GetBz());
      if(fabs(nloops)>(maxrotate*(extra?1.:2.))) {
	if(debugLevel>=3) IlcDebug(3,Form("Big number of loops from previous update to new layer = %f",nloops));
	break;
      }
      if(TMath::Hypot(t->GetZ(),t->GetY())>2.*fmaxR)
	break;
      if (!t->Propagate(t->GetAlpha(),layer.GetZ())) break;

      if(t->GetCosRDir()*prevRdirection<0) prevRlayer=-1;
//       cout<<" After propagate"<<endl;
//       t->Print("params");

      if(!skipLayer){
	Double_t xyz[3];  
	t->GetXYZ(xyz,kFALSE);
	phi=TMath::ATan2(xyz[1],xyz[0]);
	idet=layer.FindDetectorIndex(phi,xyz[2]);
	if (idet<0) continue;
	const IlcTARGETdetector &det=layer.GetDetector(idet);
	//      cout<<" phi track "<<phi<<" detector phi "<<det.GetPhi()<<" idet="<<idet<<endl;
	phi=det.GetPhi();
	t->RotateZ(false);
	t->Rotate(phi);
	t->RotateZ(true);
//       cout<<" After rotate"<<endl;
//       t->Print("params");
      }
    }else{
      Double_t xyz[3];
      prevRlayer=nextlayer;

//       t->Print("params");

      double rLayerfirst=rLayer;
      if(oppositeR&&prevlayer==nextlayer){
	rLayerfirst=TMath::Hypot(t->GetX(),t->GetY())-TMath::Sign(1e-6,step*t->GetCosRDir());
      }
      if (!t->GetXYZAtR(rLayerfirst,t->GetBz(),xyz,(oppositeR?-1:1))) continue;
      phi=TMath::ATan2(xyz[1],xyz[0]);

      if(oppositeR){
	if(fabs(xyz[2]-t->GetZ())<kAlmost0) break;
	t->RotateZ(true);
	if (!t->Propagate(t->GetAlpha(),xyz[2])){
	  t->RotateZ(false);
	  continue;
	}
	t->RotateZ(false);
      }      
//       cout<<"phi "<<phi<<" z "<<xyz[2]<<" xy "<<xyz[0]<<" "<<xyz[1]<<" rl "<<rLayer<<" "<<rLayerfirst-rLayer<<endl;
//       t->Print("params");
      double rLayersecond=rLayer;
      if(oppositeR&&prevlayer==nextlayer){
	rLayersecond=((step*t->GetCosRDir()>0)?layer.GetRmax():layer.GetRmin());
	if (!t->GetXYZAtR(rLayersecond,t->GetBz(),xyz)) continue;
	phi=TMath::ATan2(xyz[1],xyz[0]);
      }
//       cout<<"phi "<<phi<<" z "<<xyz[2]<<" xy "<<xyz[0]<<" "<<xyz[1]<<endl;
//       t->Print("params");

      if (!t->Propagate(phi,rLayersecond,true)) {
	if(!t->Invariant()) break;
	continue;
      }
//       t->Print("params");
     
      
      if(fabs(xyz[2]-t->GetZ())>5e-4){
	if(debugLevel>=1) IlcDebug(1,Form("wrong direction at R=%f z=%f but after propagate z=%f",rLayer,xyz[2],t->GetZ()));
      }
      double nloops=lastupdate_track.GetNLoopsToZ(t->GetZ(),lastupdate_track.GetBz());
      if(fabs(nloops)>(maxrotate*(extra?1.:2.))) {
	if(debugLevel>=3) 
	  IlcDebug(3,Form("Big number of loops from previous update to new layer = %f",nloops));
	break;
      }

      prevRdirection=t->GetCosRDir();

      if(nextlayer>=minRusedlayer&&nextlayer<=maxRusedlayer) 
	skipLayer=true;
      if(!skipLayer){
// 	t->Print("params");
// 	cout<<"sigma "<<TMath::Sqrt(t->GetSigmaZ2())<<endl;
	idet=layer.FindDetectorIndex(phi,xyz[2]);
	//       cout<<"idet "<<idet<<endl;
	if (idet<0){
	  idet=layer.FindDetectorIndex(phi,TMath::Sign(fabs(xyz[2])-2*TMath::Sqrt(t->GetSigmaZ2()),xyz[2]));
	  if(idet<0)
	    continue;
	}
	const IlcTARGETdetector &det=layer.GetDetector(idet);
	phi=det.GetPhi();
	if (!t->Propagate(phi,det.GetR()))  continue;
// 	cout<<"idet "<<idet<<endl;
// 	t->Print("params");
      }
    }
    t->SetDetectorIndex(idet);
    const IlcTARGETRecPoint *cl=0;
    Double_t maxchi2=(extra?10.:2)*kMaxChi2;

    Int_t idx=-1;
    if(!skipLayer) idx=index[nextlayer];
    if (idx!=-1) {
      const IlcTARGETRecPoint *rp=(IlcTARGETRecPoint *)GetCluster(idx);
      IlcTARGETtrackMI bckptrack_check(*t);

      if (rp){
	if(debugLevel>=3) IlcDebug(3,Form(" idet=%i cluster idet=%i ",idet,rp->GetDetectorIndex()));
	if (idet != rp->GetDetectorIndex()) {
	  idet=rp->GetDetectorIndex();
	  const IlcTARGETdetector &det=layer.GetDetector(idet);
	  phi=det.GetPhi();
	  if(layer.IsEndCap()){
	    t->RotateZ(false);
	    t->Rotate(phi);
	    t->RotateZ(true);
	  }else{
	    double dphi=phi-t->GetAlpha();
	    if(dphi>TMath::Pi()) dphi-=2*TMath::Pi();
	    if(dphi<-TMath::Pi()) dphi+=2*TMath::Pi();
	    
	    if(fabs(dphi)>TMath::Pi()/4){
	      if(debugLevel>=3) 
		IlcDebug(3,Form("Skip predifined cluster, current phi=%f, cluster phi=%f",t->GetAlpha(),phi));
	      maxchi2=-1e6;
	    }
	    if (!t->Propagate(phi,det.GetR())) {
	      break;
	    }
	  }
	  t->SetDetectorIndex(idet);
	}
	if(debugLevel>=3) 
	  IlcDebug(3,Form("Check cluster layer =%i , track Y-Z=%f %f ,cluster Y-Z= %f %f , sigma track=%f %f, cluster sigma %f %f, label=%i ",nextlayer,
			  t->GetY(),t->GetZ(),rp->GetY(),c->GetZ(),
			  TMath::Sqrt(t->GetSigmaY2()),TMath::Sqrt(t->GetSigmaZ2()),TMath::Sqrt(rp->GetSigmaY2()),TMath::Sqrt(rp->GetSigmaZ2()),
			  rp->GetLabel(0)));
	//Double_t chi2=t->GetPredictedChi2(c);
        Double_t chi2=GetPredictedChi2MI(t,rp,nextlayer);
        if(debugLevel>=3) IlcDebug(3,Form("layer =%i , chi2=%f , label=%i",nextlayer,chi2,rp->GetLabel(0)));
	if (chi2<maxchi2) { 
          cl=rp; 
	  maxchi2=chi2; 
	}else{
	  *t=bckptrack_check;
	}
      }
    }
    
    if (cl) {
      if(!extra&&!layer.IsEndCap()&&((t->GetDir()<0)!=((idx&kClusterDirMask)!=0))){
	if(debugLevel>=1) IlcDebug(1,"Warning update from different direction==> skip");
      }else{
	if (!UpdateMI(t,cl,maxchi2,idx)) {
	  return kFALSE;
	}
	t->SetSampledEdx(cl->GetQ(),t->GetNumberOfClusters()-1);
	index[nextlayer]=-1;
	if(!layer.IsEndCap()) {
	  if(nextlayer<minRusedlayer) minRusedlayer=nextlayer;
	  if(nextlayer>maxRusedlayer) maxRusedlayer=nextlayer;
	}
	lastupdate_track=*t;
      }
    }
    
    if (!skipLayer&&extra&&!cl) { //search for extra clusters
      if(debugLevel>=3) IlcDebug(3," Try extra clusters ");
//       for(int i=0;i<layer.GetNumberOfClusters();i++){
// 	IlcTARGETRecPoint *rec=layer.GetCluster(i);
// 	cout<<"rec "<<rec->GetY()<<" "<<rec->GetZ()<<" phi "<<phi<<" det "<<layer.GetDetector(rec->GetDetectorIndex()).GetPhi()<<endl;
//       };
      Double_t dz=4*TMath::Sqrt(t->GetSigmaZ2()+fParam->GetSigmaZ(nextlayer)*fParam->GetSigmaZ(nextlayer));
      Double_t dy=4*TMath::Sqrt(t->GetSigmaY2()+fParam->GetSigmaRPhi(nextlayer)*fParam->GetSigmaRPhi(nextlayer));

      Double_t zmin=t->GetZ() - dz;
      Double_t zmax=t->GetZ() + dz;
      Double_t ymin=t->GetY() + phi*rLayer - dy;
      Double_t ymax=t->GetY() + phi*rLayer + dy;
      layer.SelectClusters(zmin,zmax,ymin,ymax);
      const IlcTARGETRecPoint *rp=0,*extracl=0; Int_t ci=-1;
      maxchi2=kMaxChi2;
      IlcTARGETtrackMI bckptrack_check(*t);
      while ((rp=layer.GetNextCluster(ci))!=0) {
        if(rp->IsUsed()) continue;
        const IlcTARGETdetector &det=layer.GetDetector(rp->GetDetectorIndex());
// 	cout<<" phi track "<<phi<<" detector phi "<<det.GetPhi()<<" idet="<<idet<<" ycl "<<c->GetY() + phi*rLayer<<" zcl "<<c->GetZ()<<" rLayer "<<rLayer<<" "<<det.GetR()<<endl;
// 	cout<<" Befor rotate"<<endl;
// 	t->Print("params");

	if(layer.IsEndCap()){
	  t->RotateZ(false);
	  t->Rotate(det.GetPhi());
	  t->RotateZ(true);  
	}else{
	  if (!t->Propagate(det.GetPhi(),det.GetR())) {
	    *t=bckptrack_check;
	    continue;
	  }
 	}
// 	cout<<" After rotate"<<endl;
// 	t->Print("params");
	if(debugLevel>=3) 
	  IlcDebug(3,Form("Try extra cluster layer =%i , track Y-Z=%f %f ,cluster Y-Z= %f %f , sigma track=%f %f, cluster sigma %f %f, label=%i ",nextlayer,
			  t->GetY(),t->GetZ(),rp->GetY(),rp->GetZ(),
			  TMath::Sqrt(t->GetSigmaY2()),TMath::Sqrt(t->GetSigmaZ2()),TMath::Sqrt(rp->GetSigmaY2()),TMath::Sqrt(rp->GetSigmaZ2()),
			  rp->GetLabel(0)));
	
        Double_t chi2=GetPredictedChi2MI(t,rp,nextlayer);
	if(debugLevel>=3) 
	  IlcDebug(3,Form("extra cluster have chi2=%f ",chi2));
	if (chi2<maxchi2) { 
          maxchi2=chi2; extracl=rp; 
	  idx=(nextlayer<<kClusterLayerShift)+ci;
	}
// 	cout<<"after check "<<endl;
// 	t->Print();
	*t=bckptrack_check;
      }
      if (extracl) {
	if(debugLevel>=3) 
	  IlcDebug(3,Form("Add extra cluster at layer %i with chi2=%f",nextlayer,maxchi2));

// 	cout<<"begin update "<<endl;
// 	t->Print();
	const IlcTARGETdetector &det=layer.GetDetector(extracl->GetDetectorIndex());
	if(layer.IsEndCap()){
	  t->RotateZ(false);
	  t->Rotate(det.GetPhi());
	  t->RotateZ(true);  
	}else{
	  if (!t->Propagate(det.GetPhi(),det.GetR())) extracl=0;
 	}

// 	cout<<"at detector "<<endl;
// 	t->Print();
	if(extracl){
	  //to save needed values after backup
	  GetPredictedChi2MI(t,extracl,nextlayer);
	  if (!UpdateMI(t,extracl,maxchi2,idx)) {
// 	    cout<<"fail update "<<endl;
// 	    t->Print();
	    return kFALSE;
	  }
	  if(!layer.IsEndCap()) {
	    if(nextlayer<minRusedlayer) minRusedlayer=nextlayer;
	    if(nextlayer>maxRusedlayer) maxRusedlayer=nextlayer;
	  }
	  t->SetSampledEdx(extracl->GetQ(),t->GetNumberOfClusters()-1);
	  lastupdate_track=*t;
	}
      }
      if(debugLevel>=3) IlcDebug(3," Extra done ");
    }

    {
//       t->Print("params");
      Double_t x0;
      //      Double_t d=layer.GetThickness(t->GetY(),t->GetZ(),t->GetAlpha(),x0);
      Double_t d=layer.GetThickness(t,x0,usePrecalcMaterial);
     if(!t->CorrectForMaterial(-step*d,x0)) return kFALSE;
    }
    
    // track time update [SR, GSI 17.02.2003]
    if (t->IsStartedTimeIntegral() && step==1) {
      Double_t newX, newY, newZ;
      t->GetGlobalXYZat(t->GetX(),newX,newY,newZ);
      Double_t dL2 = (oldX-newX)*(oldX-newX) + (oldY-newY)*(oldY-newY) + 
	(oldZ-newZ)*(oldZ-newZ);
      t->AddTimeStep(TMath::Sqrt(dL2));
    }
    //

  }
  *t=lastupdate_track;
  if(t->IsEndCap()) t->RotateZ(false);
  if(debugLevel>=5){
    cout<<"Refit done ncl before="<<c->GetNumberOfClusters()<<" after="<<t->GetNumberOfClusters()<<
      " chi2="<<c->GetChi2()<<" after="<<t->GetChi2()<<endl;
    cout<<"Refit done clusters= ";
    for(int i=0;i<c->GetNumberOfClusters();i++){
      Int_t idx=c->GetClusterIndex(i),nl=(idx&kClusterLayerMask)>>kClusterLayerShift;
      cout<<nl<<" ";
    };
    cout<<endl;
    cout<<"Refit done clusters= ";
    for(int i=0;i<t->GetNumberOfClusters();i++){
      Int_t idx=t->GetClusterIndex(i),nl=(idx&kClusterLayerMask)>>kClusterLayerShift;
      cout<<nl<<" ";
    };
    cout<<endl;
    t->Print("params");
  }
  if(t->GetNumberOfClusters()<c->GetNumberOfClusters()&&extra) 
    if(debugLevel>=3) IlcDebug(3,Form("Number of clusters not same after refit before=%i after=%i",
				      c->GetNumberOfClusters(),t->GetNumberOfClusters()));

  if(t->GetNumberOfClusters()<3) return kFALSE;
//   if(xx>=-1e-10)if (!t->PropagateTo(xx,0.,0.)) return kFALSE;
  if(xx>=-1e-10) t->PropagateTo(xx,0.,0.);
  return kTRUE;
}

Bool_t 
IlcTARGETtrackerMI::RefitAt(Double_t xx,IlcTARGETtrackMI *t,const Int_t *clindex) {
  //--------------------------------------------------------------------
  // This function refits the track "t" at the position "x" using
  // the clusters from array
  //--------------------------------------------------------------------
  Int_t index[kMaxLayer];
  Int_t k;
  for (k=0; k<fnLayers; k++) index[k]=-1;
  //
  for (k=0; k<fnLayers; k++) { 
    index[k]=clindex[k]; 
  }

  Int_t from, to, step;
  if (xx > t->GetX()) {
      from=0; to=fnLayers;
      step=+1;
  } else {
      from=fnLayers-1; to=-1;
      step=-1;
  }

  for (Int_t i=from; i != to; i += step) {
     IlcTARGETlayer &layer=fgLayers[i];
     Double_t r=layer.GetR();
     if (step<0 && xx>r) break;  //

     // remember old position [SR, GSI 18.02.2003]
     Double_t oldX=0., oldY=0., oldZ=0.;
     if (t->IsStartedTimeIntegral() && step==1) {
        t->GetGlobalXYZat(t->GetX(),oldX,oldY,oldZ);
     }
     //

     Double_t x,y,z;
     if (!t->GetGlobalXYZat(r,x,y,z)) { 
       return kFALSE;
     }
     Double_t phi=TMath::ATan2(y,x);
     Int_t idet=layer.FindDetectorIndex(phi,z);
     if (idet<0) { 
       return kFALSE;
     }
     const IlcTARGETdetector &det=layer.GetDetector(idet);
     phi=det.GetPhi();
     if (!t->Propagate(phi,det.GetR())) {
       return kFALSE;
     }
     t->SetDetectorIndex(idet);

     const IlcTARGETRecPoint *cl=0;
     Double_t maxchi2=1000.*kMaxChi2;

     Int_t idx=index[i];
     if (idx>0) {
        const IlcTARGETRecPoint *c=(IlcTARGETRecPoint *)GetCluster(idx); 
	if (c){
	  if (idet != c->GetDetectorIndex()) {
	    idet=c->GetDetectorIndex();
	    const IlcTARGETdetector &ddet=layer.GetDetector(idet);
	    if (!t->Propagate(ddet.GetPhi(),ddet.GetR())) {
	      return kFALSE;
	    }
	    t->SetDetectorIndex(idet);
	  }
	  //Double_t chi2=t->GetPredictedChi2(c);
	  Int_t ilayer = (idx & kClusterLayerMask) >> kClusterLayerShift;;
	  Double_t chi2=GetPredictedChi2MI(t,c,ilayer);
	  if (chi2<maxchi2) { 
	    cl=c; 
	    maxchi2=chi2; 
	  } else {
	    return kFALSE;
	  }
	}
     }
     if (cl) {
       //if (!t->Update(cl,maxchi2,idx)) {
       if (!UpdateMI(t,cl,maxchi2,idx)) {
          return kFALSE;
       }
       t->SetSampledEdx(cl->GetQ(),t->GetNumberOfClusters()-1);
     }

     {
     Double_t x0;
     Double_t d=layer.GetThickness(t->GetY(),t->GetZ(),t->GetAlpha(),x0);
     if(!t->CorrectForMaterial(-step*d,x0)) return kFALSE;
     }
                 
     // track time update [SR, GSI 17.02.2003]
     if (t->IsStartedTimeIntegral() && step==1) {
        Double_t newX, newY, newZ;
        t->GetGlobalXYZat(t->GetX(),newX,newY,newZ);
        Double_t dL2 = (oldX-newX)*(oldX-newX) + (oldY-newY)*(oldY-newY) + 
                       (oldZ-newZ)*(oldZ-newZ);
        t->AddTimeStep(TMath::Sqrt(dL2));
     }
     //

  }

  if (!t->PropagateTo(xx,0.,0.)) return kFALSE;
  return kTRUE;
}

Double_t IlcTARGETtrackerMI::GetNormilczedChi2(IlcTARGETtrackMI * track, Int_t mode)
{
  //
  // calculate normilczed chi2
  //  return NormilczedChi2(track,0);
  Float_t chi2 = 0;
  Float_t sum=0;
  Float_t *erry = GetErrY(fCurrentEsdTrack), *errz = GetErrZ(fCurrentEsdTrack);
  //  track->fdEdxMismatch=0;
  Float_t dedxmismatch =0;
  Float_t *ny = GetNy(fCurrentEsdTrack);//, *nz = GetNz(fCurrentEsdTrack); 
  if (mode<100){
    for (Int_t i = 0;i<kMaxClusters;i++){
      if (track->GetClIndex(i)>0){
	Float_t cerry, cerrz;
	if (ny[i]>0) {cerry = erry[i]; cerrz=errz[i];}
	else 
	  { cerry= track->GetSigmaY(i); cerrz = track->GetSigmaZ(i);}
	cerry*=cerry;
	cerrz*=cerrz;	
	Float_t cchi2 = (track->GetDy(i)*track->GetDy(i)/cerry)+(track->GetDz(i)*track->GetDz(i)/cerrz);
// 	if (i>1){
// 	  Float_t ratio = track->GetNormQ(i)/track->GetExpQ();
// 	  if (ratio<0.5) {
// 	    cchi2+=(0.5-ratio)*10.;
// 	    //track->fdEdxMismatch+=(0.5-ratio)*10.;
// 	    dedxmismatch+=(0.5-ratio)*10.;	    
// 	  }
// 	}
// 	if (i<2 ||i>3){
// 	  IlcTARGETRecPoint * cl = (IlcTARGETRecPoint*)GetCluster( track->GetClIndex(i));  
// 	  Double_t delta = cl->GetNy()+cl->GetNz()-ny[i]-nz[i];
// 	  if (delta>1) chi2 +=0.5*TMath::Min(delta/2,2.); 
// 	  if (i<2) chi2+=2*cl->GetDeltaProbability();
// 	}
	chi2+=cchi2;
	sum++;
      }
    }
    if (TMath::Abs(track->GetdEdxMismatch()-dedxmismatch)>0.0001){
      track->SetdEdxMismatch(dedxmismatch);
    }
  }
  else{
    cout<<"Check GetNormilizedChi2"<<endl;
    for (Int_t i = 0;i<4;i++){
      if (track->GetClIndex(i)>0){
	Float_t cerry, cerrz;
	if (ny[i]>0) {cerry = erry[i]; cerrz=errz[i];}
	else { cerry= track->GetSigmaY(i); cerrz = track->GetSigmaZ(i);}
	cerry*=cerry;
	cerrz*=cerrz;
	chi2+= (track->GetDy(i)*track->GetDy(i)/cerry);
	chi2+= (track->GetDz(i)*track->GetDz(i)/cerrz);      
	sum++;
      }
    }
    for (Int_t i = 4;i<kMaxClusters;i++){
      if (track->GetClIndex(i)>0){	
	Float_t cerry, cerrz;
	if (ny[i]>0) {cerry = erry[i]; cerrz=errz[i];}
	else { cerry= track->GetSigmaY(i); cerrz = track->GetSigmaZ(i);}
	cerry*=cerry;
	cerrz*=cerrz;	
	Float_t cerryb, cerrzb;
	if (ny[i+6]>0) {cerryb = erry[i+6]; cerrzb=errz[i+6];}
	else { cerryb= track->GetSigmaY(i+6); cerrzb = track->GetSigmaZ(i+6);}
	cerryb*=cerryb;
	cerrzb*=cerrzb;
	chi2+= TMath::Min((track->GetDy(i+6)*track->GetDy(i+6)/cerryb),track->GetDy(i)*track->GetDy(i)/cerry);
	chi2+= TMath::Min((track->GetDz(i+6)*track->GetDz(i+6)/cerrzb),track->GetDz(i)*track->GetDz(i)/cerrz);      
	sum++;
      }
    }
  }
//   if (track->GetESDtrack()->GetTPCsignal()>85){
//     Float_t ratio = track->GetdEdx()/track->GetESDtrack()->GetTPCsignal();
//     if (ratio<0.5) {
//       chi2+=(0.5-ratio)*5.;      
//     }
//     if (ratio>2){
//       chi2+=(ratio-2.0)*3; 
//     }
//   }
  //
  Double_t match = TMath::Sqrt(track->GetChi22());
  if (track->GetConstrain())  match/=track->GetNumberOfClusters();
  if (!track->GetConstrain()) match/=TMath::Max(track->GetNumberOfClusters()-2.,1.);
  if (match<0) match=0;
  Float_t deadzonefactor = (track->GetNDeadZone()>0) ? 3*(1.1-track->GetDeadZoneProbability()):0.;
  Double_t normchi2 = 2*track->GetNSkipped()+match+deadzonefactor+(1+(2*track->GetNSkipped()+deadzonefactor)/track->GetNumberOfClusters())*
    (chi2)/TMath::Max(double(sum),1./(1.+track->GetNSkipped()));     
 
 return normchi2;
}


Double_t IlcTARGETtrackerMI::GetMatchingChi2(IlcTARGETtrackMI * track1, IlcTARGETtrackMI * track2)
{
  //
  // return matching chi2 between two tracks
  IlcTARGETtrackMI track3(*track2);
  track3.Propagate(track1->GetAlpha(),track1->GetX());
  TMatrixD vec(5,1);
  vec(0,0)=track1->GetY()   - track3.GetY();
  vec(1,0)=track1->GetZ()   - track3.GetZ();
  vec(2,0)=track1->GetSnp() - track3.GetSnp();
  vec(3,0)=track1->GetTgl() - track3.GetTgl();
  vec(4,0)=track1->Get1Pt() - track3.Get1Pt();
  //
  TMatrixD cov(5,5);
  cov(0,0) = track1->GetSigmaY2()+track3.GetSigmaY2();
  cov(1,1) = track1->GetSigmaZ2()+track3.GetSigmaZ2();
  cov(2,2) = track1->GetSigmaSnp2()+track3.GetSigmaSnp2();
  cov(3,3) = track1->GetSigmaTgl2()+track3.GetSigmaTgl2();
  cov(4,4) = track1->GetSigma1Pt2()+track3.GetSigma1Pt2();
  
  cov(0,1)=cov(1,0) = track1->GetSigmaZY()+track3.GetSigmaZY();
  cov(0,2)=cov(2,0) = track1->GetSigmaSnpY()+track3.GetSigmaSnpY();
  cov(0,3)=cov(3,0) = track1->GetSigmaTglY()+track3.GetSigmaTglY();
  cov(0,4)=cov(4,0) = track1->GetSigma1PtY()+track3.GetSigma1PtY();
  //
  cov(1,2)=cov(2,1) = track1->GetSigmaSnpZ()+track3.GetSigmaSnpZ();
  cov(1,3)=cov(3,1) = track1->GetSigmaTglZ()+track3.GetSigmaTglZ();
  cov(1,4)=cov(4,1) = track1->GetSigma1PtZ()+track3.GetSigma1PtZ();
  //
  cov(2,3)=cov(3,2) = track1->GetSigmaTglSnp()+track3.GetSigmaTglSnp();
  cov(2,4)=cov(4,2) = track1->GetSigma1PtSnp()+track3.GetSigma1PtSnp();
  //
  cov(3,4)=cov(4,3) = track1->GetSigma1PtTgl()+track3.GetSigma1PtTgl();
  
  cov.Invert();
  TMatrixD vec2(cov,TMatrixD::kMult,vec);
  TMatrixD chi2(vec2,TMatrixD::kTransposeMult,vec);
  return chi2(0,0);
}

Double_t  IlcTARGETtrackerMI::GetDeadZoneProbability(Double_t zpos, Double_t zerr)
{
  //
  //  return probability that given point - characterized by z position and error  is in dead zone
  //
  Double_t probability =0;
  Double_t absz = TMath::Abs(zpos);
  Double_t nearestz = (absz<2)? 0.:7.1;
  if (TMath::Abs(absz-nearestz)>0.25+3*zerr) return 0;
  Double_t zmin=0, zmax=0;   
  if (zpos<-6.){
    zmin = -7.25; zmax = -6.95; 
  }
  if (zpos>6){
    zmin = 7.0; zmax =7.3;
  }
  if (absz<2){
    zmin = -0.75; zmax = 1.5;
  }
  probability = (TMath::Erf((zpos-zmin)/zerr) - TMath::Erf((zpos-zmax)/zerr))*0.5;
  return probability;
}


Double_t IlcTARGETtrackerMI::GetTruncatedChi2(IlcTARGETtrackMI * track, Float_t fac)
{
  //
  // calculate normilczed chi2
  Float_t chi2[kMaxClusters];
  Float_t *erry = GetErrY(fCurrentEsdTrack), *errz = GetErrZ(fCurrentEsdTrack);
  Float_t ncl = 0;
  for (Int_t i = 0;i<kMaxClusters;i++){
    if (TMath::Abs(track->GetDy(i))>0){      
      chi2[i]= (track->GetDy(i)/erry[i])*(track->GetDy(i)/erry[i]);
      chi2[i]+= (track->GetDz(i)/errz[i])*(track->GetDz(i)/errz[i]);
      ncl++;
    }
    else{chi2[i]=10000;}
  }
  Int_t index[kMaxClusters];
  TMath::Sort(kMaxClusters,chi2,index,kFALSE);
  Float_t max = float(ncl)*fac-1.;
  Float_t sumchi=0, sumweight=0; 
  for (Int_t i=0;i<max+1;i++){
    Float_t weight = (i<max)?1.:(max+1.-i);
    sumchi+=weight*chi2[index[i]];
    sumweight+=weight;
  }
  Double_t normchi2 = sumchi/sumweight;
  return normchi2;
}


Double_t IlcTARGETtrackerMI::GetInterpolatedChi2(IlcTARGETtrackMI * forwardtrack, IlcTARGETtrackMI * backtrack)
{
  //
  // calculate normilczed chi2
  //  if (forwardtrack->fNUsed>0.3*float(forwardtrack->GetNumberOfClusters())) return 10000;
  Int_t npoints = 0;
  Double_t res =0;
  for (Int_t i=0;i<kMaxClusters;i++){
    if ( (backtrack->GetSigmaY(i)<0.000000001) || (forwardtrack->GetSigmaY(i)<0.000000001)) continue;
    Double_t sy1 = forwardtrack->GetSigmaY(i);
    Double_t sz1 = forwardtrack->GetSigmaZ(i);
    Double_t sy2 = backtrack->GetSigmaY(i);
    Double_t sz2 = backtrack->GetSigmaZ(i);
    if (i<2){ sy2=1000.;sz2=1000;}
    //    
    Double_t dy0 = (forwardtrack->GetDy(i)/(sy1*sy1) +backtrack->GetDy(i)/(sy2*sy2))/(1./(sy1*sy1)+1./(sy2*sy2));
    Double_t dz0 = (forwardtrack->GetDz(i)/(sz1*sz1) +backtrack->GetDz(i)/(sz2*sz2))/(1./(sz1*sz1)+1./(sz2*sz2));
    // 
    Double_t nz0 = dz0*TMath::Sqrt((1./(sz1*sz1)+1./(sz2*sz2)));
    Double_t ny0 = dy0*TMath::Sqrt((1./(sy1*sy1)+1./(sy2*sy2)));
    //
    res+= nz0*nz0+ny0*ny0;
    npoints++;
  }
  if (npoints>1) return 
		   TMath::Max(TMath::Abs(0.3*forwardtrack->Get1Pt())-0.5,0.)+
		   //2*forwardtrack->fNUsed+
		   res/TMath::Max(double(npoints-0*forwardtrack->GetNSkipped()),
				  1./(1.+forwardtrack->GetNSkipped()));
  return 1000;
}
   




Float_t  *IlcTARGETtrackerMI::GetWeight(Int_t index) {
  //--------------------------------------------------------------------
  //       Return pointer to a given cluster
  //--------------------------------------------------------------------
  Int_t l=(index & kClusterLayerMask) >> kClusterLayerShift;
  Int_t c=(index & kClusterCountMask) >> 00;
  return fgLayers[l].GetWeight(c);
}

void IlcTARGETtrackerMI::RegisterClusterTracks(IlcTARGETtrackMI* track,Int_t id)
{
  //---------------------------------------------
  // register track to the list
  //
  if (track->GetESDtrack()->GetKinkIndex(0)!=0) return;  //don't register kink tracks
  //
  //
  for (Int_t icluster=0;icluster<track->GetNumberOfClusters();icluster++){
    Int_t index = track->GetClusterIndex(icluster);
    Int_t l=(index & kClusterLayerMask) >> kClusterLayerShift;
    Int_t c=(index & kClusterCountMask) >> 00;
    if (c>fgLayers[l].GetNumberOfClusters()) continue;
    for (Int_t itrack=0;itrack<4;itrack++){
      if (fgLayers[l].GetClusterTracks(itrack,c)<0){
	fgLayers[l].SetClusterTracks(itrack,c,id);
	break;
      }
    }
  }
}
void IlcTARGETtrackerMI::UnRegisterClusterTracks(IlcTARGETtrackMI* track, Int_t id)
{
  //---------------------------------------------
  // unregister track from the list
  for (Int_t icluster=0;icluster<track->GetNumberOfClusters();icluster++){
    Int_t index = track->GetClusterIndex(icluster);
    Int_t l=(index & kClusterLayerMask) >> kClusterLayerShift;
    Int_t c=(index & kClusterCountMask) >> 00;
    if (c>fgLayers[l].GetNumberOfClusters()) continue;
    for (Int_t itrack=0;itrack<4;itrack++){
      if (fgLayers[l].GetClusterTracks(itrack,c)==id){
	fgLayers[l].SetClusterTracks(itrack,c,-1);
      }
    }
  }
}
Float_t IlcTARGETtrackerMI::GetNumberOfSharedClusters(IlcTARGETtrackMI* track,Int_t id, Int_t list[kMaxClusters], IlcTARGETRecPoint *clist[kMaxClusters])
{
  //-------------------------------------------------------------
  //get number of shared clusters
  //-------------------------------------------------------------
  Float_t shared=0;
  for (Int_t i=0;i<kMaxClusters;i++) { list[i]=-1, clist[i]=0;}
  // mean  number of clusters
  Float_t *ny = GetNy(id), *nz = GetNz(id); 

 
  for (Int_t icluster=0;icluster<track->GetNumberOfClusters();icluster++){
    Int_t index = track->GetClusterIndex(icluster);
    Int_t l=(index & kClusterLayerMask) >> kClusterLayerShift;
    Int_t c=(index & kClusterCountMask) >> 00;
    if (c>fgLayers[l].GetNumberOfClusters()) continue;
    if (ny[l]==0){
      printf("problem\n");
    }
    IlcTARGETRecPoint *cl = (IlcTARGETRecPoint*)GetCluster(index);
    Float_t weight=1;
    //
    Float_t deltan = 0;
    if (l>3&&cl->GetNy()+cl->GetNz()>6) continue;
    if (l>2&&track->GetNormQ(l)/track->GetExpQ()>3.5) continue;
    if (l<2 || l>3){      
      deltan = (cl->GetNy()+cl->GetNz()-ny[l]-nz[l]);
    }
    else{
      deltan = (cl->GetNz()-nz[l]);
    }
    if (deltan>2.0) continue;  // extended - highly probable shared cluster
    weight = 2./TMath::Max(3.+deltan,2.);
    //
    for (Int_t itrack=0;itrack<4;itrack++){
      if (fgLayers[l].GetClusterTracks(itrack,c)>=0 && fgLayers[l].GetClusterTracks(itrack,c)!=id){
	list[l]=index;
	clist[l] = (IlcTARGETRecPoint*)GetCluster(index);
	shared+=weight; 
	break;
      }
    }
  }
  track->SetNUsed(shared);
  return shared;
}

Int_t IlcTARGETtrackerMI::GetOverlapTrack(IlcTARGETtrackMI *track, Int_t trackID, Int_t &shared, Int_t clusterlist[kMaxClusters],Int_t overlist[kMaxClusters])
{
  //
  // find first shared track 
  //
  // mean  number of clusters
  Float_t *ny = GetNy(trackID), *nz = GetNz(trackID); 
  //
  for (Int_t i=0;i<kMaxClusters;i++) overlist[i]=-1;
  Int_t sharedtrack=100000;
  Int_t tracks[kMaxClusters*4],trackindex=0;
  for (Int_t i=0;i<kMaxClusters*4;i++) {tracks[i]=-1;}
  //
  for (Int_t icluster=0;icluster<kMaxClusters;icluster++){
    if (clusterlist[icluster]<0) continue;
    Int_t index = clusterlist[icluster];
    Int_t l=(index & kClusterLayerMask) >> kClusterLayerShift;
    Int_t c=(index & kClusterCountMask) >> 00;
    if (ny[l]==0){
      printf("problem\n");
    }
    if (c>fgLayers[l].GetNumberOfClusters()) continue;
    //if (l>3) continue;
    IlcTARGETRecPoint *cl = (IlcTARGETRecPoint*)GetCluster(index);
    //
    Float_t deltan = 0;
    if (l>3&&cl->GetNy()+cl->GetNz()>6) continue;
    if (l>2&&track->GetNormQ(l)/track->GetExpQ()>3.5) continue;
    if (l<2 || l>3){      
      deltan = (cl->GetNy()+cl->GetNz()-ny[l]-nz[l]);
    }
    else{
      deltan = (cl->GetNz()-nz[l]);
    }
    if (deltan>2.0) continue;  // extended - highly probable shared cluster
    //
    for (Int_t itrack=3;itrack>=0;itrack--){
      if (fgLayers[l].GetClusterTracks(itrack,c)<0) continue;
      if (fgLayers[l].GetClusterTracks(itrack,c)!=trackID){
       tracks[trackindex]  = fgLayers[l].GetClusterTracks(itrack,c);
       trackindex++;
      }
    }
  }
  if (trackindex==0) return -1;
  if (trackindex==1){    
    sharedtrack = tracks[0];
  }else{
    if (trackindex==2) sharedtrack =TMath::Min(tracks[0],tracks[1]);
    else{
      //
      Int_t atrack[kMaxClusters*4], cluster[kMaxClusters*4];
      for (Int_t i=0;i<trackindex;i++){ atrack[i]=-1; cluster[i]=0;}
      Int_t index =0;
      //
      for (Int_t i=0;i<trackindex;i++){
	if (tracks[i]<0) continue;
        atrack[index] = tracks[i];
	cluster[index]++;	
	for (Int_t j=i+1;j<trackindex;j++){
	  if (tracks[j]<0) continue;
	  if (tracks[j]==tracks[i]){
	    cluster[index]++;
	    tracks[j]=-1;
	  }
	}
	index++;
      }
      Int_t max=0;
      for (Int_t i=0;i<index;i++){
	if (cluster[index]>max) {
          sharedtrack=atrack[index];
	  max=cluster[index];
	}
      }
    }
  }
  
  if (sharedtrack>=100000) return -1;
  //
  // make list of overlaps
  shared =0;
  for (Int_t icluster=0;icluster<kMaxClusters;icluster++){
    if (clusterlist[icluster]<0) continue;
    Int_t index = clusterlist[icluster];
    Int_t l=(index & kClusterLayerMask) >> kClusterLayerShift;
    Int_t c=(index & kClusterCountMask) >> 00;
    if (c>fgLayers[l].GetNumberOfClusters()) continue;
    IlcTARGETRecPoint *cl = (IlcTARGETRecPoint*)GetCluster(index);
    if (l==0 || l==1){
      if (cl->GetNy()>2) continue;
      if (cl->GetNz()>2) continue;
    }
    if (l==4 || l==5){
      if (cl->GetNy()>3) continue;
      if (cl->GetNz()>3) continue;
    }
    //
    for (Int_t itrack=3;itrack>=0;itrack--){
      if (fgLayers[l].GetClusterTracks(itrack,c)<0) continue;
      if (fgLayers[l].GetClusterTracks(itrack,c)==sharedtrack){
	overlist[l]=index;
	shared++;      
      }
    }
  }
  return sharedtrack;
}


IlcTARGETtrackMI *  IlcTARGETtrackerMI::GetBest2Tracks(Int_t trackID1, Int_t trackID2, Float_t th0, Float_t th1){
  //
  // try to find track hypothesys without conflicts
  // with minimal chi2;
  TClonesArray *arr1 = (TClonesArray*)fTrackHypothesys.At(trackID1);
  Int_t entries1 = arr1->GetEntriesFast();
  TClonesArray *arr2 = (TClonesArray*)fTrackHypothesys.At(trackID2);
  if (!arr2) return (IlcTARGETtrackMI*) arr1->UncheckedAt(0);
  Int_t entries2 = arr2->GetEntriesFast();
  if (entries2<=0) return (IlcTARGETtrackMI*) arr1->UncheckedAt(0);
  //
  IlcTARGETtrackMI * track10=(IlcTARGETtrackMI*) arr1->UncheckedAt(0);
  IlcTARGETtrackMI * track20=(IlcTARGETtrackMI*) arr2->UncheckedAt(0);
  if (TMath::Abs(1./track10->Get1Pt())>0.5+TMath::Abs(1/track20->Get1Pt())) return track10;

  for (Int_t itrack=0;itrack<entries1;itrack++){
    IlcTARGETtrackMI * track=(IlcTARGETtrackMI*) arr1->UncheckedAt(itrack);
    UnRegisterClusterTracks(track,trackID1);
  }
  //
  for (Int_t itrack=0;itrack<entries2;itrack++){
    IlcTARGETtrackMI * track=(IlcTARGETtrackMI*) arr2->UncheckedAt(itrack);
    UnRegisterClusterTracks(track,trackID2);
  }
  Int_t index1=0;
  Int_t index2=0;
  Float_t maxconflicts=6;
  Double_t maxchi2 =1000.;
  //
  // get weight of hypothesys - using best hypothesy
  Double_t w1,w2;
 
  Int_t list1[kMaxClusters],list2[kMaxClusters];
  IlcTARGETRecPoint *clist1[kMaxClusters], *clist2[kMaxClusters] ;
  RegisterClusterTracks(track10,trackID1);
  RegisterClusterTracks(track20,trackID2);
  Float_t conflict1 = GetNumberOfSharedClusters(track10,trackID1,list1,clist1);
  Float_t conflict2 = GetNumberOfSharedClusters(track20,trackID2,list2,clist2);
  UnRegisterClusterTracks(track10,trackID1);
  UnRegisterClusterTracks(track20,trackID2);
  //
  // normilczed chi2
  Float_t chi21 =0,chi22=0,ncl1=0,ncl2=0;
  Float_t nerry[kMaxClusters],nerrz[kMaxClusters];
  Float_t *erry1=GetErrY(trackID1),*errz1 = GetErrZ(trackID1);
  Float_t *erry2=GetErrY(trackID2),*errz2 = GetErrZ(trackID2);
  for (Int_t i=0;i<kMaxClusters;i++){
     if ( (erry1[i]>0) && (erry2[i]>0)) {
       nerry[i] = TMath::Min(erry1[i],erry2[i]);
       nerrz[i] = TMath::Min(errz1[i],errz2[i]);
     }else{
       nerry[i] = TMath::Max(erry1[i],erry2[i]);
       nerrz[i] = TMath::Max(errz1[i],errz2[i]);
     }
     if (TMath::Abs(track10->GetDy(i))>0.000000000000001){
       chi21 += track10->GetDy(i)*track10->GetDy(i)/(nerry[i]*nerry[i]);
       chi21 += track10->GetDz(i)*track10->GetDz(i)/(nerrz[i]*nerrz[i]);
       ncl1++;
     }
     if (TMath::Abs(track20->GetDy(i))>0.000000000000001){
       chi22 += track20->GetDy(i)*track20->GetDy(i)/(nerry[i]*nerry[i]);
       chi22 += track20->GetDz(i)*track20->GetDz(i)/(nerrz[i]*nerrz[i]);
       ncl2++;
     }
  }
  chi21/=ncl1;
  chi22/=ncl2;
  //
  // 
  Float_t d1 = TMath::Sqrt(track10->GetD(0)*track10->GetD(0)+track10->GetD(1)*track10->GetD(1))+0.1;
  Float_t d2 = TMath::Sqrt(track20->GetD(0)*track20->GetD(0)+track20->GetD(1)*track20->GetD(1))+0.1;
  Float_t s1 = TMath::Sqrt(track10->GetSigmaY2()*track10->GetSigmaZ2());
  Float_t s2 = TMath::Sqrt(track20->GetSigmaY2()*track20->GetSigmaZ2());
  //
  w1 = (d2/(d1+d2)+ 2*s2/(s1+s2)+
	+s2/(s1+s2)*0.5*(chi22+2.)/(chi21+chi22+4.)
	+1.*TMath::Abs(1./track10->Get1Pt())/(TMath::Abs(1./track10->Get1Pt())+TMath::Abs(1./track20->Get1Pt()))
	);
  w2 = (d1/(d1+d2)+ 2*s1/(s1+s2)+
	s1/(s1+s2)*0.5*(chi21+2.)/(chi21+chi22+4.)
	+1.*TMath::Abs(1./track20->Get1Pt())/(TMath::Abs(1./track10->Get1Pt())+TMath::Abs(1./track20->Get1Pt()))
	);

  Double_t sumw = w1+w2;
  w1/=sumw;
  w2/=sumw;
  if (w1<w2*0.5) {
    w1 = (d2+0.5)/(d1+d2+1.);
    w2 = (d1+0.5)/(d1+d2+1.);
  }
  //  Float_t maxmax       = w1*track10->fChi2MIP[0]+w2*track20->fChi2MIP[0]+w1*conflict1+w2*conflict2+1.;
  //Float_t maxconflicts0 = w1*conflict1+w2*conflict2;
  //
  // get pair of "best" hypothesys
  //  
  Float_t * ny1 = GetNy(trackID1), * nz1 = GetNz(trackID1); 
  Float_t * ny2 = GetNy(trackID2), * nz2 = GetNz(trackID2); 

  for (Int_t itrack1=0;itrack1<entries1;itrack1++){
    IlcTARGETtrackMI * track1=(IlcTARGETtrackMI*) arr1->UncheckedAt(itrack1);
    //if (track1->fFakeRatio>0) continue;
    RegisterClusterTracks(track1,trackID1);
    for (Int_t itrack2=0;itrack2<entries2;itrack2++){
      IlcTARGETtrackMI * track2=(IlcTARGETtrackMI*) arr2->UncheckedAt(itrack2);

      //      Float_t current = w1*track1->fChi2MIP[0]+w2*track2->fChi2MIP[0];
      //if (track2->fFakeRatio>0) continue;
      Float_t nskipped=0;            
      RegisterClusterTracks(track2,trackID2);
//       Int_t list1[kMaxClusters],list2[kMaxClusters];
//       IlcTARGETRecPoint *clist1[kMaxClusters], *clist2[kMaxClusters] ;
      Float_t cconflict1 = GetNumberOfSharedClusters(track1,trackID1,list1,clist1);
      Float_t cconflict2 = GetNumberOfSharedClusters(track2,trackID2,list2,clist2);
      UnRegisterClusterTracks(track2,trackID2);
      //
      if (track1->GetConstrain()) nskipped+=w1*track1->GetNSkipped();
      if (track2->GetConstrain()) nskipped+=w2*track2->GetNSkipped();
      if (nskipped>0.5) continue;
      //
      //if ( w1*conflict1+w2*conflict2>maxconflicts0) continue;
      if (conflict1+1<cconflict1) continue;
      if (conflict2+1<cconflict2) continue;
      Float_t conflict=0;
      Float_t sumchi2=0;
      Float_t sum=0;
      for (Int_t i=0;i<kMaxClusters;i++){
	//
	Float_t c1 =0.; // conflict coeficients
	Float_t c2 =0.; 
	if (clist1[i]&&clist2[i]){
	  Float_t deltan = 0;
	  if (i<2 || i>3){      
	    deltan = (clist1[i]->GetNy()+clist1[i]->GetNz()-TMath::Max(ny1[i],ny2[i])-TMath::Max(nz1[i],nz2[i]));
	  }
	  else{
	    deltan = (clist1[i]->GetNz()-TMath::Max(nz1[i],nz2[i]));
	  }
	  c1  = 2./TMath::Max(3.+deltan,2.);	  
	  c2  = 2./TMath::Max(3.+deltan,2.);	  
	}
	else{
	  if (clist1[i]){
	    Float_t deltan = 0;
	    if (i<2 || i>3){      
	      deltan = (clist1[i]->GetNy()+clist1[i]->GetNz()-ny1[i]-nz1[i]);
	    }
	    else{
	      deltan = (clist1[i]->GetNz()-nz1[i]);
	    }
	    c1  = 2./TMath::Max(3.+deltan,2.);	  
	    c2  = 0;
	  }

	  if (clist2[i]){
	    Float_t deltan = 0;
	    if (i<2 || i>3){      
	      deltan = (clist2[i]->GetNy()+clist2[i]->GetNz()-ny2[i]-nz2[i]);
	    }
	    else{
	      deltan = (clist2[i]->GetNz()-nz2[i]);
	    }
	    c2  = 2./TMath::Max(3.+deltan,2.);	  
	    c1  = 0;
	  }	  
	}
	//
	chi21=0,chi22=0;
	if (TMath::Abs(track1->GetDy(i))>0.) {
	  chi21 = (track1->GetDy(i)/track1->GetSigmaY(i))*(track1->GetDy(i)/track1->GetSigmaY(i))+
	    (track1->GetDz(i)/track1->GetSigmaZ(i))*(track1->GetDz(i)/track1->GetSigmaZ(i));
	  //chi21 = (track1->fDy[i]*track1->fDy[i])/(nerry[i]*nerry[i])+
	  //  (track1->GetDz(i)*track1->GetDz(i))/(nerrz[i]*nerrz[i]);
	}else{
	  if (TMath::Abs(track1->GetSigmaY(i)>0.)) c1=1;
	}
	//
	if (TMath::Abs(track2->GetDy(i))>0.) {
	  chi22 = (track2->GetDy(i)/track2->GetSigmaY(i))*(track2->GetDy(i)/track2->GetSigmaY(i))+
	    (track2->GetDz(i)/track2->GetSigmaZ(i))*(track2->GetDz(i)/track2->GetSigmaZ(i));
	  //chi22 = (track2->fDy[i]*track2->fDy[i])/(nerry[i]*nerry[i])+
	  //  (track2->fDz[i]*track2->fDz[i])/(nerrz[i]*nerrz[i]);
	}
	else{
	  if (TMath::Abs(track2->GetSigmaY(i)>0.)) c2=1;
	}
	sumchi2+=w1*(1.+c1)*(1+c1)*(chi21+c1)+w2*(1.+c2)*(1+c2)*(chi22+c2);
	if (chi21>0) sum+=w1;
	if (chi22>0) sum+=w2;
	conflict+=(c1+c2);
      }
      Double_t norm = sum-w1*track1->GetNSkipped()-w2*track2->GetNSkipped();
      if (norm<0) norm =1/(w1*track1->GetNSkipped()+w2*track2->GetNSkipped());
      Double_t normchi2 = 2*conflict+sumchi2/sum;
      if ( normchi2 <maxchi2 ){	  
	index1 = itrack1;
	index2 = itrack2;
	maxconflicts = conflict;
	maxchi2 = normchi2;
      }      
    }
    UnRegisterClusterTracks(track1,trackID1);
  }
  //
  //  if (maxconflicts<4 && maxchi2<th0){   
  if (maxchi2<th0*2.){   
    Float_t orig = track10->GetFakeRatio()*track10->GetNumberOfClusters();
    IlcTARGETtrackMI* track1=(IlcTARGETtrackMI*) arr1->UncheckedAt(index1);
    track1->SetChi2MIP(5,maxconflicts);
    track1->SetChi2MIP(6,maxchi2);
    track1->SetChi2MIP(7,0.01+orig-(track1->GetFakeRatio()*track1->GetNumberOfClusters()));
    //    track1->UpdateESDtrack(IlcESDtrack::kVXDin);
    track1->SetChi2MIP(8,index1);
    fBestTrackIndex[trackID1] =index1;
    UpdateESDtrack(track1, IlcESDtrack::kVXDin);
  }  
  else if (track10->GetChi2MIP(0)<th1){
    track10->SetChi2MIP(5,maxconflicts);
    track10->SetChi2MIP(6,maxchi2);    
    //    track10->UpdateESDtrack(IlcESDtrack::kVXDin);
    UpdateESDtrack(track10,IlcESDtrack::kVXDin);
  }   
  
  for (Int_t itrack=0;itrack<entries1;itrack++){
    IlcTARGETtrackMI * track=(IlcTARGETtrackMI*) arr1->UncheckedAt(itrack);
    UnRegisterClusterTracks(track,trackID1);
  }
  //
  for (Int_t itrack=0;itrack<entries2;itrack++){
    IlcTARGETtrackMI * track=(IlcTARGETtrackMI*) arr2->UncheckedAt(itrack);
    UnRegisterClusterTracks(track,trackID2);
  }

  if (track10->GetConstrain()&&track10->GetChi2MIP(0)<kMaxChi2PerCluster[0]&&track10->GetChi2MIP(1)<kMaxChi2PerCluster[1]
      &&track10->GetChi2MIP(2)<kMaxChi2PerCluster[2]&&track10->GetChi2MIP(3)<kMaxChi2PerCluster[3]){ 
    //  if (track10->fChi2MIP[0]<kMaxChi2PerCluster[0]&&track10->fChi2MIP[1]<kMaxChi2PerCluster[1]
  //    &&track10->fChi2MIP[2]<kMaxChi2PerCluster[2]&&track10->fChi2MIP[3]<kMaxChi2PerCluster[3]){ 
    RegisterClusterTracks(track10,trackID1);
  }
  if (track20->GetConstrain()&&track20->GetChi2MIP(0)<kMaxChi2PerCluster[0]&&track20->GetChi2MIP(1)<kMaxChi2PerCluster[1]
      &&track20->GetChi2MIP(2)<kMaxChi2PerCluster[2]&&track20->GetChi2MIP(3)<kMaxChi2PerCluster[3]){ 
    //if (track20->fChi2MIP[0]<kMaxChi2PerCluster[0]&&track20->fChi2MIP[1]<kMaxChi2PerCluster[1]
    //  &&track20->fChi2MIP[2]<kMaxChi2PerCluster[2]&&track20->fChi2MIP[3]<kMaxChi2PerCluster[3]){ 
    RegisterClusterTracks(track20,trackID2);  
  }
  return track10; 
 
}

void IlcTARGETtrackerMI::UseClusters(const IlcKalmanTrack *t, Int_t from) const {
  //--------------------------------------------------------------------
  // This function marks clusters assigned to the track
  //--------------------------------------------------------------------
  IlcTracker::UseClusters(t,from);

  IlcTARGETRecPoint *c=(IlcTARGETRecPoint *)GetCluster(t->GetClusterIndex(0));
  //if (c->GetQ()>2) c->Use();
  if (c->GetSigmaZ2()>0.1) c->Use();
  c=(IlcTARGETRecPoint *)GetCluster(t->GetClusterIndex(1));
  //if (c->GetQ()>2) c->Use();
  if (c->GetSigmaZ2()>0.1) c->Use();

}


void IlcTARGETtrackerMI::AddTrackHypothesys(IlcTARGETtrackMI * track, Int_t esdindex)
{
  //------------------------------------------------------------------
  // add track to the list of hypothesys
  //------------------------------------------------------------------

  if (esdindex>=fTrackHypothesys.GetEntriesFast()) fTrackHypothesys.Expand(esdindex*2+10);
  //
  TObjArray * array = (TObjArray*) fTrackHypothesys.At(esdindex);
  if (!array) {
    array = new TObjArray(10);
    fTrackHypothesys.AddAt(array,esdindex);
  }
  array->AddLast(track);
}

void IlcTARGETtrackerMI::SortTrackHypothesys(Int_t esdindex, Int_t maxcut, Int_t mode)
{
  //-------------------------------------------------------------------
  // compress array of track hypothesys
  // keep only maxsize best hypothesys
  //-------------------------------------------------------------------
  if (esdindex>fTrackHypothesys.GetEntriesFast()) return;
  if (! (fTrackHypothesys.At(esdindex)) ) return;
  TObjArray * array = (TObjArray*) fTrackHypothesys.At(esdindex);
  Int_t entries = array->GetEntriesFast();
  //
  //- find preliminary besttrack as a reference
  Float_t minchi2=10000;
  Int_t maxn=0;
  IlcTARGETtrackMI * besttrack=0;
  for (Int_t itrack=0;itrack<array->GetEntriesFast();itrack++){
    IlcTARGETtrackMI * track = (IlcTARGETtrackMI*)array->At(itrack);
    if (!track) continue;
    Float_t chi2 = NormilczedChi2(track,0);
    //
    Int_t tpcLabel=track->GetESDtrack()->GetTPCLabel();
    track->SetLabel(tpcLabel);
    CookdEdx(track);
    track->SetFakeRatio(1.);
    CookLabel(track,0.); //For comparison only
    //
    //if (chi2<kMaxChi2PerCluster[0]&&track->fFakeRatio==0){
    if (chi2<kMaxChi2PerCluster[0]){
      if (track->GetNumberOfClusters()<maxn) continue;
      maxn = track->GetNumberOfClusters();
      if (chi2<minchi2){
	minchi2=chi2;
	besttrack=track;
      }
    }
    else{
      if (track->GetConstrain() || track->GetNumberOfClusters()>5){  //keep best short tracks - without vertex constrain
	delete array->RemoveAt(itrack);
      }	 
    }
  }
  if (!besttrack) return;
  //
  //
  //take errors of best track as a reference
  Float_t *erry = GetErrY(esdindex), *errz = GetErrZ(esdindex);
  Float_t *ny = GetNy(esdindex), *nz = GetNz(esdindex);
  for (Int_t i=0;i<kMaxClusters;i++) {
    if (besttrack->GetClIndex(i)>0){
      erry[i] = besttrack->GetSigmaY(i); //erry[i+6] = besttrack->GetSigmaY(i+6);
      errz[i] = besttrack->GetSigmaZ(i); //errz[i+6] = besttrack->GetSigmaZ(i+6);
      ny[i]   = besttrack->GetNy(i);
      nz[i]   = besttrack->GetNz(i);
    }
  }
  //
  // calculate normilczed chi2
  //
  Float_t * chi2        = new Float_t[entries];
  Int_t * index         = new Int_t[entries];  
  for (Int_t i=0;i<entries;i++) chi2[i] =10000;
  for (Int_t itrack=0;itrack<entries;itrack++){
    IlcTARGETtrackMI * track = (IlcTARGETtrackMI*)array->At(itrack);
    if (track){
      track->SetChi2MIP(0,GetNormilczedChi2(track, mode));            
      if (track->GetChi2MIP(0)<kMaxChi2PerCluster[0]) 
	chi2[itrack] = track->GetChi2MIP(0);
      else{
	if (track->GetConstrain() || track->GetNumberOfClusters()>5){  //keep best short tracks - without vertex constrain
	  delete array->RemoveAt(itrack);	     
	}
      }
    }
  }
  //
  TMath::Sort(entries,chi2,index,kFALSE);
  besttrack = (IlcTARGETtrackMI*)array->At(index[0]);
  if (besttrack&&besttrack->GetChi2MIP(0)<kMaxChi2PerCluster[0]){
    for (Int_t i=0;i<kMaxClusters;i++){
      if (besttrack->GetClIndex(i)>0){
	erry[i] = besttrack->GetSigmaY(i); //erry[i+6] = besttrack->GetSigmaY(i+6);
	errz[i] = besttrack->GetSigmaZ(i); //erry[i+6] = besttrack->GetSigmaY(i+6);
	ny[i]   = besttrack->GetNy(i);
	nz[i]   = besttrack->GetNz(i);
      }
    }
  }
  //
  // calculate one more time with updated normilczed errors
  for (Int_t i=0;i<entries;i++) chi2[i] =10000;  
  for (Int_t itrack=0;itrack<entries;itrack++){
    IlcTARGETtrackMI * track = (IlcTARGETtrackMI*)array->At(itrack);
    if (track){      
      track->SetChi2MIP(0,GetNormilczedChi2(track,mode));            
      if (track->GetChi2MIP(0)<kMaxChi2PerCluster[0]) 
	chi2[itrack] = track->GetChi2MIP(0)-0*(track->GetNumberOfClusters()+track->GetNDeadZone()); 
      else
	{
	  if (track->GetConstrain() || track->GetNumberOfClusters()>5){  //keep best short tracks - without vertex constrain
	    delete array->RemoveAt(itrack);	
	  }
	}
    }   
  }
  entries = array->GetEntriesFast();  
  //
  //
  if (entries>0){
    TObjArray * newarray = new TObjArray();  
    TMath::Sort(entries,chi2,index,kFALSE);
    besttrack = (IlcTARGETtrackMI*)array->At(index[0]);
    if (besttrack){
      //
      for (Int_t i=0;i<kMaxClusters;i++){
	if (besttrack->GetNz(i)>0&&besttrack->GetNy(i)>0){
	  erry[i] = besttrack->GetSigmaY(i); //erry[i+6] = besttrack->GetSigmaY(i+6);
	  errz[i] = besttrack->GetSigmaZ(i); //errz[i+6] = besttrack->GetSigmaZ(i+6);
	  ny[i]   = besttrack->GetNy(i);
	  nz[i]   = besttrack->GetNz(i);
	}
      }
      besttrack->SetChi2MIP(0,GetNormilczedChi2(besttrack,mode));
      minchi2 = TMath::Min(besttrack->GetChi2MIP(0)+5.+besttrack->GetNUsed(), double(kMaxChi2PerCluster[0]));
      Float_t minn = besttrack->GetNumberOfClusters()-3;
      Int_t accepted=0;
      for (Int_t i=0;i<entries;i++){
	IlcTARGETtrackMI * track = (IlcTARGETtrackMI*)array->At(index[i]);	
	if (!track) continue;
	if (accepted>maxcut) break;
	track->SetChi2MIP(0,GetNormilczedChi2(track,mode));
	if (track->GetConstrain() || track->GetNumberOfClusters()>5){  //keep best short tracks - without vertex constrain
	  if (track->GetNumberOfClusters()<5 && (track->GetChi2MIP(0)+track->GetNUsed()>minchi2)){
	    delete array->RemoveAt(index[i]);
	    continue;
	  }
	}
	Bool_t shortbest = !track->GetConstrain() && track->GetNumberOfClusters()<5;
	if ((track->GetChi2MIP(0)+track->GetNUsed()<minchi2 && track->GetNumberOfClusters()>=minn) ||shortbest){
	  if (!shortbest) accepted++;
	  //
	  newarray->AddLast(array->RemoveAt(index[i]));      
	  for (Int_t ii=0;ii<kMaxClusters;ii++){
	    if (nz[ii]==0){
	      erry[ii] = track->GetSigmaY(ii); //erry[i+6] = track->GetSigmaY(i+6);
	      errz[ii] = track->GetSigmaZ(ii); //errz[i+6]   = track->GetSigmaZ(i+6);
	      ny[ii]   = track->GetNy(ii);
	      nz[ii]   = track->GetNz(ii);
	    }
	  }
	}
	else{
	  delete array->RemoveAt(index[i]);
	}
      }
      array->Delete();
      delete fTrackHypothesys.RemoveAt(esdindex);
      fTrackHypothesys.AddAt(newarray,esdindex);
    }
    else{
      array->Delete();
      delete fTrackHypothesys.RemoveAt(esdindex);
    }
  }
  delete [] chi2;
  delete [] index;
}



IlcTARGETtrackMI * IlcTARGETtrackerMI::GetBestHypothesys(Int_t esdindex, IlcTARGETtrackMI * original, Int_t checkmax)
{
  //-------------------------------------------------------------
  // try to find best hypothesy
  // currently - minimal chi2 of track+backpropagated track+matching to the tpc track
  //-------------------------------------------------------------
  if (fTrackHypothesys.GetEntriesFast()<=esdindex) return 0;
  TObjArray * array = (TObjArray*) fTrackHypothesys.At(esdindex);
  if (!array) return 0;
  Int_t entries = array->GetEntriesFast();
  if (!entries) return 0;  
  Float_t minchi2 = 100000;
  IlcTARGETtrackMI * besttrack=0;
  //
  IlcTARGETtrackMI * backtrack    = new IlcTARGETtrackMI(*original);
  IlcTARGETtrackMI * forwardtrack = new IlcTARGETtrackMI(*original);
  Double_t xyzv[]={GetX(),GetY(),GetZ()};	
  Double_t ersv[]={GetSigmaX()/3.,GetSigmaY()/3.,GetSigmaZ()/3.};
  //
  for (Int_t i=0;i<entries;i++){    
    IlcTARGETtrackMI * track = (IlcTARGETtrackMI*)array->At(i);    
    if (!track) continue;
    Float_t sigmarfi,sigmaz;
    GetDCASigma(track,sigmarfi,sigmaz);
    track->SetDnorm(0,sigmarfi);
    track->SetDnorm(1,sigmaz);
    //
    track->SetChi2MIP(1,1000000);
    track->SetChi2MIP(2,1000000);
    track->SetChi2MIP(3,1000000);
    //
    // backtrack
    backtrack = new(backtrack) IlcTARGETtrackMI(*track); 
    if (track->GetConstrain()){
      if (!backtrack->PropagateTo(1.1,0.0018,47.22)) continue;
      if (!backtrack->Improve(0,xyzv,ersv))         continue;      
//       if (!backtrack->PropagateTo(1.,0.0018,0))     continue;
//       if (!backtrack->Improve(0,xyzv,ersv))         continue;
//       if (!backtrack->PropagateTo(.9,0.0018,0))     continue;
//       if (!backtrack->Improve(0,xyzv,ersv))         continue;            	  
//       if (!backtrack->PropagateToVertex())          continue;
      backtrack->ResetCovariance(10.);      
      //      if (!backtrack->Improve(0,xyzv,ersv))         continue;            	        
    }else{
      backtrack->ResetCovariance(10.);
    }
    backtrack->ResetClusters();

    Double_t x = original->GetX();
    if (!RefitAt(x,backtrack,track)) continue;
    //
    track->SetChi2MIP(1,NormilczedChi2(backtrack,0));
    //for (Int_t i=2;i<6;i++){track->fDy[i]+=backtrack->fDy[i]; track->fDz[i]+=backtrack->fDz[i];}
    if (track->GetChi2MIP(1)>kMaxChi2PerCluster[1]*6.)  continue;
    track->SetChi22(GetMatchingChi2(backtrack,original));

    if ((track->GetConstrain()) && track->GetChi22()>90.)  continue;
    if ((!track->GetConstrain()) && track->GetChi22()>30.)  continue;
    if ( track->GetChi22()/track->GetNumberOfClusters()>11.)  continue;


    if  (!(track->GetConstrain())&&track->GetChi2MIP(1)>kMaxChi2PerCluster[1])  continue;
    Bool_t isOK=kTRUE;
    if(!isOK) continue;
    //
    //forward track - without constraint
    forwardtrack = new(forwardtrack) IlcTARGETtrackMI(*original);
    forwardtrack->ResetClusters();
    x = track->GetX();
    RefitAt(x,forwardtrack,track);
    track->SetChi2MIP(2,NormilczedChi2(forwardtrack,0));    
    if  (track->GetChi2MIP(2)>kMaxChi2PerCluster[2]*6.0)  continue;
    if  (!(track->GetConstrain())&&track->GetChi2MIP(2)>kMaxChi2PerCluster[2])  continue;
    
    //track->fD[0] = forwardtrack->GetD(GetX(),GetY());
    //track->fD[1] = forwardtrack->GetZat(GetX())-GetZ();
    forwardtrack->GetDZ(GetX(),GetY(),GetZ(),track->GetDP());   //I.B.
    forwardtrack->SetD(0,track->GetD(0));
    forwardtrack->SetD(1,track->GetD(1));    
    {
      Int_t list[kMaxClusters];
      IlcTARGETRecPoint* clist[kMaxClusters];
      track->SetChi2MIP(4,GetNumberOfSharedClusters(track,esdindex,list,clist));      
      if ( (!track->GetConstrain()) && track->GetChi2MIP(4)>1.0) continue;
    }
    
    track->SetChi2MIP(3,GetInterpolatedChi2(forwardtrack,backtrack));
    if  ( (track->GetChi2MIP(3)>6.*kMaxChi2PerCluster[3])) continue;    
    if  ( (!track->GetConstrain()) && (track->GetChi2MIP(3)>2*kMaxChi2PerCluster[3])) {
      track->SetChi2MIP(3,1000);
      continue; 
    }
    Double_t chi2 = track->GetChi2MIP(0)+track->GetNUsed();    
    //
    for (Int_t ichi=0;ichi<5;ichi++){
      forwardtrack->SetChi2MIP(ichi, track->GetChi2MIP(ichi));
    }
    if (chi2 < minchi2){
      //besttrack = new IlcTARGETtrackMI(*forwardtrack);
      besttrack = track;
      besttrack->SetLabel(track->GetLabel());
      besttrack->SetFakeRatio(track->GetFakeRatio());
      minchi2   = chi2;
      //original->fD[0] = forwardtrack->GetD(GetX(),GetY());
      //original->fD[1] = forwardtrack->GetZat(GetX())-GetZ();
      forwardtrack->GetDZ(GetX(),GetY(),GetZ(),original->GetDP());    //I.B.
    }    
  }
  delete backtrack;
  delete forwardtrack;
  Int_t accepted=0;
  for (Int_t i=0;i<entries;i++){    
    IlcTARGETtrackMI * track = (IlcTARGETtrackMI*)array->At(i);   
    if (!track) continue;
    
    if (besttrack&&accepted>checkmax || track->GetChi2MIP(3)>kMaxChi2PerCluster[3]*6. || 
	(track->GetNumberOfClusters()<besttrack->GetNumberOfClusters()-1.)||
	track->GetChi2MIP(0)>besttrack->GetChi2MIP(0)+2.*besttrack->GetNUsed()+3.){
      if (track->GetConstrain() || track->GetNumberOfClusters()>5){  //keep best short tracks - without vertex constrain
	delete array->RemoveAt(i);    
	continue;
      }
    }
    else{
      accepted++;
    }
  }
  //
  array->Compress();
  SortTrackHypothesys(esdindex,checkmax,1);
  array = (TObjArray*) fTrackHypothesys.At(esdindex);
  if (!array) return 0; // PH What can be the reason? Check SortTrackHypothesys
  besttrack = (IlcTARGETtrackMI*)array->At(0);  
  if (!besttrack)  return 0;
  besttrack->SetChi2MIP(8,0);
  fBestTrackIndex[esdindex]=0;
  entries = array->GetEntriesFast();
  IlcTARGETtrackMI *longtrack =0;
  minchi2 =1000;
  Float_t minn=besttrack->GetNumberOfClusters()+besttrack->GetNDeadZone();
  for (Int_t itrack=entries-1;itrack>0;itrack--){
    IlcTARGETtrackMI * track = (IlcTARGETtrackMI*)array->At(itrack);
    if (!track->GetConstrain()) continue;
    if (track->GetNumberOfClusters()+track->GetNDeadZone()<minn) continue;
    if (track->GetChi2MIP(0)-besttrack->GetChi2MIP(0)>0.0) continue;
    if (track->GetChi2MIP(0)>4.) continue;
    minn = track->GetNumberOfClusters()+track->GetNDeadZone();
    longtrack =track;
  }
  //if (longtrack) besttrack=longtrack;

  Int_t list[kMaxClusters];
  IlcTARGETRecPoint * clist[kMaxClusters];
  Float_t shared = GetNumberOfSharedClusters(besttrack,esdindex,list,clist);
  if (besttrack->GetConstrain()&&besttrack->GetChi2MIP(0)<kMaxChi2PerCluster[0]&&besttrack->GetChi2MIP(1)<kMaxChi2PerCluster[1]
      &&besttrack->GetChi2MIP(2)<kMaxChi2PerCluster[2]&&besttrack->GetChi2MIP(3)<kMaxChi2PerCluster[3]){ 
    RegisterClusterTracks(besttrack,esdindex);
  }
  //
  //
  if (shared>0.0){
    Int_t nshared;
    Int_t overlist[kMaxClusters];
    Int_t sharedtrack = GetOverlapTrack(besttrack, esdindex, nshared, list, overlist);
    if (sharedtrack>=0){
      //
      besttrack = GetBest2Tracks(esdindex,sharedtrack,10,5.5);     
      if (besttrack){
	shared = GetNumberOfSharedClusters(besttrack,esdindex,list,clist);
      }
      else return 0;
    }
  }  
  
  if (shared>2.5) return 0;
  if (shared>1.0) return besttrack;
  //
  // Don't sign clusters if not gold track
  //
  if (!besttrack->IsGoldPrimary()) return besttrack;
  if (besttrack->GetESDtrack()->GetKinkIndex(0)!=0) return besttrack;   //track belong to kink
  //
  if (fConstraint[fPass]){
    //
    // sign clusters
    //
    Float_t *ny = GetNy(esdindex), *nz = GetNz(esdindex);
    for (Int_t i=0;i<kMaxClusters;i++){
      Int_t index = besttrack->GetClIndex(i);
      if (index<=0) continue; 
      Int_t ilayer =  (index & kClusterLayerMask) >> kClusterLayerShift;
      if (besttrack->GetSigmaY(ilayer)<0.00000000001) continue;
      IlcTARGETRecPoint *c = (IlcTARGETRecPoint*)GetCluster(index);     
      if (!c) continue;
      if (ilayer>3&&c->GetNy()+c->GetNz()>6) continue;
      if ( (c->GetNy()+c->GetNz() )> ny[i]+nz[i]+0.7) continue; //shared track
      if (  c->GetNz()> nz[i]+0.7) continue; //shared track
      if ( ilayer>2&& besttrack->GetNormQ(ilayer)/besttrack->GetExpQ()>1.5) continue;
      //if (  c->GetNy()> ny[i]+0.7) continue; //shared track

      Bool_t cansign = kTRUE;
      for (Int_t itrack=0;itrack<entries; itrack++){
	IlcTARGETtrackMI * track = (IlcTARGETtrackMI*)array->At(i);   
	if (!track) continue;
	if (track->GetChi2MIP(0)>besttrack->GetChi2MIP(0)+2.*shared+1.) break;
	if ( (track->GetClIndex(ilayer)>0) && (track->GetClIndex(ilayer)!=besttrack->GetClIndex(ilayer))){
	  cansign = kFALSE;
	  break;
	}
      }
      if (cansign){
	if (TMath::Abs(besttrack->GetDy(ilayer)/besttrack->GetSigmaY(ilayer))>3.) continue;
	if (TMath::Abs(besttrack->GetDz(ilayer)/besttrack->GetSigmaZ(ilayer))>3.) continue;    
	if (!c->IsUsed()) c->Use();
      }
    }
  }
  return besttrack;
} 



void  IlcTARGETtrackerMI::GetBestHypothesysMIP(TObjArray &itsTracks)
{
  //
  // get "best" hypothesys
  //

  Int_t nentries = itsTracks.GetEntriesFast();
  for (Int_t i=0;i<nentries;i++){
    IlcTARGETtrackMI* track = (IlcTARGETtrackMI*)itsTracks.At(i);
    if (!track) continue;
    TObjArray * array = (TObjArray*) fTrackHypothesys.At(i);
    if (!array) continue;
    if (array->GetEntriesFast()<=0) continue;
    //
    IlcTARGETtrackMI* longtrack=0;
    Float_t minn=0;
    Float_t maxchi2=1000;
    for (Int_t j=0;j<array->GetEntriesFast();j++){
      IlcTARGETtrackMI* track2 = (IlcTARGETtrackMI*)array->At(j);
      if (!track2) continue;
      if (track2->GetGoldV0()) {
	longtrack = track2;   //gold V0 track taken
	break;
      }
      if (track2->GetNumberOfClusters()+track2->GetNDeadZone()<minn) continue;
      Float_t chi2 = track2->GetChi2MIP(0);
      if (fAfterV0){
	if (!track2->GetGoldV0()&&track2->GetConstrain()==kFALSE) chi2+=5;
      }
      if (track2->GetNumberOfClusters()+track2->GetNDeadZone()>minn) maxchi2 = track2->GetChi2MIP(0);  
      //
      if (chi2 > maxchi2) continue;
      minn= track2->GetNumberOfClusters()+track2->GetNDeadZone();
      maxchi2 = chi2;
      longtrack=track2;
    }    
    //
    //
    //
    IlcTARGETtrackMI * besttrack = (IlcTARGETtrackMI*)array->At(0);
    if (!longtrack) {longtrack = besttrack;}
    else besttrack= longtrack;
    //
    if (besttrack){
      Int_t list[kMaxClusters];
      IlcTARGETRecPoint * clist[kMaxClusters];
      Float_t shared = GetNumberOfSharedClusters(longtrack,i,list,clist);
      //
      track->SetNUsed(shared);      
      track->SetNSkipped(besttrack->GetNSkipped());
      track->SetChi2MIP(0,besttrack->GetChi2MIP(0));
      if (shared>0){
	Int_t nshared;
	Int_t overlist[kMaxClusters]; 
	//
	Int_t sharedtrack = GetOverlapTrack(longtrack, i, nshared, list, overlist);
	//if (sharedtrack==-1) sharedtrack=0;
	if (sharedtrack>=0){       
	  besttrack = GetBest2Tracks(i,sharedtrack,10,5.5);			  
	}
      }   
      if (besttrack&&fAfterV0){
        UpdateESDtrack(besttrack,IlcESDtrack::kVXDin);
      }
      if (besttrack&&fConstraint[fPass]) 
        UpdateESDtrack(besttrack,IlcESDtrack::kVXDin);
      //if (besttrack&&besttrack->fConstrain) 
      //	UpdateESDtrack(besttrack,IlcESDtrack::kVXDin);
      if (besttrack->GetChi2MIP(0)+besttrack->GetNUsed()>1.5){
	if ( (TMath::Abs(besttrack->GetD(0))>0.1) && fConstraint[fPass]) {
	  track->SetReconstructed(kFALSE);
	}
	if ( (TMath::Abs(besttrack->GetD(1))>0.1) && fConstraint[fPass]){
	  track->SetReconstructed(kFALSE);
	}
      }       

    }    
  }
} 


void IlcTARGETtrackerMI::CookLabel(IlcTARGETtrackMI *track,Float_t wrong) const {
  //--------------------------------------------------------------------
  //This function "cooks" a track label. If label<0, this track is fake.
  //--------------------------------------------------------------------
  Int_t tpcLabel=-1; 

  
  if (track->GetESDtrack()&&track->GetESDtrack()->GetTPCNcls()>0)   tpcLabel =  TMath::Abs(track->GetESDtrack()->GetTPCLabel());

  if(tpcLabel>=0){
    track->SetChi2MIP(9,0);
    Int_t nwrong=0;
    for (Int_t i=0;i<track->GetNumberOfClusters();i++){
      Int_t cindex = track->GetClusterIndex(i);
      Int_t l=(cindex & kClusterLayerMask) >> kClusterLayerShift;
      IlcTARGETRecPoint *cl = (IlcTARGETRecPoint*)GetCluster(cindex);
      Int_t isWrong=1;
      for (Int_t ind=0;ind<3;ind++){
	if (cl->GetLabel(ind)==tpcLabel) isWrong=0;
      }
      track->SetChi2MIP(9,track->GetChi2MIP(9)+isWrong*(2<<l));
      nwrong+=isWrong;
    }
    double fakeratio=double(nwrong)/double(track->GetNumberOfClusters());
    //function used to assign label to the found track. If track is fake, the label is negative
    track->SetFakeRatio(fakeratio);
    track->SetLabel(fakeratio>wrong?-tpcLabel:tpcLabel);
  }else{
    IlcTracker::CookLabel(track,wrong);
  }
   
}



void IlcTARGETtrackerMI::CookdEdx(IlcTARGETtrackMI* track)
{
  //
  //
  //  Int_t list[kMaxClusters];
  //IlcTARGETRecPoint * clist[kMaxClusters];
  //  Int_t shared = GetNumberOfSharedClusters(track,index,list,clist);
  Float_t dedx[kMaxClusters];
  Int_t accepted=0;
  track->SetChi2MIP(9,0);
  for (Int_t i=0;i<track->GetNumberOfClusters();i++){
    Int_t cindex = track->GetClusterIndex(i);
    Int_t l=(cindex & kClusterLayerMask) >> kClusterLayerShift;
    IlcTARGETRecPoint *cl = (IlcTARGETRecPoint*)GetCluster(cindex);
    Int_t lab = TMath::Abs(track->GetESDtrack()->GetTPCLabel());
    Int_t isWrong=1;
    for (Int_t ind=0;ind<3;ind++){
      if (cl->GetLabel(ind)==lab) isWrong=0;
    }
    track->SetChi2MIP(9,track->GetChi2MIP(9)+isWrong*(2<<l));
    //if (l<2) continue;
    //if (l>3 && (cl->GetNy()>4) || (cl->GetNz()>4)) continue;  //shared track
    //if (l>3&& !(cl->GetType()==1||cl->GetType()==10)) continue;
    //if (l<4&& !(cl->GetType()==1)) continue;   
    dedx[accepted]= track->GetSampledEdx(i);
    //dedx[accepted]= track->fNormQ[l];
    accepted++;
  }
  if (accepted<1) {
    track->SetdEdx(0);
    return;
  }
  Int_t indexes[kMaxClusters];
  TMath::Sort(accepted,dedx,indexes,kFALSE);
  Double_t low=0.;
  Double_t up=0.51;    
  Double_t nl=low*accepted, nu =up*accepted;  
  Float_t sumamp = 0;
  Float_t sumweight =0;
  for (Int_t i=0; i<accepted; i++) {
    Float_t weight =1;
    if (i<nl+0.1)     weight = TMath::Max(1.-(nl-i),0.);
    if (i>nu-1)     weight = TMath::Max(nu-i,0.);
    sumamp+= dedx[indexes[i]]*weight;
    sumweight+=weight;
  }
  track->SetdEdx(sumamp/sumweight);
}


void  IlcTARGETtrackerMI::MakeCoeficients(Int_t ntracks){
  //
  //
  if (fCoeficients) delete []fCoeficients;
  fCoeficients = new Float_t[ntracks*4*kMaxClusters];
  for (Int_t i=0;i<ntracks*4*kMaxClusters;i++) fCoeficients[i]=-1.;
}


Double_t IlcTARGETtrackerMI::GetPredictedChi2MI(IlcTARGETtrackMI* track, const IlcTARGETRecPoint *cluster,Int_t layer) 
{
  //
  //
  //  
  bool debuglevel=IlcDebugLevelClass()>=6;
  if(debuglevel){
    cout<<"Getting Chi2"<<endl;
    track->Print("params");
  }
  Float_t erry,errz;
  Float_t theta = track->GetTgl();
  Float_t phi   = track->GetSnp();
  phi = TMath::Sqrt(phi*phi/(1.-phi*phi+kAlmost0));
  GetError(layer,cluster,theta,phi,track->GetExpQ(),erry,errz);
  Double_t chi2 = track->GetPredictedChi2MI(cluster->GetY(),cluster->GetZ(),erry,errz);
  Float_t ny,nz;
  GetNTeor(layer,cluster, theta,phi,ny,nz);  
//   Double_t delta = cluster->GetNy()+cluster->GetNz()-nz-ny;
//   if (delta>1){
//     chi2+=0.5*TMath::Min(delta/2,2.);
//     chi2+=2.*cluster->GetDeltaProbability();
//   }
  //
  track->SetNy(layer,ny);
  track->SetNz(layer,nz);
  track->SetSigmaY(layer,erry);
  track->SetSigmaZ(layer, errz);
  //track->fNormQ[layer] = cluster->GetQ()/TMath::Sqrt(1+theta*theta+phi*phi);
  track->SetNormQ(layer,cluster->GetQ()/TMath::Sqrt((1.+ track->GetTgl()*track->GetTgl())/(1.- track->GetSnp()*track->GetSnp()+kAlmost0)));
  return chi2;

}

Int_t    IlcTARGETtrackerMI::UpdateMI(IlcTARGETtrackMI* track, const IlcTARGETRecPoint* cl,Double_t chi2,Int_t index) const 
{
  //
  //
  //
  int debugLevel=IlcDebugLevelClass();
  if(debugLevel>=1)
    IlcDebug(1,Form("Update with new meusarement chi2=%f",chi2));
  Int_t layer = (index & kClusterLayerMask) >> kClusterLayerShift;
  track->SetClIndex(layer, index);
  if ( (layer>1) &&track->GetNormQ(layer)/track->GetExpQ()<0.5 ) {
//     chi2+= (0.5-track->GetNormQ(layer)/track->GetExpQ())*10.;
    track->SetdEdxMismatch(track->GetdEdxMismatch()+(0.5-track->GetNormQ(layer)/track->GetExpQ())*10.);
  }
  //mark direction in update
  if(track->GetDir()<0) index|=kClusterDirMask;
  else index&=(~kClusterDirMask);

  if(debugLevel>=1)
    IlcDebug(1,Form("Before Pt=%f",1./track->Get1Pt()));
  Int_t status=track->UpdateMI(cl->GetY(),cl->GetZ(),track->GetSigmaY(layer),track->GetSigmaZ(layer),chi2,index);
  if(debugLevel>=1)
    IlcDebug(1,Form("After  Pt=%f, status=%i",1./track->Get1Pt(),status));
  return status;
}

void IlcTARGETtrackerMI::GetNTeor(Int_t layer, const IlcTARGETRecPoint* /*cl*/, Float_t theta, Float_t phi, Float_t &ny, Float_t &nz)
{
  //
  //get "mean shape"
  //
  if (layer==0){
    ny = 1.+TMath::Abs(phi)*3.2;
    nz = 1.+TMath::Abs(theta)*0.34;
    return;
  }
  if (layer==1){
    ny = 1.+TMath::Abs(phi)*3.2;
    nz = 1.+TMath::Abs(theta)*0.28;
    return;
  }
  
  if (layer>3){
    ny = 2.02+TMath::Abs(phi)*1.95;
    nz = 2.02+TMath::Abs(phi)*2.35;
    return;
  }
  ny  = 6.6-2.7*TMath::Abs(phi);
  nz  = 2.8-3.11*TMath::Abs(phi)+0.45*TMath::Abs(theta);
}



Int_t IlcTARGETtrackerMI::GetError(Int_t layer, const IlcTARGETRecPoint*cl, Float_t theta, Float_t phi,Float_t expQ, Float_t &erry, Float_t &errz)
{
  //calculate cluster position error
  //
  Float_t nz,ny;
  GetNTeor(layer, cl,theta,phi,ny,nz);  
  erry   = TMath::Sqrt(cl->GetSigmaY2()); 
  errz   = TMath::Sqrt(cl->GetSigmaZ2()); 
  return 200;
  //
  // PIXELS
  if (layer<2){
    
    if (TMath::Abs(ny-cl->GetNy())>0.6)  {
      if (ny<cl->GetNy()){
	erry*=0.4+TMath::Abs(ny-cl->GetNy());
	errz*=0.4+TMath::Abs(ny-cl->GetNy());
      }else{
	erry*=0.7+0.5*TMath::Abs(ny-cl->GetNy());
	errz*=0.7+0.5*TMath::Abs(ny-cl->GetNy());
      }
    }
    if (TMath::Abs(nz-cl->GetNz())>1.)  {
      erry*=TMath::Abs(nz-cl->GetNz());
      errz*=TMath::Abs(nz-cl->GetNz());	      
    }
    erry*=0.85;
    errz*=0.85;
    erry= TMath::Min(erry,float(0.005));
    errz= TMath::Min(errz,float(0.03));
    return 10;
  }

//STRIPS
  if (layer>3){ 
    //factor 1.8 appears in new simulation
    //
    Float_t scale=1.8;
    if (cl->GetNy()==100||cl->GetNz()==100){
      erry = 0.004*scale;
      errz = 0.2*scale;
      return 100;
    }
    if (cl->GetNy()+cl->GetNz()>12){
      erry = 0.06*scale;
      errz = 0.57*scale;
      return 100;
    }
    Float_t normq = cl->GetQ()/(TMath::Sqrt(1+theta*theta+phi*phi));
    Float_t chargematch = TMath::Max(double(normq/expQ),2.);
    //
    if (cl->GetType()==1 || cl->GetType()==10 ){     							       
      if (chargematch<1.0 || (cl->GetNy()+cl->GetNz()<nz+ny+0.5)){
	errz = 0.043*scale;
	erry = 0.00094*scale;
	return 101;
      }
      if (cl->GetNy()+cl->GetNz()<nz+ny+1.2){
	errz = 0.06*scale;
	erry =0.0013*scale;
	return 102;
      }
      erry = 0.0027*scale;
      errz = TMath::Min(0.028*(chargematch+cl->GetNy()+cl->GetNz()-nz+ny),0.15)*scale;
      return 103;
    }
    if (cl->GetType()==2 || cl->GetType()==11 ){ 
      erry = TMath::Min(0.0010*(1+chargematch+cl->GetNy()+cl->GetNz()-nz+ny),0.05)*scale;
      errz = TMath::Min(0.025*(1+chargematch+cl->GetNy()+cl->GetNz()-nz+ny),0.5)*scale;
      return 104;
    }
    
    if (cl->GetType()>100 ){     							       
      if ((chargematch+cl->GetNy()+cl->GetNz()-nz-ny<1.5)){
	errz = 0.05*scale;
	erry = 0.00096*scale;
	return 105;
      }
      if (cl->GetNy()+cl->GetNz()-nz-ny<1){
	errz = 0.10*scale;
	erry = 0.0025*scale;
	return 106;
      }

      errz = TMath::Min(0.05*(chargematch+cl->GetNy()+cl->GetNz()-nz-ny),0.4)*scale;
      erry = TMath::Min(0.003*(chargematch+cl->GetNy()+cl->GetNz()-nz-ny),0.05)*scale;
      return 107;
    }    
    Float_t diff = cl->GetNy()+cl->GetNz()-ny-nz;
    if (diff<1) diff=1;
    if (diff>4) diff=4;
        
    if (cl->GetType()==5||cl->GetType()==6||cl->GetType()==7||cl->GetType()==8){
      errz = 0.14*diff;
      erry = 0.003*diff;
      return 108;
    }  
    erry = 0.04*diff;
    errz = 0.06*diff;
    return 109;
  }
  //DRIFTS
  Float_t normq = cl->GetQ()/(TMath::Sqrt(1+theta*theta+phi*phi));
  Float_t chargematch = normq/expQ;
  Float_t factorz=1;
  Int_t   cnz = cl->GetNz()%10;
  //charge match
  if (cl->GetType()==1){
    if (chargematch<1.25){
      erry =  0.0028*(1.+6./cl->GetQ());  // gold clusters
    }
    else{
      erry = 0.003*chargematch;
      if (cl->GetNz()==3) erry*=1.5;
    }
    if (chargematch<1.0){
      errz =  0.0011*(1.+6./cl->GetQ());
    }
    else{
      errz = 0.002*(1+2*(chargematch-1.));
    }
    if (cnz>nz+0.6) {
      erry*=(cnz-nz+0.5);
      errz*=1.4*(cnz-nz+0.5);
    }
  }
  if (cl->GetType()>1){
    if (chargematch<1){
      erry =  0.00385*(1.+6./cl->GetQ());  // gold clusters
      errz =  0.0016*(1.+6./cl->GetQ());
    }
    else{
      errz = 0.0014*(1+3*(chargematch-1.));
      erry = 0.003*(1+3*(chargematch-1.));
    } 
    if (cnz>nz+0.6) {
      erry*=(cnz-nz+0.5);
      errz*=1.4*(cnz-nz+0.5);
    }
  }

  if (TMath::Abs(cl->GetY())>2.5){
    factorz*=1+2*(TMath::Abs(cl->GetY())-2.5);
  }
  if (TMath::Abs(cl->GetY())<1){
    factorz*=1.+0.5*TMath::Abs(TMath::Abs(cl->GetY())-1.);
  }
  factorz= TMath::Min(factorz,float(4.));  
  errz*=factorz;

  erry= TMath::Min(erry,float(0.05));
  errz= TMath::Min(errz,float(0.05));  
  return 200;
}



void   IlcTARGETtrackerMI::GetDCASigma(IlcTARGETtrackMI* track, Float_t & sigmarfi, Float_t &sigmaz)
{
  //
  //DCA sigmas parameterization
  //to be paramterized using external parameters in future 
  //
  // 
  sigmarfi = 0.004+1.4 *TMath::Abs(track->GetC())+332.*track->GetC()*track->GetC();
  sigmaz   = 0.011+4.37*TMath::Abs(track->GetC());
}


void IlcTARGETtrackerMI::SignDeltas( TObjArray *ClusterArray, Float_t vz)
{
  //
  //  
  Int_t entries = ClusterArray->GetEntriesFast();
  if (entries<4) return;
  IlcTARGETRecPoint* cluster = (IlcTARGETRecPoint*)ClusterArray->At(0);
  Int_t layer = cluster->GetLayer();
  if (layer>1) return;
  Int_t index[10000];
  Int_t ncandidates=0;
  Float_t r = (layer>0)? 7:4;
  // 
  for (Int_t i=0;i<entries;i++){
    IlcTARGETRecPoint* cl0 = (IlcTARGETRecPoint*)ClusterArray->At(i);
    Float_t nz = 1+TMath::Abs((cl0->GetZ()-vz)/r);
    if (cl0->GetNy()+cl0->GetNz()<=5+2*layer+nz) continue;
    index[ncandidates] = i;  //candidate to belong to delta electron track
    ncandidates++;
    if (cl0->GetNy()+cl0->GetNz()>9+2*layer+nz) {
      cl0->SetDeltaProbability(1);
    }
  }
  //
  //  
  //
  for (Int_t i=0;i<ncandidates;i++){
    IlcTARGETRecPoint* cl0 = (IlcTARGETRecPoint*)ClusterArray->At(index[i]);
    if (cl0->GetDeltaProbability()>0.8) continue;
    // 
    Int_t ncl = 0;
    Float_t y[100],z[100],sumy,sumz,sumy2, sumyz, sumw;
    sumy=sumz=sumy2=sumyz=sumw=0.0;
    for (Int_t j=0;j<ncandidates;j++){
      if (i==j) continue;
      IlcTARGETRecPoint* cl1 = (IlcTARGETRecPoint*)ClusterArray->At(index[j]);
      //
      Float_t dz = cl0->GetZ()-cl1->GetZ();
      Float_t dy = cl0->GetY()-cl1->GetY();
      if (TMath::Sqrt(dz*dz+dy*dy)<0.2){
	Float_t weight = cl1->GetNy()+cl1->GetNz()-2;
	y[ncl] = cl1->GetY();
	z[ncl] = cl1->GetZ();
	sumy+= y[ncl]*weight;
	sumz+= z[ncl]*weight;
	sumy2+=y[ncl]*y[ncl]*weight;
	sumyz+=y[ncl]*z[ncl]*weight;
	sumw+=weight;
	ncl++;
      }
    }
    if (ncl<4) continue;
    Float_t det = sumw*sumy2  - sumy*sumy;
    Float_t delta=1000;
    if (TMath::Abs(det)>0.01){
      Float_t z0  = (sumy2*sumz - sumy*sumyz)/det;
      Float_t k   = (sumyz*sumw - sumy*sumz)/det;
      delta = TMath::Abs(cl0->GetZ()-(z0+k*cl0->GetY()));
    }
    else{
      Float_t z0  = sumyz/sumy;
      delta = TMath::Abs(cl0->GetZ()-z0);
    }
    if ( delta<0.05) {
      cl0->SetDeltaProbability(1-20.*delta);
    }   
  }
}


void IlcTARGETtrackerMI::UpdateESDtrack(IlcTARGETtrackMI* track, ULong_t flags) const
{
  //
  //
  track->UpdateESDtrack(flags);
  IlcTARGETtrackMI * oldtrack = (IlcTARGETtrackMI*)(track->GetESDtrack()->GetVXDtrack());
  if (oldtrack) delete oldtrack; 
  track->GetESDtrack()->SetVXDtrack(new IlcTARGETtrackMI(*track));
  if (TMath::Abs(track->GetDnorm(1))<0.000000001){
    printf("Problem\n");
  }
}



Int_t IlcTARGETtrackerMI::GetNearestLayer(const Double_t *xr) const{
  //
  //Get nearest upper layer close to the point xr.
  // rough approximation 
  //
  Float_t radius = TMath::Sqrt(xr[0]*xr[0]+xr[1]*xr[1]);
  Int_t res =fParam->GetNlayers();
  for (Int_t i=0;i<fParam->GetNlayers();i++){
    if (radius<fgLayers[i].GetR()){
      res =i;
      break;
    }
  }
  return res;
}

Int_t IlcTARGETtrackerMI::GetNearestLayer(const IlcExternalTrackParam *track,double bz,bool tovtx,int clayer,int prevZlayer,int prevRlayer) const{
  //
  //Get nearest to direction.tovtx
  bool debuglevel=IlcDebugLevelClass()>=5;
  if(debuglevel){
    cout<<"IlcTARGETtrackerMI::GetNearestLayer: tovtx="<<tovtx<<endl;
    track->Print("params");
  }
  double xr[3];
  track->GetXYZ(xr,kFALSE);
  double radius = TMath::Sqrt(xr[0]*xr[0]+xr[1]*xr[1]);
  double zmin=-1e6,zmax=1e6;
  double rmin=0.,rmax=1e6,rprev=1e6;

  int layerRmin=-1,layerRmax=-1;
  int layerZmin=-1,layerZmax=-1;
  
  if(debuglevel){
    cout<<"IlcTARGETtrackerMI::GetNearestLayer: current x,y,z="<<xr[0]<<" "<<xr[1]<<" "<<xr[2]<<" r="<<radius<<" current layer "<<clayer<<endl;
  }

  for(int i=0;i<fnLayersAll;i++){
//     IlcInfo(Form("***lay: %d fgLayers[i].IsEndCap(): %d fgLayers[i].GetZ(): %f fgLayers[i].GetR(): %f fgLayers[i].GetRmin(): %f fgLayers[i].GetRmax(): %f", i, fgLayers[i].IsEndCap(), fgLayers[i].GetZ(), fgLayers[i].GetR(), fgLayers[i].GetRmin(), fgLayers[i].GetRmax()));
// // // //     if(fgLayers[i].IsEndCap()){
// // // //       if(i==prevZlayer||i==clayer) continue;
// // // //       double zlay=fgLayers[i].GetZ();
// // // //       if(zlay<xr[2]&&zlay>zmin){
// // // // 	  zmin=zlay;
// // // // 	  layerZmin=i;
// // // //       }
// // // //       if(zlay>xr[2]&&zlay<zmax){
// // // // 	zmax=zlay;
// // // // 	layerZmax=i;
// // // //       }
// // // //     }else{
      if(i==prevRlayer||i==clayer) {
	rprev=fgLayers[i].GetR();
	continue;
      }
      double rlay=fgLayers[i].GetR();
      if(prevRlayer>=0) {
	if(i<prevRlayer&&i>layerRmin){
	  rmin=rlay;
	  layerRmin=i;
	}
	if(i>prevRlayer&&(layerRmax<0||i<layerRmax)){
	  rmax=rlay;
	  layerRmax=i;
	}
      }else{
	if(rlay<radius&&rlay>rmin){
	  if(prevRlayer>=0&&tovtx&&i>prevRlayer) continue;
	  rmin=rlay;
	  layerRmin=i;
	}
	if(rlay>radius&&rlay<rmax){
	  if(prevRlayer>=0&&!tovtx&&i<prevRlayer) continue;
	  rmax=rlay;
	  layerRmax=i;
	}
      }
// // // //     }
  }
  
  int zdir=(track->GetTgl()*int(tovtx==false?1:-1))>0?1:-1;

  if(debuglevel)
    cout<<"Nearest layer tgl="<<track->GetTgl()<<" zdir="<<zdir<<" zmin,zmax="<<zmin<<" "<<zmax<<" layerZmin,layerZmax="<<layerZmin<<" "<<layerZmax<<
      endl<<" tovtx="<<tovtx<<" rmin,rmax="<<rmin<<" "<<rmax<<" layerRmin,layerRmax="<<layerRmin<<" "<<layerRmax<<endl;
  
  
  double cosdir=track->GetCosRDir();
  bool tovtxR=cosdir>0?tovtx:!tovtx;
  double rnext=tovtxR?rmin:rmax;
  double znext=zdir>0?zmax:zmin;
  int layerRnext=tovtxR?layerRmin:layerRmax;
  int layerZnext=zdir>0?layerZmax:layerZmin;

  double rnextOpposite=tovtxR?rmax:rmin;
  int layerRnextOpposite=tovtxR?layerRmax:layerRmin;
  if(prevRlayer>=0){
    if((rnextOpposite-rprev)*(rprev-rnext)>0){
      layerRnextOpposite=prevRlayer;
      if((rnextOpposite-rprev)*(rprev-radius)<=1e-6){
	rnextOpposite=radius+TMath::Sign(1e-6,(rnextOpposite-rprev));
      }else{
	rnextOpposite=rprev;
      }
    }
  }
  //all sensitive endcaps between frist barrel and last
  // forr barrel same
// // // //   if(layerZnext<0) return -1;

   double xyzatR[3],zatR;

  if(!track->GetXYZAtR(rnext,bz,xyzatR)||rnext<kAlmost0){
    if(debuglevel)
      cout<<"can't Prolongated at R next="<<TMath::Hypot(xyzatR[0],xyzatR[1])<<" Z will be= "<<xyzatR[2]<<endl;
    
    if(!track->GetXYZAtR(rnextOpposite,bz,xyzatR,-1)){
      if(debuglevel)
	cout<<"can't Prolongated at R opposite next="<<TMath::Hypot(xyzatR[0],xyzatR[1])<<
	  " Z will be= "<<xyzatR[2]<<endl;
      return layerZnext;
    }else{
      if(debuglevel)
	cout<<"Change rnext to opposite radius: ";
      layerRnext=-layerRnextOpposite-10;
      rnext=rnextOpposite;
      zatR=xyzatR[2];
    }
  }else{
    zatR=xyzatR[2];
  }
  
  
  if(debuglevel)
    cout<<"Prolongated at R next="<<rnext<<" Z will be= "<<zatR<<endl;
  

  if((znext-xr[2])*(zatR-znext)>0){
    double xyzAt[3];
    track->GetXYZAtZ(znext,bz,xyzAt);
    double ratZ=TMath::Hypot(xyzAt[0],xyzAt[1]);
    if(debuglevel)
      cout<<"Prolongated at Z next="<<znext<<" R will be= "<<ratZ<<endl;

    if((rnext-radius)*(ratZ-rnext)>0){
      return layerRnext;
    }
    return layerZnext;
  }else{
    if(layerRnext==-1) return layerZnext; 
    return layerRnext;
  }

}

void IlcTARGETtrackerMI::UpdateTPCV0(IlcESDEvent *event){
  //
  //try to update, or reject TPC  V0s
  //
  Int_t nv0s = event->GetNumberOfV0s();
  Int_t nitstracks = fTrackHypothesys.GetEntriesFast();

  for (Int_t i=0;i<nv0s;i++){
    IlcESDv0 * vertex = event->GetV0(i);
    Int_t ip = vertex->GetIndex(0);
    Int_t im = vertex->GetIndex(1);
    //
    TObjArray * arrayp = (ip<nitstracks) ? (TObjArray*)fTrackHypothesys.At(ip):0;
    TObjArray * arraym = (im<nitstracks) ? (TObjArray*)fTrackHypothesys.At(im):0;
    IlcTARGETtrackMI * trackp = (arrayp!=0) ? (IlcTARGETtrackMI*)arrayp->At(0):0;
    IlcTARGETtrackMI * trackm = (arraym!=0) ? (IlcTARGETtrackMI*)arraym->At(0):0;
    //
    //
    if (trackp){
      if (trackp->GetNumberOfClusters()+trackp->GetNDeadZone()>5.5){
	if (trackp->GetConstrain()&&trackp->GetChi2MIP(0)<3) vertex->SetStatus(-100);
	if (!trackp->GetConstrain()&&trackp->GetChi2MIP(0)<2) vertex->SetStatus(-100); 
      }
    }

    if (trackm){
      if (trackm->GetNumberOfClusters()+trackm->GetNDeadZone()>5.5){
	if (trackm->GetConstrain()&&trackm->GetChi2MIP(0)<3) vertex->SetStatus(-100);
	if (!trackm->GetConstrain()&&trackm->GetChi2MIP(0)<2) vertex->SetStatus(-100); 
      }
    }
    if (vertex->GetStatus()==-100) continue;
    //
    Double_t Xr[3]; for(Int_t ii=0; ii<3; ii++) Xr[ii] = vertex->GetXr(ii);
    Int_t clayer = GetNearestLayer(Xr);
    vertex->SetNBefore(clayer);        //
    vertex->SetChi2Before(9*clayer);   //
    vertex->SetNAfter(6-clayer);       //
    vertex->SetChi2After(0);           //
    //
    if (clayer >1 ){ // calculate chi2 before vertex
      Float_t chi2p = 0, chi2m=0;  
      //
      if (trackp){
	for (Int_t ilayer=0;ilayer<clayer;ilayer++){
	  if (trackp->GetClIndex(ilayer)>0){
	    chi2p+=trackp->GetDy(ilayer)*trackp->GetDy(ilayer)/(trackp->GetSigmaY(ilayer)*trackp->GetSigmaY(ilayer))+
	      trackp->GetDz(ilayer)*trackp->GetDz(ilayer)/(trackp->GetSigmaZ(ilayer)*trackp->GetSigmaZ(ilayer));
	  }
	  else{
	    chi2p+=9;
	  }
	}
      }else{
	chi2p = 9*clayer;
      }
      //
      if (trackm){
	for (Int_t ilayer=0;ilayer<clayer;ilayer++){
	  if (trackm->GetClIndex(ilayer)>0){
	    chi2m+=trackm->GetDy(ilayer)*trackm->GetDy(ilayer)/(trackm->GetSigmaY(ilayer)*trackm->GetSigmaY(ilayer))+
	      trackm->GetDz(ilayer)*trackm->GetDz(ilayer)/(trackm->GetSigmaZ(ilayer)*trackm->GetSigmaZ(ilayer));
	  }
	  else{
	    chi2m+=9;
	  }
	}
      }else{
	chi2m = 9*clayer;
      }
      vertex->SetChi2Before(TMath::Min(chi2p,chi2m));
      if (TMath::Min(chi2p,chi2m)/Float_t(clayer)<4) vertex->SetStatus(-10);  // track exist before vertex
    }
    
    if (clayer < 5 ){ // calculate chi2 after vertex
      Float_t chi2p = 0, chi2m=0;  
      //
      if (trackp&&TMath::Abs(trackp->GetTgl())<1.){
	for (Int_t ilayer=clayer;ilayer<kMaxClusters;ilayer++){
	  if (trackp->GetClIndex(ilayer)>0){
	    chi2p+=trackp->GetDy(ilayer)*trackp->GetDy(ilayer)/(trackp->GetSigmaY(ilayer)*trackp->GetSigmaY(ilayer))+
	      trackp->GetDz(ilayer)*trackp->GetDz(ilayer)/(trackp->GetSigmaZ(ilayer)*trackp->GetSigmaZ(ilayer));
	  }
	  else{
	    chi2p+=9;
	  }
	}
      }else{
	chi2p = 0;
      }
      //
      if (trackm&&TMath::Abs(trackm->GetTgl())<1.){
	for (Int_t ilayer=clayer;ilayer<kMaxClusters;ilayer++){
	  if (trackm->GetClIndex(ilayer)>0){
	    chi2m+=trackm->GetDy(ilayer)*trackm->GetDy(ilayer)/(trackm->GetSigmaY(ilayer)*trackm->GetSigmaY(ilayer))+
	      trackm->GetDz(ilayer)*trackm->GetDz(ilayer)/(trackm->GetSigmaZ(ilayer)*trackm->GetSigmaZ(ilayer));
	  }
	  else{
	    chi2m+=9;
	  }
	}
      }else{
	chi2m = 0;
      }
      vertex->SetChi2After(TMath::Max(chi2p,chi2m));
      if (TMath::Max(chi2m,chi2p)/Float_t(kMaxClusters-clayer)>9) vertex->SetStatus(-20);  // track not found in TARGET
    }
  }
  //
}



void  IlcTARGETtrackerMI::FindV02(IlcESDEvent *event)
{
  //
  // V0 finder
  //
  //  Cuts on DCA -  R dependent
  //          max distance DCA between 2 tracks cut 
  //          maxDist = TMath::Min(kMaxDist,kMaxDist0+pvertex->GetRr()*kMaxDist);
  //
  IlcDebug(5,"Begin V0 Finding");

  const Float_t kMaxDist0      = 0.1;    
  const Float_t kMaxDist1      = 0.1;     
  const Float_t kMaxDist       = 1;
  const Float_t kMinPointAngle  = 0.85;
  const Float_t kMinPointAngle2 = 0.99;
  const Float_t kMinR           = 0.1;
  const Float_t kMaxR           = 200;
  //const Float_t kCausilcty0Cut   = 0.19;
  //const Float_t kLikelihood01Cut = 0.25;
  //const Float_t kPointAngleCut   = 0.9996;
  const Float_t kCausality0Cut   = 0.19;
  const Float_t kLikelihood01Cut = 0.45;
  const Float_t kLikelihood1Cut  = 0.5;
  const Float_t kCombinedCut     = 0.55;

  //
  //
  TTreeSRedirector &cstream = *fDebugStreamer;
  Int_t ntracks    = event->GetNumberOfTracks(); 
  Int_t nitstracks = fTrackHypothesys.GetEntriesFast();
  fOriginal.Expand(ntracks);
  fTrackHypothesys.Expand(ntracks);
  fBestHypothesys.Expand(ntracks);
  //
  IlcHelix * helixes   = new IlcHelix[ntracks+2];
  TObjArray trackarray(ntracks+2);     //array with tracks - with vertex constrain
  TObjArray trackarrayc(ntracks+2);    //array of "best    tracks" - without vertex constrain
  TObjArray trackarrayl(ntracks+2);    //array of "longest tracks" - without vertex constrain
  Bool_t * forbidden   = new Bool_t [ntracks+2];
  Int_t   *itsmap      = new Int_t  [ntracks+2];
  Float_t *dist        = new Float_t[ntracks+2];
  Float_t *normdist0   = new Float_t[ntracks+2];
  Float_t *normdist1   = new Float_t[ntracks+2];
  Float_t *normdist    = new Float_t[ntracks+2];
  Float_t *norm        = new Float_t[ntracks+2];
  Float_t *maxr        = new Float_t[ntracks+2];
  Float_t *minr        = new Float_t[ntracks+2];
  Float_t *minPointAngle= new Float_t[ntracks+2];
  //
  IlcV0 *pvertex      = new IlcV0;
  IlcTARGETtrackMI * dummy= new IlcTARGETtrackMI;
  dummy->SetLabel(0);
  IlcTARGETtrackMI  trackat0;    //temporary track for DCA calculation
  //
  Float_t primvertex[3]={GetX(),GetY(),GetZ()};
  //
  // make its -  esd map
  //
  for (Int_t itrack=0;itrack<ntracks+2;itrack++) {
    itsmap[itrack]        = -1;
    forbidden[itrack]     = kFALSE;
    maxr[itrack]          = kMaxR;
    minr[itrack]          = kMinR;
    minPointAngle[itrack] = kMinPointAngle;
  }
  for (Int_t itrack=0;itrack<nitstracks;itrack++){
    IlcTARGETtrackMI * original =   (IlcTARGETtrackMI*)(fOriginal.At(itrack));
    Int_t           esdindex =   original->GetESDtrack()->GetID();
    itsmap[esdindex]         =   itrack;
  }
  //
  // create its tracks from esd tracks if not done before
  //
  for (Int_t itrack=0;itrack<ntracks;itrack++){
    if (itsmap[itrack]>=0) continue;
    IlcTARGETtrackMI * tpctrack = new IlcTARGETtrackMI(*(event->GetTrack(itrack)));
    //tpctrack->fD[0] = tpctrack->GetD(GetX(),GetY());
    //tpctrack->fD[1] = tpctrack->GetZat(GetX())-GetZ(); 
    tpctrack->GetDZ(GetX(),GetY(),GetZ(),tpctrack->GetDP());   //I.B.
    if (fabs(tpctrack->GetD(0))<fmaxR && fabs(tpctrack->GetD(1))<fmaxZ){
      // tracks which can reach inner part of TARGET
      // propagate track to outer its volume - with correction for material
      CorrectForDeadZoneMaterial(tpctrack);  
    }
    itsmap[itrack] = nitstracks;
    fOriginal.AddAt(tpctrack,nitstracks);
    nitstracks++;
  }
  //
  // fill temporary arrays
  //
  for (Int_t itrack=0;itrack<ntracks;itrack++){
    IlcESDtrack *  esdtrack = event->GetTrack(itrack);
    Int_t          itsindex = itsmap[itrack];
    IlcTARGETtrackMI *original = (IlcTARGETtrackMI*)fOriginal.At(itsmap[itrack]);
    if (!original) continue;
    IlcTARGETtrackMI *bestConst  = 0;
    IlcTARGETtrackMI *bestLong   = 0;
    IlcTARGETtrackMI *best       = 0;    
    //
    //
    TObjArray * array    = (TObjArray*)  fTrackHypothesys.At(itsindex);
    Int_t       hentries = (array==0) ?  0 : array->GetEntriesFast();
    // Get best track with vertex constrain
    for (Int_t ih=0;ih<hentries;ih++){
      IlcTARGETtrackMI * trackh = (IlcTARGETtrackMI*)array->At(ih);
      if (!trackh->GetConstrain()) continue;
      if (!bestConst) bestConst = trackh;
      if (trackh->GetNumberOfClusters()>4.0){
	bestConst  = trackh;                         // full track -  with minimal chi2
	break;
      }
      if (trackh->GetNumberOfClusters()+trackh->GetNDeadZone()<=bestConst->GetNumberOfClusters()+bestConst->GetNDeadZone())  continue;      
      bestConst = trackh;
      break;
    }
    // Get best long track without vertex constrain and best track without vertex constrain
    for (Int_t ih=0;ih<hentries;ih++){
      IlcTARGETtrackMI * trackh = (IlcTARGETtrackMI*)array->At(ih);
      if (trackh->GetConstrain()) continue;
      if (!best)     best     = trackh;
      if (!bestLong) bestLong = trackh;
      if (trackh->GetNumberOfClusters()>4.0){
	bestLong  = trackh;                         // full track -  with minimal chi2
	break;
      }
      if (trackh->GetNumberOfClusters()+trackh->GetNDeadZone()<=bestLong->GetNumberOfClusters()+bestLong->GetNDeadZone())  continue;      
      bestLong = trackh;	
    }
    if (!best) {
      best     = original;
      bestLong = original;
    }
    //I.B. trackat0 = *bestLong;
    new (&trackat0) IlcTARGETtrackMI(*bestLong);
    Double_t xx,yy,zz,alpha; 
    bestLong->GetGlobalXYZat(bestLong->GetX(),xx,yy,zz);
    alpha = TMath::ATan2(yy,xx);    
    trackat0.Propagate(alpha,0); 
    // calculate normilczed distances to the vertex 
    //
    Float_t ptfac  = (1.+100.*TMath::Abs(trackat0.GetC()));
    if ( bestLong->GetNumberOfClusters()>3 ){      
      dist[itsindex]      = trackat0.GetY();
      norm[itsindex]      = ptfac*TMath::Sqrt(trackat0.GetSigmaY2());
      normdist0[itsindex] = TMath::Abs(trackat0.GetY()/norm[itsindex]);
      normdist1[itsindex] = TMath::Abs((trackat0.GetZ()-primvertex[2])/(ptfac*TMath::Sqrt(trackat0.GetSigmaZ2())));
      normdist[itsindex]  = TMath::Sqrt(normdist0[itsindex]*normdist0[itsindex]+normdist1[itsindex]*normdist1[itsindex]);
      if (!bestConst){
	if (bestLong->GetNumberOfClusters()+bestLong->GetNDeadZone()<5) normdist[itsindex]*=2.;
	if (bestLong->GetNumberOfClusters()+bestLong->GetNDeadZone()<4) normdist[itsindex]*=2.;
	if (bestLong->GetNumberOfClusters()+bestLong->GetNDeadZone()<3) normdist[itsindex]*=2.;
      }else{
	if (bestConst->GetNumberOfClusters()+bestConst->GetNDeadZone()<5) normdist[itsindex]*=1.5;
	if (bestConst->GetNumberOfClusters()+bestConst->GetNDeadZone()<4) normdist[itsindex]*=1.5;
      }
    }
    else{      
      if (bestConst&&bestConst->GetNumberOfClusters()+bestConst->GetNDeadZone()>4.5){
	dist[itsindex] = bestConst->GetD(0);
	norm[itsindex] = bestConst->GetDnorm(0);
	normdist0[itsindex] = TMath::Abs(bestConst->GetD(0)/norm[itsindex]);
	normdist1[itsindex] = TMath::Abs(bestConst->GetD(0)/norm[itsindex]);
	normdist[itsindex]  = TMath::Sqrt(normdist0[itsindex]*normdist0[itsindex]+normdist1[itsindex]*normdist1[itsindex]);
      }else{
	dist[itsindex]      = trackat0.GetY();
	norm[itsindex]      = ptfac*TMath::Sqrt(trackat0.GetSigmaY2());
	normdist0[itsindex] = TMath::Abs(trackat0.GetY()/norm[itsindex]);
	normdist1[itsindex] = TMath::Abs((trackat0.GetZ()-primvertex[2])/(ptfac*TMath::Sqrt(trackat0.GetSigmaZ2())));
	normdist[itsindex]  = TMath::Sqrt(normdist0[itsindex]*normdist0[itsindex]+normdist1[itsindex]*normdist1[itsindex]);
	if (TMath::Abs(trackat0.GetTgl())>1.05){
	  if (normdist[itsindex]<3) forbidden[itsindex]=kTRUE;
	  if (normdist[itsindex]>3) {
	    minr[itsindex] = TMath::Max(Float_t(10.),minr[itsindex]);
	  }
	}
      }
    }
    //
    //-----------------------------------------------------------
    //Forbid primary track candidates - 
    //
    //treetr->SetIlcas("forbidden0","Tr0.fN<4&&Tr1.fN+Tr1.fNDeadZone>4.5");
    //treetr->SetIlcas("forbidden1","ND<3&&Tr1.fN+Tr1.fNDeadZone>5.5");
    //treetr->SetIlcas("forbidden2","ND<2&&Tr1.fClIndex[0]>0&&Tr1.fClIndex[0]>0");
    //treetr->SetIlcas("forbidden3","ND<1&&Tr1.fClIndex[0]>0");
    //treetr->SetIlcas("forbidden4","ND<4&&Tr1.fNormChi2[0]<2");
    //treetr->SetIlcas("forbidden5","ND<5&&Tr1.fNormChi2[0]<1");
    //-----------------------------------------------------------
    if (bestConst){
      if (bestLong->GetNumberOfClusters()<4       && bestConst->GetNumberOfClusters()+bestConst->GetNDeadZone()>4.5)               forbidden[itsindex]=kTRUE;
      if (normdist[itsindex]<3 && bestConst->GetNumberOfClusters()+bestConst->GetNDeadZone()>5.5)               forbidden[itsindex]=kTRUE;
      if (normdist[itsindex]<2 && bestConst->GetClIndex(0)>0 && bestConst->GetClIndex(1)>0 ) forbidden[itsindex]=kTRUE;
      if (normdist[itsindex]<1 && bestConst->GetClIndex(0)>0)                              forbidden[itsindex]=kTRUE;
      if (normdist[itsindex]<4 && bestConst->GetNormChi2(0)<2)                             forbidden[itsindex]=kTRUE;
      if (normdist[itsindex]<5 && bestConst->GetNormChi2(0)<1)                             forbidden[itsindex]=kTRUE;      
      if (bestConst->GetNormChi2(0)<2.5) {
	minPointAngle[itsindex]= 0.9999;
	maxr[itsindex]         = 10;
      }
    }
    //
    //forbid daughter kink candidates
    //
    if (esdtrack->GetKinkIndex(0)>0) forbidden[itsindex] = kTRUE;
    Bool_t isElectron = kTRUE;
    Bool_t isProton   = kTRUE;
    Double_t pid[5];
    esdtrack->GetESDpid(pid);
    for (Int_t i=1;i<5;i++){
      if (pid[0]<pid[i]) isElectron= kFALSE;
      if (pid[4]<pid[i]) isProton= kFALSE;
    }
    if (isElectron){
      forbidden[itsindex]=kFALSE;	
      normdist[itsindex]*=-1;
    }
    if (isProton){
      if (normdist[itsindex]>2) forbidden[itsindex]=kFALSE;	
      normdist[itsindex]*=-1;
    }

    //
    // Causilcty cuts in TPC volume
    //
//     if (esdtrack->GetTPCdensity(512,512+15) >0.6)  maxr[itsindex] = TMath::Min(Float_t(40),maxr[itsindex]);
//     if (esdtrack->GetTPCdensity(512+15,512+30)>0.6)  maxr[itsindex] = TMath::Min(Float_t(50),maxr[itsindex]);
//     if (esdtrack->GetTPCdensity(512+32+512,512+32+512+15)>0.6)  maxr[itsindex] = TMath::Min(Float_t(60),maxr[itsindex]);
//     if (esdtrack->GetTPCdensity(512+32+512+15,512+32+512+30)>0.6)  maxr[itsindex] = TMath::Min(Float_t(70),maxr[itsindex]);
//     //
//     if (esdtrack->GetTPCdensity(512,512+31)<0.4&&
// 	esdtrack->GetTPCdensity(512+32+512,512+32+512+31)<0.4&&
// 	bestLong->GetNumberOfClusters()<3) minr[itsindex]=55;  
//     cout<<"TPC density "<<
//       esdtrack->GetTPCdensity(512,512+15)<<" "<<
//       esdtrack->GetTPCdensity(512+15,512+30)<<" "<<
//       esdtrack->GetTPCdensity(512+32+512,512+32+512+15)<<" "<<
//       esdtrack->GetTPCdensity(512+32+512+15,512+32+512+30)<<" "<<
//       esdtrack->GetTPCdensity(512,512+31)<<" "<<
//       esdtrack->GetTPCdensity(512+32+512,512+32+512+31)<<endl;
    //
    //
    if (kFALSE){
      cstream<<"Track"<<
	"Tr0.="<<best<<
	"Tr1.="<<((bestConst)? bestConst:dummy)<<
	"Tr2.="<<bestLong<<
	"Tr3.="<<&trackat0<<
	"Esd.="<<esdtrack<<
	"Dist="<<dist[itsindex]<<
	"ND0="<<normdist0[itsindex]<<
	"ND1="<<normdist1[itsindex]<<
	"ND="<<normdist[itsindex]<<
	"Pz="<<primvertex[2]<<
	"Forbid="<<forbidden[itsindex]<<
	"\n";
      //
    }
    trackarray.AddAt(best,itsindex);
    trackarrayc.AddAt(bestConst,itsindex);
    trackarrayl.AddAt(bestLong,itsindex);
    new (&helixes[itsindex]) IlcHelix(*best);
  }
  //
  //
  //
  // first iterration of V0 finder
  //
  for (Int_t iesd0=0;iesd0<ntracks;iesd0++){
    Int_t itrack0 = itsmap[iesd0];
//     cout<<"iesd0 "<<iesd0<<" "<<itrack0<<" "<<forbidden[itrack0]<<endl;
    if (forbidden[itrack0]) continue;
    IlcTARGETtrackMI * btrack0 = (IlcTARGETtrackMI*)trackarray.At(itrack0);
    if (!btrack0) continue;    
//     cout<<"charge "<<btrack0->GetSign()<<endl;
    if (btrack0->GetSign()>0) continue;
    IlcTARGETtrackMI *trackc0 = (IlcTARGETtrackMI*)trackarrayc.At(itrack0);
    //
    for (Int_t iesd1=0;iesd1<ntracks;iesd1++){
      Int_t itrack1 = itsmap[iesd1];
//       cout<<"iesd1 "<<iesd1<<" "<<itrack1<<" "<<forbidden[itrack1]<<endl;
      if (forbidden[itrack1]) continue;

      IlcTARGETtrackMI * btrack1 = (IlcTARGETtrackMI*)trackarray.At(itrack1); 
      if (!btrack1) continue;
//       cout<<"charge "<<btrack1->GetSign()<<endl;
      if (btrack1->GetSign()<0) continue;
      Bool_t isGold = kFALSE;
      if (TMath::Abs(TMath::Abs(btrack0->GetLabel())-TMath::Abs(btrack1->GetLabel()))==1){
	isGold = kTRUE;
      }
      IlcTARGETtrackMI *trackc1 = (IlcTARGETtrackMI*)trackarrayc.At(itrack1);
      IlcHelix &h1 = helixes[itrack0];
      IlcHelix &h2 = helixes[itrack1];
      //
      // find linear distance
      Double_t rmin =0;
      //
      //
      //
      Double_t phase[2][2],radius[2];
      Int_t  points = h1.GetRPHIintersections(h2, phase, radius);
//       cout<<"Points RPHI intersection "<<points<<endl;
      if    (points==0)  continue;
      Double_t delta[2]={1000000,1000000};        
      rmin = radius[0];
      h1.ParabolicDCA(h2,phase[0][0],phase[0][1],radius[0],delta[0]);
      if (points==2){    
	if (radius[1]<rmin) rmin = radius[1];
	h1.ParabolicDCA(h2,phase[1][0],phase[1][1],radius[1],delta[1]);
      }
      rmin = TMath::Sqrt(rmin);
      Double_t distance = 0;
      Double_t radiusC  = 0;
      Int_t    iphase   = 0;
      if (points==1 || delta[0]<delta[1]){
	distance = TMath::Sqrt(delta[0]);
	radiusC  = TMath::Sqrt(radius[0]);
      }else{
	distance = TMath::Sqrt(delta[1]);
	radiusC  = TMath::Sqrt(radius[1]);
	iphase=1;
      }
//       cout<<"Radius cut "<<itrack0<<" "<<itrack1<<" "<<radiusC<<" min,max "<<minr[itrack0]<<" "<<
// 	maxr[itrack0]<<" "<<minr[itrack1]<<" "<<maxr[itrack1]<<endl;
      if (radiusC<TMath::Max(minr[itrack0],minr[itrack1]))    continue;
      if (radiusC>TMath::Min(maxr[itrack0],maxr[itrack1]))     continue; 
      Float_t maxDist  = TMath::Min(kMaxDist,Float_t(kMaxDist0+radiusC*kMaxDist1));    
//       cout<<"MaxDist "<<distance<<" "<<maxDist<<endl;
      if (distance>maxDist) continue;
      Float_t pointAngle = h1.GetPointAngle(h2,phase[iphase],primvertex);
//       cout<<"PointAngle "<<pointAngle<<" "<<TMath::Max(minPointAngle[itrack0],minPointAngle[itrack1])<<endl;
      if (pointAngle<TMath::Max(minPointAngle[itrack0],minPointAngle[itrack1])) continue;
      //
      //
      //      Double_t distance = TestV0(h1,h2,pvertex,rmin);
      //
      //       if (distance>maxDist)           continue;
      //       if (pvertex->GetRr()<kMinR)     continue;
      //       if (pvertex->GetRr()>kMaxR)     continue;
      IlcTARGETtrackMI * track0=btrack0;
      IlcTARGETtrackMI * track1=btrack1;
      //      if (pvertex->GetRr()<3.5){  
      if (radiusC<3.5){  
	//use longest tracks inside the pipe
	track0 = (IlcTARGETtrackMI*)trackarrayl.At(itrack0);
	track1 = (IlcTARGETtrackMI*)trackarrayl.At(itrack1);
      }      
      //
      //
      pvertex->SetM(*track0);
      pvertex->SetP(*track1);
      pvertex->Update(primvertex);
      pvertex->SetClusters(track0->ClIndex(),track1->ClIndex());  // register clusters

      if (pvertex->GetRr()<kMinR) continue;
      if (pvertex->GetRr()>kMaxR) continue;
      if (pvertex->GetPointAngle()<kMinPointAngle) continue;
      if (pvertex->GetDist2()>maxDist) continue;
      pvertex->SetLab(0,track0->GetLabel());
      pvertex->SetLab(1,track1->GetLabel());
      pvertex->SetIndex(0,track0->GetESDtrack()->GetID());
      pvertex->SetIndex(1,track1->GetESDtrack()->GetID());
      
      //      
      IlcTARGETtrackMI * htrackc0 = trackc0 ? trackc0:dummy;      
      IlcTARGETtrackMI * htrackc1 = trackc1 ? trackc1:dummy;

      //
      //
      TObjArray * array0b     = (TObjArray*)fBestHypothesys.At(itrack0);
      if (!array0b&&pvertex->GetRr()<40 && TMath::Abs(track0->GetTgl())<1.1) 
	FollowProlongationTree((IlcTARGETtrackMI*)fOriginal.At(itrack0),itrack0, kFALSE);
      TObjArray * array1b    = (TObjArray*)fBestHypothesys.At(itrack1);
      if (!array1b&&pvertex->GetRr()<40 && TMath::Abs(track1->GetTgl())<1.1) 
	FollowProlongationTree((IlcTARGETtrackMI*)fOriginal.At(itrack1),itrack1, kFALSE);
      //
      IlcTARGETtrackMI * track0b = (IlcTARGETtrackMI*)fOriginal.At(itrack0);       
      IlcTARGETtrackMI * track1b = (IlcTARGETtrackMI*)fOriginal.At(itrack1);
      IlcTARGETtrackMI * track0l = (IlcTARGETtrackMI*)fOriginal.At(itrack0);       
      IlcTARGETtrackMI * track1l = (IlcTARGETtrackMI*)fOriginal.At(itrack1);
      
      Float_t minchi2before0=16;
      Float_t minchi2before1=16;
      Float_t minchi2after0 =16;
      Float_t minchi2after1 =16;
      Double_t Xr[3]; for(Int_t i=0; i<3; i++) Xr[i] = pvertex->GetXr(i);
      Int_t maxLayer = GetNearestLayer(Xr);
      
      if (array0b) for (Int_t i=0;i<5;i++){
	// best track after vertex
	IlcTARGETtrackMI * btrack = (IlcTARGETtrackMI*)array0b->At(i);
	if (!btrack) continue;
	if (btrack->GetNumberOfClusters()>track0l->GetNumberOfClusters()) track0l = btrack;     
	//	if (btrack->fX<pvertex->GetRr()-2.-0.5/(0.1+pvertex->GetAnglep()[2])) {
	if (btrack->GetX()<pvertex->GetRr()-2.) {
	  if ( (maxLayer>i+2|| (i==0)) && btrack->GetNumberOfClusters()==(5-i)&&i<3){
	    Float_t sumchi2= 0;
	    Float_t sumn   = 0;
	    if (maxLayer<3){   // take prim vertex as additional measurement
	      if (normdist[itrack0]>0 && htrackc0){
		sumchi2 += TMath::Min((3.-maxLayer)*normdist[itrack0]*normdist[itrack0],16.);
	      }else{
		sumchi2 +=  TMath::Min((3.-maxLayer)*(3*normdist[itrack0]*normdist[itrack0]+3.),16.);
	      }
	      sumn    +=  3-maxLayer;
	    }
	    for (Int_t ilayer=i;ilayer<maxLayer;ilayer++){
	      sumn+=1.;	      
	      if (!btrack->GetClIndex(ilayer)){
		sumchi2+=25;
		continue;
	      }else{
		Int_t c=( btrack->GetClIndex(ilayer) & kClusterCountMask);
		for (Int_t itrack=0;itrack<4;itrack++){
		  if (fgLayers[ilayer].GetClusterTracks(itrack,c)>=0 && fgLayers[ilayer].GetClusterTracks(itrack,c)!=itrack0){
		    sumchi2+=18.;  //shared cluster
		    break;
		  }
		}
		sumchi2+=btrack->GetDy(ilayer)*btrack->GetDy(ilayer)/(btrack->GetSigmaY(ilayer)*btrack->GetSigmaY(ilayer));
		sumchi2+=btrack->GetDz(ilayer)*btrack->GetDz(ilayer)/(btrack->GetSigmaZ(ilayer)*btrack->GetSigmaZ(ilayer));	       
	      }
	    }
	    sumchi2/=sumn;
	    if (sumchi2<minchi2before0) minchi2before0=sumchi2; 
	  }
	  continue;   //safety space - Geo manager will give exact layer
	}
	track0b       = btrack;
	minchi2after0 = btrack->GetNormChi2(i);
	break;
      }
      if (array1b) for (Int_t i=0;i<5;i++){
	// best track after vertex
	IlcTARGETtrackMI * btrack = (IlcTARGETtrackMI*)array1b->At(i);
	if (!btrack) continue;
	if (btrack->GetNumberOfClusters()>track1l->GetNumberOfClusters()) track1l = btrack;     
	//	if (btrack->fX<pvertex->GetRr()-2-0.5/(0.1+pvertex->GetAnglep()[2])){
	if (btrack->GetX()<pvertex->GetRr()-2){
	  if ((maxLayer>i+2 || (i==0))&&btrack->GetNumberOfClusters()==(5-i)&&(i<3)){
	    Float_t sumchi2= 0;
	    Float_t sumn   = 0;
	    if (maxLayer<3){   // take prim vertex as additional measurement
	      if (normdist[itrack1]>0 && htrackc1){
		sumchi2 +=  TMath::Min((3.-maxLayer)*normdist[itrack1]*normdist[itrack1],16.);
	      }else{
		sumchi2 += TMath::Min((3.-maxLayer)*(3*normdist[itrack1]*normdist[itrack1]+3.),16.);
	      }
	      sumn    +=  3-maxLayer;
	    }
	    for (Int_t ilayer=i;ilayer<maxLayer;ilayer++){
	      sumn+=1.;
	      if (!btrack->GetClIndex(ilayer)){
		sumchi2+=30;
		continue;
	      }else{
		Int_t c=( btrack->GetClIndex(ilayer) & kClusterCountMask);
		for (Int_t itrack=0;itrack<4;itrack++){
		  if (fgLayers[ilayer].GetClusterTracks(itrack,c)>=0 && fgLayers[ilayer].GetClusterTracks(itrack,c)!=itrack1){
		    sumchi2+=18.;  //shared cluster
		    break;
		  }
		}
		sumchi2+=btrack->GetDy(ilayer)*btrack->GetDy(ilayer)/(btrack->GetSigmaY(ilayer)*btrack->GetSigmaY(ilayer));
		sumchi2+=btrack->GetDz(ilayer)*btrack->GetDz(ilayer)/(btrack->GetSigmaZ(ilayer)*btrack->GetSigmaZ(ilayer));	       
	      }
	    }
	    sumchi2/=sumn;
	    if (sumchi2<minchi2before1) minchi2before1=sumchi2; 
	  }
	  continue;   //safety space - Geo manager will give exact layer	   
	}
	track1b = btrack;
	minchi2after1 = btrack->GetNormChi2(i);
	break;
      }
      //
      // position resolution - used for DCA cut
      Float_t sigmad = track0b->GetSigmaY2()+track0b->GetSigmaZ2()+track1b->GetSigmaY2()+track1b->GetSigmaZ2()+
	(track0b->GetX()-pvertex->GetRr())*(track0b->GetX()-pvertex->GetRr())*(track0b->GetSigmaSnp2()+track0b->GetSigmaTgl2())+
	(track1b->GetX()-pvertex->GetRr())*(track1b->GetX()-pvertex->GetRr())*(track1b->GetSigmaSnp2()+track1b->GetSigmaTgl2());
      sigmad =TMath::Sqrt(sigmad)+0.04;
      if (pvertex->GetRr()>50){
	Double_t cov0[15],cov1[15];
	track0b->GetESDtrack()->GetInnerExternalCovariance(cov0);
	track1b->GetESDtrack()->GetInnerExternalCovariance(cov1);
	sigmad = cov0[0]+cov0[2]+cov1[0]+cov1[2]+
	  (80.-pvertex->GetRr())*(80.-pvertex->GetRr())*(cov0[5]+cov0[9])+
	  (80.-pvertex->GetRr())*(80.-pvertex->GetRr())*(cov1[5]+cov1[9]);
	sigmad =TMath::Sqrt(sigmad)+0.05;
      }
      //       
      IlcV0 vertex2;
      vertex2.SetM(*track0b);
      vertex2.SetP(*track1b);
      vertex2.Update(primvertex);
      if (vertex2.GetDist2()<=pvertex->GetDist2()&&(vertex2.GetPointAngle()>=pvertex->GetPointAngle())){
	pvertex->SetM(*track0b);
	pvertex->SetP(*track1b);
	pvertex->Update(primvertex);
	pvertex->SetClusters(track0b->ClIndex(),track1b->ClIndex());  // register clusters
	pvertex->SetIndex(0,track0->GetESDtrack()->GetID());
	pvertex->SetIndex(1,track1->GetESDtrack()->GetID());
      }
      pvertex->SetDistSigma(sigmad);
      pvertex->SetDistNorm(pvertex->GetDist2()/sigmad);       
      pvertex->SetNormDCAPrim(normdist[itrack0],normdist[itrack1]);
      //
      // define likelihhod and causilcties
      //
      Float_t pa0=1, pa1=1, pb0=0.26, pb1=0.26;      
      if (maxLayer<1){
	Float_t fnorm0 = normdist[itrack0];
	if (fnorm0<0) fnorm0*=-3;
	Float_t fnorm1 = normdist[itrack1];
	if (fnorm1<0) fnorm1*=-3;
 	if (pvertex->GetAnglep()[2]>0.1 ||  (pvertex->GetRr()<10.5)&& pvertex->GetAnglep()[2]>0.05 || pvertex->GetRr()<3){
 	  pb0    =  TMath::Exp(-TMath::Min(fnorm0,Float_t(16.))/12.);
 	  pb1    =  TMath::Exp(-TMath::Min(fnorm1,Float_t(16.))/12.);
 	}
	pvertex->SetChi2Before(normdist[itrack0]);
	pvertex->SetChi2After(normdist[itrack1]);       
	pvertex->SetNAfter(0);
	pvertex->SetNBefore(0);
      }else{
	pvertex->SetChi2Before(minchi2before0);
	pvertex->SetChi2After(minchi2before1);
	 if (pvertex->GetAnglep()[2]>0.1 || ( pvertex->GetRr()<10.5 && pvertex->GetAnglep()[2]>0.05) || pvertex->GetRr()<3){
	   pb0    =  TMath::Exp(-TMath::Min(minchi2before0,Float_t(16))/12.);
	   pb1    =  TMath::Exp(-TMath::Min(minchi2before1,Float_t(16))/12.);
	 }
	 pvertex->SetNAfter(maxLayer);
	 pvertex->SetNBefore(maxLayer);      
      }
      if (pvertex->GetRr()<90){
		pa0  *= TMath::Min(track0->GetESDtrack()->GetTPCdensity(0,60),Double_t(1.));
		pa1  *= TMath::Min(track1->GetESDtrack()->GetTPCdensity(0,60),Double_t(1.));
      }
      if (pvertex->GetRr()<20){
		pa0  *= (0.2+TMath::Exp(-TMath::Min(minchi2after0,Float_t(16))/8.))/1.2;
		pa1  *= (0.2+TMath::Exp(-TMath::Min(minchi2after1,Float_t(16))/8.))/1.2;
      }
      //
      pvertex->SetCausality(pb0,pb1,pa0,pa1);
      //
      //  Likelihood calculations  - apply cuts
      //         
      Bool_t v0OK = kTRUE;
      Float_t p12 = pvertex->GetParamP()->GetParameter()[4]*pvertex->GetParamP()->GetParameter()[4];
      p12        += pvertex->GetParamN()->GetParameter()[4]*pvertex->GetParamN()->GetParameter()[4];
      p12         = TMath::Sqrt(p12);                             // "mean" momenta
      Float_t    sigmap0   = 0.0001+0.001/(0.1+pvertex->GetRr()); 
      Float_t    sigmap    = 0.5*sigmap0*(0.6+0.4*p12);           // "resolution: of point angle - as a function of radius and momenta

#ifdef WIN32
	  Float_t causilctyA  = (1.0-pvertex->GetCausalityP()[0])*(1.0-pvertex->GetCausalityP()[1]);
      Float_t causilctyB  = TMath::Sqrt(TMath::Min(pvertex->GetCausalityP()[2],Double_t(0.7))*
					TMath::Min(pvertex->GetCausalityP()[3],Double_t(0.7)));
#else
      Float_t causilctyA  = (1.0-pvertex->GetCausalityP()[0])*(1.0-pvertex->GetCausalityP()[1]);
      Float_t causilctyB  = TMath::Sqrt(TMath::Min(pvertex->GetCausalityP()[2],Float_t(0.7))*
					TMath::Min(pvertex->GetCausalityP()[3],Double_t(0.7)));
#endif
      //
      Float_t likelihood0 = (TMath::Exp(-pvertex->GetDistNorm())+0.1) *(pvertex->GetDist2()<0.5)*(pvertex->GetDistNorm()<5);

      Float_t likelihood1 = TMath::Exp(-(1.0001-pvertex->GetPointAngle())/sigmap)+
	0.4*TMath::Exp(-(1.0001-pvertex->GetPointAngle())/(4.*sigmap))+
	0.4*TMath::Exp(-(1.0001-pvertex->GetPointAngle())/(8.*sigmap))+
	0.1*TMath::Exp(-(1.0001-pvertex->GetPointAngle())/0.01);
      //
      if (causilctyA<kCausality0Cut)                                          v0OK = kFALSE;
      if (TMath::Sqrt(likelihood0*likelihood1)<kLikelihood01Cut)              v0OK = kFALSE;
      if (likelihood1<kLikelihood1Cut)                                        v0OK = kFALSE;
      if (TMath::Power(likelihood0*likelihood1*causilctyB,0.33)<kCombinedCut) v0OK = kFALSE;
      
      //
      //
      if (kFALSE){	
	Bool_t gold = TMath::Abs(TMath::Abs(track0->GetLabel())-TMath::Abs(track1->GetLabel()))==1;
	cstream<<"It0"<<
	  "Tr0.="<<track0<<                       //best without constrain
	  "Tr1.="<<track1<<                       //best without constrain  
	  "Tr0B.="<<track0b<<                     //best without constrain  after vertex
	  "Tr1B.="<<track1b<<                     //best without constrain  after vertex 
	  "Tr0C.="<<htrackc0<<                    //best with constrain     if exist
	  "Tr1C.="<<htrackc1<<                    //best with constrain     if exist
	  "Tr0L.="<<track0l<<                     //longest best           
	  "Tr1L.="<<track1l<<                     //longest best
	  "Esd0.="<<track0->GetESDtrack()<<           // esd track0 params
	  "Esd1.="<<track1->GetESDtrack()<<           // esd track1 params
	  "V0.="<<pvertex<<                       //vertex properties
	  "V0b.="<<&vertex2<<                       //vertex properties at "best" track
	  "ND0="<<normdist[itrack0]<<             //normilcze distance for track0
	  "ND1="<<normdist[itrack1]<<             //normilcze distance for track1
	  "Gold="<<gold<<                         //
	  //	  "RejectBase="<<rejectBase<<             //rejection in First itteration
	  "OK="<<v0OK<<
	  "rmin="<<rmin<<
	  "sigmad="<<sigmad<<
	  "\n";
      }      
      //if (rejectBase) continue;
      //
      pvertex->SetStatus(0);
      //      if (rejectBase) {
      //	pvertex->SetStatus(-100);
      //}
      if (pvertex->GetPointAngle()>kMinPointAngle2) {
// 	  pvertex->SetESDindexes(track0->GetESDtrack()->GetID(),track1->GetESDtrack()->GetID());
          pvertex->SetIndex(0,track0->GetESDtrack()->GetID());
          pvertex->SetIndex(1,track1->GetESDtrack()->GetID());
          if (v0OK){
	  //	  IlcV0vertex vertexjuri(*track0,*track1);
	  //	  vertexjuri.SetESDindexes(track0->fESDtrack->GetID(),track1->fESDtrack->GetID());
	  //	  event->AddV0(&vertexjuri);
	  pvertex->SetStatus(100);
	}
        pvertex->SetOnFlyStatus(kTRUE);
	event->AddV0(pvertex);
      }
    }
  }
  //
  //
  // delete temporary arrays
  //  
//   Info("FindV02","Number of V0: %d\n",event->GetNumberOfV0s());
  delete[] minPointAngle;
  delete[] maxr;
  delete[] minr;
  delete[] norm;
  delete[] normdist;
  delete[] normdist1;
  delete[] normdist0;
  delete[] dist;
  delete[] itsmap;
  delete[] helixes;
  delete   pvertex;

  IlcDebug(5,"End V0 Finding");

}


void IlcTARGETtrackerMI::RefitV02(IlcESDEvent *event)
{
  //
  //try to refit  V0s in the third path of the reconstruction
  //
  TTreeSRedirector &cstream = *fDebugStreamer;
  //
  Int_t  nv0s = event->GetNumberOfV0s();
  Float_t primvertex[3]={GetX(),GetY(),GetZ()};
  IlcV0 v0temp;
  for (Int_t iv0 = 0; iv0<nv0s;iv0++){
    IlcV0 * v0mi = (IlcV0*)event->GetV0(iv0);
    if (!v0mi) continue;
    Int_t     itrack0   = v0mi->GetIndex(0);
    Int_t     itrack1   = v0mi->GetIndex(1);
    IlcESDtrack *esd0   = event->GetTrack(itrack0);
    IlcESDtrack *esd1   = event->GetTrack(itrack1);
    if (!esd0||!esd1) continue;
    IlcTARGETtrackMI tpc0(*esd0);  
    IlcTARGETtrackMI tpc1(*esd1);
    Double_t alpha =TMath::ATan2(v0mi->GetXr(1),v0mi->GetXr(0));
    if (v0mi->GetRr()>85){
      if (tpc0.Propagate(alpha,v0mi->GetRr())&&tpc1.Propagate(alpha,v0mi->GetRr())){
	v0temp.SetM(tpc0);
	v0temp.SetP(tpc1);
	v0temp.Update(primvertex);
	if (kFALSE) cstream<<"Refit"<<
	  "V0.="<<v0mi<<
	  "V0refit.="<<&v0temp<<
	  "Tr0.="<<&tpc0<<
	  "Tr1.="<<&tpc1<<
	  "\n";
	if (v0temp.GetDist2()<v0mi->GetDist2() || v0temp.GetPointAngle()>v0mi->GetPointAngle()){
	  v0mi->SetM(tpc0);
	  v0mi->SetP(tpc1);
	  v0mi->Update(primvertex);
	}
      }
      continue;
    }
    if (v0mi->GetRr()>35){
      CorrectForDeadZoneMaterial(&tpc0);
      CorrectForDeadZoneMaterial(&tpc1);
      if (tpc0.Propagate(alpha,v0mi->GetRr())&&tpc1.Propagate(alpha,v0mi->GetRr())){
	v0temp.SetM(tpc0);
	v0temp.SetP(tpc1);
	v0temp.Update(primvertex);
	if (kFALSE) cstream<<"Refit"<<
	  "V0.="<<v0mi<<
	  "V0refit.="<<&v0temp<<
	  "Tr0.="<<&tpc0<<
	  "Tr1.="<<&tpc1<<
	  "\n";
	if (v0temp.GetDist2()<v0mi->GetDist2() || v0temp.GetPointAngle()>v0mi->GetPointAngle()){
	  v0mi->SetM(tpc0);
	  v0mi->SetP(tpc1);
	  v0mi->Update(primvertex);
	}	
      }
      continue;
    }
    CorrectForDeadZoneMaterial(&tpc0);
    CorrectForDeadZoneMaterial(&tpc1);    
    //    if (tpc0.Propagate(alpha,v0mi->GetRr())&&tpc1.Propagate(alpha,v0mi->GetRr())){
    if (RefitAt(v0mi->GetRr(),&tpc0, v0mi->GetClusters(0)) && RefitAt(v0mi->GetRr(),&tpc1, v0mi->GetClusters(1))){
      v0temp.SetM(tpc0);
      v0temp.SetP(tpc1);
      v0temp.Update(primvertex);
      if (kFALSE) cstream<<"Refit"<<
	"V0.="<<v0mi<<
	"V0refit.="<<&v0temp<<
	"Tr0.="<<&tpc0<<
	"Tr1.="<<&tpc1<<
	"\n";
      if (v0temp.GetDist2()<v0mi->GetDist2() || v0temp.GetPointAngle()>v0mi->GetPointAngle()){
	v0mi->SetM(tpc0);
	v0mi->SetP(tpc1);
	v0mi->Update(primvertex);
      }	
    }    
  }
}








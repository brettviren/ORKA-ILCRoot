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

/* $Id: IlcTARGETtrackerSA.cxx,v 1.11 2009/02/19 10:58:11 cgatto Exp $ */

////////////////////////////////////////////////////
//  Stand alone tracker class                     //
//  Origin:  Elisabetta Crescio                   //
//  e-mail:  crescio@to.infn.it                   //
//  tracks are saved as IlcTARGETtrackV2 objects     //
////////////////////////////////////////////////////

#include <stdlib.h>
#include <map>

#include <TArrayI.h>
#include <TBranch.h>
#include <TObjArray.h>
#include <TTree.h>
#include<vector>
#include<algorithm>
#include<utility>

#include "IlcESDEvent.h"
#include "IlcESDVertex.h"
#include "IlcESDtrack.h"
#include "IlcTARGETVertexer.h"
#include "IlcTARGETclusterTable.h"
#include "IlcTARGETRecPoint.h"
#include "IlcTARGETgeom.h"
#include "IlcTARGETtrackSA.h"
#include "IlcTARGETtrackerSA.h"
#include "IlcRun.h"
#include "IlcTARGETParam.h"
#include "TStopwatch.h"
#include "TFile.h"

ClassImp(IlcTARGETtrackerSA)

//____________________________________________________________________________
IlcTARGETtrackerSA::IlcTARGETtrackerSA():IlcTARGETtrackerMI(),
fPhiEstimate(0),
fLambdac(0),
fPhic(0),
fCoef1(0),
fCoef2(0),
fCoef3(0),
fNloop(0),
fPhiWin(0),
fLambdaWin(0),
fVert(0),
fVertexer(0),
fGeom(0),
fListOfTracks(0),
fTARGETclusters(0),
fMinPoints(4),
fCluLayer(0),
fCluCoord(0){
  // Default constructor
  Init();
 
}
//____________________________________________________________________________
IlcTARGETtrackerSA::IlcTARGETtrackerSA(IlcTARGETgeom *geom):IlcTARGETtrackerMI(geom),
fPhiEstimate(0),
fLambdac(0),
fPhic(0),
fCoef1(0),
fCoef2(0),
fCoef3(0),
fNloop(0),
fPhiWin(0),
fLambdaWin(0),
fVert(0),
fVertexer(0),
fGeom(0),
fListOfTracks(0),
fTARGETclusters(0),
fMinPoints(4),
fCluLayer(0),
fCluCoord(0) 
{
  // Standard constructor (Vertex is known and passed to this obj.)
  Init();
  fVert = 0;
  fGeom = geom;
 
}

//____________________________________________________________________________
IlcTARGETtrackerSA::IlcTARGETtrackerSA(IlcTARGETgeom *geom, IlcESDVertex *vert):IlcTARGETtrackerMI(geom),
fPhiEstimate(0),
fLambdac(0),
fPhic(0),
fCoef1(0),
fCoef2(0),
fCoef3(0),
fNloop(0),
fPhiWin(0),
fLambdaWin(0),
fVert(vert),
fVertexer(0),
fGeom(0),
fListOfTracks(0),
fTARGETclusters(0),
fMinPoints(4),
fCluLayer(0),
fCluCoord(0) 
{
  // Standard constructor (Vertex is known and passed to this obj.)
  Init();
  fGeom = geom;
 
}

//____________________________________________________________________________
IlcTARGETtrackerSA::IlcTARGETtrackerSA(IlcTARGETgeom *geom, IlcTARGETVertexer *vertexer):IlcTARGETtrackerMI(geom),
fPhiEstimate(0),
fLambdac(0),
fPhic(0),
fCoef1(0),
fCoef2(0),
fCoef3(0),
fNloop(0),
fPhiWin(0),
fLambdaWin(0),
fVert(),
fVertexer(vertexer),
fGeom(0),
fListOfTracks(0),
fTARGETclusters(0),
fMinPoints(4),
fCluLayer(0),
fCluCoord(0)  
{
  // Standard constructor (Vertex is unknown - vertexer is passed to this obj)
  Init();
  fVertexer = vertexer;
  fGeom = geom;
 
}

//____________________________________________________________________________
IlcTARGETtrackerSA::IlcTARGETtrackerSA(const IlcTARGETtrackerSA& tracker):IlcTARGETtrackerMI(),
fPhiEstimate(tracker.fPhiEstimate),
fLambdac(tracker.fLambdac),
fPhic(tracker.fPhic),
fCoef1(tracker.fCoef1),
fCoef2(tracker.fCoef2),
fCoef3(tracker.fCoef3),
fNloop(tracker.fNloop),
fPhiWin(tracker.fPhiWin),
fLambdaWin(tracker.fLambdaWin),
fVert(tracker.fVert),
fVertexer(tracker.fVertexer),
fGeom(tracker.fGeom),
fListOfTracks(tracker.fListOfTracks),
fTARGETclusters(tracker.fTARGETclusters),
fMinPoints(tracker.fMinPoints),
fCluLayer(tracker.fCluLayer),
fCluCoord(tracker.fCluCoord) {
  // Copy constructor
  for(Int_t i=0;i<3;i++){
    fPoint1[i]=tracker.fPoint1[i];
    fPoint2[i]=tracker.fPoint2[i];
    fPoint3[i]=tracker.fPoint3[i];
    fPointc[i]=tracker.fPointc[i];
  }
  for(Int_t i=0;i<4;i++)
    fLayer[i]=tracker.fLayer[i];
  

  if(tracker.fVertexer && tracker.fVert){
    fVert = new IlcESDVertex(*tracker.fVert);
  }
  else {
    fVert = tracker.fVert;
  }
  int nlayers=fParam->GetNlayers()+fParam->GetNendcaps()+fParam->GetNlayersStrip()+fParam->GetNendcapsStrip();
  for(Int_t i=0;i<nlayers;i++){
    fCluLayer[i] = tracker.fCluLayer[i];
    fCluCoord[i] = tracker.fCluCoord[i];
  } 
}
//______________________________________________________________________
IlcTARGETtrackerSA& IlcTARGETtrackerSA::operator=(const IlcTARGETtrackerSA& source){
    // Assignment operator. 
  this->~IlcTARGETtrackerSA();
  new(this) IlcTARGETtrackerSA(source);
  return *this;
 
}

//____________________________________________________________________________
IlcTARGETtrackerSA::~IlcTARGETtrackerSA(){
  // destructor
  // if fVertexer is not null, the IlcESDVertex obj. is owned by this class
  // and is deleted here
  if(fVertexer){
    if(fVert)delete fVert;
  }
  fVert = 0;
  fVertexer = 0;
 
  if(fPhiWin)delete []fPhiWin;
  if(fLambdaWin)delete []fLambdaWin;
  fListOfTracks->Delete();
  int nlayers=fParam->GetNlayers()+fParam->GetNendcaps()+fParam->GetNlayersStrip()+fParam->GetNendcapsStrip();
  if(fCluLayer){
    for(Int_t i=0;i<nlayers;i++){
      if(fCluLayer[i]){
	fCluLayer[i]->Delete();
	delete fCluLayer[i];
      }
    }
    delete [] fCluLayer;
  }
  if(fCluCoord){
    for(Int_t i=0;i<nlayers;i++){
      if(fCluCoord[i]){
	fCluCoord[i]->Delete();
	delete fCluCoord[i];
      }
    }
    delete [] fCluCoord;
  }
  
  fGeom = 0; 
}

//____________________________________________________________________________
void IlcTARGETtrackerSA::Init(){
  //  Reset all data members
    fPhiEstimate=0;
    for(Int_t i=0;i<3;i++){fPoint1[i]=0;fPoint2[i]=0;fPoint3[i]=0;}
    for(Int_t i=0;i<4;i++) fLayer[i]=0;

    fLambdac=0;
    fPhic=0;
    fCoef1=0;
    fCoef2=0;
    fCoef3=0;
    fPointc[0]=0;
    fPointc[1]=0;
    fPointc[2]=0;
    fVert = 0;
    fVertexer = 0;
    fGeom = 0;
    SetWindowSizes();
    fTARGETclusters = 0;
    SetMinPoints(4);
    fListOfTracks=new TObjArray(0,0);
    fCluLayer = 0;
    fCluCoord = 0;
 }
//_______________________________________________________________________
void IlcTARGETtrackerSA::ResetForFinding(){
  //  Reset data members used in all loops during track finding
    fPhiEstimate=0;
    for(Int_t i=0;i<3;i++){fPoint1[i]=0;fPoint2[i]=0;fPoint3[i]=0;}
    for(Int_t i=0;i<4;i++) fLayer[i]=0;

    fLambdac=0;
    fPhic=0;
    fCoef1=0;
    fCoef2=0;
    fCoef3=0;
    fPointc[0]=0;
    fPointc[1]=0;
    fPointc[2]=0;
    fListOfTracks->Delete();
}

 

//______________________________________________________________________
Int_t IlcTARGETtrackerSA::FindTracks(IlcESDEvent* event){

// Track finder using the ESD object
  TStopwatch stime;stime.Start();

  //controllare numero cluster sui layer1 e 2 (morti?)
  //non trova tracce...controllare..

  if(!fTARGETclusters){
    Fatal("FindTracks","TARGET cluster tree is not accessed - Abort!!!\n Please use method SetClusterTree to pass the pointer to the tree\n");
    return -1;
  }
  
  //Reads event and mark clusters of traks already found, with flag kVXDin
  Int_t nbefore=event->GetNumberOfTracks();
   Int_t nentr=event->GetNumberOfTracks();
   while (nentr--) {
     IlcESDtrack *track=event->GetTrack(nentr);
     if (track->GetStatus()&IlcESDtrack::kVXDin==IlcESDtrack::kVXDin){
       Int_t idx[kMaxClusters];
       Int_t ncl = track->GetVXDclusters(idx);
       for(Int_t k=0;k<ncl;k++){
       	 IlcTARGETRecPoint* cll = (IlcTARGETRecPoint*)GetCluster(idx[k]);
	 cll->SetBit(kSAflag);
       }
     }
   }

   Double_t primaryVertex[3];
   event->GetVertex()->GetXYZ(primaryVertex);
   //Creates TClonesArray with clusters for each layer. The clusters already used
   //by IlcTARGETtrackerMI are not considered
   int nlayers=fParam->GetNlayers()+fParam->GetNendcaps()+fParam->GetNlayersStrip()+fParam->GetNendcapsStrip();
   Int_t *nclusters=new Int_t[nlayers];
   Int_t *dmar=new Int_t[nlayers];
   fClusterMap=new std::vector<std::vector<int> >[nlayers];
   fNphiMap=100;
   fNlamMap=100;
   for(int i=0;i<nlayers;i++){
     nclusters[i]=0;
     dmar[i]=0;
     fClusterMap[i].resize(fNphiMap*fNlamMap);
   };
   fCluLayer = new TClonesArray*[nlayers];
   fCluCoord = new TClonesArray*[nlayers];

   for(Int_t i=0;i<nlayers;i++){
     IlcTARGETlayer &layer=fgLayers[i];
     for(Int_t cli=0;cli<layer.GetNumberOfClusters();cli++){
       IlcTARGETRecPoint* cls = (IlcTARGETRecPoint*)layer.GetCluster(cli);
       if(cls->TestBit(kSAflag)==kTRUE) continue; //clusters used by TPC prol.
       if(cls->GetQ()==0) continue; //fake clusters dead zones
       nclusters[i]++;
     }
     dmar[i]=0;
     fCluLayer[i] = new TClonesArray("IlcTARGETRecPoint",nclusters[i]);
     fCluCoord[i] = new TClonesArray("IlcTARGETclusterTable",nclusters[i]);
   }

   
   Int_t * firstmod = new Int_t[nlayers];
   for(Int_t i=0;i<nlayers;i++){
     firstmod[i]=fGeom->GetModuleIndex(i+1,1,1);
   }
//     TClonesArray *ptr=new TClonesArray("IlcTARGETclusterTable");
//    TFile *ff=new TFile("targetcltable.root","recreate");
//    TTree *tr=new TTree("cltable","Treee ");
   
//    tr->Branch("cl", &ptr,32000,98);
   for(Int_t ilay=0;ilay<nlayers;ilay++){
     TClonesArray &clulay = *fCluLayer[ilay];
     TClonesArray &clucoo = *fCluCoord[ilay];
     IlcTARGETlayer &layer=fgLayers[ilay];
     for(Int_t cli=0;cli<layer.GetNumberOfClusters();cli++){
       IlcTARGETRecPoint* cls = (IlcTARGETRecPoint*)layer.GetCluster(cli);
       if(cls->TestBit(kSAflag)==kTRUE) continue;
       if(cls->GetQ()==0) continue;
       Double_t phi=0;Double_t lambda=0;
       Float_t x=0;Float_t y=0;Float_t z=0;
       Float_t sx=0;Float_t sy=0;Float_t sz=0;
       Int_t module = cls->GetDetectorIndex()+firstmod[ilay];
       GetCoorAngles(cls,module,phi,lambda,x,y,z,primaryVertex);
       GetCoorErrors(cls,module,sx,sy,sz);
       int mapindex=GetMapIndex(phi,lambda);
       new (clulay[dmar[ilay]]) IlcTARGETRecPoint(*cls);
       new (clucoo[dmar[ilay]]) IlcTARGETclusterTable(x,y,z,sx,sy,sz,phi,lambda,cli,mapindex);
       fClusterMap[ilay][mapindex].push_back(dmar[ilay]);
       dmar[ilay]++;
     }
//      tr->SetBranchAddress("cl",&fCluCoord[ilay]);
//      tr->Fill();
   }
//    tr->Print();
//    tr->Write();
//    ff->Write();
//    delete ff;

   if(IlcDebugLevelClass()>5)
     for(int i=0;i<nlayers;i++){
       cout<<"Layer "<<i<<" nclust "<<nclusters[i]<<" dmar "<<dmar[i]<<endl;
     };

   delete[] nclusters;
   delete[] dmar;
  //Get primary vertex
   if(IlcDebugLevelClass()>0){
     cout<<"Loaded Clusters: ";
     stime.Print();stime.Continue();
   }
   
   Int_t ntrack=0;
   int nfind=0;
   //loop on the different windows
   for(Int_t nloop=0;nloop<fNloop;nloop++){
     for(int blayer=0;blayer<nlayers;blayer++){
       if(IlcDebugLevelClass()>0){
	 cout<<"Loop "<<nloop<<" from "<<fNloop<<" first layer "<<blayer<<" ncl " <<fCluLayer[blayer]->GetEntries()
	     <<" win "<<fPhiWin[nloop]<<" "<<fLambdaWin[nloop]<<" ";
	 stime.Print();stime.Continue();
       }
       for(Int_t ncl=0;ncl<fCluLayer[blayer]->GetEntriesFast();ncl++){ //loop starting from layer 0
	 int nclbeforeFind=fCluLayer[blayer]->GetEntriesFast();
	 nfind=0;
	 if(IlcDebugLevelClass()>5)
	   cout<<"Loop "<<nloop<<" from "<<fNloop<<" first layer "<<blayer<<" first cluster "<<ncl<<" from "<<fCluLayer[blayer]->GetEntriesFast()<< endl;
	 ResetForFinding();
	 Int_t pflag=0;
	 
	 IlcTARGETRecPoint* cl = (IlcTARGETRecPoint*)fCluLayer[blayer]->UncheckedAt(ncl);
	 
	 if(!cl) continue;
	 
	 
	 if (cl->GetQ()<=0) continue;
       
	 IlcTARGETclusterTable* arr = (IlcTARGETclusterTable*)GetClusterCoord(blayer,ncl); 
	 fPhic = arr->GetPhi();
	 fLambdac = arr->GetLambda();
// 	 if (TMath::Abs(fLambdac)>0.76*TMath::Pi()) continue;
	 fPhiEstimate = fPhic;
	 IlcTARGETtrackSA* trs = new IlcTARGETtrackSA(); 
	 fPoint1[0]=primaryVertex[0];
	 fPoint1[1]=primaryVertex[1];
	 fPoint1[2]=primaryVertex[2];
	 fLayer[0]=0;
	 
	 fPoint2[0]=arr->GetX();
	 fPoint2[1]=arr->GetY();
	 fPoint2[2]=arr->GetZ();
	 fLayer[1]=blayer;

	 Int_t * nn = new Int_t[nlayers];//counter for clusters on each layer
	 for(Int_t i=0;i<nlayers;i++){ nn[i]=0;}
	 for(int nlay=blayer;nlay<nlayers;nlay++){
	   if(nn[nlay?nlay-1:0]>0) nfind++;
	   if(nfind<2) pflag=0;
	   if(nfind==2) {
	     pflag=1;
	     fPoint3[0]=fPointc[0];
	     fPoint3[1]=fPointc[1];
	     fPoint3[2]=fPointc[2];
	     fLayer[2]=fLayer[3];
	   }
	   double wincoeff=1.;
	   if(nfind==0) wincoeff=TMath::Max(0.2,fPhiWin[0]/fPhiWin[nloop]);
	   if(nfind==1) wincoeff=TMath::Min(5.,fPhiWin[fNloop-1]/fPhiWin[nloop]);
	   if(nfind==2) wincoeff=TMath::Min(2.,fPhiWin[fNloop-1]/fPhiWin[nloop]);
	   if(nfind>2) { pflag=1; if(nn[nlay-1]!=0) UpdatePoints();}
	   nn[nlay] = SearchClusters(nlay,fPhiWin[nloop],fLambdaWin[nloop],trs,primaryVertex[2],pflag);
	 }
	 
	 Int_t layOK=0;
	 Int_t numberofpoints=fMinPoints;
	 for(Int_t nnp=0;nnp<nlayers;nnp++){    //of points) it is written on file
	   if(IlcDebugLevelClass()>5)
	     cout<<"nlayer ="<<nnp<<" nselected="<<nn[nnp]<<endl;
	   if(nn[nnp]!=0) layOK+=1;
	 }
	 if(layOK>=numberofpoints){
	   
	   IlcTARGETtrackV2* tr2 = FitTrack(trs,primaryVertex);
	   
	   if(tr2==0) continue;
	   
	   IlcESDtrack outtrack;
           outtrack.UpdateTrackParams(tr2,IlcESDtrack::kVXDin);
	   event->AddTrack(&outtrack);
	   ntrack++;
	   if(IlcDebugLevelClass()>5){
	     int nfreetotal=0;
	     for(int ilay=0;ilay<nlayers;ilay++){
	       nfreetotal+=fCluLayer[ilay]->GetEntries();
	       cout<<"Layer = "<<ilay<<" have free number of clusters = "<<fCluLayer[ilay]->GetEntries()<<endl;
	     };
	     cout<<" Total free number of clusters = "<<nfreetotal<<endl;
	   }
	 }
	 
	 
	 delete trs;
	 delete[] nn;
	 
	 //current cluster was removed and new one on same position
	 if(nclbeforeFind!=fCluLayer[blayer]->GetEntriesFast()) ncl--;
	 
       }//end loop on clusters of layer1
       
       //end loop2
     }
   }
   delete [] fClusterMap;
   delete [] firstmod;
   Info("FindTracks","Number of found standalone tracks: %d",event->GetNumberOfTracks()-nbefore);
   return 0;
   
}
 




//________________________________________________________________________

IlcTARGETtrackV2* IlcTARGETtrackerSA::FitTrack(IlcTARGETtrackSA* tr,Double_t */*primaryVertex*/){
  //fit of the found track
  int debugLevel=IlcDebugLevelClass();

  int nlayers=fParam->GetNlayers()+fParam->GetNendcaps()+fParam->GetNlayersStrip()+fParam->GetNendcapsStrip();
  Int_t firstmod[kMaxLayer];
  for(Int_t i=0;i<nlayers;i++){
    firstmod[i]=fGeom->GetModuleIndex(i+1,1,1);
  }  

  Int_t nclusters = tr->GetNumberOfClustersSA();
  if(debugLevel>=2) IlcDebug(2,Form("Fit track ncluster=%i ",nclusters));
  TObjArray* listlayer[kMaxLayer];
  for(Int_t i=0;i<nlayers;i++){
    listlayer[i] = new TObjArray(0,0);
  }

  TArrayI clind[kMaxLayer];
  for(int i=0;i<nlayers;i++) clind[i].Set(20);

  TArrayI mark[kMaxLayer];
  for(int i=0;i<nlayers;i++) mark[i].Set(20);
  
  Int_t nnn[kMaxLayer];
  Int_t kkk[kMaxLayer];
  for(Int_t i=0;i<nlayers;i++) {nnn[i]=0;kkk[i]=0;}
  
  for(Int_t ncl=0;ncl<nclusters;ncl++){
    Int_t index = tr->GetClusterIndexSA(ncl);
    IlcTARGETRecPoint* cl = (IlcTARGETRecPoint*)GetCluster(index);
    if(!cl->IsUsed()) cl->Use();
    if(cl->TestBit(kSAflag)==kTRUE) cl->ResetBit(kSAflag);
    Int_t lay = (index & kClusterLayerMask) >> kClusterLayerShift;
    listlayer[lay]->AddLast(cl); clind[lay][nnn[lay]]=index;nnn[lay]++;
  }

  for(Int_t nlay=0;nlay<nlayers;nlay++){
    for(Int_t ncl=0;ncl<tr->GetNumberOfMarked(nlay);ncl++){
      Int_t mark_c = tr->GetClusterMark(nlay,ncl);
      mark[nlay][kkk[nlay]]=mark_c;kkk[nlay]++;
    }
  }


 
  Int_t end[kMaxLayer];
  for(Int_t i=0;i<nlayers;i++){
    if(listlayer[i]->GetEntriesFast()==0) end[i]=1;
    else end[i]=listlayer[i]->GetEntriesFast();
  }

//   TClonesArray* listSA = new TClonesArray("IlcTARGETtrackSA");
//   TClonesArray &tri = *listSA;
//   Int_t nlist=0;
  Int_t l[kMaxLayer];

  IlcTARGETRecPoint* cl[kMaxLayer];

  Double_t x1,y1,z1,sx1,sy1,sz1;
  Double_t x2,y2,z2,sx2,sy2,sz2;
  Double_t x3,y3,z3,sx3,sy3,sz3;
  IlcTARGETRecPoint* p1=0;
  IlcTARGETRecPoint* p2=0;
  IlcTARGETRecPoint* p3=0;
  Int_t index1=0;
  Int_t index2=0;
  Int_t index3=0;
  Int_t mrk1 = 0;
  Int_t mrk2 = 0;
  Int_t mrk3 = 0;
  Int_t lay1=0;
  Int_t lay2=1;
  Int_t lay3=0;
  Int_t module1=-1;
  

  int ncombination=1;
  for(int llayer=0;llayer<nlayers;llayer++) {
    ncombination*=end[llayer];
  };
  if(debugLevel>=5) IlcDebug(5,Form("Total number of combinations=%i",ncombination));


  for(int ll=0;ll<ncombination;ll++){
    int nfind=0;
    int curll=ll;
    for(int llayer=0;llayer<nlayers;llayer++){ 
      l[llayer]=curll%end[llayer];
      curll/=end[llayer];
      cl[llayer] = (IlcTARGETRecPoint*)listlayer[llayer]->UncheckedAt(l[llayer]);
      if(debugLevel>=5) IlcDebug(5,Form("layer %i n=%i cl=%p",llayer,l[llayer],cl[llayer]));
      if(cl[llayer]!=0) {
	nfind++;
	if(nfind==1){
	  index1=clind[llayer][l[llayer]];
	  mrk1 = mark[llayer][l[llayer]];
	  lay1=llayer;
	  p1=cl[llayer];
	  module1=cl[llayer]->GetDetectorIndex()+firstmod[llayer];
	}
	if(nfind==2){
	  index2=clind[llayer][l[llayer]];
	  mrk2 = mark[llayer][l[llayer]];
	  lay2=llayer;
	  p2=cl[llayer];	  
	}
	if(nfind==3){
	  index3=clind[llayer][l[llayer]];
	  mrk3 = mark[llayer][l[llayer]];
	  lay3=llayer;
	  p3=cl[llayer];	  
	}
      }
    }
	 
    Int_t cln1=mrk1;
    Int_t cln2=mrk2;
    Int_t cln3=mrk3;
    IlcTARGETclusterTable* arr = (IlcTARGETclusterTable*)GetClusterCoord(lay1,cln1);
    IlcTARGETclusterTable* arr1= (IlcTARGETclusterTable*)GetClusterCoord(lay2,cln2);
    IlcTARGETclusterTable* arr2= (IlcTARGETclusterTable*)GetClusterCoord(lay3,cln3);
    x1 = arr->GetX();
    x2 = arr1->GetX();
    x3 = arr2->GetX();
    y1 = arr->GetY();
    y2 = arr1->GetY();
    y3 = arr2->GetY();
    z1 = arr->GetZ();
    z2 = arr1->GetZ();
    z3 = arr2->GetZ();
    sx1 = arr->GetSx();
    sx2 = arr1->GetSx();
    sx3 = arr2->GetSx();
    sy1 = arr->GetSy();
    sy2 = arr1->GetSy();
    sy3 = arr2->GetSy();
    sz1 = arr->GetSz();
    sz2 = arr1->GetSz();
    sz3 = arr2->GetSz();
	
    Int_t layer,ladder,detector;
    fGeom->GetModuleId(module1,layer,ladder,detector);
    Float_t yclu1 = p1->GetY();
    Float_t zclu1 = p1->GetZ();
//     Double_t cv=Curvature(primaryVertex[0],primaryVertex[1],x1,y1,x2,y2);
    Double_t cv=Curvature(x1,y1,x2,y2,x3,y3);
    
    double dist=TMath::Sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1));
    double dphi=TMath::ASin(dist*0.5*cv);
    if(fabs(dist)<kAlmost0||fabs(dphi)<kAlmost0) continue;
//     cout<<x1<<" "<<y1<<" "<<x2<<" "<<y2<<
//       " dist "<<dist<<" dz "<<(z2-z1)<<" dphi "<<dphi<<endl;
    Double_t tgl2 = (z2-z1)/(fabs(cv)>1e-10?(2*dphi/cv):dist);
    Double_t phi2 = TMath::ATan2((y2-y1),(x2-x1))-dphi;

    static IlcTARGETtrackSA trac;
    trac = IlcTARGETtrackSA(fGeom,layer,ladder,detector,yclu1,zclu1,phi2,tgl2,cv,-1);
    for(int ilay=nlayers-1;ilay>=0;ilay--){
      //	    cout<<"ilay "<<ilay<<" "<<cl[ilay]<<" "<<l[ilay]<<endl;
      if(cl[ilay]!=0){
	//	      cout<<"mark "<<((clind[ilay][l[ilay]] & 0x0fffffff)>>0)<<" "<<mark[ilay][l[ilay]]<<endl;
	trac.AddClusterV2(ilay,(clind[ilay][l[ilay]] & kClusterCountMask)>>0);
	trac.AddClusterMark(ilay,mark[ilay][l[ilay]]);
      }
    };
    
    //fit with Kalman filter using IlcTARGETtrackerMI::RefitAt()
    
    
    static IlcTARGETtrackSA ot;
    ot = IlcTARGETtrackSA(trac);
    
    ot.ResetCovariance(10.);
    ot.ResetClusters();
    
    if(RefitAt(-fmaxR,&ot,&trac,true,true)&&ot.GetNumberOfClusters()>=fMinPoints){
      if(debugLevel>=5) 
	IlcDebug(5,Form("Refit done to outer chi2=%f",ot.GetChi2()));
      IlcTARGETtrackMI *otrack2 = new IlcTARGETtrackMI(ot);
      otrack2->ResetCovariance(10.); 
      otrack2->ResetClusters();
      if(RefitAt(fgLayers[0].GetRmin(),otrack2,&ot,false,true)&&otrack2->GetNumberOfClusters()>=fMinPoints) {
	if(debugLevel>=5) 
	  IlcDebug(5,Form("Refit done to inner chi2=%f",otrack2->GetChi2()));
	fListOfTracks->AddLast(otrack2);
// 	new (tri[nlist]) IlcTARGETtrackSA(*trac);
// 	nlist++;
      } else {
	delete otrack2;
      }
      
    }       
    
//     delete ot;
//     delete trac;
  }

  for(Int_t ncl=0;ncl<nclusters;ncl++){
    Int_t index = tr->GetClusterIndexSA(ncl);
    IlcTARGETRecPoint* cll = (IlcTARGETRecPoint*)GetCluster(index);
    cll->UnUse();
  }

  Int_t dim=fListOfTracks->GetEntriesFast();
  if(dim==0){
    for(Int_t i=0;i<nlayers;i++){
      delete listlayer[i];
    }
    return 0;
  }

  Int_t lowchi2 = FindTrackLowChiSquare(fListOfTracks,dim);
  if(debugLevel>=5) 
    IlcDebug(5,Form("Select candidate = %i from ncombination = %i",lowchi2,dim));

  IlcTARGETtrackMI* otrack =(IlcTARGETtrackMI*)fListOfTracks->UncheckedAt(lowchi2);
 
  if(otrack==0) {
    for(Int_t i=0;i<nlayers;i++){
      delete listlayer[i];
    }
    return 0;
  }

  otrack->SetLabel(-1);
  CookLabel(otrack,0.);
  //remove clusters of found track
  
  for(Int_t nind=0;nind<otrack->GetNumberOfClusters();nind++){
    Int_t indexc = otrack->GetClusterIndex(nind);
    IlcTARGETRecPoint *c=(IlcTARGETRecPoint *)GetCluster(indexc);
    if(!c->IsUsed()) c->Use();
    Int_t nlay = (indexc & kClusterLayerMask) >> kClusterLayerShift;
    Int_t localIndex= (indexc & kClusterCountMask); 
    int nclu=fCluCoord[nlay]->GetEntriesFast();
    for(int i=0;i<nclu;i++){
      IlcTARGETclusterTable *clt=(IlcTARGETclusterTable *)fCluCoord[nlay]->UncheckedAt(i);
      if(clt&&clt->GetOrInd()==localIndex){
        int index=clt->GetMapIndex();
	std::vector<int> &indexes=fClusterMap[nlay][index];
	unsigned int ii=0;
	for(ii=0;ii<indexes.size();ii++){
	  if(indexes[ii]==i) break;
	}
	if(ii>=indexes.size()||indexes.size()==0) {
	  cout.precision(10);
	  cout<<"I lost somewhere a RecPoint "<<nlay<<" "<<index<<" "<<i<<" "<<indexes.size()<<" "
              <<(clt->GetPhi()+TMath::Pi())/TMath::Pi()/2<<" "<<(clt->GetLambda()+TMath::Pi()/2)/TMath::Pi()<<endl;
	  break;
	}
	indexes[ii]=indexes[indexes.size()-1];
	indexes.pop_back();
 	fCluLayer[nlay]->RemoveAt(i);
 	fCluCoord[nlay]->RemoveAt(i);
	
	break;
      }
    }
    //    fCluLayer[nlay]->Compress();
    //    fCluCoord[nlay]->Compress();
  }

  for(Int_t i=0;i<nlayers;i++){
    delete listlayer[i];
  }
  return otrack;

}



//_______________________________________________________
Int_t IlcTARGETtrackerSA::SearchClusters(Int_t layer,Double_t phiwindow,Double_t lambdawindow, IlcTARGETtrackSA* trs,Double_t /*zvertex*/,Int_t pflag){
  //function used to to find the clusters associated to the track
  IlcTARGETlayer &lay = fgLayers[layer];
  Double_t r=lay.GetR();
  double sigmaPhi=fParam->GetSigmaRPhi(layer)/lay.GetR(),sigmaLam=fParam->GetSigmaZ(layer)/lay.GetR();
  if(pflag==1){ 
    double cx=lay.GetR(),cy=0;
    if(!lay.IsEndCap()){
      Float_t cx1,cx2,cy1,cy2;
      FindEquation(fPoint1[0],fPoint1[1],fPoint2[0],fPoint2[1],fPoint3[0],fPoint3[1],fCoef1,fCoef2,fCoef3);
      if (FindIntersection(fCoef1,fCoef2,fCoef3,-r*r,cx1,cy1,cx2,cy2)!=0){
	Double_t fiGl1=TMath::ATan2(cy1,cx1);
	Double_t fiGl2=TMath::ATan2(cy2,cx2);
	double dphi1=fiGl1-fPhic;
	if(dphi1> TMath::Pi()) dphi1-=2*TMath::Pi();
	if(dphi1<-TMath::Pi()) dphi1+=2*TMath::Pi();
	double dphi2=fiGl2-fPhic;
	if(dphi2> TMath::Pi()) dphi2-=2*TMath::Pi();
	if(dphi2<-TMath::Pi()) dphi2+=2*TMath::Pi();
	
	if(fabs(dphi1)<fabs(dphi2)){
	  fPhiEstimate=fiGl1;
	  cx=cx1;
	  cy=cy1;
	}else{
	  fPhiEstimate=fiGl2;
	  cx=cx2;
	  cy=cy2;
	}
	double fi2=TMath::ATan2(fPoint2[1]+fCoef2/2,fPoint2[0]+fCoef1/2);
	double fi3=TMath::ATan2(fPoint3[1]+fCoef2/2,fPoint3[0]+fCoef1/2);
	double fic=TMath::ATan2(cy+fCoef2/2,cx+fCoef1/2);
	double R=TMath::Sqrt(fabs(fCoef1*fCoef1/4+fCoef2*fCoef2/4-fCoef3));
	double dfi=fi3-fi2;
	if(dfi<-TMath::Pi()) dfi+=2*TMath::Pi();
	if(dfi>TMath::Pi())  dfi-=2*TMath::Pi();	
	if(fabs(dfi)<kAlmost0) return 0;
	double tgl=(fPoint3[2]-fPoint2[2])/dfi/R;

	double dfic=fic-fi3;
	if(dfic<-TMath::Pi()) dfic+=2*TMath::Pi();
	if(dfic>TMath::Pi())  dfic-=2*TMath::Pi();

	double cz=tgl*dfic*R+fPoint3[2];
	fLambdac=TMath::ATan(cz/TMath::Hypot(cx,cy));

      }
    }else{
      FindEquation(fPoint1[0],fPoint1[1],fPoint2[0],fPoint2[1],fPoint3[0],fPoint3[1],fCoef1,fCoef2,fCoef3);
      double fi2=TMath::ATan2(fPoint2[1]+fCoef2/2,fPoint2[0]+fCoef1/2);
      double fi3=TMath::ATan2(fPoint3[1]+fCoef2/2,fPoint3[0]+fCoef1/2);
      double R=TMath::Sqrt(fabs(fCoef1*fCoef1/4+fCoef2*fCoef2/4-fCoef3));
      if(R<kAlmost0) R=kVeryBig;
      double dfi=fi3-fi2;
      if(dfi<-TMath::Pi()) dfi+=2*TMath::Pi();
      if(dfi>TMath::Pi()) dfi-=2*TMath::Pi();


      if(fabs(dfi)<kAlmost0) return 0;
      double tgl=(fPoint3[2]-fPoint2[2])/dfi/R;
      if(fabs(tgl)<kAlmost0) return 0;
      double dphi=(lay.GetZ()-fPointc[2])/tgl/R;
      double fic=TMath::ATan2(fPointc[1]+fCoef2/2,fPointc[0]+fCoef1/2);
      double cs=TMath::Cos(fic+dphi),sn=TMath::Sin(fic+dphi);
      cx=-fCoef1/2+R*cs;
      cy=-fCoef2/2+R*sn;
      fPhiEstimate=TMath::ATan2(cy,cx);
      fLambdac=TMath::ATan(lay.GetZ()/TMath::Hypot(cx,cy));
    }

    sigmaPhi=fParam->GetSigmaRPhi(layer)/TMath::Hypot(cx,cy);sigmaPhi*=sigmaPhi;

    sigmaLam=fParam->GetSigmaZ(layer)/TMath::Hypot(cx,cy);sigmaLam*=sigmaLam;
    double rphidist2=((fPoint3[0]-fPoint2[0])*(fPoint3[0]-fPoint2[0])+(fPoint3[1]-fPoint2[1])*(fPoint3[1]-fPoint2[1]));
    if(rphidist2>fParam->GetSigmaRPhi(fLayer[2])*fParam->GetSigmaRPhi(fLayer[2])){
      sigmaPhi+=(2*fParam->GetSigmaRPhi(fLayer[2])*fParam->GetSigmaRPhi(fLayer[2])+
		 fParam->GetSigmaRPhi(fLayer[1])*fParam->GetSigmaRPhi(fLayer[1]))/
	((fPoint3[0]-fPoint2[0])*(fPoint3[0]-fPoint2[0])+(fPoint3[1]-fPoint2[1])*(fPoint3[1]-fPoint2[1]));
      sigmaLam+=(2*fParam->GetSigmaZ(fLayer[2])*fParam->GetSigmaZ(fLayer[2])+
		 fParam->GetSigmaZ(fLayer[1])*fParam->GetSigmaZ(fLayer[1]))/
	((fPoint3[0]-fPoint2[0])*(fPoint3[0]-fPoint2[0])+(fPoint3[1]-fPoint2[1])*(fPoint3[1]-fPoint2[1]));

    }
    sigmaPhi=TMath::Sqrt(sigmaPhi);
    sigmaLam=TMath::Sqrt(sigmaLam);

  }
  sigmaPhi=TMath::Hypot(0.0005,sigmaPhi);
  sigmaLam=TMath::Hypot(0.0005,sigmaLam);
  
  phiwindow*=3*sigmaPhi;
  lambdawindow*=3*sigmaLam;
  
  //Int_t ncl = fCluLayer[layer]->GetEntriesFast();

  std::vector<std::pair<double,int> > distwithindex;

  int debugLevel=IlcDebugLevelClass();

  int iphimin,iphimax,ilammin,ilammax;
  GetMapIndex(fPhiEstimate-phiwindow,fLambdac-lambdawindow,iphimin,ilammin);
  GetMapIndex(fPhiEstimate+phiwindow,fLambdac+lambdawindow,iphimax,ilammax);

  for(int iphi=iphimin;;iphi++){
    iphi=iphi%fNphiMap;
    for(int ilam=ilammin;ilam<=ilammax;ilam++){
//      cout<<"check "<<layer<<" "<<iphi*fNlamMap+ilam<<" "<<iphi<<" "<<fNphiMap<<" "<<ilam<<" "<<fNlamMap<<" "<<endl;
      std::vector<int>& indexes= fClusterMap[layer][iphi*fNlamMap+ilam];
//       cout<<indexes.size()<<endl;
      for(unsigned int in=0;in<indexes.size();in++){
	Int_t index=indexes[in];
// 	cout<<index<<" from "<<fCluLayer[layer]->GetEntriesFast()<<endl;
	//  for (Int_t index=0; index<ncl; index++) {
	IlcTARGETRecPoint *c = (IlcTARGETRecPoint*)fCluLayer[layer]->UncheckedAt(index);
	if (!c) continue;
	if (c->GetQ()<=0) continue;
	
	IlcTARGETclusterTable* arr = (IlcTARGETclusterTable*)GetClusterCoord(layer,index);
	if(debugLevel>=6)
	  IlcDebug(6,Form("layer=%i phi=%f est=%f window=%f , lambda=%f est=%f window=%f",
			  layer,arr->GetPhi(),fPhiEstimate,phiwindow,arr->GetLambda(),fLambdac,lambdawindow));
	Double_t dphi = TMath::Abs(arr->GetPhi()-fPhiEstimate);
	if(dphi>TMath::Pi()) dphi=TMath::Abs(dphi-2*TMath::Pi());
	if (dphi>phiwindow) continue;
	
	Double_t lambda = arr->GetLambda();
	if (TMath::Abs(lambda-fLambdac)>lambdawindow) continue;
	
	double dist=TMath::Hypot(dphi/phiwindow,(lambda-fLambdac)/lambdawindow);
	if(debugLevel>=5)
	  IlcDebug(6,Form("Have distance =%f",dist));
	distwithindex.push_back(std::pair<double,int>(dist,index));
      }
    }
    if(iphi==iphimax) break;
  }
  
  std::sort(distwithindex.begin(),distwithindex.end());

  for(unsigned int i=0;i<distwithindex.size();i++){
    if(trs->GetNumberOfClustersSA()>=trs->GetMaxNumberOfClusters()) return i;
    if(trs->GetNumberOfMarked(layer)>=trs->GetMaxNMarkedPerLayer(layer)) return i;
   
    IlcTARGETclusterTable* arr = (IlcTARGETclusterTable*)GetClusterCoord(layer,distwithindex[i].second);
    if(debugLevel>=5)
      IlcDebug(5,Form("Take cluster %i dist=%f, index=%i",i,distwithindex[i].first,distwithindex[i].second));

    if(i==0){
      fLambdac=arr->GetLambda();
      fPhiEstimate=arr->GetPhi();
      fPointc[0]=arr->GetX();
      fPointc[1]=arr->GetY();
      fPointc[2]=arr->GetZ();
      fLayer[3]=layer;
    }
    
    trs->AddClusterSA(layer,arr->GetOrInd());
    trs->AddClusterMark(layer,distwithindex[i].second);

  };
  
  return distwithindex.size();
}

//________________________________________________________________
void IlcTARGETtrackerSA::UpdatePoints(){
  //update of points for the estimation of the curvature  

  fPoint2[0]=fPoint3[0];
  fPoint2[1]=fPoint3[1];
  fPoint2[2]=fPoint3[2];
  fLayer[1]=fLayer[2];
  fPoint3[0]=fPointc[0];
  fPoint3[1]=fPointc[1];
  fPoint3[2]=fPointc[2];
  fLayer[2]=fLayer[3];

  
}

//___________________________________________________________________
Int_t IlcTARGETtrackerSA::FindEquation(Float_t x1, Float_t y1, Float_t x2, Float_t y2, Float_t x3, Float_t y3,Float_t& a, Float_t& b, Float_t& c){

   //given (x,y) of three recpoints (in global coordinates) 
   //returns the parameters a,b,c of circonference x*x + y*y +a*x + b*y +c

   Float_t den = (x3-x1)*(y2-y1)-(x2-x1)*(y3-y1);
   if(den==0) return 0;
   a = ((y3-y1)*(x2*x2+y2*y2-x1*x1-y1*y1)-(y2-y1)*(x3*x3+y3*y3-x1*x1-y1*y1))/den;
   b= -(y2+y1+
	(-(x2-x1)*(x3*x3+y3*y3-x1*x1-y1*y1)+(y2+y1)*(y3-y1)*(x2-x1)+
	 (x2*x2-x1*x1)*(x3-x1))/den
	);
   c = -x1*x1-y1*y1-a*x1-b*y1;
   return 1;
 }
//__________________________________________________________________________
 Int_t IlcTARGETtrackerSA::FindIntersection(Float_t a1, Float_t b1, Float_t c1, Float_t c2,Float_t& x1,Float_t& y1, Float_t& x2, Float_t& y2){
 
 //Finds the intersection between the circonference of the track and the circonference centered in (0,0) represented by one layer
 //c2 is -rlayer*rlayer

  if(a1==0) return 0;
 Double_t m = c2-c1; 
 Double_t aA = (b1*b1)/(a1*a1)+1;
 Double_t bB = (-2*m*b1/(a1*a1));
 Double_t cC = c2+(m*m)/(a1*a1);
 Double_t dD = bB*bB-4*aA*cC;
 if(dD<0) return 0;
 
 y1 = (-bB+TMath::Sqrt(dD))/(2*aA); 
 y2 = (-bB-TMath::Sqrt(dD))/(2*aA); 
 x1 = (c2-c1-b1*y1)/a1;
 x2 = (c2-c1-b1*y2)/a1;

 return 1; 
}
//____________________________________________________________________
Double_t IlcTARGETtrackerSA::Curvature(Double_t x1,Double_t y1,Double_t 
x2,Double_t y2,Double_t x3,Double_t y3){

  //calculates the curvature of track  
  Double_t den = (x3-x1)*(y2-y1)-(x2-x1)*(y3-y1);
  if(fabs(den)<kAlmost0) return 0;
  Double_t a = ((y3-y1)*(x2*x2+y2*y2-x1*x1-y1*y1)-(y2-y1)*(x3*x3+y3*y3-x1*x1-y1*y1))/den;
  //  Double_t b = -(x2*x2-x1*x1+y2*y2-y1*y1+a*(x2-x1))/(y2-y1);
  double b =  -(y2+y1)+(x2-x1)/den*((x3-x2)*(x3-x1)+(y3-y2)*(y3-y1));
  Double_t c = -x1*x1-y1*y1-a*x1-b*y1;

  if((a*a+b*b-4*c)<=0) return 0;
  Double_t rad = TMath::Sqrt(a*a+b*b-4*c)/2.;
  if(fabs(rad)<kAlmost0) return 0;
  
  if(den>0) rad*=-1;

  return 1/rad;
 
}


//____________________________________________________________________
Double_t IlcTARGETtrackerSA::ChoosePoint(Double_t p1, Double_t p2, Double_t pp){

  //Returns the point closest to pp

  Double_t diff1 = p1-pp;
  Double_t diff2 = p2-pp;
  
  if(TMath::Abs(diff1)<TMath::Abs(diff2)) fPhiEstimate=p1;
  else fPhiEstimate=p2;  
  return fPhiEstimate;
  
}


//_________________________________________________________________
Int_t IlcTARGETtrackerSA::FindTrackLowChiSquare(TObjArray* tracklist, Int_t dim) const {
  // returns track with lowes chi square  
  if(dim==1){
    //IlcTARGETtrackV2* trk = (IlcTARGETtrackV2*)tracklist->At(0);
    //return trk;
    return 0;
  }
  //if(dim==0) return 0;
  Double_t * chi2 = new Double_t[dim];
  Int_t * index = new Int_t[dim];
  for(Int_t i=0;i<dim;i++){
    IlcTARGETtrackV2* trk = (IlcTARGETtrackV2*)tracklist->UncheckedAt(i);
    chi2[i]=trk->GetChi2()/trk->GetNumberOfClusters()/2.-2*trk->GetNumberOfClusters();
    index[i]=i;
  }

  Int_t w=0;Double_t value;
  Int_t lp;
  while(w<dim){
    for(Int_t j=w+1;j<dim;j++){
      if(chi2[w]<chi2[j]){
        value=chi2[w];
        chi2[w]=chi2[j];
        chi2[j]=value;
        lp=index[w];
        index[w]=index[j];
        index[j]=lp;
      }
    }
    w++;
  }
  delete [] chi2;
  Int_t findindex= index[dim-1];
  delete [] index;
  return findindex;
}

//_____________________________________________________________________________
Int_t IlcTARGETtrackerSA::Label(Int_t* lb0, Int_t* lb1, Int_t* lb2,Int_t numberofpoints,double &fakeratio){

 
  //function used to assign label to the found track. If track is fake, the label is negative
  std::map<int,int> labels;
  for(int i=0;i<numberofpoints;i++) labels[lb0[i]]++;
  int maxlabel=-1,ntimes=0;
  for(std::map<int,int>::iterator it=labels.begin();it!=labels.end();it++){
    if((*it).second>ntimes){
      maxlabel=(*it).first;
      ntimes=(*it).second;
    }
  }
  Int_t lflag=0;

  for(Int_t i=0;i<numberofpoints;i++){
    if(lb0[i]==maxlabel || lb1[i]==maxlabel || lb2[i]==maxlabel) lflag++;
  }
  
  fakeratio=1.-double(lflag)/numberofpoints;

  if(lflag==numberofpoints) return maxlabel;
  else return -maxlabel;

  
}

//_____________________________________________________________________________
void IlcTARGETtrackerSA::SetWindowSizes(Int_t n, Double_t *phi, Double_t *lam){
  // Set sizes of the phi and lambda windows used for track finding
  fNloop = n;
  if(phi){ // user defined values
    fPhiWin = new Double_t[fNloop];
    fLambdaWin = new Double_t[fNloop];
    for(Int_t k=0;k<fNloop;k++){
      fPhiWin[k]=phi[k];
      fLambdaWin[k]=lam[k];
    }
  }
  else {  // default values
            
//     Double_t phid[33]   = {0.002,0.003,0.004,0.0045,0.0047,
// 			   0.005,0.0053,0.0055,0.006,
// 			   0.0063,0.0065,0.007,0.0073,
// 			   0.0075,0.0077,0.008,0.0083,0.0085,
// 			   0.0087,0.009,0.0095,0.0097,
// 			   0.01,0.0105,0.011,0.0115,0.012,0.0125,0.02,0.04,0.08,0.16,0.3};
//     Double_t lambdad[33] = {0.003,0.004,0.005,0.005,0.005,
// 			    0.005,0.005,0.006,
// 			    0.006,0.006,0.006,0.007,0.007,0.007,0.007,
// 			    0.007,0.007,0.007,0.007,0.007,0.007,0.007,
// 			    0.008,0.008,0.008,0.008,0.008,0.008,0.016,0.032,0.064,0.1,0.1};
    
//     if(fNloop!=33){
//       fNloop = 33;
//     }
    
    
//     fPhiWin = new Double_t[fNloop];
//     fLambdaWin = new Double_t[fNloop];
   
//     for(Int_t k=0;k<fNloop;k++){
//       fPhiWin[k]=phid[k]/phid[0];
//       fLambdaWin[k]=phid[k]/phid[0];
//     }

    fNloop=12;
    fPhiWin = new Double_t[fNloop];
    fLambdaWin = new Double_t[fNloop];
    for(Int_t k=0;k<fNloop;k++){
      fPhiWin[k]=TMath::Power(1.5,k);
      fLambdaWin[k]=TMath::Power(1.5,k);
    }
    
    
  }

}
//_______________________________________________________________________
void IlcTARGETtrackerSA::GetCoorAngles(IlcTARGETRecPoint* cl,Int_t module,Double_t &phi,Double_t &lambda, Float_t &x, Float_t &y,Float_t &z,Double_t* vertex){
  //Returns values of phi (azimuthal) and lambda angles for a given cluster

  int ind=cl->GetModule();
  if(module!=ind)
    IlcWarning(Form("Something wrong in module numeration in RecPoint=%i, but calculated=%i ",ind,module));
  float xyzLf[3]={0,0,0};
  fGeom->TrackingV2ToDetL(ind,cl->GetY(),cl->GetZ(),xyzLf[0],xyzLf[2]);
  double xyzL[3]={xyzLf[0],xyzLf[1],xyzLf[2]};
  double xyzG[3];
  fGeom->LtoG(ind,xyzL,xyzG);
  
  x=xyzG[0];
  y=xyzG[1];
  z=xyzG[2];
  
//   int layer=cl->GetLayer();
//   cout<<"layer "<<layer<<" ladder "<<module/fGeom->GetNdetectors(layer+1)<<" dtector "<<module%fGeom->GetNdetectors(layer+1)<<" module "<<module<<endl;
//   cout<<"y-z "<<cl->GetY()<<" "<<cl->GetZ()<<" local "<<xyzL[0]<<" "<<xyzL[1]<<" "<<xyzL[2]<<" global "<<xyzG[0]<<" "<<xyzG[1]<<" "<<xyzG[2]<<endl;

  phi=TMath::ATan2(y,x);
  lambda=TMath::ATan2(z-vertex[2],TMath::Sqrt((x-vertex[0])*(x-vertex[0])+(y-vertex[1])*(y-vertex[1])));

}

//________________________________________________________________________
void IlcTARGETtrackerSA::GetCoorErrors(IlcTARGETRecPoint* cl, Int_t module,Float_t &sx,Float_t &sy, Float_t &sz){

  //returns x,y,z of cluster in global coordinates

  int ind=cl->GetModule();
  if(module!=ind)
    IlcWarning(Form("Something wrong in module numeration in RecPoint=%i, but calculated=%i ",ind,module));
  float xyzLf[3]={0,0,0};
  fGeom->TrackingV2ToDetL(ind,TMath::Sqrt(cl->GetSigmaY2()),TMath::Sqrt(cl->GetSigmaZ2()),xyzLf[0],xyzLf[2]);
  double xyzL[3]={xyzLf[0],xyzLf[1],xyzLf[2]};
  double xyzG[3];
  fGeom->LtoG(ind,xyzL,xyzG);
  sx = TMath::Abs(xyzG[0]);
  sy = TMath::Abs(xyzG[1]);
  sz = TMath::Abs(xyzG[2]);


  /*Double_t rot[9];     fGeom->GetRotMatrix(module,rot);
  Int_t lay,lad,det; fGeom->GetModuleId(module,lay,lad,det);
 
  Double_t alpha=TMath::ATan2(rot[1],rot[0])+TMath::Pi();
  Double_t phi=TMath::Pi()/2+alpha;
  if (lay==1) phi+=TMath::Pi();

  Float_t cp=TMath::Cos(phi), sp=TMath::Sin(phi);

  sx = TMath::Sqrt(sp*sp*cl->GetSigmaY2());
  sy = TMath::Sqrt(cp*cp*cl->GetSigmaY2());
  sz = TMath::Sqrt(cl->GetSigmaZ2());
  */
}


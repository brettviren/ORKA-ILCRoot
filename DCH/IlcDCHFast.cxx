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

/* $Id: IlcDCHFast.cxx,v 1.21 2009/06/30 10:38:10 cgatto Exp $ */

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//  fast DCH cluster simulation                                              //
//                                                                          //
///////////////////////////////////////////////////////////////////////////////

#include <TParticle.h>
#include <TVector.h>

#include "IlcRunLoader.h"
#include "IlcRun.h"
#include "IlcMC.h"

#include "IlcDCHFast.h"
#include "IlcDCH.h"
#include "IlcDCHhit.h"
#include "IlcDCHParam.h"
#include "IlcDCHclusterizer.h"
#include "IlcDCHcluster.h"
#include "IlcDCHwireposition.h"
#include "IlcLog.h"

using namespace std; 


ClassImp(IlcDCHFast)

IlcDCHFast::IlcDCHFast(IlcDCHwireposition *lwireMatr, IlcDCHclusterizer *lclus)
{ 

  kExact=kFALSE;

  xtr = new IlcDCHXTREL();


  if(!lwireMatr) {
    wireMatr = new IlcDCHwireposition();
    localOpened = kTRUE;
  } else {
    wireMatr = lwireMatr;
    localOpened = kFALSE;
  }

  if(!lclus) clus = new IlcDCHclusterizer("clusterer", "DCH clusterizer");
  else clus = lclus;

}

IlcDCHFast::~IlcDCHFast()
{
 delete xtr;
// delete param;
 if(localOpened) {
   delete wireMatr;
   delete clus ;
 }

}

//_____________________________________________________________________________
void IlcDCHFast::Hits2Clusters(IlcRunLoader* runLoader, Int_t iev, Bool_t /*CheckQuality*/) const
{
  //--------------------------------------------------------
  // DCH simple cluster generator from hits
  // obtained from the DCH Fast Simulator
  // The point errors are taken from the parametrization
  // one track to one row cluster
  //--------------------------------------------------------       

  IlcLoader* loader = runLoader->GetLoader("DCHLoader");
  if (!loader) {
    IlcError("No DCH loader found");
    return;
  }
  if (!runLoader->GetIlcRun()) runLoader->LoadgIlc();
  IlcRun* ilcRun = runLoader->GetIlcRun();
  if (!ilcRun) {
    IlcError("Couldn't get IlcRun object");
    return;
  }
  IlcDCH* dch = (IlcDCH*) ilcRun->GetDetector("DCH");
  if (!dch) {
    IlcError("Couldn't get DCH detector");
    return;
  }
  IlcDCHParam* param = dch->GetParam();
  
  if (!param) {
    IlcError("No DCH parameters available");
    return;
  }
  
  //  IlcDCHParam* locparam = new IlcDCHParam();
  
  //---------------------------------------------------------------
  //  Get the access to the tracks 
  //---------------------------------------------------------------
  
  TTree *tH = loader->TreeH();
  if (tH == 0x0) IlcFatal("Can not find TreeH in folder");

  dch->SetTreeAddress();
  
  Int_t ntracks=tH->GetEntries();

  //Switch to the output file
  
  if (loader->TreeR() == 0x0) loader->MakeTree("R");
  
  IlcDebug(1,Form("param->GetTitle() = %s",param->GetTitle()));
  
  runLoader->CdGAFile();



  clus->Open(runLoader->GetFileName(), iev);
  clus->ResetRecPoints();

  Int_t nclusters=0; //cluster counter
  
  Float_t valarraymin[7];

  Double_t GIMP[3]={0,0,0};
  Double_t LIMP[3]={0,0,0};
  Double_t localC[3], globalC[3];
  Double_t localCo[3], globalCo[3];

  IlcDCHhit *dchHit; // pointer to a sigle DCH hit

  hitstype *hits = new hitstype;

  for(Int_t track=0;track<ntracks;track++){
    
    dch->ResetHits();
    
    tH->GetEvent(track);
    //
    //  Get number of the DCH hits
    //     
    
    dchHit = (IlcDCHhit*)dch->FirstHit(-1);
    Int_t ipart=-1;
    Int_t oldpartN=-100;
    Int_t prevCelId=-1;
    Int_t firstCelId=-1;
    // Loop over hits
    //
    for(int i=0;i<3;i++) localC[i]=-1000;

    while(dchHit){
      
      	

      
      Int_t isec = dchHit->GetSuperlayer(); 
      Int_t irow = dchHit->GetCelRing();
      Int_t padN = dchHit->GetWire();
      Int_t CelId= dchHit->GetCelId();
      
      if(firstCelId!=CelId)
	wireMatr->SelectWireDet(CelId);
//      Int_t flag=dchHit->GetTrkStat();
//        cout<<"hit "<<dchHit->Track()<<" "<<flag<<" "
// 	   <<CelId<<" "<<prevCelId<<" "<<firstCelId<<endl;
  
      if  (firstCelId!=CelId) {
        globalC[0]=dchHit->X();
        globalC[1]=dchHit->Y();
        globalC[2]=dchHit->Z();

        firstCelId=CelId;

	wireMatr->Global2Local(globalC,localC);
      }else if (firstCelId==CelId) {

        globalCo[0]=dchHit->X();
        globalCo[1]=dchHit->Y();
        globalCo[2]=dchHit->Z();
	
	wireMatr->Global2Local(globalCo,localCo);
	
	double len2=(pow((localCo[0]-localC[0]),2)+pow((localCo[1]-localC[1]),2));
	
	if(len2<1e-4){
	  dchHit = (IlcDCHhit*)dch->NextHit();
	  continue;
	}

	LIMP[0]=-((localCo[1]-localC[1] )*((localCo[0]*localC[1])-(localC[0]*localCo[1])))/len2;
	LIMP[1]=((localCo[0]-localC[0] )*((localCo[0]*localC[1])-(localC[0]*localCo[1])))/len2;
	LIMP[2]=-((localC[0]*(localCo[0]-localC[0])+localC[1]*(localCo[1]-localC[1]))/len2*
		  (localCo[2]-localC[2]))+localC[2];

	wireMatr->Local2Global(LIMP,GIMP);
	
	Double_t Ldistance=(TMath::Sqrt(pow(LIMP[0],2)+pow(LIMP[1],2)));
	
	valarraymin[0]=Ldistance;
	valarraymin[1]=GIMP[1];//Hity[findMin];
	valarraymin[2]=GIMP[2];//Hitz[findMin];
	valarraymin[3]=GIMP[0];//Hitx[findMin];
	valarraymin[4]=dchHit->GetTrackTime();
        valarraymin[5]=(Float_t) dchHit->GetCelId();
        valarraymin[6]=LIMP[2];

	if(IlcDebugLevelClass()>=2){
	  cout<<"Cell Id: "<<dchHit->GetCelId()<<" Track number: "<<track << " "<<dchHit->Track()<<" "
	      <<" "<<isec<<" "<<irow<<" "<<padN<<endl;
	  cout<<"dist "<<Ldistance<<" "<<localCo[0]<<" "<<localCo[1]<<" "<<localCo[2]
	      <<" prev "<<localC[0]<<" "<<localC[1]<<" "<<localC[2]
	      <<" closest "<<LIMP[0]<<" "<<LIMP[1]<<" "<<LIMP[2]<<endl;
	  cout<<"global "<<globalCo[0]<<" "<<globalCo[1]<<" "<<globalCo[2]
	      <<" closest "<<GIMP[0]<<" "<<GIMP[1]<<" "<<GIMP[2]<<endl;
	}

	localC[0] = localCo[0]; 
	localC[1] = localCo[1]; 
	localC[2] = localCo[2]; 
	
	

	ipart=dchHit->Track();
	
	if (prevCelId==CelId){	  
	  if(ipart==oldpartN){
	    int ele = (*hits)[std::pair<IntPair,IntPair>(IntPair(ipart,isec),IntPair(irow,padN))].size() -1;	    
	    std::vector <Float_t> *tempvalar=
	      &((*hits)[std::pair<IntPair,IntPair>(IntPair(ipart,isec),IntPair(irow,padN))].at(ele));
	    
	    if (valarraymin[0] < tempvalar->at(0)){
//cout<<"cambio "<<ipart<<" "<<isec<<" "<<tempisec<<" "<<irow<<" "<<padN<<" "<<ele<<endl;
	      tempvalar->at(0)=valarraymin[0];
	      tempvalar->at(1)=valarraymin[1];
	      tempvalar->at(2)=valarraymin[2];
	      tempvalar->at(3)=valarraymin[3];
    	      tempvalar->at(4)=valarraymin[4];
	      tempvalar->at(5)=valarraymin[5];
	      tempvalar->at(6)=valarraymin[6];

// 	      cout<<"Update RecPoint cellid="<<CelId<<" track="<<ipart<<endl;   
	    }
	    
	  }else{

	    (*hits)[std::pair<IntPair,IntPair>(IntPair(ipart,isec),IntPair(irow,padN))].push_back(std::vector<Float_t>(valarraymin,valarraymin+7));

	    oldpartN=ipart;
	  }
	}else{
	  (*hits)[std::pair<IntPair,IntPair>(IntPair(ipart,isec),IntPair(irow,padN))].push_back(std::vector<Float_t>(valarraymin,valarraymin+7));
	  
	  oldpartN=ipart;
	  prevCelId=CelId;
	}
      } dchHit = (IlcDCHhit*)dch->NextHit();   
    } // end of loop over hits

  } 
  // end of loop over tracks
//cout<<"-------------------- inizio clusterizzazione ----------------------\n";


  
  Float_t sigmaImp        = param->GetSigmaFrsTrk();//locparam->GetSigmaFrsTrk();
  Float_t sigmaz	  = param->GetMaxZResolution();//locparam->GetMaxZResolution();

  Float_t sigmaImp_2      = param->GetSigmaSecTrk();//locparam->GetSigmaSecTrk();

  Float_t sigmaImp2	= pow(sigmaImp,2);
  Float_t sigmaImp2_2	= pow(sigmaImp_2,2);
  Float_t sigmaz2	= pow(sigmaz,2);
  Float_t z;
  Float_t semizleng = param->GetLength();
    
  Float_t position[5],sigmas[5];
  Int_t tracklabel[3];
  Float_t DeltaTrkTime  = param->GetTimeBetweenTrk();//locparam->GetTimeBetweenTrk();
  bool  notprol = false;


  std::vector<Int_t> idtrack;
  std::vector<std::vector<Float_t> > *hvec;
  std::vector<Float_t> *valvec;
  std::vector<Float_t> *firsthit;
  std::vector<Float_t> *secondhit;
  Int_t idsec, idrow, idpad, idt[3],cellid;
  Float_t hittime,minhittime[2];

  vector<Int_t> cellidused; 
  for (hitstype::iterator hitpad=hits->begin();hitpad!=hits->end();hitpad++){

    cellid=((Int_t)(*hitpad).second[0].at(5));
    if(std::find(cellidused.begin(),cellidused.end(),cellid)!=cellidused.end())
    	continue;
    idtrack.clear();
    idtrack.push_back((*hitpad).first.first.first);
    idsec=(*hitpad).first.first.second;
    idrow=(*hitpad).first.second.first;
    idpad=(*hitpad).first.second.second;

    float xyzatendcap1[3],xyzatendcap2[3],xyztmp[3];
    wireMatr->SelectWireDet(int(cellid/1000)*1000);
    wireMatr->WirePosAtEndcap(xyzatendcap1,xyztmp);
    wireMatr->SelectWireDet(int(cellid/1000)*1000+1);
    wireMatr->WirePosAtEndcap(xyzatendcap2,xyztmp);
    float maxdrifttime=xtr->dist2time(TMath::Hypot(xyzatendcap1[0]-xyzatendcap2[0],
						   xyzatendcap1[1]-xyzatendcap2[1])/3.)[0];
//     cout<<cellid<<" "<<maxdrifttime<<endl;
    //cout<<"iter track "<<(*hitpad).first.first.first<<" superlayer "<<idsec<<" layer "<<idrow<<" wire "<<idpad
    //<<" cellId "<<((Int_t)(*hitpad).second[0].at(5))<<endl;
    minhittime[0]=10000.;
    minhittime[1]=10000.;
    firsthit=0x0;
    secondhit=0x0;
//     firsthitY=0x0;
//     secondhitY=0x0;
    idt[0]=-1;
    idt[1]=-1; 	
    for (hitstype::iterator hitpad1=hits->begin();hitpad1!=hits->end();hitpad1++){
      if(hitpad==hitpad1) {continue;}


      if( ( (idsec==(*hitpad1).first.first.second) && (idrow==(*hitpad1).first.second.first) && (idpad==(*hitpad1).first.second.second) ) ){

        idtrack.push_back((*hitpad1).first.first.first);

      }
    }

    for(UInt_t molt=0; molt<idtrack.size(); molt++){
      hvec = &((*hits)[std::pair<IntPair,IntPair>(IntPair(idtrack.at(molt),idsec),IntPair(idrow,idpad))]);
//       hvecY = &((*hitsY)[std::pair<IntPair,IntPair>(IntPair(idtrack.at(molt),idsec),IntPair(idrow,idpad))]);
      for(UInt_t nBend=0; nBend<hvec->size(); nBend++){
        valvec=&(hvec->at(nBend));
//         valvecY=&(hvecY->at(nBend));

	if(valvec->at(4)* 1.e+6>maxdrifttime) continue;
	z=valvec->at(2);
	if (z<=0) z=-z+semizleng;
        hittime=valvec->at(4)* 1.e+6 + z/29979245800.* 1.e+6 + xtr->dist2time(valvec->at(0))[0] ;
	if(hittime<minhittime[0]) {
	  if (minhittime[0]<10000. && (minhittime[0]-hittime)>=DeltaTrkTime){
	    minhittime[1]=minhittime[0];
	    secondhit=firsthit;
// 	    secondhitY=firsthitY;
	    idt[1]=idt[0];
	  }
	  minhittime[0]=hittime;
	  firsthit=valvec;
// 	  firsthitY=valvecY;
	  idt[0]=idtrack.at(molt);
	}
	else if (hittime<minhittime[1] && (hittime-minhittime[0])>=DeltaTrkTime){
	    minhittime[1]=hittime;
	    secondhit=valvec;
// 	    secondhitY=valvecY;
	    idt[1]=idtrack.at(molt);
	}
      }
    }

    if(!firsthit) continue;
    z=firsthit->at(2);

    position[0]=firsthit->at(3);
    position[1]=firsthit->at(1);
    position[2]=z;
    position[3]=firsthit->at(0);
    position[4]=firsthit->at(6);

    if(!kExact){
      double dz=gRandom->Gaus(0,sigmaz);
      position[2]=z+dz;
      position[3]=gRandom->Gaus(firsthit->at(0),sigmaImp);
      position[4]=position[4]+dz;
    }

    sigmas[0]=0.0;
    sigmas[1]=0.0;
    sigmas[2]=sigmaz2;
    sigmas[3]=sigmaImp2;
    sigmas[4]=0.0;

    tracklabel[0]=idt[0];
    tracklabel[1]=-1;
    tracklabel[2]=-1;
  
  
    cellidused.push_back((Int_t) firsthit->at(5));
    IlcDCHcluster *cl=clus->AddCluster(position,sigmas,tracklabel,((Int_t) firsthit->at(5)));
    cl->SetZLocal(position[4]);
    cl->SetZLocalexact(firsthit->at(6));
    cl->SetImPexact(firsthit->at(0));
    cl->SetTrackTime(firsthit->at(4));
    //cout<<"Second Id cell: "<<firsthit->at(5)<<" Track label: "<<tracklabel[0]<<endl;
    nclusters++;


    notprol=false;
    if(secondhit!=0x0){

      z=secondhit->at(2);

      position[0]=secondhit->at(3);
      position[1]=secondhit->at(1);
      position[2]=z;
      position[3]=secondhit->at(0);
      position[4]=secondhit->at(6);
      
      if(!kExact){
	double dz=gRandom->Gaus(0,sigmaz);
	position[2]=z+dz;
	position[3]=gRandom->Gaus(secondhit->at(0),sigmaImp_2);
	position[4]=position[4]+dz;
      }
      
      sigmas[0]=0.0;
      sigmas[1]=0.0;
      sigmas[2]=sigmaz2;
      sigmas[3]=sigmaImp2_2;
      sigmas[4]=0.0;


      tracklabel[0]=idt[1];
      tracklabel[1]=idt[0];
      tracklabel[2]=-1;
    
      cl=clus->AddCluster(position,sigmas,tracklabel,((Int_t) secondhit->at(5)));
      cl->SetZLocal(position[4]);
      cl->SetZLocalexact(secondhit->at(6));
      cl->SetImPexact(secondhit->at(0));
      cl->SetTrackTime(secondhit->at(4));
      //cout<<"3° Cell Id: "<<secondhit->at(5) <<" Track label: " <<tracklabel[0]<<endl; 
      
      nclusters++;

      notprol=false;
    }

    for(UInt_t molt=0; molt<idtrack.size(); molt++){
      hvec = &((*hits)[std::pair<IntPair,IntPair>(IntPair(idtrack.at(molt),idsec),IntPair(idrow,idpad))]);
      for(UInt_t nBend=0; nBend<hvec->size(); nBend++){
        valvec=&(hvec->at(nBend));
	if(valvec!=firsthit&&valvec!=secondhit){
	  position[0]=valvec->at(3);
	  position[1]=valvec->at(1);
	  position[2]=valvec->at(2);
	  position[3]=valvec->at(0);
	  position[4]=valvec->at(6);
	  
	  tracklabel[0]=idtrack.at(molt);
	  tracklabel[1]=idt[0];
	  tracklabel[2]=idt[1];
	  cl=clus->AddCluster(position,sigmas,tracklabel,((Int_t) valvec->at(5)));
	  cl->SetZLocal(position[4]);
	  cl->SetZLocalexact(valvec->at(6));
	  cl->SetImPexact(valvec->at(0));
	  cl->SetTrackTime(valvec->at(4));
	  cl->Use(100);
	  if(valvec->at(4)* 1.e+6>maxdrifttime) cl->Use(10);//will be 110 use
	}
      }
    }
  }
//cout<<"-------------------- finita clusterizzazione ----------------------\n";


  clus->WriteClusters();

  IlcInfo(Form("Number of made clusters : %d",nclusters));

  delete hits;
//   delete hitsY;

} // end of function



//_____________________________________________________________________________
void IlcDCHFast::Hits2ClustersOld(IlcRunLoader* runLoader, Int_t iev, Bool_t /*CheckQuality*/) const
{
  //--------------------------------------------------------
  // DCH simple cluster generator from hits
  // obtained from the DCH Fast Simulator
  // The point errors are taken from the parametrization
  // one track to one row cluster
  //--------------------------------------------------------       

  IlcLoader* loader = runLoader->GetLoader("DCHLoader");
  if (!loader) {
    IlcError("No DCH loader found");
    return;
  }
  if (!runLoader->GetIlcRun()) runLoader->LoadgIlc();
  IlcRun* ilcRun = runLoader->GetIlcRun();
  if (!ilcRun) {
    IlcError("Couldn't get IlcRun object");
    return;
  }
  IlcDCH* dch = (IlcDCH*) ilcRun->GetDetector("DCH");
  if (!dch) {
    IlcError("Couldn't get DCH detector");
    return;
  }
  IlcDCHParam* param = dch->GetParam();
  
    if (!param) {
    IlcError("No DCH parameters available");
    return;
  }
  
//  IlcDCHParam* locparam = new IlcDCHParam();

  //---------------------------------------------------------------
  //  Get the access to the tracks 
  //---------------------------------------------------------------
  
  TTree *tH = loader->TreeH();
  if (tH == 0x0) IlcFatal("Can not find TreeH in folder");

  dch->SetTreeAddress();
  
 Int_t ntracks=tH->GetEntries();

  //Switch to the output file
  
  if (loader->TreeR() == 0x0) loader->MakeTree("R");
  
  IlcDebug(1,Form("param->GetTitle() = %s",param->GetTitle()));
  
  runLoader->CdGAFile();



  clus->Open(runLoader->GetFileName(), iev);
  clus->ResetRecPoints();

std::vector<Float_t>distance;
std::vector<Float_t>Hitx;
std::vector<Float_t>Hity;
std::vector<Float_t>Hitz;

  Int_t findMin=-1;

  //Int_t c=0;
  Int_t flag=-1;
  Double_t minimo=100;

  Int_t nclusters=0; //cluster counter
  Int_t isec, irow, tempisec, centralPad, oldpartN, oldsecN, oldrowN, padN, oldpadN, ele, index[3];
  Float_t x[5],Xx[5],xm[5],Xxm[5],  xo[5],Xxo[5]; //entering, in volume, exitin coordinates 
  
  Float_t valarray[6],valarraymin[6];
  Float_t valarraym[6];
  Float_t valarrayo[6],  valarrayYmin[3];//valarrayYo[3]

  Float_t locz,    oldlocx, oldlocy,  oldlocz, dist, locx=0,  locy=0; //entering position
  Float_t locxm,   locym,   loczm; //in volume position
  Float_t locxo,   locyo,   loczo; //exiting position oldlocxo, oldlocyo,   oldloczo
  //Float_t locxmin, locymin, loczmin, oldlocxmin, oldlocymin, oldloczmin; //minimum position
  
  Float_t ppWidth, nppWidth, alfl, pady, oldpady, z;
  Float_t padym,padyo;
  
  Float_t semizleng = param->GetLength();
  bool  xcrosed;

    
  Double_t localC[3], globalC[3];
  Double_t localCm[3], globalCm[3];
  Double_t localCo[3], globalCo[3];

  IlcDCHhit *dchHit; // pointer to a sigle DCH hit
  Int_t ipart;


  hitstype *hits = new hitstype;
  hitstype *hitsY = new hitstype;

  std::vector <Float_t> *tempvalar;
  std::vector <Float_t> *tempvalarY;



  
  
  for(Int_t track=0;track<ntracks;track++){
    
    dch->ResetHits();
    
    tH->GetEvent(track);
    //
    //  Get number of the DCH hits
    //     
    
    dchHit = (IlcDCHhit*)dch->FirstHit(-1);
    ipart=-1;
    oldpartN=-100;
    oldsecN=-100;
    oldrowN=-100;
    oldpadN=-100;
    
    // Loop over hits
    //
    
    oldlocx = 0.;
    oldlocy = 0.;
    oldlocz = 0.;
    pady = 0.;
    oldpady = 0.;
    xcrosed = false;

    while(dchHit){
      
      	

      wireMatr->SelectWireDet(dchHit->GetCelId());
      
      isec=dchHit->GetSuperlayer(); 
      irow = dchHit->GetCelRing();
      padN = dchHit->GetWire();
      
      flag=dchHit->GetTrkStat();
  
  
if  (flag==1) {
//distance between entering position and wire(center)

        x[0]=dchHit->X();
	x[1]=dchHit->Y();
	x[2]=dchHit->Z();
	x[3]=1.0;
	x[4]=0.0;//dchHit->Time();	
		
        globalC[0]=x[0];
        globalC[1]=x[1];
        globalC[2]=x[2];

        index[0]=1;
	index[1]=isec;
	index[2]=0;
	
	
        
	ppWidth=0.0;  //param->GetPadPitchWidth(isec);
	nppWidth = 0.0;  //ppWidth * 4./3.;
	alfl = 0.0;  //ppWidth/3.;

	//padN = 0;  //TMath::Nint(x[1]+0.5);
 
        memcpy(Xx,x,5*sizeof(Float_t));
	valarray[1]=Xx[1];
	valarray[2]=Xx[2];
	valarray[3]=Xx[0];
	valarray[4]=Xx[4];
	
	valarray[5]=(Float_t) dchHit->GetCelId();
        
	centralPad = 0; //(param->GetNPads(isec,irow))/2;
	
      

wireMatr->Global2Local(globalC,localC);

locx = localC[0];  //x[0]*ppLength;
pady = 0.0;  //padN*ppWidth;   // TMath::Nint(x[1]+0.5)*ppWidth;
locy = localC[1];  //valarray[1]-pady;
locz = localC[2];  //valarray[2];


//distance[c]=TMath::Sqrt(pow(locx,2)+pow(locy,2));
distance.clear();
Hitx.clear();
Hity.clear();
Hitz.clear();
Hitx.push_back(globalC[0]);
Hity.push_back(globalC[1]);
Hitz.push_back(globalC[2]);

distance.push_back(TMath::Sqrt(pow(locx,2)+pow(locy,2)));



}


else if ((flag==0) && (flag!=1) && (flag!=2)) 
{
//c++;
//distance between half position and wire(center)
        xm[0]=dchHit->X();
	xm[1]=dchHit->Y();
	xm[2]=dchHit->Z();
	xm[3]=1.0;
	xm[4]=0.0;//dchHit->Time();	
	
		
        globalCm[0]=xm[0];
        globalCm[1]=xm[1];
        globalCm[2]=xm[2];

        index[0]=1;
	index[1]=isec;
	index[2]=0;
	
	//irow = dchHit->GetCelRing();
        
	ppWidth=0.0;  //param->GetPadPitchWidth(isec);
	nppWidth = 0.0;  //ppWidth * 4./3.;
	alfl = 0.0;  //ppWidth/3.;

	//padN = 0;  //TMath::Nint(x[1]+0.5);
 
        memcpy(Xxm,xm,5*sizeof(Float_t));
	
	valarraym[1]=Xxm[1];
	valarraym[2]=Xxm[2];
	valarraym[3]=Xxm[0];
	valarraym[4]=Xxm[4];
	valarraym[5]=(Float_t) dchHit->GetCelId();
        
	centralPad = 0; //(param->GetNPads(isec,irow))/2;
	wireMatr->Global2Local(globalCm,localCm);

	locxm = localCm[0];  //x[0]*ppLength;
	padym = 0.0;  //padN*ppWidth;   // TMath::Nint(x[1]+0.5)*ppWidth;
	locym = localCm[1];  //valarray[1]-pady;
	loczm = localCm[2];  //valarray[2];


//distance[c]=TMath::Sqrt(pow(locxm,2)+pow(locym,2));
Hitx.push_back(globalCm[0]);
Hity.push_back(globalCm[1]);
Hitz.push_back(globalCm[2]);
distance.push_back(TMath::Sqrt(pow(locxm,2)+pow(locym,2)));

}     
  
  else if (flag==2) {
//distance between exiting position and wire(center)
// c++;

        xo[0]=dchHit->X();
	xo[1]=dchHit->Y();
	xo[2]=dchHit->Z();
	xo[3]=1.0;
	xo[4]=0.0;//dchHit->Time();	
	
	
        globalCo[0]=xo[0];
        globalCo[1]=xo[1];
        globalCo[2]=xo[2];

        index[0]=1;
	index[1]=isec;
	index[2]=0;
	
	
        
	ppWidth=0.0;  
	nppWidth = 0.0;
	alfl = 0.0;  
 
        memcpy(Xxo,xo,5*sizeof(Float_t));
	
	valarrayo[1]=Xxo[1];
	valarrayo[2]=Xxo[2];
	valarrayo[3]=Xxo[0];
	valarrayo[4]=Xxo[4];
	valarrayo[5]=(Float_t) dchHit->GetCelId();
        
	centralPad = 0; 
	

        

wireMatr->Global2Local(globalCo,localCo);

locxo = localCo[0];  //x[0]*ppLength;
padyo = 0.0;  //padN*ppWidth;   // TMath::Nint(x[1]+0.5)*ppWidth;
locyo = localCo[1];  //valarray[1]-pady;
loczo = localCo[2];  //valarray[2];

Hitx.push_back(globalCo[0]);
Hity.push_back(globalCo[1]);
Hitz.push_back(globalCo[2]);

//distance[c]=TMath::Sqrt(pow(locxm,2)+pow(locym,2)); 
distance.push_back(TMath::Sqrt(pow(locxo,2)+pow(locyo,2)));
   

  
minimo=TMath::MinElement (distance.size(), &(*distance.begin()));  
findMin= TMath::LocMin (distance.size(), &(*distance.begin()));

        dist=minimo;
	
	valarraymin[0]=dist;
	valarraymin[1]=Hity[findMin];
	valarraymin[2]=Hitz[findMin];
	valarraymin[3]=Hitx[findMin];
	valarraymin[4]=0.0;
        valarraymin[5]=(Float_t) dchHit->GetCelId();
//c=0;
	
	//cout<<"Cell Id: "<<dchHit->GetCelId()<<" Track number: "<<track << " "<<dchHit->Track()<<" "
	//<<" "<<isec<<" "<<irow<<" "<<padN<<endl;
      

	ipart=dchHit->Track();

	tempisec = isec;


	if (isec==oldsecN && irow==oldrowN && padN==oldpadN){
	  
	  if(ipart==oldpartN){
	    ele = (*hits)[std::pair<IntPair,IntPair>(IntPair(ipart,tempisec),IntPair(irow,padN))].size() -1;

	    tempvalar=&((*hits)[std::pair<IntPair,IntPair>(IntPair(ipart,tempisec),IntPair(irow,padN))].at(ele));
	    
	    tempvalarY=&((*hitsY)[std::pair<IntPair,IntPair>(IntPair(ipart,tempisec),IntPair(irow,padN))].at(ele));
	    
	    if (valarraymin[0] < tempvalar->at(0)){
//cout<<"cambio "<<ipart<<" "<<isec<<" "<<tempisec<<" "<<irow<<" "<<padN<<" "<<ele<<endl;
	      tempvalar->at(0)=valarraymin[0];
	      tempvalar->at(1)=valarraymin[1];
	      tempvalar->at(2)=valarraymin[2];
	      tempvalar->at(3)=valarraymin[3];
    	      tempvalar->at(4)=valarraymin[4];
	      tempvalar->at(5)=valarraymin[5];

//	      memcpy(oldvalarrayo,valarrayo,6*sizeof(Float_t));
	        
	    }
	    
	  }
	  else{

(*hits)[std::pair<IntPair,IntPair>(IntPair(ipart,tempisec),IntPair(irow,padN))].push_back(std::vector<Float_t>(valarraymin,valarraymin+6));

//	    memcpy(oldvalarrayo,valarrayo,6*sizeof(Float_t));

	    oldpartN=ipart;

//cout<<"New track in same cell "<<ipart<<" "<<tempisec<<" "<<irow<<" "<<padN<<" "<<(*hits)[std::pair<IntPair,IntPair>(IntPair(ipart,tempisec),IntPair(irow,padN))].size()<<endl;

(*hitsY)[std::pair<IntPair,IntPair>(IntPair(ipart,tempisec),IntPair(irow,padN))].push_back(std::vector<Float_t>(valarrayYmin,valarrayYmin+3));

//	    memcpy(oldvalarrayYo,valarrayYo,3*sizeof(Float_t));

	    xcrosed = false;


	  }
	}
	else {

	 
(*hits)[std::pair<IntPair,IntPair>(IntPair(ipart,tempisec),IntPair(irow,padN))].push_back(std::vector<Float_t>(valarraymin,valarraymin+6));

//	  memcpy(oldvalarray,valarray,6*sizeof(Float_t));

//cout<<"New "<<ipart<<" "<<tempisec<<" "<<irow<<" "<<padN<<" "<<(*hits)[std::pair<IntPair,IntPair>(IntPair(ipart,tempisec),IntPair(irow,padN))].size()<<endl;

(*hitsY)[std::pair<IntPair,IntPair>(IntPair(ipart,tempisec),IntPair(irow,padN))].push_back(std::vector<Float_t>(valarrayYmin,valarrayYmin+3));

//	  memcpy(oldvalarrayYo,valarrayYo,3*sizeof(Float_t));

	  xcrosed = false;

	  oldsecN=isec;
	  oldrowN=irow;
	  oldpadN=padN;
	  oldpartN=ipart;


	}
/*
	oldlocxo = locxo;
	oldlocyo = locyo;
	oldloczo = loczo;
*/
          

    } dchHit = (IlcDCHhit*)dch->NextHit();   
    } // end of loop over hits

  } 
  // end of loop over tracks
//cout<<"-------------------- inizio clusterizzazione ----------------------\n";


  
  Float_t sigmaImp        = param->GetSigmaFrsTrk();//locparam->GetSigmaFrsTrk();
  Float_t sigmaz		= param->GetMaxZResolution();//locparam->GetMaxZResolution();
  Float_t sigmaz2;

  Float_t sigmaImp_2      = param->GetSigmaSecTrk();//locparam->GetSigmaSecTrk();

  Float_t sigmaz_2=0;
  
  Float_t sigmaImp2	= pow(sigmaImp,2);
  Float_t sigmaImp2_2	= pow(sigmaImp_2,2);
  sigmaz2		= pow(sigmaz,2);

  Float_t position[4],sigmas[5];
  Int_t tracklabel[3];
  Float_t DeltaTrkTime  = param->GetTimeBetweenTrk();//locparam->GetTimeBetweenTrk();
  bool  notprol = false;


  std::vector<Int_t> idtrack;
  std::vector<std::vector<Float_t> > *hvec;
  std::vector<std::vector<Float_t> > *hvecY;
  std::vector<Float_t> *valvec;
  std::vector<Float_t> *firsthit;
  std::vector<Float_t> *secondhit;
  std::vector<Float_t> *valvecY;
  std::vector<Float_t> *firsthitY;
  std::vector<Float_t> *secondhitY;
  Int_t idsec, idrow, idpad, idt[2];
  Float_t hittime,minhittime[2];

  vector<Int_t> cellidused; 
  for (hitstype::iterator hitpad=hits->begin();hitpad!=hits->end();hitpad++){

    idtrack.clear();
    idtrack.push_back((*hitpad).first.first.first);
    idsec=(*hitpad).first.first.second;
    idrow=(*hitpad).first.second.first;
    idpad=(*hitpad).first.second.second;
    //cout<<"iter track "<<(*hitpad).first.first.first<<" superlayer "<<idsec<<" layer "<<idrow<<" wire "<<idpad
    //<<" cellId "<<((Int_t)(*hitpad).second[0].at(5))<<endl;
    minhittime[0]=10000.;
    minhittime[1]=10000.;
    firsthit=0x0;
    secondhit=0x0;
    firsthitY=0x0;
    secondhitY=0x0;
    idt[0]=-1;
    idt[1]=-1; 	
    for (hitstype::iterator hitpad1=hits->begin();hitpad1!=hits->end();hitpad1++){
      if(hitpad==hitpad1) {continue;}



      if( ( (idsec==(*hitpad1).first.first.second) && (idrow==(*hitpad1).first.second.first) && (idpad==(*hitpad1).first.second.second) ) ){

        idtrack.push_back((*hitpad1).first.first.first);

      }
    }

    for(UInt_t molt=0; molt<idtrack.size(); molt++){
      hvec = &((*hits)[std::pair<IntPair,IntPair>(IntPair(idtrack.at(molt),idsec),IntPair(idrow,idpad))]);
      hvecY = &((*hitsY)[std::pair<IntPair,IntPair>(IntPair(idtrack.at(molt),idsec),IntPair(idrow,idpad))]);
      for(UInt_t nBend=0; nBend<hvec->size(); nBend++){
        valvec=&(hvec->at(nBend));
        valvecY=&(hvecY->at(nBend));

	z=valvec->at(2);
	if (z<=0) z=-z+semizleng;
        hittime=valvec->at(4)* 1.e+6 + z/29979245800.* 1.e+6 + xtr->dist2time(valvec->at(0))[0] ;
	if(hittime<minhittime[0]) {
	  if (minhittime[0]<10000. && (minhittime[0]-hittime)>=DeltaTrkTime){
	    minhittime[1]=minhittime[0];
	    secondhit=firsthit;
	    secondhitY=firsthitY;
	    idt[1]=idt[0];
	  }
	  minhittime[0]=hittime;
	  firsthit=valvec;
	  firsthitY=valvecY;
	  idt[0]=idtrack.at(molt);
	}
	else if (hittime<minhittime[1] && (hittime-minhittime[0])>=DeltaTrkTime){
	    minhittime[1]=hittime;
	    secondhit=valvec;
	    secondhitY=valvecY;
	    idt[1]=idtrack.at(molt);
	}
      }
    }


    z=firsthit->at(2);

    tempisec = idsec;

    position[0]=firsthit->at(3);
    position[1]=firsthit->at(1);
    position[2]=z;
    position[3]=firsthit->at(0);//gRandom->Gaus(firsthit->at(0),sigmaImp);

    if(!kExact){
      position[2]=gRandom->Gaus(z,sigmaz);
      position[3]=gRandom->Gaus(firsthit->at(0),sigmaImp);
    }

    sigmas[0]=0.0;
    sigmas[1]=0.0;
    sigmas[2]=sigmaz2;
    sigmas[3]=sigmaImp2;
    sigmas[4]=0.0;

    tracklabel[0]=idt[0];
    tracklabel[1]=-1;
    tracklabel[2]=-1;
  
  
    if(std::find(cellidused.begin(),cellidused.end(),(Int_t) firsthit->at(5))!=cellidused.end())
    	continue;
    cellidused.push_back((Int_t) firsthit->at(5));
    clus->AddCluster(position,sigmas,tracklabel,((Int_t) firsthit->at(5)));
    //cout<<"Second Id cell: "<<firsthit->at(5)<<" Track label: "<<tracklabel[0]<<endl;
    nclusters++;


    notprol=false;
    if(secondhit!=0x0){

      z=secondhit->at(2);
      tempisec = idsec;

      position[0]=secondhit->at(3);
      position[1]=secondhit->at(1);
      position[2]=z;
      position[3]=secondhit->at(0);//
      
      if(!kExact){
	position[2]=gRandom->Gaus(z,sigmaz_2);
	position[3]=gRandom->Gaus(secondhit->at(0),sigmaImp_2);
      }
      
      sigmas[0]=0.0;
      sigmas[1]=0.0;
      sigmas[2]=sigmaz2;
      sigmas[3]=sigmaImp2_2;
      sigmas[4]=0.0;


      tracklabel[0]=idt[1];
      tracklabel[1]=idt[0];
      tracklabel[2]=-1;
      
      clus->AddCluster(position,sigmas,tracklabel,((Int_t) secondhit->at(5)));
      //cout<<"3° Cell Id: "<<secondhit->at(5) <<" Track label: " <<tracklabel[0]<<endl; 
    
      nclusters++;

      notprol=false;
    }

  }
//cout<<"-------------------- finita clusterizzazione ----------------------\n";


  clus->WriteClusters();

  IlcInfo(Form("Number of made clusters : %d",nclusters));

  delete hits;
  delete hitsY;

} // end of function

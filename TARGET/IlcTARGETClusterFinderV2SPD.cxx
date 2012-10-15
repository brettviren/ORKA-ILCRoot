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
////////////////////////////////////////////////////////////////////////////
//            Implementation of the TARGET clusterer V2 class                //
//                                                                        //
//          Origin: Iouri Belikov, CERN, Jouri.Belikov@cern.ch            //
//                                                                        //
///////////////////////////////////////////////////////////////////////////


#include "IlcTARGETClusterFinderV2SPD.h"
#include "IlcTARGETRecPoint.h"
#include "IlcTARGETDetTypeRec.h"
#include "IlcRawReader.h"
#include "IlcTARGETRawStreamSPD.h"
#include "IlcTARGETParam.h"
#include "IlcTARGET.h"
#include "IlcRun.h"
#include <TClonesArray.h>
#include <TString.h>
#include "IlcTARGETdigitSPD.h"

ClassImp(IlcTARGETClusterFinderV2SPD)

extern IlcRun *gIlc;
IlcTARGETClusterFinderV2::IlcBin * IlcTARGETClusterFinderV2SPD::fBins=0;
int IlcTARGETClusterFinderV2SPD::fMaxNbins=0;

IlcTARGETClusterFinderV2SPD::IlcTARGETClusterFinderV2SPD(IlcTARGETDetTypeRec* dettyp):IlcTARGETClusterFinderV2(dettyp){
  const Float_t kconv=0.0001;
  //Default constructor

  IlcTARGET *ilcTARGET  = (IlcTARGET*)gIlc->GetModule("TARGET");
  fParam=0;
  IlcTARGETgeom *geom=LoadGeometry();
  //if (geom->GetDetVersion() !=3 ) cout <<"in IlcTARGETClusterFinderV2SPD: Minor Version: "<<geom->GetDetVersion()<<endl;
  
  fParam=new IlcTARGETParam(ilcTARGET->GetMinorVersion());
  
//   if (geom->GetDetVersion()==3) fParam=new IlcTARGETParam(geom->GetDetVersion());
//   else if (geom->GetDetVersion()==2) fParam=new IlcTARGETParam(geom->GetDetVersion());
//   else fParam=new IlcTARGETParam()geom->GetDetVersion();

  Int_t check=fParam->GetTotLayers();
  
  //const Int_t ntot=fParam->GetNlayers()+fParam->GetNendcaps();
  //const Int_t ntot=fParam->GetTotLayers();
  //cout <<"***************************ntot: "<<ntot<<endl;
  
  const Int_t ntot=fParam->GetNlayers()+fParam->GetNendcaps()+fParam->GetNlayersStrip()+fParam->GetNendcapsStrip();
  
  
  
  fNySPD=new Int_t[ntot];          //number of pixels in Y
  fNzSPD=new Int_t[ntot];          //number of pixels in Z
  fXpitchSPD=new Double_t[ntot];    //pixel size in X
  fYpitchSPD=new Double_t[ntot];    //pixel size in Y
  fZ1pitchSPD=new Double_t[ntot];
  fZ2pitchSPD=new Double_t[ntot];    //pixel sizes in Z
  fHwSPD=new Double_t[ntot];        //half width of the SPD detector
  fHlSPD=new Double_t[ntot];        //half length of the SPD detector

  Int_t npixelx=0,npixelz=0;
  Float_t pixelx=0,pixely=0,pixelz=0;
  fLastSPD1=fDetTypeRec->GetTARGETgeom()->GetModuleIndex(
    fParam->GetNlayers()+fParam->GetNendcaps()+fParam->GetNlayersStrip()+fParam->GetNendcapsStrip(),
    fParam->GetNladders(fParam->GetNlayers()+fParam->GetNendcaps()+fParam->GetNlayersStrip()+fParam->GetNendcapsStrip()-1),
    fParam->GetNdetectors(fParam->GetNlayers()+fParam->GetNendcaps()+fParam->GetNlayersStrip()+fParam->GetNendcapsStrip()-1)
  );


  IlcInfo(Form("****************** Nmodules: %d",fLastSPD1+1));
  //fLastSPD1=12;
  //fDetTypeRec->GetTARGETgeom()->GetNDetTypes(max);
  IlcDebug(1,Form("Number of modules: %d",fLastSPD1));
  // fLastSPD1=max;
  for (Int_t lay=0;lay<fParam->GetNlayers()+fParam->GetNendcaps()+fParam->GetNlayersStrip()+fParam->GetNendcapsStrip();lay++) {//Dan Endcap
    fDetTypeRec->GetTARGETgeom()->GetPixelDimension(lay,pixelx,pixely,pixelz);
    fDetTypeRec->GetTARGETgeom()->GetNPixelX(lay,npixelx);
    fDetTypeRec->GetTARGETgeom()->GetNPixelZ(lay,npixelz);
    fNySPD[lay]=npixelx; fNzSPD[lay]=npixelz;
    //  cout <<"numero di pixel: "<<fNzSPD[lay]<<endl;
    fZ1pitchSPD[lay]=(Float_t)pixelz*kconv; fZ2pitchSPD[lay]=(Float_t)pixelz*kconv;
    fXpitchSPD[lay]=pixely*kconv;
    fYpitchSPD[lay]=pixelx*kconv;
    IlcDebug(1,Form("--- pixelx: %2f, pixely: %2f, pixelz: %2f, npixelx: %d, npixelz: %d",pixelx*kconv,pixely*kconv,pixelz*kconv,npixelx,npixelz));
    //printf("--- pixelx: %2f, pixely: %2f, pixelz: %2f, npixelx: %d, npixelz: %d",pixelx*kconv,pixely,pixelz*kconv,npixelx,npixelz);
    
    //  fZ1pitchSPD=0.0425; fZ2pitchSPD=0.0625;
    fYSPD[lay][0]=fYpitchSPD[lay]*0.5;
    //cout <<"fYSPD[lay][0]: "<<fYSPD[0][0]<<endl;
    for (Int_t m=1; m<fNySPD[lay]; m++)
      {
	fYSPD[lay][m]=fYSPD[lay][0]+m*fYpitchSPD[lay];
	IlcDebug(1,Form("fYSPD[%d][%d]: %2f\n",lay,m,fYSPD[lay][m]));
      }
    //  fHwSPD=0.64; 
    IlcTARGETgeomSPD *gspd=dynamic_cast<IlcTARGETgeomSPD*> (fDetTypeRec->GetTARGETgeom()->GetShape2(lay));
    if(gspd){
      fHlSPD[lay]=gspd->GetDz();
      fHwSPD[lay]=gspd->GetDx();
    }else{
      fHlSPD[lay]=0;
      fHwSPD[lay]=0;
    }
    IlcDebug(1,Form("lay: %d, Dx: %f, Dz: %f",lay,fHlSPD[lay],fHwSPD[lay]));

 // fYSPD[0]=fYpitchSPD;
/*fYSPD[lay][0]=fYpitchSPD[lay];
for (Int_t m=1; m<fNySPD[lay]; m++) fYSPD[lay][m]=fYSPD[lay][m-1]+fYpitchSPD[lay]; */
//  for (Int_t m=0; m<fNySPD[lay]; m++) fYSPD[lay][m]=fYpitchSPD[lay]; 
 // fZSPD[0]=fZ1pitchSPD;

// fZSPD[lay][0]=fZ1pitchSPD[lay];
// for (Int_t m=1; m<fNzSPD[lay]; m++) fZSPD[lay][m]=fZSPD[lay][m-1]+fZ1pitchSPD[lay]; 
//cout <<"Qui arriva bene"<<endl;
fZSPD[lay][0]=fZ1pitchSPD[lay]*0.5;
//cout <<"Qui invece non va!!!"<<endl;
for (Int_t m=1; m<fNzSPD[lay]; m++) {
fZSPD[lay][m]=fZSPD[lay][0]+m*fZ1pitchSPD[lay]; 
IlcDebug(1,Form("fZSPD[%d][%d]: %2f\n",lay,m,fZSPD[lay][m]));
}
}

}
 
IlcTARGETClusterFinderV2SPD::~IlcTARGETClusterFinderV2SPD(){
  delete fParam;
  if(fBins){
    delete fBins;
    fBins=0;
    fMaxNbins=0;
  }
}

void IlcTARGETClusterFinderV2SPD::FindRawClusters(Int_t mod){

  //Find clusters V2
  Int_t lay=0,lad=0,det=0;
//  cout <<"qui inizia la clusterizzazione per SPD!!!"<<endl;
  SetModule(mod);

  fDetTypeRec->GetTARGETgeom()->GetModuleId(mod,lay,lad,det);


  FindClustersSPD(lay-1,fDigits);

}

void IlcTARGETClusterFinderV2SPD::RawdataToClusters(IlcRawReader* rawReader, TClonesArray** clusters){
    //------------------------------------------------------------
  // This function creates TARGET clusters from raw data
  //------------------------------------------------------------


  rawReader->Reset();
  IlcTARGETRawStreamSPD inputSPD(rawReader);
  FindClustersSPD(&inputSPD, clusters);

}

Int_t IlcTARGETClusterFinderV2SPD::ClustersSPD(Int_t lay,IlcBin* bins, TClonesArray* digits,TClonesArray* clusters,Int_t maxBins,Int_t nzbins,Int_t iModule,Bool_t rawdata){
   

 

  //Cluster finder for SPD (from digits and from rawdata)
  Int_t nclu=0;
//   cout<<"Clusters "<<lay<<" "<<maxBins<<" "<<digits->GetEntries()<<" "<<digits->GetEntriesFast()<<" "<<nzbins<<" "
//       <<iModule<<" "<<rawdata<<endl;
  //  for(Int_t iBin =0; iBin < maxBins;iBin++){
  for(Int_t iDigi =0; iDigi < digits->GetEntriesFast();iDigi++){
    IlcTARGETdigitSPD *d=(IlcTARGETdigitSPD*)digits->UncheckedAt(iDigi);
    Int_t iBin=(d->GetCoord2()+1)*(fNzSPD[lay] + 2)+(d->GetCoord1()+1);
    if(bins[iBin].IsUsed()) continue;
    Int_t nBins = 0;
    Int_t idxBins[2000];
    FindCluster(iBin,nzbins,bins,nBins,idxBins);
    if (nBins == 2000){
      Error("ClustersSPD","SPD Too big cluster !\n"); 
      continue;
    }
    Int_t milab[10];
    for(Int_t ilab=0;ilab<10;ilab++){
      milab[ilab]=-2;
    }
    if(rawdata){
      milab[3]=fNdet[iModule];
    }
    Int_t ymin,ymax,zmin,zmax;
    if(rawdata){
      ymin = (idxBins[0] / nzbins) - 1;
      ymax = ymin;
      zmin = (idxBins[0] % nzbins) - 1;
      zmax = zmin;
    }
    else{
      IlcTARGETdigitSPD* dig = (IlcTARGETdigitSPD*)digits->UncheckedAt(idxBins[0]);
      ymin=dig->GetCoord2();
      ymax=ymin;
      zmin=dig->GetCoord1();
      zmax=zmin;
    }
    for (Int_t idx = 0; idx < nBins; idx++) {
      Int_t iy;
      Int_t iz; 
      if(rawdata){
	iy  = (idxBins[idx] / nzbins) - 1;
	iz  = (idxBins[idx] % nzbins) - 1;
      }
      else{
	IlcTARGETdigitSPD* dig = (IlcTARGETdigitSPD*)digits->UncheckedAt(idxBins[idx]);
	iy = dig->GetCoord2();
	iz = dig->GetCoord1();
      }
      if (ymin > iy) ymin = iy;
      if (ymax < iy) ymax = iy;
      if (zmin > iz) zmin = iz;
      if (zmax < iz) zmax = iz;

    }
    if(!rawdata){
      for(Int_t l=0;l<nBins;l++){
	IlcTARGETdigitSPD* dig = (IlcTARGETdigitSPD*)digits->UncheckedAt(idxBins[l]);
	for(Int_t dlab=0;dlab<10;dlab++){
	  Int_t digitlab = (dig->GetTracks())[dlab];
	  if(digitlab<0) continue;
	  AddLabel(milab,digitlab);
	}
	if (milab[9]>0) CheckLabels2(milab);
      }
      CheckLabels2(milab);
    }
    Int_t idy =0; //max 2 clusters
    if((iModule <= fLastSPD1) &&idy<10) idy=fParam->GetRangeY();
    if((iModule > fLastSPD1) &&idy<4) idy=4;
    Int_t idz=fParam->GetRangeZ();

    for(Int_t iiz=zmin; iiz<=zmax;iiz+=idz){
      for(Int_t iiy=ymin;iiy<=ymax;iiy+=idy){

	Int_t ndigits=0;
	Float_t y=0.,z=0.,q=0.;
	for(Int_t idx=0;idx<nBins;idx++){
	  Int_t iy;
	  Int_t iz; 
	  if(rawdata){
	    iy  = (idxBins[idx] / nzbins)-1;
	    iz  = (idxBins[idx] % nzbins)-1;
	  }
	  else{
	    IlcTARGETdigitSPD* dig = (IlcTARGETdigitSPD*)digits->UncheckedAt(idxBins[idx]);
	    iy = dig->GetCoord2();
	    iz = dig->GetCoord1();
	  }
	  if(zmax-zmin>=idz || ymax-ymin>=idy){
            //printf("Entra in un caso particolare!!!\n");
	    if(TMath::Abs(iy-iiy)>0.75*idy) continue;
	    if(TMath::Abs(iz-iiz)>0.75*idz) continue;
	  }
	  ndigits++;
	  Float_t qBin;
	  if(rawdata) qBin = bins[idxBins[idx]].GetQ();
	  if(!rawdata){
	    IlcTARGETdigitSPD* dig = (IlcTARGETdigitSPD*)digits->UncheckedAt(idxBins[idx]);
	    qBin = (Float_t)dig->GetSignal();
	    if(IlcDebugLevelClass()>0)
	      IlcDebug(1,Form("qBin: %d",qBin));
	  }
	  y+= qBin * fYSPD[lay][iy];
	  z+= qBin * fZSPD[lay][iz];
	  q+= qBin;
	  if(IlcDebugLevelClass()>0)
	    IlcDebug(1,Form("iy: %d, yz: %d fYSPD[lay][iy]: %f, fZSPD[lay][iz]: %f\n",iy,iz,fYSPD[lay][iy],fZSPD[lay][iz]));
	
	}// for idx
	if(ndigits==0) continue;
	y /= q;
	z /= q;
	y -= fHwSPD[lay];

	z -= fHlSPD[lay];
        //printf("ClusterDan... y: %f, z: %f, fHwSPD: %f,fHlSPD:%f, q: %f, fLastSPD1: %d\n",y,z,fHwSPD[lay],fHlSPD[lay],q,fLastSPD1);
	Float_t hit[5]; //y,z,sigma(y)^2, sigma(z)^2, charge
	hit[0] = -(-y+fYshift[iModule]);
        //cout << "hit[0]; " << hit[0] << endl;
        hit[0] = y;//-0.0001;//->>>>>>>>>>>
        //cout << "hit[0]; ->1 " << hit[0] << endl;
	
	//if(iModule <= fLastSPD1) hit[0] = -hit[0];
	hit[1] = -z+fZshift[iModule];
        //cout << "y: "<< y << "--- fYshift["<<iModule<<"]: "<< fYshift[iModule]<<endl;

        //cout << "z: "<< z << "--- fZshift["<<iModule<<"]: "<< fZshift[iModule]<<endl;

        hit[2] = fXpitchSPD[lay]*fXpitchSPD[lay]/12. + fYpitchSPD[lay]*fYpitchSPD[lay]/12.;//FIXME
	hit[3] = fZ1pitchSPD[lay]*fZ1pitchSPD[lay]/12.;
	//hit[2] = 0.0006*0.0006;
	//hit[3] = 0.0005*0.0005;
	hit[4] = 1.;
        //hit[0]= -hit[0];
        hit[1]= -hit[1];
	if(IlcDebugLevelClass()>0){
	  IlcDebug(1,Form("LastCluster... y: %2f, z: %2f, fHwSPD: %2f,fHlSPD:%2f, \n",y,z,fHwSPD[lay],fHlSPD[lay]));
	  IlcDebug(1,Form("iModule: %d",iModule));
	  IlcDebug(1,Form("ClusterDan... hit[0]: %f, hit[1]: %f, fYshift: %f, fZshift:%f \n",hit[0],hit[1],fYshift[iModule],fZshift[iModule]));
	}


// // // // 	  IlcInfo(Form("iModule: %d",iModule));
// // // // 	  IlcInfo(Form("LastCluster... y: %2f, z: %2f, fHwSPD: %2f,fHlSPD:%2f",y,z,fHwSPD[lay],fHlSPD[lay]));
// // // // 	  IlcInfo(Form("ClusterDan... hit[0]: %f, hit[1]: %f, fYshift: %f, fZshift:%f", hit[0],hit[1],fYshift[iModule],fZshift[iModule]));


	// hit[0]= -hit[0];
	if(!rawdata) milab[3]=fNdet[iModule];
	Int_t info[3] = {ymax-ymin+1,zmax-zmin+1,fNlayer[iModule]};
	if(!rawdata){
                  
	  if(IlcDebugLevelClass()>0){
	    for (Int_t i=0;i<3;i++) IlcDebug(1,Form("Info[%d]: %f",i,info[i]));
	    for (Int_t i=0;i<5;i++) IlcDebug(1,Form("hits[%d]: %f",i,hit[i]));
	  }
	 //IlcTARGETRecPoint cl(iModule,fDetTypeRec->GetTARGETgeom(),milab,hit,info);
         IlcTARGETgeom *gm=fDetTypeRec->GetTARGETgeom();
         IlcTARGETRecPoint cl(gm);
         //cl.SetTARGETgeom();
         
	 cl.SetXZ(iModule,hit[0],hit[1]);
         
	 cl.SetSigmaDetLocX2(hit[2]);
	 cl.SetSigmaZ2(hit[3]);
         cl.SetQ(hit[4]);
         Int_t layer=0,lad=0,det=0;
         gm->GetModuleId(iModule,layer,lad,det);
         cl.SetLayer(layer-1);
         //cout <<"Layer: " << layer <<" ---- Hit[0]: " <<hit[0]<<" --- Hit[1]: " <<hit[1]<<endl;

         Int_t ind=(lad-1)*gm->GetNdetectors(layer)+(det-1);
          cl.SetNz(info[1]);
          cl.SetNy(info[0]);
         for (Int_t i=0;i<3;i++) cl.SetLabel(milab[i],i);
         cl.SetDetectorIndex(ind);
         cl.SetLayer(info[2]);
       // cout <<"ClusterDan... Layer: "<<layer-1<<" Module: "<<iModule<<" hit[x]: "<<hit[0]<<", hit[z]: "<<hit[1]<<endl;
         cl.SetModule(iModule);
	 fDetTypeRec->AddRecPoint(cl);
	}
        else{
	  Int_t label[4]={milab[0],milab[1],milab[2],milab[3]};
	  new (clusters->AddrAt(nclu)) 
		IlcTARGETRecPoint(iModule,fDetTypeRec->GetTARGETgeom(),label, hit,info);
	} 
	nclu++;
      }// for iiy
    }// for iiz
  }//end for iBin
  return nclu;
  
}




void IlcTARGETClusterFinderV2SPD::FindClustersSPD(IlcTARGETRawStream* input, 
					TClonesArray** clusters) 
{
  //------------------------------------------------------------
  // Actual SPD cluster finder for raw data
  //------------------------------------------------------------


  Int_t nClustersSPD = 0;
  Int_t kNzBins = fNzSPD[0] + 2;
  Int_t kNyBins = fNySPD[0] + 2;
  Int_t kMaxBin = kNzBins * kNyBins;
  IlcBin *binsSPD = new IlcBin[kMaxBin];
  IlcBin *binsSPDInit = new IlcBin[kMaxBin];  
  IlcBin* bins = NULL;

  // read raw data input stream
  while (kTRUE) {
    Bool_t next = input->Next();
    if (!next || input->IsNewModule()) {
      Int_t iModule = input->GetPrevModuleID();

      // when all data from a module was read, search for clusters
      if (bins) { 
	clusters[iModule] = new TClonesArray("IlcTARGETRecPoint");
	Int_t nClusters = ClustersSPD(0,bins,0,clusters[iModule],kMaxBin,kNzBins,iModule,kTRUE);
	nClustersSPD += nClusters;
	bins = NULL;
      }

      if (!next) break;
      bins = binsSPD;
      memcpy(binsSPD,binsSPDInit,sizeof(IlcBin)*kMaxBin);
    }

    // fill the current digit into the bins array
    Int_t index = (input->GetCoord2()+1) * kNzBins + (input->GetCoord1()+1);
    bins[index].SetIndex(index);
    bins[index].SetMask(1);
    bins[index].SetQ(1);
  }

  delete [] binsSPDInit;
  delete [] binsSPD;
  
  Info("FindClustersSPD", "found clusters in TARGET SPD: %d", nClustersSPD);
}



void IlcTARGETClusterFinderV2SPD::FindClustersSPD(Int_t lay,TClonesArray *digits) {
  //------------------------------------------------------------
  // Actual SPD cluster finder
  //------------------------------------------------------------

//  cout <<"layer: "<<lay<<endl;
//  cout <<"non so perche il problema Ã¨ qui!"<<endl;
//  cout <<"fYSPD[0][0]: "<<fYSPD[0][0]<<endl;
  //cout <<"fNzSPD: "<<fNzSPD[0]<<endl;
  Int_t kNzBins = fNzSPD[lay] + 2;
//cout <<"qui chiama!!!"<<endl;
  const Int_t kMAXBIN=kNzBins*(fNySPD[lay]+2);
//  cout <<"qui invece non va piu!!!"<<endl;
  Int_t ndigits=digits->GetEntriesFast();
  if(!fBins||fMaxNbins<kMAXBIN){
    if(fBins) delete fBins;
    fBins=new IlcBin[kMAXBIN];
    fMaxNbins=kMAXBIN;
  }

  Int_t k;
  IlcTARGETdigitSPD *d=0;
  for (k=0; k<ndigits; k++) {
     d=(IlcTARGETdigitSPD*)digits->UncheckedAt(k);
     Int_t i=d->GetCoord2()+1;   //y
     Int_t j=d->GetCoord1()+1;
     if(IlcDebugLevelClass()>0)
       IlcDebug(1,Form("1) digits number: %d, Coordinate 2: %d, Coordinate 1: %d",ndigits,i,j));

//cout<<" Digit Numero "<<k <<"Coordinata i "<<i <<" Coordinata j "<< j <<endl;

     Int_t index=i*kNzBins+j;
     fBins[index].SetIndex(k);
     fBins[index].SetMask(1);
  }
   
  ClustersSPD(lay,fBins,digits,0,kMAXBIN,kNzBins,fModule,kFALSE); 

  for (k=0; k<ndigits; k++) {
     d=(IlcTARGETdigitSPD*)digits->UncheckedAt(k);
     Int_t i=d->GetCoord2()+1;   //y
     Int_t j=d->GetCoord1()+1;
     Int_t index=i*kNzBins+j;
     fBins[index].SetIndex(0);
     fBins[index].SetMask(0xFFFFFFFE);
     fBins[index].SetQ(0);
  }  
  //  delete [] bins;
//   <<"qui ha finito"<<endl;
}

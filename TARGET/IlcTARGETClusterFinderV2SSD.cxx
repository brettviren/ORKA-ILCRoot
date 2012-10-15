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


#include "IlcTARGETClusterFinderV2SSD.h"
#include "IlcTARGETRecPoint.h"
#include "IlcTARGETDetTypeRec.h"
#include "IlcRawReader.h"
#include "IlcTARGETRawStreamSSD.h"
#include "IlcTARGET.h"
#include "IlcRun.h"
#include <TClonesArray.h>
#include <TRandom.h>
#include "IlcTARGETdigitSSD.h"
#include "IlcTARGETgeomSSD.h"

ClassImp(IlcTARGETClusterFinderV2SSD)


IlcTARGETClusterFinderV2SSD::IlcTARGETClusterFinderV2SSD(IlcTARGETDetTypeRec* dettyp):IlcTARGETClusterFinderV2(dettyp){

  //Default constructor

  IlcTARGET *ilcTARGET  = (IlcTARGET*)gIlc->GetModule("TARGET");
  IlcTARGETgeom *geom=LoadGeometry();
  //if (geom->GetDetVersion() !=3 ) <<"in IlcTARGETClusterFinderV2SSD: Minor Version: "<<geom->GetDetVersion()<<endl;

  fParam=new IlcTARGETParam(ilcTARGET->GetMinorVersion());

// if (geom->GetDetVersion() == 3) fParam=new IlcTARGETParam(geom->GetDetVersion());
// else if (geom->GetDetVersion() == 2) fParam=new IlcTARGETParam(geom->GetDetVersion());
// else fParam=new IlcTARGETParam(geom->GetDetVersion());

  const Double_t kconv=0.0001;
//cout <<"nlayers: "<<fParam->GetNlayers()<<" nendcaps: "<<fParam->GetNendcaps() << " nlayersStrip: "<<fParam->GetNendcapsStrip()<<" nendcapsStrip: "<<fParam->GetNlayersStrip()<<endl;
  fLastSSD1=fDetTypeRec->GetTARGETgeom()->GetModuleIndex(fParam->GetNlayers()+fParam->GetNendcaps()+fParam->GetNendcapsStrip()+fParam->GetNlayersStrip(),12,1);
  IlcDebug(1,Form("fLastSSD1: %d",fLastSSD1));
  Int_t npixelx=0,npixelz=0;
  Float_t pixelx=0,pixely=0,pixelz=0;
  Int_t nlayers=fParam->GetNlayers();
  Int_t nendcaps=fParam->GetNendcaps();
  Int_t nendcapsStrip=fParam->GetNendcapsStrip()+fParam->GetNlayersStrip();

  fYpitchSSD=new Float_t[nendcapsStrip];
  fHwSSD=new Float_t[nendcapsStrip];
  fHlSSD=new Float_t[nendcapsStrip];
  fTanP=new Float_t[nendcapsStrip];
  fTanN=new Float_t[nendcapsStrip];

  for (Int_t lay=0;lay<nendcapsStrip;lay++) {
    Float_t stereop=fParam->GetStereoN(lay+nlayers+nendcaps);
    Float_t stereon=fParam->GetStereoP(lay+nlayers+nendcaps);
    fDetTypeRec->GetTARGETgeom()->GetPixelDimension(lay+nlayers+nendcaps,pixelx,pixely,pixelz);
    fDetTypeRec->GetTARGETgeom()->GetNPixelX(lay+nlayers+nendcaps,npixelx);
    fDetTypeRec->GetTARGETgeom()->GetNPixelZ(lay+nlayers+nendcaps,npixelz);
    IlcDebug(1,Form("pixelx in reconstruction: %f",pixelx));
    fYpitchSSD[lay]=pixelx*kconv;
    fTanP[lay]=TMath::Tan(stereop);
    fTanN[lay]=TMath::Tan(stereon);
    
    IlcTARGETgeomSSD *s0 = 
      dynamic_cast<IlcTARGETgeomSSD*> (fDetTypeRec->GetTARGETgeom()->GetShape2(lay+nlayers+nendcaps));

    if(s0){
      fHlSSD[lay]=s0->GetDz();
      fHwSSD[lay]=s0->GetDx();
    }else{
      fHlSSD[lay]=0;
      fHwSSD[lay]=0;
    }
  //cout <<"fYpitchSSD[lay]: "<<fYpitchSSD[lay]<<endl;
//cout <<"fHlSSD[lay]: "<<fHlSSD[lay]<<endl;
//cout <<"fHwSSD[lay]: "<<fHwSSD[lay]<<endl;
//cout <<"fHwSSD[lay]: "<<fHwSSD[lay]<<endl;
  //fTanP[lay]=0.0;
  //fTanN[lay]=0.0;
  }
}

IlcTARGETClusterFinderV2SSD::~IlcTARGETClusterFinderV2SSD(){
delete fParam;
}

void IlcTARGETClusterFinderV2SSD::FindRawClusters(Int_t mod){

  //Find clusters V2
  //cout <<"chiama questo clusterizer"<<endl;
  Int_t lay=0,lad=0,det=0;
  //cout <<"modulo: "<<mod<<endl;
  SetModule(mod);
  //cout <<"qui inizia la clusterizzazione per SSD"<<endl;
  fDetTypeRec->GetTARGETgeom()->GetModuleId(mod,lay,lad,det);
  Int_t nlayers=fParam->GetNlayers();
  Int_t nendcaps=fParam->GetNendcaps();
  if ((fParam->GetSensor((lay-1)-nlayers-nendcaps))>1)
    FindClustersSSD((lay-1)-nlayers-nendcaps,fDigits);
  else FindClustersSSDv1Sens((lay-1)-nlayers-nendcaps,fDigits);
}


//2 sensor version
void IlcTARGETClusterFinderV2SSD::FindClustersSSD(Int_t lay,TClonesArray *alldigits) {
  //------------------------------------------------------------
  // Actual SSD cluster finder
  //------------------------------------------------------------
  //cout <<"Chiama questo metodo!!!"<<endl;
  int debuglevel=IlcDebugLevelClass();
  Int_t smaxall=alldigits->GetEntriesFast();
  if (smaxall==0) {/*cout <<"non ci sono digits! layer: "<<lay<<endl;*/ return;}
  TObjArray *digits = new TObjArray;
  for (Int_t i=0;i<smaxall; i++){
    IlcTARGETdigitSSD *d=(IlcTARGETdigitSSD*)alldigits->UncheckedAt(i);
    //cout <<"coord1: "<<d->GetCoord1()<<" coord2: "<<d->GetCoord2()<<" segnale: "<<d->GetSignal()<<endl;
    //if ((d->GetCoord2()%2)!=0) continue;
    if (d->GetSignal()<3) continue;
    //cout <<"digit number: "<<i<<endl;
    digits->AddLast(d);
  }
  Int_t smax = digits->GetEntriesFast();
//cout <<"numero dei digits: "<<smax<<endl;
  if (smax==0) return;
  
  const Int_t kMax=1000;
  Int_t np=0, nn=0; 
  Ilc1Dcluster pos[kMax], neg[kMax];
  Float_t y=0., q=0., qmax=0.; 
  Int_t lab[4]={-2,-2,-2,-2};
  
  IlcTARGETdigitSSD *d=(IlcTARGETdigitSSD*)digits->UncheckedAt(0);
  q += d->GetSignal();
  if(debuglevel>0){
    IlcDebug(1,Form("adesso salva come negativo"));
    IlcDebug(1,Form("strip: %d side: %d",d->GetCoord2(),d->GetCoord1()));
  }
  y += d->GetCoord2()*d->GetSignal();
  qmax=d->GetSignal();
  lab[0]=d->GetTrack(0); lab[1]=d->GetTrack(1); lab[2]=d->GetTrack(2);
  Int_t curr=d->GetCoord2();
  Int_t flag=d->GetCoord1();
      //cout <<"first strip: "<<curr<<endl;
      //cout <<"first coord1: "<<d->GetCoord1()<<endl; 

  Int_t *n=&nn;
  Ilc1Dcluster *c=neg;
  
  Int_t nd=1;
  Int_t milab[10];
  for (Int_t ilab=0;ilab<10;ilab++){
    milab[ilab]=-2;
  }
  milab[0]=d->GetTrack(0); milab[1]=d->GetTrack(1); milab[2]=d->GetTrack(2);

  for (Int_t s=1; s<smax; s++) {
      d=(IlcTARGETdigitSSD*)digits->UncheckedAt(s);      
      Int_t strip=d->GetCoord2();
      if(debuglevel>0) IlcDebug(1,Form("strip: %d side: %d",d->GetCoord2(),d->GetCoord1()));
      //cout <<"next strip: "<<strip<<endl;      
      //cout <<"coord1: "<<d->GetCoord1()<<endl; 
      //cout <<"strip and current: "<<strip<< " " <<curr<<endl;
      if ((strip-curr)>2 || flag!=d->GetCoord1()) {
         c[*n].SetY(y/q);
         c[*n].SetQ(q);
         c[*n].SetNd(nd);
	 CheckLabels2(milab);
         c[*n].SetLabels(milab);
         if(debuglevel>0) IlcDebug(1,Form("1) numero digit sospetti: %d",nd));
/*	 if (nd>31&&nd<35) {
	   c[*n].SetY(y/q-0.25*nd);
	   c[*n].SetQ(0.5*q);
           //cout <<"1) inserisce il cluster: "<<endl;
           //cout <<"Y: "<<c[*n].GetY()<<endl;
           //cout <<"Q: "<<c[*n].GetQ()<<endl;
           //cout <<"Nd: "<<c[*n].GetNd()<<endl;
           //cout <<"Label: "<<c[*n].GetLabel(0)<<endl;

          cout <<"1) Strip: "<<y/q-0.25*nd<<endl;
	   (*n)++;
	   if (*n==kMax) {
	     Error("FindClustersSSD","Too many 1D clusters !");
	     return;
	   }
           //cout <<"!!!!!!!!!!!!!!!!!!!!!!! 1) secondo cluster sospetto"<<endl;
          cout <<"2) Strip: "<<y/q+0.25*nd<<endl;
	   c[*n].SetY(y/q+0.25*nd);
	   c[*n].SetQ(0.5*q);
	   c[*n].SetNd(nd);
	   c[*n].SetLabels(milab);
	 }	 */
           //cout <<"2) inserisce il cluster: "<<endl;
           //cout <<"Y: "<<c[*n].GetY()<<endl;
           //cout <<"Q: "<<c[*n].GetQ()<<endl;
           //cout <<"Nd: "<<c[*n].GetNd()<<endl;
           //cout <<"Label: "<<c[*n].GetLabel(0)<<endl;
           //cout <<"strip candidata: "<<curr<<endl;
         if(debuglevel>0) IlcDebug(1,Form("3) Strip: %f",y/q));
         (*n)++;
         if (*n==kMax) {
          Error("FindClustersSSD","Too many 1D clusters !");
          return;
         }
         y=q=qmax=0.;
         nd=0;
         lab[0]=lab[1]=lab[2]=-2;
	 //
	 for (Int_t ilab=0;ilab<10;ilab++){
	   milab[ilab]=-2;
	 }
	 //
         if (flag!=d->GetCoord1()) { 
           /*if (d->GetCoord1()) {
           cout <<"ha cambiato cambiato in positivo"<<endl; */
           n=&np; c=pos; 
          /* }
           else {
           cout <<"ha cambiato cambiato in negativo"<<endl; 
           n=&nn; c=neg; 
           }*/

           }
      }
      flag=d->GetCoord1();
      q += d->GetSignal();
      y += d->GetCoord2()*d->GetSignal();
      nd++;
      if (d->GetSignal()>qmax) {
         qmax=d->GetSignal();
         lab[0]=d->GetTrack(0); lab[1]=d->GetTrack(1); lab[2]=d->GetTrack(2);
      }
      for (Int_t ilab=0;ilab<10;ilab++) {
	if (d->GetTrack(ilab)>=0) AddLabel(milab, (d->GetTrack(ilab))); 
      }
      curr=strip;
  }
//cout <<"2) aggiungo cluster"<<endl;
         //cout <<"y: "<<y<<" q: "<<q<<" nd:" <<nd<<" y/q: "<<y/q<<endl;
  if(debuglevel>0) IlcDebug(1,Form("valore di q: %f",q));
  c[*n].SetY(y/q);
  c[*n].SetQ(q);
  c[*n].SetNd(nd);
  c[*n].SetLabels(lab);
  //Split suspiciously big cluster
// cout <<"numero digit: "<<nd<<endl;
  if(debuglevel>0) IlcDebug(1,Form("2) numero digit sospetti: %d",nd));
/*  if (nd>31 && nd<35) {
     //cout <<"!!!!!!!!!!!!!!!!!!!!!!! 2) split cluster sospetto"<<endl;
     //cout <<"!!!!!!!!!!!!!!!!!!!!!!! 2) primo cluster sospetto"<<endl;
     c[*n].SetY(y/q-0.25*nd);
     c[*n].SetQ(0.5*q);
           //cout <<"3) inserisce il cluster: "<<endl;
           //cout <<"Y: "<<c[*n].GetY()<<endl;
           //cout <<"Q: "<<c[*n].GetQ()<<endl;
           //cout <<"Nd: "<<c[*n].GetNd()<<endl;
           //cout <<"Label: "<<c[*n].GetLabel(0)<<endl;

     (*n)++;
     if (*n==kMax) {
        Error("FindClustersSSD","Too many 1D clusters !");
        return;
     }
      //cout <<"!!!!!!!!!!!!!!!!!!!!!!! 2) secondo cluster sospetto"<<endl;
     c[*n].SetY(y/q+0.25*nd);
     c[*n].SetQ(0.5*q);
     c[*n].SetNd(nd);
     c[*n].SetLabels(lab);
  }*/
           //cout <<"4) inserisce il cluster: "<<endl;
           //cout <<"Y: "<<c[*n].GetY()<<endl;
           //cout <<"Q: "<<c[*n].GetQ()<<endl;
           //cout <<"Nd: "<<c[*n].GetNd()<<endl;
           //cout <<"Label: "<<c[*n].GetLabel(0)<<endl;
           //cout <<"strip candidata: "<<d->GetCoord2()<<endl;
  if(debuglevel>0) IlcDebug(1,Form("4) Strip: %f",y/q));
  (*n)++;
  
  if (*n==kMax) {
     Error("FindClustersSSD","Too many 1D clusters !");
     return;
  }
  //cout <<"arriva a chiamare questo"<<endl;
  FindClustersSSD(lay,neg, nn, pos, np);
}

//1 sensor version
void IlcTARGETClusterFinderV2SSD::FindClustersSSDv1Sens(Int_t lay,TClonesArray *alldigits) {
  //------------------------------------------------------------
  // Actual SSD cluster finder
  //------------------------------------------------------------
  //cout <<"Chiama questo metodo!!!"<<endl;
  Int_t smaxall=alldigits->GetEntriesFast();
  if (smaxall==0) {/*cout <<"non ci sono digits! layer: "<<lay<<endl;*/ return;}
  TObjArray *digits = new TObjArray;
  for (Int_t i=0;i<smaxall; i++){
    IlcTARGETdigitSSD *d=(IlcTARGETdigitSSD*)alldigits->UncheckedAt(i);
    //cout <<"coord1: "<<d->GetCoord1()<<" coord2: "<<d->GetCoord2()<<" segnale: "<<d->GetSignal()<<endl;
    if (d->GetSignal()<3) continue;
    //cout <<"digit number: "<<i<<endl;
    digits->AddLast(d);
  }
  Int_t smax = digits->GetEntriesFast();
//cout <<"numero dei digits: "<<smax<<endl;
  if (smax==0) return;
  
  const Int_t kMax=1000;
  Int_t nn=0; 
  Ilc1Dcluster neg[kMax];
  Float_t y=0., q=0., qmax=0.; 
  Int_t lab[4]={-2,-2,-2,-2};
  
  IlcTARGETdigitSSD *d=(IlcTARGETdigitSSD*)digits->UncheckedAt(0);
  q += d->GetSignal();
  IlcDebug(1,Form("adesso salva come negativo"));
  IlcDebug(1,Form("strip:  %d side: %d",d->GetCoord2(),d->GetCoord1()));
  y += d->GetCoord2()*d->GetSignal();
  qmax=d->GetSignal();
  lab[0]=d->GetTrack(0); lab[1]=d->GetTrack(1); lab[2]=d->GetTrack(2);
  Int_t curr=d->GetCoord2();
      //cout <<"first strip: "<<curr<<endl;
      //cout <<"first coord1: "<<d->GetCoord1()<<endl; 

  Int_t *n=&nn;
  Ilc1Dcluster *c=neg;
  
  Int_t nd=1;
  Int_t milab[10];
  for (Int_t ilab=0;ilab<10;ilab++){
    milab[ilab]=-2;
  }
  milab[0]=d->GetTrack(0); milab[1]=d->GetTrack(1); milab[2]=d->GetTrack(2);

  for (Int_t s=1; s<smax; s++) {
    d=(IlcTARGETdigitSSD*)digits->UncheckedAt(s);      
    Int_t strip=d->GetCoord2();
    IlcDebug(1,Form("strip:  %d side: %d",d->GetCoord2(),d->GetCoord1()));
      //cout <<"next strip: "<<strip<<endl;      
      //cout <<"coord1: "<<d->GetCoord1()<<endl; 
      //cout <<"strip and current: "<<strip<< " " <<curr<<endl;
    if ((strip-curr)>2) {

      c[*n].SetY(y/q);
      c[*n].SetQ(q);
      c[*n].SetNd(nd);
      CheckLabels2(milab);
      c[*n].SetLabels(milab);
      IlcDebug(1,Form("1) numero digit sospetti: %d",nd));
/*	 if (nd>31&&nd<35) {
      c[*n].SetY(y/q-0.25*nd);
      c[*n].SetQ(0.5*q);
           //cout <<"1) inserisce il cluster: "<<endl;
           //cout <<"Y: "<<c[*n].GetY()<<endl;
           //cout <<"Q: "<<c[*n].GetQ()<<endl;
           //cout <<"Nd: "<<c[*n].GetNd()<<endl;
           //cout <<"Label: "<<c[*n].GetLabel(0)<<endl;

      cout <<"1) Strip: "<<y/q-0.25*nd<<endl;
      (*n)++;
      if (*n==kMax) {
      Error("FindClustersSSD","Too many 1D clusters !");
      return;
    }
           //cout <<"!!!!!!!!!!!!!!!!!!!!!!! 1) secondo cluster sospetto"<<endl;
      cout <<"2) Strip: "<<y/q+0.25*nd<<endl;
      c[*n].SetY(y/q+0.25*nd);
      c[*n].SetQ(0.5*q);
      c[*n].SetNd(nd);
      c[*n].SetLabels(milab);
    }	 */
           //cout <<"2) inserisce il cluster: "<<endl;
           //cout <<"Y: "<<c[*n].GetY()<<endl;
           //cout <<"Q: "<<c[*n].GetQ()<<endl;
           //cout <<"Nd: "<<c[*n].GetNd()<<endl;
           //cout <<"Label: "<<c[*n].GetLabel(0)<<endl;
           //cout <<"strip candidata: "<<curr<<endl;
      IlcDebug(1,Form("3) Strip: %f",y/q));
      (*n)++;
      if (*n==kMax) {
        Error("FindClustersSSD","Too many 1D clusters !");
        return;
      }
      y=q=qmax=0.;
      nd=0;
      lab[0]=lab[1]=lab[2]=-2;
      //
      for (Int_t ilab=0;ilab<10;ilab++){
        milab[ilab]=-2;
      }
      //
    }
    q += d->GetSignal();
    y += d->GetCoord2()*d->GetSignal();
    nd++;
    if (d->GetSignal()>qmax) {
      qmax=d->GetSignal();
      lab[0]=d->GetTrack(0); lab[1]=d->GetTrack(1); lab[2]=d->GetTrack(2);
    }
    for (Int_t ilab=0;ilab<10;ilab++) {
      if (d->GetTrack(ilab)>=0) AddLabel(milab, (d->GetTrack(ilab))); 
    }
    curr=strip;
  }
//cout <<"2) aggiungo cluster"<<endl;
         //cout <<"y: "<<y<<" q: "<<q<<" nd:" <<nd<<" y/q: "<<y/q<<endl;
  c[*n].SetY(y/q);
  c[*n].SetQ(q);
  c[*n].SetNd(nd);
  c[*n].SetLabels(lab);
  //Split suspiciously big cluster
// cout <<"numero digit: "<<nd<<endl;
  IlcDebug(1,Form("2) numero digit sospetti: %d",nd));
/*  if (nd>31 && nd<35) {
     //cout <<"!!!!!!!!!!!!!!!!!!!!!!! 2) split cluster sospetto"<<endl;
     //cout <<"!!!!!!!!!!!!!!!!!!!!!!! 2) primo cluster sospetto"<<endl;
  c[*n].SetY(y/q-0.25*nd);
  c[*n].SetQ(0.5*q);
           //cout <<"3) inserisce il cluster: "<<endl;
           //cout <<"Y: "<<c[*n].GetY()<<endl;
           //cout <<"Q: "<<c[*n].GetQ()<<endl;
           //cout <<"Nd: "<<c[*n].GetNd()<<endl;
           //cout <<"Label: "<<c[*n].GetLabel(0)<<endl;

  (*n)++;
  if (*n==kMax) {
  Error("FindClustersSSD","Too many 1D clusters !");
  return;
}
      //cout <<"!!!!!!!!!!!!!!!!!!!!!!! 2) secondo cluster sospetto"<<endl;
  c[*n].SetY(y/q+0.25*nd);
  c[*n].SetQ(0.5*q);
  c[*n].SetNd(nd);
  c[*n].SetLabels(lab);
}*/
           //cout <<"4) inserisce il cluster: "<<endl;
           //cout <<"Y: "<<c[*n].GetY()<<endl;
           //cout <<"Q: "<<c[*n].GetQ()<<endl;
           //cout <<"Nd: "<<c[*n].GetNd()<<endl;
           //cout <<"Label: "<<c[*n].GetLabel(0)<<endl;
           //cout <<"strip candidata: "<<d->GetCoord2()<<endl;
  IlcDebug(1,Form("4) Strip: %f",y/q));
  (*n)++;
  if (*n==kMax) {
    Error("FindClustersSSD","Too many 1D clusters !");
    return;
  }
  //cout <<"arriva a chiamare questo"<<endl;
  FindClustersSSD(lay,neg, nn);
}


void IlcTARGETClusterFinderV2SSD::RawdataToClusters(IlcRawReader* rawReader,TClonesArray** clusters){

    //------------------------------------------------------------
  // This function creates TARGET clusters from raw data
  //------------------------------------------------------------
  rawReader->Reset();
  IlcTARGETRawStreamSSD inputSSD(rawReader);
  FindClustersSSD(&inputSSD,clusters);
  
}

void IlcTARGETClusterFinderV2SSD::FindClustersSSD(IlcTARGETRawStream* input, 
					TClonesArray** clusters) 
{
  //------------------------------------------------------------
  // Actual SSD cluster finder for raw data
  //------------------------------------------------------------
  Int_t nClustersSSD = 0;
  const Int_t kMax = 1000;
  Ilc1Dcluster clusters1D[2][kMax];
  Int_t nClusters[2] = {0, 0};
  Int_t lab[3]={-2,-2,-2};
  Float_t q = 0.;
  Float_t y = 0.;
  Int_t nDigits = 0;
  Int_t prevStrip = -1;
  Int_t prevFlag = -1;
  Int_t prevModule = -1;

  // read raw data input stream
  while (kTRUE) {
    Bool_t next = input->Next();

    if(input->GetSignal()<3 && next) continue;
    // check if a new cluster starts
    Int_t strip = input->GetCoord2();
    Int_t flag = input->GetCoord1();
    if ((!next || (input->GetModuleID() != prevModule)||
	 (strip-prevStrip > 1) || (flag != prevFlag)) &&
	(nDigits > 0)) {
      if (nClusters[prevFlag] == kMax) {
	Error("FindClustersSSD", "Too many 1D clusters !");
	return;
      }
      Ilc1Dcluster& cluster = clusters1D[prevFlag][nClusters[prevFlag]++];
      cluster.SetY(y/q);
      cluster.SetQ(q);
      cluster.SetNd(nDigits);
      cluster.SetLabels(lab);

      //Split suspiciously big cluster
// cout <<"numero digit: "<<nDigits<<endl;
      if(IlcDebugLevelClass()>0)
	IlcDebug(1,Form("3) numero digit sospetti: %d",nDigits));
      if (nDigits > 25&&nDigits < 35) {
	cluster.SetY(y/q - 0.25*nDigits);
        cluster.SetQ(0.5*q);
	if (nClusters[prevFlag] == kMax) {
	  Error("FindClustersSSD", "Too many 1D clusters !");
	  return;
	}
	Ilc1Dcluster& cluster2 = clusters1D[prevFlag][nClusters[prevFlag]++];
	cluster2.SetY(y/q + 0.25*nDigits);
	cluster2.SetQ(0.5*q);
	cluster2.SetNd(nDigits);
	cluster2.SetLabels(lab);
      }
      y = q = 0.;
      nDigits = 0;
    }

    if (!next || (input->GetModuleID() != prevModule)) {
      Int_t iModule = prevModule;

      // when all data from a module was read, search for clusters
      if (prevFlag >= 0) {
	clusters[iModule] = new TClonesArray("IlcTARGETRecPoint");
	fModule = iModule;
	FindClustersSSD(0,&clusters1D[0][0], nClusters[0], 
			&clusters1D[1][0], nClusters[1], clusters[iModule]);
	Int_t numClusters = clusters[iModule]->GetEntriesFast();
	nClustersSSD += numClusters;
      }

      if (!next) break;
      nClusters[0] = nClusters[1] = 0;
      y = q = 0.;
      nDigits = 0;
    }

    // add digit to current cluster
    q += input->GetSignal();
    y += strip * input->GetSignal();
    nDigits++;
    prevStrip = strip;
    prevFlag = flag;
    prevModule = input->GetModuleID();

  }

  Info("FindClustersSSD", "found clusters in TARGET SSD: %d", nClustersSSD);
}

//2 sensor version
void IlcTARGETClusterFinderV2SSD::
FindClustersSSD(Int_t lay,Ilc1Dcluster* neg, Int_t nn, 
		Ilc1Dcluster* pos, Int_t np,
		TClonesArray *clusters) {
  //------------------------------------------------------------
  // Actual SSD cluster finder
  //------------------------------------------------------------
  int debuglevel=IlcDebugLevelClass();
  TClonesArray &cl=*clusters;
  //
  Float_t tanp=fTanP[lay], tann=fTanN[lay];
  if (fModule>fLastSSD1) {tann=fTanP[lay]; tanp=fTanN[lay];}
  Int_t idet=fNdet[fModule];
  Int_t ncl=0;
  //

  Int_t negativepair[30000];
  Int_t cnegative[3000];  
  Int_t cused1[3000];
  Int_t positivepair[30000];
  Int_t cpositive[3000];
  Int_t cused2[3000];
  for (Int_t i=0;i<3000;i++) {cnegative[i]=0; cused1[i]=0;}
  for (Int_t i=0;i<3000;i++) {cpositive[i]=0; cused2[i]=0;}
  for (Int_t i=0;i<30000;i++) {negativepair[i]=0; positivepair[i]=0;}
  static Short_t pairs[1000][1000];
  memset(pairs,0,sizeof(Short_t)*1000000);
//   Short_t ** pairs = new Short_t*[1000];
//   for (Int_t i=0; i<1000; i++) {
//     pairs[i] = new Short_t[1000];
//     memset(pairs[i],0,sizeof(Short_t)*1000);
//   }  
  //
  // find available pairs
  //
  if(debuglevel>0){
    IlcDebug(1,Form("numero di strip positive: %d",np));
    IlcDebug(1,Form("numero di strip negative: %d",nn));
  }
  for (Int_t i=0; i<np; i++) {
    //cout <<"1)trova coppia per np: "<<np<<endl;
    Float_t yp=pos[i].GetY()*fYpitchSSD[lay]; 

    if (pos[i].GetQ()<3) {/*cout <<"1)ritorna qui, np: "<<np<<endl;*/ continue;}

    for (Int_t j=0; j<nn; j++) {
//cout <<"1)trova coppia per nn: "<<nn<<endl;

      if (neg[j].GetQ()<3) {/*cout <<"1)ritorna qui, nn: "<<nn<<endl;*/ continue;}
      Float_t yn=neg[j].GetY()*fYpitchSSD[lay];
      if(debuglevel>0){
	IlcDebug(1,Form("2 dopo strip P: %f carica: %f",pos[i].GetY(),pos[i].GetQ()));
	IlcDebug(1,Form("2 dopo strip N: %f carica: %f",neg[j].GetY(),neg[j].GetQ()));
      }
      Float_t zt=(2*fHlSSD[lay]*tanp + yp - yn)/(tann+tanp);
      Float_t yt=yn + tann*zt;
      zt-=fHlSSD[lay]; yt-=fHwSSD[lay];
      //cout <<"Dan arriva qui!!!"<<endl;
      //cout <<"TMath::Abs(yt): "<<TMath::Abs(yt)<<endl;
      //cout<<"TMath::Abs(zt): "<<TMath::Abs(zt)<<endl;
      //cout<<"neg[j].GetNd(): "<<neg[j].GetNd()<<endl;
      //cout<<"pos[i].GetNd(): "<<pos[i].GetNd()<<endl;
//      if ((nn==np) && (i!=j)) continue;
      if (TMath::Abs(yt)<fHwSSD[lay]/*+0.01*/)
      if (TMath::Abs(zt)<fHlSSD[lay]/*+0.01*(neg[j].GetNd()+pos[i].GetNd())*/) {
 	negativepair[i*10+cnegative[i]] =j;  //index
	positivepair[j*10+cpositive[j]] =i;
        //cout <<"negativepair[i*10+cnegative[i]]: "<<negativepair[i*10+cnegative[i]]<<endl;
        //cout <<"positivepair[i*10+cpositive[i]]: "<<positivepair[i*10+cpositive[i]]<<endl;

	cnegative[i]++;  //counters
	cpositive[j]++;	
	if(debuglevel>0){
	  IlcDebug(1,Form("trovata coppia"));
          IlcDebug(1,Form("cpositive[%d]: %d",j,cpositive[j]));
          IlcDebug(1,Form("cnegative[%d]: %d",i,cnegative[i]));
	}
	pairs[i][j]=100;
      }
      if(debuglevel>0)
	IlcDebug(1,Form("cambia"));
    }
  }
  //
  for (Int_t i=0; i<np; i++) {
//cout <<"2)trova coppia per np: "<<np<<endl;
    Float_t yp=pos[i].GetY()*fYpitchSSD[lay]; 
    if (pos[i].GetQ()<3) { if(debuglevel>0) IlcDebug(1,Form("2)ritorna qui, np: %d",np));
     continue;}
    for (Int_t j=0; j<nn; j++) {
//cout <<"2)trova coppia per nn: "<<nn<<endl;
      if (neg[j].GetQ()<3) {/*cout <<"2)ritorna qui, nn: "<<nn<<endl;*/ continue; }
      if(debuglevel>0){
        IlcDebug(1,Form("cpositive[%d]: %d",j,cpositive[j]));
        IlcDebug(1,Form("cnegative[%d]: %d",i,cnegative[i]));
      }
      if (cpositive[j]&&cnegative[i]) { 
	if(debuglevel>0) IlcDebug(1,Form("ritorna in questo punto"));
	continue;
      }
      Float_t yn=neg[j].GetY()*fYpitchSSD[lay];
      Float_t zt=(2*fHlSSD[lay]*tanp + yp - yn)/(tann+tanp);
      Float_t yt=yn + tann*zt;
      zt-=fHlSSD[lay]; yt-=fHwSSD[lay];
      if (TMath::Abs(yt)<fHwSSD[lay]/*+0.01*/)
      if (TMath::Abs(zt)<fHlSSD[lay]/*+0.01*/) {
	if (cnegative[i]==0) pos[i].SetNd(100);  // not available pair
	if (cpositive[j]==0) neg[j].SetNd(100);  // not available pair
	negativepair[i*10+cnegative[i]] =j;  //index
	positivepair[j*10+cpositive[j]] =i;
	cnegative[i]++;  //counters
	cpositive[j]++;	
	pairs[i][j]=100;
      }
    }
  }
  //
  Float_t lp[5];
  Int_t milab[10];
  Double_t ratio;
  
  //
  // sign gold tracks
  //
  for (Int_t ip=0;ip<np;ip++){
    Float_t ybest=1000,zbest=1000,qbest=0;
    //
    // select gold clusters
    if ( (cnegative[ip]==1) && cpositive[negativepair[10*ip]]==1){
      //cout <<"seleziona gold cluster"<<endl; 
      Float_t yp=pos[ip].GetY()*fYpitchSSD[lay]; 
      Int_t j = negativepair[10*ip];      
      ratio = (pos[ip].GetQ()-neg[j].GetQ())/(pos[ip].GetQ()+neg[j].GetQ());
      //

      Float_t yn=neg[j].GetY()*fYpitchSSD[lay];
      //cout <<"yn = "<< neg[j].GetY() << " * "<<fYpitchSSD[lay]<< " = "<<yn<<endl;
      Float_t zt=(2*fHlSSD[lay]*tanp + yp - yn)/(tann+tanp);
      //cout <<"zt = "<<"2 * "<<fHlSSD[lay]<<" * "<<tanp<< " + " <<yp <<" - "<<yn<< " / "<<tann << " + " <<tanp<< " = " <<zt<<endl;  

      Float_t yt=yn + tann*zt;
      zt-=fHlSSD[lay]; yt-=fHwSSD[lay];
      ybest=yt; zbest=zt; 
      qbest=0.5*(pos[ip].GetQ()+neg[j].GetQ());
      lp[0]=-(-ybest+fYshift[fModule]);
      lp[1]=zbest+fZshift[fModule];
      if (lay<=4) { 
       lp[1]=0.0;      
       lp[2]=4.5e-4*4.5e-4;  //SigmaY2
       lp[3]=4.58155*4.58155;  //SigmaZ2      
      } else {
       lp[2]=7.7e-4*7.7e-4;  //SigmaY2
       lp[3]=438e-4*438e-4;  //SigmaZ2
      }
      lp[4]=qbest;        //Q
      for (Int_t ilab=0;ilab<10;ilab++) milab[ilab]=-2;
      for (Int_t ilab=0;ilab<3;ilab++){
	milab[ilab] = pos[ip].GetLabel(ilab);
	milab[ilab+3] = neg[j].GetLabel(ilab);
      }
      //
      CheckLabels2(milab);
      milab[3]=(((ip<<12) + j)<<12) + idet; // pos|neg|det
      Int_t info[3] = {pos[ip].GetNd(),neg[j].GetNd(),fNlayer[fModule]};
      IlcTARGETRecPoint * cl2;
      if(clusters){
	cl2 = new (cl[ncl]) IlcTARGETRecPoint(fModule,fDetTypeRec->GetTARGETgeom(),milab,lp,info);
	cl2->SetChargeRatio(ratio);    	
        cl2->SetXZ(fModule,lp[0],lp[1]);
	cl2->SetType(1);
        cl2->SetDetector(fNdet[fModule]%fDetTypeRec->GetTARGETgeom()->GetNdetectors(lay+fParam->GetNlayers()+fParam->GetNendcaps()+1));
        cl2->SetModule(fModule);
	pairs[ip][j]=1;
	if(debuglevel>0)
          IlcDebug(1,Form("limite multi cluster: %d",pos[ip].GetNd()+neg[j].GetNd()));
	if ((pos[ip].GetNd()+neg[j].GetNd())>6){ //multi cluster
	  if(debuglevel>0) IlcDebug(1,Form("multi cluster!!!"));
	  cl2->SetType(2);
	  pairs[ip][j]=2;
	}
	cused1[ip]++;
	cused2[j]++;
	
      }
      else{
	cl2 = new IlcTARGETRecPoint(fModule,fDetTypeRec->GetTARGETgeom(),milab,lp,info);	
	cl2->SetChargeRatio(ratio);    	
        cl2->SetXZ(fModule,lp[0],lp[1]);
	cl2->SetType(1);
        cl2->SetModule(fModule);
        cl2->SetDetector(fNdet[fModule]%fDetTypeRec->GetTARGETgeom()->GetNdetectors(lay+fParam->GetNlayers()+fParam->GetNendcaps()+1));
	pairs[ip][j]=1;
	if(debuglevel>0)
	  IlcDebug(1,Form("limite multi cluster: %d",pos[ip].GetNd()+neg[j].GetNd()));
	if ((pos[ip].GetNd()+neg[j].GetNd())>6){ //multi cluster
          if(IlcDebugLevelClass()>0)IlcDebug(1,Form("multi cluster!!!"));
	  cl2->SetType(2);
	  pairs[ip][j]=2;
	}
	cused1[ip]++;
	cused2[j]++;
        if(debuglevel>0) cout <<"1) num cluster:"<<ncl<<" Type: "<<cl2->GetType()<<" SSD Layer: "<<info[2]<<" Module: "<<fModule<<" pos[x]: "<<lp[0]<<" pos[z]: "<<lp[1]<<endl;
        //cout <<"valore di XZ:"<<cl2->GetDetLocalX()<<endl;
	fDetTypeRec->AddRecPoint(*cl2);
      }
      ncl++;
    }
  }
    
  for (Int_t ip=0;ip<np;ip++){
    Float_t ybest=1000,zbest=1000,qbest=0;
    //
    //
    // select "silber" cluster
    if ( cnegative[ip]==1 && cpositive[negativepair[10*ip]]==2){
      Int_t in  = negativepair[10*ip];
      Int_t ip2 = positivepair[10*in];
      if (ip2==ip) ip2 =  positivepair[10*in+1];
      Float_t pcharge = pos[ip].GetQ()+pos[ip2].GetQ();
      if (TMath::Abs(pcharge-neg[in].GetQ())<10){
	//
	// add first pair
	if (pairs[ip][in]==100){  //
	  Float_t yp=pos[ip].GetY()*fYpitchSSD[lay]; 
	  Float_t yn=neg[in].GetY()*fYpitchSSD[lay];
	  Float_t zt=(2*fHlSSD[lay]*tanp + yp - yn)/(tann+tanp);
	  Float_t yt=yn + tann*zt;
	  zt-=fHlSSD[lay]; yt-=fHwSSD[lay];
	  ybest =yt;  zbest=zt; 
	  qbest =pos[ip].GetQ();
	  lp[0]=-(-ybest+fYshift[fModule]);
	  lp[1]=zbest+fZshift[fModule];
          if (lay<=4) { 
           lp[1]=0.0;
           lp[2]=4.5e-4*4.5e-4;  //SigmaY2
           lp[3]=4.58155*4.58155;  //SigmaZ2  
          } else {
           lp[2]=7.7e-4*7.7e-4;  //SigmaY2
           lp[3]=438e-4*438e-4;  //SigmaZ2
          }

	  
	  lp[4]=qbest;        //Q
	  for (Int_t ilab=0;ilab<10;ilab++) milab[ilab]=-2;
	  for (Int_t ilab=0;ilab<3;ilab++){
	    milab[ilab] = pos[ip].GetLabel(ilab);
	    milab[ilab+3] = neg[in].GetLabel(ilab);
	  }
	  //
	  CheckLabels2(milab);
	  ratio = (pos[ip].GetQ()-neg[in].GetQ())/(pos[ip].GetQ()+neg[in].GetQ());
	  milab[3]=(((ip<<10) + in)<<10) + idet; // pos|neg|det
	  Int_t info[3] = {pos[ip].GetNd(),neg[in].GetNd(),fNlayer[fModule]};

	  IlcTARGETRecPoint * cl2;
	  if(clusters){
	    cl2 = new (cl[ncl]) IlcTARGETRecPoint(fModule,fDetTypeRec->GetTARGETgeom(),milab,lp,info);
	    cl2->SetChargeRatio(ratio); 
            cl2->SetXZ(fModule,lp[0],lp[1]);
	    cl2->SetType(5);
            cl2->SetDetector(fNdet[fModule]%fDetTypeRec->GetTARGETgeom()->GetNdetectors(lay+fParam->GetNlayers()+fParam->GetNendcaps()+1));
            cl2->SetModule(fModule);
	    pairs[ip][in] = 5;
	    if(debuglevel>0)
	      IlcDebug(1,Form("limite multi cluster: %d",pos[ip].GetNd()+neg[in].GetNd()));
	    if ((pos[ip].GetNd()+neg[in].GetNd())>6){ //multi cluster
              if(debuglevel>0) IlcDebug(1,Form("multi cluster!!!"));
	      cl2->SetType(6);
	      pairs[ip][in] = 6;
	    }	    
	  }
	  else{
	    cl2 = new IlcTARGETRecPoint(fModule,fDetTypeRec->GetTARGETgeom(),milab,lp,info);
	    cl2->SetChargeRatio(ratio);    	
            cl2->SetXZ(fModule,lp[0],lp[1]);
	    cl2->SetType(5);
            cl2->SetDetector(fNdet[fModule]%fDetTypeRec->GetTARGETgeom()->GetNdetectors(lay+fParam->GetNlayers()+fParam->GetNendcaps()+1));
            cl2->SetModule(fModule);
	    pairs[ip][in] = 5;
	    if(debuglevel>0)
	      IlcDebug(1,Form("limite multi cluster: %d",pos[ip].GetNd()+neg[in].GetNd()));
	    if ((pos[ip].GetNd()+neg[in].GetNd())>6){ //multi cluster
	      if(IlcDebugLevelClass()>0) IlcDebug(1,Form("multi cluster"));
	      cl2->SetType(6);
	      pairs[ip][in] = 6;
	    }
            if(debuglevel>0) cout <<"2) num cluster:"<<ncl<<" Type: "<<cl2->GetType()<<" SSD Layer: "<<info[2]<<" Module: "<<fModule<<" pos[x]: "<<lp[0]<<" pos[z]: "<<lp[1]<<endl;
        //cout <<"valore di XZ:"<<cl2->GetDetLocalX()<<endl;
	    
	    fDetTypeRec->AddRecPoint(*cl2);
	  }
	  ncl++;
	}
	
	//
	// add second pair
	
      //	if (!(cused1[ip2] || cused2[in])){  //
	if (pairs[ip2][in]==100){
          
	  Float_t yp=pos[ip2].GetY()*fYpitchSSD[lay];
	  Float_t yn=neg[in].GetY()*fYpitchSSD[lay];
	  Float_t zt=(2*fHlSSD[lay]*tanp + yp - yn)/(tann+tanp);
	  Float_t yt=yn + tann*zt;
	  zt-=fHlSSD[lay]; yt-=fHwSSD[lay];
	  ybest =yt;  zbest=zt; 
	  qbest =pos[ip2].GetQ();
	  lp[0]=-(-ybest+fYshift[fModule]);
	  lp[1]=zbest+fZshift[fModule];
          if (lay<=4) { 
           lp[1]=0.0;
           lp[2]=4.5e-4*4.5e-4;  //SigmaY2
           lp[3]=4.58155*4.58155;  //SigmaZ2  
          } else {
           lp[2]=7.7e-4*7.7e-4;  //SigmaY2
           lp[3]=438e-4*438e-4;  //SigmaZ2
          }

	  
	  lp[4]=qbest;        //Q
	  for (Int_t ilab=0;ilab<10;ilab++) milab[ilab]=-2;
	  for (Int_t ilab=0;ilab<3;ilab++){
	    milab[ilab] = pos[ip2].GetLabel(ilab);
	    milab[ilab+3] = neg[in].GetLabel(ilab);
	  }
	  //
	  CheckLabels2(milab);
	  ratio = (pos[ip2].GetQ()-neg[in].GetQ())/(pos[ip2].GetQ()+neg[in].GetQ());
	  milab[3]=(((ip2<<12) + in)<<12) + idet; // pos|neg|det
	  Int_t info[3] = {pos[ip2].GetNd(),neg[in].GetNd(),fNlayer[fModule]};
	  
	  IlcTARGETRecPoint * cl2;
	  if(clusters){
	    cl2 = new (cl[ncl]) IlcTARGETRecPoint(fModule,fDetTypeRec->GetTARGETgeom(),milab,lp,info);
	    cl2->SetChargeRatio(ratio);    	
            cl2->SetXZ(fModule,lp[0],lp[1]);
	    cl2->SetType(5);
            cl2->SetDetector(fNdet[fModule]%fDetTypeRec->GetTARGETgeom()->GetNdetectors(lay+fParam->GetNlayers()+fParam->GetNendcaps()+1));
            cl2->SetModule(fModule);
	    pairs[ip2][in] =5;
            if(debuglevel>0)IlcDebug(1,Form("limite multi cluster: %d",pos[ip2].GetNd()+neg[in].GetNd()));
	    if ((pos[ip2].GetNd()+neg[in].GetNd())>6){ //multi cluster
              if(debuglevel>0) IlcDebug(1,Form("multi cluster"));
	      cl2->SetType(6);
	      pairs[ip2][in] =6;
	    }
	  }
	  else{
	    cl2 = new IlcTARGETRecPoint(fModule,fDetTypeRec->GetTARGETgeom(),milab,lp,info);
	    cl2->SetChargeRatio(ratio);    	
            cl2->SetXZ(fModule,lp[0],lp[1]);
	    cl2->SetType(5);
            cl2->SetDetector(fNdet[fModule]%fDetTypeRec->GetTARGETgeom()->GetNdetectors(lay+fParam->GetNlayers()+fParam->GetNendcaps()+1));
            cl2->SetModule(fModule);
	    pairs[ip2][in] =5;
            if(debuglevel>0) IlcDebug(1,Form("limite multi cluster: %d",pos[ip2].GetNd()+neg[in].GetNd()));
	    if ((pos[ip2].GetNd()+neg[in].GetNd())>6){ //multi cluster
              IlcDebug(1,Form("multi cluster!!!"));
	      cl2->SetType(6);
	      pairs[ip2][in] =6;
	    }
            if(debuglevel>0) cout <<"3) num cluster:"<<ncl<<" Type: "<<cl2->GetType()<<" SSD Layer: "<<info[2]<<" Module: "<<fModule<<" pos[x]: "<<lp[0]<<" pos[z]: "<<lp[1]<<endl;
	    fDetTypeRec->AddRecPoint(*cl2);
	  }
	  ncl++;
	}	
	cused1[ip]++;
	cused1[ip2]++;
	cused2[in]++;
      }
    }    
  }

  //  
  for (Int_t jn=0;jn<nn;jn++){
    if (cused2[jn]) continue;
    Float_t ybest=1000,zbest=1000,qbest=0;
    // select "silber" cluster
    if ( cpositive[jn]==1 && cnegative[positivepair[10*jn]]==2){
      Int_t ip  = positivepair[10*jn];
      Int_t jn2 = negativepair[10*ip];
      if (jn2==jn) jn2 =  negativepair[10*ip+1];
      Float_t pcharge = neg[jn].GetQ()+neg[jn2].GetQ();
      //
      if (TMath::Abs(pcharge-pos[ip].GetQ())<10){
	//
	// add first pair
	//	if (!(cused1[ip]||cused2[jn])){
	if (pairs[ip][jn]==100){
	  Float_t yn=neg[jn].GetY()*fYpitchSSD[lay]; 
	  Float_t yp=pos[ip].GetY()*fYpitchSSD[lay];
	  Float_t zt=(2*fHlSSD[lay]*tanp + yp - yn)/(tann+tanp);
	  Float_t yt=yn + tann*zt;
	  zt-=fHlSSD[lay]; yt-=fHwSSD[lay];
	  ybest =yt;  zbest=zt; 
	  qbest =neg[jn].GetQ();
	  lp[0]=-(-ybest+fYshift[fModule]);
	  lp[1]=zbest+fZshift[fModule];
          if (lay<=4) { 
           lp[1]=0.0;
           lp[2]=4.5e-4*4.5e-4;  //SigmaY2
           lp[3]=4.58155*4.58155;  //SigmaZ2  
          } else {
           lp[2]=7.7e-4*7.7e-4;  //SigmaY2
           lp[3]=438e-4*438e-4;  //SigmaZ2
          }

	  
	  lp[4]=qbest;        //Q
	  for (Int_t ilab=0;ilab<10;ilab++) milab[ilab]=-2;
	  for (Int_t ilab=0;ilab<3;ilab++){
	    milab[ilab] = pos[ip].GetLabel(ilab);
	    milab[ilab+3] = neg[jn].GetLabel(ilab);
	  }
	  //
	  CheckLabels2(milab);
	  ratio = (pos[ip].GetQ()-neg[jn].GetQ())/(pos[ip].GetQ()+neg[jn].GetQ());
	  milab[3]=(((ip<<12) + jn)<<12) + idet; // pos|neg|det
	  Int_t info[3] = {pos[ip].GetNd(),neg[jn].GetNd(),fNlayer[fModule]};

	  IlcTARGETRecPoint * cl2;
	  if(clusters){
	    cl2 = new (cl[ncl]) IlcTARGETRecPoint(fModule,fDetTypeRec->GetTARGETgeom(),milab,lp,info);
	    cl2->SetChargeRatio(ratio);    
            cl2->SetXZ(fModule,lp[0],lp[1]);	
            cl2->SetDetector(fNdet[fModule]%fDetTypeRec->GetTARGETgeom()->GetNdetectors(lay+fParam->GetNlayers()+fParam->GetNendcaps()+1));
	    cl2->SetType(7);
            cl2->SetModule(fModule);
	    pairs[ip][jn] =7;
            if(debuglevel>0) IlcDebug(1,Form("limite multi cluster: %d",pos[ip].GetNd()+neg[jn].GetNd()));
	    if ((pos[ip].GetNd()+neg[jn].GetNd())>6){ //multi cluster
              if(debuglevel>0) IlcDebug(1,Form("multi cluster!!!"));
	      cl2->SetType(8);
	      pairs[ip][jn]=8;
	    }

	  }
	  else{
	    cl2 = new IlcTARGETRecPoint(fModule,fDetTypeRec->GetTARGETgeom(),milab,lp,info);
	    cl2->SetChargeRatio(ratio);    
            cl2->SetXZ(fModule,lp[0],lp[1]);	
	    cl2->SetType(7);
            cl2->SetDetector(fNdet[fModule]%fDetTypeRec->GetTARGETgeom()->GetNdetectors(lay+fParam->GetNlayers()+fParam->GetNendcaps()+1));
            cl2->SetModule(fModule);
	    pairs[ip][jn] =7;
            if(debuglevel>0) IlcDebug(1,Form("limite multi cluster: %d",pos[ip].GetNd()+neg[jn].GetNd()));
	    if ((pos[ip].GetNd()+neg[jn].GetNd())>6){ //multi cluster
              if(debuglevel>0) IlcDebug(1,Form("multi cluster!!!"));
	      cl2->SetType(8);
	      pairs[ip][jn]=8;
	    }
            if(debuglevel>0) cout <<"4) num cluster:"<<ncl<<" Type: "<<cl2->GetType()<<" SSD Layer: "<<info[2]<<" Module: "<<fModule<<" pos[x]: "<<lp[0]<<" pos[z]: "<<lp[1]<<endl;
        //cout <<"valore di XZ:"<<cl2->GetDetLocalX()<<endl;
            
	    fDetTypeRec->AddRecPoint(*cl2);
	  }
	  ncl++;
	}
	//
	// add second pair
	//	if (!(cused1[ip]||cused2[jn2])){
	if (pairs[ip][jn2]==100){
          
	  Float_t yn=neg[jn2].GetY()*fYpitchSSD[lay]; 
	  Double_t yp=pos[ip].GetY()*fYpitchSSD[lay]; 
	  Double_t zt=(2*fHlSSD[lay]*tanp + yp - yn)/(tann+tanp);
	  Double_t yt=yn + tann*zt;
	  zt-=fHlSSD[lay]; yt-=fHwSSD[lay];
	  ybest =yt;  zbest=zt; 
	  qbest =neg[jn2].GetQ();
	  lp[0]=-(-ybest+fYshift[fModule]);
	  lp[1]=zbest+fZshift[fModule];
          if (lay<=4) { 
           lp[1]=0.0;
           lp[2]=4.5e-4*4.5e-4;  //SigmaY2
           lp[3]=4.58155*4.58155;  //SigmaZ2  
          } else {
           lp[2]=7.7e-4*7.7e-4;  //SigmaY2
           lp[3]=438e-4*438e-4;  //SigmaZ2
          }

	  
	  lp[4]=qbest;        //Q
	  for (Int_t ilab=0;ilab<10;ilab++) milab[ilab]=-2;
	  for (Int_t ilab=0;ilab<3;ilab++){
	    milab[ilab] = pos[ip].GetLabel(ilab);
	    milab[ilab+3] = neg[jn2].GetLabel(ilab);
	  }
	  //
	  CheckLabels2(milab);
	  ratio = (pos[ip].GetQ()-neg[jn2].GetQ())/(pos[ip].GetQ()+neg[jn2].GetQ());
	  milab[3]=(((ip<<12) + jn2)<<12) + idet; // pos|neg|det
	  Int_t info[3] = {pos[ip].GetNd(),neg[jn2].GetNd(),fNlayer[fModule]};
	  IlcTARGETRecPoint * cl2;
	  if(clusters){
	    cl2 = new (cl[ncl]) IlcTARGETRecPoint(fModule,fDetTypeRec->GetTARGETgeom(),milab,lp,info);
	    cl2->SetChargeRatio(ratio);    
            cl2->SetXZ(fModule,lp[0],lp[1]);	
            cl2->SetDetector(fNdet[fModule]%fDetTypeRec->GetTARGETgeom()->GetNdetectors(lay+fParam->GetNlayers()+fParam->GetNendcaps()+1));
            cl2->SetModule(fModule);
	    pairs[ip][jn2]=7;
	    cl2->SetType(7);
	    if(debuglevel>0)
	      IlcDebug(1,Form("limite multi cluster: %d",pos[ip].GetNd()+neg[jn2].GetNd()));
	    if ((pos[ip].GetNd()+neg[jn2].GetNd())>6){ //multi cluster
              if(debuglevel>0) IlcDebug(1,Form("multi cluster!!!"));
	      cl2->SetType(8);
	      pairs[ip][jn2]=8;
	    }
	    
	  }
	  else{
	    cl2 = new IlcTARGETRecPoint(fModule,fDetTypeRec->GetTARGETgeom(),milab,lp,info);
	    cl2->SetChargeRatio(ratio);    
            cl2->SetXZ(fModule,lp[0],lp[1]);	
            cl2->SetDetector(fNdet[fModule]%fDetTypeRec->GetTARGETgeom()->GetNdetectors(lay+fParam->GetNlayers()+fParam->GetNendcaps()+1));
            cl2->SetModule(fModule);
	    pairs[ip][jn2]=7;
	    cl2->SetType(7);
            if(debuglevel>0) IlcDebug(1,Form("limite multi cluster: %d",pos[ip].GetNd()+neg[jn2].GetNd()));
	    if ((pos[ip].GetNd()+neg[jn2].GetNd())>6){ //multi cluster
              if(debuglevel>0) IlcDebug(1,Form("multi cluster!!!"));
	      cl2->SetType(8);
	      pairs[ip][jn2]=8;
	    }
            if(debuglevel>0) cout <<"5) num cluster:"<<ncl<<" Type: "<<cl2->GetType()<<" SSD Layer: "<<info[2]<<" Module: "<<fModule<<" pos[x]: "<<lp[0]<<" pos[z]: "<<lp[1]<<endl;
        //cout <<"valore di XZ:"<<cl2->GetDetLocalX()<<endl;
	    fDetTypeRec->AddRecPoint(*cl2);
	  }

	  ncl++;
	}
	cused1[ip]++;
	cused2[jn]++;
	cused2[jn2]++;
      }
    }    
  }
  
  for (Int_t ip=0;ip<np;ip++){
    Float_t ybest=1000,zbest=1000,qbest=0;
    //
    // 2x2 clusters
    //
    if ( (cnegative[ip]<5) && cpositive[negativepair[10*ip]]<5){ 
      Float_t minchargediff =4.;
      Int_t j=-1;
      for (Int_t di=0;di<cnegative[ip];di++){
	Int_t   jc = negativepair[ip*10+di];
	Float_t chargedif = pos[ip].GetQ()-neg[jc].GetQ();
	if (TMath::Abs(chargedif)<minchargediff){
	  j =jc;
	  minchargediff = TMath::Abs(chargedif);
	}
      }
      if (j<0) continue;  // not proper cluster      
      Int_t count =0;
      for (Int_t di=0;di<cnegative[ip];di++){
	Int_t   jc = negativepair[ip*10+di];
	Float_t chargedif = pos[ip].GetQ()-neg[jc].GetQ();
	if (TMath::Abs(chargedif)<minchargediff+3.) count++;
      }
      if (count>1) continue;  // more than one "proper" cluster for positive
      //
      count =0;
      for (Int_t dj=0;dj<cpositive[j];dj++){
	Int_t   ic  = positivepair[j*10+dj];
	Float_t chargedif = pos[ic].GetQ()-neg[j].GetQ();
	if (TMath::Abs(chargedif)<minchargediff+3.) count++;
      }
      if (count>1) continue;  // more than one "proper" cluster for negative
      
      Int_t jp = 0;
      
      count =0;
      for (Int_t dj=0;dj<cnegative[jp];dj++){
	Int_t   ic = positivepair[jp*10+dj];
	Float_t chargedif = pos[ic].GetQ()-neg[jp].GetQ();
	if (TMath::Abs(chargedif)<minchargediff+4.) count++;
      }
      if (count>1) continue;   
      if (pairs[ip][j]<100) continue;
      //
      //almost gold clusters
      Float_t yp=pos[ip].GetY()*fYpitchSSD[lay]; 
      Float_t yn=neg[j].GetY()*fYpitchSSD[lay];
      Float_t zt=(2*fHlSSD[lay]*tanp + yp - yn)/(tann+tanp);
      Float_t yt=yn + tann*zt;
      zt-=fHlSSD[lay]; yt-=fHwSSD[lay];
      ybest=yt; zbest=zt; 
      qbest=0.5*(pos[ip].GetQ()+neg[j].GetQ());
      lp[0]=-(-ybest+fYshift[fModule]);
      lp[1]=zbest+fZshift[fModule];
      if (lay<=4) { 
       lp[1]=0.0;
       lp[2]=4.5e-4*4.5e-4;  //SigmaY2
       lp[3]=4.58155*4.58155;  //SigmaZ2  
      } else {
       lp[2]=7.7e-4*7.7e-4;  //SigmaY2
       lp[3]=438e-4*438e-4;  //SigmaZ2
      }

      lp[4]=qbest;        //Q
      for (Int_t ilab=0;ilab<10;ilab++) milab[ilab]=-2;
      for (Int_t ilab=0;ilab<3;ilab++){
	milab[ilab] = pos[ip].GetLabel(ilab);
	milab[ilab+3] = neg[j].GetLabel(ilab);
      }
      //
      CheckLabels2(milab);
      ratio = (pos[ip].GetQ()-neg[j].GetQ())/(pos[ip].GetQ()+neg[j].GetQ());
      milab[3]=(((ip<<12) + j)<<12) + idet; // pos|neg|det
      Int_t info[3] = {pos[ip].GetNd(),neg[j].GetNd(),fNlayer[fModule]};
      IlcTARGETRecPoint * cl2;
      if(clusters){
	cl2 = new (cl[ncl]) IlcTARGETRecPoint(fModule,fDetTypeRec->GetTARGETgeom(),milab,lp,info);
	cl2->SetChargeRatio(ratio);    	
        cl2->SetXZ(fModule,lp[0],lp[1]);
        cl2->SetDetector(fNdet[fModule]%fDetTypeRec->GetTARGETgeom()->GetNdetectors(lay+fParam->GetNlayers()+fParam->GetNendcaps()+1));
	cl2->SetType(10);
        cl2->SetModule(fModule);
	pairs[ip][j]=10;
        if(debuglevel>0) IlcDebug(1,Form("limite multi cluster: %d",pos[ip].GetNd()+neg[j].GetNd()));
	if ((pos[ip].GetNd()+neg[j].GetNd())>6){ //multi cluster
          if(debuglevel>0) IlcDebug(1,Form("multi cluster!!!"));
	  cl2->SetType(11);
	  pairs[ip][j]=11;
	}
	cused1[ip]++;
	cused2[j]++;      
      }
      else{
	cl2 = new IlcTARGETRecPoint(fModule,fDetTypeRec->GetTARGETgeom(),milab,lp,info);
	cl2->SetChargeRatio(ratio);    	
        cl2->SetXZ(fModule,lp[0],lp[1]);
        cl2->SetDetector(fNdet[fModule]%fDetTypeRec->GetTARGETgeom()->GetNdetectors(lay+fParam->GetNlayers()+fParam->GetNendcaps()+1));
	cl2->SetType(10);
        cl2->SetModule(fModule);
	pairs[ip][j]=10;
        if(debuglevel>0) IlcDebug(1,Form("limite multi cluster: %d",pos[ip].GetNd()+neg[j].GetNd()));
	if ((pos[ip].GetNd()+neg[j].GetNd())>6){ //multi cluster
          if(debuglevel>0) IlcDebug(1,Form("multi cluster!!!"));
	  cl2->SetType(11);
	  pairs[ip][j]=11;
	}
	cused1[ip]++;
	cused2[j]++;      
        if(debuglevel>0) cout <<"6) num cluster:"<<ncl<<" Type: "<<cl2->GetType()<<" SSD Layer: "<<info[2]<<" Module: "<<fModule<<" pos[x]: "<<lp[0]<<" pos[z]: "<<lp[1]<<endl;
        //cout <<"valore di XZ:"<<cl2->GetDetLocalX()<<endl;
	
	fDetTypeRec->AddRecPoint(*cl2);
      }      
      ncl++;
    }

  }
  
  //  
  for (Int_t i=0; i<np; i++) {
    Float_t ybest=1000,zbest=1000,qbest=0;
    Float_t yp=pos[i].GetY()*fYpitchSSD[lay]; 
    if (pos[i].GetQ()<3) continue;
    for (Int_t j=0; j<nn; j++) {
    //    for (Int_t di = 0;di<cpositive[i];di++){
    //  Int_t j = negativepair[10*i+di];
      if (neg[j].GetQ()<3) continue;
      if (cused2[j]||cused1[i]) continue;      
      if(debuglevel>0) IlcDebug(1,Form("pairs[%d][%d]: %d",i,j,pairs[i][j]));
      if (pairs[i][j]>0 &&pairs[i][j]<100) continue;
      ratio = (pos[i].GetQ()-neg[j].GetQ())/(pos[i].GetQ()+neg[j].GetQ());      
      Float_t yn=neg[j].GetY()*fYpitchSSD[lay];
      Float_t zt=(2*fHlSSD[lay]*tanp + yp - yn)/(tann+tanp);
      Float_t yt=yn + tann*zt;
      zt-=fHlSSD[lay]; yt-=fHwSSD[lay];
      if (TMath::Abs(yt)<fHwSSD[lay]/*+0.005*/)
      if (TMath::Abs(zt)<fHlSSD[lay]/*+0.005*(neg[j].GetNd()+pos[i].GetNd())*/) {
        ybest=yt; zbest=zt; 
        qbest=0.5*(pos[i].GetQ()+neg[j].GetQ());
        lp[0]=-(-ybest+fYshift[fModule]);
        lp[1]= zbest+fZshift[fModule];
        if (lay<=4) { 
         lp[1]=0.0;
         lp[2]=4.5e-4*4.5e-4;  //SigmaY2
         lp[3]=4.58155*4.58155;  //SigmaZ2  
        } else {
         lp[2]=7.7e-4*7.7e-4;  //SigmaY2
         lp[3]=438e-4*438e-4;  //SigmaZ2
        }


        lp[4]=qbest;        //Q
	for (Int_t ilab=0;ilab<10;ilab++) milab[ilab]=-2;
	for (Int_t ilab=0;ilab<3;ilab++){
	  milab[ilab] = pos[i].GetLabel(ilab);
	  milab[ilab+3] = neg[j].GetLabel(ilab);
	}
	//
	CheckLabels2(milab);
	milab[3]=(((i<<12) + j)<<12) + idet; // pos|neg|det
	Int_t info[3] = {pos[i].GetNd(),neg[j].GetNd(),fNlayer[fModule]};
	IlcTARGETRecPoint * cl2;
	if(clusters){
	  cl2 = new (cl[ncl]) IlcTARGETRecPoint(fModule,fDetTypeRec->GetTARGETgeom(),milab,lp,info);
	  cl2->SetChargeRatio(ratio);
          cl2->SetXZ(fModule,lp[0],lp[1]);
          cl2->SetDetector(fNdet[fModule]%fDetTypeRec->GetTARGETgeom()->GetNdetectors(lay+fParam->GetNlayers()+fParam->GetNendcaps()+1));
          cl2->SetModule(fModule);
	  cl2->SetType(100+cpositive[j]+cnegative[i]);	  
	}
	else{
	  cl2 = new IlcTARGETRecPoint(fModule,fDetTypeRec->GetTARGETgeom(),milab,lp,info);
	  cl2->SetChargeRatio(ratio);
          cl2->SetModule(fModule);
          cl2->SetXZ(fModule,lp[0],lp[1]);
          cl2->SetDetector(fNdet[fModule]%fDetTypeRec->GetTARGETgeom()->GetNdetectors(lay+fParam->GetNlayers()+fParam->GetNendcaps()+1));
	  cl2->SetType(100+cpositive[j]+cnegative[i]);
          if(debuglevel>0) cout <<"7) num cluster:"<<ncl<<" Type: "<<cl2->GetType()<<" SSD Layer: "<<info[2]<<" Module: "<<fModule<<" pos[x]: "<<lp[0]<<" pos[z]: "<<lp[1]<<endl;
        //cout <<"valore di XZ:"<<cl2->GetDetLocalX()<<endl;

	  fDetTypeRec->AddRecPoint(*cl2);
	}
      	ncl++;
	//cl2->SetType(0);
	/*
	  if (pairs[i][j]<100){
	  printf("problem:- %d\n", pairs[i][j]);
	  }
	  if (cnegative[i]<2&&cpositive[j]<2){
	  printf("problem:- %d\n", pairs[i][j]);
	  }
	*/
      }
    }
  }

//   for (Int_t i=0; i<1000; i++) delete [] pairs[i];
//   delete [] pairs;

}


/////////////////////////////////////////////////////////////////////////////////////////

//1 sensor version

void IlcTARGETClusterFinderV2SSD::
FindClustersSSD(Int_t lay,Ilc1Dcluster* neg, Int_t nn, TClonesArray *clusters) {
  //------------------------------------------------------------
  // Actual SSD cluster finder
  //------------------------------------------------------------
  TClonesArray &cl=*clusters;
  //
  Float_t tanp=fTanP[lay], tann=fTanN[lay];
  Int_t idet=fNdet[fModule];
  Int_t ncl=0;
  //
  Float_t lp[5];
  Int_t milab[10];
  Double_t ratio;
  
  //
  // sign gold tracks
  //
  for (Int_t j=0;j<nn;j++){
    Float_t ybest=1000,zbest=1000,qbest=0;
      Float_t yn=neg[j].GetY()*fYpitchSSD[lay];
      Float_t zt=fHlSSD[lay];
      Float_t yt=yn;
      zt-=fHlSSD[lay]; yt-=fHwSSD[lay];
      ybest=yt; zbest=zt; 
      qbest=neg[j].GetQ();
      lp[0]=-(-ybest+fYshift[fModule]);
      lp[1]=zbest+fZshift[fModule];
      if (lay<=4) lp[1]=0.0;
      lp[2]=8.9e-04*8.9e-04;  //SigmaY2
      lp[3]=2.645159*2.645159;  //SigmaZ2      
      lp[4]=qbest;        //Q
      for (Int_t ilab=0;ilab<10;ilab++) milab[ilab]=-2;
      for (Int_t ilab=0;ilab<3;ilab++){
	milab[ilab] = neg[j].GetLabel(ilab);
      }
      //
      CheckLabels2(milab);
      milab[3]=(((j<<12) + j)<<12) + idet; // pos|neg|det
      Int_t info[3] = {0,neg[j].GetNd(),fNlayer[fModule]};
      IlcTARGETRecPoint * cl2;
	cl2 = new IlcTARGETRecPoint(fModule,fDetTypeRec->GetTARGETgeom(),milab,lp,info);	
	//cl2->SetChargeRatio(ratio);    	
        lp[0]=lp[0]+gRandom->Gaus(0,0.00075);
        cl2->SetXZ(fModule,lp[0],lp[1]);
        cl2->SetDetector(fNdet[fModule]%fDetTypeRec->GetTARGETgeom()->GetNdetectors(lay+fParam->GetNlayers()+fParam->GetNendcaps()+1));
	cl2->SetType(1);
        cl2->SetModule(fModule);
	if ((neg[j].GetNd())>6){ //multi cluster
          IlcDebug(1,Form("multi cluster!!!"));
	  cl2->SetType(2);
	}
        if (IlcLog::GetGlobalDebugLevel()>0) cout <<"1) num cluster:"<<ncl<<" Type: "<<cl2->GetType()<<" SSD Layer: "<<info[2]<<" Module: "<<fModule<<" pos[x]: "<<lp[0]<<" pos[z]: "<<lp[1]<<endl;
        //cout <<"valore di XZ:"<<cl2->GetDetLocalX()<<endl;
	fDetTypeRec->AddRecPoint(*cl2);

      ncl++;
    }
  
    

}


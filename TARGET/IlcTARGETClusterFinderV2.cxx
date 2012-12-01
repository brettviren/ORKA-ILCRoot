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


#include "IlcRun.h"
#include "IlcTARGETClusterFinderV2.h"
#include "IlcTARGETRecPoint.h"
#include "IlcTARGETDetTypeRec.h"
//#include "IlcTARGETgeom.h"
#include <TParticle.h>
#include "IlcMC.h"

ClassImp(IlcTARGETClusterFinderV2)

#ifdef WIN32
//extern "C" IlcRun *gIlc;
#else
extern IlcRun *gIlc;
#endif

IlcTARGETClusterFinderV2::IlcTARGETClusterFinderV2(IlcTARGETDetTypeRec* dettyp):IlcTARGETClusterFinder(dettyp){

  //Default constructor
  fEvent = 0;
  fModule = 0;
  fNModules = dettyp->GetTARGETgeom()->GetIndexMax();
}

//______________________________________________________________________
IlcTARGETClusterFinderV2::IlcTARGETClusterFinderV2(const IlcTARGETClusterFinderV2 &source) : IlcTARGETClusterFinder(source) {
  // Copy constructor
  // Copies are not allowed. The method is protected to avoid misuse.
  Fatal("IlcTARGETClusterFinderV2","Copy constructor not allowed\n");
}

//______________________________________________________________________
IlcTARGETClusterFinderV2& IlcTARGETClusterFinderV2::operator=(const IlcTARGETClusterFinderV2& /* source */){
  // Assignment operator
  // Assignment is not allowed. The method is protected to avoid misuse.
  Fatal("= operator","Assignment operator not allowed\n");
  return *this;
}


//______________________________________________________________________
void IlcTARGETClusterFinderV2::CheckLabels2(Int_t lab[10]) {
  //------------------------------------------------------------
  // Tries to find mother's labels
  //------------------------------------------------------------
  Int_t nlabels =0; 
  for (Int_t i=0;i<10;i++) if (lab[i]>=0) nlabels++;
  if(nlabels == 0) return; // In case of no labels just exit


  Int_t ntracks = gIlc->GetMCApp()->GetNtrack();

  for (Int_t i=0;i<10;i++){
    Int_t label = lab[i];
    if (label>=0 && label<ntracks) {
      TParticle *part=(TParticle*)gIlc->GetMCApp()->Particle(label);

      if (part->P() < 0.02) {
	  Int_t m=part->GetFirstMother();
	  if (m<0) {	
	    continue;
	  }
	  if (part->GetStatusCode()>0) {
	    continue;
	  }
	  lab[i]=m;       
      }
      else
	if (part->P() < 0.12 && nlabels>3) {
	  lab[i]=-2;
	  nlabels--;
	} 
    }
    else{
      if ( (label>ntracks||label <0) && nlabels>3) {
	lab[i]=-2;
	nlabels--;
      } 
    }
  }  
  if (nlabels>3){
    for (Int_t i=0;i<10;i++){
      if (nlabels>3){
	Int_t label = lab[i];
	if (label>=0 && label<ntracks) {
	  TParticle *part=(TParticle*)gIlc->GetMCApp()->Particle(label);
	  if (part->P() < 0.1) {
	    lab[i]=-2;
	    nlabels--;
	  }
	}
      }
    }
  }

  //compress labels -- if multi-times the same
  Int_t lab2[10];
  for (Int_t i=0;i<10;i++) lab2[i]=-2;
  for (Int_t i=0;i<10  ;i++){
    if (lab[i]<0) continue;
    for (Int_t j=0;j<10 &&lab2[j]!=lab[i];j++){
      if (lab2[j]<0) {
	lab2[j]= lab[i];
	break;
      }
    }
  }
  for (Int_t j=0;j<10;j++) lab[j]=lab2[j];
  
}

//______________________________________________________________________
void IlcTARGETClusterFinderV2::AddLabel(Int_t lab[10], Int_t label) {

  //add label to the cluster

  if(label<0) return; // In case of no label just exit

  Int_t ntracks = gIlc->GetMCApp()->GetNtrack();
  if (label>ntracks) return;
  for (Int_t i=0;i<10;i++){
    //    if (label<0) break;
    if (lab[i]==label) break;
    if (lab[i]<0) {
      lab[i]= label;
      break;
    }
  }
}


//______________________________________________________________________
void IlcTARGETClusterFinderV2:: 
FindCluster(Int_t k,Int_t maxz,IlcBin *bins,Int_t &n,Int_t *idx) {
  //------------------------------------------------------------
  // returns an array of indices of digits belonging to the cluster
  // (needed when the segmentation is not regular) 
  //------------------------------------------------------------
  if (n<2000) idx[n++]=bins[k].GetIndex();
  bins[k].Use();

  if (bins[k-maxz].IsNotUsed()) FindCluster(k-maxz,maxz,bins,n,idx);
  if (bins[k-1   ].IsNotUsed()) FindCluster(k-1   ,maxz,bins,n,idx);
  if (bins[k+maxz].IsNotUsed()) FindCluster(k+maxz,maxz,bins,n,idx);
  if (bins[k+1   ].IsNotUsed()) FindCluster(k+1   ,maxz,bins,n,idx);
  /*
  if (bins[k-maxz-1].IsNotUsed()) FindCluster(k-maxz-1,maxz,bins,n,idx);
  if (bins[k-maxz+1].IsNotUsed()) FindCluster(k-maxz+1,maxz,bins,n,idx);
  if (bins[k+maxz-1].IsNotUsed()) FindCluster(k+maxz-1,maxz,bins,n,idx);
  if (bins[k+maxz+1].IsNotUsed()) FindCluster(k+maxz+1,maxz,bins,n,idx);
  */
}

//______________________________________________________________________
Bool_t IlcTARGETClusterFinderV2::IsMaximum(Int_t k,Int_t max,const IlcBin *bins) {
  //------------------------------------------------------------
  //is this a local maximum ?
  //------------------------------------------------------------
  UShort_t q=bins[k].GetQ();
  if (q==1023) return kFALSE;
  if (bins[k-max].GetQ() > q) return kFALSE;
  if (bins[k-1  ].GetQ() > q) return kFALSE; 
  if (bins[k+max].GetQ() > q) return kFALSE; 
  if (bins[k+1  ].GetQ() > q) return kFALSE; 
  if (bins[k-max-1].GetQ() > q) return kFALSE;
  if (bins[k+max-1].GetQ() > q) return kFALSE; 
  if (bins[k+max+1].GetQ() > q) return kFALSE; 
  if (bins[k-max+1].GetQ() > q) return kFALSE;
  return kTRUE; 
}

//______________________________________________________________________
void IlcTARGETClusterFinderV2::
FindPeaks(Int_t k,Int_t max,IlcBin *b,Int_t *idx,UInt_t *msk,Int_t& n) {
  //------------------------------------------------------------
  //find local maxima
  //------------------------------------------------------------
  if (n<31)
  if (IsMaximum(k,max,b)) {
    idx[n]=k; msk[n]=(2<<n);
    n++;
  }
  b[k].SetMask(0);
  if (b[k-max].GetMask()&1) FindPeaks(k-max,max,b,idx,msk,n);
  if (b[k-1  ].GetMask()&1) FindPeaks(k-1  ,max,b,idx,msk,n);
  if (b[k+max].GetMask()&1) FindPeaks(k+max,max,b,idx,msk,n);
  if (b[k+1  ].GetMask()&1) FindPeaks(k+1  ,max,b,idx,msk,n);
}

//______________________________________________________________________
void IlcTARGETClusterFinderV2::
MarkPeak(Int_t k, Int_t max, IlcBin *bins, UInt_t m) {
  //------------------------------------------------------------
  //mark this peak
  //------------------------------------------------------------
  UShort_t q=bins[k].GetQ();

  bins[k].SetMask(bins[k].GetMask()|m); 

  if (bins[k-max].GetQ() <= q)
     if ((bins[k-max].GetMask()&m) == 0) MarkPeak(k-max,max,bins,m);
  if (bins[k-1  ].GetQ() <= q)
     if ((bins[k-1  ].GetMask()&m) == 0) MarkPeak(k-1  ,max,bins,m);
  if (bins[k+max].GetQ() <= q)
     if ((bins[k+max].GetMask()&m) == 0) MarkPeak(k+max,max,bins,m);
  if (bins[k+1  ].GetQ() <= q)
     if ((bins[k+1  ].GetMask()&m) == 0) MarkPeak(k+1  ,max,bins,m);
}

//______________________________________________________________________
void IlcTARGETClusterFinderV2::
MakeCluster(Int_t k,Int_t max,IlcBin *bins,UInt_t m,IlcTARGETRecPoint &c) {
  //------------------------------------------------------------
  //make cluster using digits of this peak
  //------------------------------------------------------------
  Float_t q=(Float_t)bins[k].GetQ();
  Int_t i=k/max, j=k-i*max;

  c.SetQ(c.GetQ()+q);
  c.SetYZ(GetModule(),c.GetY()+i*q,c.GetZ()+j*q);
  c.SetSigmaY2(c.GetSigmaY2()+i*i*q);
  c.SetSigmaZ2(c.GetSigmaZ2()+j*j*q);

  bins[k].SetMask(0xFFFFFFFE);
  
  if (bins[k-max].GetMask() == m) MakeCluster(k-max,max,bins,m,c);
  if (bins[k-1  ].GetMask() == m) MakeCluster(k-1  ,max,bins,m,c);
  if (bins[k+max].GetMask() == m) MakeCluster(k+max,max,bins,m,c);
  if (bins[k+1  ].GetMask() == m) MakeCluster(k+1  ,max,bins,m,c);
}

IlcTARGETgeom *IlcTARGETClusterFinderV2::LoadGeometry() {

  // Connect the Root Gilc file containing Geometry, Kine and Hits
    IlcRunLoader* rl = IlcRunLoader::Open("gilc.root");
    if (rl == 0x0)
     {
       ::Error("IlcTARGETClusterFinderV2SPD","Can not open session RL=NULL");
       return NULL;
     }
     
    Int_t retval = rl->LoadgIlc();
    if (retval)
     {
       ::Error("IlcTARGETClusterFinderV2SPD","LoadgIlc returned error");
       delete rl;
       return NULL;
     }
    gIlc=rl->GetIlcRun();
    TDirectory * olddir = gDirectory;
    rl->CdGAFile();
    IlcTARGETgeom* geom = (IlcTARGETgeom*)gDirectory->Get("IlcTARGETgeom");
    olddir->cd();
    if(!geom){
      Error("GetTARGETgeom","no TARGET geometry available");
      return NULL;
    }
 return geom;
}

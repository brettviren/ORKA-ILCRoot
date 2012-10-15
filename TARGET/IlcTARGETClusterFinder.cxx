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
//                                                                        //
// Base Class used to find                                                //
// the reconstructed points for TARGET                                       //
// See also IlcTARGETClusterFinderSPD, IlcTARGETClusterFinderSDD,               //
// IlcTARGETClusterFinderSDD  IlcTARGETClusterFinderV2                          //
////////////////////////////////////////////////////////////////////////////

#include "IlcTARGETClusterFinder.h"
#include "IlcTARGETRecPoint.h"
#include "IlcTARGETdigit.h"
#include "IlcTARGETDetTypeRec.h"
#include "IlcTARGETMap.h"

ClassImp(IlcTARGETClusterFinder)

//----------------------------------------------------------------------
IlcTARGETClusterFinder::IlcTARGETClusterFinder():
TObject(),
fDebug(0),
fModule(0),
fDigits(0),
fNdigits(0),
fDetTypeRec(0),
fClusters(0),
fNRawClusters(0),
fNperMax(0),
fDeclusterFlag(0),
fClusterSize(0),
fNPeaks(-1){
    // default cluster finder
    // Input:
    //   none.
    // Output:
    //   none.
    // Return:
    //   A default constructed IlcTARGETCulsterFinder
    fMap=new TObjArray(100);
}
//----------------------------------------------------------------------
IlcTARGETClusterFinder::IlcTARGETClusterFinder(IlcTARGETDetTypeRec* dettyp):
TObject(),
fDebug(0),
fModule(0),
fDigits(0),
fNdigits(0),
fClusters(0),
fNRawClusters(0),
fNperMax(0),
fDeclusterFlag(0),
fClusterSize(0),
fNPeaks(-1){
    // Standard constructor for cluster finder
    // Input:
    //   IlcTARGETsegmentation *seg  The segmentation class to be used
    //   IlcTARGETresponse     *res  The response class to be used
    // Output:
    //   none.
    // Return:
    //   A Standard constructed IlcTARGETCulsterFinder
fMap=new TObjArray(100);
    SetNperMax();
    SetClusterSize();
    SetDeclusterFlag();
    fDetTypeRec = dettyp;

}
//----------------------------------------------------------------------
IlcTARGETClusterFinder::IlcTARGETClusterFinder(IlcTARGETDetTypeRec* dettyp,
					 TClonesArray *digits):
TObject(),
fDebug(0),
fModule(0),
fDigits(digits),
fNdigits(0),
fDetTypeRec(dettyp),
fClusters(0),
fNRawClusters(0),
fNperMax(0),
fDeclusterFlag(0),
fClusterSize(0),
fNPeaks(-1){
    // Standard + cluster finder constructor
    // Input:
    //   IlcTARGETsegmentation *seg  The segmentation class to be used
    //   IlcTARGETresponse     *res  The response class to be used
    //   TClonesArray    *digits  Array of digits to be used
    // Output:
    //   none.
    // Return:
    //   A Standard constructed IlcTARGETCulsterFinder
fMap=new TObjArray(100);
    fNdigits = fDigits->GetEntriesFast();
    SetNperMax();
    SetClusterSize();
    SetDeclusterFlag();
}
//______________________________________________________________________
IlcTARGETClusterFinder::IlcTARGETClusterFinder(const IlcTARGETClusterFinder &source) : TObject(source) {
  // Copy constructor
  // Copies are not allowed. The method is protected to avoid misuse.
  Fatal("IlcTARGETClusterFinder","Copy constructor not allowed\n");
}

//______________________________________________________________________
IlcTARGETClusterFinder& IlcTARGETClusterFinder::operator=(const IlcTARGETClusterFinder& /* source */){
  // Assignment operator
  // Assignment is not allowed. The method is protected to avoid misuse.
  Fatal("= operator","Assignment operator not allowed\n");
  return *this;
}

//----------------------------------------------------------------------
IlcTARGETClusterFinder::~IlcTARGETClusterFinder(){
    // destructor cluster finder
    // Input:
    //   none.
    // Output:
    //   none.
    // Return:
    //   none.

    if(fMap) {fMap->Delete();}
    // Zero local pointers. Other classes own these pointers.
    fMap          = 0;
    fDigits       = 0;
    fNdigits      = 0;
    fNRawClusters = 0;
    fNperMax      = 0;
    fDeclusterFlag= 0;
    fClusterSize  = 0;
    fNPeaks       = 0;
    fDetTypeRec   = 0;

}
//__________________________________________________________________________
void IlcTARGETClusterFinder::InitGeometry(){


 //Initialisation of TARGET geometry
  if(!fDetTypeRec->GetTARGETgeom()) {
    Error("InitGeometry","TARGET geom is null!");
    return;
  }
  Int_t mmax=fDetTypeRec->GetTARGETgeom()->GetIndexMax();
  if (mmax>8752) {//Dan Endcap
    Fatal("IlcTARGETClusterFinder","Too many TARGET subdetectors !"); 
  }
  Int_t m;
  for (m=0; m<mmax; m++) {
//    if (m>95) continue; //Dan Endcap
    Int_t lay,lad,det; fDetTypeRec->GetTARGETgeom()->GetModuleId(m,lay,lad,det);
    Float_t x,y,z;     fDetTypeRec->GetTARGETgeom()->GetTrans(lay,lad,det,x,y,z); 
    Double_t rot[9];   fDetTypeRec->GetTARGETgeom()->GetRotMatrix(lay,lad,det,rot);
    Double_t alpha=TMath::ATan2(rot[1],rot[0])+TMath::Pi();

    Double_t ca=TMath::Cos(alpha), sa=TMath::Sin(alpha);
    //fYshift[m] = x*ca + y*sa;
    //fZshift[m] = (Double_t)z;
    fYshift[m]=0.0;
    fZshift[m] = 0.0;
    fNdet[m] = (lad-1)*fDetTypeRec->GetTARGETgeom()->GetNdetectors(lay) + (det-1);
    fNlayer[m] = lay-1;

    if(IlcDebugLevelClass()>0)
      IlcDebug(1,Form("ClusterDan... fYshift[%d]: %f, fZshift[%d]: %f, ca: %f, sa:%f, fNdet[%d]: %d, fNlayer[%d]: %d",m,fYshift[m],m,fZshift[m],ca,sa,m,fNdet[m],m,fNlayer[m]));

  }
}



//----------------------------------------------------------------------
void IlcTARGETClusterFinder::AddCluster(Int_t branch, IlcTARGETRawCluster *c){
    // Add a raw cluster copy to the list
    // Input:
    //   Int_t       branch  The branch to which the cluster is to be added to
    //   IlcTARGETRawCluster *c The cluster to be added to the array of clusters
    // Output:
    //   none.
    // Return:
    //   none.

   if(!fDetTypeRec) {
    Error("AddCluster","fDetTypeRec is null!");
    return;
  }
  fDetTypeRec->AddCluster(branch,c); 
  fNRawClusters++;
}
//----------------------------------------------------------------------
void IlcTARGETClusterFinder::AddCluster(Int_t branch, IlcTARGETRawCluster *c, 
				     IlcTARGETRecPoint &rp){
    // Add a raw cluster copy to the list and the RecPoint
    // Input:
    //   Int_t       branch  The branch to which the cluster is to be added to
    //   IlcTARGETRawCluster *c The cluster to be added to the array of clusters
    //   IlcTARGETRecPoint  &rp The RecPoint to be added to the array of RecPoints
    // Output:
    //   none.
    // Return:
    //   none.
  if(!fDetTypeRec) {
    Error("AddCluster","fDetTypeRec is null!");
    return;
  }

  fDetTypeRec->AddCluster(branch,c); 
  fNRawClusters++;
  fDetTypeRec->AddRecPoint(rp); 

}
/*
//______________________________________________________________________
void IlcTARGETClusterFinder::CheckLabels(Int_t lab[3]) {
  //------------------------------------------------------------
  // Tries to find mother's labels
  //------------------------------------------------------------

  if(lab[0]<0 && lab[1]<0 && lab[2]<0) return; // In case of no labels just exit
  // Check if simulation
  IlcMC* mc = gIlc->GetMCApp();
  if(!mc)return;

  Int_t ntracks = mc->GetNtrack();
  for (Int_t i=0;i<3;i++){
    Int_t label = lab[i];
    if (label>=0 && label<ntracks) {
      TParticle *part=(TParticle*)mc->Particle(label);
      if (part->P() < 0.005) {
	Int_t m=part->GetFirstMother();
	if (m<0) {	
	  continue;
	}
	if (part->GetStatusCode()>0) {
	  continue;
	}
	lab[i]=m;       
      }
    }    
  }
  
}
*/
//______________________________________________________________________
void IlcTARGETClusterFinder::FindRawClusters(Int_t module){
    // Default Cluster finder.
    // Input:
    //   Int_t module   Module number for which culster are to be found.
    // Output:
    //   none.
    // Return:
    //   none.
    const Int_t kelms = 10;
    Int_t ndigits = fDigits->GetEntriesFast();
    TObjArray *digs = new TObjArray(ndigits);
    TObjArray *clusts = new TObjArray(ndigits); // max # cluster
    TObjArray *clust0=0; // A spacific cluster of digits
    TObjArray *clust1=0; // A spacific cluster of digits
    IlcTARGETdigit *dig=0; // locat pointer to a digit
    Int_t i=0,nc=0,j[4],k,k2=0;

    // Copy all digits for this module into a local TObjArray.
    for(i=0;i<ndigits;i++) digs->AddAt(new IlcTARGETdigit(*(GetDigit(i))),i);
    digs->Sort();
    // First digit is a cluster.
    i  = 0;
    nc = 0;
    clusts->AddAt(new TObjArray(kelms),nc);
    clust0 = (TObjArray*)(clusts->UncheckedAt(nc));
    clust0->AddAtFree(digs->UncheckedAt(i)); // move owner ship from digs to clusts
    nc++;
    for(i=1;i<ndigits;i++){
        if(IsNeighbor(digs,i,j)){
          dig = (IlcTARGETdigit*)(digs->UncheckedAt(j[0]));
            // Add to existing cluster. Find which cluster this digis 
            for(k=0;k<nc;k++){
                clust0 = ((TObjArray*)(clusts->UncheckedAt(k)));
                if(clust0->IndexOf(dig)>=0) break;
            } // end for k
            if(k>=nc){
                Fatal("FindRawClusters","Digit not found as expected");
            } // end if
            if(j[1]>=0){
              dig = (IlcTARGETdigit*)(digs->UncheckedAt(j[1]));
                // Add to existing cluster. Find which cluster this digis 
                for(k2=0;k2<nc;k2++){
                  clust1 = ((TObjArray*)(clusts->UncheckedAt(k2)));
                    if(clust1->IndexOf(dig)>=0) break;
                } // end for k2
                if(k2>=nc){
                    Fatal("FindRawClusters","Digit not found as expected");
                } // end if
            } // end if j[1]>=0
            // Found cluster with neighboring digits add this one to it.
            if(clust0==clust1){ // same cluster
              clust0->AddAtFree(digs->UncheckedAt(i));
                clust0 = 0; // finished with cluster. zero for safty
                clust1 = 0; // finished wit hcluster. zero for safty
            }else{ // two different clusters which need to be merged.
              clust0->AddAtFree(digs->UncheckedAt(i)); // Add digit to this cluster.
                for(k=0;k<clust1->GetEntriesFast();k++){
                    // move clust1 into clust0
                    //move digit to this cluster
                  clust0->AddAtFree(clust1->UncheckedAt(k));
                    clust1->AddAt(0,k); // zero this one
                } // end for k
                delete clust1;
                clusts->AddAt(0,k2); // zero array of clusters element clust1
                clust0 = 0; // finished with cluster. zero for safty
                clust1 = 0; // finished wit hcluster. zero for safty
            } // end if clust0==clust1
        }else{// New cluster
            clusts->AddAt(new TObjArray(kelms),nc);
            clust0 = ((TObjArray*)(clusts->UncheckedAt(nc)));
            // move owner ship from digs to clusts
            clust0->AddAtFree(digs->UncheckedAt(i));
            clust0 = 0; // finished with cluster. zero for safty
            nc++;
        } // End if IsNeighbor
    } // end for i
    // There are now nc clusters in clusts. Each element of clust is an
    // array of digits which are clustered together.

    // For each cluster call detector specific CreateRecPoints
    for(i=0;i<nc;i++) CreateRecPoints((TObjArray*)(clusts->UncheckedAt(i)),module);

    // clean up at the end.
    for(i=0;i<nc;i++){ 
      clust0 =(TObjArray*)(clusts->UncheckedAt(i));
        // Digits deleted below, so zero this TObjArray
        for(k=0;k<clust0->GetEntriesFast();k++) clust0->AddAt(0,k);
        delete clust0; // Delete this TObjArray
        clusts->AddAt(0,i); // Contents deleted above, so zero it.
    } // end for i
    delete clusts; // Delete this TObjArray/
    // Delete the digits then the TObjArray which containted them.
    for(i=0;i<ndigits;i++) delete ((IlcTARGETdigit*)(digs->UncheckedAt(i)));
    delete digs;
}
//______________________________________________________________________
Bool_t IlcTARGETClusterFinder::IsNeighbor(TObjArray *digs,Int_t i,Int_t n[])const{
    // Locagical function which checks to see if digit i has a neighbor.
    // If so, then it returns kTRUE and its neighbor index j.
    // This routine checks if the digits are side by side or one before the
    // other. Requires that the array of digits be in proper order.
    // Returns kTRUE in the following cases.
    //                 ji   0j   if kdiagonal  j0    0i
    //                 00   0i   if kdiagonal  0i    j0
    // Inputs:
    //    TObjArray *digs   Array to search for neighbors in
    //    Int_t      i      Index of digit for which we are searching for
    //                      a neighbor of.
    // Output:
    //    Int_t      j[4]   Index of one or more of the digits which is a
    //                      neighbor of digit a index i.
    // Return:
    //    Bool_t            kTRUE if a neighbor was found kFALSE otherwise.
    Int_t ix,jx,iz,jz,j;
    const Bool_t kdiagonal=kFALSE;
    Bool_t nei[4];

    // No neighbors found if array empty.
    if(digs->GetEntriesFast()<=0) return kFALSE;
    // can not be a digit with first element or elements out or range
    if(i<=0 || i>=digs->GetEntriesFast()) return kFALSE;

    for(j=0;j<4;j++){n[j] = -1;nei[j]=kFALSE;}
    ix = ((IlcTARGETdigit*)(digs->UncheckedAt(i)))->GetCoord1();
    iz = ((IlcTARGETdigit*)(digs->UncheckedAt(i)))->GetCoord2();
    for(j=0;j<i;j++){
      jx = ((IlcTARGETdigit*)(digs->UncheckedAt(j)))->GetCoord1();
      jz = ((IlcTARGETdigit*)(digs->UncheckedAt(j)))->GetCoord2();
        if(jx+1==ix && jz  ==iz){n[0] = j;nei[0] = kTRUE;}
        if(jx  ==ix && jz+1==iz){n[1] = j;nei[1] = kTRUE;}
        if(jx+1==ix && jz+1==iz){n[2] = j;nei[2] = kTRUE;}
        if(jx+1==ix && jz-1==iz){n[3] = j;nei[3] = kTRUE;}
    } // end for k
    if(nei[0]||nei[1]) return kTRUE;
    if(kdiagonal&&(nei[2]||nei[3])) return kTRUE;
    // no Neighbors found.
    return kFALSE;
}

//______________________________________________________________________
void IlcTARGETClusterFinder::Print(ostream *os) const{
    //Standard output format for this class
    // Inputs:
    //    ostream *os   Output stream
    // Output:
    //    ostream *os   Output stream
    // Return:
    //    none.

    *os << fDebug<<",";
    *os << fModule<<",";
    *os << fNdigits<<",";
    *os << fNRawClusters<<",";
    *os << fNperMax<<",";
    *os << fDeclusterFlag<<",";
    *os << fClusterSize<<",";
    *os << fNPeaks<<endl;
}
/*
//______________________________________________________________________
void IlcTARGETClusterFinder::RecPoints2Clusters
(const TClonesArray *points, Int_t idx, TClonesArray *clusters) {
  //------------------------------------------------------------
  // Conversion IlcTARGETRecPoint -> IlcTARGETclusterV2 for the TARGET 
  // subdetector indexed by idx 
  //------------------------------------------------------------
  if(!fDetTypeRec->GetTARGETgeom()) {
    Error("RecPoints2Clusters","TARGET geom is null!");
    return;
  }
  Int_t lastSPD1=fDetTypeRec->GetTARGETgeom()->GetModuleIndex(2,1,1)-1;
  TClonesArray &cl=*clusters;
  Int_t ncl=points->GetEntriesFast();
  for (Int_t i=0; i<ncl; i++) {
    IlcTARGETRecPoint *p = (IlcTARGETRecPoint *)points->UncheckedAt(i);
    Float_t lp[5];
    lp[0]=-(-p->GetX()+fYshift[idx]); if (idx<=lastSPD1) lp[0]*=-1; //SPD1
    lp[1]=  -p->GetZ()+fZshift[idx];
    lp[2]=p->GetSigmaX2();
    lp[3]=p->GetSigmaZ2();
    lp[4]=p->GetQ()*36./23333.;  //electrons -> ADC
    Int_t lab[4]; 
    lab[0]=p->GetLabel(0); lab[1]=p->GetLabel(1); lab[2]=p->GetLabel(2);
    lab[3]=fNdet[idx];
    CheckLabels(lab);
    Int_t dummy[3]={0,0,0};
    new (cl[i]) IlcTARGETclusterV2(lab,lp, dummy);
  }  
} 
*/
//______________________________________________________________________
void IlcTARGETClusterFinder::Read(istream *is)  {
    //Standard input for this class
    // Inputs:
    //    istream *is   Input stream
    // Output:
    //    istream *is   Input stream
    // Return:
    //    none.

    *is >> fDebug;
    *is >> fModule;
    *is >> fNdigits;
    *is >> fNRawClusters;
    *is >> fNperMax;
    *is >> fDeclusterFlag;
    *is >> fClusterSize;
    *is >> fNPeaks;
}
//______________________________________________________________________
ostream &operator<<(ostream &os,IlcTARGETClusterFinder &source){
    // Standard output streaming function.
    // Inputs:
    //    ostream             *os     Output stream
    //    IlcTARGETClusterFinder &source Class to be printed
    // Output:
    //    ostream             *os     Output stream
    // Return:
    //    none.

    source.Print(&os);
    return os;
}
//______________________________________________________________________
istream &operator>>(istream &is,IlcTARGETClusterFinder &source){
    // Standard output streaming function.
    // Inputs:
    //    istream              *is      Input stream
    //     IlcTARGETClusterFinder &source  Class to be read in.
    // Output:
    //    istream              *is      Input stream
    // Return:
    //    none.

    source.Read(&is);
    return is;
}
/*
void IlcTARGETClusterFinder::RecPoints2Clusters
(const TClonesArray *points, Int_t idx, TClonesArray *clusters) {
  //------------------------------------------------------------
  // Conversion IlcTARGETRecPoint -> IlcTARGETclusterV2 for the TARGET 
  // subdetector indexed by idx 
  //------------------------------------------------------------
  TClonesArray &cl=*clusters;
  Int_t ncl=points->GetEntriesFast();
  for (Int_t i=0; i<ncl; i++) {
    IlcTARGETRecPoint *p = (IlcTARGETRecPoint *)points->UncheckedAt(i);
    Float_t lp[5];
    lp[0]=-(-p->GetX()+fYshift[idx]); if (idx<=fLastSPD1) lp[0]*=-1; //SPD1
    lp[1]=  -p->GetZ()+fZshift[idx];
    lp[2]=p->GetSigmaX2();
    lp[3]=p->GetSigmaZ2();
    lp[4]=p->GetQ()*36./23333.;  //electrons -> ADC
    Int_t lab[4]; 
    lab[0]=p->GetLabel(0); lab[1]=p->GetLabel(1); lab[2]=p->GetLabel(2);
    lab[3]=fNdet[idx];
    CheckLabels(lab);
    Int_t dummy[3]={0,0,0};
    new (cl[i]) IlcTARGETclusterV2(lab,lp, dummy);
  }  
} 
*/

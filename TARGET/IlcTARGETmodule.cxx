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

/* $Id: IlcTARGETmodule.cxx,v 1.2 2008/05/07 13:35:22 cgatto Exp $ */

#include <TArrayI.h>

#include <stdlib.h>

#include "IlcLog.h"
#include "IlcRun.h"
#include "IlcTARGET.h"
#include "IlcTARGEThit.h"
#include "IlcTARGETmodule.h"
#include "IlcTARGETgeom.h"

ClassImp(IlcTARGETmodule)

//_______________________________________________________________________
//
// Impementation of class IlcTARGETmodule
//
// created by: A. Bouchm, W. Peryt, S. Radomski, P. Skowronski 
//             R. Barbers, B. Batyunia, B. S. Nilsen
// ver 1.0     CERN 16.09.1999
//_______________________________________________________________________
//________________________________________________________________________
// 
// Constructors and deconstructor
//________________________________________________________________________
//
IlcTARGETmodule::IlcTARGETmodule() {
    // constructor

    fHitsM       = 0;
    fTrackIndex  = 0;
    fHitIndex    = 0;
    fTARGET         = 0;

}
//_________________________________________________________________________
IlcTARGETmodule::IlcTARGETmodule(Int_t index) {
  // constructor

    fIndex      = index;
    fHitsM      = new TObjArray();
    fTrackIndex = new TArrayI(16);
    fHitIndex   = new TArrayI(16);
    fTARGET        = (IlcTARGET*)(gIlc->GetDetector("TARGET"));
}
//__________________________________________________________________________
IlcTARGETmodule::~IlcTARGETmodule() {
    // The destructor for IlcTARGETmodule. Before destoring IlcTARGETmodule
    // we must first destroy all of it's members.

    if(fHitsM){
      fHitsM->Delete();
      delete fHitsM;
      fHitsM = 0;
    } // end if
    delete fTrackIndex;
    delete fHitIndex;
    fTARGET = 0; // We don't delete this pointer since it is just a copy.
}
//____________________________________________________________________________
IlcTARGETmodule::IlcTARGETmodule(const IlcTARGETmodule &source):TObject(source){
////////////////////////////////////////////////////////////////////////
//     Copy Constructor 
////////////////////////////////////////////////////////////////////////
  Error("IlcTARGETmodule","IlcTARGETmodule class has not to be copied! Exit.");
  exit(1);
}
//_____________________________________________________________________________
IlcTARGETmodule& IlcTARGETmodule::operator=(const IlcTARGETmodule &source){
////////////////////////////////////////////////////////////////////////
//    Assignment operator 
////////////////////////////////////////////////////////////////////////
    if(&source == this) return *this;
    Error("IlcTARGETmodule","IlcTARGETmodule class has not to be copied! Exit.");
    exit(1);
    return *this; // fake return neded on Sun
}
//_________________________________________________________________________
// 
// Hits management
//__________________________________________________________________________
Int_t IlcTARGETmodule::AddHit(IlcTARGEThit* hit,Int_t t,Int_t h) {
// Hits management

  //printf("AddHit: beginning hit %p t h %d %d\n",hit,t,h);
  

  
    fHitsM->AddLast(new IlcTARGEThit(*hit));
    Int_t fNhitsM = fHitsM->GetEntriesFast();
    if(fNhitsM-1>=fTrackIndex->GetSize()){ // need to expand the TArrayI
      fTrackIndex->Set(fNhitsM+64);
    } // end if
    if(fNhitsM-1>=fHitIndex->GetSize()){ // need to expand the TArrayI
      fHitIndex->Set(fNhitsM+64);
    } // end if
    (*fTrackIndex)[fNhitsM-1] = t;
    (*fHitIndex)[fNhitsM-1]   = h;
    return fNhitsM;
}
//___________________________________________________________________________
Double_t IlcTARGETmodule::PathLength(Int_t index,IlcTARGEThit *itsHit1,
				  IlcTARGEThit *itsHit2) {
				  

   				  				  
  // path lenght
   Float_t  x1g,y1g,z1g;   
   Float_t  x2g,y2g,z2g;
   Double_t s;

   index = 0;
   itsHit1->GetPositionG(x1g,y1g,z1g);
   itsHit2->GetPositionG(x2g,y2g,z2g);

   s = TMath::Sqrt( ((Double_t)(x2g-x1g)*(Double_t)(x2g-x1g)) +
		    ((Double_t)(y2g-y1g)*(Double_t)(y2g-y1g)) +
		    ((Double_t)(z2g-z1g)*(Double_t)(z2g-z1g))  );
   return s;
}
//___________________________________________________________________________
void IlcTARGETmodule::PathLength(Int_t index,
			      Float_t x,Float_t y,Float_t z,
			      Int_t status,Int_t &nseg,
			      Float_t &x1,Float_t &y1,Float_t &z1,
			      Float_t &dx1,Float_t &dy1,Float_t &dz1,
			      Int_t &flag) const{
			      

			      
  // path length
    static Float_t x0,y0,z0;

    index = 0;
    if ((status&0x0002)!=0){ // entering
	x0 = x;
	y0 = y;
	z0 = z;
	nseg = 0;
	flag = 1;
    }else{
	x1 = x0;
	y1 = y0;
	z1 = z0;
	dx1 = x-x1;
	dy1 = y-y1;
	dz1 = z-z1;
	nseg++;
	if ((status&0x0004)!=0) flag = 0; //exiting
	if ((status&0x0001)!=0) flag = 2; // inside
	else flag = 2; //inside ?
	x0 = x;
	y0 = y;
	z0 = z;
    } // end if
}
//___________________________________________________________________________
Bool_t IlcTARGETmodule::LineSegmentL(Int_t hitindex,Double_t &a,Double_t &b,
				  Double_t &c,Double_t &d,
				  Double_t &e,Double_t &f,Double_t &de){
				  

				  
  // line segment
    IlcTARGEThit *h1;
    Double_t t;

    if(hitindex>= fHitsM->GetEntriesFast()) return kFALSE;

    h1 = (IlcTARGEThit *) (fHitsM->UncheckedAt(hitindex));
    if(h1->StatusEntering()){ // if track entering volume, get index for next
	                      // step
	return kFALSE;
    } // end if StatusEntering()
    // else stepping
    de = h1->GetIonization();
    h1->GetPositionL0(a,c,e,t);
    h1->GetPositionL(b,d,f);
    b = b - a;
    d = d - c;
    f = f - e;
    return kTRUE;
}
//___________________________________________________________________________
Bool_t IlcTARGETmodule::LineSegmentG(Int_t hitindex,Double_t &a,Double_t &b,
				  Double_t &c,Double_t &d,
				  Double_t &e,Double_t &f,Double_t &de){
				  

				  
  // line segment
    IlcTARGEThit *h1;
    Double_t t;

    if(hitindex>= fHitsM->GetEntriesFast()) return kFALSE;

    h1 = (IlcTARGEThit *) (fHitsM->UncheckedAt(hitindex));
    if(h1->StatusEntering()){ // if track entering volume, get index for next
	                      // step
	return kFALSE;
    } // end if StatusEntering()
    // else stepping
    de = h1->GetIonization();
    h1->GetPositionG0(a,c,e,t);
    h1->GetPositionG(b,d,f);
    b = b - a;
    d = d - c;
    f = f - e;
    return kTRUE;
}
//___________________________________________________________________________
Bool_t IlcTARGETmodule::LineSegmentL(Int_t hitindex,Double_t &a,Double_t &b,
				  Double_t &c,Double_t &d,
				  Double_t &e,Double_t &f,
				  Double_t &de,Int_t &track){
				  

				  
  // line segmente
    IlcTARGEThit *h1;
    Double_t t;

    if(hitindex>= fHitsM->GetEntriesFast()) return kFALSE;

    h1 = (IlcTARGEThit *) (fHitsM->UncheckedAt(hitindex));
    if(h1->StatusEntering()){ // if track entering volume, get index for next
	                      // step
	track = h1->GetTrack();
	return kFALSE;
    } // end if StatusEntering()
    // else stepping
    de = h1->GetIonization();
    IlcDebug(1,Form("Carica: %f",de));
    h1->GetPositionL0(a,c,e,t);
    h1->GetPositionL(b,d,f);
    b = b - a;
    d = d - c;
    f = f - e;
    track = h1->GetTrack();
    return kTRUE;
}
//___________________________________________________________________________
Bool_t IlcTARGETmodule::LineSegmentG(Int_t hitindex,Double_t &a,Double_t &b,
				  Double_t &c,Double_t &d,
				  Double_t &e,Double_t &f,
				  Double_t &de,Int_t &track){
				  

				  
  // line segment
    IlcTARGEThit *h1;
    Double_t t;

    if(hitindex>= fHitsM->GetEntriesFast()) return kFALSE;

    h1 = (IlcTARGEThit *) (fHitsM->UncheckedAt(hitindex));
    if(h1->StatusEntering()){ // if track entering volume, get index for next
	                      // step
	track = h1->GetTrack();
	return kFALSE;
    } // end if StatusEntering()
    // else stepping
    de = h1->GetIonization();
    h1->GetPositionG0(a,c,e,t);
    h1->GetPositionG(b,d,f);
    b = b - a;
    d = d - c;
    f = f - e;
    track = h1->GetTrack();
    return kTRUE;
}
//______________________________________________________________________
Bool_t IlcTARGETmodule::MedianHitG(IlcTARGEThit *h1,IlcTARGEThit *h2,
				Float_t &x,Float_t &y,Float_t &z){
				

				
    // Computes the mean hit location for a set of hits that make up a track
    // passing through a volume. Returns kFALSE untill the the track leaves
    // the volume.
    // median hit
   IlcTARGETgeom *gm = fTARGET->GetTARGETgeom2();
   Float_t x1l=0.,y1l=0.,z1l=0.;
   Float_t x2l=0.,y2l=0.,z2l=0.;
   Float_t xMl,yMl=0,zMl;
   Float_t l[3], g[3];

   h1->GetPositionG(x1l,y1l,z1l);
   h2->GetPositionG(x2l,y2l,z2l);

   //if((y2l*y1l)<0.) {
   //  xMl = (-y1l / (y2l-y1l))*(x2l-x1l) + x1l;
   //  zMl = (-y1l / (y2l-y1l))*(z2l-z1l) + z1l;
   //} else {
     xMl = 0.5*(x1l+x2l);
     zMl = 0.5*(z1l+z2l);
   //}

   l[0] = xMl;
   l[1] = yMl;
   l[2] = zMl;
   gm->LtoG(h1->GetModule(),l,g);
   x = g[0];
   y = g[1];
   z = g[2];
   return kTRUE;
}
//___________________________________________________________________________
void IlcTARGETmodule::MedianHitG(Int_t index,
			      Float_t hitx1,Float_t hity1,Float_t hitz1,
			      Float_t hitx2,Float_t hity2,Float_t hitz2,
			      Float_t &xMg, Float_t &yMg, Float_t &zMg){
			      

			      			      
  // median hit
   IlcTARGETgeom *gm = fTARGET->GetTARGETgeom2();
   Float_t x1l,y1l,z1l;
   Float_t x2l,y2l,z2l;
   Float_t xMl,yMl=0,zMl;
   Float_t l[3], g[3];

   g[0] = hitx1;
   g[1] = hity1;
   g[2] = hitz1;
   gm->GtoL(index,g,l);
   x1l = l[0];
   y1l = l[1];
   z1l = l[2];

   g[0] = hitx2;
   g[1] = hity2;
   g[2] = hitz2;
   gm->GtoL(index,g,l);
   x2l = l[0];
   y2l = l[1];
   z2l = l[2];

   if((y2l*y1l)<0.||y2l!=y1l) {
     xMl = (-y1l / (y2l-y1l))*(x2l-x1l) + x1l;
     zMl = (-y1l / (y2l-y1l))*(z2l-z1l) + z1l;
   } else {
     xMl = 0.5*(x1l+x2l);
     zMl = 0.5*(z1l+z2l);
   }

   l[0] = xMl;
   l[1] = yMl;
   l[2] = zMl;
   IlcDebug(1,Form("xyz1 %f %f %f xyz2 %f %f %f xyzml %f %f %f",x1l,y1l,z1l,x2l,y2l,z2l,xMl,yMl,zMl));
     
   gm->LtoG(index,l,g);
   xMg = g[0];
   yMg = g[1];
   zMg = g[2];
}
//___________________________________________________________________________
Bool_t IlcTARGETmodule::MedianHitL( IlcTARGEThit *itsHit1, 
	       	    	     IlcTARGEThit *itsHit2, 
	       		     Float_t &xMl, Float_t &yMl, Float_t &zMl) const{
			     

			     
  // median hit
   Float_t x1l,y1l,z1l;
   Float_t x2l,y2l,z2l;

   itsHit1->GetPositionL(x1l,y1l,z1l);
   itsHit2->GetPositionL(x2l,y2l,z2l);

   yMl = 0.0;
   if((y2l*y1l)<0.) {
     xMl = (-y1l / (y2l-y1l))*(x2l-x1l) + x1l;
     zMl = (-y1l / (y2l-y1l))*(z2l-z1l) + z1l;	     
   } else {
     xMl = 0.5*(x1l+x2l);
     zMl = 0.5*(z1l+z2l);
   }
   return kTRUE;
}
//___________________________________________________________________________
void IlcTARGETmodule::MedianHit(Int_t index,
			     Float_t xg,Float_t yg,Float_t zg,
			     Int_t status,
			     Float_t &xMg,Float_t &yMg,Float_t &zMg,
			     Int_t &flag){
			     

			     
  // median hit
   static Float_t x1,y1,z1;
   
   IlcDebug(1, Form(" Track status = %d, flag = %d", status, flag));
   

//   if ((status&0x0002)!=0){ // entering
//       x1 = xg;
//       y1 = yg;
//       z1 = zg;
//       flag = 1;
       
//       IlcDebug(1, "x1 = xg, y1 = xg, z1 = zg");
       
//       IlcDebug(1, Form(" Track status = %d, flag = %d", status, flag));
       
//   } else if ((status&0x0004)!=0){ // exiting
       MedianHitG(index,x1,y1,z1,xg,yg,zg,xMg,yMg,zMg);
//       flag = 0;
       
       IlcDebug(1, Form("x1 = %f, y1 = %f, z1 = %f", x1, y1, z1));
       
       IlcDebug(1, Form(" Track status = %d, flag = %d", status, flag));
       
//   } // end if
//   else  flag = 1;
    
   
}
//___________________________________________________________________________
void IlcTARGETmodule::GetID(Int_t &lay,Int_t &lad,Int_t &det){


   
  // get ID
	fTARGET->GetTARGETgeom2()->GetModuleId(fIndex,lay,lad,det);
	return ;
}


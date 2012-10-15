#ifndef ILCTARGETMODULE_H
#define ILCTARGETMODULE_H
/* Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcTARGETmodule.h,v 1.1.1.1 2008/03/11 14:52:51 vitomeg Exp $ */
///////////////////////////////////////////////////////////////////////
//                                                                   //
//  Class IlcTARGETmodule                                               //
//  is a superclass for IlcTARGETmoduleSSD, SPD and SDD.                //
//  The main function of modules is to simulate DIGTARGET from          //
//  GEANT HTARGET and produce POINTS from DIGTARGET                        //
//  It also make fast simulation without use of DIGTARGET               //
//                                                                   //
//  created by: A.Boucham, W.Peryt, S.Radomski, P.Skowronski         //
//              R.Barbera, B. Batynia, B. Nilsen                     //
//  ver. 1.0    CERN, 16.09.1999                                     //
//                                                                   //
///////////////////////////////////////////////////////////////////////

#include <TObject.h>
#include <TObjArray.h>
#include <TArrayI.h>  // used in inline functions GetHitTrackIndex,GetHitHitIndex, and GetHitTrackAndHitIndex.

class IlcTARGET;
class IlcTARGEThit;


class IlcTARGETmodule:public TObject{


 public:
    //________________________________________________________________
    //
    // Constructors and deconstructor
    //________________________________________________________________
    // 
    IlcTARGETmodule();             // default constructor
    IlcTARGETmodule(Int_t index);  //index in TObjectArray in TARGET object
    virtual ~IlcTARGETmodule();

    //________________________________________________________________
    //
    // Position managenent (id's getters)
    //________________________________________________________________
    //

    Int_t GetIndex()  const { 
      // get index
      return fIndex;
    }
    void  GetID(Int_t &lay,Int_t &lad,Int_t &det);
    Int_t GetLayer() {
      // get layer
      Int_t i,j,k;GetID(i,j,k);return i;
    }
    Int_t GetLadder(){
      // get ladder
      Int_t i,j,k;GetID(i,j,k);return j;
    }
    Int_t GetDet(){
      // get det
      Int_t i,j,k;GetID(i,j,k);return k;
    }
    //________________________________________________________________
    //
    // Hits management
    //________________________________________________________________
    //
    Int_t GetNhits()   const { 
      // get num of hits
      return fHitsM->GetEntriesFast();
    }
    IlcTARGET *GetTARGET() const { 
      // get TARGET
      return fTARGET;
    }
    TObjArray *GetHits() const { 
      // get hits
      return fHitsM; 
    }
    IlcTARGEThit *GetHit(Int_t i) { 
    // returns pointer to array (TClonesArray) of pointers to hits
      return (IlcTARGEThit*)fHitsM->UncheckedAt(i);
    }
    // Adds pointer of hit belonging to this module
    // and returns number of hits in this module
    Int_t  AddHit(IlcTARGEThit *hit,Int_t TrackIndex,Int_t HitIndex);
    Int_t GetHitTrackIndex(Int_t i) {// get hit track index
      return fTrackIndex->At(i);}
    Int_t GetHitHitIndex(Int_t i) { 
	// return the hit index number in TreeH for the given sorted hit in
	// the module.
      return fHitIndex->At(i);}
    void GetHitTrackAndHitIndex(Int_t i,Int_t &TrackIndex,Int_t &HitIndex) {
    // return the hit index number for the given hit. Hits are kept on
    // file according to their track index and hit index numbers.
      TrackIndex = fTrackIndex->At(i);HitIndex = fHitIndex->At(i);}
    // Computes mean global location from hits that make up a track passing
    // through a volume.
    Bool_t MedianHitG(IlcTARGEThit *h1,IlcTARGEThit *h2,
		      Float_t &x,Float_t &y,Float_t &z);
    void MedianHitG(Int_t index, Float_t hitx1,Float_t hity1,Float_t hitz1, 
                    Float_t hitx2,Float_t hity2,Float_t hitz2, Float_t &xMg, 
                    Float_t &yMg, Float_t &zMg);
    // Computes mean local location from hits that make up a track passing
    // through a volume.
    Bool_t MedianHitL(IlcTARGEThit *h1,IlcTARGEThit *h2,
		      Float_t &x,Float_t &y,Float_t &z) const;
    void MedianHitL(Int_t,IlcTARGEThit *,IlcTARGEThit *,Float_t &,Float_t &,
		    Float_t &){};
    Double_t PathLength(Int_t index, IlcTARGEThit *itsHit1, IlcTARGEThit *itsHit2);
    // returns both the track and hit index numbers for the given hit. Hits
    // are kept on file according to their track index and hit index numbers.
    void MedianHit(Int_t index, Float_t xg,Float_t yg,Float_t zg,
                   Int_t status,Float_t &xMg, Float_t &yMg, Float_t &zMg,
	       	    	           Int_t &flag);
    void PathLength(Int_t index,Float_t x,Float_t y,Float_t z,Int_t status,
                    Int_t &nseg,Float_t &x1,Float_t &y1,Float_t &z1,
		    Float_t &dx1,Float_t &dy1, Float_t &dz1,Int_t &flag) const;
    Bool_t LineSegmentL(Int_t hindex,Double_t &a,Double_t &b, 
			Double_t &c,Double_t &d,Double_t &e,Double_t &f,
			Double_t &de);
    Bool_t LineSegmentL(Int_t hindex,Double_t &a,Double_t &b,
			Double_t &c,Double_t &d,Double_t &e,Double_t &f,
			Double_t &de,Int_t &track);
    // if returns kTRUE; gives the parameterization of the line segment
    // from this step. x=a+b*t, y=c+d*t, and z=e+f*t for 0.0t<=1.0 and the
    // energy lost during this step all in the local detector coordinates.
    // if returns kFALSE; this is a begining step and no energy loss has
    // occured. This step is used to set up the next steps. Do not use
    // this parameterization.
    Bool_t LineSegmentG(Int_t hindex,Double_t &a,Double_t &b,
			Double_t &c,Double_t &d,Double_t &e,Double_t &f,
			Double_t &de);
    Bool_t LineSegmentG(Int_t hindex,Double_t &a,Double_t &b,
			Double_t &c,Double_t &d,Double_t &e,Double_t &f,
			Double_t &de,Int_t &track);
    // if returns kTRUE; gives the parameterization of the line segment
    // from this step. x=a+b*t, y=c+d*t, and z=e+f*t for 0.0t<=1.0 and the
    // energy lost during this step all in the global detector coordinates.
    // if returns kFALSE; this is a begining step and no energy loss has
    // occured. This step is used to set up the next steps. Do not use
    // this parameterization.

 protected:
    IlcTARGETmodule(const IlcTARGETmodule &source); 
    IlcTARGETmodule& operator=(const IlcTARGETmodule &source); 
    //________________________________________________________________
    //
    // Data members
    //________________________________________________________________
    //
    IlcTARGET       *fTARGET;       // Pointer to TARGET detector
    Int_t         fIndex;      //Index of this module in TARGETmodules TObjectArray
    TObjArray    *fHitsM;     // Pointer to list of hits on this module
    TArrayI      *fTrackIndex; // track index
    TArrayI      *fHitIndex; // hit index

    ClassDef(IlcTARGETmodule,0) // Copy the hits into a more useful order
	// Version set to zero so that we do not write out this class.
};

#endif




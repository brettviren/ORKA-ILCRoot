#ifndef ILCTARGETGEOMSDD_H
#define ILCTARGETGEOMSDD_H
/* Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcTARGETgeomSDD.h,v 1.1.1.1 2008/03/11 14:52:51 vitomeg Exp $ */


////////////////////////////////////////////////////////////////////////
// This class is for the Silicon Drift Detector, SDD, specific geometry.
// It is being replaced by IlcTARGETsegmentationSDD class. This file also
// constains classes derived from IlcTARGETgeomSDD which do nothing but
// initilize this one with predefined values.
////////////////////////////////////////////////////////////////////////

#include <TObject.h>
#include <TBRIK.h>

class TShape;

class IlcTARGETgeomSDD: public TObject {
 public:
    IlcTARGETgeomSDD();
    IlcTARGETgeomSDD(const Float_t *box,Float_t per,Float_t vel,
		  Float_t axL,Float_t axR,
		  Int_t nA0,Float_t *le0,Int_t nA1,Float_t *le1);
    IlcTARGETgeomSDD(IlcTARGETgeomSDD &source);
    IlcTARGETgeomSDD& operator=(IlcTARGETgeomSDD &source);
    virtual ~IlcTARGETgeomSDD();
    void ResetSDD(const Float_t *box,Float_t per,Float_t vel,
		  Float_t axL,Float_t axR,
		  Int_t nA0,Float_t *le0,Int_t nA1,Float_t *le1);
    virtual TShape *GetShape() const {return fShapeSDD;}
    virtual Float_t GetDx() const { // Get TBRIK Dx
	if(fShapeSDD!=0) return fShapeSDD->GetDx();
	else return 0.0;}
    virtual Float_t GetDy() const {// Get TBRIK Dy
	if(fShapeSDD!=0) return fShapeSDD->GetDy();
	else return 0.0;}
    virtual Float_t GetDz() const {// Get TBRIK Dz
	if(fShapeSDD!=0) return fShapeSDD->GetDz();
	else return 0.0;}
    virtual Float_t GetAnodeX(Int_t a,Int_t s) const {
	// returns X position of anode
	a = 0; if(s==0) return fAnodeXL; else return fAnodeXR;}
    virtual Float_t GetAnodeZ(Int_t a,Int_t s)const {
	// returns X position of anode
	if(s==0) return 0.5*(fAnodeLowEdgeL[a]+fAnodeLowEdgeL[a+1]);
	else return 0.5*(fAnodeLowEdgeR[a]+fAnodeLowEdgeR[a+1]);}
    virtual void SetNAnodesL(Int_t s)
	{fNAnodesL = s;} // sets the number of anodes on side 0.
    virtual void SetNAnodesR(Int_t s)
	{fNAnodesR = s;} // sets the anodes spacing for side 1.
    virtual void SetSamplingPeriod(Float_t s)
	{fPeriod = s;} // sets the clock sampling period s.
    virtual void SetDriftVelocity(Float_t s)
	{fDvelocity = s;} // sets the SDD Drift velocity cm/s.
    virtual void SetShape(char *name,char *title,char *mat,
			  Float_t dx,Float_t dy,Float_t dz)
	                 {fShapeSDD = new TBRIK(name,title,mat,dx,dy,dz);}
    virtual void Local2Det(Float_t xl,Float_t zl,Int_t &a,Int_t &t,Int_t &s);
    virtual void Det2Local(Int_t a,Int_t t,Int_t s,Float_t &xl,Float_t &zl);
    virtual void Print(ostream *os) const; // Output streamer to standard out.
    virtual void Read(istream *is);   // Input streamer to standard in.
    virtual void Print(Option_t *option="") const {TObject::Print(option);}
    virtual Int_t Read(const char *name) {return TObject::Read(name);}
    // or what other or different information that is needed.

 protected:
    // (L)  -+-> x  (R)
    //       |
    //       V z
    Float_t fPeriod; // ADC sampiling period
    Float_t fDvelocity; // Drift velocity
    Int_t   fNAnodesL;  // number of Anodes on size 0
    Int_t   fNAnodesR;  // number of Anodes on size 1
    Float_t fAnodeXL;   // Anode location in x Left side
    Float_t fAnodeXR;   // Anode location in x Right side
    Float_t *fAnodeLowEdgeL; //[fNAnodesL] Anode spacing left edge
    Float_t *fAnodeLowEdgeR; //[fNAnodesR] Anode spacing right edge
    TBRIK *fShapeSDD;     // shape of sensitive volume

    ClassDef(IlcTARGETgeomSDD,1) // TARGET SDD detector geometry class

};
// Input and output function for standard C++ input/output.
ostream &operator<<(ostream &os,IlcTARGETgeomSDD &source);
istream &operator>>(istream &os,IlcTARGETgeomSDD &source);
#endif
//======================================================================
#ifndef ILCTARGETGEOMSDD256_H
#define ILCTARGETGEOMSDD256_H
/* Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 * See cxx source for full Copyright notice                               */

/*
  $Id: IlcTARGETgeomSDD.h,v 1.1.1.1 2008/03/11 14:52:51 vitomeg Exp $
*/

//#include "IlcTARGETgeomSDD.h"


class IlcTARGETgeomSDD256 : public IlcTARGETgeomSDD {

 public:
    IlcTARGETgeomSDD256();
    IlcTARGETgeomSDD256(Int_t npar,const Float_t *par);

    // This clas now has version 0 so that it will not be written to a root
    // file. This is good since there are no longer any data members to this
    // class. It is only designed to make it easer to define this standard
    // SDD detector geometry.
    ClassDef(IlcTARGETgeomSDD256,1) // TARGET SDD detector geometry class for 256 anodes per side

};
// Input and output function for standard C++ input/output.
ostream &operator<<(ostream &os,IlcTARGETgeomSDD256 &source);
istream &operator>>(istream &os,IlcTARGETgeomSDD256 &source);
#endif
//======================================================================
#ifndef ILCTARGETGEOMSDD300_H
#define ILCTARGETGEOMSDD300_H
/* Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 * See cxx source for full Copyright notice                               */

/*
  $Id: IlcTARGETgeomSDD.h,v 1.1.1.1 2008/03/11 14:52:51 vitomeg Exp $
*/

//#include "IlcTARGETgeomSDD.h"


class IlcTARGETgeomSDD300 : public IlcTARGETgeomSDD {

 public:
    IlcTARGETgeomSDD300();

    // This clas now has version 0 so that it will not be written to a root
    // file. This is good since there are no longer any data members to this
    // class. It is only designed to make it easer to define this standard
    // SDD detector geometry.
    ClassDef(IlcTARGETgeomSDD300,1) // TARGET SDD detector geometry class for 300 anodes per side

};
// Input and output function for standard C++ input/output.
ostream &operator<<(ostream &os,IlcTARGETgeomSDD300 &source);
istream &operator>>(istream &os,IlcTARGETgeomSDD300 &source);
#endif
